"""Dependency view of the non-STRICT functions: which are roots and which only wait for a neighbour. Diagnostic.

    python tools/blockers.py [--tu src/DIR/FILE.c] [-v]

A function is DEPENDENT when its own instructions are right and only one of these differs:
- the distance of a jump/branch to code outside the function (Watcom shares epilogues and tails between functions:
  the jump lands in another function, so every wrongly sized function between the two shifts the distance);
- its extent: zero differing bytes but the original extent is longer, because the NEXT function's switch table sits
  in front of that function (plain profile) and the candidate's next function has no such table.
Everything else is a ROOT. For each root the tool lists the dependents that sit behind it, so the roots with the
most dependents come first: fixing one of those releases several functions at once.
"""
import bisect
import collections
import json
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
MD.detail = False
BR = re.compile(r"^(j\w+|call|loop\w*)$")


def insns(code, base):
    return [(i.address, i.size, i.mnemonic, i.op_str, bytes(i.bytes)) for i in MD.disasm(code, base)]


def classify(orig, cand, va, fixups):
    """orig/cand: bytes of the same length (the candidate extent). Returns (kind, [original outside targets])."""
    if len(orig) != len(cand):
        return "root", []
    a, b = insns(orig, va), insns(cand, va)
    if len(a) != len(b) or any(x[0] != y[0] or x[1] != y[1] for x, y in zip(a, b)):
        return "root", []
    targets = []
    for x, y in zip(a, b):
        if x[4] == y[4]:
            continue
        lo, hi = x[0], x[0] + x[1]
        if any(lo <= f < hi for f in fixups):
            continue                    # a relocated field: compared through its binding, not its bytes
        if x[2] == y[2] and BR.match(x[2]) and x[3].startswith("0x") and y[3].startswith("0x"):
            t = int(x[3], 16)
            if not (va <= t < va + len(orig)):
                targets.append(t)
                continue
        return "root", []
    return ("dep-jump" if targets else "reloc"), targets


def main(argv):
    only = argv[argv.index("--tu") + 1].replace("\\", "/") if "--tu" in argv else None
    verbose = "-v" in argv
    ledger = gate.load_ledger()
    roots = collections.defaultdict(list)       # root (tu, va, label) -> [dependent labels]
    info = {}
    counts = collections.Counter()
    for tu, ent in sorted(ledger["tus"].items()):
        if (only and tu != only) or str(ent.get("profile", "")).startswith("asm"):
            continue
        inst, tool, flags = gate.flags_for(ent)
        with tempfile.TemporaryDirectory(prefix="b", dir=ROOT / "build") as td:
            obj = Path(td) / "unit.obj"
            rc, text = cc.compile_one(ROOT / tu, obj, flags, inst, tool, as_name=ent.get("as"))
            if rc:
                print(f"compile failed: {tu}")
                continue
            reports, img = compare.compare_object(obj, None, None, False, False)
            mods = omf.load(obj)
        segdata = {s.name: s for m in mods for s in m.segments if s}
        fix = collections.defaultdict(set)
        for m in mods:
            for f in m.fixups:
                if f.seg < len(m.segments) and m.segments[f.seg]:
                    fix[m.segments[f.seg].name].add(f.off)
        for r in reports:
            for s in r["segments"]:
                if s["class"].upper() != "CODE" or s["seg"] != "_TEXT":
                    continue
                ext = sorted((e for e in s.get("extents", []) if e.get("va")), key=lambda e: e["off"])
                vas = [int(e["va"], 16) for e in ext]
                bad = [e for e in ext if e["status"] != "STRICT"]
                state = {}
                for e in bad:
                    va, size = int(e["va"], 16), e["end"] - e["off"]
                    seg = segdata.get(s["seg"])
                    cand = bytes(seg.data[e["off"]:e["end"]]) if seg is not None else b""
                    orig = img.read_va(va, size)
                    fx = {va + (o - e["off"]) + k for o in fix[s["seg"]] if e["off"] <= o < e["end"] for k in range(4)}
                    if e.get("diff_bytes", 1) == 0 and e.get("size_bad"):
                        kind, tg = "dep-table", []
                    elif e["status"] == "UNBOUND":
                        kind, tg = "reloc", []
                    else:
                        kind, tg = classify(orig, cand, va, fx)
                    state[va] = (kind, tg, e)
                for va, (kind, tg, e) in state.items():
                    lab = symbols.label(e["names"][0])
                    counts[kind] += 1
                    info[(tu, va)] = (kind, lab, e.get("diff_bytes"))
                    if kind == "dep-table":
                        i = bisect.bisect_right(vas, va)
                        if i < len(vas):
                            nxt = ext[i]
                            roots[(tu, vas[i], symbols.label(nxt["names"][0]), nxt["status"])].append(f"{lab} (table)")
                    elif kind == "dep-jump":
                        for t in sorted(set(tg)):
                            lo, hi = sorted((va, t))
                            between = [x for x in ext if lo <= int(x["va"], 16) <= hi and x["status"] != "STRICT"
                                       and int(x["va"], 16) != va]
                            i = bisect.bisect_right(vas, t) - 1
                            if 0 <= i < len(ext) and ext[i]["status"] != "STRICT" and int(ext[i]["va"], 16) != va \
                                    and ext[i] not in between:
                                between.append(ext[i])
                            for x in between:
                                xv = int(x["va"], 16)
                                if state.get(xv, ("root",))[0] == "root":
                                    roots[(tu, xv, symbols.label(x["names"][0]), x["status"])].append(lab)
                            if not between:
                                roots[(tu, va, lab, "self")].append(f"{lab} (jump to {t:08x}: no wrong-sized function between; tail differs)")
    print(f"non-STRICT main-code functions: {sum(counts.values())}  roots {counts['root']}, waiting for a neighbour: "
          f"{counts['dep-jump']} by jump distance, {counts['dep-table']} by the next function's switch table; "
          f"relocation/binding only: {counts['reloc']}")
    print("\nroots by number of dependents:")
    for (tu, va, lab, st), deps in sorted(roots.items(), key=lambda kv: (-len(set(kv[1])), kv[0])):
        d = sorted(set(deps))
        k = info.get((tu, va), ("?", lab, None))
        print(f"  {len(d):2d}  {va:08x} {lab[:44]:44s} {tu}  ({k[2]} B differ)")
        if verbose:
            for x in d:
                print(f"        <- {x}")
    if verbose:
        print("\nrelocation/binding only:")
        for (tu, va), (kind, lab, db) in sorted(info.items()):
            if kind == "reloc":
                print(f"  {va:08x} {lab[:44]:44s} {tu}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
