#include <string.h>

class FileRecord;
struct LogicActor;
struct LogicActorList {
    void Clear();
    LogicActor *First(), *Next(LogicActor *), *SelectActor(LogicActor *), *FindByUnit(void *);
    int AllLocalActorsPresent();
    int AllRemoteActorsPresent(), HasLocalMismatch();
    void UpdateActorPercentages();
    void NotifyActorLinks(), ResetActorValues(), ResetActorState();
};

struct Script {
    unsigned char header[0x38];
    unsigned long workArea[0x1a6];
};
extern LogicActorList logicActorList;
extern Script gScript;
extern int gScriptRestorePending;

unsigned long StreamRead(FileRecord *, void *, unsigned long);
unsigned long StreamWrite(FileRecord *, const void *, unsigned long);
int f_0046406c(void *, void *);
int f_004640f6(void *, void *);
int ScriptRestore(unsigned long *, FileRecord *);
int ScriptSave(unsigned long *, FileRecord *);
void f_0046b2ac(void *);
void ScriptLoad(unsigned long *, int, int);
void f_0046799a(unsigned long *);
void f_004654cc(void *);
void f_0046415f(void);
void f_00434770(const char *, void *, int, void *);
void f_0046ac21(void *);
void f_0046a97f(void *);
void f_00434890(const char *, void *, int, int);
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
char *ActorName(LogicActor *);
void ActorRelease(LogicActor *);
extern "C" int LogicCopyPoint(int *, int *);
int f_004771aa(const char *);
char *f_0047c350(void *, int, int);
extern int DAT_00511e38;
extern int DAT_004d2722;
extern int DAT_004d3551;
extern int DAT_004d35ac;
extern int DAT_004d35c0;
extern int DAT_004d2de6;
extern char DAT_0048b5ac[];
extern char DAT_0048b5ae[];
extern char DAT_0048b5b2[];
extern char DAT_0048b5b4[];
extern int DAT_004fb944[];
extern char DAT_00511ddc[];
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

struct LogicActor {};
class StrPool { public: int Lock(); void Unlock(); };
extern StrPool missionImageName;
extern char fallbackName[];
extern char gShellBackground[];
extern void AssertFail(const char *, int, const char *);
#define ASSERT(test) if (!(test)) AssertFail(__FILE__, __LINE__, 0)

int LogicReadActorRecord(void *record, FileRecord *stream)
{
    if (StreamRead(stream, record, 0xa8) != 0xa8)
        return 0;
    int result = f_0046406c(&logicActorList, stream);
    if (result != 0) {
        result = ScriptRestore((unsigned long *)&gScript, stream);
        if (result != 0) {
            gScriptRestorePending = 0;
            f_0046b2ac(record);
            result = 1;
        } else {
            result = 0;
        }
    }
    return result;
}

