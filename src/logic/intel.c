typedef unsigned char u8;
typedef unsigned int u32;
extern char modeCheckName[], fallbackName[], actorStateTable[]; extern int logicActorList;
// AssertFail gets the path from the compiler's source-name literal.
extern int gLocalActorKind, modeSwitch, g4d359c; extern char actorNameOrigin[];
extern int MissionRandomTable[];
extern void AssertFail(const char *, int, const char *);
extern int LogicCopyPoint(const char *, const char *), GridClamp(int), GridFindClosest(int, u32, u32);
extern const char *ActorName(int *);
extern int ActorValue(int *), ActorArrayGet(int *, int), ActorIsAvailable(int *);
extern int ActorCanChoose(int *), ActorPercent(int *), BestLocRun(int *), BestLocBegin(int *, int);
extern int next_link_seed(void *), ResolveName(const char *, const int *);
extern int *ActorListFind(void *, int);
extern int *IntelChooseCandidate(int *, int); extern int IntelUpdate(int *, int);
extern int IntelFindBest(int *, int, int), IntelFindBestByName(int, int);
extern int IntelResetOne(int *, int), IntelClearReservation(int *, int);
extern int IntelSetMode(const char *, int), IntelSetMode3(const char *, int, int), IntelClearMode(const char *);
extern int IntelUnitQuery(int, int, int), IntelUnitState(int);
extern int IntelPrepareState(int *, int, int), IntelContinueState(int *, int, int);
struct ActorPoint { int x, y; }; class LogicActorList { public: int *CreateActor(int, ActorPoint *, int, const char *); };
struct IntelUnitState { u8 pad[5]; u8 type; }; struct IntelUnit { u8 pad[0x20]; struct IntelUnitState info; };
extern int IntelTargetData(int, int *, int), IntelGetActorState(int *), IntelChooseSpawnPos(void *, int, int, int, int), IntelFinalizeActor(int *);
extern int PointAngle(int *, int *);
extern void BestLocSetData(int, int *, int);
int IntelStateCommon(int *a);
void IntelActorCommand(int *a, int command, int value, int *target);

static inline int IntelState(int *a) { return a[0x22]; }
static inline char IntelUnitAllowed(int *a)
{
    int unit = a[0];
    if (unit == 0) return 0;
    u8 *unitData = (u8 *)unit + 0x20;
    if (unitData[5] == 1 || unitData[5] == 7 || unitData[5] == 14 ||
        unitData[5] == 17 || unitData[5] == 18) return 0;
    return 1;
}
static inline int IntelUnitActive(int *unit)
{
    int *data;
    if (unit == 0 || *(char *)(unit[0] + 0x88) == 0) return 0;
    data = *(int **)(unit[0] + 0x1c);
    if (*(int *)((char *)data + 0x12) < 1 || *(char *)(unit[0] + 0x89) != 0) return 0;
    return 1;
}

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
    u32 state = (u32)a[0x22];
    int allowed;
    int value;
    if (state == 0) goto state0;
    if (state <= 1) goto state1;
    if (state == 3) goto state3;
    goto finish;
state1:
    allowed = IntelUnitAllowed(a);
    if (allowed) {
        if (a[1] != 0) IntelResetOne(a, 1);
        goto finish;
    }
    a[0x22] = 3;
    value = LogicCopyPoint(ActorName(a), actorNameOrigin);
    if (value >= g4d359c) goto finish;
    value = BestLocRun(a);
    if (value == 0) goto finish;
    IntelSetMode(modeCheckName, value);
    if (IntelUnitAllowed(a)) a[0x22] = 1;
    goto finish;
state3:
    if (a[1] == 0 || IntelResetOne(a, 0) != 0) a[0x22] = 8;
    goto finish;
state0:
    a[0x22] = 1;
    IntelChooseCandidate(a, 0);
finish:
    if (a[0x22] == 8 && LogicCopyPoint(ActorName(a), actorNameOrigin) == g4d359c)
        IntelClearMode(modeCheckName);
    return a[0x22] == 8;
}

int IntelMaybeBlock(int *a, int allow)
{
    if (IntelChooseCandidate(a, 0) == 0) return 0;
    int score = LogicCopyPoint(ActorName(a), ActorName((int *)a[1]));
    if (score > 8 && allow == 0) return 0;
    a[0x22] = 1;
    IntelUnit *unit = (IntelUnit *)a[0];
    if (unit != 0) {
        char unitState = unit->info.type;
        if (unitState != 1 && unitState != 7 && unitState != 14 &&
            unitState != 17 && unitState != 18)
            *(u8 *)(a[0] + 0x56) = 3;
    }
    return 1;
}

