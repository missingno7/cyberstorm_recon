inline const char *MedlabRoomName() { return "bioroom"; }
#include <stdio.h>
typedef unsigned long U32;
typedef unsigned char byte;
struct InputReq; struct GuiPoint { long x,y; }; struct GuiRect {long x,y,width,height;};
struct MenuImage {U32 offset;unsigned short width,height;U32 flags;};
struct StrPool {char *GetString(long,int=0);};
struct ActorListObject; struct ProfileManager {ActorListObject *Select(int=-1);int Count();};
struct GameSession; struct Scene;
struct Shell {int UpdateCredits();};
struct SoundSettings; int SoundPlay(unsigned,const SoundSettings *);int SoundTrigger(unsigned,int=0);void SoundClearContext();
long SendEvent(void *,int,void * =0,int=0,int=0);
extern "C" int f_0047723b(const char *);
extern "C" void f_0043ba3e(void *,void *);
extern "C" void f_0043befa(void *,int);
extern "C" void f_0043bd78(void *,int);
extern "C" void f_0043d6b1(void *,int);
extern "C" int BriefDone(const char *);
extern "C" void f_0040c63e(Scene *,int,int);
extern "C" void f_0041170c(Scene *);
extern "C" void f_0041204b(Scene *);
extern "C" void f_0041207d(Scene *);
extern "C" void f_004120e2(Scene *);
extern "C" void f_004120fe(Scene *,int);
extern "C" void BriefBlit(void *,int,int,int,int,int,int,int,int);
extern "C" void BriefRender(int,int,int,int,const char *,int,int,int,int *,int *,int,int,int=1,int=0,int=0,int=0x7fffffff);
void MenuDrawImage(void *,const MenuImage *,long,long);void SurfacePresent(void *);void MenuSetClip(GuiRect *,long,long,long,long);
/* The destination-buffer overload is inferred from this screen's calls. */
void MenuFormatNumber(long,char *,long);
extern StrPool gShellStrings;extern char gShellGame[],profileManager[];extern Shell uiSystem;
extern long g4ae17f;extern int g4a706d;
inline int ScreenCanAfford(long cost){return cost<=g4ae17f;}
extern int screenChanged, g4affb8,g4affc2;extern byte g4affbc;extern long gGuiEffectPriority;extern byte screenSlideStep;
inline int ScreenChanged(){return screenChanged;}
inline byte OtherPanels(){return g4affbc&0xfe;}
extern GuiPoint g4bb78c;extern int g513888;extern GuiRect gMenuClip;extern byte *gShellImages,*gScreenImages;
extern Scene g4aff6c,g4b16a4;
class GuiControl;
#pragma pack(push,1)
class GuiScreen {public:byte header[16];InputReq *input;byte rest[56];
 GuiScreen(GuiControl **,unsigned,U32);~GuiScreen(){input=0;}
 int IsActive()const{return input!=0;}
 void LoadLayout(long);void Open(InputReq *,StrPool *,long,int);int Update();void Draw();void Invalidate();long HitTest(const GuiPoint *);
};
struct GuiControlData:GuiRect{byte style[4];U32 attributes,shortcut;};
class GuiControl:public GuiControlData {public:
 virtual ~GuiControl();virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);virtual int HitTest(const GuiPoint *);
 void SetState(U32 n){attributes=(attributes&0xff1fffffUL)+(n<<21);}void Invalidate(){attributes&=~0x1000000UL;}
};
class MenuButton:public GuiControl {public:U32 unused;const char *text;MenuButton(const char * =0,U32=0);virtual ~MenuButton();virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);};
class MenuHelpRegion:public GuiControl {public:long id;int activated;MenuHelpRegion();virtual ~MenuHelpRegion();virtual void Draw(unsigned);virtual int Event(unsigned,U32);};
class MenuLabel:public GuiControl {public:const char *text;long align;byte font;int background;long color;MenuLabel(const char *,long,byte,long,int);virtual ~MenuLabel();virtual void Reset(unsigned);virtual void Draw(unsigned);void SetFont(byte);void SetText(const char *);};
class MenuNumber:public GuiControl {public:U32 unused;const char *text;long value;signed char font,color,flash,align;void Align(int n){align=n;}MenuNumber(signed char,signed char);virtual ~MenuNumber();virtual void Reset(unsigned);virtual void Draw(unsigned);void SetValue(long,signed char);};
class MenuPanel:public GuiControl {public:const char *text;MenuPanel();virtual ~MenuPanel();virtual void Draw(unsigned);};
#pragma pack(pop)


