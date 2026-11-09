/* shell\commscrn.c - comms screen (lane o4, partial). Names are working labels. */
#pragma aux file_init_name "W?FileInit$n(pn$FileRecord$$)v";
typedef unsigned long U32;
struct GuiRect { long x, y, width, height; };
struct GuiPoint { long x, y; };
struct InputReq; struct StrPool;
struct Timer; void TimerSet(Timer *, U32); int TimerExpired(Timer *);
struct MenuImage { U32 offset; unsigned short width, height; U32 flags; };
struct MenuImages { U32 count; MenuImage images[1]; };
struct ViewRect { int left, top, right, bottom; };
struct Timer { U32 start, end, duration; Timer(U32 n) { TimerSet(this, n); } };
struct FileRecord { U32 handle, open, flags, error; signed char direct, counted; unsigned char reserved[2]; U32 position, length;
    __pragma("file_init_name") FileRecord(); ~FileRecord(); };
struct Surface;
struct Palette { char data[0x452]; };
void ShellApplyPalette(Palette *data);
struct Flic { char body[0x3c]; Palette *palette; char unknown40[0xc]; long loops; char rest[0x40]; Flic(); ~Flic(); int Open(long source, int priority = 0x64, int sourceMode = 0); void Close(); unsigned long Update(Surface *surface); void SetLoops(long count) { loops = count; } Palette *GetPalette() { return palette; } };
struct GuiControlData : GuiRect { unsigned char style[4]; U32 attributes; U32 shortcut; };
class GuiControl : public GuiControlData {
public:
    GuiControl(U32, U32);
    virtual ~GuiControl();
    virtual void Reset(unsigned index);
    virtual void Draw(unsigned index);
    virtual int Event(unsigned index, U32 event);
    virtual int HitTest(const GuiPoint *point);
    void Invalidate() { attributes &= ~0x1000000UL; }
    void SetVisibility(U32 state) { attributes = (attributes & ~0xe00000UL) + (state << 21); }
};
#pragma pack(push, 1)
class MenuLabel : public GuiControl {
public:
    const char *text; long align; unsigned char font; int background; long color;
    MenuLabel(const char *, long, unsigned char, long, int); virtual ~MenuLabel(); virtual void Reset(unsigned); virtual void Draw(unsigned);
    void SetFont(unsigned char); void SetText(const char *); void SetColor(long); void SetAlign(long); void SetBackground(int);
};
class MenuPrefixButton : public GuiControl {
public:
    const char *text; U32 state; signed char font, color, align, prefix;
    MenuPrefixButton(const char * = 0, signed char = 1, signed char = 2, signed char = -1); virtual ~MenuPrefixButton(); virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned, U32);
    void SetText(const char *); void SetColor(signed char); void SetAlign(signed char); void SetPrefix(signed char); void SetFont(signed char);
};
class MenuNumber : public GuiControl {
public:
    U32 unused; const char *text; long value; signed char font, color, flash, align;
    MenuNumber(signed char, signed char); void Align(int a) { align = a; } virtual ~MenuNumber(); virtual void Reset(unsigned); virtual void Draw(unsigned);
    void SetValue(long, signed char); void SetFont(signed char);
};
class MenuHelpRegion : public GuiControl {
public:
    long id; int activated; MenuHelpRegion(); virtual ~MenuHelpRegion(); virtual void Draw(unsigned); virtual int Event(unsigned, U32);
};
class MenuPanel : public GuiControl {
public:
    const char *text; MenuPanel(); virtual ~MenuPanel(); virtual void Draw(unsigned);
};
class MenuButton : public GuiControl {
public:
    U32 state; const char *text; MenuButton(const char *, U32); virtual ~MenuButton(); virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned, U32);
};
class MenuListButton : public GuiControl {
public:
    U32 state; MenuListButton(); virtual ~MenuListButton(); virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned, U32);
};
#pragma pack(pop)
class GuiScreen : public GuiRect {
public:
    InputReq *input;
    char unknown14[0x38];
    GuiScreen(GuiControl **list, unsigned length, U32 flags);
    ~GuiScreen() { input = 0; }
    int Update();
    void LoadLayout(long resource);
    void Open(InputReq *, StrPool *, long base, int navigate);
    void Draw();
    void Invalidate();
    long HitTest(const GuiPoint *);
    int IsActive() const { return input != 0; }
};
extern "C" void GlobalBlockFree(void *);
extern "C" void *GlobalBlockAlloc(unsigned int, unsigned int);
extern "C" void *IORelease(void *);
extern "C" int IoClose(void *);
extern "C" int StreamRead(void *, void *, int);
extern void GuiFadeOut(); extern void ShellClearFrame();
extern "C" void HercSetMission(void *control, int value);
extern "C" void BriefBlit(void *surface, int a, int b, int c, int d, int e, int f, int g, int h);
extern "C" void SurfacePresent(void *surface);
extern "C" void BriefDone(const char *name);
extern "C" int BriefFormat(const char *format, ...);
extern "C" int BriefRender(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m = 1, int n = 0, int o = 0, int p = 0x7fffffff);
void AssertFail(const char *file, int line, const char *message);
struct StrPool { char *GetString(long id, int mode = 0); };
extern "C" void f_0043ba3e(void *a, void *b);
struct ResMgr { int FindPath(const char *, FileRecord *, int); };
extern ResMgr gResMgr;
extern void GuiResetClip(void *);
extern GuiRect gMenuClip;
extern ViewRect viewportClipLeft, gBriefDrawSource;
extern void *gDrawRemap;
extern void MenuSetClip(GuiRect *, long, long, long, long);
extern void MenuDrawImage(void *, const MenuImage *, long, long);

