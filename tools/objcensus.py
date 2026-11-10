"""Object census: original object boundaries from link-order evidence, against the accepted TUs. Diagnostic.

    python tools/objcensus.py [-v]

Evidence (docs/codegen_rules.md, "object boundaries from static initialization"):
- every object with static initialization has one XI record (6 bytes: 00, priority, address); the target is the
  FIRST function of that object;
- every object that asserts keeps its `__FILE__` path string in _DATA; path strings and code are both in link
  order, so the strings between two anchored objects belong to the code between them.
Reports, in link order, one row per path string or XI target, with the accepted TU that owns it, and flags:
  SPLIT    the TU's code range contains another object's start (XI target inside it, or several path strings)
  NO-TU    a path string whose object is not published under that name
  NAME     an XI-anchored object published under a `_unknown` name although a path string falls to it
"""
import json
import re
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import gate  # noqa: E402
import pe as pemod  # noqa: E402
from common import BUILD  # noqa: E402

PATH_RX = re.compile(rb"(?<![A-Za-z0-9_\x5c])([A-Za-z0-9_]+(?:\x5c[A-Za-z0-9_]+)+\.c)\x00", re.I)


def xi_records(img):
    """[(record VA, priority, target VA)] of the contiguous XI table (found from its game records)."""
    base = img.image_base
    dg = next(s for s in img.sections if s.name == "DGROUP")
    tx = next(s for s in img.sections if s.name == "BEGTEXT")
    lo, hi = base + tx.rva, base + tx.rva + tx.rawsize
    data = img.read_va(base + dg.rva, dg.rawsize)

    def rec(off):
        if off < 0 or off + 6 > len(data):
            return None
        kind, prio = data[off], data[off + 1]
        tgt = struct.unpack_from("<I", data, off + 2)[0]
        return (prio, tgt) if kind == 0 and 0 < prio <= 0x40 and lo <= tgt < hi else None
    best = []
    off = 0
    while off < len(data) - 6:
        run = []
        k = off
        while rec(k):
            run.append((base + dg.rva + k, *rec(k)))
            k += 6
        if len(run) > len(best):
            best = run
        off = k + 1 if run else off + 1
    return best


def path_strings(img):
    base = img.image_base
    dg = next(s for s in img.sections if s.name == "DGROUP")
    data = img.read_va(base + dg.rva, dg.rawsize)
    return [(base + dg.rva + m.start(1), m.group(1).decode()) for m in PATH_RX.finditer(data)]


