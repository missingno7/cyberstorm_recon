"""Strict comparison of a compiled Watcom OMF object against the oracle PE.

    python tools/compare.py OBJ [--oracle O] [--at LABEL=VA ...] [--find] [--imply] [-v] [--json OUT]

--imply: externs referenced from byte-equal extents get a binding to their implied VA for this run, if every
reference implies the same VA and the VA is plausible (function start / data section). promote.py --publish
records such bindings as 'hypothesis' symbols.

Extents. Every segment is cut at its public offsets (Watcom emits LPUBDEFs for statics, so every function has
a name). An extent runs from its public to the next public or the segment end, so it includes the compiler's
alignment filler and any jump table that follows the function.

Placement (per extent, so a TU can be matched incrementally while earlier functions still differ in size):
  CODE extents: by a bound symbol of the extent (config/symbols.json, library identity) or --at. When all
  placed extents of a code segment imply one base, the remaining extents are placed from it ('segment').
  With --find, a still-unplaced code extent is placed if its fixup-masked bytes occur exactly once in BEGTEXT.
  DATA/BSS segments: one base per segment, implied by fixups from placed bytes (or bound publics). If the
  implied bases disagree (the TU's data layout is not reconstructed yet), the segment is not placed and each
  fixup into it is checked as an *item*: the oracle target must hold the same bytes as the candidate item
  (from the fixup's offset to the next public/referenced offset; a string item ends at its NUL).

Fixups. off32 absolute: field = target + inline addend, and the oracle must carry a HIGHLOW base relocation at
the site. Self-relative off32: field = target + addend - (site + 4). Targets: segment placement + disp, an
extern resolved by tools/symbols.py (symbols.json, imports, library identity), or a proven data item.

Verdict per extent:
  STRICT    every byte equal, every fixup resolved, base-relocation sites inside the extent exactly equal
  UNBOUND   equal except for fields of fixups whose target is not bound (implied target VAs are printed)
  DIFF      at least one byte differs, or relocation sites differ
  UNPLACED  no address
Initialized data segments placed at one base are compared byte for byte the same way. Nothing here writes
the ledger.
"""
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import omf  # noqa: E402
import pe as pemod  # noqa: E402
import symbols  # noqa: E402


def _code_window(img):
    s = img.section("BEGTEXT")
    return img.image_base + s.rva, img.section_bytes("BEGTEXT")


def masked_search(img, data, covered, fixups, lo, hi):
    """Unique-placement search of data[lo:hi] (fixup fields masked) in BEGTEXT."""
    t0, code = _code_window(img)
    pat = bytes(data[lo:hi])
    mask = bytearray(covered[lo:hi])
    for f in fixups:
        for k in range(f.size()):
            if lo <= f.off + k < hi:
                mask[f.off + k - lo] = 0
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
    if alen < 6:
        return []
    anchor = pat[a0:a0 + alen]
    hits, p = [], code.find(anchor)
    while p != -1:
        b = p - a0
        if b >= 0 and b + len(pat) <= len(code) and all(not mask[i] or code[b + i] == pat[i] for i in range(len(pat))):
            hits.append(t0 + b)
        p = code.find(anchor, p + 1)
    return hits


def is_code(seg):
    return seg.cls.upper() == "CODE"


def is_bss(seg):
    return seg.cls.upper() == "BSS" or (getattr(seg, "comdat", None) is None and seg.size and not any(seg.covered))


_STARTS = {}


def _all_filler(gap):
    """True if gap is a sequence of Watcom no-op filler instructions (see funcmap.pad_len)."""
    from funcmap import pad_len
    k = 0
    while k < len(gap):
        n = pad_len(gap, k)
        if not n or gap[k] == 0:
            return False
        k += n
    return True


def function_starts(img):
    """Sorted function starts of the oracle's function map (build/funcmap), or None if absent."""
    if img.name not in _STARTS:
        fm = Path(__file__).resolve().parent.parent / "build" / "funcmap" / f"{img.name}.json"
        _STARTS[img.name] = (sorted(int(f["va"], 16) for f in json.loads(fm.read_text())["functions"])
                             if fm.exists() else None)
    return _STARTS[img.name]


