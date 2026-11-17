# Lane sb baseline audit

All 43 targets in 29 translation units were rebuilt with the compiler settings in `recovery.json` and checked against the pinned 1.10A oracle. The supplied sizes and diagnostic byte counts reproduced. The verifier reported 42 DIFF targets and one UNBOUND target. This audit produced no source candidates, promotion receipts, or completed objects.

[Baseline logs](../../build/sb/audit) include a fresh `notes.py show` and `parked.py` result for each VA, a `try.py --all` result for each TU, and each original function through its recorded code end. The compile driver is [audit.py](../../build/sb/audit.py). [Attempt entries](attempts.jsonl) record three completed diagnostic checks.

## Findings

- Two SHAPE cases have fully accounted-for size changes without evidence of a missing operation. At `0046d74c`, the candidate saves and restores EDI for `FlashWindow` (+2 bytes) and pushes the capacity directly (-1 byte). At `0044591f`, the candidate's two surface-dimension loads use the one-byte-longer ECX and ESI encodings (+2 bytes). Its remap reset uses the one-byte-shorter EAX absolute store (-1 byte). The original and candidate have the same instruction count within the latter function.
- `try.py --all` reads the original using the candidate extent length. The display omits the original tail at `004456f4`, including the final store and jump, and shows the first instruction of the next original function at `0046d74c` and `0044591f`. Its text alignment can also mark calls whose original relative target is printed as a function offset and whose candidate target is printed as a symbol. The pinned comparison supplies the reported status and byte counts independently of this display.
- At `00464054`, the code is byte-equal. The relocation at `00464055` points to the `LogicActor` type signature at `0048e0d8`, which the candidate does not prove. The original descriptor contains references to `00464d74` at offset `+1`, `0047e008` at `+5`, and `00464d6c` at `+9`. The two local routines both return the incoming pointer, so their bodies alone do not establish their descriptor roles. This confirms the binding blocker independently of source spelling.

## Target results

The byte counts below are diagnostic and use the candidate extent. The cited instructions are examples from the fresh comparison. Prior reports supply context where the complete difference spans several blocks.

