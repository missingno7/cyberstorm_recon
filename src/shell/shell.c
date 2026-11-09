#include <string.h>
/* shell\shell.c, partial reconstruction of the single supported object cluster.
 * Corrected-map continuation: shared case blocks and tables belong to their owners.
 * Shell fields marked unknown are unobserved layout, not initialized storage. */
typedef unsigned long U32;
struct Surface {long width,height;unsigned char *pixels;U32 flags;Surface(U32);~Surface();};
struct FileRecord {U32 handle,open,flags,error;signed char direct,counted;unsigned char reserved[2];U32 position,length;
    FileRecord();~FileRecord();U32 IsOpen(){return open;}};
struct FlicSound {unsigned short id,frame;unsigned char volume,pan,pitch,flags;};
struct SoundSettings {int loop,pan,pitch,volume;};

struct FrameHeader {long size;short magic,chunks;long reserved[2];};
struct FlicPalette;
class Flic {
public:
    Surface *target;
    Surface bitmap;
    U32 unknown14,unknown18; /* unobserved; not fabricated initialized fields */
    FlicSound *sounds;
    FileRecord file;
    FlicPalette *palette;
    U32 status;
    U32 currentFrame;
    U32 deadline;
    long loops;
    int active;
    U32 memoryPosition;
    unsigned char *resource;
    FrameHeader pending;
    U32 pendingRead;
    long fileSize,magic,frames,width,height,delay;
    U32 firstFrame,secondFrame;
    Flic();~Flic();int Open(long,int=100,int=0);void Close();void ApplyPalette();U32 Update(Surface *);
};

struct GuiRect {long x,y,width,height;};
struct PaletteColor {unsigned char red,green,blue,flags;};
struct Palette {PaletteColor entries[256];unsigned char unknown[82];};
struct Timer;extern void TimerSet(Timer *,U32);struct Timer {U32 start,end,duration;Timer(U32 n){TimerSet(this,n);}};
struct InputReq {U32 mask,state;InputReq(U32 m):mask(m){} int NewPress(){return (state&0x50)==0x10;}int HelpPressed(){return (state&0xa0)==0x20;}};
extern void TimerSet(Timer *,U32);
extern int TimerExpired(Timer *);
extern long gGuiEffectPriority;
extern long gShellCreditStep;
extern Timer gShellCreditTimer;
extern char g513888;extern Surface g511948,gMenuSavedSurface;
extern GuiRect gMenuClip;
extern Palette gGuiPalette;
extern unsigned char *gSurfaceColorRemap;
extern int gShellOverlayPaletteActive,gShellTintModeActive;
extern PaletteColor gShellOverlayColors[];
extern "C" void AssertFail(const char *,int,const char *);
/* A TU-local filename follows the C++ registration record in original _DATA. */
static const char ShellSourceFile[]=__FILE__;
struct ResMgr {void *Load(const char *,int=1,void * =0,U32=0);};
extern ResMgr gResMgr;
extern void SigsStart(void *,unsigned char);
extern void SurfacePresent(void *);
extern void GuiResetClip(void *);
extern void SurfaceCopyRegion(void *,void *,long,long);
extern void PaletteCopyRange(Palette *,const PaletteColor *,int,int);
extern void GuiFilterPalette(Palette *,int);
extern void ApplyPaletteData(void *,int=0,int=256);
extern unsigned char PaletteFindColor(Palette *,const PaletteColor *);
extern void SurfaceRemapRect(void *,long,long,long,long);
extern void GuiRemapClipRect(GuiRect *,long,long,long,long);

/* Opaque screen instances are passed by address only in the close-all routine. */
struct Scene;struct MissBrief {unsigned char unknown00[0x50];unsigned char choice;void Begin(void *,unsigned char);};extern MissBrief g4b3390;
extern Scene g4aff6c,g4b2e30,g4b3598,g4b37c4,g4b416c,g4b5b50;
extern Scene g4b700c,g4aebe0,g4afa84,g4afbfc,g4b7904,g4b82fc,g4ba2bc;
extern Scene *gShellHotseat;
extern void ShellSelectionClose(Scene *);
extern void MissBriefClose(MissBrief *);
extern void CommsScreenClose(Scene *);
extern void ShellMissionClose(Scene *);
extern void HercBayShutdown(Scene *);
extern void HercBuyClose(Scene *);
extern void ShellStorageClose(Scene *);
extern void HercUpgradeClose(Scene *);
extern void ShellMainMenuClose(Scene *);
extern void ShellIntroClose(Scene *);
extern void ShellMapClose(Scene *);
extern void ShellBriefingClose(Scene *);
extern void ShellMultiplayerClose(Scene *);
extern void MultiplayerLaunchClose(Scene *);
extern void HotseatClose(Scene *);
extern void SoundClearContext();

#pragma pack(push,1)