class Comparison:
    def __init__(self, mod, img, resolver, at=None, find=False):
        self.m, self.img, self.res = mod, img, resolver
        self.starts = function_starts(img)
        self.relsites = img.reloc_sites()
        self.conflicts = []
        self.fx_by_seg = {}
        for f in mod.fixups:
            self.fx_by_seg.setdefault(f.seg, []).append(f)
        # extents
        self.extents = {}       # si -> list of [off, end, names]
        for si, seg in enumerate(mod.segments):
            if not seg:
                continue
            offs = {0}
            names = {}
            for n, s, o, _ in mod.publics:
                if s == si:
                    offs.add(o)
                    names.setdefault(o, []).append(n)
            offs = sorted(o for o in offs if o < max(seg.size, 1))
            self.extents[si] = [[o, offs[k + 1] if k + 1 < len(offs) else seg.size, names.get(o, [])]
                                for k, o in enumerate(offs)]
        self.ext_va = {}        # (si, off) -> (va, how)
        self.base = {}          # data si -> (va, how)
        self.data_conflict = set()
        at = at or {}
        # 1. bound placement
        for si, exts in self.extents.items():
            seg = mod.segments[si]
            for off, end, names in exts:
                for n in names:
                    lab = symbols.label(n)
                    if lab in at or n in at:
                        self._place(si, off, at.get(lab, at.get(n)), "at")
                        continue
                    va, how = resolver.resolve(n)
                    if va is not None and how.split("~")[0] in ("matched", "hypothesis", "lib"):
                        self._place(si, off, va, "bound")
        self.notes = []
        # 2. masked search (before completion: one anchor must not drag a partial TU's later functions)
        if not find:
            self._complete_code()
        # 3. masked search: whole segment first (unique hit), then per extent, then completion again
        if find:
            for si, exts in self.extents.items():
                seg = mod.segments[si]
                if not is_code(seg) or not seg.size or any((si, o) in self.ext_va for o, _, _ in exts):
                    continue
                hits = masked_search(img, seg.data, seg.covered, self.fx_by_seg.get(si, []), 0, seg.size)
                if len(hits) == 1:
                    for o, _, _ in exts:
                        self.ext_va[(si, o)] = (hits[0] + o, "found")
            for si, exts in self.extents.items():
                seg = mod.segments[si]
                if not is_code(seg):
                    continue
                for o, e, names in exts:
                    if (si, o) in self.ext_va or e <= o:
                        continue
                    hits = masked_search(img, seg.data, seg.covered, self.fx_by_seg.get(si, []), o, e)
                    if len(hits) == 1:
                        self._place(si, o, hits[0], "found")
                    elif len(hits) > 1:
                        self.notes.append(f"{symbols.label(names[0]) if names else seg.name}: "
                                          f"{len(hits)} masked hits, not placed")
            self._complete_code()
        # 4. data bases implied by fixups from placed bytes
        implied = {}
        for _ in range(4):
            for si in list(self.extents):
                seg = mod.segments[si]
                for f in self.fx_by_seg.get(si, []):
                    m, d = f.target
                    if m & 3 != 0 or f.size() != 4 or is_code(mod.segments[d]):
                        continue
                    site = self.va_of(si, f.off)
                    if site is None:
                        continue
                    val = self._oracle_u32(site)
                    if val is None:
                        continue
                    tgt = (val + site + 4) & 0xFFFFFFFF if f.self_rel else val
                    disp = 0 if m & 4 else f.disp
                    implied.setdefault(d, set()).add((tgt - self._addend(si, f) - disp) & 0xFFFFFFFF)
            for d, bs in implied.items():
                if d in self.base:
                    continue
                if len(bs) == 1:
                    self.base[d] = (next(iter(bs)), "inferred")
                else:
                    self.data_conflict.add(d)

    def _complete_code(self):
        for si, exts in self.extents.items():
            if not is_code(self.m.segments[si]):
                continue
            bases = {self.ext_va[(si, o)][0] - o for o, _, _ in exts if (si, o) in self.ext_va}
            if len(bases) == 1:
                b = bases.pop()
                for o, _, _ in exts:
                    if (si, o) not in self.ext_va:
                        self.ext_va[(si, o)] = (b + o, "segment")

    def _place(self, si, off, va, how):
        seg = self.m.segments[si]
        if not is_code(seg):
            b = va - off
            if si in self.base and self.base[si][0] != b:
                self.conflicts.append(f"{seg.name}: base {self.base[si][0]:08x} vs {b:08x} ({how})")
            else:
                self.base[si] = (b, how)
            return
        if (si, off) in self.ext_va and self.ext_va[(si, off)][0] != va:
            self.conflicts.append(f"{seg.name}+{off:x}: {self.ext_va[(si, off)][0]:08x} vs {va:08x} ({how})")
            return
        self.ext_va[(si, off)] = (va, how)

    def va_of(self, si, off):
        seg = self.m.segments[si]
        if not is_code(seg):
            return self.base[si][0] + off if si in self.base else None
        for o, e, _ in self.extents.get(si, []):
            if o <= off < max(e, o + 1) and (si, o) in self.ext_va:
                return self.ext_va[(si, o)][0] + off - o
        return None

    def _oracle_u32(self, va):
        rva = va - self.img.image_base
        o = self.img.rva_to_off(rva)
        if o is None or self.img.rva_to_off(rva + 3) is None:
            return None
        return struct.unpack_from("<I", self.img.data, o)[0]

    def _addend(self, si, f):
        seg = self.m.segments[si]
        return struct.unpack_from("<I", bytes(seg.data[f.off:f.off + 4]).ljust(4, b"\0"))[0]

    def _item_bytes(self, d, disp):
        """Candidate bytes of the data item at segment d + disp (see module doc)."""
        seg = self.m.segments[d]
        if not any(seg.covered[disp:disp + 1]):
            return None
        bounds = {seg.size} | {o for n, s, o, _ in self.m.publics if s == d}
        for f in self.m.fixups:
            if f.target[0] & 3 == 0 and f.target[1] == d:
                bounds.add(0 if f.target[0] & 4 else f.disp)
        end = min(b for b in bounds if b > disp) if any(b > disp for b in bounds) else seg.size
        raw = bytes(seg.data[disp:end])
        if 0 in raw and all(32 <= c < 127 or c in (9, 10, 13) for c in raw[:raw.index(0)]) and raw.index(0) > 0:
            raw = raw[:raw.index(0) + 1]
        return raw

    def target(self, f, site):
        """(va, how, name) for a fixup target; va None if unknown."""
        m, d = f.target
        disp = 0 if m & 4 else f.disp
        if m & 3 == 0:
            seg = self.m.segments[d]
            if is_code(seg) or d in self.base:
                va = self.va_of(d, disp)
                if va is not None:
                    how = self.base[d][1] if d in self.base else self.ext_va.get((d, disp), (0, "code"))[1]
                    return va & 0xFFFFFFFF, "seg:" + how, seg.name
                return None, None, seg.name
            if d in self.data_conflict and site is not None:
                val = self._oracle_u32(site)
                item = self._item_bytes(d, disp)
                if val is not None and item:
                    tgt = (val + site + 4) & 0xFFFFFFFF if f.self_rel else val
                    tgt = (tgt - self._addend(f.seg, f)) & 0xFFFFFFFF
                    if self.img.read_va(tgt, len(item)) == item:
                        return (tgt + 0) & 0xFFFFFFFF, "item", f"{seg.name}+{disp:x}"
            return None, None, seg.name
        if m & 3 == 2:
            name = self.m.externs[d]
            va, how = self.res.resolve(name)
            if va is None:
                for n, si, off, _ in self.m.publics:
                    if n == name:
                        v = self.va_of(si, off)
                        if v is not None:
                            return (v + disp) & 0xFFFFFFFF, "self", name
                return None, None, name
            return (va + disp) & 0xFFFFFFFF, how, name
        return None, None, f"method{m}"

    def _evaluate(self, si, lo, hi, va0):
        """Compare segment si bytes [lo,hi) placed at va0. Returns dict."""
        seg = self.m.segments[si]
        n = hi - lo
        exp = bytearray(seg.data[lo:hi])
        known = bytearray(b"\x01" * n)
        abs_sites, unbound, binds = set(), {}, set()
        for f in self.fx_by_seg.get(si, []):
            if not (lo <= f.off < hi):
                continue
            sz = f.size()
            site = va0 + f.off - lo
            if not f.self_rel:
                abs_sites.add(site)
            if sz != 4:
                for k in range(sz):
                    if f.off + k - lo < n:
                        known[f.off + k - lo] = 0
                unbound.setdefault(f"<loc {f.kind()}>", set()).add(None)
                continue
            tva, how, name = self.target(f, site)
            add = self._addend(si, f)
            if tva is None:
                for k in range(4):
                    known[f.off + k - lo] = 0
                ov = self._oracle_u32(site)
                imp = None
                if ov is not None:
                    disp = 0 if f.target[0] & 4 else f.disp
                    imp = ((((ov + site + 4) if f.self_rel else ov) - add - disp) & 0xFFFFFFFF)
                unbound.setdefault(name, set()).add(imp)
                continue
            if how == "item":
                val = (tva - (site + 4)) & 0xFFFFFFFF if f.self_rel else tva
                val = (val + add) & 0xFFFFFFFF
            else:
                val = (tva + add - (site + 4)) & 0xFFFFFFFF if f.self_rel else (tva + add) & 0xFFFFFFFF
            exp[f.off - lo:f.off - lo + 4] = struct.pack("<I", val)
            binds.add((name, how))
        orc = self.img.read_va(va0, n)
        diff = [i for i in range(n) if known[i] and exp[i] != orc[i]]
        osites = {s for s in self.relsites if va0 <= s < va0 + n}
        size_bad = None
        if is_code(seg) and self.starts is not None:
            # the extent must end where the original function ends: at the next function start, or (last
            # extent of the segment) where only zero linker fill remains before the next start
            import bisect
            k = bisect.bisect_right(self.starts, va0)
            nxt = self.starts[k] if k < len(self.starts) else None
            end = va0 + n
            last = hi >= seg.size
            tail_fill = 0
            if nxt is not None and end != nxt:
                gap = self.img.read_va(end, nxt - end) if end < nxt else b""
                if last and gap and not any(gap):
                    pass                      # zero linker fill after the object's last function
                elif last and gap and _all_filler(gap):
                    tail_fill = len(gap)      # partial TU: compiler filler of the original, not produced here
                else:
                    size_bad = f"extent ends {end:08x}, original function ends {nxt:08x}"
        if diff or abs_sites != osites or size_bad:
            st = "DIFF"
        elif unbound:
            st = "UNBOUND"
        else:
            st = "STRICT"
        return {"status": st, "diff_bytes": len(diff), "first_diff": diff[0] if diff else None, "size_bad": size_bad,
                "tail_fill": tail_fill if is_code(seg) and self.starts is not None else 0,
                "reloc_extra": sorted(f"{s:08x}" for s in osites - abs_sites),
                "reloc_missing": sorted(f"{s:08x}" for s in abs_sites - osites),
                "unbound": sorted(unbound), "implied": {k: sorted(f"{v:08x}" for v in vs if v is not None)
                                                         for k, vs in unbound.items()},
                "bindings": sorted(binds)}

    def report(self):
        segs = []
        for si, seg in enumerate(self.m.segments):
            if not seg:
                continue
            rep = {"seg": seg.name, "class": seg.cls, "size": seg.size, "extents": []}
            code = is_code(seg)
            if not code:
                if si in self.base:
                    rep["base"] = f"{self.base[si][0]:08x}"
                    rep["placed"] = self.base[si][1]
                if is_bss(seg) or not seg.size:
                    rep["status"] = ("BSS" if si in self.base else "BSS-UNPLACED") if seg.size else "EMPTY"
                    segs.append(rep)
                    continue
                if si not in self.base:
                    rep["status"] = "ITEMS" if si in self.data_conflict else "UNPLACED"
                    segs.append(rep)
                    continue
            for o, e, names in self.extents.get(si, []):
                if e <= o:
                    continue
                va = self.va_of(si, o)
                ext = {"off": o, "end": e, "names": names or ["<segment start>"]}
                if va is None:
                    ext.update(status="UNPLACED", va=None)
                else:
                    ext.update(self._evaluate(si, o, e, va), va=f"{va:08x}",
                               placed=(self.ext_va.get((si, o), (0, "segment"))[1] if code else rep["placed"]))
                rep["extents"].append(ext)
            sts = {x["status"] for x in rep["extents"]}
            rep["status"] = next((s for s in ("DIFF", "UNPLACED", "UNBOUND") if s in sts), "STRICT")
            segs.append(rep)
        return {"module": self.m.name, "segments": segs, "conflicts": self.conflicts, "notes": self.notes}


