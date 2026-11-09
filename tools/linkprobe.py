"""Image-level probe: does WLINK lay out runs of reproduced objects exactly as the original image does?

    python tools/linkprobe.py [--min 2] [-v]

1. Every accepted TU is checked with the TU-level rules (tools/tucheck.py). COMPLETE TUs are sorted by code base and
   grouped into runs of objects that are adjacent in the original (only zero linker fill between them, no original
   function start in the gap).
2. Each run is compiled fresh (pinned toolchain) and linked alone with the pinned WLINK 10.6 (`system nt_win`,
   `option undefsok`, objects in original order, no libraries) into build/linkprobe/run_NN.exe with a map file.
3. From the map, every object's segment addresses are read and compared with the original as offsets relative to
   the run's first object: code (BEGTEXT), CONST, CONST2, _DATA, _BSS. Equal offsets mean the linker's alignment,
   fill and segment concatenation rules are reproduced for that run; absolute addresses are not compared (objects
   before the run are missing in the probe).
This is a diagnostic: no counted proof level, nothing written outside build/linkprobe.
"""
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cc  # noqa: E402
import gate  # noqa: E402
import pe as pemod  # noqa: E402
import tucheck  # noqa: E402
from common import BUILD, ROOT  # noqa: E402

OUT = BUILD / "linkprobe"
SEGS = ("CONST", "CONST2", "_DATA", "_BSS")


def runs_of(rows, img, starts):
    rows = sorted((r for r in rows if r["ok"] and "code" in r["place"]), key=lambda r: r["place"]["code"])
    runs, cur = [], []
    for r in rows:
        if cur:
            prev = cur[-1]
            end = prev["place"]["code"] + prev["size"]
            gap = r["place"]["code"] - end
            if 0 <= gap < 16 and not any(img.read_va(end, gap)) and not any(end <= s < r["place"]["code"]
                                                                           for s in starts):
                cur.append(r)
                continue
            runs.append(cur)
        cur = [r]
    if cur:
        runs.append(cur)
    return runs


def parse_map(text):
    """{module basename: {segment: address}} from a WLINK map's 'Segment' table."""
    out = {}
    for line in text.splitlines():
        m = re.match(r"^(\S+)\s+(\S+)\s+(\S+)\s+([0-9a-fA-F]{4}):([0-9a-fA-F]{8})\s+([0-9a-fA-F]{8})", line)
        if m:
            seg, _cls, _grp, _sel, addr, _size = m.groups()
            out.setdefault("_", []).append((seg, int(addr, 16)))
    return out


def link_run(k, run):
    OUT.mkdir(parents=True, exist_ok=True)
    objs = []
    for i, r in enumerate(run):
        inst, tool, flags = gate.flags_for(r["ent"])
        obj = OUT / f"run{k:02d}_{i:02d}.obj"
        rc, text = cc.compile_one(ROOT / r["tu"], obj, flags, inst, tool, as_name=r["ent"].get("as"))
        if rc:
            raise RuntimeError(text)
        objs.append(obj)
    exe = OUT / f"run{k:02d}.exe"
    lnk = OUT / f"run{k:02d}.lnk"
    lnk.write_text("system nt_win\noption quiet, undefsok, stack=48k\n"
                   f"option map={exe.with_suffix('.map').name}\nname {exe.name}\n"
                   + "".join(f"file {o.name}\n" for o in objs), encoding="ascii")
    ent, base = cc.install("wc106")
    env = {"SYSTEMROOT": "C:\\Windows", "WATCOM": str(base), "PATH": str(base / "BINNT"),
           "LIB": str(base / "LIB386") + ";" + str(base / "LIB386" / "NT")}
    r = subprocess.run([str(base / ent["tools"]["wlink"]), f"@{lnk.name}"], cwd=OUT, env=env,
                       stdin=subprocess.DEVNULL, capture_output=True, text=True, timeout=300)
    return exe, objs, r.stdout + r.stderr


def _find(sec_bytes, data, covered, fixups, size):
    """Offsets where data[:size] occurs with fixup fields and uncovered bytes masked."""
    known = bytearray(covered[:size])
    for f in fixups:
        for k in range(f.size()):
            if f.off + k < size:
                known[f.off + k] = 0
    runs = [(i, j) for i, j in _runs(known)]
    if not runs:
        return []
    a0, a1 = max(runs, key=lambda r: r[1] - r[0])
    needle = bytes(data[a0:a1])
    hits, k = [], sec_bytes.find(needle)
    while k >= 0:
        o = k - a0
        if o >= 0 and o + size <= len(sec_bytes) and all(not known[j] or sec_bytes[o + j] == data[j]
                                                          for j in range(size)):
            hits.append(o)
        k = sec_bytes.find(needle, k + 1)
    return hits


