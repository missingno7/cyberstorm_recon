void BestLocEmpty(void)
{
}

struct BestLocEntry;
extern int ActorValue(void *actor, ...);
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
extern unsigned char bestLocSortKey[];
extern unsigned char bestLocOutput[];

extern void BestLocSetData(void *table, void *entries, void *data);
extern int ActorEnable(void *actor);
extern char *ActorName(void *actor);
extern int BestLocFindEntry(void *entries, int key);
extern int BestLocDistance(int *left, int *right);
extern int BestLocEvaluate(const char *table, int key, void *data, int limit, void *result = 0);
extern void BestLocExtract(const char *table, int *output, void *data);
extern int BestLocCompare(int *left, int *right);
extern int BestLocMeasurement(void *value);
extern int bestLocCurrentScore;
extern int bestLocCurrentTie;
extern int bestLocTolerance;
extern int bestLocBestValue;
extern int bestLocCompareMode;
extern int bestLocLimit;
extern unsigned char bestLocLimitBase[];
extern int bestLocLookup[];
extern unsigned char bestLocMetrics[];
extern unsigned char bestLocScoreKey[];
extern "C" int abs(int value);
extern void AssertFail(const char *file, int line, int condition);
extern int ActorArrayGet(void *actor, int index);
extern int RandomRange(int *seed, int low, int high);
extern int missionRandomSeed;
extern int missionPhase;

int BestLocFindCandidate(void *value)
{
    int left[2];
    int right[2];
    int distance;
    int index = ((int *)value)[3];
    if (index == 0)
        return 0;
    if (bestLocLimit != 0 &&
        BestLocDistance((int *)((char *)value + 4), (int *)bestLocLimitBase) > bestLocLimit)
        return 0;
    int rank = BestLocFindEntry(bestLocEntries, index);
    if (rank <= 0)
        return 0;
    if (rank > ActorEnable(bestLocActor))
        return 0;
    int mapActor = bestLocLookup[index * 2];
    if (mapActor != 0) {
        int actor = *(int *)bestLocActor;
        if (actor != 0)
            actor = *(int *)actor;
        if (mapActor != actor)
            return 0;
    }

    distance = BestLocDistance((int *)((char *)value + 4), (int *)bestLocOutput);
    if (distance < *(int *)((char *)bestLocActor + 0x68))
        return 0;
    int score;
    int tie;
    if (distance <= *(int *)((char *)bestLocActor + 0x6c)) {
        score = BestLocEvaluate((const char *)bestLocSortKey, index, bestLocData, -1);
        if (score < 0x50) {
            tie = 0;
        } else {
            score = 0x65;
            if (bestLocCompareMode == 0) {
                tie = -BestLocDistance((int *)ActorName(bestLocActor), (int *)bestLocOutput);
            } else {
                BestLocExtract((const char *)bestLocSortKey, left, bestLocData);
                BestLocExtract((const char *)bestLocSortKey, right, (void *)index);
                tie = abs((bestLocBestValue - BestLocCompare(left, right)) >> 16);
            }
        }
        int delta = abs(distance - bestLocTolerance);
        if (delta < 1)
            delta = 0;
        else if (delta > 4)
            delta = 5;
        score += (5 - delta) * 100;
    } else {
        score = -distance;
        tie = -(int)bestLocMetrics[index * 12 + 1];
    }
    if (bestLocIndex != 0) {
        if (score < bestLocCurrentScore)
            return 0;
        if (score == bestLocCurrentScore && tie <= bestLocCurrentTie)
            return 0;
    }
    bestLocCurrentScore = score;
    bestLocCurrentTie = tie;
    bestLocIndex = index;
    return 0;
}

