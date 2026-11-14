# Lane t1fm: parked functions in six files

All work started from the published `src/` files (commit 8157b67; unchanged for these six files at 832b188).
Pinned Watcom 10.6 through `tools/try.py` / `tools/promote.py` only. No `src/`, `tools/`, `config/`, `docs/`,
`recovery.json` edits, no `--publish`, no git state changes. Scratch variants, generators and receipts are in
`build/t1fm/` (`v.py` = variant runner, `*v*.txt` = variant sets, `hb_*.c`, `hc_*.c`, `mp_*.c`, `wa_*.c` = snapshots).

Result: 29 of the 47 targets are STRICT in the delivered candidates (30 with the alternative hbupgrad file),
26 counted as new by the dry runs (the table-ownership commit c1eed07 already credits some), 0 lost.

## Promote dry runs (exactly as run)

```text
python tools/promote.py candidates/t1fm/shell/hbupgrad.c --tu src/shell/hbupgrad.c --as 'shell\hbupgrad.c' --profile plain
src/shell/hbupgrad.c: 59 strict functions (12705 bytes), new 5, lost 0
python tools/promote.py candidates/t1fm/shell/hercbuy.c --tu src/shell/hercbuy.c --as 'shell\hercbuy.c' --profile plain-oe
src/shell/hercbuy.c: 35 strict functions (6858 bytes), new 7, lost 0
python tools/promote.py candidates/t1fm/shell/mplaunch.c --tu src/shell/mplaunch.c --as 'shell\mplaunch.c' --profile plain-oe
src/shell/mplaunch.c: 54 strict functions (18391 bytes), new 6, lost 0
python tools/promote.py candidates/t1fm/sim/weapai.c --tu src/sim/weapai.c --as 'sim\weapai.c' --profile ot-oe
src/sim/weapai.c: 8 strict functions (864 bytes), new 3, lost 0
python tools/promote.py candidates/t1fm/logic/miss.c --tu src/logic/miss.c --as 'logic\miss.c' --profile plain-oe
src/logic/miss.c: 10 strict functions (553 bytes), new 3, lost 0
python tools/promote.py candidates/t1fm/logic/intel.c --tu src/logic/intel.c --as 'logic\intel.c' --profile plain-oe
src/logic/intel.c: 12 strict functions (4208 bytes), new 2, lost 0
```

All six end with `dry run OK (add --publish to accept)`.

Alternative, NOT the delivered hbupgrad candidate (see "Size dependency" below; run on a copy named hbupgrad.c):

```text
python tools/promote.py build/t1fm/cmp/hbupgrad.c --tu src/shell/hbupgrad.c --as 'shell\hbupgrad.c' --profile plain
src/shell/hbupgrad.c: 60 strict functions (13074 bytes), new 6, lost 0      (= candidates/t1fm/shell/hbupgrad_size177.c)
```

## Functions that became STRICT, with the change that did it

### shell/hbupgrad.c (plain)
| VA | Label | Change |
|---|---|---|
| 00419901 | f_00419901 | cost list written directly (`sprintf(dst, fmt, cost); SetText(dst)`) instead of the `UpgradeCostText` helper: destination is computed after the format push, as in the original |
| 004199e8 | f_004199e8 | STRICT in the candidate (already credited by the table-ownership rule) |
| 00419b3c | f_00419b3c | follows from the purchase functions getting their original sizes (shared epilogue distance) |
| 0041b912 | f_0041b912 | no `sound` variable: `SoundTrigger(0x78,0)` / `SoundTrigger(0x36,0)` in each branch (original pushes immediates and tail-merges the call) |
| 0041bb4c | f_0041bb4c | same (`0x79` / `0x36`); 355 bytes now by its own body, the helper spelling is no longer needed for size |
| 0041bdda | f_0041bdda | shared-tail distance, follows from the two above |
| 0041b7a1 | f_0041b7a1 | body is exact after removing the `id` local (`*(screen+0x3e7) = entries[selected].id; f(..., *(screen+0x3e7))`); STRICT only in `hbupgrad_size177.c`, 1 byte (jump into the 0041986d epilogue) in the delivered file because f_00419413 is 13 bytes short |

