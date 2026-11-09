#pragma aux file_init_name "W?FileInit$n(pn$FileRecord$$)v";
#pragma aux file_release_name "W?Release$:FileRecord$n()pn$1$";
#include <windows.h>
#include <string.h>

struct WinConfig { int f0; int noMenuSize; int f8; int forceMenu; int framed; int width; int f18; int captionAdjust; int resizable; int f24; };
class StrPool { public: long id; char *buffer; long capacity; void *data; long size; int Lock(); void Unlock(); char *GetString(long id, int loadData); int IsLocked() { return data != 0; } };
struct GuiCanvas;
typedef unsigned long U32;
struct FileRecord {U32 handle,open,flags,error;signed char direct,counted;unsigned char reserved[2];U32 position,length;
    __pragma("file_init_name") FileRecord();__pragma("file_release_name") ~FileRecord();int Open(const char *path, unsigned long options);};
int IORelease(FileRecord *file);
extern char gDebugText[];
void DebugTextClear();
extern int f_477352(int, char *filter, char *extension, char *path);
extern int f_43ee17(void *, FileRecord *file, int);
extern int f_43ee71(void *, FileRecord *file);
extern void (*ShowStatus)(HWND, const char *, int, int);
extern "C" int sprintf(char *buffer, const char *format, ...);
struct ModeSystem;
extern "C" void AssertFail(const char *file, int line, const char *message);
#define ASSERT(e) { unsigned long assertValue = (unsigned long)(e); if (assertValue == 0) AssertFail(__FILE__, __LINE__, (const char *)assertValue); }
extern int logicMode;
inline int IsMultiplayer() { return logicMode == 2 || logicMode == 3; }
struct ProfileManager { int Count(); };
extern ProfileManager profileManager;
extern int HasMission(void *manager);
extern int g4d030f;
inline int PlayerCount() { return g4d030f; }
struct Shell { int active; char pad[0x4b]; int mode; int IsActive() { return active; } void Open(); void Close(int reason); };
extern Shell uiSystem;
extern int g48e4ec[];
extern unsigned long TimeNow();
extern unsigned long g512594;
extern unsigned long StreamWrite(FileRecord *file, const void *buffer, unsigned long size);
extern int FileGetAndClearError(FileRecord *file);
struct SaveHeader { int f0; int f4; int network; int type; int f10; char description[40]; };
extern void f_476d44(SaveHeader *header, FileRecord *file);
extern void f_43edf0(ProfileManager *manager, FileRecord *file);
extern void f_4524c0(ModeSystem *mode, FileRecord *file);
struct GameSession { char data[0x74f3]; GameSession(); ~GameSession(); };
struct PlayerInfo { char name[0x10b]; };
extern PlayerInfo g4cc3f7[];
extern int g4d0413;
extern StrPool gTextPool;
extern "C" char *BriefFormat(const char *format, ...);
extern "C" void f_0043ba3e(void *destination, void *source);
extern int f_476d78(SaveHeader *header, FileRecord *file);
extern void f_44b470(ModeSystem *mode, int state);
extern void f_44aff0(ModeSystem *mode);
extern int f_43ecf8(ProfileManager *manager, FileRecord *file);
extern int f_43c844(ProfileManager *manager);
extern void f_452340(ModeSystem *mode, FileRecord *file);
extern unsigned long StreamRead(FileRecord *file, void *buffer, unsigned long size);
extern int f_0043dcfd(ProfileManager *manager, int state);
extern void f_43dab1(ProfileManager *manager);

