// sim\simdo.c: camera and actor actions. Working names unless bound.
// Gaps are temporary unnamed layout fields, not reconstructed payloads.
#pragma pack(push,1)
typedef unsigned long U32;
struct GridPoint { long x,y;
 int operator!=(const GridPoint &p) const { return x!=p.x || y!=p.y; }
};
struct ViewState { GridPoint point; long zoom,scale,direction; U32 flags,requested; long spare; ViewState(const ViewState &v){point.x=v.point.x;point.y=v.point.y;zoom=v.zoom;scale=v.scale;direction=v.direction;flags=v.flags;requested=v.requested;spare=v.spare;} };
struct ItemSlot {unsigned char head[3],cooldown;long count;};struct ActorItem;struct UnitState {unsigned char bytes[0xa9];ItemSlot items[4];unsigned char tail[0x10b-0xc9];ActorItem *Item(int,int=-1);};struct UnitModel {unsigned char bytes[0x480];int cost,kind;};
struct SimStat {long value,maximum,spare;};
struct SimAnim {unsigned char bytes[6];unsigned char mode,frame;};
struct ActorTraits {unsigned char unknown[5],type;int Mobile()const{return type!=1 && type!=7 && type!=14 && type!=17 && type!=18;}};
struct ActorRecord {
 int id; int unknown04; int owner; int typeId; int unknown10,target;
 UnitState *unit; void *pilot; ActorTraits action;
 unsigned char unknown26[6]; unsigned short model;
 unsigned char unknown2e[4];GridPoint position;unsigned char unknown3a[8];int index;unsigned char unknown46[12];UnitModel *selectedUnit;unsigned char unknown56[0x87-0x56]; unsigned char done,active,disabled;
 unsigned char unknown8a[3]; unsigned char spent,returnFire;
 int Ready() const {return active && !disabled;} const char *UnitText()const{return (const char *)unit;} const char *PilotText()const{return (const char *)pilot;}
};
struct TargetSlot {int used;unsigned char rest[106];}; struct TargetSelection {unsigned char head[0xdf];TargetSlot slots[8];}; struct PlayerSlot { int status,id,points,actor; unsigned char rest[0xbc-16]; int NetworkOwner() const {return status==1 || status==3;} };
struct SimGame {
 unsigned char unknown00[0x14];int uiState,selectedMode;int previousMode,savedMode;unsigned char unknown24[8]; int screen,pending,focusIndex; ActorRecord *focus;
 UnitState *focusUnit; void *focusPilot;SimStat stats[3];SimAnim portrait,face,statusAnim;UnitState oldState,focusState;SimStat oldStats[3];
 int phase; unsigned char unknown2be[4]; int multi; int waiting; int tick,deadline;
 unsigned char unknown2d2[4]; int palette,playerCount; PlayerSlot slots[9];
 PlayerSlot *slot; int mode,unknown982,player,localPlayer,unknown98e,linked,remote,automated;
 unsigned char unknown99e[8]; TargetSelection targets; ViewState bookmarks[13],view,savedView;
 unsigned char unknownfd5[0x1025-0xfd5]; int preview; long healthTrend; long energyTrend; int actionCost; unsigned char targetRecord[0xb4]; unsigned char unknown10e9[8]; int extraSaved;
};
#pragma pack(pop)
struct TerrainGrid; struct FastCoord;
extern TerrainGrid fallbackName;
extern char gView[];
extern FastCoord gFastTransform;
extern ViewState simTransform;
extern long gInA4;extern int cameraSpeed,viewMinimum,viewMaximum,viewFog;
inline int SimShiftDown() { return gInA4; }
extern int g4d368c;
extern unsigned char viewScales[];
struct ViewMask { U32 mask; int threshold; };
extern ViewMask viewMasks[];
extern long GridPointToIndex(TerrainGrid *,GridPoint *);
extern void RotateViewPoint(GridPoint *,int);
extern void ExploCompact(int);
extern void SetTerrainView(void *,const ViewState *,int);
extern void SetFastView(FastCoord *,const ViewState *);
struct SoundSettings;extern int SoundPlay(unsigned,const SoundSettings *);
extern ActorRecord *ActorFindRelated(ActorRecord *),*ActorFindRelatedOther(ActorRecord *);
extern void SimSetFocus(SimGame *,ActorRecord *);
extern int StateInvoke(UnitState *,long *,long *),StateInvokeEnergy(UnitState *,long *,long *);
extern int UnitSpend(UnitState *,int);
extern int SoundTrigger(unsigned,int);
extern void SimResetLimits(SimGame *);
void SimPanBy(SimGame *,GridPoint *);
void f_44db70(SimGame *,int);

