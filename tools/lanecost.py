"""Token usage and list-price cost of worker lanes (diagnostic; prices are parameters, not facts of this repo).

    python tools/lanecost.py --codex NAME [...] --claude LABEL=TRANSCRIPT.jsonl [...]

Codex lanes: cx run records (~/.codex-dashboard/runs) joined with the session rollout's last token_count.
Claude subagents: sum of the `usage` of every assistant message in the subagent transcript (deduplicated by message
id). Prices in USD per million tokens, defaults: gpt-6.1-sol 2.00 in / 0.10 cached / 10.00 out (third-party price
lists, 2026-11); claude-fable-5-2 8.00 in / 0.20 cache read / 40.00 out, cache writes 1.25x input (5 min) and 2x
(1 h). Override with --price MODEL=in,cached,out.
"""
import glob
import json
import sys
from pathlib import Path

PRICE = {"gpt-6.1-sol": (2.0, 0.10, 10.0), "gpt-6-luna": (0.1, 0.005, 0.5), "claude-fable-5-2": (8.0, 0.20, 40.0)}


def codex(names):
    home = Path.home()
    runs = {}
    for f in glob.glob(str(home / ".codex-dashboard" / "runs" / "*.json")):
        try:
            d = json.load(open(f))
        except Exception:
            continue
        if d.get("name") in names:
            runs[Path(f).stem] = d
    out = []
    for f in glob.glob(str(home / ".codex" / "sessions" / "**" / "rollout-*.jsonl"), recursive=True):
        sid = next((s for s in runs if s in f), None)
        if not sid:
            continue
        last, model, t0, t1 = None, None, None, None
        for line in open(f, encoding="utf-8", errors="replace"):
            try:
                e = json.loads(line)
            except Exception:
                continue
            t0, t1 = t0 or e.get("timestamp"), e.get("timestamp") or t1
            p = e.get("payload") or {}
            if e.get("type") == "turn_context":
                model = p.get("model", model)
            if p.get("type") == "token_count" and p.get("info"):
                last = p["info"].get("total_token_usage") or last
        if last:
            cached = last.get("cached_input_tokens", 0)
            out.append((runs[sid]["started"], runs[sid]["name"], model, last["input_tokens"] - cached, cached, 0,
                        last["output_tokens"], t0, t1))
    return [r[1:] for r in sorted(out)]


def claude(label, path):
    seen = {}
    model, t0, t1 = None, None, None
    for line in open(path, encoding="utf-8", errors="replace"):
        try:
            e = json.loads(line)
        except Exception:
            continue
        t0, t1 = t0 or e.get("timestamp"), e.get("timestamp") or t1
        m = e.get("message") or {}
        u = m.get("usage")
        if e.get("type") == "assistant" and u:
            seen[m.get("id") or len(seen)] = u
            model = m.get("model", model)
    inp = sum(u.get("input_tokens", 0) for u in seen.values())
    read = sum(u.get("cache_read_input_tokens", 0) for u in seen.values())
    write = sum(u.get("cache_creation_input_tokens", 0) for u in seen.values())
    w1h = sum((u.get("cache_creation") or {}).get("ephemeral_1h_input_tokens", 0) for u in seen.values())
    outp = sum(u.get("output_tokens", 0) for u in seen.values())
    return (label, model, inp, read, (write, w1h), outp, t0, t1)


def main(argv):
    rows = []
    i = 1
    mode = None
    names = []
    while i < len(argv):
        a = argv[i]
        if a in ("--codex", "--claude", "--price"):
            mode = a
        elif mode == "--codex":
            names.append(a)
        elif mode == "--claude":
            label, path = a.split("=", 1)
            rows.append(claude(label, path))
        elif mode == "--price":
            k, v = a.split("=")
            PRICE[k] = tuple(float(x) for x in v.split(","))
        i += 1
    rows = codex(set(names)) + rows
    print(f"{'lane':10s} {'model':18s} {'uncached in':>12s} {'cached in':>12s} {'cache write':>12s} {'output':>9s} {'minutes':>8s} {'USD':>8s}")
    for label, model, inp, read, write, outp, t0, t1 in rows:
        key = next((k for k in PRICE if model and model.startswith(k)), None)
        pin, pc, pout = PRICE.get(key, (0, 0, 0))
        if isinstance(write, tuple):
            w, w1h = write
            wcost = ((w - w1h) * 1.25 + w1h * 2.0) * pin
        else:
            w, wcost = write, write * pin
        usd = (inp * pin + read * pc + wcost + outp * pout) / 1e6
        mins = ""
        if t0 and t1:
            import datetime
            f = lambda s: datetime.datetime.fromisoformat(s.replace("Z", "+00:00"))
            mins = f"{(f(t1) - f(t0)).total_seconds() / 60:8.1f}"
        print(f"{label:10s} {str(model):18s} {inp:12d} {read:12d} {w:12d} {outp:9d} {mins:>8s} {usd:8.2f}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
