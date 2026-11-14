# Lane o1h report: parked functions in anim.c, simdo.c, mcp.c, noise.c

Result: 10 new STRICT functions (9 in sim\anim.c, 1 in core\mcp.c), lost 0 in every file. simdo.c and
noise.c are unchanged (no safe gain). Candidate files: candidates/o1h/sim/anim.c, candidates/o1h/core/mcp.c
(candidates/o1h/sim/simdo.c and candidates/o1h/noise/noise.c are byte-identical copies of src).
anim.c is rebuilt by `python build/o1h/anim_apply.py` from git HEAD plus the pieces in build/o1h/anim/
(plan.txt lists the substitutions; the script keeps every ASSERT on its original line: 541, 625, 664, 758,
783, 851, 891).

## Promote dry runs

```
python tools/promote.py candidates/o1h/sim/anim.c --tu src/sim/anim.c --as 'sim\anim.c' --profile ot-oe
src/sim/anim.c: 23 strict functions (2289 bytes), new 9, lost 0

python tools/promote.py candidates/o1h/core/mcp.c --tu src/core/mcp.c --as 'core\mcp.c' --profile plain-oe
src/core/mcp.c: 67 strict functions (7630 bytes), new 1, lost 0

python tools/promote.py candidates/o1h/sim/simdo.c --tu src/sim/simdo.c --as 'sim\simdo.c' --profile ot-oe
src/sim/simdo.c: 24 strict functions (4210 bytes), new 0, lost 0

python tools/promote.py candidates/o1h/noise/noise.c --tu src/noise/noise.c --as 'noise\noise.c' --profile plain-oe
src/noise/noise.c: 14 strict functions (1718 bytes), new 0, lost 0
```

## anim.c: what changed

The raw `unsigned char *p` / `volatile` / union code was replaced by typed records (all `volatile` gone,
16 -> 0): `AnimAction` (0x67 bytes, packed) with an `AnimFlags` union at +0x0a whose bits are 1-bit fields
(`done` bit 0, `stepped` bit 2, `loop` bit 5, `running` bit 7; bits 4..6 survive a notify), `ExploPoint`
with an inline `operator!=`, `AnimBlock` (0x494 bytes: ids[128], costs[128], marks[128], total, count,
ready, ?, actor), `AnimBitmap`, `AnimFrameRecord`/`AnimDrawInfo`, `AnimActionState` (5-byte head + position,
destination, target, heading). `WorldActionNotify` takes `int state`, `AnimActionInitialize` takes four
`int`s and calls `WorldActionNotify(action, 0)`. The STRICT neighbours AnimActionClear, AnimActionInitialize
etc. are now natural code and stay STRICT. Parameter types of several functions changed (prototype
spelling; the dry run accepts them with lost 0).

## New STRICT functions

- `WorldActionNotify` 004573f0: bitfield flags; clear the five low/high fields, then `running = 1; dirty = 1;` (int state parameter).
- `AnimActionStep` 00457470: early `return 0` guard, `AnimFlags flags = action->flags; if (flags.loop)`, `++frame >= frames` forms, `done = 1; running = 0;`, `stepped = 0`.
- `AnimActionClearGroup` 00457650: written out (count via `AnimActionGetMetadata`, `int list[10]; int index = action->target;`, slot through `AnimResourceSlot`) instead of an inlined `AnimActionAssign`.
- `AnimActionReplace` 004576d0: count read directly from the metadata table, both release blocks written out with `int first[10], second[10]` (first list at the higher address, as in the original).
- `AnimActionSetExternalBlock` 00458ad0: `if (!actor || !sourceId || action->IsMoving()) AssertFail(...)`, inline `AnimActionCreateBlock`, typed `action->block->...` stores.
- `AnimActionInitTargetActor` 00458bf0: `if (!(action->block && action->block->ready == 1 && action->block->ids[1])) AssertFail(...)` (direct member chain; it alone leaves the original dead `mov eax,edx`).
- `AnimActionConfigure` 00458c90: `if (state == 1 && AnimActionHasMetadata(action)) { WorldActionNotify(action, 12); frame = index < 0 ? frame : 0; markIndex = index; markFrame = frame; }` with `int frame`.
- `AnimActionInterpolateDrawPos` 004579e0: typed frame record, `record += action->variant`, scalar offsets declared `int y, x;` (spill slots x at [esp], y at [esp+4]).
- `AnimActionCopyChangedFields` 00458ea0: `const ExploPoint &position = state->position; if (position != action->position) action->position = position;` (same for destination).

