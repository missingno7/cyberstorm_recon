#pragma pack(push,1)
#include "actor.h"
typedef unsigned char u8; typedef unsigned long u32; struct ActorPoint { int x, y; }; struct ActorPilot { u8 pad[0x12]; int active; int Active() { return active > 0; } }; struct ApplyPoint { int x, y; }; extern int PointAngle(ApplyPoint *left, ApplyPoint *right);
struct LogicActor {
    void *unit;
    int field4; u8 pad8[4];
    u32 flags;
    const char *name;
    int index, next, type, value, timer;
    u8 pad28[0x70 - 0x28];
    ActorPoint position; int randomValue, state7c, linkedIndex; u8 pad84[4]; int state88, state8c; u8 pad90[0xa8 - 0x90];
    int Available() { ActorRecord *u = (ActorRecord *)unit; return u && u->active && u->pilot->Active() && !((ActorRecord *)unit)->hidden; } int Hostile() { return (flags & 1) != 0; } int Friendly() { return (flags & 1) == 0; } int Attached() { return unit != 0; } int Flag2() { return (flags & 2) != 0; } int Live() { return unit && ((ActorRecord *)unit)->active && ((ActorRecord *)unit)->pilot->Active(); } LogicActor() {} int IsUnit() { return type == 30; } int Unlinked() { return (flags & 2) == 0; } int Ready() { return unit && ((ActorRecord *)unit)->active && ((ActorRecord *)unit)->pilot->Active() && !((ActorRecord *)unit)->hidden; }
};
struct ActorGlobals { LogicActor actorRecords[512]; int actorCount, actorSerial; };
ActorGlobals logicActors;
extern int StreamRead(void *stream, void *buffer, int count), StreamWrite(void *stream, const void *buffer, int count);
extern int ActorRead(LogicActor *actor, void *stream), ActorWrite(LogicActor *actor, void *stream);
extern void AssertFail(const char *file, int line, const char *message);
static const char lactorsSourcePath[] = __FILE__; extern const char invalidActorTypeFormat[]; // AssertFail uses the embedded source path.
extern "C" char *BriefFormat(const char *format, ...); extern int gRemoteActorKind, gLocalActorKind, modeFlags, actorModeValue, currentActorValue;
extern unsigned int MissionRandomTable[];
extern unsigned int next_link_seed(void *table);
extern int GetModeValue(const char *name, int value), CompareActorNames(const char *left, const char *right); extern void UpdateLogicActor(LogicActor *actor), RemoveLogicActor(LogicActor *actor);
extern int ActorListIsBusy(); extern void ActorArraySet(LogicActor *actor, int array, int value);
extern int ActorArrayGet(LogicActor *actor, int array), ActorCanChoose(LogicActor *actor), ActorPercent(void *data), ActorValue(LogicActor *actor), ActorIsAvailable(LogicActor *actor), ActorLinkStrength(int value); extern const char *ActorName(LogicActor *actor); inline int ActorArmed(LogicActor *actor) { return ActorArrayGet(actor, 7) != 0; } extern void ActorSetMode(LogicActor *actor, int mode), SetModeForUnit(const char *name, void *unit); extern int f_00434bd0(const char *message, int actor, int flag, int start, int end, int percent); extern int *GetModePlayer(const char *name, int value); extern int LogicCopyPoint(const char *left, const char *right); extern int DirectionCompare(int *left, int *right), DirectionBlend(int *result, int kind, int direction), DirectionLength(int *direction), NeighborResponse(LogicActor *actor, const char *name); extern char modeCheckName[]; extern int g4d2498, g4dd0c0; extern char fallbackName[]; inline int LogicFlag8() { return (g4dd0c0 & 8) != 0; }
extern "C" void *memset(void *buffer, int value, unsigned int count);
class LogicActorList {
public:
    int Load(void *stream); int Save(void *stream);
    void Clear(); LogicActor *At(int index); LogicActor *First(); LogicActor *Next(LogicActor *actor);
    LogicActor *Reserve();
    LogicActor *CreateActor(int type, ActorPoint *position, int value, const char *name); LogicActor *FindByUnit(void *unit);
    LogicActor *SelectActor(LogicActor *actor); void ResetActorState(); void ResetActorValues(); int HasLocalMismatch(); int AllLocalActorsPresent(); int AllRemoteActorsPresent(); void UpdateActorPercentages(); void CalculateActorValues(int force); void NotifyActorLinks();
};

