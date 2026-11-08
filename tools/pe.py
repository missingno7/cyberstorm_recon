"""Pure-Python PE32 reader for the oracles (no third-party modules).

    python tools/pe.py info   [ORACLE]   headers, sections, directories
    python tools/pe.py census [ORACLE]   account for every file byte; fails if any byte is unexplained
    python tools/pe.py lock   [ORACLE]   (re)write config/locks/ORACLE.json (identity hashes)
    python tools/pe.py check  [ORACLE]   compare the oracle against its lock

Addresses are VAs (image base included) unless named 'rva' or 'off'.
"""
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import CONFIG, default_oracle, dump_json, load_json, oracle_bytes, sha256_bytes  # noqa: E402

DIR_NAMES = ["export", "import", "resource", "exception", "security", "basereloc", "debug", "copyright",
             "globalptr", "tls", "load_config", "bound_import", "iat", "delay_import", "com", "reserved"]


class Section:
    def __init__(self, name, vsize, rva, rawsize, rawoff, chars):
        self.name, self.vsize, self.rva, self.rawsize, self.rawoff, self.chars = name, vsize, rva, rawsize, rawoff, chars

    @property
    def filesize(self):
        """Bytes backed by the file. WLINK writes a nonzero SizeOfRawData with PointerToRawData 0 for .bss;
        the loader reads nothing for such a section, so it has no file bytes."""
        return self.rawsize if self.rawoff else 0


