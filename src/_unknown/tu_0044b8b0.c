// Unknown original world/input TU, 0044b8b0..0044d150. Working names unless bound.
// Gaps are temporary unnamed layout fields, not reconstructed payloads.
#pragma pack(push,1)
typedef unsigned long U32;
struct GridPoint { long x,y;
 int operator!=(const GridPoint &p) const { return x!=p.x || y!=p.y; }
};
struct ViewState { GridPoint point; long zoom,scale,direction; U32 flags,requested; long spare; };
struct UnitState;
struct ActorRecord {
 int id; int unknown04; int owner; int typeId; int unknown10,target;
 UnitState *unit; void *pilot; unsigned char unknown20[5]; unsigned char type;
 unsigned char unknown26[6]; unsigned short model;
 unsigned char unknown2e[0x87-0x2e]; unsigned char done,active,disabled;
 unsigned char unknown8a[3]; unsigned char spent;
 int Ready() const {return active && !disabled;}
};
struct PlayerSlot { int status,id,points,actor; unsigned char rest[0xbc-16]; };
struct SimGame {
 unsigned char unknown00[0x2c]; int screen; unsigned char unknown30[8]; ActorRecord *focus;
 void *unknown3c; UnitState *focusUnit; unsigned char unknown44[0x2ba-0x44];
 int phase; unsigned char unknown2be[4]; int multi; unsigned char unknown2c6[4]; int tick,deadline;
 int action; int palette,playerCount; PlayerSlot slots[9];
 PlayerSlot *slot; int mode,unknown982,player,localPlayer,unknown98e,linked;
 unsigned char unknown996[0xdf5-0x996]; ViewState bookmarks[13],view,savedView;
 unsigned char unknownfd5[0x1025-0xfd5]; int preview; long healthTrend; long energyTrend; int actionCost; unsigned char unknown1035[0x10f1-0x1035]; int extraSaved;
};
#pragma pack(pop)
struct GameSettings {unsigned char bytes[211];};
extern GameSettings gGameSettings,simSettings;
struct DifficultyLimit { int threshold; signed char first,second,third,fourth; };
extern DifficultyLimit difficultyLimits[];
extern int retryTimeout,simLimit1,simLimit2,simLimit3,simLimit4;
struct FastCoord;
extern FastCoord gFastTransform;
extern ViewState simTransform;
extern void SetFastView(FastCoord *,const ViewState *);
extern int gIn80,gInA0;extern long gInA4;
inline int SimShiftDown(){return gInA4;} inline int SimAltDown(){return gInA0;}
extern signed char simShortcutModes[];
void f_44bba0(SimGame *s) {
 simSettings=gGameSettings;
 if(s->multi) { int limit=retryTimeout; if(limit<=0)return;
  DifficultyLimit *p=difficultyLimits;
  for(;p->threshold;++p) {
   if(limit<p->threshold) {
    if(p->first>simLimit1)simLimit1=p->first;
    if(p->second>simLimit2)simLimit2=p->second;
    if(p->third<simLimit3)simLimit3=p->third;
    if(p->fourth<simLimit4)simLimit4=p->fourth;
    return;
   }

  }
 }
}

void SimResetLimits(SimGame *unused) {simLimit1=4;simLimit2=4;simLimit3=0;simLimit4=0;}
void SimSetTransformX(int x) {simTransform.point.x=x; SetFastView(&gFastTransform,&simTransform);}
int SimShortcutMode() {
 int n=gIn80-59; if(n<0 || n>11)return 0;
 if(SimShiftDown() || SimAltDown())return 0;
 return simShortcutModes[n];
}
struct ViewRect {long left,top,right,bottom;};
extern ViewRect gViewRect;
extern GridPoint g4bb78c;
extern char gView[];
extern int useWindowScroll,windowScrollActive,simScrollDeadline;
extern long GuiPickTarget(void *,const GridPoint *);
extern int GetWindowOffset(int *,int *);
extern void SimHoverActor(SimGame *,int,int);
extern void SimHoverObject(SimGame *,int);
extern void SimPan(SimGame *,int,int);
void SimHandleCursor(SimGame *s) {
 int dx,dy;
 if(g4bb78c.x>=gViewRect.left && g4bb78c.x<=gViewRect.right && g4bb78c.y>=gViewRect.top && g4bb78c.y<=gViewRect.bottom) {
  int n=GuiPickTarget(gView,&g4bb78c);
  if(s->linked) SimHoverActor(s,n,0); else SimHoverObject(s,n);
 }
 if(useWindowScroll) {
  if(GetWindowOffset(&dx,&dy)) {if(windowScrollActive && simScrollDeadline<s->tick){SimPan(s,dx,dy);simScrollDeadline=s->tick+100;}}else simScrollDeadline=s->tick+500;
 }
}
extern ActorRecord *ActorFindFree(),*ActorFindNext(ActorRecord *);
extern void SimHandleInput(SimGame *);
extern int SimPreviewCost(SimGame *,int,int);
extern void SimActorStep(ActorRecord *);
extern char g4dd0c0[];
extern void LogicDispatchIntel(unsigned char *);
extern int SimHotChooseHotseatOwner(void *);
extern void HotbriefMainLoop(void *,int),SimDelayPhase(SimGame *);
struct FogBuffer {void Initialize(int,int);}; extern FogBuffer g4d35f4;
void SimWorldStep(SimGame *s) {
 if(s->mode!=s->player) {SimHandleInput(s);SimHandleCursor(s);}
 else if(s->focus) {
  SimHandleInput(s);SimHandleCursor(s);
  int only,cost;
  if(s->action<1 || s->action!=s->focus->target) {only=0;cost=0;}
  else {only=1;cost=s->actionCost;}
  SimPreviewCost(s,cost,only);
 }
 s->unknown98e=s->linked?simLimit1:simLimit2;
 for(ActorRecord *a=ActorFindFree();a;a=ActorFindNext(a)) SimActorStep(a);
 LogicDispatchIntel((unsigned char *)g4dd0c0);
 if(s->extraSaved) {int owner=SimHotChooseHotseatOwner(s);if(owner){HotbriefMainLoop(s,owner);SimDelayPhase(s);s->deadline=s->tick;g4d35f4.Initialize(5,1);}}
}
