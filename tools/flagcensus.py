"""Option census over all accepted TUs (diagnostic, never proof): does any single compiler option, added to or
swapped into a TU's recorded flags, make MORE of its functions STRICT without losing accepted ones?

    python tools/flagcensus.py [--tu src/DIR/FILE.c] [--variants=-oa,-oi,...]

For every C/C++ TU: baseline = recorded flags (verify's view, no --at, no search). Each variant is compiled and
compared the same way. Printed per TU only when a variant changes the count: gained (STRICT now, not in the
ledger), lost (ledger functions no longer STRICT). A summary per variant follows. Record the outcome in
docs/codegen_rules.md whatever it is.
"""
import collections
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import gate  # noqa: E402
from common import ROOT  # noqa: E402

ADD = ["-oa", "-ob", "-oc", "-oe", "-oh", "-oi", "-ok", "-ol", "-ol+", "-om", "-on", "-oo", "-op", "-or", "-os", "-ot",
       "-zp1", "-zp4", "-zp8", "-ei", "-ri", "-of", "-of+", "-3s", "-5s", "-6s", "-d1", "-d2", "-fp5", "-fpi87",
       "-zc", "-zm", "-s-", "-j", "-ox", "-oneatx", "-otexan", "-xs", "-xd", "-zq"]
SWAP = {"-oxt": ["-ot", "-ox", "-oxs", "-os", "-oneatx", "-otexan", "-oaxt", "-ol+xt", ""],
        "-oe": ["", "-oe=10", "-oe=40", "-oe=80", "-oe=200"],
        "-bm": ["", "-bt=nt", "-bd"],
        "-4s": ["-4r", "-5r"]}


def strict_set(src, entry):
    reports, _, _ = gate.build_and_compare(src, entry)
    ext = gate.extents(reports)
    by = gate.by_label(ext)
    return {n: e["va"] for n, e in by.items() if e["status"] == "STRICT" and e["cls"].upper() == "CODE" and e.get("va")}


def main(argv):
    only = argv[argv.index("--tu") + 1].replace("\\", "/") if "--tu" in argv else None
    add = next((a.split("=", 1)[1].split(",") for a in argv if a.startswith("--variants=")), ADD)
    ledger = gate.load_ledger()
    total = collections.defaultdict(lambda: [0, 0, 0])      # variant -> [gained, lost, TUs improved without loss]
    for tu, ent in sorted(ledger["tus"].items()):
        if (only and tu != only) or str(ent.get("profile", "")).startswith("asm"):
            continue
        inst, tool, flags = gate.flags_for(ent)
        base = dict(ent, flags=list(flags), tool=tool, install=inst)
        base.pop("profile", None)
        want = {n: f["va"] for n, f in ent["functions"].items()}
        try:
            have = strict_set(ROOT / tu, base)
        except RuntimeError:
            print(f"{tu}: baseline does not compile")
            continue
        variants = [(f"+{v}", list(flags) + [v]) for v in add if v not in flags]
        if "--variants=" not in " ".join(argv):
            for old, news in SWAP.items():
                if old in flags:
                    for new in news:
                        fl = [new if f == old else f for f in flags]
                        variants.append((f"{old}=>{new or '(none)'}", [f for f in fl if f]))
        for name, fl in variants:
            try:
                got = strict_set(ROOT / tu, dict(base, flags=fl))
            except RuntimeError:
                continue                # option not accepted by this compiler/tool
            gained = sorted(n for n, va in got.items() if n not in have and "#" not in n)
            lost = sorted(n for n in want if n not in got)
            if gained or lost:
                total[name][0] += len(gained)
                total[name][1] += len(lost)
                if gained and not lost:
                    total[name][2] += 1
                if gained:
                    print(f"{tu} [{' '.join(flags)}] {name}: +{len(gained)} -{len(lost)}  gained: {', '.join(gained[:8])}")
        sys.stdout.flush()
    print("\nvariant: functions gained / lost over all TUs / TUs improved without loss")
    for name, (g, l, t) in sorted(total.items(), key=lambda kv: (-kv[1][2], -kv[1][0])):
        print(f"  {name:16s} +{g:4d} -{l:5d}   {t} TUs")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