extern int g513888, g4b74f4, g4b2ac8;
extern GuiPoint g4bb78c;
extern Surface g511948;
extern short g4c975c;
extern StrPool gShellStrings;
struct ContactHeader { unsigned char id, system; unsigned char unknown2[10]; long field0c; long field10; unsigned char unknown14[8]; char *name; char *rank; char *ship; char *text; };
struct CommsLog { ContactHeader header; char name[100]; char rank[100]; char ship[100]; char text[0x2868 - 0x158]; };
extern CommsLog *gCommsLogs;
extern char g4b1db4[200], g4b1e7c[100], g4b1ee0[100], g4b1f44[100];
extern char g4b1fa8[12], g4b1fb4[12], g4b1fc0[12], g4b1fcc[12], g4b1fd8[12], g4b1fe4[12], g4b1ff0[12], g4b1ffc[12],
    g4b2008[12], g4b2014[12];
struct PlayerInfo { unsigned char kind; char unknown01[3]; char *name; unsigned long rating; long fieldC; long field10;
    long field14; long field18; long field1c; unsigned char counts[4]; long field24; int value; char unknown2C[4]; };
extern void GetPlayerInfo(void *, PlayerInfo *);
extern "C" void BriefWaitA(int value);
extern "C" void BriefWaitB(int value);
extern "C" void HercSetMissionStart(void *control, int value);
void VoicePlay(int id, int flags);
extern int gGuiPaletteFaded, g5138ac, g5138b0;
inline int Read5138ac() { return g5138ac; }
inline int Read5138b0() { return g5138b0; }
extern char gShellBackground[];
extern "C" int BriefLookup(int table, int number);
extern "C" char *strcpy(char *, const char *);
extern "C" int sprintf(char *, const char *, ...);
extern char gShellGame[]; extern char profileManager[];
extern GuiControl *g48e8f4[0x4a];

extern MenuPrefixButton g4b2070, g4b209c, g4b20ec;
extern MenuListButton g4b20c8;
extern MenuButton g4b2020, g4b2048;
extern MenuNumber g4b2118, g4b218c, g4b21bc, g4b21ec, g4b2b04;
extern MenuLabel g4b221c, g4b2250, g4b2284, g4b22b8, g4b22ec, g4b2320, g4b2354, g4b2388, g4b23bc, g4b23f0,
    g4b2424, g4b2458, g4b248c, g4b24c0, g4b24f4, g4b2528, g4b255c, g4b2590, g4b25c4, g4b25f8, g4b262c,
    g4b2660, g4b2694, g4b26c8, g4b26fc, g4b2730;
extern MenuPrefixButton gCommsSystems[14], gCommsOptions[5];
extern MenuLabel g4b2ad0, g4b2b34, g4b2b68, g4b2b9c, g4b2bd0, g4b2c04, g4b2c38, g4b2c6c;
extern MenuPrefixButton gCommsButtons[9];

#pragma pack(push, 1)
class CommsScreen : public GuiScreen {
public:
    int selected;               /* 4c */
    int lastSelected;           /* 50 */
    int unknown54;
    FileRecord file;            /* 58 */
    char *buffer;               /* 74 */
    int size;                   /* 78 */
    int offset;                 /* 7c */
    long frame;                 /* 80 */
    int scrollDirection;        /* 84 */
    int scrollSpeed;            /* 88 */
    Timer timer;                /* 8c */
    MenuImages *images;         /* 98 */
    int loaded;                 /* 9c */
    unsigned char colors[20 * 8]; /* a0: 20 rows of 8 palette entries */
    char unknown140[0x64];
    unsigned short mode;        /* 1a4 */
    char unknown1a6[4];
    Flic animation;             /* 1aa */
    Flic voice;                 /* 23a */
    int saveFlag;               /* 2ca */
    int system;                 /* 2ce */
    int contactSystem[9];       /* 2d2 */
    int unknown2f6;
    char names[9][30];          /* 2fa */
    char unknown408[0x1e];
    ContactHeader player;       /* 426 */
    unsigned char lastSystem;   /* 452 */
    Timer timer2;               /* 453 */
    unsigned char blink;        /* 45f */
    CommsScreen();
    int IsLoaded() const { return loaded; }
};
#pragma pack(pop)

class CommsLogView : public GuiControl {
public:
    CommsLogView();
    virtual void Draw(unsigned index);
    virtual int Event(unsigned index, U32 event);
};

extern CommsScreen g4b2e30;

static char SourceFile[] = __FILE__;
char g4b1db4[200], g4b1e7c[100], g4b1ee0[100], g4b1f44[100];
char g4b1fa8[12], g4b1fb4[12], g4b1fc0[12], g4b1fcc[12], g4b1fd8[12], g4b1fe4[12], g4b1ff0[12], g4b1ffc[12],
    g4b2008[12], g4b2014[12];
