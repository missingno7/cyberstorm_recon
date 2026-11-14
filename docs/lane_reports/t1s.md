# Lane t1s ? parked targets

STATUS: done

All 47 requested targets were inspected against their full-TU diffs and prior reports. Two source changes add STRICT functions; all six final promotion dry runs pass, with no accepted function or data losses. There is no publish or canonical-source change from this lane. The remaining 42 targets are parked below; three additional targets became STRICT in the externally updated comparison view without a source change.

## Final source gains

- `WeaponMaximum`, `0045d1c0`, 64 B ? value-returning ternary maximum helper, with the damage result evaluated before multiplying by the ring count.
- `f_00417842`, `00417842`, 217 B ? restore dispatcher case order 0/1/3/2, use a reference row-refresh helper and typed image accessors; this restores its earlier shared epilogue.

## Promotion receipts

Commands below are the exact Python invocations; stdout was saved in `build/t1s/<stem>-promote-final.txt`. All end with `dry run OK (add --publish to accept)`.

### shell/hbupgrad.c

```text
python tools/promote.py candidates/t1s/shell/hbupgrad.c --tu src/shell/hbupgrad.c --as 'shell\hbupgrad.c' --profile plain
src/shell/hbupgrad.c: 54 strict functions (11025 bytes), new 0, lost 0
dry run OK (add --publish to accept)
```

### shell/hercbuy.c

```text
python tools/promote.py candidates/t1s/shell/hercbuy.c --tu src/shell/hercbuy.c --as 'shell\hercbuy.c' --profile plain-oe --at 'W?.fn_init$n()v=00416444'
src/shell/hercbuy.c: 29 strict functions (4091 bytes), new 1, lost 0
dry run OK (add --publish to accept)
```

### shell/mplaunch.c

```text
python tools/promote.py candidates/t1s/shell/mplaunch.c --tu src/shell/mplaunch.c --as 'shell\mplaunch.c' --profile plain-oe
src/shell/mplaunch.c: 48 strict functions (16458 bytes), new 0, lost 0
dry run OK (add --publish to accept)
```

### sim/weapai.c

```text
python tools/promote.py candidates/t1s/sim/weapai.c --tu src/sim/weapai.c --as 'sim\weapai.c' --profile ot-oe
src/sim/weapai.c: 6 strict functions (624 bytes), new 1, lost 0
dry run OK (add --publish to accept)
```

### logic/miss.c

```text
python tools/promote.py candidates/t1s/logic/miss.c --tu src/logic/miss.c --as 'logic\miss.c' --profile plain-oe
src/logic/miss.c: 7 strict functions (333 bytes), new 0, lost 0
dry run OK (add --publish to accept)
```

### logic/intel.c

```text
python tools/promote.py candidates/t1s/logic/intel.c --tu src/logic/intel.c --as 'logic\intel.c' --profile plain-oe
src/logic/intel.c: 10 strict functions (3786 bytes), new 0, lost 0
dry run OK (add --publish to accept)
```

## Tool and ledger update during the run

`tools/compare.py` changed externally at 00:59:37, followed by `recovery.json` at 01:02:20. The new view separates compiler-owned jump-table extents differently. Intermediate dry runs rejected the unchanged baseline files for stale ledger sizes; the final ledger update resolved every rejection. Identical baseline control receipts are retained as `*-baseline-promote-current.txt`.

An earlier isolated hbupgrad formatter change passed the earlier gate (`new 1, lost 0`, `hbupgrad-promote.txt`), making the old 251-byte f_004199e8 extent STRICT. In the new view the published 247-byte handler is already STRICT, so that source proposal was withdrawn; the final hbupgrad file is byte-identical to src. It is not counted as a source gain.

Baseline-only target changes in the final tool view:

- `IntelStateLink` at `00467461`: STRICT, 237 B.
- `f_004199e8` at `004199e8`: STRICT, 247 B.
- `f_00416eae` at `00416eae`: STRICT, 36 B.

## Compiler facts for docs/codegen_rules.md

