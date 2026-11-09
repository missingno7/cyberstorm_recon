struct FlicSound; inline const char *BioRoomName(){return "bioroom";}
typedef unsigned long U32;
struct GuiRect { long x, y, width, height; };
struct GuiPoint { long x, y; };
struct InputReq; struct StrPool;
struct Timer; void TimerSet(Timer *, U32); int TimerExpired(Timer *);
struct MenuImage { U32 offset; unsigned short width, height; U32 flags; };
struct MenuImages { U32 count; MenuImage images[1]; };
struct ViewRect { int left, top, right, bottom; };
struct Timer { U32 start, end, duration; Timer(U32 n) { TimerSet(this, n); } };
struct FileRecord { U32 handle, open, flags, error; signed char direct, counted; unsigned char reserved[2]; U32 position, length;
    FileRecord(); ~FileRecord(); };
struct Surface;
struct Palette { char data[0x452]; };
void ShellApplyPalette(Palette *data);
struct Flic { char body[0x3c]; Palette *palette; char unknown40[0xc]; long loops; char rest[0x40]; Flic(); ~Flic(); int Open(long source, int priority = 0x64, int sourceMode = 0); void Close(); void SetSounds(FlicSound *); unsigned long Update(Surface *surface); void SetLoops(long count) { loops = count; } Palette *GetPalette() { return palette; } };
struct GuiControlData : GuiRect { unsigned char style[4]; U32 attributes; U32 shortcut; };
class GuiControl : public GuiControlData {
public:
    GuiControl(U32, U32);
    virtual ~GuiControl();
    virtual void Reset(unsigned index);
    virtual void Draw(unsigned index);
    virtual int Event(unsigned index, U32 event);
    virtual int HitTest(const GuiPoint *point);
    void Invalidate() { attributes &= ~0x1000000UL; }
    void SetVisibility(U32 state) { attributes = (attributes & ~0xe00000UL) + (state << 21); }
};
#pragma pack(push, 1)
class MenuLabel : public GuiControl {
public:
    const char *text; long align; unsigned char font; int background; long color;
    MenuLabel(const char *, long, unsigned char, long, int); virtual ~MenuLabel(); virtual void Reset(unsigned); virtual void Draw(unsigned);
    void SetFont(unsigned char); void SetText(const char *); void SetColor(long); void SetAlign(long); void SetBackground(int);
};
class MenuPrefixButton : public GuiControl {
public:
    const char *text; U32 state; signed char font, color, align, prefix;
    MenuPrefixButton(const char * = 0, signed char = 1, signed char = 2, signed char = -1); virtual ~MenuPrefixButton(); virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned, U32);
    void SetText(const char *); void SetColor(signed char); void SetAlign(signed char); void SetPrefix(signed char); void SetFont(signed char);
};
class MenuCaption : public GuiControl { public: const char *text; signed char align; unsigned char reserved[2],drawn; MenuCaption(); MenuCaption(const char *, signed char, int, int, int); };
class MenuNumber : public GuiControl {
public:
    U32 unused; const char *text; long value; signed char font, color, flash, align;
    MenuNumber(signed char, signed char); void Align(int a) { align = a; } virtual ~MenuNumber(); virtual void Reset(unsigned); virtual void Draw(unsigned);
    void SetValue(long, signed char); void SetFont(signed char);
};
class MenuHelpRegion : public GuiControl {
public:
    long id; int activated; MenuHelpRegion(); virtual ~MenuHelpRegion(); virtual void Draw(unsigned); virtual int Event(unsigned, U32);
};
class MenuPanel : public GuiControl {
public:
    const char *text; MenuPanel(); virtual ~MenuPanel(); virtual void Draw(unsigned);
};
class MenuButton : public GuiControl {
public:
    U32 state; const char *text; MenuButton(const char *, U32); virtual ~MenuButton(); virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned, U32);
};
class MenuListButton : public GuiControl {
public:
    U32 state; MenuListButton(); virtual ~MenuListButton(); virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned, U32);
};
#pragma pack(pop)
class GuiScreen : public GuiRect {
public:
    InputReq *input;
    char unknown14[0x38];
    GuiScreen(GuiControl **list, unsigned length, U32 flags);
    ~GuiScreen() { input = 0; }
    int Update();
    void LoadLayout(long resource);
    void Open(InputReq *, StrPool *, long base, int navigate);
    void Draw();
    void Invalidate();
    long HitTest(const GuiPoint *);
    int IsActive() const { return input != 0; }
};

