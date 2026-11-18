# Lane nr6 near-miss round

`FastBuffer::BuildFlags` reached STRICT at `00434e30` (288 bytes). All ten code extents and the initialized data in the candidate `tfast.c` passed the promotion dry run.

```text
python tools/promote.py candidates/nr6/terr/tfast.c --tu src/terr/tfast.c --as 'terr\tfast.c' --profile ot
src/terr/tfast.c: 10 strict functions (1026 bytes), new 1, lost 0
```

## BuildFlags

The original keeps the item pointer at `[esp+0x14]` and the offset into the 12-byte terrain records at `[esp+0x10]`. The baseline reversed those slots at seven instructions. The change is local to the side loop. It skips an empty neighbor with `if (!cell) continue;`, then reads the neighbor's color into an `int` before comparing it to `currentColor`.

| Empty-cell guard | Neighbor color in comparison | Differing bytes |
|---|---|---:|
| Positive test | Read inline | 7 |
| Early continue | Read inline | 7 |
| Positive test | Named local inside the guard | 7 |
| Early continue | Named local after the guard | 0 (STRICT) |

Earlier reports covered loop, declaration and color-expression spellings individually. The 36-way search crossed the guard/local pair with three direction calculations and all six legal orders of the four coordinate writes. The four controls above isolated the change with the baseline coordinate and direction expressions. Every other function in this TU remains STRICT.

## Open functions

The aligned listings place every baseline residue inside its own function. No differing byte in these five targets comes from a call or branch displacement to a size-shifted neighbor.

| Function | Earlier single levers and new combinations | Retained result and original behavior |
|---|---|---|
| `0042bcb0 DisplayZoomArea` | Earlier half-scale and adjust/copy spellings kept D9. Crossed typed view-field captures and staged division with both delta-store orders, aggregate subtraction and adjust exits (36 variants, best D9). Tested a by-value half-scale helper at the initial division and either zoom loop with three ring-count spellings (36, best D30). | 544 bytes, D9. At +0x3d the original loads height into EAX, copies it to EDX and divides by ECX. At +0x139 it loads and tests adjust in EDX. The candidate divides by EDI and tests adjust in EAX. |
| `0042cd90 AreaGetPoint` | Earlier staged division reached D2. Walk initialization orders, loop forms and divisor type had held there. Factored the radius calculation shared with `AreaIndexToPoint` and crossed division form, triangular loop and zero-index exit (18, best D2). Crossed center capture and first-index calculation with five walk search/exit forms (30, best D2). | 208 bytes, D2. The original loads 6 into ESI at +0x23 and uses ESI for signed division at +0x2f. The candidate uses ECX at both instructions. |
| `00433240 FogUpdateCell` | Earlier typed records and standalone object-ID captures kept D3. Crossed a typed 8-byte occupant accessor with optional lookup and nested object-index capture (18, best D3). Factored the guarded flag write and crossed its input with band calculation and circular-mask start expressions (24, best D3). | 352 bytes, D3. At +0xa0 the original loads the grid and its neighbor-table pointer through EDX, then the object ID into EAX. The candidate uses EAX for all three loads. |
| `0042f490 RayDrawSamples` | Earlier image/y captures and image addressing kept D9. Crossed initialization of a `RayPoint` screen cursor with direct/member coordinate increments (12, best D171). Passed the chosen image into whole-grid and single-row drawing helpers, varying parameter position and y capture (8, best D145). | 167 bytes, D9. The original loads `drawCursorImages` into EBX at +0x07 and y into EDI at +0x0d. The candidate loads y first. This accounts for the shifted relocation and all nine differing bytes. |

The four open candidates are byte-for-byte copies of their starting source. Each passed its full-TU dry run with zero losses. Family results are in `candidates/nr6/attempts.jsonl`. The full listings, variant templates and scores are under `build/nr6/`.