MenuButton g4b2020(0, 0), g4b2048(0, 0);
MenuPrefixButton g4b2070, g4b209c;
MenuListButton g4b20c8;
MenuPrefixButton g4b20ec;
MenuNumber g4b2118(-1, 0x12);
CommsLogView g4b2148;
MenuPanel g4b2168;
MenuNumber g4b218c(-1, 0x12), g4b21bc(-1, 0x12), g4b21ec(-1, 0x12);
MenuLabel g4b221c(0, -1, 1, 0xb, 1),
    g4b2250(0, -1, 1, 0xb, 1),
    g4b2284(0, -1, 1, 0xb, 1),
    g4b22b8(0, -1, 1, 0xb, 1),
    g4b22ec(0, -1, 1, 0xb, 1),
    g4b2320(0, -1, 1, 0xb, 1),
    g4b2354(0, -1, 1, 0xb, 1),
    g4b2388(0, -1, 1, 0xb, 1),
    g4b23bc(0, -1, 1, 0xb, 1),
    g4b23f0(0, -1, 1, 0xb, 1),
    g4b2424(0, -1, 1, 0xb, 1),
    g4b2458(0, -1, 1, 0xb, 1),
    g4b248c(0, -1, 1, 0xb, 1);
MenuLabel g4b24c0(0, 1, 1, 0xb, 1),
    g4b24f4(0, 1, 1, 0xb, 1),
    g4b2528(0, 1, 1, 0xb, 1),
    g4b255c(0, 1, 1, 0xb, 1),
    g4b2590(0, 1, 1, 0xb, 1),
    g4b25c4(0, 1, 1, 0xb, 1),
    g4b25f8(0, 1, 1, 0xb, 1),
    g4b262c(0, 1, 1, 0xb, 1),
    g4b2660(0, 1, 1, 0xb, 1),
    g4b2694(0, 1, 1, 0xb, 1),
    g4b26c8(0, 1, 1, 0xb, 1),
    g4b26fc(0, 1, 1, 0xb, 1),
    g4b2730(0, 1, 1, 0xb, 1);
MenuPrefixButton gCommsSystems[14], gCommsOptions[5];
MenuHelpRegion g4b2aa8;
MenuLabel g4b2ad0(0, -1, 1, 0xb, 1);
MenuNumber g4b2b04(-1, 0x12);
MenuLabel g4b2b34(0, -1, 1, 0xb, 1),
    g4b2b68(0, -1, 1, 0xb, 1),
    g4b2b9c(0, -1, 1, 0xb, 1),
    g4b2bd0(0, -1, 1, 0xb, 1),
    g4b2c04(0, -1, 1, 0xb, 1),
    g4b2c38(0, -1, 1, 0xb, 1),
    g4b2c6c(0, -1, 1, 0xb, 1);
MenuPrefixButton gCommsButtons[9];
CommsLog *gCommsLogs;
CommsScreen g4b2e30;
GuiControl *g48e8f4[0x4a] = {
    &gCommsButtons[0], &gCommsButtons[1], &gCommsButtons[2], &gCommsButtons[3], &gCommsButtons[4], &gCommsButtons[5],
    &gCommsButtons[6], &gCommsButtons[7], &gCommsButtons[8], &g4b2020, &g4b2070, &g4b209c, &g4b20c8, &g4b20ec, &g4b2118,
    &g4b2ad0, &g4b2b04, &g4b2b34, &g4b2b68, &g4b2c04, &g4b2c38, &g4b2c6c, &g4b2168, &g4b218c, &g4b221c, &g4b2250,
    &g4b2284, &g4b22b8, &g4b22ec, &g4b2320, &g4b2354, &g4b2388, &g4b23bc, &g4b23f0, &g4b2424, &g4b2458, &g4b248c,
    &g4b24c0, &g4b24f4, &g4b2528, &g4b255c, &g4b2590, &g4b25c4, &g4b25f8, &g4b262c, &g4b2660, &g4b2694, &g4b26c8,
    &g4b26fc, &g4b2730, &gCommsSystems[0], &gCommsSystems[1], &gCommsSystems[2], &gCommsSystems[3], &gCommsSystems[4],
    &gCommsSystems[5], &gCommsSystems[6], &gCommsSystems[7], &gCommsSystems[8], &gCommsSystems[9], &gCommsSystems[10],
    &gCommsSystems[11], &gCommsSystems[12], &gCommsSystems[13], &g4b21bc, &g4b2048, &g4b21ec, &gCommsOptions[0],
    &gCommsOptions[1], &gCommsOptions[2], &gCommsOptions[3], &gCommsOptions[4], &g4b2148, &g4b2aa8
};
inline long Percent(long value, long percent) { return value * percent / 100; }

CommsScreen::CommsScreen() : GuiScreen(g48e8f4, 0x4a, 0), timer(0), timer2(0) {}

CommsLogView::CommsLogView() : GuiControl(8, 0) {}

void CommsLogView::Draw(unsigned) {}

int CommsLogView::Event(unsigned, U32)
{
    if (!g4b2e30.IsLoaded()) return 0;
    {
        long mouse = g4bb78c.x;
        long left = x;
        long w = width;
        long low = left + w * 42 / 100;
        long high = left + w * 58 / 100;
        long right = w + left;
        if (mouse < low) {
            g4b2e30.scrollDirection = 1;
            g4b2e30.scrollSpeed = (mouse - left) * 150 / 100 + 5;
        } else if (mouse > high) {
            g4b2e30.scrollDirection = -1;
            g4b2e30.scrollSpeed = (right - mouse) * 150 / 100 + 5;
        } else {
            g4b2e30.scrollDirection = 0;
            g4b2e30.scrollSpeed = 0x53;
        }
    }
    return 0;
}

