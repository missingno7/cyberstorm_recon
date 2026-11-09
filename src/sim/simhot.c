#include <string.h>

class FileRecord; class ResMgr; class FogBuffer; struct FogSource; int sprintf(char *, const char *, ...);
FogSource *FogSetSource(FogBuffer *, FogSource *);
class FogBuffer { public: int mode; char objectBytes[0x14]; FogBuffer(FogSource *s) { FogSetSource(this, s); } ~FogBuffer(); int Resize(); int Initialize(int, int); };
struct FogData { unsigned long words[8]; }; struct FogSource {}; extern char fallbackName[];
FogBuffer gFogBufferObjects[9] = { (FogSource *)fallbackName, (FogSource *)fallbackName, (FogSource *)fallbackName, (FogSource *)fallbackName, (FogSource *)fallbackName, (FogSource *)fallbackName, (FogSource *)fallbackName, (FogSource *)fallbackName, (FogSource *)fallbackName };
void FogRelease(FogBuffer *); void FogCopy(FogBuffer *, FogBuffer *); int FogRead(FogBuffer *, void *); int FogWrite(FogBuffer *, void *);
unsigned long StreamRead(FileRecord *, void *, unsigned long); unsigned long StreamWrite(FileRecord *, const void *, unsigned long); void TimerSet(unsigned long *, unsigned long); int SimHotReadTimer(char *);
class ResMgr { public: }; extern ResMgr gResMgr;
extern char g513888[], gMenuClip[], gShellFont[], gShellFontBefore[]; extern int gScreenWidth, gScreenHeight, gInputPolling, gGameOverDismissed;
void *f_0047c920(ResMgr *, const char *, int, void *, int); unsigned long f_00476512(int); void f_00477798(int); void SigsStart(void *, int); void MenuDrawImage(void *, void *, int, int); void ReleasePoolData(void *); void GuiResetClip(void *); void f_cba0(void *, const char *); void f_cbf0(void *); void f_0047cfc0(void *, int, int, int, char *); void SurfacePresent(void *); void f_0047ddf0(void); void InputPoll(void *); void f_0046cbc6(int, int); void f_0047c240(void *); void f_00475887(void); void f_004758ac(void); void GuiFadeOut(void); void f_00450b00(void *); void f_00450aa0(void *, int);
extern FogBuffer *gFogBuffers[9], *gFogBufferIndex[10]; extern int gFogBufferInitialized; extern FogBuffer gFogBufferSource; extern FogData gFogData[9], gFogDefaultData;
extern int retryTimeout; extern char modeCheckName[]; extern unsigned long gFogTimer;void SimHotResizeFogBuffers(void)
{
    int offset = 0;
    FogData *destination = gFogData;
    do {
        (*(FogBuffer **)((char *)gFogBuffers + offset))->Resize();
        FogData *source = &gFogDefaultData;
        FogCopy(*(FogBuffer **)((char *)gFogBuffers + offset), &gFogBufferSource);
        *destination = *source;
        ++destination;
        offset += 4;
    } while (offset != 36);
    gFogBufferInitialized = 1;
}

void SimHotReleaseFogBuffers(void)
{
    for (int offset = 0; offset != 36; offset += 4)
        FogRelease(*(FogBuffer **)((char *)gFogBuffers + offset));
    gFogBufferInitialized = 0;
}

void SimHotReadFogBuffers(int index)
{
    FogData *source = gFogData;
    FogCopy(&gFogBufferSource, gFogBufferIndex[index]);
    source += index;
    --source;
    gFogDefaultData = *source;
}

void SimHotWriteFogBuffers(int index)
{
    FogData *source = &gFogDefaultData;
    FogCopy(gFogBufferIndex[index], &gFogBufferSource);
    FogData *destination = gFogData;
    destination += index;
    --destination;
    *destination = *source;
}

void SimHotInitializeFog(void)
{
    int mode = gFogBuffers[0]->mode;
    if (mode == 4)
        gFogBufferSource.Initialize(mode, 1);
    else
        gFogBufferSource.Initialize(5, 1);
}

