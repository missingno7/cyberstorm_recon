void BestLocEmpty(void)
{
}

struct BestLocPoint { int x, y; };
struct BestLocEntry { int type; BestLocPoint pos; int index; };
struct LogicUnitRef { int id; };
struct LogicActor { LogicUnitRef *unit; char pad04[0x64]; int nearRange, farRange; };   // + 0x68, + 0x6c
struct TargetTriple { unsigned char kind, cost; char pad[10]; };                      // 12 bytes
struct SearchWorkspace;
extern int SearchGetCost(SearchWorkspace *, int);
extern int ActorEnable(LogicActor *);
extern "C" int LogicCopyPoint(void *, void *);
extern int ActorNetStrengthAt(const char *);
extern int *actorIndexTable;
extern unsigned char *targetDefinitionTable;
extern int logicWorldOffsetX;
inline TargetTriple *BestLocTarget(int index) { return (TargetTriple *)targetDefinitionTable + index; }
inline int ActorIndexUnit(int index) { return actorIndexTable[index * 2]; }
extern int ActorValue(LogicActor *actor);
extern int BestLocSetup(void *list, int count, int mode);
extern void BestLocVisit(void *list, int (*callback)(void *item));
extern int ScoreCandidate(void *item);
extern int BestLocSubCandidate(BestLocEntry *item);
extern unsigned char bestLocEntries[];
extern void *bestLocActor;
extern void *bestLocData;
extern int bestLocIndex;
extern int bestLocCursor;
extern int bestLocCount;
extern unsigned char bestLocOutput[];

extern void BestLocSetData(void *table, void *entries, void *data);
extern char *ActorName(LogicActor *actor);
extern "C" int PointAngle(BestLocPoint *, BestLocPoint *);
struct RayPoint;
extern int RayOcclusion(int *grid, int origin, int target, int sample = -1, RayPoint *hit = 0);
extern "C" void TargetObjectInit(const char *, BestLocPoint *, int);
extern char fallbackName[];
extern int currentActorValue;
static char bestLocSourcePath[] = __FILE__;
inline int ClampRange(int value, int low, int high)
{
    if (value <= low) return low;
    if (value >= high) return high;
    return value;
}
extern int bestLocCurrentScore;
extern int bestLocCurrentTie;
extern int bestLocTolerance;
extern int bestLocBestValue;
extern int bestLocCompareMode;
extern int bestLocLimit;
extern unsigned char bestLocLimitBase[];
extern "C" int abs(int value);
extern "C" void AssertFail(const char *file, int line, const char *message);
extern int ActorArrayGet(LogicActor *actor, int index);
extern "C" int RandomRange(unsigned int *, int low, int high);
extern unsigned int MissionRandomTable[];

int BestLocFindCandidate(BestLocEntry *entry)
{
    BestLocPoint right, left;
    int index = entry->index;
    if (!index) return 0;
    if (bestLocLimit && LogicCopyPoint(&entry->pos, bestLocLimitBase) > bestLocLimit)
        return 0;
    int rank = SearchGetCost((SearchWorkspace *)bestLocEntries, index);
    if (rank <= 0 || rank > ActorEnable((LogicActor *)bestLocActor))
        return 0;
    if (ActorIndexUnit(index)) return 0;
    int distance = LogicCopyPoint(&entry->pos, bestLocOutput);
    if (distance < ((LogicActor *)bestLocActor)->nearRange) return 0;
    int score, tie;
    if (distance <= ((LogicActor *)bestLocActor)->farRange) {
        int visibility = RayOcclusion((int *)fallbackName, index, (int)bestLocData);
        if (visibility < 80) {
            score = visibility;
            tie = 0;
        } else {
            score = 101;
            if (bestLocCompareMode) {
                TargetObjectInit(fallbackName, &left, (int)bestLocData);
                TargetObjectInit(fallbackName, &right, index);
                tie = abs((bestLocBestValue - PointAngle(&left, &right)) >> 16);
            } else {
                tie = -LogicCopyPoint(&entry->pos, ActorName((LogicActor *)bestLocActor));
            }
        }
        int delta = abs(distance - bestLocTolerance);
        score += (5 - ClampRange(delta, 0, 5)) * 100;
    } else {
        TargetTriple *target = BestLocTarget(index);
        score = -distance;
        tie = -(int)target->cost;
    }
    if (!bestLocIndex || score > bestLocCurrentScore ||
        (score == bestLocCurrentScore && tie > bestLocCurrentTie)) {
        bestLocIndex = index;
        bestLocCurrentScore = score;
        bestLocCurrentTie = tie;
    }
    return 0;
}

