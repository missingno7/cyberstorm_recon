# Lane o1m report (parked functions in anim.c, simdo.c, mcp.c, noise.c)

Result: 9 new STRICT functions, all in src/sim/anim.c; 0 lost anywhere. simdo.c and noise.c carry
non-STRICT improvements only; mcp.c is unchanged (copy of src, no accepted change found).

## Dry runs (exactly as run)

    python tools/promote.py candidates/o1m/sim/anim.c --tu src/sim/anim.c --as 'sim\anim.c' --profile ot-oe
    src/sim/anim.c: 23 strict functions (2145 bytes), new 9, lost 0

    python tools/promote.py candidates/o1m/sim/simdo.c --tu src/sim/simdo.c --as 'sim\simdo.c' --profile ot-oe
    src/sim/simdo.c: 24 strict functions (4210 bytes), new 0, lost 0

    python tools/promote.py candidates/o1m/core/mcp.c --tu src/core/mcp.c --as 'core\mcp.c' --profile plain-oe
    src/core/mcp.c: 66 strict functions (7540 bytes), new 0, lost 0

    python tools/promote.py candidates/o1m/noise/noise.c --tu src/noise/noise.c --as 'noise\noise.c' --profile plain-oe
    src/noise/noise.c: 14 strict functions (1718 bytes), new 0, lost 0

All seven anim.c ASSERT lines are still 541/625/664/758/783/851/891 (checked after every edit).

## What changed in anim.c (the common cause)

The file was written with `void *action` + byte-pointer casts and `volatile` stores. The real code is a typed
0x67-byte record, `struct AnimAction` (pack 1), with typed parameters. A `void *` parameter plus an aliasing
`AnimAction *a = (AnimAction *)action` local was enough to keep Equal/CopyChangedFields at D107/D111, and the
same bodies with typed parameters were STRICT. Layout written down (working names): depth(0) type state angle
rate frame count flags ready(0xb) kind(u16 0xc) time(0xe) position(AnimPair 0x12) target(0x1a) resource(0x22)
direction(0x26) x,y(0x2a, also viewed as ExploPoint point) block(AnimBlock* 0x32) b36..b3a drawX/Y/Width/Height
(0x3b..0x47) bitmap(0x4b) from(0x4f) to(0x57) step(0x5f) steps(0x63). `AnimBlock` (0x494): path[128], cost[128],
marks[128], budget, count, index, unknown48c, actor. `AnimState` (CopyState output): header[5], position, target,
resource, direction.

## New STRICT functions (label, VA, the change that did it)

- WorldActionNotify 004573f0: `frame=0; count=0; state=s; time=0; ready=1;` then `flags&=0x70; flags|=0x80;`
  (ready store BEFORE the two flag ops, typed record); the `mov dh,dl` copy appears only in that order.
  Parameter is `int state` (needed for its inlined copies, see Configure/Initialize).
- AnimActionStep 00457470: typed; one guard `if (!AnimActionAdvance(a) || !a->count || a->state == 0) return 0;`,
  loop flag tested as `(unsigned)a->flags & 0x20`, `++a->frame >= a->count` in both arms.
- AnimActionClearGroup 00457650: `int list[10];` declared first, count read directly
  `animationMetaAt0cbc[a->kind*15]` (not via the GetMetadata helper), if/else body.
- AnimActionReplace 004576d0: same first half as ClearGroup (own count/index/list, direct table read), then
  GridPointToIndex/BestLocSetData (the bound names of 42afb0/42b290) and the inlined AnimActionAssign(a, value,
  a->resource, count) reusing `count`.
- AnimActionSetExternalBlock 00458ad0: typed AnimBlock stores through `a->block->...` (reloaded every time, as in
  the original), guard `if (!(actor && sourceId && !AnimActionBusy(a)))` with inline
  `AnimActionBusy = state!=1 && state!=7 && state!=14 && state!=17 && state!=18`.
- AnimActionInitTargetActor 00458bf0: assert as member expression
  `if (!a->block || a->block->index != 1 || !a->block->path[1]) AssertFail(...)`, then
  `TargetObjectInit(fallbackName,&a->to,a->block->path[1]); a->from.Copy(a->position);`.
