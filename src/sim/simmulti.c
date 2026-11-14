/* 00454210..00454990. Working names; unknown structure ranges are padding.
 * The constructor entry at 00454210 initializes the repeat timer. */
static char simmultiSourceFile[]=__FILE__;
#include <string.h>
#include <stdio.h>
#pragma pack(push, 1)
struct PlayerSlot { int state, owner; char unknown08[0xbc-8]; };
#include "actor.h"
struct SimGame {
    int active;
    int IsActive() { return active; }
    char unknown04[0x2ba-4]; int networkMode; char unknown2be[4];
    int timed; char unknown2c6[0x2de-0x2c6]; PlayerSlot players[9];
    char unknown97a[4]; int currentPlayer; char unknown982[4];
    int nextPlayer, localPlayer, unknown98e, localTurn;
    char unknown996[8]; int turn; int option;
    char unknown9a6[0x10e9-0x9a6]; int busy; int remaining; int timedExtra; int CanEndTurn() { return busy == 0; }
};

struct ViewRect { int left,top,right,bottom; };
struct MenuImage { unsigned long offset; unsigned short width,height; unsigned long flags; };
struct MenuImages { unsigned long count; MenuImage images[1]; };
struct Font { void *data; int IsOpen() { return data != 0; } };
struct StrPool { char *GetString(long, int = 0); };
struct ResMgr { void *Load(const char *, int = 1, void * = 0, unsigned long = 0); };
struct PacketList { void EndTurn(int); };
struct Timer;
struct GameTimer;
void GameTimerSet(GameTimer *, unsigned long);
struct GameTimer { unsigned long start,end,duration; GameTimer(unsigned long n) { GameTimerSet(this,n); } };
#pragma pack(pop)
extern char modeCheckName[], g4dd0c0[], g513888;
extern PacketList modeLogName;
extern int retryTimeout;
extern Timer gFogTimer;
extern unsigned long gIn80,gIn7c;
extern unsigned long gMenuColors[30],gMenuColorMap[30],gMenuTextColor;
extern unsigned long *gMenuTextMap;
struct ScreenDimensions { long width,height; }; extern ScreenDimensions g5138ac; extern int gRedrawFlags;
extern ViewRect viewportClipLeft,gBriefDrawSource;
extern GuiRect gMenuClip;
extern Font gShellFont,gDefaultFont;
extern unsigned long deathTextColor,deathTextMap[];
extern ResMgr gResMgr;
extern StrPool targetBriefResource;
unsigned long TimeNow();
unsigned long TimerRemaining(Timer *);
int GameTimerExpired(GameTimer *);
extern "C" int LogicCallMode(int,void *);
ActorRecord *ActorFindByOwner(int);
ActorRecord *ActorFindOwnerNext(ActorRecord *,int);
int SimHotbriefCheckActor(ActorRecord *);
void SimHotbriefClearActor(ActorRecord *);
void LogicSetActorLink(void *,int,int,int);
void SimStartPhase(SimGame *);
void SimChooseNextPlayer(SimGame *);
int ModeCheck(const char *);
extern "C" int f_cba0(void *,const char *);
extern "C" void f_cbf0(void *);
void SigsStart(void *,int);
void MenuSetColors(void *,unsigned long,long);
void MenuFill(void *,long,long,long,long);
void MenuSetClip(GuiRect *,long,long,long,long);
void MenuDrawImage(void *,const MenuImage *,long,long);
void MenuDrawTextAt(Font *,long,long,int,const char *);
void ReleasePoolData(void *);
void SurfacePresent(void *);
void GuiResetClip(void *);

static int lastTimerSecond;
static Font timerFont;
static unsigned long repeatStarted;
static int repeatCount;
static GameTimer repeatTimer(0);

void SimEliminatePlayer(SimGame *,int);

void SimResetPlayers(SimGame *game)
{
    for (int i = 0; i < 9; ++i) {
        if (game->players[i].state == 2) SimEliminatePlayer((SimGame *)modeCheckName,game->players[i].owner);
        if (game->players[i].state == 5) game->players[i].state = 3;
    }
}

void SimEliminatePlayer(SimGame *game,int player)
{
    if (player < 1 || !((SimGame *)modeCheckName)->IsActive()) return;
    if (game->players[player-1].state == 0 || game->players[player-1].state == 7) return;
    for (ActorRecord *a = ActorFindByOwner(player); a; a = ActorFindOwnerNext(a,player)) {
        a->owner = game->localPlayer;
        if (!SimHotbriefCheckActor(a)) SimHotbriefClearActor(a);
        else LogicSetActorLink(g4dd0c0,(int)a,6,0);
    }
    game->players[player-1].state = 0;
    if (player == game->currentPlayer) SimStartPhase(game);
}

int SimHasEarlierPlayer(SimGame *game,int player)
{
    for (int i = 1; i < player; ++i)
        if (game->players[i].state == 2 || game->players[i].state == 1) return 1;
    return 0;
}

