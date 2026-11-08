"""Compile one source with a pinned Watcom install, natively, in a private short scratch directory.

    python tools/cc.py [--profile P | --install I --tool wpp386|wcc386] [FLAGS...] SRC -fo=OUT.obj

Before each run the install's key files are hash-checked. The environment is reduced to WATCOM, INCLUDE
(install headers + src/include), PATH (install BINNT), TMP/TEMP (private dir). Sources are copied into the
private directory under their own basename, so the path the compiler sees is short and stable
(Watcom 10.6 BINNT loaders crash on long paths; __FILE__ is not path-dependent here because the original
paths are relative like 'sim\\sim.c' -- see docs/toolchain.md before relying on __FILE__).
"""
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import CONFIG, ROOT, SRC, TOOLS_DIR, load_json, sha256_file  # noqa: E402

SCRATCH = Path(os.environ.get("CSTORM_SCRATCH", str(ROOT / "build" / "cc")))  # keep short: WCC386 crashes on ~150-char paths
_checked = set()


def install(name):
    ent = load_json(CONFIG / "toolchain.json")["installs"][name]
    base = TOOLS_DIR / ent["dir"]
    if name not in _checked:
        for rel, want in ent["key_files"].items():
            got = sha256_file(base / rel)
            if got != want:
                raise SystemExit(f"{name}: {rel} sha256 {got} != pinned {want}")
        _checked.add(name)
    return ent, base


def profile(name):
    return load_json(CONFIG / "toolchain.json")["profiles"][name]


def compile_one(src, out, flags, inst="wc106", tool="wpp386", extra_include=(), as_name=None):
    """Return (returncode, compiler text). `as_name` = relative path to present to the compiler
    (e.g. 'sim\\sim.c'); directories are recreated inside the private scratch dir."""
    ent, base = install(inst)
    SCRATCH.mkdir(parents=True, exist_ok=True)
    src = Path(src).resolve()
    out = Path(out).resolve()
    inc = [str(base / p) for p in ent.get("include", [])] + [str(Path(p).resolve()) for p in extra_include]
    inc.append(str(SRC / "include"))
    with tempfile.TemporaryDirectory(prefix="c", dir=SCRATCH) as td:
        td = Path(td)
        rel = Path(as_name.replace("\\", "/")) if as_name else Path(src.name)
        (td / rel).parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(src, td / rel)
        obj = td / "unit.obj"
        env = {"SYSTEMROOT": os.environ.get("SYSTEMROOT", "C:\\Windows"), "WATCOM": str(base),
               "INCLUDE": ";".join(inc), "PATH": str(base / "BINNT"), "TMP": str(td), "TEMP": str(td)}
        exe = base / ent["tools"][tool]
        cmd = [str(exe), *flags, str(rel).replace("/", "\\"), "-fo=unit.obj"]
        r = subprocess.run(cmd, env=env, cwd=td, capture_output=True, text=True, timeout=120)
        text = r.stdout + r.stderr
        if r.returncode == 0 and obj.is_file() and obj.stat().st_size:
            out.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(obj, out)
            return 0, text
        return r.returncode or 1, text + "\n(no object produced)"


def main(argv):
    argv = list(argv[1:])
    inst, tool, flags, as_name = "wc106", "wpp386", [], None
    for opt in ("--install", "--tool", "--profile", "--as"):
        if opt in argv:
            i = argv.index(opt)
            val = argv[i + 1]
            del argv[i:i + 2]
            if opt == "--install":
                inst = val
            elif opt == "--tool":
                tool = val
            elif opt == "--as":
                as_name = val
            else:
                p = profile(val)
                inst, tool, flags = p["install"], p["tool"], list(p["flags"])
    out = [a for a in argv if a.startswith("-fo=")]
    srcs = [a for a in argv if not a.startswith("-")]
    flags += [a for a in argv if a.startswith("-") and not a.startswith("-fo=")]
    if len(srcs) != 1 or len(out) != 1:
        print(__doc__)
        return 2
    rc, text = compile_one(srcs[0], out[0][4:], flags, inst, tool, as_name=as_name)
    print(text, end="")
    return rc


if __name__ == "__main__":
    sys.exit(main(sys.argv))
