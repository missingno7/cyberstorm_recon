// sim\simout.c - simulation screen output (working labels; names are guesses unless bound)
#pragma pack(push, 1)
struct SimStat { long value, maximum, spare; };
#include "actor.h"
struct ViewRect { int left, top, right, bottom; };
struct GuiCanvas;
struct GuiScreen { void Invalidate(); };
struct FastBuffer;
struct StrPool { int Lock(); char *GetString(long, int = 0); };
struct ProfileManager { void BeginMissions(); void *Mission(int); };

struct MenuImage { unsigned long offset; unsigned short width, height; unsigned long flags; };
struct MenuImages { unsigned long count; MenuImage images[1]; };
struct Font;
struct GameTimer;
extern void GameTimerSet(GameTimer *, unsigned long); extern int GameTimerExpired(GameTimer *);
struct GameTimer { unsigned long start, end, duration; GameTimer(unsigned long n) { GameTimerSet(this, n); } };
struct SimPoint { long x,y; SimPoint() {} SimPoint(const SimPoint &p) { x=p.x; y=p.y; } };











struct SimAnim { unsigned char unknown00[6]; unsigned char mode; unsigned char frame; };
struct FileRecord;
struct TerrainGrid;
struct FogBuffer { void Initialize(int,int); };
struct LogicSystem { int Save(FileRecord *); int Load(FileRecord *); };
struct FastCoord;
struct PlayerSlot { int state, id, points, actor; char name[0xb4-16]; int actionState; char tail[4]; };
struct SimGame {
    int active; int IsActive() { return active; }
    unsigned char unknown04[0xc]; unsigned char saved[0x2c - 0x10];
    int screen; unsigned char unknown30[0x4];
    int focusIndex; ActorRecord *focus;
    unsigned char unknown3c[0x44 - 0x3c]; SimStat stats[3];
    SimAnim portrait; SimAnim face;
    SimAnim status; unsigned char unknown80[0x18b - 0x80];
    ActorUnit focusState; unsigned char unknown296[0x2ba-0x296];
    int phase, round, multi; int waiting;
    union { unsigned long tick; unsigned char tickBytes[4]; } ;
    unsigned long deadline, action; int palette;
    int playerCount; PlayerSlot players[9];
    struct PlayerSlot *slot; int mode;
    unsigned char unknown982[0x986 - 0x982]; int player;
    int localPlayer, viewOwner, linked, remote, automated, nextPlayer, queued; unsigned char unknown9a6[0xf95-0x9a6];
    long view[8]; unsigned char unknownfb5[0x1029-0xfb5];
    long healthTrend; long energyTrend;
    unsigned char unknown1031[0x10cd-0x1031]; ActorRecord *target;
    unsigned char unknown10d1[0x10ed-0x10d1]; int turnTime;
    int extraSaved; unsigned char unknown10f5[0x112d - 0x10f5];
    unsigned char savedEnd;
};
#pragma pack(pop)
extern GuiCanvas g513888; extern ViewRect viewportClipLeft; extern ViewRect g4d381c, g4d385c;
extern GuiScreen g4d4094; extern FastBuffer g4d35e4; extern char gShellBackground;
extern char g4c9234, g4c68b4; extern StrPool missionImageName, targetBriefResource; extern ProfileManager profileManager;
extern int g4d030f; extern MenuImages *drawCursorImages; extern int g4d3485;
int g4a031c[3] = { 0, 11, 22 }; static char simoutSourceFile[] = __FILE__; long g4a0338 = 148, g4a033c = 395; extern GuiRect g4d3cec, g4d3c9c; extern unsigned long gMenuColors[30], gMenuColorMap[30];
extern unsigned long gMenuTextColor; extern unsigned long *gMenuTextMap; extern Font g511dc0;
extern SimPoint g4d4150; extern ActorRecord *g4d2498; extern unsigned char g4a0350[];
extern int gGameTurn; extern TerrainGrid fallbackName; extern FogBuffer g4d35f4;
extern LogicSystem g4dd0c0; extern int *actorIndexTable; extern unsigned char *g4d3604;
extern unsigned char g4d2490; extern FastCoord gFastTransform; extern void ApplyPaletteData(void *, int = 0, int = 256);
extern void PopDrawSurface(void *surface, int a, int b, int c, int d, int e, int f); extern void MenuLine(void *, long, long, long, long); extern void FastBufferReset(FastBuffer *);
extern void SimLoadPalette(int, void *); extern void SimLoadImage(void *, int, void *); extern void SimSetMode(SimGame *, int);
extern void SimSetView(SimGame *, int); extern void SimAddMission(void *); extern void MenuDrawImage(void *, MenuImage *, long, long);
extern void MenuSetClip(GuiRect *, long, long, long, long); extern int StateInvoke(ActorUnit *, long *, long *); extern int StateInvokeEnergy(ActorUnit *, long *, long *);
extern void *gDrawRemap; extern Font g511dbc;
extern void *SimUnitRemap(ActorUnit *, ActorUnit *); extern void *SimUnitRemapVariant(ActorUnit *, int, ActorUnit *); extern void MenuDrawText(Font *, long, long, const char *);
extern "C" int BriefFormat(const char *format, ...); extern void MenuDrawTextAt(Font *, long, long, int, const char *); extern void SimMarkPosition(long, long, int, int);
extern MenuImage *SimAnimFrame(SimAnim *, int); extern void SimAnimAdvance(SimAnim *); extern void SimPaletteCycle(void *);
extern unsigned long StreamWrite(FileRecord *, const void *, unsigned long); extern void ExploCompact(int); extern int ActorListRead(void *);
extern int WorldActionRemove(void *); extern int ExploSave(FileRecord *); extern long GridSave(TerrainGrid *, void *);
extern int FogWrite(FogBuffer *, void *); extern void SimSaveExtra(FileRecord *); extern void SimLoadExtra(FileRecord *);
extern unsigned long StreamRead(FileRecord *, void *, unsigned long); extern int ActorListWrite(void *); extern int WorldActionClear(void *);
extern int ExploLoad(FileRecord *); extern long GridLoad(TerrainGrid *, void *); extern int FogRead(FogBuffer *, void *);
extern "C" void *memset(void *, int, unsigned int); extern PlayerSlot *GetModePlayer(void *, int); extern ActorRecord *ActorGetByIndex(int);
extern void SimSetFocus(SimGame *, ActorRecord *); extern void SimSetUpdate(SimGame *, int); extern ActorRecord *ActorFindFree(void);
extern ActorRecord *ActorFindNext(ActorRecord *); extern void FastProject(FastCoord *, SimPoint *, void *); extern void MenuSetColors(void *, unsigned long, long);
extern void MenuFill(void *, long, long, long, long);
void SimGameDrawPanel(SimGame *game);
inline int Clamp(int value, int low, int high) { if (value <= low) return low; if (value >= high) return high; return value; }
inline void SimRestoreBackground(long x, long y, long w, long h) { PopDrawSurface(&g513888, 1, 0, x, y, w, h); }
void SimGameDrawStatus(SimGame *game);
void SimGameLeave(SimGame *game);
extern "C" void AssertFail(const char *,int,const char *); extern "C" int sprintf(char *,const char *,...); extern "C" void *memcpy(void *,const void *,unsigned);
extern "C" int memcmp(const void *,const void *,unsigned); extern void *gShellSurfacePixels,*gShellSurfacePixelsBefore,*gShellDrawPixels; extern int *shellScanlines;
extern long viewportClipTop,viewportClipBottom; extern long g5138ac,g5138b0; inline long ScreenWidth() { return g5138ac; } inline long ScreenHeight() { return g5138b0; } extern ViewRect gBriefDrawSource,gViewRect,simPortraitRect;
extern Font gDefaultFont; extern char targetPrompt5,simPanel,popInfo,popWorld,simActions,g4d1dec,simInventory; extern GuiRect gMenuClip,simClip,simMainClip,simMainSaved,simDetailClip,simDetailSaved;
extern signed char focusFrameColors[4]; extern void SigsStart(void *,int),SurfacePresent(void *),GuiFilterPalette(void *,int),BriefStatus(void *),GuiResetClip(void *),SimClipFlush(void *); extern void SimDrawTimer(SimGame *,int,int,int,int);
extern void BriefBlit(void *,int,int,int,int,int,int,int,int); extern void PrepareDeathMap(FastCoord *); extern void MapPointToScreen(FastCoord *,SimPoint *,void *);
extern long MapPointScale(FastCoord *,long); extern int DisplayScale(void *); extern char gView[];
extern void ResetProfileUnits(ProfileManager *); extern short mainSelectedMission; extern unsigned long missionScripts[][22];
extern unsigned char selectedMissionUnits[][2]; extern long gLaunchRule;
struct MenuNetwork { int id, flags; int IsHost() { return flags; } void CompactPlayers(); void Synchronize(int,int,int=0); void SendStart(struct GenRecord *,int=0); void Launch(); };
extern MenuNetwork gMenuNetwork; extern void HotbriefEmpty(void *); extern void *LogicActorBuild(void *,const unsigned long *);
extern void BestLocSetData(void *,void *,void *); extern int SimAddPlayer(SimGame *,int,PlayerSlot * = 0); extern "C" void *f_0043c6ed(void *,void *);
struct ActorBlock; struct ActorListObject; struct ActorPosition; struct GridPoint; extern ActorRecord *ActorListAdd(int,ActorBlock *,ActorListObject *,ActorPosition *,int,int); extern void LogicActorDelete(void *,int),ActorActivate(ActorRecord *); extern "C" int ResolveName(void *,void *);
extern void TargetObjectInit(char *,ActorPosition *,int); extern void SimCenterOn(SimGame *,GridPoint *),SimRefreshFog(SimGame *,int),SimChangePlayer(SimGame *,int); extern void FogInitialize(FogBuffer *,int,int);
inline int SimActorHidden(int index) { return g4d3604[index]==0; }
inline int SimActorUnitId(int index) { return actorIndexTable[index*2]; }


