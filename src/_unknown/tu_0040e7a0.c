inline const char *BiodermRoomName() { return "bioroom"; }
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


/* VR statistics screen. Names are inferred; layout is packed to the observed offsets. */
#pragma pack(push,1)
class BiodermStatsScreen:public GuiScreen {public:
 unsigned short state;ActorListObject *pilot;short slide;
 BiodermStatsScreen();~BiodermStatsScreen(){}
 void Open(InputReq *);void Close();void UpdateText();void SetControls();void UpdateCredits();int Room();void Message(const char *);void DrawMessage(const char *);
};
#pragma pack(pop)
extern GuiControl *statsControls[];extern int statsHandle;
char statsText0[8],statsText1[8],statsText2[8],statsText3[8],statsText4[8],statsText5[8],statsText6[8],statsText7[8];
MenuButton statsButtons[21];MenuButton statsConfirm(0,0);MenuHelpRegion statsHelp;
MenuLabel statsLabel0(0,1,1,11,1),statsLabel1(0,1,1,11,1),statsLabel2(0,1,1,11,1),statsLabel3(0,1,1,11,1),statsLabel4(0,1,1,11,1),statsLabel5(0,1,1,11,1),statsLabel6(0,1,1,11,1),statsLabel7(0,1,1,11,1);
MenuNumber statsCredits(-1,18);MenuPanel statsPanel;BiodermStatsScreen g4b07f8;
inline byte OtherStatsPanels(){return g4affbc&0xfb;}

BiodermStatsScreen::BiodermStatsScreen():GuiScreen(statsControls,22,0){}
void BiodermStatsScreen::Open(InputReq *request){if(!IsActive()){
 statsHandle=-1;
 statsLabel0.SetFont(1);statsLabel1.SetFont(1);statsLabel2.SetFont(1);statsLabel3.SetFont(1);statsLabel4.SetFont(1);statsLabel5.SetFont(1);statsLabel6.SetFont(1);statsLabel7.SetFont(1);
 statsCredits.Align(1);LoadLayout((long)"vrmenu.box");GuiScreen::Open(request,&gShellStrings,1401,1);g4a706d=g4ae17f-1;UpdateText();slide=-170;state=3;SetControls();
}}
void BiodermStatsScreen::Close(){if(IsActive())input=0;}
U32 BiodermStatsExit(BiodermStatsScreen *s){
 switch(s->state){case 2:case 4:
 statsHandle=s->HitTest(&g4bb78c);
 if(statsHandle<0){int room=BriefDone(BiodermRoomName());f_0040c63e(&g4aff6c,room,1);if(room>=0)statsHandle=room+100;}
 return s->Update();default:return 0;}
}
void ShellBiodermStatsRender(BiodermStatsScreen *s){switch(s->state){
case 2:
 if(ScreenChanged()){s->UpdateText();screenChanged=0;}
 MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x28),17,37);s->UpdateCredits();s->Invalidate();
case 4:s->Draw();break;
case 0:
 if(!OtherStatsPanels()){
  s->slide+=screenSlideStep;
  if(s->slide>17 || gGuiEffectPriority<50){s->slide=17;s->state=2;s->SetControls();g4affbc=4;s->UpdateText();f_004120fe(&g4b16a4,1);f_0041207d(&g4b16a4);MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x34),0,0);}
  else{MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x34),0,0);MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x28),s->slide,37);}
 }break;
case 1:
 if(gGuiEffectPriority>50){s->slide-=screenSlideStep;if(s->slide < -170){s->state=3;s->SetControls();g4affbc=0;}else{MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x34),0,0);MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x28),s->slide,37);}}
 else{s->slide=-170;s->state=3;s->SetControls();g4affbc=0;MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x34),0,0);}break;