/* Names and packed unknown fields are working reconstruction labels. */
#pragma pack(push,1)
class MedlabScreen:public GuiScreen {public:
 unsigned short state;short slide;byte unknown50[4];ActorListObject *pilot;int audible;
 MedlabScreen();~MedlabScreen(){}
 void Open(InputReq *);void Close();int Update();void Render();void Invalidate();void UpdateText();void UpdateCredits();int Room();
 void Cancel();void Apply();void Treat(unsigned,unsigned);int TotalCost();void TreatAll();void Dismiss();
};
#pragma pack(pop)
extern GuiControl *medlabControls[];extern int medlabHandle;
extern MenuLabel medlabHealth,medlabTraining,medlabRepair,medlabTotal,medlabValue;
extern MenuNumber medlabCredits;extern MenuPanel medlabPanel;extern char medlabCreditText[16];
char medlabCreditText[16];
char medlabHealthText[12],medlabTrainingText[12],medlabTotalText[12],medlabRepairText[12],medlabValueText[12];
MenuButton medlabButtons[14];
MenuHelpRegion medlabHelp;
MenuLabel medlabHealth(0,1,1,11,1),medlabTraining(0,1,1,11,1),medlabRepair(0,1,1,11,1),medlabTotal(0,1,1,11,1),medlabValue(0,1,1,11,1);
MenuNumber medlabCredits(-1,18);
MenuPanel medlabPanel;
MedlabScreen g4b0c50;

MedlabScreen::MedlabScreen():GuiScreen(medlabControls,15,0){}
void MedlabScreen::Open(InputReq *request){if(!IsActive()){
 medlabHandle=-1;medlabHealth.SetFont(1);medlabTraining.SetFont(1);medlabRepair.SetFont(1);medlabTotal.SetFont(1);medlabValue.SetFont(1);
 medlabCredits.Align(1);LoadLayout((long)"medlab.box");GuiScreen::Open(request,&gShellStrings,1201,1);
 pilot=((ProfileManager *)gShellGame)->Select(-1);TotalCost();g4a706d=g4ae17f-1;UpdateText();slide=-170;state=3;audible=1;
}}
void MedlabScreen::Close(){if(IsActive())input=0;}
int MedlabScreen::Update(){
 if(state!=2)return 0;
 medlabHandle=HitTest(&g4bb78c);
 if(medlabHandle<0){int room=BriefDone(MedlabRoomName());f_0040c63e(&g4aff6c,room,1);if(room>=0)medlabHandle=room+100;}
 return GuiScreen::Update();
}
void ShellBiodermSkillsRender(MedlabScreen *s){
 switch(s->state){
 case 2:
  if(ScreenChanged()){s->UpdateText();screenChanged=0;}
  MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x40),35,37);
  s->UpdateCredits();s->GuiScreen::Invalidate();s->Draw();break;
 case 0:
  if(!OtherPanels()){
   s->slide+=screenSlideStep;
   if(s->slide>35 || gGuiEffectPriority<50){s->slide=35;s->state=2;g4affbc=1;s->UpdateText();f_004120fe(&g4b16a4,1);f_0041207d(&g4b16a4);MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x4c),0,0);}
   else {MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x4c),0,0);MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x40),s->slide,37);}
  }break;
 case 1:
  if(gGuiEffectPriority>50){s->slide-=screenSlideStep;if(s->slide < -170){s->state=3;g4affbc=0;}else{MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x4c),0,0);MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x40),s->slide,37);}}
  else{s->slide=-170;s->state=3;g4affbc=0;MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x4c),0,0);}break;
 case 3:break;