- AnimActionConfigure 00458c90: `int frame` parameter (not unsigned char), `if (state==1 && HasMetadata(a))`,
  inlined `WorldActionNotify(a, 12)`, `frame = (index<0) ? frame : 0;` then `b39=index; b38=frame;`.
- AnimActionEqual 00458e20 and AnimActionCopyChangedFields 00458ea0: typed `(AnimAction *, AnimState *)`
  parameters, `AnimPair::operator!=` and one reused pointer local
  `AnimPair *pair = &s->position; if (*pair != a->position) ...; pair = &s->target; ...` (CopyChanged uses
  `a->position.Copy(*pair)`).
Already STRICT functions converted to the typed form and still STRICT: AnimActionClear, AnimActionInitialize
(now `type; kind; rate; flags; WorldActionNotify(a,0)` with int parameters; volatile hack gone), AnimActionIsSet
(`(unsigned)a->flags & 1`), AnimActionAdvance, AnimActionHasMetadata.

## New compiler facts (docs/codegen_rules.md rows)

| Context | Trigger | Effect | Status |
|---|---|---|---|
| ot-oe, typed record vs `void *` | `void *action` parameter plus `T *a = (T *)action` alias local vs a typed `T *a` parameter, same body | alias form swaps the two pointer homes and loses the CSE of the compared address (Equal D107 -> STRICT, CopyChangedFields D111 -> STRICT); corroborates the t1fm alias row for parameters | verified (o1m, anim.c, 2026-11-14) |
| ot-oe, inline with a byte store of a parameter | inlined callee whose parameter is `int` but is stored into a byte/word field (`state = s`), called with a constant | the constant stays in a register (`mov edx,0xc; mov [x],dl`); an `unsigned char` parameter folds to `mov byte ptr [x],imm`. Notify/Configure/Initialize/ctor all need the int form; standalone Notify bytes are identical either way | verified (o1m: Configure, Initialize STRICT; ctor D97 -> D21) |
| ot-oe, consecutive RMW of one byte field | `ready = 1;` placed before `flags &= 0x70; flags |= 0x80;` vs between them | only the first order gives the original `mov dh,dl` copy (CSE of the stored byte); with the store in between Watcom reloads `mov dh,[eax+0xa]` (s17's bitfield negative) | verified (WorldActionNotify) |
| ot-oe, bit test of a byte field | `(unsigned)a->flags & 0x20` / `& 1` vs `a->flags & 0x20` / bitfield read | cast form gives `mov al,[x]; test al,0x20` (and `xor eax; mov al; and eax,1` in a return); plain form gives `test byte ptr [x],0x20` | verified (Step, IsSet) |
| ot-oe, address-taken locals in a function that inlines a helper with its own array | local `int list[10]` declared before the scalars, count read directly from the table | Replace/ClearGroup STRICT; calling the inline GetMetadata accessor for count adds a register copy (`mov esi,eax`) | verified (Replace, ClearGroup) |
| ot-oe, inline predicate in a condition | `if (r < 0 && !Hidden(t))` with `inline int Hidden(int t){return !table[t];}` vs `table[t] != 0` | helper form keeps the original `cmp byte ptr [..],0; sete cl; and ecx,0xff; jne` materialisation; the direct form folds to `mov cl; and; je`. `== 0` inside the helper widens first (C++ explicit compare) | verified shape (PrepareBlock, still DIFF for other reasons) |
| ot-oe, assert over a member chain | `if (!a->block || a->block->index != 1 || !a->block->path[1]) AssertFail` vs a captured `block` local | member form gives the original `mov eax,edx` copy and post-assert reload; captured local D93 | verified (InitTargetActor) |

## Remaining functions (residue, one line each)

anim.c
- 00457510 ActorClose: is `AnimAction::AnimAction()` (returns this in eax, blocked.md). Candidate now has the real
  constructor: Clear; zero pair Copy; frame/count/ready/b36/time/direction; AnimActionInitialize(this,1,0,50,0);
  time=0; block=0; SetState(1) -> D21 (was D97). Left: one early `time` store with EDX and the missing
  `lea eax,[edi-0x12]` after the pair copy. Its label becomes `AnimAction::$ct` (integrator: rename/binding).
- 004577d0 AnimActionUpdateTrajectory: typed rewrite D497 -> D44, correct 528-byte size; left: &point/&position
  argument registers (EAX/EDX swapped) and the curve term order (`int curve` computed before the sum gives the
  right homes but the wrong instruction order).
- 004579e0 AnimActionInterpolateDrawPos: typed rewrite reaches D241 (not kept, D253 kept): original does the
  y -= h/4 as a memory RMW and keeps angle/ends in ESI (movzx); 4 spellings invariant.
- 00457af0 AnimActionCommitTrajectory, 00457f90 ActorDrawEffect: not reworked (1046/1620 bytes, still byte-pointer
  style); the typed record is now available for them.
- 00458840 AnimActionPrepareBlock: typed rewrite, correct 656-byte size, D423 -> D240; left: sourceId/resource
  ESI/EDI swap and one extra 4-byte stack slot (frame 0x18 vs 0x14) around actorIndexTable[previous*2].
simdo.c
- 0044f520 SimReturnShot: early `return 0`s plus an `int result=0` returned at the end (original always returns
  the EBP zero) D261 -> D200, size 334 vs 336; old/focus live-range split (EBP -> ESI after SimAdoptFocus) not
  reproduced. Uses `(signed char)target->active == 0` for the original movsx (field view open).
- 0044eb00 ModeCheck: 13 spellings (aliases, NetworkOwner(), slot accessors, switch); best D37; original reloads
  the parameter and keeps status in EBX; its inlined copies (SimFaceTarget etc.) want `[eax+0x2de]` addressing.
- 0044f030 SimUseItem: D6 invariant under 10 forms (direct PopFindItem, ActorRecord::Item, captures).
- 0044eb50 SimFireFocus: its 16 bytes are SimExecuteItem's jump table (follows SimExecuteItem's layout).
- 0044d980 SimCenterOn: `GridPoint *view` local / reference / operator spellings D120 best; original keeps s in
  EBP and loads view.x first; not kept.
