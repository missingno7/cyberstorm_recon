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
  SHAPE       the function has a different SIZE: control flow, expression shape, inlining or a field is wrong
  SAME-SIZE   original size, but some arithmetic/compare/branch/call instruction differs (one construct is off);
              `--fn` and the index's last column name those instructions on both sides
  HOMES       original size and the same core instructions with the same normalised operands (immediates,
              conditions and non-stack lea-adds kept; registers, stack slots and addresses abstracted); only moves,
              register homes or instruction order differ (same family as REG-SWAP: needs a different
              expression/record set, see docs/watcom_playbook.md). A hypothesis, not a proven value mapping.
  RELOC       every instruction equal once the candidate's unlinked fixup fields are masked: a binding/identity
              problem (wrong extern, ctor/dtor role), not a source-shape problem
  UNPLACED    an extent the comparator could not place; the last column names the placed publics around it
  ASM         the original starts with `enter` or pushes segment registers: hand assembly, no C source matches it
Column `work`: yes = give it to a lane; no:<why> = withhold (asm origin, waits for a neighbour, or a register/slot
swap that three or more lanes already reported on: invariant under respelling until there is new evidence).
`--targets FILE` writes only the `yes` rows, largest first, as a lane target list.
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
# capstone prints small displacements in decimal (`esp + 4`), larger ones in hex
SLOT = re.compile(r"e[sb]p [+-] (?:0x[0-9a-f]+|\d+)|e[sb]p\]")
BR = re.compile(r"^(j\w+|call|loop\w*)$")
# an address operand: a linked image address (>= 0x400000) in the original; in the candidate, classify() writes
# the SENTINEL address into every fixup field first (unlinked fields hold segment offsets, often 0, which capstone
# prints as `[ebx]` or `0`), so both sides print an image-range value and abstract to A
SENTINEL = (0x00FA0000).to_bytes(4, "little")
HEX = re.compile(r"\b0x[0-9a-f]+\b")


def absn(s):
    return HEX.sub(lambda m: "A" if int(m.group(), 16) >= 0x400000 else m.group(), s)


# `lea r, [base + imm]` with a non-stack base is `add` in another register home; `lea r, [esp + n]` is the
# address of a local and stays a move-class instruction (review rv1: DrawDeathMap counted 11 vs 10 stack LEAs)
LEA_ADD = re.compile(r"^\w+, (?:dword ptr )?\[(?:e?[abcd]x|e[sd]i) ([+-]) (0x[0-9a-f]+|\d+)\]$")


LAST_HINT = []
SKIP = ("mov", "push", "pop", "lea", "nop")


def core_hint(a, b):
    """The core instructions (no moves) that only one side has, at their offsets: what the source must change."""
    import difflib
    ca = [(x[0] - a[0][0], f"{x[2]} {x[3]}") for x in a if x[2] not in SKIP]
    cb = [(y[0] - b[0][0], f"{y[2]} {y[3]}") for y in b if y[2] not in SKIP]
    # relocated operands are not linked in the candidate object ([0], 0): compare them as one symbol A; branch and
    # call targets are distances, not constructs
    # (the branch mnemonic is kept: je vs jne to the same target is a real difference)
    norm = lambda s: REG.sub("r", absn(re.sub(r"^(j\w+|call) .*$", r"\1 T", s)))
    out = []
    sm = difflib.SequenceMatcher(None, [norm(s) for _, s in ca], [norm(s) for _, s in cb], autojunk=False)
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == "equal":
            continue
        left = "; ".join(f"+{o:x} {s}" for o, s in ca[i1:i2][:3])
        right = "; ".join(f"+{o:x} {s}" for o, s in cb[j1:j2][:3])
        out.append(f"original [{left or '-'}] vs candidate [{right or '-'}]")
        if len(out) == 3:
            break
    return " | ".join(out)


