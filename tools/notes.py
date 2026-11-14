"""Attempt ledger: what was tried on a parked function and how it ended. One line per attempt, structured.

    python tools/notes.py add --lane L --va VA --label NAME --hyp "what was changed and why" --result "what happened"
                              [--before N --after N] [--verdict open|invariant|dead-end|fixed]
    python tools/notes.py show VA_OR_LABEL          class, structured attempts, then the report lines that mention it
    python tools/notes.py merge                     integrator: lane files -> docs/attempts.jsonl, reports -> docs/lane_reports/

Workers: call `add` once per hypothesis FAMILY you finish on a function (not per compile), also when it failed:
a recorded negative is what saves the next lane the same hour. It appends to candidates/<lane>/attempts.jsonl.
`show` reads docs/attempts.jsonl plus every candidates/*/attempts.jsonl, and greps docs/lane_reports/*.md and
candidates/*/REPORT.md. candidates/ is not in git: `merge` is what makes the knowledge survive.
"""
import datetime
import glob
import json
import shutil
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import ROOT  # noqa: E402

MAIN = ROOT / "docs" / "attempts.jsonl"
REPORTS = ROOT / "docs" / "lane_reports"
VERDICTS = ("open", "invariant", "dead-end", "fixed")


def opt(argv, name, default=None):
    return argv[argv.index(name) + 1] if name in argv else default


def load():
    rows = []
    files = [MAIN] + [Path(p) for p in sorted(glob.glob(str(ROOT / "candidates" / "*" / "attempts.jsonl")))]
    seen = set()
    for f in files:
        if not f.exists():
            continue
        for line in f.read_text(encoding="utf-8", errors="replace").splitlines():
            try:
                r = json.loads(line)
            except ValueError:
                continue
            key = json.dumps(r, sort_keys=True)
            if key not in seen:
                seen.add(key)
                rows.append(r)
    return rows


def report_lines(key):
    out = []
    files = {Path(p).stem: Path(p) for p in glob.glob(str(REPORTS / "*.md"))}
    for p in glob.glob(str(ROOT / "candidates" / "*" / "REPORT.md")):
        files[Path(p).parent.name] = Path(p)          # the live report wins over the archived copy
    for lane, p in sorted(files.items()):
        for ln in p.read_text(encoding="utf-8", errors="replace").split("\n"):
            if any(k and k in ln.lower() for k in key):
                out.append((lane, ln.strip()))
    return out


def main(argv):
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(errors="replace")
    cmd = argv[1] if len(argv) > 1 else ""
    if cmd == "add":
        lane, va = opt(argv, "--lane"), (opt(argv, "--va") or "").lower().removeprefix("0x").zfill(8)
        hyp, res = opt(argv, "--hyp"), opt(argv, "--result")
        verdict = opt(argv, "--verdict", "open")
        if not (lane and hyp and res and len(va) == 8) or verdict not in VERDICTS:
            print(__doc__)
            return 2
        row = {"va": va, "label": opt(argv, "--label", ""), "lane": lane, "date": datetime.date.today().isoformat(),
               "hyp": hyp, "result": res, "verdict": verdict}
        for k in ("--before", "--after"):
            if k in argv:
                row[k[2:]] = int(opt(argv, k))
        f = ROOT / "candidates" / lane / "attempts.jsonl"
        f.parent.mkdir(parents=True, exist_ok=True)
        with open(f, "a", encoding="utf-8", newline="\n") as fh:
            fh.write(json.dumps(row, ensure_ascii=False) + "\n")
        print("recorded")
        return 0
    if cmd == "show" and len(argv) > 2:
        q = argv[2].lower().removeprefix("0x")
        rows = [r for r in load() if q in (r["va"], r.get("label", "").lower())]
        idx = ROOT / "build" / "parked_index.tsv"
        label, va = q, q
        if idx.exists():
            for ln in idx.read_text(encoding="utf-8").splitlines():
                c = ln.split("\t")
                if q in (c[1], c[6].lower()):
                    va, label = c[1], c[6].lower()
                    print(f"{c[1]} {c[6]}  {c[0]}  size {c[2]}, {c[3]} bytes differ, class {c[4]}, work {c[7]}")
        print(f"-- {len(rows)} structured attempts")
        for r in rows:
            d = f" ({r['before']} -> {r['after']} B)" if "before" in r and "after" in r else ""
            print(f"  [{r['lane']} {r['date']} {r['verdict']}]{d} {r['hyp']}  =>  {r['result']}")
        hits = report_lines({va, label if len(label) > 5 else ""})
        print(f"-- {len(hits)} report lines")
        for lane, ln in hits[:60]:
            print(f"  [{lane}] {ln[:240]}")
        return 0
    if cmd == "merge":
        rows = load()
        MAIN.write_text("".join(json.dumps(r, ensure_ascii=False, sort_keys=True) + "\n"
                                for r in sorted(rows, key=lambda r: (r["va"], r["date"], r["lane"]))),
                        encoding="utf-8", newline="\n")
        REPORTS.mkdir(parents=True, exist_ok=True)
        n = 0
        for p in glob.glob(str(ROOT / "candidates" / "*" / "REPORT.md")):
            shutil.copyfile(p, REPORTS / f"{Path(p).parent.name}.md")
            n += 1
        print(f"{len(rows)} attempts in docs/attempts.jsonl; {n} lane reports archived in docs/lane_reports/")
        return 0
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv))
