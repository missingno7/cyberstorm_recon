// sim\sim.c, full function bodies. Unbound names and unnamed fields are provisional.
#pragma pack(push,1)
typedef unsigned long U32; static char simSourceFile[]=__FILE__;
struct GuiRect { long x,y,width,height; };
struct GridPoint {long x,y;GridPoint(){}GridPoint(long a,long b){x=a;y=b;}};
struct ClipState { long words[10]; };
struct GameTimer; extern void GameTimerSet(GameTimer *,U32); extern int GameTimerExpired(GameTimer *);
struct GameTimer {U32 start,end,duration;GameTimer(U32 n){GameTimerSet(this,n);}};
extern int gShellExitCode;
class ShellBase { public: long active; long IsActive()const{return active;} ShellBase():active(0){} virtual ~ShellBase(){Close(0);} virtual void Open(){active=1;}virtual void Close(int n){active=0;gShellExitCode=n;}virtual void Update()=0;virtual void Event(unsigned char)=0;};
struct ViewState {GridPoint point;long zoom,scale,direction;U32 flags,requested;long spare;ViewState();void Set(long z,GridPoint p,long d,U32 f,long n){point=p;zoom=z;direction=d;requested=f;spare=n;flags=requested;}};
struct SimAnim {unsigned char bytes[8];};
struct PlayerSlot {int status,id,points,actor;unsigned char rest[0xbc-16];};
struct InputReq {unsigned long mask,flags; InputReq(unsigned long n):mask(n){} };
struct ActorRecord {int id;unsigned char unknown04[0x20-4];unsigned char actionData[6],actionMode;unsigned char unknown27[0x42-0x27];int index;};
class SimGame:public ShellBase {
public:
 InputReq input;int unknown10,uiState,selectedMode,previousMode,savedMode; int worldOpen,unknown28,screen,pending;
 int focusIndex; ActorRecord *focus;
 unsigned char unknown3c[0x68-0x3c];SimAnim portrait,face,statusAnim;unsigned char unknown80[0x2ba-0x80];int phase;
 int round,multi,waiting,tick,deadline,action,palette,playerCount;PlayerSlot slots[9];PlayerSlot *slot;int mode,modeType,player,localPlayer,viewOwner,linked;
 int remote,automated,nextPlayer,queued;unsigned char unknown9a6[0xdf5-0x9a6];ViewState bookmarks[13],view,savedView;
 unsigned char unknownfd5[0x10f1-0xfd5];int extraSaved;
 int finishHotseat;unsigned char unknown10f9[0x112d-0x10f9];int persistentLast;
 SimGame():input(0x8004){} virtual void Open(); virtual void Close(int);virtual void Update();virtual void Event(unsigned char);
};
#pragma pack(pop)
SimGame modeCheckName;
extern int gameSessionMode,ambientSetting;
extern char gShellResources;
extern unsigned gFxSeed;
extern "C" int f_d270(void *);
extern "C" void f_d2f0(void *,int);
extern "C" unsigned next_link_seed(unsigned *);
#pragma pack(push,1)
struct GuiPoint {long x,y;};
struct StrPool {long id;char *buffer;long capacity;void *data;long size;StrPool(long,long=0,long=0);~StrPool();int Lock();void Unlock();char *GetString(long,int=1);};
struct GuiControlData:GuiRect {unsigned char style[4];U32 attributes,shortcut;};
class GuiControl:public GuiControlData {public:virtual ~GuiControl();virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);virtual int HitTest(const GuiPoint *);};
class MenuNumber:public GuiControl {public:U32 unused;const char *text;long value;signed char font,color,flash,align;MenuNumber(signed char=-1,signed char=-1);};
class MenuButton:public GuiControl {public:U32 state;const char *text;MenuButton(const char * =0,U32=0);};
class MenuSpacer:public GuiControl {public:MenuSpacer();};
class MenuImageLabel:public GuiControl {public:const char *text;long image;signed char alignX,alignY,font,unused,color,bevel;MenuImageLabel(long=0,signed char=0,signed char=0,signed char=1,signed char=0,signed char=11,signed char=0);virtual ~MenuImageLabel();};
class HercControl:public GuiControl {public:U32 state;HercControl();};
class GuiScreen:public GuiRect {public:InputReq *input;unsigned char rest[0x4c-20];~GuiScreen(){ResetInput();}void ResetInput(){input=0;}void Invalidate();};
struct Surface {long width,height;unsigned char *pixels;U32 flags;~Surface();};
struct Flic {unsigned char bytes[0x90];~Flic();};
struct FileRecord {U32 handle,open,flags,error;signed char direct,counted;unsigned char reserved[2];U32 position,length;~FileRecord();};
class PopHerc:public GuiScreen {public:
 unsigned char unknown4c[0x61c-0x4c];FileRecord file;unsigned char unknown638[0x743-0x638];
 MenuButton first,second;MenuSpacer spacer;MenuNumber number;MenuImageLabel labels[4];HercControl controls[24];unsigned char unknownbfb[0xca5-0xbfb];
 PopHerc();
};
class PilotScreen:public GuiScreen {public:
 unsigned char unknown4c[16];Flic movie;Surface surface;unsigned char unknownfc[0x7a6-0xfc];
 MenuNumber numbers[2];MenuImageLabel labels[13];MenuSpacer spacers[8];MenuNumber number;MenuButton button;GuiControl last;unsigned char unknownbd4[0xc54-0xbd4];
 PilotScreen();void Begin(void *);void Select(ActorRecord *);void Close();
};
#pragma pack(pop)
extern PopHerc *DAT_004d1fa4;extern PilotScreen *gPilotScreen;
class MovementPopup:public GuiScreen {public:void Close();};class WeaponPopup:public GuiScreen {public:void Close();};class ReportScreen:public GuiScreen {};
extern MovementPopup simPanel;extern WeaponPopup simActions;extern GuiScreen simInventory;extern ReportScreen simPilot;struct PopNode;extern PopNode g4d1dec;
struct PopInfo;struct PopWorld;struct SimAlert;extern PopInfo popInfo;extern PopWorld popWorld;extern SimAlert g4d4094;
extern void PopNodeClear(PopNode *),SimInventoryClose(void *),ReportClose(ReportScreen *),PopInfoClose(PopInfo *),PopWorldClose(PopWorld *);
extern void f_00445cfe(int);
extern "C" void ShellRestore(SimGame *);
void SimClosePopups(SimGame *s) {
 s->selectedMode=0;
 simPanel.Close();simActions.Close();PopNodeClear(&g4d1dec);SimInventoryClose(&simInventory);ReportClose(&simPilot);PopInfoClose(&popInfo);PopWorldClose(&popWorld);
 if(gPilotScreen){gPilotScreen->Close();delete gPilotScreen;gPilotScreen=0;ShellRestore(s);}
 if(DAT_004d1fa4){f_00445cfe((int)DAT_004d1fa4);delete DAT_004d1fa4;DAT_004d1fa4=0;ShellRestore(s);}
 ((GuiScreen *)&g4d4094)->Invalidate();
}
struct ViewRect {long left,top,right,bottom;};extern ViewRect viewportClipLeft,g4d381c;
extern int g5138b0;
extern GuiRect gMenuClip;
extern char g513888;
extern void GuiResetClip(void *),PopDrawSurface(void *,int,int,long,long,long,long);
extern void SimAlertSetValue(void *,int);
extern void SimPanelOpen(void *,void *),SimActionsOpen(void *,void *),SimInventoryOpen(void *,void *),SimPilotOpen(void *,void *);
extern void PopNodeOpen(PopNode *,void *),PopInfoOpen(PopInfo *,InputReq *),PopWorldOpen(PopWorld *,InputReq *);
extern void f_0044579c(int,int),SimHercSelect(PopHerc *,ActorRecord *);
extern "C" void f_d2c0(void *);
void PilotSetState(SimGame *s,int mode) {
 if(!s->linked && mode!=7 && mode!=8 && mode!=9)mode=7;
 if(mode!=s->selectedMode) {
  s->previousMode=s->selectedMode;
  SimClosePopups(s);
  s->worldOpen=1;s->selectedMode=mode;s->screen|=8;
  if(mode!=6 && mode!=5) {
   int previous=s->previousMode;GridPoint point(0,167);if(previous==7){GuiResetClip(&gMenuClip);point.y=139;}
   viewportClipLeft=g4d381c;
   PopDrawSurface(&g513888,1,0,point.x,point.y,148,g5138b0-point.y);SimAlertSetValue(&g4d4094,s->selectedMode);
  }
  switch(mode) {
  case 0:break;
  case 1:SimPanelOpen(&simPanel,&s->input);s->savedMode=1;return;
  case 2:SimActionsOpen(&simActions,&s->input);s->savedMode=2;return;
  case 3:PopNodeOpen(&g4d1dec,&s->input);s->savedMode=3;return;
  case 4:SimInventoryOpen(&simInventory,&s->input);s->savedMode=4;return;
  case 8:PopInfoOpen(&popInfo,&s->input);return;
  case 9:PopWorldOpen(&popWorld,&s->input);return;
  case 7:SimPilotOpen(&simPilot,&s->input);return;
  case 6:{f_d2c0(&gShellResources);gPilotScreen=new PilotScreen;gPilotScreen->Begin(&s->input);gPilotScreen->Select(s->focus);s->worldOpen=0;return;}
  case 5:{f_d2c0(&gShellResources);DAT_004d1fa4=new PopHerc;f_0044579c((int)DAT_004d1fa4,(int)&s->input);SimHercSelect(DAT_004d1fa4,s->focus);s->worldOpen=0;break;}
  }
 }
}
void SimAmbientSound(SimGame *s) {
 if(gameSessionMode!=1 && s->worldOpen && ambientSetting>=50) {
  static GameTimer timer(0);
  if(GameTimerExpired(&timer)) {GameTimerSet(&timer,15000);if(f_d270(&gShellResources))f_d2f0(&gShellResources,next_link_seed(&gFxSeed)%3+2);}
 }
}
extern int gGuiPaletteFaded;
struct FogBuffer {int mode;int Resize();int Mode()const{return mode;}}; extern FogBuffer g4d35f4;
extern char g513888;
extern GuiRect gMenuClip,g4d3c9c,simClip,g4d3cec,simSmallClip;
extern ClipState simMainClip,simMainSaved,simDetailClip,simDetailSaved;
extern void GuiFadeIn(),SurfaceFlush(void *);
extern int SimClipIntersects(const GuiRect *,const void *);
extern void SimClipFlush(void *),SimClipRestore(void *);
struct TerrainGrid {long fields[17];void *cells,*neighbors;long spare;long Count()const{return fields[10];}};struct FastBuffer;struct FastCoord;
extern TerrainGrid fallbackName;extern FastBuffer g4d35e4;extern FastCoord gFastTransform;
extern void f_0047e200(int *),ShellMultiplayerInit(void *),ShellMultiplayerShutdown(void *),SetGameWindowSize(int,int),SurfaceAttach(void *,void *,long,long),SimGameEnd(int),SimSetupWorld(SimGame *),SimTargetInitialize();
extern int SurfaceCreate(void *,long,long);extern void SurfaceFree(void *);
extern char g4b74f4,g511948,gMenuSavedSurface,gShellBackground[],g4dd0c0[];
extern void *gShellSurfacePixels,*gShellDrawPixels,*savedSurfacePixels;
extern long g5138ac;inline long SurfaceWidth(){return g5138ac;}inline long SurfaceHeight(){return g5138b0;}inline void *SurfacePixels(){return gShellSurfacePixels;}
struct MenuImages;extern MenuImages *gMenuImages,*drawCursorImages;
struct ResMgr {void *Load(const char *,int=1,void * =0,U32=0);};extern ResMgr gResMgr;
extern "C" void AssertFail(const char *,int,const char *);extern "C" void *memset(void *,int,unsigned);
extern StrPool targetBriefResource,gShellMiniStrings,gShellParaStrings;
extern void SetPointer(StrPool *,const char *);extern "C" int BriefWaitB(int);
extern long GridInitialize(TerrainGrid *,long,long);extern int FastBufferCreate(FastBuffer *,long);
extern long simGridCount;
extern void SimBestLocOpen(void *),SimBestLocClose(void *),SimFastSetFog(FastCoord *,FogBuffer *);
extern int bestLocCursor,bestLocCount;extern unsigned char bestLocEntries[];
extern FogBuffer *simFogSource;
extern void ActorListConstruct(),ActorListDestroy(),WorldActionInitialize(),WorldActionShutdown(),ExploInit(),ExploShutdown();
extern void SimAlertOpen(SimAlert *,InputReq *),SimAlertClose(SimAlert *),TargetMapAlert(void *,InputReq *),TargetClearActive(void *);
extern void SimHotStartMission(SimGame *),SimStartMission(SimGame *),SimSetMode(SimGame *,int),SimSetView(SimGame *,int),SimSetMenuState(SimGame *,int);
extern int simInitialPalette;extern unsigned char simInitialWarning;
extern ActorRecord *ActorFindFree(),*ActorFindNext(ActorRecord *);
extern void AnimActionReplace(void *,int),GuiFilterPalette(void *,int),SimHotResizeFogBuffers(),SimHotReleaseFogBuffers();
extern void ResetMultiplayerInput(),create_chat_menu(void *),destroy_chat_menu(void *);
extern char simChatMenu;extern int g4faf44;
extern short mainSelectedMission;extern unsigned long missionScripts[][22];
extern int simWorldBackground,simWorldForeground;
extern void SimGameLoadMissions(SimGame *),SigsStart(void *,int),SurfacePresent(void *),ReleasePoolData(void *),LogicTextBegin(void *),GridDestroy(TerrainGrid *),FastBufferDestroy(FastBuffer *),FogRelease(FogBuffer *),HotbriefEmpty(void *),ReportAnimClose(SimAnim *),TerrainReleaseImage();
extern int AnimBitmapSweep(int,int,int,int,int);
extern void SimSetUpdate(SimGame *,int);
struct Timer {U32 start,end,duration;};extern Timer gFogTimer;
extern int TimerExpired(Timer *),retryTimeout,ModeCheck(const char *),SimOnlyStationary(SimGame *);
extern void MenuHelpSetTopic(long),PilotManagerPoll(void *),flush_chat_log(),NetworkPoll();
struct MenuNetwork {int Poll();};extern MenuNetwork gMenuNetwork;
struct PacketList {void Poll();void Process();void EndTurn(int);};extern PacketList modeLogName,networkLog;
extern U32 TimeNow();extern void InputPoll(InputReq *),InputReset(InputReq *);
extern int SimHotReadTimer(char *),SimChooseNextPlayer(SimGame *);
extern void SimResetLimits(SimGame *),SimChangePlayer(SimGame *,int),SimBeginTurn(SimGame *),SimStartPhase(SimGame *),SimUpdateOwnerPayloads(SimGame *);
struct RepeatInput;extern void SimPollRepeat(RepeatInput *);
extern PlayerSlot *GetModePlayer(void *,int);extern StrPool DAT_00511e7c;
extern void f_00477295(char *),SaveGame(const char *);extern "C" char *BriefFormat(const char *,...);
extern int gInDirty,targetLimits;
#pragma aux report_input_name "W?ReportGlobalInput$n()v";
void __pragma("report_input_name") ReportGlobalInput(SimGame *);
extern void SimAlertEvent(SimGame *),TargetHandleEvent(SimGame *),SimWorldStep(SimGame *),MovementCommand(SimGame *),WeaponCommand(SimGame *),SimInventoryEvent(SimGame *),PopSystemCallback(void *);
#pragma aux callback_popworldevent "W?PopWorldEvent$n()i";
#pragma aux callback_popinfoevent "W?PopInfoEvent$n()i";
int __pragma("callback_popworldevent") PopWorldEvent(SimGame *);int __pragma("callback_popinfoevent") PopInfoEvent(SimGame *);extern void f_00446781(int),PilotScreenUpdate(SimGame *);
void SimGame::Update() {
 if(IsActive()) {
  MenuHelpSetTopic(0);PilotManagerPoll(&g4b74f4);tick=TimeNow();action=0;
  if(multi) {
   flush_chat_log();gMenuNetwork.Poll();modeLogName.Poll();networkLog.Process();NetworkPoll();
   int time=SimHotReadTimer((char *)this);if(time<=15000 || (time<=30000 && !linked))SimResetLimits(this);
  } else if(extraSaved) {
   int time=SimHotReadTimer((char *)this);if(time<=15000 || (time<=30000 && !linked))SimResetLimits(this);
  }
  switch(phase) {
  case 0:SimChangePlayer(this,nextPlayer);SimBeginTurn(this);if(phase==0)phase=1;break;
  case 1:if((multi || extraSaved) && linked && retryTimeout>0 && TimerExpired(&gFogTimer))SimStartPhase(this);break;
  case 2:if(SimOnlyStationary(this))phase=3;break;
  case 3:
   SimClosePopups(this);SimChooseNextPlayer(this);
   if(multi && ModeCheck((const char *)this))modeLogName.EndTurn(nextPlayer);
   SimUpdateOwnerPayloads(this);phase=0;
   if(!nextPlayer){phase=5;Close(4);return;}
   if(extraSaved && finishHotseat && slots[nextPlayer-1].status==1) {
    f_00477295(DAT_00511e7c.GetString(410));SaveGame(0);SimChooseNextPlayer(this);
    f_00477295(BriefFormat(DAT_00511e7c.GetString(411),GetModePlayer(this,nextPlayer)->rest));Close(0);return;
   }
   break;
  case 4:if(tick>deadline)phase=5;break;
  case 5:unknown28=1;break;
  }
  gInDirty=1;InputPoll(&input);
  if(multi || extraSaved) {SimPollRepeat((RepeatInput *)this);if(phase!=1 || SimHotReadTimer((char *)this)<1000)InputReset(&input);}
  SimAmbientSound(this);
  if(!linked) {
   targetLimits=linked;TargetHandleEvent(this);SimWorldStep(this);ReportGlobalInput(this);PopWorldEvent(this);PopInfoEvent(this);
  } else {
   if(worldOpen) {
    SimAlertEvent(this);TargetHandleEvent(this);SimWorldStep(this);PopWorldEvent(this);PopInfoEvent(this);MovementCommand(this);WeaponCommand(this);SimInventoryEvent(this);PopSystemCallback(this);return;
   }
   if(selectedMode==5)f_00446781((int)this);
   if(selectedMode==6)PilotScreenUpdate(this);
  }
 }
}
void WorldDrawTerrain(SimGame *s) {
 if(gGuiPaletteFaded) {GuiFadeIn();gGuiPaletteFaded=0;return;}
 if(!s->worldOpen) SurfaceFlush(&g513888);
 else if(!s->extraSaved || g4d35f4.Mode()!=5) {
  if(SimClipIntersects(&gMenuClip,&simSmallClip))SimClipFlush(&simSmallClip);
  if(SimClipIntersects(&gMenuClip,&g4d3c9c))SimClipFlush(&g4d3c9c);
  if(SimClipIntersects(&gMenuClip,&simClip))SimClipFlush(&simClip);
  if(SimClipIntersects(&gMenuClip,&g4d3cec))SimClipFlush(&g4d3cec);
  SurfaceFlush(&g513888);
  SimClipRestore(&simSmallClip);SimClipRestore(&g4d3c9c);SimClipRestore(&simClip);SimClipRestore(&g4d3cec);
  simMainSaved=simMainClip;simDetailSaved=simDetailClip;
  SimClipFlush(&simClip);SimClipFlush(&simMainClip);SimClipFlush(&simDetailClip);SimClipFlush(&g4d3cec);SimClipFlush(&g4d3c9c);
 }
}
struct PopNode; struct PopInfo; struct PopWorld; struct SimAlert;