def plausible(img, name, va):
    """An implied binding must point where such a symbol can live: code symbols at a function start of the
    function map, data symbols inside DGROUP or .bss."""
    fm = Path(__file__).resolve().parent.parent / "build" / "funcmap" / f"{img.name}.json"
    is_func = symbols.is_function(name)
    rva = va - img.image_base
    sec = img.section_at_rva(rva)
    if sec is None:
        return False
    if is_func:
        if sec.name != "BEGTEXT":
            return False
        if fm.exists():
            starts = {int(f["va"], 16) for f in json.loads(fm.read_text())["functions"]}
            return va in starts
        return True
    return sec.name in ("DGROUP", ".bss")


def implied_bindings(reports, img):
    """Externs implied by byte-equal UNBOUND extents: {symbol: va} when unique, consistent and plausible;
    plus a list of rejected names with reasons."""
    seen, rejected = {}, []
    for r in reports:
        for s in r["segments"]:
            for e in s.get("extents", []):
                if e["status"] != "UNBOUND":
                    continue
                for n, vs in e["implied"].items():
                    if n.startswith("<"):
                        continue
                    seen.setdefault(n, set()).update(vs)
    out = {}
    for n, vs in seen.items():
        if len(vs) != 1:
            rejected.append((n, f"inconsistent {sorted(vs)}"))
            continue
        va = int(next(iter(vs)), 16)
        if not plausible(img, n, va):
            rejected.append((n, f"implausible target {va:08x}"))
            continue
        out[n] = va
    return out, rejected


