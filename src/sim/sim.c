// sim\sim.c, partial reconstruction. Labels are guesses unless already bound.
#pragma pack(push,1)
typedef unsigned long U32;
struct GuiRect { long x,y,width,height; };
struct ClipState { long words[10]; };
struct GameTimer; extern void GameTimerSet(GameTimer *,U32); extern int GameTimerExpired(GameTimer *);
struct GameTimer {U32 start,end,duration;GameTimer(U32 n){GameTimerSet(this,n);}};
class ShellBase { public: long active; long IsActive()const{return active;} virtual ~ShellBase(); virtual void Open();virtual void Close(int);virtual void Update();virtual void Event(unsigned char);};
struct ActorRecord;
class SimGame:public ShellBase {
public:
 unsigned char unknown08[0x24-8]; int worldOpen,unknown28,screen,pending;
 int focusIndex; ActorRecord *focus;
 unsigned char unknown3c[0x2ba-0x3c]; int phase;
 unsigned char unknown2be[0x992-0x2be]; int linked;
 unsigned char unknown996[0x10f1-0x996]; int extraSaved;
};
#pragma pack(pop)
extern int gameSessionMode,ambientSetting;
extern char gShellResources;
extern unsigned gFxSeed;
extern "C" int f_d270(void *);
extern "C" void f_d2f0(void *,int);
extern "C" unsigned next_link_seed(unsigned *);
void SimAmbientSound(SimGame *s) {
 if(gameSessionMode!=1 && s->worldOpen && ambientSetting>=50) {
  static GameTimer timer(0);
  if(GameTimerExpired(&timer)) {GameTimerSet(&timer,15000);if(f_d270(&gShellResources))f_d2f0(&gShellResources,next_link_seed(&gFxSeed)%3+2);}
 }
}
extern int gGuiPaletteFaded;
struct FogBuffer {int mode; int Mode()const{return mode;}}; extern FogBuffer g4d35f4;
extern char g513888;
extern GuiRect gMenuClip,g4d3c9c,simClip,g4d3cec,simSmallClip;
extern ClipState simMainClip,simMainSaved,simDetailClip,simDetailSaved;
extern void GuiFadeIn(),SurfaceFlush(void *);
extern int SimClipIntersects(const GuiRect *,const void *);
extern void SimClipFlush(void *),SimClipRestore(void *);
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
extern char simPanel,simActions,simInventory,simPilot;
extern PopNode g4d1dec;
extern PopInfo popInfo;
extern PopWorld popWorld;
extern SimAlert g4d4094;
extern int targetPrompt5;
extern void SimPanelDraw(void *),SimActionsDraw(void *),SimInventoryDraw(void *),SimPilotDraw(void *);
extern void PopInfoDraw(PopInfo *),PopWorldDraw(PopWorld *),PopNodeUpdate(PopNode *);
extern void WorldDrawBegin(void *),SimGameDrawMap(SimGame *),SimGameRefresh(SimGame *),SimGameDrawPortrait(SimGame *),SimAlertDraw(SimAlert *),TargetUpdate(void *);
void SimDraw(SimGame *s) {
 if(s->IsActive()) {
  if(s->phase!=3) {
   s->pending=0;
   SimPanelDraw(&simPanel);PopInfoDraw(&popInfo);PopWorldDraw(&popWorld);SimActionsDraw(&simActions);
   SimInventoryDraw(&simInventory);PopNodeUpdate(&g4d1dec);SimPilotDraw(&simPilot);
   if(s->worldOpen) {
    WorldDrawBegin(s);SimGameDrawMap(s);
    if(s->linked && s->focus) {SimGameRefresh(s);SimGameDrawPortrait(s);SimAlertDraw(&g4d4094);}
    TargetUpdate(&targetPrompt5);
   }
   WorldDrawTerrain(s);s->screen=s->pending;
  }
  if(s->unknown28) s->Close(4);
 }
}
