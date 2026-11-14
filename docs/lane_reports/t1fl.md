# Lane t1fl: parked functions in six files

Candidates: `candidates/t1fl/{sim/weapai.c, logic/miss.c, logic/intel.c, shell/hercbuy.c, shell/mplaunch.c}` (changed),
`candidates/t1fl/shell/hbupgrad.c` (identical to the published file, no gain). 20 targets became STRICT, none lost.

## Promote dry runs (exactly as run) and summary lines

```text
python tools/promote.py candidates/t1fl/sim/weapai.c --tu src/sim/weapai.c --as 'sim\weapai.c' --profile ot-oe
src/sim/weapai.c: 11 strict functions (2047 bytes), new 6, lost 0
python tools/promote.py candidates/t1fl/logic/miss.c --tu src/logic/miss.c --as 'logic\miss.c' --profile plain-oe
src/logic/miss.c: 11 strict functions (1791 bytes), new 4, lost 0
python tools/promote.py candidates/t1fl/logic/intel.c --tu src/logic/intel.c --as 'logic\intel.c' --profile plain-oe
src/logic/intel.c: 14 strict functions (4906 bytes), new 4, lost 0
python tools/promote.py candidates/t1fl/shell/hercbuy.c --tu src/shell/hercbuy.c --as 'shell\hercbuy.c' --profile plain-oe
src/shell/hercbuy.c: 32 strict functions (5643 bytes), new 4, lost 0
python tools/promote.py candidates/t1fl/shell/mplaunch.c --tu src/shell/mplaunch.c --as 'shell\mplaunch.c' --profile plain-oe
src/shell/mplaunch.c: 50 strict functions (17682 bytes), new 2, lost 0
python tools/promote.py candidates/t1fl/shell/hbupgrad.c --tu src/shell/hbupgrad.c --as 'shell\hbupgrad.c' --profile plain
src/shell/hbupgrad.c: 54 strict functions (11025 bytes), new 0, lost 0
```

## New STRICT functions (20)

| Label | VA | Change that did it |
|---|---|---|
| WeaponEnabled | 0045cc00 | assert as `if (c->target.sourceActor == 0) AssertFail(__FILE__, __LINE__, 0);` (no captured local) |
| WeaponMaximum | 0045d1c0 | helper `inline int Larger(int a,const int &b)` with the `direct` local passed by reference; `best*=ringCount; return best;`; fields through `r->weapon.` |
| WeaponExpected | 0045d200 | one combined null test; reads `c->target.modifier` (0xb4), not targetDirection; `factor` local reused for the damage argument; `armorDamage > directDamage`; `if (armor) {...} else result/=2`; declaration order `unit, direction, other` |
| WeaponAmmoFactor | 0045d390 | `== 0` assert with literal 0; `int left=unit->slot[index+11].ammo;` captured before the percentage |
| WeaponEvaluate | 0045d430 | follows from WeaponEnabled and WeaponMaximum (both inlined here) |
| WeaponChoose | 0045d830 | `weight>=minimum`; `ActorUnit *unit=actor->unit;` before PopFindItem; `unsigned roll` then `int choice=roll`; assert on line 684 |
| MissionRef::Resolve | 004654cc | calls `Address()` (member with its own body) instead of MissionItemAddress |
| MissionRef::Address | 00465bba | locals from the members, `type=MissionBound(type,0,2); item=MissionBound(item,0,limit[type]-1); return offset[type]+item;` |
| MissionDataInit | 004656a4 | roll loop as `for (index=first; index<=last; index++, p++)`; `MissionBound(record->type,0,2)`; `record->field20 + 3`; `int estimate = record->field4c = MissionEstimate(...)`; grouping `type*4 + (field34+100)` etc. |
| MissionCopy | 00465994 | inline helper `MissionSelect(record)` (uses `ClampRange(type)` defaults, loads palette, returns offset) whose result is the caller's `offset`; record fields used directly; `if (image) {...} else banner = 0` |
| IntelSpawnTimer | 004673b4 | follows from IntelStateSeek reaching its original size (553 B) via `if (a[0x22]<3)` + `switch((u32)IntelState(a))` |
| IntelActorCommand | 00467560 | case 3 tests and passes `a[0x28]` (the field just stored), not the `value` parameter |
| IntelActorDispatch | 00467690 | leader test through an inline bool accessor, leader branch first; `int state = IntelState(a)` reused as the zero argument; `for (count = 0; ...)` item loop; case order 1, 13, 2, 3, 7/8, 4/5/6/9/10, 11, 12; else branch `if (a[7] >= 10 && a[7] <= 16) switch (a[0x1f])`; calls SimUseItem (0044f030), not SimFaceTarget |
| IntelIsAvailable | 00467827 | same shape: `switch (ActorArrayGet(a, 7))` with three case groups calling IntelStateLink, SimUseItem, else branch range test + inner switch |
| f_0041686f | 0041686f | no local copy of the screen pointer (the `object` argument is used directly, also in the inlined label loop); f_004176b1 rewritten with typed fields so that it falls under the `-oe` limit and is inlined here |
| f_00416bad | 00416bad | `object` used directly (no `screen` local) |
| f_00416eec | 00416eec | case order 0, 1, 3, 2 in the source; `f_00417732(object, base)` called directly (no `screenCopy`); f_00417732 without a local copy and with plain array forms; image address as `image*0xc + (resource + 4)` |
| f_00417842 | 00417842 | follows from the sizes above (shared tail) |
| ShellNetworkSetupRefresh | 00423664 | ready labels through the inline members `SetText()` / `Invalidate()` instead of direct field stores; `screen->loaded=1` once after the switch (inside the `if`) |
| ShellNetworkSetupSelectPlayer | 00423b84 | follows from ShellNetworkSetupRefresh (shared tail position) |