void CommsScreenUpdate(CommsScreen *screen);
void CommsSetupText(CommsScreen *screen);
void CommsSetupFonts(CommsScreen *);
void CommsSetupColors(CommsScreen *);
void CommsSetupLabels(CommsScreen *);
void CommsShowPlayer(CommsScreen *);
void CommsLoadContacts(CommsScreen *screen);
void CommsDrawLog(CommsScreen *screen);
void CommsDrawLogB(CommsScreen *screen);
void CommsFreeLog(CommsScreen *screen);
void CommsOpenLog(CommsScreen *screen);
void CommsDispatch(CommsScreen *screen);

void CommsDrawSelected(CommsScreen *screen);
void CommsReadChunk(CommsScreen *screen);
void CommsAnimate(CommsScreen *screen);
void CommsShowContact(CommsScreen *screen);





















void CommsScreenOpen(CommsScreen *screen, InputReq *input, unsigned char mode)
{
    if (screen->IsActive()) return;
    BriefWaitA(2);
    screen->voice.Open((long)"comidle.flx", 0x64, 1);
    screen->voice.SetLoops(-1);
    screen->Open(input, &gShellStrings, 0xa29, 1);
    screen->LoadLayout((long)"commscrn.box");
    g4b2ac8 = -1;
    screen->mode = mode;
    PlayerInfo info;
    GetPlayerInfo(profileManager, &info);
    screen->lastSystem = info.kind;
    TimerSet(&screen->timer2, 1000);
    CommsSetupFonts(screen);
    CommsSetupColors(screen);
    screen->selected = -1;
    screen->lastSelected = -2;
    viewportClipLeft = gBriefDrawSource;
    screen->voice.Update(&g511948);
    BriefBlit(&g513888, 0, 1, 0, 0, 0, 0, Read5138ac(), Read5138b0());
    *(Palette *)gShellBackground = *screen->voice.GetPalette();
    ShellApplyPalette((Palette *)gShellBackground);
    screen->player = *(ContactHeader *)BriefLookup((int)profileManager, 0);
    screen->system = screen->player.id;
    if (screen->mode == 2 || screen->mode == 6) {
        int found = gResMgr.FindPath("GALAXY.BMX", &screen->file, 1);
        if (found == 0) AssertFail(SourceFile, __LINE__, (const char *)found);
    } else {
        int found = gResMgr.FindPath((const char *)BriefFormat("COMMSYS%d.BMX", screen->system), &screen->file, 1);


        if (found == 0) AssertFail(SourceFile, __LINE__, (const char *)found);
        gGuiPaletteFaded = 0;
    }
    CommsOpenLog(screen);
    gCommsLogs = new CommsLog[9];
    CommsLog *logs = gCommsLogs;


    if (logs == 0) AssertFail(SourceFile, __LINE__, (const char *)logs);
    CommsSetupLabels(screen);
    CommsLoadContacts(screen);
    CommsScreenUpdate(screen);
    CommsShowPlayer(screen);
    CommsSetupText(screen);
    VoicePlay(0x34, 0);
    HercSetMissionStart(&g4b74f4, 0x929);
}

void CommsScreenClose(CommsScreen *screen)
{
    if (screen->IsActive()) {
        CommsFreeLog(screen);
        screen->voice.Close();
        GuiFadeOut();
        ShellClearFrame();
        if (gCommsLogs) {
            delete[] gCommsLogs;
            gCommsLogs = 0;
        }
        screen->input = 0;
        HercSetMission(&g4b74f4, 0x929);
    }
}

void CommsScreenExit(CommsScreen *screen)
{
    if (screen->IsActive()) {
        int handle = screen->HitTest(&g4bb78c);
        screen->selected = handle;
        if (handle >= 0 && handle < 9) {
            gCommsButtons[handle].Invalidate();
        }
        g4b2ac8 = handle;
        screen->Update();
    }
}

void CommsSetupText(CommsScreen *screen)
{
    g4b2070.SetAlign(2);
    g4b209c.SetAlign(2);
    g4b20ec.SetAlign(2);
    unsigned char i;
    for (i = 0; i < 9; i++) {
        gCommsButtons[i].SetColor(2);
        gCommsButtons[i].SetAlign(1);
        gCommsButtons[i].SetText(screen->names[i]);
    }
    for (i = 0; i <= screen->lastSystem; i++)
        gCommsSystems[i].SetText(gShellStrings.GetString(i + 0xa5b, 1));
    gCommsOptions[0].SetText(gShellStrings.GetString(0xa6c, 1));
    gCommsOptions[1].SetText(gShellStrings.GetString(0xa6d, 1));
    gCommsOptions[2].SetText(gShellStrings.GetString(0xa6e, 1));
    gCommsOptions[3].SetText(gShellStrings.GetString(0xa6f, 1));
    gCommsOptions[4].SetText(gShellStrings.GetString(0xa70, 1));
}

