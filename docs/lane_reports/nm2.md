# Lane nm2

Two of the three targets reached STRICT against cs11 in promotion dry runs, totaling 1,280 bytes. Both changed TUs preserved every previously STRICT function and data proof. The full-TU comparisons show no regression in any other function.

| VA | Function | Baseline | Final | Source and evidence |
|---|---|---|---|---|
| 00464f50 | ActorChooseByMode | 685 B, D2 | **STRICT**, 685 B | Use an int for the visibility flag and the existing ActorHidden accessor. In cases 8 and 9, load value34 into value, value38 into divisor, and count30 into a separate count. Square value, then call Product(divisor, count). The flag is cleared and tested in EBX. Both cases load the rate into EAX and the count into ESI. Twelve of 672 combinations matched. |
| 0045cc60 | WeaponStatus | 736 B, D2 | **DIFF**, 736 B, D2 | At +0x1f7 the original loads gRemoteActorKind into EBP. At +0x1fd it compares EBP with the owner at [EAX+8]. The candidate uses EDX for both, changing the bytes at +0x1f8 and +0x1fe. Guard, unit, and type snapshots (54 variants), followed by energy, range, restricted-action, and owner-guard couplings (300 variants), left the best score at D2. The submitted source is byte-identical to src/sim/weapai.c. |
| 0046fb3c | MenuNetwork::CompactPlayers | 595 B, D7 | **STRICT**, 595 B | Initialize last to 9 before the for loop, iterate while last is positive, and break when the slot is occupied. Keep the existing state, palette, and slot-color store order. EDX now carries last from +0x188 through the LEA at +0x1b2. Two of 42 scan/store combinations matched. The candidate uses the explicit break. |

For lfire, the candidate uses the bound DAT_004d35ac and the BestLocSetData signature from terr/tgrid.c. The caller supplies the two-long output array and the existing TerrainGrid binding for fallbackName. For mstart, the candidate uses the existing bindings gLaunchTeams, gRemoteActorKind, g511ddc, SimAddPlayer, and gLocalActorKind. The SimGame pointer cast follows the existing SimResetPlayers call in this file and supplies SimAddPlayer's matched parameter type. The final dry runs propose no new external bindings.

## DropPeer dependency

MenuNetwork::DropPeer at 0046ecd8 remains 681 B, D4. Before and after the CompactPlayers fix, the branches at +0x50, +0xba, +0x181, and +0x2a4 target +0x173b in cs11 and +0x16fe in the candidate. All four displacements are 61 bytes short. The original has a 61-byte MenuNetwork constructor between ThreadProc and Reset. The candidate emits that STRICT constructor in its own COMDAT. StopMultiplayer already matches at 590 bytes and CompactPlayers occupied 595 bytes in both runs. [Final branch listing](../../build/nm2/mstart.final)

## Promotion dry runs

~~~text
python tools/promote.py candidates/nm2/logic/lfire.c --tu src/logic/lfire.c --as 'logic\lfire.c' --flags "-4s -s -bm -oe=90" --at lfireSourceFile=004a3af4
src/logic/lfire.c: 6 strict functions (1846 bytes), new 1, lost 0
~~~

~~~text
python tools/promote.py candidates/nm2/multi/mstart.c --tu src/multi/mstart.c --as 'multi\mstart.c' --profile plain
src/multi/mstart.c: 41 strict functions (6056 bytes), new 1, lost 0
~~~

The unchanged weapai copy also passed its ot-oe dry run with 12 STRICT functions (2,351 bytes), new 0, lost 0. [Dry-run output](../../build/nm2/weapai.dry)

The triage records the prior attempts and the two rounds of searches are documented in the lane ledger. [Triage](TRIAGE.md) and [attempts](attempts.jsonl)