extern WinConfig gConfigSmall;
extern WinConfig gConfigDefault;
extern WinConfig gConfigAlt;
extern StrPool DAT_00511e7c;
extern void *g_mainWindow;
extern HINSTANCE g512580;
extern char gCurDir[];
extern GuiCanvas g513888;
extern "C" void RegisterAssertCallback(void (*callback)());
extern "C" void AssertExit(unsigned int code);
extern "C" void BriefWaitA(int);
extern "C" void DrainAssertCallbacks();
extern void f_47d710();
extern void f_47dde0();
extern void f_46b9bd();
extern void f_47682d();
extern int f_47b290(GuiCanvas *canvas);
extern int f_47b2d0(GuiCanvas *canvas);
extern void f_47d6d0();
extern void f_47dd80();
extern void f_476b3c();
extern int g512584;
extern HMENU g512544;
extern "C" char assertVersion[];
extern char *g4a425c;
extern int g512560;
extern int g512564;
extern int g512550;
extern int g512554;
extern int g512568;
extern int g5138ac;
extern int g5138b0;
extern int g4a48e4;
extern int g5125c0;
extern int g5125c4;
extern HDC g512538;
extern int g51256c;
extern int g512588;
extern int gViewX;
extern int gViewY;
extern int gViewW;
extern int gViewH;
extern int gViewChanged;
extern HACCEL g512540;
extern int g4a48f0;
extern int networkEnabled;
struct GuiRect;
extern GuiRect gMenuClip;
extern void GuiResetClip(void *clip);
extern int f_4765e6();
extern void f_47b7c0(GuiCanvas *canvas, int);
void LayoutGameWindow(int mode);
void ClearGameWindow();
extern "C" char *strlwr(char *text);
struct GameSettings { char head[0x14]; int detail; union { char video[0x18]; struct { int videoMode; int f1c; int noCaption; int largeWindow; int f28; int f2c; }; }; char rest[0xa3]; };
extern GameSettings gGameSettings;
extern void f_476860(GameSettings *settings);
extern int (*gOptionsDialog)(void *window, GameSettings *settings, void *dialogProc);
extern int OptionsDlgProc();
extern void f_00477295(char *text);
extern "C" void f_d2c0(void *);
extern char gShellResources;
extern void ShellSoundShutdown();
struct ModeSystem { int active; int IsActive() { return active; } };
extern ModeSystem modeCheckName;
extern void f_44bba0(ModeSystem *mode);
extern void f_44db70(ModeSystem *mode, int);
extern int gRedrawFlags;
extern HINSTANCE gInstance;
extern int gScreenWidth;
extern int gScreenHeight;
extern int gWindowX;
extern int gWindowY;
extern void f_4770d0();
extern void f_475a18(int settingsOk);
LRESULT CALLBACK WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
struct MenuKey { int key; int command; signed char shift; signed char control; };
extern MenuKey gMenuKeys[];
extern int f_47df40();
extern int f_47df80();
struct MenuNetwork { static int Active(); void DropPeer(int peer); };
extern MenuNetwork gMenuNetwork;
extern int StopMultiplayer(int code = 0, int restart = 0);
extern int f_4771aa(char *question);
extern int ModeCheck(const char *name);
extern int g51252c;
extern int g512530;
extern int g51257c;
extern unsigned long g51258c;
extern unsigned long g512590;
extern int f_475196();
extern void f_47519c();
extern void f_47b410(GuiCanvas *canvas);
extern void f_47d900(int x, int y, unsigned int buttons);
extern void f_475033(WPARAM wParam, LPARAM lParam);
extern void f_47dee0(int scan);
extern void f_47de70(int scan, int character, int command);
extern void f_47df00(int scan);
extern void f_47d980(int, int);
extern void f_476cda(int, int);
extern void MenuHelpSetTopic(long topic);
extern void f_47634d();
struct Scene { char pad[0x10]; int loaded; int IsLoaded() { return loaded != 0; } };
extern Scene g4b700c;
extern Scene g4b416c;
extern Scene g4ba2bc;
extern void f_41c7a3(Scene *scene);
extern void f_00417de7(void *scene);
extern void ShellNetworkSetupLoaded(Scene *scene);
extern void f_476412(int);
extern void f_476411();
extern int process_chat(void *chat);
extern char g4f71ac[];
extern StrPool resourcePool;
int LoadMission(const char *path, int showBriefing);
void HandleMenuKey(WPARAM key, LPARAM flags);
extern "C" int memcmp(const void *a, const void *b, unsigned int n);

WinConfig gWinConfig = gConfigDefault;

