"""Shared build-and-compare step for promote.py and verify.py (fresh compile, no cached objects)."""
import re
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cc  # noqa: E402
import compare  # noqa: E402
import omf  # noqa: E402
from common import CONFIG, ROOT, load_json  # noqa: E402

LEDGER = ROOT / "recovery.json"

BANNED = [
    (re.compile(r"\b_?_asm\b|\basm\s*\("), "inline assembly"),
    (re.compile(r"__emit__|_emit\b"), "byte emission"),
    (re.compile(r"^\s*#\s*line\b", re.M), "#line directive"),
    (re.compile(r"#\s*pragma\s+aux\b[^\n]*=", re.M), "#pragma aux code bytes"),
]


def policy(path):
    text = Path(path).read_text(encoding="latin-1")
    text_nc = re.sub(r"/\*.*?\*/|//[^\n]*", "", text, flags=re.S)
    return [f"{why}" for rx, why in BANNED if rx.search(text_nc)]


def load_ledger():
    if LEDGER.exists():
        return load_json(LEDGER)
    return {"note": "Sole acceptance ledger; written only by tools/promote.py --publish.", "oracle": "cs11", "tus": {}}


def flags_for(entry):
    if entry.get("flags") is not None:
        return entry.get("install", "wc106"), entry.get("tool", "wpp386"), entry["flags"]
    p = load_json(CONFIG / "toolchain.json")["profiles"][entry["profile"]]
    return p["install"], p["tool"], p["flags"]


def build_and_compare(src, entry, oracle=None, at=None, find=False, imply=False):
    """Compile `src` with the TU entry's toolchain, compare; returns (reports, compiler_text) or raises."""
    inst, tool, flags = flags_for(entry)
    with tempfile.TemporaryDirectory(prefix="g", dir=ROOT / "build") as td:
        obj = Path(td) / "unit.obj"
        rc, text = cc.compile_one(src, obj, flags, inst, tool, as_name=entry.get("as"))
        if rc:
            raise RuntimeError(f"compile failed for {src}:\n{text}")
        reports, img = compare.compare_object(obj, oracle, at, find, imply)
        mods = omf.load(obj)
    return reports, mods, text


def extents(reports):
    """{object symbol: extent} and {label: extent} over all extents (every public name of each extent)."""
    import symbols
    out = {}
    for r in reports:
        for s in r["segments"]:
            for e in s.get("extents", []):
                for n in e["names"]:
                    out[n] = dict(e, seg=s["seg"], cls=s["class"])
    return out


def by_label(ext):
    import symbols
    return {symbols.label(n): e for n, e in ext.items()}