extern PopNode g4d1dec;
extern PopInfo popInfo;
extern PopWorld popWorld;
extern SimAlert g4d4094;
extern int targetPrompt5;
extern void SimPanelDraw(void *),SimActionsDraw(void *),SimInventoryDraw(void *),SimPilotDraw(void *);
extern void PopInfoDraw(PopInfo *),PopWorldDraw(PopWorld *),PopNodeUpdate(PopNode *);
extern void WorldDrawBegin(void *),SimGameDrawMap(SimGame *),SimGameRefresh(SimGame *),SimGameDrawPortrait(SimGame *),SimAlertDraw(SimAlert *),TargetUpdate(void *);
void SimGame::Event(unsigned char) {

 if(IsActive()) {
  if(phase!=3) {
   pending=0;
   SimPanelDraw(&simPanel);PopInfoDraw(&popInfo);PopWorldDraw(&popWorld);SimActionsDraw(&simActions);
   SimInventoryDraw(&simInventory);PopNodeUpdate(&g4d1dec);SimPilotDraw(&simPilot);
   if(worldOpen) {
    WorldDrawBegin(this);SimGameDrawMap(this);
    if(linked && focus) {SimGameRefresh(this);SimGameDrawPortrait(this);SimAlertDraw(&g4d4094);}
    TargetUpdate(&targetPrompt5);
   }
   WorldDrawTerrain(this);screen=pending;
  }
  if(unknown28) Close(4);
 }
}

