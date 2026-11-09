/* shell\mplaunch.c. Packed screen/state views follow the observed offsets.
 * Scene is the existing cross-TU screen label; unknown spans are not recovered. */
#include <string.h>
#include <stdio.h>
typedef unsigned long U32;
struct InputReq {U32 mask,state;int HelpPressed(){return (state&0xa0)==0x20;}};
struct Timer;extern void TimerSet(Timer *,U32);extern int TimerExpired(Timer *);
struct Timer {U32 start,end,duration;Timer(U32 n){TimerSet(this,n);}};
struct Surface {long width,height;unsigned char *pixels;U32 flags;Surface(U32);~Surface();};
struct FileRecord {U32 handle,open,flags,error;signed char direct,counted;unsigned char reserved[2];U32 position,length;FileRecord();~FileRecord();};
struct FlicSound;struct FlicPalette;
struct FrameHeader {long size;short magic,chunks;long reserved[2];};
struct Flic {
    Surface *target;Surface bitmap;U32 unknown14,unknown18;FlicSound *sounds;FileRecord file;
    FlicPalette *palette;U32 status,currentFrame,deadline;long loops;int active;U32 memoryPosition;
    unsigned char *resource;FrameHeader pending;U32 pendingRead;long fileSize,magic,frames,width,height,delay;
    U32 firstFrame,secondFrame;Flic();~Flic();
};
struct GuiControl;struct StrPool;
struct GuiRect {long x,y,width,height;};
struct GuiPoint {long x,y;};
struct GuiControlData:GuiRect {unsigned char frameStyle[4];union {U32 attributes,style,visibility;struct {unsigned char styleBytes[3],flags;};};U32 shortcut;};
class GuiControl:public GuiControlData {public:virtual ~GuiControl();virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);virtual int HitTest(const GuiPoint *);};
struct GuiScreen:GuiRect {
    InputReq *input;GuiControl **controls;StrPool *strings;unsigned count,focus;
    long hovered;unsigned pressed;U32 options;long firstString;int navigation,delayed;
    unsigned delayedControl;Timer repeatTimer;
    GuiScreen(GuiControl **,unsigned,U32); ~GuiScreen(){input=0;}
    void Invalidate();void Draw();int Update();long HitTest(const GuiPoint *);void LoadLayout(long);void Open(InputReq *,StrPool *,long,int);
};
#pragma pack(push,1)
struct Scene:GuiScreen {
    unsigned char unknown4c[0x3c];
    const char *playerNames[8];
    long selectedPlayer;
    long selectedIndex;
    unsigned char mode;
    int dirty;
    int playerDirty;
    int loaded;
    long playerStates[8],nameDirty[8],previousNames[8];
    int moneyDirty[8],limitDirty[8];
    unsigned char unknown15d[0x20];
    long teamValues[8];
    unsigned char playerFlags[8];
    int affordable[8],previousAffordable[8],eligible[8],previousEligible[8];
    long lastConfig,lastLimit;
    unsigned char unknown22d[0x14];
    int settingChanged[5];
    unsigned char unknown255[4];
    void *resource;
    long value25d,value261,value265,value269,value26d;
    long value271,value275,value279,value27d,value281;
    long costs[10];
    short durations[13];
    signed char teamSlots[8];
    Timer syncTimer,refreshTimer,soundTimer;
    int twoPlayersOnly;
    Flic movie;
    Scene();void Open(InputReq *);
    int IsActive(){return input!=0;}long SelectedPlayer(){return selectedPlayer;}
};
struct PlayerState {long state;unsigned char unknown04[0x40];char shortName[20];unsigned char unknown58[0x50];long peer;U32 limit,cost;long unknownb4;U32 ready;};
class MenuButton:public GuiControl {public: U32 state;const char *text; MenuButton(const char * =0,U32=0);virtual ~MenuButton();void Reset(unsigned);void SetText(const char *s){text=s;} void Invalidate(){flags&=~1;} void SetControlState(unsigned state){attributes=(attributes&0xff1fffffUL)+(state<<21);}};
typedef MenuButton LaunchReadyLabel;
class MenuLabel:public GuiControl {public:const char *text;long align;unsigned char font;int background;long color;MenuLabel(const char * =0,long=-1,unsigned char=1,long=0xb,int=1);virtual ~MenuLabel(); void Invalidate(){flags&=~1;} void SetControlState(unsigned state){attributes=(attributes&0xff1fffffUL)+(state<<21);} void SetText(const char *);void SetColor(long);void SetAlign(long);void SetBackground(int);void SetFont(unsigned char);};
class MenuNumber:public GuiControl {public: U32 unused; const char *text;long value;signed char font,color,flash,align;MenuNumber(signed char=-1,signed char=0x12);virtual ~MenuNumber(); void Invalidate(){flags&=~1;} void SetControlState(unsigned state){attributes=(attributes&0xff1fffffUL)+(state<<21);} void SetValue(long,signed char);void SetFont(signed char);};
class MenuTextButton:public GuiControl {public:U32 state,style;const char *text;signed char font,color,align;MenuTextButton(const char * =0,U32=0,signed char=1,signed char=2,signed char=-1);void Invalidate(){flags&=~1;} void SetControlState(unsigned state){attributes=(attributes&0xff1fffffUL)+(state<<21);}void SetColor(signed char);};
typedef MenuTextButton LaunchStatusLabel;
class MenuPrefixButton:public GuiControl {public:const char *text;U32 state;signed char font,color,align,prefix;MenuPrefixButton(const char * =0,signed char=1,signed char=2,signed char=-1);virtual ~MenuPrefixButton();void Invalidate(){flags&=~1;} void SetControlState(unsigned state){attributes=(attributes&0xff1fffffUL)+(state<<21);}void SetAlign(signed char);};
#pragma pack(pop)
extern PlayerState chat_player_state_table[];
extern int gLinkIsSigs;
inline int LaunchLinkType(){return gLinkIsSigs;}
inline int LaunchFirstPlayerState(){return chat_player_state_table[1].state;}
struct MenuNetwork {int localSlot,host;int BroadcastSlot(int,PlayerState *);int BroadcastLocal();int BroadcastInfo(void *);int Poll();};extern MenuNetwork gMenuNetwork;
extern char gameOptions;





struct StrPool {char *GetString(long,int=0);};extern StrPool gShellStrings;
extern long gLaunchScenario;extern int retryTimeout;extern long gLaunchDurationIndex,gLaunchSetting;
extern char gLaunchPlayerNames[8][80];
struct MenuMessage {unsigned char bytes[476];};
extern MenuMessage *MenuMessageBasic(MenuMessage *,int);
extern void MenuSend(const MenuMessage *,int);
extern int hostPeer;
struct MenuImage {U32 offset;unsigned short width,height;U32 flags;};
struct MenuImages {U32 count;MenuImage images[1];};
extern void *gShellImages;
extern char g513888;
extern void SurfacePresent(void *);
extern void MenuDrawImage(void *,const MenuImage *,long,long);
extern void BriefBlit(void *,int,int,long,long,long,long,long,long);
extern void RichTextDraw(long,long,long,long,const unsigned char *,long,long,long,long *,long *,const long *,int,int=1,U32=0,long=0,long=0x7fffffff);
extern unsigned char gLaunchTeams[9];
extern U32 gLaunchBudget;
extern long gLaunchConfig,gLaunchMap;
extern char *searchPaths[];
extern int FileExists(const char *);extern int LoadMission(const char *,int);
extern void Problem(long,int);
extern int MissionRandomTable[];extern void f_0047e200(int *);
extern "C" U32 next_link_seed(int *);
inline U32 LaunchRandomIndex(U32 count){return next_link_seed(MissionRandomTable)%count;}
extern long gLaunchRule;
extern int gLaunchOption105,gLaunchOption78;
extern unsigned char gLaunchRuleButton,gLaunchOption105Button,gLaunchOption78Button;
extern unsigned char gLaunchAloneCancel,gLaunchAloneAccept,gLaunchNotReadyCancel;
extern unsigned char gLaunchPlayerCancel,gLaunchPlayerAccept;
extern void LaunchRefreshRules(Scene *);
extern "C" void f_cb80(void *);
extern "C" void f_cbf0(void *);
extern char DAT_00511dcc;
extern void GuiFadeOut();
extern void ShellClearFrame();
extern GuiControl *gLaunchControls[105];

extern void LaunchRefreshBudgetLabels(Scene *),LaunchRefreshCostLabels(Scene *),LaunchRefreshPlayerLabels(Scene *),LaunchRefreshPlayerControls(Scene *);
void LaunchCheckConfiguration(Scene *);

void LaunchRefreshEligibility(Scene *screen);
void LaunchRefreshAffordability(Scene *screen);

extern void LaunchSetActiveButtons(Scene *);

int LaunchFindFreeTeamSlot(Scene *screen,int player);





extern char gLaunchMoneyText[8][20],gLaunchLimitText[8][4];
extern const char *gLaunchOption78Text,*gLaunchRuleText,*gLaunchOption105Text;

struct UpgradeRange {U32 value;int maximum;UpgradeRange(int,int);U32 Value(){return value;}}; typedef UpgradeRange LaunchChoice;