void SimHotReadFogStream(FileRecord *stream)
{
    unsigned long state;
    unsigned long clockValue;
    unsigned long timerValue;
    int offset = 0;
    FogData *destination = gFogData;
    do {
        (*(FogBuffer **)((char *)gFogBuffers + offset))->Resize();
        FogData *source = &gFogDefaultData;
        FogCopy(*(FogBuffer **)((char *)gFogBuffers + offset), &gFogBufferSource);
        *destination = *source;
        ++destination;
        offset += 4;
    } while (offset != 36);
    gFogBufferInitialized = 1;
    for (offset = 0; offset != 36; offset += 4)
        FogRead(*(FogBuffer **)((char *)gFogBuffers + offset), stream);
    StreamRead(stream, gFogData, 0x120);
    StreamRead(stream, &state, 4);
    for (offset = 0; offset != 36; offset += 4)
        (*(FogBuffer **)((char *)gFogBuffers + offset))->mode = state;
    gFogBufferSource.mode = state;
    StreamRead(stream, &clockValue, 4);
    StreamRead(stream, &timerValue, 4);
    retryTimeout = clockValue;
    TimerSet(&gFogTimer, timerValue);
}

void SimHotWriteFogStream(FileRecord *stream)
{
    unsigned long state;
    unsigned long clockValue;
    unsigned long timerValue;
    int offset = 0;
    for (; offset != 36; offset += 4)
        FogWrite(*(FogBuffer **)((char *)gFogBuffers + offset), stream);
    StreamWrite(stream, gFogData, 0x120);
    state = (unsigned long)gFogBuffers[0]->mode;
    StreamWrite(stream, &state, 4);
    clockValue = retryTimeout;
    timerValue = SimHotReadTimer(modeCheckName);
    StreamWrite(stream, &clockValue, 4);
    StreamWrite(stream, &timerValue, 4);
}
struct MissionRecord { unsigned char header[16]; char name[172]; }; extern MissionRecord *gMissionRecords;
void *AllocateMissionImage(void *, int, int); char *BriefFormat(void *, const char *); void f_00477295(char *); void AssertExit(int);void SimHotLoadMission(int index)
{
    char *image = (char *)AllocateMissionImage((void *)"", 0x1a2, 1);
    char *name = gMissionRecords[index].name;
    char *loaded = BriefFormat(image, name);
    f_00477295(loaded);
    AssertExit(0);
}
struct ActorRecord; class ProfileManager { public: void *Mission(int); }; struct HotseatState {};
extern ProfileManager profileManager; extern HotseatState *gShellHotseat; extern char *searchPaths[]; extern char gHotbriefStrings[];
void f_0043dcfd(ProfileManager *, int); void f_0043ef34(ProfileManager *); int HasMission(ProfileManager *); int FileExists(const char *); int LoadMission(const char *, int);
void AssertFail(const char *, int, const char *);
#define ASSERT(test) { unsigned long assertValue = (unsigned long)(test); if (assertValue == 0) AssertFail(__FILE__, __LINE__, 0); }
extern char DAT_0048b5ac[], DAT_0048b5ae[], DAT_0048b5b2[], DAT_0048b5b4[];
extern char DAT_004d028b[]; extern int DAT_004d030f, DAT_004d3551, DAT_004d3555, DAT_004d3559, DAT_004c975c, DAT_004fb940;
extern unsigned long DAT_004c9354[], DAT_004f6600; extern char g4dd0c0[];
int f_00450930(void *, int, void *); char *f_0047c350(void *, int, int); void MissionRecordInitialize(void *, int, int, int, int, int); void MissionDataInit(void *, int, int, int, int, int, int);
void HotbriefEmpty(void); void *LogicActorBuild(void *, void *); void BestLocSetData(void *, void *, void *); void f_00434d70(void *);
ActorRecord *ActorGetByIndex(int); void *GetModePlayer(void *, int); void *f_0043c6ed(void *, int); void *Allocate(int); void *f_00435dec(int);
struct ActorBlobA { unsigned char bytes[0x10b]; }; struct ActorBlobB { unsigned char bytes[0x7e]; };
void *ActorListAdd(void *, void *, void *, void *, int, int); void LogicActorDelete(void *, int); void ActorActivate(void *);void SimHotCheckMission(int index)
{
    char path[256];
    f_0043dcfd(&profileManager, 3);
    if (*(int *)((char *)gShellHotseat + 0x1218 + index * 4) != 0) {
        if (LoadMission((const char *)((char *)gShellHotseat + 0x218 + index * 0x200), 1) != 0)
            goto loaded;
        char *image = (char *)AllocateMissionImage(gHotbriefStrings, 0x1a2, 1);
        char *name = gMissionRecords[index].name;
        char *loaded = BriefFormat(image, name);
        f_00477295(loaded);
        AssertExit(0);
    } else {
        int found = 0;
        int pathOffset = 0;
        do {
            char *searchPath = *(char **)((char *)searchPaths + pathOffset);
            if (searchPath == 0)
                break;
            sprintf(path, "%sqstart\\MP%02d_%c.CBM", searchPath,
                    *(int *)((char *)gShellHotseat + 0x1278 + index * 4),
                    *(int *)((char *)gShellHotseat + 0x1238 + index * 4) + 0x40);
            if (FileExists(path) != 0)
                found = 1;
            pathOffset += 4;
        } while (found == 0);
        if (found != 0 && LoadMission(path, 1) != 0)
            goto loaded;
        char *image = (char *)AllocateMissionImage(gHotbriefStrings, 0x1a2, 1);
        char *name = gMissionRecords[index].name;
        char *loaded = BriefFormat(image, name);
        f_00477295(loaded);
        AssertExit(0);
    }
loaded:
    if (HasMission(&profileManager) == 0) {
        char *image = (char *)AllocateMissionImage(gHotbriefStrings, 0x1a2, 1);
        char *name = gMissionRecords[index].name;
        char *loaded = BriefFormat(image, name);
        f_00477295(loaded);
        AssertExit(0);
    }
    f_0043ef34(&profileManager);
    f_0043dcfd(&profileManager, 1);
}
void SimHotStartMission(int *mission)
{
    int result, slot = 0, pathOffset = 0, briefOffset = 0, recordOffset = 0;
    int localPosition[2], localRecord[0x11], packedSize, width, height;
    int ownerIndex, searchIndex, found, rawOwner;
    char path[256];
    char *missionName = (char *)mission + 0x2ee;
    char *missionOwner = (char *)mission + 0x2de;
    char *missionFile = (char *)gShellHotseat + 0x218;
    char *logicData;
    ActorRecord *actor;
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
    ASSERT(*mission != 0 && gShellHotseat != 0);
    for (int fogIndex = 0; fogIndex != 9; ++fogIndex) {
        gFogBuffers[fogIndex]->Resize(); FogCopy(gFogBuffers[fogIndex], &gFogBufferSource); gFogData[fogIndex] = gFogDefaultData;
    }
    gFogBufferInitialized = 1;
    for (slot = 0; slot < *(int *)((char *)gShellHotseat + 0x4c); ++slot) {
        result = f_00450930(mission, 1, 0);
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
        ASSERT(result == slot + 1);
        strcpy(missionName, (char *)gShellHotseat + 0x58 + briefOffset);
        *(int *)(missionOwner + recordOffset + 0xb4) = *(signed char *)((char *)gShellHotseat + 0x1298 + slot);
        briefOffset += 0x20; recordOffset += 0xbc; missionName += 0xbc;
    }
    *(int *)((char *)mission + 0x97e) = 1;
    *(int *)((char *)mission + 0x99e) = *(int *)((char *)mission + 0x97e);
    *(int *)((char *)mission + 0x986) = *(int *)((char *)mission + 0x97e);
    *(int *)((char *)mission + 0x98a) = f_00450930(mission, 3, 0);
    *(int *)((char *)mission + ((*(int *)((char *)mission + 0x98a) - 1) * 0xbc) + 0x392) = 8;
    char *poolText = f_0047c350(gHotbriefStrings, 0x9d, 0);
    strcpy((char *)mission + ((*(int *)((char *)mission + 0x98a) - 1) * 0xbc) + 0x2ee, poolText);
    packedSize = *(unsigned short *)((char *)gShellHotseat + 0x12c0);
    width = (*(short *)((char *)gShellHotseat + 0x12c0) >> 8) - 1;
    height = (packedSize & 0xf) - 1;
    MissionRecordInitialize(localRecord, 8, width, height, 1, 1);
    MissionDataInit(localRecord, 3, 0, 0, *(int *)((char *)gShellHotseat + 0x12b0), *(int *)((char *)gShellHotseat + 0x12b4), *(int *)((char *)gShellHotseat + 0x12a0));
    localRecord[7] = 0x67; HotbriefEmpty(); logicData = (char *)LogicActorBuild(g4dd0c0, localRecord);
    slot = 0; pathOffset = 0; recordOffset = 0; missionFile = (char *)gShellHotseat + 0x218;
    for (;;) {
        if (slot >= *(int *)((char *)gShellHotseat + 0x4c)) {
            DAT_004c975c = 0; memcpy(DAT_004c9354, localRecord, 0x58);
            gFogBufferSource.Initialize(*(int *)((char *)gShellHotseat + 0x12a4), 1);
            DAT_004d3551 = 1; DAT_004d3555 = *(int *)((char *)gShellHotseat + 0x12c4); DAT_004d3559 = *(int *)((char *)gShellHotseat + 0x12c8);
            DAT_004f6600 = *(unsigned long *)((char *)gShellHotseat + 0x12bc); f_00434d70((void *)0x004d35e4); *(int *)((char *)mission + 0x2ba) = 0; return;
        }
        *mission = 0; DAT_004fb940 = 0; f_0043dcfd(&profileManager, 3);
        if (*(int *)((char *)gShellHotseat + 0x1218 + slot * 4) != 0) {
            result = LoadMission(missionFile, 1);
            if (result == 0) { char *failure = BriefFormat(f_0047c350(gHotbriefStrings, 0x1a2, 1), gMissionRecords[slot].name); f_00477295(failure); AssertExit(0); }
        } else {
            found = 0; searchIndex = 0;
            do {
                char *searchPath = searchPaths[searchIndex];
                if (searchPath == 0) break;
                sprintf(path, "%sqstart\\MP%02d_%c.CBM", searchPath, *(int *)((char *)gShellHotseat + 0x1278 + slot * 4), *(int *)((char *)gShellHotseat + 0x1238 + slot * 4) + 0x40);
                if (FileExists(path) != 0) found = 1; ++searchIndex;
            } while (found == 0);
            if (found == 0 || LoadMission(path, 1) == 0) { char *failure = BriefFormat(f_0047c350(gHotbriefStrings, 0x1a2, 1), gMissionRecords[slot].name); f_00477295(failure); AssertExit(0); }
        }
        if (HasMission(&profileManager) == 0) { char *failure = BriefFormat(f_0047c350(gHotbriefStrings, 0x1a2, 1), gMissionRecords[slot].name); f_00477295(failure); AssertExit(0); }
        f_0043ef34(&profileManager); f_0043dcfd(&profileManager, 1); *mission = 1;
        BestLocSetData((void *)0x004d3594, localPosition, (void *)(*(int *)logicData));
        if (*(int *)((char *)gShellHotseat + 0x12a0) != 0) {
            actor = ActorGetByIndex(slot + 1);
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
            ASSERT(actor != 0 && *(char *)(*(int *)((char *)actor + 0x18) + 0x12) == 0x1f);
            *(int *)((char *)actor + 8) = slot + 1; void *player = GetModePlayer(mission, slot + 1); *(unsigned char *)((char *)actor + 0x5a) = *(unsigned char *)((char *)player + 0xb4);
        }
        ownerIndex = 0;
        for (int flagIndex = 0; flagIndex < DAT_004d030f; ++flagIndex) {
            if (DAT_004d028b[flagIndex * 2] != 0) {
                void *unitData = profileManager.Mission(ownerIndex + 1); void *ownerData = f_0043c6ed(&profileManager, (int)unitData);
                void *actorCopy = Allocate(0x10b); rawOwner = (int)Allocate(0x7e); void *ownerObject = rawOwner != 0 ? f_00435dec(rawOwner) : 0;
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
// Original ASSERT source-line padding.
                ASSERT(actorCopy != 0 && ownerObject != 0);
                *(ActorBlobA *)actorCopy = *(ActorBlobA *)unitData;
                *(ActorBlobB *)ownerObject = *(ActorBlobB *)ownerData;
                actor = (ActorRecord *)ActorListAdd((void *)(slot + 1), actorCopy, ownerObject, localPosition, 1, 0);
// Original ASSERT source-line padding.
                ASSERT(actor != 0 && actorCopy != 0 && ownerObject != 0);
                LogicActorDelete(g4dd0c0, (int)actor); ((int *)actor)[4] = -9999; ActorActivate(actor);
            }
            ++ownerIndex;
        }
        pathOffset += 0x200; recordOffset += 0xbc; missionFile += 0x200; logicData += 4; ++slot;
    }
}struct ActorRecord;
void *AllocateMissionImage(const char *, int, int);
int f_0047723b(const char *);
ActorRecord *ActorFindByOwner(ActorRecord *);
ActorRecord *ActorFindOwnerNext(ActorRecord *, ActorRecord *);
int SimHotbriefCheckActor(ActorRecord *);
void SimHotbriefClearActor(ActorRecord *);
void LogicSetActorLink(void *, int, int, int);
void SimHotbriefRefresh(void *);
extern char gHotbriefStrings[];
extern char g4dd0c0[];
extern int gHotbriefCounter;
extern int gFogUpdateCount;
ActorRecord *f_00456b20(void);
ActorRecord *f_00456bb0(ActorRecord *);
int f_00455d10(ActorRecord *);

