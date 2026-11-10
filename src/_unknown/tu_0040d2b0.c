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


/* Bioderm purchase/list screen, no surviving source path. Working names. */
long PilotValue(void *,int);
extern "C" int f_0043bd5f(void *);
extern "C" void f_0043bced(void *,int);
extern "C" void f_0043c167(void *);
extern "C" void f_00412068(Scene *);
extern byte biodermListVisible;
#pragma pack(push,1)
class BiodermListScreen:public GuiScreen {public:
 unsigned short state;ActorListObject *pilot;short slide;
 BiodermListScreen();~BiodermListScreen(){}
 void Open(InputReq *);void Close();int Update();int Room();void UpdateText();void UpdateCredits();void Buy();void Cancel();void Apply();void SetControls();void MessageDone();void Message(const char *);void DrawMessage(const char *);
};
#pragma pack(pop)
extern GuiControl *biodermListControls[];extern int biodermListHandle;
char biodermListPriceText[8];
MenuButton biodermListBuy(0,0),biodermListNext(0,0),biodermListPrevious(0,0),biodermListConfirm(0,0);
MenuHelpRegion biodermListHelp;MenuLabel biodermListPrice(0,1,1,11,1);MenuNumber biodermListCredits(-1,18);MenuPanel biodermListPanel;BiodermListScreen g4b01c8;
inline byte OtherListPanels(){return g4affbc&0xfd;}

BiodermListScreen::BiodermListScreen():GuiScreen(biodermListControls,8,0){}
void BiodermListScreen::Open(InputReq *request){if(!IsActive()){
 biodermListHandle=-1;LoadLayout((long)"biovat.box");GuiScreen::Open(request,&gShellStrings,1001,1);biodermListPrice.SetFont(1);biodermListCredits.Align(1);g4a706d=g4ae17f-1;UpdateText();slide=-170;state=4;SetControls();
}}
void BiodermListScreen::Close(){if(IsActive()){input=0;f_0041204b(&g4b16a4);}}
int BiodermListScreen::Update(){switch(state){case 3:case 5:
 biodermListHandle=HitTest(&g4bb78c);if(biodermListHandle<0){int room=Room();f_0040c63e(&g4aff6c,room,1);if(room>=0)biodermListHandle=room+100;}return GuiScreen::Update();default:return 0;}}
int BiodermListScreen::Room(){return BriefDone("bioroom");}
void ShellBiodermListRender(BiodermListScreen *s){switch(s->state){
case 3:
 if(ScreenChanged()){s->UpdateText();screenChanged=0;}
 MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x58),35,55);s->UpdateCredits();s->Invalidate();
case 5:s->Draw();break;
case 0:f_0043c167(gShellGame);f_0041170c(&g4b16a4);s->state=1;g4a706d=g4ae17f-1;break;
case 1:
 if(!OtherListPanels()){
  s->slide+=screenSlideStep;
  if(s->slide>35 || gGuiEffectPriority<50){s->slide=35;s->state=3;s->SetControls();g4affbc=2;s->UpdateText();f_004120fe(&g4b16a4,1);f_00412068(&g4b16a4);MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+100),0,0);}
  else{MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+100),0,0);MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x58),s->slide,55);}
 }break;
case 2:
 if(gGuiEffectPriority>50){s->slide-=screenSlideStep;if(s->slide < -170){f_0041204b(&g4b16a4);s->state=4;s->SetControls();g4affbc=0;biodermListVisible=0;}else{MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+100),0,0);MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+0x58),s->slide,55);}}
 else{f_0041204b(&g4b16a4);s->slide=-170;s->state=4;s->SetControls();g4affbc=0;biodermListVisible=0;MenuDrawImage(&g513888,(MenuImage *)(gScreenImages+100),0,0);}break;
case 4:break;
default:break;
}}
void BiodermListScreen::UpdateText(){
 if(!((ProfileManager *)gShellGame)->Count())biodermListPrice.SetText(" ");
 else{pilot=((ProfileManager *)gShellGame)->Select(-1);sprintf(biodermListPriceText,"%7d",PilotValue(pilot,0xcb));biodermListPrice.SetText(biodermListPriceText);}
 UpdateCredits();
}
void BiodermListScreen::UpdateCredits(){if(uiSystem.UpdateCredits())biodermListCredits.SetValue(g4a706d,0);}
void BiodermListScreen::Buy(){
 long cost=PilotValue(pilot,0xcb);
 if(!f_0043bd5f(gShellGame)){SoundTrigger(0x45,0);Message(gShellStrings.GetString(1008,1));}
 else if(ScreenCanAfford(cost)){
  int kind=PilotValue(pilot,0xca);f_0043bced(gShellGame,kind);f_0043d6b1(gShellGame,cost);UpdateText();g4affb8=1;SoundPlay(0x168,0);SoundTrigger(0,500);f_004120fe(&g4b16a4,1);SoundTrigger(0x42,0);
  if(kind<17 || kind>19)SoundTrigger(kind+0x120,0);
 }else{SoundTrigger(0x37,0);Message(gShellStrings.GetString(1009,1));}
}
void BiodermListScreen::Cancel(){if(g4affb8){f_0043ba3e(profileManager,gShellGame);g4affb8=0;}}
void BiodermListScreen::Apply(){if(g4affb8){if(f_0047723b(gShellStrings.GetString(3981,0))){g4affb8=1;f_0043ba3e(profileManager,gShellGame);g4affb8=0;}else{f_0043ba3e(gShellGame,profileManager);g4affb8=0;f_0041170c(&g4b16a4);SoundTrigger(0x2f,0);g4a706d=g4ae17f-1;}}}
void BiodermListScreen::SetControls(){switch(state){
 case 3:biodermListBuy.SetState(0);biodermListNext.SetState(0);biodermListPrevious.SetState(0);biodermListConfirm.SetState(5);break;
 case 1:case 2:case 4:default:biodermListBuy.SetState(5);biodermListNext.SetState(5);biodermListPrevious.SetState(5);biodermListConfirm.SetState(5);break;
 case 5:biodermListBuy.SetState(5);biodermListNext.SetState(5);biodermListPrevious.SetState(5);biodermListConfirm.SetState(0);break;
}}
void BiodermListScreen::MessageDone(){BriefBlit(&g513888,1,0,280,170,280,170,250,150);SurfacePresent(&g513888);g4affc2=2;state=3;SetControls();Invalidate();}
void BiodermListScreen::Message(const char *text){
 BriefBlit(&g513888,0,1,280,170,280,170,250,150);MenuSetClip(&gMenuClip,280,170,250,150);g4affc2=208;MenuDrawImage(&g513888,(MenuImage *)(gShellImages+0x31c),280,170);SurfacePresent(&g513888);state=5;SetControls();biodermListConfirm.Invalidate();DrawMessage(text);SurfacePresent(&g513888);
}
void BiodermListScreen::DrawMessage(const char *text){int x,y;BriefRender(300,190,210,120,text,0,-1,0,&x,&y,0,15);}


GuiControl *biodermListControls[8] = {
    (GuiControl *)&biodermListBuy,
    (GuiControl *)&biodermListPrevious,
    (GuiControl *)&biodermListNext,
    (GuiControl *)&biodermListConfirm,
    (GuiControl *)&biodermListCredits,
    (GuiControl *)&biodermListPrice,
    (GuiControl *)&biodermListPanel,
    (GuiControl *)&biodermListHelp
};
