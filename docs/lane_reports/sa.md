# Lane sa report

Rebuilt the accepted source for all 43 targets and saved the history, residue classification, and comparison under `build/sa/` by work-order row number. The `--all` output in `try.py` clips its original column at the candidate extent, so the full original menu-policy listing is available separately in `build/sa/menu-original.txt`.

Two diagnostic source improvements passed dry-run promotion with all existing STRICT functions preserved. There were no newly STRICT functions or complete translation units. The remaining targets include four neighbour-dependent residues. Only rows marked **Probed** or **Candidate** received fresh source variants in this lane. The other rows record current baseline evidence and earlier attempt history.

## Candidate promotion dry runs

```text
python tools/promote.py candidates/sa/pop/preport.c --tu src/pop/preport.c --as 'pop\preport.c' --profile plain-oe --at ReportDrawUnit=004486d9
src/pop/preport.c: 12 strict functions (1380 bytes), new 0, lost 0
dry run OK (add --publish to accept)
```

```text
python tools/promote.py candidates/sa/sim/simturn.c --tu src/sim/simturn.c --as 'sim\simturn.c' --profile ot-oe --at SimApplyLeadership=004507d0
src/sim/simturn.c: 12 strict functions (2148 bytes), new 0, lost 0
dry run OK (add --publish to accept)
```

Object checks with the retained sources yielded zero complete TUs for both candidate filters. The raw outputs are in `build/sa/tucheck-preport.txt` and `build/sa/tucheck-simturn.txt`.

## Function results

The extent and differing-byte columns are diagnostics from `try.py`. A larger diagnostic count can result from a shifted branch or tail. Attempt families and probe measurements are in `attempts.jsonl`.

