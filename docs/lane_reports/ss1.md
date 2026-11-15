# ss1: original-size construct audit

All 23 targets were checked with their complete attempt histories, fresh `parked.py` output, and fresh full `try.py` listings using the recovery ledger's profile/flags. There were 24 new source trials on 14 functions, at most six on any function. Nine targets were left at baseline after the audit found only previously recorded families and no new evidence. The lane's 24 family/audit records are in `candidates/ss1/attempts.jsonl`.

**New STRICT functions: none.** Three small diagnostic improvements are retained below. Each promotion dry run succeeds with **new 0, lost 0**, and no `REVIEW twin helpers` warning. They remain DIFF and are not strict matching gains. All other candidate source copies equal their source baselines.

## Changed files and promotion dry runs

python tools/promote.py candidates/ss1/pop/ppilot.c --tu src/pop/ppilot.c --as 'pop\ppilot.c' --profile plain-oe

Summary: `src/pop/ppilot.c: 16 strict functions (3976 bytes), new 0, lost 0`; dry run OK. Log: `build/ss1/promote_ppilot.txt`.

`00447907 PilotScreen::Draw`: 1583 bytes, D63 -> D59. Spell the stat step as `delta > 8 || delta < -8 ? 4 : 1`, and the direction test as `shown[i] > target[i]`. The bound-test branches and direction comparison now agree. The third frame expansion, flash temporary slots and step/value-pointer homes still differ. Explicit step-assignment and early value-pointer forms were reverted because they changed size and lost the accepted initializer.

python tools/promote.py candidates/ss1/sim/fxout.c --tu src/sim/fxout.c --as 'sim\fxout.c' --profile ot-oe

Summary: `src/sim/fxout.c: 8 strict functions (2069 bytes), new 0, lost 0`; dry run OK. Log: `build/ss1/promote_fxout.txt`.

`0045e430 FxBeam`: 928 bytes, D561 -> D559. Use `at33 <= at2f ? at2f : at33` for this call's maximum. This reproduces the original JG / scalar MOV selection instead of the opposite branch and repeated field read. Scalar homes, initial width/zero stores, jitter/interpolation and redraw layout remain DIFF. The shared MAX macro and other call sites are unchanged.

python tools/promote.py candidates/ss1/terr/tdisplay.c --tu src/terr/tdisplay.c --as 'terr\tdisplay.c' --profile ot

Summary: `src/terr/tdisplay.c: 24 strict functions (2970 bytes), new 0, lost 0`; dry run OK. Log: `build/ss1/promote_tdisplay.txt`.

`0042bcb0 DisplayZoomArea`: 544 bytes, D434 -> D432. Expose the observed grid divisor at +4 as the provisionally named `cellWidth`, and read it through a scalar `CellWidth()` accessor. This restores `MOV ECX,[ECX+4]; IDIV ECX` and the following candidate test form. The 0x2c versus 0x30 frame and point/argument slots remain unresolved. `DisplayConfigure` is unchanged; its independent multiplication trial grew to 736 bytes and was reverted.

## Target outcomes