int IntelFindBest(int *actor, int setting, int limit)
{














































    if (actor[1] == 0) AssertFail(bestLocSourcePath, __LINE__, 0);
    int actorValue = ActorValue((LogicActor *)actor[1]);
    bestLocLimit = 0;
    if (setting != 0 && limit != 0) {
        BestLocSetData(fallbackName, bestLocLimitBase, (void *)setting);
        bestLocLimit = limit;
    }
    bestLocActor = actor;
    bestLocIndex = 0;
    if (actorValue != 0) {
        bestLocData = (void *)actorValue;
        BestLocSetData(fallbackName, bestLocOutput, bestLocData);

        int switchValue = ActorArrayGet((LogicActor *)actor, 7);
        switch (switchValue) {
        case 6:
        case 7:
        case 9:
            bestLocTolerance = actor[0x1a];
            break;
        case 8:
        case 10:
        {
            int parity = actor[5];
            int phase = currentActorValue;
            if ((parity + phase) & 1)
                bestLocTolerance = actor[0x1a];
            else
                bestLocTolerance = actor[0x1b];
        }
            break;
        default:
            bestLocTolerance = RandomRange(MissionRandomTable, actor[0x1a], actor[0x1b]);
            break;
        }

        int *nestedActor = *(int **)(actor + 1);
        unsigned char *actorState = (unsigned char *)*(int *)(nestedActor[0] + 0x18);
        bestLocCompareMode = actorState[0xca] >= 5;
        if (bestLocCompareMode) {
            nestedActor = *(int **)(actor + 1);
            int stateShift = (int)(*(unsigned char **)(nestedActor[0] + 0x18))[0xc9] << 24;
            bestLocBestValue = *(int *)(nestedActor[0] + 0x46) - stateShift - 0x7fffffcc;
        }
        bestLocCursor = 0;
        BestLocSetup(bestLocEntries, ActorValue((LogicActor *)actor), 7);
        bestLocCount = 0;
        BestLocVisit(bestLocEntries, (int (*)(void *))BestLocFindCandidate);
        if (ActorValue((LogicActor *)actor) == bestLocIndex)
            bestLocIndex = 0;
        else if (bestLocCurrentScore < 0 && -bestLocCurrentScore > LogicCopyPoint(ActorName((LogicActor *)actor), bestLocOutput))
            bestLocIndex = 0;
    }
    return bestLocIndex;
}

int BestLocSubCandidate(BestLocEntry *entry)
{
    int index = entry->index;
    if (index == 0)
        return 0;
    int rank = SearchGetCost((SearchWorkspace *)bestLocEntries, index);
    if (rank <= 0)
        return 0;
    if (rank > ActorEnable((LogicActor *)bestLocActor))
        return 0;
    int mapActor = ActorIndexUnit(index);
    if (mapActor != 0) {
        LogicActor *actor = (LogicActor *)bestLocActor;
        if (mapActor != (actor->unit ? actor->unit->id : 0))
            return 0;
    }
    int score = LogicCopyPoint(&entry->pos, bestLocOutput);
    if (score >= ((LogicActor *)bestLocActor)->farRange + 3)
        score = ActorNetStrengthAt((const char *)&entry->pos) + 1000;
    int tie = targetDefinitionTable[index * 12 + 1];
    tie = -tie;
    if (bestLocIndex == 0 || score > bestLocCurrentScore ||
        (score == bestLocCurrentScore && tie > bestLocCurrentTie)) {
        bestLocIndex = index;
        bestLocCurrentScore = score;
        bestLocCurrentTie = tie;
    }

    return 0;
}

int BestLocBegin(void *actor, void *settings)
{
    bestLocActor = actor;
    bestLocIndex = 0;
    if (ActorValue((LogicActor *)actor)) {
        if (settings) {
            bestLocData = settings;
            BestLocSetData(fallbackName, bestLocOutput, settings);
            bestLocCursor = 0;
            BestLocSetup(bestLocEntries, ActorValue((LogicActor *)actor), 7);
            bestLocCount = 0;
            BestLocVisit(bestLocEntries, (int (*)(void *))BestLocSubCandidate);
            if (ActorValue((LogicActor *)actor) == bestLocIndex)
                bestLocIndex = 0;
        }
    }
    return bestLocIndex;
}

int ScoreCandidate(void *value)
{
    int index = ((BestLocEntry *)value)->index;
    if (index == 0)
        return 0;
    int rank = SearchGetCost((SearchWorkspace *)bestLocEntries, index);
    if (rank <= 0)
        return 0;
    if (rank > ActorEnable((LogicActor *)bestLocActor))
        return 0;
    int mapActor = ActorIndexUnit(index);
    if (mapActor != 0) {
        LogicActor *actor = (LogicActor *)bestLocActor;
        if (mapActor != (actor->unit ? actor->unit->id : 0))
            return 0;
    }
    int score = LogicCopyPoint(&((BestLocEntry *)value)->pos, &logicWorldOffsetX);
    int tie = -targetDefinitionTable[index * 12 + 1];
    if (bestLocIndex == 0 || score > bestLocCurrentScore ||
        (score == bestLocCurrentScore && tie > bestLocCurrentTie)) {
        bestLocIndex = index;
        bestLocCurrentScore = score;
        bestLocCurrentTie = tie;
    }

    return 0;
}

int BestLocRun(void *actor)
{
    bestLocActor = actor;
    bestLocIndex = 0;
    if (ActorValue((LogicActor *)actor)) {
        bestLocCursor = 0;
        BestLocSetup(bestLocEntries, ActorValue((LogicActor *)actor), 7);
        bestLocCount = 0;
        BestLocVisit(bestLocEntries, ScoreCandidate);
        if (ActorValue((LogicActor *)actor) == bestLocIndex)
            bestLocIndex = 0;
    }
    return bestLocIndex;
}


