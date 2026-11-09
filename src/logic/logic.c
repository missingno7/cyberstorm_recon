#include <string.h>
#pragma inline_depth(10)

class FileRecord;
struct LogicActor;
struct LogicActorList {
    void Clear(); int Load(FileRecord *), Save(FileRecord *);
    LogicActor *First(), *Next(LogicActor *), *SelectActor(LogicActor *), *FindByUnit(void *);
    int AllLocalActorsPresent();
    int AllRemoteActorsPresent(), HasLocalMismatch();
    void UpdateActorPercentages();
    void NotifyActorLinks(), ResetActorValues(), ResetActorState();
};

struct Script {
    unsigned char header[0x38];
    unsigned long workArea[0x1a6]; unsigned long *Data() { return workArea; }
};
extern LogicActorList logicActorList;
extern Script gScript;
extern int DAT_004dd168;

unsigned long StreamRead(FileRecord *, void *, unsigned long);
unsigned long StreamWrite(FileRecord *, const void *, unsigned long);
int f_0046406c(void *, void *);
int f_004640f6(void *, void *);
int ScriptRestore(unsigned long *, FileRecord *);
int ScriptSave(unsigned long *, FileRecord *);
void LogicTextBegin(void *);
void ScriptLoad(unsigned long *, int, int);
void f_0046799a(unsigned long *);
void f_004654cc(void *);
void f_0046415f(void);
void TerrainBuildHeights(const char *, void *, int, void *);
void LogicReset(void *);
void f_0046a97f(void *);
void TerrainBuildFeatures(const char *, void *, int, int);
void f_0047c320(const char *);
void f_0047c300(const char *);
void f_0046b03a(void *);
void LogicTextBegin(void *);
void LogicCheckMode(void *);
int ScriptRun(unsigned long *);
void LogicGenSpawnActor(void *);
int f_0044eb00(void *);
void f_00451370(void *);
void f_00452950(void *, void *, void *, int);
void *f_0045d830(void *, void *, int, int);
void *f_00463c56(void *);
char *ActorName(LogicActor *); char *ActorGetEvent(LogicActor *);
void ActorRelease(LogicActor *);
extern "C" int LogicCopyPoint(int *, int *);
int f_004771aa(const char *);
char *AllocateMissionImage(void *, int, int);
extern int DAT_00511e38;
extern int modeSwitch;
extern int DAT_004d3551;
extern int DAT_004d35ac;
extern int DAT_004d35c0;
extern int DAT_004d2de6;
extern char DAT_0048b5ac[];
extern char DAT_0048b5ae[];
extern char DAT_0048b5b2[];
extern char DAT_0048b5b4[];
extern int DAT_004fb944[];
extern char g511ddc[];
extern char DAT_004a3a8c[];
extern void *gLogicRandomTable;
extern char gLogicTextMap[];
extern char gLogicPoolName[];
extern char gLogicBackground[];
extern int gLogicModeSwitch;
extern void *gLogicCurrentOwner;
extern int DAT_004dd0b8;
extern int DAT_004f65e8;
extern int DAT_004dd0b4;

struct LogicHealth { int Alive() { return *(int *)((char *)this+0x12)>0; } }; struct LogicUnit { LogicHealth *Health() { return *(LogicHealth **)((char *)this+0x1c); }  }; struct LogicActor { LogicUnit *Unit() { return *(LogicUnit **)this; } int Available() { LogicUnit *u=Unit(); return u && ((unsigned char *)u)[0x88] && u->Health()->Alive() && !((unsigned char *)Unit())[0x89]; } int Owner() { LogicUnit *u=Unit(); if (u) return *(int *)((char *)u+8); return 0; } int Kind30() { return ((int *)this)[7]==30; } int Flag2Clear() { return ((((unsigned char *)this)[0xc])&2)==0; } int Attached() { return *(void **)this != 0; } int Value() { return ((int *)this)[5]; } int Friendly() { return (((unsigned char *)this)[0xc] & 1) == 0; } int Enemy() { return (((unsigned char *)this)[0xc] & 1) != 0; } };
class StrPool { public: int Lock(); void Unlock(); int IsOpen() { return DAT_00511e38 != 0; } };
extern StrPool missionImageName;
extern char fallbackName[]; extern char g4c9234[]; int LogicStoreActorLink(void *, void *); static inline int DefaultActorLink() { return DAT_004d35ac; }
extern char gShellBackground[];
extern void AssertFail(const char *, int, const char *);
#define ASSERT(test) if (!(test)) AssertFail(__FILE__, __LINE__, 0)

