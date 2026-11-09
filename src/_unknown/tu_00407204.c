/* Shared working GUI names and layouts, derived from accepted GUI candidates.
 * Unknown fields stay explicit; no original path survives in these objects. */
#include <stdio.h>
#include <string.h>
typedef unsigned long U32;
typedef unsigned char byte;
struct GuiRect { long x,y,width,height; };
struct GuiPoint { long x,y; };
struct InputReq; struct Palette; struct Surface;
struct Timer; void TimerSet(Timer *,U32); int TimerExpired(Timer *);
struct Timer {U32 start,end,duration; Timer(U32 n){TimerSet(this,n);} };
struct StrPool {char *GetString(long,int=1);};
struct GuiControlData:GuiRect {byte style[4];U32 attributes,shortcut;};
class GuiControl:public GuiControlData {
public:
    GuiControl(U32,U32); virtual ~GuiControl(); virtual void Reset(unsigned);
    virtual void Draw(unsigned); virtual int Event(unsigned,U32); virtual int HitTest(const GuiPoint *);
    void SetVisibility(U32 n){attributes=(attributes&~0xe00000UL)+(n<<21);}
    void Invalidate(){attributes&=~0x1000000UL;}
};
#pragma pack(push,1)
class MenuLabel:public GuiControl {
public:
    const char *text;long align;byte font;int background;long color;
    MenuLabel(const char * =0,long=-1,byte=1,long=11,int=1);virtual ~MenuLabel(){};
    virtual void Reset(unsigned);virtual void Draw(unsigned);
    void SetFont(byte);void SetText(const char *);void SetColor(long);void SetAlign(long);void SetBackground(int);
};
class MenuHelpRegion:public GuiControl {
public: long id;int activated;MenuHelpRegion();virtual ~MenuHelpRegion(){};
    virtual void Draw(unsigned);virtual int Event(unsigned,U32);
};
class MenuButton:public GuiControl {
public: U32 state;const char *text;MenuButton(const char * =0,U32=0);virtual ~MenuButton(){};
    virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);
};
class MenuPrefixButton:public GuiControl {
public:
    const char *text;U32 state;signed char font,color,align,prefix;
    MenuPrefixButton(const char * =0,signed char=1,signed char=2,signed char=-1);
    virtual ~MenuPrefixButton(){};virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);
    void SetText(const char *);void SetColor(signed char);void SetAlign(signed char);void SetPrefix(signed char);void SetFont(signed char);
};
class MenuPanel:public GuiControl {
public:const char *text;MenuPanel();virtual ~MenuPanel(){};virtual void Draw(unsigned);
};
class MenuNumber:public GuiControl {
public:U32 unused;const char *text;long value;signed char font,color,flash,align;
    MenuNumber(signed char=-1,signed char=18);virtual ~MenuNumber(){};
    virtual void Reset(unsigned);virtual void Draw(unsigned);void SetValue(long,signed char);void SetFont(signed char);
};
class GuiScreen:public GuiRect {
public:
    InputReq *input;byte unknown14[56];
    GuiScreen(GuiControl **,unsigned,U32);~GuiScreen(){input=0;}
    int IsActive() const{return input!=0;} void LoadLayout(long);void Open(InputReq *,StrPool *,long,int);
    void Invalidate();void ResetControls();long HitTest(const GuiPoint *);int Update();void Draw();
};
struct FileRecord {U32 handle,open,rest[5];U32 IsOpen(){return open;}};
struct FlicSound {unsigned short id,frame;byte volume,pan,pitch,flags;};
struct Flic {byte header[32];FileRecord file;Palette *palette;U32 fields40[3];long loops;byte tail[64];
    Flic();~Flic();int Open(long,int=100,int=0);void Close();U32 Update(Surface *);void SetSounds(FlicSound *);
    long CurrentFrame(){return fields40[1];} int IsOpen(){return file.IsOpen();}void SetLoops(long n){loops=n;}Palette *GetPalette(){return palette;}
};
struct RangeCounter {
    unsigned short value,first,last,step;
    RangeCounter();~RangeCounter();void Set(unsigned short);void Increment();
    void SetBounds(unsigned short lo,unsigned short hi){last=hi;first=lo;}
    unsigned short Get(){return value;}
};
#pragma pack(pop)
extern StrPool gShellStrings;
extern int g4b74f4,g513888;
extern Surface g511948;
extern GuiPoint g4bb78c;
extern void ShellClearFrame();extern void GuiFadeOut();extern void ShellLoadPalette(const char *);extern void ShellApplyPalette(Palette *);
extern void SoundFlush(int);extern void SoundStop(unsigned);extern void SoundClearContext();extern int SoundTrigger(unsigned,int=0);
extern "C" void BriefWaitA(int);extern "C" void BriefWaitB(int);extern "C" int BriefDone(const char *);
extern "C" void HercSetMissionStart(void *,int);extern "C" void HercSetMission(void *,int);
extern "C" void BriefBlit(void *,int,int,int,int,int,int,int,int);
extern void SurfacePresent(void *);
extern U32 TimeNow();
extern unsigned MissionRandomTable;
extern void f_0047e200(int *);extern "C" unsigned next_link_seed(void *);
inline unsigned MenuRandom(unsigned maximum){return next_link_seed(&MissionRandomTable)%maximum;}