case 3:break;
default:break;
}}
void BiodermStatsScreen::UpdateText(){
 if(!((ProfileManager *)gShellGame)->Count()){
 pilot=0;statsLabel0.SetText(" ");statsLabel1.SetText(" ");statsLabel2.SetText(" ");statsLabel3.SetText(" ");statsLabel4.SetText(" ");statsLabel5.SetText(" ");statsLabel6.SetText(" ");statsLabel7.SetText(" ");
 }else{
 pilot=((ProfileManager *)gShellGame)->Select(-1);
 sprintf(statsText0,"%6d",SendEvent(pilot,0x71,(void *)6));sprintf(statsText1,"%6d",SendEvent(pilot,0x71,(void *)0));
 sprintf(statsText2,"%6d",SendEvent(pilot,0x71,(void *)1));sprintf(statsText3,"%6d",SendEvent(pilot,0x71,(void *)2));
 sprintf(statsText4,"%6d",SendEvent(pilot,0x71,(void *)3));sprintf(statsText5,"%6d",SendEvent(pilot,0x71,(void *)4));
 sprintf(statsText6,"%6d",SendEvent(pilot,0x71,(void *)5));sprintf(statsText7,"%6d",SendEvent(pilot,0x71,(void *)7));
 statsLabel0.SetText(statsText0);statsLabel1.SetText(statsText1);statsLabel2.SetText(statsText2);statsLabel3.SetText(statsText3);statsLabel4.SetText(statsText4);statsLabel5.SetText(statsText5);statsLabel6.SetText(statsText6);statsLabel7.SetText(statsText7);
 }UpdateCredits();
}
void BiodermStatsScreen::SetControls(){int i;switch(state){
case 2:for(i=0;i<10;i++)statsButtons[i].SetState(0);statsConfirm.SetState(5);break;
case 0:case 1:case 3:for(i=0;i<10;i++)statsButtons[i].SetState(5);statsConfirm.SetState(5);break;
case 4:for(i=0;i<10;i++)statsButtons[i].SetState(5);statsConfirm.SetState(0);break;
default:break;
}}
void BiodermStatsScreen::UpdateCredits(){if(uiSystem.UpdateCredits())statsCredits.SetValue(g4a706d,0);}
int BiodermStatsScreen::Room(){return BriefDone(BiodermRoomName());}
void BiodermStatsSelect(BiodermStatsScreen *s,unsigned skill){
 if(s->pilot){U32 cost=SendEvent(s->pilot,0x71,(void *)skill);if(cost>0){
 if(ScreenCanAfford(cost)){
 g4affb8=1;SendEvent(s->pilot,0x72,(void *)skill);f_0043d6b1(gShellGame,cost);s->UpdateText();f_0041170c(&g4b16a4);SoundClearContext();SoundTrigger(0,500);SoundPlay(0x114,0);
 switch(skill){case 6:SoundTrigger(0x4f,0);break;case 0:SoundTrigger(0x50,0);break;case 1:SoundTrigger(0x51,0);break;case 2:SoundTrigger(0x52,0);break;case 3:SoundTrigger(0x53,0);break;case 4:SoundTrigger(0x5c,0);break;case 5:SoundTrigger(0x5d,0);break;case 7:SoundTrigger(0x5e,0);break;}
 }else{s->Message(gShellStrings.GetString(1422,1));}
 }}
}
void BiodermStatsAction(BiodermStatsScreen *s){BriefBlit(&g513888,1,0,280,170,280,170,250,150);SurfacePresent(&g513888);g4affc2=32;s->state=2;s->SetControls();s->Invalidate();}
void BiodermStatsScreen::Message(const char *text){
 BriefBlit(&g513888,0,1,280,170,280,170,250,150);MenuSetClip(&gMenuClip,280,170,250,150);g4affc2=208;
 MenuDrawImage(&g513888,(MenuImage *)(gShellImages+0x31c),280,170);SurfacePresent(&g513888);state=4;SetControls();statsConfirm.Invalidate();DrawMessage(text);SurfacePresent(&g513888);
}
void BiodermStatsScreen::DrawMessage(const char *text){int x,y;BriefRender(300,190,210,120,text,0,-1,0,&x,&y,0,15);}
void BiodermStatsClose(BiodermStatsScreen *){if(g4affb8){f_0043ba3e(profileManager,gShellGame);g4affb8=0;}}
void BiodermStatsApply(BiodermStatsScreen *){if(g4affb8){if(f_0047723b(gShellStrings.GetString(3981,0))){g4affb8=1;f_0043ba3e(profileManager,gShellGame);g4affb8=0;}else{f_0043ba3e(gShellGame,profileManager);g4affb8=0;f_0041170c(&g4b16a4);g4a706d=g4ae17f-1;}}}


