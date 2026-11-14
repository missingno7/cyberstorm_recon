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
3. `python tools/try.py candidates/<lane>/file.c --as 'dir\file.c' --profile P [--at Label=VA]` — compile +
   compare + an aligned original-vs-candidate instruction diff of every non-strict function in ONE command
   (`--fn Name` to focus, `--context N`, `--all`). Use this instead of cc/compare/objdis/show.
4. When you have several plausible spellings, test them in one go: put choice points in a copy of the file
   (`@{a@|b@}` inline, or `//@{` ... `//@|` ... `//@}` blocks) and run
   `python tools/variants.py tmpl.c --as ... --profile P --fn Name [--write best.c]`. Alternatives must be
   natural spellings, never register-steering tricks.
5. `python tools/promote.py candidates/<lane>/file.c --tu src/dir/file.c --as 'dir\file.c' --profile ot|plain`
   (dry run). Report the dry-run output.

## Watcom facts that already cost time
- Profile `ot` (`-4s -s -oxt`) for 16-byte-aligned modules, profile `plain` (`-4s -s`) for modules with no
  function alignment (`tools/modules.py` prints the hint). A module that mixes both is two objects. Do not
  sweep flags blindly; `tools/flagsweep.py` is for a deliberate, recorded sweep.
- Profile `plain-oe` (`-4s -s -bm -oe`): if uncalled copies of small functions exist and their bodies appear
  inlined at call sites in the same TU (multi\ files), the TU was compiled with `-oe`.
- `x &= ~M; x |= v & M;` (two statements) gives the in-memory `and/or` shape; one expression does not.
- Pointer-walk loops (`T *e = table; ...; i++, e++`) and indexing (`&table[i]`) compile differently.
- A 1-byte function (`ret`) cannot be placed by search: use `--at`.
- C or C++ is per TU. Prefer C++ (wpp386, profiles ot/plain) unless the bytes need C. Note: a bare truth test
  of a `char` (`if (!c)`) gives byte-width tests in C++ too; `if (c == 0)` widens in C++. Global objects with
  constructors/destructors, `W?` names and default arguments mean C++.
- Asserts: the original macro evaluates the asserted expression into a scalar temporary first
  (`unsigned long v = f->open; if (v == 0) AssertFail(__FILE__, __LINE__, (char *)v);`). Using the member
  expression twice adds ESI/EDI saves to the prologue. Diffs at +1..+3 in assert-bearing functions: try this.
- Use Watcom's `<windows.h>` for Win32 calls, not hand-written `__stdcall` prototypes.
- Never call Watcom runtime internals under invented names. `__wcpp_2_mod_register_`, `__wcpp_2_ctor_array_`,
  `__wcpp_2_dtor_array_*`, `W?$nwn`/`W?$nan` (new/new[]), `W?$dln`/`W?$dan` (delete/delete[]) are generated
  by the compiler from global objects, arrays of class objects, `new` and `delete`. Write those C++ constructs.
  `python tools/show.py` names library functions; anything named there is runtime, not game code.
- An EBP frame without locals can come from calling a function with default arguments (C++).
- `mov eax,[g]; test eax,eax` vs `cmp [g],0` can come from an inline accessor.
- Statement order is visible in store order; operand order in `a < b` vs `b > a` is visible.
- Struct layouts and global addresses are evidence: use `char pad[N]` only as a temporary, say so.

## Before touching a parked function (the memory of this project)
1. `python tools/notes.py show LABEL_OR_VA`: its residue class, every structured attempt with its verdict, and every
   report line that mentions it. If the verdict is `invariant` or the class is REG-SWAP / SLOT-SWAP / HOMES with
   several reports, leave it unless you have NEW evidence (a different expression shape, a field type, a callee).
2. Residue classes (`python tools/parked.py`): SHAPE = wrong size, the source says something else: rebuild the body
   from the original listing. SAME-SIZE = right size, one construct off: find that instruction. HOMES / REG-SWAP /
   SLOT-SWAP = same instructions, different homes: needs a different record set, rarely yields to respelling
   (docs/watcom_playbook.md). WAIT-SIZE / WAIT-TABLE = fix the neighbour. ASM = hand assembly, not C.
3. Compiler facts: read `docs/codegen_rules.md` fully (verified core, short). `docs/codegen_rules_lanes.md` holds
   150+ rows reported by lanes and not re-verified: search it for your symptom, do not read it through.
   `docs/watcom_playbook.md` is the symptom -> what-to-try table from two other Watcom projects.
4. When you finish a hypothesis family on a function, also when it failed, record it:
   `python tools/notes.py add --lane L --va VA --label NAME --hyp "..." --result "..." --before N --after N
   --verdict open|invariant|dead-end|fixed`. One line per family, not per compile. This replaces long prose in the
   report: the report then only needs the promote commands and what is new.
5. `tools/promote.py` prints `REVIEW twin helpers` when an inline helper's body nearly equals another function's.
   A second spelling of one helper that exists only to move bytes is rejected; do not build on it.

## Structure work
Lanes that rename files to their path strings, cut combined files or complete `_DATA` follow
`docs/structure_brief.md` (census tool, `promote.py --from`, what "done" means for an object).
`tools/try.py` accepts `--flags "-4s -s ..."` for TUs recorded with explicit flags; `tools/hexdump.py` dumps
oracle data.

## Report (final message)
Module(s), STRICT functions with VA, remaining DIFF functions with the best hypothesis and what was tried,
implied bindings, new codegen facts (with evidence), and the candidate file path.
