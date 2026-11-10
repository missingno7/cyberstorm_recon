typedef unsigned char u8;
typedef unsigned int u32;
struct WorldAction { WorldAction() { pad[0] = 0; } int &Duration() { return timer; } char pad[0x67]; int timer; char rest[4]; }; struct BestLocBuffer { char bytes[0x28]; };

extern int worldActionCount;
extern WorldAction *worldActions;
extern int worldActionFlag;
extern int worldActionBusy;
extern int *actorIndexTable; extern int worldActionTypeTable[];
extern void *globalActorData;
extern void *worldActionData;

extern "C" void AssertFail(const char *, int, const char *);
extern "C" void *memset(void *, int, unsigned int);
extern int BestLocInitialize(void *, const char *, int);
extern int WorldActionCopyName(const void *, void *, void *);
extern "C" int ReadToken(void *, void *);
extern "C" int ReadValue(void *, void *);
extern "C" int ParseTokens(void *, int (*)(void *)); extern void BestLocVisit(void *, int (*)(void *));
extern "C" int WorldActorClose(void *); extern const char fallbackName[];
extern "C" int WorldActorSave(void *); extern int BestLocSetCandidate(void *, void *);
extern "C" int WorldActorApply(void *, int, int, int, int);
extern "C" int WorldActorSet(void *, int); extern unsigned int RandomUpdate(unsigned int *); extern int WorldActionStart(void *, int); int WorldActionCallback(void *);

#define ASSERT(e) if (!(e)) AssertFail(__FILE__, __LINE__, 0)

int WorldActionInitialize()
{





        /* ASSERT_LINE_34 */ ASSERT(worldActions == 0);
    worldActionCount = 0;
    worldActions = new WorldAction[250];
    /* ASSERT_LINE_37 */ ASSERT(worldActions != 0);
    memset(worldActions, 0, 0x6c66);
    return worldActions != 0;
}


extern "C" int StreamRead(void *, void *, unsigned int);
extern "C" int StreamWrite(void *, void *, unsigned int);
extern "C" int WorldActionNotify(void *, int);
extern "C" int WorldActionFinalize(void *);

int WorldActionShutdown()
{
    if (worldActions != 0)
        delete[] worldActions;
    worldActions = 0;
    worldActionCount = 0;
    return 1;
}

int WorldActionAdd(void *actor, int type, int duration)
{
    BestLocBuffer candidate;
    int unit = worldActionTypeTable[type * 15] + 1;
    if (actor != 0) {
        worldActionFlag = 1;
        BestLocInitialize(&candidate, fallbackName, unit);
        BestLocSetCandidate(&candidate, actor);
        BestLocVisit(&candidate, WorldActionCallback);
    }
    if (worldActionFlag == 0)
        return 0;
    if (worldActionCount >= 250)
        return 0;
    int count = worldActionCount;
    WorldAction *entry = worldActions + count;
    register int newCount = count + 1;
    worldActionCount = newCount;
    memset(entry, 0, 0x6f);
    WorldActionCopyName(fallbackName, (char *)entry + 0x12, actor);
    unsigned int value = RandomUpdate((unsigned int *)0x51445c);
    int fractional = (value % 6) * 0x2aaaaabc;
    *(void **)((char *)entry + 0x22) = actor;
    *(int *)((char *)entry + 0x26) = fractional;
    WorldActorApply(entry, 1, *(volatile int *)&type, 0x63, 0);
    WorldActionNotify(entry, 1);
    WorldActionStart(entry, -newCount);
    *(int *)((char *)entry + 0x6b) = 1;
    *(int *)((char *)entry + 0x67) = duration;
    return 1;
}

int WorldActionUpdate(int actor, int elapsed)
{
    int index = actorIndexTable[actor * 2];
    if (index >= 0)
        return 0;
    index = -index;
    WorldAction *entry = worldActions + index;
    int *remaining = &entry->Duration();
    int oldValue = *remaining;
    if (oldValue == -1)
        return 1;
    int newValue = oldValue - elapsed;
    *remaining = newValue;
    if (newValue > 0)
        return 1;
    *remaining = 0;
    if (oldValue != 0) {
        *(int *)(entry->rest) = 1;
        WorldActionNotify(entry, 0x11);
        WorldActionFinalize(entry);
    }
    return 1;
}

int WorldActionRemove(void *stream)
{
    StreamWrite(stream, &worldActionCount, 4);
    if (worldActionCount != 0) {
        StreamWrite(stream, worldActions, worldActionCount * sizeof(WorldAction));
    }
    return 1;
}

int WorldActionClear(void *stream)
{
    register void *reader = stream;
    StreamRead(reader, &worldActionCount, 4);
    register int count = worldActionCount;






































    /* ASSERT_LINE_165 */ ASSERT(count >= 0 && count < 250);
    if (count != 0) {
        register WorldAction *actions = worldActions;
        register unsigned int size = count * sizeof(WorldAction);
        StreamRead(reader, actions, size);
    }
    return 1;
}

int WorldActionCallback(void *entry)
{
    int index = *(int *)((char *)entry + 0x0c);
    if (index != 0) {
        if (index == 0)
            goto invalidCallback;
        int mappedIndex = actorIndexTable[index * 2];
        if (mappedIndex == 0)
            goto invalidCallback;
    }
    worldActionFlag = 0;
    return 1;
invalidCallback:
    return 0;
}

int WorldActionRead(void *actor, int unit)
{
    BestLocBuffer candidate;
    if (actor == 0)
        return 0;
    worldActionFlag = 1;
    BestLocInitialize(&candidate, fallbackName, unit);
    BestLocSetCandidate(&candidate, actor);
    BestLocVisit(&candidate, WorldActionCallback);
    return worldActionFlag;
}

int WorldActionStop(void *actor)
{
    return WorldActorClose(actor);
}
