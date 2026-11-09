#include <string.h>

struct GuiControl;
struct GuiPoint {};
extern GuiControl *gHotbriefResourceList[];
extern GuiPoint gHotbriefMousePoint;
extern int gHotbriefHit;
extern int gDrawRemap;
extern char gShellFont[];
extern char gShellFontBefore[];
extern char g4b74f4[];
extern unsigned long gBriefTimer;
extern char g513888[];
extern char gMenuClip[];
extern char gHotbriefPool[];
extern char DAT_00511e54[];
extern char DAT_00511e7c[];
extern char gResourceManager[];
extern char gMissionNamePool[];
extern int gHotbriefPanelState;
extern int gInputPolling;
extern int g5138ac;
extern int g5138b0;
extern int playerCount;
extern int gRemoteActorKind;
extern int gHotbriefStringIndex;
extern int gHotbriefCounter;
extern int gMenuTextColor;
extern char *gMenuTextMap;
extern int viewportClipLeft;
extern char gBriefDrawSource[];
extern char gBriefTextMap[];
extern int gBriefTextColor;
extern char DAT_004d274e[];
extern char targetBriefResource[];
extern void AssertFail(const char *, int, const char *);
#define ASSERT(test) if (!(test)) AssertFail(__FILE__, __LINE__, 0)
void f_cb80(void *);
void f_cbf0(void *);
void f_0041cab0(void *);
void ReleasePoolData(void *);
void GuiFadeOut(void);
void ShellClearFrame(void);
int f_0042604d(void *, GuiPoint *);
int f_00425ba1(void *);
void SigsStart(void *, int);
void SurfacePresent(void *);
void GuiResetClip(void *);
void *ActorGetByIndex(int);
void *f_00477798(int);
unsigned long f_00476512(int);
void f_cba0(void *, const char *);
char *f_0047c350(void *, int, int);
void *f_004259f7(void *, const char *);
void *f_00425f87(void *, void *, void *, int, int);
void *f_0047c920(void *, const char *, int, void *, int);
void f_d2c0(void *);
void HercSetMissionStart(void *, int);
void MenuDrawImage(void *, void *, int, int);
void f_0047ddf0(void);
void InputPoll(void *);
void f_0047c240(void *);
void f_0047cfc0(void *, int, int, int, char *);
void f_00475887(void);
void f_004758ac(void);
int f_0047e920(const char *);
char *BriefFormat(void *);
void f_00477295(char *);
void f_00477295(void *);
void *AllocateMissionImage(void *, int, int);
void HotbriefSetup(void *);
void f_00450b00(void *);
void f_00450aa0(void *, int);
void TimerSet(unsigned long *, unsigned long);
int TimerExpired(unsigned long *);
void MenuDrawImage(void *, void *, int, int);
void MenuSetClip(void *, int, int, int, int);
void BriefRenderUpdate(void *);
int f_00438678(int);
char *f_00438374(int, int, int);
void f_0042b290(void *, void *, int);
int f_0042b420(int *, int *);
unsigned long f_0047e160(void *);
void f_0045a9f0(void *, void *, int, int);
int f_004394d8(void);
int f_0045d830(void *, void *, int, int);
void f_00452950(void *, void *, void *, int);
int f_0044eb00(void *);
void f_00451370(void *);
void *ActorFindByOwner(void *);
void *ActorFindOwnerNext(void *, void *);
void *f_00456b20(void);
void *f_00456bb0(void *);
int f_00464956(void *);
void *f_004641ac(void *);
void *f_004641bb(void *, void *);
void *f_00463c56(void *);
char *f_00463be8(void *);
void f_00463c22(void *);
int f_004771aa(const char *);
extern unsigned char DAT_004a3a0c[];
extern unsigned char DAT_004c9265[];
extern int DAT_004d2dea;
extern int DAT_004d2de6;
extern int DAT_004d3551;
extern int DAT_004dd0b4;
extern int DAT_004dd0b8;
extern int DAT_004f65e8;
extern char DAT_004fb944[];
extern char DAT_00511ddc[];
extern char DAT_0048b5ac[];
extern char DAT_0048b5ae[];
extern char DAT_0048b5b2[];
extern char DAT_0048b5b4[];
extern char DAT_0051445c[];
extern int gHotbriefMissionFlags;