def _runs(known):
    i, n = 0, len(known)
    while i < n:
        if known[i]:
            j = i
            while j < n and known[j]:
                j += 1
            yield i, j
            i = j
        else:
            i += 1


def object_layout(exe, objs):
    """Per object: linked address of each segment, found by its own bytes (fixup fields masked) in the linked
    image; a segment counts only with a unique hit. BSS has no bytes and is not located."""
    import omf
    lk = pemod.PE(exe.read_bytes(), "probe")
    code_sec = lk.sections[0].name              # the code section is named after its first segment
    data_sec = next((x.name for x in lk.sections[1:] if x.rawsize and x.name not in (".reloc", ".idata", ".edata")),
                    None)
    secs = {"BEGTEXT": (lk.image_base + lk.section(code_sec).rva, lk.section_bytes(code_sec)),
            "DGROUP": (lk.image_base + lk.section(data_sec).rva, lk.section_bytes(data_sec)) if data_sec else (0, b"")}
    lay = {}
    for o in objs:
        mod = omf.load(o)[0]
        segs = {}
        for si, seg in enumerate(mod.segments):
            if not seg or not seg.size or getattr(seg, "comdat", None) is not None or seg.cls.upper() == "BSS":
                continue
            where = "BEGTEXT" if seg.cls.upper() == "CODE" else "DGROUP"
            base, data = secs[where]
            hits = _find(data, seg.data, seg.covered, [f for f in mod.fixups if f.seg == si], seg.size)
            if len(hits) == 1:
                segs["_TEXT" if seg.cls.upper() == "CODE" else seg.name] = (0, base + hits[0])
        lay[o.name.lower()] = segs
    return lay, ""


def main(argv):
    ledger = gate.load_ledger()
    img = pemod.load(None)
    import json
    fm = json.loads((BUILD / "funcmap" / "cs11.json").read_text())
    starts = {int(f["va"], 16) for f in fm["functions"]}
    rows = []
    for tu, ent in sorted(ledger["tus"].items()):
        try:
            ok, why, size, place = tucheck.check_tu(tu, ent, img, starts)
        except RuntimeError:
            continue
        rows.append({"tu": tu, "ent": ent, "ok": ok, "size": size, "place": place})
    outl = {tu for tu, _ in tucheck.order_outliers([(r["tu"], r["place"]) for r in rows])}
    for r in rows:
        if r["tu"] in outl:
            r["ok"] = False
    mn = int(argv[argv.index("--min") + 1]) if "--min" in argv else 2
    runs = [r for r in runs_of(rows, img, starts) if len(r) >= mn]
    print(f"{sum(len(r) for r in runs)} complete TUs in {len(runs)} adjacent runs of >= {mn}")
    agree = total = 0
    for k, run in enumerate(runs):
        exe, objs, text = link_run(k, run)
        if not exe.exists():
            print(f"run {k}: link failed: {text[:300]}")
            continue
        lay, mp = object_layout(exe, objs)
        b0 = {}
        lines = []
        for o, r in zip(objs, run):
            got = lay.get(o.name.lower(), {})
            for seg in ("_TEXT",) + SEGS:
                orig = r["place"].get("code" if seg == "_TEXT" else seg)
                mine = got.get(seg)
                mine = None if mine is None else mine[0] * 0x10000000 + mine[1]
                if orig is None or mine is None:
                    continue
                if seg not in b0:
                    b0[seg] = (orig, mine)
                total += 1
                same = orig - b0[seg][0] == mine - b0[seg][1]
                agree += same
                if not same or "-v" in argv:
                    lines.append(f"    {'ok ' if same else 'BAD'} {r['tu']} {seg}: original +{orig - b0[seg][0]:#x}, "
                                 f"linked +{mine - b0[seg][1]:#x}")
        print(f"run {k}: {len(run)} TUs {run[0]['tu']} .. {run[-1]['tu']}")
        print("\n".join(lines))
    print(f"relative segment offsets equal: {agree} of {total}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
