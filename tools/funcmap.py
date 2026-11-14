"""Recover function starts and extents in BEGTEXT by recursive descent (read-only census).

Seeds: entry point, exports, library symbols (build/libsyms), every base-relocated dword in any section whose
value points into BEGTEXT and that is not inside a recognised jump table, and every rel32 CALL target found
while descending. Jump tables (jmp [reg*4+table] with relocated entries) are followed and their bytes are
attributed to the function that owns them.

    python tools/funcmap.py [--oracle O]        write build/funcmap/O.json and print a summary
    python tools/funcmap.py --gaps [--oracle O] list BEGTEXT bytes not owned by any function

Output functions: {va, end, size, kind: lib|code, name?, callers, refs, jumptables}. `end` is the next
function start (so the extent includes alignment padding and tables). A heuristic map: names and extents are
hypotheses until a strict match covers them.
"""
import json
import struct
import sys
from pathlib import Path

import capstone
from capstone import x86

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pe as pemod  # noqa: E402
from common import BUILD, ROOT, dump_json  # noqa: E402


def pad_len(code, k):
    """Length of a no-op filler instruction at code[k] (Watcom alignment padding), else 0.
    Recognises nop, mov r,r and lea r,[r+0] with disp8/disp32 and the no-index SIB forms."""
    b0 = code[k]
    if b0 in (0x90, 0x00):
        return 1
    if k + 1 >= len(code):
        return 0
    m = code[k + 1]
    mod, reg, rm = m >> 6, (m >> 3) & 7, m & 7
    if b0 == 0x8B and mod == 3 and reg == rm:
        return 2
    if b0 != 0x8D or mod == 3 or mod == 0:
        return 0
    if rm == 4:  # SIB
        if k + 2 >= len(code):
            return 0
        sib = code[k + 2]
        if (sib >> 3) & 7 != 4 or sib & 7 != reg:
            return 0
        n = 3
    else:
        if rm != reg:
            return 0
        n = 2
    dl = 1 if mod == 1 else 4
    if code[k + n:k + n + dl] != bytes(dl):
        return 0
    return n + dl


PROLOGUE = (b"\x53", b"\x55", b"\x56", b"\x57", b"\x8b\x44\x24", b"\x8b\x54\x24", b"\x8b\x4c\x24",
            b"\x83\xec", b"\x2e\xff\x15", b"\x68", b"\x6a", b"\xa1", b"\xc7\x05")


