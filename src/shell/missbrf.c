struct GuiControl; struct InputReq; struct StrPool; struct Timer; typedef unsigned long U32; class GuiScreen { char pad00[0x10]; public: int active; char pad14[0x40]; GuiScreen(GuiControl **, unsigned, U32); ~GuiScreen(){active=0;} void LoadLayout(long); void Open(InputReq *, StrPool *, long, int); };
void TimerSet(Timer *, U32); struct Timer { U32 words[3]; Timer(U32 n) { TimerSet(this, n); } };
struct Surface; struct Flic { char body[0x80]; Flic(); ~Flic(); int Open(long source, int priority = 0x64, int mode = 0); unsigned long Update(Surface *surface); };
class MissBrief : public GuiScreen { public: Timer timer; Flic animation; int IsActive() const { return active != 0; } void Begin(void *callback, unsigned char number); void LoadBackground(); MissBrief(); };
inline void *operator new(unsigned int, void *place) { return place; }


int PopLoadResource(int owner, const char *descriptor, int count, int flags = 0); extern "C" int BriefObjectRelease(char *object);
extern "C" void BriefCloseWait(); extern "C" void BriefCloseAudio(); extern "C" void BriefCloseScreen(char *object); extern "C" int BriefGetHandle(char *object, void *resource); extern "C" void BriefStateSet(char *object); extern "C" int BriefSwitch(char *object, void *resource);
extern "C" void BriefBlit(void *surface, int a, int b, int c, int d, int e, int f, int g, int h); extern "C" int BriefLookup(int table, int number); extern "C" int BriefMissionFile(int table, int number); extern "C" int BriefFormat(const char *format, ...);
extern "C" int BriefRender(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p); extern "C" void BriefStatus(char *object); extern "C" void BriefRenderUpdate(char *object); extern "C" void BriefDone(const char *name);
extern "C" void BriefWaitA(int value); extern "C" void BriefWaitB(int value); void AssertFail(const char *file, int line, const char *message); extern "C" int BriefOpenFile(char *object, const char *name); extern "C" void BriefOpenResource(char *object, void *callback, void *resource, int a, int b);
extern "C" void BriefSetMode(void *control, int mode); extern "C" void BriefAdjust(int surface, int y); extern "C" void BriefPlayAnimation(char *object, const char *name, int frames, int flags); extern "C" void BriefCopyHeader(void *object, void *format); extern "C" void BriefDrawCurrent(void *surface, int a, int b, int c, int d, int e, int f, int g, int h); struct Palette; void ShellApplyPalette(Palette *data);

void MissBriefDrawMap(MissBrief *screen);

extern int g48ea70, g48ea50, g4bb78c, g513888;
extern int g48ea28[2], g48ea2c[2];
extern int gBriefStatus, gBriefSurface, viewportClipLeft, gBriefDrawSource;
extern StrPool gShellStrings; struct ProfileManager; extern ProfileManager profileManager;
extern int gBriefDrawA, gBriefDrawB, gBriefHeaderFormat;
extern char gBriefDataBuffer[];
extern char gShellBackground[]; extern int g5138ac, g5138b0;
extern char g48a9df[]; extern char g48a9d0[]; extern char g48ea60[];
extern int g48ea1c, g48ea24, g4b3290, g4b32b8, g4b32e0;
extern int g4b3308, g4b3310, g4b3338, g4b3364;
extern "C" void BriefModuleConstruct(void *object);
extern "C" void BriefModuleInitA(void *object, int a, int b);
extern "C" void BriefModuleInitB(void *object);
extern "C" void BriefModuleInitC(void *object, int a, int b);
extern "C" void BriefModuleInitD(void *object, void *surface, int value);
extern "C" void BriefModulePlace(void *object, void *surface, int x, int y);

inline int Read5138ac() { return g5138ac; }
inline int Read5138b0() { return g5138b0; }














MissBrief::MissBrief() : GuiScreen((GuiControl **)&g48ea70, 6, 0), timer(0), animation() {}





/* AssertFail remains on source line 68. */ struct BriefMissionEntry { unsigned long words[11]; };
void MissBrief::Begin(void *callback, unsigned char number)
{
    if (IsActive()) return;
    BriefWaitA(6); BriefWaitB(0x19);
    BriefCloseAudio(); gBriefStatus = -1;
    if (number > 9)





        AssertFail(g48ea60, 68, g48a9d0);
    ((unsigned char *)this)[0x50] = number;
    BriefMissionEntry *entry = (BriefMissionEntry *)BriefLookup((int)&profileManager, number);
    *(BriefMissionEntry *)((char *)this + 0x100) = *entry;
    ((unsigned char *)this)[0x53] = ((unsigned char *)this)[0x101];
    ((unsigned char *)this)[0x51] = ((unsigned char *)this)[0x100] + 1;
    *(int *)((char *)this + 0x12c) = BriefMissionFile((int)&profileManager,
                                                       ((unsigned char *)this)[0x50]);
    *(int *)((char *)this + 0xf0) = 0;
    LoadLayout((long)"missbrf.box");
    register void *callbackCopy = callback;
    Open((InputReq *)callbackCopy, &gShellStrings, 0xa8d, 1);
    BriefSetMode((void *)&gBriefSurface, 0);
    *(int *)((char *)this + 0xf4) = 0;
    LoadBackground();
    BriefBlit(&g513888, 0, 1, 0x140, 0, 0x140, 0, 0x11d, 0x1e0);
    MissBriefDrawMap(this);
    BriefAdjust((int)&gBriefSurface, *(int *)((char *)this + 0xf8) - 0x165);
}

