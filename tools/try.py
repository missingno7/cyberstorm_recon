"""One-shot worker loop: compile + compare + aligned instruction diff of every non-strict function.

    python tools/try.py SRC.c --as 'dir\\file.c' --profile ot|plain|ot-c|plain-c [--at Label=VA ...]
                        [--fn SUBSTR ...] [--context 6] [--all]

Replaces cc.py + compare.py + objdis.py + show.py. For each code extent: status line. For DIFF extents (or the
ones selected with --fn): a side-by-side diff of original (left) vs candidate (right) instructions, aligned with
difflib on a normalised form (relocated operands become their target name or `<reloc>`, branch targets become
function offsets), showing only `--context` lines around each differing run; `--all` shows full functions.
Diagnostic only: STRICT comes from compare.py through the gate.
"""
import difflib
import json
import re
import sys
import tempfile
from pathlib import Path

import capstone

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cc  # noqa: E402
import compare  # noqa: E402
import omf  # noqa: E402
import symbols  # noqa: E402
from common import BUILD, CONFIG, ROOT, load_json  # noqa: E402

MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
HEX = re.compile(r"0x[0-9a-f]+|(?<=[\[\s,])0(?=[\]\s,]|$)")


def norm_name(n):
    imp = symbols.import_name(n) if n else None
    return imp or symbols.label(n)


def names(img):
    out = {va: n for dll, n, va in img.imports()}
    p = BUILD / "libsyms" / f"{img.name}.json"
    if p.exists():
        for n, e in json.loads(p.read_text()).items():
            out.setdefault(int(e["va"], 16), n)
    for n, e in json.loads((CONFIG / "symbols.json").read_text())["symbols"].items():
        out.setdefault(int(e["va"], 16), n)
    return out


def insns(code, base, start, reloc_name, branch_base):
    """[(text, raw)] with normalised operands. reloc_name(off) -> name or None for fields at code offset."""
    out = []
    for x in MD.disasm(code, base):
        off = x.address - base
        txt = f"{x.mnemonic} {x.op_str}".strip()
        fields = [reloc_name(off + k) for k in range(x.size) if reloc_name(off + k)]
        if fields:
            txt = HEX.sub(lambda m: f"<{fields[0]}>", txt, count=1) if HEX.search(txt) else txt + f"  <{fields[0]}>"
        elif x.mnemonic.startswith("j") or x.mnemonic in ("call", "loop"):
            if x.op_str.startswith("0x"):
                t = int(x.op_str, 16) - branch_base
                txt = f"{x.mnemonic} +{t:x}" if 0 <= t < 0x10000 else f"{x.mnemonic} {x.op_str}"
        out.append((txt, f"{off:4x} {x.bytes.hex():16s}"))
    return out


def side_by_side(a, b, context, show_all):
    sm = difflib.SequenceMatcher(a=[t for t, _ in a], b=[t for t, _ in b], autojunk=False)
    lines = []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        n = max(i2 - i1, j2 - j1)
        for k in range(n):
            left = a[i1 + k] if i1 + k < i2 else None
            right = b[j1 + k] if j1 + k < j2 else None
            mark = " " if tag == "equal" else "|" if (left and right) else "<" if left else ">"
            lines.append((mark, left, right))
    keep = [False] * len(lines)
    for i, (m, _, _) in enumerate(lines):
        if m != " ":
            for j in range(max(0, i - context), min(len(lines), i + context + 1)):
                keep[j] = True
    out, skipped = [], False
    for i, (m, l, r) in enumerate(lines):
        if show_all or keep[i]:
            if skipped:
                out.append("        ...")
                skipped = False
            lt = f"{l[1]} {l[0]}" if l else ""
            rt = f"{r[0]}" if r else ""
            out.append(f"  {m} {lt[:62]:62s} {m} {rt[:58]}")
        else:
            skipped = True
    return out


