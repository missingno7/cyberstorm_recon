# ro1 reopen round

One new function passes STRICT against cs11. Four candidate TUs contain retained source changes. All nine TU dry runs report lost 0. D is the number of differing bytes reported by the comparator. Sizes below include the complete function extent. All results are dry runs.

## Changed files and promotion receipts

```text
python tools/promote.py candidates/ro1/_unknown/tu_0042d3d0.c --tu src/_unknown/tu_0042d3d0.c --as '_unknown\tu_0042d3d0.c' --profile ot-oe
src/_unknown/tu_0042d3d0.c: 2 strict functions (784 bytes), new 1, lost 0
```

```text
python tools/promote.py candidates/ro1/_unknown/tu_004349c0.c --tu src/_unknown/tu_004349c0.c --as '_unknown\tu_004349c0.c' --profile ot
src/_unknown/tu_004349c0.c: 4 strict functions (227 bytes), new 0, lost 0
```

```text
python tools/promote.py candidates/ro1/_unknown/tu_00435dec.c --tu src/_unknown/tu_00435dec.c --as '_unknown\tu_00435dec.c' --profile plain-oe
src/_unknown/tu_00435dec.c: 36 strict functions (3594 bytes), new 0, lost 0
```

```text
python tools/promote.py candidates/ro1/_unknown/tu_0045ad00.c --tu src/_unknown/tu_0045ad00.c --as 'tu_0045ad00.c' --profile ot-oe
src/_unknown/tu_0045ad00.c: 7 strict functions (824 bytes), new 0, lost 0
```

## Results by function

