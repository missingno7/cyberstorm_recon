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
public: U32 state;const char *text;MenuButton(const char *,U32);virtual ~MenuButton(){};
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
    MenuNumber(signed char,signed char);virtual ~MenuNumber(){};
    virtual void Reset(unsigned);virtual void Draw(unsigned);void SetValue(long,signed char);void SetFont(signed char);
};
class GuiScreen:public GuiRect {
public:
    InputReq *input;byte unknown14[56];
    GuiScreen(GuiControl **,unsigned,U32);~GuiScreen(){input=0;}void ResetInput();
    int IsActive() const{return input!=0;} void LoadLayout(long);void Open(InputReq *,StrPool *,long,int);
    void Invalidate();void ResetControls();long HitTest(const GuiPoint *);int Update();void Draw();
};
struct FileRecord {U32 handle,open,rest[5];U32 IsOpen(){return open;}};
struct FlicSound {unsigned short id,frame;byte volume,pan,pitch,flags;};
struct Flic {byte header[32];FileRecord file;Palette *palette;U32 fields40[3];long loops;byte tail[64];
    Flic();~Flic();int Open(long,int=100,int=0);void Close();U32 Update(Surface *);void SetSounds(FlicSound *);
    int IsOpen(){return file.IsOpen();}void SetLoops(long n){loops=n;}Palette *GetPalette(){return palette;}
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

void GuiScreen::ResetInput(){input=0;}
/* Top-menu screen. Global order and packed offsets are evidence; names are working labels.
 * Includes destructor COMDATs at 004093c0 and 004093d8. */
#pragma pack(push,1)
class TopMenuScreen:public GuiScreen {
public:
    byte page;int field4d;U32 lastTime;Flic animation;byte unknownE5[4];RangeCounter counter;
    TopMenuScreen();~TopMenuScreen(){}
    void SetControls();void ClearPanel();void Open(InputReq *,byte);void Select(byte);void Close();
    void Update();void Render();void AdvanceFrame();void NextAnimation();void Reset();void Finish();
};
#pragma pack(pop)
extern GuiControl *topMenuControls[];
extern MenuPrefixButton topMenuButtons[19];
extern MenuHelpRegion topMenuHelp;
extern MenuPanel topMenuPanel;
extern MenuLabel topMenuLabel;
extern TopMenuScreen g4afa84;
extern long topMenuStatus;
extern const char *topMenuText;

/* The shared base cleanup helper at 004087e0 is grouped here provisionally.
 * Its original object ownership is not established by the function proof. */


TopMenuScreen::TopMenuScreen():GuiScreen(topMenuControls,22,0),animation(),counter(){}

void TopMenuScreen::SetControls() {
    int i;
    switch(page) {
    case 0:
        BriefWaitB(5);topMenuText=gShellStrings.GetString(142);topMenuPanel.Invalidate();
        for(i=0;i<5;++i){topMenuButtons[i].SetVisibility(0);topMenuButtons[i].Invalidate();}
        for(;i<=18;++i)topMenuButtons[i].SetVisibility(5);
        break;
    case 1:
        BriefWaitB(5);topMenuText=gShellStrings.GetString(143);topMenuPanel.Invalidate();
        for(i=0;i<18;++i)topMenuButtons[i].SetVisibility(5);
        for(i=5;i<9;++i){topMenuButtons[i].SetVisibility(0);topMenuButtons[i].Invalidate();}
        break;
    case 3:
        BriefWaitB(6);topMenuText=gShellStrings.GetString(144);topMenuPanel.Invalidate();
        for(i=0;i<18;++i)topMenuButtons[i].SetVisibility(5);
        for(i=9;i<13;++i){topMenuButtons[i].SetVisibility(0);topMenuButtons[i].Invalidate();}
        break;
    case 2:
        BriefWaitB(8);topMenuText=gShellStrings.GetString(145);topMenuPanel.Invalidate();
        for(i=0;i<18;++i)topMenuButtons[i].SetVisibility(5);
        for(i=15;i<19;++i){topMenuButtons[i].SetVisibility(0);topMenuButtons[i].Invalidate();}
        break;
    default:
        BriefWaitB(5);
        for(i=0;i<5;++i){topMenuButtons[i].SetVisibility(0);topMenuButtons[i].Invalidate();}
        for(;i<=18;++i)topMenuButtons[i].SetVisibility(5);
        break;
    }
}
void TopMenuScreen::ClearPanel(){
    BriefBlit(&g513888,1,0,0,200,0,200,270,280);SurfacePresent(&g513888);
}
void TopMenuScreen::Open(InputReq *req,byte selected){
    if(!IsActive()){
        BriefWaitA(6);BriefDone("promo");ShellClearFrame();topMenuStatus=-1;
        topMenuLabel.SetFont(2);topMenuLabel.SetColor(2);
        LoadLayout((long)"topmenu.box");GuiScreen::Open(req,&gShellStrings,101,1);
        f_0047e200((int *)&MissionRandomTable);unsigned seed=MenuRandom(16);
        counter.SetBounds(0,15);counter.Set((unsigned short)seed);
        SoundClearContext();SoundTrigger(0,1500);NextAnimation();
        for(int i=0;i<19;++i)topMenuButtons[i].SetAlign(2);
        page=selected;SetControls();field4d=0;lastTime=TimeNow();BriefWaitA(1);HercSetMissionStart(&g4b74f4,2341);
    }
}
void TopMenuScreen::Select(byte selected){ClearPanel();page=selected;SetControls();}
void TopMenuScreen::Close(){
    if(IsActive()){SoundFlush(0);animation.Close();input=0;GuiFadeOut();ShellClearFrame();HercSetMission(&g4b74f4,2341);}
}
void TopMenuScreen::Update(){
    if(IsActive()){
        topMenuStatus=HitTest(&g4bb78c);
        if(topMenuStatus<0){int n=BriefDone("topmenu");if(n>=0)topMenuStatus=n+100;}
        GuiScreen::Update();
    }
}
void TopMenuScreen::Render(){if(IsActive()){AdvanceFrame();GuiScreen::Draw();}}
void TopMenuScreen::AdvanceFrame(){if(animation.Update(&g511948)&32)ShellApplyPalette(animation.GetPalette());}

/* Original per-animation sound sequences are external, initialized data. */
extern FlicSound topSounds0[],topSounds1[],topSounds2[],topSounds3[],topSounds4[],topSounds5[],topSounds6[],topSounds7[];
extern FlicSound topSounds8[],topSounds9[],topSounds10[],topSounds11[],topSounds12[],topSounds13[],topSounds14[],topSounds15[];
void TopMenuScreen::NextAnimation(){
    counter.Increment();animation.Close();SoundClearContext();SoundTrigger(0,1000);
    switch(counter.value){
    case 7:animation.Open((long)"pre_jugg.flx");SoundTrigger(112);topMenuLabel.SetText(gShellStrings.GetString(126));animation.SetSounds(topSounds7);break;
    case 5:animation.Open((long)"pre_defn.flx");SoundTrigger(110);topMenuLabel.SetText(gShellStrings.GetString(127));animation.SetSounds(topSounds5);break;
    case 3:animation.Open((long)"pre_sens.flx");SoundTrigger(107);topMenuLabel.SetText(gShellStrings.GetString(128));animation.SetSounds(topSounds3);break;
    case 6:animation.Open((long)"pre_dest.flx");SoundTrigger(111);topMenuLabel.SetText(gShellStrings.GetString(130));animation.SetSounds(topSounds6);break;
    case 1:animation.Open((long)"pre_shad.flx");SoundTrigger(105);topMenuLabel.SetText(gShellStrings.GetString(131));animation.SetSounds(topSounds1);break;
    case 2:animation.Open((long)"pre_ogre.flx");SoundTrigger(108);topMenuLabel.SetText(gShellStrings.GetString(132));animation.SetSounds(topSounds2);break;
    case 0:GuiFadeOut();ShellClearFrame();animation.Open((long)"pre_remo.flx");SoundTrigger(106);topMenuLabel.SetText(gShellStrings.GetString(133));animation.SetSounds(topSounds0);break;
    case 4:animation.Open((long)"pre_gian.flx");SoundTrigger(109);topMenuLabel.SetText(gShellStrings.GetString(135));animation.SetSounds(topSounds4);break;
    case 8:GuiFadeOut();ShellClearFrame();animation.Open((long)"pre_spe.flx");SoundTrigger(123);topMenuLabel.SetText(gShellStrings.GetString(136));animation.SetSounds(topSounds8);break;
    case 9:animation.Open((long)"pre_para.flx");SoundTrigger(124);topMenuLabel.SetText(gShellStrings.GetString(137));animation.SetSounds(topSounds9);break;
    case 10:animation.Open((long)"pre_cere.flx");SoundTrigger(125);topMenuLabel.SetText(gShellStrings.GetString(138));animation.SetSounds(topSounds10);break;
    case 11:animation.Open((long)"pre_arac.flx");SoundTrigger(126);topMenuLabel.SetText(gShellStrings.GetString(134));animation.SetSounds(topSounds11);break;
    case 12:animation.Open((long)"pre_gen.flx");SoundTrigger(127);topMenuLabel.SetText(gShellStrings.GetString(139));animation.SetSounds(topSounds12);break;
    case 13:animation.Open((long)"pre_mali.flx");SoundTrigger(128);topMenuLabel.SetText(gShellStrings.GetString(140));animation.SetSounds(topSounds13);break;
    case 14:animation.Open((long)"pre_had.flx");SoundTrigger(129);topMenuLabel.SetText(gShellStrings.GetString(141));animation.SetSounds(topSounds14);break;
    case 15:animation.Open((long)"pre_nih.flx");SoundTrigger(130);topMenuLabel.SetText(gShellStrings.GetString(129));animation.SetSounds(topSounds15);break;
    }
    topMenuPanel.Invalidate();Invalidate();animation.SetLoops(-1);
}
void TopMenuScreen::Reset(){ClearPanel();topMenuText=0;page=0;ResetControls();lastTime=TimeNow();}
void TopMenuScreen::Finish(){BriefDone("topmenu");}

MenuPrefixButton topMenuButtons[19];
MenuHelpRegion topMenuHelp;
MenuPanel topMenuPanel;
MenuLabel topMenuLabel(0,0,1,11,1);
TopMenuScreen g4afa84;








