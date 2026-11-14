static char targetSourceFile[] = __FILE__; struct TargetTriple { int x, y, z; };
struct TargetPoint2 { long x, y; }; struct TargetObject { long x, y; char rest[0x18]; }; struct TargetInit { char bytes[0xc]; }; struct TargetSide { int type; TargetPoint2 point; TargetObject object; TargetTriple triple; TargetInit init; int state, index, distance, clamp; }; struct TargetRecord { TargetSide source, destination; int distance, occlusion, angle; };
struct TargetBriefObject { char pad[0x10]; int active; int IsActive() const { return active != 0; } };
inline int TargetSpecialFlag(const unsigned char &x) { return (x & 16) != 0; } inline int TargetInput(int x) { return x; } struct TargetModuleState { int mode; int IsOpen() const { return mode != 0; } };
extern "C" void BriefModuleSetup(void *, int, int, int, int); extern "C" void BriefModuleOpen(void *, int, int, int); extern "C" void PopLoadResource(void *, void **, int, int);
extern "C" void BriefObjectOpen(void *, const char *); extern "C" void BriefObjectLoadResource(void *, int, void *, int, int); extern "C" int BriefGetHandle(void *, void *); extern "C" int BriefStateSet(void *);
extern "C" void BriefRenderUpdate(void *); extern "C" void SimSetCursor(int, int); extern "C" void LogicCallMode(int, void *); extern "C" int MapPickAt(void *, void *); extern "C" int MapNearestAt(void *, void *); extern "C" void SimHoverActor(void *, int, int);
extern "C" void SimSetMode(void *, int); extern "C" void SimSetView(void *, int); extern "C" void TargetObjectInit(const char *, void *, int); extern "C" void TargetInitAt(const char *, void *, int, int);
extern "C" void BestLocSetData(void *, void *, void *); extern "C" int CreateState(int); extern "C" int GridClamp(int); extern "C" int TargetActorById(int); extern "C" int TargetDistance(void *, void *); extern "C" int TargetSqrt(int);
extern "C" int TargetCompute(const char *, int, int, int, void *); extern "C" int TargetObjectData(int); extern "C" void *memset(void *, int, unsigned int); extern "C" void AssertFail(const char *, int, const char *);
extern int targetPrompt6, targetObject, targetSavedObject, gFastTransform, targetFallback, targetLimits, gInA0, targetSharedState;
extern long gInA4; extern unsigned char targetFlag;
extern TargetModuleState targetMode;
extern TargetTriple *targetDefinitionTable;
extern char targetFallbackName[], targetMenuResource[], targetBriefResource[];
// sim\mapalert.c (the alert popup and its static gadgets) is the preceding object: src/sim/mapalert.c.
// The object starts with an empty static initializer (0045a910, XI record 004a6c84): a global whose class has an
// empty inline constructor. Which global is unknown; only the initializer is evidence.
struct TargetPoint { long x, y; TargetPoint() {} };
TargetPoint targetStaticPoint;































inline int TargetFill(TargetSide *side, int type)
{
    if (type == 0) return 0;
    side->type = type; TargetObjectInit(targetFallbackName, &side->object, type);
    BestLocSetData(targetFallbackName, &side->point, (void *)type); TargetInitAt(targetFallbackName, &side->init, type, 0);
    side->triple = targetDefinitionTable[type]; return 1;
}
inline int TargetSetup(TargetSide *side, int index)
{
    if (index <= 0) return 0;
    int state = CreateState(index);
    if (state != 0) {
        side->index = index; side->state = state; side->distance = *(int *)(state + 0x46); side->type = *(int *)(state + 0x42);
        side->clamp = GridClamp(side->distance); return 1;
    }
    return 0;
}
int TargetFillRecord(void *unused, TargetRecord *record, int index) { return TargetFill(&record->source, index); }
int TargetSetupState(void *unused, TargetRecord *record, int index) { return TargetSetup(&record->source, index); }





void TargetInitializeRecord(void *unused, TargetRecord *record, int source, int destination)
{
    memset(record, 0, sizeof(TargetRecord));
    if (destination == 0) return;
    TargetSetup(&record->source, source);
    TargetFill(&record->source, record->source.type);
    if (record->source.state == 0 || record->source.type == 0) AssertFail(targetSourceFile, __LINE__, 0);


    TargetSide *other = &record->destination;
    if (destination < 0) {
        TargetSetup(other, -destination);
        TargetFill(&record->destination, record->destination.type);
        if (record->destination.state == 0 || record->destination.type == 0) AssertFail(targetSourceFile, __LINE__, 0);
    } else {
        TargetFill(other, destination);
        int *actor = (int *)TargetActorById(destination);
        if (actor != 0) TargetSetup(&record->destination, *actor);
    }
    TargetObject *from = &record->source.object, *to = &record->destination.object;
    record->angle = TargetDistance(from, to);
    int dx = to->x - from->x;
    int dy = to->y - from->y;
    record->distance = TargetSqrt(dx * dx + dy * dy);
    record->occlusion = TargetCompute(targetFallbackName, record->source.type, record->destination.type, 0x37, targetMenuResource);
    if (record->destination.state != 0) { int count = TargetObjectData(record->destination.state + 0x20); if (count > 1) record->distance -= (count - 1) * 1000; }
}
int TargetRecordType(void *unused, int target)
{
    if (target == 0) return 0;
    int *record = (int *)TargetActorById(target);
    if (record != 0) return -*record;
    return target;
}
