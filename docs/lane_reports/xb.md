# Lane xb

All 43 targets in 29 files were rebuilt with their recorded compiler settings. Six candidate files passed dry-run promotion. They preserve 110 previously STRICT functions. No target became STRICT and no object was completed.

## Candidate promotion checks

```text
python tools/promote.py candidates/xb/pop/pherc.c --tu src/pop/pherc.c --as 'pop\pherc.c' --profile plain-oe --at 'W?.fn_init$n()v=00445238'
src/pop/pherc.c: 16 strict functions (2245 bytes), new 0, lost 0
```

```text
python tools/promote.py candidates/xb/shell/commscrn.c --tu src/shell/commscrn.c --as 'shell\commscrn.c' --profile plain-oe --at 'W?.fn_init$n()v=004123a0'
src/shell/commscrn.c: 36 strict functions (10548 bytes), new 0, lost 0
```

```text
python tools/promote.py candidates/xb/sim/simhot.c --tu src/sim/simhot.c --as 'sim\simhot.c' --profile ot --at 'W?.fn_init$n()v=00460c50'
src/sim/simhot.c: 12 strict functions (1461 bytes), new 0, lost 0
```

```text
python tools/promote.py candidates/xb/pop/pmove.c --tu src/pop/pmove.c --as 'pop\pmove.c' --profile plain-oe --at 'W?.fn_init$n()v=004425d4'
src/pop/pmove.c: 9 strict functions (1218 bytes), new 0, lost 0
```

```text
python tools/promote.py candidates/xb/_unknown/tu_0043ef34.c --tu src/_unknown/tu_0043ef34.c --as '_unknown\tu_0043ef34.c' --profile plain-oe
src/_unknown/tu_0043ef34.c: 13 strict functions (1413 bytes), new 0, lost 0
```

```text
python tools/promote.py candidates/xb/sim/anim.c --tu src/sim/anim.c --as 'sim\anim.c' --profile ot-oe
src/sim/anim.c: 24 strict functions (2417 bytes), new 0, lost 0
```

## Function results

Lengths include any alignment attributed to the extent by the comparer. Each row reports the final candidate for a changed file or the source baseline for a restored file. The last column identifies rejected experiments where relevant. Detailed hypotheses and results are in [the attempt ledger](attempts.jsonl).

