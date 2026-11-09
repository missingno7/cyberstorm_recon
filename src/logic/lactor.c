typedef unsigned char u8;
typedef unsigned long u32;

struct LogicActor {
    void *state;
    u32 flags;
    u8 pad[0x9c];
};

struct ActorUnit {
    u8 pad0[0x18];
    void *action;
    u8 pad1[0x26 - 0x1c];
    u8 rank;
};

struct ActorPayload {
    u8 pad[0x1c];
    ActorUnit *unit;
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
extern void ReleaseUnit(void *state, int flags);
extern void StopUnit(void *state, int flags);
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
        actor->state = CreateState(type);
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
    return StateInvoke(((ActorUnit *)actor->state)->action, 0, 0);
}

int ActorStateCall(LogicActor *actor) {


    ASSERT(actor->state != 0);
    return StateAction(((ActorUnit *)actor->state)->action);
}

void ActorSetMode(LogicActor *actor, int value) {


    ASSERT(actor->state != 0);
    int low = 10;
    int lowIsDefault = ((*(u8 *)((char *)actor + 0x0c) & 1) == 0);
    if (!lowIsDefault)
        low = 0;
    ((u8 *)((char *)((ActorUnit *)actor->state)->action + 0xc9))[0] = (u8)(value >> 24);
    ((u8 *)((char *)((ActorUnit *)actor->state)->action + 0xca))[0] = low;
    UpdatePayload(((ActorUnit *)actor->state)->action,
                  ((ActorUnit *)actor->state)->rank);
    if (modeSwitch && ModeCheck(modeCheckName)) {
        LogMode(modeLogName, actor->state,
                (u8)*((char *)((ActorUnit *)actor->state)->action + 0xc9),
                (u8)*((char *)((ActorUnit *)actor->state)->action + 0xca));
    }
}

int ActorCanAct(LogicActor *actor)
{
    LogicActor *actorArg = actor;
    void *state = actorArg->state;
    u8 active;
    if (!state || !*(char *)((char *)state + 0x88) ||
        *(int *)((char *)*(void **)((char *)state + 0x1c) + 0x12) < 1 ||
        *(char *)((char *)state + 0x89))
        active = 0;
    else
        active = 1;
    if (active &&
        ((*(u8 *)((char *)actorArg + 0x0c) & 2) != 0 ||
         (((*(u8 *)((char *)actorArg + 0x0c) & 1) == 0) &&
          *(int *)((char *)actorArg + 0x7c) < 5)))
        return 1;
    return 0;
}

extern LogicActor *ActorFind(const char *table, void *key);
extern int ActorCompare(const char *name, const char *text);
extern const char actorTable[];
extern const char unknownTest[];

int ActorSelect(LogicActor *actor, int mode, int limit, const char *name)
{
    LogicActor *initialActor = actor; u8 result = 0; int stateOnEntry = *(int *)actor; int *textPointer;
    if (stateOnEntry) {
        switch (mode) {
        case 3:
            for (; actor; actor = ActorFind(actorTable, *(void **)((char *)actor + 0x80))) { result = 0;
                void *state = actor->state;
                if (!state || !*(char *)((char *)state + 0x88) ||
                    *(int *)((char *)*(void **)((char *)state + 0x1c) + 0x12) < 1 ||
                    *(char *)((char *)state + 0x89))
                    result = 0;
                else
                    result = 1;
                if (result)
                    return 1;
            }
            break;
        case 2:
            for (; actor; actor = ActorFind(actorTable, *(void **)((char *)actor + 0x80))) { result = 0;
                void *state = actor->state;
                if (!state || !*(char *)((char *)state + 0x88) ||
                    *(int *)((char *)*(void **)((char *)state + 0x1c) + 0x12) < 1 ||
                    *(char *)((char *)state + 0x89))
                    result = 0;
                else
                    result = 1;
                if (result &&
                    ((*(u8 *)((char *)actor + 0x0c) & 2) != 0 ||
                     (((*(u8 *)((char *)actor + 0x0c) & 1) == 0) &&
                      *(int *)((char *)actor + 0x7c) < 5)))
                    return 1;
            }
            break;
        case 0:
            for (; actor; actor = ActorFind(actorTable, *(void **)((char *)actor + 0x80))) {
                if (*(int *)((char *)actor + 0x7c) < 5) {
                    void *state = actor->state;
                    if (!state || (*(char *)((char *)state + 0x88) &&
                        *(int *)((char *)*(void **)((char *)state + 0x1c) + 0x12) > 0))
                        result = 0;
                    else
                        result = 1;
                    if (!result)
                        return result;
                }
            }
            break;
        case 1:
            if (name) {
                if (!stateOnEntry) textPointer = (int *)initialActor + 0x1c;
                else textPointer = (int *)((char *)stateOnEntry + 0x3a);
                stateOnEntry = ActorCompare(name, (const char *)textPointer);
                result = stateOnEntry <= limit;
            }
            break;
        default:














            AssertFail(actorSourcePath, __LINE__, unknownTest);
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

void ActorReleaseUnit(LogicActor *actor)
{
    if (actor->state)
        ReleaseUnit(actor->state, 0);
}

void ActorStopUnit(LogicActor *actor)
{
    if (actor->state)
        StopUnit(actor->state, 0);
}

int ActorIsAvailable(LogicActor *actor)
{
    if (actor->state) {
        if (!*(u8 *)((char *)actor->state + 0x88))
            return 1;
        int hasCount = *(int *)((char *)*(void **)((char *)actor->state + 0x1c) + 0x12) > 0;
        if (hasCount)
            return 0;
        return 1;
    }
    return 0;
}