def analyse(img, known_data=frozenset(), known_jt_sites=frozenset()):
    """known_data: addresses inside BEGTEXT used as memory operands (jump/lookup tables) by a previous pass;
    they are never function starts, and an uncovered gap that starts with one is skipped to the next
    prologue-like byte (Watcom puts switch tables in front of the function that uses them)."""
    text = img.section("BEGTEXT")
    t0 = img.image_base + text.rva
    code = img.section_bytes("BEGTEXT")
    t1 = t0 + len(code)
    relsites = img.reloc_sites()
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True

    def in_text(v):
        return t0 <= v < t1

    libsyms = {}
    p = BUILD / "libsyms" / f"{img.name}.json"
    if p.exists():
        for n, e in json.loads(p.read_text()).items():
            libsyms.setdefault(int(e["va"], 16), n)

    # pointers into code from relocated dwords anywhere in the image
    ptr_refs = {}
    for site in relsites:
        v = img.u32_va(site)
        if in_text(v):
            ptr_refs.setdefault(v, []).append(site)

    seeds = {img.image_base + img.entry_rva: "entry"}
    for n, _, va in img.exports():
        seeds[va] = "export"
    for va in libsyms:
        seeds[va] = "lib"

    data_refs = set()       # BEGTEXT addresses used as absolute memory operands (tables)
    insn_owner = {}         # va -> function start
    insn_cover = bytearray(len(code))
    calls = {}              # callee -> callers
    jtables = {}            # table va -> (owner, n entries)
    func_starts = set()
    work = []

    def add_func(va, why):
        if in_text(va) and va not in func_starts:
            func_starts.add(va)
            work.append(va)

    for va in seeds:
        add_func(va, seeds[va])
    # config/nongame.json "starts": function starts nothing references (an empty function that the preceding
    # function falls through into); each needs its evidence written there
    xs = ROOT / "config" / "nongame.json"
    if img.name == "cs11" and xs.exists():
        for s in json.loads(xs.read_text(encoding="utf-8")).get("starts", []):
            add_func(int(s["va"], 16), "declared")

    def descend(start):
        last_imm = {}
        stack = [start]
        while stack:
            pc = stack.pop()
            while in_text(pc) and not insn_cover[pc - t0]:
                off = pc - t0
                try:
                    x = next(md.disasm(code[off:off + 16], pc, 1))
                except StopIteration:
                    break
                for k in range(x.size):
                    insn_cover[off + k] = 1
                insn_owner[pc] = start
                m = x.mnemonic
                if m == "mov" and len(x.operands) == 2 and x.operands[1].type == x86.X86_OP_IMM                         and in_text(x.operands[1].imm):
                    last_imm[x.operands[0].reg] = x.operands[1].imm
                if m.startswith("repne scas") or m.startswith("repe scas") or m.startswith("repnz scas"):
                    v = last_imm.get(x86.X86_REG_EDI)
                    if v is not None:
                        data_refs.add(v)       # scan table addressed through EDI
                for op in x.operands:
                    if op.type == x86.X86_OP_MEM and not op.mem.base and in_text(op.mem.disp):
                        data_refs.add(op.mem.disp)
                nxt = pc + x.size
                if m == "call":
                    op = x.operands[0]
                    if op.type == x86.X86_OP_IMM:
                        tgt = op.imm
                        calls.setdefault(tgt, []).append(pc)
                        add_func(tgt, "call")
                elif m == "jmp" or m.startswith("j") or m in ("loop", "loope", "loopne", "jecxz"):
                    op = x.operands[0]
                    if op.type == x86.X86_OP_IMM:
                        stack.append(op.imm)
                    elif op.type == x86.X86_OP_MEM and op.mem.index and op.mem.scale == 4 and in_text(op.mem.disp):
                        tab = op.mem.disp
                        n = 0
                        while tab + 4 * n in relsites and in_text(img.u32_va(tab + 4 * n)):
                            stack.append(img.u32_va(tab + 4 * n))
                            n += 1
                        jtables[tab] = (start, n)
                        for k in range(4 * n):
                            if 0 <= tab - t0 + k < len(code):
                                insn_cover[tab - t0 + k] = 1
                    if m == "jmp":
                        break
                elif m in ("ret", "retf", "iret", "iretd", "hlt") or (m == "int3"):
                    break
                pc = nxt

    while work:
        descend(work.pop())

    # relocated code pointers not inside jump tables and not already inside a descended instruction body
    jt_sites = set(known_jt_sites)
    for tab, (_, n) in jtables.items():
        jt_sites.update(range(tab, tab + 4 * n, 4))
    for v, sites in ptr_refs.items():
        if all(s in jt_sites for s in sites) or v in known_data or v in data_refs:
            continue
        if v not in insn_owner and not insn_cover[v - t0]:
            add_func(v, "ptr")
        elif v not in insn_owner:
            continue
        elif insn_owner[v] != v and any(s not in jt_sites for s in sites):
            add_func(v, "ptr")   # pointer to an instruction inside another 'function': it is a start
    while work:
        descend(work.pop())

    # Unreferenced functions (Watcom keeps every function of a linked object): descend from the first
    # non-padding byte of each uncovered gap until nothing new appears.
    orphans = set()
    while True:
        new = 0
        i = 0
        while i < len(code):
            if insn_cover[i]:
                i += 1
                continue
            j = i
            while j < len(code) and not insn_cover[j]:
                j += 1
            k = i
            while k < j:
                n = pad_len(code, k)
                if not n or k + n > j:
                    break
                k += n
            if k < j and t0 + k in known_data:
                # a table in front of a function: start at the first prologue-like byte after it
                p = k + 1
                while p < j and not any(code[p:p + len(pp)] == pp for pp in PROLOGUE[:4] + PROLOGUE[4:7]):
                    p += 1
                for q in range(k, p):
                    insn_cover[q] = 1
                k = p
            if k < j and t0 + k not in func_starts and t0 + k not in known_data and any(code[k:j]):
                add_func(t0 + k, "orphan")
                orphans.add(t0 + k)
                new += 1
            i = j
        if not new:
            break
        while work:
            descend(work.pop())

    starts = sorted(func_starts)
    # config/nongame.json: classified non-C ranges (runtime members libscan misses, import glue, data, hand asm)
    ng = []
    ngp = ROOT / "config" / "nongame.json"
    if img.name == "cs11" and ngp.exists():
        ng = [(int(r["start"], 16), int(r["end"], 16), r["class"]) for r in json.loads(ngp.read_text())["ranges"]]

    def kind_of(s):
        if s in libsyms:
            return "lib"
        for a, b, c in ng:
            if a <= s < b:
                return "asm" if c == "asm" else "lib"
        return "orphan" if s in orphans else "code"
    funcs = []
    for i, s in enumerate(starts):
        e = starts[i + 1] if i + 1 < len(starts) else t1
        ce = e
        while ce > s and not insn_cover[ce - 1 - t0]:
            ce -= 1
        funcs.append({"va": f"{s:08x}", "end": f"{e:08x}", "size": e - s, "code_end": f"{ce:08x}",
                      "kind": kind_of(s),
                      **({"name": libsyms[s]} if s in libsyms else {}),
                      "callers": len(calls.get(s, [])),
                      "ptr_refs": len([x for x in ptr_refs.get(s, []) if x not in jt_sites]),
                      "jumptables": [f"{t:08x}" for t, (o, n) in jtables.items() if o == s]})
    covered = sum(insn_cover)
    all_jt = set()
    for tab, (_, n) in jtables.items():
        all_jt.update(range(tab, tab + 4 * n, 4))
    return {"oracle": img.name, "text": [f"{t0:08x}", f"{t1:08x}"], "functions": funcs,
            "jt_sites": sorted(f"{v:08x}" for v in all_jt),
            "data_refs": sorted(f"{v:08x}" for v in data_refs),
            "covered_bytes": covered, "text_bytes": len(code)}, insn_cover, t0


