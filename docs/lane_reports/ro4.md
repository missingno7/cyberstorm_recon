# ro4 reopen round

The 624-byte function at **004418a9** is newly STRICT in a promotion dry run. All 15 final TU copies passed their dry runs with **lost 0**. Three other candidates reduce the target's differing-byte count. The acceptance ledger was not published.

## Promotion dry runs

```text
python tools/promote.py candidates/ro4/core/mcphmisc.c --tu src/core/mcphmisc.c --as 'core\mcphmisc.c' --profile plain-oe
src/core/mcphmisc.c: 13 strict functions (2749 bytes), new 1, lost 0

python tools/promote.py candidates/ro4/graph/gfx/g_cds.c --tu src/graph/gfx/g_cds.c --as 'graph\gfx\g_cds.c' --profile ot-oe
src/graph/gfx/g_cds.c: 5 strict functions (896 bytes), new 0, lost 0

python tools/promote.py candidates/ro4/noise/noise.c --tu src/noise/noise.c --as 'noise\noise.c' --profile plain-oe
src/noise/noise.c: 20 strict functions (3596 bytes), new 0, lost 0

python tools/promote.py candidates/ro4/terr/tsurface.c --tu src/terr/tsurface.c --as 'terr\tsurface.c' --profile ot-oe
src/terr/tsurface.c: 16 strict functions (1792 bytes), new 0, lost 0
```

## Changed candidates

| VA and function | Baseline → final differing bytes | Finding |
|---|---:|---|
| 004418a9 · f_004418a9 | 4 → **0 STRICT**, 624 B | Each arm selecting the report's final string now calls DebugTextAppend. This reproduces the earlier three-byte LEA into EBX at +2c and the global load into EAX at +2f. In the 16-way control, all four versions with the original index guard and the separate appends were STRICT. Splitting the index guard lost the match. The new implied bindings are reportBreak_0048b1e6 at 0048b1e6 and g4c933c at 004c933c. |
| 00480bf0 · GFXCDSSurface::setPalette | 9 → **5**, 750 B | Capture the window DC immediately before updating currentPalette and pass it to RealizePalette. This gives the original counter register at +1a0 and DC argument at +2d5. The five remaining displacement bytes exchange the two induction-pointer slots at ESP+400h and ESP+404h. |
| 0046bb20 · SoundCreateBuffer | 143 → **123**, 599/602 B | Use one reference to the caller's sound-buffer pointer from creation through Lock, Restore and Unlock. This removes the second load of the output slot. The original releases the resource through ESI and retains the sample base across the first memcpy. The candidate is three bytes short. |
| 00431a60 · SurfaceNoiseAt | 106 → **51**, 160/160 B | Capture both coordinate products, then compute the linear hash term and amplitude-scaled sample in one inline helper. The original four register saves, extent and final clamp now agree. Coordinate registers, scheduling of the polynomial and sample multiplication still differ. The unresolved recursive caller f_0042fc90 shrinks from 5984 to 5632 B against a 7456 B target. The loader's assertion lines and all accepted extents are preserved. |

An alternative signed-sample helper gives SurfaceNoiseAt 144 B with 74 differing bytes and grows the recursive caller to 6224 B. Both tradeoffs are reproducible in the [terrain comparison](../../build/ro4/terrain_review/results.json).

## Other targets

These rows give the final candidate extent and differing bytes. A smaller experimental number is identified separately when it matters for the next attempt.

| VA and function | Final | What the original does and what the reopened families established |
|---|---:|---|
| 0043c5d6 · f_0043c5d6 | 279 B, D44 | Loads the window handle after the title lookup and uses EDX/ESI for rectangle heights. Combining the name copy with the prompt and staging y before x reached D35, but the converter and seven other accepted functions lost STRICT. |
| 0043d591 · f_0043d591 | 288 B, D248 | Saves EDI and shares failure/return tails with 0043c5d6. The name-copy prompt with staged height and split y update reached 289 B/D214 and lost thirteen accepted functions. Across the 24 follow-up geometry/prompt combinations, the only loss-free version retained both dialog baselines. |
| 00441208 · BriefLookup | 299/297 B, D120 | Repeats category comparisons in memory, then adds progress to the DL-derived cost. Initializing a cost accumulator and adding progress produced 297 B/D2, with only the mission-record address registers different. It lost the accepted 93-byte MissionIsBlocked, whose standalone body became 91 B. Caller-only store/decoding changes did not preserve that improvement. |
| 0047b2d0 · f_47b2d0 | 208 B, D17 | Uses ECX in the height multiply and ESI for the window argument. Permuting all three descriptor stores and all four pixel-pointer store groups, with pitch/traversal variations, retained D17 at best. |
| 0047bad0 · MenuFill | 336/320 B, D95 | Keeps border right in EBP and bottom on the stack. Endpoint recomputation, clipped rectangle references and a four-edge outline helper did not remove the extra instruction byte that pushes the epilogue over the alignment boundary. |
| 0047bc10 · SurfaceRemapRect | 96/80 B, D38 | Uses JBE at entry, an unconditional inner back edge to the column test, and exits through MenuFill. Varying both row and column control flow, including a row-mapping helper, did not recover the shared exit. |
| 00425e1a · GuiScreen::Draw | 172 B, D31 | Copies the loop index into EDX for the hidden test, then into EDI for clip and draw. Separating the lookups, changing receiver captures and testing all 24 GUI-global store orders did not recover both homes. |
| 00426c72 · Flic::DecodeRegion | 61 B, D4 | Preserves left/top in EDI/EBP across unpack. Sharing the frame decoder's region operation and using an advancing coordinate cursor or copied header gave D6 at best. Separate unpack/copy wrappers did worse. |
| 0047e610 · IoClose | 96 B, D4 | Keeps current/end positions in ESI/EDI. Joining direct and seek arms on one result and passing the saved position into an inline measure-and-restore operation left the best D4. |
| 0042cd90 · AreaGetPoint | 208 B, D2 | Uses ESI for divisor 6. Triangular sum variants crossed with walk setup, followed by all 120 orders of five independent walk-field assignments, retained D2 at best. |
| 0042caf0 · BestLocInitialize | 64 B, D58 | Adjusts EDX by +4, writes the name at [EDX−4], then subtracts 4. Passing dimensions by address produced the adjusted store through EAX at D57, but retained the original base in EBX and overlapped the existing radius setter. The one-point layout evidenced by AreaRecord and constructor/base variants did not recover the full sequence. |
| 00433240 · FogUpdateCell | 352 B, D3 | Walks grid and neighbour base in EDX, then loads the object ID into EAX. Visibility store/guard coupling and an optional-object ternary with nested identity checks did not change the best three bytes. |
| 0042f490 · RayDrawSamples | 167 B, D9 | Loads the image global before the y argument at entry. Screen-coordinate induction and indexing, then a per-row sample pointer, did not reproduce that order. |
| 00431a00 · SurfaceBuildNoiseTable | 96 B, D17 | Compares DI to 491 and jumps into subdivision at 004319ab. Coupling narrower outer bounds with sample loop rotation and factoring the signed average did not recover the shared exit. |

The [triage sheet](../../build/ro4/triage.md) identifies earlier single levers and the combinations selected for each address. The [49 family records](attempts.jsonl) reference lane-local templates and scores. Each address has a complete original-versus-candidate listing before and after the round under build/ro4, with suffixes .base and .final. Compiler and promotion scratch stayed inside that directory. The nested build/cc path preserves the comparator's translation-unit scope for local symbols.