| Context | Trigger | Effect | Status |
|---|---|---|---|
| wpp386 10.6, ot-oe, WeaponMaximum | Evaluate `Maximum(a,b)` using a value-returning `a>b?a:b` helper into a local damage result, then multiply by ringCount | Produces the otherwise unread four-byte direct-damage temporary and the original score-relative load/result homes; 64-byte function STRICT. Inline product expression or explicit if maximum differs. | verified t1s; weapon-max-sequence-0/3.txt; final weapai promotion |
| wpp386 10.6, plain-oe, hercbuy dispatcher | Physical case order 0/1/3/2, reference-form row refresh, native image accessor | Places the common Draw/return before case 2 and removes a nested screen-pointer slot. After image accessor/row-array spelling restores surrounding extents, f_00417842 is STRICT and every accepted neighbor stays STRICT. | verified t1s; buy-case-image-2.txt and final hercbuy promotion |
| wpp386 10.6, ot-oe, WeaponStatus | Separate `IsTypeVisible` returning `IsTypeHidden(type)==0` before the visible-range adjustment | Preserves two scalar Boolean stages; 720 B/D149 becomes original 736 B/D25. Explicit inner byte equality and captured mission/global values do not finish it. | measured diagnostic t1s; weapon-visible-*.txt |
| wpp386 10.6, ot-oe, WeaponTerrain | Pretested pointer for-loop rather than do/while, same increment work | Moves end pointer to EBP, height to stack, restores 304-byte extent; D146 becomes D53. Six point declaration orders and two POD position forms do not finish it. | measured diagnostic t1s; weapon-terrain-*.txt |
| wpp386 10.6, plain-oe, MissionBound call sites | Statement/conditional helper forms, then const-reference and macro forms | No STRICT gain: statement forms retain input/result coalescing, reference/conditional alternatives add spills or wrong extents. | negative local result t1s; miss-bound-*.txt |
| wpp386 10.6, plain-oe, IntelStateSeek | Retain metadata pointer and use member/accessor for the final type check | Optimizer merges the separately reloaded byte, shortening the body and changing later homes. Five metadata-relative/member/reference spellings do not match. | negative local result t1s; intel-info-*.txt |
| wpp386 10.6, plain-oe, hercbuy startup | Inline wrapper constructors calling the already-bound control initialization functions | Grows initializer 665 to 705-794 B; does not reproduce original state-10/state-18 reuse. | negative local result t1s; buy-init-inline-*.txt |
| wpp386 10.6, plain-oe/plain, launch and upgrade callers | Semantic array getters, value-returning formatter helpers, or typed list reference helpers | Do not give a general allocation repair. Launch getters/formatters mostly grow functions; typed upgrade list helper variants retain the parked homes. | negative local result t1s; launch-*.txt and upgrade-list-view-*.txt |

## Per-target residue

Each remaining target has one line below. Counts are local compare diagnostics, not repository progress or acceptance-ledger updates. Shared-tail/table-only targets were worked through their owning functions rather than by padding or displacement tricks.