extern U32 gInDirty;
extern void InputPoll(InputReq *);
extern void SurfaceClear(void *);
extern void ShellPlayMovie(const char *);
extern void ShellIntroStart(Scene *,InputReq *,int);
struct ProfileManager {unsigned char unknown00[0x426];U32 mode;unsigned char unknown42a[0x15];unsigned char ready;unsigned char unknown440[0x6c87];long credits;U32 Mode(){return mode;}long Credits(){return credits;}void SetDifficulty(unsigned char);void RefreshMissions();void UnlockItem(int,int);void RestartCampaign();void ApplyCampaignCheat();void AdvanceCampaign();void PrepareRoster();void SetMissionChoice(unsigned char);int LaunchMission(void *);int IsReady(){return ready;}long PendingCost();void CommitPurchases();int HasContinuation();void SelectScenario(int);int Count();void *Select(int);void Advance(int);long Value(int);};
extern ProfileManager profileManager;
extern int g4d030f;
extern long SendEvent(void *,int,void * =0,int=0,int=0);
extern void ShellMainMenuOpen(Scene *,InputReq *,int);
extern void ShellMultiplayerOpen(Scene *,InputReq *,int);
extern int gGuiPaletteFaded;
extern void GuiFadeIn();
extern Scene g4b16a4,g4b07f8,g4b0c50,g4b01c8,g4b0e18,g4ba9a8;
extern void ShellIntroRender(Scene *);
extern void ShellMainMenuRender(Scene *);
extern void ShellSelectionRender(Scene *);
extern void ShellSelectionControlsRender(Scene *);
extern void ShellBiodermStatsRender(Scene *);
extern void ShellBiodermSkillsRender(Scene *);
extern void ShellBiodermListRender(Scene *);
extern void ShellBiodermDetailsRender(Scene *);
extern void MissBriefUpdate(MissBrief *);
extern void CommsScreenRender(Scene *);
extern void ShellMissionRender(Scene *);
extern void HercBayRender(Scene *);
extern void HercBuyRender(Scene *);
extern void ShellStorageRender(Scene *);
extern void HercUpgradeRender(Scene *);
extern void ShellMapRender(Scene *);
extern void ShellBriefingRender(Scene *);
extern void ShellMultiplayerRender(Scene *);
extern void ShellNetworkSetupRender(Scene *);
extern void MultiplayerLaunchRender(Scene *);
extern void HotseatRender(Scene *);

class ShellBase {
public:
    long active;long IsActive()const{return active;}
    ShellBase():active(0){}
    virtual ~ShellBase();
    virtual void Open();
    virtual void Close(int);
    virtual void Update()=0;
    virtual void Event(unsigned char)=0;
};
class Shell:public ShellBase {
public:
    Shell():ShellBase(),input(0x8004){}
    virtual ~Shell();
    virtual void Open();
    virtual void Close(int);
    virtual void Update();
    virtual void Event(unsigned char);
    unsigned char unknown08[8];
    long status;
    unsigned char difficulty;
    long displayedCredits;
    U32 unknown19,unknown1d;
    char sessionName[30];
    U32 unknown3f;
    InputReq input;
    long auxiliaryState,screen;
    U32 resumeReason;unsigned char unknown57[8];
    int WithinBudget();
    void ProcessInput();
    void UpdateMainMenuSelection();
    void UpdateComms();
    void UpdateMainMenu();
    void UpdateMission();
    void UpdateBioderm();
    void UpdateBriefing();
    void UpdateHercBay();
    void UpdateHercBuy();
    void UpdateStorage();
    void UpdateHercUpgrade();
    void UpdateSelection();
    void UpdateMap();
    void UpdateMissionBrief();
    void UpdateCampaignMenu();
    void UpdateBiodermSkills();
    void UpdateBiodermPanel();
    void UpdateMultiplayer();
    void UpdateMultiplayerMenu();
    void UpdateNetworkSetup();
    void UpdateLaunch();

    void RestoreMenu(int);
    void ResumeMenus();
    void BeginComms(unsigned char);
    long SumRosterResponses();
    long ProfileValueTotal();
    InputReq *Request(){return &input;}
    int PlayTakeoff(int=0);
    void PlayTravel(U32);
    void BeginBriefing(unsigned char);
    void ShowIntro();
    void ReplayIntro();
    int UpdateCredits();
    U32 NextCreditBreakpoint(U32);
};
#pragma pack(pop)


/* Class extents are independently present in the original destructor type records. */
struct Rate {U32 count,last,interval;Rate(U32);~Rate();};
#pragma pack(push,1)
struct GameSession {unsigned char unknown00[0x70c7];long credits;unsigned char unknown70cb[0x428];GameSession();~GameSession();long Credits(){return credits;}};
#pragma pack(pop)
long gShellCreditStep;
Rate gShellRate(5);
Timer gShellIdleTimer(0);
Timer gShellCreditTimer(0);
Shell uiSystem;
GameSession gShellGame;
Timer gShellAnimationTimer(0);