void SimHotbriefRemove(int context, int confirm, int slot)
{
    if (confirm != 0) {
        char *message = (char *)AllocateMissionImage(gHotbriefStrings, 0x1a5, 1);
        if (f_0047723b(message) == 0)
            return;
    }
    for (ActorRecord *actor = ActorFindByOwner((ActorRecord *)slot); actor != 0;
         actor = ActorFindOwnerNext(actor, (ActorRecord *)slot)) {
        ((int *)actor)[2] = *(int *)((char *)context + 0x98a);
        if (SimHotbriefCheckActor(actor) == 0)
            SimHotbriefClearActor(actor);
        else
            LogicSetActorLink(g4dd0c0, (int)actor, 6, 0);
    }
    *(int *)((char *)context + 0x2de + (slot - 1) * 0xbc) = 0;
    if (slot == *(int *)((char *)context + 0x986))
        SimHotbriefRefresh((void *)context);
}

int SimHotChooseHotseatOwner(void *mission)
{
    if (*(int *)((char *)mission + 0x10f1) != 0) {
        int check = gFogUpdateCount % 10;
        ++gFogUpdateCount;
        if (check == 0) {
            int seen[10];
            memset(seen, 0, sizeof(seen));
            int count = 0;
            int lastOwner = 0;
            for (ActorRecord *actor = f_00456b20(); actor != 0; actor = f_00456bb0(actor)) {
                int owner = ((int *)actor)[2];
                if (owner > 0 && f_00455d10(actor) != 0 &&
                    ((char *)actor)[0x88] != 0 && seen[owner] == 0) {
                    ++count;
                    seen[owner] = 1;
                    lastOwner = owner;
                }
            }
            int homeOwner = *(int *)((char *)mission + 0x98a);
            if (count == 2 && seen[homeOwner] != 0)
                *(int *)((char *)mission + 0x10f5) = 0;
            int selectedOwner = *(int *)((char *)mission + 0x986);
            if (count > 1 && seen[selectedOwner] == 0)
                SimHotbriefRemove((int)mission, 0, selectedOwner);
            if (count == 1)
                return lastOwner;
        }
    }
    return 0;
}