void __stdcall CenterWindow(HWND hwnd, UINT flags)
{
    RECT r;
    SIZE screen;
    screen.cx = GetSystemMetrics(SM_CXSCREEN);
    screen.cy = GetSystemMetrics(SM_CYSCREEN);
    GetWindowRect(hwnd, &r);
    int w = r.right - r.left;
    int h = r.bottom - r.top;
    HWND parent = GetParent(hwnd);
    int x, y;
    if (parent && (flags & 1)) {
        GetWindowRect(parent, &r);
        SIZE area;
        area.cx = r.right - r.left;
        area.cy = r.bottom - r.top;
        x = r.left + (area.cx - w) / 2;
        if (x + w > screen.cx) x = screen.cx - w;
        if (flags & 2) {
            y = r.top;
        } else {
            y = r.top + (area.cy - h) / 2;
            if (y + h > screen.cy) y = screen.cy - h;
        }
    } else {
        x = (screen.cx - w) / 2;
        if (flags & 2) {
            y = r.top;
        } else {
            y = (screen.cy - h) / 2;
        }
    }
    SetWindowPos(hwnd, 0, x, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
}

void ActivateMainWindow()
{
    if (g_mainWindow) {
        if (IsIconic((HWND)g_mainWindow)) ShowWindow((HWND)g_mainWindow, SW_RESTORE);
        SetForegroundWindow((HWND)g_mainWindow);
    }
}

void ShutdownCallback()
{
    f_47d710();
    f_47dde0();
    f_46b9bd();
    if (g512580) FreeLibrary(g512580);
}

int InitApplication()
{
    f_47682d();
    if (!f_47b290(&g513888)) {
        MessageBoxA((HWND)g_mainWindow, "This version of CyberStorm requires that WinG is installed.  Please install WinG to run CyberStorm.", "Error", MB_ICONHAND);
        return 0;
    }
    RegisterAssertCallback(ShutdownCallback);
    if (!f_47b2d0(&g513888)) return 0;
    f_47d6d0();
    f_47dd80();
    BriefWaitA(0);
    f_476b3c();
    gCurDir[0] = 0;
    GetCurrentDirectoryA(0xc3, gCurDir);
    if (strlen(gCurDir)) strcat(gCurDir, "\\");
    return 1;
}

void Problem(unsigned long id, int fatal)
{
    int locked = 0;
    char *text = "Unable to run this program at this time!";
    if (id > 0) {
        locked = !DAT_00511e7c.IsLocked();
        if (locked) DAT_00511e7c.Lock();
        char *s = DAT_00511e7c.GetString(id, 0);
        if (s) text = s;
    }
    MessageBoxA((HWND)g_mainWindow, text, "PROBLEM:", MB_OK | MB_ICONHAND | MB_SETFOREGROUND | MB_TASKMODAL);
    if (locked) DAT_00511e7c.Unlock();
    if (fatal) AssertExit(1);
}

void RunOptionsDialog()
{
    GameSettings old = gGameSettings;
    gOptionsDialog(g_mainWindow, &gGameSettings, (void *)OptionsDlgProc);
    if (memcmp(old.video, gGameSettings.video, sizeof old.video)) {
        int locked = !DAT_00511e7c.IsLocked();
        if (locked) DAT_00511e7c.Lock();
        f_00477295(DAT_00511e7c.GetString(200, 0));
        if (locked) DAT_00511e7c.Unlock();
    }
    if (gGameSettings.detail < old.detail) {
        f_d2c0(&gShellResources);
        ShellSoundShutdown();
    }
    if (modeCheckName.IsActive()) {
        f_44bba0(&modeCheckName);
        f_44db70(&modeCheckName, 0);
        gRedrawFlags |= 1;
    }
}

void SetMenuVisible(int show)
{
    if (gWinConfig.forceMenu) show = 1;
    if (show != g512584) {
        SetMenu((HWND)g_mainWindow, show ? g512544 : 0);
        DrawMenuBar((HWND)g_mainWindow);
        g512584 = show;
    }
}

BOOL CALLBACK AboutDlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_COMMAND:
        EndDialog(dlg, 1);
        break;
    case WM_INITDIALOG:
        SetDlgItemTextA(dlg, 200, assertVersion);
        SetDlgItemTextA(dlg, 201, g4a425c);
        return TRUE;
    }
    return FALSE;
}

void SetGameWindowSize(int w, int h)
{
    if (gWinConfig.resizable) {
        g512564 = g512560 = 0;
        if (w < 0 && h < 0) {
            w = g512550;
            h = g512554;
            g512564 = 1;
            if (gWinConfig.captionAdjust) {
                g512560 = f_4765e6();
                h -= g512560;
            }
        }
        if (h < 480) h = 480;
        if (h > 1200) h = 1200;
        if (w != g5138ac || h != g5138b0) {
            g5138ac = w;
            g5138b0 = h;
            f_47b7c0(&g513888, 0);
            f_47b2d0(&g513888);
        }
        LayoutGameWindow(g512568);
        g4a48e4 = 1;
    }
}

int GetWindowOffset(int *x, int *y)
{
    *x = g5125c0;
    *y = g5125c4;
    return g5125c0 || g5125c4;
}

