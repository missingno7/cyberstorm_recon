struct GuiControl; struct InputReq; struct StrPool; struct MenuPrefixButton; struct MenuNumber; struct MenuImage; struct Palette; typedef unsigned long U32;
class GuiScreen { char pad00[0x10]; public: int active; char pad14[0x44]; GuiScreen(GuiControl **, unsigned, U32); void LoadLayout(long); void Open(InputReq *, StrPool *, long, int); };
struct Flic { char body[0x80]; Flic(); int Open(long source, int priority = 0x64, int sourceMode = 0); };
class HercBay : public GuiScreen { public: Flic animation; int IsActive() const { return active != 0; } void Start(void *callback); void DrawImage(void *image); void StartVariantA(void *callback); HercBay(); };
 static char hercbaySourceFile[] = __FILE__;


int PopLoadResource(int owner, const char *resource, int count, int flags = 0);
int HercObjectRelease(char *object);
extern "C" void HercCloseScreen(char *object); extern "C" int HercGetHandle(char *object, void *resource);
extern "C" void HercRefresh(char *object); extern "C" void HercUpdate(char *object);
extern "C" void HercSelect(int mode); extern "C" void HercWait(int value);
extern "C" void BriefCloseAudio(); extern "C" void HercPrepareAssets();
struct ResMgr { void *Load(long id, int a = 1, int b = 0, int c = 0); };

extern ResMgr gResMgr;
struct StrPool { int Lock(); void Unlock(); };
extern StrPool g511e18;
extern void *AllocateMissionImage(const char *image, int bytes, int mode);
struct MenuPrefixButton { void SetText(char *text); void SetAlign(char alignment); };
struct MenuNumber { void SetValue(long value, char mode); };
void VoicePlay(int id, int flags);
extern "C" void HercSetMission(void *control, int value); extern "C" void HercSetMissionStart(void *control, int value);
void AssertFail(const char *file, int line, const char *message);
void MenuDrawImage(void *surface, MenuImage *image, long width, long height); void SurfacePresent(void *surface); void ShellApplyPalette(Palette *data);
extern "C" void HercDraw16(int a, int b, int c, int d, int e, int f, int g, int h,
                             int i, int j, int k, int l, int m = 1, int n = 0,
                             int o = 0, int p = 0x7fffffff);
extern "C" void HercPlayAnimation(char *object, const char *name, int frames, int flags);
extern "C" void HercDone(const char *name);
extern "C" void BriefStatus(char *object); extern "C" void BriefCloseWait(); extern "C" void BriefCloseAudio();
extern "C" int BriefSwitch(char *object, void *resource); extern "C" void BriefRenderUpdate(char *object);
extern "C" void f_cb80(void *data);




extern int g4b3748; extern int g48eb10;
extern int g4b37bc; extern int g4bb78c;
extern int logicMode; extern StrPool gShellStrings;
extern int g4d030f; extern int g4d0413;
extern int g4b36b0; extern int g4b36dc;
extern int g4b3708;
extern int g4b3734;
extern int g4b369c;
extern int g4b36c8;
extern int g4b36f4;
extern char g4b3777;
extern int g511dd4;
extern void *gShellImages;
extern int g511948;
extern int g513888;
extern int g4b74f4;
extern char g48aa2c[]; extern char g48aa3c[], g48aa48[], g48aa54[], g48aa60[], g48aa6c[], g48aa78[];
extern int g48eaf4, g48eafc, g4b3720, g4b3778, g4b379c;
extern "C" void HercModuleConstruct(void *object);
extern "C" void HercModuleInitWidget(void *object, int a, int b, int c, int d);
extern "C" void HercModuleInitA(void *object, int a, int b);
extern "C" void HercModuleInitB(void *object, int a, int b);
extern "C" void HercModuleInitC(void *object);
extern "C" void HercModuleInitD(void *object);
HercBay::HercBay() : GuiScreen((GuiControl **)&g48eb10, 7, 0), animation() {}




