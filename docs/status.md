# Status

Numbers come from `python tools/verify.py`; this file only summarises them.

- 2026-11-09: infrastructure bootstrap. 8 STRICT functions (265 bytes) in 2 TUs:
  `src/_unknown/tu_0047abc0.c` (timer, complete object, 7/7) and `src/misc/strpool.c` (partial, 1/7).
  Both also strict in 1.00A (`--secondary`). Game code ~3,233 functions / ~531 KB; 273 heuristic modules
  (76 with profile `ot` shape, 53 with original path names).

- 2026-11-09: second profile `plain` (`-4s -s`) found for unaligned modules. 20 STRICT functions (709 bytes)
  in 5 TUs; 19/20 also strict in 1.00A (the 1-byte `InputNop` cannot be located by search there).

- 2026-11-09 (end of day): 1,925 STRICT C/C++ functions, 278,913 of ~521,036 game bytes (53.5%) in 176 TUs;
  2,417 game functions after classifying non-game code (config/nongame.json: runtime members libscan misses,
  WLINK import glue, data misread as code). Hand assembly: 11 of 12 routines STRICT with Borland TASM32 -ml -m5.
  TU level (tools/tucheck.py, reported by verify): 55 of 176 objects COMPLETE. Link probe (tools/linkprobe.py):
  WLINK reproduces inter-object layout of adjacent complete objects; differences are explained by pick-first COMDAT
  placement and literals a neighbour does not yet emit. About 440 functions remain DIFF/parked; 127 of them differ
  in 16 bytes or fewer (lanes s48-s51). Register allocation is the dominant blocker (codegen_rules.md, r1/r2).

- 2026-11-09 (late): compiler release settled by measurement (11.0 and 10.0a lose most accepted functions; 10.6
  stays). Resources: `.rsrc` rebuilt from `src/res/` with WRC 10.6, 21 of 21 resources STRICT, section layout equal
  except the bind stamp and 88 bytes of original slack garbage (UNKNOWN).

## Next
- Near-miss functions (s48-s51), TU completion (s52), pdeath object split.
- Image level: startup/BEGTEXT and library member order, .rsrc, then a full link once enough objects are complete.
- Name the assert function, resource manager and other hubs from strings and call counts.