void LayoutGameWindow(int mode)
{
    if (mode == 0) {
        gViewW = g5138ac;
        gViewH = g5138b0;
    } else {
        gViewW = g512550;
        gViewH = g512554;
    }
    if (mode == 1) {
        int w = g512554 * 640 / 480;
        int h = g512550 * 480 / 640;
        if (w < gViewW) gViewW = w;
        if (h < gViewH) gViewH = h;
    }
    gViewChanged = gViewW != g5138ac || gViewH != g5138b0;
    gViewX = (g512550 - gViewW) / 2 & ~3;
    gViewY = (g512554 - g512560 - gViewH) / 2;
    if (gViewX < 0) gViewX = 0;
    if (gViewY < 0) gViewY = 0;
    if (gWinConfig.f0 && !g512564) {
        gViewY = GetSystemMetrics(SM_CYCAPTION) / 2;
        gViewY = -gViewY;
    }
    f_47b7c0(&g513888, 0);
    GuiResetClip(&gMenuClip);
    ClearGameWindow();
}

void ClearGameWindow()
{
    HDC dc = g512538;
    if (!g51256c) {
        SelectObject(dc, GetStockObject(BLACK_BRUSH));
        Rectangle(dc, 0, 0, g512550, g512554);
    }
    GuiResetClip(&gMenuClip);
    g4a48e4 = 0;
}

int SaveHercConfig(void *herc, int arg)
{
    DebugTextClear();
    if (f_477352(0, "Herc Configuration file (*.hrc)|*.hrc|All files (*.*)|*.*||", "hrc", gDebugText) != 1) return 0;
    FileRecord file;
    if (!file.Open(gDebugText, 5)) {
        Problem(300, 0);
        return -1;
    }
    int result = f_43ee17(herc, &file, arg);
    IORelease(&file);
    if (!result) {
        Problem(300, 0);
        return -1;
    }
    return 1;
}

int LoadHercConfig(void *herc)
{
    DebugTextClear();
    if (f_477352(1, "Herc Configuration file (*.hrc)|*.hrc|All files (*.*)|*.*||", "hrc", gDebugText) != 1) return 0;
    FileRecord file;
    if (!file.Open(gDebugText, 0)) {
        Problem(302, 0);
        return -1;
    }
    int result = f_43ee71(herc, &file);
    IORelease(&file);
    if (result < 0) {
        Problem(302, 0);
        return result;
    }
    if (result > 0) {
        char text[512];
        sprintf(text, "Rank is not sufficient to load herc configuration.\n\nMinimum required rank: %d", result);
        ShowStatus((HWND)g_mainWindow, text, 0, 0);
    }
    return result;
}

// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
// source-line padding before the line-867 ASSERT
void SaveGame(const char *path)
{
    SaveHeader header;
    if (IsMultiplayer()) {
        if (!profileManager.Count()) {
            Problem(101, 0);
            return;
        }
        if (!PlayerCount()) {
            Problem(102, 0);
            return;
        }
        if (!HasMission(&profileManager)) {
            Problem(103, 0);
            return;
        }
    }
    if (path) {
        strcpy(gDebugText, path);
    } else {
        char *filter;
        char *extension;
        DebugTextClear();
        if (IsMultiplayer()) {
            filter = "CyberStorm Multiplayer file (*.cbm)|*.cbm||";
            extension = "cbm";
        } else {
            filter = "CyberStorm file (*.cbs)|*.cbs||";
            extension = "cbm";
        }
        if (f_477352(0, filter, extension, gDebugText) != 1) return;
    }
    g512594 = TimeNow();
    FileRecord file;
    if (!file.Open(gDebugText, 5)) {
        Problem(300, 0);
        return;
    }
    if (uiSystem.IsActive()) {
        header.network = 0;
        header.f10 = g48e4ec[uiSystem.mode];
    } else { int active=modeCheckName.IsActive();
        if(active) {header.network=1;header.f10=0;} else AssertFail(__FILE__,__LINE__,(const char *)active);


    }
    header.type = (IsMultiplayer() != 0) + 2;
    strncpy(header.description, "No default description", sizeof header.description);
    f_476d44(&header, &file);
    f_43edf0(&profileManager, &file);
    f_4524c0(&modeCheckName, &file);
    unsigned long tag = 0x3a444e45;
    StreamWrite(&file, &tag, 4);
    IORelease(&file);
    if (FileGetAndClearError(&file)) Problem(300, 0);
}

// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
// source-line padding before the line-984 ASSERT
int LoadMission(const char *path, int showBriefing)
{
    char text[512];
    SaveHeader header;
    int multi = IsMultiplayer();
    if (path) strcpy(gDebugText, path);
    else {
        char *filter, *extension;
        DebugTextClear();
        if (multi) {
            filter = "CyberStorm file (*.cbm,*.cbs)|*.cbm;*.cbs||";
            extension = "cbm";
        } else {
            filter = "CyberStorm file (*.cbs)|*.cbs||";
            extension = "cbm";
        }
        if (f_477352(1, filter, extension, gDebugText) != 1) return 0;
    }
    FileRecord file;
    if (!file.Open(gDebugText, 0)) { Problem(302, 0); return 0; }
    if (!f_476d78(&header, &file)) return 0;
    if (multi) {
        header.type = 3;
        header.network = 0;
    } else if (header.network == 0 && !uiSystem.IsActive()) {
        f_44b470(&modeCheckName, 3);
        uiSystem.Open();
    } else if (header.network == 1) {
        uiSystem.Close(7);
        f_44b470(&modeCheckName, 7);
        f_44aff0(&modeCheckName);
    }
    GameSession *session = new GameSession;
    ASSERT(session);
    memset(session, 0, sizeof(GameSession));
    f_0043ba3e(session, &profileManager);
    if (f_43ecf8(&profileManager, &file)) {
        int ok = 1;
        if (multi) {
            if (!profileManager.Count()) {
                Problem(104, 0);
                ok = 0;
            } else if (!PlayerCount()) {
                Problem(105, 0);
                ok = 0;
            } else if (PlayerCount() > 12) {
                int missing = 1;
                for (int i = 0; i < PlayerCount(); i++) {
                    if (!strcmp(g4cc3f7[i].name, "Big John")) {
                        missing = 0;
                        break;
                    }
                }
                if (missing) {
                    Problem(108, 0);
                    ok = 0;
                }
            } else if (!HasMission(&profileManager)) {
                Problem(106, 0);
                if (!f_43c844(&profileManager)) {
                    Problem(107, 0);
                    ok = 0;
                }
            }
        } else if (IsMultiplayer()) {
            Problem(109, 0);
            ok = 0;
        }
        if (!ok) {
            f_0043ba3e(&profileManager, session);
            delete session;
            return 0;
        }
        delete session;
    } else {
        f_0043ba3e(&profileManager, session);
        delete session;
    }
    f_452340(&modeCheckName, &file);
    unsigned long tag;
    StreamRead(&file, &tag, 4);
    IORelease(&file);
    if (FileGetAndClearError(&file) || (tag != 0x3a444e45 && !multi)) Problem(302, 0);
    if (multi && !IsMultiplayer()) {
        int saved = g4d0413;
        f_0043dcfd(&profileManager, 3);
        f_43dab1(&profileManager);
        g4d0413 = saved;
    }
    if (header.type == 3) return 1;
    if (!showBriefing) return 1;
    strcpy(text, BriefFormat("%s\n\n", gTextPool.GetString(228, 1)));
    if (header.network == 0) {
        strcat(text, gTextPool.GetString(229, 1));
        f_00477295(text);
        f_47de70(0, 0, 0x75fa);
    } else {
        strcat(text, gTextPool.GetString(230, 1));
        f_00477295(text);
    }
    return 1;
}

void PostCornerClick()
{
    if (g512588) {
        LPARAM lp = ((g512554 - 1) << 16) + (g512550 - 1);
        PostMessageA((HWND)g_mainWindow, WM_LBUTTONDOWN, MK_LBUTTON, lp);
        PostMessageA((HWND)g_mainWindow, WM_LBUTTONUP, 0, lp);
    }
}

inline int IsNetworkGame() { return networkEnabled; }

void HandleMenuKey(WPARAM key, LPARAM flags)
{
    if (flags & 0x40000000) return;
    for (MenuKey *entry = gMenuKeys; entry->key; entry++) {
        if (key != entry->key) continue;
        if (f_47df40() != entry->shift) continue;
        if (f_47df80() != entry->control) continue;
        UINT previous = EnableMenuItem(g512544, entry->command, MF_BYCOMMAND | MF_ENABLED);
        if (!previous) PostMessageA((HWND)g_mainWindow, WM_COMMAND, entry->command, 0);
        EnableMenuItem(g512544, entry->command, previous);
    }
}

