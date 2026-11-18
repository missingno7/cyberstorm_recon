# Lane nr1 near-miss round

`HercFreePower` gained a 144-byte STRICT match. `ResetMultiplayerInput` gained two bound operands and remains four bytes from a match. The other five targets retain their starting candidates. All four dry runs reported zero losses. No new symbol binding was proposed for promotion.

## Changed files and dry runs

```text
python tools/promote.py candidates/nr1/_unknown/tu_00438374.c --tu src/_unknown/tu_00438374.c --as '_unknown\tu_00438374.c' --profile plain-oe
src/_unknown/tu_00438374.c: 25 strict functions (2145 bytes), new 1, lost 0
```

```text
python tools/promote.py candidates/nr1/_unknown/tu_00476ddc.c --tu src/_unknown/tu_00476ddc.c --as '_unknown\tu_00476ddc.c' --profile plain-oe
src/_unknown/tu_00476ddc.c: 3 strict functions (196 bytes), new 0, lost 0
```

The second command covers a declaration correction only. Both changed files preserve every other function's status, size and differing-byte count. The actor-state and trajectory candidate files are byte-identical to their starting files. Their control dry runs reported 36 and 7 strict functions respectively, both with `new 0, lost 0`.

## Per-function findings

The baseline comparisons used the full translation units, the assigned profiles and the `cs11` oracle. None of the seven byte counts includes a displacement caused by a neighbouring function's size. At `ActorRecoverInjuries+0x4c` both conditional branches land at `+0x50`. The opcode differs. Two other functions also have unresolved operand identities, detailed below.

| Function | Baseline → final | Prior scope, new experiment and remaining evidence |
|---|---|---|
| `00439218 HercFreePower` | 144 B, 3 → **STRICT** | Earlier work captured the slot, the item pointer and the power update. Capturing the byte used for the comparison as `int type = p->detail.bytes[41]` closed it with the existing counted loop. Of nine combinations, only that one produced `lea ebx,[edx+0x13]` at +0x58, copied EBX to EAX and read the slot byte through EAX. The type byte and comparison with 4 are visible in the original. |
| `00476ea0 ResetMultiplayerInput` | 558 B, 4 → 4. Unbound operands 6 → 4 | Earlier key, mask and ID captures retained the swap. Sharing the separator/text line builder with alternative ID branches, then testing loop exits with network flag and mask widths, both bottomed out at four bytes. At +0x11b the original saves ID in EDI and text in EBP. The candidate reverses the two copies and consequently the pushes at +0x184/+0x185. The declaration corrections below are retained. |
| `0045b0b0 TrajectoryEffects` | 320 B, 9 → 9 | Earlier trail-store permutations and default joins reduced the diff to nine. This round crossed all six trail-store orders with predicate captures and both particle-store orders, then tested a particle setter that resolved both defaults. Best results were 9 and 292 bytes respectively. The original preloads kind/rate/param/part/value into AL/DL/CH/DH/CL and writes kind first. The retained version uses CH/DH/AL/DL/CL and writes param first. See the callee evidence below. |
| `0043718a ActorEffectiveSkill` | 179 B, 3 → 3 | Earlier selection and scale spellings left the tenfold skill product in EDX. Staging that product and the sixfold morale penalty together with alternative final caps retained the three-byte minimum. Sharing the tenfold operation with `ActorQuery` and varying midpoint capture increased the best diff to 67. The original loads `[ebx+0x1e]` into EAX at +0x12 and uses EAX in the copy and addition. |
| `0045b4a0 TrajectoryDirection` | 672 B, 2 → 2 | Earlier sum order and square captures left the ADD/PUSH pair open. Comparing the live projected position against the saved snapshot with asymmetric square expressions also bottomed out at two. All 120 orders of the five independent scaling stores were crossed with nested vs local distance tests. The squares reach EDX and EAX as expected, but only the original accumulates in EDX and pushes it at +0x21c. |
| `00438085 ActorRecoverInjuries` | 130 B, 3 → 3 | Prior clamp orientation and row/maximum captures had failed independently. Crossing the signed update paths with injury branching and loop-increment placement retained three bytes. A const-reference minimum crossed with row capture had a best diff of 112. The original compares EDI with EAX, skips the copy with JAE and stores EAX. The retained candidate uses JB and stores EDI. |
| `004375f4 ActorTrainSkill` | 90 B, 3 → 3 | Prior typed address, factor capture and compound-update trials left the two address terms swapped. Moving training onto the skill record gave a best diff of 78. Keeping the calculation in the caller and crossing the index guard, pointer/reference, scale capture order and signedness, and rate staging retained three. The original shifts the index in EAX, computes the array base in EBX and then adds into EBX. The candidate exchanges the two initial homes. |

## Operand identities

- `ResetMultiplayerInput+0x22` calls `0047c920`. Using the signature in `src/io/resource.c` binds `ResMgr::Load`. The buffer conversion supplies the matched API's `int *` parameter. The dword load at +0x99 reads the first member at `004d2460`. The candidate uses the existing `ModeSystem::IsActive` declaration from `src/main/WIN/winmain.c`, a working type already bound at that address. Each change preserves the code residue.
- Four global references in the menu function are still unbound. Their indicated addresses are `optionNetwork=004aebf0`, `specialOptionFound=004dd0b4`, `specialOptionEnabled=00494290` and `specialOptionSelection=004c9748`. The symbol database has no entries at those addresses. They remain diagnostic observations.
- `TrajectoryEffects+0xa6` calls `0045fcd0`, the matched `ExploAddPart(char,int)` in `src/sim/explo.c`. The retained caller declares `(int,int)`, which leaves the call unbound. Substituting the matched prototype alone makes Watcom reload the low byte from `[ebx+0x5c]`, zero-extend it and push EAX. The original pushes the already-loaded ECX. The experiment produced 320 B with 179 differing bytes and shifted relocation sites. The nearer candidate retains the unresolved declaration.

[Structured attempt records](attempts.jsonl) give the bounds and best result of each family. Full listings, variant templates, scores, operand audits and dry-run transcripts are in `build/nr1/`.
