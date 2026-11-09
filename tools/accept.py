"""Integrator helper: run the promote commands found in worker reports through the gate.

    python tools/accept.py candidates/<lane>/REPORT.md [...] [--publish]

Each line of the form `python tools/promote.py ...` (optionally in backticks) is re-run exactly, as a dry run or
with --publish, and only within candidates/. Then tools/verify.py runs once. Output: one summary line per TU.
"""
import re
import shlex
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import ROOT  # noqa: E402

LINE = re.compile(r"python(?:\s+-\w+)*\s+(?:\.[\\/])?tools[\\/]promote\.py\s+(candidates/\S+.*?)\s*`?\s*$")


def commands(report):
    out = []
    for line in Path(report).read_text(encoding="utf-8", errors="replace").splitlines():
        m = LINE.search(line.strip().strip("`"))
        if m:
            args = shlex.split(m.group(1), posix=True)
            args = [a for a in args if a != "--publish"]
            if args and args[0].startswith("candidates/") and "--tu" in args:
                out.append(args)
    return out


def main(argv):
    publish = "--publish" in argv
    reports = [a for a in argv[1:] if not a.startswith("--")]
    seen = set()
    ok = True
    for rep in reports:
        for args in commands(rep):
            key = args[args.index("--tu") + 1]
            if key in seen:
                continue
            seen.add(key)
            r = subprocess.run([sys.executable, "tools/promote.py", *args] + (["--publish"] if publish else []),
                               cwd=ROOT, capture_output=True, text=True)
            lines = [l for l in r.stdout.splitlines()
                     if re.search(r"strict functions|LOSS|CONFLICT|refusing|POLICY|collision|ALIAS|failed", l)]
            print(f"{'OK ' if r.returncode == 0 else 'ERR'} {key}: " + " | ".join(lines)[:400])
            if r.returncode:
                ok = False
                print(r.stdout[-1500:])
    if publish:
        r = subprocess.run([sys.executable, "tools/verify.py"], cwd=ROOT, capture_output=True, text=True)
        print(r.stdout.strip().splitlines()[-2] if r.stdout.strip() else r.stderr[-500:])
        print(r.stdout.strip().splitlines()[-1] if r.stdout.strip() else "")
        ok = ok and r.returncode == 0
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv))
