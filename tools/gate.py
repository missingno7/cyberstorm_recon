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


ASM_BANNED = [
    (re.compile(r"\bincbin\b|\binclude\s+binary\b", re.I), "binary inclusion"),
    # byte blobs: four or more comma-separated values after db/dw/dd, unless labelled UNKNOWN debt (hand review)
    (re.compile(r"^[ \t]*(?:\w+[ \t]+)?d[bwd][ \t]+[^;\n]*,[^;\n]*,[^;\n]*,[^\n]*$", re.I | re.M), "data blob in assembly"),
]


def policy(path):
    text = Path(path).read_text(encoding="latin-1")
    if str(path).lower().endswith(".asm"):
        # hand assembly is legitimate source only for ranges classified asm (config/nongame.json); no blobs
        lines = [ln for ln in text.splitlines() if "UNKNOWN" not in ln]
        text_nc = "\n".join(re.sub(r";.*", "", ln) for ln in lines)
        return [f"{why}" for rx, why in ASM_BANNED if rx.search(text_nc)]
    text_nc = re.sub(r"/\*.*?\*/|//[^\n]*", "", text, flags=re.S)
    return [f"{why}" for rx, why in BANNED if rx.search(text_nc)]


def twin_helpers(path, min_tokens=14, ratio=0.8):
    """Pairs (inline helper, any other function) in one file whose bodies are the same up to renaming, or nearly
    so. A second spelling of one helper that exists only so that two call sites get different bytes is not source
    the original could have had (one definition there): reviewers must look at every pair.
    Returns [(inline name, other name, ratio)]."""
    import difflib
    text = re.sub(r"/\*.*?\*/|//[^\n]*", "", Path(path).read_text(encoding="latin-1"), flags=re.S)
    funcs = []
    rx = re.compile(r"([A-Za-z_]\w*(?:::~?\w+)?)\s*\(([^;{}()]|\([^()]*\))*\)\s*(?:const\s*)?(?::[^;{}]*)?\{")
    for m in rx.finditer(text):
        if m.group(1) in ("if", "for", "while", "switch", "catch", "return", "sizeof"):
            continue
        depth, k = 0, m.end() - 1
        while k < len(text):
            depth += text[k] == "{"
            depth -= text[k] == "}"
            if depth == 0:
                break
            k += 1
        line0 = text.rfind("\n", 0, m.start()) + 1
        stmt0 = max(text.rfind(";", 0, m.start()), text.rfind("}", 0, m.start()), line0 - 1) + 1
        inline = bool(re.search(r"\binline\b", text[stmt0:m.start()]))
        toks = re.findall(r"[A-Za-z_]\w*|\d+|[^\sA-Za-z_\d]", text[m.end():k])
        names = {}
        norm = [names.setdefault(x, f"i{len(names)}") if re.match(r"[A-Za-z_]", x) else x for x in toks]
        if len(norm) >= min_tokens:
            funcs.append((m.group(1), norm, inline))
    out = []
    for i in range(len(funcs)):
        for k in range(i + 1, len(funcs)):
            a, b = funcs[i], funcs[k]
            if not (a[2] or b[2]) or a[0] == b[0]:
                continue
            r = difflib.SequenceMatcher(None, a[1], b[1], autojunk=False).ratio()
            if r >= ratio:
                out.append((a[0], b[0], round(r, 2)) if a[2] else (b[0], a[0], round(r, 2)))
    return out


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


def data_losses(reports, recorded):
    """Recorded data segments ({seg: {va, size}}) that are no longer STRICT at the same base and size."""
    now = {s["seg"]: s for r in reports for s in r["segments"] if s["class"].upper() != "CODE"}
    bad = []
    for seg, e in (recorded or {}).items():
        x = now.get(seg)
        if not x or x["status"] != "STRICT" or x.get("base") != e["va"] or x["size"] != e["size"]:
            bad.append(f"data:{seg}")
    return bad


def by_label(ext):
    """{label: extent}, plus `label#mangled` for every C++ name (ledger key of overloads sharing a label)."""
    import symbols
    out = {symbols.label(n): e for n, e in ext.items()}
    out.update({f"{symbols.label(n)}#{n}": e for n, e in ext.items() if n.startswith("W?")})
    return out