int IntelStateApproach(int *a)
{
    if (a[1] != 0) return IntelStateCommon(a);
    if (IntelChooseCandidate(a, 0) != 0) {
        int score = LogicCopyPoint(ActorName(a), ActorName((int *)a[1]));
        if (score > 8) a[1] = 0;
        else {
            a[0x22] = 1;
            if (IntelUnitAllowed(a)) *(u8 *)(a[0] + 0x56) = 3;
            return 0;
        }
    }
    if (a[0x22] == 0) {
        int *actor = 0;
        if (a[0x28] != 0) actor = ActorListFind((void *)&logicActorList, a[0x28]);
        if (actor != 0) a[0x26] = ActorValue(actor);
        if (a[0x26] != 0) {
            IntelSetMode(modeCheckName, a[0x26]);
            a[0x22] = 1;
            goto finish;
        }
    } else {
        if (a[0x22] != 1) goto finish;
        if (IntelUnitAllowed(a)) goto finish;
    }
    a[0x22] = 8;
finish:
    return a[0x22] == 8;
}

int IntelStateSearch(int *a)
{
    int value;
    if (a[0x26] != 0) {
        int local[2];
        BestLocSetData(0x4d3594, local, a[0x26]);
        value = LogicCopyPoint((const char *)local, ActorName(a));
        if (value <= a[0x27]) a[0x26] = 0;
    }
    u32 divisor = g4d359c; if (a[0x26] == 0) {
        u32 first = next_link_seed(&MissionRandomTable);
        u32 second = next_link_seed(&MissionRandomTable);
        int found = GridFindClosest(0x4d3594, first % divisor, (second % 0x168) * 0xb60b61);
        if (found != 0) {
            int local[2];
            BestLocSetData(0x4d3594, local, found);
            IntelActorCommand(a, 2, 5, local);
            a[0x22] = 0;
        }
    }
    if (a[0x22] == 0) {
        if (a[0x26] != 0) {
            IntelSetMode(modeCheckName, a[0x26]);
            a[0x22] = 1;
            goto finish;
        }
    } else if (a[0x22] != 1) goto finish;
    else if (IntelUnitAllowed(a)) goto finish;
    a[0x22] = 8;
finish:
    return a[0x22] == 8;
}

int IntelStateMatch(int *a)
{
    int state = a[0x22];
    int value;
    int *candidate;
    if (a[1] != 0) return IntelStateCommon(a);
    candidate = IntelChooseCandidate(a, 0);
    if (candidate != 0) {
        value = LogicCopyPoint(ActorName(a), ActorName((int *)a[1]));
        if (value > 8) a[1] = 0;
        else {
            a[0x22] = 1;
            if (IntelUnitAllowed(a)) *(u8 *)(a[0] + 0x56) = 3;
            return 0;
        }
    }
    value = IntelUnitState(*(int *)(a[0] + 0x18));
    if (value == 0) {
        candidate = IntelChooseCandidate(a, 0);
        if (candidate != 0) {
            LogicCopyPoint(ActorName(a), ActorName((int *)a[1]));
            a[0x22] = 1;
            if (IntelUnitAllowed(a)) *(u8 *)(a[0] + 0x56) = 3;
            return 0;
        }
    }
    state = a[0x22];
    if (state == 0) {
        a[0x22] = 6;
        goto finish;
    }
    if (state > 1) {
        if (state <= 5) goto finish;
        if (state > 6) {
            if (state == 7) a[0x22] = 6;
            goto finish;
        }
        a[0x22] = 8;
        value = ActorValue(a);
        value = IntelFindBestByName(0x4d3594, value);
        if (value != 0) {
            IntelSetMode(modeCheckName, value);
            if (IntelUnitAllowed(a)) a[0x22] = 1;
        }
        if (a[0x22] != 8) goto finish;
        candidate = IntelChooseCandidate(a, 0);
        if (candidate != 0) {
            LogicCopyPoint(ActorName(a), ActorName((int *)a[1]));
            a[0x22] = 1;
            if (IntelUnitAllowed(a)) *(u8 *)(a[0] + 0x56) = 3;
            return 0;
        }
        goto finish;
    }
    if (!IntelUnitAllowed(a)) a[0x22] = 7;
finish:
    return a[0x22] == 8;
}

