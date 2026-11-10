# Worker brief: object structure (names, boundaries, data ownership)

For lanes that do not hunt function matches but make each published file equal ONE original object, under its real
name, with its data. Read `docs/worker_brief.md` (rules, loop, Watcom facts) and the rows of
`docs/codegen_rules.md` from "object boundaries from static initialization" to "`delete[] p`" first. Worked
examples: `src/sim/mapalert.c` + `src/sim/target.c`, `src/sim/simout.c` / `simmini.c` / `simgame.c`,
`src/pop/pherc.c`, `src/shell/dbrief.c`.

## Evidence
- `python tools/objcensus.py -v`: XI records (one per object with static initialization; the target is the FIRST
  function of that object), path strings in link order with the accepted TU that carries each, and for every path
  string without a TU the accepted TUs whose code uses the data right after it.
- `python tools/hexdump.py START END [--dwords]`: oracle bytes of a data range (`*` marks relocated pointers).
- `_DATA` of one object, in order: 12-byte module header (only objects with static destructors), tables declared
  above the path string, the path string (`static char xSourceFile[] = __FILE__;`, kept even when unreferenced),
  then the object's other initialized data. The next object's header or leading tables follow directly.
  So in the census line `P  data used by: A (1 items...); B (2 items...)` the EARLIER object in code order (A) is
  normally P's object and B is the next object using its own header/leading tables. Check with the bytes.
- Code and `_DATA` are both in link order: between two named objects, k objects and k path strings pair up in order.
  If the counts differ, a published file covers several objects or an object has no path string: do not guess.
- The 12-byte function `mov eax,[esp+4]; mov dword [eax+0x10],0; ret` right before a static initializer is the
  inline destructor of the PRECEDING object's screen class (last function of that object), never the first
  function of the following one.

## What "done" means for one object
1. Candidate `candidates/<lane>/<dir>/<file>.c` named after the path string, compiled `--as 'dir\file.c'`.
2. Every function that is STRICT today is still STRICT (no loss), at the same VA.
3. `_DATA` is STRICT as one segment and its range contains the path string's oracle address: define the object's
   own initialized data in the file (values from the oracle) instead of `extern` declarations of them. CONST2 and
   XI stay STRICT. If an item's owner is unclear, leave it and say so; do not pad with invented variables.
4. Dry run passes: `python tools/promote.py candidates/<lane>/dir/file.c --tu src/dir/file.c --as 'dir\file.c'
   --profile P --from src/_unknown/tu_XXXXXXXX.c` prints `takes N functions from ... (file is retired)` and
   `lost 0`. `--from OLD` lets the candidate claim functions currently accepted in OLD at the same VA; without it
   the gate refuses ("already matched"). Use the old file's profile or flags (`recovery.json`, key `tus`).
   `tools/try.py` takes `--flags "..."` for TUs recorded with explicit flags.

## Cases
- **Rename**: an `_unknown/tu_*.c` file that is exactly one object with a path string: steps above.
- **Cut**: a published file that covers several objects (an XI target inside its range, or several path strings
  each with own data). One candidate per object, each with `--from` the same old file; together they must take
  every function. A static initializer written as a named function (`void f(void) {}` at an XI target) becomes the
  real construct: a global object whose class has an empty inline constructor (see simmini.c).
- **Destructor ownership**: move the 12-byte destructor to the preceding object's file as that class's in-class
  destructor (the compiler emits it last); promote the receiving file with `--from` the other file, and the other
  file without the function using `--accept-loss NAME`. List the two commands in that order.
- **Hand-written static initializer** (a function calling `PopObjectConstruct`/`HercModuleConstruct`/... and then
  constructors one by one, or wrapper objects doing so; `#pragma aux NAME "W?..."` aliases for constructors):
  replace by the real global objects with constructor arguments (mapalert.c, pherc.c). The compiler then generates
  the initializer, module header, destructor table (CONST2) and XI record; all must be STRICT. Destructor
  addresses are in the oracle's CONST2 table (pairs object, destructor after a count).
- Never keep both an old and a new definition of a function across files; never change a function's code beyond
  what the structure needs. Function matching is not this lane's job; note chances you see, do not chase them.

## Traps
- Shell: write Python helpers to files instead of heredocs when they contain backslashes; quote `--as 'dir\file.c'`
  in single quotes (inside double quotes `"dir\\$f.c"` becomes `dir$f.c`).
- A file's as-name changes `__FILE__`: asserts must use the path array (or `__FILE__`) and stay on their lines.
- `tools/promote.py` without `--publish` never writes; you may run it as often as needed. Never pass `--publish`
  or `--retire`.

## Report
`candidates/<lane>/REPORT.md`: one line per candidate, exactly as run, starting with
`python tools/promote.py candidates/<lane>/...` (dependency order: receivers before givers), followed by its summary
line; then per object: path string, code range, the evidence that identifies it (census line, bytes), and what
stays open. List objects you could not identify with what would decide them.
