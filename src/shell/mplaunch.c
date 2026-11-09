/* shell\mplaunch.c. Packed screen/state views follow the observed offsets.
 * Scene is the existing cross-TU screen label; unknown spans are not recovered. */
#include <string.h>
#include <stdio.h>
typedef unsigned long U32;
struct InputReq {U32 mask,state;};
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
struct GuiScreen:GuiRect {
    InputReq *input;GuiControl **controls;StrPool *strings;unsigned count,focus;
    long hovered;unsigned pressed;U32 options;long firstString;int navigation,delayed;
    unsigned delayedControl;Timer repeatTimer;
    GuiScreen(GuiControl **,unsigned,U32);
    void Invalidate();void Draw();int Update();long HitTest(const GuiPoint *);
};
#pragma pack(push,1)
struct Scene:GuiScreen {
    unsigned char unknown4c[0x5c];
    long selectedPlayer;
    unsigned char unknownac[4];
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
    Scene();
    int IsActive(){return input!=0;}
};
struct PlayerState {long state;unsigned char unknown04[0xa4];long peer;U32 limit,cost;long unknownb4;U32 ready;};
struct LaunchReadyLabel {unsigned char unknown00[0x17],flags,unknown18[0xc];const char *text;};
class MenuLabel {public:unsigned char unknown00[0x17],flags,unknown18[0x19];void SetText(const char *);};
class MenuNumber {public:unsigned char unknown00[0x17],flags;void SetValue(long,signed char);};
struct LaunchStatusLabel {unsigned char unknown00[0x17],flags,unknown18[0xc];long style;const char *text;unsigned char unknown2c[3];};
#pragma pack(pop)
extern PlayerState chat_player_state_table[];
extern int gLinkIsSigs;
inline int LaunchLinkType(){return gLinkIsSigs;}
inline int LaunchFirstPlayerState(){return chat_player_state_table[1].state;}
struct MenuNetwork {int localSlot,host;int BroadcastSlot(int,PlayerState *);int BroadcastLocal();int BroadcastInfo(void *);};extern MenuNetwork gMenuNetwork;
extern char gameOptions;
extern LaunchReadyLabel gLaunchReadyLabels[8];
extern MenuLabel gLaunchNameLabels[8];
extern LaunchStatusLabel gLaunchStatusLabels[8];
extern MenuNumber gLaunchBudgetNumber,gLaunchCostNumber,gLaunchSettingNumber;
extern LaunchReadyLabel gLaunchTitleLabel,gLaunchDurationLabel;
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
void ShellNetworkSetupJoinPlayer(Scene *,unsigned char index);
void LaunchDropMissingPlayers(Scene *screen);
int LaunchFindFreeTeamSlot(Scene *screen,int player);

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

void ShellNetworkSetupJoinPlayer(Scene *,unsigned char index)
{
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
        gLaunchNameLabels[player].SetText(gLaunchPlayerNames[player]);gLaunchNameLabels[player].flags&=~1;
    }
}

void ShellNetworkSetupAction70(Scene *screen)
{
    if(gMenuNetwork.host){
        int player=screen->selectedPlayer;
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