void HotbriefSetup(void *screen)
{
    SigsStart(g513888, 0);
    SurfacePresent(g513888);
    GuiResetClip(gMenuClip);
    struct DrawCopy { unsigned long words[4]; };
    *(DrawCopy *)&viewportClipLeft = *(DrawCopy *)gBriefDrawSource;
    gDrawRemap = 0;
    *(int *)((char *)screen + 0x50) = (g5138ac - 0x280) / 2;
    *(int *)((char *)screen + 0x54) = (g5138b0 - 0x1e0) / 2;
    *(int *)((char *)screen + 0x58) = 0;
    *(int *)((char *)screen + 0x5c) = *(int *)((char *)screen + 0x58);
    if (*(void **)((char *)screen + 0x60) != 0)
        *(int *)((char *)screen + 0x5c) = **(int **)((char *)screen + 0x60);
    gMenuTextMap = gBriefTextMap;
    gMenuTextColor = gBriefTextColor;
    char *title = f_0047c350(targetBriefResource, 0x5e2, 0);
    title = BriefFormat(title);
    f_0047cfc0(gShellFont, *(int *)((char *)screen + 0x50) + 400,
               *(int *)((char *)screen + 0x54) + 0x1b3, 0, title);
    gMenuTextMap = gBriefTextMap;
    gMenuTextColor = gBriefTextColor;
    title = f_0047c350(targetBriefResource, 0x5e3, 0);
    f_0047cfc0(gShellFontBefore, *(int *)((char *)screen + 0x50) + 10,
               *(int *)((char *)screen + 0x54) + 0x4b, -1, title);
    title = f_0047c350(targetBriefResource, 0x5e4, 0);
    f_0047cfc0(gShellFontBefore, *(int *)((char *)screen + 0x50) + 0xb9,
               *(int *)((char *)screen + 0x54) + 0x4b, 1, title);
    int ownerCounts[9];
    int aiOwnerCounts[9];
    memset(ownerCounts, 0, sizeof(ownerCounts));
    memset(aiOwnerCounts, 0, sizeof(aiOwnerCounts));
    for (int actorId = 1; ; ++actorId) {
        char *actor = (char *)ActorGetByIndex(actorId);
        if (actor == 0)
            break;
        int owner = *(int *)(actor + 8) - 1;
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
        if (owner < 0 || owner > 8)
            ASSERT(owner >= 0 && owner < 9);
        ++ownerCounts[owner];
        if (actor[0x88] == 0)
            ++aiOwnerCounts[owner];
    }
    int row = 0;
    int yOffset = 0;
    char *rowName = (char *)0x004bc588;
    char *missionNames = DAT_004d274e;
    for (; row < playerCount; ++row) {
        if (ownerCounts[row] == 0) {
            if (row != gRemoteActorKind - 1) {
                gMenuTextColor = gBriefTextColor;
                gMenuTextMap = gBriefTextMap;
                f_0047cfc0(gShellFontBefore,
                           *(int *)((char *)screen + 0x50) + 10,
                           *(int *)((char *)screen + 0x54) + 100 + yOffset,
                           -1, missionNames);
                title = f_0047c350(targetBriefResource, 0x5e5, 0);
                f_0047cfc0(gShellFontBefore,
                           *(int *)((char *)screen + 0x50) + 0xb9,
                           *(int *)((char *)screen + 0x54) + 100 + yOffset,
                           1, title);
            }
        } else {
            gMenuTextMap = gBriefTextMap;
            gMenuTextColor = gBriefTextColor;
            f_0047cfc0(gShellFontBefore,
                       *(int *)((char *)screen + 0x50) + 10,
                       *(int *)((char *)screen + 0x54) + 100 + yOffset,
                       -1, missionNames);
            title = BriefFormat((char *)0x0048b5a7);
            f_0047cfc0(gShellFontBefore,
                       *(int *)((char *)screen + 0x50) + 0xb9,
                       *(int *)((char *)screen + 0x54) + 100 + yOffset,
                       1, title);
        }
        yOffset += 0x16;
        rowName += 4;
        missionNames += 0xbc;
    }
}