void MissBriefClose(MissBrief *screen)
{
    if (screen->IsActive()) {
        screen->active = 0;
        BriefCloseWait();
        BriefCloseAudio();
        BriefCloseScreen((char *)screen + 0x60);
    }
}

void MissBriefExit(MissBrief *screen)
{
    if (screen->IsActive()) {
        *(int *)((char *)screen + 0xf4) = 1;
        gBriefStatus = BriefGetHandle((char *)screen, &g4bb78c);
        BriefStateSet((char *)screen);
    }
}

void MissBriefUpdate(MissBrief *screen)
{
    if (screen->IsActive()) {
        extern int g511948;
        BriefSwitch((char *)screen + 0x60, &g511948);
        int oldSurface = *(int *)((char *)screen + 0xf0);
        int newSurface = gBriefSurface;
        *(int *)((char *)screen + 0xf0) = newSurface;
        if (oldSurface != newSurface) {
            BriefBlit(&g513888, 1, 0, 0x140, 0, 0x140, 0, 0x11d, 0x1e0);
            MissBriefDrawMap(screen);
            BriefStatus((char *)screen);
            BriefRenderUpdate((char *)screen);
        }
        BriefRenderUpdate((char *)screen);
    }
}

void MissBriefOrphan()
{
    BriefDone("missbrf");
}

void MissBriefDrawMap(MissBrief *screen)
{
    int tileStep = 2;
    for (int i = 0; ; i += 2) {
        int empty = (g48ea28[i] | g48ea2c[i]) == 0;
        BriefRender(g48ea28[i] + 0x140, g48ea2c[i] + 0x1e, 0x11d, 0x179,
                    *(int *)((char *)screen + 0x12c),
                    *(int *)((char *)screen + 0xf0), -1, 6,
                    (int)((char *)screen + 0xf8),
                    (int)((char *)screen + 0xfc), (int)&g48ea50,
                    empty ? 0x10 : 0, tileStep, !empty, 0, 0x7fffffff);
        if (empty)
            break;
    }
}

struct BriefHeader { unsigned long words[4]; };
struct BriefCopyArea { unsigned long words[0x114]; unsigned short tail; };

void MissBrief::LoadBackground()
{
    register MissBrief *self = this; char *name = (char *)BriefFormat("BG_%1d%02d%c.FLX",
                                     ((unsigned char *)self)[0x51],
                                     ((unsigned char *)self)[0x53],
                                     ((unsigned char *)self)[0x52] + 0x41);
    ((Flic *)((char *)self + 0x60))->Open((long)name);
    BriefCloseWait();
    BriefCloseAudio();
    char *region = (char *)self; region += 0x80;
    int regionValue = *(int *)(region + 4);
    if (regionValue == 0)
        return;
    *(BriefHeader *)&viewportClipLeft = *(BriefHeader *)&gBriefDrawSource;
    register char *animation = (char *)self + 0x60;
    ((Flic *)animation)->Update((Surface *)&g511948);
    BriefBlit(&g513888, 0, 1, 0, 0, 0, 0,
              Read5138ac(), Read5138b0());
    char *source = *(char **)(animation + 0x3c);
    *(BriefCopyArea *)gShellBackground = *(BriefCopyArea *)source;
    ShellApplyPalette((Palette *)gShellBackground);
    *(int *)(animation + 0x4c) = -1;
}


/* Real global objects supply Watcom's constructor-state stores. The opaque
 * layouts here preserve the shared widgets' observed sizes. */
#pragma aux brief_button_constructor "BriefModuleInitA";
#pragma aux brief_help_constructor "BriefModuleInitB";
#pragma aux brief_range_constructor "BriefModuleInitC";
#pragma aux brief_bar_constructor "BriefModuleInitD";
#pragma aux brief_arrow_constructor "BriefModulePlace";
class MenuButton {public:char data[40];__pragma("brief_button_constructor") MenuButton(int,int);~MenuButton();};
class MenuHelpRegion {public:char data[40];__pragma("brief_help_constructor") MenuHelpRegion();~MenuHelpRegion();};
class UpgradeRange {public:char data[8];__pragma("brief_range_constructor") UpgradeRange(int,int);};
class UpgradeBar {public:char data[40];__pragma("brief_bar_constructor") UpgradeBar(UpgradeRange *,int);~UpgradeBar();};
class UpgradeArrow {public:char data[44];__pragma("brief_arrow_constructor") UpgradeArrow(UpgradeRange *,int,int);~UpgradeArrow();};
MenuButton briefFirst(0,0),briefSecond(0,0);
MenuHelpRegion briefHelp;
UpgradeRange briefRange(50,0);
UpgradeBar briefBar(&briefRange,0);
UpgradeArrow briefNext(&briefRange,20,48),briefPrevious(&briefRange,-20,45);
MissBrief g4b3390;
