# nm1 near misses

All three targets remain open. Six families covered 367 full-TU variants. Every variant that tied its target's baseline was rebuilt with `try.py --all` and produced the same target listing. The retained candidate files are byte-for-byte copies of the current source files. All three dry runs report `new 0, lost 0`.

| Target | Retained extent | Differing bytes | Family results |
|---|---:|---:|---|
| 00434aa0 TerrainBlendHeight | 304 B | 4 | Moving the base or delta across the helper boundary and varying the sum gave a best of 4 in 27 variants. Crossing the strength clamp, height/target capture order, result join and helper return width gave a best of 4 in 120. At +df through +eb, cs11 reads the old height into ECX, subtracts it from EAX, and forms the base product in EDX with `imul edx,ecx,100`. The candidate reads the height into EDX and multiplies there. |
| 00437c68 ActorTick | 367 B | 8 | A value-returning minimum crossed with argument order and indexed-field capture reached 13 in 24 variants. Crossing fixed-point update order and delta negation with the owner of the clamp result and capture of the updated value reached 8 in 52. At +b9 and +ff, both versions compare EDI with EDX. Cs11 emits `jb`, copies EDX into EDI and stores EDI. The candidate emits `jae`, copies EDI into EDX and stores EDX. Each site accounts for three bytes. |
| 00480bf0 GFXCDSSurface::setPalette | 750 B | 5 | With the late DC capture, 72 combinations of outer traversal, independent low/high scans, loop bounds and comparison order reached 5 only with the original loops. A further 72 combinations crossed the two flag-clear store orders, loop origin and bound, and sharing of the high range's base across API, clears and scan. Best remained 5. Cs11 keeps the destination cursor at ESP+400h and the high scan start at ESP+404h. The candidate exchanges them at instructions +15a, +161, +170, +185 and +213. |

## ActorTick shared exit

Both branches land at 004376e6, offset +98h in the STRICT 160-byte ActorGainExperience. Relative to ActorTick's start, the oracle target is −582h and the candidate target is −57eh. The only intervening size differences are ActorHealthCost at 00437848 (116 of 118 bytes) and ActorMoraleCost at 004378e5 (94 of 96 bytes). Together they account for the four-byte distance change and the single differing displacement byte in each branch, at +46h and +110h. The six clamp bytes are local to ActorTick.

## Promotion dry runs

```text
python tools/promote.py candidates/nm1/_unknown/tu_004349c0.c --tu src/_unknown/tu_004349c0.c --as '_unknown\tu_004349c0.c' --profile ot
src/_unknown/tu_004349c0.c: 4 strict functions (227 bytes), new 0, lost 0

python tools/promote.py candidates/nm1/_unknown/tu_00435dec.c --tu src/_unknown/tu_00435dec.c --as '_unknown\tu_00435dec.c' --profile plain-oe
src/_unknown/tu_00435dec.c: 36 strict functions (3594 bytes), new 0, lost 0

python tools/promote.py candidates/nm1/graph/gfx/g_cds.c --tu src/graph/gfx/g_cds.c --as 'graph\gfx\g_cds.c' --profile ot-oe
src/graph/gfx/g_cds.c: 5 strict functions (896 bytes), new 0, lost 0
```

The [attempt ledger](attempts.jsonl) records one result per family. [Triage](../../build/nm1/triage.md), full baseline listings (`*.base`), six variant templates and their complete rankings (`*.scores`), and dry-run output (`*.gate`) are in `build/nm1/`. The scratch launcher runs the repository tools with temporary directories and live candidate-note discovery scoped to nm1.