void HercBay::Start(void *callback)
{
    if (IsActive()) return;
    register int selector = logicMode; if (selector == 1) {
        HercSelect(2);
    } else {
        HercSelect(9);
    }

    HercWait(0xc);
    BriefCloseAudio();
    HercPrepareAssets();
    g4b37bc = -1;
    void *loaded = gResMgr.Load((long)g48aa3c);
    g511dd4 = (int)loaded;
    if (loaded == 0)
    {
        /* source line pad */
        /* source line pad */
        /* source line pad */
        /* source line pad */
        /* source line pad */
        /* source line pad */
        /* source line pad */
        /* source line pad */
        /* source line pad */
        /* source line pad */
        /* source line pad */
        /* source line pad */
        /* source line pad */
        /* source line pad */
        /* source line pad */
        /* source line pad */

        AssertFail(hercbaySourceFile, 101, 0);
    }
    LoadLayout((long)g48aa48);
    Open((InputReq *)callback, &gShellStrings, 0x709, 1);
    Flic *animation;
    (animation = (Flic *)((char *)this + 0x58))->Open((long)g48aa54, 0x64);
    *(int *)((char *)animation + 0x4c) = -1;
    g511e18.Lock();
    *(int *)((char *)this + 0xec) = 0;
    *(int *)((char *)this + 0xe8) = 0;
    ((MenuPrefixButton *)&g4b369c)->SetText((char *)AllocateMissionImage((const char *)&gShellStrings, 0x709, 1));
    ((MenuPrefixButton *)&g4b36c8)->SetText((char *)AllocateMissionImage((const char *)&gShellStrings, 0x70a, 1));
    ((MenuPrefixButton *)&g4b36f4)->SetText((char *)AllocateMissionImage((const char *)&gShellStrings, 0x70b, 1));
    ((MenuPrefixButton *)&g4b369c)->SetAlign(2);
    ((MenuPrefixButton *)&g4b36c8)->SetAlign(2);
    ((MenuPrefixButton *)&g4b36f4)->SetAlign(2);
    register int one = 1;
    g4b3777 = one;
    g4b3734 = (g4b3734 & 0xff1fffff) + 0xa00000;
    register int display = g4d0413;
    ((MenuNumber *)&g4b3748)->SetValue(display, 0);
    *(int *)((char *)this + 0x4c) = g4d030f;
    VoicePlay(0x33, 0);
    HercSetMissionStart(&g4b74f4, 0x927);
}

void HercBayConfigureControls()
{
    ((MenuPrefixButton *)&g4b369c)->SetText((char *)AllocateMissionImage((const char *)&gShellStrings, 0x709, 1));
    ((MenuPrefixButton *)&g4b36c8)->SetText((char *)AllocateMissionImage((const char *)&gShellStrings, 0x70a, 1));
    ((MenuPrefixButton *)&g4b36f4)->SetText((char *)AllocateMissionImage((const char *)&gShellStrings, 0x70b, 1));
    ((MenuPrefixButton *)&g4b369c)->SetAlign(2);
    ((MenuPrefixButton *)&g4b36c8)->SetAlign(2);
    ((MenuPrefixButton *)&g4b36f4)->SetAlign(2);
    register int one = 1;
    g4b3777 = one;
    g4b3734 = (g4b3734 & 0xff1fffff) + 0xa00000;
}

void HercBayReturnFromPurchase(HercBay *bay, void *callback)
{
    if (bay->IsActive())
        return;
    register char *screen = (char *)bay;
    register int selector = logicMode;
    if (selector == 1) { HercSelect(2); } else { HercSelect(9); }
    g4b37bc = -1;
    ((GuiScreen *)screen)->LoadLayout((long)g48aa60);
    register void *handler = callback;
    ((GuiScreen *)screen)->Open((InputReq *)handler, &gShellStrings, 0x709, 1);
    BriefStatus(screen);
    screen += 0x58;
    ((Flic *)screen)->Open((long)g48aa6c);
    *(int *)(screen + 0x4c) = -1;
    ((MenuNumber *)&g4b3748)->SetValue(g4d0413, 0);
    *(int *)((char *)bay + 0x4c) = g4d030f;
    *(int *)((char *)bay + 0xec) = 1;
    *(int *)((char *)bay + 0x50) = 0;
}

