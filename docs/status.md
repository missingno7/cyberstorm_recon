# Status

Numbers come from `python tools/verify.py`; this file only summarises them.

- 2026-11-09: infrastructure bootstrap. 8 STRICT functions (265 bytes) in 2 TUs:
  `src/_unknown/tu_0047abc0.c` (timer, complete object, 7/7) and `src/misc/strpool.c` (partial, 1/7).
  Both also strict in 1.00A (`--secondary`). Game code ~3,233 functions / ~531 KB; 273 heuristic modules
  (76 with profile `ot` shape, 53 with original path names).

- 2026-11-09: second profile `plain` (`-4s -s`) found for unaligned modules. 20 STRICT functions (709 bytes)
  in 5 TUs; 19/20 also strict in 1.00A (the 1-byte `InputNop` cannot be located by search there).

## Next
- TU-level proof (data ownership, BSS order) and an image-level WLINK experiment.
- Name the assert function, resource manager and other hubs from strings and call counts.
