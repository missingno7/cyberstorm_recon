# Working contract (cyberstorm_recon)

Goal: readable C/C++ source for MissionForce: CyberStorm (Dynamix, 1996) that Watcom C/C++ 10.6 compiles back
to the original bytes of `CSTORM.EXE` 1.10A. Proof is strict and per function first, then per TU, later image.

## Hard rules (enforced by the tools)
- Oracles are pinned in `config/oracles.json` and hash-checked on every load. `assets/` is read-only, never
  committed, never patched, never run. Primary: `cs11` (1.10A). `cs10` (1.00A) is corroboration only.
  `gog` (store-patched 1.1) and `CWARSDLL.DLL` are not matching targets.
- `recovery.json` is the only acceptance ledger; only `tools/promote.py --publish` writes it (and the
  `matched`/implied entries of `config/symbols.json`). Progress numbers come only from `tools/verify.py`.
- Only STRICT counts (definition in `docs/verification.md`). UNBOUND, masked, "similar" are diagnostics.
- Compile only through `tools/cc.py` (pinned, hash-checked Watcom 10.6, reduced environment).
- `src/` must not contain inline asm, `_emit`, `#line`, `#pragma aux` code bytes, copied machine code or byte
  arrays standing in for code. `tools/gate.py` rejects the obvious forms; the rest is review.
- Record negative results (`docs/codegen_rules.md` for compiler facts, `docs/blocked.md` for stuck functions).

## Layout
- `src/<dir>/<file>.c` — one file per original TU, named after the embedded `__FILE__` path
  (`sim\sim.c` -> `src/sim/sim.c`, compiled `--as 'sim\sim.c'`). TUs with no path evidence:
  `src/_unknown/tu_<start VA>.c`. Names of functions/types/globals are working labels unless evidence says
  otherwise; `__LINE__` values are evidence (assert calls must sit on their original line).
- `tools/` — the only tools; `config/` — pins (oracles, locks, toolchain, symbols); `docs/` — method and status.
- `build/`, `candidates/` — scratch, ignored. Recreate `build/` with `python tools/setup.py`.

## Commands
```text
python tools/setup.py                         regenerate build/ (locks, library identity, function/TU/module maps)
python tools/modules.py                       work list: modules, alignment profile hint, path names, matched count
python tools/show.py --module VA              disassembly of one module with relocations, symbols, strings
python tools/cc.py --profile ot|plain --as 'misc\strpool.c' SRC.c -fo=build/LANE/x.obj
python tools/compare.py build/LANE/x.obj [--at Label=VA] [--find] [--imply] [-v]
python tools/promote.py SRC.c --tu src/DIR/FILE.c --as 'DIR\FILE.c' --profile ot [--at Label=VA]   (dry run)
python tools/promote.py --retire src/DIR/OLD.c   integrator: drop a combined file before promoting the real objects
python tools/verify.py [--secondary]          fresh rebuild of every accepted TU
python tools/try.py SRC.c --as A --profile P  compile + compare + aligned instruction diff (worker loop)
python tools/variants.py TPL.c --as A --profile P --fn F   test all choice-point combinations
python tools/flagsweep.py SRC.c AS Label=VA   deliberate flag sweep (diagnostic)
python tools/test_gate.py                     self-tests of the proof machinery
python tools/accept.py candidates/L/REPORT.md [--publish]   integrator: re-run a report's promotions
python tools/commit.py "msg"                  integrator: commit only if verify and test_gate pass
```

## Roles
One integrator (the main session) owns `src/`, `tools/`, `config/`, `docs/`, `recovery.json`, commits and
`--publish`. Workers get disjoint modules, work in `candidates/<lane>/` and `build/<lane>/`, run dry-run
promotions only, and report results. Brief: `docs/worker_brief.md`.