def compare_object(path, oracle=None, at=None, find=False, imply=False):
    """Compare; with imply=True run a second pass that adopts implied extern bindings (how='implied')."""
    img = pemod.load(oracle)
    res = symbols.Resolver(img)
    mods = omf.load(path)
    reports = [Comparison(m, img, res, at, find).report() for m in mods]
    if imply:
        extra, rejected = implied_bindings(reports, img)
        if extra:
            res.extra = extra
            reports = [Comparison(m, img, res, at, find).report() for m in mods]
        for r in reports:
            r["implied_bindings"] = {k: f"{v:08x}" for k, v in extra.items()}
            r["implied_rejected"] = rejected
    return reports, img


def print_report(reports, verbose=False):
    for r in reports:
        for c in r["conflicts"]:
            print("CONFLICT:", c)
        for c in r.get("notes", []):
            print("note:", c)
        for s in r["segments"]:
            if s["status"] == "EMPTY" and not verbose:
                continue
            print(f"[{s['status']:8s}] {s['seg'][:40]:<12s} {s['class']:5s} size {s['size']:#06x}"
                  + (f" @ {s['base']} ({s['placed']})" if "base" in s else ""))
            for e in s.get("extents", []):
                if e["status"] == "STRICT" and not verbose:
                    continue
                extra = ""
                if e["status"] == "DIFF":
                    if e["diff_bytes"]:
                        extra = f" {e['diff_bytes']} bytes differ, first at +{e['first_diff']}"
                    if e["reloc_extra"] or e["reloc_missing"]:
                        extra += f" relocs extra {e['reloc_extra']} missing {e['reloc_missing']}"
                    if e.get("size_bad"):
                        extra += " " + e["size_bad"]
                elif e["status"] == "UNBOUND":
                    extra = " unbound: " + ", ".join(f"{symbols.label(k)}->{v}" for k, v in e["implied"].items())
                va = e.get("va") or "--------"
                print(f"    {e['status']:8s} {va} +{e['end'] - e['off']:<5d} {symbols.label(e['names'][0])}"
                      + (f" ({e['placed']})" if verbose and e.get("placed") else "") + extra)


