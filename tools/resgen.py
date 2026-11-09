"""Generate .ico/.cur files from readable pixel text (src/res/*.ico.txt, *.cur.txt).

    python tools/resgen.py SRC.txt OUT.(ico|cur)

Text format (top row first):
    hotspot X Y            cursors only
    sizeimage N            DIB header biSizeImage as the authoring tool wrote it
    palette RRGGBB ...     2 entries (cursor) or 16 (icon), in index order
    pixels                 followed by 32 lines of 32 characters
Cursor characters: '.' transparent (AND 1, XOR 0), '#' colour 0 (AND 0, XOR 0), 'o' colour 1 (AND 0, XOR 1),
'~' inverted (AND 1, XOR 1). Icon characters: a hex digit = palette index with AND 0; '.' = AND 1, index 0
(the format has no spelling for a masked pixel with a non-zero index).
The output is the conventional file layout (ICONDIR + one entry + DIB); WRC turns it into RT_ICON/RT_CURSOR data.
"""
import struct
import sys
from pathlib import Path


def parse(text):
    kv, rows, inpix = {}, [], False
    for line in text.splitlines():
        if inpix:
            if line.strip():
                rows.append(line.rstrip("\n"))
            continue
        line = line.split("#", 1)[0].strip() if not line.startswith("pixels") else line.strip()
        if not line:
            continue
        if line == "pixels":
            inpix = True
            continue
        k, *v = line.split()
        kv[k] = v
    if len(rows) != 32 or any(len(r) != 32 for r in rows):
        raise SystemExit("need 32 rows of 32 pixels")
    return kv, rows


def pack_bits(bits):
    out = bytearray()
    for i in range(0, len(bits), 8):
        v = 0
        for b in bits[i:i + 8]:
            v = v << 1 | b
        out.append(v)
    return bytes(out)


def build(text, cursor):
    kv, rows = parse(text)
    pal = b"".join(bytes.fromhex(c)[::-1] + b"\0" for c in kv["palette"])
    bpp = 1 if cursor else 4
    if len(kv["palette"]) != (2 if cursor else 16):
        raise SystemExit("palette size")
    xor_rows, and_rows = [], []
    for r in rows:
        xs, ms = [], []
        for ch in r:
            if cursor:
                m, x = {".": (1, 0), "#": (0, 0), "o": (0, 1), "~": (1, 1)}[ch]
            elif ch == ".":
                m, x = 1, 0
            else:
                m, x = 0, int(ch, 16)
            xs.append(x)
            ms.append(m)
        if cursor:
            xor_rows.append(pack_bits(xs))
        else:
            xor_rows.append(bytes(xs[i] << 4 | xs[i + 1] for i in range(0, 32, 2)))
        and_rows.append(pack_bits(ms))
    dib = (struct.pack("<IiiHHIIiiII", 40, 32, 64, 1, bpp, 0, int(kv["sizeimage"][0]), 0, 0, 0, 0) + pal
           + b"".join(reversed(xor_rows)) + b"".join(reversed(and_rows)))
    if cursor:
        hx, hy = (int(v) for v in kv["hotspot"])
        entry = struct.pack("<BBBBHHII", 32, 32, 0, 0, hx, hy, len(dib), 22)
        head = struct.pack("<HHH", 0, 2, 1)
    else:
        entry = struct.pack("<BBBBHHII", 32, 32, 16, 0, 1, bpp, len(dib), 22)
        head = struct.pack("<HHH", 0, 1, 1)
    return head + entry + dib


def main(argv):
    src, out = Path(argv[1]), Path(argv[2])
    out.write_bytes(build(src.read_text(encoding="ascii"), out.suffix.lower() == ".cur"))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
