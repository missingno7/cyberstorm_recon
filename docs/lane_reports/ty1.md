# Lane ty1: one shared header for the actor record cluster (pilot)

Header: `candidates/ty1/include/actor.h` (GridPoint, GuiRect, AnimBlock 0x494, AnimAction 0x67, UnitSlot 8,
ActorUnit 0x10b, forward `ActorPilot`, ActorRecord 0x98). Types and inline accessors only, no prototypes.
Converted sources: `candidates/ty1/<dir>/<file>.c` (41 files). Every conversion script is `build/ty1/c*_*.py`
(edits a copy of src/, keeps the line count; `build/ty1/all.sh` regenerates all and dry-runs them).

## Result in one paragraph
41 files converted, 0 accepted functions lost, 1 function newly STRICT (AnimActionInterpolateDrawPos
004579e0, 272 B). 38 of the 41 objects are byte-identical to the current src objects (checked with an object
diff, `build/ty1/fdiff.py`); the three that change are anim.c (the new STRICT function), pop/pmove.c
(MovementCommand: a wrong private offset corrected, 991 -> 990 B differ) and sim/opfire.c (stack slots of
HotbriefProcessOpposingActions renumbered, 752 -> 751). Six facts about the real types were forced by
accepted functions while building the header (below). The shared type pays as structure and as a check of the
private views (it found a wrong offset), but it does not unlock parked functions: their residues are register
allocation / shape, not field modelling.

## Header field table
Proof column: an original instruction or a STRICT function that compiles byte-identically with that field
(all converted files keep all their STRICT functions, so every field used in a STRICT function is proven).

### ActorRecord (0x98, actorTable entries; `new ActorRecord[400]` in actorlst)
| off | type | name | proof |
|---|---|---|---|
| 00 | long | id | actorlst ActorListRead/Transfer, message.c MessageActorCreated (SetMapping(actor->id)) |
| 04 | long | previous | message.c MessageActorCreated/Update (netId), mstart GetActorState (`->state`), actoradd stores -1 |
| 08 | long | owner | simmulti (a->owner = localPlayer), actorlst Owner(), apply, pweapon (GetModePlayer(owner)) |
| 0c | long | targetId | actor.c ActorAdvanceState (`other->targetId = 0`), minipkt PacketApply/Fire, pweapon TargetInitializeRecord |
| 10 | long | source | actorlst ActorListDestroy/Write (`<= -9999` custom, `0` own, else Mission(source)), message.c (-1) |
| 14 | long | target | simdo SimSetFocus (`target != index`), pmove MovementCommand (original reads +0x14 at 10 sites), preport (mission) |
| 18 | ActorUnit * | unit | every file (StreamRead 0x10b in actorlst) |
| 1c | ActorPilot * | pilot | every file; StreamRead 0x7e in actorlst; completed per file (see below) |
| 20 | AnimAction | action | anim.c (all AnimAction* functions get &actor->action), fxout, apply, sim.c AnimActionReplace(&a->action) |
| 87 | signed char | done | actor.c (`action.resetFlag = 1`), simdo SimFinishFocus (done=1), actoradd (busy=0) |
| 88 | signed char | active | actor.c Active(), simdo SimFocusReady (`active && !hidden`), minipkt (hasPilot=0), lactors Available() |
| 89 | signed char | hidden | actor.c Hidden(), simactor (`hidden == (signed char)0`), pweapon IsVisible(), lactors (inactive) |
| 8a | unsigned char | flag8a | actoradd ActorListAdd (`= 1`) only |
| 8b | unsigned char | flag8b | actoradd ActorListAdd (`= 1`) only |
| 8c | unsigned char | unknown8c | no access seen |
| 8d | signed char | spent | actor.c ActorCrouch/ActorStand (`= 1` after UnitSpend), Crc16(&spent,1), simdo, minipkt (moved) |
| 8e | unsigned char | returnFire | simdo SimSetReturnFire (bit mask), actoradd (`= 0xff`), weapai WeaponEnabled (`& 1 << index`), pweapon |
| 8f | long | relatedIndex | actorlst ActorListTransfer/RelatedIndex() (`mov [reg+0x8f]`) |
| 93 | long | related | actor.c ReleaseState (`mov eax,[ebx+0x93]`) |
| 97 | unsigned char | blink | simmini SimGameDrawActors (blink--), apply ApplyOne (store) |