def main(argv):
    argv = list(argv)
    name = None
    if "--oracle" in argv:
        i = argv.index("--oracle"); name = argv[i + 1]; del argv[i:i + 2]
    img = pemod.load(name)
    res, cover, t0 = analyse(img)
    # second pass: tables found anywhere in pass 1 are data, never function starts
    res, cover, t0 = analyse(img, frozenset(int(v, 16) for v in res["data_refs"]),
                             frozenset(int(v, 16) for v in res["jt_sites"]))
    if "--gaps" in argv:
        code = img.section_bytes("BEGTEXT")
        i = 0
        while i < len(cover):
            if not cover[i]:
                j = i
                while j < len(cover) and not cover[j]:
                    j += 1
                chunk = code[i:j]
                if any(b not in (0, 0x90) for b in chunk) and not (j - i < 8 and chunk in (b"\x8d\x40\x00", b"\x8d\x44\x20\x00", b"\x8b\xc0", b"\x90")):
                    print(f"{t0 + i:08x} +{j - i:<5d} {chunk[:24].hex()}", flush=True)
                i = j
            else:
                i += 1
        return 0
    out = BUILD / "funcmap" / f"{img.name}.json"
    dump_json(out, res)
    f = res["functions"]
    lib = [x for x in f if x["kind"] == "lib"]
    print(f"{len(f)} functions ({len(lib)} library), instruction coverage {res['covered_bytes']}/{res['text_bytes']} "
          f"bytes -> {out}")
    print(f"orphans (no caller, no pointer): {sum(1 for x in f if x['kind'] == 'orphan')}")
    sizes = sorted(x["size"] for x in f if x["kind"] != "lib")
    print(f"code function sizes: min {sizes[0]} median {sizes[len(sizes) // 2]} max {sizes[-1]}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
