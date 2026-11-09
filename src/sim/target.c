struct TargetTriple { int x, y, z; };
struct TargetRecord { unsigned char bytes[0xb4]; };
struct TargetBriefObject { char pad[0x10]; int active; int IsActive() const { return active != 0; } };
struct TargetModuleState { int mode; int IsOpen() const { return mode != 0; } };
extern "C" void BriefModuleSetup(void *, int, int, int, int); extern "C" void BriefModuleOpen(void *, int, int, int); extern "C" void PopLoadResource(void *, void **, int, int);
extern "C" void BriefObjectOpen(void *, const char *); extern "C" void BriefObjectLoadResource(void *, int, void *, int, int); extern "C" int BriefGetHandle(void *, void *); extern "C" int BriefStateSet(void *);
extern "C" void BriefRenderUpdate(void *); extern "C" void BriefTargetAction(int, int); extern "C" void LogicRedraw(int, void *); extern "C" int TargetLookup(void *, void *); extern "C" int TargetLookupFallback(void *, void *); extern "C" void TargetDisplay(void *, int, int);
extern "C" void TargetEventUp(void *, int); extern "C" void TargetEventDown(void *, int); extern "C" void TargetObjectInit(const char *, void *, int); extern "C" void TargetInitAt(const char *, void *, int, int);
extern "C" void BestLocSetData(void *, void *, void *); extern "C" int CreateState(int); extern "C" int GridClamp(int); extern "C" int TargetActorById(int); extern "C" int TargetDistance(void *, void *); extern "C" int TargetSqrt(int);
extern "C" int TargetCompute(const char *, int, int, int, void *); extern "C" int TargetObjectData(int); extern "C" void *memset(void *, int, unsigned int); extern "C" void AssertFail(const char *, int, const char *);
extern int targetModuleState, targetHeader, targetPrompt, targetPrompt1, targetPrompt2, targetPrompt3, targetPrompt4;
extern int targetPrompt5, targetPrompt6, targetObject, targetSavedObject, targetTable, targetFallback, targetLimits, gInA4, gInA0, targetSharedState;
extern unsigned char targetFlag;
extern TargetModuleState targetMode;
extern TargetTriple *targetDefinitionTable;
extern char targetFallbackName[], targetMenuResource[], targetBriefResource[];
void TargetLoadResource(void *resource);
void TargetEmpty(void);
class TargetModuleStartup
{
public:
    TargetModuleStartup()
    {
        BriefModuleSetup(&targetPrompt, 0xd, 1, 0, 0); targetModuleState = 1; BriefModuleOpen(&targetPrompt1, 0x85, 500, 0); targetModuleState = 2; BriefModuleOpen(&targetPrompt2, 0x88, 500, 0); targetModuleState = 3; BriefModuleOpen(&targetPrompt3, 0x8b, 500, 0); targetModuleState = 4; BriefModuleOpen(&targetPrompt4, 0x8e, 500, 0); targetModuleState = 5; TargetLoadResource(&targetPrompt5); targetModuleState = 6;
    }
    ~TargetModuleStartup() { TargetEmpty(); }
};
TargetModuleStartup targetModuleStartup;
void TargetLoadResource(void *resource) { PopLoadResource(resource, (void **)&targetSavedObject, 5, 0); }
void TargetMapAlert(void *object, int callback)
{
    if (!((TargetBriefObject *)object)->IsActive()) { BriefObjectOpen(object, "MAPALERT.BOX"); BriefObjectLoadResource(object, callback, targetBriefResource, 0xc9, 0); }
}
void TargetClearActive(void *object) { if (((TargetBriefObject *)object)->IsActive()) *(int *)((char *)object + 0x10) = 0; }
void TargetClose(void *object) { if (((TargetBriefObject *)object)->IsActive()) { int handle = BriefGetHandle(object, &targetSharedState); if (handle >= 0) targetLimits = handle + 100; BriefStateSet(object); } }
void TargetUpdate(void *object) { if (((TargetBriefObject *)object)->IsActive()) BriefRenderUpdate(object); }
void TargetHandleEvent(void *object)
{
    int state; if (targetMode.IsOpen()) { int handle = BriefGetHandle(&targetPrompt5, &targetSharedState); if (handle >= 0) targetLimits = handle + 100; state = BriefStateSet(&targetPrompt5); } else state = 0;
    switch (state) {
    case 1:
        if ((targetFlag & 0x10) == 0) { int mode = 0xb; if (gInA4 != 0) mode = 6; else if (gInA0 != 0) mode = 7; BriefTargetAction(mode, 0); targetLimits = mode + 0xf; return; }
        { int target = TargetLookup((void *)&targetTable, &targetSharedState); if (target == 0) target = TargetLookupFallback((void *)&targetTable, &targetSharedState); TargetDisplay(object, target, 0xb); return; }
    case 2: TargetEventUp(object, 1); LogicRedraw(9, (void *)0); return;
    case 3: TargetEventUp(object, -1); LogicRedraw(9, (void *)0); return;
    case 4: TargetEventDown(object, -1); return;
    case 5: TargetEventDown(object, 1); return;
    default: return;
    }
}
void TargetSetInactive(void *object) { ((TargetBriefObject *)object)->active = 0; }
void TargetEmpty(void) { }
int TargetFillRecord(void *unused, TargetRecord *record, int index)
{
    if (index == 0) return 0;
    *(int *)&record->bytes[0] = index; TargetObjectInit(targetFallbackName, &record->bytes[0xc], index);
    BestLocSetData(targetFallbackName, &record->bytes[4], (void *)index); TargetInitAt(targetFallbackName, &record->bytes[0x38], index, 0);
    *(TargetTriple *)&record->bytes[0x2c] = targetDefinitionTable[index]; return 1;
}
int TargetSetupState(void *unused, TargetRecord *record, int index)
{
    if (index <= 0) return 0;
    int state = CreateState(index);
    if (state != 0) {
        *(int *)&record->bytes[0x48] = index; *(int *)&record->bytes[0x44] = state;
        int distance = *(int *)(state + 0x46); *(int *)&record->bytes[0x4c] = distance;
        int type = *(int *)(state + 0x42); *(int *)&record->bytes[0] = type;
        *(int *)&record->bytes[0x50] = GridClamp(*(int *)&record->bytes[0x4c]); return 1;
    }
    return 0;
}
// The encoded line values are evidence from sim\target.c.
void TargetInitializeRecord(void *unused, register TargetRecord *record,
                           register int source, register int destination)
{
    memset(record, 0, 0xb4);
    if (destination == 0) return;
    if (source > 0) { int sourceState = CreateState(source); if (sourceState != 0) { *(int *)&record->bytes[0x48] = source; *(int *)&record->bytes[0x44] = sourceState; *(int *)&record->bytes[0x4c] = *(int *)(sourceState + 0x46); int sourceType = *(int *)(sourceState + 0x42); *(int *)&record->bytes[0] = sourceType; *(int *)&record->bytes[0x50] = GridClamp(*(int *)&record->bytes[0x4c]); } }
    int type = *(int *)&record->bytes[0];
    if (type != 0) { TargetObjectInit(targetFallbackName, &record->bytes[0xc], type); BestLocSetData(targetFallbackName, &record->bytes[4], (void *)type); TargetInitAt(targetFallbackName, &record->bytes[0x38], type, 0); *(TargetTriple *)&record->bytes[0x2c] = targetDefinitionTable[type]; }
    if (*(int *)&record->bytes[0x44] == 0 || *(int *)&record->bytes[0] == 0)
        AssertFail("sim\\target.c", 0x52, 0);
    /* Source line spacing before the second original assertion. */
    if (destination < 0) { int opposite = -destination;
        if (opposite > 0) { int stateObject = CreateState(opposite); if (stateObject != 0) { TargetRecord *other = (TargetRecord *)&record->bytes[0x54];
            *(int *)&other->bytes[0x48] = opposite; *(int *)&other->bytes[0x44] = stateObject; *(int *)&other->bytes[0x4c] = *(int *)(stateObject + 0x46); *(int *)&other->bytes[0] = *(int *)(stateObject + 0x42); *(int *)&other->bytes[0x50] = GridClamp(*(int *)&other->bytes[0x4c]); } }
        int otherType = *(int *)&record->bytes[0x54]; if (otherType != 0) { TargetObjectInit(targetFallbackName, &record->bytes[0x60], otherType); BestLocSetData(targetFallbackName, &record->bytes[0x58], (void *)otherType); TargetInitAt(targetFallbackName, &record->bytes[0x8c], otherType, 0); *(TargetTriple *)&record->bytes[0x80] = targetDefinitionTable[otherType]; }
        if (*(int *)&record->bytes[0x98] != 0 && *(int *)&record->bytes[0x54] == 0)
            AssertFail("sim\\target.c", 0x59, 0);
    } else {
        *(int *)&record->bytes[0x54] = destination;
        TargetObjectInit(targetFallbackName, &record->bytes[0x60], destination);
        BestLocSetData(targetFallbackName, &record->bytes[0x58], (void *)destination);
        TargetInitAt(targetFallbackName, &record->bytes[0x8c], destination, 0);
        *(TargetTriple *)&record->bytes[0x80] = targetDefinitionTable[destination];
        int *actorState = (int *)TargetActorById(destination);
        if (actorState != 0) { int otherIndex = *actorState;
            if (otherIndex > 0) { int stateObject = CreateState(otherIndex);
                if (stateObject != 0) { TargetRecord *other = (TargetRecord *)&record->bytes[0x54];
                    *(int *)&other->bytes[0x48] = otherIndex; *(int *)&other->bytes[0x44] = stateObject;
                    *(int *)&other->bytes[0x4c] = *(int *)(stateObject + 0x46); *(int *)&other->bytes[0] = *(int *)(stateObject + 0x42);
                    *(int *)&other->bytes[0x50] = GridClamp(*(int *)&other->bytes[0x4c]);
                }
            }
        }
    }
    *(int *)&record->bytes[0xb0] = TargetDistance(&record->bytes[0x60], &record->bytes[0xc]);
    int dx = *(int *)&record->bytes[0x60] - *(int *)&record->bytes[0xc];
    int dy = *(int *)&record->bytes[0x64] - *(int *)&record->bytes[0x10];
    *(int *)&record->bytes[0xa8] = TargetSqrt(dx * dx + dy * dy);
    *(int *)&record->bytes[0xac] = TargetCompute(targetFallbackName,
        *(int *)&record->bytes[0], *(int *)&record->bytes[0x54], 0x37, targetMenuResource);
    int actor = *(int *)&record->bytes[0x98];
    if (actor != 0) { int count = TargetObjectData(actor + 0x20); if (count > 1) *(int *)&record->bytes[0xa8] -= (count - 1) * 1000; }
}
int TargetRecordType(void *unused, int target)
{
    if (target == 0) return 0;
    int *record = (int *)TargetActorById(target);
    if (record != 0) return -*record;
    return target;
}