class MenuButton {
public:
    char objectBytes[40];
    MenuButton(const char *, unsigned long);
    ~MenuButton();
};
class MenuSpacer {
public:
    char objectBytes[32];
    MenuSpacer();
    ~MenuSpacer();
};
class MenuHelpRegion {
public:
    char objectBytes[40];
    MenuHelpRegion();
    ~MenuHelpRegion();
};
class MenuPanel {
public:
    char objectBytes[36];
    MenuPanel();
    ~MenuPanel();
};
class GuiScreen {
public:
    char objectBytes[36];
    GuiScreen(GuiControl *, unsigned int, unsigned long);
    long HitTest(GuiPoint *);
    int Update();
};
class HotbriefResources : public GuiScreen {
public:
    char resourceFields[0x40];
    HotbriefResources();
    ~HotbriefResources();
};
HotbriefResources::HotbriefResources()
    : GuiScreen((GuiControl *)gHotbriefResourceList, 5, 0)
{
}

MenuButton gBriefMenuButton(0, 0);
MenuSpacer gBriefMenuSpacerA;
MenuSpacer gBriefMenuSpacerB;
MenuHelpRegion gBriefHelpRegion;
MenuPanel gBriefMenuPanel;
HotbriefResources gHotbriefResources;

void HotbriefRelease(void *screen)
{
    int active = *(int *)((char *)screen + 0x10) != 0;
    if (active) {
        if (*(void **)((char *)screen + 0x60) != 0)
            ReleasePoolData(*(void **)((char *)screen + 0x60));
        *(int *)((char *)screen + 0x60) = 0;
        f_cbf0(gShellFont);
        f_0041cab0(g4b74f4);
        *(int *)((char *)screen + 0x10) = 0;
        GuiFadeOut();
        ShellClearFrame();
    }
}

void HotbriefUpdate(void *screen)
{
    int active = *(int *)((char *)screen + 0x10) != 0;
    if (active) {
        gHotbriefHit = f_0042604d(screen, &gHotbriefMousePoint);
        f_00425ba1(screen);
    }
}

struct HotbriefTimerInit {
    HotbriefTimerInit() { TimerSet(&gBriefTimer, 0); }
};

void HotbriefTimerUpdate(void *screen)
{
    int active = *(int *)((char *)screen + 0x10) != 0;
    if (active) {
        static HotbriefTimerInit timerInit;
        if (*(int *)((char *)screen + 0x5c) != 0) {
            int expired = TimerExpired(&gBriefTimer);
            if (expired != 0) {
                TimerSet(&gBriefTimer, 100);
                MenuDrawImage(g513888,
                              ((char *)*(void **)((char *)screen + 0x60) + 4) +
                                  *(int *)((char *)screen + 0x58) * 0xc,
                              *(int *)((char *)screen + 0x50) + 0xf4,
                              *(int *)((char *)screen + 0x54) + 0x3c);
                MenuSetClip(gMenuClip,
                            *(int *)((char *)screen + 0x50) + 0xf4,
                            *(int *)((char *)screen + 0x54) + 0x3c,
                            0x18b, 0x16d);
                *(int *)((char *)screen + 0x58) =
                    (*(int *)((char *)screen + 0x58) + 1) %
                    *(int *)((char *)screen + 0x5c);
            }
        }
        BriefRenderUpdate(screen);
    }
}

void HotbriefOpen(void *screen, void *input, int mission)
{
    if (*(int *)((char *)screen + 0x10) == 0) {
        f_00477798(0);
        f_00476512(2);
        *(int *)((char *)screen + 0x4c) = mission;
        gHotbriefPanelState = -1;
        f_004259f7(screen, "hotbrief.box");
        f_00425f87(screen, input, DAT_00511e54, 0x5dd, 1);
        *(int *)((char *)screen + 0x60) =
            (int)f_0047c920(gResourceManager, "HOTSEAT.BMX", 1, 0, 0);
        f_cba0(gShellFont, "font3.fnx");
        f_d2c0((void *)0x00513958);
        HercSetMissionStart(g4b74f4, 0x979);
        HotbriefSetup(screen);
        for (int offset = 0; offset != 0x14; offset += 4) {
            int *point = *(int **)((char *)gHotbriefResourceList + offset);
            point[0] += *(int *)((char *)screen + 0x50);
            point[1] += *(int *)((char *)screen + 0x54);
        }
    }
}