def main(argv):
    verbose = "-v" in argv
    img = pemod.load(None)
    ledger = gate.load_ledger()
    prev_tu = None
    fm = json.loads((BUILD / "funcmap" / "cs11.json").read_text())["functions"]
    game = sorted(int(f["va"], 16) for f in fm if f["kind"] not in ("lib", "asm"))
    # main code range of a TU = its largest run of accepted functions not interleaved with another TU's (inline
    # COMDAT copies are linked next to whichever object defined them first and would stretch the range)
    allf = sorted((int(f["va"], 16), f["size"], tu) for tu, ent in ledger["tus"].items()
                  if not str(ent.get("profile", "")).startswith("asm") for f in ent["functions"].values())
    runs = {}
    for va, size, tu in allf:
        cur = runs.setdefault(tu, [])
        if cur and cur[-1]["last"] == prev_tu:
            cur[-1]["hi"], cur[-1]["n"], cur[-1]["bytes"] = va + size, cur[-1]["n"] + 1, cur[-1]["bytes"] + size
        else:
            cur.append({"lo": va, "hi": va + size, "n": 1, "bytes": size, "last": tu})
        prev_tu = tu
    tus = []
    for tu, rs in runs.items():
        r = max(rs, key=lambda r: r["bytes"])
        tus.append({"tu": tu, "as": ledger["tus"][tu]["as"].replace("/", "\\").lower(), "lo": r["lo"], "hi": r["hi"],
                    "n": r["n"]})
    tus.sort(key=lambda t: t["lo"])
    by_as = {t["as"]: t for t in tus}

    def owner(va):
        return next((t for t in tus if t["lo"] <= va < t["hi"]), None)
    xi = [(rva, prio, tgt) for rva, prio, tgt in xi_records(img) if prio == 0x40 and game and game[0] <= tgt <= game[-1] + 0x4000]
    paths = path_strings(img)
    flags = []
    print(f"{len(xi)} game XI records, {len(paths)} path strings, {len(tus)} C/C++ TUs")
    print("\n== static-initializer anchors (object starts) ==")
    for rva, prio, tgt in xi:
        t = owner(tgt)
        if t is None:
            note = "no TU"
        elif t["lo"] == tgt:
            note = t["tu"]
        else:
            note = f"SPLIT inside {t['tu']} (starts {t['lo']:08x})"
            flags.append(("SPLIT", f"{t['tu']}: another object starts at {tgt:08x} (XI {rva:08x})"))
        if verbose or "SPLIT" in note or note == "no TU":
            print(f"  {tgt:08x}  {note}")
    # direct identification: code that pushes a path string's address (assert calls) belongs to that file
    tx = next(s for s in img.sections if s.name == "BEGTEXT")
    code = img.read_va(img.image_base + tx.rva, tx.rawsize)
    relocs = set(img.reloc_sites())
    refs = {}
    for dva, name in paths:
        pat = struct.pack("<I", dva)
        k = code.find(pat)
        while k >= 0:
            if img.image_base + tx.rva + k in relocs:
                refs.setdefault(name, []).append(img.image_base + tx.rva + k)
            k = code.find(pat, k + 1)
    print("\n== path strings referenced by code of a TU published under another name ==")
    per_tu = {}
    for name, sites in refs.items():
        for s in sites:
            t = owner(s)
            if t and t["as"] != name.lower():
                per_tu.setdefault(t["tu"], {}).setdefault(name, []).append(s)
    for tu, names in sorted(per_tu.items()):
        desc = "; ".join(f"{n} at {min(v):08x}..{max(v):08x} ({len(v)} refs)"
                         for n, v in sorted(names.items(), key=lambda kv: min(kv[1])))
        own = [n for n, sites in refs.items() if by_as.get(n.lower(), {}).get("tu") == tu
               and any((owner(s) or {}).get("tu") == tu for s in sites)]
        kind = "RENAME" if len(names) == 1 and not own and not by_as.get(next(iter(names)).lower()) else "SPLIT"
        print(f"  {kind:6s} {tu}: {desc}" + ("; its own path is referenced too" if own else ""))
        flags.append((kind, f"{tu}: {desc}"))
    unowned = {}
    for name, sites in refs.items():
        for s in sites:
            if owner(s) is None:
                unowned.setdefault(name, []).append(s)
    for name, v in sorted(unowned.items(), key=lambda kv: min(kv[1])):
        print(f"  NO-CODE {name}: referenced at {min(v):08x}..{max(v):08x} ({len(v)} refs) outside every accepted TU range")
    # data adjacency: initialized data between two path strings belongs to the first one's object (or is a
    # leading table of the second: codegen_rules.md); which accepted TU's code references that data?
    import bisect
    pvas = [dva for dva, _ in paths]
    near = {}
    base = img.image_base + tx.rva
    for site in sorted(relocs):
        if not (base <= site < base + len(code) - 3):
            continue
        tgt = struct.unpack_from("<I", code, site - base)[0]
        i = bisect.bisect_right(pvas, tgt) - 1
        if i < 0 or tgt - pvas[i] > 0x400 or (i + 1 < len(pvas) and tgt >= pvas[i + 1]) or tgt == pvas[i]:
            continue
        t = owner(site)
        if t:
            near.setdefault(paths[i][1], {}).setdefault(t["tu"], set()).add(tgt)
    print("\n== path strings in link order ==")
    # anchored = path whose TU is published under its name; the unanchored ones between two anchors belong to the
    # code between those TUs (or inside a combined one)
    last = None
    pending = []
    for dva, name in paths:
        t = by_as.get(name.lower())
        if t:
            if pending:
                lo = last["hi"] if last else 0
                span = f"{lo:08x}..{t['lo']:08x}"
                inside = [x["tu"] for x in tus if lo <= x["lo"] < t["lo"]]
                for pdva, pname in pending:
                    users = "; ".join(f"{k} ({len(v)} items after the string)" for k, v in sorted(near.get(pname, {}).items()))
                    print(f"  {pdva:08x} {pname:<22s} NO-TU  code in {span}" + (f"  data used by: {users}" if users else
                          (f"  between: {', '.join(Path(x).name for x in inside[:8])}" if inside else "")))
                    flags.append(("NO-TU", f"{pname} ({pdva:08x}): code in {span}; published there: {', '.join(inside) or 'nothing'}"))
                pending = []
            if verbose:
                print(f"  {dva:08x} {name:<22s} {t['tu']}  {t['lo']:08x}..{t['hi']:08x} ({t['n']} strict)")
            if last and t["lo"] < last["lo"]:
                flags.append(("ORDER", f"{name}: code {t['lo']:08x} before previous anchor {last['tu']}"))
            last = t
        else:
            pending.append((dva, name))
    for pdva, pname in pending:
        print(f"  {pdva:08x} {pname:<22s} NO-TU  code after {last['hi']:08x}" if last else f"  {pdva:08x} {pname} NO-TU")
        flags.append(("NO-TU", f"{pname} ({pdva:08x}): after {last['tu'] if last else '?'}"))
    print(f"\n{sum(1 for k, _ in flags if k == 'RENAME')} RENAME, {sum(1 for k, _ in flags if k == 'SPLIT')} SPLIT, {sum(1 for k, _ in flags if k == 'NO-TU')} NO-TU, "
          f"{sum(1 for k, _ in flags if k == 'ORDER')} ORDER")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