## New STRICT function in mcp.c

- `f_0043d25f` 0043d25f: in the inlined bit setter `f_0043d1a6` the mask is `int mask = 1<<(index%8)` instead of `U8 mask = (U8)(...)`; f_0043d1a6 and f_0043d1fd stay STRICT, the caller's session/value homes (EDI/EBP) become original.

## Codegen facts (docs/codegen_rules.md rows)

| Context | Trigger | Effect | Status |
|---|---|---|---|
| ot-oe, 1-bit fields in a packed byte (sim\anim.c) | consecutive `f.a = 0; f.b = 0; ...` then `f.top = 1; other = 1;` | the zero stores merge into one `mov dl,[x]; and dl,mask; mov [x],dl`; setting the next field BEFORE an unrelated byte store gives `mov dh,dl ... or dh,80h` (no reload); writing `other = 1` first forces `mov dh,[x]` (reload). The volatile/union spellings are unnecessary | verified (WorldActionNotify, AnimActionInitialize) (o1h, 2026-11-14) |
| ot-oe, bitfield test | `AnimFlags flags = action->flags; if (flags.loop)` vs `if (action->flags.loop)` | local struct copy gives `mov al,[x]; test al,20h` and moves the later byte temporaries (ah/dl, dh/cl, ch) to the original homes; the direct test gives `test byte ptr [x],20h`. `==1`, `!=0`, `&1`, signed, int-based, multi-bit and accessor spellings all add shifts | verified (AnimActionStep) (o1h, 2026-11-14) |
| ot-oe, inline parameter type | inline callee with `int` parameter stored into a byte/word field, called with a constant | the constant is materialised in a register (`xor edx,edx; mov [state],dl`) and reused for other zero stores of the inline (`mov [deadline],edx`); an `unsigned char` parameter folds to an immediate store | verified (AnimActionInitialize, ActorClose inlines) (o1h, 2026-11-14) |
| ot-oe, assert member chain | `!(a->block && a->block->ready == 1 && a->block->id)` vs inline `Ready()` / captured `block` local | only the direct chain gives the original short-circuit with the dead `mov eax,edx`; the inline method materialises a bool, the captured local drops the reload after the assert | verified (AnimActionInitTargetActor) (o1h, 2026-11-14) |
| ot-oe, two aggregate locals from inline expansions | helper with its own `int list[10]` inlined twice vs both lists declared in the caller | inline expansions place the first expansion's array at the LOWER offset; the original had the first-used list higher: two declared arrays `int first[10], second[10]` | verified (AnimActionReplace) (o1h, 2026-11-14) |
| ot-oe, struct copy after inline `operator!=` | `const T &src = state->m; if (src != dst) dst = src;` vs `if (state->m != dst) dst = state->m;` | the reference keeps the left operand pointer (ESI) alive as the movsd source and lets the bool take the right operand's register; the direct form recomputes ESI | verified (AnimActionCopyChangedFields); no effect on AnimActionEqual (o1h, 2026-11-14) |
| ot-oe, spill slots of two scalars | `int x = ..., y = ...` spilled across calls | the LATER declared scalar gets the lower slot: `int y, x;` puts x at [esp], y at [esp+4] | verified (AnimActionInterpolateDrawPos) (o1h, 2026-11-14) |
| plain-oe, inlined bit setter | `int mask = 1<<(n%8)` vs `U8 mask = (U8)(1<<(n%8))` in a small helper inlined into a loop | same standalone bytes; in the inlining caller the byte-typed mask swaps the homes of the caller's two parameters (EDI/EBP) | verified (f_0043d25f) (o1h, 2026-11-14) |
| ot-oe, constructor this-adjust | a base/member subobject at +0x12 initialised from a temporary in a constructor | gives `add eax,12h; mov edi,eax; sub eax,12h` around the movsd and a 0xc frame; the original ActorClose has `lea edi,[eax+12h] ... lea eax,[edi-12h]` with an 8-byte frame; free-function spellings never re-derive EAX | measured, open (ActorClose) (o1h, 2026-11-14) |