void CommsSetupFonts(CommsScreen *)
{
    g4b20c8.attributes = (g4b20c8.attributes & ~0xe00000UL) + 0xa00000UL;
    g4b218c.SetFont(0);
    g4b21bc.SetFont(0);
    g4b21ec.SetFont(0);
    g4b2118.SetFont(0);
    g4b2b04.SetFont(0);
    g4b2ad0.SetFont(1);
    g4b2ad0.SetBackground(0);
    g4b2b04.Align(1);
    g4b2b34.SetFont(1);
    g4b2b34.SetBackground(0);
    g4b2b68.SetFont(1);
    g4b2b68.SetBackground(0);
    g4b221c.SetFont(1);
    g4b221c.SetBackground(0);
    g4b2250.SetFont(1);
    g4b2250.SetBackground(0);
    g4b2284.SetBackground(0);
    g4b248c.SetBackground(0);
    g4b22b8.SetBackground(0);
    g4b22ec.SetBackground(0);
    g4b2320.SetBackground(0);
    g4b2354.SetBackground(0);
    g4b2388.SetBackground(0);
    g4b23bc.SetBackground(0);
    g4b23f0.SetBackground(0);
    g4b2424.SetBackground(0);
    g4b2458.SetBackground(0);
}

void CommsSetupColors(CommsScreen *)
{
    g4b2c04.SetColor(0xb);
    g4b2c04.SetBackground(0);
    g4b2c38.SetColor(0xb);
    g4b2c38.SetBackground(0);
    g4b2c6c.SetColor(0xb);
    g4b2c6c.SetBackground(0);
    g4b2528.SetBackground(0);
    g4b2730.SetBackground(0);
    g4b255c.SetBackground(0);
    g4b2590.SetBackground(0);
    g4b25c4.SetBackground(0);
    g4b25f8.SetBackground(0);
    g4b262c.SetBackground(0);
    g4b2660.SetBackground(0);
    g4b2694.SetBackground(0);
    g4b26c8.SetBackground(0);
    g4b26fc.SetBackground(0);
    g4b24c0.SetColor(0xb);
    g4b24f4.SetColor(0xb);
    g4b2528.SetColor(0xb);
    g4b255c.SetColor(0xb);
    g4b2590.SetColor(0xb);
    g4b25c4.SetColor(0xb);
    g4b25f8.SetColor(0xb);
    g4b262c.SetColor(0xb);
    g4b2660.SetColor(0xb);
    g4b2694.SetColor(0xb);
    g4b26c8.SetColor(0xb);
    g4b26fc.SetColor(0xb);
    g4b2730.SetColor(0xb);
    g4b2ad0.SetColor(0xb);
    g4b2b34.SetColor(0xb);
    g4b2b68.SetColor(0xb);
    g4b2b9c.SetColor(0xb);
}






















































































































void CommsShowSystem(CommsScreen *screen)
{
    screen->mode = 3;
    GuiResetClip(&gMenuClip);
    CommsFreeLog(screen);
    int found = gResMgr.FindPath((const char *)BriefFormat("COMMSYS%d.BMX", screen->system), &screen->file, 1);
    if (found == 0) AssertFail(SourceFile, __LINE__, (const char *)found);
    CommsOpenLog(screen);
    CommsScreenUpdate(screen);
    Flic *voice = &screen->voice;
    voice->Close();
    voice->Open((long)"comidle.flx", 0x64, 1);
    voice->loops = -1;
}











void CommsShowGalaxy(CommsScreen *screen)
{
    screen->mode = 2;
    CommsFreeLog(screen);
    CommsDispatch(screen);
    int found = gResMgr.FindPath("GALAXY.BMX", &screen->file, 1);
    if (found == 0) AssertFail(SourceFile, __LINE__, (const char *)found);
    CommsOpenLog(screen);
    CommsScreenUpdate(screen);
    screen->Invalidate();
    Flic *voice = &screen->voice;
    voice->Close();
    voice->Open((long)"comidle.flx", 0x64, 1);
    voice->loops = -1;
}

void CommsScreenRefresh(CommsScreen *screen)
{
    screen->mode = 6;
    CommsScreenUpdate(screen);
    screen->Invalidate();
    BriefBlit(&g513888, 1, 0, 0, 0x168, 0, 0x168, 0xe0, 0x57);
    SurfacePresent(&g513888);
}

