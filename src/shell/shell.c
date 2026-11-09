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
struct InputReq {U32 mask,state;InputReq(U32 m):mask(m){} int NewPress(){return (state&0x50)==0x10;}int HelpPressed(){return (state&0xa0)==0x20;}int Released(){return (state&0x50)==0x40;}int HelpReleased(){return (state&0xa0)==0x80;}};
extern void TimerSet(Timer *,U32); extern int TimerExpired(Timer *); extern long gGuiEffectPriority; extern long gShellCreditStep;
extern Timer gShellCreditTimer;
struct GuiCanvas;extern GuiCanvas g513888;extern Surface g511948,gMenuSavedSurface;
extern GuiRect gMenuClip; extern Palette gGuiPalette; extern void *gDrawRemap; extern int gShellOverlayPaletteActive,gShellTintModeActive;
extern PaletteColor gShellOverlayColors[]; extern "C" void AssertFail(const char *,int,const char *);
/* A TU-local filename follows the C++ registration record in original _DATA. */
static const char ShellSourceFile[]=__FILE__;
struct ResMgr {void *Load(const char *,int=1,void * =0,U32=0);};
extern ResMgr gResMgr; extern void SigsStart(void *,int=0); extern void SurfacePresent(void *); extern void GuiResetClip(void *);
struct GuiCanvas {unsigned char unknown00[0x94];GuiRect clip;void ResetClip(){GuiResetClip(&clip);}};
extern void SurfaceCopyRegion(void *,void *,long,long); extern void PaletteCopyRange(Palette *,const PaletteColor *,int,int);
extern void GuiFilterPalette(Palette *,int); extern void ApplyPaletteData(void *,int=0,int=256);
extern unsigned char PaletteFindColor(Palette *,const PaletteColor *); extern void SurfaceRemapRect(void *,long,long,long,long);
extern void GuiRemapClipRect(GuiRect *,long,long,long,long);

/* Opaque screen instances are passed by address only in the close-all routine. */
struct Scene;struct MissBrief {unsigned char unknown00[0x50];unsigned char choice;unsigned char Choice(){return choice;}void Begin(void *,unsigned char);};extern MissBrief g4b3390;
extern Scene g4aff6c,g4b2e30,g4b3598,g4b37c4,g4b416c,g4b5b50; extern Scene g4b700c,g4aebe0,g4afa84,g4afbfc,g4b7904,g4b82fc,g4ba2bc; extern Scene *gShellHotseat;
extern void ShellSelectionClose(Scene *); extern void MissBriefClose(MissBrief *); extern void CommsScreenClose(Scene *);
extern void ShellMissionClose(Scene *); extern void HercBayShutdown(Scene *); extern void HercBuyClose(Scene *); extern void ShellStorageClose(Scene *);
extern int HercUpgradeClose(Scene *); extern void ShellMainMenuClose(Scene *); extern void ShellIntroClose(Scene *); extern void ShellMapClose(Scene *);
extern void ShellBriefingClose(Scene *); extern void ShellMultiplayerClose(Scene *); extern void MultiplayerLaunchClose(Scene *);
extern void HotseatClose(Scene *); extern void SoundClearContext();

#pragma pack(push,1)

extern U32 gInDirty; extern void InputPoll(InputReq *); extern void SurfaceClear(void *); extern void ShellPlayMovie(const char *);
extern void ShellIntroStart(Scene *,InputReq *,int=0);
struct ProfileManager {unsigned char unknown00[0x426];U32 mode;unsigned char unknown42a[0x15];unsigned char ready;unsigned char unknown440[0x6c87];long credits;U32 Mode(){return mode;}long Credits(){return credits;}ProfileManager *SetProfileMode(unsigned char);void RefreshMissions();void UnlockItem(int,int);void RestartCampaign();void ApplyCampaignCheat();void AdvanceCampaign();void PrepareRoster();void SetMissionChoice(U32);int LaunchMission(void *);int IsReady(){return ready;}long PendingCost();void CommitPurchases();int HasContinuation();void SelectScenario(int);void SetMode(U32);int Count();void *Select(int);void Advance(int);long Value(int);};
extern ProfileManager profileManager; extern int g4d030f; extern long SendEvent(void *,int,void * =0,int=0,int=0);
extern void ShellMainMenuOpen(Scene *,InputReq *,int); extern void ShellMultiplayerOpen(Scene *,InputReq *,int); extern int gGuiPaletteFaded;
extern void GuiFadeIn();extern void GuiFadeOut(); extern Scene g4b16a4,g4b07f8,g4b0c50,g4b01c8,g4b0e18,g4ba9a8; extern void ShellIntroRender(Scene *);
extern void ShellMainMenuRender(Scene *); extern void ShellSelectionRender(Scene *); extern void ShellSelectionControlsRender(Scene *);
extern void ShellBiodermStatsRender(Scene *); extern void ShellBiodermSkillsRender(Scene *); extern void ShellBiodermListRender(Scene *);
extern void ShellBiodermDetailsRender(Scene *); extern void MissBriefUpdate(MissBrief *); extern void CommsScreenRender(Scene *);
extern void ShellMissionRender(Scene *); extern void HercBayRender(Scene *); extern void HercBuyRender(Scene *); extern void ShellStorageRender(Scene *);
extern void HercUpgradeRender(Scene *); extern void ShellMapRender(Scene *); extern void ShellBriefingRender(Scene *);
extern void ShellMultiplayerRender(Scene *); extern void ShellNetworkSetupRender(Scene *); extern void MultiplayerLaunchRender(Scene *);
extern void HotseatRender(Scene *);

extern int gShellExitCode;
class ShellBase {
public:
    long active;long IsActive()const{return active;}
    ShellBase():active(0){}
    virtual ~ShellBase(){Close(0);}
    virtual void Open(){active=1;}
    virtual void Close(int reason){active=0;gShellExitCode=reason;}
    virtual void Update()=0;
    virtual void Event(unsigned char)=0;
};
class Shell:public ShellBase {
public:
    Shell():ShellBase(),input(0x8004){}
    virtual ~Shell(){}
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

    void UpdateMainMenu();
    void UpdateMission();
    void UpdateBioderm();
    void UpdateBriefing();
    void UpdateHercBay();

