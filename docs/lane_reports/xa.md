# Lane xa

All 43 targets in the work list were rebuilt against cs11. The retained change to `ScriptCall` proves **`ScriptEvalOp` at 004697a8, 534 bytes STRICT**. A second candidate improves the viewport setup in `SimSetupWorld`. The two promotion dry runs preserve every accepted function. No object was completed.

## Promotion dry runs

```text
python tools/promote.py candidates/xa/logic/sinterp.c --tu src/logic/sinterp.c --as 'logic\sinterp.c' --profile plain-oe --at ScriptCall=0046915d --at ScriptEvalOp=004697a8
src/logic/sinterp.c: 21 strict functions (3228 bytes), new 1, lost 0
dry run OK (add --publish to accept)
```

`ScriptCall` handles three ways to consume operands. The actor family consumes the actor reference first. The generic family leaves the whole argument count to the common cleanup. Weighted selection advances past the arguments before scanning them backward. On leaving that scan it goes straight to the result store. At offsets +cb through +d3 the actor family now loads the count from EBP−18h, scales it and adds it to the VM stack pointer exactly as the original does. The original then jumps to the result store at +561. Watcom puts the store at +d4 in this build.

That change places the following opcode tables correctly. Relative to `ScriptEvalOp`, the word scan starts 136 bytes before entry at 00469720 and the branch table starts 92 bytes before entry at 0046974c. All 136 bytes and all 23 relocation sites agreed in an independent owner-relative check even before the change. The remaining one-byte discrepancy in the gate was the relocated scan-table address. The new candidate resolves it without changing `ScriptEvalOp` or supplying an address for either table.

```text
python tools/tucheck.py --tu src/logic/sinterp.c --src candidates/xa/logic/sinterp.c -v
partial  src/logic/sinterp.c (9004 code bytes): data CONST: ITEMS; 4 of 25 code extents not STRICT
TU complete: 0 of 185 accepted TUs, 0 code bytes
```

The four remaining code extents are `ScriptFormatArgs`, `ScriptStep`, `ScriptCall` and `ScriptFormat`. The TU check above was filtered to this candidate.

```text
python tools/promote.py candidates/xa/sim/simglob.c --tu src/sim/simglob.c --as 'sim\simglob.c' --profile ot --at SimSetupWorld=0044b9c0
src/sim/simglob.c: 5 strict functions (563 bytes), new 0, lost 0
dry run OK (add --publish to accept)
```

`SimSetupWorld` sets the portrait, view, full-screen, status and bottom-panel rectangles in that order. It decrements the width once and uses the full-screen bottom for the two dependent rectangles. The extent stays at 480 bytes with 246 differences, down from 318. The early decrement now precedes `SetFastView`. The original holds width in ESI and the candidate holds it in EBX. This candidate is a partial reconstruction.

## Per-function results

Each row gives the final state of the submitted source. Measurements of alternatives belong to the scratch build unless the row says retained. The workbook of comparisons is under `build/xa/`: each VA has a `.notes`, `.parked` and full `.listing` file. Source trials and their listings are under `build/xa/probes/<VA>/`. The 53 attempt records are in `candidates/xa/attempts.jsonl`.