int LogicWriteActorRecord(const void *record, FileRecord *stream)
{
    if (StreamWrite(stream, record, 0xa8) != 0xa8)
        return 0;
    if (f_004640f6(&logicActorList, stream) == 0)
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
    return &gScript.workArea[2];
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

void *LogicActorBuild(void *record, const unsigned long *source)
{
    struct RecordHeader { unsigned long words[0x16]; };
    *(RecordHeader *)((char *)record + 4) = *(const RecordHeader *)source;
    int acquiredTextPool = DAT_00511e38 == 0;
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
    f_00434770(fallbackName, (void *)0x004c9234,
               *(int *)((char *)record + 0x18), gShellBackground);
    f_0046ac21(record);
    logicActorList.UpdateActorPercentages();
    if (ScriptRun((unsigned long *)&gScript) == 0) {
        *(unsigned char *)record |= 1;
        LogicCheckMode(record);
    }
    for (LogicActor *actor = logicActorList.First(); actor != 0;
         actor = logicActorList.Next(actor)) {
        if (*(int *)actor == 0)
            LogicGenSpawnActor(actor);
    }
    if (*(int *)0x004d2722 == 0)
        logicActorList.NotifyActorLinks();
    f_00434890(fallbackName, (void *)0x004c9234,
               *(int *)((char *)record + 0x18),
               *(int *)((char *)record + 0x24));
    *(int *)((char *)record + 0x68) = *(int *)0x004d35c0;
    if (*(int *)((char *)record + 0x84) == 0 && *(int *)0x004d35ac != 0) {
        int index;
        for (index = 0; index < 8 &&
                        *(int *)((char *)record + 0x84 + index * 4) != 0;
             ++index) {
        }
        if (index < 8)
            *(int *)((char *)record + 0x84 + index * 4) = *(int *)0x004d35ac;
    }
    if (acquiredTextPool)
        missionImageName.Unlock();
    return (char *)record + 0x84;
}

int LogicCheckActorCollision(unsigned char *mission, int unused)
{
    if (DAT_004d3551 != 0)
        return 0;
    LogicActor *candidateActors[32];
    int candidateCount = DAT_004d3551;
    if (!logicActorList.AllLocalActorsPresent()) {
        for (LogicActor *entry = logicActorList.First(); entry != 0;
             entry = logicActorList.Next(entry)) {
            unsigned char *actor = *(unsigned char **)entry;
            int eligible = actor != 0 && actor[0x88] != 0 &&
                           *(int *)(*(int *)(actor + 0x1c) + 0x12) >= 1 &&
                           actor[0x89] == 0;
            if (eligible && ((((unsigned char *)entry)[0x0c] & 1) != 0)) {
                int owner = actor == 0 ? 0 : *(int *)(actor + 8);
                if (owner == DAT_004d2de6 &&
                    ((((unsigned char *)entry)[0x0c] & 2) != 0)) {
                    int foundCloseName = 0;
                    int foundOtherUnit = 0;
                    int foundCandidateKind = 0;
                    int foundSimilarName = 0;
                    for (LogicActor *other = logicActorList.First(); other != 0;
                         other = logicActorList.Next(other)) {
                        unsigned char *otherActor = *(unsigned char **)other;
                        int otherEligible = otherActor != 0 &&
                                            otherActor[0x88] != 0 &&
                                            *(int *)(*(int *)(otherActor + 0x1c) + 0x12) >= 1 &&
                                            otherActor[0x89] == 0;
                        if (otherEligible) {
                            if (((int *)other)[7] == 0x1e) {
                                foundCandidateKind = 1;
                                if (LogicCopyPoint((int *)ActorName(entry),
                                                   (int *)ActorName(other)) < 7)
                                    foundCloseName = 1;
                            } else if ((((unsigned char *)other)[0x0c] & 1) == 0) {
                                foundOtherUnit = 1;
                                if (LogicCopyPoint((int *)ActorName(entry),
                                                   (int *)ActorName(other)) < 0x11)
                                    foundSimilarName = 1;
                            }
                        }
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
                            ASSERT(candidateCount <= 0x1f);
                        candidateActors[candidateCount++] = entry;
                    }
                }
            }
        }
    }
    if (candidateCount == 0) {
        if ((mission[0] & 0x10) == 0) {
            char *prompt = f_0047c350(DAT_00511ddc, 0xf3, 0);
            if (f_004771aa(prompt) == 0)
                return 0;
        }
    } else {
        char *prompt = f_0047c350(DAT_00511ddc, 0xf6, 0);
        strcpy((char *)DAT_004fb944, prompt);
        strcat((char *)DAT_004fb944, (char *)DAT_0048b5ac);
        for (int i = 0; i < candidateCount; ++i) {
            strcat((char *)DAT_004fb944, (char *)DAT_0048b5ae);
            strcat((char *)DAT_004fb944, ActorName(candidateActors[i]));
            strcat((char *)DAT_004fb944, (char *)DAT_0048b5b2);
        }
        strcat((char *)DAT_004fb944, (char *)DAT_0048b5b4);
        prompt = f_0047c350(DAT_00511ddc, 0xf3, 0);
        strcat((char *)DAT_004fb944, prompt);
        if (f_004771aa((char *)DAT_004fb944) == 0)
            return 0;
        for (int releaseIndex = 0; releaseIndex < candidateCount; ++releaseIndex)
            ActorRelease(candidateActors[releaseIndex]);
    }
    return 1;
}

extern int DAT_004d3551, DAT_004d2dfa, DAT_004d271a, DAT_004d3549;
extern int DAT_004dd168, DAT_004dd0bc, DAT_004d2722, DAT_004d35bc;
extern int DAT_004d3600, DAT_004d35c0;
extern int MissionRandomTable[];
extern int modeSwitch;
extern char modeCheckName[];
extern char DAT_00511ddc[];
void ScriptAbort(Script *);
void ScriptTick(Script *);
void LogicReset(void);
int next_link_seed(unsigned long *);
int IntelActorDispatch(int *);
int IntelIsAvailable(int *);
int ActorIsAvailable(LogicActor *);
int ActorStateCall(LogicActor *);
void ActorArraySet(LogicActor *, int, int);
void SimHotbriefRefresh(void *);
void f_0044f6a0(void *);

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
    if (order >= 2) {
        if (order <= 2) {
            *(int *)(mission + 0x5c) = 1;
            value = next_link_seed((unsigned long *)MissionRandomTable);
            *(unsigned int *)(mission + 0x60) = value % 0xba2;
            value = next_link_seed((unsigned long *)MissionRandomTable);
            *(unsigned int *)(mission + 0x6c) = value % 500;
            value = next_link_seed((unsigned long *)MissionRandomTable);
            *(unsigned int *)(mission + 0x68) = *(int *)(mission + 0x6c) + value % 500;
            value = next_link_seed((unsigned long *)MissionRandomTable);
            *(unsigned int *)(mission + 0x78) = value % 0x19;
            value = next_link_seed((unsigned long *)MissionRandomTable);
            *(unsigned int *)(mission + 0x7c) = *(int *)(mission + 0x78) + value % 5;
            value = next_link_seed((unsigned long *)MissionRandomTable);
            *(unsigned int *)(mission + 0x80) = value % 0x400;
        } else {
            if (order == 3)
                *(int *)(mission + 0x5c) = 2;
            goto done;
        }
    } else if (order == 1) {
        *(int *)(mission + 0x5c) = order;
        *(int *)(mission + 0x60) = 0x582;
        *(int *)(mission + 0x6c) = 0x87;
        *(int *)(mission + 0x68) = 0xb4;
        *(int *)(mission + 0x78) = 0xe;
        *(int *)(mission + 0x7c) = 0x14;
        *(int *)(mission + 0x80) = 0x28c;
    } else {
        goto done;
    }
    *mission |= 2;
done:
    f_0044f6a0(modeCheckName);
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
    LogicReset();
    logicActorList.UpdateActorPercentages();
    result = ScriptRun((unsigned long *)&gScript);
    if (result == 0) {
        *mission |= 1;
        LogicCheckMode(mission);
    }
    for (actor = logicActorList.First(); actor != 0; actor = logicActorList.Next(actor))
        if (*(int *)actor == 0)
            LogicGenSpawnActor(actor);
    *mission &= 0xf7;
    logicActorList.ResetActorState();
    DAT_004dd168 = (int)actor;
}