int LogicReadActorRecord(void *record, FileRecord *stream)
{
    if (StreamRead(stream, record, 0xa8) != 0xa8)
        return 0;
    if (!logicActorList.Load(stream)) return 0;
    if (!ScriptRestore((unsigned long *)&gScript, stream)) return 0;
    DAT_004dd168 = 0;
    LogicTextBegin(record);
    return 1;
}

int LogicWriteActorRecord(const void *record, FileRecord *stream)
{
    if (StreamWrite(stream, record, 0xa8) != 0xa8)
        return 0;
    if (logicActorList.Save(stream) == 0)
        return 0;
    if (ScriptSave((unsigned long *)&gScript, stream) == 0)
        return 0;
    return 1;
}

void LogicLoadActorState(unsigned long *record, const unsigned long *source,
                         unsigned long *scriptCopy)
{
    struct HeaderCopy { unsigned long words[0x16]; };
    struct ScriptCopy { unsigned long words[0x1a6]; };
    *(HeaderCopy *)(record + 1) = *(const HeaderCopy *)source;
    ScriptLoad((unsigned long *)&gScript, record[8], record[6]);
    *(ScriptCopy *)scriptCopy = *(ScriptCopy *)gScript.workArea;
    f_0046799a((unsigned long *)&gScript);
}

void *LogicGetScriptData(void)
{
    return gScript.Data() + 2;
}

void *LogicActorBuild(void *record, const unsigned long *source)
{
    struct RecordHeader { unsigned long words[0x16]; };
    *(RecordHeader *)((char *)record + 4) = *(const RecordHeader *)source;
    int acquiredTextPool = !missionImageName.IsOpen();
    if (acquiredTextPool)
        missionImageName.Lock();
    f_004654cc((char *)record + 4);
    logicActorList.Clear();
    *(unsigned long *)record = 0;
    memset((char *)record + 0x84, 0, 0x24);
    memset((char *)record + 0x5c, 0, 0x28);
    *(unsigned long *)((char *)record + 0x64) = 0xba1;
    *(unsigned long *)((char *)record + 0x74) = 10;
    LogicTextBegin(record);
    ScriptLoad((unsigned long *)&gScript,
               *(unsigned long *)((char *)record + 0x20),
               *(unsigned long *)((char *)record + 0x18));
    TerrainBuildHeights(fallbackName, g4c9234,
               *(int *)((char *)record + 0x18), gShellBackground);
    LogicReset(record);
    logicActorList.UpdateActorPercentages();
    if (ScriptRun((unsigned long *)&gScript) == 0) {
        *(unsigned char *)record |= 1;
        LogicCheckMode(record);
    }
    LogicActor *actor = logicActorList.First();
    while (actor) {
        if (!actor->Attached()) LogicGenSpawnActor(actor);
        actor = logicActorList.Next(actor);
    }
    if (modeSwitch == 0)
        logicActorList.NotifyActorLinks();
    TerrainBuildFeatures(fallbackName, g4c9234,
               *(int *)((char *)record + 0x18),
               *(int *)((char *)record + 0x24));
    *(int *)((char *)record + 0x68) = DAT_004d35c0;
    if (*(int *)((char *)record + 0x84) == 0)
        LogicStoreActorLink(record, (void *)DefaultActorLink());
    if (acquiredTextPool)
        missionImageName.Unlock();
    return (char *)record + 0x84;
}

int LogicStoreActorLink(void *object, void *link)
{
    int index;
    if (link != 0) {
        for (index = 0; index < 8 && *(int *)((char *)object + 0x84 + index * 4) != 0; ++index) {
        }
        if (index < 8) {
            *(void **)((char *)object + 0x84 + index * 4) = link;
            return 1;
        }
    }
    return 0;
}