struct StrPool {};
struct ResMgr {void *Load(long,int=1,int=0,int=0);};
extern ResMgr gResMgr;
struct InputReq;
extern StrPool gShellStrings;
extern char gShellGame[],profileManager[],g4b74f4;
extern long g4ae17f,gGuiEffectPriority;
extern int g4a706d,logicMode;
extern unsigned short gBiodermSelectionMode;
extern unsigned char biodermListVisible,bioOtherVisible;
extern unsigned char *gScreenImages;
extern Surface g511948; extern char gMenuSavedSurface,gShellBackground;
extern int g513888;
extern GuiPoint g4bb78c;
extern GuiRect gMenuClip; extern ViewRect viewportClipLeft,gBriefDrawSource;
int bioLastRegion=-999; static char biomenuFile[]=__FILE__; extern int bioRoomHandle,bioListState; extern unsigned short bioMedlabState,bioListMode,bioVrMode,bioStandbyState;
extern int bioAnimationEnabled;
extern GuiControl *bioControls[];
extern "C" void AssertFail(const char *,int,const char *);
void ShellClearFrame(); void ShellLoadPalette(const char *); void GuiFadeOut();
extern "C" void BriefWaitB(int); extern "C" void BriefWaitA(int);
extern "C" void f_0043ba3e(void *,void *);
void MenuLoadBackground(const char *,void *);void SurfaceCopyRegion(void *,void *,long=0,long=0);
void GuiResetClip(void *);void SurfacePresent(void *);void MenuDrawImage(void *,const MenuImage *,long,long);
extern "C" void f_cb80(void *);
void PopObjectConstruct(void *);
extern "C" int BriefDone(const char *);
void BioLoadRegions(const char *);
struct SoundSettings;int SoundPlay(unsigned,const SoundSettings *);int SoundTrigger(unsigned,int=0);
extern "C" void HercSetMissionStart(void *,int);extern "C" void HercSetMission(void *,int);
void PilotBriefOpenStandby(void *,void *);void PilotBriefResetIfOpen(void *);
void PilotGuiOpen(void *,void *);void PilotGuiCloseResources(void *);void PilotGuiDisable(void *);void PilotGuiEnable(void *);
void PilotGuiClearProfileModified(void *);void PilotGuiSetProfileModified(void *);void PilotGuiSelectionControlsRender(void *);
extern "C" void f_004120fe(void *,int);
struct BiodermListScreen {void Open(InputReq *);void Close();};
struct BiodermStatsScreen {void Open(InputReq *);void Close();};
struct MedlabScreen {void Open(InputReq *);void Close();void Invalidate();};
extern BiodermListScreen g4b01c8;extern BiodermStatsScreen g4b07f8;extern MedlabScreen g4b0c50;
extern char g4b0e18;struct PilotGuiModule;extern PilotGuiModule g4b16a4;
#pragma pack(push,1)
class BioScreen:public GuiScreen {public:
 int status; unsigned char selection,pad51; unsigned char *lights; unsigned state;int ready,vatOpen,medOpen,vrOpen,standbyOpen;unsigned short suppressed;unsigned char unknown70[4];Flic animation;
 BioScreen();~BioScreen(){};
 void Open(InputReq *);void OpenVatExit(InputReq *);void OpenMedExit(InputReq *);void OpenVrExit(InputReq *);void OpenStandbyExit(InputReq *);void Setup(InputReq *);
 int Suppressed(){return suppressed;}unsigned char *Lights(){return lights;}void Suppress(unsigned short);void Close();void Deactivate();void DrawRegions(int,int);int Event();void Render();int IsVatOpening();void Finish();
 void ToggleVat();void ToggleMed();void ToggleStandby();void ToggleVr();void StartVat();void PlayOnce(const char *,FlicSound *,const char * =0);void PlayLoop(const char *,FlicSound *,const char * =0);
};
#pragma pack(pop)
extern FlicSound bioVatStart[],bioVatClose[],bioLoopSounds[],bioStandbyStart[],bioStandbyEnd[],bioVrStart[],bioVrEnd[],bioMedEnd[],bioMedStart[];
MenuPanel bioPanel;
MenuPrefixButton bioButtons[5];
MenuHelpRegion bioHelp;
MenuCaption bioCaption(0,0,0,0,1);
BioScreen g4aff6c;
BioScreen::BioScreen():GuiScreen(bioControls,7,0),animation(){}
void BioScreen::Open(InputReq *request){if(!IsActive()){
 ShellClearFrame();BriefWaitB(0x12);f_0043ba3e(gShellGame,profileManager);g4a706d=g4ae17f-1;
 Setup(request);ready=1;ShellLoadPalette("bioroom.plx");MenuLoadBackground("BGBIO.BMX",&gShellBackground);SurfaceCopyRegion(&gMenuSavedSurface,&g511948);
 PilotGuiOpen(&g4b16a4,request);gBiodermSelectionMode=3;PilotGuiClearProfileModified(&g4b16a4);
 state=0;vatOpen=0;medOpen=0;vrOpen=0;standbyOpen=0;suppressed=0;selection=0;
 g4b07f8.Open(request);g4b0c50.Open(request);g4b01c8.Open(request);PilotBriefOpenStandby(&g4b0e18,request);
 *(int *)((char *)this+0x4c)=0;biodermListVisible=0;SoundTrigger(0x32,0);HercSetMissionStart(&g4b74f4,0x928);
 bioButtons[0].SetAlign(2);bioButtons[1].SetAlign(2);bioButtons[2].SetAlign(2);bioButtons[3].SetAlign(2);bioButtons[4].SetAlign(2);
}}
void BioScreen::OpenVatExit(InputReq *request){if(!IsActive()){
 Setup(request);MenuLoadBackground("BGBIO.BMX",&gShellBackground);SurfaceCopyRegion(&gMenuSavedSurface,&g511948);
 if(gGuiEffectPriority>50){PlayOnce("vdclose.flx",bioVatClose,"vatdoor.plx");state=3;}else{bioRoomHandle=-1;state=0;}vatOpen=0;
}}
void BioScreen::OpenMedExit(InputReq *request){if(!IsActive()){
 bioRoomHandle=-1;Setup(request);MenuLoadBackground("BGBIO.BMX",&gShellBackground);SurfaceCopyRegion(&gMenuSavedSurface,&g511948);
 if(gGuiEffectPriority>50){PlayOnce("mlend.flx",bioMedEnd);state=12;}else{bioRoomHandle=-1;state=0;}medOpen=0;
}}
void BioScreen::OpenVrExit(InputReq *request){if(!IsActive()){
 Setup(request);MenuLoadBackground("BGBIO.BMX",&gShellBackground);SurfaceCopyRegion(&gMenuSavedSurface,&g511948);
 if(gGuiEffectPriority>50){PlayOnce("vrend.flx",bioVrEnd,"vrloop.plx");state=48;}else{bioRoomHandle=-1;state=0;}vrOpen=0;
}}
void BioScreen::OpenStandbyExit(InputReq *request){if(!IsActive()){
 Setup(request);MenuLoadBackground("BGBIO.BMX",&gShellBackground);SurfaceCopyRegion(&gMenuSavedSurface,&g511948);
 if(gGuiEffectPriority>50){PlayOnce("sbend.flx",bioStandbyEnd);state=192;}else{bioRoomHandle=-1;state=0;}standbyOpen=0;
}}
void BioScreen::Setup(InputReq *request){
 int mode=logicMode; if(mode==1)BriefWaitA(2);else BriefWaitA(9);g4a706d=g4ae17f-1;BriefWaitB(0x12);bioListState=-1;BioLoadRegions("bioroom.ply");LoadLayout((long)"bioroom.box");GuiScreen::Open(request,&gShellStrings,801,1);
 if(!gScreenImages)gScreenImages=(unsigned char *)gResMgr.Load((long)"biogui.bmx");
























































































































































 if(!gScreenImages)AssertFail(biomenuFile,__LINE__,(const char *)gScreenImages);
 if(!Lights())lights=(unsigned char *)gResMgr.Load((long)"biolite.bmx");


 if(!lights)AssertFail(biomenuFile,__LINE__,(const char *)lights);
 animation.Open((long)"biodlop1.flx",100,1);animation.SetLoops(-1);
}
void BioScreen::Suppress(unsigned short n){suppressed=n;}
void BioScreen::Close(){if(IsActive()){
 ready=0;animation.Close();GuiFadeOut();ShellClearFrame();PilotGuiCloseResources(&g4b16a4);g4b07f8.Close();g4b0c50.Close();g4b01c8.Close();PilotBriefResetIfOpen(&g4b0e18);
 if(gScreenImages){f_cb80(gScreenImages);gScreenImages=0;}if(lights){f_cb80(lights);lights=0;}input=0;biodermListVisible=0;bioOtherVisible=0;HercSetMission(&g4b74f4,0x928);
}}
void BioScreen::Deactivate(){if(IsActive()){BriefWaitA(6);input=0;}}
void BioScreen::DrawRegions(int region,int sound){
 if(IsVatOpening()||!lights||region==bioLastRegion){if(!(state&8)&&region!=5)return;sound=0;}
 GuiResetClip(&gMenuClip);viewportClipLeft=gBriefDrawSource;
 for(int i=0;i<4;++i){int n=i*2+(region==i);switch(state){
case 192:break;

 case 0:MenuDrawImage(&g513888,(const MenuImage *)(lights+4)+n,0,0);break;
 case 2:if(i<3){if(n==4)n=10;MenuDrawImage(&g513888,(const MenuImage *)(lights+4)+n,0,0);}break;
 case 8:if(i)MenuDrawImage(&g513888,(const MenuImage *)(lights+4)+n,0,0);break;
 case 32:if(i!=1)MenuDrawImage(&g513888,(const MenuImage *)(lights+4)+n,0,0);break;
 case 128:if(i!=2){if(i==3)n=8+(region==3);MenuDrawImage(&g513888,(const MenuImage *)(lights+4)+n,0,0);}break;

 }
 if(region==i && state && sound && i!=bioLastRegion)SoundPlay(0x188,0);
 }bioLastRegion=region;
}
int BioScreen::Event(){if(IsActive()){
 if(state)return 0;int region=HitTest(&g4bb78c);
 for(int i=0;i<5;++i)bioButtons[i].SetVisibility(0);
 bioListState=region;if(region<0){region=BriefDone(BioRoomName());DrawRegions(region,0);if(region>=0&&region<4){bioListState=region+100;switch(region){case 0:bioButtons[0].attributes=(bioButtons[0].attributes&0xff1fffffUL)+0x400000;break;case 1:bioButtons[1].attributes=(bioButtons[1].attributes&0xff1fffffUL)+0x400000;break;case 2:bioButtons[2].attributes=(bioButtons[2].attributes&0xff1fffffUL)+0x400000;break;case 3:bioButtons[3].attributes=(bioButtons[3].attributes&0xff1fffffUL)+0x400000;break;}}}
 if(gGuiEffectPriority>25&&region>=0&&region<=4)DrawRegions(region,0);return GuiScreen::Update();
 }return 0;}