void CommsScreenUpdate(CommsScreen *screen)
{
    int i;
    switch (screen->mode) {
    case 3:
        BriefWaitB(0x18);
        g4b2020.SetVisibility(0);
        for (i = 0; i < 9; i++) gCommsButtons[i].SetVisibility(0);
        g4b2c04.SetVisibility(0);
        g4b2118.SetVisibility(0);
        g4b2ad0.SetVisibility(0);
        g4b2b34.SetVisibility(0);
        g4b2c38.SetVisibility(0);
        g4b2b68.SetVisibility(0);
        g4b2c6c.SetVisibility(0);
        g4b2b04.SetVisibility(0);
        g4b218c.SetVisibility(5);
        g4b221c.SetVisibility(5);
        g4b2250.SetVisibility(5);
        g4b22b8.SetVisibility(5);
        g4b2284.SetVisibility(5);
        g4b248c.SetVisibility(5);
        g4b22ec.SetVisibility(5);
        g4b2320.SetVisibility(5);
        g4b2354.SetVisibility(5);
        g4b2388.SetVisibility(5);
        g4b23bc.SetVisibility(5);
        g4b23f0.SetVisibility(5);
        g4b2424.SetVisibility(5);
        g4b2458.SetVisibility(5);
        g4b21bc.SetVisibility(5);
        g4b2048.SetVisibility(5);
        g4b21ec.SetVisibility(5);
        g4b24c0.SetVisibility(5);
        g4b24f4.SetVisibility(5);
        g4b2528.SetVisibility(5);
        g4b2730.SetVisibility(5);
        g4b255c.SetVisibility(5);
        g4b2590.SetVisibility(5);
        g4b25c4.SetVisibility(5);
        g4b25f8.SetVisibility(5);
        g4b262c.SetVisibility(5);
        g4b2660.SetVisibility(5);
        g4b2694.SetVisibility(5);
        g4b26c8.SetVisibility(5);
        g4b26fc.SetVisibility(5);
        g4b2070.SetVisibility(5);
        g4b209c.SetVisibility(5);
        g4b20c8.SetVisibility(5);
        g4b20ec.SetVisibility(5);
        gCommsOptions[0].SetVisibility(5);
        gCommsOptions[1].SetVisibility(5);
        gCommsOptions[2].SetVisibility(5);
        gCommsOptions[3].SetVisibility(5);
        gCommsOptions[4].SetVisibility(5);
        break;
    case 2:
        BriefWaitB(0x17);
        for (i = 0; i < 9; i++) gCommsButtons[i].SetVisibility(5);
        for (i = 0; i < 14; i++) gCommsSystems[i].SetVisibility(5);
        g4b2118.SetVisibility(5);
        g4b2c04.SetVisibility(5);
        g4b2ad0.SetVisibility(5);
        g4b2b34.SetVisibility(5);
        g4b2c38.SetVisibility(5);
        g4b2b68.SetVisibility(5);
        g4b2c6c.SetVisibility(5);
        g4b2020.SetVisibility(5);
        g4b2b04.SetVisibility(5);
        g4b2070.SetVisibility(0);
        g4b209c.SetVisibility(0);
        g4b20ec.SetVisibility(0);
        g4b218c.SetVisibility(0);
        g4b221c.SetVisibility(0);
        g4b2250.SetVisibility(0);
        g4b22b8.SetVisibility(0);
        g4b2284.SetVisibility(0);
        g4b248c.SetVisibility(0);
        g4b22b8.SetVisibility(0);
        g4b22ec.SetVisibility(0);
        g4b2320.SetVisibility(0);
        g4b2354.SetVisibility(0);
        g4b2388.SetVisibility(0);
        g4b23bc.SetVisibility(0);
        g4b23f0.SetVisibility(0);
        g4b2424.SetVisibility(0);
        g4b21bc.SetVisibility(5);
        g4b2048.SetVisibility(5);
        g4b21ec.SetVisibility(5);
        g4b24c0.SetVisibility(0);
        g4b24f4.SetVisibility(0);
        g4b2528.SetVisibility(0);
        g4b2730.SetVisibility(0);
        g4b255c.SetVisibility(0);
        g4b2590.SetVisibility(0);
        g4b25c4.SetVisibility(0);
        g4b25f8.SetVisibility(0);
        g4b262c.SetVisibility(0);
        g4b2660.SetVisibility(0);
        g4b2694.SetVisibility(0);
        g4b26c8.SetVisibility(0);
        gCommsOptions[0].SetVisibility(5);
        gCommsOptions[1].SetVisibility(5);
        gCommsOptions[2].SetVisibility(5);
        gCommsOptions[3].SetVisibility(5);
        gCommsOptions[4].SetVisibility(5);
        break;
    case 6:
        BriefWaitB(0x1a);
        for (i = 0; i <= screen->lastSystem; i++) gCommsSystems[i].SetVisibility(0);
        for (i = 0; i < 9; i++) gCommsButtons[i].SetVisibility(5);
        g4b2c04.SetVisibility(5);
        g4b2ad0.SetVisibility(5);
        g4b2b34.SetVisibility(5);
        g4b2c38.SetVisibility(5);
        g4b2b68.SetVisibility(5);
        g4b2c6c.SetVisibility(5);
        g4b2020.SetVisibility(5);
        g4b2b04.SetVisibility(5);
        g4b2070.SetVisibility(5);
        g4b209c.SetVisibility(5);
        g4b20ec.SetVisibility(5);
        g4b218c.SetVisibility(5);
        g4b221c.SetVisibility(5);
        g4b2250.SetVisibility(5);
        g4b22b8.SetVisibility(5);
        g4b2284.SetVisibility(5);
        g4b248c.SetVisibility(5);
        g4b22ec.SetVisibility(5);
        g4b2320.SetVisibility(5);
        g4b2354.SetVisibility(5);
        g4b2388.SetVisibility(5);
        g4b23bc.SetVisibility(5);
        g4b23f0.SetVisibility(5);
        g4b2424.SetVisibility(5);
        g4b2458.SetVisibility(5);
        g4b21bc.SetVisibility(0);
        g4b2048.SetVisibility(0);
        g4b21ec.SetVisibility(0);
        g4b24c0.SetVisibility(5);
        g4b24f4.SetVisibility(5);
        g4b2528.SetVisibility(5);
        g4b2730.SetVisibility(5);
        g4b255c.SetVisibility(5);
        g4b2590.SetVisibility(5);
        g4b25c4.SetVisibility(5);
        g4b25f8.SetVisibility(5);
        g4b262c.SetVisibility(5);
        g4b2660.SetVisibility(5);
        g4b2694.SetVisibility(5);
        g4b26c8.SetVisibility(5);
        g4b26fc.SetVisibility(5);
        g4b2118.SetVisibility(5);
        gCommsOptions[0].SetVisibility(0);
        gCommsOptions[1].SetVisibility(0);
        gCommsOptions[2].SetVisibility(0);
        gCommsOptions[3].SetVisibility(0);
        gCommsOptions[4].SetVisibility(0);
        break;
    }
}

