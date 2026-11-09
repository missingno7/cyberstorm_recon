/* 004556f0..004561d0. Working names. Packed offsets come from the code;
 * unknown ranges are temporary structure padding, not data definitions. */
#include <string.h>
#pragma pack(push,1)
struct GridPoint { int x,y; GridPoint() {} GridPoint(const GridPoint &p) { x=p.x; y=p.y; } };
struct GuiRect { int x,y,width,height; };
struct AnimAction {
    int id;
    unsigned char kind,state,variant,rate,frame,frames,flags;
    signed char dirty;
    unsigned short type;
    unsigned long deadline;
    GridPoint position;
    char unknown1a[8];
    int target;
    int heading;
    char unknown2a[0x3b-0x2a];
    GuiRect rect;
    char unknown4b[0x67-0x4b];
    signed char resetFlag;
    void SetHeading(int n) { heading = n * 0x2aaaaabc; }
    int IsMoving() {
        return state != 1 && state != 7 && state != 14 && state != 17 && state != 18;
    }
};
struct UnitState { int CrouchCost(); int StandCost(); unsigned char unknown00[0x12]; unsigned char type; unsigned char unknown13[0x10b-0x13]; };
struct ActorState { int Equal(const ActorState *); }; /* Opaque state; used only through pointers. */
struct ActorRecord {
    int id;
    unsigned char kind,subtype,variant,unknown07;
    int owner,targetId,unknown10,targetName;
    UnitState *unit;
    ActorState *world;
    AnimAction action;
    signed char active,hidden;
    char unknown8a[3];
    signed char stance;
    unsigned char color;
    char unknown8f[4];
    int related;
    UnitState *Unit() { return unit; }
    int Owner() { return owner; }
    int Index() { return action.target; }
    unsigned char blink;
    int Active() { return active; }
    int Hidden() { return hidden; }
    int Stance() { return stance; }
    int Color() { return color; }
};
struct ActorWireState { unsigned char type; signed char active,hidden,stance; unsigned char color;
    int Active() { return active; }
    int Hidden() { return hidden; }
    int Stance() { return stance; }
    int Color() { return color; }
};
struct SimGame;
struct TerrainGrid;
struct SimSound { int values[4]; };
struct ExploPoint;
struct UnitState;
struct ActorItem { char unknown00[0x285]; int crouchCost,standCost; };
struct FogObject;
struct FogBuffer;
struct PopNode;
struct PacketList { void Crouch(ActorRecord *); void Stand(ActorRecord *); void DeadActor(ActorRecord *); };
#pragma pack(pop)
extern int gRedrawFlags,modeSwitch,actorCount,gLocalActorKind;
extern ActorRecord *actorTable,*g4d2498;
extern char modeCheckName[],simPanel,simActions;
extern PopNode g4d1dec;
extern PacketList modeLogName;
extern unsigned char *simActorVisibility;
extern unsigned char bestLocEntries[];
extern int bestLocCursor,bestLocCount;
extern TerrainGrid fallbackName;
extern FogBuffer gFogBufferSource;
extern int actorFollowSelection,actorFogPending;
extern SimSound defaultActorSound;
int ModeCheck(const char *);
void AnimActionMarkReady(void *);
extern "C" void WorldActionNotify(void *,unsigned char);
extern "C" void WorldActionFinalize(void *);
void AnimActionFreeBlock(void *);
int AnimActionHasMetadata(void *);
void AnimActionSetExternalBlock(void *,int,int,unsigned long);
void AnimActionInitTargetActor(void *);
void AnimActionChecksum(void *,unsigned short *);
void AnimActionCopyState(void *,void *);
int AnimActionEqual(void *,void *);
void AnimActionCopyChangedFields(void *,void *);
ActorRecord *ActorFindRelatedOther(ActorRecord *);
ActorRecord *ActorFindByOwner(int);
ActorRecord *ActorFindOwnerNext(ActorRecord *,int);
void SimSetFocus(SimGame *,ActorRecord *);
void Crc16(unsigned short *,void *,int);
int UnitSpend(UnitState *,int);
ActorItem *PopFindItem(UnitState *,int,int);
int f_00439363(UnitState *,void *);

void f_00439438(UnitState *,void *);
void ActorCopyState(ActorState *,ActorState *);
void ModeEvent(SimGame *,ActorRecord *,int,int);
void ActorDrawEffect(void *);
int BestLocSetup(void *,int,int);
int GridClearSelection(TerrainGrid *);
void SimRefreshFog(SimGame *,int);
void SimCenterOn(SimGame *,GridPoint *);
int SimCenterTarget(SimGame *,int);
void f_00443d08(void *);
void f_004429f8(void *);
void PopNodeChanged(PopNode *);
FogObject *UnitFogObject(UnitState *,int,void *);
int FogCastAlternate(FogBuffer *,long,FogObject *);
int SoundTrigger(unsigned,int);
int f_00432d60(TerrainGrid *,int);
void f_00432db0(TerrainGrid *,int);
int f_0043a97f(UnitState *,int);
int f_0043a90e(UnitState *,int);
void SimSoundSetup(SimSound *,ExploPoint *);
extern "C" int LogicCallMode(int,void *);
inline int ActorHiddenFromView(int name) { return !simActorVisibility[name]; }
inline int ClampActorValue(int v,int low,int high) {
    if (v <= low) return low;
    if (v >= high) return high;
    return v;
}