struct PacketList {void Focus(ActorRecord *);void Face(ActorRecord *,int,int);void Remove(ActorRecord *);void ReturnFire(ActorRecord *,char,char,char);void JackUp(ActorRecord *);void Placidex(ActorRecord *);void Toggle(ActorRecord *,char);};
extern PacketList modeLogName;
extern SimGame modeCheckName;
extern int ModeCheck(const char *);
extern "C" void AssertFail(const char *,int,const char *);
static const char SimDoSourceFile[]=__FILE__;
static int simLastActor=-1,simLastIndex=-1,simLastTarget=-1;
extern int AnimBitmapSweep(int,int,int,int,int);
extern int UnitPortrait(UnitState *);
extern void SimAnimSet(SimAnim *,int,int,int);
extern void SimRefreshPanel(SimGame *,int);
extern "C" void *memcpy(void *,const void *,unsigned);
extern "C" void *memset(void *,int,unsigned);
extern void SimClosePopups(SimGame *),PilotSetState(SimGame *,int);
extern void GridSetActor(TerrainGrid *,UnitState *,void *);
extern PlayerSlot *GetModePlayer(void *,int);
extern int bestLocCursor,bestLocCount;
extern unsigned char bestLocEntries[],*actorVisibility;
extern int BestLocSetup(void *,int,int);
extern void SimRefreshFog(SimGame *,int),SimTargetAction(SimGame *,int);
struct TargetRecord {unsigned char head[156];int targetActor;unsigned char rest[20];};
extern void TargetInitializeRecord(void *,TargetRecord *,int,int);
extern int GridClearSelection(TerrainGrid *);
extern long GridClamp(U32);
extern void ActorSetDirection(ActorRecord *,int),ActorClearSelection(ActorRecord *);
extern int SimFire(SimGame *,void *,long,long,long);
int SimPreviewCost(SimGame *,int,int);
struct FogBuffer;extern FogBuffer g4d35f4;
extern int AnimActionHasMetadata(void *),UnitCapability(void *,int);
extern int AnimActionPrepareBlock(void *,int,int);extern void GridSetSelection(TerrainGrid *,UnitModel *),AnimActionInitTargetActor(void *);
extern int simAutoCenter;
extern int FogSetValue(FogBuffer *,long,int),AdjustTargetView(void *,ViewState *,int,int,int);
extern "C" int memcmp(const void *,const void *,unsigned);
extern void FogResetMode(FogBuffer *);
struct FogObject;extern FogObject *UnitFogObject(UnitState *,int,void *);
extern long FogCast(FogBuffer *,long,FogObject *);
extern int TargetRecordType(void *,int);
extern UnitState simTargetState;
extern char simActions;
extern void SimClearActions(void *,int=0,int=0,int=0);
extern long PilotValue(void *,int);
extern ActorRecord *ActorFindByOwner(int),*ActorFindOwnerNext(ActorRecord *,int),*ActorFindFree(),*ActorFindNext(ActorRecord *);
struct ActorItem {unsigned char head[7],amount;unsigned char unknown08[0x1f-8];U32 kind;long parameter1,parameter2;unsigned char unknown2b[8];long cost;unsigned char unknown37[4];long sound,stopSound;unsigned char cooldown,mode;};
extern int PopFindItem(void *,int,int);extern void UnitRefresh(void *);inline ActorItem *UnitState::Item(int i,int f){return (ActorItem *)PopFindItem(this,i,f);}
int SimExecuteItem(SimGame *,int,int);
int SimFocusReady(SimGame *s) { if(s->focus && s->focus->active && !s->focus->disabled) return 1; return 0; }
inline int SimClamp(int n,int lo,int hi) { if(n<=lo)return lo; if(n>=hi)return hi; return n; }
inline int SimCanCenter(SimGame *s){return s->linked?0:s->automated?0:SimFocusReady(s);}inline int SimActorHidden(int n){return !actorVisibility[n];}
void SimPan(SimGame *s,int dx,int dy) {
 int speed=SimClamp(cameraSpeed,0,10)*70+50;
 int multiplier=SimShiftDown()?10:1;
 dx=dx*866*multiplier*speed*16/(s->view.scale*100);
 dy=dy*500*multiplier*speed*16/(s->view.scale*100);
 GridPoint d;
 d.x=dx;d.y=dy;
 SimPanBy(s,&d);
}
void SimPanBy(SimGame *s,GridPoint *d) {
 GridPoint point=s->view.point;
 RotateViewPoint(d,-g4d368c);
 point.x+=d->x; point.y+=d->y;
 if(!GridPointToIndex(&fallbackName,&point)) { point.x=s->view.point.x+d->x/4; point.y=s->view.point.y+d->y/4; }
 if(point!=s->view.point) {
  if(GridPointToIndex(&fallbackName,&point)) { ExploCompact(1);
   s->view.point=point; ViewState *v=&s->view; ViewState *saved=&s->savedView; SetTerrainView(gView,v,1); *saved=*v; s->screen|=1;
  }
 }
}
void SimCenterOn(SimGame *s,GridPoint *point) {
 if(s->view.point!=*point) {
  if(GridPointToIndex(&fallbackName,point)) { ExploCompact(1);
   s->view.point=*point; ViewState *v=&s->view; ViewState *saved=&s->savedView; SetTerrainView(gView,v,0); *saved=*v; s->screen|=1;
  }
 }
}
void SimSetMode(SimGame *s,int delta) {
 s->view.direction=(s->view.direction+delta)%6;
 if(s->view.direction<0) s->view.direction+=6;
 simTransform.direction=s->view.direction; SetFastView(&gFastTransform,&simTransform);
 ViewState *v=&s->view; ViewState *saved=&s->savedView; SetTerrainView(gView,v,0); *saved=*v; s->screen|=3;
}
void SimSetView(SimGame *s,register int delta) {
 register int old=s->view.zoom; s->view.zoom+=delta;
 if(SimShiftDown() && delta) s->view.zoom=delta>0?9:2;
 if(s->view.zoom<0) s->view.zoom=0; if(s->view.zoom>15) s->view.zoom=15;
 if(old!=s->view.zoom || delta==0) {
  if(s->linked && delta) SoundPlay(6,0);
  f_44db70(s,0); s->view.scale=viewScales[s->view.zoom];
  ViewState *v=&s->view; ViewState *saved=&s->savedView; SetTerrainView(gView,v,0); *saved=*v; s->screen|=1;
 }
}
void f_44db70(SimGame *s,int toggle) {
 int low=SimClamp(viewMinimum,0,100);
 int high=SimClamp(viewMaximum,0,100);
 low/=2; high=high/2+50; if(high<low) high=low;
 int level=((high-low)*s->view.zoom+4)/9+low;
 if(level<=low) level=low; else if(level>=100) level=100;
 U32 mask=0; for(int i=0;viewMasks[i].mask;i++) if(viewMasks[i].threshold<=level) mask|=viewMasks[i].mask;
 s->view.requested^=toggle;
 if(viewFog) s->view.requested|=16; else s->view.requested&=~16;
 s->view.flags=s->view.requested&mask;
 ViewState *v=&s->view; ViewState *saved=&s->savedView; SetTerrainView(gView,v,0); *saved=*v;
}
void SimSaveBookmark(SimGame *s,int n) {ViewState *dst=s->bookmarks+n; ViewState *src=&s->view; *dst=*src;}
void SimRestoreBookmark(SimGame *s,int n) {ViewState *src=s->bookmarks+n,*dst=&s->view; *dst=*src; f_44db70(s,0);s->screen=-1;}
int SimCycleFocus(SimGame *s,int direction) {
 ActorRecord *a;
 if(direction<0) a=ActorFindRelated(s->focus); else a=ActorFindRelatedOther(s->focus);
 if(a) { SimSetFocus(s,a); SoundPlay(4,0); return 1; }
 return 0;
}
void SimSetFocus(SimGame *s,ActorRecord *a) {
 GridPoint point;
 int oldMode=s->selectedMode;
 if(s->multi && ModeCheck((const char *)s))modeLogName.Focus(a);
 if(s->linked)SimClosePopups(s);
 s->focus=a;s->focusPilot=a->pilot;s->focusUnit=a->unit;
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 if(!s->focus || !s->focusUnit || !s->focusPilot)AssertFail(SimDoSourceFile,__LINE__,0);
 s->focus->done=0;s->focusIndex=a->id;ExploCompact(1);
 AnimBitmapSweep(60000,5000,s->automated?5000:60000,s->automated?60000:5000,15000);
 if(s->linked) {SimRefreshPanel(s,1);SimAnimSet(&s->face,2,UnitPortrait(s->focus->unit),1);}
 s->focusState=*s->focus->unit;memcpy(s->oldStats,s->stats,36);memset(&s->oldState,0,267);
 GridSetActor(&fallbackName,s->focus->unit,s->focus->pilot);
 ActorRecord *focus=s->focus;GetModePlayer(s,focus->owner)->actor=focus->id;
 bestLocCursor=0;
 if(s->focus->owner==s->player)BestLocSetup(bestLocEntries,s->focus->index,0);
 bestLocCount=0;
 if(s->linked) {
  if(!s->focus->disabled) {
   SimRefreshFog(s,0);TargetInitializeRecord(s,(TargetRecord *)s->targetRecord,s->focus->id,s->focus->typeId);
   int destination=*(int *)(s->targetRecord+0x54);
   if(destination) {
    if(!actorVisibility[destination])s->focus->typeId=0;
    ActorRecord *target=*(ActorRecord **)(s->targetRecord+0x98);
    if(target && target->disabled)s->focus->typeId=0;
    if(target && !target->active)s->focus->typeId=0;
    TargetInitializeRecord(s,(TargetRecord *)s->targetRecord,s->focus->id,s->focus->typeId);
   }
  }
  GridClearSelection(&fallbackName);SimPreviewCost(s,1,0);SimPreviewCost(s,0,0);s->actionCost=0;
  point=s->focus->position;SimCenterOn(s,&point);
  int target=s->focus->target;if(target && target!=s->focus->index)SimTargetAction(s,target);
  if(oldMode>4)oldMode=(s->focus->typeId!=0)+1;
  PilotSetState(s,oldMode);s->screen=-1;
 } else bestLocCount=0;
}
void SimFinishFocus(SimGame *s) {
 if(s->focus) { s->focus->done=1; ActorRecord *a=ActorFindRelatedOther(s->focus);
  if(a) { SimSetFocus(s,a); SoundPlay(4,0); }
 }
}
int SimPreviewCost(SimGame *s,int cost,int healthOnly) {
 if(!s->linked) return 0;
 if(!SimFocusReady(s)) { s->preview=0; return 0; }
 if(cost || s->preview) {
  s->screen|=64; s->preview=cost>0;
  s->healthTrend=StateInvoke(s->focus->unit,0,0); s->energyTrend=StateInvokeEnergy(s->focus->unit,0,0);
  if(healthOnly) { s->healthTrend-=cost; if(s->healthTrend<0) {s->healthTrend=-1; return 0;} }
  else { s->healthTrend-=cost; if(s->healthTrend<0) { s->energyTrend+=s->healthTrend;s->healthTrend=0; if(s->energyTrend<0) {s->energyTrend=-1; return 0;} } }
 }
 return 1;
}
int SimSpendOnActor(SimGame *,ActorRecord *,int);