def classify(orig, cand, va, fix, size_bad=False):
    if size_bad:
        return "SHAPE"              # the extent does not end where the original function ends
    c = bytearray(cand)
    for f in sorted(fix):
        p = f - va
        if 0 <= p and f - 1 not in fix:                 # first byte of a fixup field
            n = min(4, len(c) - p)
            c[p:p + n] = SENTINEL[:n]
    cand = bytes(c)
    a = [(i.address, i.size, i.mnemonic, i.op_str, bytes(i.bytes)) for i in MD.disasm(orig, va)]
    b = [(i.address, i.size, i.mnemonic, i.op_str, bytes(i.bytes)) for i in MD.disasm(cand, va)]
    if len(orig) != len(cand) or len(a) != len(b) or any(x[1] != y[1] or x[2] != y[2] for x, y in zip(a, b)):
        if len(orig) != len(cand):
            return "SHAPE"              # different size: the source still says something else
        # `lea r,[s+imm]` and `add r,imm` are the same operation in another register home (TerrainBlendHeight):
        # count them as one; scaled-index and stack-address lea stay address computations, skipped like mov
        def op(r):
            m = LEA_ADD.match(r[3]) if r[2] == "lea" else None
            if m:
                return "add", f"r, {'-' if m.group(1) == '-' else ''}{absn(m.group(2))}"
            if BR.match(r[2]):
                return r[2], "T"                  # the condition counts, the distance does not
            return r[2], SLOT.sub("S", REG.sub("r", absn(r[3])))
        # HOMES needs the same core instructions WITH their normalised operands (registers, stack slots and
        # addresses abstracted, immediates and conditions kept), not only the same mnemonic counts (review rv1)
        core = lambda rows: collections.Counter(op(r) for r in rows if op(r)[0] not in SKIP)
        if core(a) == core(b):
            return "HOMES"
        # equal once register and memory operands are one kind: a value read from memory on one side is already
        # in a register on the other (`sub eax,[ebx]` vs `sub edx,ecx`). Still a home question, but tentative:
        # the hint names the operands so a lane can see which value is homed differently.
        mem = re.compile(r"(?:\w+ ptr )?\[[^\]]*\]")
        core2 = lambda rows: collections.Counter((o, re.sub(r"\b[rS]\b", "X", mem.sub("X", s))) for o, s in
                                                 (op(r) for r in rows) if o not in SKIP)
        if core2(a) == core2(b):
            LAST_HINT.clear()
            LAST_HINT.append("operand homes (tentative): " + core_hint(a, b))
            return "HOMES"
        LAST_HINT.clear()
        LAST_HINT.append(core_hint(a, b))
        return "SAME-SIZE"
    kinds = set()
    for x, y in zip(a, b):
        if x[4] == y[4]:
            continue
        # only the fixup field is unlinked in the candidate: mask those bytes, still compare the rest of the
        # instruction (review rv1: a changed register in `mov r32,[absolute]` was reported as RELOC)
        fx = [k for k in range(x[1]) if x[0] + k in fix]
        xo, yo = x[3], y[3]
        if fx:
            if bytes(0 if k in fx else c for k, c in enumerate(x[4])) == \
                    bytes(0 if k in fx else c for k, c in enumerate(y[4])):
                continue
            xo, yo = absn(xo), absn(yo)
        if BR.match(x[2]) and x[3].startswith("0x") and not fx:
            t = int(x[3], 16)
            kinds.add("jump-out" if not (va <= t < va + len(orig)) else "ops")
        elif REG.sub("r", xo) == REG.sub("r", yo):
            kinds.add("reg")
        elif SLOT.sub("S", xo) == SLOT.sub("S", yo):
            kinds.add("slot")
        elif SLOT.sub("S", REG.sub("r", xo)) == SLOT.sub("S", REG.sub("r", yo)):
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
    for p in sorted(glob.glob(str(ROOT / "docs" / "lane_reports" / "*.md"))):       # archived (in git)
        text[Path(p).stem] = Path(p).read_text(encoding="utf-8", errors="replace").split("\n")
    for p in sorted(glob.glob(str(ROOT / "candidates" / "*" / "REPORT.md"))):      # live copies win
        try:
            text[Path(p).parent.name] = Path(p).read_text(encoding="utf-8", errors="replace").split("\n")
        except OSError:
            pass
    return text


def verdicts():
    """VA -> set of verdicts recorded in the attempt ledger (tools/notes.py)."""
    import notes
    out = collections.defaultdict(set)
    for r in notes.load():
        # only a function-level record decides: a SUMMARY, or a record of a lane that tested the function itself;
        # one invariant hypothesis family among several does not withhold the function
        if r.get("hyp", "").startswith("SUMMARY") or not r.get("lane", "").startswith("bf"):
            out[r["va"]].add(r.get("verdict", "open"))
    return out