void HotbriefMainLoop(void *mission, int selection)
{
    char title[100];
    char *missionTitle = f_0047c350(
        gMissionNamePool, *(int *)((char *)mission + 0x2d6) + 0x28, 1);
    strcpy(title, missionTitle);
    char *message = (char *)AllocateMissionImage(DAT_00511e7c, 0x1a4, 1);
    message = BriefFormat(message);
    f_00477295(message);
    HotbriefOpen(&gHotbriefResources, (char *)mission + 8, selection);
    f_0047ddf0();
    for (;;) {
        gInputPolling = 1;
        InputPoll((char *)mission + 8);
        int result;
        if (*(int *)((char *)&gHotbriefResources + 0x10) == 0) {
            result = 0;
        } else {
            gHotbriefHit = f_0042604d(&gHotbriefResources, &gHotbriefMousePoint);
            result = f_00425ba1(&gHotbriefResources);
        }
        if (result != 0)
            break;
        HotbriefTimerUpdate(&gHotbriefResources);
        f_0047c240(g513888);
        f_00475887();
        f_004758ac();
    }
    if (*(int *)((char *)&gHotbriefResources + 0x10) != 0) {
        void *poolData = *(void **)((char *)&gHotbriefResources + 0x60);
        if (poolData != 0)
            ReleasePoolData(poolData);
        *(void **)((char *)&gHotbriefResources + 0x60) = 0;
        f_cbf0(gShellFont);
        f_0041cab0(g4b74f4);
        *(int *)((char *)&gHotbriefResources + 0x10) = 0;
        GuiFadeOut();
        ShellClearFrame();
    }
}

void HotbriefClearActive(void *screen)
{
    *(int *)((char *)screen + 0x10) = 0;
}

int HotbriefCanSelectAction(void *actor, int rankLimit)
{
    if (*(int *)(*(int *)((char *)actor + 0x1c) + 0x12) > 0) {
        int mission = *(int *)((char *)actor + 0x18);
        char *slot = (char *)mission + 0x69;
        int skill = f_00438678(mission);
        for (int index = 0, row = 0xb; index < 8; ++index, ++row, slot += 8) {
            if (slot[1] && slot[2] &&
                ((*(unsigned char *)((char *)actor + 0x8e) & (1 << index)) != 0)) {
                char *record = f_00438374(mission, row, -1);
                if ((unsigned char)slot[3] < (signed char)record[0x20] &&
                    *(unsigned char *)(0x004c9265 + (signed char)record[0x1f]) &&
                    (*(int *)(record + 0x13) == 0 || *(int *)(slot + 4) != 0) &&
                    (DAT_004d2dea == *(int *)((char *)actor + 8) ||
                     *(int *)(record + 0x2b) <= skill) &&
                    rankLimit <= (signed char)record[0x21])
                    return 1;
            }
        }
    }
    return 0;
}

int HotbriefCheckAction(void *actorA, void *actorB, int location,
                        int priorLocation, int *target)
{
    if (!((char *)actorB)[0x22])
        return 0;
    unsigned long actorLocation[2];
    f_0042b290((void *)0x004d3594, actorLocation, location);
    int locationRank = f_0042b420((int *)actorLocation,
                                  (int *)((char *)actorB + 0x3a));
    if (locationRank >= 0x15)
        return 0;
    int chance = *(int *)(0x004a3a0c + locationRank * 4);
    if (chance == 0)
        return 0;
    if (chance != 100 && chance < (int)(f_0047e160(DAT_0051445c) % 100))
        return 0;
    if (priorLocation != 0) {
        unsigned long previous[2];
        f_0042b290((void *)0x004d3594, previous, priorLocation);
        if (f_0042b420((int *)previous,
                       (int *)((char *)actorB + 0x3a)) < locationRank)
            return 0;
    }
    int bestRank = locationRank;
    int bestSkill = 0;
    int mission = *(int *)((char *)actorB + 0x18);
    int skill = 0;
    if (*(int *)(*(int *)((char *)actorB + 0x1c) + 0x12) > 0) {
        char *slot = (char *)mission + 0x69;
        for (int index = 0, row = 0xb; index < 8; ++index, ++row, slot += 8) {
            if (slot[1] && slot[2] &&
                ((*(unsigned char *)((char *)actorB + 0x8e) & (1 << index)) != 0)) {
                char *record = f_00438374(mission, row, -1);
                if ((unsigned char)slot[3] < (signed char)record[0x20] &&
                    *(unsigned char *)(0x004c9265 + (signed char)record[0x1f]) &&
                    (*(int *)(record + 0x13) == 0 || *(int *)(slot + 4) != 0) &&
                    (DAT_004d2dea == *(int *)((char *)actorB + 8) ||
                     *(int *)(record + 0x2b) <= skill) &&
                    bestRank <= (signed char)record[0x21]) {
                    bestSkill = 1;
                    break;
                }
            }
        }
    }
    if (!bestSkill)
        return 0;
    f_0045a9f0((void *)0x004d2460, target, *(int *)actorB, -*((int *)actorA));
    if (target[0x2b] < 0x3c)
        return 0;
    return 1;
}