struct PoolData;struct StrPool {long id;char *buffer;long capacity;PoolData *data;long size;int IsLocked(){return data!=0;}int Lock();void Unlock();char *GetString(long,int);};
extern StrPool gShellStrings,missionImageName;
extern void *gShellImages;
extern char gShellFont,g4b74f4,gShellResources;
extern void ShellMultiplayerShutdown(void *);
extern void ShellSoundShutdown();
extern void FlicFreeBuffers();
extern void SurfaceFree(void *);
extern "C" void f_cbf0(void *);
extern "C" void f_d2c0(void *);
extern "C" void f_cb80(void *);
extern int gShellExitCode;inline U32 ShellExitReason(){return gShellExitCode;}



extern void ShellMultiplayerInit(void *);
extern void SurfaceAttach(void *,void *,long,long);
extern void SurfaceCreate(void *,long,long);
extern void *gShellSurfacePixels,*gShellDrawPixels;
extern long gShellWidth,gShellHeight;inline long ShellWidth(){return gShellWidth;}inline long ShellHeight(){return gShellHeight;}
extern void ShellRefreshResources();
extern void FlicInitBuffers(U32,U32);
extern StrPool gShellMiniStrings,gShellParaStrings;
extern "C" void SetPointer(void *,void *);
extern void FontLoad(void *,const char *);
struct MenuImages;extern MenuImages *gMenuImages;
extern char gShellDefaultName[];
extern int gShellPlayIntro;
extern int g4d0413;
extern signed char gShellContinuationMap;
extern void ShellMapOpen(Scene *,InputReq *,long);
extern void ShellMissionBegin(Scene *,InputReq *);
extern int StopMultiplayer(int,int);
extern void CommsScreenOpen(Scene *,InputReq *,unsigned char);
extern void ShellNetworkSetupOpen(Scene *,InputReq *,unsigned char);
extern Scene g4ba9a8;

extern long gIn7c;
inline long ShellInputCommand(){return gIn7c;}
extern void ShellInputSound(long);
extern void ShellRefreshDifficulty();
extern void ShellSelectionOpen(Scene *,InputReq *,const char *);
struct HercBay {unsigned char unknown00[0x4c];long ownedCount,buyActive,storageActive;long OwnedCount(){return ownedCount;}long Buying(){return buyActive;}long Storing(){return storageActive;}void Start(void *);};
extern StrPool gShellMissionStrings;
extern void ShellMainMenuRefresh(Scene *);
class GuiScreen:public GuiRect {public:InputReq *input;int IsActive(){return input!=0;}void Invalidate();};
extern GuiRect gShellCurrentRect,gShellScreenRect;
extern void MenuSetColors(void *,U32,long);
extern void MenuFill(void *,long,long,long,long);
extern void ShellSelectionActivate(Scene *,long);
extern void MenuHelpSetTopic(long);
extern void ShellMultiplayerTick(void *);
extern int MissBriefExit(MissBrief *);
extern U32 ShellStorageExit(Scene *);
extern unsigned char ShellStorageSelection(Scene *);
extern void HercUpgradeOpen(Scene *,InputReq *,unsigned char);
extern void HercBayOpen(Scene *,InputReq *);
extern int ShellBriefingExit(Scene *);
extern void ShellBriefingContinue(Scene *);
extern void ShellBriefingBack(Scene *);
extern void ShellBriefingAbort(Scene *);
extern char gShellBriefingMission;
extern U32 gIn80;inline U32 ShellInputKey(){return gIn80;}
struct PlayerInfo {
    int Kind(){return kind;}
    unsigned char kind;
    char unknown01[39];
    int value;
    char unknown2c[4];
};
extern void GetPlayerInfo(void *,PlayerInfo *);
extern long gShellMapMode;
extern U32 ShellMissionExit(Scene *);
extern void ShellBriefingOpen(Scene *,InputReq *);
extern int SoundTrigger(unsigned,int);
extern void HercBayClose(HercBay *);
extern int HercBaySetStatus(HercBay *);
struct HercBayActions {void Reactivate();};
extern void HercBuyOpen(Scene *,InputReq *);
extern void ShellStorageOpen(Scene *,InputReq *);
extern void HercBayNoStorage(HercBay *);
extern U32 HercBuyExit(Scene *);
extern void HercBuySelect(Scene *,unsigned char);
extern void HercBuyAccept(Scene *);
extern void HercBuyCancel(Scene *);
extern void HercBayReturnFromPurchase(Scene *,InputReq *);
extern int HercBuyCommit(Scene *);
extern void HercBuyRefresh(Scene *);
void ShellClearFrame()
{
    SigsStart(&g513888,0);
    SurfacePresent(&g513888);
    GuiResetClip(&gMenuClip);
}

void ShellCloseAllScreens()
{
    ShellSelectionClose(&g4aff6c);
    MissBriefClose(&g4b3390);
    CommsScreenClose(&g4b2e30);
    ShellMissionClose(&g4b3598);
    HercBayShutdown(&g4b37c4);
    HercBuyClose(&g4b416c);
    ShellStorageClose(&g4b5b50);
    HercUpgradeClose(&g4b700c);
    ShellMainMenuClose(&g4aebe0);
    ShellIntroClose(&g4afa84);
    ShellMapClose(&g4afbfc);
    ShellBriefingClose(&g4b7904);
    ShellMultiplayerClose(&g4b82fc);
    MultiplayerLaunchClose(&g4ba2bc);
    if(gShellHotseat)HotseatClose(gShellHotseat);
    SoundClearContext();
}


