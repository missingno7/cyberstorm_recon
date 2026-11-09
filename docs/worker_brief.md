# Worker brief: matching CyberStorm functions

You reconstruct C++ source that Watcom 10.6 compiles to the exact bytes of `CSTORM.EXE` 1.10A. Read
`AGENTS.md`, `docs/verification.md` and `docs/codegen_rules.md` first. You own only the modules assigned to you.

## Rules
- Write only in `candidates/<lane>/` and `build/<lane>/`. Never edit `src/`, `tools/`, `config/`, `docs/`,
  `recovery.json`; never commit; never run `promote.py --publish`; never touch `assets/`.
- No inline asm, `_emit`, `#line`, `#pragma aux` with code bytes, byte arrays for code, copied machine code.
- Readable source: real types, structs, control flow. Names are labels you choose (prefer what strings,
  callers and data suggest); say which are guesses. Before naming a callee or global, run
  `python tools/symbols.py show VA`: if it already has a name, use that name (aliases are refused later).
- Only STRICT counts. Stop a hypothesis family after ~8 tries without new evidence; write what you tried.

## Loop
1. `python tools/show.py --module VA` — the module's functions, relocations, called symbols, string literals.
   `build/modules/cs11.json` gives the module's functions and whether all are 16-byte aligned (profile `ot`).
2. Write the whole module in order into `candidates/<lane>/<file>.c` (one object = one file; function order =
   address order). If the module has a path string, compile with `--as 'dir\file.c'` and keep every ASSERT on
   its original line (the line number is an immediate in the code).
3. `python tools/cc.py --profile ot|plain --as 'dir\file.c' candidates/<lane>/file.c -fo=build/<lane>/file.obj`
4. `python tools/compare.py build/<lane>/file.obj --find --imply -v` (or `--at Label=VA` for the first
   function). Fix DIFF extents one by one; earlier functions do not need to match for later ones to be placed.
5. `python tools/promote.py candidates/<lane>/file.c --tu src/dir/file.c --as 'dir\file.c' --profile ot|plain`
   (dry run). Report the dry-run output.

## Watcom facts that already cost time
- Profile `ot` (`-4s -s -oxt`) for 16-byte-aligned modules, profile `plain` (`-4s -s`) for modules with no
  function alignment (`tools/modules.py` prints the hint). A module that mixes both is two objects. Do not
  sweep flags blindly; `tools/flagsweep.py` is for a deliberate, recorded sweep.
- `x &= ~M; x |= v & M;` (two statements) gives the in-memory `and/or` shape; one expression does not.
- Pointer-walk loops (`T *e = table; ...; i++, e++`) and indexing (`&table[i]`) compile differently.
- A 1-byte function (`ret`) cannot be placed by search: use `--at`.
- C or C++ is per TU. Byte-width tests of `char` fields (`mov dl,[x]; test dl,dl`) mean the C front end
  (`--profile ot-c|plain-c`, plain C structs and functions); `xor r,r; mov rl,[x]; test r,r` or `movsx` mean
  C++. Global objects with constructors/destructors, `W?` names and default arguments mean C++.
- Use Watcom's `<windows.h>` for Win32 calls, not hand-written `__stdcall` prototypes.
- Never call Watcom runtime internals under invented names. `__wcpp_2_mod_register_`, `__wcpp_2_ctor_array_`,
  `__wcpp_2_dtor_array_*`, `W?$nwn`/`W?$nan` (new/new[]), `W?$dln`/`W?$dan` (delete/delete[]) are generated
  by the compiler from global objects, arrays of class objects, `new` and `delete`. Write those C++ constructs.
  `python tools/show.py` names library functions; anything named there is runtime, not game code.
- An EBP frame without locals can come from calling a function with default arguments (C++).
- `mov eax,[g]; test eax,eax` vs `cmp [g],0` can come from an inline accessor.
- Statement order is visible in store order; operand order in `a < b` vs `b > a` is visible.
- Struct layouts and global addresses are evidence: use `char pad[N]` only as a temporary, say so.

## Report (final message)
Module(s), STRICT functions with VA, remaining DIFF functions with the best hypothesis and what was tried,
implied bindings, new codegen facts (with evidence), and the candidate file path.
