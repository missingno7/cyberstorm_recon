# Lane t1fh - parked functions in six files

Baseline note: while the lane ran, tools/compare.py changed (a switch table in front of the next function is
no longer part of the previous extent); f_004199e8, f_00416eae and IntelStateLink became STRICT in src/ through
that rule and are not counted below. All numbers are lane dry-run numbers.

## Promote dry runs (exact commands, summary lines)

```text
python tools/promote.py candidates/t1fh/logic/intel.c --tu src/logic/intel.c --as 'logic\intel.c' --profile plain-oe
src/logic/intel.c: 14 strict functions (4906 bytes), new 4, lost 0
python tools/promote.py candidates/t1fh/logic/miss.c --tu src/logic/miss.c --as 'logic\miss.c' --profile plain-oe
src/logic/miss.c: 12 strict functions (2083 bytes), new 5, lost 0
python tools/promote.py candidates/t1fh/shell/hbupgrad.c --tu src/shell/hbupgrad.c --as 'shell\hbupgrad.c' --profile plain
src/shell/hbupgrad.c: 55 strict functions (11229 bytes), new 1, lost 0
python tools/promote.py candidates/t1fh/shell/hercbuy.c --tu src/shell/hercbuy.c --as 'shell\hercbuy.c' --profile plain-oe
src/shell/hercbuy.c: 29 strict functions (4649 bytes), new 1, lost 0
python tools/promote.py candidates/t1fh/shell/mplaunch.c --tu src/shell/mplaunch.c --as 'shell\mplaunch.c' --profile plain-oe
src/shell/mplaunch.c: 48 strict functions (16458 bytes), new 0, lost 0      (candidate identical to src/)
python tools/promote.py candidates/t1fh/sim/weapai.c --tu src/sim/weapai.c --as 'sim\weapai.c' --profile ot-oe
src/sim/weapai.c: 5 strict functions (560 bytes), new 0, lost 0             (candidate identical to src/)
```

Every run ended with `dry run OK`. Full outputs of the four changed files: build/t1fh/promote_NAME.txt.

## New STRICT (11)

| Label | VA | Change |
|---|---|---|
| IntelSpawnTimer | 004673b4 | body unchanged; exact once IntelStateSeek has its original size (see residue) |
| IntelActorCommand | 00467560 | case 3 tests the stored member (`a[0x28] = value; if (a[0x28] != 0)`), so `value` stays in its stack home and is loaded per arm |
| IntelActorDispatch | 00467690 | rebuilt from the listing: inline flag accessor returning int, true branch first, `count = a[0x22]` reused as loop counter and as the zero argument, `for` loop, case order 1,13,2,3,7/8,4/5/6/9/10,11,12, inner `switch((u32)a[0x1f])`, callee SimUseItem (0044f030) instead of SimFaceTarget |
| IntelIsAvailable | 00467827 | same accessor and branch order; four separate case groups (3 / 4-6 / 7-8 / 9-10) with identical bodies, `switch((u32)a[0x1f])` for 2..4, SimUseItem |
| MissionRef::Resolve | 004654cc | calls `Address()` (inlined by -oe) |
| MissionRef::Address | 00465bba | own body: `t`/`i` locals reassigned through MissionBound |
| MissionEstimate | 00465580 | `int column=--players; column+=row*5;` |
| MissionDataInit | 004656a4 | `for` loop with `id=index+1` computed before the test, in-place type clamp, `index > last` assert, `field4c` assigned from the call directly, `type*2 + (field34/2 + 0x5f)`, `field20 = field20 + 3` |
| MissionCopy | 00465994 | select sequence as an inline helper returning the offset (the offset then lives in a stack slot), record fields instead of an `int *` alias, non-zero branch first |
| f_00419901 | 00419901 | cost column loop spelled like the standalone f_0041b0d2 (`sprintf(buf+i*4,...); SetText(buf+i*4)`) through its own inline `UpgradeCostColumn` |
| f_0041686f | 0041686f | parameter used directly (no local view pointer), f_004176b1 rewritten with typed fields so -oe inlines it, row styling through inline `HercBuyStyleRows`/`HercBuyRefreshRows` |

Review flags for the integrator:
- hercbuy: `HercBuyStyleRows`/`HercBuyRefreshRows` duplicate f_00417219/f_00417732 with the array spelling
  `screen->names[baseIndex+i]`; the existing `HercBuyRefreshLabels` (arithmetic spelling) is still what makes the
  standalone f_00417219 STRICT. One spelling for both contexts was not found (10 spellings tried). 16 padding
  comment lines were removed so the assert at line 785 keeps its line.
- hbupgrad: `UpgradeCostColumn` is used only by f_00419901; `UpgradeCostList` (formatter helper) is kept for
  f_004194c4/b7a1/b912/bb4c because their extents hold the shared-tail distances of later accepted functions.
- intel/miss: alias names replaced by the canonical ones (gVehicleInfo, LogicGenFindType, LogicGenSpawnActor,
  f_0043ac9c, ActorUpdateView, g511ddc, HercMassScale); the AssertFail prototype in miss.c now takes `const char *`.
  Four signature-variant alias notes remain in the intel dry run (CreateActor, LogicGenSpawnActor, ResolveName,
  ActorArrayGet).

## New compiler facts (rows for docs/codegen_rules.md)