void HotbriefProcessOpposingActions(void *mission, int *actor, int location,
                                    int priorLocation)
{
    DAT_004dd0b8 = 0;
    int owner = DAT_004f65e8;
    if (*(int *)((char *)mission + 0x2c2) == 0) {
        owner = DAT_004dd0b4;
        if (*(int *)((char *)mission + 0x10f1) != 0)
            owner = *(int *)((char *)mission + 0x10f9);
    }
    if (owner != 0 && actor[2] == *(int *)((char *)mission + 0x97e) &&
        (char)actor[0x22]) {
        int *other = (int *)f_00456b20();
        if (other != 0) {
            int *targetRecord = (int *)((char *)mission + 0x9a6);
            int targetData[44];
            do {
                if (actor[2] != other[2]) {
                    int valid = 0;
                    if ((char)other[0x22]) {
                        unsigned long actorLocation[2];
                        f_0042b290((void *)0x004d3594, actorLocation, location);
                        int rank = f_0042b420((int *)actorLocation,
                                              (int *)((char *)other + 0x3a));
                        if (rank < 0x15) {
                            int chance = *(int *)(0x004a3a0c + rank * 4);
                            if (chance != 0 &&
                                (chance == 100 || (int)(f_0047e160(DAT_0051445c) % 100) <= chance)) {
                                if (priorLocation != 0) {
                                    unsigned long previous[2];
                                    f_0042b290((void *)0x004d3594, previous, priorLocation);
                                    if (f_0042b420((int *)previous,
                                                   (int *)((char *)other + 0x3a)) < rank)
                                        goto consider_next;
                                }
                                if (*(int *)(other[7] + 0x12) > 0) {
                                    int missionData = other[6];
                                    char *slot = (char *)missionData + 0x69;
                                    int skill = f_00438678(missionData);
                                    for (int index = 0, row = 0xb; index < 8;
                                         ++index, ++row, slot += 8) {
                                        if (slot[1] && slot[2] &&
                                            ((*(unsigned char *)((char *)other + 0x8e) & (1 << index)) != 0)) {
                                            unsigned int recordId = f_004394d8();
                                            char *record = f_00438374(missionData, row, recordId);
                                            if ((unsigned char)slot[3] < (signed char)record[0x20] &&
                                                *(unsigned char *)(0x004c9265 + (signed char)record[0x1f]) &&
                                                (*(int *)(record + 0x13) == 0 || *(int *)(slot + 4) != 0) &&
                                                (DAT_004d2dea == other[2] || *(int *)(record + 0x2b) <= skill) &&
                                                rank <= (signed char)record[0x21]) {
                                                f_0045a9f0((void *)0x004d2460, targetData,
                                                           *other, -*actor);
                                                if (targetData[0x2b] >= 0x3c)
                                                    valid = 1;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
consider_next:
                    if (valid) {
                        DAT_004dd0b8 = 1;
                        int index;
                        while ((index = f_0045d830(targetRecord, targetData,
                                                   0x23, 0)) >= 0) {
                            f_00452950(mission, other, actor, index);
                            DAT_004dd0b8 = 0;
                            if (actor[2] == *(int *)((char *)mission + 0x986) &&
                                f_0044eb00(mission) != 0) {
                                ((unsigned char *)mission)[0x2c] |= 0x20;
                                f_00451370(mission);
                                ((unsigned char *)mission)[0x2c] &= 0xdf;
                            }
                            if (!(char)actor[0x22])
                                return;
                        }
                    }
                }
                other = (int *)f_00456bb0(other);
            } while (other != 0);
        }
    }
    DAT_004dd0b8 = 0;
}

void HotbriefEmpty(void)
{
}