int LogicCheckActorCollision(unsigned char *mission, int unused)
{
    if (DAT_004d3551 != 0)
        return 0;
    LogicActor *candidateActors[32];
    int candidateCount = DAT_004d3551;
    if (!logicActorList.AllLocalActorsPresent()) {
        LogicActor *entry = logicActorList.First(); while (entry != 0) {
            if (entry->Available() && entry->Enemy()) {
                if (entry->Owner() == DAT_004d2de6 && !entry->Flag2Clear()) {
                    int foundCloseName = 0;
                    int foundSimilarName = 0;
                    int foundCandidateKind = 0;
                    int foundOtherUnit = 0;
                    LogicActor *other = logicActorList.First(); while (other != 0) {
                        if (other->Available()) {
                            if (other->Kind30()) {
                                foundCandidateKind = 1;
                                if (LogicCopyPoint((int *)ActorName(entry),
                                                   (int *)ActorName(other)) <= 6)
                                    foundCloseName = 1;
                            } else if (other->Friendly()) {
                                foundOtherUnit = 1;
                                if (LogicCopyPoint((int *)ActorName(entry),
                                                   (int *)ActorName(other)) <= 0x10)
                                    foundSimilarName = 1;
                            }
                        }
                        other = logicActorList.Next(other);
                    }
                    if ((!foundCloseName && foundSimilarName) ||
                        (!foundCandidateKind && foundOtherUnit)) {
                        if (candidateCount > 0x1f)
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
                            ASSERT(candidateCount <= 0x1f);
                        candidateActors[candidateCount++] = entry;
                    }
                }
            }
            entry = logicActorList.Next(entry);
        }
    }
    if (candidateCount == 0) {
        if ((mission[0] & 0x10) == 0) {
            char *prompt = AllocateMissionImage(g511ddc, 0xf3, 0);
            if (f_004771aa(prompt) == 0)
                return 0;
        }
    } else {
        char *prompt = AllocateMissionImage(g511ddc, 0xf6, 0);
        strcpy((char *)DAT_004fb944, prompt);
        strcat((char *)DAT_004fb944, (char *)DAT_0048b5ac);
        for (int i = 0; i < candidateCount; ++i) {
            strcat((char *)DAT_004fb944, (char *)DAT_0048b5ae);
            strcat((char *)DAT_004fb944, ActorGetEvent(candidateActors[i]));
            strcat((char *)DAT_004fb944, (char *)DAT_0048b5b2);
        }
        strcat((char *)DAT_004fb944, (char *)DAT_0048b5b4);
        prompt = AllocateMissionImage(g511ddc, 0xf3, 0);
        strcat((char *)DAT_004fb944, prompt);
        if (f_004771aa((char *)DAT_004fb944) == 0)
            return 0;
        for (int releaseIndex = 0; releaseIndex < candidateCount; ++releaseIndex)
            ActorRelease(candidateActors[releaseIndex]);
    }
    return 1;
}

extern int DAT_004d3551, DAT_004d2dfa, DAT_004d271a, DAT_004d3549;
extern int DAT_004dd168, DAT_004dd0bc, modeSwitch, DAT_004d35bc; static inline int MissionPromptEnabled() { return DAT_004d3549 == 0; }
extern int DAT_004d3600, DAT_004d35c0;
extern int MissionRandomTable[];
extern int modeSwitch;
extern char modeCheckName[];
extern char g511ddc[];
void ScriptAbort(Script *);
void ScriptTick(Script *);
void LogicReset(void *);
int next_link_seed(unsigned long *);
int IntelActorDispatch(int *);
int IntelIsAvailable(int *);
int ActorIsAvailable(LogicActor *);
int ActorStateCall(LogicActor *);
void ActorArraySet(LogicActor *, int, int);
void SimHotbriefRefresh(void *);
void SimDelayPhase(void *);

static inline unsigned LogicRandomBelow(unsigned divisor) { return (unsigned)next_link_seed((unsigned long *)MissionRandomTable) % divisor; }