static inline void RequireActive(int active) {
    if (!active)
        AssertFail(__FILE__, __LINE__, (const char *)active);
}

// Actor creation uses the original line 189 assertion.
static inline void RequireCreated(ActorRecord *actor, void *unit, void *pilot) {
    if (!actor || !unit || !pilot)






































































        AssertFail(__FILE__, __LINE__, 0);
}

extern "C" void ShellRestore(SimGame *game)
{
    gShellSurfacePixels=gShellSurfacePixelsBefore;
    viewportClipLeft=gBriefDrawSource;
    SigsStart(&g513888,0);
    SurfacePresent(&g513888);
    ApplyPaletteData(&gShellBackground);
    GuiFilterPalette(&gShellBackground,4);
    g4a0338=g4d385c.left; g4a033c=g4d385c.top;
    int y=167;
    int image=game->mode==game->player ? 78 : 84;
    for(int i=0;i<6;i++) {
        MenuImage *frame=&drawCursorImages->images[image+i];
        MenuDrawImage(&g513888,frame,0,y);
        y+=frame->height;
    }
    int x=g4a0338;
    image=game->mode==game->player ? 66 : 72;
    for(i=0;i<6;i++) {
        MenuImage *frame=&drawCursorImages->images[image+i];
        MenuDrawImage(&g513888,frame,x,g4a033c);
        x+=frame->width;
    }
    if(game->mode!=game->player) MenuDrawImage(&g513888,&drawCursorImages->images[174],0,353);
    if(game->mode!=game->player) {
        char text[80];
        int mode=game->mode;
        if(mode!=game->localPlayer) {
            gMenuTextMap=&gMenuColorMap[20+mode];gMenuTextColor=gMenuColors[20+mode];
            if(game->multi && mode!=game->player)
                sprintf(text,targetBriefResource.GetString(1403,0),mode,game->players[mode-1].name);
            else sprintf(text,targetBriefResource.GetString(1405,0));
        } else {
            gMenuTextMap=&gMenuColorMap[16]; gMenuTextColor=gMenuColors[16];
            sprintf(text,targetBriefResource.GetString(1404,0));
        }
        MenuDrawText(&gDefaultFont,g4a0338+10,g4a033c+4,text);
    }
    PopDrawSurface(&g513888,0,1,0,0,ScreenWidth(),ScreenHeight());
    BriefStatus(&g4d4094);BriefStatus(&targetPrompt5);BriefStatus(&simPanel);BriefStatus(&popInfo);BriefStatus(&popWorld);BriefStatus(&simActions);BriefStatus(&g4d1dec);BriefStatus(&simInventory);
    GuiResetClip(&gMenuClip);
    SimClipFlush(&g4d3c9c);SimClipFlush(&simClip);SimClipFlush(&g4d3cec);SimClipFlush(&simMainClip);SimClipFlush(&simMainSaved);SimClipFlush(&simDetailClip);SimClipFlush(&simDetailSaved);
    game->screen=-1;
}

