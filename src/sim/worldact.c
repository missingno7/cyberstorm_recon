typedef unsigned char u8;
typedef unsigned int u32;
struct WorldAction { WorldAction() { pad[0] = 0; } int &Duration() { return timer; } char pad[0x12]; char position[0x10]; void *actor; int angle; char unknown2a[0x67-0x2a]; int timer; int active; }; struct BestLocBuffer { char bytes[0x28]; };

extern int worldActionCount;
extern WorldAction *worldActions;
extern int worldActionFlag;
extern int worldActionBusy;
extern int *actorIndexTable; struct AnimMeta { int radius; char rest[56]; }; extern AnimMeta animationMetaAt0cbc[];
extern void *globalActorData;
extern void *worldActionData;

extern "C" void AssertFail(const char *, int, const char *);
extern "C" void *memset(void *, int, unsigned int);
extern int BestLocInitialize(void *, const char *, int);
extern "C" void TargetObjectInit(const char *, char *, void *);
extern "C" int ReadToken(void *, void *);
extern "C" int ReadValue(void *, void *);
extern "C" int ParseTokens(void *, int (*)(void *)); extern void BestLocVisit(void *, int (*)(void *));
extern "C" int WorldActorClose(void *); extern const char fallbackName[];
extern "C" int WorldActorSave(void *); extern int BestLocSetCandidate(void *, void *);
extern "C" int WorldActorApply(void *, int, int, int, int);
extern "C" int WorldActorSet(void *, int); extern "C" unsigned long next_link_seed(unsigned long *); extern unsigned long MissionRandomTable[]; extern int WorldActionStart(void *, int); int WorldActionCallback(void *);
inline int WorldActionPlace(void *actor, int unit) { if (!actor) return 0; BestLocBuffer candidate; worldActionFlag = 1; BestLocInitialize(&candidate, fallbackName, unit); BestLocSetCandidate(&candidate, actor); BestLocVisit(&candidate, WorldActionCallback); return worldActionFlag; }
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
    if (!WorldActionPlace(actor, animationMetaAt0cbc[type].radius + 1))
        return 0;
    if (worldActionCount >= 250)
        return 0;
    int count = worldActionCount;
    WorldAction *entry = &worldActions[count++]; worldActionCount = count;
    memset(entry, 0, sizeof(WorldAction));
    TargetObjectInit(fallbackName, entry->position, actor);
    int angle = next_link_seed(MissionRandomTable) % 6 * 0x2aaaaabc;
    entry->actor = actor;
    entry->angle = angle;
    WorldActorApply(entry, 1, type, 0x63, 0);
    WorldActionNotify(entry, 1);
    WorldActionStart(entry, -count);
    entry->active = 1;
    entry->timer = duration;
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
        entry->active = 1;
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
    
    StreamRead(stream, &worldActionCount, 4);
    





































    /* ASSERT_LINE_165 */ ASSERT(worldActionCount >= 0 && worldActionCount < 250);
    if (worldActionCount != 0) {
        
        
        StreamRead(stream, worldActions, worldActionCount * sizeof(WorldAction));
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