void LogicCheckActorOrder(unsigned char *mission, unsigned int order)
{
    unsigned int value;
    if (DAT_004d3551 != 0)
        return;
    if (order == 0 && LogicCheckActorCollision(mission, 0) == 0)
        return;
    ScriptAbort(&gScript);
    *mission |= 1;
    LogicCheckMode(mission);
    if (*(int *)(mission + 0x5c) == 0) {
        *(int *)(mission + 0x5c) = 3;
    }
    switch (order) {
    case 1:
        *(int *)(mission + 0x5c) = order;
        *(int *)(mission + 0x60) = 0x582;
        *(int *)(mission + 0x6c) = 0x87;
        *(int *)(mission + 0x68) = 0xb4;
        *(int *)(mission + 0x78) = 0xe;
        *(int *)(mission + 0x7c) = 0x14;
        *(int *)(mission + 0x80) = 0x28c;
        *mission |= 2;
        break;
    case 2:
        *(int *)(mission + 0x5c) = 1;
        *(unsigned *)(mission + 0x60) = LogicRandomBelow(0xba2);
        *(unsigned *)(mission + 0x6c) = LogicRandomBelow(500);
        *(unsigned *)(mission + 0x68) = *(int *)(mission + 0x6c) + LogicRandomBelow(500);
        *(unsigned *)(mission + 0x78) = LogicRandomBelow(25);
        *(unsigned *)(mission + 0x7c) = *(int *)(mission + 0x78) + LogicRandomBelow(5);
        *(unsigned *)(mission + 0x80) = LogicRandomBelow(1024);
        *mission |= 2;
        break;
    case 3: *(int *)(mission + 0x5c) = 2; break;
    }
    SimDelayPhase(modeCheckName);
}

void LogicResetActorList(void)
{
    logicActorList.ResetActorValues();
    DAT_004dd168 = 0;
    SimHotbriefRefresh(modeCheckName);
}

void LogicUpdateMissionStatus(unsigned char *mission)
{
    int result;
    LogicActor *actor;
    ScriptTick(&gScript);
    LogicReset(mission);
    logicActorList.UpdateActorPercentages();
    result = ScriptRun((unsigned long *)&gScript);
    if (result == 0) {
        *mission |= 1;
        LogicCheckMode(mission);
    }
    actor = logicActorList.First();
    while (actor) {
        if (!actor->Attached()) LogicGenSpawnActor(actor);
        actor = logicActorList.Next(actor);
    }
    *mission &= 0xf7;
    logicActorList.ResetActorState();
    DAT_004dd168 = (int)actor;
}

void LogicMarkMissionReady(unsigned char *mission, int value)
{
    if ((*mission & 0x10) == 0) {
        if (value != 0 && MissionPromptEnabled() && f_004771aa((const char *)value) == 0)
            return;
        *mission |= 0x10;
    }
}

void LogicCheckMissionAction(unsigned char *mission)
{
    int initial = (*mission & 1) != 0;
    int initial20 = (*mission & 0x20) != 0;
    int initial40 = (*mission & 0x40) != 0;
    if (modeSwitch) {
        if (logicActorList.AllRemoteActorsPresent()) LogicMarkMissionReady(mission, 0);
        else if (!initial20 && logicActorList.HasLocalMismatch()) {
            LogicMarkMissionReady(mission, (int)AllocateMissionImage(g511ddc, 0xfd, 0));
            *mission |= 0x20;
        } else if (!initial40 && logicActorList.AllLocalActorsPresent()) {
            LogicMarkMissionReady(mission, (int)AllocateMissionImage(g511ddc, 0xfc, 0));
            *mission |= 0x40;
        }
    } else {
        if (logicActorList.HasLocalMismatch()) LogicMarkMissionReady(mission, 0);
        else if (initial) {
            int result = logicActorList.AllLocalActorsPresent();
            if (result && (*mission & 4) == 0)
                LogicMarkMissionReady(mission, (int)AllocateMissionImage(g511ddc, 0xf4, 0));
            if (result) *mission |= 4;
            else *mission &= 0xfb;
        }
    }
    if ((*mission & 0x10) != 0 && (*mission & 0x80) == 0) {
        DAT_004d3549 = 1;
        if (DAT_004dd168 != 0) LogicResetActorList();
        LogicCheckActorOrder(mission, 0);
        *mission |= 0x80;
    }
}