static inline LogicActor *ActorAt(int index) { if (index > 0 && index <= logicActors.actorCount) return logicActors.actorRecords + (index - 1); return 0; }
static inline LogicActor *ActorFirst() { if (logicActors.actorCount > 0) return logicActors.actorRecords; return 0; }
static inline LogicActor *ActorNext(LogicActor *actor) { if (actor->index + 1 <= logicActors.actorCount) return (LogicActor *)((char *)actor + 0xa8); return 0; }

int LogicActorList::Load(void *stream)
{
    logicActors.actorCount = 0;
    logicActors.actorSerial = 0;
    memset(logicActors.actorRecords, 0, 0x15000);
    if (StreamRead(stream, &logicActors.actorCount, 4) != 4)
        return 0;
    if (StreamRead(stream, &logicActors.actorSerial, 4) != 4)
        return 0;
    LogicActor *actor = logicActors.actorRecords;
    for (int i = 0; i < logicActors.actorCount; ++i, ++actor) {
        if (!ActorRead(actor, stream))
            return 0;
    }
    return 1;
}

int LogicActorList::Save(void *stream)
{
    if (StreamWrite(stream, &logicActors.actorCount, 4) != 4)
        return 0;
    if (StreamWrite(stream, &logicActors.actorSerial, 4) != 4)
        return 0;
    LogicActor *actor = logicActors.actorRecords;
    for (int i = 0; i < logicActors.actorCount; ++i, ++actor) {
        if (!ActorWrite(actor, stream))
            return 0;
    }
    return 1;
}

void LogicActorList::Clear()
{
    logicActors.actorCount = 0;
    logicActors.actorSerial = 0;
    memset(logicActors.actorRecords, 0, 0x15000);
}

LogicActor *LogicActorList::At(int index)
{
    if (index > 0 && index <= logicActors.actorCount)
        return logicActors.actorRecords + (index - 1);
    return 0;
}

LogicActor *LogicActorList::First()
{
    if (logicActors.actorCount > 0)
        return logicActors.actorRecords;
    return 0;
}

LogicActor *LogicActorList::Next(LogicActor *actor)
{
    if (actor->index + 1 <= logicActors.actorCount)
        return (LogicActor *)((char *)actor + 0xa8);
    return 0;
}
     
     
     
     
     
LogicActor *LogicActorList::Reserve() { if (logicActors.actorCount >= 512) AssertFail(lactorsSourcePath, __LINE__, 0); LogicActor *actor = logicActors.actorRecords + logicActors.actorCount; actor->index = ++logicActors.actorCount; return actor; }








LogicActor *LogicActorList::CreateActor(int type, ActorPoint *position, int value, const char *name)
{
    if (type <= 0 || type >= 32) AssertFail(lactorsSourcePath, __LINE__, BriefFormat("Invalid Actor Type (%d)", type));
    if (name == 0) {
        if (type >= 17) name = (const char *)gLocalActorKind;
        else name = (const char *)gRemoteActorKind;
    } else {
        if (type >= 17) {
            if (name == (const char *)gRemoteActorKind) {
            //
            //
            //
            //
            //
            AssertFail(lactorsSourcePath, __LINE__, 0);
            }
        } else if (name != (const char *)gRemoteActorKind) AssertFail(lactorsSourcePath, __LINE__, 0);
    }
    if (type >= 1 && type <= 9 && ActorListIsBusy()) return 0;
    LogicActor *actor = Reserve();
    actor->name = name;
    
    ActorArraySet(actor, 0, actor->index);
    actor->type = type;
    actor->value = value;
    actor->flags = 0;
        if (type >= 17) *((u8 *)&actor->flags) |= 1;
    if ((type >= 1 && type <= 9) || (type >= 17 && type <= 24)) *((u8 *)&actor->flags) |= 2;
    actor->timer = currentActorValue;
    ActorArraySet(actor, 1, 0);
    actor->position = *position;
    actor->randomValue = (next_link_seed(&MissionRandomTable) % 6) * 0x2aaaaabc;
    return actor;
}