inline int ConfirmExit()
{
    if (TimeNow() - g512594 > 30000) {
        MessageBeep(MB_ICONQUESTION);
        if (!f_4771aa("Exit to Windows?\n(Any unsaved changes will be lost)")) return 0;
        Sleep(500);
    }
    if (MenuNetwork::Active()) StopMultiplayer(0, 0);
    return 1;
}

LRESULT CALLBACK WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        if (MenuNetwork::Active()) StopMultiplayer();
        return 0;
    case WM_QUERYENDSESSION:
        return ConfirmExit();
    case WM_CLOSE:
        if (MenuNetwork::Active() && modeCheckName.IsActive() && ModeCheck((const char *)&modeCheckName) && !g51252c) {
            g51252c = 1;
            char *text = DAT_00511e7c.GetString(314, 1);
            if (text) {
                f_00477295(text);
                return 0;
            }
        }
        if (TimeNow() - g512594 > 30000) {
            MessageBeep(MB_ICONQUESTION);
            if (!f_4771aa("Exit to Windows?\n(Any unsaved changes will be lost)")) return 0;
            Sleep(500);
        }
        if (MenuNetwork::Active()) StopMultiplayer(0, 0);
        break;
    case WM_ENDSESSION:
        if (wParam == 1) DestroyWindow(window);
        break;
    case WM_PAINT:
        ClearGameWindow();
        GuiResetClip(&gMenuClip);
        break;
    case WM_SYSCOMMAND:
        switch (wParam & 0xfff0) {
        case SC_MINIMIZE:
            f_d2c0(&gShellResources);
            break;
        case SC_SIZE:
        case SC_MOVE:
            if (!gWinConfig.framed) return 0;
            break;
        case SC_MAXIMIZE:
            break;
        }
        break;
    case 0x812:
    {
        RECT rect;
        RECT frameRect;
        gScreenWidth = GetSystemMetrics(SM_CXSCREEN);
        gScreenHeight = GetSystemMetrics(SM_CYSCREEN);
        if (gScreenWidth == 640 && gScreenHeight == 480) {
            DWORD style = gWinConfig.framed ? WS_OVERLAPPEDWINDOW : WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX;
            SetRect(&rect, 0, 0, 640, 480);
            AdjustWindowRectEx(&rect, style, FALSE, 0);
            SetWindowPos(window, 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOACTIVATE);
        } else {
            SetWindowPos(window, 0, 50, 50, 660, 530, SWP_NOZORDER | SWP_NOACTIVATE);
            GetWindowRect(window, &frameRect);
            CenterWindow(window, 0);
            GetWindowRect(window, &frameRect);
        }
        break;
    }
    case WM_DISPLAYCHANGE:
        gScreenWidth = LOWORD(lParam);
        gScreenHeight = HIWORD(lParam);
        PostMessageA(window, 0x812, 0, 0);
        break;
    case WM_ACTIVATE:
        wParam = LOWORD(wParam) != 0;
    case WM_ACTIVATEAPP:
        g4a48f0 = wParam;
        g51257c = 1;
        if (gWinConfig.f8) {
            if (wParam) {
                SetWindowPos((HWND)g_mainWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
                g512530 = 1;
            } else if (g512530) {
                SetWindowPos((HWND)g_mainWindow, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
                g512530 = 0;
            }
        }
        break;
    case WM_GETMINMAXINFO: {
        MINMAXINFO *info = (MINMAXINFO *)lParam;
        RECT windowRect;
        RECT clientRect;
        GetWindowRect(window, &windowRect);
        GetClientRect(window, &clientRect);
        if (!clientRect.right || !clientRect.bottom) return 0;
        windowRect.right -= windowRect.left;
        windowRect.bottom -= windowRect.top;
        windowRect.right += 640 - clientRect.right;
        windowRect.bottom += 480 - clientRect.bottom;
        info->ptMinTrackSize.x = windowRect.right;
        info->ptMinTrackSize.y = windowRect.bottom;
        break;
    }
    case WM_PALETTECHANGED:
        if (g51257c) g51257c = 0;
        else if ((HWND)wParam == window) break;
    case WM_QUERYNEWPALETTE:
        if (!f_475196()) {
            HDC dc = g512538;
            f_47b410(&g513888);
            int changed = RealizePalette(dc);
            if (changed) InvalidateRect(window, 0, TRUE);
            return changed;
        }
        break;
    case WM_MOUSEMOVE:
        if (g512584) SetMenuVisible(0);
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP: {
        if (g512590) {
            unsigned long now = TimeNow();
            if (!g51258c) {
                if (now < g512590) g51258c = wParam;
                else g512590 = 0;
            } else if (!(wParam & g51258c)) {
                g512590 = 0;
                g51258c = 0;
            }
        }
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        g5125c0 = g5125c4 = 0;
        if (gWindowY + y >= gScreenHeight - 3) g5125c4 = -1;
        if (x <= 2) g5125c0 = -1;
        else if (x >= gScreenWidth - 3) g5125c0 = 1;
        if (g51258c) break;
        int px, py;
        if (gViewChanged) {
            px = g5138ac * x / gViewW - gViewX;
            py = g5138b0 * y / gViewH - gViewY;
        } else {
            px = x - gViewX;
            py = y - gViewY;
        }
        f_47d900(px, py, wParam);
        break;
    }
    case WM_NCLBUTTONDOWN:
    case WM_NCRBUTTONDOWN:
    case WM_NCMBUTTONDOWN:
        SetMenuVisible(1);
        break;
    case MM_MCINOTIFY:
        f_475033(wParam, lParam);
        break;
    case WM_MOVE:
        gWindowX = LOWORD(lParam);
        gWindowY = HIWORD(lParam);
        break;
    case WM_SIZE: {
        int w = LOWORD(lParam);
        int h = HIWORD(lParam);
        g4a48e4 = w != g512550 || h != g512554;
        g512550 = w;
        g512554 = h;
        LayoutGameWindow(g512568);
        if (f_475196()) f_47519c();
        break;
    }
    case WM_KEYDOWN: {
        HandleMenuKey(wParam, lParam);
        f_47dee0(lParam >> 16 & 0xff);
        signed char key = (signed char)wParam;
        if (key == VK_CLEAR || (key >= VK_PRIOR && key <= VK_DOWN) || (key >= VK_INSERT && key <= VK_DELETE) || (key >= VK_F1 && key <= VK_F10))
            f_47de70(lParam >> 16 & 0xff, 0, 0);
        break;
    }
    case WM_KEYUP:
        f_47df00(lParam >> 16 & 0xff);
        break;
    case WM_CHAR:
        f_47de70(lParam >> 16 & 0xff, wParam & 0xff, 0);
        break;
    case WM_NCMOUSEMOVE: {
        f_47d980(-1, -1);
        f_476cda(1, 1);
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        g5125c0 = g5125c4 = 0;
        if (y <= 2) g5125c4 = 1;
        else if (y >= gScreenHeight - 3) g5125c4 = -1;
        if (x <= 2) g5125c0 = -1;
        else if (x >= gScreenWidth - 3) g5125c0 = 1;
        break;
    }
    case WM_EXITMENULOOP:
        g512588 = 0;
        g512590 = TimeNow() + 50;
        SetMenuVisible(0);
        break;
    case WM_ENTERMENULOOP:
        g512588 = 1;
        if (!wParam) SetMenuVisible(1);
        break;
    case WM_MENUSELECT: {
        unsigned int item = LOWORD(wParam);
        if (!lParam || HIWORD(wParam) & MF_POPUP) item = 0;
        if (item >= 0x7724 && item <= 0x7787) item = 0x7724;
        MenuHelpSetTopic(item);
        f_47634d();
        break;
    }
    case WM_COMMAND:
        switch (wParam &= 0xffff) {
        case 0x7537:
            f_41c7a3(&g4b700c);
            break;
        case 0x7538:
            f_00417de7(&g4b416c);
            break;
        case 0x7531:
            if (IsMultiplayer() && !HasMission(&profileManager)) f_43c844(&profileManager);
            SaveGame(0);
            break;
        case 0x7532:
            LoadMission(0, 1);
            if (g4ba2bc.IsLoaded()) ShellNetworkSetupLoaded(&g4ba2bc);
            break;
        case 0x7536:
            PostMessageA(window, WM_CLOSE, 0, 0);
            break;
        case 0x7535:
            RunOptionsDialog();
            break;
        case 0x765c:
            f_476412(-1);
            break;
        case 0x765d:
            f_476412(2);
            break;
        case 0x765e:
        case 0x7661:
            f_476411();
            break;
        case 0x7669:
            process_chat(g4f71ac);
            break;
        case 0x7663:
            StopMultiplayer();
            break;
        case 0x7664:
            gMenuNetwork.DropPeer(lParam);
            break;
        case 0x7665:
            f_00477295(resourcePool.GetString(8, 0));
            break;
        case 0x7666:
            f_00477295(resourcePool.GetString(9, 0));
            break;
        case 0x7662:
            DialogBoxParamA(gInstance, "AppAbout", window, (DLGPROC)AboutDlgProc, 0);
            break;
        default:
            f_47de70(0, 0, wParam);
            break;
        }
        return 0;
    }
    return DefWindowProcA(window, msg, wParam, lParam);
}