struct LaunchControlStyle {U32 bits;void SetControlState(unsigned state){bits=(bits&0xff1fffffUL)+(state<<21);}};
extern LaunchControlStyle gLaunchScenarioNextStyle,gLaunchScenarioPreviousStyle,gLaunchScenarioRandomStyle,gLaunchCostNextStyle,gLaunchCostPreviousStyle,gLaunchCostLabelStyle,gLaunchLimitNextStyle,gLaunchLimitPreviousStyle,gLaunchLimitLabelStyle,gLaunchSettingNextStyle,gLaunchSettingPreviousStyle,gLaunchSettingLabelStyle,gLaunchDurationNextStyle,gLaunchDurationPreviousStyle,gLaunchDurationLabelStyle;
extern LaunchControlStyle gLaunchDurationSummaryStyle;
extern LaunchControlStyle gLaunchSummary0Style,gLaunchSummary1Style,gLaunchSummary2Style,gLaunchSummary3Style,gLaunchCostNumberStyle,gLaunchBudgetNumberStyle,gLaunchTitleLabelStyle,gLaunchSettingNumberStyle,gLaunchSummary6Style,gLaunchSummary7Style,gLaunchOption78Style,gLaunchRuleStyle,gLaunchOption105Style,gLaunchOptionLabel0Style,gLaunchOptionLabel1Style,gLaunchOptionLabel2Style,gLaunchOptionLabel3Style,gLaunchPlayerNameEditStyle,gLaunchReadyErrorCancelStyle,gLaunchPlayerCancelStyle,gLaunchPlayerAcceptStyle,gLaunchAloneCancelStyle,gLaunchAloneAcceptStyle,gLaunchNotReadyCancelStyle,gLaunchStatusHeadingStyle;
void LaunchUpdateChoiceVisibility(Scene *);
extern int multiState,g4bb78c;extern long gLaunchHoveredControl;
extern void LinkSigsPoll();extern void NetworkPoll();extern void MenuHelpOpenTopic(long);
struct SoundSettings;extern int SoundPlay(unsigned,const SoundSettings * =0);extern int SoundTrigger(unsigned,int);
extern unsigned char gLaunchReadyErrorCancel;

class MenuChat:public GuiControl {public:U32 state,scrollStart,maxPosition,visibleLength,cursor;Timer sendTimer,blinkTimer,forceTimer;int pending;signed char font;long caretColor;MenuChat(U32=20);void Invalidate(){flags&=~1;} void SetControlState(unsigned state){attributes=(attributes&0xff1fffffUL)+(state<<21);}void Clear();};


struct ResMgr {void *Load(long,int=1,int=0,int=0);};extern ResMgr gResMgr;
struct Palette;extern char gShellBackground,g511948,gMenuSavedSurface;
extern void ShellApplyPalette(Palette *);extern void DecodeLaunchPalette(Palette *,const void *);
extern void SurfaceCopyRegion(void *,void *,long,long);
extern int viewportClipLeft,gBriefDrawSource;
extern "C" int f_cba0(void *,const char *);
extern "C" void AssertFail(const char *,int,const char *);
extern "C" void BriefWaitA(int);extern "C" void BriefWaitB(int);extern "C" void BriefSetMode(LaunchChoice *,long);
struct PlayerInfo {unsigned char kind,unknown01[39];long value;unsigned char unknown2c[4];};
struct ProfileManager;extern ProfileManager profileManager;extern void GetPlayerInfo(void *,PlayerInfo *);
void LaunchSetupControlStyles(Scene *);



#pragma pack(push,1)
struct MenuPanel {char object[36]; MenuPanel(); ~MenuPanel();};
class MenuEdit:public GuiControl {public:U32 state;char *text;U32 capacity,cursor;MenuEdit(char *,U32);};
struct MenuHelpRegion {char object[40]; MenuHelpRegion(); ~MenuHelpRegion();};

struct UpgradeBar {char object[40];UpgradeBar(UpgradeRange *,int);~UpgradeBar();};
struct UpgradeArrow {char object[44];UpgradeArrow(UpgradeRange *,int,int);~UpgradeArrow();};
#pragma pack(pop)