void CommsOpenLog(CommsScreen *screen)
{
    screen->loaded = 0;
    screen->size = IoClose(&screen->file);
    screen->buffer = (char *)GlobalBlockAlloc(screen->size, 0);
    screen->offset = 0;
    screen->images = (MenuImages *)screen->buffer;
    screen->scrollDirection = 1;
    screen->scrollSpeed = 0x53;
    screen->frame = 0;
    TimerSet(&screen->timer, 0);
}

void CommsFreeLog(CommsScreen *screen)
{
    if (screen->buffer) GlobalBlockFree(screen->buffer);
    screen->buffer = 0;
    screen->images = 0;
    IORelease(&screen->file);
}

void CommsScreenRender(CommsScreen *screen)
{
    switch (screen->mode) {
    case 1:
        if (screen->animation.Update(&g511948) & 4) {
            screen->animation.Close();
            screen->mode = 3;
        }
        break;
    case 2:
        screen->voice.Update(&g511948);
        if (screen->selected != screen->lastSelected) {
            screen->lastSelected = screen->selected;
            CommsDispatch(screen);
            GuiResetClip(&gMenuClip);
        }
        CommsAnimate(screen);
        screen->Invalidate();
        screen->Draw();
        break;
    case 3:
        screen->voice.Update(&g511948);
        if (screen->selected != screen->lastSelected) {
            BriefBlit(&g513888, 1, 0, 0x19, 0x10e, 0x19, 0x10e, 0xed, 0x78);
            BriefBlit(&g513888, 1, 0, 0x6b, 0x154, 0x6b, 0x154, 0xfa, 0x69);
            screen->lastSelected = screen->selected;
            CommsDispatch(screen);
            CommsShowContact(screen);
            SurfacePresent(&g513888);
        }
        CommsAnimate(screen);
        CommsDrawSelected(screen);
        if (TimerExpired(&screen->timer2) && g4c975c >= 0) {
            int blink = screen->blink;
            MenuPrefixButton *button = &gCommsButtons[g4c975c];
            if (blink == 0) {
                screen->blink = 1;
                button->SetColor(2);
                TimerSet(&screen->timer2, 800);
            } else {
                screen->blink = 0;
                button->SetColor(10);
                TimerSet(&screen->timer2, 600);
            }
        }
        screen->Invalidate();
        screen->Draw();
        break;
    case 6:
        screen->voice.Update(&g511948);
        CommsAnimate(screen);
        screen->Invalidate();
        screen->Draw();
        break;
    }
}
















































































































































char *CommsMissionTypeName(CommsScreen *screen, unsigned char type)
{
    if (type > 3) AssertFail(SourceFile, __LINE__, "mission type index was greater than 3");
    return gShellStrings.GetString(type + 0xb54, 1);
}

void CommsLoadContacts(CommsScreen *screen)
{
    for (unsigned char i = 0; i < 9; i++) {
        gCommsLogs[i].header = *(ContactHeader *)BriefLookup((int)profileManager, i);
        strcpy(gCommsLogs[i].name, gCommsLogs[i].header.name);
        strcpy(gCommsLogs[i].rank, gCommsLogs[i].header.rank);
        strcpy(gCommsLogs[i].ship, gCommsLogs[i].header.ship);
        strcpy(gCommsLogs[i].text, gCommsLogs[i].header.text);
        screen->contactSystem[i] = gCommsLogs[i].header.system;
        sprintf(screen->names[i], screen->player.ship);
    }
}

void CommsSetupLabels(CommsScreen *)
{
    g4b2ad0.SetText(gShellStrings.GetString(0xa72));
    g4b2b34.SetText(gShellStrings.GetString(0xa3a));
    g4b2b68.SetText(gShellStrings.GetString(0xa3b));
    g4b221c.SetText(gShellStrings.GetString(0xa41));
    g4b2250.SetText(gShellStrings.GetString(0xa42));
    g4b2284.SetText(gShellStrings.GetString(0xa43));
    g4b248c.SetText(gShellStrings.GetString(0xa4d));
    g4b22b8.SetText(gShellStrings.GetString(0xa44));
    g4b22ec.SetText(gShellStrings.GetString(0xa45));
    g4b2320.SetText(gShellStrings.GetString(0xa46));
    g4b2354.SetText(gShellStrings.GetString(0xa47));
    g4b2388.SetText(gShellStrings.GetString(0xa48));
    g4b23bc.SetText(gShellStrings.GetString(0xa49));
    g4b23f0.SetText(gShellStrings.GetString(0xa4a));
}

void CommsShowContact(CommsScreen *screen)
{
    if (screen->mode == 3) {
        if (screen->selected >= 0 && screen->selected < 9) {
            sprintf(g4b1db4, gCommsLogs[screen->selected].rank);
            sprintf(g4b1e7c, "%d %s", gCommsLogs[screen->selected].header.field10, gShellStrings.GetString(0xa42, 1));
            sprintf(g4b1ee0, "%d %s", gCommsLogs[screen->selected].header.field0c, gShellStrings.GetString(0xb58, 1));
            CommsDrawSelected(screen);
        } else {
            sprintf(g4b1db4, " ");
            sprintf(g4b1e7c, " ");
            sprintf(g4b1ee0, " ");
        }
    } else {
        sprintf(g4b1db4, " ");
        sprintf(g4b1e7c, " ");
        sprintf(g4b1ee0, " ");
    }
    g4b2c04.SetText(g4b1db4);
    g4b2c38.SetText(g4b1e7c);
    g4b2c6c.SetText(g4b1ee0);
}