def main(argv):
    argv = list(argv[1:])
    opts = {"at": {}, "fn": []}
    flags_all = "--all" in argv
    if flags_all:
        argv.remove("--all")
    for key in ("--as", "--profile", "--flags", "--tool", "--context", "--at", "--fn"):
        while key in argv:
            i = argv.index(key)
            v = argv[i + 1]
            del argv[i:i + 2]
            if key == "--at":
                k, va = v.split("=")
                opts["at"][k] = int(va, 16)
            elif key == "--fn":
                opts["fn"].append(v)
            else:
                opts[key[2:]] = v
    if len(argv) != 1 or ("profile" not in opts and "flags" not in opts):
        print(__doc__)
        return 2
    if "flags" in opts:         # a TU recorded with explicit flags (recovery.json "flags"), e.g. "-4s -s -bm -oxt -oe=80"
        prof = {"flags": opts["flags"].split(), "install": "wc106", "tool": opts.get("tool", "wpp386")}
    else:
        prof = load_json(CONFIG / "toolchain.json")["profiles"][opts["profile"]]
    src = Path(argv[0])
    context = int(opts.get("context", 6))
    with tempfile.TemporaryDirectory(prefix="try", dir=ROOT / "build") as td:
        obj = Path(td) / "t.obj"
        norm = Path(td) / src.name
        norm.write_bytes(src.read_bytes().replace(b"\r\n", b"\n"))
        rc, text = cc.compile_one(norm, obj, prof["flags"], prof["install"], prof["tool"],
                                  as_name=opts.get("as"))
        if rc:
            print(text)
            return 1
        reports, img = compare.compare_object(obj, None, opts["at"], True, True)
        mods = omf.load(obj)
    nm = names(img)
    rel = img.reloc_sites()
    n_strict = n_total = 0
    for r, m in zip(reports, mods):
        for c in r["conflicts"]:
            print("CONFLICT:", c)
        segidx = {s.name: i for i, s in enumerate(m.segments) if s}
        for s in r["segments"]:
            if s["class"].upper() != "CODE":
                if s["status"] not in ("EMPTY", "BSS", "STRICT"):
                    print(f"[{s['status']}] {s['seg']} {s['class']} size {s['size']:#x}")
                continue
            si = segidx[s["seg"]]
            seg = m.segments[si]
            fx = {f.off + k: m.target_name(f) for f in m.fixups if f.seg == si for k in range(f.size())}
            for e in s["extents"]:
                n_total += 1
                lab = symbols.label(e["names"][0])
                st = e["status"]
                n_strict += st == "STRICT"
                extra = ""
                if st == "DIFF":
                    extra = f"  {e['diff_bytes']} B differ" + (f", first +{e['first_diff']:x}" if e["first_diff"] is not None else "")
                    if e.get("size_bad"):
                        extra += f"; {e['size_bad']}"
                    if e["reloc_extra"] or e["reloc_missing"]:
                        extra += "; reloc sites differ"
                elif st == "UNBOUND":
                    extra = "  unbound: " + ", ".join(f"{symbols.label(k)}->{v}" for k, v in e["implied"].items())
                print(f"{e.get('va') or '--------'} {st:8s} {lab} ({e['end'] - e['off']} B){extra}")
                want = (st == "DIFF" and not opts["fn"]) or any(f in lab for f in opts["fn"])
                if not want or not e.get("va"):
                    continue
                va = int(e["va"], 16)
                size = e["end"] - e["off"]
                ours = bytes(seg.data[e["off"]:e["end"]])
                orig = img.read_va(va, size + 16)

                def oname(off, va=va):
                    if va + off in rel:
                        t = img.u32_va(va + off - ((va + off - va) % 1))
                        return None
                    return None
                orel = {}
                for k in range(size + 16):
                    if va + k in rel:
                        t = img.u32_va(va + k)
                        for j in range(4):
                            orel[k + j] = norm_name(nm.get(t, f"{t:08x}"))
                a = insns(orig[:size], va, 0, lambda o: orel.get(o), va)
                b = insns(ours, 0, 0, lambda o: norm_name(fx[e["off"] + o]) if e["off"] + o in fx else None, 0)
                # calls in the original: rel32 targets -> names, so they compare with our fixup names
                a = [(re.sub(r"call 0x([0-9a-f]+)", lambda mm: f"call <{symbols.label(nm.get(int(mm.group(1), 16), mm.group(0)))}>", t), raw) for t, raw in a]
                b = [(t.replace("call +", "call <self+").replace("call 0x", "call <0x") if "<" not in t else t, raw) for t, raw in b]
                print(f"    {'original':67s} candidate")
                for line in side_by_side(a, b, context, flags_all):
                    print(line)
    print(f"# {n_strict}/{n_total} extents STRICT")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
