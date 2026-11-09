# Watcom 10.6 codegen facts for this target

Each row: context, trigger, effect, status, where. Add rows; replace a row when it is narrowed.

| Context | Trigger | Effect | Status |
|---|---|---|---|
| `wpp386 -4s -s -oxt` | `-ot` | functions start on 16-byte boundaries inside the object, filler `8d4000`/`8bc0`/`8d8000000000`/`8d542200`…; `_TEXT` paragraph-aligned so the next object starts on 16 (WLINK fills with zeros) | verified (timer TU) |
| same | `-4` vs `-5` | `-5s` schedules differently and drops the `-ot` alignment shape | verified (timer TU) |
| any | import call | `call dword ptr cs:[__imp__X]` (2E FF 15) — Watcom's own NT headers | verified |
| `-4s -s -oxt` | struct store order | `a=f(); t->x=a; t->y=a+d; t->z=d;` compiles to stores x,z,y; source order x,y,z is the original (TimerSet, TimerNext) | verified |
| C++ | callee has default arguments (`Load(long id, int a=1, int b=0, int c=0)` called as `Load(id)`) | caller gets a standard EBP frame (`push ebx; push esi; push ebp; mov ebp,esp`) and parameters via `[ebp+x]`; writing the arguments out gives no frame | verified (StrPool::Lock) |
| C++ | member of a global object read through an inline accessor (`gResMgr.IsOpen()`) | `mov eax,[g+off]; test eax,eax` instead of `cmp dword ptr [g+off],0` | verified (StrPool::Lock) |
| `-of` | traceable frames | `push ebp; mov ebp,esp` *before* the register saves — not this program's frame shape | measured negative |
| asserts | `AssertFail(__FILE__, __LINE__, 0)` at 0047d5a0 (label) | the line number is an immediate: the ASSERT must sit on its original source line | verified (strpool line 59) |
| `-d1` | debug line info | no code change | measured (timer TU) |
| `-4s -s` (plain) | `x &= ~M; x \|= v & M;` on a struct member | read-modify-write in memory: `and word ptr [eax+4],imm16; or [eax+4],edx` (narrowed to byte/word when the mask allows). `x = (x & ~M) \| (v & M)` loads into a register instead | verified (input module) |
| plain | `T *e = table; for (i = 0; i < n; i++, e++)` | `mov edi,table; xor esi,esi; jmp test; inc esi; add edi,size` — pointer initialised before the index, index incremented first. Indexing `&table[i]` gives the reverse order | verified (ply module) |
| plain | loop shape | `for` loops compile as `init; jmp test; body...; test: cmp; jcc body` with no `-ol` needed | verified |
| C++ | global object with constructor/destructor | the file's static initializer calls each constructor and bumps a per-module state counter (`mov [state],1/2/3…`); destructors are registered in a DGROUP table (e.g. 0048c830) and an empty destructor is `mov eax,[esp+4]; ret` | verified (Rate) |
| options | combined letters like `-obmilr`, `-obmiler` | rejected by 10.6 `wpp386` (no object): sweeps that list them silently skip them | measured |
| module boundaries | an `ot` object whose code ends on a dword boundary followed by a `plain` object | no zero fill between them: `tools/modules.py` cannot see the boundary (e.g. io\memory.c 0047eb00 + object at 0047eb98) | observed |
| C vs C++ front end | `char` field tested against 0 | `wpp386` with an explicit comparison (`if (c == 0)`) widens: `xor eax,eax; mov al,[x]; test eax,eax`; a bare truth test (`if (!c)` / `if (c)`) gives the byte form `mov ah,[x]; test ah,ah` in C++ too. `wcc386` gives byte tests for both. So a byte test alone does NOT prove C; only an explicit `== 0` compiled to a byte test does. ioile.c and strpool reproduce as C++ with bare truth tests (b1, A/B in candidates/b1/REPORT.md) | verified (narrowed 2026-11-09) |
| asserts | `if (f->open == 0) AssertFail(__FILE__, __LINE__, (char *)f->open)` vs `unsigned long v = f->open; if (v == 0) AssertFail(..., (char *)v)` | the member-expression form makes Watcom keep extra values in ESI/EDI (prologue saves ESI/EDI); the scalar-capture form gives the original EBX-only prologue. The original ASSERT macro evaluates its expression into a temporary and passes that value as the third argument (or 0 / a message) | verified (ioile.c: 4 functions, decisive A/B in candidates/b1/REPORT.md) |
| runtime | C++ library | the game links the multithreaded C++ runtime (`PLIBMT3S`/`PLBXMT3S`: 24 exact members vs 13 for `PLIB3S`; operator new is the 99-byte MT version at 0047ebcc). `-bm` itself changes no code in the tested TU | verified (library identity) |
| Win32 API | hand-written `__declspec(dllimport) __stdcall` prototypes | use Watcom's `<windows.h>`; its pragmas define the call's register effects | recommendation |
| `-oe` (profile `plain-oe`) | small functions defined in the TU | inlined at every call in the TU (even calls before the definition) while an uncalled out-of-line copy stays in definition order. Needed by multi\linkdlg/linkq/message (o1), harmful in lsys/psystem/hercbay/missbrf: per-TU | verified (o1, integrator sweep) |
| switch lowering | ~22-24 case values on distinct labels | `repne scasw` scan table; extra values sharing the `default:` label add values but not ranges and tip it to a compare tree | verified (o1, synthetic files) |
| struct copy | `T *d = &m->x; *d = *src;` vs `m->x = *src;` | pointer form emits `lea edi` before `mov ecx` (scalar temps in edi); member assignment emits `mov ecx` first | verified (o1: 3 functions) |
| 16-bit locals | `unsigned short x = 0` in a 4-byte slot | dword store; the original word store `mov word ptr [esp],dx` needs `unsigned short x[2]; x[0] = 0;` | verified (o1, MessageChecksum) |
| tail merging | identical function tails | later functions jump into earlier functions' tails: every function in between must have its original size first | observed (o1) |
| aggregate locals | several struct/array locals | stack layout follows declaration order and scope | verified (o1, MessageJoinRequest) |
| 10.5 vs 10.6 compiler | | identical bytes on StrPool::Lock variants | measured (does not discriminate yet) |