    void UpdateStorage();
    void UpdateHercUpgrade();
    char *DifficultyLabel();
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


extern "C" void ShellHercBuyHandler(Shell *);
#pragma aux ShellHercBuyHandler "W?UpdateHercBuy$:Shell$n()v";
extern "C" void ShellCommsHandler(void *);
#pragma aux ShellCommsHandler "W?UpdateComms$:Shell$n()v";
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
extern StrPool gShellStrings,missionImageName; extern void *gShellImages; extern char gShellFont,g4b74f4,gShellResources;
extern void ShellMultiplayerShutdown(void *); extern void ShellSoundShutdown(); extern void FlicFreeBuffers(); extern void SurfaceFree(void *);
extern "C" void f_cbf0(void *); extern "C" void f_d2c0(void *); extern "C" void f_cb80(void *);
extern int gShellExitCode;inline U32 ShellExitReason(){return gShellExitCode;}



extern void ShellMultiplayerInit(void *); extern void SurfaceAttach(void *,void *,long,long); extern void SurfaceCreate(void *,long,long);
extern void *gShellSurfacePixels,*gShellDrawPixels;
extern long g5138ac,g5138b0;inline long ShellWidth(){return g5138ac;}inline long ShellHeight(){return g5138b0;}
extern void HercPrepareAssets(); extern void FlicInitBuffers(U32,U32); extern StrPool gShellMiniStrings,gShellParaStrings;
extern "C" void SetPointer(void *,void *); extern "C" int f_cba0(void *,const char *);
struct MenuImages;extern MenuImages *gMenuImages;
extern char gShellDefaultName[]; struct GameSettings {int moviesEnabled;};extern GameSettings gGameSettings; extern int g4d0413; extern signed char gShellContinuationMap;
extern void ShellMapOpen(Scene *,InputReq *,long); extern void ShellMissionBegin(Scene *,InputReq *); extern int StopMultiplayer(int,int);
extern void CommsScreenOpen(Scene *,InputReq *,unsigned char); extern void ShellNetworkSetupOpen(Scene *,InputReq *,unsigned char); extern Scene g4ba9a8;

extern long gIn7c;
inline long ShellInputCommand(){return gIn7c;}
extern void SpecialOptionSound(int); extern void ResetMultiplayerInput(); extern void ShellSelectionOpen(Scene *,InputReq *,const char *);
struct HercBay {unsigned char unknown00[0x4c];long ownedCount,buyActive,storageActive;long OwnedCount(){return ownedCount;}long Buying(){return buyActive;}long Storing(){return storageActive;}void Start(void *);};
extern StrPool gShellMissionStrings; extern void ShellMainMenuRefresh(Scene *);
class GuiScreen:public GuiRect {public:InputReq *input;int IsActive(){return input!=0;}void Invalidate();};
extern GuiRect gShellCurrentRect,gShellScreenRect; extern void MenuSetColors(void *,U32,long); extern void MenuFill(void *,long,long,long,long);
extern void ShellSelectionActivate(Scene *,long); extern void MenuHelpSetTopic(long); extern void ShellMultiplayerTick(void *);
extern int MissBriefExit(MissBrief *); extern U32 ShellStorageExit(Scene *); extern unsigned char ShellStorageSelection(Scene *);
extern void HercUpgradeOpen(Scene *,InputReq *,unsigned char); extern void HercBayOpen(Scene *,InputReq *); extern int ShellBriefingExit(Scene *);
extern void ShellBriefingContinue(Scene *); extern void ShellBriefingBack(Scene *); extern void ShellBriefingAbort(Scene *); extern char gShellBriefingMission;
extern U32 gIn80;inline U32 ShellInputKey(){return gIn80;}
struct PlayerInfo {
    int Kind(){return kind;}
    unsigned char kind;
    char unknown01[39];
    int value;
    char unknown2c[4];
};
extern void GetPlayerInfo(void *,PlayerInfo *); extern long gShellMapMode; extern U32 ShellMissionExit(Scene *);
extern void ShellBriefingOpen(Scene *,InputReq *); extern int SoundTrigger(unsigned,int); extern void HercBayClose(HercBay *);
extern int HercBaySetStatus(HercBay *);
struct HercBayActions {void Reactivate();};
extern void HercBuyOpen(Scene *,InputReq *); extern void ShellStorageOpen(Scene *,InputReq *); extern void HercBayNoStorage(HercBay *);
extern U32 HercBuyExit(Scene *); extern void HercBuySelect(Scene *,unsigned char); extern void HercBuyAccept(Scene *); extern void HercBuyCancel(Scene *);
extern void HercBayReturnFromPurchase(Scene *,InputReq *); extern int HercBuyCommit(Scene *); extern void HercBuyRefresh(Scene *);
extern U32 CommsScreenExit(Scene *); extern void CommsScreenNext(Scene *); extern void CommsScreenPrevious(Scene *); extern void CommsScreenConfirm(Scene *);
extern void MenuHelpOpenTopic(long);
// Partial views of the child screens; only the observed return flag is exposed.
struct BiodermPanelState {unsigned char unknown00[0x4c];unsigned short returning;};
extern unsigned short gBiodermReturnToList,gBiodermSelectionMode;
inline unsigned short BiodermReturnToList(){return gBiodermReturnToList;}
extern U32 BiodermDetailsExit(Scene *); extern void BiodermDetailsAction1(Scene *); extern void BiodermDetailsAction2(Scene *);
extern void BiodermDetailsAction3(Scene *); extern void BiodermDetailsAction4(Scene *); extern void BiodermDetailsClose(Scene *);
extern void BiodermDetailsApply(Scene *); extern void ShellSelectionDetails(Scene *,InputReq *,const char *);
extern long gShellAttractReady,gShellAttractEnabled,gShellEndingScreen; extern void ShellIntroNextImage(Scene *);
extern U32 ShellIntroExit(Scene *); extern long ShellIntroTopMenu(Scene *); extern void ShellIntroSetMode(Scene *,int); extern long MenuHelpSetContext(long);
extern int LoadMission(const char *,int); extern int ShellOpenHotseat(Shell *); extern void InputClearEvents(); extern void GuiPumpMessages();
extern char gShellBackground; extern int MenuLoadBackground(const char *,void *); extern void ShellServiceFrame(Shell *);
extern void ShellLimitFrameRate(Shell *);
void ShellApplyPalette(Palette *);
struct SoundSettings;extern int SoundPlay(unsigned,const SoundSettings * =0);
extern U32 BiodermStatsExit(Scene *); extern void BiodermStatsSelect(Scene *,int); extern void BiodermStatsClose(Scene *);
extern void BiodermStatsApply(Scene *); extern void BiodermStatsAction(Scene *); extern void ShellSelectionStats(Scene *,InputReq *,const char *);
extern U32 TimeNow(); extern U32 gHercUpgradeLastClick; extern U32 HercUpgradeExit(Scene *); extern void HercUpgradeSelectPart(Scene *,U32);
extern void HercUpgradeSelectInventory(Scene *,U32); extern void HercUpgradeSelectCategory(Scene *,U32); extern void HercUpgradeAction47(Scene *);
extern void HercUpgradeAction48(Scene *); extern void HercUpgradeAction49(Scene *); extern void HercUpgradeAction50(Scene *);
extern void HercUpgradeAction52(Scene *); extern void HercUpgradeAction53(Scene *); extern void HercUpgradeRefresh(Scene *);
extern void HercUpgradeAction57(Scene *); extern void HercUpgradeAction58(Scene *); extern void HercUpgradeAction64(Scene *);
extern void HercUpgradeAction65(Scene *); extern void HercUpgradeAction85(Scene *); extern void ShellStorageReopen(Scene *,InputReq *);
extern int InputButtonPressed(int); extern long ShellMainMenuHitTest(Scene *); extern U32 ShellMainMenuExit(Scene *);
extern int ShellMainMenuCanLaunch(Scene *); extern void ShellMainMenuAction9(Scene *); extern void ShellMainMenuAction6(Scene *);
extern void ShellMainMenuAction11(Scene *); extern void ShellLeaveSession(int); extern U32 BiodermSkillsExit(Scene *); extern void BiodermSkillsClose(Scene *);
extern void BiodermSkillsApply(Scene *); extern void BiodermSkillsBatch(Scene *,int,int); extern void BiodermSkillsAction6(Scene *);
extern void BiodermSkillsAction7(Scene *); extern void ShellSelectionSkills(Scene *,InputReq *,const char *); extern long ShellMultiplayerHitTest(Scene *);
extern U32 ShellMultiplayerExit(Scene *); extern void ExploIdle(); extern int ShellMultiplayerCanStart(Scene *); extern int StartMultiplayer(const char *,int);
extern void ShellNetworkSetupBegin(Scene *,InputReq *); extern char gNetworkPlayerName[13]; extern void ShellMultiplayerReset(Scene *);
extern void ShellMultiplayerRefresh(Scene *); extern int ShellMultiplayerBusy(Scene *); extern int ShellMultiplayerPending(Scene *);
extern int HasMission(void *); extern U32 ShellNetworkSetupExit(Scene *); extern int ShellNetworkSetupCanStart(Scene *);
extern void ShellNetworkSetupLoaded(Scene *); extern void ShellNetworkSetupRefresh(Scene *); extern void ShellNetworkSetupSelectPlayer(Scene *,int);
extern void ShellNetworkSetupAction70(Scene *); extern void ShellNetworkSetupAction85(Scene *); extern void ShellNetworkSetupJoinPlayer(Scene *,int);
extern void ShellNetworkSetupAction4(Scene *); extern void ShellNetworkSetupAction78(Scene *); extern void ShellNetworkSetupAction79(Scene *);
extern void ShellNetworkSetupAction105(Scene *);

extern unsigned char gNetworkPlayerSelection;
struct MenuNetwork {int Begin();};extern MenuNetwork gMenuNetwork;
extern int logicMode;inline U32 LogicMode(){return logicMode;} extern long HotseatExit(Scene *); extern int HotseatCanLaunch(Scene *);
extern void HotseatSelectTeam(Scene *,long); extern void HotseatSelectColor(Scene *,long); extern void HotseatSelectPlayer(Scene *,long);
extern void HotseatRefresh(Scene *); extern void HotseatAction2(Scene *);
#pragma pack(push,1)
struct HotseatState {unsigned char unknown00[0x12a0];unsigned char option;unsigned char unknown12a1[3];long difficulty;unsigned char unknown12a8[0x20];unsigned char option2;};
#pragma pack(pop)
extern long ShellSelectionTabHitTest(Scene *); extern U32 ShellSelectionTabExit(Scene *); extern void BiodermListClose(Scene *);
extern void ShellSelectionList(Scene *,InputReq *,const char *); extern void ShellSelectionTabReset(Scene *); extern void ShellSelectionChooseTab(Scene *,int);
extern void ShellSelectionPrevious(Scene *); extern void ShellSelectionNext(Scene *); extern void ShellSelectionTabAction30(Scene *);
extern unsigned char gBiodermSelectionDirty;
#pragma pack(push,1)
struct BiodermSelectionState {
    unsigned char unknown00[0x56];U32 command;unsigned char unknown5a[4];
    long listRequested,skillsRequested,statsRequested,detailsRequested;
    int WantsList(){return listRequested;}
    int WantsSkills(){return skillsRequested;}
    int WantsStats(){return statsRequested;}
    int WantsDetails(){return detailsRequested;}
};
#pragma pack(pop)
extern U32 ShellMapExit(Scene *); extern int ShellMapBlocked(Scene *); extern long ShellMapHitTest(Scene *);
#pragma pack(push,1)
struct ShellMapState {unsigned char unknown00[0x4c];int inputEnabled;unsigned char zoom;};
#pragma pack(pop)
extern U32 BiodermListExit(Scene *); extern void BiodermListAction1(Scene *); extern void BiodermListAction4(Scene *); extern void BiodermListApply(Scene *);
extern void ShellSelectionRequestList(Scene *); extern void ShellSelectionRequestSkills(Scene *); extern void ShellSelectionRequestStats(Scene *);
extern void ShellSelectionRequestDetails(Scene *); extern void ShellSelectionFinish(Scene *); extern long ShellSelectionHitTest(Scene *);
extern U32 ShellSelectionExit(Scene *);
void ShellClearFrame()
{
    SigsStart(&g513888);
    SurfacePresent(&g513888);
    g513888.ResetClip();
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


extern StrPool gShellMessages; extern void GenerateMissions(ProfileManager *); extern "C" int f_0043d0c8(GameSession *); extern "C" int f_0043cef6(GameSession *);
extern "C" char *BriefFormat(const char *,...);
extern void f_00477295(const char *);
extern int f_0047723b(const char *);
extern int g4d0413;
/* The public extent also owns the following ProcessInput dispatch tables. */
static void ShellConfirmPurchases()
{


    long subtotal=0;
    int index=1;
    void *current;
    for(; (current=profileManager.Select(index))!=0; ++index){
        subtotal+=SendEvent(current,0x73);
        subtotal+=SendEvent(current,0x77);
        subtotal+=SendEvent(current,0x75);

    }
    long pending=f_0043cef6((GameSession *)&profileManager);
    long total=subtotal+pending;
    long budget=profileManager.Credits();
    if(total>budget){
        f_00477295(BriefFormat(gShellMessages.GetString(423,1),total,budget));
    }else if(f_0047723b(BriefFormat(gShellMessages.GetString(424,1),pending,subtotal,total,budget))){
        f_0043d0c8((GameSession *)&profileManager);
        index=1;
        for(; (current=profileManager.Select(index))!=0; ++index){
            SendEvent(current,0x74);
            SendEvent(current,0x78);
            SendEvent(current,0x76);
    
        }
    }


}
void Shell::ProcessInput()
{
    long key=ShellInputCommand();
    SpecialOptionSound(key);
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
        profileManager.SetProfileMode(0);GenerateMissions(&profileManager);ResetMultiplayerInput();break;
    case 31996:
        profileManager.SetProfileMode(1);GenerateMissions(&profileManager);ResetMultiplayerInput();break;
    case 31997:
        profileManager.SetProfileMode(2);GenerateMissions(&profileManager);ResetMultiplayerInput();break;
    case 32001:++profileManager.credits;break;
    case 32002:profileManager.credits+=1000;break;
    case 32003:profileManager.credits+=10000;break;
    case 32004:profileManager.credits+=100000;break;
    case 32005:profileManager.credits=47000000;break;
    case 32007:{
        f_0043d0c8((GameSession *)&profileManager);
        for(int i=1;i<=profileManager.Count();++i){
            void *item=profileManager.Select(i);
            if(item){SendEvent(item,0x74);SendEvent(item,0x76);SendEvent(item,0x78);}
        }
        break;}
    case 32013:{
        for(int i=101;i<1024;++i)profileManager.UnlockItem(i,1);
        break;}
    case 32014:{
        for(int i=51;i<=80;++i)profileManager.UnlockItem(i,1);
        break;}
    case 32015:{
        profileManager.RestartCampaign();
        for(int i=101;i<1024;++i)profileManager.UnlockItem(i,1);
        break;}
    case 32011:{
        int unlocked=!gShellMissionStrings.IsLocked();
        if(unlocked)gShellMissionStrings.Lock();
        profileManager.ApplyCampaignCheat();
        if(unlocked)gShellMissionStrings.Unlock();
        ShellCloseAllScreens();screen=5;ShellMainMenuOpen(&g4aebe0,&input,0);
        break;}
    case 32008:GenerateMissions(&profileManager);break;
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
    case 10:ShellCommsHandler(this);break;
    case 1:UpdateMainMenu();break;
    case 31:UpdateMission();break;
    case 5:UpdateMainMenuSelection();break;
    case 8:UpdateBioderm();break;
    case 28:UpdateBriefing();break;
    case 9:UpdateHercBay();break;
    case 25:ShellHercBuyHandler(this);break;
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
/* Watcom places this method's three return fragments before its public entry. */
char *Shell::DifficultyLabel()
{
    switch(difficulty){
    case 0:return gShellStrings.GetString(501,1);
    case 1:return gShellStrings.GetString(502,1);
    case 2:return gShellStrings.GetString(503,1);
    }

















































    AssertFail(ShellSourceFile,__LINE__,"invalid difficulty value");
    return " ";
}
void Shell::UpdateSelection()
{
    if(BiodermReturnToList()){
        gBiodermSelectionDirty=0;BiodermListClose(&g4b01c8);
        ((BiodermPanelState *)&g4b01c8)->returning=2;
        ShellSelectionList(&g4aff6c,&input,"Bioderm");screen=8;return;
    }
    U32 result=BiodermListExit(&g4b01c8);
    if(!result&&ShellInputKey()==1)result=3;
    if(!result)return;
    switch(result){
    case 4:BiodermListAction4(&g4b01c8);break;
    case 2:
        gBiodermSelectionDirty=0;BiodermListClose(&g4b01c8);
        gBiodermSelectionMode=1;((BiodermPanelState *)&g4b01c8)->returning=2;
        ShellSelectionList(&g4aff6c,&input,"Bioderm");screen=8;break;
    case 1:BiodermListAction1(&g4b01c8);break;
    case 3:
        gBiodermSelectionDirty=0;BiodermListApply(&g4b01c8);
        gBiodermSelectionMode=1;((BiodermPanelState *)&g4b01c8)->returning=2;
        ShellSelectionList(&g4aff6c,&input,"Bioderm");screen=8;break;
    }
}
void Shell::UpdateBioderm()
{
    switch(BiodermReturnToList()){
    case 4:ShellSelectionRequestList(&g4aff6c);break;
    case 1:ShellSelectionRequestSkills(&g4aff6c);break;
    case 3:ShellSelectionRequestStats(&g4aff6c);break;
    case 2:ShellSelectionRequestDetails(&g4aff6c);break;
    case 0:default:break;
    }
    if(((BiodermSelectionState *)&g4aff6c)->WantsList()){
        ShellSelectionFinish(&g4aff6c);screen=12;MenuHelpSetContext(19);return;
    }
    if(((BiodermSelectionState *)&g4aff6c)->WantsSkills()){
        ShellSelectionFinish(&g4aff6c);screen=15;MenuHelpSetContext(20);return;
    }
    if(((BiodermSelectionState *)&g4aff6c)->WantsStats()){
        ShellSelectionFinish(&g4aff6c);screen=17;MenuHelpSetContext(21);return;
    }
    if(((BiodermSelectionState *)&g4aff6c)->WantsDetails()){
        ShellSelectionFinish(&g4aff6c);screen=16;MenuHelpSetContext(22);return;
    }
    long hit=ShellSelectionHitTest(&g4aff6c);
    if(!InputButtonPressed(1))hit=-1;
    if(hit>=0){
        SoundPlay(5,0);
        switch(hit){
        case 1:ShellSelectionRequestDetails(&g4aff6c);break;
        case 3:ShellSelectionRequestList(&g4aff6c);break;
        case 0:ShellSelectionRequestSkills(&g4aff6c);break;
        case 2:ShellSelectionRequestStats(&g4aff6c);break;
        }
    }
    U32 result=ShellSelectionExit(&g4aff6c);
    if(!result&&ShellInputKey()==1)result=5;
    if(!result)return;
    switch(result){
    case 1:ShellSelectionRequestSkills(&g4aff6c);break;
    case 3:ShellSelectionRequestStats(&g4aff6c);break;
    case 4:ShellSelectionRequestList(&g4aff6c);break;
    case 2:ShellSelectionRequestDetails(&g4aff6c);break;
    case 5:ShellSelectionClose(&g4aff6c);resumeReason=0;ResumeMenus();break;
    }
}
void Shell::UpdateBriefing()
{
    U32 result=MissBriefExit(&g4b3390);
    if(!result)return;
    switch(result){
    case 1:MissBriefClose(&g4b3390);BeginComms(3);break;
    case 2:
        MissBriefClose(&g4b3390);unknown1d=g4b3390.Choice();
        profileManager.SetMissionChoice(unknown1d);resumeReason=0;ResumeMenus();
        break;
    }
}
void Shell::BeginBriefing(unsigned char number)
{
    CommsScreenClose(&g4b2e30);
    screen=28;
    g4b3390.Begin(&input,number);
}
extern "C" void ShellCommsHandler(void *object)
{ Shell *self=(Shell *)object;
    U32 result=CommsScreenExit(&g4b2e30);
    if(!result&&ShellInputKey()==1)result=14;
    if(!result)return;
    switch(result){
    case 68:
        self->auxiliaryState=10;CommsScreenClose(&g4b2e30);self->screen=32;
        ShellMapOpen(&g4afbfc,&self->input,2200);break;
    case 69:MenuHelpOpenTopic(66);break;
    case 70:MenuHelpOpenTopic(67);break;
    case 71:ShellPlayMovie("cst001");gInDirty=1;InputPoll(&self->input);break;
    case 72:ShellPlayMovie("cst002");gInDirty=1;InputPoll(&self->input);break;
    case 14:CommsScreenClose(&g4b2e30);self->resumeReason=0;self->ResumeMenus();break;
    case 1:self->BeginBriefing(0);break;
    case 2:self->BeginBriefing(1);break;
    case 3:self->BeginBriefing(2);break;
    case 4:self->BeginBriefing(3);break;
    case 5:self->BeginBriefing(4);break;
    case 6:self->BeginBriefing(5);break;
    case 7:self->BeginBriefing(6);break;
    case 8:self->BeginBriefing(7);break;
    case 9:self->BeginBriefing(8);break;
    case 10:CommsScreenConfirm(&g4b2e30);break;
    case 11:CommsScreenNext(&g4b2e30);break;
    case 12:CommsScreenPrevious(&g4b2e30);break;
    case 51:
        self->auxiliaryState=10;CommsScreenClose(&g4b2e30);self->screen=32;
        gShellMapMode=0;ShellMapOpen(&g4afbfc,&self->input,0);break;
    case 52:case 53:case 54:case 55:case 56:case 57:case 58:case 59:case 60:case 61:case 62:case 63:case 64: 
        self->auxiliaryState=10;CommsScreenClose(&g4b2e30);self->screen=32;
        gShellMapMode=1;ShellMapOpen(&g4afbfc,&self->input,13-(64-result));break;
    case 66:CommsScreenConfirm(&g4b2e30);break;
    }
}
int Shell::WithinBudget()
{
    long roster=SumRosterResponses();
    long total=ProfileValueTotal();total+=roster;total+=profileManager.Credits();
    PlayerInfo player;GetPlayerInfo(&profileManager,&player);
    U32 limit;
    switch(player.Kind()){
    case 0:limit=7000;break;
    case 1:limit=10000;break;
    case 2:limit=15000;break;
    case 3:limit=20000;break;
    case 4:limit=30000;break;
    case 5:limit=40000;break;
    case 6:limit=50000;break;
    case 7:limit=60000;break;
    case 8:limit=70000;break;
    case 9:limit=80000;break;
    case 10:limit=90000;break;
    case 11:limit=100000;break;
    case 12:limit=110000;break;
    case 13:limit=120000;break;
    default:return 0;
    }
    if((U32)total<limit)return 1;
    return 0;
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
extern "C" void ShellHercBuyHandler(Shell *self)
{
    U32 result=HercBuyExit(&g4b416c);
    if(!result)return;
    if(result<=15){HercBuySelect(&g4b416c,(unsigned char)result);return;}
    switch(result){
    case 31:
        HercBuyAccept(&g4b416c);HercBuyClose(&g4b416c);
        HercBayReturnFromPurchase(&g4b37c4,&self->input);self->screen=9;break;
    case 32:
        HercBuyCancel(&g4b416c);HercBuyClose(&g4b416c);
        HercBayReturnFromPurchase(&g4b37c4,&self->input);self->screen=9;break;
    case 33:case 38:HercBuyRefresh(&g4b416c);break;
    case 39:
        if(HercBuyCommit(&g4b416c)){self->screen=25;HercBuyRefresh(&g4b416c);}break;
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
// Numeric action labels are working names pending reconstruction of hercupgr.
void Shell::UpdateHercUpgrade()
{
    U32 result=HercUpgradeExit(&g4b700c);
    if(!result)return;
    if(result>=1&&result<=23){HercUpgradeSelectPart(&g4b700c,result-1);return;}
    if(result>23&&result<=46){HercUpgradeSelectInventory(&g4b700c,result-24);return;}
    if(result>=66&&result<=73){
        HercUpgradeSelectCategory(&g4b700c,result-66);
        U32 now=TimeNow();
        if((long)(now-gHercUpgradeLastClick)<333)result=64;
        gHercUpgradeLastClick=now;
    }
    switch(result){
    case 85:HercUpgradeAction85(&g4b700c);break;
    case 47:
        HercUpgradeAction47(&g4b700c);HercUpgradeClose(&g4b700c);
        ShellStorageReopen(&g4b5b50,&input);screen=27;break;
    case 50:HercUpgradeAction50(&g4b700c);break;
    case 49:HercUpgradeAction49(&g4b700c);break;
    case 54:HercUpgradeRefresh(&g4b700c);break;
    case 57:HercUpgradeAction57(&g4b700c);break;
    case 59:HercUpgradeRefresh(&g4b700c);break;
    case 64:HercUpgradeAction64(&g4b700c);break;
    case 65:HercUpgradeAction65(&g4b700c);break;
    case 52:HercUpgradeAction52(&g4b700c);break;
    case 53:
        HercUpgradeAction53(&g4b700c);HercUpgradeRefresh(&g4b700c);HercUpgradeAction48(&g4b700c);
        if(HercUpgradeClose(&g4b700c)){
            ShellStorageReopen(&g4b5b50,&input);screen=27;
        }else{
            ShellStorageClose(&g4b5b50);HercBayOpen(&g4b37c4,&input);screen=9;
        }
        break;
    case 58:HercUpgradeAction58(&g4b700c);break;
    case 48:
        HercUpgradeAction48(&g4b700c);HercUpgradeAction48(&g4b700c);HercUpgradeClose(&g4b700c);
        ShellStorageReopen(&g4b5b50,&input);screen=27;break;
    }
}
void Shell::UpdateMainMenuSelection()
{
    long hit=ShellMainMenuHitTest(&g4aebe0);
    if(!InputButtonPressed(1))hit=-1;
    if(hit>=0){
        SoundPlay(5,0);
        switch(hit){
        case 3:
            if(ShellMainMenuCanLaunch(&g4aebe0)){PlayTravel(1);Close(6);return;}
            break;
        case 0:
            ShellMainMenuClose(&g4aebe0);ShellSelectionOpen(&g4aff6c,&input,"Bioderm");screen=8;break;
        case 1:
            ShellMainMenuClose(&g4aebe0);((HercBay *)&g4b37c4)->Start(&input);screen=9;break;
        case 2:ShellMainMenuClose(&g4aebe0);BeginComms(2);break;
        }
    }
    U32 result=ShellMainMenuExit(&g4aebe0);
    if(!result&&ShellInputKey()==1)result=6;
    if(!result)return;
    switch(result){
    case 2:
        ShellMainMenuClose(&g4aebe0);ShellSelectionOpen(&g4aff6c,&input,"Bioderm");screen=8;break;
    case 3:
        ShellMainMenuClose(&g4aebe0);((HercBay *)&g4b37c4)->Start(&input);screen=9;break;
    case 4:ShellMainMenuClose(&g4aebe0);BeginComms(2);break;
    case 9:case 10:case 12:ShellMainMenuAction9(&g4aebe0);break;
    case 6:ShellMainMenuAction6(&g4aebe0);break;
    case 11:ShellMainMenuAction11(&g4aebe0);
    case 5:
        if(ShellMainMenuCanLaunch(&g4aebe0)){PlayTravel(1);Close(6);}break;
    case 7:ShellLeaveSession(0);
    case 8:
        ShellMainMenuClose(&g4aebe0);TimerSet(&gShellIdleTimer,120000);
        profileManager.SelectScenario(1);ShellIntroStart(&g4afa84,&input,0);screen=1;break;
    }
}
void Shell::UpdateBiodermPanel()
{
    if(BiodermReturnToList()){
        BiodermSkillsClose(&g4b0c50);
        ((BiodermPanelState *)&g4b0c50)->returning=1;
        ShellSelectionSkills(&g4aff6c,&input,"Bioderm");screen=8;return;
    }
    U32 result=BiodermSkillsExit(&g4b0c50);
    if(!result)return;
    switch(result){
    case 6:
        BiodermSkillsClose(&g4b0c50);
        ((BiodermPanelState *)&g4b0c50)->returning=gBiodermSelectionMode=1;
        ShellSelectionSkills(&g4aff6c,&input,"Bioderm");screen=8;break;
    case 7:
        BiodermSkillsApply(&g4b0c50);
        ((BiodermPanelState *)&g4b0c50)->returning=gBiodermSelectionMode=1;
        ShellSelectionSkills(&g4aff6c,&input,"Bioderm");screen=8;break;
    case 1:BiodermSkillsBatch(&g4b0c50,0x73,0x74);break;
    case 3:BiodermSkillsBatch(&g4b0c50,0x77,0x78);break;
    case 2:BiodermSkillsBatch(&g4b0c50,0x75,0x76);break;
    case 4:BiodermSkillsAction6(&g4b0c50);break;
    case 5:BiodermSkillsAction7(&g4b0c50);break;
    }
}
void Shell::UpdateMultiplayerMenu()
{
    long hit=ShellMultiplayerHitTest(&g4b82fc);
    if(!InputButtonPressed(1))hit=-1;
    if(hit>=0){
        SoundPlay(5,0);
        ++hit; switch(hit){
        case 0:ExploIdle();break;
        case 4:if(ShellMultiplayerCanStart(&g4b82fc)){
            if(StartMultiplayer(uiSystem.sessionName,0)){
                ShellMultiplayerClose(&g4b82fc);ShellNetworkSetupBegin(&g4ba2bc,&input);screen=37;
                strncpy(gNetworkPlayerName,sessionName,13);gNetworkPlayerName[12]=0;
            }else{
                ShellMultiplayerClose(&g4b82fc);ShellMultiplayerOpen(&g4b82fc,&input,0);auxiliaryState=35;
            }
        }break;
        case 1:
            ShellMultiplayerClose(&g4b82fc);ShellSelectionOpen(&g4aff6c,&input,"Bioderm");screen=8;break;
        case 2:
            ShellMultiplayerClose(&g4b82fc);((HercBay *)&g4b37c4)->Start(&input);screen=9;break;
        }
        return;
    }
    hit=ShellMultiplayerExit(&g4b82fc);
    if(!hit&&ShellInputKey()==1)hit=6;
    if(!hit)return;
    ++hit;switch(hit){
    case 1:case 2:case 5:case 12:default:break;
    case 0:ExploIdle();break;
    case 3:
        ShellMultiplayerClose(&g4b82fc);ShellSelectionOpen(&g4aff6c,&input,"Bioderm");screen=8;break;
    case 4:
        ShellMultiplayerClose(&g4b82fc);((HercBay *)&g4b37c4)->Start(&input);screen=9;break;
    case 6:if(ShellMultiplayerCanStart(&g4b82fc)){
            if(StartMultiplayer(uiSystem.sessionName,0)){
                ShellMultiplayerClose(&g4b82fc);ShellNetworkSetupBegin(&g4ba2bc,&input);screen=37;
                strncpy(gNetworkPlayerName,sessionName,13);gNetworkPlayerName[12]=0;
            }else{
                ShellMultiplayerClose(&g4b82fc);ShellMultiplayerOpen(&g4b82fc,&input,0);auxiliaryState=35;
            }
        }break;
    case 14:ShellMultiplayerReset(&g4b82fc);ShellMultiplayerRefresh(&g4b82fc);break;
    case 10:case 11:case 13:ShellMultiplayerReset(&g4b82fc);break;
    case 7:
        if(ShellMultiplayerBusy(&g4b82fc)||ShellMultiplayerPending(&g4b82fc))break;
    case 9:
show_intro:
        ShellMultiplayerClose(&g4b82fc);TimerSet(&gShellIdleTimer,120000);
        profileManager.SelectScenario(1);ShellIntroStart(&g4afa84,&input,0);screen=1;break;
    case 8:
        if(!HasMission(&profileManager))profileManager.PrepareRoster();
        ShellLeaveSession(0);goto show_intro;
    }
}
void Shell::UpdateNetworkSetup()
{
    U32 result=ShellNetworkSetupExit(&g4ba2bc);
    if(!result)return;
    switch(result){
    case 2:
        if(ShellNetworkSetupCanStart(&g4ba2bc)){gMenuNetwork.Begin();MultiplayerLaunchClose(&g4ba2bc);}break;
    case 100:gMenuNetwork.Begin();MultiplayerLaunchClose(&g4ba2bc);break;
    case 85:ShellNetworkSetupAction85(&g4ba2bc);break;
case 87:ShellNetworkSetupAction85(&g4ba2bc);break;
case 101:ShellNetworkSetupAction85(&g4ba2bc);break;
case 102:ShellNetworkSetupAction85(&g4ba2bc);break;
    case 86:ShellNetworkSetupAction85(&g4ba2bc);ShellNetworkSetupJoinPlayer(&g4ba2bc,gNetworkPlayerSelection);break;
    case 4:ShellNetworkSetupAction4(&g4ba2bc);break;
    case 5:
        MultiplayerLaunchClose(&g4ba2bc);
        switch(LogicMode()){
        case 1:
            screen=5;ShellMainMenuClose(&g4aebe0);ShellMainMenuOpen(&g4aebe0,&input,0);auxiliaryState=5;break;
        case 2:case 3:
            screen=35;ShellMultiplayerClose(&g4b82fc);ShellMultiplayerOpen(&g4b82fc,&input,0);auxiliaryState=35;break;
        }
        StopMultiplayer(0,1);break;
    case 78:ShellNetworkSetupAction78(&g4ba2bc);break;
    case 79:ShellNetworkSetupAction79(&g4ba2bc);break;
    case 105:ShellNetworkSetupAction105(&g4ba2bc);break;
    case 3:if(LoadMission(0,1))ShellNetworkSetupLoaded(&g4ba2bc);break;
    }
    if(result>=62&&result<=69)ShellNetworkSetupRefresh(&g4ba2bc);
    if(result>=38&&result<=45){gNetworkPlayerSelection=result-38;ShellNetworkSetupSelectPlayer(&g4ba2bc,gNetworkPlayerSelection);}
    if(result>=70&&result<=77)ShellNetworkSetupAction70(&g4ba2bc);
}
void Shell::UpdateLaunch()
{
    long result=HotseatExit(gShellHotseat)-1;
    if(result<0)return;
    switch(result){
    case 1:if(HotseatCanLaunch(gShellHotseat)){HotseatClose(gShellHotseat);Close(10);}break;
    case 3:
        HotseatClose(gShellHotseat);profileManager.SelectScenario(1);ShellIntroStart(&g4afa84,&input,0);screen=1;break;
    case 56:((HotseatState *)gShellHotseat)->option^=1;HotseatRefresh(gShellHotseat);break;
    case 57:
        ((HotseatState *)gShellHotseat)->difficulty++;
        if(((HotseatState *)gShellHotseat)->difficulty>=5)((HotseatState *)gShellHotseat)->difficulty=0;
        HotseatRefresh(gShellHotseat);break;
    case 75:((HotseatState *)gShellHotseat)->option2^=1;HotseatRefresh(gShellHotseat);break;
    case 2:HotseatAction2(gShellHotseat);break;
    }
    if(result>=32&&result<=39)HotseatSelectTeam(gShellHotseat,result-32);
    if(result>=40&&result<=47)HotseatSelectColor(gShellHotseat,result-40);
    if(result>=48&&result<=55)HotseatSelectPlayer(gShellHotseat,result-48);
}
/* Bioderm tab strip shared by list, skills, statistics and details screens. */
void Shell::UpdateMap()
{
    if(((BiodermSelectionState *)&g4aff6c)->command==208)return;
    long hit=ShellSelectionTabHitTest(&g4b16a4);
    if(!input.NewPress())hit=-1;
    if(hit<0){
        switch(ShellInputKey()){
        case 48:hit=3;break;
        case 50:hit=0;break;
        case 47:hit=1;break;
        case 38:hit=2;break;
        case 19:hit=5;break;
        }
    }
    if(hit>=0){
        switch(hit){
        case 3:
            if(screen==12){
                BiodermListClose(&g4b01c8);gBiodermSelectionMode=1;((BiodermPanelState *)&g4b01c8)->returning=2;
                ShellSelectionList(&g4aff6c,&input,"Bioderm");screen=8;gBiodermSelectionDirty=0;
            }else{
                ShellSelectionTabReset(&g4b16a4);ShellSelectionChooseTab(&g4aff6c,4);((BiodermPanelState *)&g4b01c8)->returning=0;
            }break;
        case 0:
            if(screen==15){
                BiodermSkillsClose(&g4b0c50);((BiodermPanelState *)&g4b0c50)->returning=gBiodermSelectionMode=1;
                ShellSelectionSkills(&g4aff6c,&input,"Bioderm");screen=8;
            }else{
                ShellSelectionTabReset(&g4b16a4);ShellSelectionChooseTab(&g4aff6c,1);((BiodermPanelState *)&g4b0c50)->returning=0;
            }break;
        case 1:
            if(screen==17){
                BiodermStatsClose(&g4b07f8);((BiodermPanelState *)&g4b07f8)->returning=gBiodermSelectionMode=1;
                ShellSelectionStats(&g4aff6c,&input,"Bioderm");screen=8;
            }else{
                ShellSelectionTabReset(&g4b16a4);ShellSelectionChooseTab(&g4aff6c,2);((BiodermPanelState *)&g4b07f8)->returning=0;
            }break;
        case 2:
            if(screen==16){
                BiodermDetailsClose(&g4b0e18);((BiodermPanelState *)&g4b0e18)->returning=gBiodermSelectionMode=1;
                ShellSelectionDetails(&g4aff6c,&input,"Bioderm");screen=8;
            }else{
                ShellSelectionTabReset(&g4b16a4);ShellSelectionChooseTab(&g4aff6c,3);((BiodermPanelState *)&g4b0e18)->returning=0;
            }break;
        }
    }
    U32 result=ShellSelectionTabExit(&g4b16a4);
    if(!result){switch(ShellInputKey()){case 51:result=2;break;case 52:result=1;break;}}
    if(!result)return;
    switch(result){
    case 1:ShellSelectionPrevious(&g4b16a4);break;
    case 2:ShellSelectionNext(&g4b16a4);break;
    case 30:ShellSelectionTabAction30(&g4b16a4);break;
    }
}
/* Star-map exit and click handling; the working method name predates this body. */
void Shell::UpdateMultiplayer()
{
    U32 result=ShellMapExit(&g4afbfc);
    if(!result&&ShellInputKey()){
        if(!((ShellMapState *)&g4afbfc)->inputEnabled)((ShellMapState *)&g4afbfc)->zoom=100;
        else result=1;
    }
    if(result){
    switch(result){
    case 1:
        if(ShellMapBlocked(&g4afbfc))break;
        if(status){
            status=0;ShellMapClose(&g4afbfc);TimerSet(&gShellIdleTimer,120000);TimerSet(&gShellAnimationTimer,9000);
            ShellIntroStart(&g4afa84,&input,0);screen=1;
        }else{
            ShellMapClose(&g4afbfc);
            if(auxiliaryState==10){
                BeginComms(6);
            }else{
                if(gShellMapMode){SoundTrigger(141,0);gShellMapMode=0;}
                RestoreMenu(1);
            }
        }break;
    }
    }
    long hit=ShellMapHitTest(&g4afbfc);
    if(input.NewPress()){
        if(!((ShellMapState *)&g4afbfc)->inputEnabled){((ShellMapState *)&g4afbfc)->zoom=100;hit=-1;}
    }else hit=-1;
    if(hit>=0){
        switch(hit){
        case 0:
            if(ShellMapBlocked(&g4afbfc))break;
            ShellMapClose(&g4afbfc);
            if(status){
                status=0;profileManager.SelectScenario(1);displayedCredits=g4d0413;unknown1d=0;
                TimerSet(&gShellIdleTimer,120000);TimerSet(&gShellAnimationTimer,9000);
                screen=1;ShellIntroStart(&g4afa84,&input,0);
            }else if(auxiliaryState==10){
                BeginComms(6);
            }else{
                if(gShellMapMode){SoundTrigger(141,0);gShellMapMode=0;}
                RestoreMenu(resumeReason);
            }break;
        }
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
void Shell::UpdateCampaignMenu()
{
    if(BiodermReturnToList()){
        BiodermDetailsClose(&g4b0e18);
        ((BiodermPanelState *)&g4b0e18)->returning=1;
        ShellSelectionDetails(&g4aff6c,&input,"Bioderm");screen=8;
        return;
    }
    U32 result=BiodermDetailsExit(&g4b0e18);
    if(!result)return;
    switch(result){
    case 1:BiodermDetailsAction1(&g4b0e18);break;
    case 2:BiodermDetailsAction2(&g4b0e18);break;
    case 3:BiodermDetailsAction3(&g4b0e18);break;
    case 4:BiodermDetailsAction4(&g4b0e18);break;
    case 6:
        BiodermDetailsApply(&g4b0e18);
        ((BiodermPanelState *)&g4b0e18)->returning=gBiodermSelectionMode=1;
        ShellSelectionDetails(&g4aff6c,&input,"Bioderm");screen=8;
        break;
    case 5:
        BiodermDetailsClose(&g4b0e18);
        ((BiodermPanelState *)&g4b0e18)->returning=gBiodermSelectionMode=1;
        ShellSelectionDetails(&g4aff6c,&input,"Bioderm");screen=8;
        break;
    }
}
void Shell::UpdateMainMenu()
{

    if(TimerExpired(&gShellAnimationTimer)){
        if(gShellAttractReady){
            TimerSet(&gShellAnimationTimer,9000);
            ShellIntroNextImage(&g4afa84);
        }else{
            TimerSet(&gShellAnimationTimer,9000);
            gShellAttractReady=1;
        }
    }
    if(gGameSettings.moviesEnabled&&gShellAttractEnabled&&TimerExpired(&gShellIdleTimer)){
        ReplayIntro();
        TimerSet(&gShellAnimationTimer,9000);
        TimerSet(&gShellIdleTimer,120000);
    }else{
        U32 result=ShellIntroExit(&g4afa84);
        if(!result&&ShellInputKey()==1)result=5;
        if(result)switch(result){
        case 1:ShellIntroSetMode(&g4afa84,1);gShellAttractEnabled=0;break;
        case 2:MenuHelpSetContext(8);ShellIntroSetMode(&g4afa84,2);gShellAttractEnabled=0;break;
        case 3:ReplayIntro();TimerSet(&gShellAnimationTimer,9000);break;
        case 4:
            auxiliaryState=1;screen=32;ShellIntroClose(&g4afa84);
            ShellMapOpen(&g4afbfc,&input,2000);TimerSet(&gShellAnimationTimer,9000);break;
        case 5:
            GuiFadeOut();
            ShellClearFrame();
            if(MenuLoadBackground("SEQUEL.ART",&gShellBackground)){
                gShellEndingScreen=1;
                {Palette *palette=(Palette *)&gShellBackground;
    SurfaceCopyRegion(&gMenuSavedSurface,&g511948,0,0);
    if(gShellOverlayPaletteActive)PaletteCopyRange(palette,gShellOverlayColors,171,37);
    GuiFilterPalette(palette,gShellTintModeActive?3:1);
    ApplyPaletteData(palette);
}
                GuiPumpMessages();SoundPlay(30);SurfacePresent(&g513888);InputClearEvents();
                while(1){
                    gInDirty=1;InputPoll(&input);
                    if(ShellInputKey()||input.Released()||input.HelpReleased())break;
                    ShellServiceFrame(this);ShellLimitFrameRate(this);
                }
                GuiFadeOut();
                ShellClearFrame();
            }
            Close(0);break;
        case 6:profileManager.SetMode(1);gShellAttractReady=0;LoadMission(0,1);break;
        case 7:
            profileManager.SelectScenario(1);profileManager.SetMode(1);gShellAttractReady=0;
            ShellIntroSetMode(&g4afa84,3);break;
        case 8:
            profileManager.SetMode(0);gShellAttractReady=0;ShellIntroClose(&g4afa84);
            screen=34;ShellBriefingOpen(&g4b7904,&input);break;
        case 9:
            MenuHelpSetContext(5);gShellAttractReady=0;ShellIntroSetMode(&g4afa84,0);
            TimerSet(&gShellIdleTimer,120000);gShellAttractEnabled=1;break;
        case 13:gShellAttractReady=0;ShellIntroSetMode(&g4afa84,1);break;
        case 10:
            difficulty=0;gShellAttractReady=0;ShellIntroClose(&g4afa84);resumeReason=1;ResumeMenus();
            profileManager.SetProfileMode(0);GenerateMissions(&profileManager);break;
        case 11:
            gShellAttractReady=0;ShellIntroClose(&g4afa84);resumeReason=1;ResumeMenus();
            difficulty=1;profileManager.SetProfileMode(1);GenerateMissions(&profileManager);break;
        case 12:
            gShellAttractReady=0;ShellIntroClose(&g4afa84);resumeReason=1;ResumeMenus();
            difficulty=2;profileManager.SetProfileMode(2);GenerateMissions(&profileManager);break;
        case 16:
            profileManager.SetMode(3);
            if(LoadMission(0,1)){
                gShellAttractReady=0;ShellIntroClose(&g4afa84);resumeReason=0;RestoreMenu(resumeReason);
            }
            break;
        case 17:
            profileManager.SetMode(3);gShellAttractReady=0;ShellIntroClose(&g4afa84);
            resumeReason=1;RestoreMenu(resumeReason);break;
        case 18:ShellOpenHotseat(this);break;
        case 19:
            MenuHelpSetContext(5);profileManager.SelectScenario(1);gShellAttractReady=0;
            ShellIntroSetMode(&g4afa84,0);TimerSet(&gShellIdleTimer,120000);gShellAttractEnabled=1;break;
        case 14:case 15:default:break;
        }
    }
    long hit=ShellIntroTopMenu(&g4afa84);
    if(!input.NewPress())hit=-1;
    if(hit>=0)switch(hit){case 0:
        TimerSet(&gShellAnimationTimer,9000);ShellIntroNextImage(&g4afa84);break;
    }

}
void Shell::UpdateBiodermSkills()
{
    if(BiodermReturnToList()){
        BiodermStatsClose(&g4b07f8);
        ((BiodermPanelState *)&g4b07f8)->returning=1;
        ShellSelectionStats(&g4aff6c,&input,"Bioderm");screen=8;
        return;
    }
    U32 result=BiodermStatsExit(&g4b07f8);
    if(!result)return;
    switch(result){
    case 1:BiodermStatsSelect(&g4b07f8,6);break;
    case 2:BiodermStatsSelect(&g4b07f8,0);break;
    case 3:BiodermStatsSelect(&g4b07f8,1);break;
    case 4:BiodermStatsSelect(&g4b07f8,2);break;
    case 5:BiodermStatsSelect(&g4b07f8,3);break;
    case 6:BiodermStatsSelect(&g4b07f8,4);break;
    case 7:BiodermStatsSelect(&g4b07f8,5);break;
    case 8:BiodermStatsSelect(&g4b07f8,7);break;
    case 9:
        BiodermStatsClose(&g4b07f8);
        ((BiodermPanelState *)&g4b07f8)->returning=gBiodermSelectionMode=1;
        ShellSelectionStats(&g4aff6c,&input,"Bioderm");screen=8;break;
    case 10:
        SoundTrigger(96,0);BiodermStatsApply(&g4b07f8);
        ((BiodermPanelState *)&g4b07f8)->returning=gBiodermSelectionMode=1;
        ShellSelectionStats(&g4aff6c,&input,"Bioderm");screen=8;break;
    case 12:BiodermStatsAction(&g4b07f8);break;
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
    ShellIntroStart(&g4afa84,&input);
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
extern "C" void BriefWaitA(int);
extern int MenuLoadBackground(const char *,void *);
extern char gShellBackground;
extern void SurfaceFlush(void *);
extern void ShellServiceFrame(Shell *);
extern void ShellLimitFrameRate(Shell *);

extern void SoundStop(unsigned);




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
    gDrawRemap=map;
    SurfaceRemapRect(&g513888,x,y,width,height);
    GuiRemapClipRect(&gMenuClip,x,y,width,height);
    gDrawRemap=0;
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
    HercPrepareAssets();
    gShellStrings.Lock();missionImageName.Lock();
    SurfaceClear(&gMenuSavedSurface);GuiFadeOut();
    gShellMiniStrings.Unlock();gShellParaStrings.Unlock();
    SetPointer(&gShellMiniStrings,(void *)"minish.bin");
    SetPointer(&gShellParaStrings,(void *)"parash.bin");
    gShellMiniStrings.Lock();
    gShellImages=gResMgr.Load("shellgui.bmx");gMenuImages=(MenuImages *)gShellImages;
    f_cba0(&gShellFont,"Font3.fnx");GuiResetClip(&gMenuClip);
    memset(sessionName,0,30);strncpy(sessionName,gShellDefaultName,13);
    U32 mode=profileManager.Mode();
    switch(reason){
    case 0:case 2:
        if(gGameSettings.moviesEnabled)ShowIntro();
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
/* Takeoff movie, including its original palette literal, is STRICT. */
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
    BriefWaitA(6);
    HelpClearTopic();
    if(gGuiEffectPriority>50){
if(PlayTakeoff()==0){
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