static inline int MissionIntelPhase(unsigned char *m) { return (*m & 8) != 0; }
void LogicAdvanceMissionState(unsigned char *mission);

void LogicDispatchIntel(unsigned char *mission)
{
    if (DAT_004d2dfa != 0 && DAT_004d271a == 1) {
        if (!DAT_004dd168) DAT_004dd168 = (int)logicActorList.SelectActor(0);
        LogicActor *current = (LogicActor *)DAT_004dd168;
        if (current) {
            int result;
            if (MissionIntelPhase(mission)) result = IntelIsAvailable((int *)current);
            else result = IntelActorDispatch((int *)current);
            if (result) {
                DAT_004dd168 = (int)logicActorList.SelectActor((LogicActor *)DAT_004dd168);
                if (!DAT_004dd168) {
                    if (MissionIntelPhase(mission)) { logicActorList.ResetActorValues(); current = 0; }
                    else {
                        *mission |= 8;
                        logicActorList.ResetActorState();
                        DAT_004dd168 = 0;
                        goto tick;
                    }
                } else goto tick;
            } else goto tick;
        } else logicActorList.ResetActorValues();
        DAT_004dd168 = (int)current;
        SimHotbriefRefresh(modeCheckName);
    }
tick:
    ++DAT_004dd0bc;
    if (DAT_004dd0bc >= 10) {
        LogicAdvanceMissionState(mission);
        LogicCheckMissionAction(mission);
        DAT_004dd0bc = 0;
    }
}

void LogicCopyActorValues(void *unused, int actorId, int unitId)
{
    LogicActor *left;
    LogicActor *right;
    if (DAT_004dd168 == 0) {
        left = logicActorList.FindByUnit((void *)actorId);
        right = logicActorList.FindByUnit((void *)unitId);
        if (left != 0 && right != 0)
            ActorArraySet(right, 9, left->Value());
    }
}

void LogicAdvanceMissionState(unsigned char *mission)
{
    int result;
    LogicActor *actor;
    LogicReset(mission);
    logicActorList.UpdateActorPercentages();
    result = ScriptRun((unsigned long *)&gScript);
    if (result == 0) {
        *mission |= 1;
        LogicCheckMode(mission);
    }
    actor = logicActorList.First();
    while (actor) {
        if (!actor->Attached()) LogicGenSpawnActor(actor);
        actor = logicActorList.Next(actor);
    }
}

static inline int VisibleType(int index) { return ((signed char *)DAT_004d3600)[index]; }
static inline int ClampMissionCount(int n) { if (n <= 0) return 0; if (n >= 0xba1) return 0xba1; return n; }

void LogicGetMissionSummary(unsigned char *mission, unsigned long *summary)
{
    if ((*mission & 2) == 0) {
        int limit = DAT_004d35bc;
        for (int index = 0; index <= limit; ++index)
            if (VisibleType(index)) ++*(int *)(mission + 0x60);
        *(int *)(mission + 0x60) = ClampMissionCount(*(int *)(mission + 0x60));
        *(unsigned long *)(mission + 0x70) = DAT_004d35c0;
        *(int *)(mission + 0x6c) = 0;
        *(int *)(mission + 0x7c) = 0;
        *(int *)(mission + 0x78) = 0;
        LogicActor *actor = logicActorList.First();
        while (actor) {
            if (actor->Attached()) {
                if (actor->Friendly()) {
                    ++*(int *)(mission + 0x7c);
                    if (ActorIsAvailable(actor)) ++*(int *)(mission + 0x78);
                } else if (actor->Enemy()) {
                    *(int *)(mission + 0x6c) += ActorStateCall(actor);
                }
            }
            actor = logicActorList.Next(actor);
        }
    }
    struct Summary { unsigned long words[10]; };
    *(Summary *)summary = *(Summary *)(mission + 0x5c);
}

