# ro6 invariant reopen round

Four of nine targets are STRICT against cs11 in fresh dry runs, totaling 1,901 bytes. Five files changed. All five dry runs report `lost 0`. The remaining five targets are DIFF. D below is the differing-byte count from the comparator.

## Function results

| VA | Function | Baseline → final | Evidence |
|---|---|---|---|
| 00469d26 | LogicGenFindType | 675 B, D8 → STRICT | In the vehicle branch, declare `bestCost` before `best` and `i`, then assign it after reading `gLogic.level`. All eight spill displacements now select the original slots, vehicle at +1c and Herc at +18. One of 36 combinations matched. |
| 00469a95 | ScriptFormat | 428 B, D89 → STRICT | A separate list index and two cursor advances reproduce the 0x38 frame, the spilled list base, and the index in ESI. In the resource and numeric branches, advance `src += 5` after evaluating the operand. Nest the resource lookup in the recursive formatting call. Both ESI reloads now occur between argument setup and the corresponding call. |
| 0046d308 | send_link_packet | 208 B, D28 → STRICT | In the `first == 0` broadcast branch, pass literal zero as the final `PostMessageA` argument. The peer moves into ESI and the DirectPlay result into EBX. This one argument is the entire source change. |
| 0046f7b6 | StopMultiplayer | 574 B, D451 → 590 B, STRICT | Return 1 directly at the end of the restart branch. Capture the connection type in `DisconnectMessage` before selecting resource 12 or 13. Together with the original thread guard this restores EBP, the separate shutdown call on restart failure, and the local epilogue. The candidate uses the existing bound names and method signatures for the calls and globals exposed by the match. |
| 00464f50 | ActorChooseByMode | 681 B, D195 → 685 B, D2 | In cases 8 and 9, read value34, value38 and count30 in that order, then square the value and multiply the divisor by the saved rate. The byte-sized visibility flag holds a boolean and the switch and both multiplies match. At +a5 and +de cs11 uses XOR/TEST EBX, the candidate uses BL. A full-width flag with the staged Product spelling gives D4, reversing the factor-load registers in both cases. |
| 0046a0ce | LogicMapViewUpdate | 404 B, D24 → D24 | Crossed array-store orders and previous-slot cost capture with loop scope, then FindType argument forms with output indexing and count calculation. cs11 retains the FindType result in EDX, uses ESI as the output loop index and EBX for its scaled offset. The candidate retains the result in EBP and the index in EBX. |
| 004637ed | LogicDispatchIntel | 388 B, D190 → D190 | Tested duplicated refresh exits, dispatch and tick captures, and flattened selection guards. cs11 calls ResetActorValues on the phase arm, clears EDI, then jumps to the common actor store. The second reset falls through to that store. The candidate routes the phase arm to a different block and uses EBX for current. Original length is 390 B. |
| 0046718b | IntelStateSeek | 553 B, D6 → D6 | Moving the predicate onto the metadata record and combining typed access with case exits failed to reproduce the fifth read. Target-value nesting and named unit captures also failed. cs11 adjusts EAX by 0x20 before copying it to EDX and reloads [EDX+5]. The candidate copies first and reloads [EDX+0x25]. |
| 0046fb3c | MenuNetwork::CompactPlayers | 595 B, D7 → D7 | Tested all six orders of extra-slot state and color stores with slot access, then combined the copy increment, reverse scan, and final iterator updates. Only the backward scan at +188 through the LEA at +1b2 differs. cs11 carries the last occupied slot in EDX and the candidate uses EBX. |

The four dry runs propose the bindings `g4c928c = 004c928c`, `MissionAllocateFromData = 0046ae4b`, `SigsSend = 00474dc3`, and `bailingMessage = 004a4810`, respectively in lgen, sinterp, link, and mstart. The lfire command uses the established address of its unreferenced source-path array to re-prove the accepted 14-byte _DATA segment.

## Dry runs for changed files

```text
python tools/promote.py candidates/ro6/logic/lgen.c --tu src/logic/lgen.c --as 'logic\lgen.c' --profile plain-oe
src/logic/lgen.c: 11 strict functions (2152 bytes), new 1, lost 0

python tools/promote.py candidates/ro6/logic/sinterp.c --tu src/logic/sinterp.c --as 'logic\sinterp.c' --profile plain-oe
src/logic/sinterp.c: 23 strict functions (3856 bytes), new 1, lost 0

python tools/promote.py candidates/ro6/multi/link.c --tu src/multi/link.c --as 'multi\link.c' --profile plain-oe
src/multi/link.c: 12 strict functions (1611 bytes), new 1, lost 0

python tools/promote.py candidates/ro6/multi/mstart.c --tu src/multi/mstart.c --as 'multi\mstart.c' --profile plain
src/multi/mstart.c: 40 strict functions (5461 bytes), new 1, lost 0

python tools/promote.py candidates/ro6/logic/lfire.c --tu src/logic/lfire.c --as 'logic\lfire.c' --flags "-4s -s -bm -oe=90" --at lfireSourceFile=004a3af4
src/logic/lfire.c: 5 strict functions (1161 bytes), new 0, lost 0
```

The lane ledger contains 25 family records. [Attempt ledger](attempts.jsonl) · [Triage and prior scopes](../../build/ro6/TRIAGE.md)

