typedef unsigned char u8;
typedef unsigned int u32;
#pragma pack(push,1)
struct ActorPoint { int x,y; }; struct AnimAction { int depth; u8 type,state,angle,rate,frame,count,flags,ready; unsigned short kind; u32 time; ActorPoint position,target; int resource,direction,x,y; void *block; u8 b36,random,b38,b39,color; int drawX,drawY,drawWidth,drawHeight; void *bitmap; ActorPoint from,to; int step,steps; };
struct ActorRecord { int index,previous; void *owner; int field0c,field10,field14; void *unit,*pilot; AnimAction action; u8 busy,enabled,unknown89,draw,active; u8 unknown8c[2],selected,unknown8f[9]; };
#pragma pack(pop)
struct ActorListGlobalObject { char bytes[0x7e]; }; struct BestLocBuffer { char bytes[0x28]; }; extern ActorListGlobalObject actorListGlobalObject;

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
extern ActorRecord *ActorListAdd(void *, void *, void *, int, int, int); extern int BestLocInitialize(void *, const char *, int); extern int BestLocSetCandidate(void *, void *); extern void BestLocVisit(void *, int (*)(void *)); struct AnimAction; struct SimGame; struct ActorUnit; struct PlayerSlot { char pad00[0xc]; int slot; char pad10[0xa4]; unsigned char color; }; extern PlayerSlot *GetModePlayer(SimGame *, int); extern char modeCheckName[]; inline int CellActor(int cell) { return actorIndexTable[cell * 2]; } extern int animationMetaAt0cbc[]; extern "C" int f_00439d02(ActorUnit *); extern void SimAddMission(ActorUnit *); extern "C" int ResolveName(const char *, int); extern "C" void TargetObjectInit(const char *, void *, int); extern void AnimActionInitialize(AnimAction *, int, int, int, int); extern void WorldActionNotify(AnimAction *, int); extern void AnimActionReplace(AnimAction *, unsigned long); extern "C" unsigned int next_link_seed(unsigned int *); extern unsigned int MissionRandomTable;

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
    record->field10 = 0;
    return record;
}

ActorRecord *ActorListAdd(void *input, void *a, void *b, int d, int e, int f)
{
    ActorRecord *record;
    PlayerSlot *player;
    int slot, name;
    if (actorCount >= 400)
        return 0;
    slot = ++actorCount;
    record = ActorGetByIndex(slot);







    ASSERT(record);
    memset(record, 0, sizeof(ActorRecord));
    record->previous = -1;
    record->busy = 0;
    record->enabled = 1;
    record->draw = 1;
    record->active = 1;
    record->selected = (char)0xff;
    record->index = slot;
    record->owner = input;
    record->unit = a;
    record->pilot = b;
    record->action.random = next_link_seed(&MissionRandomTable) % 100;
    player = GetModePlayer((SimGame *)modeCheckName, (int)input);
    player->slot = slot;
    if (f == 0) f = f_00439d02((ActorUnit *)a);
    int typeIndex = animationMetaAt0cbc[f * 15] + e - 1;

    SimAddMission((ActorUnit *)a);
    name = ResolveName(fallbackName, d);





    ASSERT(name);
    if (CellActor(name)) {
        BestLocBuffer search;
        actorContext = typeIndex;
        currentActor = 0;
        BestLocInitialize(&search, fallbackName, 0x20);
        BestLocSetCandidate(&search, (void *)name);
        BestLocVisit(&search, ActorFindAndSet);
        name = (int)currentActor;
















        ASSERT(name);
    }
    TargetObjectInit(fallbackName, &record->action.position, name);
    AnimActionInitialize(&record->action, 1, f, 0x63, 0);
    WorldActionNotify(&record->action, 1);
    record->action.direction = next_link_seed(&MissionRandomTable) % 6 * 0x2aaaaabc;
    AnimActionReplace(&record->action, record->index);
    record->action.color = player->color;
    return record;
}

