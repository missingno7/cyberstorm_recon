// Unknown original TU: SIMALERT.BOX screen, 0044d150..0044d770.
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
struct GuiControlData:GuiRect {unsigned char style[4]; U32 attributes,shortcut;};
class GuiControl:public GuiControlData {public: virtual ~GuiControl(); virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned,U32); virtual int HitTest(const GuiPoint *);};
class SimMapGadget:public GuiControl {public:U32 state;long kind,data;SimMapGadget(U32,long,long,U32);virtual ~SimMapGadget();};
class SimImageGadget:public GuiControl {public:U32 state;long image,repeat;SimImageGadget(long,long,U32);virtual ~SimImageGadget();};
class MenuNumber:public GuiControl {public:U32 unused;const char *text;long value;signed char font,color,flash,align;MenuNumber(signed char,signed char);virtual ~MenuNumber();};
class MenuButton:public GuiControl {public:U32 state;const char *text;MenuButton(const char *,U32);virtual ~MenuButton();};
class SimValueGadget:public GuiControl {public:U32 state;long value;SimValueGadget(U32);void SetValue(long);};
class MenuSpacer:public GuiControl {public:MenuSpacer();virtual ~MenuSpacer();};
extern GuiControl *simAlertControls[];
struct SimAlert:GuiScreen {SimAlert();~SimAlert();};
SimMapGadget alertMap(16,6,0,0),alertMap2(16,7,0,0);
MenuNumber alertPilot(-1,18),alertUnit(-1,18);
SimImageGadget alertImage(127,1000,0),alertImage2(130,1000,0);
MenuButton alertButton(0,0),alertButton2(0,0);
SimValueGadget alertValue1(0),alertValue2(0),alertValue3(0),alertValue4(0);
MenuSpacer alertSpacers[3];
SimAlert g4d4094;
extern StrPool targetBriefResource;
extern GuiPoint g4bb78c;
extern long targetLimits,g5138b0,screenHeight;
SimAlert::SimAlert():GuiScreen(simAlertControls,16,0) {}
void SimAlertOpen(SimAlert *p,InputReq *input) {
 if(!p->IsActive()) {
  p->LoadLayout((long)"SIMALERT.BOX"); p->Open(input,&targetBriefResource,101,0);
  int x=0,y=g5138b0-480; if(screenHeight<500)y-=20;
  for(int i=4;i<15;i++) { GuiControl *c=simAlertControls[i]; c->x+=x; c->y+=y; }
 }
}
void SimAlertClose(SimAlert *p) {if(p->IsActive())p->input=0;}
void SimAlertUpdate(SimAlert *p) {if(p->IsActive()){targetLimits=p->HitTest(&g4bb78c);p->Update();}}
struct ViewRect {long left,top,right,bottom;};
extern ViewRect viewportClipLeft,gBriefDrawSource;
struct ActorRecord { unsigned char head[24]; void *unit,*pilot;const char *UnitText()const{return (const char *)unit;}const char *PilotText()const{return (const char *)pilot;} };
extern ActorRecord *g4d2498;
extern char g513888;
extern void SimAlertPrepare(void *,const GuiRect *);
extern void MenuDrawWrapped(const char *,long,long,long,long,const GuiRect *,int=0,int=0);
void SimAlertDraw(SimAlert *p) {
 if(p->IsActive()) {
  int visible=(alertPilot.attributes&0x1000000UL)==0; p->Draw();
  if(visible && g4d2498) {
   SimAlertPrepare(&g513888,&alertUnit);
   MenuDrawWrapped(g4d2498->PilotText(),-1,0,1,18,&alertUnit);
   SimAlertPrepare(&g513888,&alertPilot);
   MenuDrawWrapped((const char *)g4d2498->unit,-1,0,1,18,&alertPilot);
   viewportClipLeft=gBriefDrawSource;
  }
 }
}
void SimAlertSetValue(void *unused,int n) {
 alertValue1.SetValue(n==1);alertValue2.SetValue(n==2);alertValue3.SetValue(n==3);alertValue4.SetValue(n==4);
}
struct SimGame;
extern void PilotSetState(SimGame *,int);
extern int SimCycleFocus(SimGame *,int);
extern void SimFinishFocus(SimGame *),SimStartPhase(SimGame *);
struct SoundSettings;extern int SoundPlay(unsigned,const SoundSettings *);
extern int SoundTrigger(unsigned,int);
inline int SimAlertReadEvent(){if(g4d4094.IsActive()){targetLimits=g4d4094.HitTest(&g4bb78c);return g4d4094.Update();}return 0;}
void SimAlertEvent(SimGame *s) {
 int event=SimAlertReadEvent();
 switch(event) {
 case 5:SoundPlay(3,0);PilotSetState(s,5);break;
 case 7:SoundPlay(3,0);PilotSetState(s,6);break;
 case 9:SimCycleFocus(s,-1);break;
 case 10:SimCycleFocus(s,1);break;
 case 11:SoundPlay(6,0);SimFinishFocus(s);break;
 case 12:SoundPlay(6,0);SimStartPhase(s);break;
 case 1:SoundPlay(3,0);SoundTrigger(212,0);PilotSetState(s,1);break;
 case 2:SoundPlay(3,0);SoundTrigger(214,0);PilotSetState(s,2);break;
 case 4:SoundPlay(3,0);SoundTrigger(216,0);PilotSetState(s,4);break;
 case 3:SoundPlay(3,0);SoundTrigger(215,0);PilotSetState(s,3);break;
 }
}


SimAlert::~SimAlert() {input=0;}


