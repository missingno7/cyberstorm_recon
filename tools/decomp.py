"""Ghidra pseudo-C drafts per module (a reading aid for workers; never proof, never pasted unchecked).

    python tools/decomp.py --export        (re)run headless Ghidra on a hash-checked copy of cs11 (~minutes)
    python tools/decomp.py VA              print the draft of the function at VA
    python tools/decomp.py --module VA     print the drafts of the whole module containing VA

Functions are seeded from build/funcmap (starts), named from config/symbols.json and library identity, and
grouped by build/modules. Output: build/decomp/modules/<module start>.c
"""
import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import BUILD, CONFIG, ROOT, TOOLS_DIR, oracle_bytes, sha256_file, oracle_entry  # noqa: E402

GHIDRA = TOOLS_DIR / "ghidra_12.1_PUBLIC" / "support" / "analyzeHeadless.bat"
JAVA_HOME = Path(r"C:\Program Files\Eclipse Adoptium\jdk-25.0.4.101-hotspot")
OUT = BUILD / "decomp"


def seeds():
    fm = json.loads((BUILD / "funcmap" / "cs11.json").read_text())["functions"]
    mods = json.loads((BUILD / "modules" / "cs11.json").read_text())
    mod_of = {f: m["start"] for m in mods if m["kind"] == "game" for f in m["funcs"]}
    names = {}
    p = BUILD / "libsyms" / "cs11.json"
    if p.exists():
        for n, e in json.loads(p.read_text()).items():
            names.setdefault(e["va"], n)
    for n, e in json.loads((CONFIG / "symbols.json").read_text())["symbols"].items():
        names[e["va"]] = n
    rows = []
    for f in fm:
        nm = re.sub(r"[^A-Za-z0-9_]", "_", names.get(f["va"], f"f_{f['va']}")).strip("_") or f"f_{f['va']}"
        rows.append((f["va"], str(f["size"]), nm, mod_of.get(f["va"], "")))
    return rows


def export():
    OUT.mkdir(parents=True, exist_ok=True)
    exe = OUT / "CSTORM.EXE"
    data = oracle_bytes("cs11")
    if not exe.exists() or sha256_file(exe) != oracle_entry("cs11")["sha256"]:
        exe.write_bytes(data)
    (OUT / "seeds.tsv").write_text("".join("\t".join(r) + "\n" for r in seeds()), encoding="utf-8")
    proj = OUT / "project"
    if proj.exists():
        shutil.rmtree(proj)
    proj.mkdir(parents=True)
    (OUT / "appdata").mkdir(exist_ok=True)
    cmd = [str(GHIDRA), str(proj), "cs", "-import", str(exe), "-overwrite",
           "-scriptPath", str(ROOT / "tools" / "ghidra"),
           "-postScript", "ModuleExport.java", str(OUT / "seeds.tsv"), str(OUT / "modules")]
    env = dict(os.environ, JAVA_HOME=str(JAVA_HOME), APPDATA=str(OUT / "appdata"))
    r = subprocess.run(cmd, cwd=ROOT, env=env, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, text=True, encoding="utf-8", errors="replace")
    (OUT / "ghidra.log").write_text(r.stdout, encoding="utf-8")
    tail = [l for l in r.stdout.splitlines() if "ModuleExport" in l or "ERROR" in l][-5:]
    print("\n".join(tail))
    return r.returncode


def module_of(va):
    mods = json.loads((BUILD / "modules" / "cs11.json").read_text())
    return next(m for m in mods if int(m["start"], 16) <= va < int(m["end"], 16))


def main(argv):
    if len(argv) > 1 and argv[1] == "--export":
        return export()
    whole = "--module" in argv
    va = int([a for a in argv[1:] if not a.startswith("--")][0], 16)
    m = module_of(va)
    text = (OUT / "modules" / f"{m['start']}.c").read_text(encoding="utf-8")
    if whole:
        print(text)
        return 0
    parts = re.split(r"(?m)^(?=/\* VA=)", text)
    want = f"/* VA={va:08x} "
    for p in parts:
        if p.startswith(want):
            print(p)
            return 0
    print(f"no draft for {va:08x}")
    return 1


if __name__ == "__main__":
    sys.exit(main(sys.argv))
