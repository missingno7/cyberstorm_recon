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
6. table ownership (2026-11-11): without `-ot` Watcom puts a switch table in FRONT of its function, so it lies
   at the end of the previous public's range, behind 0..3 filler bytes that depend on the absolute address. Such
   a trailing table that the extent itself does not reference (`foreign_table`) is not part of that extent: the
   extent ends at its own last byte (`end`; `full_end` keeps the range to the next public), and the original must
   continue with at most 3 filler bytes and whole relocated dwords up to the next function start. The table is
   proven by the function that jumps through it (an external jump table must be STRICT at the oracle address,
   audit v1 F3) and is addressed backwards from that function's start. Effect: a function no longer fails
   because the NEXT function's switch is wrong or because earlier functions shift the table's alignment filler;
   table and filler bytes are not counted in any function's size. test_gate covers both directions.
UNBOUND = all bytes equal but some fixup target unbound; DIFF = any byte, relocation or extent difference.

Soundness note (2026-11-09): Watcom references into a segment as `SEG + 0` with the item offset in the
fixup's data field. Both the code-segment resolution and the per-item data proof use offset = disp + data-field
addend. An earlier item proof ignored the addend and compared the segment's first item, so a wrong literal could
pass (found by lane b1 with a negative control; no accepted function was affected; test_gate now covers it).

Audit v1 (2026-11-09, candidates/v1/REPORT.md): six gate defects found with negative controls and fixed:
F1 own initialized objects referenced through their extern names are proven (bytes + own fixups + relocations);
F2 every reference into initialized data requires the whole segment proven at its base or the referenced item
proven at its address, and recorded data segments must stay STRICT (promote, verify, rollback); F3 a jump table
outside the referencing function's extent is proven as part of that function (length from the candidate's
consecutive table fixups); F4 an extent must start at an original function start; F5 a size loss is a loss;
F6 C-linkage names carry no kind (kind comes from the target section). Follow-ups: references whose mangled
name differs from the bound one (overloads, e.g. copy constructors) are no longer resolved by readable label;
weak/lazy externs (OMF WKEXT/LZEXT comment classes A8/A9) resolve to their default; single-function search hits
must lie within the object's anchored address window (identical code elsewhere is not identity).

Audit v1 round 2 (same day): R2.1 NUL-shortened proofs only for anonymous literals and char arrays (a named
integer 64 = `40 00 00 00` was proven over 2 bytes); R2.2 zero (BSS) objects and BSS references must be zero in
the oracle; R2.3 external jump-table length comes from the oracle (bounded by the next known table/function
start) and the candidate must supply every entry; R2.4 search identity = within 0x2000 of an anchor (not the hull
between anchors); R2.5 local-symbol fallback needs an exact owner and the overload check; R2.6 weak/lazy defaults
resolve through the full proof path with a cycle guard. 16 functions of multi\link/linkapp/chat that referenced
a fake zero array over initialized data were withdrawn (docs/blocked.md). test_gate covers F1-F5 and R2.1/R2.2.

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
Object splits: `--retire src/DIR/OLD.c` removes a combined file from the ledger (its matched symbols become
hypothesis bindings, the loss is logged, the file is deleted) before the real per-object files are promoted; a
function named with `--accept-loss` may be re-accepted at another VA inside its own TU (e.g. a static initializer
that moves when the file is cut at the real object boundary).