def main(argv):
    argv = list(argv)
    oracle, at, find, verbose, jout = None, {}, False, False, None
    while "--at" in argv:
        i = argv.index("--at")
        k, v = argv[i + 1].split("=")
        at[k] = int(v, 16)
        del argv[i:i + 2]
    if "--oracle" in argv:
        i = argv.index("--oracle"); oracle = argv[i + 1]; del argv[i:i + 2]
    if "--json" in argv:
        i = argv.index("--json"); jout = argv[i + 1]; del argv[i:i + 2]
    if "--find" in argv:
        argv.remove("--find"); find = True
    if "-v" in argv:
        argv.remove("-v"); verbose = True
    imply = "--imply" in argv
    if imply:
        argv.remove("--imply")
    if len(argv) != 2:
        print(__doc__)
        return 2
    reports, _ = compare_object(argv[1], oracle, at, find, imply)
    print_report(reports, verbose)
    for r in reports:
        for k, v in r.get("implied_bindings", {}).items():
            print(f"implied binding: {symbols.label(k)} = {v}")
        for k, why in r.get("implied_rejected", []):
            print(f"implied binding REJECTED: {symbols.label(k)}: {why}")
    if jout:
        Path(jout).write_text(json.dumps(reports, indent=1))
    bad = any(s["status"] not in ("STRICT", "BSS", "EMPTY") for r in reports for s in r["segments"])
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