- `logic/intel.c` `0046718b` `IntelStateSeek`: 551 B  81 B differ, first +a; extent ends 004673b2, original function ends 004673b4 ? Metadata-pointer/byte access variants collapse the distinct reload and shorten the function; baseline retained.
- `logic/intel.c` `004673b4` `IntelSpawnTimer`: 173 B  3 B differ, first +15 ? Three displacements to the earlier shared epilogue; no independent body mismatch. Owner/seek variants did not repair it.
- `logic/intel.c` `00467560` `IntelActorCommand`: 245 B  186 B differ, first +9; extent ends 00467655, original function ends 00467690; reloc sites differ ? Target pointer versus value formal allocation, late value reload and following dispatch table. Orders-accessor context did not repair it.
- `logic/intel.c` `00467690` `IntelActorDispatch`: 432 B  323 B differ, first +3e; extent ends 00467840, original function ends 00467827; reloc sites differ ? Permission predicate needs scalar materialization and a different branch layout; positive branch and state-count capture still differ.
- `logic/intel.c` `00467827` `IntelIsAvailable`: 275 B  196 B differ, first +3b; extent ends 0046793a, original function ends 0046794c; reloc sites differ ? Same permission predicate family plus result/target homes; accessor and positive-branch forms fail.
- `logic/miss.c` `004654cc` `MissionRef::Resolve`: 89 B  39 B differ, first +2; reloc sites differ ? Inline address calculation coalesces input/result clamp homes; eight statement/conditional forms and reference/macro follow-ups fail.
- `logic/miss.c` `00465580` `MissionEstimate`: 292 B  4 B differ, first +41 ? Four-byte EBX/ESI exchange for row product and column. Prior six arithmetic/index experiments already cover the plausible family; no repeated sweep.
- `logic/miss.c` `004656a4` `MissionDataInit`: 752 B  174 B differ, first +d7; reloc sites differ ? Interval-bound/index homes, loop and scale expressions; interval/index reconstruction gives 753 B/D430, rejected.
- `logic/miss.c` `00465994` `MissionCopy`: 550 B  218 B differ, first +f; reloc sites differ ? Source/offset stack lifetime and banner branch allocation; ordered input/banner captures give 546-547 B/D276-351, rejected.
- `logic/miss.c` `00465bba` `MissionRef::Address`: 63 B  42 B differ, first +2; extent ends 00465bf9, original function ends 00465bfd; reloc sites differ ? Clamp result/input coalescing and branch copies; shared clamp experiments fail.
- `logic/miss.c` `00465c27` `MissionItemAddress`: 53 B  40 B differ, first +2; extent ends 00465c5c, original function ends 00465c67 ? Clamp inputs/results and shared return placement; reference and macro variants fail.
- `shell/hbupgrad.c` `00419413` `f_00419413`: 164 B  129 B differ, first +5; extent ends 004194b7, original function ends 004194c4; reloc sites differ ? Width reuse, percentage division and shared branch arithmetic; separate branch expressions, percentage member helpers and preserved result all fail.
- `shell/hbupgrad.c` `004194c4` `f_004194c4`: 945 B  24 B differ, first +244; reloc sites differ ? First/second stat homes and nested cost formatter; prior typed-list/ref and isolated formatter follow-ups fail to make the body STRICT.
- `shell/hbupgrad.c` `00419901` `f_00419901`: 204 B  20 B differ, first +86; reloc sites differ ? Cost destination evaluation order; isolated screen/index formatter does not improve it under the final table-boundary rules.
- `shell/hbupgrad.c` `00419b3c` `f_00419b3c`: 481 B  3 B differ, first +1f ? Three shared-return displacements depend on later list functions; tested owning helper contexts, no artificial tail introduced.
- `shell/hbupgrad.c` `00419d52` `f_00419d52`: 402 B  286 B differ, first +1c; reloc sites differ ? Streaming screen-copy/index homes; read-block and MovieHeader::Image extraction fail.
- `shell/hbupgrad.c` `0041b7a1` `f_0041b7a1`: 374 B  223 B differ, first +8e; extent ends 0041b917, original function ends 0041b912; reloc sites differ ? Inlined name/cost loops have wrong homes and extent; native list-view reference experiments remain DIFF.
- `shell/hbupgrad.c` `0041b912` `f_0041b912`: 577 B  434 B differ, first +2c; extent ends 0041bb53, original function ends 0041bb4c; reloc sites differ ? Inlined list loops and sound/shared return; list-view reference family remains DIFF.
- `shell/hbupgrad.c` `0041bb4c` `f_0041bb4c`: 355 B  230 B differ, first +20; reloc sites differ ? Inlined list loops and sound/shared return; list-view reference family remains DIFF.
- `shell/hbupgrad.c` `0041bdda` `f_0041bdda`: 70 B  1 B differ, first +42 ? Single jump displacement into 0041b741; owning list/layout changes do not repair it.
- `shell/hercbuy.c` `00416444` `W?.fn_init$n()v`: 665 B  138 B differ, first +132; extent ends 004166dd, original function ends 004166de; reloc sites differ ? Construction-state constant reuse starts at +132; inline native constructor wrappers grow 705-794 B, rejected. Prior argument-type sweeps not repeated.
- `shell/hercbuy.c` `00416764` `f_00416764`: 267 B  2 B differ, first +e3 ? Input receiver EDI/ESI swap at +e3; InputReq and screen formal typing are neutral.
- `shell/hercbuy.c` `0041686f` `f_0041686f`: 662 B  608 B differ, first +6; extent ends 00416b05, original function ends 00416b76; reloc sites differ ? Nested update/list inline context and extra stack slot; reference label-helper experiment leaves D608 and loses an accepted neighbor.
- `shell/hercbuy.c` `00416bad` `f_00416bad`: 77 B  7 B differ, first +d ? Widened index/record ESI-EDI exchange. Prior stride, capture and index experiments already exhausted; no invented extra use.
- `shell/hercbuy.c` `00416c65` `f_00416c65`: 282 B  2 B differ, first +c2 ? CH versus CL in selected-byte sum. Prior direct narrow/sum/wide forms recorded by s55; no new evidence warrants repeating them.
- `shell/hercbuy.c` `00416eec` `f_00416eec`: 700 B  17 B differ, first +1c9 ? Only indexed image receiver load/LEA scheduling remains after case order, reference row helper and typed image accessor. Free/member/screen image APIs are neutral.
- `shell/mplaunch.c` `0042070f` `LaunchRefreshSlots`: 202 B  13 B differ, first +67 ? Second-loop scalar/index homes; semantic team accessor worsens to 205 B/D198.
- `shell/mplaunch.c` `00422538` `LaunchRefreshBudgetLabels`: 760 B  73 B differ, first +48; reloc sites differ ? First cache pointer homes and formatting/receiver lifetimes; new value-returning money/limit format helpers give no match.
- `shell/mplaunch.c` `00422830` `LaunchRefreshCostLabels`: 634 B  230 B differ, first +4e; reloc sites differ ? Y/label local order and color/image evaluation; row-coordinate helper worsens to 632 B/D535.
- `shell/mplaunch.c` `00423664` `ShellNetworkSetupRefresh`: 741 B  223 B differ, first +1d5; extent ends 00423949, original function ends 00423961; reloc sites differ ? Ready-label receiver formation and shared cleanup; reference/pointer element accessors give 787 B/D284, rejected.
- `shell/mplaunch.c` `00423b84` `ShellNetworkSetupSelectPlayer`: 459 B  2 B differ, first +17 ? Two jumps to shared 0041fdb5 return; refreshed owner variants do not repair it.
- `shell/mplaunch.c` `00423d4f` `ShellNetworkSetupJoinPlayer`: 249 B  15 B differ, first +1e ? ESI/EDI swap for selected index/status receiver; reference status accessor worsens to 257 B/D160.
- `shell/mplaunch.c` `00423eaa` `LaunchSetPlayerName`: 106 B  16 B differ, first +3 ? Player ESI/destination EBX allocation; name-array accessor gives 107 B/D51. Prior destination/strcpy result captures not repeated.
- `shell/mplaunch.c` `00423f14` `ShellNetworkSetupAction70`: 152 B  10 B differ, first +a ? Screen/selected-player homes and call evaluation; member-method reconstruction gives 154 B/D130.
- `sim/weapai.c` `0045cc00` `WeaponEnabled`: 80 B  3 B differ, first +5 ? Assert pointer EAX instead of EDX; scalar int/unsigned-long capture is neutral. Prior pointer-getter/qualifier probes not repeated.
- `sim/weapai.c` `0045cc60` `WeaponStatus`: 736 B  25 B differ, first +1f8; reloc sites differ ? Visibility stages restore 736-byte extent and D25; mission/global and range lookup homes remain. Capture/getter follow-ups fail.
- `sim/weapai.c` `0045cf40` `WeaponAccuracy`: 480 B  291 B differ, first +44; reloc sites differ ? Skill/pilot argument loads, action mode and later locals; direct pilot argument and Mode accessor families fail.
- `sim/weapai.c` `0045d200` `WeaponExpected`: 384 B  274 B differ, first +d; extent ends 0045d380, original function ends 0045d390; reloc sites differ ? Actor/unit, output-local and arithmetic homes; positive armor branch/comparison reconstruction remains 384 B/D270 and is rejected.
- `sim/weapai.c` `0045d390` `WeaponAmmoFactor`: 160 B  61 B differ, first +9; reloc sites differ ? Score/actor/divisor homes; pointer walk and real ammo-capacity capture worsen D61 to D96/D144.
- `sim/weapai.c` `0045d430` `WeaponEvaluate`: 336 B  215 B differ, first +5a; reloc sites differ ? Maximum now emits the real temporary; assert and inlined maximum result homes still differ. Existing STRICT functions stay preserved.
- `sim/weapai.c` `0045d580` `WeaponTerrain`: 304 B  53 B differ, first +38; reloc sites differ ? Pretested pointer loop restores 304-byte extent and end-pointer/height layout; six Point orders and two position-type forms do not close D53.
- `sim/weapai.c` `0045d830` `WeaponChoose`: 455 B  263 B differ, first +61; extent ends 0045d9f7, original function ends 0045d9f0; reloc sites differ ? Inlined area scoring and stack accumulator; accumulating the stored weight does not repair it.

## Reproduction and scope

- Delivered files: `candidates/t1s/{shell,logic,sim}/` (the six requested TUs). Only hercbuy.c and weapai.c differ from the published source.
- Full-TU try.py receipts: `build/t1s/*-final-try.txt`; source/receipt pairs for rejected hypotheses: `build/t1s/experiments/`.
- `build/t1s/validation.json` records source/candidate hashes and a fresh baseline-to-final STRICT comparison: zero function/status/VA/extent-size losses in all six files.
- Build scratch was redirected into build/t1s by the local sitecustomize helper; every compiler invocation uses tools/cc.py through the prescribed tools. No background job remains.
- New helper/type names are working semantic labels. Existing class/data declarations remain provisional where the original source provides no name/layout proof. No bindings were published.
- Stop condition: all 47 targets classified, productive local source directions exhausted; previous general allocation/flag/compiler/declaration sweeps were not repeated.
