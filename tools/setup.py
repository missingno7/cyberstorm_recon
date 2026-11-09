"""Regenerate every derived artefact under build/ from the pinned oracles (idempotent, ~10 s).

    python tools/setup.py

Order: oracle locks check -> library identity (both EXE oracles) -> function map -> TU map -> module list.
"""
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import CONFIG, ROOT, TOOLS_DIR, load_json  # noqa: E402

PY = sys.executable


def run(*args):
    print("+", " ".join(args), flush=True)
    r = subprocess.run([PY, *args], cwd=ROOT)
    if r.returncode:
        raise SystemExit(f"failed: {args}")


def main():
    for o in ("cs11", "cs10", "gog", "dll11", "dll10"):
        run("tools/pe.py", "check", o)
    inst = load_json(CONFIG / "toolchain.json")["installs"]["wc106"]
    lib = TOOLS_DIR / inst["dir"] / "LIB386"
    libs = [str(lib / "NT" / "CLIB3S.LIB"), str(lib / "PLIBMT3S.LIB"), str(lib / "MATH387S.LIB"), str(lib / "MATH3S.LIB")]
    for o in ("cs11", "cs10"):
        run("tools/libscan.py", "--oracle", o, "--symbols", *libs)
        run("tools/funcmap.py", "--oracle", o)
        run("tools/tumap.py", "--oracle", o)
    run("tools/modules.py")
    print("setup OK")


if __name__ == "__main__":
    main()