void GuiPumpMessages()
{
    MSG msg;
    msg.message = 0;
    while (msg.message != WM_QUIT) {
        if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
            if (!TranslateAcceleratorA((HWND)g_mainWindow, g512540, &msg)) {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        } else {
            if (g4a48f0 || IsNetworkGame()) break;
            WaitMessage();
        }
    }
    if (msg.message == WM_QUIT) AssertExit(0);
}

int ApplySettings(char *commandLine)
{
    int ok = 1;
    f_476860(&gGameSettings);
    if (gGameSettings.largeWindow) gWinConfig = gConfigDefault;
    else gWinConfig = gConfigAlt;
    gWinConfig.captionAdjust = gGameSettings.noCaption == 0;
    gWinConfig.width = gGameSettings.videoMode;
    gWinConfig.f18 = 0;
    if (GetSystemMetrics(SM_CYSCREEN) < 550) gWinConfig = gConfigSmall;
    gWinConfig.resizable = gWinConfig.width;
    gWinConfig.f8 = gGameSettings.f2c;
    gWinConfig.f24 = gGameSettings.f28;
    if (!gWinConfig.noMenuSize) gWinConfig.captionAdjust = gWinConfig.noMenuSize;
    if (commandLine) strlwr(commandLine);
    return ok;
}