inline int LocalActorKind() { return gLocalActorKind; }
void ActorAdvanceState(ActorRecord *actor)
{
    if (actor->action.IsMoving()) gRedrawFlags |= 4;
    if (actor->owner != gLocalActorKind) {
        do { ActorDrawEffect(&actor->action); }
        while (actor->action.IsMoving() && ActorHiddenFromView(actor->action.target));
    } else ActorDrawEffect(&actor->action);
    if (gLocalActorKind == actor->Owner() && actor == g4d2498 && bestLocCount != 0) {
        if (!g4d2498->action.IsMoving()) {
            bestLocCursor = 0;
            BestLocSetup(bestLocEntries,actor->action.target,0);
            bestLocCount = 0;
            f_00443d08(&simActions);
            if (actor->targetName == actor->action.target && GridClearSelection(&fallbackName))
                gRedrawFlags |= 1;
            SimRefreshFog((SimGame *)modeCheckName,0);
            if (actorFollowSelection) {
                GridPoint position = actor->action.position;
                SimCenterOn((SimGame *)modeCheckName,&position);
                gRedrawFlags |= 1;
            }
        }
    }
}

#define CLAMP_ACTOR_VALUE(v,l,h) ((v)<=(l)?(l):((v)>=(h)?(h):(v)))
inline int ClampActorOrdered(int low,int value,int high) { if(value<=low)return low; if(value>=high)return high; return value; }
void ActorFinishMove(ActorRecord *actor,int force)
{
    char fogScratch[40];
    if (gLocalActorKind != actor->Owner() && actor == g4d2498)
        SimCenterTarget((SimGame *)modeCheckName,0);
    if (gLocalActorKind == actor->Owner()) {
        if (actor == g4d2498) {
            SimCenterTarget((SimGame *)modeCheckName,0);
            f_004429f8(&simPanel);
            f_00443d08(&simActions);
            PopNodeChanged(&g4d1dec);
        }
        if (force || ActorHiddenFromView(actor->action.target)) SimRefreshFog((SimGame *)modeCheckName,0);
        else if (actorFogPending) {
            if (FogCastAlternate(&gFogBufferSource,actor->action.target,UnitFogObject(actor->unit,8,fogScratch)) & 4) {
                SimRefreshFog((SimGame *)modeCheckName,0);
                *((char *)&actor->action+0x36) = 3;
                SoundTrigger(419,0);
                actorFogPending = force;
            }
        }
    }
    int location = f_00432d60(&fallbackName,actor->action.target);
    if (location && f_0043a97f(actor->unit,0)) {
        if (UnitSpend(actor->unit,f_0043a90e(actor->unit,location))) {
            actor->stance = 1;
            f_0043a97f(actor->unit,location);
            f_00432db0(&fallbackName,actor->action.target);
            SimSound sound = defaultActorSound;
            SimSoundSetup(&sound,(ExploPoint *)&actor->action.position);
            sound.values[1] = CLAMP_ACTOR_VALUE(sound.values[1],50,150);
            sound.values[2] = CLAMP_ACTOR_VALUE(sound.values[2],-100,100);
            sound.values[3] = CLAMP_ACTOR_VALUE(sound.values[1],20,100);
            LogicCallMode(29,&sound);
            gRedrawFlags |= 3;
            PopNodeChanged(&g4d1dec);
        }
    }
}

void ReleaseState(ActorRecord *actor)
{
    if (modeSwitch && ModeCheck(modeCheckName)) modeLogName.DeadActor(actor);
    actor->active = 0;
    actor->action.resetFlag = 1;
    for (int offset = 0; offset != 0xb8; offset += 8)
        *((char *)actor->unit+0x13+offset) = 0;
    *(int *)((char *)actor->unit+0x3d) = 0;
    *(int *)((char *)actor->unit+0x45) = 0;
    *(int *)((char *)actor->unit+0x4d) = 0;
    memset((char *)actor->unit+0xcb,0,0x18);
    AnimActionMarkReady(&actor->action);
    WorldActionNotify(&actor->action,17);
    WorldActionFinalize(&actor->action);
    ModeEvent((SimGame *)modeCheckName,actor,1000000,0);
    gRedrawFlags |= 4;
    for (int i=0; i<actorCount; ++i) {
        ActorRecord *other = &actorTable[i];
        if (other != actor && other->active && other->related == actor->id) ReleaseState(other);
        if (other->targetId == actor->id) other->targetId = 0;
    }
}