Accessors (all appear in at least one accepted file): Unit(), Pilot(), Owner(), Index() (= action.resource),
RelatedIndex(), Active(), Hidden(), Ready(), IsVisible(), Spent(), ReturnFire(), UnitText(), PilotText().

### AnimAction (0x67, ActorRecord+0x20, constructor 00457510)
| off | type | name | proof |
|---|---|---|---|
| 00 | long | depth | simactor SimSortDrawQueue, explo ExploPosition (`depth + 10`), anim.c UpdateTrajectory |
| 04 | unsigned char | type | anim.c AnimActionInitialize/Clear (STRICT) |
| 05 | unsigned char | state | anim.c WorldActionNotify, IsMoving() in simdo/simturn/weapai/minipkt/pmove, fxout (`== 12`) |
| 06 | unsigned char | angle | anim.c (GridClamp result), simturn/simdo UpdatePayload(unit, angle), lactor (rank), pweapon (face) |
| 07 | unsigned char | rate | anim.c AnimActionInitialize/Advance (STRICT) |
| 08 | unsigned char | frame | anim.c WorldActionNotify (STRICT) |
| 09 | unsigned char | count | anim.c WorldActionNotify, AnimActionStep (STRICT) |
| 0a | unsigned char | flags | anim.c WorldActionNotify (`&= 0x70; |= 0x80`), AnimActionIsSet |
| 0b | signed char | ready | anim.c WorldActionNotify (`= 1`); signed: simactor SimBuildDrawQueue `int dirty = action->dirty` |
| 0c | unsigned short | kind | anim.c (`kind * 15` metadata), simdo (model > 8 && < 18), simactor drawAnimationTypes[kind] |
| 0e | unsigned long | time | anim.c AnimActionAdvance (STRICT) |
| 12 | GridPoint | position | anim.c AnimActionCopyState/Equal (STRICT), simdo SimSetFocus, actoradd TargetObjectInit |
| 1a | GridPoint | target | anim.c CopyState/Equal, opfire HotbriefCheckAction (original `add eax,0x3a`), pmove Process (`lea edx,[ebx+0x3a]`), actorlst LogicCopyPoint |
| 22 | long | resource | anim.c AnimActionAssign (GridPointToIndex), simdo/simmain/simmini (index), actor.c Index() |
| 26 | long | direction | anim.c ctor (`mov [eax+0x26],edx`), actor.c ActorSetHeading (`* 0x2aaaaabc`), actoradd, pmove/pweapon/message |
| 2a | GridPoint | screen | anim.c AnimActionInterpolateDrawPos (now STRICT), simfire (`screen = other->action.screen`) |
| 32 | AnimBlock * | block | anim.c ctor/CreateBlock/SetExternalBlock, actorlst (`block->actor = entry`), simdo (block->budget/count) |
| 36 | unsigned char | special | anim.c ctor (`mov byte [eax+0x36],0`), pmove (state = 3) |
| 37 | unsigned char | random | actoradd ActorListAdd (`seed % 100`, `mov [ebx+0x57],dl`), simactor (drawFrame + random) |
| 38 | signed char | b38 | anim.c AnimActionConfigure (STRICT) |
| 39 | signed char | b39 | anim.c AnimActionConfigure (STRICT) |
| 3a | signed char | color | actoradd (player->color); signed: simmini SimGameBlink/SimGameDrawActors (movsx index) |
| 3b | GuiRect | rect | anim.c InterpolateDrawPos (`rect.height/4`), simactor SimInvalidateActors (Moved()), simout Width() |
| 4b | int * | bitmap | anim.c UpdateTrajectory/AnimBitmapFrame |
| 4f | GridPoint | from | anim.c CommitTrajectory (DIFF) and ActorDrawEffect; AnimActionPrepareBlock |
| 57 | GridPoint | to | as from |
| 5f | long | step | anim.c (step * dx / steps) |
| 63 | long | steps | anim.c |