void CommsShowPlayer(CommsScreen *)
{
    PlayerInfo info;
    GetPlayerInfo(profileManager, &info);
    sprintf(g4b1f44, "%s", info.name);
    sprintf(g4b1fa8, "%4d.%2d", info.rating / 10, info.rating % 10);
    sprintf(g4b1fb4, "%8d", info.fieldC);
    sprintf(g4b1ffc, "%6d", info.field10);
    sprintf(g4b2008, "%5d", info.field14);
    sprintf(g4b2014, "%5d", info.field18);
    sprintf(g4b1fc0, "%5d", info.counts[0]);
    sprintf(g4b1fcc, "%5d", info.counts[1]);
    sprintf(g4b1fd8, "%5d", info.counts[2]);
    sprintf(g4b1fe4, "%5d", info.counts[3]);
    sprintf(g4b1ff0, "%9d", info.field24);
    g4b2528.SetText(g4b1f44);
    g4b24c0.SetText(g4b1fa8);
    g4b2730.SetText(g4b1fb4);
    g4b255c.SetText(g4b1fc0);
    g4b2590.SetText(g4b1fcc);
    g4b25c4.SetText(g4b1fd8);
    g4b25f8.SetText(g4b1fe4);
    g4b24f4.SetText(g4b1ff0);
    g4b262c.SetText(g4b1ffc);
    g4b2660.SetText(g4b2008);
    g4b2694.SetText(g4b2014);
    g4b26c8.SetText(" ");
}

void CommsDrawSelected(CommsScreen *screen)
{
    int x, y;
    if (screen->selected >= 0 && screen->selected < 9)
        BriefRender(0x19, 0x10e, 0xe6, 0x78, (int)gCommsLogs[screen->selected].text,
                    0, -1, 0, (int)&x, (int)&y, 0, 0xb, 0);
}

void CommsReadChunk(CommsScreen *screen)
{
    if (screen->offset < screen->size) {
        int n = screen->size - screen->offset;
        if (n > 0x4000) n = 0x4000;
        StreamRead(&screen->file, screen->buffer + screen->offset, n);
        screen->offset += n;
    } else {
        screen->loaded = 1;
    }
}

void CommsAnimate(CommsScreen *screen)
{
    if (screen->size) {
        if (!screen->loaded) CommsReadChunk(screen);
        if (TimerExpired(&screen->timer)) {
            TimerSet(&screen->timer, screen->scrollSpeed);
            viewportClipLeft = gBriefDrawSource;
            BriefBlit(&g513888, 1, 0, 0, 0, 0, 0, 0x280, 0x14a);
            MenuSetClip(&gMenuClip, 0, 0, 0x280, 0x14a);
            gDrawRemap = screen->colors;
            MenuDrawImage(&g513888, &screen->images->images[screen->frame], 0, 0);
            gDrawRemap = 0;
            if (screen->scrollDirection > 0) {
                if (++screen->frame >= screen->images->count) screen->frame = 0;
            } else if (screen->scrollDirection < 0) {
                if (--screen->frame < 0) screen->frame = screen->images->count - 1;
            }
        }
    }
}

void CommsDone()
{
    BriefDone("commscrn");
}

void CommsSaveA(CommsScreen *screen)
{
    if (g4b2e30.saveFlag) {
        f_0043ba3e(profileManager, gShellGame);
        screen->saveFlag = 0;
    }
}

void CommsSaveB(CommsScreen *screen)
{
    if (g4b2e30.saveFlag) {
        f_0043ba3e(gShellGame, profileManager);
        screen->saveFlag = 0;
    }
}

void CommsDrawLog(CommsScreen *screen)
{
    int selected = screen->HitTest(&g4bb78c);
    int row, i;
    if (selected >= 0 && selected < 9) {
        for (row = 0; row <= 0x12; row++) {
            if (row == screen->contactSystem[selected])
                for (i = 0; i < 8; i++) screen->colors[(row + 1) * 8 + i] = i + 0x10;
            else
                for (i = 0; i < 8; i++) screen->colors[(row + 1) * 8 + i] = i + 0x18;
        }
    } else {
        for (row = 0; row < 0x12; row++)
            for (i = 0; i < 8; i++) screen->colors[row * 8 + i] = i + 0x10;
    }
    if (screen->system < 2)
        for (i = 0; i < 8; i++) screen->colors[i] = i + 0x28;
}

void CommsDrawLogB(CommsScreen *screen)
{
    int selected = screen->HitTest(&g4bb78c);
    int row, i;
    if (selected >= 0 && selected < 5) {
        for (row = 0; row < 0x12; row++)
            for (i = 0; i < 8; i++) screen->colors[row * 8 + i] = i + 0x18;
        for (i = 0; i < 8; i++) screen->colors[(selected + 1) * 8 + i] = i + 0x20;
    } else {
        for (row = 0; row < 5; row++)
            for (i = 0; i < 8; i++) screen->colors[row * 8 + i] = i + 0x20;
    }
}

void CommsDispatch(CommsScreen *screen)
{
    if (screen->mode == 3) CommsDrawLog(screen);
    else if (screen->mode == 2) CommsDrawLogB(screen);
}

