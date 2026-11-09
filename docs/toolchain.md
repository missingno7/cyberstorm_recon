# Toolchain

## Evidence (CSTORM.EXE 1.10A)
| Fact | Evidence | Status |
|---|---|---|
| Watcom C/C++, WLINK | `BEGTEXT`/`DGROUP` sections, linker field 2.18, Watcom runtime banner | verified |
| Runtime = Watcom **10.6** | `tools/libscan.py --compare` 10.5 vs 10.6 `CLIB3S`: 228 vs 211 exact+sites members; 17 members exact only in 10.6, 0 only in 10.5. Banner "1988-1995" excludes 10.0/10.0a/11.0 | verified (runtime release) |
| Stack calling convention (`-3s/-4s/-5s`) | runtime matches `CLIB3S`/`PLIB3S`/`MATH387S`/`MATH3S`, not the `R` libraries; caller-popped stack args | verified |
| C++ front end (`wpp386`) | Watcom C++ mangled exports `W?MainWndProc$n(pnvuiuil)l`, C++ runtime members (`__wcpp_2_*`), class names in strings. Files are named `.c` | strong (C vs C++ per TU not yet discriminated) |
| Game compiler = 10.6 `wpp386` | timer TU strict with 10.6; 10.5 produces the same bytes on the tested function, so the *compiler* release is not separated yet | reproducing configuration |
| Profile `ot` = `-4s -s -oxt` | timer TU 7/7 strict; `-5s`, `-ox` (no `-ot`) and `-ot` alone each fail; `-d1` no effect | verified on 1 TU |
| Profile `plain` = `-4s -s` (no `-o`) | modules without function alignment (~197 of 273 heuristic modules, ~76% of game code): rate module 4/4 (complete object), ply 2/2, input 6/6. On the input module `-ox`, `-or`, `-ot`, `-os`, `-oxt` and `-3s` each fail; `-oe/-oi/-ol/-on` and `-5s` give identical bytes (undecided; `-4s` kept for consistency with `ot`) | verified on 3 modules |
| `-bm` (multithread) in every profile | gui\menu.c: 56/61 strict with `-bm`, 53 without; the 25 other accepted TUs give identical results with and without it (232 = 232); runtime is the MT C++ library `PLIBMT3S` | verified 2026-11-09 |
| Profile choice per module | 16-byte-aligned function starts with no-op filler -> `ot`; no alignment at all (functions start right after `ret`) -> `plain` | verified on 5 modules |
| WLINK `system nt_win`, `option stack=48k` | probe link (build/link1, 2026-11-09): WLINK 10.6 reproduces linker field 2.18, OS 1.11, subsystem 3.10, characteristics 0x8182, heap 0x2000/0x1000, the 128-byte MZ header + "Windows NT windowed executable" stub byte for byte, section order BEGTEXT/DGROUP/.bss/.idata/.edata/.reloc with identical characteristics, VirtualSize 0, `.bss` raw size with no file offset; `option stack=48k` gives the original 0xc000/0xc000. WLINK 10.5 also writes 2.18 (not a release discriminator). Still to reproduce: `.rsrc` (resource compiler), object/library order, BSS/DGROUP layout. WLINK must run with stdin closed (it waits for input otherwise) | verified (headers) |
| `__FILE__` relative paths | strings like `sim\sim.c`: the original compiled from the project root with relative paths. `tools/cc.py --as` reproduces this | verified on strpool |

## Installs (pinned in `config/toolchain.json`)
- `wc106`: `C:\tools\watcom-10.6` (archive.org ISO, see rayman_recon pins). NT loaders `BINNT\*.EXE` run the
  `BINW` payloads natively. Keep compile paths short (scratch in `build/cc`).
- `wc105`: `C:\tools\watcom-10.5-pck` (ISO tree with the truncated `BINW\WCC386.EXE` replaced from the packed
  set). A/B probes only.

## CWARSDLL.DLL (deferred)
Microsoft linker 2.55, MSVC C++ mangling, static MFC. Points at Visual C++ 2.x (hypothesis); not in `C:\tools`.

## Hand assembly (2026-11-09)
`config/nongame.json` classifies 12 ranges (2074 bytes) as hand assembly (ENTER/LEAVE frames, string
instructions, register idioms a C compiler does not emit). Profile `asm` assembles `.asm` sources with the pinned
WATCOM 10.6 WASM (`-4s -q`) through tools/cc.py; the comparator and gate are the same as for C. Update (same day):
WASM is rejected: it encodes register-register instructions in the r/m-destination form (89/01), the original
uses the register-destination form (8B/03) in 162 places (s46). Borland TASM32 with `-ml -m5` (profiles
`asm-tasm40`, `asm-tasm50`, pinned) reproduces all 12 routines byte for byte (RenderClipLine by s49); `-m5` (multi-pass) is required,
single-pass TASM reserves near jumps for forward references and pads with NOPs. TASM 4.0 and 5.0 give identical
bytes, so the version stays undecided (5.0 is used: 1996, same year as the game). MASM 6.x is untested. Gate policy for
`.asm`: no binary inclusion, no data blobs (4+ comma-separated db/dw/dd values) unless a line is labelled
UNKNOWN debt. verify counts accepted asm TUs separately from the C/C++ census.
