"""Hex dump of an oracle VA range, with dword view for data tables. Read-only diagnostic.

    python tools/hexdump.py START_VA END_VA [--dwords] [--oracle cs10]
"""
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pe as pemod  # noqa: E402


def main(argv):
    args = [a for a in argv[1:] if not a.startswith("--")]
    if len(args) != 2:
        print(__doc__)
        return 2
    oracle = argv[argv.index("--oracle") + 1] if "--oracle" in argv else None
    args = [a for a in args if a != oracle]
    img = pemod.load(oracle)
    a, b = int(args[0], 16), int(args[1], 16)
    d = img.read_va(a, b - a)
    relocs = img.reloc_sites()
    for i in range(0, len(d), 16):
        c = d[i:i + 16]
        if "--dwords" in argv:
            words = []
            for k in range(0, len(c) - 3, 4):
                v = struct.unpack_from("<I", c, k)[0]
                words.append(f"{v:08x}{'*' if a + i + k in relocs else ' '}")
            print(f"{a + i:08x}  {' '.join(words)}   (* = relocated pointer)")
        else:
            print(f"{a + i:08x}  {c.hex(' '):<48s} {''.join(chr(x) if 32 <= x < 127 else '.' for x in c)}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
