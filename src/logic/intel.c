#pragma inline_depth(10)
typedef unsigned char u8;
typedef unsigned int u32;
extern char modeCheckName[], fallbackName[], actorStateTable[]; extern int logicActorList;
// AssertFail gets the path from the compiler's source-name literal.
extern int gLocalActorKind, modeSwitch, g4d359c; extern char logicWorldOffsetX[];
extern int MissionRandomTable[];
extern void AssertFail(const char *, int, const char *);
extern int LogicCopyPoint(const char *, const char *), GridClamp(int), GridFindClosest(int, u32, u32);
extern const char *ActorName(int *);
extern int ActorValue(int *), ActorArrayGet(int *, int), ActorIsAvailable(int *);
extern int ActorCanChoose(int *), ActorPercent(int *), BestLocRun(int *), BestLocBegin(int *, int);
extern int next_link_seed(void *), ResolveName(const char *, const int *);
extern int *ActorListFind(void *, int);
extern int *IntelChooseCandidate(int *, int); extern int IntelUpdate(int *, int);
extern int IntelFindBest(int *, int = 0, int = 0), IntelFindBestByName(int, int);
extern int IntelResetOne(int *, int), IntelClearReservation(int *, int);
extern int SimPrepareAction(const char *, int), IntelSetMode3(const char *, int, int), IntelClearMode(const char *);
extern int IntelUnitQuery(int, int, int), HercNeedsAmmo(int);
extern int IntelPrepareState(int *, int = 0, int = 0), IntelContinueState(int *, int, int);
struct ActorPoint { int x, y; }; class LogicActorList { public: int *At(int); int *CreateActor(int, ActorPoint *, int, const char *); };
struct IntelUnitState { u8 pad[5]; u8 type; int Type() { return type; } }; struct IntelUnit { u8 pad[0x20]; struct IntelUnitState info; struct IntelUnitState *Info() { return &info; } int Allowed() { int n = Info()->Type(); return n != 1 && n != 7 && n != 14 && n != 17 && n != 18; } };
extern int IntelTargetData(int, int *, int), IntelGetActorState(int *), IntelChooseSpawnPos(void *, int, int, int, int), IntelFinalizeActor(int *);
extern int PointAngle(int *, int *);
extern void BestLocSetData(int, int *, int);
int IntelStateCommon(int *a);
void IntelActorCommand(int *a, int command, int value, int *target);

static inline int IntelState(int *a) { return a[0x22]; } static inline int IntelRadius() { return g4d359c; } static inline int IntelOwner(int *a) { if (a[0]) return *(int *)(a[0] + 8); return 0; }
static inline int IntelUnitAllowed(int *a) { return a[0] && ((IntelUnit *)a[0])->Allowed(); }
static inline int IntelHealthAlive(int *data) { return *(int *)((char *)data + 0x12) > 0; }
static inline int IntelUnitActive(int *actor) { return actor[0] && *(u8 *)(actor[0] + 0x88) && IntelHealthAlive(*(int **)(actor[0] + 0x1c)) && !*(u8 *)(actor[0] + 0x89); }

void IntelPredict(int *a)
{
    if (a[1] != 0) {
        int direction = PointAngle((int *)(a[0] + 0x32), (int *)(*(int *)a[1] + 0x32));
        int value = GridClamp(direction);
        IntelSetMode3(modeCheckName, value * 0x2aaaaabc, 0);
    }
}

int IntelDisableMode()
{
    IntelClearMode(modeCheckName);
    return 1;
}

int IntelStateWait(int *a)
{
    switch ((u32)IntelState(a)) {
    case 0:
        a[0x22] = 1;
        IntelChooseCandidate(a, 0);
        break;
    case 1:
        if (!IntelUnitAllowed(a)) {
            a[0x22] = 3;
            int value = LogicCopyPoint(ActorName(a), logicWorldOffsetX);
            if (value < IntelRadius()) {
                value = BestLocRun(a);
                if (value) {
                    SimPrepareAction(modeCheckName, value);
                    if (IntelUnitAllowed(a)) a[0x22] = 1;
                }
            }
        } else if (a[1]) IntelResetOne(a, 1);
        break;
    case 3:
        if (!a[1] || IntelResetOne(a, 0)) a[0x22] = 8;
        break;
    }
    if (a[0x22] == 8 && LogicCopyPoint(ActorName(a), logicWorldOffsetX) == IntelRadius())
        IntelClearMode(modeCheckName);
    return a[0x22] == 8;
}