void SimHotbriefClearActor(ActorRecord *actor)
{
    AnimActionMarkReady(&actor->action);
    WorldActionNotify(&actor->action,1);
    WorldActionFinalize(&actor->action);
    AnimActionFreeBlock(&actor->action);
    actor->hidden = 1;
    gRedrawFlags |= 4;
}

int ActorClearAndSelect(ActorRecord *actor)
{
    int none = 0;
    ActorRecord *selected = ActorFindRelatedOther(actor);
    if (!selected || selected == actor) {
        ActorRecord *replacement = 0;
        for (ActorRecord *p = ActorFindByOwner(actor->owner); p; p = ActorFindOwnerNext(p,actor->owner))
            if (AnimActionHasMetadata(&p->action) && p != actor) replacement = p;
        if (replacement) selected = replacement;
        else none = 1;
    }
    if (selected) SimSetFocus((SimGame *)modeCheckName,selected);
    SimHotbriefClearActor(actor);
    return none;
}

int SimHotbriefCheckActor(ActorRecord *actor) { return AnimActionHasMetadata(&actor->action); }

int ActorIsSpecial(ActorRecord *actor)
{
    if (actor->unit && actor->unit->type == 31) return 1;
    return 0;
}

inline int UnitState::CrouchCost() { return PopFindItem(this,0,-1)->crouchCost; }
inline int UnitState::StandCost() { return PopFindItem(this,0,-1)->standCost; }
inline int ActorCrouchCost(ActorRecord *a) { return PopFindItem(a->unit,0,-1)->crouchCost; }
inline int ActorStandCost(ActorRecord *a) { return PopFindItem(a->unit,0,-1)->standCost; }
int ActorCrouch(ActorRecord *actor,int force)
{
    if (AnimActionHasMetadata(&actor->action)) {
        if (!force) {
            if (actor->action.IsMoving()) return 0;
            if (actor->active == 0) return 0;
            if (actor->action.state == 7) return 0;
        }
        if (UnitSpend(actor->unit,actor->unit->CrouchCost()) || force) {
            actor->stance = 1;
            WorldActionNotify(&actor->action,6);
            if (modeSwitch && ModeCheck(modeCheckName)) modeLogName.Crouch(actor);
            return 1;
        }
    }
    return 0;
}

int ActorStand(ActorRecord *actor,int force)
{
    if (AnimActionHasMetadata(&actor->action)) {
        if (!force) {
            if (actor->action.IsMoving()) return 0;
            if (actor->active == 0) return 0;
            if (actor->action.state == 1) return 0;
        }
        if (UnitSpend(actor->unit,actor->unit->StandCost()) || force) {
            actor->stance = 1;
            WorldActionNotify(&actor->action,8);
            if (modeSwitch && ModeCheck(modeCheckName)) modeLogName.Stand(actor);
            return 1;
        }
    }
    return 0;
}

void ActorSetExternal(ActorRecord *actor,int target,unsigned long flags)
{
    AnimActionSetExternalBlock(&actor->action,(int)actor,target,flags);
    AnimActionInitTargetActor(&actor->action);
}

inline int ScaleActorHeading(int n) { return n*0x2aaaaabc; }
void ActorSetHeading(ActorRecord *actor,int heading)
{
    actor->action.heading = ScaleActorHeading(heading);
    AnimActionMarkReady(&actor->action);
}

int ActorChecksum(ActorRecord *actor)
{
    unsigned short crc[2];
    if (actor->action.IsMoving()) return 0xbe1a;
    crc[0] = 0;
    Crc16(crc,&actor->active,1);
    Crc16(crc,&actor->hidden,1);
    Crc16(crc,&actor->stance,1);
    Crc16(crc,&actor->color,1);
    if (actor->active) AnimActionChecksum(&actor->action,crc);
    return crc[0];
}

void ActorSaveState(ActorRecord *actor,ActorWireState *state)
{
    state->active = actor->active;
    state->hidden = actor->hidden;
    state->stance = actor->stance;
    state->color = actor->color;
    AnimActionCopyState(&actor->action,state);
}

int ActorLoad(ActorRecord *actor,void *unit,ActorState *world,ActorWireState *state)
{
    int equal = 1;
    if (state->Active() != actor->Active()) equal = 0;
    if (state->Hidden() != actor->Hidden()) equal = 0;
    if (state->Stance() != actor->Stance()) equal = 0;
    if (state->Color() != actor->Color()) equal = 0;
    if (!AnimActionEqual(&actor->action,state)) equal = 0;
    if (!f_00439363(actor->unit,unit)) equal = 0;
    if (!actor->world->Equal(world)) equal = 0;
    return equal;
}

void ActorUpdate(ActorRecord *actor,void *unit,ActorState *world,ActorWireState *state)
{
    if (state->active != actor->active) actor->active = state->active;
    if (state->hidden != actor->hidden) actor->hidden = state->hidden;
    if (state->stance != actor->stance) actor->stance = state->stance;
    if (state->color != actor->color) actor->color = state->color;
    AnimActionCopyChangedFields(&actor->action,state);
    f_00439438(actor->unit,unit);
    ActorCopyState(actor->world,world);
}