extern unsigned MissionRandomTable[];extern int SoundTrigger(unsigned,int);extern void FastBufferReset(FastBuffer *);extern void *gShellSurfacePixelsBefore;
void SimGame::Open() {
 if(!IsActive()) {
  int mode=gShellExitCode;
  active=1;memset(&unknown10,0,(char *)&persistentLast-(char *)&unknown10);palette=-1;
  f_0047e200((int *)&MissionRandomTable);f_0047e200((int *)&gFxSeed);
  ShellMultiplayerInit(&g4b74f4);SetGameWindowSize(-1,-1);
  SurfaceAttach(&g511948,gShellSurfacePixels,SurfaceWidth(),SurfaceHeight());SurfaceCreate(&gMenuSavedSurface,SurfaceWidth(),SurfaceHeight());gShellDrawPixels=savedSurfacePixels;
  SimGameEnd(0);SimSetupWorld(this);SimTargetInitialize();
  void *paletteData=gResMgr.Load("simgui.plx",1,gShellBackground,0x452);





































































































































































































































































































  if(!paletteData)AssertFail(simSourceFile,__LINE__,(const char *)paletteData);
  void *images=gResMgr.Load("simgui.bmx");drawCursorImages=(MenuImages *)images;gMenuImages=(MenuImages *)images;

  if(!images)AssertFail(simSourceFile,__LINE__,(const char *)images);
  ShellRestore(this);
  gShellMiniStrings.Unlock();gShellParaStrings.Unlock();SetPointer(&gShellMiniStrings,"miniSim.bin");SetPointer(&gShellParaStrings,"paraSim.bin");gShellMiniStrings.Unlock();gShellMiniStrings.Lock();BriefWaitB(28);targetBriefResource.Lock();
  GridInitialize(&fallbackName,1000,31);FastBufferCreate(&g4d35e4,fallbackName.Count());g4d35f4.Resize();SimBestLocOpen(bestLocEntries);simFogSource=&g4d35f4;SimFastSetFog(&gFastTransform,&g4d35f4);bestLocCursor=0;bestLocCount=0;
  view.point=GridPoint(0,0);view.zoom=9;view.direction=0;view.requested=-17;view.flags=view.requested;view.spare=100;
  for(int i=0;i<13;i++)bookmarks[i]=view;
  ActorListConstruct();WorldActionInitialize();ExploInit();SimAlertOpen(&g4d4094,&input);TargetMapAlert(&targetPrompt5,&input);
  selectedMode=0;savedMode=1;worldOpen=1;screen=-1;pending=0;unknown28=0;playerCount=0;
  if(mode==8)multi=1;
  if(mode!=7) {
   if(mode==10)SimHotStartMission(this);else SimStartMission(this);
   SimSetMode(this,next_link_seed(&gFxSeed)%6);palette=simInitialPalette;if(simInitialWarning<100)SoundTrigger(151,0);
  }
  for(ActorRecord *a=ActorFindFree();a;a=ActorFindNext(a))AnimActionReplace(a->actionData,a->id);
  GuiFilterPalette(gShellBackground,5);ResetMultiplayerInput();if(multi){create_chat_menu(&simChatMenu);g4faf44=1;}
  SimSetMenuState(this,1);FastBufferReset(&g4d35e4);SimSetMode(this,0);SimSetView(this,0);
  if(extraSaved)SimHotResizeFogBuffers();
  const unsigned long *mission=missionScripts[mainSelectedMission];simWorldBackground=mission[13];simWorldForeground=mission[7];
  if(extraSaved){simWorldBackground=9;simWorldForeground=0;}
 }
}
void SimGame::Close(int reason) {
 g4faf44=0;
 if(IsActive()) {
  SimGameLoadMissions(this);SigsStart(&g513888,0);SurfacePresent(&g513888);gShellDrawPixels=gShellSurfacePixelsBefore;SurfaceFree(&gMenuSavedSurface);
  targetBriefResource.Unlock();ReleasePoolData(drawCursorImages);drawCursorImages=0;LogicTextBegin(g4dd0c0);GridDestroy(&fallbackName);FastBufferDestroy(&g4d35e4);FogRelease(&g4d35f4);SimBestLocClose(bestLocEntries);
  HotbriefEmpty(g4dd0c0);ActorListDestroy();WorldActionShutdown();ExploShutdown();ReportAnimClose(&portrait);ReportAnimClose(&face);ReportAnimClose(&statusAnim);TerrainReleaseImage();AnimBitmapSweep(-1,-1,-1,-1,-1);
  SimSetUpdate(this,0);f_d2c0(&gShellResources);ShellMultiplayerShutdown(&g4b74f4);SimClosePopups(this);SimAlertClose(&g4d4094);TargetClearActive(&targetPrompt5);
  if(extraSaved){SimHotReleaseFogBuffers();if(reason>=1 && reason<=5)reason=5;}
  SetGameWindowSize(640,480);active=0;gShellExitCode=reason;ResetMultiplayerInput();destroy_chat_menu(&simChatMenu);
 }
}
inline ViewState::ViewState() {}