int IntelMaybeBlock(int *a, int allow)
{
    int score;
    if (IntelChooseCandidate(a, 0) == 0) goto failed;
    score = LogicCopyPoint(ActorName(a), ActorName((int *)a[1]));
    if (score > 8 && allow == 0) goto clear;
    a[0x22] = 1;
    if (IntelUnitAllowed(a)) *(u8 *)(a[0] + 0x56) = 3;
    return 1;
clear: a[1] = 0;
failed: return 0;
}

int IntelStateApproach(int *a)
{
    if (a[1] != 0) return IntelStateCommon(a);
    if (IntelMaybeBlock(a, 0)) return 0;
    switch ((u32)IntelState(a)) {
    case 0:
        if (a[0x28]) {
            int *actor = ((LogicActorList *)&logicActorList)->At(a[0x28]);
            if (actor) a[0x26] = ActorValue(actor);
        }
        if (a[0x26]) { SimPrepareAction(modeCheckName, a[0x26]); a[0x22] = 1; }
        else a[0x22] = 8;
        break;
    case 1:
        if (!IntelUnitAllowed(a)) a[0x22] = 8;
        break;
    }
    return a[0x22] == 8;
}

int IntelStateSearch(int *a)
{
    int value;
    if (a[0x26] != 0) {
        int local[2];
        BestLocSetData((int)fallbackName, local, a[0x26]);
        value = LogicCopyPoint((const char *)local, ActorName(a));
        if (value <= a[0x27]) a[0x26] = 0;
    }
    if (a[0x26] == 0) {
        u32 divisor = IntelRadius();
        u32 distance = (u32)next_link_seed(&MissionRandomTable) % divisor;
        u32 direction = ((u32)next_link_seed(&MissionRandomTable) % 0x168) * 0xb60b61;
        int found = GridFindClosest((int)fallbackName, distance, direction);
        if (found != 0) {
            int local[2];
            BestLocSetData((int)fallbackName, local, found);
            IntelActorCommand(a, 2, 5, local);
            a[0x22] = 0;
        }
    }
    switch ((u32)IntelState(a)) {
    case 0:
        if (a[0x26]) { SimPrepareAction(modeCheckName, a[0x26]); a[0x22] = 1; }
        else a[0x22] = 8;
        break;
    case 1:
        if (!IntelUnitAllowed(a)) a[0x22] = 8;
        break;
    }
    return a[0x22] == 8;
}

int IntelStateMatch(int *a)
{
    if (a[1]) return IntelStateCommon(a);
    if (IntelMaybeBlock(a, 0)) return 0;
    if (!HercNeedsAmmo(*(int *)(a[0] + 0x18)) && IntelMaybeBlock(a, 1)) return 0;
    switch ((u32)IntelState(a)) {
    case 0: a[0x22] = 6; break;
    case 6:
        a[0x22] = 8;
        { int value = IntelFindBestByName((int)fallbackName, ActorValue(a));
          if (value) { SimPrepareAction(modeCheckName, value); if (IntelUnitAllowed(a)) a[0x22] = 1; } }
        if (a[0x22] == 8 && IntelMaybeBlock(a, 1)) return 0;
        break;
    case 1: if (!IntelUnitAllowed(a)) a[0x22] = 7; break;
    case 7: a[0x22] = 6; break;
    }
    return a[0x22] == 8;
}

int IntelStateRetry(int *a)
{
    switch ((u32)IntelState(a)) {
    case 0:
        if (a[0x28]) {
            int *actor = ((LogicActorList *)&logicActorList)->At(a[0x28]);
            if (actor) a[0x26] = ActorValue(actor);
        }
        if (a[0x26]) {
            if (!IntelPrepareState(a, a[0x26], a[0x27]) && !IntelPrepareState(a, a[0x26], a[0x27] + 8))
                IntelPrepareState(a, a[0x26], 0);
            a[0x22] = 1;
        } else {
            if (IntelPrepareState(a)) a[0x22] = 1;
            else a[0x22] = 8;
        }
        break;
    case 1: case 2:
        if (!IntelUnitAllowed(a)) {
            if (!a[1]) { a[0x22] = 8; return 1; }
            int value = 0;
            if (a[0x26]) {
                int point[2];
                BestLocSetData((int)fallbackName, point, a[0x26]);
                int distance = LogicCopyPoint((const char *)point, ActorName(a));
                if (distance > a[0x27] + 4) value = a[0x26];
                else value = IntelFindBest(a, a[0x26], a[0x27]);
                if (distance <= a[0x27] && a[0x22] == 2) value = 0;
            } else if (a[1]) value = IntelFindBest(a);
            a[0x22] = 3;
            if (value) { SimPrepareAction(modeCheckName, value); if (IntelUnitAllowed(a)) a[0x22] = 2; }
        } else if (a[1]) IntelResetOne(a, 1);
        break;
    case 3:
        if (!a[1] || IntelResetOne(a, 0)) a[0x22] = 8;
        break;
    }
    return a[0x22] == 8;
}