- 0044d850, 0044dd40, 0044e420, 0044e650, 0044ebe0, 0044f0d0, 0044f290, 0044f990, 0044fa30: not reworked
  (68..1243 bytes; inlined ModeCheck shape is part of SimFaceTarget's residue).
mcp.c (unchanged)
- 0043bbcc: 10 spellings of the colon trim; only `base+strlen(base); --end` keeps the 289-byte size; the
  `int length=strlen(base)-1` form that is STRICT in f_0043c4bf gives `lea edx,[eax-1]` here (base at esp+0)
  and loses accepted tail users.
- 0043d25f: 10 loop/capture spellings, EDI/EBP swap invariant.
- 0043c5d6, 0043c844, 0043cb47, 0043d2b9, 0043d591, 0043dbb2, 0043e04d, 0043e21a, 0043cab9, 0043e37c: not
  reworked beyond p6's records (shared-tail chain; cab9/e37c/e21a wait for the roots).
noise.c
- 0046c3fc SoundAllowed (and its inlined copies in SoundCache, SoundCacheList, SoundLoad, SoundPlayDefault,
  SoundPlay): new families tried and negative: inline helper with (priority, category) in both orders and
  int/char parameter types, struct copy `SoundPriority p = soundPriority[id]`, switch on category. Best stays D12.
- 0046c89c SoundSetSettings: `SoundRateOf(buffer, id, percent)` computing `soundFrequency[id]*percent/100` inside
  the helper: D32 -> D7 (kept); left: percent is stored to [esp] before the buffer load in the original.
- 0046c461 SoundPlay: SoundAllowed residue plus one extra 4-byte local (0x34 vs 0x38); removing `looping` is not it.
- 0046bb20 SoundCreateBuffer: not reworked.

Candidate files: candidates/o1m/sim/anim.c, candidates/o1m/sim/simdo.c, candidates/o1m/noise/noise.c,
candidates/o1m/core/mcp.c (unchanged). Templates and logs: build/o1m/t/.
