/* Mission debriefing screen (shell\dbrief.c). */
typedef unsigned long U32;typedef unsigned char byte;
struct GuiControl;struct InputReq;struct StrPool;struct ProfileManager;
struct ViewRect{long left,top,right,bottom;};
struct Palette{U32 colors[0x114];unsigned short tail;};
struct Surface{long width,height;void *pixels;U32 flags;};
struct Timer;extern void TimerSet(Timer *,U32);struct Timer{U32 words[3];Timer(U32 n){TimerSet(this,n);}};
struct Flic {char header[0x20];Surface frame;char unknown30[12];Palette *palette;char unknown40[12];long loops;char unknown50[64];Flic();~Flic();int Open(long,int=100,int=0);};
class GuiScreen{char unknown[0x10];public:int active;char rest[0x38];GuiScreen(GuiControl **,unsigned,U32);~GuiScreen(){active=0;}void LoadLayout(long);void Open(InputReq *,StrPool *,long,int);};
#pragma pack(push,1)
class MissionDebrief:public GuiScreen{public:
 long mission;byte unknown50[6],variant;Timer timer;Flic animation;long current,unknownF7,textHeight,textWidth;
 MissionDebrief();int IsActive(){return active!=0;}void LoadBackground();void DrawMap();
};
#pragma pack(pop)
extern GuiControl *debriefControls[5];extern StrPool gShellStrings;extern ProfileManager profileManager;
extern ViewRect viewportClipLeft,gBriefDrawSource;extern Palette gShellBackground;
extern long g5138ac,g5138b0;extern char g513888,g511948;extern int g4bb78c;
extern long debriefResult;long debriefTiles[]={-1,1,1,-1,1,1,2,2,0,0};
struct DebriefTextStyle {long flags,spacing,low,middle,high;};
DebriefTextStyle debriefTextStyle={0,10,80,150,230};
static char dbriefSourceFile[]=__FILE__;
extern int ProfileValue(ProfileManager *);
extern "C" void BriefWaitA(int);extern "C" void BriefWaitB(int);extern "C" void BriefCloseAudio();extern "C" void BriefCloseWait();
extern "C" void BriefCloseScreen(char *);extern "C" int BriefGetHandle(void *,void *);extern "C" void BriefStateSet(void *);
extern "C" int BriefSwitch(void *,void *);extern "C" void BriefStatus(void *);extern "C" void BriefRenderUpdate(void *);
extern "C" void BriefDone(const char *);
extern "C" void BriefSetMode(void *,int);extern "C" void BriefAdjust(void *,int);
extern "C" void BriefBlit(void *,int,int,int,int,int,int,int,int);
extern "C" void BriefRender(int,int,int,int,int,int,int,int,int *,int *,void *,int,int=2,int=0,int=0,int=0x7fffffff);
extern "C" void *BriefLookup(ProfileManager *,int);extern "C" char *BriefFormat(const char *,...);
extern void ShellApplyPalette(Palette *);
#pragma aux debrief_button_constructor "BriefModuleInitA";
#pragma aux debrief_help_constructor "BriefModuleInitB";
#pragma aux debrief_range_constructor "BriefModuleInitC";
#pragma aux debrief_bar_constructor "BriefModuleInitD";
#pragma aux debrief_arrow_constructor "BriefModulePlace";
class MenuButton{public:char data[40];__pragma("debrief_button_constructor") MenuButton(int,int);~MenuButton();};
class MenuHelpRegion{public:char data[40];__pragma("debrief_help_constructor") MenuHelpRegion();~MenuHelpRegion();};
class UpgradeRange{public:int current,maximum;__pragma("debrief_range_constructor") UpgradeRange(int,int);};
class UpgradeBar{public:char data[40];__pragma("debrief_bar_constructor") UpgradeBar(UpgradeRange*,int);~UpgradeBar();};
class UpgradeArrow{public:char data[44];__pragma("debrief_arrow_constructor") UpgradeArrow(UpgradeRange*,int,int);~UpgradeArrow();};
MenuButton debriefButton(0,0);MenuHelpRegion debriefHelp;UpgradeRange debriefRange(50,0);UpgradeBar debriefBar(&debriefRange,0);
UpgradeArrow debriefNext(&debriefRange,20,48),debriefPrevious(&debriefRange,-20,45);
MissionDebrief g4b3598;
GuiControl *debriefControls[5]={(GuiControl *)&debriefButton,(GuiControl *)&debriefPrevious,(GuiControl *)&debriefBar,(GuiControl *)&debriefNext,(GuiControl *)&debriefHelp};
MissionDebrief::MissionDebrief():GuiScreen(debriefControls,5,0),timer(0),animation(){}
extern "C" void ShellMissionBegin(MissionDebrief *screen,InputReq *input){
 if(screen->IsActive())return;
 BriefWaitA(6);viewportClipLeft=gBriefDrawSource;BriefWaitB(27);BriefCloseAudio();debriefResult=-1;
 screen->mission=ProfileValue(&profileManager);screen->current=0;
 screen->LoadLayout((long)"missdbrf.box");screen->Open(input,&gShellStrings,2801,1);
 BriefSetMode(&debriefRange,0);screen->LoadBackground();
 BriefBlit(&g513888,0,1,320,0,320,0,285,480);screen->DrawMap();
 BriefAdjust(&debriefRange,screen->textHeight-357);
}
extern "C" void ShellMissionClose(MissionDebrief *screen){
 if(screen->IsActive()){screen->active=0;BriefCloseWait();BriefCloseAudio();BriefCloseScreen((char *)&screen->animation);}
}
extern "C" void ShellMissionExit(MissionDebrief *screen){
 if(screen->IsActive()){debriefResult=BriefGetHandle(screen,&g4bb78c);BriefStateSet(screen);}
}
extern "C" void ShellMissionRender(MissionDebrief *screen){
 if(screen->IsActive()){
  BriefSwitch(&screen->animation,&g511948);
  int previous=screen->current;int current=debriefRange.current;screen->current=current;
  if(previous!=current){BriefBlit(&g513888,1,0,320,0,320,0,285,480);screen->DrawMap();BriefStatus(screen);BriefRenderUpdate(screen);}
  BriefRenderUpdate(screen);
 }
}
void DebriefSetPolygon(){BriefDone("missbrf");}
void MissionDebrief::DrawMap(){
 for(int i=0;;i+=2){
  int empty=(debriefTiles[i]|debriefTiles[i+1])==0;
  BriefRender(debriefTiles[i]+320,debriefTiles[i+1]+30,285,377,mission,current,-1,6,(int *)&textHeight,(int *)&textWidth,&debriefTextStyle,empty?16:0,2,!empty);
  if(empty)break;
 }
}
struct MissionEntry{byte chapter,mission;byte unknown[42];};
inline long DebriefWidth(){return g5138ac;}inline long DebriefHeight(){return g5138b0;}
void MissionDebrief::LoadBackground(){
 MissionEntry entry=*(MissionEntry *)BriefLookup(&profileManager,-9);
 byte chapter=entry.chapter+1;
 char *name=BriefFormat("BG_%1d%02d%c.FLX",chapter,entry.mission,variant+65);
 Flic *f;(f=&animation)->Open((long)name);
 BriefCloseWait();BriefCloseAudio();
 char *region=(char *)&animation;region+=0x20;int open=*(int *)(region+4);
 if(open){
  viewportClipLeft=gBriefDrawSource;BriefSwitch(f,&g511948);
  BriefBlit(&g513888,0,1,0,0,0,0,DebriefWidth(),DebriefHeight());
  gShellBackground=*f->palette;ShellApplyPalette(&gShellBackground);f->loops=-1;
 }
}

