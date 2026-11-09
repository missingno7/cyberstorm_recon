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
extern void LogMode(void *actor, const char *name, int a, int b);
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
    ActorUnit *state = (ActorUnit *)actor->state;
    u8 low = (*(u8 *)((char *)actor + 0x0c) & 1) ? 0 : 10;
    char *display = (char *)state->action;
    display[0xc9] = (u8)(value >> 24);
    display[0xca] = low;
    UpdatePayload(state->action, state->rank);
    if (modeSwitch && ModeCheck(modeCheckName))
        LogMode(actor, modeLogName, (u8)display[0xc9], (u8)display[0xca]);
}

int ActorCanAct(LogicActor *actor)
{
    void *state = actor->state;
    if (state && *(u8 *)((char *)state + 0x88) &&
        *(int *)((char *)*(void **)((char *)state + 0x1c) + 0x12) > 0 &&
        !*(u8 *)((char *)state + 0x89)) {
        if (*(u8 *)((char *)actor + 0x0c) & 2)
            return 1;
        if ((*(u8 *)((char *)actor + 0x0c) & 1) == 0 &&
            *(int *)((char *)actor + 0x7c) >= 5)
            return 1;
    }
    return 0;
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
    void *state = actor->state;
    if (state) {
        if (!*(u8 *)((char *)state + 0x88))
            return 1;
        int hasCount = *(int *)((char *)*(void **)((char *)state + 0x1c) + 0x12) > 0;
        if (hasCount)
            return 0;
        return 1;
    }
    return 0;
}