default:break;
 }
}
void MedlabScreen::Invalidate(){GuiScreen::Invalidate();}
extern char medlabHealthText[12],medlabTrainingText[12],medlabTotalText[12],medlabRepairText[12],medlabValueText[12];
void MedlabScreen::UpdateText(){
 if(!((ProfileManager *)gShellGame)->Count()){
  medlabHealth.SetText(" ");medlabTraining.SetText(" ");medlabTotal.SetText(" ");medlabRepair.SetText(" ");medlabValue.SetText(" ");
 }else{
  pilot=((ProfileManager *)gShellGame)->Select(-1);if(!pilot)return;
  sprintf(medlabHealthText,"%9d",SendEvent(pilot,0x73));sprintf(medlabTrainingText,"%9d",SendEvent(pilot,0x77));
  sprintf(medlabValueText,"%9d",SendEvent(pilot,0x79));sprintf(medlabRepairText,"%9d",SendEvent(pilot,0x75));sprintf(medlabTotalText,"%9d",TotalCost());
  medlabHealth.SetText(medlabHealthText);medlabTraining.SetText(medlabTrainingText);medlabTotal.SetText(medlabTotalText);medlabRepair.SetText(medlabRepairText);medlabValue.SetText(medlabValueText);
 }UpdateCredits();
}
void MedlabScreen::UpdateCredits(){if(uiSystem.UpdateCredits()){MenuFormatNumber(g4a706d,medlabCreditText,12);medlabCredits.SetValue(g4a706d,0);}}
int MedlabScreen::Room(){return BriefDone(MedlabRoomName());}
void MedlabScreen::Cancel(){if(g4affb8){g4affb8=0;f_0043ba3e(profileManager,gShellGame);}}
void MedlabScreen::Apply(){
 SoundTrigger(47,0);
 if(g4affb8){
  if(f_0047723b(gShellStrings.GetString(3981,0))){g4affb8=0;f_0043ba3e(profileManager,gShellGame);}
  else {g4affb8=0;f_0043ba3e(gShellGame,profileManager);f_0041170c(&g4b16a4);g4a706d=g4ae17f-1;}
 }
}
void MedlabScreen::Treat(unsigned costEvent,unsigned actionEvent){
 if(pilot){U32 cost=SendEvent(pilot,costEvent);if(cost>0 && ScreenCanAfford(cost)){
  SendEvent(pilot,actionEvent);f_0043d6b1(gShellGame,cost);UpdateText();f_0041170c(&g4b16a4);g4affb8=1;
  if(audible){switch(actionEvent){case 0x74:SoundPlay(0xee,0);SoundTrigger(0,500);SoundTrigger(0x3c,0);break;case 0x78:SoundPlay(0x161,0);SoundTrigger(0,500);SoundTrigger(0x3d,0);break;case 0x76:SoundPlay(0x195,0);SoundTrigger(0,500);SoundTrigger(0x3e,0);break;}}
 }}
}
int MedlabScreen::TotalCost(){
 int total=0;int count=((ProfileManager *)gShellGame)->Count();
 if(count){for(int i=0;i<count;i++){
  total+=SendEvent(pilot,0x73);total+=SendEvent(pilot,0x77);total+=SendEvent(pilot,0x75);
  f_0043befa(gShellGame,1);pilot=((ProfileManager *)gShellGame)->Select(-1);
 }return total;}return 0;
}
void MedlabScreen::TreatAll(){
 int total=TotalCost();int count=((ProfileManager *)gShellGame)->Count();
 if(ScreenCanAfford(total)){
 if(((ProfileManager *)gShellGame)->Count()>0){SoundPlay(0xed,0);SoundTrigger(0x3f,0);}
 audible=0;
 for(int i=0;i<count;i++) {Treat(0x73,0x74);Treat(0x77,0x78);Treat(0x75,0x76);f_0043befa(gShellGame,1);pilot=((ProfileManager *)gShellGame)->Select(-1);}
 audible=1;UpdateText();f_0041170c(&g4b16a4);g4affb8=1;
 }else{SoundTrigger(0x36,0);}
}
void MedlabScreen::Dismiss(){
 if(pilot){SoundTrigger(0x40,0);g4affb8=1;SendEvent(pilot,0x7f);g4ae17f+=SendEvent(pilot,0x79);f_0043bd78(gShellGame,-1);
  if(((ProfileManager *)gShellGame)->Count()>0){UpdateText();f_004120fe(&g4b16a4,1);}else{pilot=0;UpdateText();f_004120e2(&g4b16a4);}f_0041170c(&g4b16a4);
 }
}




static char medlabSourceFile[] = __FILE__;
GuiControl *medlabControls[15] = {
    (GuiControl *)&medlabButtons[0],
    (GuiControl *)&medlabButtons[1],
    (GuiControl *)&medlabButtons[2],
    (GuiControl *)&medlabButtons[3],
    (GuiControl *)&medlabButtons[4],
    (GuiControl *)&medlabButtons[5],
    (GuiControl *)&medlabButtons[6],
    (GuiControl *)&medlabHealth,
    (GuiControl *)&medlabRepair,
    (GuiControl *)&medlabTraining,
    (GuiControl *)&medlabTotal,
    (GuiControl *)&medlabValue,
    (GuiControl *)&medlabCredits,
    (GuiControl *)&medlabPanel,
    (GuiControl *)&medlabHelp
};