MenuPanel gLaunchPanel;
MenuPrefixButton gLaunchHeadings(0,1,2,-1);
MenuPrefixButton gLaunchHeading1(0,1,2,-1);
MenuPrefixButton gLaunchHeading2(0,1,2,-1);
MenuPrefixButton gLaunchHeading3(0,1,2,-1);
MenuNumber gLaunchSummaryNumbers(-1,0x12);
MenuNumber gLaunchSummary1(-1,0x12);
MenuNumber gLaunchSummary2(-1,0x12);
MenuNumber gLaunchSummary3(-1,0x12);
MenuNumber gLaunchSummary4(-1,0x12);
MenuNumber gLaunchCostNumber(-1,0x12);
MenuNumber gLaunchBudgetNumber(-1,0x12);
MenuNumber gLaunchTitleLabel(-1,0x12);
MenuNumber gLaunchSettingNumber(-1,0x12);
MenuNumber gLaunchDurationLabel(-1,0x12);
MenuNumber gLaunchSummaryNumber6(-1,0x12);
MenuNumber gLaunchSummaryNumber7(-1,0x12);
MenuLabel gLaunchMoneyLabels[8];
MenuLabel gLaunchLimitLabels[8];
MenuLabel gLaunchStatusHeading(0,-1,1,0xb,1);
UpgradeRange gLaunchScenarioChoice(0x17,0);
UpgradeBar gLaunchScenarioBar(&gLaunchScenarioChoice,0);
UpgradeArrow gLaunchScenarioNext(&gLaunchScenarioChoice,1,0x36);
UpgradeArrow gLaunchScenarioPrevious(&gLaunchScenarioChoice,-1,0x33);
MenuTextButton gLaunchStatusLabels[8];
MenuChat gLaunchChats[8];
MenuLabel gLaunchNameLabels[8];
MenuButton gLaunchReadyLabels[8];
MenuPrefixButton gLaunchTeamLabels[8];
MenuButton gLaunchAloneCancelButton(0,0);
MenuButton gLaunchAloneAcceptButton(0,0);
MenuButton gLaunchNotReadyCancelButton(0,0);
MenuLabel gLaunchOptionLabel0(0,-1,1,0xb,1);
MenuLabel gLaunchOptionLabel1(0,-1,1,0xb,1);
MenuLabel gLaunchOptionLabel2(0,-1,1,0xb,1);
MenuLabel gLaunchOptionLabel3(0,-1,1,0xb,1);
char gLaunchOwnedNameInput[40];
MenuEdit gLaunchPlayerNameEdit(gLaunchOwnedNameInput,0xf);
MenuButton gLaunchReadyErrorCancelButton(0,0);
MenuButton gLaunchPlayerCancelButton(0,0);
MenuButton gLaunchPlayerAcceptButton(0,0);
UpgradeRange gLaunchCostChoice(0xa,0);
UpgradeBar gLaunchCostBar(&gLaunchCostChoice,0);
UpgradeArrow gLaunchCostNext(&gLaunchCostChoice,1,0x36);
UpgradeArrow gLaunchCostPrevious(&gLaunchCostChoice,-1,0x33);
UpgradeRange gLaunchLimitChoice(0xd,0);
UpgradeBar gLaunchLimitBar(&gLaunchLimitChoice,0);
UpgradeArrow gLaunchLimitNext(&gLaunchLimitChoice,1,0x36);
UpgradeArrow gLaunchLimitPrevious(&gLaunchLimitChoice,-1,0x33);
UpgradeRange gLaunchSettingChoice(0xb,0);
UpgradeBar gLaunchSettingBar(&gLaunchSettingChoice,0);
UpgradeArrow gLaunchSettingNext(&gLaunchSettingChoice,1,0x36);
UpgradeArrow gLaunchSettingPrevious(&gLaunchSettingChoice,-1,0x33);
UpgradeRange gLaunchDurationChoice(0xd,0);
UpgradeBar gLaunchDurationBar(&gLaunchDurationChoice,0);
UpgradeArrow gLaunchDurationNext(&gLaunchDurationChoice,1,0x36);
UpgradeArrow gLaunchDurationPrevious(&gLaunchDurationChoice,-1,0x33);
MenuButton gLaunchOption105Control(0,0);
MenuButton gLaunchOption78Control(0,0);
MenuButton gLaunchRuleControl(0,0);
MenuPrefixButton gLaunchExitControl(0,1,2,-1);
MenuHelpRegion gLaunchHelpRegion;
Scene g4ba2bc;
/* Definitions follow original function order; unresolved bodies stay external. */
void LaunchResetPlayers(Scene *screen);
void LaunchResetValues(Scene *screen);
void LaunchInitChoices(Scene *screen);
void LaunchSendUpdates(Scene *screen);
void ShellNetworkSetupLoaded(Scene *screen);
void LaunchShuffleSlots(Scene *screen);
void LaunchRefreshSlots(Scene *screen);
void ShellNetworkSetupAction4(Scene *screen);
void LaunchRefreshReadyLabels(Scene *screen);
void ShellNetworkSetupAction79(Scene *screen);
void ShellNetworkSetupAction105(Scene *screen);
void ShellNetworkSetupAction78(Scene *screen);
void MultiplayerLaunchClose(Scene *screen);
int LaunchConnectionReady();
void MultiplayerLaunchRender(Scene *screen);
void LaunchCheckConfiguration(Scene *screen);
void LaunchRefreshAffordability(Scene *screen);
void LaunchRefreshEligibility(Scene *screen);
void LaunchRefreshPlayerControls(Scene *screen);
void LaunchShowMessage(Scene *screen,const unsigned char *message);
void LaunchDrawMessage(Scene *,const unsigned char *message);
void ShellNetworkSetupAction85(Scene *screen);
void LaunchClearPlayerFlags(Scene *screen);
signed char LaunchReadyCount(Scene *screen);
unsigned char LaunchPlayerCount();
int LaunchCanJoin(Scene *screen);
void LaunchMarkPlayerGone(Scene *screen);
void LaunchSetPlayerName(Scene *screen,unsigned player,const char *name);
void ShellNetworkSetupAction70(Scene *screen);
void LaunchAdvanceTeam(Scene *screen,int player);
void ShellNetworkSetupJoinPlayer(Scene *,int argument);
void LaunchDropMissingPlayers(Scene *screen);
int LaunchFindFreeTeamSlot(Scene *screen,int player);
Scene::Scene():GuiScreen(gLaunchControls,105,0),syncTimer(0),refreshTimer(0),soundTimer(0)
{
}
void Scene::Open(InputReq *input)
{
    if(this->IsActive()){





        AssertFail(__FILE__,__LINE__,"Tried to open MpLaunch, already opened...try restarting CyberStorm.");return;
    }
    this->twoPlayersOnly=LaunchLinkType()!=0;
    f_cba0(&DAT_00511dcc,"font4.fnx");
    if(multiState==5){this->selectedPlayer=2;this->selectedIndex=1;this->mode=3;}
    else{this->selectedPlayer=gMenuNetwork.localSlot;this->selectedIndex=this->selectedPlayer-1;this->mode=1;}
    BriefWaitA(7);BriefWaitB(8);
    this->dirty=0;this->playerDirty=0;this->loaded=0;
    *(GuiRect *)&viewportClipLeft=*(GuiRect *)&gBriefDrawSource;
    ShellClearFrame();gLaunchHoveredControl=-1;
    this->LoadLayout((long)"mplaunch.box");this->GuiScreen::Open(input,&gShellStrings,3701,1);
    gLaunchChats[this->selectedIndex].Clear();
    {int i;for(i=0;i<8;++i)memset(gLaunchPlayerNames[i],0,80);}
    ((MenuButton *)&(&gLaunchSummaryNumbers)[3])->MenuButton::Reset(8);((MenuButton *)&(&gLaunchSummaryNumbers)[4])->MenuButton::Reset(9);
    ((MenuButton *)&gLaunchCostNumber)->MenuButton::Reset(10);((MenuButton *)&gLaunchBudgetNumber)->MenuButton::Reset(11);
    ((MenuButton *)&gLaunchTitleLabel)->MenuButton::Reset(12);((MenuButton *)&gLaunchSettingNumber)->MenuButton::Reset(13);
    ((MenuButton *)&gLaunchDurationLabel)->MenuButton::Reset(14);((MenuButton *)&gLaunchSummaryNumber6)->MenuButton::Reset(15);((MenuButton *)&gLaunchSummaryNumber7)->MenuButton::Reset(16);
    (&gLaunchSummaryNumbers)[3].text=gShellStrings.GetString(3709,1);(&gLaunchSummaryNumbers)[4].text=gShellStrings.GetString(3710,1);
    gLaunchCostNumber.text=gShellStrings.GetString(3711,1);gLaunchBudgetNumber.text=gShellStrings.GetString(3712,1);
    gLaunchTitleLabel.text=gShellStrings.GetString(3713,1);gLaunchSettingNumber.text=gShellStrings.GetString(3714,1);
    {int i;for(i=0;i<8;++i)((GuiControl *)&gLaunchStatusLabels[i])->Reset(i+37);}
    LaunchSetupControlStyles(this);LaunchShuffleSlots(this);
    this->resource=0;this->resource=gResMgr.Load((long)"mpl.bmx");
    DecodeLaunchPalette((Palette *)&gShellBackground,(const unsigned char *)this->resource+16);
    ShellApplyPalette((Palette *)&gShellBackground);
    MenuDrawImage(&g513888,&((MenuImages *)this->resource)->images[0],0,0);
    SurfaceCopyRegion(&gMenuSavedSurface,&g511948,0,0);
    LaunchInitChoices(this);LaunchClearPlayerFlags(this);LaunchResetValues(this);LaunchResetPlayers(this);
    PlayerInfo player;GetPlayerInfo(&profileManager,&player);
    if(gMenuNetwork.host){
        BriefWaitB(9);chat_player_state_table[this->selectedPlayer].ready=1;
        gLaunchReadyLabels[this->selectedIndex].SetText("X");gLaunchReadyLabels[this->selectedIndex].Invalidate();
        gLaunchBudget=player.kind;BriefSetMode(&gLaunchLimitChoice,gLaunchBudget-1);
        if(gLaunchOption105&~1)gLaunchOption105=1;
        {int i=0;while(i<10){
            if((U32)this->costs[i]>chat_player_state_table[this->selectedPlayer].cost){
                gLaunchMap=i;gLaunchConfig=this->costs[i];BriefSetMode(&gLaunchCostChoice,i);break;
            }
            ++i;
        }}
        BriefSetMode(&gLaunchScenarioChoice,gLaunchScenario);BriefSetMode(&gLaunchDurationChoice,gLaunchDurationIndex);BriefSetMode(&gLaunchSettingChoice,gLaunchSetting);
        this->value25d=(unsigned char)gLaunchScenarioChoice.Value();this->value261=(unsigned char)gLaunchLimitChoice.Value();
        this->value265=(unsigned char)gLaunchDurationChoice.Value();this->value269=(unsigned char)gLaunchSettingChoice.Value();this->value26d=(unsigned char)gLaunchCostChoice.Value();
    }else{
        chat_player_state_table[this->selectedPlayer].ready=0;
        gLaunchReadyLabels[this->selectedIndex].SetText(" ");gLaunchReadyLabels[this->selectedIndex].Invalidate();this->loaded=1;
    }
    LaunchSetActiveButtons(this);LaunchRefreshRules(this);LaunchRefreshPlayerLabels(this);SoundTrigger(97,10);
}

void LaunchResetPlayers(Scene *screen)
{
    int i;
    for(i=0;i<8;++i)screen->teamSlots[i]=-1;
    for(i=0;i<8;++i){
        screen->teamValues[i]=9;screen->affordable[i]=1;screen->previousAffordable[i]=1;
        if(chat_player_state_table[i+1].state==1||chat_player_state_table[i+1].state==2){unsigned team=gLaunchTeams[i+1];screen->teamSlots[team]=(unsigned char)(i+1);}
        screen->nameDirty[i]=1;screen->previousNames[i]=99;
    }
    screen->lastConfig=0;screen->lastLimit=99;
}

void LaunchResetValues(Scene *screen)
{
    screen->value271=99;screen->value281=99;screen->value275=99;screen->value279=99;screen->value27d=99;
    if(screen->mode==3){
        screen->value265=0;screen->value261=0;screen->value269=0;screen->value26d=0;screen->value25d=0;
    }
}

void LaunchInitChoices(Scene *screen)
{
    screen->costs[0]=20000;screen->costs[1]=50000;screen->costs[2]=100000;screen->costs[3]=200000;
    screen->costs[4]=400000;screen->costs[5]=750000;screen->costs[6]=1000000;screen->costs[7]=3000000;
    screen->costs[8]=7500000;screen->costs[9]=15000000;
    screen->durations[0]=30;screen->durations[1]=45;screen->durations[2]=60;screen->durations[3]=90;
    screen->durations[4]=120;screen->durations[5]=180;screen->durations[6]=240;screen->durations[7]=300;
    screen->durations[8]=480;screen->durations[9]=600;screen->durations[10]=900;screen->durations[11]=1200;screen->durations[12]=65535;
}

void LaunchSendUpdates(Scene *screen)
{
    if(screen->mode==1){
        if(screen->playerDirty){
            gMenuNetwork.BroadcastSlot(screen->selectedPlayer,&chat_player_state_table[screen->selectedPlayer]);
            screen->playerDirty=0;
        }
        if(screen->loaded){gMenuNetwork.BroadcastLocal();screen->loaded=0;}
        if(screen->dirty&&TimerExpired(&screen->syncTimer)){
            gMenuNetwork.BroadcastInfo(&gameOptions);screen->dirty=0;TimerSet(&screen->syncTimer,250);
        }
    }
}

void ShellNetworkSetupLoaded(Scene *screen)
{
    if(screen->IsActive())screen->loaded=1;
}

void LaunchShuffleSlots(Scene *screen)
{
    U32 choices[8];
    if(gMenuNetwork.host){
        int i;
        for(i=0;i<8;++i)choices[i]=i;
        f_0047e200(MissionRandomTable);
        for(i=1;i<=8;){
            U32 index=LaunchRandomIndex(8);
            if(choices[index]<99){choices[index]=99;gLaunchTeams[i]=index;++i;}
        }
        screen->dirty=1;
    }
}

void LaunchRefreshSlots(Scene *screen)
{
    int i;
    for(i=0;i<8;++i){
        if(chat_player_state_table[i+1].state==1||chat_player_state_table[i+1].state==2){unsigned team=gLaunchTeams[i+1];screen->teamSlots[team]=(unsigned char)(i+1);}
    }
    if(screen->mode==1){
        for(i=1;i<=8;++i){
            unsigned team=gLaunchTeams[i];
            if(team!=screen->teamValues[i-1]&&team<8){
                screen->teamValues[i-1]=team;
                if(!screen->twoPlayersOnly||i<=2)MenuDrawImage(&g513888,&((MenuImages *)screen->resource)->images[team+25],139,i*26+24);
            }
        }
    }
}