LogicActor *LogicActorList::FindByUnit(void *unit)
{
    LogicActor *actor;
    if (logicActors.actorCount > 0) actor = logicActors.actorRecords;
    else actor = 0;
    while (actor != 0) {
        int hasUnit = actor->unit != 0;
        if (hasUnit && unit == actor->unit) break;
        if (actor->index + 1 <= logicActors.actorCount) ++actor;
        else actor = 0;
    }
    return actor;
}

extern void ActorRandomAction(LogicActor *actor);
extern void ActorFinalize(LogicActor *actor);
extern int f_00439d48(int data);
extern int ActorDirectionCompare(int *left, int *right);
extern int ActorDirectionBlend(int *result, int kind, int direction);
extern int ActorDirectionLength(int *direction);
extern int ActorNeighborResponse(LogicActor *actor, const char *name);
extern void IntelActorEvent(LogicActor *actor);
extern int ActorModeType(int type);
extern unsigned char *targetDefinitionTable;

LogicActor *LogicActorList::SelectActor(LogicActor *actor)  {
    if (actor != 0)  {
        if (LogicFlag8() && actor->Flag2() && next_link_seed(&MissionRandomTable) % 100 < 25) ActorRandomAction(actor);
        actor = ActorAt(actor->next);
    }
    else  {
        actor = ActorFirst();
        while (actor != 0)  {
            actor->next = 0;
            actor = ActorNext(actor);
        }
        LogicActor *first = actor;
        LogicActor *previous = 0;
        for (;;)  {
            LogicActor *best = 0;
            int bestScore;
            LogicActor *scan = ActorFirst();
            while (scan != 0)  {
                if (scan->Ready() && scan->next == 0)  {
                    if (scan->Friendly() || ActorArmed(scan) || *GetModePlayer(modeCheckName, (int)scan->name) == 0)  {
                        int score = 0;
                        if (previous != 0) score = -LogicCopyPoint(ActorName(previous), ActorName(scan));
                        if (scan->Friendly()) score -= 1000;
                        if (scan->Friendly() && ((ActorRecord *)scan->unit)->unit->slots[0].item == 9) score -= 1000;
                        if (best == 0 || score > bestScore)  {
                            best = scan;
                            bestScore = score;
                        }
                    }
                }
                scan = ActorNext(scan);
            }
            if (best == 0) break;
            if (first == 0) first = best;
            if (previous != 0) previous->next = best->index;
            previous = best;
            best->next = -1;
        }
        if (previous != 0) previous->next = 0;
        actor = first;
    }
    g4d2498 = 0;
    if (actor != 0)  {
        SetModeForUnit(modeCheckName, actor->unit);
        if (actor->Flag2()) ActorFinalize(actor);
    }
    return actor;
}

extern "C" int LogicDeleteFromMap(void *unused, int index)
{
    LogicActor *actor;
    if (index) {
        actor = ActorAt(index);
        if (actor != 0) actor = ActorNext(actor);
    } else {
        actor = ActorFirst();
    }
    while (actor) {
        if (actor->Live() && actor->Friendly() && actor->Flag2()) return actor->index;
        actor = ActorNext(actor);
    }
    return 0;
}

extern "C" int LogicRefreshMap(void *unused, int index)
{
    LogicActor *actor;
    if (index) {
        actor = ActorAt(index);
        if (actor != 0) actor = ActorNext(actor);
    } else {
        actor = ActorFirst();
    }
    while (actor) {
        if (actor->Live() && actor->Hostile() && actor->Flag2()) return actor->index;
        actor = ActorNext(actor);
    }
    return 0;
}


void LogicActorList::ResetActorState()
{
    for (LogicActor *actor = ActorFirst(); actor != 0; actor = ActorNext(actor)) {
        actor->state88 = 0;
        actor->state8c = 0;
        actor->field4 = 0;
        *((u8 *)&actor->flags) &= (u8)~8;
        if (actor->IsUnit() && actor->Ready() && ActorArrayGet(actor, 7) == 11) {
            SetModeForUnit(modeCheckName, actor->unit);
            IntelActorEvent(actor);
        }
    }
    CalculateActorValues(1);
}

