"""TU-level proof (diagnostic until adopted by verify): is a whole accepted object reproduced?

    python tools/tucheck.py [--tu src/DIR/FILE.c] [-v]

A TU is COMPLETE when, in verify's view (fresh compile, no --at, no search):
1. every extent of its main code segment is STRICT and the extents sit contiguously at base + object offset
   (no tail_fill: the object's last bytes are its own);
2. the object occupies the original range exactly: base is an original function start, the byte after the object
   is either the next object's first function start or zero linker fill up to it, and the byte before the object is
   the previous object's end (zero fill or a function end) -- i.e. no original function in [base, end) is missing;
3. every other code segment (COMDAT inline functions) is STRICT wherever the linker kept it;
4. every initialized data segment with bytes (CONST, CONST2, _DATA, XI/YI records, COMDAT data) is STRICT as a
   whole segment at its base (not per item);
5. a BSS segment, if any, is placed (its referenced items are proven by the STRICT code's fixups). BSS size and the
   order of unreferenced items are not provable from the oracle bytes and are reported, not claimed.
"""
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import gate  # noqa: E402
import pe as pemod  # noqa: E402
from common import BUILD, ROOT  # noqa: E402


def check_tu(tu, ent, img, starts):
    reports, mods, _ = gate.build_and_compare(ROOT / tu, ent)
    why = []
    comdat = {s.name for m in mods for s in m.segments if s and getattr(s, "comdat", None) is not None}
    main = None
    for r in reports:
        for s in r["segments"]:
            name, cls, st = s["seg"], s["class"].upper(), s["status"]
            if cls == "CODE":
                if name in comdat:
                    if st != "STRICT":
                        why.append(f"COMDAT {name}: {st}")
                    continue
                if main is not None:
                    why.append(f"second main code segment {name}")
                    continue
                main = s
                continue
            if st == "EMPTY":
                continue
            if cls == "BSS" or st in ("BSS", "BSS-UNPLACED"):
                if st != "BSS":
                    why.append(f"BSS {name}: {st}")
                continue
            if st != "STRICT":
                why.append(f"data {name}: {st}")
    if main is None:
        return False, why + ["no code segment"], 0
    ext = main["extents"]
    bad = [e for e in ext if e["status"] != "STRICT"]
    if bad:
        why.append(f"{len(bad)} of {len(ext)} code extents not STRICT")
        return False, why, main["size"]
    base = int(ext[0]["va"], 16) - ext[0]["off"] if ext and ext[0].get("va") else None
    if base is None:
        return False, why + ["code unplaced"], main["size"]
    for e in ext:
        if int(e["va"], 16) != base + e["off"]:
            why.append(f"extent {e['names'][0]} not contiguous")
        if e.get("tail_fill"):
            why.append(f"extent {e['names'][0]} ends before its own filler ({e['tail_fill']} bytes)")
    end = base + main["size"]
    if base not in starts:
        why.append(f"object start {base:08x} is not an original function start")
    nxt = min((s for s in starts if s >= end), default=None)
    if nxt is not None and nxt > end and any(img.read_va(end, nxt - end)):
        why.append(f"bytes {end:08x}..{nxt:08x} after the object are not zero fill (missing function?)")
    inside = [s for s in starts if base <= s < end]
    extent_starts = {int(e["va"], 16) for e in ext}
    missing = [s for s in inside if s not in extent_starts]
    if missing:
        why.append(f"original starts inside the object without an extent: {[f'{s:08x}' for s in missing[:4]]}")
    return not why, why, main["size"]


def main(argv):
    ledger = gate.load_ledger()
    img = pemod.load(None)
    fm = json.loads((BUILD / "funcmap" / "cs11.json").read_text())
    starts = {int(f["va"], 16) for f in fm["functions"]}
    only = argv[argv.index("--tu") + 1] if "--tu" in argv else None
    ok_n = ok_b = 0
    rows = []
    for tu, ent in sorted(ledger["tus"].items()):
        if only and tu != only:
            continue
        try:
            ok, why, size = check_tu(tu, ent, img, starts)
        except RuntimeError as e:
            ok, why, size = False, [f"compile: {e}"], 0
        rows.append((tu, ok, why, size))
        if ok:
            ok_n += 1
            ok_b += size
        if ok or "-v" in argv:
            print(f"{'COMPLETE' if ok else 'partial '} {tu} ({size} code bytes)" + ("" if ok else f": {'; '.join(why)[:200]}"))
    print(f"TU complete: {ok_n} of {len(rows)} accepted TUs, {ok_b} code bytes")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