int PASCAL WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR commandLine, int show)
{
    WNDCLASS wc;
    gInstance = instance;
    HWND existing = FindWindowA("CYBRID_GAMEWIND", 0);
    if (existing) {
        if (IsIconic(existing)) ShowWindow(existing, SW_RESTORE);
        SetForegroundWindow(existing);
        return 0;
    }
    int settingsOk = ApplySettings(commandLine);
    wc.style = CS_OWNDC | CS_BYTEALIGNCLIENT;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = instance;
    wc.hIcon = LoadIconA(instance, "AppIcon");
    wc.hCursor = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "CYBRID_GAMEWIND";
    if (!RegisterClassA(&wc)) {
        Problem(-1, 0);
        return 0;
    }
    gScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    gScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    if (gWinConfig.noMenuSize) {
        g512550 = gScreenWidth + GetSystemMetrics(SM_CXBORDER) * 2;
        g512554 = gScreenHeight + GetSystemMetrics(SM_CYBORDER) * 2;
        gWindowX = GetSystemMetrics(SM_CXBORDER);
        gWindowX = -gWindowX;
        gWindowY = GetSystemMetrics(SM_CYBORDER);
        gWindowY = -gWindowY;
    } else {
        g512550 = g5138ac + GetSystemMetrics(SM_CXFRAME) * 2;
        g512554 = g5138b0 + GetSystemMetrics(SM_CYFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU);
        gWindowX = (gScreenWidth - g512550) / 2 & ~3;
        gWindowY = (gScreenHeight - g512554) / 2;
    }
    g512544 = LoadMenuA(gInstance, "AppMenu");
    g512540 = LoadAcceleratorsA(gInstance, "AppAccel");
    DWORD style = gWinConfig.framed ? WS_OVERLAPPEDWINDOW : WS_MAXIMIZE | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX;
    g_mainWindow = CreateWindowExA(0, "CYBRID_GAMEWIND", "CyberStorm          ",
        style,
        gWindowX, gWindowY, g512550, g512554, 0, 0, instance, 0);
    g512538 = GetDC((HWND)g_mainWindow);
    if (!g_mainWindow || !g512538) {
        Problem(-1, 0);
        return 0;
    }
    if (!InitApplication()) Problem(-1, 1);
    ShowWindow((HWND)g_mainWindow, show);
    UpdateWindow((HWND)g_mainWindow);
    g512580 = LoadLibraryA("cwarsdll.dll");
    if (g512580 <= 0) Problem(-1, 1);
    f_4770d0();
    SetMenuVisible(0);
    f_475a18(settingsOk);
    DrainAssertCallbacks();
    return 0;
}
