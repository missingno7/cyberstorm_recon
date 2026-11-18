# ro2 invariant reopen round

**SimDrawQueue at 00455220 is STRICT over 384 bytes.** The other eight functions remain DIFF. Only `sim/simactor.c` differs from its source copy. All six final translation units passed the protective dry run. This report offers one file for integration.

## Promotion

```text
python tools/promote.py candidates/ro2/sim/simactor.c --tu src/sim/simactor.c --as 'sim\simactor.c' --profile ot-oe
src/sim/simactor.c: 7 strict functions (1680 bytes), new 1, lost 0
```

`DrawActionBody` now initializes `x` and `y` from the two screen coordinates before subtracting `zoom` from each. The original single-expression initializers kept the marker in BL and spilled x. The two-step source keeps the byte marker at `[esp+0x10]` and x in ECX, giving the original `add eax,ecx` at +f8. The public `SimDrawAction` remains STRICT. The first dry run identified the existing `ClipApply(ClipState *)` binding at 0047b1c0. Using that declaration and call removed the alias from the final dry run. The reported data implication is `simSmallClip = 004d3d64`, consistent with the existing address.

## Reopen decisions and results

The nine baseline compiles reproduced the requested byte differences. The middle column identifies prior isolated results and the coupling selected from the project levers. Counts are differing bytes unless the row says STRICT.

| Target | Prior evidence and new test | Final state and remaining original behavior |
|---|---|---|
| SimRandomFire · 0044f290 | Focus snapshots and helper returns had been tried separately. Crossed assignment versus terminal return with first-shot flag type, then changed reply traversal together with nested readiness/fire versus the public helper. | DIFF 144, emitted 640 of 656 B. On the first value-producing fire the original tests readiness in EDX, clears EAX on failure and jumps to the result store. Our second expansion already has the required instruction sequence there after accounting for the earlier displacement. |
| SimExecuteItem · 0044ebe0 | Coupled the previously isolated slot-address and cost-helper forms. Also tested both orders of the two seed stores and case 10 loop post-increment with the payment guard. | DIFF 103, 1104 B. The original adds 0x98 to EDX at +62 and loads and tests the cost in ECX at +280/+283. The candidate uses LEA for the slot and compares the cost in memory. |
| SimDrawTimer · 00454440 | Direct assignment on each timer path previously repaired the join but changed later registers. Crossed this with override selection, time division and remainder staging, then with color selection and the flashing predicate. | DIFF 12, 608 B. The original assigns EBX on each timer path. The best candidate joins in EAX and copies to EBX. Direct branch assignment gives 24 differences even with the original color logic. |
| ActorFinishMove · 00455890 | On the corrected pitch checkpoint, crossed reuse of the clamped pitch with pan/volume store ordering. Coupled unit and tile accessors with the previously ineffective cost capture. | DIFF 23, 560 B. The original loads the tile into EDI at +ff and the first ammo-query unit into EBP at +11c. The candidate uses ESI and EDI. The pitch and pan register differences also remain. |
| SimDrawQueue · 00455220 | The point constructor had recovered the frame size. Crossed marker form and target type with how the two coordinates are loaded and adjusted. A separate dispatch/index/zoom matrix retained the baseline or made it worse. | **STRICT, 384 B, previously 251 different.** Splitting the coordinate reads and subtractions closed the function with the existing byte marker. |
| SimDrawFrame · 004554c0 | Earlier offset/index updates and remap reordering had failed separately. Coupled index placement with loop dispatch and clip/remap order. Also moved the opening clip copy and tested destination, source-reference and console-address captures. | DIFF 16, 558 counted B. The final TU still exchanges EBP and EDI for count and queue offset. The original stores zero to the remap before pushing the console address at +1f5. The candidate pushes first. |
| SimDrawVLine · 00451c70 | Combined split inset division with staging either or both bounds and both store orders. Tested a single clip save-and-set helper with precomputed bounds and global or reference receiver. | DIFF 6, 128 B under the explicit flags. The original has the upper-bound LEA at +38 before four MOVSD instructions. The best candidate schedules it at +3c after the copy. |
| SimUseItem · 0044f030 | Default-first switch and isolated receiver/index captures were known. Coupled nested lookup/kind selection with the slot adjustment, then varied receiver lifetimes at the subsequent mass and movement calls. | DIFF 6, 160 B. At +1d the original uses EDX for the item index and EAX for the focus/unit receiver. The candidate exchanges those registers. |
| ActorListWrite · 004563d0 | Coupled zero-source allocation and assert lifetime with buffer capture. Separately crossed source selector capture and direct/accessor unit reads with the final block receiver. | DIFF 2, 384 B. At +f4/+f7 the original loads and pushes the stream buffer in ECX. The candidate uses EDX. The store to the default pilot already agrees. |

## Evidence

[Structured attempts](attempts.jsonl) records each of the 18 families. The corresponding `.log` files under [build/ro2](../../build/ro2/) list the ranked full-TU variants. The nine `.asm` files contain the original and candidate listings. Five files tested against the oracle remain byte-identical to their canonical source copies. For the unchanged `simmulti.c` dry run, the documented `--at 'simmultiSourceFile=004a0494'` anchor placed the 15-byte `_DATA` segment and yielded `new 0, lost 0`.

