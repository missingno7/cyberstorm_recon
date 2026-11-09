# CyberStorm reconstruction: reconnaissance note (2026-11-08)

Static, read-only findings. Nothing has been compiled or run yet.

## Oracle set

| File | SHA-256 | Build | Notes |
|---|---|---|---|
| `assets/legacy1.1/CSTORM.EXE` | `ce1ce2b9…bb67bc` | 1.10A, PE stamp 1996-12-17, `Created: Dec 13 1996 13:51:31` | **Proposed primary oracle.** PE checksum 0 |
| `assets/legacy1.0/CSTORM.EXE` | `c5d3cb87…3e73` | 1.00A, PE stamp 1996-06-13 (`Jun 12 1996`) | Secondary oracle (same toolchain, earlier source) |
| `assets/CSTORM.EXE` (GOG) | `b9d950fe…b181` | 1.1 + store patch | Runtime evidence only. Differs from 1.1 in 1,127 bytes: 123 in BEGTEXT, 31 in DGROUP, `WINMM.dll`→`_INMM.dll` import, PE checksum, 972 in `.rsrc` |
| `assets/legacy1.1/CWARSDLL.DLL` | `723f7417…290b` | PE stamp 1996-09-26 | Proposed DLL oracle |
| `assets/legacy1.0/CWARSDLL.DLL` | `403836f8…6787` | PE stamp 1996-06-14 | Secondary |
| `assets/CWARSDLL.DLL` (GOG) | `9f5ec131…44f9` | 1.1 + 3-byte patch at file 0x15F8A | Runtime evidence only |
| `assets/_inmm.dll` | `2e69f95d…c251` | 2019 GOG WINMM wrapper | Not part of the game; out of scope |

Not yet obtained: the July 1996 demo `cstorm.exe` (652.5 KiB, 1996-07-02) and its
`cwarsdll.dll` (1996-06-14), on discmaster (`Fantasy Games.iso/cybstorm`, also `PCPP #7.iso`,
`The Other Thing.iso`). A third independent build between 1.0 and 1.1.

## CSTORM.EXE: format and toolchain

- Win32 GUI PE, image base 0x400000, sections `BEGTEXT` (0x88600 raw), `DGROUP` (0x1D000),
  `.bss` (0x6D800 virtual), `.idata`, `.edata`, `.reloc` (0xB064 of base relocations), `.rsrc`.
  No overlay, no packer, no debug directory, virtual sizes 0 (WLINK style).
- Linker field 2.18 = WLINK. Runtime banner `WATCOM C/C++32 Run-Time system … 1988-1995`:
  matches local `watcom-10.5` and `watcom-10.6`, excludes 10.0/10.0a (1988-1994) and 11.0.
  **Runtime = 10.6** (2026-11-09): 17 `CLIB3S` members are exact only with 10.6, none only with 10.5
  (`tools/libscan.py --compare`). Stack-convention libraries (`CLIB3S`, `PLIB3S`, `MATH387S`, `MATH3S`).
- **Compiled as C++**: exports are Watcom C++ mangled (`W?MainWndProc$n(pnvuiuil)l`,
  `W?AppAbout$n(...)`), runtime string `compiler error: eliminated virtual function call!`,
  classes such as `GFXCDSSurface::create`. Source files are named `.c`, so probably `wpp386` on
  `.c` files (hypothesis).
- Stack calling convention with caller cleanup. First reproduced profile: `wpp386 -4s -s -oxt`
  (see `docs/toolchain.md`); other modules use other flags.
- `.reloc` gives a base relocation for every absolute address, so strict relocation binding is
  cheap for this target.
- Imports: KERNEL32, USER32, GDI32, WINMM; WinG (`WING32.DLL`), DirectSound, DirectPlay loaded
  dynamically; Sierra SIGS (`SierraNW.dll`, `SNWValid.dll`) in 1.1.

## Embedded source paths (strongest structural evidence)

145 `__FILE__`-style paths in DGROUP of 1.1 (used by `debug\assert.c`-style reporting), e.g.
`shell\shell.c`, `sim\simmain.c`, `terr\trender.c`, `main\WIN\winmain.c`, `graph\gfx\g_cds.c`.
Per directory: sim 37, shell 20, terr 16, pop 11, logic 11, multi 10, main 8, graph 8, core 8,
gui 7, io 4, misc 2, pack 1, noise 1, debug 1. Their order in DGROUP is a candidate TU link order.
Full list: extract with `strings` and `grep -E '\.(c|cpp|asm|h)$'`.

## CWARSDLL.DLL: format and toolchain

- Different toolchain: Microsoft linker 2.55, sections `.text/.bss/.rdata/.data/.idata/.edata/.rsrc/.reloc`,
  MSVC C++ mangled exports (`?CWDLL_MessageBox@@YAKPAUHWND__@@PADKH@Z`, 10 exports), statically
  linked MFC (`AfxWnd`, `AfxFrameOrView`), CRT string `runtime error`.
- Points at Visual C++ 2.x with MFC 3.x (hypothesis). `C:\tools` has VC5/VC6 and Intel only;
  VC 2.x would have to be obtained. Mostly MFC dialogs: low value, high library mass.

## Online search (2026-11-08)

| Query / place | Result |
|---|---|
| Web: "Missionforce: Cyberstorm" source code / decompilation / reverse engineering | Nothing found |
| Web: CyberStorm 2 Corporate Wars source / decompile | Nothing found |
| Web: Dynamix Darkstar / Tribes 1 engine source | No credible release found. Torque (Tribes 2 era) is MIT but later and unrelated |
| Web: `GFXCDSSurface` / `g_cds` | Nothing found |
| github.com/juanitogan/rbxit (+ wiki) | Win7 patch: modifies CWARSDLL, re-encodes RBX audio, RBX pack/unpack scripts. "v1.2" is a 2007 fan-modded CSTORM.EXE, **not** a Dynamix build |
| github.com/open-siege (Siege Studio) | Dynamix 3Space format tools; no CyberStorm mention, no code reconstruction |
| discmaster demo copies | Demo EXE/DLL exist (see above). Not downloaded |
| Mac / console ports, symbol-bearing builds, betas | None found. Not proof of absence |

Not searched yet: CyberStorm 2 executable (sibling code, maybe same compiler), Hunter Hunted /
Earthsiege 2 (same studio and era, possible shared `gfx`/`io` libraries), Sierra/Dynamix SDK drops.

## Non-game code ranges (2026-11-09, lane s39)
`config/nongame.json` lists 35 code ranges that are not game C/C++: Watcom runtime members that libscan misses
(pure_err/vfun_err, sprintf callback, setefg, thrddata, stabthrw/chipd32/i64ts386 interiors, cinit/exit, ...),
WLINK import glue (162 FF25 thunks + 388 fill bytes), data misread as code (startup banner, itoa alphabet), and 12
hand-assembly routines (terrain blitters, unpackers, fixed-point trig, line clipping, VectorAtan; 2074 bytes).
Evidence: candidates/s39/REPORT.md, reproducible with build/s39/proof.py (32 member matches, 306 contextual checks).
funcmap marks them `lib` (runtime/glue/data) or `asm`; the C/C++ census and verify's denominator exclude them
(2693 -> 2417 game functions). Hand assembly stays future work (assembler path) and is reported separately.
Open: fold s39's libscan gap proposals (G1-G18) into tools/libscan.py so the identities come from the scanner.