def main(argv):
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(errors="replace")      # report lines may hold characters the console cannot show
    only = argv[argv.index("--tu") + 1].replace("\\", "/") if "--tu" in argv else None
    want = argv[argv.index("--fn") + 1] if "--fn" in argv else None
    out = argv[argv.index("--out") + 1] if "--out" in argv else None
    ledger = gate.load_ledger()
    reports = history()
    ledger_verdicts = verdicts()
    accepted = {f["va"]: tu for tu, ent in ledger["tus"].items() for f in ent.get("functions", {}).values()}
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
                exts = s.get("extents", [])
                for i, e in enumerate(exts):
                    if e["status"] == "STRICT":
                        continue
                    lab = symbols.label(e["names"][0]) if e.get("names") else "?"
                    if not e.get("va"):
                        # an extent the comparator could not place: listed, with the placed publics around it,
                        # instead of silently dropped (review rv2: two lactor wrappers were exact once declared)
                        if want and want != lab:
                            continue
                        prv = next((x for x in reversed(exts[:i]) if x.get("va")), None)
                        nxt = next((x for x in exts[i + 1:] if x.get("va")), None)
                        side = lambda x: f"{symbols.label(x['names'][0])}@{x['va']}" if x else "-"
                        rows.append((tu, "unplaced", e["end"] - e["off"], None, "UNPLACED", 0, lab, [], "yes",
                                     f"between {side(prv)} and {side(nxt)}"))
                        continue
                    if e["va"] in accepted and accepted[e["va"]] != tu:
                        continue                # proven in another TU (moved function): not open work
                    va, size = int(e["va"], 16), e["end"] - e["off"]
                    if want and want not in (lab, e["va"]):
                        continue
                    head = img.read_va(va, 4)
                    if head[0] == 0xC8 or head[:1] == b"\x06" or head[:2] in (b"\x0f\xa0", b"\x0f\xa8"):
                        cls = "ASM"
                    elif e.get("diff_bytes") == 0 and e.get("size_bad"):
                        cls = "WAIT-TABLE"
                    else:
                        fx = {va + (o - e["off"]) + k for o in fixups[s["seg"]] if e["off"] <= o < e["end"]
                              for k in range(4)}
                        cls = classify(img.read_va(va, size), bytes(segs[s["seg"]].data[e["off"]:e["end"]]), va, fx,
                                       bool(e.get("size_bad")))
                    hint = LAST_HINT[0] if cls in ("SAME-SIZE", "HOMES") and LAST_HINT else ""
                    LAST_HINT.clear()
                    work = ("no:asm" if cls == "ASM" else "no:neighbour" if cls.startswith("WAIT") else
                            "no:invariant" if "invariant" in ledger_verdicts.get(e["va"], ()) else
                            "yes")      # report counts no longer withhold work: only a ledger verdict does (review rv1)
                    rows.append((tu, e["va"], size, e.get("diff_bytes"), cls, 0, lab, [], work, hint))
    # report mentions: by VA always, by label only when the label names one function (a generic label such as a
    # static initializer's matched 71 reports for one VA, review rv1). Mentions are not independent attempts.
    labels = collections.Counter(symbols.label(n) for ent in ledger["tus"].values() for n in ent.get("functions", {}))
    labels.update(r[6] for r in rows)
    for k, r in enumerate(rows):
        lab, va = r[6], r[1]
        by_label = len(lab) > 5 and labels[lab] == 1 and not lab.startswith(("W?", "."))
        hits = [(lane, ln.strip()) for lane, lines in reports.items() for ln in lines
                if (by_label and lab in ln) or (va != "unplaced" and va in ln.lower())]
        rows[k] = r[:5] + (len({h[0] for h in hits}), lab, hits) + r[8:]
    counts = collections.Counter(r[4] for r in rows)
    for tu, va, size, diff, cls, n, lab, hits, work, hint in rows:
        print(f"{va} {size:5d} B  {str(diff):>5s} differ  {cls:10s} {n:2d} reports  {work:12s} {lab[:42]:42s} {tu}"
              + (f"  ({hint})" if cls == "UNPLACED" else ""))
        if want and hint:
            print(f"    core difference: {hint}")
        if want:
            for lane, ln in hits[:40]:
                print(f"    [{lane}] {ln[:230]}")
    print("\n" + ", ".join(f"{k} {v}" for k, v in counts.most_common()) + f"; total {len(rows)}")
    print("work: " + ", ".join(f"{k} {v}" for k, v in collections.Counter(r[8] for r in rows).most_common()))
    # stamp: the ledger and source revision the index was computed from (an index older than the ledger may list
    # functions that are accepted since)
    import hashlib
    import subprocess
    head = subprocess.run(["git", "rev-parse", "--short", "HEAD"], cwd=ROOT, capture_output=True, text=True).stdout.strip()
    stamp = (f"# parked index: git {head or '?'}, recovery.json sha256 "
             f"{hashlib.sha256((ROOT / 'recovery.json').read_bytes()).hexdigest()[:12]}\n")
    if out:
        Path(out).write_text(stamp + "\n".join("\t".join(str(x) for x in r[:7] + (r[8], r[9])) for r in rows) + "\n",
                             encoding="utf-8")
    if "--targets" in argv:
        sel = sorted((r for r in rows if r[8] == "yes"), key=lambda r: -r[2])
        Path(argv[argv.index("--targets") + 1]).write_text(
            stamp + "\n".join("\t".join(str(x) for x in r[:7] + (r[9],)) for r in sel) + "\n", encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