| Context | Trigger | Effect | Status |
|---|---|---|---|
| plain-oe compare-tree switch | selector spelled as a second inline-accessor read (`if (a[0x22]<3) ...; switch((u32)IntelState(a))`) vs one local used for both | the accessor form leaves the dead copy `mov eax,edx` in front of the compare tree (2 bytes); the single-local form does not | verified (IntelStateSeek size, IntelSpawnTimer STRICT) |
| plain-oe parameter home | a stack parameter used once per switch arm vs several times in one arm | re-reading the stored member (`a[0x28] = value; if (a[0x28] != 0)`) keeps the parameter unregistered (`mov eax,[esp+N]` per arm); testing `value` again loads it into a register at entry | verified (IntelActorCommand) |
| compare-tree switch ranges | adjacent case groups with identical bodies written as separate groups | each group stays its own range test (`cmp 4; jb; cmp 6; jbe; cmp 8; jbe; cmp 10; jbe`) and the bodies are merged afterwards; one group `case 3..10` gives a single range | verified (IntelIsAvailable) |
| known-zero register as argument | `count = a[0x22]; if (count == 0) { ... f(x, 1, count) ... }` vs literal 0 | passing the variable reproduces `push esi`; the literal is pushed from whichever register was tested zero last (`push eax`) | verified (IntelActorDispatch) |
| -oe inline result | `int offset = InlineHelper(record)` where the helper ends `global = offset; return offset;` | the variable of the caller is stored to its stack slot after the helper body (`mov [ebp-4],edi`) and reloaded later; the same statements written in the caller keep it in a register | verified (MissionCopy) |
| CSE across a branch | `index+1` used in the taken arm and as the loop increment | shared (`lea edi,[eax+1]` before the test, `mov eax,edi` as increment) only when the source computes it before the `if` (`int id=index+1;`) | verified (MissionDataInit) |
| -oe inline size | callee written with many casts vs typed member access (same bytes out of line) | the cast-heavy spelling of f_004176b1 exceeds the default -oe limit and stays a call; the typed spelling is inlined; `-oe=110` inlines the cast form but over-inlines two other functions | verified (hercbuy f_0041686f) |
| -oe inlined array address | `names[k]` (row size 17) inlined at depth 1 vs depth 2 | depth 1 (f_00417219) wants `lea edx,[esi+eax]; lea eax,[edi+0xf6]`, depth 2 (f_0041686f) wants `add eax,esi; lea edx,[edi+0xf6]`; no single spelling gives both | observed, open |
| hbupgrad inlining | `-4s -s -oe=70..80` with real calls f_00419901 -> f_0041b089/f_0041b0d2 | same bytes for the target functions as profile plain with explicit inline helpers; `-oe=85` and above over-inlines f_00419f3c | measured, no gain |

## Remaining targets (36), residue

intel.c
- IntelStateSeek 0046718b: 553 B (original size), 6 B. `SeekAllowed`: original computes `unit+0x20` once, copies it (`add eax,0x20; mov edx,eax`) and reads `[edx+5]` again; every two-accessor spelling is CSE-merged to one load (546 B) or gives `lea edx,[eax+0x20]` (548 B). About 35 spellings in 6 families.

miss.c
- MissionItemAddress 00465c27: 49 B. The out-of-line copy wants the MissionBound body (STRICT with it), but the same body inlined into MissionAllocateItem/MissionLoadItem then loses both (item clamp lands in the register of the limit). About 120 body combinations; the explicit body is kept.

hbupgrad.c
- f_00419413 00419413: 164 vs 177 B. Sibling CommsLogView::Event (00412a75) has identical bytes. Direct `range->width` gives the three loads but folds the first into `imul`; result register, the registers of the 100 constants and the in-place subtraction still differ. About 7,500 combinations (helper/percent/speed/result/wrapper forms), sizes 159..194, none 177; published body kept.
- f_004194c4: 24 B (4 B with the corrected cost loop: stat results ESI/EDI swapped); unchanged.
- f_00419d52: 6 B (was 286): stream read through inline `UpgradeReadStream` plus an `int direction` local give the original frame and extent; direction is in ESI (original EDX), index in EDI (original EBP).
- f_0041b7a1 / f_0041b912 / f_0041bb4c: with the corrected cost loop only the name loop differs (screen copy/index registers EBX/ESI swapped, counter slot swapped), but sizes become 374/577/358 vs 369/570/355 and three accepted functions lose their tail distances; kept on the formatter helper.
- f_00419b3c (3 B), f_0041bdda (1 B): shared-tail displacements into b7a1/b912/bb4c.

hercbuy.c
- W?.fn_init 00416444 (138 B), f_00416eec (700 vs 704 B, 574 B): not worked (time).
- f_00416764 (2 B), f_00416bad (7 B), f_00416c65 (2 B), f_00417842 (4 B): register swaps, unchanged.

mplaunch.c (candidate = src)
- ShellNetworkSetupSelectPlayer: its 2 B are jumps into the epilogue of Scene::Open; they depend only on ShellNetworkSetupRefresh being 0x18 bytes short.
- LaunchRefreshSlots (13 B): team in EAX, slot address in EDX (original reversed); 30 spellings, none.
- LaunchRefreshBudgetLabels, LaunchRefreshCostLabels, ShellNetworkSetupRefresh, ShellNetworkSetupJoinPlayer, LaunchSetPlayerName, ShellNetworkSetupAction70: not worked.

weapai.c (candidate = src)
- WeaponMaximum: the original keeps `directDamage` in a stack slot (`mov [esp],eax`) and presets the result to it (`if (sum > direct) result = sum`); this suggests a reference-taking max helper. Not tested.
- WeaponEnabled (3 B: source actor in EDX while `c` stays in EAX) and the other seven: not worked.
