extern "C" { void *memset(void *, int, unsigned int); char *strcpy(char *, const char *); } struct GuiControl; static inline int HotbriefHealth(int *info) { return *(int *)((char *)info + 0x12) > 0; } struct GuiPoint {}; struct HotbriefPoint { int x, y; void Move(int dx, int dy) { x += dx; y += dy; } }; static inline int HotbriefIsActive(void *s) { return ((int *)s)[4] != 0; }
extern GuiControl *gHotbriefResourceList[]; extern GuiPoint gHotbriefMousePoint; extern int gHotbriefHit;
extern int gDrawRemap; extern char gShellFont[]; extern char gShellResources[]; extern char fallbackName[], modeCheckName[]; extern char gShellFontBefore[];
extern char g4b74f4[]; extern unsigned long gBriefTimer; extern char g513888[];
extern char gMenuClip[]; extern char gHotbriefPool[]; extern char DAT_00511e54[];
extern char DAT_00511e7c[]; extern char gResMgr[]; extern char gMissionNamePool[];
extern int gHotbriefPanelState; extern int gInputPolling; extern int g5138ac;
extern int g5138b0; extern int playerCount; extern int gRemoteActorKind;
extern int gHotbriefStringIndex; extern int gHotbriefCounter; extern int gMenuTextColor;
extern char *gMenuTextMap; extern int viewportClipLeft; extern char gBriefDrawSource[];
extern char gBriefTextMap[]; extern int gBriefTextColor; extern char DAT_004d274e[];
extern char targetBriefResource[]; extern void AssertFail(const char *, int, const char *);
#define ASSERT(test) if (!(test)) AssertFail(__FILE__, __LINE__, 0)
void f_cb80(void *); void f_cbf0(void *); void f_0041cab0(void *);
void ReleasePoolData(void *); void GuiFadeOut(void); void ShellClearFrame(void);
int f_0042604d(void *, GuiPoint *); int f_00425ba1(void *); void SigsStart(void *, int);
void SurfacePresent(void *); void GuiResetClip(void *); void *ActorGetByIndex(int);
void *BriefWaitA(int); unsigned long BriefWaitB(int); void f_cba0(void *, const char *);
char *f_0047c350(void *, int, int);  
class ResMgr { public: void *Load(const char *, int, void * = 0, int = 0); }; void f_d2c0(void *); void HercSetMissionStart(void *, int);
void MenuDrawImage(void *, void *, int, int); void f_0047ddf0(void); void InputPoll(void *);
void f_0047c240(void *); void f_0047cfc0(void *, int, int, int, char *); void ShellServiceFrame(void *);
void ShellLimitFrameRate(void *); int f_0047e920(const char *); char *BriefFormat(void *, ...);
void f_00477295(char *); void f_00477295(void *); void *AllocateMissionImage(void *, int, int = 1);
void HotbriefSetup(void *); void f_00450b00(void *); void f_00450aa0(void *, int);
void TimerSet(unsigned long *, unsigned long); int TimerExpired(unsigned long *); void MenuDrawImage(void *, void *, int, int);
void MenuSetClip(void *, int, int, int, int); void BriefRenderUpdate(void *); int f_00438678(int);
char *f_00438374(int, int, int); void f_0042b290(void *, void *, int); int f_0042b420(int *, int *);
unsigned long f_0047e160(void *); void f_0045a9f0(void *, void *, int, int); int f_004394d8(void);
int f_0045d830(void *, void *, int, int); void f_00452950(void *, void *, void *, int); int f_0044eb00(void *);
void f_00451370(void *); void *ActorFindByOwner(void *); void *ActorFindOwnerNext(void *, void *);
void *f_00456b20(void); void *f_00456bb0(void *); int f_00464956(void *);
void *f_004641ac(void *); void *f_004641bb(void *, void *); void *f_00463c56(void *);
char *f_00463be8(void *); void f_00463c22(void *); int f_004771aa(const char *);
extern unsigned char DAT_004a3a0c[]; extern unsigned char DAT_004c9265[]; extern int DAT_004d2dea;
extern int DAT_004d2de6; extern int DAT_004d3551; extern int DAT_004dd0b4;
extern int DAT_004dd0b8; extern int DAT_004f65e8; extern char DAT_004fb944[];
extern char DAT_00511ddc[]; extern char DAT_0048b5ac[]; extern char DAT_0048b5ae[];
extern char DAT_0048b5b2[]; extern char DAT_0048b5b4[]; extern char DAT_0051445c[];
extern int gHotbriefMissionFlags; extern char g511dc0[], hotbriefPlayerMaps[], hotbriefCountFormat[]; extern int hotbriefPlayerColors[];


class MenuButton { public:
    char objectBytes[40]; MenuButton(const char *, unsigned long); ~MenuButton();
};
class MenuSpacer { public:
    char objectBytes[32]; MenuSpacer(); ~MenuSpacer();
};
class MenuHelpRegion { public:
    char objectBytes[40]; MenuHelpRegion(); ~MenuHelpRegion();
};
class MenuPanel { public:
    char objectBytes[36]; MenuPanel(); ~MenuPanel();
};
class GuiScreen { public:
    char objectBytes[36]; GuiScreen(GuiControl *, unsigned int, unsigned long); long HitTest(GuiPoint *); int Update(); void LoadLayout(const char *); void Open(void *, void *, int, int); void Draw();
};
class HotbriefResources : public GuiScreen { public:
    char resourceFields[0x40]; HotbriefResources(); ~HotbriefResources();
};