void LogicMarkMissionReady(unsigned char *mission, int value)
{
    if ((*mission & 0x10) == 0) {
        if (value != 0 && DAT_004d3549 == 0 && f_004771aa((const char *)value) == 0)
            return;
        *mission |= 0x10;
    }
}

void LogicCheckMissionAction(unsigned char *mission)
{
    unsigned char initial = *mission;
    unsigned char original = *mission;
    int result;
    char *message;
    if (modeSwitch == 0) {
        result = logicActorList.HasLocalMismatch();
        if (result != 0) {
            if ((*mission & 0x10) == 0)
                *mission |= 0x10;
            goto update;
        }
        if ((initial & 1) == 0)
            goto update;
        result = logicActorList.AllLocalActorsPresent();
        if (result != 0 && (*mission & 4) == 0) {
            message = f_0047c350(DAT_00511ddc, 0xf4, 0);
            if ((*mission & 0x10) == 0) {
                if (message != 0 && DAT_004d3549 == 0 && f_004771aa(message) == 0)
                    goto finish_prompt;
                *mission |= 0x10;
            }
        }
        if (result == 0)
            *mission &= 0xfb;
        else
            *mission |= 4;
    } else {
        result = logicActorList.AllRemoteActorsPresent();
        if (result != 0) {
            if ((*mission & 0x10) == 0)
                *mission |= 0x10;
            goto update;
        }
        if ((original & 0x20) == 0 && logicActorList.HasLocalMismatch() != 0) {
            message = f_0047c350(DAT_00511ddc, 0xfd, 0);
            if ((*mission & 0x10) == 0) {
                if (message != 0 && DAT_004d3549 == 0 && f_004771aa(message) == 0)
                    goto finish_prompt;
                *mission |= 0x10;
            }
            *mission |= 0x20;
            goto update;
        }
        if ((original & 0x40) != 0)
            goto update;
        result = logicActorList.AllLocalActorsPresent();
        if (result == 0)
            goto update;
        message = f_0047c350(DAT_00511ddc, 0xfc, 0);
        if ((*mission & 0x10) == 0) {
            if (message != 0 && DAT_004d3549 == 0 && f_004771aa(message) == 0)
                goto finish_prompt;
            *mission |= 0x10;
        }
        *mission |= 0x40;
    }
update:
    if ((*mission & 0x10) != 0 && (*mission & 0x80) == 0) {
        DAT_004d3549 = 1;
        if (DAT_004dd168 != 0) {
            logicActorList.ResetActorValues();
            DAT_004dd168 = 0;
            SimHotbriefRefresh(modeCheckName);
        }
        LogicCheckActorOrder(mission, 0);
        *mission |= 0x80;
    }
    return;
finish_prompt:
    return;
}