void SimGameDrawPortrait(SimGame *game)
{
    static GameTimer timer(0);
    if (GameTimerExpired(&timer) || (game->screen & 0x400)) {
        GameTimerSet(&timer, 200);
        long x = g4a0338 + 0x125, y = g4a033c;
        long w = 0x54, h = 0x54;
        if (game->screen & 0x400) SimRestoreBackground(x + 4, y, 0x4c, 0x54);
        if (game->screen == -1 && game->portrait.mode != 5) {
            SimRestoreBackground(x, y, w, h);
            game->portrait.frame = 0;
        }
        viewportClipLeft = g4d385c;
        MenuImage *frame = SimAnimFrame(&game->portrait, -1);
        MenuDrawImage(&g513888, frame, x, y);
        if (game->portrait.mode != 5) SimAnimAdvance(&game->portrait);
        MenuSetClip(&g4d3cec, x, y, w, h);
        if (gGameTurn > 0x14) SimPaletteCycle(&gShellBackground);
    }
}

void SimGameRefresh(SimGame *game)
{
    viewportClipLeft = g4d381c;
    if (game->screen == -1) {
        PopDrawSurface(&g513888, 1, 0, g4d385c.left, g4d385c.top,
                       g4d385c.right - g4d385c.left + 1, g4d385c.bottom - g4d385c.top + 1);
        g4d4094.Invalidate();
    }
    SimGameDrawPanel(game);
    SimGameDrawStatus(game);
}