void LaunchSetupControlStyles(Scene *)
{
    (&gLaunchHeadings)[0].SetAlign(2);(&gLaunchHeadings)[1].SetAlign(2);(&gLaunchHeadings)[2].SetAlign(2);(&gLaunchHeadings)[3].SetAlign(2);
    ((MenuNumber *)&gLaunchTitleLabel)->SetFont(0);
    (&gLaunchSummaryNumbers)[0].SetFont(0);(&gLaunchSummaryNumbers)[1].SetFont(0);(&gLaunchSummaryNumbers)[2].SetFont(0);
    (&gLaunchSummaryNumbers)[3].SetFont(0);(&gLaunchSummaryNumbers)[4].SetFont(0);
    ((MenuNumber *)&gLaunchTitleLabel)->SetFont(0);gLaunchSummaryNumber6.SetFont(0);gLaunchSummaryNumber7.SetFont(0);
    gLaunchOptionLabel0.SetBackground(0);gLaunchOptionLabel1.SetBackground(0);gLaunchOptionLabel2.SetBackground(0);gLaunchOptionLabel3.SetBackground(0);
    gLaunchOptionLabel0.SetColor(3);gLaunchOptionLabel1.SetColor(3);gLaunchStatusHeading.SetBackground(0);
    (&gLaunchSummaryNumbers)[1].SetControlState(5);
    for(unsigned char i=0;i<8;++i){
        gLaunchMoneyLabels[i].SetAlign(0);gLaunchLimitLabels[i].SetAlign(0);
        gLaunchMoneyLabels[i].SetBackground(0);gLaunchLimitLabels[i].SetBackground(0);gLaunchNameLabels[i].SetFont(4);
    }
}

