# Verification and proof levels

## Oracle
- `config/oracles.json` pins each file by SHA-256; `config/locks/<oracle>.json` pins headers, per-section
  hashes, the ordered base-relocation stream and its set (`tools/pe.py lock|check`). `tools/pe.py census`
  accounts for every file byte (no overlay; WLINK writes `.bss` with SizeOfRawData≠0 and PointerToRawData=0,
  which the loader treats as no file bytes).
- Primary `cs11` = 1.10A. Secondary `cs10` = 1.00A: `verify.py --secondary` locates each accepted TU there
  by masked search and reports how many functions are also strict (informational, not counted).

## STRICT function (the only counted unit today) — `tools/compare.py`
A function = one *extent* of a compiled object: from its public (Watcom emits LPUBDEFs for statics too) to the
next public or the segment end, including compiler alignment filler and jump tables. It is STRICT when:
1. it is placed at a VA by a bound symbol, `--at`, or a unique fixup-masked search;
2. every byte equals the oracle after applying every fixup;
3. every fixup target is resolved: imports through the oracle IAT, Watcom runtime through library identity
   (`tools/libscan.py`, exact bytes + relocation sites), other symbols through `config/symbols.json`, data
   through a data-segment base implied consistently by all fixups *and whose initialized bytes match*, or —
   while a TU's data layout is still incomplete — per item (the oracle target holds exactly the candidate's
   bytes for that literal/object);
4. the set of HIGHLOW base relocations inside the extent equals the set of absolute fixup sites;
5. the extent ends where the original function ends (next function start); the last extent of a segment may
   end before zero linker fill, or — partial TU only — before original compiler filler, whose bytes are then
   not counted (`tail_fill`).
UNBOUND = all bytes equal but some fixup target unbound; DIFF = any byte, relocation or extent difference.

Soundness note (2026-11-09): Watcom references into a segment as `SEG + 0` with the item offset in the
fixup's data field. Both the code-segment resolution and the per-item data proof use offset = disp + data-field
addend. An earlier item proof ignored the addend and compared the segment's first item, so a wrong literal could
pass (found by lane b1 with a negative control; no accepted function was affected; test_gate now covers it).

## Bindings — `config/symbols.json`
`matched` (written by promote for strict functions), `lib`, and `hypothesis`: an extern whose VA was implied
by a byte-equal caller (`--imply`): unique across the TU, plausible (function start / data section), never
contradicting an existing binding. A hypothesis becomes `matched` when that symbol's own bytes are proven.
Labels are readable names (`Class::method`); mangled names are recorded but a type disagreement between TUs
only flags `~mangle`, it does not unbind.

## Gate — `tools/promote.py`
Dry run by default. Policy check -> fresh compile (pinned toolchain) -> comparison with `--find --imply` ->
no previously accepted function may lose STRICT, VA or size (unless `--accept-loss`, logged) -> no symbol
or VA conflict. `--publish` (integrator only) writes `src/`, `recovery.json` and `symbols.json`.

## Not yet built (next proof levels)
- TU level: whole object incl. CONST/_DATA ownership and BSS order (Watcom BSS order rule is a hypothesis
  from DOS projects: size rank, groups of 25, `hashpjw % 241`).
- Image level: WLINK of all objects + CLIB3S/PLIB3S/MATH387S, compare full file.