| VA | Function | Class | Candidate B / differing B | Original versus baseline |
| --- | --- | --- | ---: | --- |
| 004456f4 | `f_004456f4` | SHAPE | 154 / 140 | Loads width three times and materializes each divisor separately. The candidate shares width and divisor. The original stores speed from EDX and jumps to the earlier epilogue at `004456d4`. |
| 00419413 | `f_00419413` | SHAPE | 169 / 133 | Loads width three times, joins the speed arithmetic after the multiply-by-five step, and returns via EDX. The candidate reuses the width and joins before the multiply. |
| 00412a75 | `CommsLogView::Event` | SHAPE | 193 / 186 | Shares the original upgrade handler's instruction sequence apart from global addresses. The candidate folds width into the threshold multiplies and computes the two speed paths separately. |
| 0046d74c | `process_link_events` | SHAPE | 290 / 27 | Uses EAX for the capacity push at the receive-loop entry and caller-saved ECX for the window handle. The immediate push and EDI save pair explain the one extra candidate byte. |
| 00461050 | `SimHotCheckMission` | SHAPE | 432 / 308 | Retains the second allocation/format argument setup starting at `+5a`. The candidate sends both failures through the same complete cleanup block. |
| 00463040 | `LogicCheckActorCollision` | SHAPE | 816 / 569 | Holds the first actor pointer in ECX across the availability test and the first hidden-byte check. The candidate reloads that pointer for the check. The count slot and later nested-loop register homes also differ. |
| 004517c0 | `SimGameDrawFrames` | SHAPE | 827 / 735 | Tests the focus flag at `[ebx+0x2cb]` independently for both color selections. The candidate reuses the loaded flag and arranges the resulting color bytes differently. |
| 00436dcc | `ActorQuery` | SHAPE | 835 / 788 | Saves EBP and retains more of the nested scale dispatch in the event cases. The candidate omits the EBP save and folds more of the inlined dispatch. |
| 0044591f | `f_0044591f` | SHAPE | 867 / 110 | Loads both surface dimensions into EAX and resets the remap pointer from EBX. The candidate's register encodings explain its one-byte excess. |
| 0041eca2 | `MultiplayerAttemptStart` | SHAPE | 896 / 501 | Forms a button receiver before accessing its `+0x14` member and spills the first text result at `[ebp-4]`. The candidate folds the button address into the access and keeps the text result in a register. |
| 00441456 | `f_00441456` | SHAPE | 1101 / 573 | Reloads the copied template name from the stack twice around the lookup. The candidate retains it in ESI. The two template copies also use different slots. |
| 00451370 | `SimGameDrawStatus` | SHAPE | 1120 / 606 | Keeps an image index in a slot and multiplies it by 12 in the inner bar-loop setup. The candidate strength-reduces the corresponding addresses over the surrounding loop. |
| 0047520b | `ShellPlayMovie` | SHAPE | 1121 / 680 | Reserves `0x318` bytes against the candidate's `0x30c`. The path, error-buffer and window slots differ, and the original retains separate skip-flag store blocks. |
| 00442af4 | `MovementCommand` | SHAPE | 1528 / 990 | Loads and tests the modifier at `+ab`. The candidate compares it directly in memory. Later focus and item values also have different lifetimes. |
| 0044052b | `GenerateMissions` | SHAPE | 1730 / 1519 | Uses stack byte slots `+0x33c` and `+0x340` for the initial values, including a separate zero store to the latter. The candidate starts with a different slot and materializes the related condition in a register. |
| 00470cf6 | `MessageDispatch` | SHAPE | 1866 / 1378 | Loads either event field into ESI and shares the whole `PostMessageA` argument setup. The candidate duplicates the setup and shares only the call. |
| 00461230 | `SimHotStartMission` | SHAPE | 1936 / 1530 | Reserves `0x188` local bytes against `0x190` and loads the fog-data destination into EBP before the buffer loop. Record copies and later mission-start paths diverge. |
| 00457f90 | `ActorDrawEffect` | SHAPE | 2096 / 1772 | Preserves the original coordinate at `[ebx+0x4f]` while deriving the scaled delta from `[ebx+0x57]` and `[ebx+0x5f]`. The candidate groups the interpolation loads and arithmetic differently. |
| 00436138 | `SendEvent` | SHAPE | 2369 / 2269 | Holds the actor and event in EBX and ECX on entry and retains more complete inlined scaling paths. The candidate starts with ESI and EAX and has fewer nested scale blocks. |
| 0044bcd0 | `SimHandleInput` | SHAPE | 3136 / 2770 | Loads both initial input globals before the first test and uses EDI and ESI for the two values. The candidate changes their initial homes and the placement of the test. |
| 0043ef64 | `ProfileManager::BeginMissions` | SHAPE | 5414 / 4664 | Uses ESI for the division by 100 and schedules the bonus and report locals differently. The candidate divides through ECX and spills the intermediate quotient earlier. |
| 00446570 | `f_00446570` | OPERANDS | 102 / 5 | Copies the list receiver into EAX, loads the count into ECX, then tests ECX. The candidate loads the count into EAX first and tests EAX. |
| 00457510 | `AnimAction::$ct` | SAME-SIZE | 144 / 21 | Recovers the full receiver address from the member pointer in EDI with `lea eax,[edi-0x12]` at `+20`. The candidate keeps its receiver address in EAX and stores through it. |
| 0043960a | `HercSaleValue` | SAME-SIZE | 252 / 191 | Copies the repair value to EBX before the final subtraction. The candidate subtracts the stack operand directly. The early return and division scheduling also differ. |
| 00433d40 | `TerrainSetTypes` | SAME-SIZE | 272 / 94 | Clears EAX before the byte index load, then widens the table's variants byte directly into EBP. The candidate masks EAX after loading the byte and addresses the variants byte differently. |
| 0042e7a0 | `DrawDeathMap` | SAME-SIZE | 352 / 247 | Interleaves vertex copies and color-argument pushes and places the final pair of dword copies at `+10d`. The candidate groups the copies before the pushes. |
| 00434070 | `TerrainTriplet` | SAME-SIZE | 384 / 293 | Uses a `0x18`-byte frame for separate base and direction slots and retains byte arithmetic. The candidate uses `0x14` and schedules the neighboring values differently. |
| 0043ae77 | `HercSelectHit` | SAME-SIZE | 429 / 141 | Adds the scalar and indexed table value to EDI in separate instructions at `+114` and `+116`. The candidate uses EDX for the table addition. |
| 0045cf40 | `WeaponAccuracy` | SAME-SIZE | 480 / 285 | Begins two operand paths with an EAX clear at `+b2` and `+ce`. The candidate instead directly widens the `+0x25` member into EDI in those paths. |
| 0042afb0 | `GridPointToIndex` | SAME-SIZE | 496 / 68 | Forms the center pointer in EAX early and loads the point's X into ESI before the center's X. The candidate advances EBX by `0x10` in the final lookup sequence. |
| 00433a80 | `TerrainHeightRange` | SAME-SIZE | 512 / 281 | Uses a `0x424`-byte frame and advances the pointer by 16 at `+6a`. The candidate has another four stack bytes and lacks that explicit advance at the corresponding point. |
| 00433e50 | `TerrainSetWater` | SAME-SIZE | 544 / 351 | Calls `NoiseGet` at `+128` and has a separate subtraction at `+14d`. The candidate's call setup and subsequent subtraction differ in that span. |
| 004352e0 | `DrawTerrainRows` | SAME-SIZE | 576 / 146 | Uses a different late row-offset expression. The baseline sequence adds EBX into EAX and then subtracts in both directions across EAX and EDX. |
| 00414204 | `CommsScreenRender` | SAME-SIZE | 612 / 6 | Clears ESI and copies AX into SI before the table dispatch. The candidate masks EAX and indexes the table with EAX. |
| 0042b9e0 | `DisplayConfigure` | SAME-SIZE | 720 / 337 | Keeps the value at receiver `+0x50` in ESI while multiplying into EAX, then uses EDI for the lower-bound value. The candidate overwrites EAX in the multiply and uses ESI for the bound. |
| 004324d0 | `BuildTerrainPath` | SAME-SIZE | 928 / 579 | Lacks the candidate's pair of `movsd` instructions at `+117`, and separately adds EDX into EAX at `+25a`. Intermediate point copies and offset computation differ. |
| 00462920 | `HotbriefProcessOpposingActions` | SAME-SIZE | 1040 / 751 | Reaches the corresponding decision and dispatch without the candidate's added branch/zero materialization at `+105` or its byte mask at `+27e`. |
| 00445e23 | `f_00445e23` | OPERANDS | 1072 / 39 | Loads and tests the inlined list count in EDI at `+39`, the same allocation issue seen in `f_00446570`. The candidate tests EAX and later carries its divisors in ECX. |
| 00457af0 | `AnimActionCommitTrajectory` | SAME-SIZE | 1184 / 991 | Uses `0x68` local bytes, an indexed memory multiply at `+41`, and a signed shift at `+52`. The candidate uses `0x64` and multiplies in registers. |
| 00454cd0 | `SimBuildDrawQueue` | SAME-SIZE | 1184 / 641 | Holds actor/action in EDI/EBP and tests visibility through a base-plus-byte-index memory test. The candidate swaps the registers and uses a separate address addition. |
| 004341f0 | `TerrainFeatures` | SAME-SIZE | 1344 / 451 | Compares the first value against `[esp+0x1d0]` and tests `[esp+0x1ec]` in two later branches. The candidate uses `+0x1f8` and `+0x1e8` respectively. |
| 0042ef10 | `RayOcclusion` | SAME-SIZE | 1408 / 850 | Compares against `[esp+0x80]` at `+6e`. The candidate uses a relocated operand there and applies a later subtraction in the opposite register direction. |
| 00464054 | `W?.fn_init$n()v` | RELOC | 24 / 0 | UNBOUND. The `LogicActor` type-signature target at `0048e0d8` remains unproved. |