/* Main menu: one plain-oe object, including the generated destructors after it.
 * Packed offsets are observed. Unknown fields are deliberately named by offset. */
struct InputReq {U32 mask,state;InputReq(U32 n):mask(n){}};
struct MenuImage {U32 offset;unsigned short width,height;U32 flags;};
struct MenuImages {U32 count;MenuImage images[1];MenuImage *Image(int i){return &images[i];}};
struct MissionChoice {long id,type;byte unknown08[24];const char *name;byte unknown24[8];};
struct CommsScreen;extern CommsScreen g4b2e30;
char *CommsMissionTypeName(CommsScreen *,byte);
struct ProfileManager {int Count();};extern ProfileManager profileManager;
extern "C" MissionChoice *BriefLookup(void *,int);extern int HasMission(void *);
extern int MainMenuMissionReady(void *);extern short mainSelectedMission;
extern int g4d030f;
struct ResMgr {void *Load(long,int=1,int=0,int=0);};extern ResMgr gResMgr;
extern MenuImages *gShellImages;
extern char gShellBackground,gMenuSavedSurface;
extern void MenuLoadBackground(const char *,void *);extern void SurfaceCopyRegion(void *,void *,long,long);
extern void MenuDrawImage(void *,const MenuImage *,long,long);
extern void ReleasePoolData(void *);
extern "C" char *BriefFormat(const char *,...);
extern void MainMenuAudioReset();extern void f_0041cab0(void *);
extern "C" void LogicCallMode(int,void *);
extern "C" void BriefRender(int,int,int,int,const char *,int,int,int,int *,int *,int,int,int=1,int=0,int=0,int=0x7fffffff);
struct ViewRect {long left,top,right,bottom;};
extern ViewRect viewportClipLeft,gBriefDrawSource;
extern GuiRect gMenuClip;extern void GuiResetClip(void *);
extern int mainMenuLastHover;
#pragma pack(push,1)
class MainMenuScreen:public GuiScreen {
public:
    int field4c,ready;InputReq request;int intro,field60;MenuImages *images;byte unavailable;
    int force,missionFlag;Timer blinkTimer;int canStart;byte blink;Flic animation;
    int playing,field116;MissionChoice mission;int modal,selection;Flic ambient[17];
    MainMenuScreen();~MainMenuScreen(){}
    void Open(InputReq *,int);void CloseResources();void Close();void SetText();void Dismiss();
    void ShowExit();void ForceStart();void ShowRestart();int Start();void DrawMessage(const char *);
    void Highlight(int,int);void Update();void Render();int Hover();
};
#pragma pack(pop)
extern GuiControl *mainMenuControls[];
extern MenuPrefixButton mainMenuButtons[11];
extern MenuButton mainButton0,mainButton1,mainButton2,mainButton3,mainButton4,mainButton5;
extern MenuHelpRegion mainHelp;
extern MenuLabel mainLabel0,mainLabel1,mainLabel2,mainLabel3,mainLabel4,mainLabel5,mainLabel6,mainLabel7,mainLabel8,mainLabel9,mainLabel10;
extern MenuPanel mainPanel;extern MenuNumber mainNumber;extern MainMenuScreen g4aebe0;
extern int mainMenuStatus;
extern char mainCredits[8],mainHercCount[8],mainMissionCount[8],mainTitle[32],mainMissionType[32],mainMissionName[32];