void HercBayOpen(HercBay *bay, void *callback)
{
    if (bay->IsActive())
        return;
    
    register int selector = logicMode;
    if (selector == 1) { HercSelect(2); } else { HercSelect(9); }
    g4b37bc = -1;
    bay->LoadLayout((long)g48aa60);
    register void *handler = callback;
    bay->Open((InputReq *)handler, &gShellStrings, 0x709, 1);
    BriefStatus((char*)bay);
    char *screen=(char*)bay+0x58;
    ((Flic *)screen)->Open((long)g48aa6c);
    *(int *)(screen + 0x4c) = -1;
    ((MenuNumber *)&g4b3748)->SetValue(g4d0413, 0);
    *(int *)(screen - 0x58 + 0xec) = 1;
    *(int *)(screen - 0x58 + 0x54) = 0;
}

void HercBayAltStart(HercBay *bay, void *callback)
{
    
    register int selector = logicMode;
    if (selector == 1) { HercSelect(2); } else { HercSelect(9); }
    g4b37bc = -1;
    bay->LoadLayout((long)g48aa60);
    bay->Open((InputReq *)callback, &gShellStrings, 0x709, 1);
    BriefStatus((char*)bay);
    ((Flic *)((char *)bay+0x58))->Open((long)g48aa6c);
    char *animation=(char*)bay+0x58;
    *(int *)(animation + 0x4c) = -1;
    int display = g4d0413;
    ((MenuNumber *)&g4b3748)->SetValue(display, 0);
}



void HercBayClose(HercBay *bay)
{
    if (bay->IsActive()) {
        HercCloseScreen((char *)bay + 0x58);
        bay->active = 0;
    }
}

void HercBayCleanup(HercBay *bay)
{
    if (bay->IsActive()) {
        g511e18.Unlock();
        HercCloseScreen((char *)bay + 0x58);
        f_cb80((void *)g511dd4);
        g511dd4 = 0;
        BriefCloseWait();
        BriefCloseAudio();
        bay->active = 0;
        HercSetMission(&g4b74f4, 0x927);
    }
}

void HercBaySetStatus(HercBay *bay)
{
    *(int *)((char *)bay + 0x4c) = g4d030f;
    if (bay->IsActive()) {
        g4b37bc = HercGetHandle((char *)bay, &g4bb78c);
        HercRefresh((char *)bay);
    }
}

void HercBaySetDisplay()
{
    register int value = g4d0413;
    ((MenuNumber *)&g4b3748)->SetValue(value, 0);
}

void HercBayUpdateState(HercBay *bay)
{
    unsigned int state;
    register char *resource;
    if (bay->IsActive())
        goto dispatch;
    return;
redraw:
    if (BriefSwitch(resource, &g511948) & 0x20) {
        register void *data = *(void **)(resource + 0x3c);
        ShellApplyPalette((Palette *)data);
    }
    BriefStatus((char *)bay);
    BriefRenderUpdate((char *)bay);
    *(int *)((char *)bay + 0xec) = 1;
    return;
state_one:
    BriefSwitch(resource, &g511948);
    BriefRenderUpdate((char *)bay);
    return;
state_nine:
    BriefRenderUpdate((char *)bay);
    return;
dispatch:
    state = *(unsigned int *)((char *)bay + 0xec);
    resource = (char *)bay + 0x58;
    if (state < 1)
        goto state_zero;
    if (state <= 1)
        goto state_one;
    if (state < 8)
        return;
    if (state <= 8)
        goto redraw;
    if (state == 9)
        goto state_nine;
    return;
state_zero:
    if (state == 0)
        goto redraw;
    return;
}