| VA | Function | Fresh source trials | Final differing bytes | Outcome |
|---|---|---:|---:|---|
| 0040130b | ShellConfirmPurchases | 0 | 8 | Body equal through RET at +12e. Differences begin at +131 in ProcessInput's following switch tables. The advertised arithmetic is data decoded as instructions. |
| 0045d580 | WeaponTerrain | 1 | 57 | Explicit six-entry pathDirections declaration is byte-invariant. Entry CMP, Point slots and direction/height homes remain. |
| 00447907 | PilotScreen::Draw | 4 | 59 | Retained branch/operand correction above; three larger or smaller forms lose initializer STRICT. |
| 00434aa0 | TerrainBlendHeight | 6 | 69 | Reversed sum/product and staged delta/strength/weighted-height forms are invariant or worse (D228/230/231). Restored baseline. The highlighted ADD is a LEA expression with different homes. |
| 0044ebe0 | SimExecuteItem | 1 | 103 | Capture the actor supplying repeated damage indices: invariant. Slot address, cost-inline context and seed/damage slots remain. |
| 0047b650 | SimPaletteCycle | 0 | 114 | Original has three saves, candidate four; rotated traversal and shared wrap tail differ. Previously tested loop/increment/helper families were not repeated. |
| 004565e0 | ActorListTransfer | 0 | 141 | Earliest residue is root-offset/head spill at +235, before highlighted stack tests. Existing getter/predicate/score families cover the apparent construct leads. |
| 0044f0d0 | SimSetReturnFire | 1 | 145 | Reversed mask/current operands gives D146. Restored baseline; MOVZX EDI versus XOR/MOV CL and byte-mask lifetime remain. |
| 0045b8c0 | UnitDamage | 1 | 179 | Compound overflow accumulation is invariant. Scaled-result/divisor homes, delayed cleanup and armor pointer remain. |
| 0043960a | HercSaleValue | 0 | 191 | FDIV address highlight is relocation display. Existing early-return/repair families cannot be repeated safely: the smaller near-match loses f_00439736. |
| 0045bb30 | UnitDamageIndicator | 2 | 230 | Reversed ternary and explicit if/else give 277 B/D241 and 293 B/D249. Restored 281-byte baseline; loop-end/phase and extent remain. |
| 0047b4a0 | PaletteAttach | 0 | 237 | Cursor across memset and byte/cycle-pointer lifetimes differ before highlighted increments. Recorded cursor/store/group families not repeated. |
| 00404d74 | Shell::UpdateMultiplayer | 1 | 278 | Scalar Status accessor gives 911 B/D839 and loses three accepted neighbors. Reverted, including the accessor. |
| 00459ee0 | AnimBitmapSweep | 0 | 285 | Frame 0x1c versus 0x18 and row/group rotations differ. Candidate 361 versus original physical extent 368; previous loop/return/type families exhausted. |
| 0045cf40 | WeaponAccuracy | 0 | 285 | Pilot/actor/skill homes and spills precede widening highlights. Prior large capture/type/order/accessor family supplies no new field evidence. |
| 0042bcb0 | DisplayZoomArea | 1 | 432 | Typed divisor accessor retained above; frame and point slots remain. |
| 0042b9e0 | DisplayConfigure | 1 | 437 | Independent scale multiplications grow to 736 B/D620. Restored 720-byte baseline. |
| 00461b20 | SimHotBriefGameOver | 1 | 502 | Split polling exit OR into sequential break tests: invariant. Width/height and receiver/mission homes remain. |
| 0045e430 | FxBeam | 2 | 559 | Retained equivalent maximum ternary above. Reversed strict-less form gives D562. |
| 0042e4d0 | MapDrawPoint | 1 | 594 | Base-record pointer with +4/+5 coordinate fields: invariant under recorded `-4s -s -bm -oxt -oe=150`. Restored baseline. |
| 0046a33c | LogicGenVehicle | 1 | 735 | Early return of leader->id instead of id is invariant. Return sharing and selected-record/point homes remain. |
| 004292e1 | MenuChat::Event | 0 | 741 | History ADDs are relocated addresses; low-key dispatch is the actual unresolved branch-tree difference. 1061 candidate bytes versus 1063 original. Prior switch/alias families not repeated. |
| 0042d640 | TerrainDrawTile | 0 | 1452 | First divergence at +c; argument homes, visibility/animation/edge scheduling differ across the body. Prior width/masked-frame/descriptor families not repeated. |

## Reproduction and scope

Fresh baseline logs: `build/ss1/<VA>_notes.txt`, `<VA>_parked.txt`, `<VA>_base.txt`. Trial definitions and logs: `build/ss1/group1.json` through `group4.json` and the named `.txt` files. Retained full listings: `build/ss1/final_pilot_step_polarity.txt`, `final_beam_max_le.txt`, `final_zoom_grid_width.txt`.

`build/ss1/run.py` runs the repository tools unchanged while routing temporary build directories and `CSTORM_SCRATCH` under `build/ss1`, with bytecode writes disabled. For example: `python build/ss1/run.py try.py candidates/ss1/pop/ppilot.c --as 'pop\ppilot.c' --profile plain-oe --fn 'PilotScreen::Draw' --all`. The promotion commands above are the normal integrator entry points; their logged lane executions used this scratch-routing wrapper.

The SAME-SIZE classifier and its core instruction excerpt are diagnostic, not evidence of a single wrong construct. These targets include relocated operands, tables decoded as code, allocation differences, shared tails and shorter physical extents. No flags were swept. No canonical sources, tools, config, docs, ledger, Git state or oracle files were changed; no publication occurred.