| Address | Function | Extent | Differing bytes | State | Remaining difference |
|---|---|---:|---:|---|---|
| `00465c27` | `MissionItemAddress` | 62 B | 49 | Probed | Standalone uses the limit as the default upper-clamp result and adds the type offset from memory. The one-based wrapper matches the standalone and both caller clamps, but each caller still needs a separate offset load and loses STRICT. |
| `00477798` | `BriefWaitA` | 77 B | 69 | Probed | Expands both submenu loops and the main-item policy. The shared split recovers the 12-byte frame and expansion after initialization. It still introduces a mode spill and exchanges the count and flag homes. |
| `0044f990` | `IntelClearMode` | 144 B | 21 | Depends on neighbour | The routine agrees through the return at +0x70. The remaining bytes are SimBeginTurn's jump table at 0044fa10 and the surrounding fill. |
| `004796ab` | `HandleMenuKey` | 198 B | 62 | Depends on neighbour | The routine agrees through +0x83. The first mismatch at +0x84 belongs to WndProc's following scan/dispatch data. |
| `0046624e` | `ScoreCandidate` | 204 B | 82 | Probed | Builds the full 12-byte row offset, adds the table base, then copies the score to ESI before the quantity load. The row-lookup variants still fold the address or change surrounding allocation. |
| `00443d2f` | `WeaponCommand` | 342 B | 69 | Baseline | Tests focus with a memory compare at +0x109, then reloads it after pushing the mount index. The current source coalesces the two loads. The historical result-local helper recovers most of the three-byte deficit while still coalescing the reads. |
| `004441f8` | `PopUpdateItem` | 469 B | 246 | Probed | Materializes the 10/15 color in EAX and copies it to EDI at +0xbf. Distinct color/amount locals and lexical scopes still coalesce that copy or move other instructions. |
| `0042d080` | `BestLocSetup` | 576 B | 442 | Baseline | Caches the initialization count in EDX, keeps the current vertex in EBP across AreaGetPoint, and walks the final scan on an EBP cost pointer while rereading the count slot. |
| `004774b8` | `MenuApplyMode` | 605 B | 339 | Probed | Keeps submenu flags in EBP and both submenu counts in separate stack slots. The legacy 0/1/11 chain has no intervening store, and the save/load initialization and five flag conversions differ. |
| `00444b20` | `ShieldPopup::Refresh` | 680 B | 351 | Baseline | Captures clip x/y/width/height in EDX/EAX/ESI/EBX and zero-extends the direction byte into EDX. The candidate uses a different clip assignment and an extra EAX-to-EDX move. |
| `00403f80` | `Shell::UpdateMainMenuSelection` | 701 B | 547 | Baseline | Keeps the launch block in the later result switch and branches to it from the hit switch. The matching control transfer in the earlier lane's experiment leaves the epilogue local, whereas the original shares UpdateStorage's epilogue. |
| `0042de30` | `TerrainDrawCliff` | 965 B | 802 | Probed | Copies textured into a second stack slot before BestLocSetData and uses it for faces two and three. It explicitly adds 2 before the right-depth byte read. The front depth read has a trailing pointer increment that the tested forms eliminate. |
| `004275dc` | `PlyMatch` | 1047 B | 941 | Baseline | Uses a 0x70 frame, reloads the polygon parameter from the stack, and holds x/y in EDI/ESI. Crossing cases also assign independent arithmetic temporaries. |
| `004486d9` | `ReportDrawUnit` | 1110 B | 121 | Candidate | Retained row-label ID evaluation before the default push. The original places diagram x/y and portrait x/y in different homes, and advances the status-image base before adding the index without the extra move. |
| `00449630` | `PopInfoDrawText` | 1139 B | 896 | Baseline | Keeps the definition pointer in ESI and copies it to EBX. It reloads the special byte in the else path and forms the action record address with a shorter load sequence. |
| `004788db` | `SaveGame` | 1163 B | 912 | Baseline | Merges particular inlined Problem blocks backward and keeps their three logical arguments in a different register order. The current error paths merge forward and add 20 bytes. |
| `00471e98` | `PacketApply` | 1264 B | 1016 | Baseline | Materializes two call arguments in registers, reloads the animation receiver in IsMoving expansions, and uses a different repair-slot index home. |
| `00402230` | `Shell::Update` | 1308 B | 804 | Baseline | Uses the pooled ProcessInput epilogue and avoids two extra receiver moves in ResumeMenus. Removing those copies in the recorded trials changes other inline expansions and loses existing proofs. |
| `0044fa30` | `SimBeginTurn` | 1440 B | 1307 | Baseline | Calls SimStartPhase out of line at +0x36. The current helper expands inline. The penalty and actor-loop allocations remain open even in the earlier diagnostic with the original call restored. |
| `004387e6` | `HercFormatItem` | 1469 B | 754 | Baseline | Evaluates the installed-slot test differently at +0x46. Later case code executes beyond the shared return at 00438dac, which is also the separately callable HercNoOperation entry. |
| `0046915d` | `ScriptCall` | 1515 B | 1202 | Baseline | Places the 0xc092 group pointer at EBP-0x1c and its next argument at EBP-0x10. Case argument lifetimes and exits shift the following EvalOp tables. |
| `00452980` | `SimFire` | 2435 B | 958 | Baseline | Uses a 0x1c0 frame, with different point and actor temporaries, shot-slot addresses and clamp state. The current snapshot improves the byte diagnostic from the older 2434-byte baseline. |
| `00405338` | `Shell::UpdateMainMenu` | 3043 B | 788 | Baseline | Expands HelpReleased and the receiver lifetimes across a different merge. It materializes zero arguments and preserves later restore reasons in different slots. |
| `004797f4` | `WndProc` | 3070 B | 1277 | Baseline | Keeps distinct SetMenuVisible and default-call register assignments and shares tails differently. Its scan/dispatch tables also affect HandleMenuKey. |
| `00401500` | `Shell::ProcessInput` | 3191 B | 1316 | Baseline | Loads screen addresses into EAX before 14 pushes in event 30207. It also uses a separate pending-cost accumulation and differs in BeginComms/screen 10 layout. |
| `0047b4a0` | `PaletteAttach` | 272 B | 229 | Baseline | Keeps the color cursor in EBP and cycle induction in ECX. It saves a second cycle pointer for the group-change branch and uses a byte group comparison. |
| `0044f0d0` | `SimSetReturnFire` | 336 B | 15 | Baseline | Zero-extends the stack mask straight into EDI at +0xaf and reads returnFire before advancing the record pointer. The candidate widens through ECX and reads from the advanced pointer. |
| `004507d0` | `SimApplyLeadership` | 352 B | 4 | Candidate | Retained the Pilot accessor for the notification. Only the penalty remains: original SUB on the live distance followed by three-operand IMUL. Natural in-place subtraction switches the actor and range homes. |
| `0040130b` | `ShellConfirmPurchases` | 401 B | 8 | Depends on neighbour | The executable agrees. The eight differing bytes are in the ProcessInput switch table beginning at +0x131. |
| `0044b9c0` | `SimSetupWorld` | 480 B | 318 | Baseline | Uses all three REP MOVSD copies, as the candidate does, but interleaves them differently with view-rectangle construction. It also decrements width before SetFastView and reloads shared bottom edges. |
| `00444766` | `ShieldControl::Event` | 500 B | 112 | Baseline | Loads the aim deltas into ESI/EBP and reads step from memory again after setting the update flag. The candidate caches step and uses different delta homes. |
| `00456d80` | `ActorListAdd` | 518 B | 263 | Baseline | Keeps player in EBP and spills type index to [esp+0x28]. Candidate assigns the opposite homes and changes a compare-with-memory into load/test. |
| `00426814` | `Flic::DecodeFrame` | 781 B | 569 | Baseline | Stores zero to pending.size even on its zero-test path. It also keeps the resource chunk pointer in EBP and uses different coordinate slots when decoding regions. |
| `0047cc50` | `RenderResourceGlyph` | 816 B | 660 | Baseline | Uses a 0x20 frame, tests a byte for space, and keeps the packed/unpacked bit in DL after the merge. The candidate spills the bit and uses a 0x28 frame. |
| `00404d74` | `Shell::UpdateMultiplayer` | 910 B | 278 | Baseline | Keeps result in EDX, uses EAX for the key, and loads the case-1 status into ESI for the test and subsequent zero sound argument. |
| `0045e430` | `FxBeam` | 928 B | 559 | Baseline | Both current builds keep the two ViewFlip paths. The first redraw's actor-loop rotation and the interpolation/aim homes still differ. |
| `004565e0` | `ActorListTransfer` | 992 B | 141 | Baseline | Writes the selected record offset and head to distinct slots, then compares the tail from memory in the score path. The recorded score fix swaps the two earlier slots. |
| `0045be00` | `ApplyOne` | 1056 B | 641 | Baseline | Interleaves counter loads/stores with the loss calculation, presents the actor through EDX then EDI, and tests gApplyBonus from memory before selecting a part. |
| `00447907` | `PilotScreen::Draw` | 1583 B | 59 | Baseline | The third frame expansion advances ECX with ADD. Flashing arguments use different slots, and the original writes the stat-step arms directly into ESI. |
| `0042d640` | `TerrainDrawTile` | 1840 B | 1452 | Baseline | Uses different argument homes at +0x0c, with later divergence in visibility, descriptor/animation handling and flat-outline scheduling. |
| `0045c220` | `ApplyVisit` | 2032 B | 1603 | Baseline | Keeps actor ID and ring in EBX/ESI and passes different scalar and aggregate copies to the nested ApplyOne expansion. The signed-byte average is already reproduced. |
| `004682dc` | `ScriptStep` | 3295 B | 2892 | Baseline | Uses a 0x64 frame and materializes reference-result addresses and stack arguments differently. The candidate has 0x60 bytes of locals and different dispatch/tail layout. |
| `004697a8` | `ScriptEvalOp` | 534 B | 1 | Depends on neighbour | Only +0x5f differs. The word-scan relocation points into the preceding ScriptCall extent, which is 96 bytes short in this snapshot. |
