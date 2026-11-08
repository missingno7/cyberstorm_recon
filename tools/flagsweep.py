"""Deliberate flag sweep for one candidate object (diagnostic only; never proof).

    python tools/flagsweep.py SRC.c AS_NAME [Label=VA ...] [--cpus -3s,-4s,-5s] [--opts -ox,-oxt] [--extra ,-d1]
                              [--tool wpp386|wcc386] [--install wc106] [--top 12]

Each combination is compiled and compared (--find --imply); ranked by (non-strict extents, differing bytes).
"""
import itertools
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cc  # noqa: E402
import compare  # noqa: E402
from common import ROOT  # noqa: E402

CPUS = ["-3s", "-4s", "-5s"]
OPTS = ["-ox", "-oxt", "-ot", "-os", "-od", "-oaxt", "-otl", "-obmilr", "-obmiler", "-olr", "-oil", "-oxs",
        "-on", "-oe", "-ol+", "-oxl+", "-oneatx", "-or", "-oi"]
EXTRA = ["", "-d1", "-d2", "-of", "-zp4", "-ei", "-ri"]


def opt(argv, name, default):
    if name in argv:
        i = argv.index(name)
        v = argv[i + 1]
        del argv[i:i + 2]
        return v.split(",")
    return default


def main(argv):
    argv = list(argv[1:])
    cpus, opts, extra = opt(argv, "--cpus", CPUS), opt(argv, "--opts", OPTS), opt(argv, "--extra", EXTRA)
    tool = opt(argv, "--tool", ["wpp386"])[0]
    inst = opt(argv, "--install", ["wc106"])[0]
    top = int(opt(argv, "--top", ["12"])[0])
    src, as_name = argv[0], argv[1]
    at = {k: int(v, 16) for k, v in (a.split("=") for a in argv[2:])}
    res = []
    with tempfile.TemporaryDirectory(dir=ROOT / "build") as td:
        obj = Path(td) / "s.obj"
        for c, o, e in itertools.product(cpus, opts, extra):
            fl = [c, "-s"] + ([o] if o else []) + ([e] if e else [])
            rc, _ = cc.compile_one(src, obj, fl, inst, tool, as_name=as_name)
            if rc:
                continue
            reps, _ = compare.compare_object(obj, None, at, True, True)
            exts = [x for r in reps for s in r["segments"] if s["class"].upper() == "CODE"
                    for x in s.get("extents", [])]
            bad = sum(1 for x in exts if x["status"] != "STRICT")
            diff = sum(x.get("diff_bytes") or 0 for x in exts)
            res.append((bad, diff, len(exts), " ".join(fl)))
    res.sort()
    for bad, diff, n, fl in res[:top]:
        print(f"{n - bad}/{n} strict, {diff:4d} diff bytes  {fl}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