### shell/hercbuy.c (plain-oe) - all 8 targets
| VA | Label | Change |
|---|---|---|
| 00416444 | W?.fn_init | `MenuNumber(signed char font = -1, signed char color = 0x12)` and default-constructed `MenuNumber` objects (as in mplaunch.c) |
| 00416764 | f_00416764 | no `resource` local: store `gResMgr.Load()` into the member and test the member (padding line keeps the assert on 185) |
| 0041686f | f_0041686f | no `HercBuyDataView *screen` alias; f_004176b1 in a natural spelling (`names[i]`, `records[i].name/price`) fits the -oe limit and is inlined as in the original; `HercBuyRefreshLabels` without alias and with `names[baseIndex+i]` |
| 00416bad | f_00416bad | no `screen` alias local |
| 00416c65 | f_00416c65 | no `current` local: `*(obj+0xfd5) = (char)HercRangeValue(); if (*(obj+0xfd5) != *(obj+0xfd6)) ...` (cl instead of ch) |
| 00416eae | f_00416eae | follows (alignment) |
| 00416eec | f_00416eec | `case 3` before `case 2` in the switch (block layout), no `screenCopy`, f_00417732 without alias and with natural `names[]`/`prices[]`, image expression `image*12 + (resource+4)` |
| 00417842 | f_00417842 | shared-tail distance into f_00416eec, follows |

### shell/mplaunch.c (plain-oe)
| VA | Label | Change |
|---|---|---|
| 0042070f | LaunchRefreshSlots | function-level `unsigned team` for the second loop, block-local `team` in the first |
| 00423664 | ShellNetworkSetupRefresh | inline members `SetText()` / `Invalidate()` on the ready label (address computed, then member stores); `screen->loaded=1` once after the switch (original tail-merges the SoundTrigger call and jumps to the common store) |
| 00423b84 | ShellNetworkSetupSelectPlayer | shared-tail distance, follows from the size of 00423664 |
| 00423d4f | ShellNetworkSetupJoinPlayer | no `player` local (`index+1` in place), ready label through `SetText(" ")` / `Invalidate()` |
| 00423eaa | LaunchSetPlayerName | no `--player`: `player-1` used three times |
| 00423f14 | ShellNetworkSetupAction70 | host branch is an inline helper taking the screen (param copy in EDI), `screen->selectedPlayer` read directly |

### sim/weapai.c (ot-oe)
| VA | Label | Change |
|---|---|---|
| 0045cc00 | WeaponEnabled | `if (!c->target.sourceActor) AssertFail(__FILE__, __LINE__, 0);` (the pushed EDX is the known-zero value, not a captured local) |
| 0045d1c0 | WeaponMaximum | `WeaponScore *r`; inline `WeaponMax(int &b, int a)` (one reference parameter = the one stack slot); `int best = WeaponMax(direct, damage); return best * ringCount;` |
| 0045d390 | WeaponAmmoFactor | plain `WeaponScore *r`, member-form assert, `int current = unit->slot[index+11].ammo; ammo = current*100/...` |

### logic/miss.c (plain-oe)
| VA | Label | Change |
|---|---|---|
| 00465c27 | MissionItemAddress | `type = MissionBound(type,0,2); item = MissionBound(item,0,limit[type]-1); return offset[type]+item;` |
| 004654cc | MissionRef::Resolve | follows (inlines the function above) |
| 00465bba | MissionRef::Address | `return MissionItemAddress(type, item);` |

MissionAllocateItem / MissionLoadItem only stay STRICT with the accumulating spelling (`offset = table[t]; offset += item`),
so they call a second inline `MissionItemOffset` (working label). Two spellings of one lookup is a hypothesis: no
single spelling gave all five (48-combination search).

### logic/intel.c (plain-oe)
| VA | Label | Change |
|---|---|---|
| 00467560 | IntelActorCommand | case 3 reads the member after the store (`a[0x28]=value; if (a[0x28]) At(a[0x28])`): `value` stays in its parameter slot, `target` gets ESI |
| 004673b4 | IntelSpawnTimer | shared-tail distance, follows from the size of IntelStateSeek |
| 00467461 | IntelStateLink | STRICT in the candidate (table ownership) |