void HercBayDrawMission(HercBay *bay)
{
    void *image = AllocateMissionImage((const char *)&gShellStrings, 0x70f, 1);
    int local[2];
    MenuDrawImage(&g513888, (MenuImage *)((char *)gShellImages + 0x31c), 0xd2, 0xaa);
    SurfacePresent(&g513888);
    *(int *)((char *)bay + 0xec) = 9;
    g4b36b0 = (g4b36b0 & 0xff1fffff) + 0xa00000;
    g4b36dc = (g4b36dc & 0xff1fffff) + 0xa00000;
    g4b3708 = (g4b3708 & 0xff1fffff) + 0xa00000;
    register int flags = g4b3734; flags = flags & 0xff1fffff; g4b3734 = flags;
    HercDraw16(0xe6, 0xb9, 0xaa, 0x4b, (int)image, 0, -1, 0,
               (int)&local[0], (int)&local[1], 0, 0xf);
    HercCloseScreen((char *)bay + 0x58);
}

void HercBay::DrawImage(void *image)
{
    int local[2];
    MenuDrawImage(&g513888, (MenuImage *)((char *)gShellImages + 0x31c), 0xd2, 0xaa);
    SurfacePresent(&g513888);
    *(int *)((char *)this + 0xec) = 9;
    g4b36b0 = (g4b36b0 & 0xff1fffff) + 0xa00000;
    g4b36dc = (g4b36dc & 0xff1fffff) + 0xa00000;
    g4b3708 = (g4b3708 & 0xff1fffff) + 0xa00000;
    register int flags = g4b3734; flags = flags & 0xff1fffff; g4b3734 = flags;
    HercDraw16(0xe6, 0xb9, 0xaa, 0x4b, (int)image, 0, -1, 0,
               (int)&local[0], (int)&local[1], 0, 0xf);
    HercCloseScreen((char *)this + 0x58);
}

class HercFrame {
public:
    void Draw(int display);
};

void HercFrame::Draw(int display)
{
    int local[2];
    HercDraw16(0xe6, 0xb9, 0xaa, 0x4b, display, 0, -1, 0,
               (int)&local[0], (int)&local[1], 0, 0xf);
}

inline int Read4b36b0() { return g4b36b0; }
inline int Read4b36dc() { return g4b36dc; }
inline int Read4b3708() { return g4b3708; }

class HercBayActions {
public:
    void Reactivate();
};

void HercBayActions::Reactivate()
{
    HercBay *bay = (HercBay *)this;
    g4b3734 = (g4b3734 & 0xff1fffff) + 0xa00000;
    g4b36b0 = Read4b36b0() & 0xff1fffff;
    g4b36dc = Read4b36dc() & 0xff1fffff;
    g4b3708 = Read4b3708() & 0xff1fffff;
    *(int *)((char *)bay + 0xec) = 1;
    ((Flic *)((char *)bay + 0x58))->Open((long)g48aa78);
    BriefStatus((char *)bay);
}

void HercBayDone()
{
    HercDone(g48aa2c);
}

struct ModuleConstruct { ModuleConstruct(void *p) { HercModuleConstruct(p); } };
struct InitWidget { InitWidget(void *p, int a, int b, int c, int d) { HercModuleInitWidget(p, a, b, c, d); } };
struct InitA { InitA(void *p, int a, int b) { HercModuleInitA(p, a, b); } };
struct InitB { InitB(void *p, int a, int b) { HercModuleInitB(p, a, b); } };
struct InitC { InitC(void *p) { HercModuleInitC(p); } };
struct InitD { InitD(void *p) { HercModuleInitD(p); } };
struct State { State(int value) { g48eafc = value; } };
ModuleConstruct moduleConstruct(&g48eaf4);
InitWidget firstWidget(&g4b369c, 0, 1, 2, -1);
State stateOne(1);
InitWidget secondWidget(&g4b36c8, 0, 1, 2, -1);
State stateTwo(2);
InitWidget thirdWidget(&g4b36f4, 0, 1, 2, -1);
State stateThree(3);
InitA fourthWidget(&g4b3720, 0, 0);
State stateFour(4);
InitB fifthWidget(&g4b3748, -1, 0x12);
State stateFive(5);
InitC sixthWidget(&g4b3778);
State stateSix(6);
InitD seventhWidget(&g4b379c);
State stateSeven(7);
HercBay g4b37c4;
State stateEight(8);






