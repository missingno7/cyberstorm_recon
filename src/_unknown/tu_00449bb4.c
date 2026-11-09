#pragma aux callback_popworldevent "W?PopWorldEvent$n()i";
struct SimGame;int __pragma("callback_popworldevent") PopWorldEvent(SimGame *);
// Unknown original TU: POPINFO.BOX screen, 0044950c..00449bb4.
// Class and function names are working labels. Partial field layout is evidence-based.
typedef unsigned long U32;
struct GuiRect { long x,y,width,height; };
struct GuiPoint { long x,y; };
struct InputReq; struct GuiControl;
struct StrPool { int Lock(); void Unlock(); char *GetString(long,int=0); char *GetSecondString(long,int=0); };
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

extern GuiControl *popWorldControls[];
struct PopWorld:public GuiScreen { int dirty; PopWorld(); ~PopWorld(); };
extern StrPool targetBriefResource;
extern GuiPoint g4bb78c;
extern long targetLimits;
class MenuNumber { char layout[48]; public: MenuNumber(signed char,signed char); ~MenuNumber(); };
class MenuSpacer:public GuiRect { char layout[16]; public: MenuSpacer(); ~MenuSpacer(); };
MenuNumber popWorldNumber(-1,18);
MenuSpacer popWorldFrame;
PopWorld popWorld;
PopWorld::PopWorld():GuiScreen(popWorldControls,2,0) {}
void PopWorldOpen(PopWorld *p,InputReq *input) {
 p->dirty=1;
 if(!p->IsActive()) { p->LoadLayout((long)"POPWORLD.BOX"); p->Open(input,&targetBriefResource,1401,0); }
}
void PopWorldClose(PopWorld *p) { if(p->IsActive()) p->input=0; }
void PopWorldUpdate(PopWorld *p) { if(p->IsActive()) { long n=p->HitTest(&g4bb78c); if(n>=0) targetLimits=n+1300; p->Update(); } }
struct ViewRect { long left,top,right,bottom; };
extern ViewRect viewportClipLeft,gBriefDrawSource;
extern GuiRect gMenuClip;
extern char g513888;
extern void GuiRemapClipRect(GuiRect *,long,long,long,long);
extern void PopDrawSurface(void *,int,int,int,int,int,int);
extern StrPool worldStrings,worldCaptionStrings;
extern long worldDescriptionId;
extern char gDebugText[];
extern void DebugTextClear();
extern "C" int sprintf(char *,const char *,...);
extern void RichTextDraw(long,long,long,long,const unsigned char *,long,long,long,long *,long *,const long *,int,int=1,U32=0,long=0,long=0x7fffffff);
void PopWorldDrawText() {
 worldStrings.Lock(); long id=worldDescriptionId;
 DebugTextClear(); id+=40;
 sprintf(gDebugText,"\n\n%s:  %s\n\n     %s",worldCaptionStrings.GetString(154),worldStrings.GetString(id,1),worldStrings.GetSecondString(id,1));
 RichTextDraw(popWorldFrame.x,popWorldFrame.y,popWorldFrame.width,popWorldFrame.height,(const unsigned char *)gDebugText,0,0,4,0,0,0,11,0);
 worldStrings.Unlock();
}
void PopWorldDraw(PopWorld *p) {
 if(p->IsActive()) { if(p->dirty) {
 viewportClipLeft=gBriefDrawSource;
 long x=p->x,y=p->y,w=p->width,h=p->height;
 GuiRemapClipRect(&gMenuClip,x,y,w,h);
 PopDrawSurface(&g513888,1,0,p->x,p->y,p->width,p->height);
 PopWorldDrawText(); p->Invalidate(); p->dirty=0;
 } p->Draw(); }
}
int PopWorldEvent(SimGame *) { if(popWorld.IsActive()) { long n=popWorld.HitTest(&g4bb78c); if(n>=0) targetLimits=n+1300; return popWorld.Update(); } return 0; }
PopWorld::~PopWorld() { input=0; }
