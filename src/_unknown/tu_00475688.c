// Shell setup and screen loop. Names/layouts remain provisional where no path evidence exists.
#include <windows.h>
struct Settings { int values[27]; char tail[103]; }; extern Settings gGameSettings;
int SettingsSave(Settings*); void f_46b9bd(); int SoundInit(); void f_47d710(); void HelpShutdown(); void f_47dde0();
struct MciState { unsigned device,pending,first,last,state; int Open(int,int); void Close(); }; extern MciState gShellResources;
struct MenuNetwork { static int Active(); }; int StopMultiplayer(int,int); void link_force_shutdown();
void ShellShutdown() {
 SettingsSave(&gGameSettings); f_46b9bd(); f_47d710(); HelpShutdown(); f_47dde0(); gShellResources.Close();
 if(MenuNetwork::Active()) StopMultiplayer(0,0);
 link_force_shutdown();
}
void ShellIdle() {}
extern unsigned long gSessionTable[];
void FormatSetBuffer(void*,unsigned);
extern "C" char *BriefFormat(const char*,...);
int FileExists(const char*); char FindGameCD(int);
extern char *cdPath;
extern int gameSessionMode,helpWindowEnabled;
extern char *searchPaths[];
extern const char *resourceArchives[];
struct ResMgr { int Init(const char **volatile,const char*); }; extern ResMgr gResMgr;
void InputInit(); void FrameInputInit();
void GdiTextSetMode(int); int GdiTextInit(); int f_4765e6(); void HelpInit();
long FileFreeSpace(char); void f_00477295(const char*); void Problem(unsigned long,int);
struct StrPool { long id; char *buffer; long capacity; void *data; long size; int Lock(); char *GetString(long,int); };
extern StrPool DAT_00511e7c,g511ddc,gHercStrings,gMiniMenuStrings;
extern "C" void RegisterAssertCallback(void (*)());
struct Font { void *data; }; extern Font shellFont,g511dc0,gDefaultFont;
extern "C" int f_cba0(Font*,const char*);
void f_0047e200(int*); extern int MissionRandomTable[]; extern unsigned gFxSeed;
int ShellInit() {
 FormatSetBuffer(gSessionTable,16384); char drive=0;
 if(FileExists("CYBDATA4.RBX")) drive='#';
 if(!drive) { char found=FindGameCD(1); *cdPath=found; if(!found) return 0; }
 gameSessionMode=0;
 for(int i=1;i<=4;i++) { if(!FileExists(BriefFormat("CYBDATA%d.RBX",i))) break; gameSessionMode=i; }
 gResMgr.Init(resourceArchives,(const char*)searchPaths);
 SoundInit(); InputInit(); FrameInputInit();
 GdiTextSetMode((helpWindowEnabled!=0)+1); GdiTextInit(); f_4765e6(); HelpInit();
 gShellResources.Open(0,999);
 if(FileFreeSpace(0)<30000000) f_00477295(DAT_00511e7c.GetString(313,1));
 RegisterAssertCallback(ShellShutdown);
 if(!f_cba0(&shellFont,"font0.fnx") || !f_cba0(&g511dc0,"font1.fnx") || !f_cba0(&gDefaultFont,"font2.fnx")) Problem(307,1);
 g511ddc.Lock(); gHercStrings.Lock(); gMiniMenuStrings.Lock(); f_0047e200(MissionRandomTable); f_0047e200((int*)&gFxSeed); return 1;
}
void HelpUpdate(); void CursorUpdate(); void SimSetCursor(int,int); void ViewFlip(int); void GuiPumpMessages();
void ShellServiceFrame() { HelpUpdate(); CursorUpdate(); SimSetCursor(1,1); ViewFlip(0); GuiPumpMessages(); }
extern unsigned long frameTime;
unsigned long TimeNow(); void TimeSleep(unsigned long); void GameClockSet(unsigned long);
struct ActiveState { int active; };
class UiSystem : public ActiveState { public: virtual ~UiSystem(); virtual void Open(); virtual void Close(); virtual void Service(); virtual void Paint(); int IsActive() { return active; } };
extern UiSystem modeCheckName,uiSystem;
inline int ModeActive() { return modeCheckName.active; }
void ShellLimitFrameRate() {
 TimeNow(); int rate=ModeActive()?45:60; unsigned long now;
 for(;;) { unsigned long deadline=frameTime+1000/rate; now=TimeNow(); if(now>=deadline) break; TimeSleep(5); }
 GameClockSet(now); frameTime=now;
}
void HelpClearTopic(); void SoundClearContext();
extern int gShellExitCode;

int ShellRunScreen(UiSystem *screen) {
 screen->Open();
 while(screen->active) {
  ShellServiceFrame(); if(screen->active) { screen->Service(); screen->Paint(); } ShellLimitFrameRate();
 }
 SimSetCursor(1,0); CursorUpdate(); HelpClearTopic(); SoundClearContext(); return gShellExitCode;
}

void f_44b470(UiSystem*,int);
void ShellRun(int initial) {
 if(!ShellInit()) return;
 gShellExitCode=initial;
 while(gShellExitCode) {
  switch(gShellExitCode) {
  case 1: if(gGameSettings.values[0]) gShellExitCode=2; else gShellExitCode=5; break;
  case 2: case 3: case 4: case 5: ShellRunScreen(&uiSystem); break;
  case 6: f_44b470(&modeCheckName,6); ShellRunScreen(&modeCheckName); break;
  case 7: case 10: ShellRunScreen(&modeCheckName); break;
  case 8: f_44b470(&modeCheckName,8); ShellRunScreen(&modeCheckName); break;
  case 9: gShellExitCode=1; break;
  }
 }
}

