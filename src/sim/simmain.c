// sim/simmain.c, 0044bcd0..0044d150; split from world globals by BSS ordering evidence.
// Gaps are temporary unnamed layout fields, not reconstructed payloads.
#pragma pack(push,1)
typedef unsigned long U32; static char simmainSourceFile[]=__FILE__;struct SoundSettings;
#include "actor.h"



struct ViewState {long flags;GridPoint point;long scale,direction;ViewState(long f,GridPoint p,long s,long d){flags=f;point.x=p.x;point.y=p.y;scale=s;direction=d;}};
struct CameraState {GridPoint point;long zoom,scale,direction;U32 flags,requested;long spare;};
struct ActorItem {unsigned char head[0x1b];int maximum;};









struct PlayerSlot { int status,id,points,actor; unsigned char rest[0xbc-16]; };
struct InputReq {U32 mask,flags;int Pressed(U32 n)const{return (flags&(n|(n<<2)))==n;}int Down(U32 n)const{return (flags&n)!=0;}};
struct SimGame {
 unsigned char unknown00[8];InputReq input;unsigned char unknown10[8];int selectedMode;unsigned char unknown1c[16];int screen; unsigned char unknown30[8]; ActorRecord *focus;
 void *unknown3c; ActorUnit *focusUnit; unsigned char unknown44[0x2ba-0x44];
 int phase; unsigned char unknown2be[4]; int multi; unsigned char unknown2c6[4]; int tick,deadline;
 int action; int palette,playerCount; PlayerSlot slots[9];
 PlayerSlot *slot; int mode,unknown982,player,localPlayer,unknown98e,linked;
 unsigned char unknown996[0xdf5-0x996]; CameraState bookmarks[13],view,savedView;
 unsigned char unknownfd5[0x1025-0xfd5]; int preview; long healthTrend; long energyTrend; int actionCost; unsigned char unknown1035[0x10d1-0x1035];int lastTarget;unsigned char unknown10d5[0x10f1-0x10d5]; int extraSaved;
};
#pragma pack(pop)
struct GameSettings {unsigned char bytes[211];};
extern GameSettings gGameSettings,simSettings;
struct DifficultyLimit { int threshold; signed char first,second,third,fourth; };
extern DifficultyLimit difficultyLimits[];
extern int retryTimeout,simLimit1,simLimit2,simLimit3,simLimit4;
struct FastCoord;
struct TerrainGrid;struct FogSource;struct FogBuffer;struct FastBuffer;
extern TerrainGrid *GridClear(TerrainGrid *);extern void GridDestroy(TerrainGrid *);extern FastBuffer *FastBufferClear(FastBuffer *);extern void FastBufferDestroy(FastBuffer *);extern FogBuffer *FogSetSource(FogBuffer *,FogSource *);extern void FogRelease(FogBuffer *);
struct TerrainGrid {unsigned char bytes[80];TerrainGrid(){GridClear(this);}~TerrainGrid(){GridDestroy(this);}};
struct FastBuffer {long count;void *items;long current,first;FastBuffer(){FastBufferClear(this);}~FastBuffer(){FastBufferDestroy(this);}};
struct FogBuffer {long mode;void *source;long count;char *lower,*upper;long state;FogBuffer(FogSource *p){FogSetSource(this,p);}void Initialize(int,int);};
struct WorldView {unsigned char bytes[140];WorldView(TerrainGrid *,unsigned);~WorldView();void SetClip(const void *);};
struct FastCoord {unsigned char bytes[88];FastCoord(TerrainGrid *,unsigned);~FastCoord();};
struct BestLocState {unsigned char bytes[280];BestLocState(TerrainGrid *,int);~BestLocState();};