int IntelStateRetry(int *a)
{
    int state = a[0x22];
    int value = 0;
    int local[2];
    if (state == 0) {
        int *actor = 0;
        if (a[0x28] != 0) actor = ActorListFind((void *)&logicActorList, a[0x28]);
        if (actor != 0) a[0x26] = ActorValue(actor);
        if (a[0x26] == 0) {
            if (IntelPrepareState(a, 0, 0) == 0) goto fail;
        } else if (IntelPrepareState(a, a[0x26], a[0x27]) == 0 &&
                   IntelPrepareState(a, a[0x26], a[0x27] + 8) == 0)
            IntelPrepareState(a, a[0x26], 0);
        a[0x22] = 1;
        goto finish;
    }
    if (state >= 3) {
        if (state != 3 || (a[1] != 0 && IntelResetOne(a, 0) == 0)) goto finish;
        a[0x22] = 8;
        goto finish;
    }
    if (IntelUnitAllowed(a)) {
        if (a[1] != 0) IntelResetOne(a, 1);
        goto finish;
    }
    if (a[1] == 0) {
        a[0x22] = 8;
        return 1;
    }
    if (a[0x26] == 0) {
        if (a[1] != 0) value = IntelFindBest(a, 0, 0);
    } else {
        BestLocSetData(0x4d3594, local, a[0x26]);
        int score = LogicCopyPoint((const char *)local, ActorName(a));
        if (a[0x27] + 4 < score) value = a[0x26];
        else value = IntelFindBest(a, a[0x26], a[0x27]);
        if (score <= a[0x27] && a[0x22] == 2) value = 0;
    }
    a[0x22] = 3;
    if (value != 0) {
        IntelSetMode(modeCheckName, value);
        if (IntelUnitAllowed(a)) a[0x22] = 2;
    }
finish:
    return a[0x22] == 8;
fail:
    a[0x22] = 8;
    goto finish;
}

int IntelStateCommon(int *a)
{
    int state = a[0x22];
    int value;
    if (a[3] & 8) {
        if (IntelPrepareState(a, 0, 0) == 0) return IntelStateSearch(a);
        *(u8 *)(a + 3) &= 0xf7;
        a[0x22] = 1;
    }
    switch (a[0x22]) {
    case 0:
        a[0x22] = 8;
        if (a[0x28] != 0) {
            int *actor = ActorListFind((void *)&logicActorList, a[0x28]);
            a[1] = (int)actor;
            if (actor != 0 && IntelUnitActive(actor)) a[0x22] = 1;
        }
        if (a[0x22] == 8 && IntelPrepareState(a, 0, 0) != 0) a[0x22] = 1;
        if (a[0x22] == 8 && (a[0] != 0 ? *(int *)(a[0] + 8) : 0) == gLocalActorKind) {
            *(u8 *)(a + 3) |= 8;
            a[0x22] = 0;
            return IntelStateSearch(a);
        }
        break;
    case 1:
        a[0x22] = 3;
        value = IntelFindBest(a, 0, 0);
        if (value == 0) break;
        IntelSetMode(modeCheckName, value);
        if (IntelUnitAllowed(a)) a[0x22] = 2;
        break;
    case 2:
        if (a[1] != 0) {
            if (IntelUnitAllowed(a)) IntelResetOne(a, 1);
            else {
                int score = LogicCopyPoint(ActorName(a), ActorName((int *)a[1]));
                if (score < a[0x1a] || a[0x1b] < score) a[0x22] = 1;
                else a[0x22] = 3;
            }
            break;
        }
        goto state3;
    case 3:
        if (IntelResetOne(a, 0) == 0) break;
state3:
        a[0x22] = 8;
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
        IntelSetMode(modeCheckName, value);
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
    int state = a[0x22];
    int value;
    if (state == 0) {
        a[0x22] = 8;
        if (a[0x28] != 0) {
            int *actor = ActorListFind((void *)&logicActorList, a[0x28]);
            a[1] = (int)actor;
            if (actor != 0 && IntelUnitActive(actor)) a[0x22] = 3;
        }
        if (a[0x22] == 8 && IntelPrepareState(a, 0, 0) != 0) a[0x22] = 3;
        state = a[0x22];
    }
    if (state == 3 && IntelResetOne(a, 0) != 0) a[0x22] = 8;
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
            int *actor = ActorListFind((void *)&logicActorList, value); if (actor == 0) AssertFail(__FILE__, __LINE__, 0);
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