void BioScreen::Render(){
 if(!state){animation.Update(&g511948);if(!Suppressed()){Invalidate();Draw();}return;}
 switch(state){
 case 0x100:break; /* Unused state boundary inferred from switch lowering. */
 case 1:if(gGuiEffectPriority<=50||(animation.Update(&g511948)&4)){state=2;vatOpen=1;PlayLoop("vatloop.flx",bioLoopSounds);}break;
 case 4:if(gGuiEffectPriority<=50||(animation.Update(&g511948)&4)){state=8;medOpen=1;PlayLoop("mlloop.flx",bioLoopSounds,"medlab.plx");g4b0c50.Invalidate();}break;
 case 16:if(gGuiEffectPriority<=50||(animation.Update(&g511948)&4)){state=32;vrOpen=1;PlayLoop("vrloop.flx",0);}break;
 case 64:if(gGuiEffectPriority<=50||(animation.Update(&g511948)&4)){state=128;standbyOpen=1;PlayLoop("sbloop.flx",0);}break;
 case 3:case 12:case 48:case 192:if(!bioAnimationEnabled||(animation.Update(&g511948)&4)){animation.Open((long)"biodlop1.flx",100,1);animation.SetLoops(-1);ShellLoadPalette("bioroom.plx");state=0;f_004120fe(&g4b16a4,1);}break;
 case 2:case 8:case 32:case 128:animation.Update(&g511948);break;
 }PilotGuiSelectionControlsRender(&g4b16a4);
}
int BioScreen::IsVatOpening(){unsigned n=state;return n==1;}
void BioScreen::Finish(){BriefDone(BioRoomName());}
void BioScreen::ToggleVat(){switch(state){ case 2:PlayOnce("vdclose.flx",bioVatClose,"vatdoor.plx");state=3;gBiodermSelectionMode=1;PilotGuiDisable(&g4b16a4);bioListMode=2;break;case 0:
 PilotGuiEnable(&g4b16a4);if(gBiodermSelectionMode==3){MenuDrawImage(&g513888,(const MenuImage *)(gScreenImages+0x1c),0,0);SurfacePresent(&g513888);}StartVat();state=1;gBiodermSelectionMode=0;bioListMode=0;suppressed=0;SoundTrigger(0x41,0);break;
}}
void BioScreen::ToggleMed(){switch(state){ case 8:PlayOnce("mlend.flx",bioMedEnd);state=12;gBiodermSelectionMode=1;bioMedlabState=1;break;case 0:
 PilotGuiDisable(&g4b16a4);if(gBiodermSelectionMode==3){MenuDrawImage(&g513888,(const MenuImage *)(gScreenImages+0x1c),0,0);SurfacePresent(&g513888);}PlayOnce("mlstart.flx",bioMedStart);state=4;gBiodermSelectionMode=0;bioMedlabState=0;suppressed=0;SoundTrigger(0x3a,0);break;
}}
void BioScreen::ToggleStandby(){switch(state){ case 128:PlayOnce("sbend.flx",bioStandbyEnd);gBiodermSelectionMode=1;state=192;bioStandbyState=1;break;case 0:
 PilotGuiDisable(&g4b16a4);if(gBiodermSelectionMode==3){MenuDrawImage(&g513888,(const MenuImage *)(gScreenImages+0x1c),0,0);SurfacePresent(&g513888);}PlayOnce("sbstart.flx",bioStandbyStart,"sbstart.plx");state=64;gBiodermSelectionMode=0;bioStandbyState=0;suppressed=0;SoundTrigger(0x3b,0);break;
}}
void BioScreen::ToggleVr(){switch(state){ case 32:PlayOnce("vrend.flx",bioVrEnd,"vrloop.plx");gBiodermSelectionMode=1;bioVrMode=1;state=48;break;case 0:
 PilotGuiDisable(&g4b16a4);if(gBiodermSelectionMode==3){MenuDrawImage(&g513888,(const MenuImage *)(gScreenImages+0x1c),0,0);SurfacePresent(&g513888);}PlayOnce("vrstart.flx",bioVrStart,"vrloop.plx");state=16;gBiodermSelectionMode=0;bioVrMode=0;suppressed=0;SoundTrigger(0x46,0);break;
}}
void BioScreen::StartVat(){
 PilotGuiClearProfileModified(&g4b16a4);MenuLoadBackground("BGBIO.BMX",&gShellBackground);SurfaceCopyRegion(&gMenuSavedSurface,&g511948);ShellLoadPalette("vatdoor.plx");bioRoomHandle=-1;animation.Open((long)"vatdoor.flx");animation.SetSounds(bioVatStart);animation.SetLoops(1);state=1;animation.SetSounds(bioVatStart);
}
void BioScreen::PlayOnce(const char *name,FlicSound *sounds,const char *palette){
 PilotGuiClearProfileModified(&g4b16a4);if(palette)ShellLoadPalette(palette);animation.Open((long)name);animation.SetLoops(1);animation.SetSounds(sounds);
}
void BioScreen::PlayLoop(const char *name,FlicSound *sounds,const char *palette){
 animation.Open((long)name,100,1);if(palette)ShellLoadPalette(palette);animation.SetSounds(sounds);animation.SetLoops(-1);PilotGuiSetProfileModified(&g4b16a4);
}