void LogicActorList::ResetActorValues()
{
    for (LogicActor *actor = ActorFirst(); actor != 0; actor = ActorNext(actor)) {
        ActorArraySet(actor, 9, 0);
    }
    CalculateActorValues(0);
}

int LogicActorList::HasLocalMismatch()
{
    for (LogicActor *actor = ActorFirst(); actor != 0; actor = ActorNext(actor)) {
        if (ActorCanChoose(actor)) {
            int kind = actor->unit != 0 ? *((int *)actor->unit + 2) : 0;
            if (kind == gLocalActorKind) return kind ^ gLocalActorKind;
        }
    }
    return 1;
}

int LogicActorList::AllLocalActorsPresent()
{
    for (LogicActor *actor = ActorFirst(); actor != 0; actor = ActorNext(actor)) {
        if (ActorCanChoose(actor)) {
            int kind = actor->unit != 0 ? *((int *)actor->unit + 2) : 0;
            if (kind != gLocalActorKind) return 0;
        }
    }
    return 1;
}

int LogicActorList::AllRemoteActorsPresent()
{
    for (LogicActor *actor = ActorFirst(); actor != 0; actor = ActorNext(actor)) {
        if (ActorCanChoose(actor)) {
            int kind = actor->unit != 0 ? *((int *)actor->unit + 2) : 0;
            if (kind != gRemoteActorKind) return 0;
        }
    }
    return 1;
}

static inline int ClampPercent(int p) { if (p <= 1) return 1; if (p >= 100) return 100; return p; }

void LogicActorList::UpdateActorPercentages()
{
    for (LogicActor *actor = ActorFirst(); actor != 0; actor = ActorNext(actor)) {
        int percent = 100;
        if (actor->Attached()) {
            if (ActorIsAvailable(actor)) percent = 0;
            else {
percent = ClampPercent(f_00439d48((int)((ActorRecord *)actor->unit)->unit));
            }
        }
        ActorArraySet(actor, 5, percent);
    }
}

inline void AddDirection(int *average, int kind, int direction) { int contribution[2]; DirectionBlend(contribution, kind, direction); average[0] += contribution[0]; average[1] += contribution[1]; }
void LogicActorList::CalculateActorValues(int force)
{
    for (LogicActor *actor = ActorFirst(); actor != 0; actor = ActorNext(actor)) {
        if (actor->Ready() && (actor->Friendly() || ActorArmed(actor))) {
            if (force) ActorSetMode(actor, -0x7fffffcc);
            else {
                int average[2];
                average[0] = 0; average[1] = 0;
                int count = 0;
                LogicActor *other = ActorFirst();
                while (other != 0) {
                    if (other->Available() && actor->name != other->name) {
                        int direction = PointAngle((ApplyPoint *)((char *)actor->unit + 0x32), (ApplyPoint *)((char *)other->unit + 0x32));
                        int response = NeighborResponse(other, ActorName(actor));
                        AddDirection(average, response, direction);
                        ++count;
                    }
                    other = ActorNext(other);
                }
                int length = 0;
                if (count > 0) {
                    average[0] /= count;
                    average[1] /= count;
                    length = DirectionLength(average);
                }
                ActorSetMode(actor, -0x7fffffcc - (length - *((int *)((char *)actor->unit + 0x46))));
            }
        }
    }
}

void LogicActorList::NotifyActorLinks()
{
    LogicActor *actor = ActorFirst();
    while (actor != 0) {
        if (actor->Live() && actor->Unlinked() && (actor->Hostile() || actor->state7c == 0)) {
            int mode = targetDefinitionTable[1 + ActorValue(actor) * 0xc];
            LogicActor *linked = actor;
            while (linked != 0) {
                int kind = ActorValue(linked);
                int strength = ActorLinkStrength(linked->value);
                f_00434bd0(fallbackName, kind, mode, strength + 1, strength + 3, 100);
                linked = ActorAt(linked->linkedIndex);
            }
        }
        actor = ActorNext(actor);
    }
}