IntelStateSeek got its original 553 bytes (`if (a[0x22] < 3) return ...; switch ((u32)a[0x22])`: the dead
`mov eax,edx` is the second read of the member), which is what fixes the two functions above; 6 bytes remain.

## Size dependency in hbupgrad (please review)
f_00419413 is still DIFF. Its published body is 164 bytes, the original 177. The shared-epilogue jump of
f_0041b7a1 depends on that size. `candidates/t1fm/shell/hbupgrad_size177.c` differs from the delivered file only
in f_00419413: a `volatile int width` member read through `Width()` plus a `goto` join gives 177 bytes (134 bytes
still differ) and makes f_0041b7a1 STRICT (new 6). I do not consider `volatile` a natural spelling, so it is not
the delivered candidate; it is evidence that the body of f_0041b7a1 is exact.

## New compiler facts (rows for docs/codegen_rules.md)

| Context | Trigger | Effect | Status |
|---|---|---|---|
| C++ static initializer, plain-oe | global objects constructed with omitted default arguments (`MenuNumber n;` with `MenuNumber(signed char=-1, signed char=0x12)`) vs spelled arguments | the register that caches a construction-state constant is chosen by a rotating index (order EAX,EDX,EBX,ECX,ESI,EDI,EBP; one step per state store); each default-argument constructor call advances it by one more. Only a callee-saved slot survives calls, so the cached constant changes (original: 10 in ESI, 0x12 in EAX; spelled arguments: 0x12 in ESI) | verified (hercbuy fn_init STRICT; probes with child argument 7..13) |
| plain / plain-oe / ot-oe | a named local that only aliases or captures a value (`T *screen=(T*)object`, `int id=...`, `void *resource=...`, `long current=...`, `int sound`, `unsigned player=index+1`) | shifts the same rotation: later temporaries land in the next register (CL/CH, ESI/EDI, EBX/ESI swaps, stack slot order of byte counters) with no other instruction change. Removing the alias fixed 9 functions; adding a real one fixed LaunchRefreshSlots and WeaponAmmoFactor. Try this first on 2-16 byte register-swap residues | verified (hbupgrad, hercbuy, mplaunch, weapai) |
| C++ any profile | inline function call (accessor included) inside a call argument while an outer argument is already pushed | caller gets an EBP frame (generalises the nested-default row): `strncpy(dst, Records()[i].name, 16)` has a frame, `((Rec*)records)[i].name` does not | verified (hercbuy f_004176b1, hbupgrad f_00419d52 probes) |
| plain-oe inline limit | cast-heavy spelling of a small function vs natural member/array spelling | the natural spelling is smaller in the inliner measure: f_004176b1 (129 B) is inlined at the default `-oe` only when written with `names[i]` / `records[i]`; no `-oe=N` change needed | verified (hercbuy f_0041686f) |
| inline parameter copy | inline function with a pointer parameter | reads through the parameter copy are not merged with reads through the caller pointer (reloads, `mov esi,ebx` copies); inline member setters (`SetText`, `Invalidate`) compute the object address into a register first, direct member stores fold it into the addressing mode | verified (mplaunch 00423664, hbupgrad f_00419d52 stream part) |
| one `int &` parameter of an inline | `inline int Max(int &b, int a)` called with a local | exactly that local gets a stack slot and one store; later reads use the register copy | verified (WeaponMaximum) |
| assert spelling | `if (!p->member) AssertFail(__FILE__, __LINE__, 0)` vs captured local passed as third argument | member form reuses the tested (zero) register for the push and creates no copy; a captured local leaves a dead `mov eax,edx` when the value is already loaded (inlined WeaponEnabled). The io\file.c scalar-capture rule is not universal | verified (weapai, 3 functions) |
| statements after a switch | same statement at the end of every case vs once after the switch | once after the switch gives the original cross-jumped call and common store; per-case copies let a cached constant (EDI=1) block the merge | verified (ShellNetworkSetupRefresh) |
| per-branch calls | `SoundTrigger(x,0)` in both branches vs `sound=x` + one call | per-branch calls give `push 0; push imm; jmp` into the shared call; the variable form gives `mov eax,imm` | verified (hbupgrad b912/bb4c) |
| switch case order | source order of case bodies | emitted in source order, dispatch at the end; a `break` case placed before a long case puts the post-switch code after it | verified (hercbuy f_00416eec) |
| second read of a member | `if (a[n] < 3) ...; switch ((u32)a[n])` | dead `mov eax,edx` before the dispatch compares (+2 bytes) | verified (IntelStateSeek size) |