extern void SetFastView(FastCoord *,const ViewState *);
extern int gIn80,gInA0;extern long gInA4;
inline int SimShiftDown(){return gInA4;} inline int SimAltDown(){return gInA0;}
extern signed char simShortcutModes[];
struct GuiPoint {long x,y;};
struct GuiControlData:GuiRect {unsigned char style[4];U32 attributes,shortcut;};
class GuiControl:public GuiControlData {public:virtual ~GuiControl();virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);virtual int HitTest(const GuiPoint *);};
class MenuHelpRegion:public GuiControl {public:long id;int activated;MenuHelpRegion();virtual ~MenuHelpRegion();virtual void Draw(unsigned);virtual int Event(unsigned,U32);};
extern TerrainGrid fallbackName;
extern FastBuffer g4d35e4;
extern FogBuffer g4d35f4;
extern WorldView gView;
extern FastCoord gFastTransform;
extern ViewState simTransform;
extern BestLocState bestLocEntries;
struct ViewRect {long left,top,right,bottom;};
extern ViewRect g4d381c,gViewRect,simPortraitRect,simStatusRect,g4d385c;
extern MenuHelpRegion simHelp;
extern unsigned char worldColorsBackup[1024];
extern int simWorldBackground,simWorldForeground;
struct ClipState {long words[10];};
extern ClipState g4d3c9c,simClip,g4d3cec,simMainClip,simMainSaved,simSmallClip,simDetailClip,simDetailSaved,simScratchClip;
struct SimRandomSeed {unsigned value;SimRandomSeed(){value=0x12345678;}};
extern SimRandomSeed gFxSeed,simItemSeed;
extern int simScrollDeadline;
extern char g513888;extern int g5138ac,g5138b0,screenHeight;
extern GuiRect gMenuClip;
extern void SigsStart(void *,int),SurfacePresent(void *),GuiResetClip(void *),SimClipInitialize(ClipState *,const ViewRect *);
void f_44bba0(SimGame *);
extern int SimShortcutMode();extern void SimResetLimits(SimGame *);extern unsigned char *simActorVisibility;extern int SoundPlay(unsigned,const SoundSettings *);
extern void SimPan(SimGame *,int,int);extern void SimCenterOn(SimGame *,GridPoint *);
extern ViewRect gViewRect;
extern GridPoint g4bb78c;