void Shell::Close(int exitCode)
{
    if(IsActive()){
        gShellStrings.Unlock();
        missionImageName.Unlock();
        f_cb80(gShellImages);gShellImages=0;
        f_cbf0(&gShellFont);
        ShellCloseAllScreens();
        ShellMultiplayerShutdown(&g4b74f4);
        f_d2c0(&gShellResources);
        ShellSoundShutdown();
        FlicFreeBuffers();
        SurfaceFree(&gMenuSavedSurface);
        active=0;gShellExitCode=exitCode;
    }
}


extern StrPool gShellMessages;
extern const char *ShellFormatText(const char *,...);
extern void ShellErrorMessage(const char *);
extern int ShellConfirmMessage(const char *);
extern int g4d0413;
/* Six register-byte differences and an extent blocker remain. */
inline void ShellConfirmPurchasesCore()
{


    long subtotal=0;
    int index=1;
    void *current;
    for(; (current=profileManager.Select(index))!=0; ++index){
        subtotal+=SendEvent(current,0x73);
        subtotal+=SendEvent(current,0x77);
        subtotal+=SendEvent(current,0x75);

    }
    long total=profileManager.PendingCost();
    long pending=total;
    total+=subtotal;
    long budget=profileManager.Credits();
    if(total>budget){
        ShellErrorMessage(ShellFormatText(gShellMessages.GetString(423,1),total,budget));
    }else if(ShellConfirmMessage(ShellFormatText(gShellMessages.GetString(424,1),pending,subtotal,total,budget))){
        profileManager.CommitPurchases();
        index=1;
        for(; (current=profileManager.Select(index))!=0; ++index){
            SendEvent(current,0x74);
            SendEvent(current,0x78);
            SendEvent(current,0x76);
    
        }
    }


}
void ShellConfirmPurchases(){ShellConfirmPurchasesCore();}
void Shell::ProcessInput()
{
    long key=ShellInputCommand();int i;
    ShellInputSound(key);
    switch(key){
    case 30202:
        ShellCloseAllScreens();resumeReason=0;ResumeMenus();break;
    case 30203:
        if(screen!=8){ShellCloseAllScreens();ShellSelectionOpen(&g4aff6c,&input,"Bioderm");screen=8;}break;
    case 30204:
        if(screen!=9){ShellCloseAllScreens();((HercBay *)&g4b37c4)->Start(&input);screen=9;}break;
    case 30205:
        if(screen!=10){ShellCloseAllScreens();BeginComms(2);}break;
    case 30206:
        if(screen!=10){ShellCloseAllScreens();BeginComms(2);}break;
    case 30207:
        if(screen!=10){ShellCloseAllScreens();BeginComms(6);}break;
    case 31995:
        profileManager.SetDifficulty(0);profileManager.RefreshMissions();ShellRefreshDifficulty();break;
    case 31996:
        profileManager.SetDifficulty(1);profileManager.RefreshMissions();ShellRefreshDifficulty();break;
    case 31997:
        profileManager.SetDifficulty(2);profileManager.RefreshMissions();ShellRefreshDifficulty();break;
    case 32001:++profileManager.credits;break;
    case 32002:profileManager.credits+=1000;break;
    case 32003:profileManager.credits+=10000;break;
    case 32004:profileManager.credits+=100000;break;
    case 32005:profileManager.credits=47000000;break;
    case 32007:
        profileManager.CommitPurchases();
        for(i=1;i<=profileManager.Count();++i){
            void *item=profileManager.Select(i);
            if(item){SendEvent(item,0x74);SendEvent(item,0x76);SendEvent(item,0x78);}
        }
        break;
    case 32013:
        for(i=101;i<1024;++i)profileManager.UnlockItem(i,1);
        break;
    case 32014:
        for(i=51;i<=80;++i)profileManager.UnlockItem(i,1);
        break;
    case 32015:
        profileManager.RestartCampaign();
        for(i=101;i<1024;++i)profileManager.UnlockItem(i,1);
        break;
    case 32011:{
        int unlocked=!gShellMissionStrings.IsLocked();
        if(unlocked)gShellMissionStrings.Lock();
        profileManager.ApplyCampaignCheat();
        if(unlocked)gShellMissionStrings.Unlock();
        ShellCloseAllScreens();screen=5;ShellMainMenuOpen(&g4aebe0,&input,0);
        break;}
    case 32008:profileManager.RefreshMissions();break;
    case 32010:profileManager.AdvanceCampaign();break;
    case 32012:profileManager.RestartCampaign();break;
    case 31992:
        ShellConfirmPurchases();
        if(((GuiScreen *)&g4aebe0)->IsActive())ShellMainMenuRefresh(&g4aebe0);
        break;
    case 31993:
        profileManager.PrepareRoster();
        if(((GuiScreen *)&g4aebe0)->IsActive()){
            ShellMainMenuRefresh(&g4aebe0);((GuiScreen *)&g4aebe0)->Invalidate();
            gShellCurrentRect=gShellScreenRect;
            MenuSetColors(&g513888,196,196);MenuFill(&g513888,604,129,25,11);
        }
        break;
    }
    if(key>20000&&key<20199){long index=key-20000;if(index>0)ShellSelectionActivate(&g4b16a4,index);}
}
void Shell::Update()
{
    if(!IsActive())return;
    MenuHelpSetTopic(0);gInDirty=1;InputPoll(&input);
    if(ShellInputCommand()){ProcessInput();return;}
    // The original constructs this function-local timer once, even though no later read survives here.
    static Timer updateTimer(0);
    ShellMultiplayerTick(&g4b74f4);
    switch(screen){
    case 2:case 3:case 4:case 6:case 7:case 11:case 13:case 14:case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 26:case 29:case 33:case 36:default:break;
    case 10:UpdateComms();break;
    case 1:UpdateMainMenu();break;
    case 31:UpdateMission();break;
    case 5:UpdateMainMenuSelection();break;
    case 8:UpdateBioderm();break;
    case 28:UpdateBriefing();break;
    case 9:UpdateHercBay();break;
    case 25:UpdateHercBuy();break;
    case 27:UpdateStorage();break;
    case 30:UpdateHercUpgrade();break;
    case 12:UpdateSelection();UpdateMap();break;
    case 16:UpdateCampaignMenu();UpdateMap();break;
    case 17:UpdateBiodermSkills();UpdateMap();break;
    case 15:UpdateBiodermPanel();UpdateMap();break;
    case 32:UpdateMultiplayer();break;
    case 34:UpdateMissionBrief();break;
    case 35:UpdateMultiplayerMenu();break;
    case 37:UpdateNetworkSetup();break;
    case 38:UpdateLaunch();break;
    }
}
void Shell::UpdateBriefing()
{
    U32 result=MissBriefExit(&g4b3390);
    if(!result)return;
    switch(result){
    case 1:MissBriefClose(&g4b3390);BeginComms(3);break;
    case 2:
        MissBriefClose(&g4b3390);unknown1d=g4b3390.choice;
        profileManager.SetMissionChoice(g4b3390.choice);resumeReason=0;ResumeMenus();
        break;
    }
}
void Shell::BeginBriefing(unsigned char number)
{
    CommsScreenClose(&g4b2e30);
    screen=28;
    g4b3390.Begin(&input,number);
}
long Shell::SumRosterResponses()
{

    long total=0;
    int count=profileManager.Count();
    if(!count)return 0;
    void *current=profileManager.Select(-1);
    if(!current)return 0;
    for(int i=0;i<count;++i){
        total+=SendEvent(current,0x79);
        profileManager.Advance(1);
        current=profileManager.Select(-1);
    }
    return total;

}
long Shell::ProfileValueTotal()
{
    long total=0;
    int count=g4d030f;
    for(int i=0;i<count;++i)total+=profileManager.Value(i);
    return total;
}


