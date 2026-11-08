"""Map code to original translation units using the embedded __FILE__ path strings.

Each path string ('sim\\sim.c') lives in its own TU's CONST. A function that references it (absolute
relocation inside the function's extent) belongs to that TU. Watcom/WLINK place code and CONST in object
order, so per-TU code spans and path-string addresses must both increase in link order; that is checked.

    python tools/tumap.py [--oracle O]          write build/tumap/O.json and print spans
"""
import bisect
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pe as pemod  # noqa: E402
from common import BUILD, dump_json  # noqa: E402

PATH_RE = re.compile(rb"[A-Za-z0-9_]+(?:\\[A-Za-z0-9_]+)+\.(?:c|cpp|asm)\0")


def main(argv):
    argv = list(argv)
    name = None
    if "--oracle" in argv:
        i = argv.index("--oracle"); name = argv[i + 1]; del argv[i:i + 2]
    img = pemod.load(name)
    fm = json.loads((BUILD / "funcmap" / f"{img.name}.json").read_text())
    funcs = [(int(f["va"], 16), int(f["end"], 16), f) for f in fm["functions"]]
    starts = [f[0] for f in funcs]
    dg = img.section("DGROUP")
    dva = img.image_base + dg.rva
    data = img.section_bytes("DGROUP")
    paths = []
    for m in PATH_RE.finditer(data):
        # require a NUL (or start) before: a whole string, not a suffix
        if m.start() and data[m.start() - 1] != 0:
            continue
        paths.append((dva + m.start(), m.group()[:-1].decode()))
    pva = {va: p for va, p in paths}
    t0 = img.image_base + img.section("BEGTEXT").rva
    t1 = t0 + len(img.section_bytes("BEGTEXT"))
    refs = {}
    for site in img.reloc_sites():
        if not (t0 <= site < t1):
            continue
        v = img.u32_va(site)
        if v in pva:
            k = bisect.bisect_right(starts, site) - 1
            refs.setdefault(v, set()).add(starts[k])
    tus = []
    for va, p in paths:
        fs = sorted(refs.get(va, ()))
        tus.append({"path": p, "string_va": f"{va:08x}",
                    "funcs": [f"{x:08x}" for x in fs],
                    "first": f"{fs[0]:08x}" if fs else None, "last": f"{fs[-1]:08x}" if fs else None})
    # monotonicity check in link order
    prev_last, bad = 0, []
    for t in tus:
        if not t["first"]:
            continue
        f, l = int(t["first"], 16), int(t["last"], 16)
        if f < prev_last:
            bad.append(t["path"])
        prev_last = max(prev_last, l)
    out = BUILD / "tumap" / f"{img.name}.json"
    dump_json(out, {"oracle": img.name, "tus": tus, "order_violations": bad})
    for t in tus:
        print(f"{t['string_va']} {t['path']:28s} funcs {len(t['funcs']):3d} code {t['first']}..{t['last']}")
    print(f"{len(tus)} paths, {sum(1 for t in tus if t['funcs'])} with referencing code, "
          f"order violations: {bad or 'none'} -> {out}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
