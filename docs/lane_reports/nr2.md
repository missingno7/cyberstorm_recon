# Near-miss round nr2

**SimUseItem is STRICT at 0044f030 (160 bytes).** Its default switch arm now breaks to the existing final `return result;`. That one change puts `item + 19` in EDX and the unit receiver in EAX when calling `PopFindItem`, matching the original six operand and staging bytes at +1d through +2d. The candidate preserves every other code extent's status and byte-difference count in `simdo.c`. The dry run reports one new hypothesis binding, `ConfigureMovement = 00432930`.

```text
python tools/promote.py candidates/nr2/sim/simdo.c --tu src/sim/simdo.c --as 'sim\simdo.c' --profile ot-oe
src/sim/simdo.c: 31 strict functions (5698 bytes), new 1, lost 0
```

## Six open targets

The counts below come from comparing the retained full-TU candidates with the pinned 1.10A oracle. The differing local instructions account for every reported byte. None of the seven targets depends on a neighboring function's size for its baseline difference.

| Function and VA | Final extent | What the original does and what nr2 tried |
|---|---|---|
| ActorListWrite `004563d0` | 384 B, 2 differ | At +f4/+f7 the original loads and pushes the unit buffer in ECX. The retained candidate uses EDX. A typed stream local crossed with allocation assertions and buffer access regressed. Advancing the index at entry acquisition, coupled with custom-pilot and mission-unit accessors, did not improve the baseline. |
| AnimBitmapFrame `00459a30` | 1072 B, 3 differ | The FX cache reload, test and assert push at +6d/+77/+7b use EDI. The retained candidate uses EAX. Testing the FX table directly with a literal-zero assert and naming the HRC fallback reload reproduced EDI. That diagnostic moved all five remaining differing bytes into the HRC path: its assert used EAX instead of EDX and its mode reload used EDX instead of ECX. All six case-2 store orders and both case-0 frame/group store orders were tried on that spelling. The three-byte baseline was better. |
| HotbriefCheckAction `00462710` | 528 B, 4 differ | The inlined ranking loop puts its rank limit at ESP+10h and unit pointer at ESP+18h. The candidate swaps those slots at four instructions. Nesting the returned point in the first distance call preserved the four-byte result. Call-site guard/return forms and the computed equipment row crossed with actor accessors also left the best result at four. |
| SimDrawVLine `00451c70` | 128 B, 6 differ | The original schedules `lea edx,[eax+6]` at +38 before four MOVSDs. Both clip-store orders, changes to the guard and the timing of the snapshot and signed division still left the best LEA after the copy. The bound canvas prototype alone preserved that schedule. Explicit memcpy for either direction regressed. |
| SimDistanceToDepot `00454180` | 141 B, 7 differ | At +39..+52 the original uses ECX for the input actor's position base and EBX for the depot's. A break-then-compute search, a continue guard, and native GridPoint arguments with staged or reversed component subtraction left the best candidate with opposite register assignments. |
| SimApplyLeadership `004507d0` | 352 B, 4 differ | The original subtracts two in ESI, then multiplies ESI by ten into EDX. The retained candidate uses LEA into EDX and multiplies EDX. Destructive subtraction with a continue guard recovered SUB but displaced the actor and distance registers (30 bytes differ). Capturing the pilot or its return value and changing the post-loop pilot reads did not close that gap. |

All seven final dry runs reported `lost 0`. The six open candidates are byte-for-byte copies of their input source files. The `simmisc.c` dry run used the existing placement `--at 'simmiscSourceFile=004a0474'` to verify the accepted data segment. Only the `simdo.c` command above is a promotion candidate.

[Per-family attempts](attempts.jsonl) · [Baseline triage and prior scopes](../../build/nr2/triage.md) · [SimUseItem switch variants](../../build/nr2/use_exits.log) · [Promotion output](../../build/nr2/simdo.promote)