Accessors: SetState(), Width() (simout), SetHeading() (actor.c), IsMoving() (simdo/simfire/simturn/minipkt;
minipkt's `Busy()` spelling compiles identically and was dropped).

### AnimBlock (0x494): path[128] +000, cost[128] +200, marks[0x80] +400, budget +480 (simdo `selectedUnit->cost`),
count +484 (simdo `->kind == 1`), index +488, unknown48c, actor +490 (actorlst `*(void **)(block + 0x490) = entry`,
anim.c SetExternalBlock). Size from AnimActionCreateBlock / actorlst StreamRead 0x494.

### UnitSlot (8) and ActorUnit (0x10b, StreamRead size in actorlst)
| off | type | name | proof |
|---|---|---|---|
| +0 | unsigned char | type | tu_00438374 (HercSlot.type) |
| +1 | unsigned char | item | tu_00438374 PopFindItem (`slots[slot].value`), weapai WeaponStatus (`!kind` -> 1), mcp, actor.c (slot 0 item 31) |
| +2 | unsigned char | condition | weapai (`!health` -> 3), apply ApplyOne, hbmanage StorageColors (`data[0x13+i*8]`), mcp |
| +3 | unsigned char | uses | weapai (`charge >= threshold`), opfire (`uses < maxUses`), simdo SimExecuteItem (cooldown) |
| +4 | int | amount | tu_0043a294 HercAdvanceTurn (`int *power = &slots[5].amount`: must be int, not long), weapai |
| ActorUnit 000 | char[17] | name | UnitText(), sprintf of the unit in simdo/tu_004413e0 |
| 011 | UnitSlot[23] | slots | all herc TUs; slot 0 item = chassis type (actor.c `== 31`, simmisc `== 0x1f`) |
| 0c9 | unsigned char | shieldDirection | pshield SetDirection(), lactor (`+0xc9`), minipkt (front) |
| 0ca | unsigned char | shieldRange | pshield, lactor (`+0xca`), minipkt (back) |
| 0cb | int[6] | armor | tu_00438374 (armor), pshield (arcs) |
| 0e3 | int | fieldE3 | tu_00438374 |
| 0e7 | int | mass | tu_00438374 HercCalculateMass |
| 0eb | unsigned char[32] | damage | tu_0043a294 HercBuildDamage |

Accessors: UnitSlot::Condition() (SimDamageActor), ActorUnit::SetDirection() (pshield),
ActorUnit::GetStatus() (ppilot, unsigned short return of slots[10].condition).

### GridPoint / GuiRect
GridPoint {long x, y; (), (x,y), copy ctor, Copy, Equal, operator!=}: replaces anim.c AnimPair, simgad ExploPoint,
message.c ActorPosition, simdo/sim.c/simturn GridPoint, actoradd ActorPoint. GuiRect {long x,y,width,height;
GuiRect() {}; Moved()}: replaces every converted file's private GuiRect (also as base of GuiControlData /
GuiScreen / GuiControl in sim.c, simglob, simmain, simgad, pop/*): no byte changes anywhere.

## Facts about the real types (forced by accepted functions; recorded with notes.py)
- AnimAction +0x2a is a plain point member, not anim.c's `union { struct {long x,y;}; ExploPoint point; }`:
  with the member AnimActionInterpolateDrawPos 004579e0 becomes STRICT (204 B differ before). The point's copy
  constructor is irrelevant there (same bytes without it).
- AnimAction +0x0b is signed char (simactor SimBuildDrawQueue 641 -> 833 B differ with unsigned).
- AnimAction +0x3a is signed char (simmini SimGameBlink 00451b10 and SimGameDrawActors 004521f0 lose STRICT with unsigned).
- GuiRect has an empty user default constructor (simactor SimInvalidateActors 00454ad0 loses STRICT without it).
- The unit-slot record has an inline accessor returning its +2 byte: SimDamageActor 00453cb0 needs
  `unit->slots[10].Condition()` (original `lea eax,[unit+0x61]; mov al,[eax+2]; and eax,0ffh`); a direct byte read loses STRICT.
  ppilot's DrawDetails 004475de instead needs an ActorUnit accessor with an unsigned short return (GetStatus): 13 B
  stays 13, direct byte 32, Condition() 323. Both accessors are in the header.
- UnitSlot.amount is int (int* taken in tu_0043a294; long* does not convert).
- actor.c ActorCrouch/ActorStand: the cost helper must take the unit (`UnitCrouchCost(actor->unit)`); the file's
  existing `ActorCrouchCost(actor)` loses both functions.
- pmove MovementPopup::Process 00442800: `p += actor->action.target` must go through GuiPoint::operator+=(const
  GuiPoint &) (reference operand: `lea edx,[ebx+0x3a]`); spelled as two adds it loses STRICT. Kept as
  `p+=(GuiPoint &)actor->action.target;` - evidence that in the original the action's points and GuiPoint are
  one class (or related), which the GridPoint/GuiPoint split here does not model yet.
- pmove's private record had `start` at +0x10; the original reads +0x14 (the header's `target`) - the private
  view was wrong; MovementCommand 991 -> 990.
- opfire's `position` is +0x3a (action.target), not +0x32.

## Newly STRICT / changed class
- NEW STRICT: AnimActionInterpolateDrawPos 004579e0 (sim\anim.c, 272 B; was SAME-SIZE 204 B differ).
- Closer: MovementCommand 00442af4 991 -> 990 (offset fix; SHAPE), HotbriefProcessOpposingActions 00462920
  752 -> 751 (stack slots; SAME-SIZE). No class changes otherwise; no function got worse in the final state.
- `cmp.py` shows a few count changes in simdo/simfire/simmain/apply (e.g. SimSetFocus 905 -> 901, SimBeginTurn
  1295 -> 1303): the objects are byte-identical, only relocation names bind differently (new hypothesis bindings
  such as GridSetSelection, simTargetState).
- Tried and recorded, no gain: ExploPosition 00460340 typed (identical), with `actor->Unit()` (59 B, worse);
  IntelClearMode, WeaponStatus, SimApplyLeadership, ActorListAdd, f_004418a9 residues are data/register, not fields.

## Where the shared type did not fit, and why
- sim/animout.c: not converted. Replacing its DrawPoint by GridPoint gives byte-identical code, but promote
  refuses the object: the static `GridPoint steps[6]` typesig brings `GridPoint::$ct/$dt` COMDATs whose names are
  already bound elsewhere (GridPoint::$dt 0042c9d0; 004599a0 "already matched as UnknownIdentityThunk").
  Class names that appear in typesigs are binding-relevant; the integrator has to decide the one point class name.
- Pilot record: left out of the header (`struct ActorPilot;`). Each file names it differently and those names are
  bound by label (ActorState::Equal, ActorListObject::$ct/Fixup, ActorListGlobalObject::$dt, ActorLogic, Pilot,
  UnitInfo). Converted files complete it as `struct ActorPilot : <their class> {}` (offset-0 upcast, no code).
- Unit accessors that were members in actor.c / simdo / opfire (UnitState::CrouchCost/StandCost/Item) became file
  inline free functions taking the unit (a header with them would need PopFindItem's prototype). promote prints
  REVIEW twin-helper lines for actor.c (UnitCrouchCost/UnitStandCost vs ActorCrouchCost/ActorStandCost); they were
  the same twins as members before, now visible to the check.
- pmove needs the `(GuiPoint &)` cast above; explo.c keeps its ExploActor view (ExploPoint with out-of-line ctor
  is explo's own point type and is copied into ExploRecord).
- Mangled names change with the type names (UnitState/Payload/ActorBlock -> ActorUnit, ExploPoint/ActorPosition ->
  GridPoint): promote reports "new hypothesis binding ... ALIAS of [...]: use that name" for GridPointToIndex,
  StateInvoke, StateInvokeEnergy, f_00439363 (HercEqual), f_00439438 (HercCopy). Dry runs pass; labels unchanged.

## Judgement
Shared types pay as structure and as a cross-check, not (so far) as STRICT yield: 41 files, 0 losses, 38
byte-identical objects, raw offsets and casts gone in lactor/lactors/actorlst/fxout/apply/hbmanage/sim.c, one
wrong private offset found (pmove), six type facts pinned by accepted functions, one new STRICT function. The
anim.c effect (nine functions at once) came from replacing byte offsets inside the parked functions themselves;
in the other files the parked functions were already typed privately and their residues are allocation/shape.
For integration: put the header in src/include (e.g. actor.h), promote the 41 files with CSTORM_INCLUDE as
listed, and accept the ALIAS/~mangle notices. Next cluster: the pilot record (0x7e; 6+ names bound by label -
needs a naming decision first), then the GUI base (GuiRect/GuiPoint/GuiControl/GuiScreen, ~38 files; the
GuiPoint::operator+= evidence from pmove ties it to the map point), then SimGame (19 files, 14 shapes).

## Dry runs (one per converted file; run from the repository root)
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/anim.c --tu src/sim/anim.c --as 'sim\anim.c' --profile ot-oe
      -> src/sim/anim.c: 24 strict functions (2417 bytes), new 1, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/actor.c --tu src/sim/actor.c --as 'sim\actor.c' --profile ot-oe
      -> src/sim/actor.c: 13 strict functions (1906 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/simactor.c --tu src/sim/simactor.c --as 'sim\simactor.c' --profile ot-oe
      -> src/sim/simactor.c: 6 strict functions (1296 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/simdo.c --tu src/sim/simdo.c --as 'sim\simdo.c' --profile ot-oe
      -> src/sim/simdo.c: 28 strict functions (5090 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/simturn.c --tu src/sim/simturn.c --as 'sim\simturn.c' --profile ot-oe
      -> src/sim/simturn.c: 12 strict functions (2148 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/simmain.c --tu src/sim/simmain.c --as 'sim\simmain.c' --profile ot
      -> src/sim/simmain.c: 3 strict functions (1030 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/simglob.c --tu src/sim/simglob.c --as 'sim\simglob.c' --profile ot
      -> src/sim/simglob.c: 5 strict functions (563 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/simgad.c --tu src/sim/simgad.c --as 'sim\simgad.c' --profile ot-oe
      -> src/sim/simgad.c: 20 strict functions (2480 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/simfire.c --tu src/sim/simfire.c --as 'sim\simfire.c' --profile ot-oe
      -> src/sim/simfire.c: 1 strict functions (48 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/simmisc.c --tu src/sim/simmisc.c --as 'sim\simmisc.c' --profile ot-oe
      -> src/sim/simmisc.c: 8 strict functions (1296 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/actorlst.c --tu src/sim/actorlst.c --as 'sim\actorlst.c' --profile ot-oe
      -> src/sim/actorlst.c: 13 strict functions (1380 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/actoradd.c --tu src/sim/actoradd.c --as 'sim\actoradd.c' --profile ot
      -> src/sim/actoradd.c: 3 strict functions (224 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/simout.c --tu src/sim/simout.c --as 'sim\simout.c' --flags '-4s -s -bm -oxt -oe=80'
      -> src/sim/simout.c: 3 strict functions (864 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/simmini.c --tu src/sim/simmini.c --as 'sim\simmini.c' --flags '-4s -s -bm -oxt -oe=80'
      -> src/sim/simmini.c: 4 strict functions (697 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/simgame.c --tu src/sim/simgame.c --as 'sim\simgame.c' --flags '-4s -s -bm -oxt -oe=80'
      -> src/sim/simgame.c: 6 strict functions (1537 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/weapai.c --tu src/sim/weapai.c --as 'sim\weapai.c' --profile ot-oe
      -> src/sim/weapai.c: 11 strict functions (2047 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/opfire.c --tu src/sim/opfire.c --as 'sim\opfire.c' --flags '-4s -s -bm -oxt -oe=200'
      -> src/sim/opfire.c: 2 strict functions (273 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/apply.c --tu src/sim/apply.c --as 'sim\apply.c' --profile ot-oe
      -> src/sim/apply.c: 3 strict functions (922 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/fxout.c --tu src/sim/fxout.c --as 'sim\fxout.c' --profile ot-oe
      -> src/sim/fxout.c: 8 strict functions (2069 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/sim.c --tu src/sim/sim.c --as 'sim\sim.c' --profile ot-oe
      -> src/sim/sim.c: 11 strict functions (5373 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/simmulti.c --tu src/sim/simmulti.c --as 'sim\simmulti.c' --profile ot-oe
      -> src/sim/simmulti.c: 11 strict functions (942 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/simalert.c --tu src/sim/simalert.c --as 'sim\simalert.c' --profile ot-oe
      -> src/sim/simalert.c: 10 strict functions (1488 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/pop/ppilot.c --tu src/pop/ppilot.c --as 'pop\ppilot.c' --profile plain-oe
      -> src/pop/ppilot.c: 16 strict functions (3976 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/pop/pshield.c --tu src/pop/pshield.c --as 'pop\pshield.c' --profile plain-oe
      -> src/pop/pshield.c: 16 strict functions (1556 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/pop/pweapon.c --tu src/pop/pweapon.c --as 'pop\pweapon.c' --profile plain-oe
      -> src/pop/pweapon.c: 20 strict functions (1964 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/pop/preport.c --tu src/pop/preport.c --as 'pop\preport.c' --profile plain-oe
      -> src/pop/preport.c: 12 strict functions (1380 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/shell/hbmanage.c --tu src/shell/hbmanage.c --as 'shell\hbmanage.c' --profile plain-oe
      -> src/shell/hbmanage.c: 23 strict functions (2894 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/pop/pmove.c --tu src/pop/pmove.c --as 'pop\pmove.c' --profile plain-oe
      -> src/pop/pmove.c: 9 strict functions (1218 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/multi/message.c --tu src/multi/message.c --as 'multi\message.c' --flags '-4s -s -bm -oe'
      -> src/multi/message.c: 30 strict functions (3358 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/multi/minipkt.c --tu src/multi/minipkt.c --as 'multi\minipkt.c' --profile plain-oe
      -> src/multi/minipkt.c: 34 strict functions (8312 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/multi/mstart.c --tu src/multi/mstart.c --as 'multi\mstart.c' --profile plain
      -> src/multi/mstart.c: 39 strict functions (4871 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/logic/lactors.c --tu src/logic/lactors.c --as 'logic\lactors.c' --profile plain-oe
      -> src/logic/lactors.c: 21 strict functions (3332 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/logic/lactor.c --tu src/logic/lactor.c --as 'logic\lactor.c' --profile plain
      -> src/logic/lactor.c: 12 strict functions (692 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/_unknown/tu_00438374.c --tu src/_unknown/tu_00438374.c --as '_unknown\tu_00438374.c' --profile plain-oe
      -> src/_unknown/tu_00438374.c: 24 strict functions (2001 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/_unknown/tu_004394e0.c --tu src/_unknown/tu_004394e0.c --as '_unknown\tu_004394e0.c' --profile plain-oe
      -> src/_unknown/tu_004394e0.c: 19 strict functions (3254 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/_unknown/tu_0043a294.c --tu src/_unknown/tu_0043a294.c --as '_unknown\tu_0043a294.c' --profile plain-oe
      -> src/_unknown/tu_0043a294.c: 15 strict functions (1834 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/core/mcp.c --tu src/core/mcp.c --as 'core\mcp.c' --profile plain-oe
      -> src/core/mcp.c: 67 strict functions (7630 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/_unknown/tu_0043b7e0.c --tu src/_unknown/tu_0043b7e0.c --as '_unknown\tu_0043b7e0.c' --profile plain
      -> src/_unknown/tu_0043b7e0.c: 1 strict functions (264 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/_unknown/tu_0043ecf8.c --tu src/_unknown/tu_0043ecf8.c --as '_unknown\tu_0043ecf8.c' --profile plain
      -> src/_unknown/tu_0043ecf8.c: 4 strict functions (569 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/_unknown/tu_004413e0.c --tu src/_unknown/tu_004413e0.c --as '_unknown\tu_004413e0.c' --profile plain-oe
      -> src/_unknown/tu_004413e0.c: 11 strict functions (1942 bytes), new 0, lost 0
    CSTORM_INCLUDE=candidates/ty1/include python tools/promote.py candidates/ty1/sim/explo.c --tu src/sim/explo.c --as 'sim\explo.c' --profile ot-oe
      -> src/sim/explo.c: 23 strict functions (3243 bytes), new 0, lost 0
