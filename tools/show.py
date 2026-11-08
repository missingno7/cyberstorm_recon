"""Disassemble oracle bytes with relocation, import, symbol and string annotations.

    python tools/show.py VA [COUNT] [--oracle O]       COUNT instructions from VA (hex)
    python tools/show.py --module VA                   whole module containing VA (build/modules), per function
"""
import json
import sys
from pathlib import Path

import capstone

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pe as pemod  # noqa: E402
from common import BUILD  # noqa: E402


def names(img):
    out = {va: f"{dll}!{n}" for dll, n, va in img.imports()}
    from common import CONFIG
    sp = CONFIG / "symbols.json"
    if sp.exists() and img.name == "cs11":
        for n, e in json.loads(sp.read_text())["symbols"].items():
            out.setdefault(int(e["va"], 16), f"{n} [{e['status']}]")
    p = BUILD / "libsyms" / f"{img.name}.json"
    if p.exists():
        for n, e in json.loads(p.read_text()).items():
            out.setdefault(int(e["va"], 16), n)
    return out


def string_at(img, va):
    try:
        b = img.read_va(va, 64)
    except Exception:
        return None
    if 0 not in b:
        return None
    t = b[:b.index(0)]
    if len(t) >= 2 and all(32 <= c < 127 or c in (9, 10, 13) for c in t):
        return t.decode("latin-1").replace("\n", "\\n")
    return None


def disasm(img, va, n, rel, nm, stop=None):
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    for i, x in enumerate(md.disasm(img.read_va(va, n * 15), va)):
        if i >= n or (stop is not None and x.address >= stop):
            break
        notes = []
        for v in range(x.address, x.address + x.size):
            if v in rel:
                tgt = img.u32_va(v)
                st = string_at(img, tgt)
                notes.append(f"R->{tgt:08x}" + (f" {nm[tgt]}" if tgt in nm else "") + (f' "{st}"' if st else ""))
        if x.mnemonic in ("call", "jmp") and x.op_str.startswith("0x"):
            t = int(x.op_str, 16)
            if t in nm:
                notes.append(nm[t])
        lab = f"{nm[x.address]}:\n" if x.address in nm else ""
        print(f"{lab}{x.address:08x} {x.bytes.hex():22s} {x.mnemonic} {x.op_str}" + (f"   ; {' '.join(notes)}" if notes else ""))


def main(argv):
    argv = list(argv)
    name = None
    if "--oracle" in argv:
        i = argv.index("--oracle"); name = argv[i + 1]; del argv[i:i + 2]
    img = pemod.load(name)
    rel, nm = img.reloc_sites(), names(img)
    if argv[1] == "--module":
        va = int(argv[2], 16)
        mods = json.loads((BUILD / "modules" / f"{img.name}.json").read_text())
        m = next(m for m in mods if int(m["start"], 16) <= va < int(m["end"], 16))
        fm = {f["va"]: f for f in json.loads((BUILD / "funcmap" / f"{img.name}.json").read_text())["functions"]}
        print(f"# module {m['start']}..{m['end']} {m['size']} bytes, {m['nfuncs']} functions, "
              f"align16={m['align16']} paths={m['paths']}")
        for fva in m["funcs"]:
            f = fm[fva]
            print(f"\n# function {fva} size {f['size']} kind {f['kind']} callers {f['callers']} ptr_refs {f['ptr_refs']}")
            disasm(img, int(fva, 16), 100000, rel, nm, stop=int(f["code_end"], 16))
        return
    va = int(argv[1], 16)
    n = int(argv[2]) if len(argv) > 2 else 40
    disasm(img, va, n, rel, nm)


if __name__ == "__main__":
    main(sys.argv)
