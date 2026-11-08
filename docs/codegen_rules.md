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
| 10.5 vs 10.6 compiler | | identical bytes on StrPool::Lock variants | measured (does not discriminate yet) |