int IntelStateCommon(int *a)
{
    int state = a[0x22];
    int value;
    if (a[3] & 8) {
        if (IntelPrepareState(a) == 0) return IntelStateSearch(a);
        *(u8 *)(a + 3) &= 0xf7;
        a[0x22] = 1;
    }
    switch (a[0x22]) {
    case 0:
        a[0x22] = 8;
        if (a[0x28] != 0) {
            int *actor = ((LogicActorList *)&logicActorList)->At(a[0x28]);
            a[1] = (int)actor;
            if (a[1] && IntelUnitActive((int *)a[1])) a[0x22] = 1;
        }
        if (a[0x22] == 8 && IntelPrepareState(a) != 0) a[0x22] = 1;
        if (a[0x22] == 8 && IntelOwner(a) == gLocalActorKind) {
            *(u8 *)(a + 3) |= 8;
            a[0x22] = 0;
            return IntelStateSearch(a);
        }
        break;
    case 1:
        a[0x22] = 3;
        value = IntelFindBest(a);
        if (value == 0) break;
        SimPrepareAction(modeCheckName, value);
        if (IntelUnitAllowed(a)) a[0x22] = 2;
        break;
    case 2:
        if (!a[1]) a[0x22] = 8;
        else if (!IntelUnitAllowed(a)) {
            int score = LogicCopyPoint(ActorName(a), ActorName((int *)a[1]));
            if (score < a[0x1a] || score > a[0x1b]) a[0x22] = 1;
            else a[0x22] = 3;
        } else IntelResetOne(a, 1);
        break;
    case 3:
        if (IntelResetOne(a, 0)) a[0x22] = 8;
        break;
    }
    return a[0x22] == 8;
}

int IntelStateSeek(int *a)
{
    u32 state = a[0x22];
    int value;
    if ((int)state < 3) return IntelStateCommon(a);
    if (state <= 3) {
        if (state != 3) goto finish;
        if (a[1] == 0) { a[0x22] = 8; return 1; }
        if (IntelResetOne(a, 0) == 0) goto finish;
        a[0x22] = 8;
        if (a[1] == 0) return 1;
        value = LogicCopyPoint(ActorName((int *)a[1]), ActorName(a));
        if (a[0x1b] < value) goto finish;
        value = ActorValue((int *)a[1]);
        value = BestLocBegin(a, value);
        if (value == 0) goto finish;
        SimPrepareAction(modeCheckName, value);
        if (IntelUnitAllowed(a)) a[0x22] = 4;
        goto finish;
    }
    if (state > 4) {
        if (state != 5) goto finish;
        if (!IntelUnitAllowed(a)) {
            if (a[1] != 0) {
                int direction = PointAngle((int *)(a[0] + 0x32), (int *)(*(int *)a[1] + 0x32));
                int clamped = GridClamp(direction);
                IntelSetMode3(modeCheckName, clamped * 0x2aaaaabc, 0);
            }
            a[0x22] = 8;
        }
        goto finish;
    }
    if (IntelUnitAllowed(a) == 0) a[0x22] = 5;
finish:
    return a[0x22] == 8;
}

int IntelSpawnTimer(int *a)
{
    if (modeSwitch == 0) {
        if (--a[0x21] <= 0) {
            u8 *record = (u8 *)actorStateTable + a[8] * 0x2f;
            u8 rawActorType = record[9]; u32 count = record[11];
            int actorType = rawActorType - 8;
            while (--count != 0xffffffff) {
                int point = IntelChooseSpawnPos(&logicActorList, actorType, 0, 0, 0);
                int *actor = ((LogicActorList *)&logicActorList)->CreateActor(actorType,
                    (ActorPoint *)ActorName(a), point, 0);
                if (actor != 0) IntelFinalizeActor(actor);
            }
            a[0x21] = record[10];
        }
    }
    return 1;
}

int IntelStateLink(int *a)
{
    switch ((u32)IntelState(a)) {
    case 0:
        a[0x22] = 8;
        if (a[0x28]) {
            a[1] = (int)((LogicActorList *)&logicActorList)->At(a[0x28]);
            if (a[1] && IntelUnitActive((int *)a[1])) a[0x22] = 3;
        }
        if (a[0x22] == 8 && IntelPrepareState(a)) a[0x22] = 3;
        if (a[0x22] != 3) break;
    case 3:
        if (IntelResetOne(a, 0)) a[0x22] = 8;
        break;
    }
    return a[0x22] == 8;
}

// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
// reserved source line for original AssertFail immediates
void IntelActorCommand(int *a, int command, int value, int *target)
{
    a[0x25] = command;
    a[0x26] = 0;
    a[0x27] = 0;
    a[0x28] = 0;
    switch (a[0x25]) {
    case 0: break;
    case 1: if (target != 0) { int id = ResolveName(fallbackName, (const int *)target); a[0x26] = id; a[0x27] = 3; return; } break;
    case 2: if (target != 0) { int id = ResolveName(fallbackName, (const int *)target); a[0x26] = id; a[0x27] = value; return; } break;
    case 3: a[0x28] = value;
        // keep AssertFail source lines at their original immediates
        if (value != 0) {
            // assert source line follows the null lookup check
            // preserve the second original line boundary























































































            int *actor = ((LogicActorList *)&logicActorList)->At(value); if (actor == 0) AssertFail(__FILE__, __LINE__, 0);
            a[0x26] = ActorValue(actor);
            a[0x27] = 3;
            return;
        }
        break;
    default: AssertFail(__FILE__, __LINE__, "Unknown Target"); return;
    }
}

int IntelActorDispatch(int *a)
{
    int result;
    int kind;
    if (ActorIsAvailable(a) != 0) return 1;
    if (*(char *)(a[0] + 0x25) == 8) return 0;
    result = 1;
    if ((*(u8 *)(a + 3) & 2) == 0) {
        if (a[7] > 9 && a[7] < 17 && a[0x1f] != 0) {
            if ((u32)a[0x1f] < 2) result = IntelSpawnTimer(a);
            else if ((u32)a[0x1f] < 5) result = IntelStateLink(a);
        }
    } else {
        if (a[0x22] == 0) {
            kind = ActorArrayGet(a, 7);
            int count = 0;
            if (kind == 2) {
                count = IntelUnitQuery(*(int *)(a[0] + 0x18), 1, 1);
                if (count == 0) {
                    count = IntelUnitQuery(*(int *)(a[0] + 0x18), 1, 0);
                    if (count != 0) IntelSetMode3(modeCheckName, count - 0x13, 1);
                }
            } else {
                while (count < 4 && (kind = IntelUnitQuery(*(int *)(a[0] + 0x18), 1, 1)) != 0) {
                    IntelSetMode3(modeCheckName, kind - 0x13, 0);
                    ++count;
                }
            }
        }
        kind = ActorArrayGet(a, 7);
        switch (kind) {
        case 1: result = IntelStateApproach(a); break;
        case 2: result = IntelStateMatch(a); break;
        case 3: result = IntelStateRetry(a); break;
        case 4: case 5: case 6: case 9: case 10: result = IntelStateCommon(a); break;
        case 7: case 8: result = IntelStateSeek(a); break;
        case 11: IntelClearMode(modeCheckName); result = 1; break;
        case 12: result = IntelStateWait(a); break;
        case 13: result = IntelStateSearch(a); break;
        }
    }
    if (result == 0) IntelClearReservation(a, 0);
    return result;
}

int IntelIsAvailable(int *a)
{
    int result;
    int kind;
    int state;
    if (ActorIsAvailable(a) != 0) return 1;
    if (*(char *)(a[0] + 0x25) == 8) return 0;
    result = 1;
    if ((*(u8 *)(a + 3) & 2) == 0) {
        if (a[7] > 9 && a[7] < 17 && (u32)a[0x1f] > 1 && (u32)a[0x1f] < 5)
            result = IntelStateLink(a);
    } else {
        kind = ActorArrayGet(a, 7);
        if (kind <= 3) {
            if (kind == 3) result = IntelStateLink(a);
        } else if (kind < 7 || kind < 9 || kind < 11) {
            result = IntelStateLink(a);
        }
        if (result != 0 && a[7] == 1 && a[1] != 0) {
            int distance = LogicCopyPoint(ActorName(a), ActorName((int *)a[1]));
            if (distance <= a[0x1b]) {
                int mode = IntelUnitQuery(*(int *)(a[0] + 0x18), 4, 0);
                if (mode != 0) IntelSetMode3(modeCheckName, mode - 0x13, -1);
            }
        }
    }
    if (result == 0) IntelClearReservation(a, 0);
    return result;
}
