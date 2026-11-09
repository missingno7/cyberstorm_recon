typedef unsigned char u8;
typedef unsigned int u32;
typedef struct ActorRecord { char bytes[0x98]; } ActorRecord; struct ActorListGlobalObject { char bytes[0x7e]; }; struct BestLocBuffer { char bytes[0x28]; }; extern ActorListGlobalObject actorListGlobalObject;

extern int actorCount;
extern ActorRecord *actorTable;
extern int actorContext;
extern ActorRecord *currentActor;
extern int worldState;
extern int *actorSettings;
extern int *objectIndex; extern int *actorIndexTable; extern int actorAddTypeTable[]; extern const char fallbackName[];

extern "C" void AssertFail(const char *, int, const char *);
extern "C" void *memset(void *, int, unsigned int);
extern "C" void *Allocate(unsigned int);
extern "C" void *LookupRecord(int);
extern "C" int LoadActorData(void *, int, int);
extern "C" int SelectActorData(void *);
extern "C" int ReadActorData(void *, void *);
extern "C" int EnumerateActorData(void *, int (*)(void *, void *));
extern "C" int WorldActorCheck(void *, void *);
extern "C" void CopyActorName(void *, void *, void *);
extern "C" int ActorInit(void *, int, int, int, int);
extern "C" int ActorSetState(void *, int);
extern "C" int ActorSetOwner(void *, int);
extern "C" int ActorGetSeed(void *);
extern "C" void ActorSetRandom(void *, int);
extern "C" void ActorFinish(ActorRecord *, int, int, int, int, int);
extern ActorRecord *ActorGetByIndex(int);
extern ActorRecord *ActorListAdd(void *, void *, void *, int, int, int); extern void *ActorInstanceCreate(void *, void *); extern int ResolveActorName(const char *, int); extern int BestLocInitialize(void *, const char *, int); extern int BestLocSetCandidate(void *, void *); extern void BestLocVisit(void *, int (*)(void *)); extern int WorldActionCopyName(const void *, void *, void *); extern int WorldActorApply(void *, int, int, int, int); extern int WorldActionNotify(void *, int); extern int WorldActionStart(void *, int); extern unsigned int RandomUpdate(unsigned int *);

#define ASSERT(e) if (!(e)) AssertFail(__FILE__, __LINE__, 0)

int ActorFindAndSet(void *candidate)
{
    ActorRecord *selected = *(ActorRecord **)((char *)candidate + 0x0c);
    if (selected != 0 && WorldActorCheck(selected, (void *)actorContext)) {
        currentActor = selected;
        return 1;
    }
    return 0;
}

int ActorListIsBusy()
{
    return actorCount >= 320;
}

ActorRecord *ActorCreate(void *input, void *actorData, int c, int d)
{
    ActorRecord *record;
    void *temporary;
    if (actorCount >= 400)
        return 0;
    temporary = Allocate(0x10b);










    /* ASSERT_LINE_66 */ ASSERT(temporary != 0);
    LoadActorData(temporary, (int)(unsigned long)actorData, 0);
    record = ActorListAdd(input, temporary, &actorListGlobalObject, c, d,
                          *(int *)((char *)actorData + 4));
    *(int *)(record->bytes + 0x10) = 0;
    return record;
}

ActorRecord *ActorListAdd(void *input, void *a, void *b, int d, int e, int f)
{
    ActorRecord *record;
    int slot;
    void *resource;
    register void *ownerArgument = (void *)*(volatile void **)&input;
    register int typeAddend = e;
    register int typeArgument = *(volatile int *)&f;
    if (actorCount >= 400)
        return 0;
    slot = ++actorCount;
    record = (ActorRecord *)ActorGetByIndex(slot);








    /* ASSERT_LINE_90 */ ASSERT(record != 0);
    memset(record, 0, sizeof(ActorRecord));
    *(int *)(record->bytes + 4) = -1;
    record->bytes[0x87] = 0;
    record->bytes[0x88] = 1;
    record->bytes[0x8a] = 1;
    record->bytes[0x8b] = 1;
    record->bytes[0x8e] = (char)0xff;
    *(int *)(record->bytes + 0) = slot;
    *(void **)(record->bytes + 8) = ownerArgument;
    *(void **)(record->bytes + 0x18) = a;
    *(void **)(record->bytes + 0x1c) = b;
    unsigned int randomValue = RandomUpdate((unsigned int *)0x51445c);
    record->bytes[0x57] = (char)(randomValue % 100);
    resource = ActorInstanceCreate(&actorSettings, input);
    *(int *)((char *)resource + 0x0c) = slot;
    if (typeArgument == 0)
        typeArgument = SelectActorData(a);
    int typeIndex = actorAddTypeTable[typeArgument * 15] + typeAddend - 1;
    int nameIndex = ResolveActorName(fallbackName, d);





    /* ASSERT_LINE_115 */ ASSERT(nameIndex != 0);
    int chosen = nameIndex;
    if (actorIndexTable[nameIndex * 2] != 0) {
        actorContext = (int)(unsigned long)resource;
        currentActor = 0;
        BestLocBuffer search;
        BestLocInitialize(&search, fallbackName, 0x20);
        BestLocSetCandidate(&search, (void *)(unsigned long)nameIndex);
        BestLocVisit(&search, ActorFindAndSet);
        chosen = (int)(unsigned long)currentActor;















        /* ASSERT_LINE_140 */ ASSERT(chosen != 0);
    }
    WorldActionCopyName(fallbackName, record->bytes + 0x32,
                        (void *)(unsigned long)chosen);
    WorldActorApply(record->bytes + 0x20, 1, typeArgument, 0x63, 0);
    WorldActionNotify(record->bytes + 0x20, 1);
    unsigned int fractional = RandomUpdate((unsigned int *)0x51445c) % 6;
    *(int *)(record->bytes + 0x46) = fractional * 0x2aaaaabc;
    WorldActionStart(record->bytes + 0x20, *(int *)record->bytes);
    record->bytes[0x5a] = *((char *)resource + 0xb4);
    return record;
}