int SimCountPlayers(SimGame *game)
{
    int count = 0;
    for (int i = 1; i < 9; ++i)
        if (game->players[i].state == 2 || game->players[i].state == 1) ++count;
    return count;
}

void ResetPlayer(PlayerSlot *player) { memset(player,0,sizeof(*player)); }

int SimHotReadTimer(SimGame *game)
{
    if (game->currentPlayer != game->localPlayer && (game->timed || game->timedExtra) && retryTimeout > 0)
        return TimerRemaining(&gFogTimer);
    return 216000000;
}

void SimDrawTimer(SimGame *game,int x,int y,int boxed,int overrideTime = -1)
{
    if ((game->timed || game->timedExtra) && retryTimeout > 0) {
        int time = SimHotReadTimer(game);
        if (overrideTime >= 0) time = overrideTime;
        if (time >= 216000000) return;
        time /= 1000;
        int hours = time/3600;
        int remainder = time-hours*3600;
        int minutes = remainder/60;
        remainder -= minutes*60;
        if (!timerFont.IsOpen() && !f_cba0(&timerFont,"font10.fnx")) return;
        int color = 16;
        if (time <= 15 && (time & 1)) {
            color = 14;
            if (game->localTurn && time != lastTimerSecond) LogicCallMode(5,0);
        }
        if (boxed) {
            if (color == 14) MenuSetColors(&g513888,gMenuColors[color],gMenuColors[0]);
            else MenuSetColors(&g513888,gMenuColors[0],gMenuColors[0]);
            MenuFill(&g513888,x-70,y-3,140,30);
            MenuSetClip(&gMenuClip,x-70,y-3,140,30);
        }
        char text[16];
        if (time >= 3600) sprintf(text,"%d:%02d:%02d",hours,minutes,remainder);
        else if (time >= 60) sprintf(text,"%d:%02d",minutes,remainder);
        else sprintf(text,"%d",remainder);
        gMenuTextMap = &gMenuColorMap[color];
        gMenuTextColor = gMenuColors[color];
        MenuDrawTextAt(&timerFont,x,y,!boxed,text);
        lastTimerSecond = time;
    }
}

void SimSetRepeat(SimGame *,int enabled)
{
    if (enabled) LogicCallMode(39,0);
    repeatStarted = enabled ? TimeNow() : 0;
    repeatCount = 0;
    GameTimerSet(&repeatTimer,2000);
}

struct RepeatInput { char unknown[8]; struct Buttons { int unused; unsigned long flags; int Has(unsigned long mask) { return (flags & mask) != 0; } } buttons; };
inline unsigned long In80() { return gIn80; }
inline unsigned long In7c() { return gIn7c; }
void SimPollRepeat(RepeatInput *input)
{
    if (repeatStarted) {
        if (In80() || In7c() || input->buttons.Has(16) || input->buttons.Has(32)) {
            repeatStarted = 0;
            return;
        }
        if (GameTimerExpired(&repeatTimer)) {
            LogicCallMode(11,0);
            ++repeatCount;
            int delay = 2000 - repeatCount * 200;
            if (delay < 500) delay = 500;
            GameTimerSet(&repeatTimer,delay);
        }
    }
}

void SimSetOption(SimGame *game,int option) { game->option = option; }

inline long ScreenHeight() { return g5138ac.height; }
void ModeBeginDiagnose()
{
    SigsStart(&g513888,0);
    viewportClipLeft = gBriefDrawSource;
    MenuImages *image = (MenuImages *)gResMgr.Load("BGREDHEX.BMX",1,0,0);
    MenuDrawImage(&g513888,image->images,(g5138ac.width-640)/2,(g5138ac.height-480)/2);
    ReleasePoolData(image);
    int loaded = gShellFont.IsOpen();
    if (!loaded) f_cba0(&gShellFont,"font3.fnx");
    int y = ScreenHeight()/2; long x = g5138ac.width/2;
    gMenuTextColor = deathTextColor;
    gMenuTextMap = deathTextMap;
    MenuDrawTextAt(&gShellFont,x,y-15,0,targetBriefResource.GetString(1409,0));
    MenuDrawTextAt(&gDefaultFont,x,y+15,0,targetBriefResource.GetString(1410,0));
    SurfacePresent(&g513888);
    GuiResetClip(&gMenuClip);
    if (!loaded) f_cbf0(&gShellFont);
    extern int simRedrawOther;
    simRedrawOther = -2;
    gRedrawFlags = -2;
}

void SimCheckEndTurn(SimGame *game)
{
    if (game->CanEndTurn() && game->localTurn && (game->networkMode == 1 || game->networkMode == 0)) {
        SimChooseNextPlayer(game);
        if (ModeCheck((const char *)game)) modeLogName.EndTurn(game->turn);
    }
}