void LaunchSetActiveButtons(Scene *screen)
{
    int i;
    switch(screen->mode){
    case 3:
        gLaunchAloneCancelStyle.SetControlState(5);
        gLaunchAloneAcceptStyle.SetControlState(5);
        gLaunchNotReadyCancelStyle.SetControlState(5);
        gLaunchPlayerCancelStyle.SetControlState(5);
        gLaunchPlayerAcceptStyle.SetControlState(5);
        gLaunchPlayerNameEditStyle.SetControlState(5);
        gLaunchReadyErrorCancelStyle.SetControlState(5);
        (&gLaunchHeadings)[0].SetControlState(5);
        (&gLaunchHeadings)[1].SetControlState(1);
        (&gLaunchHeadings)[2].SetControlState(1);
        (&gLaunchHeadings)[3].SetControlState(1);
        for(i=0;i<8;++i){
            gLaunchChats[i].SetControlState(5);
            gLaunchNameLabels[i].SetControlState(5);
            gLaunchStatusLabels[i].SetControlState(1);
            gLaunchMoneyLabels[i].SetControlState(1);
            gLaunchLimitLabels[i].SetControlState(1);
            gLaunchReadyLabels[i].SetControlState(5);
            gLaunchTeamLabels[i].SetControlState(5);
        }
        gLaunchSummary0Style.SetControlState(1);
        gLaunchSummary1Style.SetControlState(1);
        gLaunchSummary2Style.SetControlState(1);
        gLaunchSummary3Style.SetControlState(1);
        gLaunchCostNumberStyle.SetControlState(1);
        gLaunchBudgetNumberStyle.SetControlState(1);
        gLaunchTitleLabelStyle.SetControlState(1);
        gLaunchSettingNumberStyle.SetControlState(1);
        gLaunchDurationSummaryStyle.SetControlState(1);
        gLaunchSummary6Style.SetControlState(1);
        gLaunchSummary7Style.SetControlState(1);
        gLaunchOption78Style.SetControlState(1);
        gLaunchRuleStyle.SetControlState(1);
        gLaunchOption105Style.SetControlState(1);
        gLaunchOptionLabel0Style.SetControlState(5);
        gLaunchOptionLabel1Style.SetControlState(5);
        gLaunchOptionLabel2Style.SetControlState(5);
        gLaunchOptionLabel3Style.SetControlState(5);
        gLaunchStatusHeadingStyle.SetControlState(1);
        LaunchUpdateChoiceVisibility(screen);break;
    case 2:
        gLaunchPlayerCancelStyle.SetControlState(0);
        gLaunchPlayerAcceptStyle.SetControlState(0);
        gLaunchAloneCancelStyle.SetControlState(5);
        gLaunchAloneAcceptStyle.SetControlState(5);
        gLaunchNotReadyCancelStyle.SetControlState(5);
        gLaunchPlayerNameEditStyle.SetControlState(5);
        gLaunchReadyErrorCancelStyle.SetControlState(5);
        (&gLaunchHeadings)[0].SetControlState(1);
        (&gLaunchHeadings)[1].SetControlState(1);
        (&gLaunchHeadings)[2].SetControlState(1);
        (&gLaunchHeadings)[3].SetControlState(1);
        for(i=0;i<8;++i){
            if(screen->twoPlayersOnly&&i>1)gLaunchStatusLabels[i].SetControlState(5);
            else gLaunchStatusLabels[i].SetControlState(1);
            gLaunchMoneyLabels[i].SetControlState(1);
            gLaunchLimitLabels[i].SetControlState(1);
            gLaunchReadyLabels[i].SetControlState(5);
            gLaunchNameLabels[i].SetControlState(5);
            gLaunchChats[i].SetControlState(5);
            gLaunchTeamLabels[i].SetControlState(5);
        }
        gLaunchSummary0Style.SetControlState(1);
        gLaunchSummary1Style.SetControlState(1);
        gLaunchSummary2Style.SetControlState(1);
        gLaunchSummary3Style.SetControlState(1);
        gLaunchCostNumberStyle.SetControlState(1);
        gLaunchBudgetNumberStyle.SetControlState(1);
        gLaunchTitleLabelStyle.SetControlState(1);
        gLaunchSettingNumberStyle.SetControlState(1);
        gLaunchDurationSummaryStyle.SetControlState(1);
        gLaunchSummary6Style.SetControlState(1);
        gLaunchSummary7Style.SetControlState(1);
        gLaunchOption78Style.SetControlState(1);
        gLaunchRuleStyle.SetControlState(1);
        gLaunchOption105Style.SetControlState(1);
        gLaunchOptionLabel0Style.SetControlState(5);
        gLaunchOptionLabel1Style.SetControlState(5);
        gLaunchOptionLabel2Style.SetControlState(5);
        gLaunchOptionLabel3Style.SetControlState(5);
        gLaunchStatusHeadingStyle.SetControlState(1);
        LaunchUpdateChoiceVisibility(screen);break;
    case 1:
        gLaunchPlayerNameEditStyle.SetControlState(5);
        gLaunchReadyErrorCancelStyle.SetControlState(5);
        gLaunchAloneCancelStyle.SetControlState(5);
        gLaunchAloneAcceptStyle.SetControlState(5);
        gLaunchNotReadyCancelStyle.SetControlState(5);
        gLaunchPlayerCancelStyle.SetControlState(5);
        gLaunchPlayerAcceptStyle.SetControlState(5);
        if(gMenuNetwork.host){
            (&gLaunchHeadings)[0].SetControlState(0);
            (&gLaunchHeadings)[1].SetControlState(0);
            (&gLaunchHeadings)[2].SetControlState(0);
            (&gLaunchHeadings)[3].SetControlState(0);
        for(i=0;i<8;++i){
            if(screen->twoPlayersOnly&&i>1){
                gLaunchNameLabels[i].SetControlState(5);
                gLaunchStatusLabels[i].SetControlState(5);
            }else{
                gLaunchNameLabels[i].SetControlState(0);
                gLaunchStatusLabels[i].SetControlState(0);
            }
            gLaunchChats[i].SetControlState(5);
            gLaunchReadyLabels[i].SetControlState(5);
            gLaunchTeamLabels[i].SetControlState(5);
            screen->playerStates[i]=99;
        }
            gLaunchNameLabels[screen->selectedIndex].SetControlState(5);
            gLaunchChats[screen->selectedIndex].SetControlState(0);
            gLaunchReadyLabels[screen->selectedIndex].SetControlState(0);
            gLaunchTeamLabels[screen->selectedIndex].SetControlState(0);
            gLaunchStatusLabels[screen->selectedIndex].SetControlState(1);
        gLaunchSummary0Style.SetControlState(0);
        gLaunchSummary1Style.SetControlState(0);
        gLaunchSummary2Style.SetControlState(0);
        gLaunchSummary3Style.SetControlState(0);
        gLaunchCostNumberStyle.SetControlState(0);
        gLaunchBudgetNumberStyle.SetControlState(0);
        gLaunchTitleLabelStyle.SetControlState(0);
        gLaunchSettingNumberStyle.SetControlState(0);
        gLaunchDurationSummaryStyle.SetControlState(0);
        gLaunchSummary6Style.SetControlState(0);
        gLaunchSummary7Style.SetControlState(0);
        gLaunchOption78Style.SetControlState(0);
        gLaunchRuleStyle.SetControlState(0);
        gLaunchOption105Style.SetControlState(0);
        gLaunchScenarioNextStyle.SetControlState(0);
        gLaunchScenarioPreviousStyle.SetControlState(0);
        gLaunchScenarioRandomStyle.SetControlState(0);
        gLaunchCostNextStyle.SetControlState(0);
        gLaunchCostPreviousStyle.SetControlState(0);
        gLaunchCostLabelStyle.SetControlState(0);
        gLaunchLimitNextStyle.SetControlState(0);
        gLaunchLimitPreviousStyle.SetControlState(0);
        gLaunchLimitLabelStyle.SetControlState(0);
        gLaunchSettingNextStyle.SetControlState(0);
        gLaunchSettingPreviousStyle.SetControlState(0);
        gLaunchSettingLabelStyle.SetControlState(0);
        gLaunchDurationNextStyle.SetControlState(0);
        gLaunchDurationPreviousStyle.SetControlState(0);
        gLaunchDurationLabelStyle.SetControlState(0);
        gLaunchStatusHeadingStyle.SetControlState(0);
            screen->value271=99;
        }else{
            (&gLaunchHeadings)[0].SetControlState(5);
            (&gLaunchHeadings)[1].SetControlState(0);
            (&gLaunchHeadings)[2].SetControlState(0);
            (&gLaunchHeadings)[3].SetControlState(0);
        for(i=0;i<8;++i){
            if(screen->twoPlayersOnly&&i>1){
                gLaunchNameLabels[i].SetControlState(5);
                gLaunchStatusLabels[i].SetControlState(5);
            }else{
                gLaunchNameLabels[i].SetControlState(0);
                gLaunchStatusLabels[i].SetControlState(1);
            }
            gLaunchChats[i].SetControlState(5);
            gLaunchMoneyLabels[i].SetControlState(5);
            gLaunchLimitLabels[i].SetControlState(5);
            gLaunchReadyLabels[i].SetControlState(5);
            gLaunchTeamLabels[i].SetControlState(1);
            screen->playerStates[i]=99;
        }
            gLaunchChats[screen->selectedIndex].SetControlState(0);
            gLaunchNameLabels[screen->selectedIndex].SetControlState(5);
            gLaunchReadyLabels[screen->selectedIndex].SetControlState(0);
            gLaunchTeamLabels[screen->selectedIndex].SetControlState(0);
        gLaunchSummary0Style.SetControlState(0);
        gLaunchSummary1Style.SetControlState(0);
        gLaunchSummary2Style.SetControlState(0);
        gLaunchSummary3Style.SetControlState(0);
        gLaunchCostNumberStyle.SetControlState(0);
        gLaunchBudgetNumberStyle.SetControlState(0);
        gLaunchTitleLabelStyle.SetControlState(0);
        gLaunchSettingNumberStyle.SetControlState(0);
        gLaunchDurationSummaryStyle.SetControlState(0);
        gLaunchSummary6Style.SetControlState(0);
        gLaunchSummary7Style.SetControlState(0);
        gLaunchOption78Style.SetControlState(1);
        gLaunchRuleStyle.SetControlState(1);
        gLaunchOption105Style.SetControlState(1);
        gLaunchOptionLabel0Style.SetControlState(5);
        gLaunchOptionLabel1Style.SetControlState(5);
        gLaunchOptionLabel2Style.SetControlState(5);
        gLaunchOptionLabel3Style.SetControlState(5);
        gLaunchScenarioNextStyle.SetControlState(5);
        gLaunchScenarioPreviousStyle.SetControlState(5);
        gLaunchScenarioRandomStyle.SetControlState(5);
        gLaunchCostNextStyle.SetControlState(5);
        gLaunchCostPreviousStyle.SetControlState(5);
        gLaunchCostLabelStyle.SetControlState(5);
        gLaunchLimitNextStyle.SetControlState(5);
        gLaunchLimitPreviousStyle.SetControlState(5);
        gLaunchLimitLabelStyle.SetControlState(5);
        gLaunchSettingNextStyle.SetControlState(5);
        gLaunchSettingPreviousStyle.SetControlState(5);
        gLaunchSettingLabelStyle.SetControlState(5);
        gLaunchDurationNextStyle.SetControlState(5);
        gLaunchDurationPreviousStyle.SetControlState(5);
        gLaunchDurationLabelStyle.SetControlState(5);
        }
        gLaunchStatusHeadingStyle.SetControlState(0);
        screen->value271=99;break;
    case 4:
        gLaunchAloneCancelStyle.SetControlState(0);
        gLaunchAloneAcceptStyle.SetControlState(0);
        gLaunchPlayerCancelStyle.SetControlState(5);
        gLaunchPlayerAcceptStyle.SetControlState(5);
        gLaunchPlayerNameEditStyle.SetControlState(5);
        gLaunchReadyErrorCancelStyle.SetControlState(5);
        (&gLaunchHeadings)[0].SetControlState(1);
        (&gLaunchHeadings)[1].SetControlState(1);
        (&gLaunchHeadings)[2].SetControlState(1);
        (&gLaunchHeadings)[3].SetControlState(1);
        for(i=0;i<8;++i){
            if(screen->twoPlayersOnly&&i>1)gLaunchStatusLabels[i].SetControlState(5);
            else gLaunchStatusLabels[i].SetControlState(1);
            gLaunchMoneyLabels[i].SetControlState(1);
            gLaunchLimitLabels[i].SetControlState(1);
            gLaunchNameLabels[i].SetControlState(5);
            gLaunchChats[i].SetControlState(5);
            gLaunchReadyLabels[i].SetControlState(5);
            gLaunchTeamLabels[i].SetControlState(5);
        }
        gLaunchSummary0Style.SetControlState(1);
        gLaunchSummary1Style.SetControlState(1);
        gLaunchSummary2Style.SetControlState(1);
        gLaunchSummary3Style.SetControlState(1);
        gLaunchCostNumberStyle.SetControlState(1);
        gLaunchBudgetNumberStyle.SetControlState(1);
        gLaunchTitleLabelStyle.SetControlState(1);
        gLaunchSettingNumberStyle.SetControlState(1);
        gLaunchDurationSummaryStyle.SetControlState(1);
        gLaunchSummary6Style.SetControlState(1);
        gLaunchSummary7Style.SetControlState(1);
        gLaunchOption78Style.SetControlState(1);
        gLaunchRuleStyle.SetControlState(1);
        gLaunchOption105Style.SetControlState(1);
        gLaunchStatusHeadingStyle.SetControlState(1);
        gLaunchOptionLabel0Style.SetControlState(5);
        gLaunchOptionLabel1Style.SetControlState(5);
        gLaunchOptionLabel2Style.SetControlState(5);
        gLaunchOptionLabel3Style.SetControlState(5);
        LaunchUpdateChoiceVisibility(screen);break;
    case 5:
        gLaunchNotReadyCancelStyle.SetControlState(0);
        gLaunchPlayerCancelStyle.SetControlState(5);
        gLaunchPlayerAcceptStyle.SetControlState(5);
        gLaunchPlayerNameEditStyle.SetControlState(5);
        gLaunchReadyErrorCancelStyle.SetControlState(5);
        (&gLaunchHeadings)[0].SetControlState(1);
        (&gLaunchHeadings)[1].SetControlState(1);
        (&gLaunchHeadings)[2].SetControlState(1);
        (&gLaunchHeadings)[3].SetControlState(1);
        for(i=0;i<8;++i){
            if(screen->twoPlayersOnly&&i>1)gLaunchStatusLabels[i].SetControlState(5);
            else gLaunchStatusLabels[i].SetControlState(1);
            gLaunchMoneyLabels[i].SetControlState(1);
            gLaunchLimitLabels[i].SetControlState(1);
            gLaunchNameLabels[i].SetControlState(5);
            gLaunchChats[i].SetControlState(5);
            gLaunchReadyLabels[i].SetControlState(5);
            gLaunchTeamLabels[i].SetControlState(5);
        }
        gLaunchSummary0Style.SetControlState(1);
        gLaunchSummary1Style.SetControlState(1);
        gLaunchSummary2Style.SetControlState(1);
        gLaunchSummary3Style.SetControlState(1);
        gLaunchCostNumberStyle.SetControlState(1);
        gLaunchBudgetNumberStyle.SetControlState(1);
        gLaunchTitleLabelStyle.SetControlState(1);
        gLaunchSettingNumberStyle.SetControlState(1);
        gLaunchDurationSummaryStyle.SetControlState(1);
        gLaunchSummary6Style.SetControlState(1);
        gLaunchSummary7Style.SetControlState(1);
        gLaunchStatusHeadingStyle.SetControlState(1);
        gLaunchOption78Style.SetControlState(1);
        gLaunchRuleStyle.SetControlState(1);
        gLaunchOption105Style.SetControlState(1);
        gLaunchOptionLabel0Style.SetControlState(5);
        gLaunchOptionLabel1Style.SetControlState(5);
        gLaunchOptionLabel2Style.SetControlState(5);
        gLaunchOptionLabel3Style.SetControlState(5);
        LaunchUpdateChoiceVisibility(screen);break;
    case 6:
        gLaunchAloneCancelStyle.SetControlState(5);
        gLaunchAloneAcceptStyle.SetControlState(5);
        gLaunchNotReadyCancelStyle.SetControlState(5);
        gLaunchPlayerCancelStyle.SetControlState(5);
        gLaunchPlayerAcceptStyle.SetControlState(5);
        gLaunchPlayerNameEditStyle.SetControlState(5);
        gLaunchReadyErrorCancelStyle.SetControlState(0);
        (&gLaunchHeadings)[0].SetControlState(5);
        (&gLaunchHeadings)[1].SetControlState(1);
        (&gLaunchHeadings)[2].SetControlState(1);
        (&gLaunchHeadings)[3].SetControlState(1);
        for(i=0;i<8;++i){
            if(screen->twoPlayersOnly&&i>1)gLaunchStatusLabels[i].SetControlState(5);
            else gLaunchStatusLabels[i].SetControlState(1);
            gLaunchNameLabels[i].SetControlState(5);
            gLaunchChats[i].SetControlState(5);
            gLaunchReadyLabels[i].SetControlState(5);
            gLaunchTeamLabels[i].SetControlState(5);
            gLaunchMoneyLabels[i].SetControlState(1);
            gLaunchLimitLabels[i].SetControlState(1);
        }
        gLaunchSummary0Style.SetControlState(1);
        gLaunchSummary1Style.SetControlState(1);
        gLaunchSummary2Style.SetControlState(1);
        gLaunchTitleLabelStyle.SetControlState(1);
        gLaunchSettingNumberStyle.SetControlState(1);
        gLaunchDurationSummaryStyle.SetControlState(1);
        gLaunchSummary6Style.SetControlState(1);
        gLaunchSummary7Style.SetControlState(1);
        gLaunchOption78Style.SetControlState(1);
        gLaunchRuleStyle.SetControlState(1);
        gLaunchOption105Style.SetControlState(1);
        gLaunchStatusHeadingStyle.SetControlState(1);
        gLaunchOptionLabel0Style.SetControlState(5);
        gLaunchOptionLabel1Style.SetControlState(5);
        gLaunchOptionLabel2Style.SetControlState(5);
        gLaunchOptionLabel3Style.SetControlState(5);
        gLaunchSummary3Style.SetControlState(5);
        gLaunchCostNumberStyle.SetControlState(5);
        gLaunchBudgetNumberStyle.SetControlState(5);
        LaunchUpdateChoiceVisibility(screen);break;
    default:















































































































































































































































































































































































































































































































        AssertFail(__FILE__,__LINE__,"impossible case value in setActiveButtons");break;
    }
}

