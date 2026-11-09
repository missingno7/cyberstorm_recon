void BestLocEmpty(void)
{
}

extern int ActorValue(void *actor, ...);
extern int BestLocSetup(void *list, int count, int mode);
extern void BestLocVisit(void *list, int (*callback)(void *item));
extern int ScoreCandidate(void *item);
extern int BestLocSubCandidate(void *item);
extern unsigned char bestLocEntries[];
extern void *bestLocActor;
extern void *bestLocData;
extern int bestLocIndex;
extern int bestLocCursor;
extern int bestLocCount;
extern unsigned char bestLocSortKey[];
extern unsigned char bestLocOutput[];

extern void BestLocSetData(void *table, void *entries, void *data);

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
            BestLocVisit(bestLocEntries, BestLocSubCandidate);
            if (ActorValue(actor) == bestLocIndex)
                bestLocIndex = 0;
        }
    }
    return bestLocIndex;
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