void LogicDispatchIntel(unsigned char *mission)
{
    int result;
    LogicActor *current;
    if (DAT_004d2dfa == 0 || DAT_004d271a != 1)
        goto tick;
    if (DAT_004dd168 == 0)
        DAT_004dd168 = (int)logicActorList.SelectActor(0);
    current = (LogicActor *)DAT_004dd168;
    if (current == 0) {
        logicActorList.ResetActorValues();
    } else {
        if ((*mission & 8) == 0)
            result = IntelActorDispatch((int *)current);
        else
            result = IntelIsAvailable((int *)current);
        if (result == 0)
            goto tick;
        DAT_004dd168 = (int)logicActorList.SelectActor(current);
        if (DAT_004dd168 != 0)
            goto tick;
        if ((*mission & 8) == 0) {
            *mission |= 8;
            logicActorList.ResetActorState();
            DAT_004dd168 = 0;
            goto tick;
        }
        logicActorList.ResetActorValues();
        current = 0;
    }
    DAT_004dd168 = (int)current;
    SimHotbriefRefresh(modeCheckName);
tick:
    ++DAT_004dd0bc;
    if (DAT_004dd0bc > 9) {
        ScriptTick(&gScript);
        LogicReset();
        logicActorList.UpdateActorPercentages();
        result = ScriptRun((unsigned long *)&gScript);
        if (result == 0) {
            *mission |= 1;
            LogicCheckMode(mission);
        }
        for (LogicActor *actor = logicActorList.First(); actor != 0;
             actor = logicActorList.Next(actor))
            if (*(int *)actor == 0)
                LogicGenSpawnActor(actor);
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
            ActorArraySet(right, 9, *(int *)((char *)left + 0x14));
    }
}

void LogicAdvanceMissionState(unsigned char *mission)
{
    int result;
    LogicActor *actor;
    ScriptTick(&gScript);
    LogicReset();
    logicActorList.UpdateActorPercentages();
    result = ScriptRun((unsigned long *)&gScript);
    if (result < 1) {
        *mission |= 1;
        LogicCheckMode(mission);
    }
    for (actor = logicActorList.First(); actor != 0; actor = logicActorList.Next(actor))
        if (*(int *)actor == 0)
            LogicGenSpawnActor(actor);
}

void LogicGetMissionSummary(unsigned char *mission, unsigned long *summary)
{
    int limit = DAT_004d35bc;
    LogicActor *actor;
    if ((*mission & 2) == 0) {
        for (int index = 0; index <= limit; ++index)
            if (*(char *)(index + DAT_004d3600) != 0)
                ++*(int *)(mission + 0x60);
        limit = *(int *)(mission + 0x60);
        if (limit < 1)
            limit = 0;
        else if (0xba0 < limit)
            limit = 0xba1;
        *(int *)(mission + 0x60) = limit;
        *(unsigned long *)(mission + 0x70) = DAT_004d35c0;
        *(int *)(mission + 0x6c) = 0;
        *(int *)(mission + 0x7c) = 0;
        *(int *)(mission + 0x78) = 0;
        for (actor = logicActorList.First(); actor != 0; actor = logicActorList.Next(actor)) {
            if (*(int *)actor != 0) {
                if ((((unsigned char *)actor)[0x0c] & 1) == 0) {
                    ++*(int *)(mission + 0x7c);
                    if (ActorIsAvailable(actor) != 0)
                        ++*(int *)(mission + 0x78);
                } else {
                    *(int *)(mission + 0x6c) += ActorStateCall(actor);
                }
            }
        }
    }
    unsigned char *source = mission + 0x5c;
    for (int count = 10; count != 0; --count) {
        *summary = *(unsigned long *)source;
        source += 4;
        ++summary;
    }
}