int SimSpendFocus(SimGame *s,int cost) {if(!SimFocusReady(s))return 0;return SimSpendOnActor(s,s->focus,cost);}

int SimSpendOnActor(SimGame *s,ActorRecord *a,int cost) {
 if(a && a->active) {
  if(UnitSpend(a->unit,cost) || !a->spent) { a->spent=1;
   if(a->owner==s->player && a==s->focus) { SoundPlay(2,0); s->screen|=64; }
   return 1;
  }
  if(a->owner==s->player) {SoundPlay(7,0); SoundTrigger(196,0);}
 }
 return 0;
}
int SimTrySpendFocus(SimGame *s,int cost) {
 if(SimFocusReady(s)) {
  if(StateInvoke(s->focus->unit,0,0)>=cost || !s->focus->spent) return SimSpendFocus(s,cost);
  if(s->linked) SoundPlay(7,0);
 }
 return 0;
}
void SimFaceTarget(SimGame *s,int direction,int flags) {
 if(SimFocusReady(s)) {
  int facing=GridClamp(direction);ActorSetDirection(s->focus,facing);s->screen|=4;
  if(s->multi && ModeCheck((const char *)s))modeLogName.Face(s->focus,facing,flags);
 }
}
void SimTargetAction(SimGame *s,int target) {
 if(SimFocusReady(s) && AnimActionHasMetadata(&s->focus->action)) {
  if(!UnitCapability(s->focusPilot,6))SoundTrigger(166,0);
  else if(target) {
   int mobile=s->focus->action.Mobile();
   if(!mobile) {
    AnimActionPrepareBlock(&s->focus->action,(int)s->focus,target);
    if(s->focus->owner==s->player){s->actionCost=s->focus->selectedUnit->cost;SimPreviewCost(s,s->actionCost,1);s->focus->target=target;GridClearSelection(&fallbackName);GridSetSelection(&fallbackName,s->focus->selectedUnit);s->screen|=1;}
    if(s->focus->selectedUnit->kind==1 && s->linked)SoundPlay(7,0);
   }
  }
 }
}
void SimResetFocusCache() {simLastActor=-1;simLastIndex=-1;simLastTarget=-1;}
int SimCenterTarget(SimGame *s,int target) {
 if(SimCanCenter(s)) {
  if(simLastActor==s->focus->id && simLastIndex==s->focus->index && (!target || target==simLastTarget))return 0;
  simLastActor=s->focus->id;simLastIndex=s->focus->index;simLastTarget=target;
  int changed=0;
  if(!s->linked && target && !actorVisibility[s->focus->index]){s->screen|=FogSetValue(&g4d35f4,simLastIndex,99);changed=1;}
  if(!simAutoCenter)return changed;
  if(!simLastTarget && !actorVisibility[s->focus->index])return changed;
  ViewState view=s->view;
  if(AdjustTargetView(gView,&view,simLastIndex,simLastTarget,1) && memcmp(&view,&s->view,32)) {
   s->view=view;SetTerrainView(gView,&s->view,0);s->screen|=1;return 1;
  }
  return changed;
 }
 return 0;
}
void SimPrepareAction(SimGame *s,int target) {
 if(SimFocusReady(s) && AnimActionHasMetadata(&s->focus->action)) {
  if(!UnitCapability(s->focusPilot,6)){SoundTrigger(166,0);return;}
  if(target) {
   int mobile=s->focus->action.Mobile();
   if(!mobile) {
    s->actionCost=0;AnimActionPrepareBlock(&s->focus->action,(int)s->focus,target);
    if(s->focus->selectedUnit->kind==1){if(s->linked){SoundPlay(7,0);return;}}
    else {if(s->linked)*(int *)(s->unknown00+0x14)=target;AnimActionInitTargetActor(&s->focus->action);bestLocCount=target;}
   }
  }
 }
}
void SimRefreshFog(SimGame *s,int force) {
 unsigned char buffer[40];
 if((force || (s->focus && s->focus->owner==s->player)) && s->player!=s->localPlayer) {
  FogResetMode(&g4d35f4);U32 flags=0;
  for(ActorRecord *a=ActorFindFree();a;a=ActorFindNext(a))if(a->owner==s->player && a->action.type!=17)flags|=FogCast(&g4d35f4,a->index,UnitFogObject(a->unit,8,buffer));
  if(flags)s->screen|=7;
 }
}
int SimSelectTarget(SimGame *s,int target) {
 int ready=SimFocusReady(s);
 if(ready) {
  if(s->linked){if(GridClearSelection(&fallbackName))s->screen|=1;if(SimActorHidden(target))return 0;}
  s->focus->typeId=TargetRecordType(s,target);
  TargetInitializeRecord(s,(TargetRecord *)s->targetRecord,s->focus->id,s->focus->typeId);
  ActorRecord *a=*(ActorRecord **)(s->targetRecord+0x98);if(a)simTargetState=*a->unit;
  if(s->linked){SoundPlay(33,0);PilotSetState(s,2);}
  SimClearActions(&simActions,0);SimCenterTarget(s,*(int *)(s->targetRecord+0x54));
  return 0;
 }
 return ready;
}

