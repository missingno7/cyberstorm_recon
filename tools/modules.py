"""Split the game part of BEGTEXT into probable object modules (work units) and annotate them.

A module boundary is a function start preceded by linker fill: zero bytes (WLINK pads between objects; the
compiler pads inside an object with no-op instructions). Modules whose functions all start on 16-byte
boundaries were most likely compiled with -ot (profile 'ot'); others with profile 'plain' (-4s -s). Path strings
(tools/tumap.py) give original file names. Boundaries between dword-aligned modules whose size is already a
multiple of 4 are invisible here, so a 'module' may contain several objects. Heuristic work list, not proof.

    python tools/modules.py [--oracle O]       write build/modules/O.json, print the list
"""
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pe as pemod  # noqa: E402
from common import BUILD, dump_json, load_json  # noqa: E402


def main(argv):
    argv = list(argv)
    name = None
    if "--oracle" in argv:
        i = argv.index("--oracle"); name = argv[i + 1]; del argv[i:i + 2]
    img = pemod.load(name)
    fm = json.loads((BUILD / "funcmap" / f"{img.name}.json").read_text())
    tm = json.loads((BUILD / "tumap" / f"{img.name}.json").read_text())
    led = load_json(Path(__file__).resolve().parent.parent / "recovery.json") if (Path(__file__).resolve().parent.parent / "recovery.json").exists() else {"tus": {}}
    matched = {e["va"]: tu for tu, t in led["tus"].items() for e in t["functions"].values()}
    path_of = {}
    for t in tm["tus"]:
        for f in t["funcs"]:
            path_of[f] = t["path"]
    funcs = fm["functions"]
    mods, cur, prevf = [], None, None
    for f in funcs:
        va = int(f["va"], 16)
        gap = b""
        if prevf is not None:
            ce = int(prevf["code_end"], 16)
            gap = img.read_va(ce, va - ce)
        prevf = f
        # linker fill is zero bytes; compiler fill is no-op instructions (never a 00 byte first)
        boundary = (cur is None or f["kind"] == "lib" or cur["kind"] == "lib"
                    or (len(gap) > 0 and not any(gap)))
        if boundary:
            cur = {"start": f["va"], "kind": "lib" if f["kind"] == "lib" else "game", "funcs": [], "paths": set()}
            mods.append(cur)
        cur["funcs"].append(f["va"])
        cur["end"] = f["end"]
        if f["va"] in path_of:
            cur["paths"].add(path_of[f["va"]])
    out = []
    for m in mods:
        vas = [int(v, 16) for v in m["funcs"]]
        size = int(m["end"], 16) - vas[0]
        out.append({"start": m["start"], "end": m["end"], "size": size, "kind": m["kind"], "nfuncs": len(vas),
                    "align16": all(v % 16 == 0 for v in vas), "paths": sorted(m["paths"]),
                    "matched": sum(1 for v in m["funcs"] if v in matched), "funcs": m["funcs"]})
    # merge consecutive library runs for readability
    dump_json(BUILD / "modules" / f"{img.name}.json", out)
    game = [m for m in out if m["kind"] == "game"]
    for m in game:
        print(f"{m['start']}..{m['end']} {m['size']:6d} B {m['nfuncs']:3d} f {'ot   ' if m['align16'] else 'plain'} "
              f"{m['matched']:3d} ok  {' '.join(m['paths'])}")
    print(f"{len(game)} game modules, {sum(1 for m in game if m['align16'])} all-16-aligned, "
          f"{sum(1 for m in game if m['paths'])} with path names")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
