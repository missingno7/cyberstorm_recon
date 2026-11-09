"""Test many author-written source alternatives in one command (a manual permuter; diagnostic only).

    python tools/variants.py TEMPLATE.c --as 'dir\\file.c' --profile P [--at Label=VA ...] [--fn SUBSTR ...]
                             [--max 256] [--jobs 6] [--top 8] [--write BEST.c]

TEMPLATE is a complete candidate TU with choice points:
  inline  `@{alt1@|alt2@|...@}` inside one line, e.g. `if (@{i > 9@|i >= 10@}) break;`
  block   a line `//@{`, alternatives separated by lines `//@|`, closed by `//@}` (no nesting)
Every combination is compiled (pinned toolchain) and compared (--find --imply). If there are more than --max
combinations, coordinate descent from the best so far is used (each pass varies one choice point). Ranking:
more STRICT extents among the selected functions (--fn, default all), then fewer differing bytes.
Alternatives must be natural spellings the original could have used: no dummy declarations, register steering
or copies that exist only to move bytes. The winner is still proven only by promote.py.
"""
import itertools
import random
import re
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cc  # noqa: E402
import compare  # noqa: E402
import symbols  # noqa: E402
from common import CONFIG, ROOT, load_json  # noqa: E402

INLINE = re.compile(r"@\{(.*?)@\}")


def parse(text):
    """-> (pieces, choices): pieces is a list of str or int (choice index); choices[i] = list of alternatives."""
    pieces, choices = [], []
    lines = text.split("\n")
    i = 0
    while i < len(lines):
        line = lines[i]
        if line.strip().startswith("//@{"):
            alts, cur = [], []
            i += 1
            while i < len(lines) and not lines[i].strip().startswith("//@}"):
                if lines[i].strip().startswith("//@|"):
                    alts.append("\n".join(cur))
                    cur = []
                else:
                    cur.append(lines[i])
                i += 1
            alts.append("\n".join(cur))
            choices.append(alts)
            pieces.append(len(choices) - 1)
            pieces.append("\n")
            i += 1
            continue
        pos = 0
        for m in INLINE.finditer(line):
            pieces.append(line[pos:m.start()])
            choices.append(m.group(1).split("@|"))
            pieces.append(len(choices) - 1)
            pos = m.end()
        pieces.append(line[pos:] + "\n")
        i += 1
    return pieces, choices


def render(pieces, choices, vec):
    return "".join(p if isinstance(p, str) else choices[p][vec[p]] for p in pieces)


def score(text, prof, as_name, at, fns, name):
    with tempfile.TemporaryDirectory(prefix="var", dir=ROOT / "build") as td:
        src = Path(td) / name
        src.write_text(text, encoding="latin-1", newline="\n")
        obj = Path(td) / "v.obj"
        rc, _ = cc.compile_one(src, obj, prof["flags"], prof["install"], prof["tool"], as_name=as_name)
        if rc:
            return (-1, 10 ** 9, "compile error")
        reps, _ = compare.compare_object(obj, None, at, True, True)
    strict, diff, det = 0, 0, []
    for r in reps:
        for s in r["segments"]:
            if s["class"].upper() != "CODE":
                continue
            for e in s["extents"]:
                lab = symbols.label(e["names"][0])
                if fns and not any(f in lab for f in fns):
                    continue
                strict += e["status"] == "STRICT"
                d = e.get("diff_bytes") or (0 if e["status"] in ("STRICT", "UNBOUND") else e["end"] - e["off"])
                diff += d
                det.append(f"{lab}:{e['status'][0]}{d if d else ''}")
    return (strict, diff, " ".join(det))


def main(argv):
    argv = list(argv[1:])
    opts = {"at": {}, "fn": [], "max": "256", "jobs": "6", "top": "8"}
    for key in ("--as", "--profile", "--at", "--fn", "--max", "--jobs", "--top", "--write"):
        while key in argv:
            i = argv.index(key)
            v = argv[i + 1]
            del argv[i:i + 2]
            if key == "--at":
                k, va = v.split("=")
                opts["at"][k] = int(va, 16)
            elif key == "--fn":
                opts["fn"].append(v)
            else:
                opts[key[2:]] = v
    if len(argv) != 1 or "profile" not in opts:
        print(__doc__)
        return 2
    tpl = Path(argv[0])
    pieces, choices = parse(tpl.read_text(encoding="latin-1").replace("\r\n", "\n"))
    if not choices:
        print("no choice points in template")
        return 2
    prof = load_json(CONFIG / "toolchain.json")["profiles"][opts["profile"]]
    total = 1
    for c in choices:
        total *= len(c)
    budget, jobs = int(opts["max"]), int(opts["jobs"])
    seen = {}

    def run(vecs):
        vecs = [v for v in vecs if v not in seen]
        with ThreadPoolExecutor(jobs) as ex:
            res = list(ex.map(lambda v: score(render(pieces, choices, v), prof, opts.get("as"), opts["at"],
                                              opts["fn"], tpl.name), vecs))
        for v, r in zip(vecs, res):
            seen[v] = r

    def best():
        return max(seen, key=lambda v: (seen[v][0], -seen[v][1]))

    if total <= budget:
        run(list(itertools.product(*[range(len(c)) for c in choices])))
    else:
        rng = random.Random(1)
        cur = tuple(0 for _ in choices)
        run([cur])
        while len(seen) < budget:
            improved = False
            for i, c in enumerate(choices):
                cur = best()
                run([cur[:i] + (k,) + cur[i + 1:] for k in range(len(c))])
                if best() != cur:
                    improved = True
                if len(seen) >= budget:
                    break
            if not improved:
                run([tuple(rng.randrange(len(c)) for c in choices)])
    ranked = sorted(seen.items(), key=lambda kv: (-kv[1][0], kv[1][1]))
    print(f"{len(choices)} choice points, {total} combinations, {len(seen)} compiled")
    for v, (st, d, det) in ranked[:int(opts["top"])]:
        print(f"strict {st:3d} diff {d:5d}  choices {list(v)}  {det[:160]}")
    if opts.get("write"):
        Path(opts["write"]).write_text(render(pieces, choices, ranked[0][0]), encoding="latin-1", newline="\n")
        print(f"best written to {opts['write']}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
