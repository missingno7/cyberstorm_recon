"""Fresh-build verification of every accepted TU, plus progress counts. The only source of progress numbers.

    python tools/verify.py [--secondary]    (--secondary: also locate each TU in cs10, informational only)

Checks: oracle locks; each ledger TU recompiled from src/ with its pinned toolchain; every ledger function
STRICT at its recorded VA; symbols.json 'matched' entries agree with the ledger. Exit 1 on any failure.
"""
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import gate  # noqa: E402
import pe as pemod  # noqa: E402
import symbols  # noqa: E402
from common import BUILD, CONFIG, ROOT, load_json  # noqa: E402


def main(argv):
    ok = True
    for o in ("cs11", "cs10"):
        want = load_json(CONFIG / "locks" / f"{o}.json")
        got = pemod.lock_record(pemod.load(o))
        if want != got:
            print(f"LOCK MISMATCH {o}")
            ok = False
    ledger = gate.load_ledger()
    syms = symbols.load()["symbols"]
    nfun = nbytes = 0
    sec = {"funcs": 0, "strict": 0}
    for tu, ent in sorted(ledger["tus"].items()):
        src = ROOT / tu
        try:
            reports, _, _ = gate.build_and_compare(src, ent)
        except RuntimeError as e:
            print(f"FAIL {tu}: {e}")
            ok = False
            continue
        ext = gate.by_label(gate.extents(reports))
        bad = [n for n, e in ent["functions"].items()
               if (ext.get(n) or {}).get("status") != "STRICT" or ext[n]["va"] != e["va"]
               or ext[n]["end"] - ext[n]["off"] != e["size"]]
        for n, e in ent["functions"].items():
            x = ext.get(n) or {}
            s = syms.get(f"{n}@{x.get('scope')}") if x.get("local") else syms.get(n)
            if not s or s["status"] != "matched" or s["va"] != e["va"]:
                bad.append(f"{n} (symbols.json)")
        bad += gate.data_losses(reports, ent.get("data"))      # recorded data must stay STRICT (audit v1 F2)
        if bad:
            print(f"FAIL {tu}: {bad}")
            ok = False
        nfun += len(ent["functions"])
        nbytes += sum(e["size"] for e in ent["functions"].values())
        if "--secondary" in argv:
            try:
                r2, _, _ = gate.build_and_compare(src, ent, oracle="cs10", find=True, imply=True)
                e2 = gate.by_label(gate.extents(r2))
                sec["funcs"] += len(ent["functions"])
                sec["strict"] += sum(1 for n in ent["functions"] if (e2.get(n) or {}).get("status") == "STRICT")
            except RuntimeError:
                pass
    fm = json.loads((BUILD / "funcmap" / "cs11.json").read_text()) if (BUILD / "funcmap" / "cs11.json").exists() else None
    if fm:
        game = [f for f in fm["functions"] if f["kind"] != "lib"]
        gbytes = sum(f["size"] for f in game)
        print(f"strict: {nfun} functions / ~{len(game)} game functions; {nbytes} / ~{gbytes} bytes "
              f"({100 * nbytes / gbytes:.2f}%) in {len(ledger['tus'])} TUs")
    else:
        print(f"strict: {nfun} functions, {nbytes} bytes in {len(ledger['tus'])} TUs")
    if "--secondary" in argv:
        print(f"secondary cs10 (informational): {sec['strict']}/{sec['funcs']} functions strict when located")
    print("VERIFY OK" if ok else "VERIFY FAILED")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv))