## Remaining targets (residue, one line each)

| VA | Label | Residue |
|---|---|---|
| 00419413 | f_00419413 | 164 vs 177 B: original reloads `[range+8]` three times and keeps the flag in EDX (`xor edx,edx; mov eax,edx` on return); only a volatile member reproduces the reloads (rejected). ~35 variants here, >60 in earlier lanes |
| 004194c4 | f_004194c4 | 4 B: the two stat results in EDI/ESI swapped. An inline taking them as parameters fixes the pair but changes the name-loop index form (942 B); 20 variants |
| 00419d52 | f_00419d52 | 286 -> 6 B: stream part as inline `UpgradeStreamStep(screen)`, raw image expression, `direction` local. Left: direction in EDX and index in EBP (candidate ESI/EDI); ~70 variants |
| 0041b7a1 | f_0041b7a1 | 1 B (jump distance), see size dependency |
| 00422538 | LaunchRefreshBudgetLabels | 73 -> 51 B: `y` expression inline, name label through SetText/Invalidate. Left: first money block has the two address registers swapped (EBP/EDI) and the limit label needs the direct SetText/Invalidate form, which flips the mode==1 block; 60 variants |
| 00422830 | LaunchRefreshCostLabels | 230 B: first block keeps `label` in EBP and spills `y`, second block the reverse; original evaluates `player-1` before `y`. 61 variants, none separates the two blocks |
| 0045cc60 | WeaponStatus | 149 -> 25 B, size now 736: `visible = !IsTypeHidden(type)`, mission range first. Left: `cmp byte ptr [eax+ebp],0` (candidate loads CH) and gRemoteActorKind in EBP |
| 0045cf40 | WeaponAccuracy | 297 B: pilot local removed (prefix exact to +4b). Left: sourceSkill/targetSkill registers, `heights` stored and reloaded (looks like another `int &` inline parameter), slot order of range/modifiers |
| 0045d200 | WeaponExpected | 274 B, not reworked |
| 0045d430 | WeaponEvaluate | 57 -> 14 B: only the inlined `enabled & ((1<<index)!=0)` operand registers (EAX/EDX) |
| 0045d580 | WeaponTerrain | 146 B, not reworked |
| 0045d830 | WeaponChoose | 263 B, not reworked |
| 00465580 | MissionEstimate | 4 B: `column` in ESI and `row*5` in EBX swapped; 14 variants |
| 004656a4 | MissionDataInit | 174 B, not reworked |
| 00465994 | MissionCopy | 226 B, not reworked |
| 0046718b | IntelStateSeek | 81 -> 6 B: fifth mode test re-reads through a saved `unit+0x20` pointer (`mov edx,eax` after the add); every natural spelling is merged with the first read |
| 00467690 | IntelActorDispatch | 323 B, not reworked |
| 00467827 | IntelIsAvailable | 196 B, not reworked |

Suggested next step: apply the alias-removal / inline-member levers to WeaponExpected, WeaponTerrain, WeaponChoose,
MissionDataInit, MissionCopy, IntelActorDispatch and IntelIsAvailable; they were only inspected, not searched.

## Candidate files
- candidates/t1fm/shell/hbupgrad.c (delivered), candidates/t1fm/shell/hbupgrad_size177.c (evidence only)
- candidates/t1fm/shell/hercbuy.c, candidates/t1fm/shell/mplaunch.c
- candidates/t1fm/sim/weapai.c, candidates/t1fm/logic/miss.c, candidates/t1fm/logic/intel.c