void Shell::UpdateMission()
{

    PlayerInfo player;
    U32 result=ShellMissionExit(&g4b3598);
    if(!result)return;
    switch(result){
    case 1:
    ShellMissionClose(&g4b3598);
    switch(profileManager.Mode()){
    case 0:
        profileManager.SelectScenario(1);displayedCredits=profileManager.Credits();
        screen=34;ShellBriefingOpen(&g4b7904,&input);break;
    case 1:
        if(WithinBudget()){
            GetPlayerInfo(&profileManager,&player);
            int kind=player.Kind();
            gShellMapMode=0;
            screen=32;
            switch(kind){
            case 0:ShellMapOpen(&g4afbfc,&input,1000);break;
            case 1:case 2:case 3:case 4:case 5:ShellMapOpen(&g4afbfc,&input,1001);break;
            case 6:case 7:case 8:case 9:ShellMapOpen(&g4afbfc,&input,1002);break;
            case 10:case 11:case 12:ShellMapOpen(&g4afbfc,&input,1003);break;
            case 13:ShellMapOpen(&g4afbfc,&input,1004);break;
            }
        }else{resumeReason=0;unknown1d=999;ResumeMenus();}
        break;
    case 2:case 3:
        displayedCredits=profileManager.Credits();TimerSet(&gShellIdleTimer,120000);RestoreMenu(0);break;
    default:
        profileManager.SelectScenario(1);displayedCredits=profileManager.Credits();
        TimerSet(&gShellIdleTimer,120000);ShellIntroStart(&g4afa84,&input,0);screen=1;break;
    }
    }

}
void Shell::UpdateHercBay()
{

    if(gShellContinuationMap==1&&profileManager.IsReady()){
        gShellContinuationMap=0;SoundTrigger(209,0);
    }
    if(((HercBay *)&g4b37c4)->Buying()){HercBayClose((HercBay *)&g4b37c4);screen=25;return;}
    if(((HercBay *)&g4b37c4)->Storing()){HercBayClose((HercBay *)&g4b37c4);screen=27;return;}
    U32 result=HercBaySetStatus((HercBay *)&g4b37c4);
    if(!result&&ShellInputKey()==1)result=3;
    if(!result)return;
    switch(result){
    case 1:HercBayClose((HercBay *)&g4b37c4);HercBuyOpen(&g4b416c,&input);screen=25;break;
    case 2:
        if(((HercBay *)&g4b37c4)->ownedCount){
            HercBayClose((HercBay *)&g4b37c4);ShellStorageOpen(&g4b5b50,&input);screen=27;
        }else HercBayNoStorage((HercBay *)&g4b37c4);
        break;
    case 3:HercBayShutdown(&g4b37c4);resumeReason=0;ResumeMenus();break;
    case 4:((HercBayActions *)&g4b37c4)->Reactivate();break;
    }
}
void Shell::UpdateHercBuy()
{
    U32 result=HercBuyExit(&g4b416c);
    if(!result)return;
    if(result<=15){HercBuySelect(&g4b416c,(unsigned char)result);return;}
    switch(result){
    case 31:
        HercBuyAccept(&g4b416c);HercBuyClose(&g4b416c);
        HercBayReturnFromPurchase(&g4b37c4,&input);screen=9;break;
    case 32:
        HercBuyCancel(&g4b416c);HercBuyClose(&g4b416c);
        HercBayReturnFromPurchase(&g4b37c4,&input);screen=9;break;
    case 33:case 38:HercBuyRefresh(&g4b416c);break;
    case 39:
        if(HercBuyCommit(&g4b416c)){screen=25;HercBuyRefresh(&g4b416c);}break;
    }
}
void Shell::UpdateStorage()
{
    U32 result=ShellStorageExit(&g4b5b50);
    if(!result){
        long key=ShellInputKey();
        if(key==1||key==28)result=50;
    }
    if(!result)return;
    if(result<=24){
        ShellStorageClose(&g4b5b50);
        screen=30;
        HercUpgradeOpen(&g4b700c,&input,ShellStorageSelection(&g4b5b50));
        return;
    }
    switch(result){
    case 50:
        ShellStorageClose(&g4b5b50);
        HercBayOpen(&g4b37c4,&input);
        screen=9;
        break;
    }
}
void Shell::UpdateMissionBrief()
{
    U32 result=ShellBriefingExit(&g4b7904);
    if(!result)return;
    switch(result){
    case 1:
        ShellBriefingClose(&g4b7904);ShellIntroStart(&g4afa84,&input,1);
        TimerSet(&gShellIdleTimer,120000);screen=1;break;
    case 3:
        if(profileManager.LaunchMission(&gShellBriefingMission)){
            ShellBriefingClose(&g4b7904);Close(6);
        }else ShellBriefingContinue(&g4b7904);
        break;
    case 2:ShellBriefingBack(&g4b7904);break;
    case 21:ShellBriefingAbort(&g4b7904);break;
    }
}
void Shell::ShowIntro()
{
    ShellClearFrame();
    ShellPlayMovie("csintro");
    gInDirty=1;
    InputPoll(Request());
}
void Shell::ReplayIntro()
{
    ShellIntroClose(&g4afa84);
    SurfaceClear(&g511948);
    SurfacePresent(&g513888);
    ShowIntro();
    SurfaceClear(&g511948);
    SurfacePresent(&g513888);
    ShellIntroStart(&g4afa84,&input,0);
}
void ShellLoadPalette(const char *name)
{
    Palette palette;
    void *loaded=gResMgr.Load(name,1,&palette,sizeof(palette));






















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































    if(!loaded)AssertFail(ShellSourceFile,__LINE__,0);
    SurfaceCopyRegion(&gMenuSavedSurface,&g511948,0,0);
    if(gShellOverlayPaletteActive)PaletteCopyRange(&palette,gShellOverlayColors,171,37);
    GuiFilterPalette(&palette,gShellTintModeActive?3:1);
    ApplyPaletteData(&palette);
}


