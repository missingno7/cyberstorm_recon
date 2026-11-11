"""Instruction diff of one function between the two oracle builds (cs11 1.10A vs cs10 1.00A). Read-only diagnostic.

    python tools/verdiff.py VA_CS11 VA_CS10 SIZE [--all]

A function that the reconstructed source reproduces in 1.00A but not in 1.10A changed between the builds: the
diff shows what the patch changed (code, a constant, a struct offset, or only the register assignment).
Relocated operands and call targets are masked (addresses differ between the builds anyway).
"""
import difflib
import re
import sys
from pathlib import Path

import capstone

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pe as pemod  # noqa: E402

MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)


def listing(img, va, size):
    rel = img.reloc_sites()
    code = img.read_va(va, size)
    out = []
    for i in MD.disasm(code, va):
        s = f"{i.mnemonic} {i.op_str}"
        if any(a in rel for a in range(i.address, i.address + i.size)):
            s = re.sub(r"0x[0-9a-f]{6,8}", "<reloc>", s)
        if i.mnemonic == "call":
            s = "call <fn>"
        elif i.mnemonic.startswith("j") and i.op_str.startswith("0x"):
            t = int(i.op_str, 16)
            s = f"{i.mnemonic} +{t - va:x}" if va <= t < va + size else f"{i.mnemonic} <out>"
        out.append(s)
    return out


def main(argv):
    if len(argv) < 4:
        print(__doc__)
        return 2
    a, b, size = int(argv[1], 16), int(argv[2], 16), int(argv[3], 0)
    la, lb = listing(pemod.load("cs11"), a, size), listing(pemod.load("cs10"), b, size)
    same = 0
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, la, lb, autojunk=False).get_opcodes():
        if tag == "equal":
            same += i2 - i1
            if "--all" in argv:
                for k in range(i1, i2):
                    print(f"      {la[k]}")
            else:
                print(f"      ... {i2 - i1} equal")
            continue
        for k in range(max(i2 - i1, j2 - j1)):
            x = la[i1 + k] if i1 + k < i2 else ""
            y = lb[j1 + k] if j1 + k < j2 else ""
            print(f"  |   {x:<46s} | {y}")
    print(f"# cs11 (1.10A) left, cs10 (1.00A) right; {same} of {len(la)} instructions equal")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