## Remaining functions (residue, one line each)

sim\anim.c
- `ActorClose` 00457510 (AnimAction constructor): all stores, inlined Initialize/SetState and frame match except the original `lea eax,[edi-12h]` after the position copy (3 bytes, shifts the rest); 5 position-copy spellings, return types and 3 constructor/base layouts tried (build/o1h/t/ac*.c).
- `AnimActionEqual` 00458e20: 13 bytes, EBX/ESI swap of action vs &state->position; operator form/operand order/const/reference/result-local variants invariant.
- `AnimActionPrepareBlock` 00458840: D423 -> D177; remaining: ESI/EDI swap of sourceId and loop index/offset, second visibility predicate loads the byte instead of `cmp byte ptr [edx+eax],0`, spill slots of limit/resource swapped.
- `AnimActionUpdateTrajectory` 004577d0: D497 -> D328 (needs --at); typed rewrite has the original shape, actor EBP vs EDI, state EDI vs EBP and distance/lift homes differ.
- `AnimActionCommitTrajectory` 00457af0 (1200 B) and `ActorDrawEffect` 00457f90 (1840 B): not reworked (still raw-pointer code); the new AnimAction/AnimBlock types are the starting point.

sim\simdo.c (unchanged)
- `ModeCheck` 0044eb00: `int status = s->slots[s->mode-1].status; if (status == 1 || status == 3) return 1; return 0;` matches every instruction except status in EAX vs EBX (D54 standalone); 30 spellings tried; this variant worsens the inlined copies (SimFaceTarget D68 -> D82), so src is kept.
- `SimFaceTarget` 0044e420, `IntelClearMode` 0044f990: inlined ModeCheck address form (`lea eax,[eax*4]; add eax,base; mov r,[eax+2deh]`) and status register (EDX / ESI) differ; same root as ModeCheck.
- `SimUseItem` 0044f030: 6 bytes, EAX/EDX swap of item+19 and unit for PopFindItem; 13 spellings (aliases, direct PopFindItem, switch forms) invariant.
- `SimFireFocus` 0044eb50: body STRICT; the following switch table belongs to SimExecuteItem and differs with it.
- `SimCenterOn` 0044d980: a `ViewState *view = &s->view` alias in the inner block makes everything after the compare original (D134 -> D112) but the compare keeps point in ESI instead of view EBX / point EDI; not kept.
- `SimPanBy`, `SimSetFocus`, `SimCenterTarget`, `SimExecuteItem`, `SimSetReturnFire`, `SimRandomFire`, `SimReturnShot`, `SimBeginTurn`: not reworked.

core\mcp.c
- `f_0043bbcc`: DEC after base add; 16 length/index spellings: `base+(strlen(base)-1)` gives the right order but `lea edx,[eax-1]` (+1 byte).
- `f_0043c5d6`, `f_0043d591`, `f_0043d2b9`, `f_0043c844`, `f_0043cab9`, `f_0043dbb2`, `f_0043e21a`, `f_0043e37c`, `f_0043e04d`, `f_0043cb47`: unchanged shared-tail / dialog-frame cluster (see p6, s30, s51); original c5d6 shows the inlined f_0043b956 conversion with window RECT at ebp-10h, client at ebp-20h and `GetString(250,1)` pushed inside the dialog call (EBP frame).

noise\noise.c (unchanged)
- `SoundAllowed` 0046c3fc and its inlined copies in SoundCache, SoundCacheList, SoundLoad, SoundPlayDefault, SoundPlay: threshold DH vs AL; struct-copy / reference / switch record spellings are worse (D56-D99); not kept.
- `SoundCreateBuffer`, `SoundSetSettings`: not reworked.

## Experiments (build/o1h/)

anim pieces and plan: build/o1h/anim/*.txt, build/o1h/anim_apply.py; isolated compiler probes:
build/o1h/t/bf.c, st*.c, ac*.c; templates: build/o1h/*.tpl.c (useitem, mode, center, d25f, d1a6, bbcc, snd,
init, close, target, eq, replace, clear, prep, interp, upd).