int BestLocDispatch(int *actor, int setting, int limit)
{
    /* ASSERT_PAD_01 */
    /* ASSERT_PAD_02 */
    /* ASSERT_PAD_03 */
    /* ASSERT_PAD_04 */
    /* ASSERT_PAD_05 */
    /* ASSERT_PAD_06 */
    /* ASSERT_PAD_07 */
    /* ASSERT_PAD_08 */
    /* ASSERT_PAD_09 */
    /* ASSERT_PAD_10 */
    /* ASSERT_PAD_11 */
    /* ASSERT_PAD_12 */
    /* ASSERT_PAD_13 */
    /* ASSERT_PAD_14 */
    /* ASSERT_PAD_15 */
    /* ASSERT_PAD_16 */
    /* ASSERT_PAD_17 */
    /* ASSERT_PAD_18 */
    /* ASSERT_PAD_19 */
    /* ASSERT_PAD_20 */
    /* ASSERT_PAD_21 */
    /* ASSERT_PAD_22 */
    /* ASSERT_PAD_23 */
    /* ASSERT_PAD_24 */
    /* ASSERT_PAD_25 */
    /* ASSERT_PAD_26 */
    /* ASSERT_PAD_27 */
    /* ASSERT_PAD_28 */
    /* ASSERT_PAD_29 */
    /* ASSERT_PAD_30 */
    /* ASSERT_PAD_31 */
    /* ASSERT_PAD_32 */
    /* ASSERT_PAD_33 */
    /* ASSERT_PAD_34 */
    /* ASSERT_PAD_35 */
    /* ASSERT_PAD_36 */
    /* ASSERT_PAD_37 */
    /* ASSERT_PAD_38 */
    /* ASSERT_PAD_39 */
    if (actor[1] == 0)
        AssertFail(__FILE__, __LINE__, 0);
    int actorValue = ActorValue((void *)actor[1]);
    bestLocLimit = 0;
    if (setting != 0 && limit != 0) {
        BestLocSetData(bestLocSortKey, bestLocLimitBase, (void *)setting);
        bestLocLimit = limit;
    }
    bestLocActor = actor;
    bestLocIndex = 0;
    if (actorValue != 0) {
    bestLocData = (void *)actorValue;
    BestLocSetData(bestLocSortKey, bestLocOutput, bestLocData);

    int switchValue = ActorArrayGet(actor, 7);
    switch (switchValue) {
    case 8:
    case 10:
        {
        register int parity = actor[5];
        register int phase = missionPhase;
        if (((parity + phase) & 1) == 0) {
            bestLocTolerance = actor[0x1b];
            break;
        }
        }
        // fall through
    case 6:
    case 7:
    case 9:
        bestLocTolerance = actor[0x1a];
        break;
    default:
        bestLocTolerance = RandomRange(&missionRandomSeed, actor[0x1a], actor[0x1b]);
        break;
    }

    int *nestedActor = *(int **)(actor + 1);
    unsigned char *actorState = (unsigned char *)*(int *)(nestedActor[0] + 0x18);
    bestLocCompareMode = actorState[0xca] >= 5;
    if (bestLocCompareMode) {
        int actorValueField = *(int *)(nestedActor[0] + 0x46);
        int stateShift = (int)actorState[0xc9] << 24;
        bestLocBestValue = actorValueField - stateShift - 0x7fffffcc;
    }
    bestLocCursor = 0;
    BestLocSetup(bestLocEntries, ActorValue(actor), 7);
    bestLocCount = 0;
    BestLocVisit(bestLocEntries, BestLocFindCandidate);
    if (ActorValue(actor) != bestLocIndex) {
        if (bestLocCurrentScore >= 0)
            return bestLocIndex;
        int nameDistance = BestLocDistance((int *)ActorName(actor), (int *)bestLocOutput);
        if (-nameDistance == bestLocCurrentScore || -bestLocCurrentScore < nameDistance)
            return bestLocIndex;
    }
    bestLocIndex = 0;
    }
    return bestLocIndex;
}

struct BestLocEntry { int type; int flags; int extra; int index; int payload[2]; };

int BestLocSubCandidate(BestLocEntry *entry)
{
    int index = entry->index;
    if (index == 0)
        return 0;
    int rank = BestLocFindEntry(bestLocEntries, index);
    if (rank <= 0)
        return 0;
    if (rank > ActorEnable(bestLocActor))
        return 0;
    int mapActor = bestLocLookup[index * 2];
    if (mapActor != 0) {
        int actor = *(int *)bestLocActor;
        if (actor != 0)
            actor = *(int *)actor;
        if (mapActor != actor)
            return 0;
    }
    int score = BestLocDistance(entry->payload, (int *)bestLocOutput);
    if (*(int *)((char *)bestLocActor + 0x6c) + 3 <= score)
        score = BestLocMeasurement(entry->payload) + 1000;
    int tie = -(int)bestLocMetrics[index * 12 + 1];
    if (bestLocIndex == 0 || bestLocCurrentScore < score ||
        (score == bestLocCurrentScore && bestLocCurrentTie < tie)) {
        bestLocCurrentScore = score;
        bestLocCurrentTie = tie;
        bestLocIndex = index;
    }
    return 0;
}

int BestLocBegin(void *actor, void *settings)
{
    bestLocActor = actor;
    bestLocIndex = 0;
    if (ActorValue(actor)) {
        if (settings) {
            bestLocData = settings;
            BestLocSetData(bestLocSortKey, bestLocOutput, settings);
            bestLocCursor = 0;
            BestLocSetup(bestLocEntries, ActorValue(actor), 7);
            bestLocCount = 0;
            BestLocVisit(bestLocEntries, (int (*)(void *))BestLocSubCandidate);
            if (ActorValue(actor) == bestLocIndex)
                bestLocIndex = 0;
        }
    }
    return bestLocIndex;
}

int ScoreCandidate(void *value)
{
    int index = ((int *)value)[3];
    if (index == 0)
        return 0;
    int rank = BestLocFindEntry(bestLocEntries, index);
    if (rank <= 0)
        return 0;
    if (rank > ActorEnable(bestLocActor))
        return 0;
    int mapActor = bestLocLookup[index * 2];
    if (mapActor != 0) {
        int actor = *(int *)bestLocActor;
        if (actor != 0)
            actor = *(int *)actor;
        if (mapActor != actor)
            return 0;
    }
    int score = BestLocDistance((int *)((char *)value + 4), (int *)bestLocScoreKey);
    int tie = -(int)bestLocMetrics[index * 12 + 1];
    if (bestLocIndex == 0 || bestLocCurrentScore < score ||
        (score == bestLocCurrentScore && bestLocCurrentTie < tie)) {
        bestLocCurrentScore = score;
        bestLocCurrentTie = tie;
        bestLocIndex = index;
    }
    return 0;
}

int BestLocRun(void *actor)
{
    bestLocActor = actor;
    bestLocIndex = 0;
    if (ActorValue(actor)) {
        bestLocCursor = 0;
        BestLocSetup(bestLocEntries, ActorValue(actor), 7);
        bestLocCount = 0;
        BestLocVisit(bestLocEntries, ScoreCandidate);
        if (ActorValue(actor) == bestLocIndex)
            bestLocIndex = 0;
    }
    return bestLocIndex;
}