void LaunchUpdateChoiceVisibility(Scene *)
{
    if(gMenuNetwork.host){
        gLaunchScenarioNextStyle.SetControlState(1);
        gLaunchScenarioPreviousStyle.SetControlState(1);
        gLaunchScenarioRandomStyle.SetControlState(1);
        gLaunchCostNextStyle.SetControlState(1);
        gLaunchCostPreviousStyle.SetControlState(1);
        gLaunchCostLabelStyle.SetControlState(1);
        gLaunchLimitNextStyle.SetControlState(1);
        gLaunchLimitPreviousStyle.SetControlState(1);
        gLaunchLimitLabelStyle.SetControlState(1);
        gLaunchSettingNextStyle.SetControlState(1);
        gLaunchSettingPreviousStyle.SetControlState(1);
        gLaunchSettingLabelStyle.SetControlState(1);
        gLaunchDurationNextStyle.SetControlState(1);
        gLaunchDurationPreviousStyle.SetControlState(1);
        gLaunchDurationLabelStyle.SetControlState(1);
    }else{
        gLaunchScenarioNextStyle.SetControlState(5);
        gLaunchScenarioPreviousStyle.SetControlState(5);
        gLaunchScenarioRandomStyle.SetControlState(5);
        gLaunchCostNextStyle.SetControlState(5);
        gLaunchCostPreviousStyle.SetControlState(5);
        gLaunchCostLabelStyle.SetControlState(5);
        gLaunchLimitNextStyle.SetControlState(5);
        gLaunchLimitPreviousStyle.SetControlState(5);
        gLaunchLimitLabelStyle.SetControlState(5);
        gLaunchSettingNextStyle.SetControlState(5);
        gLaunchSettingPreviousStyle.SetControlState(5);
        gLaunchSettingLabelStyle.SetControlState(5);
        gLaunchDurationNextStyle.SetControlState(5);
        gLaunchDurationPreviousStyle.SetControlState(5);
        gLaunchDurationLabelStyle.SetControlState(5);
    }
}

void ShellNetworkSetupAction4(Scene *screen)
{
    char path[256];int found=0;
    for(int i=0;!found&&searchPaths[i];++i){
        sprintf(path,"%sqstart\\MP%02d_%c.CBM",searchPaths[i],gLaunchMap,gLaunchBudget+64);
        if(FileExists(path))found=1;
    }
    if(found&&LoadMission(path,1))ShellNetworkSetupLoaded(screen);
    else Problem(303,0);
}

void LaunchRefreshBudgetLabels(Scene *screen)
{

    int refresh=0;int player=1;
    while(player<=8){
        if(chat_player_state_table[player].state==1||chat_player_state_table[player].state==2){
            if(screen->nameDirty[player-1]!=chat_player_state_table[player].cost){
                LaunchRefreshEligibility(screen);screen->moneyDirty[player-1]=1;
                screen->nameDirty[player-1]=chat_player_state_table[player].cost;
                sprintf(gLaunchMoneyText[player-1],"%d",screen->nameDirty[player-1]);
                gLaunchMoneyLabels[player-1].SetText(gLaunchMoneyText[player-1]);gLaunchMoneyLabels[player-1].Invalidate();
            }
            if(screen->previousNames[player-1]!=chat_player_state_table[player].limit){
                LaunchRefreshAffordability(screen);screen->limitDirty[player-1]=1;
                screen->previousNames[player-1]=chat_player_state_table[player].limit;
                sprintf(gLaunchLimitText[player-1],"%2d",screen->previousNames[player-1]);
                MenuLabel *label=&gLaunchLimitLabels[player-1];label->SetText(gLaunchLimitText[player-1]);label->flags&=~1;
            }
            if(screen->mode==1){
                gLaunchReadyLabels[player-1].SetControlState(1);
                gLaunchReadyLabels[screen->selectedIndex].SetControlState(0);
                gLaunchMoneyLabels[player-1].SetControlState(0);gLaunchLimitLabels[player-1].SetControlState(0);
            }
        }else{
            gLaunchMoneyLabels[player-1].SetControlState(5);gLaunchLimitLabels[player-1].SetControlState(5);
            if(TimerExpired(&screen->refreshTimer)){
                refresh=1;
                if(screen->mode==1){
                    long y=(player-1)*26+45;
                    BriefBlit(&g513888,1,0,442,y,442,y,190,22);
                    MenuLabel *label=&gLaunchNameLabels[player-1];label->SetText(" ");label->flags&=~1;
                    gLaunchReadyLabels[player-1].SetControlState(5);
                    BriefBlit(&g513888,1,0,1,y,1,y,25,24);
                }
            }
        }
        ++player;
    }
    if(refresh)TimerSet(&screen->refreshTimer,1200);

}

void LaunchRefreshCostLabels(Scene *screen)
{

    int player=1;
    while(player<=8){
        int invalid=0;
        if(chat_player_state_table[player].state==1||chat_player_state_table[player].state==2){
            if(screen->moneyDirty[player-1]){
                long y=player*26+21;MenuLabel *label=&gLaunchMoneyLabels[player-1];
                if(screen->eligible[player-1]){
                    MenuDrawImage(&g513888,&((MenuImages *)gShellImages)->images[69],541,y);label->SetColor(18);
                }else{
                    MenuDrawImage(&g513888,&((MenuImages *)gShellImages)->images[68],541,y);label->SetColor(14);invalid=1;
                }
                SurfacePresent(&g513888);
                gLaunchMoneyLabels[player-1].SetControlState(0);gLaunchMoneyLabels[player-1].Invalidate();
                screen->moneyDirty[player-1]=0;
            }
            if(screen->limitDirty[player-1]){
                long y=player*26+21;MenuLabel *label=&gLaunchLimitLabels[player-1];
                if(screen->affordable[player-1]){
                    MenuDrawImage(&g513888,&((MenuImages *)gShellImages)->images[71],460,y);label->SetColor(18);
                }else{
                    MenuDrawImage(&g513888,&((MenuImages *)gShellImages)->images[70],460,y);label->SetColor(14);invalid=1;
                }
                SurfacePresent(&g513888);
                gLaunchLimitLabels[player-1].SetControlState(0);screen->limitDirty[player-1]=0;
                gLaunchLimitLabels[player-1].Invalidate();
            }
            LaunchStatusLabel *status=&gLaunchStatusLabels[player-1];
            if(invalid){
                status->SetColor(14);chat_player_state_table[player].ready=0;
                LaunchReadyLabel *label=&gLaunchReadyLabels[player-1];label->text=" ";label->Invalidate();
            }else status->SetColor(2);
            gLaunchStatusLabels[player-1].Invalidate();
        }
        ++player;
    }

}

void LaunchRefreshReadyLabels(Scene *screen)
{
    if(screen->mode==1){
        for(int player=1;player<=8;++player){
            U32 ready=chat_player_state_table[player].ready;
            LaunchReadyLabel *label=&gLaunchReadyLabels[player-1];
            switch(ready){
            case 0:label->text=" ";label->flags&=~1;break;
            case 1:label->text="X";label->flags&=~1;break;
            }
        }
    }
}

