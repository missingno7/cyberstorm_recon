"""Link-order evidence for the image-level proof (diagnostic).

    python tools/linkorder.py [--write]

For every accepted TU (verify's view: fresh compile, no --at, no search) record where the main code segment and each
proven data segment (STRICT or placed BSS: CONST, CONST2, _DATA, _BSS, XI) sit in the image. WLINK lays out segments class by class, and
inside a class in object order, so a single object order should sort every class consistently. The tool sorts TUs
by code base and reports, per segment name, any inversion (an object whose segment comes earlier than a preceding
object's). With --write it stores the order and the bases in build/linkorder/cs11.json (scratch, regenerated).
"""
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import gate  # noqa: E402
from common import BUILD, ROOT, dump_json  # noqa: E402

SEGS = ("CONST", "CONST2", "_DATA", "_BSS", "XI")


def main(argv):
    ledger = gate.load_ledger()
    rows = []
    for tu, ent in sorted(ledger["tus"].items()):
        try:
            reports, mods, _ = gate.build_and_compare(ROOT / tu, ent)
        except RuntimeError:
            continue
        comdat = {s.name for m in mods for s in m.segments if s and getattr(s, "comdat", None) is not None}
        row = {"tu": tu}
        for r in reports:
            for s in r["segments"]:
                if s["seg"] in comdat:
                    continue
                if s["class"].upper() == "CODE" and "code" not in row:
                    vas = [int(e["va"], 16) - e["off"] for e in s["extents"] if e.get("va")]
                    if vas:
                        row["code"] = min(vas)
                        row["code_size"] = s["size"]
                elif s["seg"] in SEGS and s.get("base") and s["status"] in ("STRICT", "BSS"):
                    row[s["seg"]] = int(s["base"], 16)
                    row[s["seg"] + "_size"] = s["size"]
        if "code" in row:
            rows.append(row)
    rows.sort(key=lambda r: r["code"])
    print(f"{len(rows)} TUs with placed code")
    for seg in SEGS:
        seq = [(r["code"], r[seg], r["tu"]) for r in rows if seg in r]
        inv = [(a, b) for a, b in zip(seq, seq[1:]) if b[1] < a[1]]
        print(f"{seg:7s} placed in {len(seq):3d} TUs; order inversions vs code order: {len(inv)}")
        for a, b in inv[:8]:
            print(f"    {a[2]} {seg}@{a[1]:08x} > {b[2]} {seg}@{b[1]:08x}")
    if "--write" in argv:
        dump_json(BUILD / "linkorder" / "cs11.json",
                  [{k: (f"{v:08x}" if isinstance(v, int) and not k.endswith("_size") else v) for k, v in r.items()}
                   for r in rows])
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