class PE:
    def __init__(self, data, name="?"):
        self.data, self.name = data, name
        if data[:2] != b"MZ":
            raise ValueError("no MZ header")
        self.e_lfanew = struct.unpack_from("<I", data, 0x3C)[0]
        if data[self.e_lfanew:self.e_lfanew + 4] != b"PE\0\0":
            raise ValueError("no PE signature")
        fh = self.e_lfanew + 4
        (self.machine, nsec, self.timestamp, self.symptr, self.nsyms, self.opt_size,
         self.characteristics) = struct.unpack_from("<HHIIIHH", data, fh)
        oh = fh + 20
        self.opt_off = oh
        magic = struct.unpack_from("<H", data, oh)[0]
        if magic != 0x10B:
            raise ValueError("not PE32")
        self.linker = (data[oh + 2], data[oh + 3])
        (self.size_code, self.size_idata, self.size_udata, self.entry_rva, self.base_code,
         self.base_data, self.image_base, self.sect_align, self.file_align) = struct.unpack_from("<IIIIIIIII", data, oh + 4)
        (self.os_ver_major, self.os_ver_minor, self.img_ver_major, self.img_ver_minor, self.ss_major, self.ss_minor,
         self.win32ver, self.size_image, self.size_headers, self.checksum, self.subsystem,
         self.dll_chars) = struct.unpack_from("<HHHHHHIIIIHH", data, oh + 40)
        (self.stack_reserve, self.stack_commit, self.heap_reserve, self.heap_commit, self.loader_flags,
         self.ndirs) = struct.unpack_from("<IIIIII", data, oh + 72)
        self.dirs = [struct.unpack_from("<II", data, oh + 96 + 8 * i) for i in range(self.ndirs)]
        so = oh + self.opt_size
        self.sec_table_off = so
        self.sections = []
        for i in range(nsec):
            b = so + 40 * i
            name = data[b:b + 8].rstrip(b"\0").decode("latin-1")
            vsize, rva, rawsize, rawoff = struct.unpack_from("<IIII", data, b + 8)
            chars = struct.unpack_from("<I", data, b + 36)[0]
            self.sections.append(Section(name, vsize, rva, rawsize, rawoff, chars))
        # effective mapped extent: up to the next section's RVA (WLINK leaves VirtualSize 0)
        for i, s in enumerate(self.sections):
            nxt = self.sections[i + 1].rva if i + 1 < len(self.sections) else self.size_image
            s.extent = nxt - s.rva
        self._relocs = None

    # -- addressing -------------------------------------------------------------------------------------
    def section(self, name):
        for s in self.sections:
            if s.name == name:
                return s
        raise KeyError(name)

    def section_at_rva(self, rva):
        for s in self.sections:
            if s.rva <= rva < s.rva + s.extent:
                return s
        return None

    def rva_to_off(self, rva):
        s = self.section_at_rva(rva)
        if s is None or rva - s.rva >= s.filesize:
            return None
        return s.rawoff + rva - s.rva

    def read_va(self, va, n):
        rva = va - self.image_base
        out = bytearray()
        for k in range(n):
            o = self.rva_to_off(rva + k)
            out.append(self.data[o] if o is not None else 0)
        return bytes(out)

    def section_bytes(self, name):
        s = self.section(name)
        return self.data[s.rawoff:s.rawoff + s.filesize]

    def u32_va(self, va):
        return struct.unpack("<I", self.read_va(va, 4))[0]

    def cstr_rva(self, rva):
        o = self.rva_to_off(rva)
        e = self.data.index(b"\0", o)
        return self.data[o:e].decode("latin-1")

    # -- directories --------------------------------------------------------------------------------------
    def relocs(self):
        """Ordered list of (va, type) base relocations, including type-0 padding entries."""
        if self._relocs is None:
            out = []
            rva, size = self.dirs[5]
            o, end = self.rva_to_off(rva), self.rva_to_off(rva) + size
            while o < end:
                page, bsize = struct.unpack_from("<II", self.data, o)
                if bsize < 8:
                    raise ValueError(f"bad reloc block size {bsize} at {o:#x}")
                for k in range((bsize - 8) // 2):
                    e = struct.unpack_from("<H", self.data, o + 8 + 2 * k)[0]
                    out.append((self.image_base + page + (e & 0xFFF), e >> 12))
                o += bsize
            self._relocs = out
        return self._relocs

    def reloc_sites(self):
        """Set of VAs that carry a HIGHLOW (type 3) base relocation."""
        return {va for va, t in self.relocs() if t == 3}

    def imports(self):
        """List of (dll, name_or_ordinal, iat_va)."""
        out = []
        rva, size = self.dirs[1]
        if not rva:
            return out
        o = self.rva_to_off(rva)
        while True:
            ilt, ts, fwd, name_rva, iat = struct.unpack_from("<IIIII", self.data, o)
            if not (ilt or name_rva or iat):
                break
            dll = self.cstr_rva(name_rva)
            src = ilt or iat
            k = 0
            while True:
                thunk = struct.unpack_from("<I", self.data, self.rva_to_off(src + 4 * k))[0]
                if not thunk:
                    break
                nm = f"#{thunk & 0xFFFF}" if thunk & 0x80000000 else self.cstr_rva(thunk + 2)
                out.append((dll, nm, self.image_base + iat + 4 * k))
                k += 1
            o += 20
        return out

    def exports(self):
        rva, size = self.dirs[0]
        if not rva:
            return []
        o = self.rva_to_off(rva)
        (_, _, _, _, name_rva, base, nfunc, nname, af, an, ao) = struct.unpack_from("<IIHHIIIIIII", self.data, o)
        funcs = [struct.unpack_from("<I", self.data, self.rva_to_off(af) + 4 * i)[0] for i in range(nfunc)]
        out = []
        for i in range(nname):
            n = self.cstr_rva(struct.unpack_from("<I", self.data, self.rva_to_off(an) + 4 * i)[0])
            ordi = struct.unpack_from("<H", self.data, self.rva_to_off(ao) + 2 * i)[0]
            out.append((n, base + ordi, self.image_base + funcs[ordi]))
        return out


def load(name=None):
    name = name or default_oracle()
    return PE(oracle_bytes(name), name)


# -- census -------------------------------------------------------------------------------------------------
def census(pe):
    """Return list of (start, end, label) covering every file byte exactly once, plus problems."""
    d, regions, problems = pe.data, [], []
    regions.append((0, 0x40, "MZ header"))
    regions.append((0x40, pe.e_lfanew, "DOS stub"))
    regions.append((pe.e_lfanew, pe.sec_table_off, "PE+optional header"))
    st_end = pe.sec_table_off + 40 * len(pe.sections)
    regions.append((pe.sec_table_off, st_end, "section table"))
    regions.append((st_end, pe.size_headers, "header padding"))
    if any(d[st_end:pe.size_headers]):
        problems.append("nonzero header padding")
    for s in sorted((s for s in pe.sections if s.filesize), key=lambda s: s.rawoff):
        regions.append((s.rawoff, s.rawoff + s.filesize, f"section {s.name}"))
    regions.sort()
    cur = 0
    for a, b, lab in regions:
        if a > cur:
            problems.append(f"gap {cur:#x}-{a:#x} before {lab}")
        if a < cur:
            problems.append(f"overlap at {a:#x} ({lab})")
        cur = max(cur, b)
    if cur != len(d):
        problems.append(f"file tail {cur:#x}-{len(d):#x} not covered (overlay?)")
    return regions, problems


def lock_record(pe):
    rel = pe.relocs()
    ordered = b"".join(struct.pack("<IB", va, t) for va, t in rel)
    sites = sorted(pe.reloc_sites())
    return {
        "oracle": pe.name,
        "file_sha256": sha256_bytes(pe.data),
        "file_size": len(pe.data),
        "timestamp": pe.timestamp,
        "linker": list(pe.linker),
        "image_base": pe.image_base,
        "entry_va": pe.image_base + pe.entry_rva,
        "checksum": pe.checksum,
        "headers_sha256": sha256_bytes(pe.data[:pe.size_headers]),
        "sections": [{"name": s.name, "rva": s.rva, "extent": s.extent, "rawoff": s.rawoff, "rawsize": s.rawsize,
                      "chars": s.chars, "sha256": sha256_bytes(pe.data[s.rawoff:s.rawoff + s.filesize])}
                     for s in pe.sections],
        "relocs": {"count": len(rel), "highlow": len(sites),
                   "ordered_sha256": sha256_bytes(ordered),
                   "set_sha256": sha256_bytes(b"".join(struct.pack("<I", v) for v in sites)),
                   "types": sorted({t for _, t in rel})},
        "imports": len(pe.imports()),
        "exports": [e[0] for e in pe.exports()],
    }


def main(argv):
    cmd = argv[1] if len(argv) > 1 else "info"
    name = argv[2] if len(argv) > 2 else None
    pe = load(name)
    if cmd == "info":
        print(f"{pe.name}: linker {pe.linker[0]}.{pe.linker[1]} base {pe.image_base:#x} entry "
              f"{pe.image_base + pe.entry_rva:#x} stamp {pe.timestamp:#x} checksum {pe.checksum:#x}")
        for s in pe.sections:
            print(f"  {s.name:8s} va {pe.image_base + s.rva:08x} extent {s.extent:06x} raw {s.rawoff:06x}+{s.rawsize:06x} "
                  f"chars {s.chars:08x}")
        for i, (r, sz) in enumerate(pe.dirs):
            if r:
                print(f"  dir {DIR_NAMES[i]:10s} rva {r:06x} size {sz:#x}")
        print(f"  relocs {len(pe.relocs())} ({len(pe.reloc_sites())} HIGHLOW), imports {len(pe.imports())}, "
              f"exports {[e[0] for e in pe.exports()]}")
        return 0
    if cmd == "census":
        regions, problems = census(pe)
        for a, b, lab in regions:
            print(f"  {a:06x}-{b:06x} {b - a:7d} {lab}")
        for p in problems:
            print("PROBLEM:", p)
        print("census OK: every byte accounted for" if not problems else "census FAILED")
        return 1 if problems else 0
    path = CONFIG / "locks" / f"{pe.name}.json"
    if cmd == "lock":
        dump_json(path, lock_record(pe))
        print(f"wrote {path}")
        return 0
    if cmd == "check":
        want, got = load_json(path), lock_record(pe)
        bad = [k for k in want if want[k] != got.get(k)]
        print("lock OK" if not bad else f"lock MISMATCH in {bad}")
        return 1 if bad else 0
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv))
