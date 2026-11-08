"""Locate Watcom library members inside the oracle's BEGTEXT (PE version of rayman/kegg libscan).

For every CODE segment of every module, search BEGTEXT for its bytes with fixup fields masked. Classes:
  exact+sites  unmasked bytes equal, every absolute (off32) fixup coincides with a PE base relocation,
               self-relative fixups have none, and no extra base relocation falls inside the extent;
  masked       bytes equal modulo fixup fields only.
Hits are library-identity evidence, not proof of targets.

    python tools/libscan.py [--oracle O] LIB...                  hits sorted by address
    python tools/libscan.py [--oracle O] --compare A.LIB B.LIB   members found with one release only
    python tools/libscan.py [--oracle O] --symbols LIB...        write build/libsyms/O.json (unique exact+sites)
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import omf  # noqa: E402
import pe as pemod  # noqa: E402
from common import BUILD, dump_json  # noqa: E402


def masked_pattern(mod, si):
    seg = mod.segments[si]
    mask = bytearray(seg.covered)
    sites = []
    for f in mod.fixups:
        if f.seg == si:
            sites.append((f.off, f.size(), f.self_rel))
            for k in range(f.size()):
                if f.off + k < len(mask):
                    mask[f.off + k] = 0
    return bytes(seg.data), bytes(mask), sites


def find_masked(hay, pat, mask, min_anchor=8):
    n = len(pat)
    best, cur, start = (0, 0), 0, 0
    for i, m in enumerate(mask):
        if m:
            if cur == 0:
                start = i
            cur += 1
            if cur > best[1]:
                best = (start, cur)
        else:
            cur = 0
    a0, alen = best
    if alen < min_anchor:
        return []
    anchor = pat[a0:a0 + alen]
    hits, p = [], hay.find(anchor)
    while p != -1:
        base = p - a0
        if base >= 0 and base + n <= len(hay) and all(not mask[i] or hay[base + i] == pat[i] for i in range(n)):
            hits.append(base)
        p = hay.find(anchor, p + 1)
    return hits


def scan(img, libs):
    text = img.section("BEGTEXT")
    code = img.section_bytes("BEGTEXT")
    tva = img.image_base + text.rva
    sites = {v - tva for v in img.reloc_sites() if tva <= v < tva + len(code)}
    out = []
    for lib in libs:
        for m in omf.load(lib):
            for si, seg in enumerate(m.segments):
                if not seg or seg.cls.upper() != "CODE" or seg.size == 0:
                    continue
                pat, mask, fx = masked_pattern(m, si)
                for base in find_masked(code, pat, mask):
                    want = {base + o for o, sz, rel in fx if not rel and sz == 4}
                    have = {s for s in sites if base <= s < base + seg.size}
                    rel_ok = all(rel or sz == 4 for o, sz, rel in fx)
                    cls = "exact+sites" if want == have and rel_ok else "masked"
                    out.append({"lib": str(lib), "module": m.name, "seg": seg.name, "size": seg.size,
                                "va": tva + base, "class": cls,
                                "publics": [n for n, s, o, _ in m.publics if s == si],
                                "public_vas": {n: tva + base + o for n, s, o, _ in m.publics if s == si}})
    return out


def main(argv):
    argv = list(argv)
    name = None
    if "--oracle" in argv:
        i = argv.index("--oracle")
        name = argv[i + 1]
        del argv[i:i + 2]
    img = pemod.load(name)
    if argv[1] == "--compare":
        a, b = argv[2], argv[3]
        ra, rb = scan(img, [a]), scan(img, [b])
        ka = {(r["module"], r["va"], r["class"]) for r in ra}
        kb = {(r["module"], r["va"], r["class"]) for r in rb}
        for lab, rr in ((a, ra), (b, rb)):
            ex = [r for r in rr if r["class"] == "exact+sites"]
            print(f"{lab}: {len(rr)} hits {sum(r['size'] for r in rr)} B; exact+sites {len(ex)} "
                  f"{sum(r['size'] for r in ex)} B")
        for k in sorted(ka - kb, key=lambda x: x[1]):
            print(f"  only A: {k[1]:08x} {k[0]} {k[2]}")
        for k in sorted(kb - ka, key=lambda x: x[1]):
            print(f"  only B: {k[1]:08x} {k[0]} {k[2]}")
        return 0
    if argv[1] == "--symbols":
        res = scan(img, argv[2:])
        count = {}
        for r in res:
            count[(r["lib"], r["module"])] = count.get((r["lib"], r["module"]), 0) + 1
        syms = {}
        for r in res:
            if r["class"] != "exact+sites" or count[(r["lib"], r["module"])] != 1:
                continue
            for n, a in r["public_vas"].items():
                syms[n] = {"va": f"{a:08x}", "module": r["module"], "lib": Path(r["lib"]).as_posix(),
                           "size": r["size"] if a == r["va"] else None}
        out = BUILD / "libsyms" / f"{img.name}.json"
        dump_json(out, syms)
        print(f"{len(syms)} library symbols -> {out}")
        return 0
    res = scan(img, argv[1:])
    for r in sorted(res, key=lambda r: r["va"]):
        print(f"{r['va']:08x} +{r['size']:<5x} {r['class']:11s} {Path(r['lib']).name}:{r['module']} "
              f"{' '.join(r['publics'][:3])}")
    print(f"# {len(res)} hits, {sum(r['size'] for r in res)} bytes, "
          f"{sum(1 for r in res if r['class'] == 'exact+sites')} exact+sites")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