int ModeCheck(const char *object) {SimGame *s=(SimGame *)object;int n=s->mode-1;PlayerSlot *slots=s->slots;int status=slots[n].status;if(status!=1 && status!=3)return 0;return 1;}
int SimFireFocus(SimGame *s,int mode) {if(SimFocusReady(s))return SimFire(s,s->focus,s->focus->typeId,mode,-1);return 0;}
struct StrPool {char *GetString(long,int=0);};extern StrPool targetBriefResource;
extern unsigned PopItemValue(void *,int);extern int PopItemReady(void *,int);
extern char *ActorItemPrepareText(ActorItem *);extern void UnitRecalculateItems(void *),UnitSetSystem(void *,int);
extern void f_00477295(char *);extern "C" int f_0047723b(char *),sprintf(char *,const char *,...);
struct PopNode;extern PopNode g4d1dec;extern void PopNodeChanged(PopNode *);
extern void SimActionsRefresh(void *);extern int SimDistanceToDepot(SimGame *,ActorRecord *);
struct ExploRecord;struct Weapon;extern ExploRecord *ExploAddTarget(int,int,int);extern void ApplyDamage(int,int,int,Weapon *);
extern void ActorSetActive(ActorRecord *,int),ReleaseState(void *);
extern unsigned MissionRandomTable[],simItemSeed;
int SimExecuteItem(SimGame *s,int item,int toggle) {
 if(s->focus) {
  UnitState *unit=s->focus->unit;
  if(item<4 && PopItemReady(unit,item+19)) {
   ActorItem *entry=unit->Item(item+19);
   ItemSlot *slot=&unit->items[item];
   if(SimFocusReady(s) || entry->kind==4) {
    PopNodeChanged(&g4d1dec);PilotSetState(s,3);
    int cost=entry->cost;unsigned amount=entry->amount;
    if((int)PopItemValue(unit,item+19)<=(int)amount)return 0;
    switch(entry->mode) {
    case 3:case 4:break;
    case 0:
     if(s->player==s->focus->owner) {
      if(PilotValue(s->focus->pilot,202)==28) {SoundTrigger(162,0);f_00477295(targetBriefResource.GetString(1411));return 0;}
      char text[200];sprintf(text,targetBriefResource.GetString(1201),ActorItemPrepareText(entry));
      if(!f_0047723b(text))return 0;
     }
     break;
    case 1:
     if(slot->cooldown || !toggle) {
      slot->cooldown=0;if(entry->stopSound)SoundPlay(entry->stopSound,0);
      if(s->multi && ModeCheck((const char *)s))modeLogName.Toggle(s->focus,(char)item);
      return 1;
     }
     break;
    case 2:
     if(!slot->count){if(entry->stopSound)SoundPlay(entry->stopSound,0);return 1;}
     int distance=SimDistanceToDepot(s,s->focus);
     if(distance<0 || entry->parameter2<distance){SoundPlay(7,0);return 0;}
     cost+=distance*entry->parameter1;
     break;
    }
    if(cost) {int paid=0;if(SimFocusReady(s))paid=SimSpendOnActor(s,s->focus,cost);if(!paid)return 0;}
    if(entry->sound)SoundPlay(entry->sound,0);
    slot->cooldown=entry->cooldown;
    if(s->multi && ModeCheck((const char *)s))modeLogName.Toggle(s->focus,(char)item);
    switch(entry->kind) {
    case 4: {
     unsigned seed=MissionRandomTable[0];MissionRandomTable[0]=0x12345678;simItemSeed=0x12345678;
     int damage=entry->parameter1;
     ExploAddTarget(7,s->focus->index,entry->parameter2);
     int index=s->focus->index;ApplyDamage((int)s,index,index,(Weapon *)damage);
     ReleaseState(s->focus);MissionRandomTable[0]=seed;
     break;
    }
    case 2:PilotSetState(s,5);break;
    case 1:ActorSetActive(s->focus,1);break;
    case 10:
     slot->count--;SimActionsRefresh(&simActions);
     for(int i=11;i<19;i++)UnitSetSystem(unit,i);
     break;
    }
    if(entry->mode==0){memset(slot,0,8);UnitRecalculateItems(unit);}
   }
  }
 }
 return 0;
}
int SimUseItem(SimGame *s,int item,int mode) {
 int result=SimExecuteItem(s,item,mode);
 ActorItem *entry=s->focus->unit->Item(item+19);
 U32 kind=entry->kind;
 if(kind==5 || kind==9) {
  UnitRefresh(s->focus->unit);GridSetActor(&fallbackName,s->focus->unit,s->focus->pilot);SimRefreshFog(s,0);
  int target=s->focus->target;if(target){SimTargetAction(s,target);return result;}
 }
 return result;
}
void SimSetReturnFire(SimGame *s,unsigned char index,int all,int value) {
 if(SimFocusReady(s)) {
  if(s->multi && ModeCheck((const char *)s))modeLogName.ReturnFire(s->focus,(char)index,(char)all,(char)value);
  unsigned char mask=1;mask<<=index;unsigned char *bits=&s->focus->returnFire;
  int enabled=value;if(value<0)enabled=(*bits&mask)==0;
  if(s->linked && value<0)SoundTrigger(enabled?420:393,0);
  if(!all){if(enabled)*bits|=mask;else *bits&=~mask;}
  else for(int i=0;i<8;i++)SimSetReturnFire(s,(unsigned char)i,0,enabled);
 }
}
void SimAdoptFocus(SimGame *s,ActorRecord *a) {
 s->focus=a;s->focusPilot=a->pilot;s->focusUnit=a->unit;
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 // Preserve original sim\simdo.c assertion line.
 if(!s->focus || !s->focusUnit || !s->focusPilot)AssertFail(SimDoSourceFile,__LINE__,0);
 s->focus->done=0;s->focusIndex=a->id;
 ActorRecord *focus=s->focus;GetModePlayer(s,focus->owner)->actor=focus->id;
}
extern unsigned SimLogicFlags(void *);
extern void SendEvent(void *,int,void * =0,int=0,int=0),TimeSleep(unsigned long),SimActorKilled(SimGame *,ActorRecord *,int,int);
extern int modeSwitch;
extern ActorRecord *ActorGetByIndex(int);
extern int TargetChoose(TargetSelection *,TargetRecord *,int,int);
extern "C" unsigned next_link_seed(unsigned *);
extern unsigned MissionRandomTable[];
inline unsigned SimRandom(unsigned limit) {return next_link_seed(MissionRandomTable)%limit;}
int SimReturnShot(SimGame *s);
int SimRandomFire(SimGame *s,int mode,int notify) {
 if(s->linked)PilotSetState(s,2);
 if(mode<0) {
  if(!SimFocusReady(s))return 0;
  ActorRecord *focus=s->focus;int type=focus->typeId;if(type==0)return 0;
  TargetRecord record;TargetInitializeRecord(s,&record,focus->id,type);
  mode=TargetChoose(&s->targets,&record,0,0);
  if(mode<0) {
   if(s->focus->owner==s->player && notify) {
    int free=0;
    for(int i=0;i<8;i++)if(!s->targets.slots[i].used){free=1;break;}
    SoundTrigger(free?394:395,0);
   }
   return 0;
  }
 }
 if(s->multi) {
  unsigned replies=SimRandom(2);
  if(SimFireFocus(s,mode)) {for(int i=0;i<(int)replies;i++)SimReturnShot(s);return 1;}
  return 0;
 }
 int result=0;
 unsigned replies=SimRandom(2);
 unsigned first=SimRandom(2);
 if(first)result=SimFireFocus(s,mode);
 for(int i=0;i<(int)replies;i++)SimReturnShot(s);
 if(!first)result=SimFireFocus(s,mode);
 return result;
}
int SimReturnShot(SimGame *s) {
 TargetRecord record;
 TargetInitializeRecord(s,&record,s->focus->id,s->focus->typeId);
 ActorRecord *target=ActorGetByIndex(record.targetActor),*old=s->focus;
 if(old && target && target!=old && target->active) {
  if(target->owner==s->mode)return 0;
  SimAdoptFocus(s,target);
  s->focus->typeId=TargetRecordType(s,old->index);
  TargetInitializeRecord(s,&record,s->focus->id,s->focus->typeId);
  int mode=TargetChoose(&s->targets,&record,25,1);
  if(mode>=0)SimFireFocus(s,mode);
  SimAdoptFocus(s,old);
  s->focus->typeId=TargetRecordType(s,target->index);
  TargetInitializeRecord(s,&record,s->focus->id,s->focus->typeId);
 }
 return 0;
}
void SimStartPhase(SimGame *s) { if(s->phase==0 || s->phase==1) {s->phase=2; SimResetLimits(s);} }
void SimDelayPhase(SimGame *s) { if(s->phase<4) {s->phase=4; s->deadline=s->tick+1500;} }



