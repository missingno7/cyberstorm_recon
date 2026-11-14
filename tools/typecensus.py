"""Record-type census: how many different definitions does each struct/class name have across src/? Diagnostic.

    python tools/typecensus.py [NAME] [--min-files 3]

Every file redeclares the records it uses, so one original type exists in several shapes (opaque byte arrays,
partial field lists, different member names). A function can only match once its file models the fields it touches;
a type with many shapes is where one careful shared definition pays in many files. With NAME: print each distinct
definition and the files using it. Shared TYPES are safe to unify; shared PROTOTYPES are not (prototypes differed
between original objects, docs/watcom_playbook.md).
"""
import collections
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import ROOT  # noqa: E402

HEAD = re.compile(r"\b(struct|class)\s+([A-Za-z_]\w*)\s*(?::[^{;]*)?\{")


def definitions(text):
    text = re.sub(r"/\*.*?\*/|//[^\n]*", "", text, flags=re.S)
    for m in HEAD.finditer(text):
        depth, k = 0, m.end() - 1
        while k < len(text):
            if text[k] == "{":
                depth += 1
            elif text[k] == "}":
                depth -= 1
                if depth == 0:
                    break
            k += 1
        yield m.group(2), re.sub(r"\s+", " ", text[m.start():k + 1]).strip()


def main(argv):
    args = [a for a in argv[1:] if not a.startswith("--")]
    minf = int(argv[argv.index("--min-files") + 1]) if "--min-files" in argv else 3
    by = collections.defaultdict(lambda: collections.defaultdict(list))
    for p in sorted((ROOT / "src").rglob("*.c")):
        for name, body in definitions(p.read_text(encoding="latin-1")):
            by[name][body].append(p.relative_to(ROOT).as_posix())
    if args:
        for body, files in sorted(by[args[0]].items(), key=lambda kv: -len(kv[1])):
            print(f"--- {len(files)} files: {', '.join(files[:8])}{' ...' if len(files) > 8 else ''}")
            print("    " + body[:600])
        return 0
    rows = [(len({f for fs in d.values() for f in fs}), len(d), n) for n, d in by.items()]
    rows = sorted((r for r in rows if r[0] >= minf and r[1] > 1), key=lambda r: (-r[1], -r[0]))
    print("files  shapes  type")
    for nf, ns, n in rows[:60]:
        print(f"{nf:5d}  {ns:6d}  {n}")
    print(f"\n{len(by)} record names; {sum(1 for r in rows)} are defined in >= {minf} files with more than one shape")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