| VA | Function | State and remaining evidence |
|---|---|---|
| 00465c27 | MissionItemAddress | DIFF. The original forms the bound and base before clamping and selects the result separately. Reusing the count helper repeats a clamp. Delegating through a mission reference also loses two accepted inline callers. |
| 00477798 | BriefWaitA | DIFF. The original applies the menu policy within the wait routine. Sharing actual submenu and command operations gives 652 or 666 bytes against 624. The second submenu count and flag spill differently. |
| 0044f990 | IntelClearMode | DIFF after its executable body. RET is +70. Padding ends at +7f, five relocated dwords occupy 0044fa10–0044fa23, and 12 padding bytes follow. The table belongs to SimBeginTurn. |
| 004796ab | HandleMenuKey | DIFF after its executable body. The final jump through +83 agrees. At +84 the original has one NOP before the WndProc word scan. Multiple SaveGame experiments make the handler STRICT solely by changing this alignment. None gave a closer save path, so this match is diagnostic. |
| 0046624e | ScoreCandidate | DIFF. The original forms the full index-times-12 address and copies the score late. A zero-aware height accessor repeats the guard and yields 216 or 220 bytes against 208. |
| 00443d2f | WeaponCommand | DIFF. The slot-permission helper using ActorRecord::Unit reaches 15 differing bytes. At +109 the original tests memory, pushes slot+11, then reloads focus at +113 to fetch the unit. The compiler keeps focus from the test. |
| 004441f8 | PopUpdateItem | DIFF. The original chooses the slot color in EAX and copies it to EDI at +bf. Returning the color or updating it by reference from the formatter produces 484–516 bytes and loses the initializer. |
| 0042d080 | BestLocSetup | DIFF. Path relaxation writes the neighbour cost and predecessor, then compares the cost to the best candidate. Factoring that operation and keeping a reference to the cost gives 592 bytes against 560. The current index across AreaGetPoint and the two point slots still differ. |
| 004774b8 | MenuApplyMode | DIFF. Shared submenu and command helpers produce 601 or 609 bytes against 583. In the original second loop, EBP carries the flags and a stack slot carries the count. |
| 00444b20 | ShieldPopup::Refresh | DIFF. A unit-to-cursor conversion by pointer or reference gives 671 bytes against 678. The original widens shieldDirection into EDX and loads the clip arguments into different registers. |
| 00403f80 | Shell::UpdateMainMenuSelection | DIFF. Hit case 3 branches into the later result-switch launch block, and other exits share the UpdateStorage epilogue. Factoring either event dispatcher fails to reproduce both jumps and loses three accepted peers. |
| 0042de30 | TerrainDrawCliff | DIFF. Three indexed signed heights and face accessors produce 974–981 bytes against 992. The oracle keeps a separate texture flag and advances the right/front detail pointers. |
| 004275dc | PlyMatch | DIFF. The bounding-box test precedes polygon edge traversal. Using the element before the end pointer for the closing vertex leaves the scalar version unchanged. Carrying whole vertices changes the frame and shortens the traversal. The original reloads the polygon argument and reserves 70h bytes. |
| 004486d9 | ReportDrawUnit | DIFF. The current canonical source already stages the row label ID. Separate selector locals reproduce the first indicator's image-base addition, but the result is 1106 bytes against 1108. Portrait coordinates and the epilogue used by ReportDrawOther remain unresolved. |
| 00449630 | PopInfoDrawText | DIFF. Reproduced the two-definition-name candidate at 1141 bytes and 376 differences. Complementing the negative action index loses the original NEG and offset adjustment. A second definition-byte load is still needed at +2a7. |
| 004788db | SaveGame | DIFF. Error 102 reuses the message-box tail for 101. Subsequent errors use different registers. A single failure ID collapses distinct original paths. Shared requirement and file chooser helpers also miss the original control flow. Both dialog extensions really are `cbm`, at 004a45ec and 004a4648. |
| 00471e98 | PacketApply | DIFF. The original tests motion separately for crouch and stand. Sharing that check changes the dispatch and loses accepted packet functions. The repair-slot address, movement receiver and materialized −1 call argument still differ. |
| 00402230 | Shell::Update | DIFF. The inlined briefing return through ResumeMenus produces an extra ESI copy and restore. Combining the continuation-map branch with a common RestoreMenu changes nine accepted peers. |
| 0044fa30 | SimBeginTurn | DIFF. Inclusive action kinds 9 through 17 recover both original comparisons at +239 and +23e. The original calls SimStartPhase out of line and guards entry into the first owner loop. The overall extent and table still differ. |
| 004387e6 | HercFormatItem | DIFF. The shared prefix lookup orders the slot+1001 calculation before the zero argument, reducing the 1469-byte comparison from 754 to 745 differences. The installed-slot address at +46 and the deferred formatter cases remain unresolved. |
| 0046915d | ScriptCall | DIFF, retained. The 1519-byte reported extent has 1160 differences, down from 1202. Four operand/group stack locations and the actor cleanup now agree. The weighted divisor and shared result-store location still differ. |
| 00452980 | SimFire | DIFF. Building the impact screen point through a reference yields the same 2435-byte function. Returning a point by value yields 2481. Frame size agrees, but scalar and point slots and call arguments still differ. |
| 00405338 | Shell::UpdateMainMenu | DIFF. The ending-screen poll tests key and mouse releases before servicing the frame. A shared release predicate loses UpdateBiodermSkills and Open. Receiver lifetimes, zero-valued drawing arguments and restore slots still differ. |
| 004797f4 | WndProc | DIFF. The original enters an existing SetMenuVisible expansion from the menu-entry case. Fall-through or a direct jump avoids the duplicate expansion and yields 3007 or 3014 bytes against 3017. The case exits and default-call argument registers still differ. |
| 00401500 | Shell::ProcessInput | DIFF. Event 30207 materializes the individual screen addresses in EAX before pushing them for close calls. Grouping the comms command IDs merges more code and loses four accepted shell functions. |
| 0047b4a0 | PaletteAttach | DIFF. The original keeps the three run bytes in AL/DL/DH and spills the current cycle to stack slot +4. A run aggregate with shared count finalization expands to 336 bytes against 272. |
| 0044f0d0 | SimSetReturnFire | DIFF. The original loads the mask into EDI, loads AL from [EBX+8e], then advances EBX. A byte reference with a conditional bit update changes the function earlier. The best existing source still differs by 15 bytes. |
| 004507d0 | SimApplyLeadership | DIFF. After rebasing on the canonical Pilot() formatter, four bytes remain. The original subtracts 2 in ESI and multiplies into EDX. A distance helper, a named penalty and in-place subtraction still produce a different instruction pair. |
| 0040130b | ShellConfirmPurchases | DIFF after its executable body. RET at +12e agrees. The eight differing bytes begin at +131 in dispatch data for Shell::ProcessInput. |
| 0044b9c0 | SimSetupWorld | DIFF, retained. 480 bytes, 246 differences. The early decrement and shared full-screen bottom are recovered. Width, height and the scheduling of the three clip copies still differ. |
| 00444766 | ShieldControl::Event | DIFF. Capturing the mouse or control center as a point gives 512 or 499 bytes against 500. At +184 the original tests the step in memory, then reloads it for the addition at +195. |
| 00456d80 | ActorListAdd | DIFF. A 60-byte animation descriptor preserves the 518-byte extent and reduces differences from 263 to 259. The original keeps the player in EBP until the final color load and spills the computed type index. |
| 00426814 | Flic::DecodeFrame | DIFF. Computing the payload as buffer+(offset+6) restores the memory-backed address sequence, yielding 780 bytes and 406 differences. On the stream path, the original uses a known-zero EDX for both header stores and restores the saved frame size in EBP. |
| 0047cc50 | RenderResourceGlyph | DIFF. With the byte pixel API, the prior rebuilt glyph loop produces 800 bytes, or 848 with a column/pixel-X for-loop, against 816. The original has a 20h-byte frame, holds the bit in DL and shares its epilogue at 0047cf79 with MenuDrawText. |
| 00404d74 | Shell::UpdateMultiplayer | DIFF. The original carries status in ESI across the close and can push the same zero for the sound. Capturing status and performing one common close produces 903 bytes and loses three accepted peers. |
| 0045e430 | FxBeam | DIFF. Both redraws have their own flip tails. Guarding the actor traversal and moving the increment into its backedge gives the same 928 bytes and 559 differences. The first traversal still exits through a different block. |
| 004565e0 | ActorListTransfer | DIFF. The original stores the selected actor offset at [ESP] and the head at [ESP+4]. Keeping head and tail in a chain aggregate changes the frame before scoring begins. Pairing it with the prior score-block repair also fails. |
| 0045be00 | ApplyOne | DIFF. Factoring per-volley and cumulative hit counts produces identical code. Grouping counters by measure loses the original load/store schedule and increases the differences from 641 to 904. |
| 00447907 | PilotScreen::Draw | DIFF. The original selects a step of 1 or 4 into ESI and reuses it for the update. Computing the signed residual first changes frame allocation. The retained 1583-byte source still has 59 differences. |
| 0042d640 | TerrainDrawTile | DIFF. At +580 the original compares the palette index with 30. Each branch pushes −1 and its color, then joins before the surface push and one MenuSetColors call. Both explicit source forms recover the constant but miss the index register and branch layout. |
| 0045c220 | ApplyVisit | DIFF. The shared hit-accounting helper reproduces baseline inlining. Regrouping stores reduces differences from 1603 to 1594 here but worsens standalone ApplyOne. Actor/ring registers and inlined damage temporaries still differ. |
| 004682dc | ScriptStep | DIFF. The original allocates 64h bytes, shifts the state index and forms the script-code address in two stages. Regrouping word and byte offsets either reproduces the 60h-frame baseline or loses standalone ScriptCode. |
| 004697a8 | ScriptEvalOp | **STRICT, retained.** All 534 bytes pass through the corrected preceding stack cleanup and the opcode-table fixups. |