MenuButton gBriefMenuButton(0, 0);
MenuSpacer gBriefMenuSpacerA;
MenuSpacer gBriefMenuSpacerB;
MenuHelpRegion gBriefHelpRegion;
MenuPanel gBriefMenuPanel;
HotbriefResources gHotbriefResources;

struct HotbriefTimerInit {
    HotbriefTimerInit() { TimerSet(&gBriefTimer, 0); }
};

HotbriefResources::HotbriefResources()
    : GuiScreen((GuiControl *)gHotbriefResourceList, 5, 0)
{
}

void HotbriefOpen(void *screen, void *input, int mission)
{
    if (!HotbriefIsActive(screen)) {
        BriefWaitA(0);
        BriefWaitB(2);
        *(int *)((char *)screen + 0x4c) = mission;
        gHotbriefHit = -1;
        ((GuiScreen *)screen)->LoadLayout("hotbrief.box");
        ((GuiScreen *)screen)->Open(input, targetBriefResource, 0x5dd, 1);
        *(int *)((char *)screen + 0x60) =
            (int)((ResMgr *)gResMgr)->Load("HOTSEAT.BMX", 1);
        f_cba0(gShellFont, "font3.fnx");
        f_d2c0(gShellResources);
        HercSetMissionStart(g4b74f4, 0x979);
        HotbriefSetup(screen);
        for (int offset = 0; offset != 0x14; offset += 4) {
            int *point = *(int **)((char *)gHotbriefResourceList + offset);
            ((HotbriefPoint *)point)->Move(*(int *)((char *)screen + 0x50), *(int *)((char *)screen + 0x54));
        }
    }
}

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

int HotbriefUpdate(void *screen)
{
    if (!HotbriefIsActive(screen)) return 0;
    gHotbriefHit = f_0042604d(screen, &gHotbriefMousePoint);
    return f_00425ba1(screen);
}

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
                int clipX = *(int *)((char *)screen + 0x50) + 0xf4;
                int clipY = *(int *)((char *)screen + 0x54) + 0x3c;
                MenuSetClip(gMenuClip, clipX, clipY, 0x18b, 0x16d);
                *(int *)((char *)screen + 0x58) =
                    (*(int *)((char *)screen + 0x58) + 1) %
                    *(int *)((char *)screen + 0x5c);
            }
        }
        ((GuiScreen *)screen)->Draw();
    }
}

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
        if (owner < 0 || owner > 8)
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
            ASSERT(owner >= 0 && owner < 9);
        ++ownerCounts[owner];
        if (actor[0x88] == 0)
            ++aiOwnerCounts[owner];
    }
    int row = 0;
    int yOffset = 0;
    char *rowName = hotbriefPlayerMaps;
    char *missionNames = DAT_004d274e;
    for (; row < playerCount; ++row) {
        if (ownerCounts[row] == 0) {
            if (row != gRemoteActorKind - 1) {
                gMenuTextMap = rowName; gMenuTextColor = hotbriefPlayerColors[row];
                gMenuTextColor = gBriefTextColor;
                gMenuTextMap = rowName; gMenuTextColor = hotbriefPlayerColors[row];
            gMenuTextMap = gBriefTextMap;
                f_0047cfc0(g511dc0,
                           *(int *)((char *)screen + 0x50) + 10,
                           *(int *)((char *)screen + 0x54) + 100 + yOffset,
                           -1, missionNames);
                title = f_0047c350(targetBriefResource, 0x5e5, 0);
                f_0047cfc0(g511dc0,
                           *(int *)((char *)screen + 0x50) + 0xb9,
                           *(int *)((char *)screen + 0x54) + 100 + yOffset,
                           1, title);
            }
        } else {
            gMenuTextMap = rowName; gMenuTextColor = hotbriefPlayerColors[row];
            gMenuTextMap = gBriefTextMap;
            gMenuTextColor = gBriefTextColor;
            f_0047cfc0(g511dc0,
                       *(int *)((char *)screen + 0x50) + 10,
                       *(int *)((char *)screen + 0x54) + 100 + yOffset,
                       -1, missionNames);
            title = BriefFormat(hotbriefCountFormat, aiOwnerCounts[row]);
            f_0047cfc0(g511dc0,
                       *(int *)((char *)screen + 0x50) + 0xb9,
                       *(int *)((char *)screen + 0x54) + 100 + yOffset,
                       1, title);
        }
        yOffset += 0x16;
        rowName += 4;
        missionNames += 0xbc;
    }
}










void HotbriefMainLoop(void *mission, int selection)
{
    char title[100];
    char *missionTitle = f_0047c350(
        gMissionNamePool, *(int *)((char *)mission + 0x2d6) + 0x28, 1);
    strcpy(title, missionTitle);
    char *message = BriefFormat(AllocateMissionImage(DAT_00511e7c, 0x1a4), title);
    f_00477295(message);
    HotbriefOpen(&gHotbriefResources, (char *)mission + 8, selection);
    f_0047ddf0();
    for (;;) {
        gInputPolling = 1;
        InputPoll((char *)mission + 8);
        if (HotbriefUpdate(&gHotbriefResources)) break;
        HotbriefTimerUpdate(&gHotbriefResources);
        f_0047c240(g513888);
        ShellServiceFrame(mission);
        ShellLimitFrameRate(mission);
    }
    if (HotbriefIsActive(&gHotbriefResources)) {
        if (*(void **)((char *)&gHotbriefResources+0x60))
            ReleasePoolData(*(void **)((char *)&gHotbriefResources+0x60));
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

