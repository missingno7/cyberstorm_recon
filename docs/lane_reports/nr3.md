# Lane nr3 near-miss results

One new STRICT function, the 293-byte HERC close routine at `00445cfe`. The weapon preview improves from eight differing bytes to six. The other five targets retain their starting distances. All four full-TU dry runs reported `lost 0`.

## Per-function results

The initial full listings reproduced every requested extent and distance. The differing bytes in all seven belong to local instructions. No call or branch displacement contributes to any of the seven baseline distances.

| VA and function | Earlier result and new test | Final comparison against cs11 |
|---|---|---|
| `00445cfe f_00445cfe` | Earlier work varied the **second** saved setting and the subsequent buffer capture. Crossing both setting reads with cleanup order exposed the **first** getter. With the original cleanup restored, a four-build control crossed `SavedManual()` versus `savedManual` with typed versus raw reads of the second setting. Both direct-first builds were STRICT. Both getter-first builds differed in five bytes. | **STRICT, 293 B.** Directly reading `panel->savedManual` for `BriefWaitB` restores EDX for the following `BriefWaitA` argument and ECX for the buffer load, test and push. The candidate uses named fields for both settings. |
| `004437a2 WeaponPopup::UpdatePreview` | r4 had observed a y-first constructor improvement. We reproduced it and tested coordinate capture order, constant versus half-size offsets, value/reference constructors, mixed initializer/body forms, and marker-index and offset expressions with the existing `drawCursorImages` identity. | **1,354 B, D8 → D6.** The y-first constructor gives x+40 in EAX and y+40 in EDX. Four differing bytes remain in the two stores because the original writes x first. The other two are at the marker call, where the original adds into and pushes EDX and the candidate uses EAX. |
| `0044579c f_0044579c` | Earlier native Open tests used the superseded 389-byte body. In the current body all 16 combinations of the screen member calls and typed surface calls stayed at D2. We also coupled the memset range with both final clear orders and the string pool identity. | **387 B, D2.** At +134 the original loads the request from `[ebp+10h]` into EDX and pushes EDX. The candidate uses EAX. |
| `0044540f f_0044540f` | Earlier trials included all-getter captures and dimension reordering. We tested all 16 mixed field/getter subsets across the setter prototypes, then crossed the preceding viewport copy with the later damage-mode capture and clip-local types. | **548 B, D4.** Width and height load into ESI and EAX in the original, EAX and ESI here. The corresponding pushes account for the remaining two bytes. |
| `00446570 f_00446570` | Earlier count/default expressions had brought the list into EDX. We coupled those expressions with the TU's existing `ScaleFraction` helper, then selected the count through a frame header returned by pointer or reference. The latter builds all differed in 85 bytes. | **102 B, D5.** At +0f the original copies EDX into EAX, loads ECX from `[edx]` and tests ECX. The candidate loads EAX from `[edx]`, copies EAX into ECX and tests EAX. |
| `00444fbb SimInventoryEvent` | Earlier angle/alias and case-exit forms left D9. We reused the existing `ShieldPopup::Commit` at either or both network sends and tested both shield-field store orders. All six case-body orders were also crossed with the angle join and squared-distance capture. | **554 B, D9.** Actor facing is in ESI and the inlined aim angle in EDI in the original. Those assignments reverse in the candidate, starting at the byte extension at +4f. |
| `00449d76 PopWorldDraw` | Earlier clip prototypes and the 24 capture orders had left D8. We crossed every field/getter subset with the viewport copy, then tested scalar/reference/endpoint-record captures with the following background-draw operands using the matched clip prototype. | **319 B, D8.** Original x/y/width/height use ESI/EDX/ECX/EAX. The candidate uses EAX/ESI/EDX/ECX. Exactly four load operand bytes and four pushes differ. `PopWorldDrawText` remains STRICT. |

## Promotion dry runs

```text
python tools/promote.py candidates/nr3/pop/pherc.c --tu src/pop/pherc.c --as 'pop\pherc.c' --profile plain-oe
src/pop/pherc.c: 17 strict functions (2538 bytes), new 1, lost 0
```

The first successful HERC dry run identified nine aliases. The retained source uses the existing bindings for the resource release, surface calls and pixels, surface height, mission call and state flag. The final dry run requests no new implied binding and reports no twin helpers.

The neighboring `0044591f` diagnostic goes from D110 to D114. Both builds emit identical 867-byte OMF code and identical fixup positions for it. The only changed fixup target name, at +2f3, is the surface height. Binding it as `g5138b0` makes those four bytes comparable in an already misaligned instruction stream. Every other placed function in the two changed TUs retains its baseline result apart from the two target improvements above.

```text
python tools/promote.py candidates/nr3/pop/pweapon.c --tu src/pop/pweapon.c --as 'pop\pweapon.c' --profile plain-oe
src/pop/pweapon.c: 20 strict functions (1964 bytes), new 0, lost 0
```

The `pshield.c` and `pworld.c` lane copies are byte-identical to their source files. Their dry runs reported 16 and 8 strict functions respectively, each with `new 0, lost 0`. The scoped family results are in `candidates/nr3/attempts.jsonl`. The full listings, variant rankings and dry-run output are under `build/nr3/`.

