# Lane ro3 reopen round

All eight targets were rebuilt against cs11 in their full translation units. Three reached STRICT in promotion dry runs (1,081 bytes). Two other candidates reduced the byte differences at the original extent size. All six dry runs reported lost 0.

## Function results

| VA | Function | Final result | What the source change established |
|---|---|---|---|
| 0045d580 | WeaponTerrain | STRICT, 304 B | Count six iterations, form the direction pointer from i, and read the weight at facing * 6 + i. This removed the extra loop-entry compare and left 12 stack displacements. Declaring sourcePoint, targetPoint, targetGrid in that order closed those 12. The six declaration orders were tested after the loop shape matched. |
| 00460340 | ExploPosition | STRICT, 640 B | With case-local actor pointers, case -6 assigns the sprite height to e->value and negates the stored value. Case -10 calls PopFindItem(actor->Unit()) as the nested argument. Only this combination with the depth read from actor matched in the eight-way test. The separate store and negation also appear in the neighboring -7/-8 case. |
| 00460bc0 | FxExtra | STRICT, 137 B | Return immediately when the extra-effect index is zero. This gives the original EBX/BL kind load. Each of the three scaling expressions matched with this exit and stayed four bytes off in the guarded body. The candidate retains multiplication at the call. |
| 0045cc60 | WeaponStatus | DIFF, 736 B, 25 → 2 | Snapshot the mission index before the visibility test and assign visible explicitly in both branches. The compiler now loads the mission index into ECX before comparing the visibility byte in memory. The two remaining bytes are at +0x1f8 and +0x1fe. The original loads and compares the remote owner using EBP, the candidate uses EDX. |
| 0045b8c0 | UnitDamage | DIFF, 288 B, 179 → 109 | Return the penetration accumulator, cap the armor amount before adding excess damage, and take a reference to the total armor before updating directional armor. This recovers the scheduling of the armor subobject address. The original keeps direct damage in EBP across GridClamp and spills the returned facing index. The candidate spills direct damage, keeps facing in EBP and cleans up PopFindItem arguments earlier. |
| 0045dbe0 | FxDrawBolt | DIFF, 1,456 B, 70 → 70 | Named coordinate offsets coupled with an intermediate shade index, and a shared final center-line draw coupled with step bounds, gave no improvement. The oracle uses ESI/EDI/EBX/EBP for x0/y0/x1/y1 and spills the step at esp+0x20. The candidate uses EBP/EDI/ESI/EBX and esp+0x24. |
| 0045f560 | FxFire | DIFF, 800 B, 36 → 36 | Tried hidden/visible branch layouts with both max expressions, then speed selection and quiet-guard layouts. The oracle loads the second max operand in ECX and reloads world for each final draw call. The candidate uses EDX and reuses world in EDI. |
| 0045dad0 | WeaponSound | DIFF, 241 B, 29 → 29 | Tested all six orders of the initial volume/pitch/flags stores with both range-read orders, then staged the final sound ID at three positions around the clamps. The oracle keeps the descriptor in ESI and pitch range in EDI, and loads the ID before pushing the settings address. Both register assignments and the final argument schedule still differ. |

## Promotion dry runs

The commands below reproduce the gated results for the four changed files.

~~~text
python tools/promote.py candidates/ro3/sim/explo.c --tu src/sim/explo.c --as 'sim\explo.c' --profile ot-oe
src/sim/explo.c: 24 strict functions (3883 bytes), new 1, lost 0
~~~

~~~text
python tools/promote.py candidates/ro3/sim/exptbl.c --tu src/sim/exptbl.c --as 'sim\exptbl.c' --profile ot
src/sim/exptbl.c: 2 strict functions (521 bytes), new 1, lost 0
~~~

~~~text
python tools/promote.py candidates/ro3/sim/weapai.c --tu src/sim/weapai.c --as 'sim\weapai.c' --profile ot-oe
src/sim/weapai.c: 12 strict functions (2351 bytes), new 1, lost 0
~~~

~~~text
python tools/promote.py candidates/ro3/sim/proj.c --tu src/sim/proj.c --as 'sim\proj.c' --profile ot
src/sim/proj.c: 3 strict functions (617 bytes), new 0, lost 0
~~~

The weapai dry run implies one data binding, weaponTerrainWeights at 004a2644. PointAngle uses the C linkage of its matched definition. The explo candidate uses the matched declarations for PopFindItem, AnimActionInterpolateDrawPos and FxBounds. FxBounds receives the existing partial rectangle view of the effect record, whose four rectangle fields start at offset 0x2b in both TUs.

The two unchanged source copies were also dry-run checked. fxout reported 8 strict functions (2069 bytes), new 0, lost 0. weapfx reported 1 strict function (224 bytes), new 0, lost 0.

[Triage](TRIAGE.md) records the prior scopes and planned couplings. The [attempt ledger](attempts.jsonl) records the 27 tested families and names their variant logs under build/ro3. The .final.lst and .final.dry files there contain the final comparisons and dry-run output.
