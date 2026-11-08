"""Disassemble the CODE segments/COMDATs of a Watcom OMF object, with publics and fixups.

    python tools/objdis.py OBJ [--data]
"""
import sys
from pathlib import Path

import capstone

sys.path.insert(0, str(Path(__file__).resolve().parent))
import omf  # noqa: E402


def main(argv):
    mods = omf.load(argv[1])
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    for m in mods:
        print(f"module {m.name}  comments {m.comments[:3]}")
        for si, seg in enumerate(m.segments):
            if not seg:
                continue
            print(f"-- seg {si} {seg.name} class {seg.cls} size {seg.size:#x} align {seg.align}"
                  + (f" comdat {seg.comdat}" if getattr(seg, "comdat", None) else ""))
            pubs = {o: n for n, s, o, _ in m.publics if s == si}
            fx = {f.off: f for f in m.fixups if f.seg == si}
            if seg.cls.upper() != "CODE":
                if "--data" in argv and seg.size:
                    print("   ", bytes(seg.data[:256]).hex())
                for o, f in sorted(fx.items()):
                    print(f"    fixup +{o:x} {f.kind()} {'rel' if f.self_rel else 'abs'} -> {m.target_name(f)}+{f.disp:#x}")
                continue
            for x in md.disasm(bytes(seg.data), 0):
                if x.address in pubs:
                    print(f"{pubs[x.address]}:")
                note = [f"{m.target_name(f)}+{f.disp:#x}{' rel' if f.self_rel else ''}"
                        for o, f in fx.items() if x.address <= o < x.address + x.size]
                print(f"  {x.address:05x} {x.bytes.hex():22s} {x.mnemonic} {x.op_str}" + (f"  ; {' '.join(note)}" if note else ""))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
