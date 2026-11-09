"""Acceptance gate for one translation unit. Dry run by default; --publish (integrator only) writes.

    python tools/promote.py CANDIDATE.c --tu src/DIR/FILE.c [--as DIR\\FILE.c] [--profile P | --flags "-4s -s -oxt"]
                            [--at LABEL=VA ...] [--accept-loss LABEL ...] [--publish]

Steps: source policy check -> fresh compile with the TU's pinned toolchain -> strict comparison (placement from
bound symbols, --at, or a unique masked search) -> no previously accepted function may lose STRICT (unless named
with --accept-loss, logged) -> on --publish: copy the candidate to --tu, record every STRICT code extent in
recovery.json and mark its symbol 'matched' in config/symbols.json. Data segments are recorded when STRICT.
"""
import datetime
import shutil
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import gate  # noqa: E402
import symbols  # noqa: E402
from common import ROOT, dump_json  # noqa: E402


def img_section(va_hex):
    import pe as pemod
    img = pemod.load(None)
    sec = img.section_at_rva(int(va_hex, 16) - img.image_base)
    return sec.name if sec else None


def main(argv):
    argv = list(argv[1:])
    opts = {"at": {}, "loss": []}
    publish = "--publish" in argv
    if publish:
        argv.remove("--publish")
    while argv and argv[0] != "--" and any(a.startswith("--") for a in argv):
        i = next(k for k, a in enumerate(argv) if a.startswith("--"))
        key, val = argv[i], argv[i + 1]
        del argv[i:i + 2]
        if key == "--at":
            k, v = val.split("=")
            opts["at"][k] = int(v, 16)
        elif key == "--accept-loss":
            opts["loss"].append(val)
        else:
            opts[key[2:]] = val
    if len(argv) != 1 or "tu" not in opts:
        print(__doc__)
        return 2
    orig_cand = Path(argv[0]).resolve()
    # canonical form = LF line endings (what git stores); compile and publish exactly that
    import tempfile
    (ROOT / "build").mkdir(exist_ok=True)
    norm_dir = Path(tempfile.mkdtemp(prefix="promote", dir=ROOT / "build"))  # private: lanes run in parallel
    cand = norm_dir / orig_cand.name
    cand.write_bytes(orig_cand.read_bytes().replace(b"\r\n", b"\n"))
    tu = opts["tu"].replace("\\", "/")
    ledger = gate.load_ledger()
    old = ledger["tus"].get(tu, {})
    entry = {"as": opts.get("as", old.get("as")), "install": "wc106"}
    if "flags" in opts:
        entry["flags"] = opts["flags"].split()
        entry["tool"] = opts.get("tool", old.get("tool", "wpp386"))
    else:
        entry["profile"] = opts.get("profile", old.get("profile", "ot"))
        entry["flags"] = None
    if not entry["as"]:
        entry["as"] = Path(tu).name
    bad = gate.policy(cand)
    if bad:
        print("POLICY FAIL:", ", ".join(bad))
        return 1
    try:
        reports, mods, _ = gate.build_and_compare(cand, entry, at=opts["at"], find=True, imply=True)
    except RuntimeError as e:
        print(e)
        return 1
    conflicts = [c for r in reports for c in r["conflicts"]]
    if conflicts:
        print("CONFLICTS:", conflicts)
        return 1
    ext = gate.extents(reports)
    strict = {n: e for n, e in ext.items() if e["status"] == "STRICT" and e["cls"].upper() == "CODE"}
    lab_ext = gate.by_label(ext)
    lost = []
    for n, e in old.get("functions", {}).items():
        now = lab_ext.get(n)
        if (not now or now["status"] != "STRICT" or now["va"] != e["va"]
                or now["end"] - now["off"] != e["size"]):       # audit v1 F5: size is part of the claim
            lost.append(n)
    lost += gate.data_losses(reports, old.get("data"))
    if lost and set(lost) - set(opts["loss"]):
        print("LOSS (not accepted):", sorted(set(lost) - set(opts["loss"])))
        return 1
    # label collisions inside the candidate
    labels = {}
    for n in strict:
        labels.setdefault(symbols.label(n), []).append(n)
    dup = {k: v for k, v in labels.items() if len(v) > 1}
    if dup:
        print("label collision (overloads need distinct labels):", dup)
        return 1
    db = symbols.load()
    syms = db["symbols"]
    scope = symbols.scope_of_as(entry["as"])

    def skey(n, e):
        """symbols.json key: TU-scoped for file-local symbols (statics, generated `W?.fn_init`); overloads whose
        readable label is already taken by a different mangled name get `label#mangled` (the resolver finds
        them by their exact mangled name first)."""
        k = symbols.key_for(n, e.get("local"), scope)
        cur = syms.get(k)
        if cur and cur.get("mangled") and cur["mangled"] != n and cur["va"] != e.get("va"):
            return f"{k}#{n}"
        return k
    for n, e in strict.items():
        lab = skey(n, e)
        cur = syms.get(lab)
        if cur and cur["status"] == "matched" and (cur["va"] != e["va"] or cur.get("tu") != tu):
            print(f"symbol {lab} already matched at {cur['va']} in {cur.get('tu')}; refusing")
            return 1
        clash = [k for k, v in syms.items() if v["va"] == e["va"] and v["status"] == "matched" and k != lab]
        if clash:
            print(f"{e['va']} already matched as {clash}; refusing")
            return 1
    implied = {}
    for r in reports:
        implied.update(r.get("implied_bindings", {}))
        for k, why in r.get("implied_rejected", []):
            print(f"  implied binding rejected: {symbols.label(k)}: {why}")
    for k, v in implied.items():
        lab = symbols.label(k)
        cur = syms.get(lab)
        if cur and (cur["va"] != v or (cur.get("mangled") and cur["mangled"] != k)):
            print(f"implied binding {lab}={v} contradicts symbols.json {cur}; refusing")
            return 1
        other = [n for n, e in syms.items() if e["va"] == v and n != lab]
        print(f"  new hypothesis binding: {lab} = {v}" + (f"  ALIAS of {other}: use that name" if other else ""))
    gain = sorted({symbols.label(n) for n in strict} - set(old.get("functions", {})))
    sbytes = sum(e["end"] - e["off"] for e in strict.values())
    data_strict = [s for r in reports for s in r["segments"]
                   if s["class"].upper() != "CODE" and s["status"] == "STRICT"]
    for r in reports:
        for s in r["segments"]:
            print(f"  [{s['status']:8s}] {s['seg']:<12s} {s.get('base', '')}")
            for e in s.get("extents", []):
                if e["status"] != "STRICT":
                    print(f"      {e['status']:8s} {e['va']} {symbols.label(e['names'][0])}")
    print(f"{tu}: {len(strict)} strict functions ({sbytes} bytes), new {len(gain)}, lost {len(lost)}"
          f"{' (accepted: ' + ', '.join(lost) + ')' if lost else ''}")
    if not publish:
        print("dry run OK (add --publish to accept)")
        return 0
    dest = ROOT / tu
    dest.parent.mkdir(parents=True, exist_ok=True)
    # snapshot for rollback if the post-publish check (verify's view: no --at, no search) fails
    backup = {p: (p.read_bytes() if p.exists() else None) for p in (dest, gate.LEDGER, symbols.PATH)}
    shutil.copyfile(cand, dest)
    now = datetime.date.today().isoformat()
    rec = {k: v for k, v in entry.items() if v is not None}
    rec["functions"] = {symbols.label(n): {"va": e["va"], "size": e["end"] - e["off"], "mangled": n}
                        for n, e in sorted(strict.items(), key=lambda kv: kv[1]["va"])}
    rec["data"] = {s["seg"]: {"va": s["base"], "size": s["size"]} for s in data_strict}
    rec["accepted"] = now
    ledger["tus"][tu] = rec
    if lost:
        ledger.setdefault("accepted_losses", []).append({"tu": tu, "functions": lost, "date": now})
    for n, e in strict.items():
        syms[skey(n, e)] = {"va": e["va"], "kind": "func", "status": "matched", "mangled": n, "tu": tu,
                                  "evidence": f"strict match {now}"}
    for k, v in implied.items():
        lab = symbols.label(k)
        if lab not in syms:
            isf = symbols.is_function(k)
            if isf is None:     # C linkage: kind from the target section
                isf = img_section(v) == "BEGTEXT"
            syms[lab] = {"va": v, "kind": "func" if isf else "data",
                         "status": "hypothesis", "mangled": k, "evidence": f"implied by {tu} {now}"}
    # this TU's own non-strict functions placed by --at or unique search: keep the placement as a hypothesis
    # binding, so references to them resolve without --at (as in verify)
    # the owning TU's name wins; other TUs' hypothesis names for the same VA stay as aliases until a naming
    # cleanup. Only a *matched* function at that VA under another name blocks the placement.
    taken = {e["va"] for e in syms.values() if e["status"] == "matched"}
    for n, e in ext.items():
        if e["cls"].upper() != "CODE" or e["status"] == "STRICT" or not e.get("va"):
            continue
        lab = skey(n, e)
        if e.get("placed") in ("at", "found") and lab not in syms and e["va"] not in taken:
            syms[lab] = {"va": e["va"], "kind": "func", "status": "hypothesis", "mangled": n, "tu": tu,
                         "evidence": f"placed ({e['placed']}) in {tu} {now}"}
            taken.add(e["va"])
    for n in lost:
        lab = n if n in syms else next((k for k in syms if k == f"{n}@{scope}"), n)
        if lab in syms and syms[lab].get("tu") == tu:
            syms[lab]["status"] = "hypothesis"
            syms[lab]["evidence"] = f"lost strict {now}"
    symbols.save(db)
    dump_json(gate.LEDGER, ledger)
    # post-publish check exactly as verify sees it
    reports2, _, _ = gate.build_and_compare(dest, rec)
    ext2 = gate.by_label(gate.extents(reports2))
    bad = [n for n, e in rec["functions"].items()
           if (ext2.get(n) or {}).get("status") != "STRICT" or ext2[n]["va"] != e["va"]
           or ext2[n]["end"] - ext2[n]["off"] != e["size"]]
    bad += gate.data_losses(reports2, rec.get("data"))
    if bad:
        for p, b in backup.items():
            if b is None:
                p.unlink(missing_ok=True)
            else:
                p.write_bytes(b)
        print(f"ROLLED BACK: not strict without --at/search: {bad}")
        return 1
    print("published")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
