typedef unsigned char u8;
typedef unsigned long u32;

#include "actor.h"
struct ActorPilot { u8 pad[0x12]; int condition; int Active() { return condition > 0; } };

struct LogicActor {
    ActorRecord *state;
    u32 flags;
    u32 reserved;
    u32 modeFlags;
    u8 pad10[0x7c - 0x10];
    int rank, linkedIndex;
    u8 pad84[0x24];
    int Attached() { return state != 0; }
    int Ready() { return state && state->active && state->pilot->Active() && !state->hidden; }
    int Flag2() { return (modeFlags & 2) != 0; }
    int Friendly() { return (modeFlags & 1) == 0; }
};

extern void AssertFail(const char *file, int line, const char *message);
extern const char actorSourcePath[];
#define ASSERT(test) if (!(test)) AssertFail(actorSourcePath, __LINE__, 0)
extern int StreamRead(void *stream, void *buffer, int count);
extern int StreamWrite(void *stream, const void *buffer, int count);
extern void *CreateState(int type);
extern void ReleaseState(void *state);
extern int ResolveName(const char *name, const char *fallback);
extern char fallbackName[];
extern char eventCode;
extern void SendEvent(void *owner, int event, void *result = &eventCode,
                      int a = 0, int b = 0);
extern int StateInvoke(void *action, int a, int b);
extern int StateAction(void *action);
extern int ActorCrouch(ActorRecord *actor, int force);
extern int ActorStand(ActorRecord *actor, int force);
extern int UpdatePayload(void *unit, int rank);
extern int ModeCheck(const char *name);
extern void LogMode(const char *name, void *state, int a, int b);
extern int modeSwitch;
extern char modeCheckName[];
extern char modeLogName[];

int ActorRead(LogicActor *actor, void *stream)
{
    int type;
    actor->flags = 0;
    actor->state = 0;
    if (StreamRead(stream, &type, 4) != 4)
        return 0;
    if (type) {
        actor->state = (ActorRecord *)CreateState(type);
        if (!actor->state)
            return 0;
    }
    if (StreamRead(stream, (char *)actor + 8,
                   (char *)actor + 0xa4 - ((char *)actor + 8)) !=
        (char *)actor + 0xa4 - ((char *)actor + 8))
        return 0;
    return 1;
}

int ActorWrite(LogicActor *actor, void *stream)
{
    int type = actor->state ? *(int *)actor->state : 0;
    if (StreamWrite(stream, &type, 4) != 4)
        return 0;
    if (StreamWrite(stream, (char *)actor + 8,
                    (char *)actor + 0xa4 - ((char *)actor + 8)) !=
        (char *)actor + 0xa4 - ((char *)actor + 8))
        return 0;
    return 1;
}

char *ActorGetEvent(LogicActor *actor)
{
    eventCode = 0;
    if (actor->state && *(void **)((char *)actor->state + 0x1c))
        SendEvent(*(void **)((char *)actor->state + 0x1c), 0x67);
    return &eventCode;
}

void ActorRelease(LogicActor *actor)
{
    if (actor->state)
        ReleaseState(actor->state);
}
int ActorValue(LogicActor *actor)
{
    if (actor->state)
        return *(int *)((char *)actor->state + 0x42);
    return ResolveName(fallbackName, (char *)actor + 0x70);
}
char *ActorName(LogicActor *actor)
{
    if (actor->state)
        return (char *)actor->state + 0x3a;
    return (char *)actor + 0x70;
}
int ActorEnable(LogicActor *actor) {
    ASSERT(actor->state != 0);
    return StateInvoke(actor->state->unit, 0, 0);
}

int ActorStateCall(LogicActor *actor) {


    ASSERT(actor->state != 0);
    return StateAction(actor->state->unit);
}

void ActorSetMode(LogicActor *actor, int value) {


    ASSERT(actor->state != 0);
    int low = 10;
    int lowIsDefault = ((actor->modeFlags & 1) == 0);
    if (!lowIsDefault)
        low = 0;
    actor->state->unit->shieldDirection = (u8)(value >> 24);
    actor->state->unit->shieldRange = low;
    UpdatePayload(actor->state->unit,
                  actor->state->action.angle);
    if (modeSwitch && ModeCheck(modeCheckName)) {
        LogMode(modeLogName, actor->state,
                actor->state->unit->shieldDirection,
                actor->state->unit->shieldRange);
    }
}

int ActorCanChoose(LogicActor *actor)
{
    if (actor->Ready()) {
        if (actor->Flag2()) return 1;
        if (actor->Friendly() && actor->rank < 5) return 1;
    }
    return 0;
}

class LogicActorList { public: LogicActor *At(int); };
extern LogicActorList logicActorList;
extern "C" int LogicCopyPoint(void *, void *);
int ActorIsAvailable(LogicActor *);

extern "C" int ActorInvoke(LogicActor *actor, int mode, int limit, void *point)
{

    int result = 0;
    if (actor->Attached()) {
        switch (mode) {
        case 3:
            for (; actor; actor = logicActorList.At(actor->linkedIndex)) {
                result = actor->Ready();
                if (result) break;
            }
            break;
        case 2:
            for (; actor; actor = logicActorList.At(actor->linkedIndex)) {
                result = ActorCanChoose(actor);
                if (result) break;
            }
            break;
        case 0:
            while (actor) {
                if (actor->rank < 5) {
                    result = ActorIsAvailable(actor);
                    if (!result) break;
                }
                actor = logicActorList.At(actor->linkedIndex);
            }
            break;
        case 1:
            if (point) {
                result = LogicCopyPoint(point, ActorName(actor));
                result = result <= limit;
            }
            break;
        default:













































            AssertFail(actorSourcePath, __LINE__, "Unknown Test");
        }
    }
    return result;
}

int ActorArrayGet(LogicActor *actor, int index)
{
    if (index >= 0 && index < 16)
        return ((int *)((char *)actor + 0x28))[index];
    return 0;
}

void ActorArraySet(LogicActor *actor, int index, int value)
{
    if (index >= 0 && index < 16)
        ((int *)((char *)actor + 0x28))[index] = value;
}

void ActorRandomAction(LogicActor *actor)
{
    if (actor->state)
        ActorCrouch(actor->state, 0);
}

void ActorFinalize(LogicActor *actor)
{
    if (actor->state)
        ActorStand(actor->state, 0);
}

int ActorIsAvailable(LogicActor *actor)
{
    if (actor->state) {
        if (!actor->state->active)
            return 1;
        int hasCount = actor->state->pilot->Active();
        if (hasCount)
            return 0;
        return 1;
    }
    return 0;
}