void SimDrawBar(long x, long y, int image, long width, long value, long maximum, long trend)
{
    long filled = value * width / maximum;
    viewportClipLeft = g4d381c;
    if (filled) MenuDrawImage(&g513888, &drawCursorImages->images[image], x, y);
    if (value != maximum) {
        viewportClipLeft.left = x + filled;
        MenuDrawImage(&g513888, &drawCursorImages->images[image + 2], x, y);
    }
    if (g4d3485) {
        viewportClipLeft.left = x + trend * width / maximum;
        viewportClipLeft.right = x + filled;
        MenuDrawImage(&g513888, &drawCursorImages->images[image + 1], x, y);
    }
    viewportClipLeft = g4d381c;
    int down = trend < 0;
    MenuDrawImage(&g513888, &drawCursorImages->images[image + 3 + down], x - 10, y);
}

void SimGameDrawPanel(SimGame *game)
{
    long value, maximum;
    if (game->screen & 0x50) {
        SimRestoreBackground(g4a0338 + 0x6c, g4a033c + 0x19, 0x23, 0x2d);
        MenuSetClip(&g4d3cec, g4a0338, g4a033c, 0x91, 0x64);
        gMenuTextColor = gMenuColors[14];
        gMenuTextMap = &gMenuColorMap[14];
        StateInvoke(game->focus->unit, &value, &maximum);
        SimDrawBar(g4a0338 + 7, g4a033c + 0x29, 0x62, 100, value, maximum, game->healthTrend);
        if (value > 999) value = 999;
        MenuDrawTextAt(&g511dc0, g4a0338 + 0x8b, g4a033c + 0x31, 1, (const char *)BriefFormat("%d", value));
        StateInvokeEnergy(game->focus->unit, &value, &maximum);
        SimDrawBar(g4a0338 + 7, g4a033c + 0x1d, 0x67, 100, value, maximum, game->energyTrend);
        if (value > 999) value = 999;
        MenuDrawTextAt(&g511dc0, g4a0338 + 0x8b, g4a033c + 0x1b, 1, (const char *)BriefFormat("%d", value));
    }
}