| VA and function | Final extent / original (bytes) | Differing bytes | Original behavior and remaining difference |
|---|---:|---:|---|
| `004456f4` f_004456f4 | 164 / 168 | 119 | Computes the 42% and 58% scroll thresholds and the signed speed. The fractional-scale helper also serves the two variable denominators. The original reloads width at +0x25 after the 42% calculation. |
| `00419413` f_00419413 | 169 / 177 | 133 | Updates upgrade-screen scrolling from mouse position. Branch-local speed updates with a shared percentage helper approach the original speed tail, but lose accepted f_0041b7a1. The original independently reloads width for the bounds. |
| `00412a75` CommsLogView::Event | 165 / 177 | 146 | Updates log scrolling when the view is loaded. The candidate shares the percentage helper across both bounds and speed branches. Original loads width three times and merges the direction-dependent speed calculations. |
| `0046d74c` process_link_events | 290 / 289 | 27 | Receives packets until one is available for this player, handles name release, then dispatches it. The original loads capacity 474 into EAX before pushing 1 and EAX. A per-call size reference produced 300 bytes. The single receive-loop variant gave 288 or 290. |
| `00461050` SimHotCheckMission | 432 / 480 | 308 | Loads a selected hotseat mission by path or searches for its quick-start map and reports failures. The original keeps three partially shared error paths. The compiler merges more of their formatting and cleanup. Inlining the real loader and using a variadic formatting declaration did not recover them. |
| `00463040` LogicCheckActorCollision | 816 / 821 | 569 | Checks actor availability and ownership in the collision scan. Near +0x50 the original separates the unit pointer from the availability test. The accepted sibling’s predicate and direct owner lookup still produce different register lifetimes in this caller. |
| `004517c0` SimGameDrawFrames | 827 / 832 | 735 | Draws focus and target corner strokes and the timer. The original tests the phase byte twice, computes the first color in DL and copies it to AL. Choosing both colors together or delaying the second choice shortened the function further. |
| `00436dcc` ActorQuery | 835 / 958 | 788 | Returns actor attributes by query code. Case 215 contains the four morale-condition returns inline. Calling ActorScale inside ActorCondition reproduces that expansion, but the separately accepted ActorCondition gains a register copy at +0x4b. The closest attempted Query extent was 956 of 958. |
| `0044591f` f_0044591f | 867 / 866 | 110 | Initializes the HERC panel, controls, portrait buffer and surface. The original pushes full-screen dimensions through EAX at +0x2f1 and +0x2f7 and clears the remap in EBX. Typed surface fields left a one-byte length difference. |
| `0041eca2` MultiplayerAttemptStart | 896 / 894 | 501 | Starts a ready multiplayer game or displays its first validation error. The original materializes the button record address and spills the first returned text pointer at +0x1b7. Splitting visibility from the clean bit or introducing a text rectangle did not recover both, and the rectangle lost accepted neighbors. |
| `00441456` f_00441456 | 1101 / 1107 | 573 | Counts available inventory templates and snapshots, then builds display rows. The original loads the same local name from [esp+0x24c] at +0x1dc and +0x210 across the first string call. The candidate caches it in ESI. A template name accessor and explicit memcpy both shortened the function. |
| `00451370` SimGameDrawStatus | 1120 / 1104 | 606 | Draws the portrait, status bars and warning indicators. The original multiplies the image index by 12 in the bar loops. The compiler advances byte offsets. A frame accessor on the sprite set also changed accepted SimDrawBar. |
| `0047520b` ShellPlayMovie | 1121 / 1149 | 680 | Pumps a playing movie, distinguishing keyboard and mouse exits. Separate exits reached the original 1149-byte extent at 694 differing bytes. The original reserves 0x318 and reuses EBX holding 1 on the skip path. The retained baseline reserves 0x30c. |
| `00442af4` MovementCommand | 1531 / 1548 | 957 | Dispatches movement commands. Restored the selected-target arm before the group iteration, in the order of the original calls. Earlier focus loads around +0x150 and the argument setup for SimFaceTarget still differ. |
| `0044052b` GenerateMissions | 1730 / 1738 | 1519 | Creates nine mission offerings and refreshes each slot. The original keeps count and slot separately at frame offsets 0x33c and 0x340 and takes the random remainder before setting up the call. Extracting the choice into a helper shortened the code to 1695–1702 bytes. |
| `00470cf6` MessageDispatch | 1866 / 1855 | 1378 | Dispatches network messages to game and shell handlers. The original loads either checksum or sender into ESI and shares the post-message tail. For the peer-drop case it loads the slot in EAX and reuses EBX for the local peer. Combining the post cases lost MessageActorJoined. Capturing the dropped peer did not repair the shared stop path. |
| `00461230` SimHotStartMission | 1824 / 1856 | 1443 | Builds player slots, loads the map, and copies the initialized mission script. Both external strcpy calls, the 22-dword REP MOVSD and frame size 0x188 now agree. The original still multiplies slot by 188 in the owner loop and shares less of the map lookup failure paths. |
| `00457f90` ActorDrawEffect | 2096 / 2112 | 1772 | Advances and draws the effect selected by animation state. For moving states the original retains the interpolated y value through the frame update. A shared coordinate interpolation operation produced 2096 or 2144 bytes and missed the original lifetime. |
| `00436138` SendEvent | 2369 / 2735 | 2269 | Applies pilot events and refreshes flags. Both versions inline descriptor binding and call RefreshActorState. The original puts the common flag refresh before that case and expands additional ActorScale trees in the restore events. Moving the disabled-event guard to the shared refresh changed the tail but did not restore those trees. |
| `0044bcd0` SimHandleInput | 3136 / 3184 | 2770 | Dispatches simulation keyboard and menu commands. The original places ExploIdle and focus cycling before PilotNext and PilotPrevious. Mirroring those arms reduced the difference to 2747 bytes but failed the promotion gate on accepted _DATA. The baseline retains its later command ordering and 48-byte extent deficit. |
| `0043ef64` ProfileManager::BeginMissions | 5423 / 5438 | 3972 | Settles mission results and produces the payout report. Both damage descriptions now branch before fetching string 0x71 or 0x72. Initializing the reward with the kill credit restores the EAX-to-EDX copy at +0x99. The six payout figures have the observed relative layout but sit eight bytes too low, and the original uses ESI for the divisor. |
| `00446570` f_00446570 | 102 / 102 | 5 | Converts the current frame into an angle and grid direction. At +0xf the original copies the list receiver from EDX to EAX, loads count directly into ECX, and tests ECX. A count out-parameter and a defaulting lookup either increased the extent or left more differences. |
| `00457510` AnimAction::$ct | 144 / 144 | 21 | Clears and initializes an animation action. Original reconstructs this with LEA EAX,[EDI-0x12] at +0x20 before the two position-copy instructions. A common initial-state reset removed the candidate’s extra time store but still missed the LEA. |
| `0043960a` HercSaleValue | 252 / 252 | 191 | Sums sale prices for the whole unit or returns price less repair for one slot. The original returns the whole-unit total early, then reloads the x87 repair result from [esp+0xc] into EBX at +0xea. Member and reference price helpers lost accepted f_00439736 through its shared epilogue. |
| `00433d40` TerrainSetTypes | 272 / 272 | 94 | Assigns terrain from noise and chooses a variant when there is more than one. The original indexes the terrain table in EAX and widens its variant-count byte into EBP. Moving random selection onto TerrainType shortened the function and lost TerrainBuildHeights. |
| `0042e7a0` DrawDeathMap | 352 / 352 | 247 | Builds six outline vertices and draws the hexagon. The original pushes -1 for MenuSetColors immediately after constructing the array and pushes color before the last copy. Moving the color call itself among vertex assignments did not recover that scheduling. |
| `00434070` TerrainTriplet | 384 / 384 | 293 | Finds two neighbors, checks their heights and sets three consecutive feature IDs. The original spills the definition base at [esp+0x10] and reloads its low byte after MUL AH. A separate byte-valued first feature reached 384 bytes with 288 differences, but the spill and subsequent byte arithmetic still differ. |
| `0043ae77` HercSelectHit | 429 / 429 | 141 | Builds weighted hit thresholds for eligible mounts and applies the selected mount’s chance. Original accumulates the two weighted terms and bias in EDI while the unit stays on the stack. A component-level weight method shortened the function by 14 bytes. |
| `0045cf40` WeaponAccuracy | 480 / 480 | 285 | Combines range, skill, elevation, movement and target defense into shot accuracy. Original clears EAX for the byte-valued actor state comparisons near +0xb2 and +0xce. Reading from the shot record or delaying the target-skill snapshot gave a 496-byte function. |
| `0042afb0` GridPointToIndex | 496 / 496 | 68 | Transforms a point to grid coordinates and checks the final lookup against the grid radius. The original takes the center address and separately loads point x into ESI. Calling the existing distance helper in either geometric direction produced 512 bytes and lost GridInitialize and GridLoad. |
| `00433a80` TerrainHeightRange | 512 / 512 | 281 | Builds a height histogram and scans downward to find the percentile cutoff. The original retains count in ESI, the cell in EDI and the iteration index in EBP. A saved center or cell-end traversal did not remove the candidate’s extra count slot. |
| `00433e50` TerrainSetWater | 544 / 544 | 351 | Generates water from temporary noise and percentile thresholds. At +0x20c the original calls 00431d80 with flag 1. Modeling a base subobject produces flag 1, but the currently bound Noise destructor at 004349a0 calls 00431d80 through a wrapper. |
| `004352e0` DrawTerrainRows | 576 / 576 | 146 | Clips and emits terrain spans one row at a time. In the unclipped loop the original keeps skip in EAX and computes left with LEA into EDX before loading the row end. Computing the width in row-local coordinates shortened the body to 560 bytes. |
| `00414204` CommsScreenRender | 612 / 612 | 6 | Dispatches communication-screen rendering by word-sized mode. Original zeroes ESI and copies AX into SI before the jump table at +0x22. An explicit zero-based switch still masks EAX or increases the difference to 14 bytes. |
| `0042b9e0` DisplayConfigure | 720 / 720 | 337 | Applies rotation, scale and range to the terrain view. The original reloads normalized rotation into ESI at +0x34 and multiplies into EAX. Encapsulating the signed rotation calculation in DisplaySettings kept the extent but did not recover that lifetime. |
| `004324d0` BuildTerrainPath | 928 / 928 | 579 | Selects and costs successive path steps, then writes the zero terminator. The prior D23 source still needs ESI for the actor-table base and EAX for the final terminator address. A moving output pointer and pointer-based limit/duplicate checks all lost the accepted GridSetSelection epilogue. |
| `00462920` HotbriefProcessOpposingActions | 1040 / 1040 | 751 | Scans opposing actors, validates reactions and fires eligible shots. Independent friendly/rejected-target guards compile identically. Inside the expanded checks the original and candidate materialize the false result differently around +0x105, before the shot-processing loop. |
| `00445e23` f_00445e23 | 1072 / 1072 | 39 | Polls the HERC panel, updates angle, and handles GUI and keyboard actions. The inlined frame count inherits the receiver/count discrepancy of 00446570 and changes later speed-register assignments. The alternate count interfaces made the caller worse. |
| `00457af0` AnimActionCommitTrajectory | 1184 / 1184 | 965 | Builds the trajectory, assigns its target to the resource list, and computes travel distance. The candidate reuses AnimActionAssign and emits the external abs call at +0x3dd. The original reserves 0x68, and its first rate-by-metadata multiply still has a different instruction form. |
| `00454cd0` SimBuildDrawQueue | 1184 / 1184 | 641 | Builds the draw queue from actors, world actions and effects. The prior reconstruction gets actor in EDI and action in EBP, but the original tests the all/dirty values directly in memory. Three guard forms left a register test or displaced the surrounding code. |
| `004341f0` TerrainFeatures | 1344 / 1344 | 451 | Places features and overlays from rules and noise. The original local destructor calls 00431d80 at +0x294 with flag 1. A derived local reproduces the flag but still calls the currently bound wrapper. Several unrelated counter slots differ as well. |
| `0042ef10` RayOcclusion | 1408 / 1408 | 850 | Samples occupants and interpolated heights along a ray to find occlusion. Original uses ECX for heightStep in the rise expression and places the two cell coordinates in the opposite slots. Splitting the interpolation into base and delta heights grew the extent to 1424–1440 bytes. |
| `00464054` W?.fn_init$n()v | 24 / 24 | 0 (UNBOUND) | Constructs the 512 global actors. All 24 code bytes agree. The type signature at 0048e0d8 is unbound. Its constructor and destructor target 00464d74 and 00464d6c respectively. Explicit placement construction optimized down to one byte and lost the accepted constructor. |

## Verification details

The six promotion checks cover accepted code and data. The source variant for SimHandleInput passed the per-function preservation check but failed promotion on `data:_DATA` and was restored. The two terrain destructor probes isolate a binding issue for the integrator. All probe sources, objects, and comparison listings are under `build/xb/`.