## TU COMPLETE (diagnostic level, reported by verify) — `tools/tucheck.py`
A TU is COMPLETE when, in verify's view: every extent of its main code segment is STRICT and contiguous at
base + object offset (no tail_fill); the object occupies the original range exactly (base is an original function
start, every original start inside the range is one of its extents, and the bytes after it up to the next start are
zero fill); every initialized data segment with bytes (CONST, CONST2, _DATA, XI/YI) is STRICT as a whole segment;
a BSS segment is placed (base implied consistently by references); and every placed CONST/CONST2/_DATA/_BSS/XI
segment lies on the longest increasing sequence of that segment name over objects sorted by code base (WLINK
concatenates same-named segments in object order, so an outlier sits in another segment's region); and for two
objects adjacent in code, the bytes between their CONST/CONST2/_DATA segments are only alignment zeros (found by
the link probe: literals between two objects belong to one of them). Not claimed at this level: COMDAT copies (the
linker keeps one copy per name, pick-any, so an object's own copy may differ or be discarded; image level proves
them), BSS size and the order of unreferenced BSS items (zero bytes cannot prove them).
Placement aids (compare.py, deterministic, also in verify's view): XI/YI records and COMDAT data are placed by
their exact bytes with resolved pointers (unresolved pointer fields masked; at least one resolved; unique in
DGROUP); own BSS objects referenced by name imply the BSS base like other data.

## Not yet built (next proof levels)
- TU level, remaining: BSS order/size (Watcom BSS order rule is a hypothesis from DOS projects: size rank,
  groups of 25, `hashpjw % 241`), CONST literal layout for TUs whose CONST is still proven per item.
- Image level: WLINK of all objects + CLIB3S/PLIB3S/MATH387S, compare full file.

Link-order evidence (2026-11-09, `tools/linkorder.py`, verify's view, proven segments only): with TUs sorted by
code base, _BSS (72 TUs), XI (62) and CONST2 (38) bases are monotonic: one object order explains them. CONST (41)
and _DATA (55) are monotonic except for small "CONST" segments that the original image holds inside the _DATA
range (hercbay 0048eb00, hotseat 0048f1cc, flic 0048f35c, menu 0048f3f0, tfast 00490660, simturn 004a030c, link
004a3eec, assert 004a4a34, file 004a4b9c, memory 004a4ba8, unpack 004a54c0; mostly 10-16 bytes, i.e. path
strings). Their bytes are proven where the code points, but in the original those bytes belong to the object's
_DATA (e.g. an initialized char array, not a literal): the source spelling must change before the TU/image level
can claim them. Two _DATA inversions (tu_0040f4e4 vs tu_0040ffb8, tu_0047d0e0 vs io\file.c) are open.

Own data placements (2026-11-09). A TU's own data/BSS objects referenced only from new functions get their
address from the segment base placed in the dry run (anchored by --at/search and the code's relocation fields).
promote.py --publish records those addresses as `hypothesis` bindings ("own data placed in TU"), so the verify view
(no --at, no search) can reach the objects; verify still proves every item (BSS zero in the oracle, data bytes and
relocations equal). Before this, such functions were withdrawn as "strict only with --at/search" (s15 tu_0041cb48).

## Resources (2026-11-09, `tools/rsrc.py`, reported by verify)
Source: `src/res/cstorm.rc` (WRC script; menu, About dialog, string table, accelerators, version info) and readable
pixel text for the icon and the seven cursors (`*.ico.txt`, `*.cur.txt`, generated to .ico/.cur by tools/resgen.py).
Policy: no RCDATA, every file a statement names must have a pixel-text source. Build: WRC 10.6 (DOS build under the
pinned DOSBox-X, `config/toolchain.json` runners/resources) compiles at the recorded build clock (the only field that
depends on it is the version block's file date); WLINK links a one-function stub; WRC binds the .res into it, as the
original build did. Proof: each resource (type, name, language) STRICT by bytes; the whole `.rsrc` section equal
after rebasing data-entry RVAs, except (a) directory TimeDateStamps = bind time, an image-level clock field, and
(b) 88 slack bytes after the last resource (0x1ba8..0x1c00) that the original binder filled from uninitialized
memory: explicit UNKNOWN debt, no copy elsewhere in the file. Result: 21 of 21 STRICT, layout equal.
Corroboration: 1.00A holds the same resources byte for byte except the menu, cursor 3 and the version date, plus
eight dialogs (ids 106..130) that 1.10A dropped.

## Image-level link probe (2026-11-09, `tools/linkprobe.py`)
COMPLETE TUs that are adjacent in the original (only zero fill between them) form 11 runs (27 TUs). Each run is
compiled fresh and linked alone with the pinned WLINK 10.6 (`system nt_win`, `option undefsok`, objects in original
order); each object's segments are located in the linked image by their own bytes (fixups masked, unique hit) and
compared with the original as offsets from the run's first object. Result: 33 of 41 relative offsets equal; all 8
differences are explained:
- 4 code offsets (runs 0 and 6): WLINK places a COMDAT function directly after the code of the first object in link
  order that defines it (UpgradeBar/UpgradeArrow destructors, 0x86 bytes -> 0x88 aligned; FileRecord constructor,
  0x12 -> 0x14). In the original an earlier object outside the run defined them first, so they are absent there.
  This confirms pick-first COMDAT placement inline in object order: a full link reproduces it naturally.
- 4 CONST offsets (runs 3 and 5): the original has literal bytes between two adjacent objects' CONST that neither
  reproduced object emits ("**********" x2 after linkdlg.c; "#:\CSTORM\", "CYBDATA1..4.RBX" after tu_00475dc8.c).
  They belong to one of the neighbours (unused or differently spelled literals), so those TUs are not truly
  complete; the TU check now tests that code-adjacent objects' data segments abut (57 -> 55 complete).
Confirmed: inter-object code fill/alignment and segment concatenation of CONST/_DATA follow object order as WLINK
does it; no unexplained linker behaviour in the probed runs.

## Image link model (2026-11-09, evidence from funcmap/libscan/modules/linkorder)
- Entry point 0047fe98 = Watcom startup `cstrtwwt` (CLIB3S); the code section is named BEGTEXT because the startup's
  (empty) BEGTEXT segment heads class CODE, as in the build/link1 header probe.
- BEGTEXT has two parts:
  A. 00401000..0047aae0 (~498 KB): explicit object files in makefile order, grouped by directory: shell (shell,
     pilotgui, commscrn, missbrf, hercbay, hercbuy, hbupgrad, mplaunch, hotseat), gui (flic, menu), terr (tgrid,
     tarea, tsurface, tfog, tfast), pop (psystem, pherc, pdeath), sim (sim, simgame, actorlst, actoradd, worldact,
     anim, animbmp, target, apply, explo, simhot), logic (logic, lactor, lactors, miss, bestloc, intel, lgen,
     lsys), multi (link, linkapp, linkdlg, chat, mstart, message, minipkt), main\WIN\winmain.c last; unnamed
     objects sit between them. No runtime member lies inside part A (one 16-byte `_getmbcp` hit at 004765b1 is
     suspect).
  B. 0047aae0..end (~59 KB): library members in WLINK's on-demand resolution order: 26 engine modules
     (misc\strpool, io\resource, debug\assert, io\file, io\memory, graph\gfx\g_cds, pack\unpack, timer, input,
     palette, ...; ~23 KB) interleaved with Watcom runtime members (~35 KB). So the engine was a project library
     linked with LIBRARY like CLIB3S/PLIBMT3S/MATH387S, not as FILE objects.
- Consequence for the image proof: link = FILE <part-A objects in this order> + LIBRARY <engine lib>, CLIB3S,
  PLIBMT3S, MATH387S (order to be tested). Part B's order is WLINK's resolution order and can only be reproduced
  once every object and the engine library's member set exist; probes on part-A runs are possible earlier.
- Several part-B engine modules (debug\assert, io\file, io\memory, pack\unpack) keep their literal data in the
  _DATA region (004a4a34..004a54c0), unlike the C++ modules: different compiler/options or char-array spelling;
  open for the TU level (tucheck flags them as segment-order outliers).
- .rsrc (7 KB) is not reproduced yet (resource compiler and script unknown).
