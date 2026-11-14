// sim\simturn.c: turn ownership, player slots and actor status. Working names unless bound.
// Gaps are temporary unnamed layout fields, not reconstructed payloads.
#pragma pack(push,1)
typedef unsigned long U32;
struct GridPoint { long x,y;
 int operator!=(const GridPoint &p) const { return x!=p.x || y!=p.y; }
};
struct ViewState { GridPoint point; long zoom,scale,direction; U32 flags,requested; long spare; };
struct UnitState { unsigned char bytes[0x10b]; };
struct SimStat {long value,maximum,spare;};
struct SimAnim {unsigned char bytes[6]; unsigned char mode,frame;};
struct ActorTraits {unsigned char prefix[5],type; int Mobile() const {return type!=1 && type!=7 && type!=14 && type!=17 && type!=18;}};
struct ActorRecord {
 int id; int unknown04; int owner; int typeId; int unknown10,target;
 UnitState *unit; void *pilot; ActorTraits traits;
 unsigned char variant; unsigned char unknown27[5]; unsigned short model;
 unsigned char unknown2e[4]; GridPoint position; unsigned char unknown3a[0x87-0x3a]; unsigned char done,active,disabled;
 unsigned char unknown8a[3]; unsigned char spent;
 int Ready() const {return active && !disabled;}
};
struct PlayerSlot { int status,id,points,actor; unsigned char rest[0xbc-16]; };
struct SimGame {
 unsigned char unknown00[0x2c]; int screen; unsigned char unknown30[8]; ActorRecord *focus;
 void *unknown3c; UnitState *focusUnit; SimStat stats[3]; SimAnim portrait,face,statusAnim; unsigned char unknown80[0x2ba-0x80];
 int phase; int turn; int multi; unsigned char unknown2c6[4]; int tick,deadline;
 unsigned char unknown2d2[4]; int palette,playerCount; PlayerSlot slots[9];
 PlayerSlot *slot; int mode,slotStatus,player,localPlayer,unknown98e,linked,remote,automated,nextPlayer,forcedPlayer;
 unsigned char unknown9a6[0xdf5-0x9a6]; ViewState bookmarks[13],view,savedView;
 unsigned char unknownfd5[0x1025-0xfd5]; int preview; long healthTrend,energyTrend; unsigned char unknown1031[0x10f1-0x1031]; int extraSaved;
};
#pragma pack(pop)
extern "C" void AssertFail(const char *,int,const char *); static char simturnSourceFile[] = __FILE__;
extern "C" void *memset(void *,int,unsigned);
extern "C" int sprintf(char *,const char *,...);
extern ActorRecord *ActorFindByOwner(int),*ActorFindOwnerNext(ActorRecord *,int),*ActorFindFree(),*ActorFindNext(ActorRecord *),*ActorListTransfer(int);
extern int SimHotbriefCheckActor(ActorRecord *);
extern int UpdatePayload(UnitState *,int);
extern void SimSetFocus(SimGame *,ActorRecord *);
extern void SimHotBriefGameOver(void *),SimHotWriteFogBuffers(int),SimHotReadFogBuffers(int),SimHotInitializeFog();
extern char g513888;
extern void SigsStart(void *,int),SurfacePresent(void *);
extern long PilotValue(void *,int);
extern void SimAnimSet(SimAnim *,int,int,int);
extern int turnPortraitMode;
extern unsigned ISqrt(unsigned);
extern void SendEvent(void *,int,void *,int,int);
struct StrPool {char *GetString(long,int=0);};
extern StrPool targetBriefResource;
extern void SimMessage(SimGame *,const char *);
extern PlayerSlot *GetModePlayer(SimGame *,int);
extern int logicMode;
extern "C" void BriefWaitA(int);
inline int ActorIsMobile(ActorRecord *a) {return a->traits.type!=1 && a->traits.type!=7 && a->traits.type!=14 && a->traits.type!=17 && a->traits.type!=18;}

int SimOnlyStationary(SimGame *s) {
 int result=1;
 for(ActorRecord *a=ActorFindByOwner(s->mode);a;a=ActorFindOwnerNext(a,s->mode)) if(a->traits.Mobile()) result=0;
 return result;
}
int SimPlayerHasUnits(SimGame *s,int n) {
 if(n==s->localPlayer) return 1;
 int result=0;
 for(ActorRecord *a=ActorFindByOwner(n);a;a=ActorFindOwnerNext(a,n)) {
  if(a->owner==s->localPlayer) result=1;
  else if(SimHotbriefCheckActor(a)) result=1;
 }
 return result;
}
void SimUpdateOwnerPayloads(SimGame *s) {
 for(ActorRecord *a=ActorFindByOwner(s->mode);a;a=ActorFindOwnerNext(a,s->mode)) UpdatePayload(a->unit,a->variant);
}
void SimChooseNextPlayer(SimGame *s) {
 int next;
 if(s->multi && s->forcedPlayer) {
  s->nextPlayer=s->forcedPlayer;
  s->forcedPlayer=0;
  return;
 }
 else {
  int remaining=18;
  int n=s->mode;
  next=0;
  int wrapped=0;
  s->nextPlayer=0;
  while(remaining--) {
   if(n==s->playerCount) {n=1;wrapped=1;} else n++;
   int status=GetModePlayer(s,n)->status;
   if(status!=0 && status!=6 && status!=7 && SimPlayerHasUnits(s,n)) next=n;
   if(next) break;
  }
  s->turn+=wrapped;
 }
 s->nextPlayer=next;
}