void SimHotBriefGameOver(void *mission)
{
    unsigned long previousDelay = f_00476512(0x1f);
    f_00477798(0);
    SigsStart(g513888, 0);
    void *image = f_0047c920(&gResMgr, "BGREDHEX.BMX", 1, 0, 0);
    MenuDrawImage(g513888, (char *)image + 4,
                  (gScreenWidth - 0x280) / 2, (gScreenHeight - 0x1e0) / 2);
    ReleasePoolData(image);
    GuiResetClip(gMenuClip);
    int halfHeight = gScreenHeight / 2;
    int halfWidth = gScreenWidth / 2;
    f_cba0(gShellFont, "font3.fnx");
    char *message = (char *)AllocateMissionImage(gHotbriefStrings, 0x19c, 1);
    f_0047cfc0(gShellFontBefore, halfWidth, halfHeight - 0x14, 0, message);
    f_0047cfc0(gShellFont, halfWidth, halfHeight + 0x14, 0,
               (char *)(*(int *)((char *)mission + 0x97a) + 0x10));
    f_cbf0(gShellFont);
    SurfacePresent(g513888);
    f_0047ddf0();
    gInputPolling = 1;
    InputPoll((char *)mission + 8);
    f_0046cbc6(0xb, 500);
    f_0046cbc6(0xb, 500);
    f_0046cbc6(0xb, 500);
    for (;;) {
        gInputPolling = 1;
        InputPoll((char *)mission + 8);
        if (gGameOverDismissed != 0 ||
            ((*(unsigned long *)((char *)mission + 0xc) & 0x50) == 0x40) ||
            ((*(unsigned long *)((char *)mission + 0xc) & 0xa0) == 0x80))
            break;
        f_0047c240(g513888);
        f_00475887();
        f_004758ac();
    }
    GuiFadeOut();
    SigsStart(g513888, 0);
    SurfacePresent(g513888);
    f_00450b00(mission);
    f_00476512(previousDelay);
    f_00450aa0(mission, 1);
}

unsigned long SimHotIdentity(unsigned long value)
{
    return value;
}