| VA | Function | Baseline to retained | Source evidence and remaining difference |
|---|---|---|---|
| `0042d3f0` | TerrainDrawOverlay | 592 B, D9 to STRICT | A TerrainView method takes the quotient by value and computes (6 - rotation + direction) % 6. The caller uses the result with the sprite remainder. Four of nine method/caller combinations matched, while staging the inverse rotation in the caller failed. +1a2 now materializes 6 in EDX, subtracts rotation, then adds EAX. The draw call binds to the existing assembly public DrawTerrainTileClipped at 00435c5e. The name-only pragma supplies its undecorated linkage. |
| `00434aa0` | TerrainBlendHeight | 304 B, D69 to D4 | A value-taking interpolation helper subtracts oldHeight from target, then scales target in place. It restored the product and rounding sequence. At +df through +eb, cs11 reads the old byte into ECX and forms the base product in EDX. The candidate reads it into EDX and multiplies there. Six formal/actual argument orders and the subsequent producer/capture variants retained a best of D4. |
| `00437c68` | ActorTick | 365 B, D277 to 367 B, D8 | A single inline fatigue operation includes both the delta update and zero floor. Emitting the injury >= step arm first reproduces the two injury update blocks. The final size is exact. Each inlined clamp still differs by three bytes at branches +b9 and +ff and the following moves/stores. The other two differences are displacements into ActorGainExperience at 004376e6. The two intermediate cost functions are each two bytes short. |
| `0045b0b0` | TrajectoryEffects | 320 B, D11 to D9 | Store trailParam, trailKind, then trailRate. Use an explicit default branch for partKind and a ternary for partValue. The candidate now loads value in CL as in cs11. Its kind/rate/param/part homes are CH/DH/AL/DL versus AL/DL/CH/DH, and the first two trail stores have exchanged order. All 120 five-field orders with each of two guard forms bottomed out at D11 with ternary defaults. |
| `0042d2b0` | SearchGetPath | 192 B, D76 | Coupled the separate predecessor cursor, return of its known zero value, and count order with a top-tested emitter. Reusing either cursor gave D142. The retained body places the decrement and test on a different edge, stores through [esi+4] in the loop, and puts the predecessor cursor in ESI where cs11 uses EAX. |
| `00434bd0` | TerrainAdjustHeight | 160 B, D17 | All 24 brush-store orders were crossed with predecrement/outer-1, then with inner search scope and local/global radius. Best D17. At +51 cs11 reads inner before the radius push, stores strength, then forms the search address in EAX. The retained candidate forms it earlier in EBX. |
| `00435700` | DrawTerrainMiddle | 304 B, D69 | Clip ternaries with staged destination offsets reached D138. Computing the pixel address in the loop with a consumed top parameter reached D98. The retained entry still loads image/x/top into EBP/EAX/EDX where cs11 uses ECX/EBX/EAX. |
| `00436c65` | ActorScale | 182 B, D137 | Switch exit and clamp combinations did not restore the common EDX result. An inline raw-rule-amount experiment for ActorWork gave this function D133 at 185 B, with an additional unplaced 79-byte helper. Kept diagnostic. Cs11 uses EDX for both selected values and returns via mov eax,edx. |
| `004372db` | ActorWork | 350 B, D187 | Initializing the lower Scale bound to one and conditionally replacing it gave 352 B/D103. Keeping the upper cap as written preserves every STRICT function, but moves standalone Scale to D146 and Rest to D151. The first clamp now loads 1 before the test. Cs11 copies the quotient first and loads 1 on the fallback arm. See build/ro1/workalt.best.c and ActorWork.workalt.gate. The submitted actor TU keeps the shared Scale baseline. |
| `0043743d` | ActorRest | 358 B, D143 | Clamp polarity crossed with named increments gave D139 and lost ActorFatigueCost. A D142 form passes the dry run but worsens SendEvent and ActorQuery. In the retained listing, the original follows the descriptor/rule chain in ESI at +5b where the candidate uses EAX. Tick branches and the shared exit into Work also differ. See build/ro1/restalt.best.c and ActorRest.restalt.gate. |
| `004375f4` | ActorTrainSkill | 90 B, D3 | Typed curve rows crossed with skill references and quotient/product staging, then reuse of count with current-value capture, did not exchange the two homes. Cs11 scales the index in EAX and forms the skill base in EBX. The candidate starts them in EBX and EAX. |
| `00437848` | ActorHealthCost | 116 B, D87 | Scale cap/exit polarity crossed with enclosing the curve in the unequal-stat arm, then one shared bound operation with a named curve ordinate, did not restore the 118-byte extent. Cs11 decrements EAX before selecting the maximum in EDX. The retained calculation uses lea edx,[eax-1]. |
| `004378e5` | ActorMoraleCost | 94 B, D64 | The same two cost families were measured in this caller. The original 96-byte function decrements EAX, tests it and copies the selected value into EDX. The candidate combines the decrement and destination choice in LEA. The jump into the preceding HealthCost tail also depends on its layout. |
| `00438085` | ActorRecoverInjuries | 130 B, D3 | Crossed clamp ownership with the injury minimum and maximum capture, then factored the cap as a value-returning operation. Cs11 branches JAE at +4c, copies EDI into EAX and stores EAX. The retained body branches JB, copies EAX into EDI and stores EDI. |
| `00439218` | HercFreePower | 144 B, D3 | Coupled lookup-table and slot capture with a continue guard, then reused maximum as the accumulator with alternative PopFindItem offset/base captures. Cs11 forms slot 19+i in EBX, copies it to EAX and uses EAX for the slot load. The candidate exchanges the two registers. |
| `0045b4a0` | TrajectoryDirection | 672 B, D2 | Crossed dx/dy calculation order with each square accumulator, then used the existing PointDistance operation at all distance sites with each snapshot orientation. Both families reached D2. The original adds into EDX at +21a and pushes EDX. The candidate adds into EAX and pushes EAX. |
| `00476ea0` | ResetMultiplayerInput | 558 B, D4 | Paired the inlined empty-text exit with the caller text increment/capture. Paired the menu-creation exit and final id capture with a split mask test. The original copies option id/text into EDI/EBP and later pushes those homes. The candidate exchanges them. |
| `0047d180` | MciState::Open | 144 B, D33 | Factoring the status operation with positive/negative guard exits gave D135 or worse. Coupling open-success assignment with alternative status/failure continuations reached D33. Cs11 keeps 0x204 in ESI and 5 in EBP, compares the status field in memory and emits a separate failure epilogue in its 160-byte extent. |

## Reproduction evidence

The per-function triage is in `build/ro1/triage.md`. Each original/candidate listing and its history are in the same directory as `<label>.try` and `<label>.notes` (the MCI files use `Open`). Final changed-file listings and dry-run outputs use `.final.try` and `.final.gate`. The variant template, complete score list and best full TU for each experiment use `.tpl.c`, `.out` and `.best.c`. Family names and measured outcomes are in `candidates/ro1/attempts.jsonl`. The unchanged MCI TU dry run used the recorded initializer placement `W?.fn_init$n()v=0047d0e0`.

The lane-local launcher redirects compiler and comparison scratch into `build/ro1` and limits live attempt/report discovery to ro1. Historical notes come from the archived project records. New helper and method names are working labels.
