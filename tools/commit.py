"""Integrator commit: run verify.py and test_gate.py, and commit all changes only if both pass.

    python tools/commit.py "message"
"""
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import ROOT  # noqa: E402

TRAILER = "\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"


def main(argv):
    if len(argv) != 2:
        print(__doc__)
        return 2
    for tool in ("tools/verify.py", "tools/test_gate.py"):
        r = subprocess.run([sys.executable, tool], cwd=ROOT, capture_output=True, text=True)
        tail = r.stdout.strip().splitlines()[-2:]
        print(f"{tool}: " + " | ".join(tail))
        if r.returncode:
            print("\n".join(l for l in r.stdout.splitlines() if "FAIL" in l))
            print("NOT COMMITTED")
            return 1
    subprocess.run(["git", "add", "-A"], cwd=ROOT, check=True)
    r = subprocess.run(["git", "commit", "-q", "-m", argv[1] + TRAILER], cwd=ROOT, capture_output=True, text=True)
    print(r.stdout + r.stderr)
    subprocess.run(["git", "log", "--oneline", "-1"], cwd=ROOT)
    return r.returncode


if __name__ == "__main__":
    sys.exit(main(sys.argv))