void SimChangePlayer(SimGame *s,int n) {
 int old=s->mode;
 s->slot=GetModePlayer(s,n); s->slotStatus=s->slot->status; s->mode=s->slot->id;
 s->linked=s->slotStatus==1; s->remote=s->slotStatus==2; s->automated=s->slotStatus==3;
 if(s->extraSaved) {
  if(!s->automated) {
   SimHotBriefGameOver(s); SimHotWriteFogBuffers(old); SimHotReadFogBuffers(s->mode);
   s->linked=1; s->remote=0; s->automated=0; int player=s->mode; s->screen=-1; s->player=player;
  } else {
   if(ActorFindByOwner(s->localPlayer)) SimHotBriefGameOver(s);
   SimHotInitializeFog(); SigsStart(&g513888,0); SurfacePresent(&g513888); int player=s->localPlayer; s->screen=-1; s->player=player;
  }
 }
}
void SimRestoreOwnerFocus(SimGame *s) {
 ActorRecord *a=ActorListTransfer(s->slot->actor);
 if(!a || a->owner!=s->mode) ActorFindByOwner(s->mode);
 SimSetFocus(s,ActorListTransfer(s->slot->actor));
}
void SimDisableInactiveActors() {
 for(ActorRecord *a=ActorFindFree();a;a=ActorFindNext(a)) {
  if(a->traits.type==17) a->disabled=1;
  if(a->model>=9 && a->model<=17 && !a->active) a->disabled=1;
 }
}
void SimRefreshPanel(SimGame *s,int force) {
 if(!s->focus || s->focus->owner==s->player) {
  ActorRecord *a=s->focus;
  if(!a || !a->pilot || !a->unit) memset(s->stats,0,36);
  else {
   void *pilot=a->pilot;
   s->stats[0].value=PilotValue(pilot,200); s->stats[0].maximum=PilotValue(pilot,201); s->stats[0].spare=-1;
   s->stats[1].value=PilotValue(pilot,204); s->stats[1].maximum=PilotValue(pilot,205); s->stats[1].spare=PilotValue(pilot,206);
   s->stats[2].value=PilotValue(pilot,207); s->stats[2].maximum=PilotValue(pilot,208); s->stats[2].spare=PilotValue(pilot,209);
   unsigned mode=PilotValue(pilot,215)-64;
   if(s->portrait.mode!=mode || force) {
    s->screen|=0x400;
    unsigned portrait=PilotValue(s->focus->pilot,202)+1;
    if(turnPortraitMode<60 || mode==4) {SimAnimSet(&s->portrait,1,portrait,5); s->portrait.frame=(char)mode-1;}
    else SimAnimSet(&s->portrait,1,portrait,mode);
   }
   s->screen|=0x50;
  }
 }
}
inline long PointDistance(const GridPoint *a,const GridPoint *b){long dx=a->x-b->x;dx*=dx;long dy=a->y-b->y;dy*=dy;dx+=dy;return ISqrt(dx);}
void SimApplyLeadership(SimGame *s,ActorRecord *target) {
 char text[128]; int total=0;
 for(ActorRecord *a=ActorFindByOwner(s->mode);a;a=ActorFindOwnerNext(a,s->mode)){
  int distance=(int)(PointDistance(&a->position,&target->position)+100)/1000;
  int value=PilotValue(a->pilot,7);if(distance>2)value-=(distance-2)*10;if(value>0)total+=value/2;}
 unsigned before=PilotValue(target->pilot,204);
 SendEvent(target->pilot,108,(void *)total,0,0); unsigned after=PilotValue(target->pilot,204);
 if(before!=after && s->linked) {sprintf(text,targetBriefResource.GetString(1204),target->pilot); SimMessage(s,text);}
}

int SimAddPlayer(SimGame *s,int status,PlayerSlot *source) {
 if(s->playerCount>=9) return 0;
 PlayerSlot *p=&s->slots[s->playerCount]; memset(p,0,sizeof(*p)); if(source) *p=*source;
 p->status=status; ++s->playerCount; p->id=s->playerCount; return s->playerCount;
}
void SimRemovePlayer(SimGame *s,int n) {GetModePlayer(s,n)->status=0;}
PlayerSlot *GetModePlayer(SimGame *s,int n) {
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
// Original checked-slot ASSERT source line follows.
 if(!(n>0 && n<=s->playerCount)) AssertFail(simturnSourceFile,__LINE__,0);
 if(n<1 || n>s->playerCount || n!=s->slots[n-1].id)return 0;return &s->slots[n-1];
}
inline int TurnMenuMode() {return logicMode==2 || logicMode==3;}
void SimSetMenuState(SimGame *s,int enabled) {
 if(s->multi || TurnMenuMode()) BriefWaitA((enabled==0)+13);
 else BriefWaitA((enabled==0)+10);
}