Names introduced here (`Larger`, `MissionSelect`, `IntelIsLeader`, locals) are guesses. f_00416eae, f_004199e8 and
IntelStateLink are STRICT in the published baseline already and are not counted.

## Compiler facts (rows for docs/codegen_rules.md)

| Context | Trigger | Effect | Status |
|---|---|---|---|
| asserts, ot-oe | `if (p->m == 0) AssertFail(__FILE__, __LINE__, 0);` vs a captured local passed as third argument | literal-0 form loads the member into EDX and pushes that known-zero register; captured-local form uses EAX and leaves a dead `mov eax,edx` in inlined copies | verified (WeaponEnabled, WeaponAmmoFactor, WeaponEvaluate) |
| known-zero / known-constant registers | register proven zero by `test r,r; jne`, or holding a constant local (`result = 1`) | later `push 0` / `push 1` and `count = 0` use that register; a state kept in a local that survives calls shows up as `push esi` | verified (IntelActorDispatch) |
| by-reference inline operand | `inline int Larger(int a,const int &b)` called with a local | the local gets a stack slot and one dead store, the comparison stays in registers | verified (WeaponMaximum) |
| local copy of an object pointer | `T *screen=(T*)object;` vs using `((T*)object)->` directly, plain-oe | the copy is a second value: extra spill slot, pointer kept in a register, and base+index adds come out as `lea edx,[idx]; lea eax,[base]` instead of `add eax,idx; lea edx,[base]`. Without the copy the parameter is reloaded from its stack home per statement | verified (hercbuy f_0041686f, f_00416bad, f_00416eec, f_00417732) |
| `-oe` limit and source spelling | cast-heavy pointer arithmetic vs typed field access in a small function | the same machine code counts as a different inline size: the typed form of f_004176b1 is inlined at `-oe` (=100), the cast form is not. Check under-inlining by simplifying the callee before changing `-oe=N` | verified (hercbuy) |
| switch layout | code after a switch | placed right after the first case body that breaks to it; later case bodies and the dispatch follow. So the source case order is visible (case 3 before case 2), and a statement common to all cases belongs after the switch | verified (f_00416eec, ShellNetworkSetupRefresh) |
| inline member vs field store | `labels[n].SetText(s); labels[n].Invalidate();` vs `labels[n].text=s; labels[n].flags&=~1;` | members compute the element address into a register (`shl; add eax,table; mov [eax+0x24],imm`), direct stores use `[eax*8+table+off]` | verified (ShellNetworkSetupRefresh) |
| inline helper result | `int offset = Helper(record);` where the helper ends `global = offset; return offset;` | the caller's variable is a separate value, spilled right after the helper's last store | verified (MissionCopy) |
| default arguments inside an inline helper | helper calls `ClampRange(type)` with two defaults | the EBP frame appears in the function it is inlined into (MissionCopy); the written-out call has none (MissionDataInit) | verified |
| `if (range) switch` | `if (x >= 10 && x <= 16) switch (y) {small}` | `cmp 10; jl end; cmp 16; jle dispatch; jmp end` with the compare tree after the bodies; a nested switch on x gives unsigned `jb/jbe` | verified (IntelActorDispatch, IntelIsAvailable) |
| comparison operand order | `minimum<=weight` vs `weight>=minimum` on memory operands (ot-oe) | not canonicalised; the second gives `mov r,[minimum]; cmp r,[weight]; jg` | verified (WeaponChoose) |
| loop increment CSE | `for (i=first; i<=last; i++, p++)` with `i+1` used in the body | `lea edi,[eax+1]` once, reused as the increment | verified (MissionDataInit) |
| commutative address add, per expansion | `base + index*k` in an inlined body | operand order can differ between two expansions of the same source (standalone f_00417219 vs its copy in f_0041686f); it follows whether the base is a copied local or the parameter itself | observed (hercbuy) |
| memory CSE, plain | repeated `r->width`: direct field vs inline accessor / inline argument | direct fields are reloaded each time, accessor forms are merged into one load | measured (hbupgrad f_00419413, unsolved) |