extern int useWindowScroll,windowScrollActive,simScrollDeadline;
extern long GuiPickTarget(void *,const GridPoint *);
extern int GetWindowOffset(int *,int *);
extern void SimHoverActor(SimGame *,int,int);
extern void SimHoverObject(SimGame *,int);
extern void SimPan(SimGame *,int,int);
extern unsigned char *simActorVisibility;
extern void SimCenterOn(SimGame *,GridPoint *);extern int SoundPlay(unsigned,const SoundSettings *);
extern U32 gIn7c;
extern char g4dd0c0[],profileManager[],chatPlayerName[],chatAddress[],chatMessage[],chatTitle[];
extern int g4faf40,g4faf44,gApplyFreeShots;
#pragma aux post_start_callback "W?PostStart$:MenuNetwork$n()v";
struct MenuNetwork {static void __pragma("post_start_callback") PostStart();};
extern void MenuTrackContext();
extern void DialogOpen(char *,char *,char *,int,int,char *,void (*)(),int *,int *),SimToggleHelp();
extern void LogicTextPrevious(void *),LogicTextNext(void *),LogicTextClear(void *),SimSaveBookmark(SimGame *,int),SimRestoreBookmark(SimGame *,int);
extern void SimSetMode(SimGame *,int),SimSetView(SimGame *,int),f_44db70(SimGame *,int),SimStartPhase(SimGame *),SimFinishFocus(SimGame *);
extern void PilotSetState(SimGame *,int),SimSetFocus(SimGame *,ActorRecord *),SimRefreshFog(SimGame *,int);
extern int SimCycleFocus(SimGame *,int);
extern void PilotNext(SimGame *,ActorRecord *,int,int),PilotPrevious(SimGame *,ActorRecord *,int,int),ExploIdle();
extern void SimChatCommand(void *,U32),SimMenuCommand(U32),LogicCheckActorOrder(void *,int),SimHotbriefRemove(SimGame *,int,int);
extern char simChatMenu;
extern ActorRecord *ActorGetByIndex(int),*ActorFindByOwner(int),*ActorFindOwnerNext(ActorRecord *,int);
extern void SimSetUpdate(SimGame *,int);
extern void SendEvent(void *,int,void * =0,int=0,int=0),SimAddMission(void *),ReleaseState(void *);
extern "C" void f_0043e38f(void *),f_00439706(void *,int);
extern ActorItem *PopFindItem(void *,int,int);
extern int f_0043ac9c(ActorUnit *,int,int),TakeTerrainFeature(TerrainGrid *,int);
inline int SimActorHidden(int);
extern void LogicSetActorLink(void *,ActorRecord *,int,int);
inline void SimRecoverPilot(void *pilot){SendEvent(pilot,116);SendEvent(pilot,118);SendEvent(pilot,120);}
inline void SimRecoverActor(ActorRecord *a){SendEvent(a->pilot,116);SendEvent(a->pilot,118);SendEvent(a->pilot,120);f_00439706(a->unit,23);SimAddMission(a->unit);}
inline int SimInputKey(){return gIn80;}inline U32 SimInputCommand(){return gIn7c;}
void SimHandleInput(SimGame *s) {
 U32 key=SimInputKey(),command=SimInputCommand();
 if(key || command) {
 if(s->multi && !g4faf40 && (key==28 || command==23013))DialogOpen(chatPlayerName,chatAddress,chatMessage,150,100,chatTitle,&MenuNetwork::PostStart,&g4faf40,&g4faf44);
 switch(key) {
 case 79:SimPan(s,-1,-1);break;
 case 81:SimPan(s,1,-1);break;
 case 75:SimPan(s,-1,0);break;
 case 71:SimPan(s,-1,1);break;
 case 73:SimPan(s,1,1);break;
 case 76:if(s->focus && !SimActorHidden(s->focus->action.resource)){GridPoint point=s->focus->action.position;SimCenterOn(s,&point);}break;
 case 82:SimSetMode(s,1);break;
 case 83:SimSetMode(s,-1);break;
 case 78:SimSetView(s,1);break;
 case 74:SimSetView(s,-1);break;
 case 50:MenuTrackContext();break;
 case 10:s->view.spare=0;f_44db70(s,0);s->screen|=1;break;
 case 11:s->view.spare=100;f_44db70(s,0);s->screen|=1;break;
 case 44:LogicTextPrevious(g4dd0c0);break;
 case 45:LogicTextNext(g4dd0c0);break;
 case 14:LogicTextClear(g4dd0c0);break;
 case 31:SimResetLimits(s);break;
 case 80:SimPan(s,0,-1);break;
 case 77:SimPan(s,1,0);break;
 case 72:SimPan(s,0,1);break;
 }
 if((int)key>=59 && (int)key<=66) {
  if(gInA4){SimSaveBookmark(s,key-59);key=0;}
  else if(gInA0){SimRestoreBookmark(s,key-59);key=0;}
 }
 if((int)command>=23000 && (int)command<=23013 && s->multi)SimChatCommand(&simChatMenu,command);
 switch(command) {
 case 30120:simTransform.flags=0;SetFastView(&gFastTransform,&simTransform);s->screen|=2;break;
 case 30121:simTransform.flags=2;SetFastView(&gFastTransform,&simTransform);s->screen|=2;break;
 case 30122:simTransform.flags=1;SetFastView(&gFastTransform,&simTransform);s->screen|=2;break;
 case 30123:simTransform.flags=3;SetFastView(&gFastTransform,&simTransform);s->screen|=2;break;
 case 30124:simTransform.flags=4;SetFastView(&gFastTransform,&simTransform);s->screen|=2;break;
 case 30134:if(s->extraSaved)SimHotbriefRemove(s,1,s->mode);else LogicCheckActorOrder(g4dd0c0,0);break;
 }
 if(s->linked && !gInA0) {
  int shift=gInA4,index=gIn80-59,mode=0;
  if(index>=0 && index<=11 && !gInA4)mode=simShortcutModes[index];
  if(mode)PilotSetState(s,mode);
  switch(key) {
  case 26:PilotNext(s,s->focus,shift,1);break;
  case 27:PilotPrevious(s,s->focus,shift,1);break;
  case 32:SimFinishFocus(s);break;
  case 51:SimCycleFocus(s,-1);break;
  case 52:SimCycleFocus(s,1);break;
  case 59:ExploIdle();break;
  }
 }
 if(s->linked) {
  if((int)command>=31980 && (int)command<=32080){if(s->multi)command=0;SimMenuCommand(command);}
  if((int)command>=20000 && (int)command<=20199) {ActorRecord *actor=ActorGetByIndex(command-20000);if(actor){SimSetFocus(s,actor);SoundPlay(4,0);}}
  switch(command) {
  case 30100:PilotSetState(s,1);return;
  case 30102:PilotSetState(s,2);return;
  case 30103:PilotSetState(s,3);return;
  case 30104:PilotSetState(s,4);return;
  case 30106:PilotSetState(s,6);return;
  case 30107:PilotSetState(s,5);return;
  case 30109:PilotSetState(s,9);return;
  case 30130:SimCycleFocus(s,1);return;
  case 30131:SimCycleFocus(s,-1);return;
  case 30132:SimFinishFocus(s);return;
  case 30133:SimStartPhase(s);return;
  case 32032:s->screen=-1;f_0043e38f(profileManager);SimSetUpdate(s,1);return;
  case 32031:s->screen=-1;SendEvent(s->focus->pilot,116);SendEvent(s->focus->pilot,118);SendEvent(s->focus->pilot,120);return;
  case 32033:s->screen=-1;f_00439706(s->focus->unit,23);return;
  case 32034: {
   s->screen=-1;ActorUnit *u=s->focus->unit;u->slots[5].amount=PopFindItem(u,5,-1)->maximum;u->slots[6].amount=PopFindItem(u,6,-1)->maximum;return;
  }
  case 32035:s->screen=-1;SimAddMission(s->focus->unit);return;
  case 32036:s->screen=-1;SendEvent(s->focus->pilot,116);SendEvent(s->focus->pilot,118);SendEvent(s->focus->pilot,120);f_00439706(s->focus->unit,23);SimAddMission(s->focus->unit);return;
  case 32038: {
   s->screen=-1;for(ActorRecord *a=ActorFindByOwner(s->mode);a;a=ActorFindOwnerNext(a,s->mode))SimRecoverActor(a);return;
  }
  case 32039: {
   s->screen=-1;for(ActorRecord *a=ActorFindByOwner(s->mode);a;a=ActorFindOwnerNext(a,s->mode)){a->unit->slots[5].amount=1000000;a->unit->slots[6].amount=1000000;}return;
  }
  case 32040:gApplyFreeShots=1;return;
  case 32041:gApplyFreeShots=10000;return;
  case 32043: {
   s->screen=-1;for(ActorRecord *a=ActorFindByOwner(s->localPlayer);a;a=ActorFindOwnerNext(a,s->localPlayer))ReleaseState(a);return;
  }
  case 32044: {
   int index=0,total=0;ActorRecord *a=ActorFindByOwner(s->mode);
   while(a && !(index=f_0043ac9c(a->unit,1,0)))a=ActorFindOwnerNext(a,s->mode);
   if(a){for(int i=0;i<4000;i++)total+=TakeTerrainFeature(&fallbackName,i);a->unit->slots[index].amount=total;}
   s->screen=-1;return;
  }
  case 32046:case 32047:case 32048:case 32049:case 32050:
   s->screen=-1;g4d35f4.Initialize(4-(command-32046),0);SimRefreshFog(s,0);return;
  case 32056: {
   s->screen=-1;for(ActorRecord *a=ActorFindByOwner(s->mode);a;a=ActorFindOwnerNext(a,s->mode))ReleaseState(a);return;
  }
  case 32060:LogicSetActorLink(g4dd0c0,s->focus,0,0);return;
  case 32061:LogicSetActorLink(g4dd0c0,s->focus,2,0);return;
  case 32062:LogicSetActorLink(g4dd0c0,s->focus,6,0);return;
  }
 }
}
}
void SimHandleCursor(SimGame *s) {
 int dx,dy;
 if(g4bb78c.x>=gViewRect.left && g4bb78c.x<=gViewRect.right && g4bb78c.y>=gViewRect.top && g4bb78c.y<=gViewRect.bottom) {
  int n=GuiPickTarget(&gView,&g4bb78c);
  if(s->linked) SimHoverActor(s,n,0); else SimHoverObject(s,n);
 }
 if(useWindowScroll) {
  if(GetWindowOffset(&dx,&dy)) {if(windowScrollActive && s->tick>simScrollDeadline){SimPan(s,dx,dy);simScrollDeadline=s->tick+100;}}else simScrollDeadline=s->tick+500;
 }
}
struct GameTimer;extern void GameTimerSet(GameTimer *,U32);extern int GameTimerExpired(GameTimer *);
struct GameTimer {U32 start,end,duration;GameTimer(U32 n){GameTimerSet(this,n);}};
struct TerrainGrid;extern TerrainGrid fallbackName;
extern unsigned char *simActorVisibility;
class ReportScreen {public:void SetOther(ActorRecord *,int=-1,int=-1);};extern ReportScreen simPilot;
struct PopInfo;extern PopInfo popInfo;extern void PopInfoSelect(PopInfo *,int);
#pragma aux set_camera_view "W?SetTerrainView$n(pnvpnx$ViewState$$i)v";
void __pragma("set_camera_view") SetTerrainView(void *,const CameraState *,int);
extern "C" void TargetObjectInit(void *,GridPoint *,int);
extern ActorRecord *ActorGetByUnit(int);
extern int SimHotbriefCheckActor(ActorRecord *),SoundPlay(unsigned,const SoundSettings *),GridClearSelection(TerrainGrid *);
extern char g4dd0c0[];inline int SimActorHidden(int n){return !simActorVisibility[n];}
extern void SimCenterOn(SimGame *,GridPoint *);extern void SimSetFocus(SimGame *,ActorRecord *),SimTargetAction(SimGame *,int),SimPrepareAction(SimGame *,int),SimSelectTarget(SimGame *,int),PilotSetState(SimGame *,int),LogicTextMark(void *);
extern int SimRandomFire(SimGame *,int,int);
extern void SimSetCursor(int,int),ReportSelect(ReportScreen *,ActorRecord *),ReportMission(ReportScreen *,int);
extern int targetLimits;
void SimHoverActor(SimGame *s,int unit,int cursor) {
 static GameTimer timer(0);
 GridPoint point,dragPoint;
 ActorRecord *actor=0;
 int left=(s->input.Pressed(16)),right=(s->input.Pressed(32));
 int shift=SimShiftDown(),alt=SimAltDown() && !SimShiftDown();
 if(cursor==11) {
  if(!s->linked){alt=0;shift=0;}
  if(!shift && !alt) {
   SimSetCursor(cursor,0);targetLimits=cursor+15;
   if(!s->input.Down(16))return;
   if((s->input.Pressed(16)) || (s->input.Pressed(32)))SoundPlay(9,0);
   TargetObjectInit(&fallbackName,&point,unit);GridPoint center=point;SimCenterOn(s,&center);return;
  }
 }
 if(unit) {
  if(!s->focus->action.IsMoving()){cursor=5;if(unit==s->focus->target)cursor=6;}
  s->action=unit;actor=ActorGetByUnit(unit);
  if(s->linked && actor && SimActorHidden(actor->action.resource))actor=0;
  if(shift)actor=0;
  if(actor && actor->owner==s->player && !SimHotbriefCheckActor(actor))actor=0;
  if(actor) {
   if(actor->id==s->focus->id)cursor=1;
   else if(actor->owner==s->mode)cursor=4;
   else if(s->lastTarget!=actor->id || s->selectedMode!=2)cursor=7;
   else cursor=9;
  }
  if(alt && actor!=s->focus)cursor=7;
 } else cursor=3;
 if(cursor){SimSetCursor(cursor,0);targetLimits=cursor+15;}
 if(right) {
  GameTimerSet(&timer,750);if(GridClearSelection(&fallbackName))s->screen|=1;
  if(unit){SoundPlay(33,0);PopInfoSelect(&popInfo,unit);PilotSetState(s,8);}
 } else if(left) {
  LogicTextMark(g4dd0c0);
  switch(cursor) {
  case 4:SimSetFocus(s,actor);SoundPlay(4,0);break;
  case 7:SimSelectTarget(s,unit);break;
  case 9:SimRandomFire(s,-1,1);break;
  case 5:SimTargetAction(s,unit);if(!shift)break;
  case 6:SimPrepareAction(s,unit);break;
  }
 }
 if(s->input.Down(32) && GameTimerExpired(&timer)) {
  GameTimerSet(&timer,250);PopInfoSelect(&popInfo,unit);
  if(unit){TargetObjectInit(&fallbackName,&dragPoint,unit);CameraState *v=&s->view;v->point=dragPoint;SetTerrainView(&gView,v,0);s->screen|=1;}
 }
}
void SimHoverObject(SimGame *s,int unit) {
 static GameTimer timer(0);
 ActorRecord *actor;
 int left=(s->input.Pressed(16)),right=(s->input.Pressed(32));
 actor=0;
 unsigned cursor;
 if(unit) {
  cursor=5;s->action=unit;actor=ActorGetByUnit(unit);ActorRecord *test=actor;
  if(actor && SimHotbriefCheckActor(actor) && !SimActorHidden(test->action.resource))cursor=4;
 } else cursor=3;
 if(cursor){SimSetCursor(cursor,0);targetLimits=cursor+15;}
 if(right) {
  GameTimerSet(&timer,750);if(GridClearSelection(&fallbackName))s->screen|=1;
  if(unit){SoundPlay(33,0);PopInfoSelect(&popInfo,unit);PilotSetState(s,8);}
 } else if(left) {
  LogicTextMark(g4dd0c0);
  switch(cursor) {
  case 4:PilotSetState(s,7);ReportSelect(&simPilot,actor);simPilot.SetOther(0);SoundPlay(6,0);break;
  case 5:ReportMission(&simPilot,unit);break;
  }
 }
 if(s->input.Down(32) && GameTimerExpired(&timer)) {
  GameTimerSet(&timer,250);PopInfoSelect(&popInfo,unit);
  if(unit){GridPoint point;TargetObjectInit(&fallbackName,&point,unit);CameraState *v=&s->view;v->point=point;SetTerrainView(&gView,v,0);s->screen|=1;}
 }
}
extern ActorRecord *ActorFindFree(),*ActorFindNext(ActorRecord *);
extern void SimHandleInput(SimGame *);
extern int SimPreviewCost(SimGame *,int,int);
extern void ActorAdvanceState(ActorRecord *);
extern char g4dd0c0[];
extern void LogicDispatchIntel(unsigned char *);
extern int SimHotChooseHotseatOwner(void *);
extern void HotbriefMainLoop(void *,int),SimDelayPhase(SimGame *);
extern FogBuffer g4d35f4;
void SimWorldStep(SimGame *s) {
 if(s->mode!=s->player) {SimHandleInput(s);SimHandleCursor(s);}
 else if(s->focus) {
  SimHandleInput(s);SimHandleCursor(s);
if(s->action>0 && s->action==s->focus->target)SimPreviewCost(s,s->actionCost,1);else SimPreviewCost(s,0,0);
 }
 s->unknown98e=s->linked?simLimit1:simLimit2;
 for(ActorRecord *a=ActorFindFree();a;a=ActorFindNext(a)) ActorAdvanceState(a);
 LogicDispatchIntel((unsigned char *)g4dd0c0);
 if(s->extraSaved) {int owner=SimHotChooseHotseatOwner(s);if(owner){HotbriefMainLoop(s,owner);SimDelayPhase(s);s->deadline=s->tick;g4d35f4.Initialize(5,1);}}
}