MainMenuScreen::MainMenuScreen():GuiScreen(mainMenuControls,24,0),request(0x8004),blinkTimer(0),animation(){}
void MainMenuScreen::Open(InputReq *req,int showIntro){
    if(!IsActive()){
        request=*req;intro=showIntro;selection=0;ShellClearFrame();mainMenuStatus=-1;
        images=(MenuImages *)gResMgr.Load((long)"BASELITE.BMX");BriefWaitB(11);
        LoadLayout((long)"mainmenu.box");GuiScreen::Open(req,&gShellStrings,601,1);
        SetText();BriefWaitA(2);MainMenuAudioReset();
        if(intro){
            MenuLoadBackground("BGBASE.BMX",&gShellBackground);animation.Open((long)"drop1.flx");animation.SetLoops(1);
            LogicCallMode(12,0);ShellLoadPalette("mainmenu.plx");ready=0;playing=1;
        }else{
            ShellLoadPalette("mainmenu.plx");MenuLoadBackground("BGBASE.BMX",&gShellBackground);
            SurfaceCopyRegion(&gMenuSavedSurface,&g511948,0,0);ready=1;playing=0;
        }
        missionFlag=0;force=0;
        mainButton0.SetVisibility(5);mainButton1.SetVisibility(5);mainButton2.SetVisibility(5);
        mainButton5.SetVisibility(5);mainButton3.SetVisibility(5);mainButton4.SetVisibility(5);
        modal=0;
        for(int i=0;i<17;++i)ambient[i].Open((long)BriefFormat("HB_AMB%d.FLX",i+1),100,1);
        SoundTrigger(49,500);if(ready)HercSetMissionStart(&g4b74f4,2);
        TimerSet(&blinkTimer,1000);unavailable=15;
        mainLabel5.SetText(gShellStrings.GetString(619));mainLabel6.SetText(gShellStrings.GetString(620));mainLabel7.SetText(gShellStrings.GetString(621));
        mainLabel5.SetBackground(0);mainLabel6.SetBackground(0);mainLabel7.SetBackground(0);
        mainLabel3.SetBackground(0);mainLabel4.SetBackground(0);mainLabel8.SetBackground(0);
        mainLabel9.SetBackground(0);mainLabel10.SetBackground(0);mainLabel1.SetBackground(0);mainLabel0.SetBackground(0);
        mainNumber.SetFont(0);
        mainLabel5.SetColor(11);mainLabel6.SetColor(11);mainLabel7.SetColor(11);mainLabel0.SetColor(11);
        for(i=0;i<11;++i)mainMenuButtons[i].SetAlign(2);
    }
}
void MainMenuScreen::CloseResources(){
    if(images)ReleasePoolData(images);images=0;
    for(int i=0;i<17;++i)ambient[i].Close();animation.Close();input=0;
}
void MainMenuScreen::Close(){
    if(IsActive()){CloseResources();SoundStop(12);GuiFadeOut();ShellClearFrame();MainMenuAudioReset();f_0041cab0(&g4b74f4);}
}
void MainMenuScreen::SetText(){
    sprintf(mainTitle,"%s",gShellStrings.GetString(614));mainLabel0.SetText(mainTitle);
    int selected=mainSelectedMission;
    if(selected>=0){
        unavailable&=~8;mission=*BriefLookup(&profileManager,selected);mainLabel1.SetColor(2);
        sprintf(mainMissionType,"%s",CommsMissionTypeName(&g4b2e30,(byte)mission.type));
        sprintf(mainMissionName,"%s",mission.name);
        if(mission.type==1)missionFlag=1;else missionFlag=0;
    }else{
        sprintf(mainMissionType,"%s",gShellStrings.GetString(632));mainLabel1.SetColor(4);
        sprintf(mainMissionName," ");unavailable|=8;
    }
    mainLabel1.SetText(mainMissionType);mainLabel2.SetText(mainMissionName);
    int credits=g4d030f;sprintf(mainCredits,"%3d",credits);mainLabel8.SetText(mainCredits);
    if(!credits){mainLabel8.SetColor(4);unavailable|=1;}else{mainLabel8.SetColor(2);unavailable&=~1;}
    sprintf(mainHercCount,"%3d",profileManager.Count());mainLabel9.SetText(mainHercCount);
    if(!profileManager.Count()){mainLabel9.SetColor(4);unavailable|=2;}else{mainLabel9.SetColor(2);unavailable&=~2;}
    sprintf(mainMissionCount,"%3d",HasMission(&profileManager));mainLabel10.SetText(mainMissionCount);
    if(!HasMission(&profileManager)){mainLabel10.SetColor(4);unavailable|=4;}else{mainLabel10.SetColor(2);unavailable&=~4;}
    if(unavailable==0){mainLabel4.SetColor(2);mainLabel4.SetText(gShellStrings.GetString(633));canStart=1;}
    else{mainLabel4.SetColor(4);mainLabel4.SetText(gShellStrings.GetString(634));canStart=0;}
}
void MainMenuScreen::Dismiss(){
    modal=0;MenuLoadBackground("BGBASE.BMX",&gShellBackground);SurfacePresent(&g513888);
    for(int i=0;i<6;++i)mainMenuButtons[i].SetVisibility(0);
    mainButton0.SetVisibility(5);mainButton1.SetVisibility(5);mainButton2.SetVisibility(5);
    mainButton5.SetVisibility(5);mainButton3.SetVisibility(5);mainButton4.SetVisibility(5);
    Invalidate();
}
void MainMenuScreen::ShowExit(){
    modal=1;MenuDrawImage(&g513888,&gShellImages->images[66],200,140);
    for(int i=0;i<6;++i)mainMenuButtons[i].SetVisibility(5);
    mainButton3.attributes&=~0x1e00000UL;mainButton4.attributes&=~0x1e00000UL;mainButton5.attributes&=~0x1e00000UL;
    playing=0;DrawMessage(gShellStrings.GetString(626));SurfacePresent(&g513888);
}
void MainMenuScreen::ForceStart(){force=1;}
void MainMenuScreen::ShowRestart(){
    modal=1;MenuDrawImage(&g513888,&gShellImages->images[66],200,140);
    for(int i=0;i<6;++i)mainMenuButtons[i].SetVisibility(5);
    mainButton1.attributes&=~0x1e00000UL;mainButton2.attributes&=~0x1e00000UL;
    playing=0;DrawMessage(gShellStrings.GetString(625));SurfacePresent(&g513888);
}
int MainMenuScreen::Start(){
    SetText();
    if(canStart){
if(!force&&missionFlag&&!MainMenuMissionReady(&profileManager)){ShowRestart();return 0;}SoundTrigger(145,0);CloseResources();if(!ready){GuiFadeOut();ShellClearFrame();ready=0;}return 1;
    }else{
        modal=1;MenuDrawImage(&g513888,&gShellImages->images[66],200,140);
        for(int i=0;i<6;++i)mainMenuButtons[i].SetVisibility(5);
        mainButton0.attributes&=~0x1e00000UL;playing=0;
        byte noCredits=unavailable&1,noHercs=unavailable&2,noMission=unavailable&4,noSelection=unavailable&8;
        if(noCredits)DrawMessage(gShellStrings.GetString(628));
        else if(noHercs)DrawMessage(gShellStrings.GetString(629));
        else if(noMission)DrawMessage(gShellStrings.GetString(631));
        else if(noSelection){DrawMessage(gShellStrings.GetString(630));SoundTrigger(144,0);}
        SurfacePresent(&g513888);
    }
    return 0;
}
void MainMenuScreen::DrawMessage(const char *text){int a,b;BriefRender(225,150,200,100,text,0,-1,0,&a,&b,0,15);}
void MainMenuScreen::Highlight(int selected,int mouse){
    if(!modal&&images&&selected!=mainMenuLastHover){
        U32 mask=~0UL;
        if(!ready){if(animation.CurrentFrame()<50)mask=~4UL;mask&=~8UL;}
        GuiResetClip(&gMenuClip);viewportClipLeft=gBriefDrawSource;
        for(int i=0;i<4;++i){
            if(mask&(1UL<<i)){
                int image=i*2+(i==selected);
                MenuDrawImage(&g513888,&images->images[image],0,0);
                if(i==3){int image=8+(i==selected);MenuDrawImage(&g513888,&images->images[image],0,0);}
            }
        }
        mainMenuLastHover=selected;
    }
}
void MainMenuScreen::Update(){
    if(IsActive()){
        long result=HitTest(&g4bb78c);mainMenuStatus=result;
        if(!modal)for(int i=0;i<11;++i)mainMenuButtons[i].SetVisibility(0);
        if(result<0){
            int selected=Hover();Highlight(selected,1);
            if(selected>=0){mainMenuStatus=selected+100;switch(selected){
            case 0:mainMenuButtons[0].SetVisibility(2);break;
            case 1:mainMenuButtons[1].SetVisibility(2);break;
            case 2:mainMenuButtons[2].SetVisibility(2);break;
            case 3:mainMenuButtons[3].SetVisibility(2);break;
            }}
        }
        if(result>=1&&result<=5){Highlight(result-1,0);selection=result;}
        GuiScreen::Update();
    }
}
void MainMenuScreen::Render(){
    if(IsActive()){
        if(playing){if(animation.Update(&g511948)&4){ready=1;animation.Close();HercSetMissionStart(&g4b74f4,2);}}
        if(!modal)for(int i=0;i<17;++i)ambient[i].Update(&g511948);
        if(TimerExpired(&blinkTimer)&&canStart){
            if(blink==0){blink=1;mainLabel4.SetColor(2);TimerSet(&blinkTimer,800);}
            else{blink=0;mainLabel4.SetColor(10);TimerSet(&blinkTimer,600);}
            mainLabel4.Invalidate();
        }
        GuiScreen::Draw();
    }
}
int MainMenuScreen::Hover(){if(modal)return -1;return BriefDone("mainmenu");}

MenuPrefixButton mainMenuButtons[11];
MenuButton mainButton0(0,0),mainButton1(0,0),mainButton2(0,0),mainButton3(0,0),mainButton4(0,0),mainButton5(0,0);
MenuHelpRegion mainHelp;
MenuLabel mainLabel0(0,-1,1,11,1),mainLabel1(0,-1,1,11,1),mainLabel2(0,-1,1,11,1),mainLabel3(0,-1,1,11,1),mainLabel4(0,-1,1,11,1),mainLabel5(0,-1,1,11,1),mainLabel6(0,-1,1,11,1),mainLabel7(0,-1,1,11,1),mainLabel8(0,-1,1,11,1),mainLabel9(0,-1,1,11,1),mainLabel10(0,-1,1,11,1);
MenuPanel mainPanel;MenuNumber mainNumber;MainMenuScreen g4aebe0;