void LaunchRefreshRules(Scene *screen)
{


    long id=gLaunchOption78+3978;gLaunchOption78Text=gShellStrings.GetString(id,0);gLaunchOption78Button&=~1;
    id=gLaunchRule+3973;gLaunchRuleText=gShellStrings.GetString(id,0);gLaunchRuleButton&=~1;
    id=gLaunchOption105+4077;gLaunchOption105Text=gShellStrings.GetString(id,0);gLaunchOption105Button&=~1;
    if(!gMenuNetwork.host){
        if(screen->mode==1){
            screen->value265=gLaunchDurationIndex;screen->value261=gLaunchBudget;screen->value269=gLaunchSetting;
            screen->value26d=gLaunchMap;screen->value25d=gLaunchScenario;
            gLaunchDurationLabel.text=gShellStrings.GetString(screen->value265+3834,1);
        }
        gLaunchBudgetNumber.SetValue(gLaunchBudget,0);gLaunchSettingNumber.SetValue(screen->value269,0);
        gLaunchCostNumber.SetValue(screen->costs[screen->value26d],0);
        BriefBlit(&g513888,1,0,500,340,500,340,140,120);
        MenuDrawImage(&g513888,&((MenuImages *)screen->resource)->images[screen->value25d+2],515,340);
        gLaunchTitleLabel.text=gShellStrings.GetString(screen->value25d+3811,1);
        gLaunchTitleLabel.flags&=~1;gLaunchSettingNumber.flags&=~1;gLaunchBudgetNumber.flags&=~1;
        gLaunchDurationLabel.flags&=~1;gLaunchCostNumber.flags&=~1;screen->Draw();SurfacePresent(&g513888);
    }
    LaunchRefreshReadyLabels(screen);LaunchRefreshPlayerLabels(screen);LaunchRefreshBudgetLabels(screen);LaunchRefreshCostLabels(screen);


}

void ShellNetworkSetupAction79(Scene *screen)
{
    if(++gLaunchRule>=5)gLaunchRule=0;
    screen->dirty=1;
    LaunchRefreshRules(screen);
    gLaunchRuleButton&=~1;
}

void ShellNetworkSetupAction105(Scene *screen)
{
    gLaunchOption105=!gLaunchOption105;
    screen->dirty=1;
    LaunchRefreshRules(screen);
    gLaunchOption105Button&=~1;
}

void ShellNetworkSetupAction78(Scene *screen)
{
    gLaunchOption78=!gLaunchOption78;
    screen->dirty=1;
    LaunchRefreshRules(screen);
    gLaunchOption78Button&=~1;
}

void MultiplayerLaunchClose(Scene *screen)
{
    if(screen->IsActive()){
        f_cb80(screen->resource);
        f_cbf0(&DAT_00511dcc);
        GuiFadeOut();ShellClearFrame();screen->input=0;
    }
}

int LaunchConnectionReady()
{
    if(LaunchLinkType()){
        int ready=LaunchFirstPlayerState();if(!ready)return ready;
    }
    return 1;
}

U32 ShellNetworkSetupExit(Scene *screen)
{
    if(screen->IsActive()){
        if(LaunchCanJoin(screen))return 5;
        LinkSigsPoll();
        if(screen->mode==3&&multiState==6){screen->mode=1;LaunchSetActiveButtons(screen);}
        if(!LaunchConnectionReady())return 5;
        LaunchReadyCount(screen);
        if(gMenuNetwork.host){
            screen->value25d=(unsigned char)gLaunchScenarioChoice.Value();screen->value261=(unsigned char)gLaunchLimitChoice.Value();
            screen->value265=(unsigned char)gLaunchDurationChoice.Value();screen->value269=(unsigned char)gLaunchSettingChoice.Value();screen->value26d=(unsigned char)gLaunchCostChoice.Value();
        }
        for(int i=0;i<5;++i){
            if((&screen->value25d)[i]!=(&screen->value271)[i]){
                screen->settingChanged[i]=1;(&screen->value271)[i]=(&screen->value25d)[i];
                if(gMenuNetwork.host)screen->dirty=1;
            }else screen->settingChanged[i]=0;
        }
        gLaunchHoveredControl=screen->HitTest((const GuiPoint *)&g4bb78c);
        if(screen->mode==1&&gLaunchHoveredControl==102&&screen->input->HelpPressed())MenuHelpOpenTopic(screen->value25d+3853);
        gMenuNetwork.Poll();NetworkPoll();
        if(TimerExpired(&screen->soundTimer)){
            for(int i=0;i<8;++i){if(!strcmp(gLaunchPlayerNames[i],"Elsa")){SoundPlay(349);break;}}
            TimerSet(&screen->soundTimer,6000);
        }
        return screen->Update();
    }
    return 0;
}

void MultiplayerLaunchRender(Scene *screen)
{
    if(screen->IsActive()){
        LaunchRefreshSlots(screen);
        if(gMenuNetwork.host){
            LaunchRefreshReadyLabels(screen);
            LaunchRefreshBudgetLabels(screen);LaunchRefreshCostLabels(screen);LaunchRefreshPlayerLabels(screen);
            if(screen->mode==1)LaunchRefreshPlayerControls(screen);
        }else{
            LaunchRefreshRules(screen);LaunchCheckConfiguration(screen);
        }
        screen->Draw();LaunchSendUpdates(screen);
    }
}

void LaunchCheckConfiguration(Scene *screen)
{
    if(screen->lastConfig!=gLaunchConfig){
        int player=1;while(player<=8){
            if(chat_player_state_table[player].state==1||chat_player_state_table[player].state==2)screen->moneyDirty[player-1]=1;
            ++player;
        }
        screen->lastConfig=gLaunchConfig;LaunchRefreshEligibility(screen);
    }
    if(screen->lastLimit!=gLaunchBudget){
        int player=1;while(player<=8){
            if(chat_player_state_table[player].state==1||chat_player_state_table[player].state==2)screen->limitDirty[player-1]=1;
            ++player;
        }
        screen->lastLimit=gLaunchBudget;LaunchRefreshAffordability(screen);
    }
}

void LaunchRefreshAffordability(Scene *screen)
{
    U32 budget=gLaunchBudget;
    for(int i=0;i<8;++i){
        if(budget>=chat_player_state_table[i+1].limit)screen->affordable[i]=1;else screen->affordable[i]=0;
        if(screen->previousAffordable[i]!=screen->affordable[i]){
            screen->previousAffordable[i]=screen->affordable[i];screen->limitDirty[i]=1;
        }
    }
}

void LaunchRefreshEligibility(Scene *screen)
{
    U32 budget=screen->costs[screen->value26d];
    for(int i=0;i<8;++i){
        if(budget>=chat_player_state_table[i+1].cost)screen->eligible[i]=1;else screen->eligible[i]=0;
        if(screen->previousEligible[i]!=screen->eligible[i]){
            screen->previousEligible[i]=screen->eligible[i];screen->moneyDirty[i]=1;
        }
    }
}

void LaunchRefreshPlayerControls(Scene *screen)
{
    for(unsigned char setting=0;setting<5;++setting){
        if(screen->settingChanged[setting])switch(setting){
        case 0:
            gLaunchScenario=screen->value25d;
            BriefBlit(&g513888,1,0,500,340,500,340,140,110);
            MenuDrawImage(&g513888,&((MenuImages *)screen->resource)->images[screen->value25d+2],515,340);
            gLaunchTitleLabel.text=gShellStrings.GetString(screen->value25d+3811,1);
            gLaunchTitleLabel.flags&=~1;screen->Draw();SurfacePresent(&g513888);break;
        case 1:
            gLaunchBudget=screen->value261+1;
            gLaunchBudgetNumber.SetValue(screen->value261+1,0);gLaunchBudgetNumber.flags&=~1;
            LaunchRefreshAffordability(screen);break;
        case 2:
            retryTimeout=screen->durations[screen->value265];gLaunchDurationIndex=screen->value265;
            gLaunchDurationLabel.text=gShellStrings.GetString(screen->value265+3834,1);
            gLaunchDurationLabel.flags&=~1;break;
        case 3:
            gLaunchSetting=screen->value269;gLaunchSettingNumber.SetValue(screen->value269,0);
            gLaunchSettingNumber.flags&=~1;break;
        case 4:
            gLaunchConfig=screen->costs[screen->value26d];gLaunchMap=screen->value26d;
            gLaunchCostNumber.SetValue(screen->costs[screen->value26d],0);gLaunchCostNumber.flags&=~1;
            LaunchRefreshEligibility(screen);break;
        }
    }
}

void LaunchShowMessage(Scene *screen,const unsigned char *message)
{
    long total,visible;
    MenuDrawImage(&g513888,&((MenuImages *)gShellImages)->images[66],200,150);SurfacePresent(&g513888);
    RichTextDraw(215,160,210,100,message,0,-1,0,&total,&visible,0,15);
    LaunchSetActiveButtons(screen);
}

void LaunchDrawMessage(Scene *,const unsigned char *message)
{
    long total,visible;
    RichTextDraw(215,160,210,100,message,0,-1,0,&total,&visible,0,15);
}

void ShellNetworkSetupAction85(Scene *screen)
{
    screen->mode=1;LaunchSetActiveButtons(screen);screen->Invalidate();
    BriefBlit(&g513888,1,0,200,150,200,150,250,150);
    screen->value261=gLaunchBudget;screen->value26d=gLaunchMap;
    LaunchRefreshAffordability(screen);LaunchRefreshEligibility(screen);
}

