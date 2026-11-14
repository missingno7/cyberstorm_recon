"""Index of the non-STRICT functions with a residue class and their attempt history. Diagnostic, for workers.

    python tools/parked.py [--tu src/DIR/FILE.c] [--fn LABEL_OR_VA] [--out build/parked_index.tsv]

Per function: file, VA, size, differing bytes, class, number of earlier reports that mention it.
Classes (candidate vs original, same placement):
  WAIT-SIZE   own instructions equal; only a jump into another function differs (fix the neighbour, not this)
  WAIT-TABLE  bytes equal; the next function's switch table follows (fix the next function)
  REG-SWAP    same instructions, only register names differ          (two temporaries exchanged)
  SLOT-SWAP   same instructions, only esp/ebp displacements differ   (stack slot order)
  REG+SLOT    both of the above
  OPERANDS    same instruction lengths and mnemonics, other operand differences (constant, field offset, callee)
  SHAPE       instruction stream differs in length or mnemonics      (control flow, expression shape, inlining)
REG-SWAP / SLOT-SWAP functions that many lanes already tried are usually invariant under respelling: read the notes
(--fn prints the report lines that mention the function) before spending time. SHAPE functions are where reading
the original listing pays: something in the source says a different thing.
"""
import bisect
import collections
import glob
import re
import sys
import tempfile
from pathlib import Path

import capstone

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cc  # noqa: E402
import compare  # noqa: E402
import gate  # noqa: E402
import omf  # noqa: E402
import symbols  # noqa: E402
from common import ROOT  # noqa: E402

MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
REG = re.compile(r"\b(e?[abcd]x|[abcd][lh]|e?[sd]i|e?bp)\b")
SLOT = re.compile(r"e[sb]p [+-] 0x[0-9a-f]+|e[sb]p\]")
BR = re.compile(r"^(j\w+|call|loop\w*)$")


def classify(orig, cand, va, fix):
    a = [(i.address, i.size, i.mnemonic, i.op_str, bytes(i.bytes)) for i in MD.disasm(orig, va)]
    b = [(i.address, i.size, i.mnemonic, i.op_str, bytes(i.bytes)) for i in MD.disasm(cand, va)]
    if len(orig) != len(cand) or len(a) != len(b) or any(x[1] != y[1] or x[2] != y[2] for x, y in zip(a, b)):
        return "SHAPE"
    kinds = set()
    for x, y in zip(a, b):
        if x[4] == y[4] or any(x[0] <= f < x[0] + x[1] for f in fix):
            continue
        if BR.match(x[2]) and x[3].startswith("0x"):
            t = int(x[3], 16)
            kinds.add("jump-out" if not (va <= t < va + len(orig)) else "ops")
        elif REG.sub("r", x[3]) == REG.sub("r", y[3]):
            kinds.add("reg")
        elif SLOT.sub("S", x[3]) == SLOT.sub("S", y[3]):
            kinds.add("slot")
        elif SLOT.sub("S", REG.sub("r", x[3])) == SLOT.sub("S", REG.sub("r", y[3])):
            kinds |= {"reg", "slot"}
        else:
            kinds.add("ops")
    if not kinds:
        return "RELOC"
    if kinds == {"jump-out"}:
        return "WAIT-SIZE"
    kinds.discard("jump-out")
    if "ops" in kinds:
        return "OPERANDS"
    return {"reg": "REG-SWAP", "slot": "SLOT-SWAP"}.get("+".join(sorted(kinds)), "REG+SLOT")


def history():
    """label / VA -> [(report, line)] over every lane report."""
    text = {}
    for p in sorted(glob.glob(str(ROOT / "candidates" / "*" / "REPORT.md"))):
        try:
            text[Path(p).parent.name] = Path(p).read_text(encoding="utf-8", errors="replace").split("\n")
        except OSError:
            pass
    return text


def main(argv):
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(errors="replace")      # report lines may hold characters the console cannot show
    only = argv[argv.index("--tu") + 1].replace("\\", "/") if "--tu" in argv else None
    want = argv[argv.index("--fn") + 1] if "--fn" in argv else None
    out = argv[argv.index("--out") + 1] if "--out" in argv else None
    ledger = gate.load_ledger()
    reports = history()
    rows = []
    for tu, ent in sorted(ledger["tus"].items()):
        if (only and tu != only) or str(ent.get("profile", "")).startswith("asm"):
            continue
        inst, tool, flags = gate.flags_for(ent)
        with tempfile.TemporaryDirectory(prefix="k", dir=ROOT / "build") as td:
            obj = Path(td) / "unit.obj"
            rc, _ = cc.compile_one(ROOT / tu, obj, flags, inst, tool, as_name=ent.get("as"))
            if rc:
                continue
            reps, img = compare.compare_object(obj, None, None, False, False)
            mods = omf.load(obj)
        segs = {s.name: s for m in mods for s in m.segments if s}
        fixups = collections.defaultdict(set)
        for m in mods:
            for f in m.fixups:
                if f.seg < len(m.segments) and m.segments[f.seg]:
                    fixups[m.segments[f.seg].name].add(f.off)
        for r in reps:
            for s in r["segments"]:
                if s["class"].upper() != "CODE" or s["seg"] != "_TEXT":
                    continue
                for e in s.get("extents", []):
                    if e["status"] == "STRICT" or not e.get("va"):
                        continue
                    va, size = int(e["va"], 16), e["end"] - e["off"]
                    lab = symbols.label(e["names"][0])
                    if want and want not in (lab, e["va"]):
                        continue
                    if e.get("diff_bytes") == 0 and e.get("size_bad"):
                        cls = "WAIT-TABLE"
                    else:
                        fx = {va + (o - e["off"]) + k for o in fixups[s["seg"]] if e["off"] <= o < e["end"]
                              for k in range(4)}
                        cls = classify(img.read_va(va, size), bytes(segs[s["seg"]].data[e["off"]:e["end"]]), va, fx)
                    hits = [(lane, ln.strip()) for lane, lines in reports.items() for ln in lines
                            if (lab in ln and len(lab) > 5) or e["va"] in ln.lower()]
                    rows.append((tu, e["va"], size, e.get("diff_bytes"), cls, len({h[0] for h in hits}), lab, hits))
    counts = collections.Counter(r[4] for r in rows)
    for tu, va, size, diff, cls, n, lab, hits in rows:
        print(f"{va} {size:5d} B  {str(diff):>5s} differ  {cls:10s} {n:2d} reports  {lab[:42]:42s} {tu}")
        if want:
            for lane, ln in hits[:40]:
                print(f"    [{lane}] {ln[:230]}")
    print("\n" + ", ".join(f"{k} {v}" for k, v in counts.most_common()) + f"; total {len(rows)}")
    if out:
        Path(out).write_text("\n".join("\t".join(str(x) for x in r[:7]) for r in rows) + "\n", encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
