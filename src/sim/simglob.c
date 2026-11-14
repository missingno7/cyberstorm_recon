// sim/simglob.c, 0044b8b0..0044bcd0. Global construction order follows the oracle.
// Gaps are temporary unnamed layout fields, not reconstructed payloads.
#pragma pack(push,1)
typedef unsigned long U32;
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
DifficultyLimit difficultyLimits[]={{30,3,4,0,0},{45,2,4,25,0},{60,1,3,50,25},{90,0,3,75,25},{180,0,2,100,50},{180,0,2,100,50},{180,0,1,100,75},{300,0,0,100,100},{0,0,0,0,0}}; static char simglobSourceFile[]=__FILE__; signed char simShortcutModes[]={0,5,6,9,1,2,3,4,0,0,0,0};
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

struct GuiPoint {long x,y;};
struct GuiControlData:GuiRect {unsigned char style[4];U32 attributes,shortcut;};
class GuiControl:public GuiControlData {public:virtual ~GuiControl();virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);virtual int HitTest(const GuiPoint *);};
class MenuHelpRegion:public GuiControl {public:long id;int activated;MenuHelpRegion();virtual ~MenuHelpRegion();virtual void Draw(unsigned);virtual int Event(unsigned,U32);};
TerrainGrid fallbackName;
FastBuffer g4d35e4;
FogBuffer g4d35f4((FogSource *)&fallbackName);
WorldView gView(&fallbackName,31);
FastCoord gFastTransform(&fallbackName,31);
ViewState simTransform(0,GridPoint(10,6),2,0);
BestLocState bestLocEntries(&fallbackName,10);
struct ViewRect {long left,top,right,bottom;void Set(long l,long t,long r,long b){left=l;top=t;right=r;bottom=b;}};
ViewRect g4d381c,gViewRect,simPortraitRect,simStatusRect,g4d385c;
MenuHelpRegion simHelp;
unsigned char worldColorsBackup[1024];
int simWorldBackground,simWorldForeground;
struct ClipState {long words[10];};
ClipState g4d3c9c,simClip,g4d3cec,simMainClip,simMainSaved,simSmallClip,simDetailClip,simDetailSaved,simScratchClip;
struct SimRandomSeed {unsigned value;SimRandomSeed(){value=0x12345678;}};
SimRandomSeed gFxSeed,simItemSeed;
int simScrollDeadline;
extern char g513888;extern int g5138ac,g5138b0,screenHeight;
extern GuiRect gMenuClip;
extern void SigsStart(void *,int),SurfacePresent(void *),GuiResetClip(void *),SimClipInitialize(ClipState *,const ViewRect *);
void f_44bba0(SimGame *);
void SimSetupWorld(SimGame *s) {
 int width=g5138ac,height=g5138b0;if(screenHeight<500)height-=20;
 SigsStart(&g513888,0);SurfacePresent(&g513888);GuiResetClip(&gMenuClip);
 simTransform.direction=s->view.direction;SetFastView(&gFastTransform,&simTransform);
 gViewRect.Set(148,0,width-1,height-87);
 simPortraitRect.Set(0,0,147,140);
 simStatusRect.Set(0,141,147,height-1);
 g4d381c.Set(0,0,width-1,height-1);
 g4d385c.Set(148,height-86,width-1,height-1);
 SimClipInitialize(&g4d3c9c,&simPortraitRect);SimClipInitialize(&simClip,&simStatusRect);SimClipInitialize(&g4d3cec,&g4d385c);SimClipInitialize(&simMainClip,&gViewRect);
 simMainSaved=simMainClip;simDetailClip=simMainClip;simDetailSaved=simMainClip;
 GuiRect clip;clip.x=gViewRect.left;clip.y=gViewRect.top;clip.width=gViewRect.right-gViewRect.left+1;clip.height=gViewRect.bottom-gViewRect.top+1;
 gView.SetClip(&clip);f_44bba0(s);
}
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
void SimSetTransformX(int x) {simTransform.flags=x; SetFastView(&gFastTransform,&simTransform);}
int SimShortcutMode() {
 int n=gIn80-59; if(n<0 || n>11)return 0;
 if(SimShiftDown() || SimAltDown())return 0;
 return simShortcutModes[n];
}