void PilotNext(SimGame *s,ActorRecord *actor,int all,int notify) {
 if(actor) {
  if(all) {
   int owner=actor->owner;
   for(ActorRecord *a=ActorFindByOwner(owner);a;a=ActorFindOwnerNext(a,owner)) {PilotNext(s,a,0,notify);TimeSleep(50);}
  } else {
   void *pilot=actor->pilot;
   if(pilot) {
    unsigned flags=SimLogicFlags(pilot)&1;
    if(flags) {
     unsigned old=PilotValue(pilot,207),limit=PilotValue(pilot,208);
     SendEvent(pilot,104);SoundPlay(39,0);SoundTrigger(172,0);
     unsigned now=PilotValue(pilot,207);
     if(now>limit && (int)limit>(int)old) {SoundTrigger(149,0);SoundTrigger(150,0);}
     if(modeSwitch)modeLogName.JackUp(actor);
     unsigned after=SimLogicFlags(pilot)&1;
     if(flags && !after)SimActorKilled(s,actor,1208,notify);
     SimRefreshPanel(s,1);
    }
   }

  }
 }
}
void PilotPrevious(SimGame *s,ActorRecord *actor,int all,int notify) {
 if(actor) {
  if(all) {
   int owner=actor->owner;
   for(ActorRecord *a=ActorFindByOwner(owner);a;a=ActorFindOwnerNext(a,owner)) {PilotPrevious(s,a,0,notify);TimeSleep(50);}
  } else {
   void *pilot=actor->pilot;
   if(pilot) {
    unsigned flags=SimLogicFlags(pilot)&1;
    if(flags) {
     unsigned old=PilotValue(pilot,207),limit=PilotValue(pilot,208);
     SendEvent(pilot,105);SoundPlay(40,0);SoundTrigger(173,0);
     unsigned now=PilotValue(pilot,207);
     if(now>limit && (int)limit>(int)old) {SoundTrigger(149,0);SoundTrigger(150,0);}
     if(modeSwitch)modeLogName.Placidex(actor);
     unsigned after=SimLogicFlags(pilot)&1;
     if(flags && !after)SimActorKilled(s,actor,1208,notify);
     SimRefreshPanel(s,1);
    }
   }

  }
 }
}
void IntelClearMode(const char *object) {
 SimGame *s=(SimGame *)object;
 if(s->focus) {if(s->multi && ModeCheck((const char *)&modeCheckName))modeLogName.Remove(s->focus);ActorClearSelection(s->focus);}
}