void LaunchClearPlayerFlags(Scene *screen)
{
    for(unsigned char player=0;player<8;++player)screen->playerFlags[player]=0;
}

void ShellNetworkSetupRefresh(Scene *screen)
{


    if(screen->mode!=1)return;
    if(!screen->eligible[screen->selectedIndex]&&!screen->affordable[screen->selectedIndex]){
        screen->mode=6;LaunchShowMessage(screen,(const unsigned char *)gShellStrings.GetString(3876,1));gLaunchReadyErrorCancel&=~1;return;
    }else if(!screen->eligible[screen->selectedIndex]){
        screen->mode=6;LaunchShowMessage(screen,(const unsigned char *)gShellStrings.GetString(3877,1));gLaunchReadyErrorCancel&=~1;return;
    }else if(!screen->affordable[screen->selectedIndex]){
        screen->mode=6;LaunchShowMessage(screen,(const unsigned char *)gShellStrings.GetString(3878,1));gLaunchReadyErrorCancel&=~1;return;
    }
    if(screen->eligible[screen->selectedIndex]&&screen->affordable[screen->selectedIndex])switch(chat_player_state_table[screen->selectedPlayer].ready){
    case 0:
        chat_player_state_table[screen->selectedPlayer].ready=1;
        gLaunchReadyLabels[screen->selectedIndex].text="X";gLaunchReadyLabels[screen->selectedIndex].flags&=~1;
        SoundTrigger(420,0);screen->loaded=1;break;
    case 1:
        chat_player_state_table[screen->selectedPlayer].ready=0;
        gLaunchReadyLabels[screen->selectedIndex].text=" ";gLaunchReadyLabels[screen->selectedIndex].flags&=~1;
        SoundTrigger(417,0);screen->loaded=1;break;
    default:
        chat_player_state_table[screen->selectedPlayer].ready=0;
        gLaunchReadyLabels[screen->selectedIndex].text=" ";gLaunchReadyLabels[screen->selectedIndex].flags&=~1;
        screen->loaded=1;break;
    }


}

signed char LaunchReadyCount(Scene *screen)
{
    signed char count=0;int player=1;
    if(gMenuNetwork.host&&screen->mode==1){
        while(player<=8){
            if((chat_player_state_table[player].state==1||chat_player_state_table[player].state==2)&&chat_player_state_table[player].ready)++count;
            ++player;
        }
    }
    if(count==1&&chat_player_state_table[1].ready==1)return -1;
    return count;
}

unsigned char LaunchPlayerCount()
{
    unsigned char count=0;
    for(int player=1;player<=8;++player){
        if(chat_player_state_table[player].state==1||chat_player_state_table[player].state==2)++count;
    }
    return count;
}

void LaunchRefreshPlayerLabels(Scene *screen)
{
    int changed=0;
    for(int player=1;player<=8;++player){
        long state=chat_player_state_table[player].state;
        if(state!=screen->playerStates[player-1]){
            LaunchStatusLabel *label=&gLaunchStatusLabels[player-1];
            switch(state){
            case 0:
                label->text=gShellStrings.GetString(3849,1);label->style=0;
                screen->nameDirty[player-1]=1;screen->previousNames[player-1]=99;
                break;
            case 1:case 2:
                screen->playerNames[player-1]=chat_player_state_table[player].shortName;
                label->text=screen->playerNames[player-1];label->style=1;break;
            case 6:
                label->text=gShellStrings.GetString(3850,1);label->style=2;
                screen->nameDirty[player-1]=1;screen->previousNames[player-1]=99;
                break;
            case 7:
                label->text=gShellStrings.GetString(3850,1);label->style=3;
                screen->nameDirty[player-1]=1;screen->previousNames[player-1]=99;
                break;
            }
            screen->playerStates[player-1]=state;changed=1;
        }
    }
    if(changed){
        BriefBlit(&g513888,1,0,25,45,25,45,111,205);
        for(int i=0;i<8;++i)gLaunchStatusLabels[i].Invalidate();
    }
}

void ShellNetworkSetupSelectPlayer(Scene *screen,int argument)
{
    if(gMenuNetwork.host){
        BriefBlit(&g513888,1,0,25,45,25,45,85,205);
        LaunchStatusLabel *label=&gLaunchStatusLabels[(unsigned char)argument];
        switch(label->style){
        case 0:
            chat_player_state_table[(unsigned char)argument+1].state=6;label->style=2;
            label->text=gShellStrings.GetString(3850,1);break;
        case 1:
            screen->mode=2;LaunchShowMessage(screen,(const unsigned char *)gShellStrings.GetString(3852,1));
            gLaunchPlayerCancel&=~1;gLaunchPlayerAccept&=~1;break;
        case 2:
            chat_player_state_table[(unsigned char)argument+1].state=0;label->style=0;
            label->text=gShellStrings.GetString(3850,1);break;
        case 3:
            chat_player_state_table[(unsigned char)argument+1].state=0;label->style=0;
            label->text=gShellStrings.GetString(3850,1);break;
        }
        for(int i=0;i<8;++i)gLaunchStatusLabels[i].Invalidate();
        int player=(unsigned char)argument+1;
        gMenuNetwork.BroadcastSlot(player,&chat_player_state_table[player]);
    }
}

void ShellNetworkSetupJoinPlayer(Scene *,int argument)
{
    unsigned char index=argument;
    MenuMessage message;MenuMessageBasic(&message,0x101f);
    unsigned player=index+1;
    MenuSend(&message,chat_player_state_table[player].peer);
    chat_player_state_table[player].state=7;
    LaunchStatusLabel *status=&gLaunchStatusLabels[index];status->style=3;
    status->text=gShellStrings.GetString(3851,1);
    chat_player_state_table[player].ready=0;
    LaunchReadyLabel *label=&gLaunchReadyLabels[index];label->text=" ";label->flags&=~1;
    gMenuNetwork.BroadcastSlot(chat_player_state_table[player].peer,&chat_player_state_table[player]);
    BriefBlit(&g513888,1,0,459,index*26+45,459,index*26+45,170,22);
}

int LaunchCanJoin(Scene *screen)
{
    if(screen->mode==1){
        switch(chat_player_state_table[screen->selectedPlayer].state){
        case 1:case 2:return 0;
        default:return 1;
        }
    }
    return 0;
}

void LaunchMarkPlayerGone(Scene *screen)
{
    chat_player_state_table[screen->selectedPlayer].state=7;
}

void LaunchSetPlayerName(Scene *screen,unsigned player,const char *name)
{
    if(screen->IsActive()&&player<=8){
        if(!player)player=1;
        strcpy(gLaunchPlayerNames[--player],name);
        gLaunchNameLabels[player].SetText(gLaunchPlayerNames[player]);gLaunchNameLabels[player].Invalidate();
    }
}

void ShellNetworkSetupAction70(Scene *screen)
{
    if(gMenuNetwork.host){
        int player=screen->SelectedPlayer();
        int next=LaunchFindFreeTeamSlot(screen,player);
        if(next>=0){
            for(int i=1;i<=8;++i){
                if(gLaunchTeams[i]==next){gLaunchTeams[i]=gLaunchTeams[player];break;}
            }
            gLaunchTeams[player]=next;screen->dirty=1;
        }
    }else{
        MenuMessage message;MenuMessageBasic(&message,0x1020);MenuSend(&message,hostPeer);
    }
}

void LaunchAdvanceTeam(Scene *screen,int player)
{
    int next=LaunchFindFreeTeamSlot(screen,player);
    if(next>=0){
        for(int i=1;i<=8;++i){
            if(gLaunchTeams[i]==next){gLaunchTeams[i]=gLaunchTeams[player];break;}
        }
        gLaunchTeams[player]=next;screen->dirty=1;
    }
}

int ShellNetworkSetupCanStart(Scene *screen)
{
    unsigned char count=LaunchPlayerCount();
    signed char ready=LaunchReadyCount(screen);
    if(count==1){
        screen->mode=4;
        LaunchShowMessage(screen,(const unsigned char *)gShellStrings.GetString(3847,1));
        gLaunchAloneCancel&=~1;gLaunchAloneAccept&=~1;
    }else if(ready==count){
        return 1;
    }else{
        screen->mode=5;
        LaunchShowMessage(screen,(const unsigned char *)gShellStrings.GetString(3848,1));
        gLaunchNotReadyCancel&=~1;
    }
    return 0;
}

void LaunchDropMissingPlayers(Scene *screen)
{
    for(int player=2;player<=8;++player){
        if((chat_player_state_table[player].state==1||chat_player_state_table[player].state==2)&&!chat_player_state_table[player].ready)ShellNetworkSetupJoinPlayer(screen,(unsigned char)player);
    }
}

int LaunchFindFreeTeamSlot(Scene *screen,int player)
{
    int tried=0;
    int previous=gLaunchTeams[player];
    int slot=previous;
    while(tried<8){
        if(slot==7){
            if(screen->teamSlots[0]==-1){
                screen->teamSlots[previous]=-1;screen->teamSlots[0]=player;return 0;
            }
        }else if(screen->teamSlots[slot+1]==-1){
            screen->teamSlots[previous]=-1;screen->teamSlots[slot+1]=player;return slot+1;
        }
        ++slot;if(slot==8)slot=0;++tried;
    }
    return -1;
}







