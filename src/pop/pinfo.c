#pragma aux callback_popinfoevent "W?PopInfoEvent$n()i";
struct SimGame;int __pragma("callback_popinfoevent") PopInfoEvent(SimGame *);
// Unknown original TU: POPINFO.BOX screen, 0044950c..00449bb4.
// Class and function names are working labels. Partial field layout is evidence-based.
typedef unsigned long U32;
struct GuiRect { long x,y,width,height; };
struct GuiPoint { long x,y; };
struct InputReq; struct GuiControl;
struct StrPool { long id; char *buffer; long capacity; void *data; long size;
 StrPool(long,long=0,long=0); ~StrPool(); int Lock(); void Unlock(); char *GetString(long,int=0); };
struct Timer { U32 start,end,duration; };
class GuiScreen:public GuiRect {
public:
 InputReq *input; GuiControl **controls; StrPool *strings;
 unsigned count,focus; long hovered; unsigned pressed; U32 options;
 long firstString; int navigation,delayed; unsigned delayedControl; Timer repeatTimer;
 GuiScreen(GuiControl **,unsigned,U32);
 void LoadLayout(long); void Open(InputReq *,StrPool *,long,int);
 long HitTest(const GuiPoint *); int Update(); void Draw(); void Invalidate();
 int IsActive() const { return input != 0; }
};
extern GuiControl *popInfoControls[];
struct PopInfo:public GuiScreen { int dirty,index; PopInfo(); ~PopInfo(); };
extern StrPool targetBriefResource;
extern GuiPoint g4bb78c;
extern long targetLimits;
extern unsigned char *simVisibleTypes;
inline int SimTypeUnavailable(int n) { return !simVisibleTypes[n]; }
class MenuNumber { char layout[48]; public: MenuNumber(signed char,signed char); ~MenuNumber(); };
class MenuSpacer { char layout[32]; public: MenuSpacer(); ~MenuSpacer(); };
MenuNumber popInfoNumber(-1,18);
MenuSpacer popInfoFrame,popInfoTextFrame;
PopInfo popInfo;
PopInfo::PopInfo():GuiScreen(popInfoControls,3,0) { index=0; }
void PopInfoOpen(PopInfo *p,InputReq *input) {
 if (!p->IsActive()) { p->LoadLayout((long)"POPINFO.BOX"); p->Open(input,&targetBriefResource,1001,0); p->dirty=1; }
}
void PopInfoClose(PopInfo *p) { if(p->IsActive()) p->input=0; }
void PopInfoUpdate(PopInfo *p) { if(p->IsActive()) { long n=p->HitTest(&g4bb78c); if(n>=0) targetLimits=n+900; p->Update(); } }
struct ViewRect { long left,top,right,bottom; };
extern ViewRect viewportClipLeft,gBriefDrawSource;
extern GuiRect gMenuClip;
extern char g513888;
extern void GuiRemapClipRect(GuiRect *,long,long,long,long);
extern void PopDrawSurface(void *,int,int,int,int,int,int);
struct Font;
struct TargetInfo { unsigned short value; signed char a,b; long action,count; };
struct TargetDefinition { unsigned char type,quantity,hasSpecial,special,hasExtra,extra; unsigned char rest[6]; };
struct ViewState { long x,y,z,zoom,a,offset,b,c; };
struct WorldAction { unsigned char head[12]; unsigned short type; unsigned char rest[97]; };
extern TargetDefinition *targetDefinitionTable;
extern WorldAction *worldActions;
extern StrPool missionImageName;
extern Font g511dc0;
extern U32 gMenuTextColor,*gMenuTextMap;
extern U32 infoTextMap[],infoTextColor;
extern char fallbackName,g4c68b4,gView,infoClip;
extern ViewState infoView;
extern void TargetInitAt(const char *,void *,int,int);
extern long InfoResourceName(void *,const char *);
extern void MenuDrawTextAt(Font *,long,long,int,const char *);
extern "C" char *BriefFormat(const char *,...);
extern "C" char *strcpy(char *,const char *);
extern "C" char *strcat(char *,const char *);
extern void RichTextDraw(long,long,long,long,const unsigned char *,long,long,long,long *,long *,const long *,int,int=1,U32=0,long=0,long=0x7fffffff);
extern void InfoSetView(void *,const ViewState *,int);
extern void InfoDrawTarget(void *,int,const GuiPoint *,int);
extern void GuiResetClip(void *);
void PopInfoDrawText(PopInfo *unused,int n) {
 GuiPoint point;
 TargetInfo target;
 gMenuTextMap=infoTextMap; gMenuTextColor=infoTextColor;
 if(n<=0) { MenuDrawTextAt(&g511dc0,72,280,0,targetBriefResource.GetString(1004)); return; }
 TargetDefinition *def=&targetDefinitionTable[n];
 TargetInitAt(&fallbackName,&target,n,0);
 MenuDrawTextAt(&g511dc0,15,230,-1,targetBriefResource.GetString(1006));
 MenuDrawTextAt(&g511dc0,125,230,1,BriefFormat("%d",def->quantity));
 int y=244;
 MenuDrawTextAt(&g511dc0,15,y,-1,targetBriefResource.GetString(1007));
 MenuDrawTextAt(&g511dc0,125,y,1,target.action<0?targetBriefResource.GetString(1014):BriefFormat("%d",target.value));
 y+=14;
 if(target.b) { MenuDrawTextAt(&g511dc0,15,y,-1,targetBriefResource.GetString(1010)); MenuDrawTextAt(&g511dc0,125,y,1,BriefFormat("%d",target.b)); y+=14; }
 if(target.a) { MenuDrawTextAt(&g511dc0,15,y,-1,targetBriefResource.GetString(1011)); MenuDrawTextAt(&g511dc0,125,y,1,BriefFormat("%d",target.a)); }
 StrPool strings(InfoResourceName(&g4c68b4,"bin")); strings.Lock();
 ViewState view;
 char text[256];
 strcpy(text,""); strcat(text,strings.GetString(def->type+101,1));
 if(def->hasSpecial) {
   if(def->special==66) strcat(text,BriefFormat(", %s (%d %s)",targetBriefResource.GetString(1012),target.count,targetBriefResource.GetString(1013)));
   else { char *name=strings.GetString(def->special+116,1); if(name) strcat(text,BriefFormat(", %s",name)); }
 }
 if(def->hasExtra) { char *name=strings.GetString(def->extra+116,1); if(name) strcat(text,BriefFormat(", %s",name)); }
 if(target.action<0) { long id=worldActions[-target.action-1].type+1716; strcat(text,", "); strcat(text,missionImageName.GetString(id,1)); }
 strings.Unlock();
 RichTextDraw(15,380,115,60,(const unsigned char *)text,0,0,4,0,0,0,11);
 view=infoView; view.zoom=16; view.offset=-57;
 InfoSetView(&gView,&view,0);
 point.x=72; point.y=330;
 InfoDrawTarget(&gView,n,&point,0); GuiResetClip(&infoClip); InfoSetView(&gView,&infoView,0);
}
void PopInfoDraw(PopInfo *p) {
 if(p->IsActive()) { if(p->dirty) {
 p->dirty=0; viewportClipLeft=gBriefDrawSource;
 long x=p->x,y=p->y,w=p->width,h=p->height;
 GuiRemapClipRect(&gMenuClip,x,y,w,h);
 PopDrawSurface(&g513888,1,0,p->x,p->y,p->width,p->height);
 p->Invalidate(); PopInfoDrawText(p,p->index);
 } p->Draw(); }
}
void PopInfoSelect(PopInfo *p,int n) { p->dirty=1; p->index=n; if(SimTypeUnavailable(p->index)) p->index=0; }
int PopInfoEvent(SimGame *) { if(popInfo.IsActive()) { long n=popInfo.HitTest(&g4bb78c); if(n>=0) targetLimits=n+900; return popInfo.Update(); } return 0; }
PopInfo::~PopInfo() { input=0; }

static char pinfoSourceFile[] = __FILE__;
GuiControl *popInfoControls[] = {(GuiControl *)&popInfoNumber,(GuiControl *)&popInfoFrame,(GuiControl *)&popInfoTextFrame};