extern unsigned char statusColorLow,statusColorEmpty;
extern void f_44bba0(SimGame *);extern "C" void ShellRestore(SimGame *);
extern int logicMode;
extern void ModeEvent(SimGame *,ActorRecord *,int,int),SimMaintainUnit(UnitState *,void *),SimUpdateTimer(SimGame *,int),SimApplyLeadership(SimGame *,ActorRecord *);
extern int UnitFindType(UnitState *,int,int),UpdatePayload(UnitState *,int);
extern ActorRecord *ActorListTransfer(int);
extern void ActivateMainWindow();struct MenuNetwork {void SetTimeout();};extern MenuNetwork gMenuNetwork;
struct Shell;extern void DeathSummary(Shell *);extern "C" int BriefWaitA(int);
extern void LogicUpdateMissionStatus(unsigned char *);extern unsigned char g4dd0c0[];
void SimSetUpdate(SimGame *,int);
inline int SimLinkedMode(){return logicMode==2 || logicMode==3;}
void SimBeginTurn(SimGame *s) {
 if(!s->slots[s->mode-1].status){SimStartPhase(s);return;}
 s->slot->points=0;f_44bba0(s);SimUpdateTimer(s,0);
 ActorRecord *a;
 for(a=ActorFindByOwner(s->mode);a;a=ActorFindOwnerNext(a,s->mode)) {
  unsigned amount=PopItemValue(a->unit,10),penalty=0;
  if(!amount)penalty=statusColorEmpty;else if((int)amount<100)penalty=statusColorLow;
  if(penalty>254)penalty=9999;
  if(penalty)ModeEvent(s,a,penalty,1205);
 }
 for(a=ActorFindByOwner(s->mode);a;a=ActorFindOwnerNext(a,s->mode)) {
  a->done=0;unsigned alive=SimLogicFlags(a->pilot)&1;
  SimMaintainUnit(a->unit,a->pilot);
  unsigned after=SimLogicFlags(a->pilot)&1;
  if(alive && !after)SimActorKilled(s,a,1207,1);
  a->spent=a->unit->bytes[0x3b]!=100;
  UpdatePayload(a->unit,a->unknown26[0]);
  int index=UnitFindType(a->unit,7,1);
  if(index)s->slot->points+=a->unit->Item(index)->parameter1;
  if(SimLogicFlags(a->pilot)&1){SimApplyLeadership(s,a);SendEvent(a->pilot,111,(void *)40);}
 }
 for(a=ActorFindFree();a;a=ActorFindNext(a)) {
  if(a->action.type==17)a->disabled=1;
  if(a->model>8 && a->model<18 && !a->active)a->disabled=1;
 }
 GridClearSelection(&fallbackName);
 if(s->unknown982!=3 && s->unknown982!=5) {
  modeCheckName.view=modeCheckName.savedView;SetTerrainView(gView,&modeCheckName.view,0);
  ActorRecord *saved=ActorListTransfer(s->slot->actor);
  if(!saved || saved->owner!=s->mode)ActorFindByOwner(s->mode);
  SimSetFocus(s,ActorListTransfer(s->slot->actor));SimRefreshFog(s,0);
 }
 switch(s->unknown982) {
 case 1:
  ShellRestore(s);SimSetUpdate(s,1);s->selectedMode=0;s->uiState=0;s->previousMode=s->selectedMode;
  PilotSetState(s,s->savedMode);
  if(s->multi || s->extraSaved){ActivateMainWindow();gMenuNetwork.SetTimeout();SimUpdateTimer(s,1);}
  DeathSummary((Shell *)s);
  BriefWaitA(s->multi || SimLinkedMode()?13:10);
  break;
 case 2:case 5:
  ShellRestore(s);BriefWaitA(s->multi || SimLinkedMode()?14:11);s->selectedMode=0;s->previousMode=s->selectedMode;PilotSetState(s,7);f_44bba0(s);break;
 case 3:
  ShellRestore(s);LogicUpdateMissionStatus(g4dd0c0);BriefWaitA(s->multi || SimLinkedMode()?14:11);s->selectedMode=0;s->previousMode=s->selectedMode;PilotSetState(s,7);break;
 }
 if(s->unknown982==1) {
  s->waiting=1;
  for(a=ActorFindFree();a;a=ActorFindNext(a))if(a->action.type!=17 && a->owner!=s->player && actorVisibility[a->index]){s->waiting=0;return;}
 } else s->waiting=0;
}
extern void *simUnitsMenu;
extern void *create_chat_popup(void *,int);
extern int chat_menu_valid(void *),SimHotbriefCheckActor(ActorRecord *);
extern void SimUnitsMenuClear(void *),build_chat_menu_line(void *,unsigned,const char *,unsigned);
extern "C" int sprintf(char *,const char *,...);
void SimSetUpdate(SimGame *s,int open) {
 char text[128];
 if(!open) {
  if(simUnitsMenu) {chat_menu_valid(simUnitsMenu);simUnitsMenu=0;}
 } else {
  if(!simUnitsMenu)simUnitsMenu=create_chat_popup((void *)"Units",-1);
  if(simUnitsMenu) {
   SimUnitsMenuClear(simUnitsMenu);
   int count=0;
   ActorRecord *a;for(a=ActorFindByOwner(s->mode);a;a=ActorFindOwnerNext(a,s->mode))count+=a->active && SimHotbriefCheckActor(a)?1:0;
   for(a=ActorFindFree();a;a=ActorFindNext(a)) {
    if(a->id && a->owner==s->mode && a->active && !a->disabled && AnimActionHasMetadata(&a->action)) {
     sprintf(text,"%s%c(%s)",a->UnitText(),(signed char)(count>18?' ':'\t'),(const char *)a->pilot);
     build_chat_menu_line(simUnitsMenu,a->id+20000,text,18);
    }
   }
  }
 }
}



