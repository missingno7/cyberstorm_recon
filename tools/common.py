"""Shared paths and small helpers (stdlib only)."""
import hashlib
import json
import os
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ASSETS = ROOT / "assets"
BUILD = ROOT / "build"
CONFIG = ROOT / "config"
SRC = ROOT / "src"
TOOLS_DIR = Path(os.environ.get("CSTORM_TOOLS", "C:/tools"))


def sha256_bytes(data):
    return hashlib.sha256(data).hexdigest()


def sha256_file(path, chunk=1 << 20):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        while True:
            b = f.read(chunk)
            if not b:
                break
            h.update(b)
    return h.hexdigest()


def load_json(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


def dump_json(path, obj):
    """Atomic JSON write (per-process temp file + replace)."""
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(path.suffix + f".{os.getpid()}.tmp")
    try:
        with open(tmp, "w", encoding="utf-8", newline="\n") as f:
            json.dump(obj, f, indent=1)
            f.write("\n")
        for attempt in range(20):
            try:
                os.replace(tmp, path)
                return
            except PermissionError:
                if attempt == 19:
                    raise
                time.sleep(0.25)
    finally:
        tmp.unlink(missing_ok=True)


_oracle_cache = {}


def oracle_entry(name):
    cfg = load_json(CONFIG / "oracles.json")
    if name not in cfg["oracles"]:
        raise SystemExit(f"unknown oracle {name!r}; known: {', '.join(cfg['oracles'])}")
    return cfg["oracles"][name]


def oracle_bytes(name):
    """Bytes of a pinned oracle, failing closed on any hash mismatch. Never written to."""
    if name in _oracle_cache:
        return _oracle_cache[name]
    ent = oracle_entry(name)
    data = (ROOT / ent["path"]).read_bytes()
    got = sha256_bytes(data)
    if got != ent["sha256"]:
        raise SystemExit(f"oracle {name}: sha256 {got} != pinned {ent['sha256']} (refusing)")
    _oracle_cache[name] = data
    return data


def default_oracle():
    return load_json(CONFIG / "oracles.json")["primary"]
