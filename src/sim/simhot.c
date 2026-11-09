#include <string.h>
struct HotseatInput { int state, bits; int Pressed(int mask, int value) { return (bits & mask) == value; } }; struct ViewRect { int left, top, right, bottom; }; extern ViewRect viewportClipLeft, gBriefDrawSource; extern unsigned long *gMenuTextMap, shellDefaultTextMap[]; extern unsigned long gMenuTextColor, shellDefaultTextColor;

extern char gHotbriefStrings[]; class FileRecord; class ResMgr; class FogBuffer; struct FogSource; int sprintf(char *, const char *, ...);
FogBuffer *FogSetSource(FogBuffer *, FogSource *);
class FogBuffer { public: long mode; FogSource *source; long count; signed char *lower, *upper; long state; FogBuffer(FogSource *s) { FogSetSource(this, s); } int Resize(); void Initialize(int mode=0, int initialize=1); };
struct GridPoint { long x,y; GridPoint() {} }; struct ViewState { GridPoint point; long zoom,scale,direction; unsigned long flags,requested; long spare; ViewState(); }; struct FogSource { long unused[10]; long count; }; extern char fallbackName[];
int gFogUpdateCount;
FogBuffer gFogBufferObjects[9] = { (FogSource *)fallbackName, (FogSource *)fallbackName, (FogSource *)fallbackName, (FogSource *)fallbackName, (FogSource *)fallbackName, (FogSource *)fallbackName, (FogSource *)fallbackName, (FogSource *)fallbackName, (FogSource *)fallbackName };
ViewState gViewState[9]; int gFogBufferInitialized; void FogRelease(FogBuffer *); void FogCopy(FogBuffer *, FogBuffer *); int FogRead(FogBuffer *, void *); int FogWrite(FogBuffer *, void *);
unsigned long StreamRead(FileRecord *, void *, unsigned long); unsigned long StreamWrite(FileRecord *, const void *, unsigned long); void TimerSet(unsigned long *, unsigned long); int SimHotReadTimer(char *);
class ResMgr { public: void *Load(long, int a=1, int b=0, int c=0); }; extern ResMgr gResMgr;
extern char g513888[], gMenuClip[], gShellFont[], gDefaultFont[]; extern int gScreenWidth, gScreenHeight, gInputPolling, gGameOverDismissed;
void *f_0047c920(ResMgr *, const char *, int, void *, int); unsigned long BriefWaitB(int); void BriefWaitA(int); void SigsStart(void *, int); void MenuDrawImage(void *, void *, int, int); void ReleasePoolData(void *); void GuiResetClip(void *); void f_cba0(void *, const char *); void f_cbf0(void *); void MenuDrawTextAt(void *, int, int, int, char *); void SurfacePresent(void *); void ResetFrameClock(void); void InputPoll(void *); void VoicePlay(int, int); void SurfaceFlush(void *); void ShellServiceFrame(void *); void ShellLimitFrameRate(void *); void BriefCloseWait(void); void ShellRestore(void *); void SimSetMenuState(void *, int);
extern FogBuffer *gFogBuffers[9], *gFogBufferIndex[10]; extern int gFogBufferInitialized; extern FogBuffer gFogBufferSource; extern ViewState gFogDefaultData;
extern int retryTimeout; extern char modeCheckName[]; extern unsigned long gFogTimer;void SimHotResizeFogBuffers(void)
{
    int offset = 0;
    ViewState *destination = gViewState;
    do {
        (*(FogBuffer **)((char *)gFogBuffers + offset))->Resize();
        ViewState *source = &gFogDefaultData;
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
FogCopy(&gFogBufferSource, gFogBufferIndex[index]);
gFogDefaultData = gViewState[index - 1];
}

void SimHotWriteFogBuffers(int index)
{
FogCopy(gFogBufferIndex[index], &gFogBufferSource);
gViewState[index - 1] = gFogDefaultData;
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
    ViewState *destination = gViewState;
    do {
        (*(FogBuffer **)((char *)gFogBuffers + offset))->Resize();
        ViewState *source = &gFogDefaultData;
        FogCopy(*(FogBuffer **)((char *)gFogBuffers + offset), &gFogBufferSource);
        *destination = *source;
        ++destination;
        offset += 4;
    } while (offset != 36);
    gFogBufferInitialized = 1;
    for (offset = 0; offset != 36; offset += 4)
        FogRead(*(FogBuffer **)((char *)gFogBuffers + offset), stream);
    StreamRead(stream, gViewState, 0x120);
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
    StreamWrite(stream, gViewState, 0x120);
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
    char *image = (char *)AllocateMissionImage(gHotbriefStrings, 0x1a2, 1);
    char *name = gMissionRecords[index].name;
    char *loaded = BriefFormat(image, name);
    f_00477295(loaded);
    AssertExit(0);
}
struct ActorRecord; class ProfileManager { public: void *Mission(int); }; struct HotseatState {};
extern ProfileManager profileManager; extern HotseatState *gShellHotseat; extern char *searchPaths[]; extern char gHotbriefStrings[];
void f_0043dcfd(ProfileManager *, int); void f_0043ef34(ProfileManager *); int HasMission(ProfileManager *); int FileExists(const char *); int LoadMission(const char *, int);
void AssertFail(const char *, int, const char *);
#define ASSERT(test) if (!(test)) AssertFail(__FILE__, __LINE__, 0)
extern char DAT_0048b5ac[], DAT_0048b5ae[], DAT_0048b5b2[], DAT_0048b5b4[];
extern short hotseatMapSizes[]; extern char g511ddc[]; extern char DAT_004d028b[]; extern int DAT_004d030f, DAT_004d3551, DAT_004d3555, DAT_004d3559, DAT_004c975c, DAT_004fb940;
extern unsigned long DAT_004c9354[], DAT_004f6600; extern char g4dd0c0[];
int f_00450930(void *, int, void *); char *f_0047c350(void *, int, int); void MissionRecordInitialize(void *, int, int, int, int, int); void MissionDataInit(void *, int, int, int, int, int, int);
void HotbriefEmpty(void); void *LogicActorBuild(void *, void *); void BestLocSetData(void *, void *, void *); struct FastBuffer; void FastBufferReset(FastBuffer *); extern FastBuffer g4d35e4;
ActorRecord *ActorGetByIndex(int); void *GetModePlayer(void *, int); void *f_0043c6ed(void *, int); 
struct ActorBlobA { unsigned char bytes[0x10b]; }; struct ActorListObject { unsigned char bytes[0x7e]; ActorListObject(); };
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
inline int MissionIsActive(int *mission) { return *mission; }
void SimHotStartMission(int *mission)
{
    int result, slot, pathOffset, briefOffset, recordOffset;
    int localPosition[2]; int localRecord[0x16]; char path[256]; int packedSize, width, height;
    int ownerIndex, searchIndex, found, rawOwner;
    char *missionName;
    char *missionOwner;
    char *logicData;
    ActorRecord *actor;
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
    ASSERT(MissionIsActive(mission) != 0 && gShellHotseat != 0);
    for (int fogIndex = 0; fogIndex != 9; ++fogIndex) {
        gFogBuffers[fogIndex]->Resize(); FogCopy(gFogBuffers[fogIndex], &gFogBufferSource); gViewState[fogIndex] = gFogDefaultData;
    }
    gFogBufferInitialized = 1;
    briefOffset = 0; recordOffset = 0; missionName = (char *)mission + 0x2ee; missionOwner = (char *)mission + 0x2de;
    for (slot = 0; slot < *(int *)((char *)gShellHotseat + 0x4c); ++slot) {
        result = f_00450930(mission, 1, 0);
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
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
    char *poolText = f_0047c350(g511ddc, 0x9d, 0);
    strcpy((char *)mission + ((*(int *)((char *)mission + 0x98a) - 1) * 0xbc) + 0x2ee, poolText);
    packedSize = (unsigned short)hotseatMapSizes[*(int *)((char *)gShellHotseat + 0x12c0)];
    width = (hotseatMapSizes[*(int *)((char *)gShellHotseat + 0x12c0)] >> 8) - 1;
    height = (packedSize & 0xf) - 1;
    MissionRecordInitialize(localRecord, 8, width, height, 1, 1);
    MissionDataInit(localRecord, 3, 0, 0, *(int *)((char *)gShellHotseat + 0x12b0), *(int *)((char *)gShellHotseat + 0x12b4), *(int *)((char *)gShellHotseat + 0x12a0));
    localRecord[7] = 0x67; HotbriefEmpty(); logicData = (char *)LogicActorBuild(g4dd0c0, localRecord);
    slot = 0; pathOffset = 0; recordOffset = 0;
    for (;;) {
        if (slot >= *(int *)((char *)gShellHotseat + 0x4c)) {
            DAT_004c975c = 0; memcpy(DAT_004c9354, localRecord, 0x58);
            gFogBufferSource.Initialize(*(int *)((char *)gShellHotseat + 0x12a4), 1);
            DAT_004d3551 = 1; DAT_004d3555 = *(int *)((char *)gShellHotseat + 0x12c4); DAT_004d3559 = *(int *)((char *)gShellHotseat + 0x12c8);
            DAT_004f6600 = *(unsigned long *)((char *)gShellHotseat + 0x12bc); FastBufferReset(&g4d35e4); *(int *)((char *)mission + 0x2ba) = 0; return;
        }
        *mission = 0; DAT_004fb940 = 0; f_0043dcfd(&profileManager, 3);
        if (*(int *)((char *)gShellHotseat + 0x1218 + slot * 4) != 0) {
            result = LoadMission((char *)gShellHotseat + 0x218 + pathOffset, 1);
            if (result == 0) { char *failure = BriefFormat(f_0047c350(gHotbriefStrings, 0x1a2, 1), (char *)gMissionRecords + recordOffset + 16); f_00477295(failure); AssertExit(0); }
        } else {
            found = 0; searchIndex = 0;
            do {
                char *searchPath = searchPaths[searchIndex];
                if (searchPath == 0) break;
                sprintf(path, "%sqstart\\MP%02d_%c.CBM", searchPath, *(int *)((char *)gShellHotseat + 0x1278 + slot * 4), *(int *)((char *)gShellHotseat + 0x1238 + slot * 4) + 0x40);
                if (FileExists(path) != 0) found = 1; ++searchIndex;
            } while (found == 0);
            if (found == 0 || LoadMission(path, 1) == 0) { char *failure = BriefFormat(f_0047c350(gHotbriefStrings, 0x1a2, 1), (char *)gMissionRecords + recordOffset + 16); f_00477295(failure); AssertExit(0); }
        }
        if (HasMission(&profileManager) == 0) { char *failure = BriefFormat(f_0047c350(gHotbriefStrings, 0x1a2, 1), (char *)gMissionRecords + recordOffset + 16); f_00477295(failure); AssertExit(0); }
        f_0043ef34(&profileManager); f_0043dcfd(&profileManager, 1); *mission = 1;
        BestLocSetData((void *)fallbackName, localPosition, (void *)(*(int *)logicData));
        if (*(int *)((char *)gShellHotseat + 0x12a0) != 0) {
            actor = ActorGetByIndex(slot + 1);
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
            ASSERT(actor != 0 && *(char *)(*(int *)((char *)actor + 0x18) + 0x12) == 0x1f);
            *(int *)((char *)actor + 8) = slot + 1; void *player = GetModePlayer(mission, slot + 1); *(unsigned char *)((char *)actor + 0x5a) = *(unsigned char *)((char *)player + 0xb4);
        }
        ownerIndex = 0;
        for (int flagIndex = 0; flagIndex < DAT_004d030f; ++flagIndex) {
            if (DAT_004d028b[flagIndex * 2] != 0) {
                void *unitData = profileManager.Mission(ownerIndex + 1); void *ownerData = f_0043c6ed(&profileManager, (int)unitData);
                void *actorCopy = new ActorBlobA; void *ownerObject = new ActorListObject;
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
                ASSERT(actorCopy != 0 && ownerObject != 0);
                *(ActorBlobA *)actorCopy = *(ActorBlobA *)unitData;
                *(ActorListObject *)ownerObject = *(ActorListObject *)ownerData;
                actor = (ActorRecord *)ActorListAdd((void *)(slot + 1), actorCopy, ownerObject, localPosition, 1, 0);
// Original assertion source-line padding.
                ASSERT(actor != 0 && actorCopy != 0 && ownerObject != 0);
                LogicActorDelete(g4dd0c0, (int)actor); ((int *)actor)[4] = -9999; ActorActivate(actor);
            }
            ++ownerIndex;
        }
        pathOffset += 0x200; recordOffset += 0xbc; logicData += 4; ++slot;
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
ActorRecord *ActorFindFree(void);
ActorRecord *ActorFindNext(ActorRecord *);
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
        if (gFogUpdateCount++ % 10 == 0) {
            int seen[10];
            memset(seen, 0, sizeof(seen));
            int count = 0;
            int lastOwner = 0;
            for (ActorRecord *actor = ActorFindFree(); actor != 0; actor = ActorFindNext(actor)) {
                if (((int *)actor)[2] > 0 && SimHotbriefCheckActor(actor) != 0 && ((signed char *)actor)[0x88] != 0) {
                    int owner = ((int *)actor)[2];
                    if (seen[owner] == 0) { lastOwner = owner; ++count; ++seen[owner]; }
                }
            }

            if (count == 2 && seen[*(int *)((char *)mission + 0x98a)] != 0)
                *(int *)((char *)mission + 0x10f5) = 0;
            if (count > 1 && seen[*(int *)((char *)mission + 0x986)] == 0)
                SimHotbriefRemove((int)mission, 0, *(int *)((char *)mission + 0x986));
            if (count == 1)
                return lastOwner;
        }
    }
    return 0;
}

inline long GameOverBackgroundName(const char *name) { viewportClipLeft = gBriefDrawSource; return (long)name; }
void SimHotBriefGameOver(void *mission)
{
    unsigned long previousDelay = BriefWaitB(0x1f);
    BriefWaitA(0);
    SigsStart(g513888, 0);
    void *image = gResMgr.Load((viewportClipLeft = gBriefDrawSource, (long)"BGREDHEX.BMX"));
    MenuDrawImage(g513888, (char *)image + 4,
                  (gScreenWidth - 0x280) / 2, (gScreenHeight - 0x1e0) / 2);
    ReleasePoolData(image);
    GuiResetClip(gMenuClip);
    int halfHeight = gScreenHeight / 2;
    int halfWidth = gScreenWidth / 2;
    gMenuTextMap = shellDefaultTextMap;
    gMenuTextColor = shellDefaultTextColor;
    f_cba0(gShellFont, "font3.fnx");
    char *message = (char *)AllocateMissionImage(gHotbriefStrings, 0x19c, 1);
    MenuDrawTextAt(gDefaultFont, halfWidth, halfHeight - 0x14, 0, message);
    MenuDrawTextAt(gShellFont, halfWidth, halfHeight + 0x14, 0,
               (char *)(*(int *)((char *)mission + 0x97a) + 0x10));
    f_cbf0(gShellFont);
    SurfacePresent(g513888);
    ResetFrameClock();
    gInputPolling = 1;
    InputPoll((char *)mission + 8);
    VoicePlay(0xb, 500);
    VoicePlay(0xb, 500);
    VoicePlay(0xb, 500);
    for (;;) {
        gInputPolling = 1;
        InputPoll((char *)mission + 8);
        if (gGameOverDismissed != 0 ||
            ((HotseatInput *)((char *)mission + 8))->Pressed(0x50,0x40) ||
            ((HotseatInput *)((char *)mission + 8))->Pressed(0xa0,0x80))
            break;
        SurfaceFlush(g513888);
        ShellServiceFrame(mission);
        ShellLimitFrameRate(mission);
    }
    BriefCloseWait();
    SigsStart(g513888, 0);
    SurfacePresent(g513888);
    ShellRestore(mission);
    BriefWaitB(previousDelay);
    SimSetMenuState(mission, 1);
}