extern void HelpClearTopic();
extern void ShellSetMenuState(int);
extern void MenuLoadBackground(const char *,void *);
extern char gShellBackground;
extern void SurfaceFlush(void *);
extern void ShellServiceFrame(Shell *);
extern void ShellLimitFrameRate(Shell *);
extern void GuiFadeOut();
extern void SoundStop(unsigned);
struct SoundSettings;extern int SoundPlay(unsigned,const SoundSettings * =0);



void ShellApplyPalette(Palette *palette)
{
    SurfaceCopyRegion(&gMenuSavedSurface,&g511948,0,0);
    if(gShellOverlayPaletteActive)PaletteCopyRange(palette,gShellOverlayColors,171,37);
    GuiFilterPalette(palette,gShellTintModeActive?3:1);
    ApplyPaletteData(palette);
}

void ShellShadeRect(long x,long y,long width,long height)
{
    unsigned char map[256];
    map[0]=0;
    for(int i=10;i<=246;++i){
        PaletteColor color=gGuiPalette.entries[i];
        color.red=color.red*6U/10U;
        color.green=color.green*6U/10U+24;
        color.blue=color.blue*6U/10U+24;
        map[i]=PaletteFindColor(&gGuiPalette,&color);
    }
    gSurfaceColorRemap=map;
    SurfaceRemapRect(&g513888,x,y,width,height);
    GuiRemapClipRect(&gMenuClip,x,y,width,height);
    gSurfaceColorRemap=0;
}