## Remaining targets

### sim/weapai.c
- WeaponStatus 0045cc60: size now right (736 B) with `int visible=!IsTypeHidden(type)` / `if (visible)`; 26 B differ: gRemoteActorKind temp in EDX instead of EBP, gMissionKind loaded after the visibility test instead of before. ~250 variants.
- WeaponAccuracy 0045cf40: allocation; published body kept. Best seen 38 B (source declared before target, no `heights` local): spill slots of skill/modifiers and UnitCapability argument registers swapped. ~190 variants.
- WeaponTerrain 0045d580: now a `for (; offset!=end; ++offset,++weightIndex)` loop with the right size (304 B), 53 B differ: stack order of the four Points, a dead `cmp ebp,ebx` before the loop entry, height/end homes swapped. ~200 variants.

### logic/miss.c
- MissionEstimate 00465580: 4 B; `column` in EBX and `row*5` in ESI, original the other way round. ~100 variants.
- MissionItemAddress 00465c27: 6 B; the standalone copy wants `add eax,[offset+type*4]`, the copies inlined in MissionAllocateItem/MissionLoadItem want the offset loaded first; no single body gives both (~450 combinations). The body that keeps the two accepted callers STRICT is used.

### logic/intel.c
- IntelStateSeek 0046718b: 553 B (original size), 6 B. In case 3 the fifth action-type comparison re-reads the byte through the saved `unit+0x20` pointer; every form tried is merged into one load except the existing `this+0x25` access, which re-reads through the wrong base. ~130 variants.

### shell/hercbuy.c
- W?.fn_init 00416444: 138 B, not worked.
- f_00416764: 2 B, `input` argument in EDI vs ESI; 12 spellings.
- f_00416c65: 2 B, `(char)index` byte in CL vs CH; 20 spellings.

### shell/mplaunch.c
- LaunchRefreshSlots 0042070f: 13 B, team value and slot address registers swapped (EDX/EAX); 24 spellings.
- LaunchRefreshBudgetLabels 00422538: 73 B, EBP/EDI swapped in the money block. Seen but not adopted: 58 B with the y expression written inside both BriefBlit calls; the limit label wants the direct `labels[i].SetText(); labels[i].Invalidate()` form but that alone changes the size to 759.
- LaunchRefreshCostLabels, ShellNetworkSetupJoinPlayer, LaunchSetPlayerName, ShellNetworkSetupAction70: not reached.

### shell/hbupgrad.c (no change proposed)
- f_00419413: evidence only: original is `result = g4b709c; if (result) {...; result = 0;} return result;` with both `*150/100+5` tails merged after the `*5` step and three reloads of `range->width`. Best 169 B vs 177: width must be loaded into a register before `imul`, 100 loaded after the sign extension, result in EDX. ~2400 variants (percent helpers, accessors, volatile, types).
- f_00419901, f_004194c4, f_00419b3c, f_0041bdda: depend on the sizes of f_00419413 / f_0041b7a1 / f_0041b912 (table alignment, shared tails).
- f_00419d52, f_0041b7a1, f_0041b912, f_0041bb4c: not reached; the hercbuy findings (no local copy of the screen pointer, case order, helper spelling vs `-oe` limit) are the first things to try.