void SimGameDrawStatus(SimGame *game)
{
    static GameTimer timer(0);
    static int blink;
    if (GameTimerExpired(&timer) || (game->screen & 0x30)) {
        long y = g4a033c, x = g4a0338 + 0x94;
        viewportClipLeft = g4d385c;
        SimRestoreBackground(x, y, 0x50, 0x50);
        MenuImage *frame = SimAnimFrame(&game->face, -1);
        gDrawRemap = SimUnitRemap(game->focus->unit, &game->focusState);
        MenuDrawImage(&g513888, frame, x, y);
        gDrawRemap = 0;
        MenuSetClip(&g4d3cec, x, y, 1000, 1000);
        gDrawRemap = SimUnitRemapVariant(game->focus->unit, game->focus->action.angle, &game->focusState);
        MenuDrawImage(&g513888, &drawCursorImages->images[90], g4a0338 + 0x95, g4a033c + 2);
        gDrawRemap = 0;
        game->focusState = *game->focus->unit;
        if (GameTimerExpired(&timer)) {
            GameTimerSet(&timer, 1500);
            blink++;
            game->face.frame++;
            if (game->face.frame >= 6) game->face.frame = 0;
        }
        gMenuTextMap = &gMenuColorMap[14];
        gMenuTextColor = gMenuColors[14];
        PopDrawSurface(&g513888, 1, 0, g4a0338 + 0x1d4, g4a033c + 0x18, 0x18, 0x20);
        int image, k;
        long text;
        for (k = 0, image = 0x75, text = 0x514; image != 0x7b; k++, image += 2, text++) {
            int filled = (game->stats[k].value * 19 + game->stats[k].maximum / 2) / game->stats[k].maximum;
            long left = g4a0338 + 0x185;
            long top = g4a033c + g4a031c[k] + 0x1b;
            filled = Clamp(filled, game->stats[k].value >= 0, 19);
            int n;
            for (n = 0; n < filled; n++) MenuDrawImage(&g513888, &drawCursorImages->images[image], left + n * 3, top);
            for (; n < 19; n++) MenuDrawImage(&g513888, &drawCursorImages->images[image + 1], left + n * 3, top);
            top -= 3;
            MenuDrawText(&g511dbc, left + 0x3e, top, targetBriefResource.GetString(text, 0));
            MenuDrawTextAt(&g511dbc, left + 0x62, top, 1, (const char *)BriefFormat("%d", game->stats[k].value));
        }
        if (!(blink & 1) || game->stats[1].value >= 0) image++;
        MenuDrawImage(&g513888, &drawCursorImages->images[image], g4a0338 + 0x181, g4a033c + 0x26);
        image = 0x7d;
        if (!(blink & 1) || game->stats[2].value <= game->stats[2].maximum) image++;
        MenuDrawImage(&g513888, &drawCursorImages->images[image], g4a0338 + 0x1be, g4a033c + 0x31);
        viewportClipLeft = g4d381c;
    }
}

// Eight corner strokes around the focused and targeted actor bounds.
inline unsigned long FrameColor(int color) { return gMenuColors[color]; }
void SimGameDrawFrames(SimGame *game)
{
    int width,height;
    viewportClipLeft=gViewRect;
    if(game->linked && game->focus) {
        signed char first=((game->tickBytes[1]&2)?focusFrameColors[0]:focusFrameColors[1]);
        signed char second=((game->tickBytes[1]&2)?focusFrameColors[3]:focusFrameColors[2]);
        if(first>=0 && !game->focus->hidden) {
            ActorRecord *actor=game->focus;
            width=actor->action.rect.width+6; height=actor->action.rect.height+6;
            int left=actor->action.rect.x-3, top=actor->action.rect.y-3;
            int right=left+width-1, bottom=top+height-1;
            int length=actor->action.Width()/6+1;
            MenuSetColors(&g513888,FrameColor(first),-1);
            MenuLine(&g513888,left,top,left+length,top); MenuLine(&g513888,left,top,left,top+length);
            MenuLine(&g513888,right,top,right-length,top); MenuLine(&g513888,right,top,right,top+length);
            MenuLine(&g513888,right,bottom,right-length,bottom); MenuLine(&g513888,right,bottom,right,bottom-length);
            MenuLine(&g513888,left,bottom,left+length,bottom); MenuLine(&g513888,left,bottom,left,bottom-length);
        }
        if(second>=0 && game->target && g4d3604[game->target->action.resource]) {
            ActorRecord *actor=game->target;
            width=actor->action.rect.width+6; height=actor->action.rect.height+6;
            int left=actor->action.rect.x-3,top=actor->action.rect.y-3;
            int right=left+width-1,bottom=top+height-1;
            int length=actor->action.Width()/6+1;
            MenuSetColors(&g513888,FrameColor(second),-1);
            MenuLine(&g513888,left,top,left+length,top); MenuLine(&g513888,left,top,left,top+length);
            MenuLine(&g513888,right,top,right-length,top); MenuLine(&g513888,right,top,right,top+length);
            MenuLine(&g513888,right,bottom,right-length,bottom); MenuLine(&g513888,right,bottom,right,bottom-length);
            MenuLine(&g513888,left,bottom,left+length,bottom); MenuLine(&g513888,left,bottom,left,bottom-length);
        }
    }
    if(game->multi || game->extraSaved) SimDrawTimer(game,g5138ac-5,5,0,game->turnTime);
}