/* Resource/state setup; remaining differences are recorded in REPORT.md. */
void Shell::Open()
{
    if(IsActive())return;
    U32 reason=ShellExitReason();
    active=1;
    ShellMultiplayerInit(&g4b74f4);
    TimerSet(&gShellIdleTimer,120000);
    status=0;
    SurfaceAttach(&g511948,gShellSurfacePixels,ShellWidth(),ShellHeight());
    SurfaceCreate(&gMenuSavedSurface,ShellWidth(),ShellHeight());
    gShellDrawPixels=gMenuSavedSurface.pixels;
    FlicInitBuffers(0x4baf0,0x4baf0);
    ShellRefreshResources();
    gShellStrings.Lock();missionImageName.Lock();
    SurfaceClear(&gMenuSavedSurface);GuiFadeOut();
    gShellMiniStrings.Unlock();gShellParaStrings.Unlock();
    SetPointer(&gShellMiniStrings,(void *)"minish.bin");
    SetPointer(&gShellParaStrings,(void *)"parash.bin");
    gShellMiniStrings.Lock();
    gShellImages=gResMgr.Load("shellgui.bmx");gMenuImages=(MenuImages *)gShellImages;
    FontLoad(&gShellFont,"Font3.fnx");GuiResetClip(&gMenuClip);
    memset(sessionName,0,30);strncpy(sessionName,gShellDefaultName,13);
    U32 mode=profileManager.Mode();
    switch(reason){
    case 0:case 2:
        if(gShellPlayIntro)ShowIntro();
    case 5:
        screen=1;auxiliaryState=1;
        ShellIntroStart(&g4afa84,&input,0);
        profileManager.SelectScenario(0);difficulty=0;
        displayedCredits=g4d0413;unknown1d=0;
        break;
    case 3:
        screen=5;auxiliaryState=5;resumeReason=0;
        ResumeMenus();
        break;
    case 4:
        screen=31;auxiliaryState=31;
        ShellMissionBegin(&g4b3598,&input);
        if(mode==2||mode==3)StopMultiplayer(0,1);
        break;
    default:



















































































        AssertFail(ShellSourceFile,__LINE__,"illegal exit code in shell.open");
    }
}
void Shell::BeginComms(unsigned char kind)
{

    switch(profileManager.Mode()){
    case 0:case 2:default:break;
    case 1:CommsScreenOpen(&g4b2e30,&input,kind);screen=10;break;
    case 3:ShellNetworkSetupOpen(&g4ba9a8,&input,kind);screen=36;break;
    }

}
void Shell::ResumeMenus()
{

    switch(profileManager.Mode()){
    case 1:
        if(profileManager.HasContinuation()){
            screen=32;ShellMapOpen(&g4afbfc,&input,999);gShellContinuationMap=1;
        }else RestoreMenu(resumeReason);
        return;
    case 2:case 3:
        RestoreMenu(resumeReason);
        return;
    default:RestoreMenu(resumeReason);break;
    }

}
void Shell::RestoreMenu(int reason)
{

    switch(profileManager.Mode()){
    case 1:
        screen=5;
        ShellMainMenuClose(&g4aebe0);
        ShellMainMenuOpen(&g4aebe0,&input,reason);
        auxiliaryState=5;
        break;
    case 2:case 3:
        screen=35;
        ShellMultiplayerClose(&g4b82fc);
        ShellMultiplayerOpen(&g4b82fc,&input,reason);
        auxiliaryState=35;
        break;
    }

}
/* Rendering dispatcher; canonical Event label retained pending coordinated rename. */
void Shell::Event(unsigned char)
{
    if(IsActive()){
        switch(screen){
        case 1:
            ShellIntroRender(&g4afa84);
            break;
        case 5:
            ShellMainMenuRender(&g4aebe0);
            break;
        case 8:
            ShellSelectionRender(&g4aff6c);
            ShellSelectionControlsRender(&g4b16a4);
            ShellBiodermStatsRender(&g4b07f8);
            ShellBiodermSkillsRender(&g4b0c50);
            ShellBiodermListRender(&g4b01c8);
            ShellBiodermDetailsRender(&g4b0e18);
            break;
        case 28:
            MissBriefUpdate(&g4b3390);
            break;
        case 10:
            CommsScreenRender(&g4b2e30);
            break;
        case 31:
            ShellMissionRender(&g4b3598);
            break;
        case 9:
            HercBayRender(&g4b37c4);
            break;
        case 25:case 29:
            HercBuyRender(&g4b416c);
            break;
        case 27:
            ShellStorageRender(&g4b5b50);
            break;
        case 30:
            HercUpgradeRender(&g4b700c);
            break;
        case 32:
            ShellMapRender(&g4afbfc);
            break;
        case 12:
            ShellSelectionRender(&g4aff6c);
            ShellSelectionControlsRender(&g4b16a4);
            ShellBiodermListRender(&g4b01c8);
            break;
        case 16:
            ShellSelectionRender(&g4aff6c);
            ShellSelectionControlsRender(&g4b16a4);
            ShellBiodermDetailsRender(&g4b0e18);
            break;
        case 17:
            ShellSelectionRender(&g4aff6c);
            ShellSelectionControlsRender(&g4b16a4);
            ShellBiodermStatsRender(&g4b07f8);
            break;
        case 15:
            ShellSelectionRender(&g4aff6c);
            ShellSelectionControlsRender(&g4b16a4);
            ShellBiodermSkillsRender(&g4b0c50);
            break;
        case 34:
            ShellBriefingRender(&g4b7904);
            break;
        case 35:
            ShellMultiplayerRender(&g4b82fc);
            break;
        case 36:
            ShellNetworkSetupRender(&g4ba9a8);
            break;
        case 37:
            MultiplayerLaunchRender(&g4ba2bc);
            break;
        case 38:
            HotseatRender(gShellHotseat);
            break;
        }
        if(gGuiPaletteFaded){GuiFadeIn();gGuiPaletteFaded=0;}
        else SurfaceFlush(&g513888);
    }
}
/* Diagnostic: byte-equal, but the inlined palette literal is still UNBOUND. */
int Shell::PlayTakeoff(int)
{
    Flic movie;
    int skipped=0;
    ShellLoadPalette("mainmenu.plx");
    MenuLoadBackground("BGBASE.BMX",&gShellBackground);
    SurfaceCopyRegion(&gMenuSavedSurface,&g511948,0,0);
    movie.Open((long)"takeoff.flx");
    movie.loops=1;
    SoundPlay(13);
    while(1){
        gInDirty=1;
        InputPoll(Request());
        if(ShellInputKey()||Request()->NewPress()||Request()->HelpPressed()){
            skipped=1;
            break;
        }
        if(movie.Update((Surface *)&g511948)&4)break;
        SurfaceFlush(&g513888);
        ShellServiceFrame(this);
        ShellLimitFrameRate(this);
    }
    movie.Close();
    GuiFadeOut();
    ShellClearFrame();
    SoundStop(13);
    return skipped;
}
/* Diagnostic: branch placement still differs; not an accepted function. */
void Shell::PlayTravel(U32 route)
{
    Flic movie;
    ShellSetMenuState(6);
    HelpClearTopic();
    if(gGuiEffectPriority>50){
        if(!PlayTakeoff()){
            switch(route){
            case 0:movie.Open((long)"icemoon.flx");break;
            case 1:SoundPlay(38);movie.Open((long)"whoosh.flx");break;
            default:return;
            }
            movie.loops=1;
            while(1){
                gInDirty=1;
                InputPoll(Request());
                if(ShellInputKey()||Request()->NewPress()||Request()->HelpPressed())break;
                U32 status=movie.Update((Surface *)&g511948);
                if(status&0x20)movie.ApplyPalette();
                if(status&4)break;
                SurfaceFlush(&g513888);
                ShellServiceFrame(this);
                ShellLimitFrameRate(this);
            }
            SoundStop(38);
        }
    }
    movie.Close();
    GuiFadeOut();
    gInDirty=1;
    InputPoll(Request());
}
int Shell::UpdateCredits()
{
    long credits=gShellGame.Credits();
    if(credits==displayedCredits){
        gShellCreditStep=0;
        if(TimerExpired(&gShellCreditTimer)){
            TimerSet(&gShellCreditTimer,1000);
            return 1;
        }
        return 0;
    }
    if(gGuiEffectPriority<100){displayedCredits=credits;gShellCreditStep=0;return 1;}
    long decrease=displayedCredits-credits;
    long increase=credits-displayedCredits;
    if(increase>0){
        if(!gShellCreditStep){gShellCreditStep=increase/6;if(!gShellCreditStep){displayedCredits=credits;return 1;}}
        if(increase<gShellCreditStep)displayedCredits=credits;else displayedCredits+=gShellCreditStep;
        return 1;
    }
    if(!gShellCreditStep){gShellCreditStep=decrease/6;if(!gShellCreditStep){displayedCredits=credits;return 1;}}
    if(decrease<gShellCreditStep)displayedCredits=credits;else displayedCredits-=gShellCreditStep;
    return 1;
}

U32 Shell::NextCreditBreakpoint(U32 credits)
{
    if(credits<=20000)return 20000;
    if(credits<=50000)return 50000;
    if(credits<=100000)return 100000;
    if(credits<=200000)return 200000;
    if(credits<=400000)return 400000;
    if(credits<=750000)return 750000;
    if(credits<=1000000)return 1000000;
    if(credits<=3000000)return 3000000;
    if(credits<=7500000)return 7500000;
    return 15000000;
}











