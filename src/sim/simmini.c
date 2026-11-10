// sim\simmini.c - mini map (working labels; names are guesses unless bound)
#pragma pack(push, 1)
struct SimStat { long value, maximum, spare; };
struct UnitState { unsigned char bytes[0x10b]; };
struct ViewRect { int left, top, right, bottom; };
struct GuiCanvas;
struct GuiScreen { void Invalidate(); };
struct FastBuffer;
struct StrPool { int Lock(); char *GetString(long, int = 0); };
struct ProfileManager { void BeginMissions(); void *Mission(int); };
struct GuiRect { long x, y, width, height; };
struct MenuImage { unsigned long offset; unsigned short width, height; unsigned long flags; };
struct MenuImages { unsigned long count; MenuImage images[1]; };
struct Font;
struct GameTimer;
extern void GameTimerSet(GameTimer *, unsigned long); extern int GameTimerExpired(GameTimer *);
struct GameTimer { unsigned long start, end, duration; GameTimer(unsigned long n) { GameTimerSet(this, n); } };
struct SimPoint { long x,y; SimPoint() {} SimPoint(const SimPoint &p) { x=p.x; y=p.y; } };
struct ActorRecord {
    unsigned char unknown00[8]; int owner;
    unsigned char unknown0c[0x18 - 0xc]; UnitState *unit;
    unsigned char unknown1c[0x25 - 0x1c]; unsigned char type;
    unsigned char variant; unsigned char unknown27[0x32 - 0x27];
    unsigned char position[0x42 - 0x32]; int index;
    unsigned char unknown46[0x5a - 0x46]; signed char color;
    int boxX,boxY,boxW,boxH; unsigned char unknown6b[0x89-0x6b];
    signed char hidden; unsigned char unknown8a[0x97-0x8a];
    unsigned char blink;
};
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
    UnitState focusState; unsigned char unknown296[0x2ba-0x296];
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
extern int g4d030f; extern MenuImages *g4d3744; extern int g4d3485;
extern long g4a0338, g4a033c; extern GuiRect g4d3cec, g4d3c9c; extern unsigned long gMenuColors[30], gMenuColorMap[30];
extern unsigned long gMenuTextColor; extern unsigned long *gMenuTextMap; extern Font g511dc0;
extern ActorRecord *g4d2498; static char simminiSourceFile[] = __FILE__; unsigned char g4a0350[9] = { 0x50, 0x4f, 0xce, 0x94, 0xe7, 0x5d, 0xd7, 0xd1, 0xd1 };
extern int gGameTurn; extern TerrainGrid fallbackName; extern FogBuffer g4d35f4;
extern LogicSystem g4dd0c0; extern int *actorIndexTable; extern unsigned char *g4d3604;
extern unsigned char g4d2490; extern FastCoord gFastTransform; extern void ApplyPaletteData(void *, int = 0, int = 256);
extern void PopDrawSurface(void *surface, int a, int b, int c, int d, int e, int f); extern void MenuLine(void *, long, long, long, long); extern void FastBufferReset(FastBuffer *);
extern void SimLoadPalette(int, void *); extern void SimLoadImage(void *, int, void *); extern void SimSetMode(SimGame *, int);
extern void SimSetView(SimGame *, int); extern void SimAddMission(void *); extern void MenuDrawImage(void *, MenuImage *, long, long);
extern void MenuSetClip(GuiRect *, long, long, long, long); extern int StateInvoke(UnitState *, long *, long *); extern int StateInvokeEnergy(UnitState *, long *, long *);
extern void *gDrawRemap; extern Font g511dbc; extern int g4a031c[3];
extern void *SimUnitRemap(UnitState *, UnitState *); extern void *SimUnitRemapVariant(UnitState *, int, UnitState *); extern void MenuDrawText(Font *, long, long, const char *);
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
extern long viewportClipTop,viewportClipBottom; extern long g5138ac,g5138b0; extern ViewRect gBriefDrawSource,gViewRect,simPortraitRect;
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

// The object starts with an empty static initializer (00451b00, XI record 004a6c60): this global, whose class has
// an empty inline constructor. It is referenced only by this object.
SimPoint g4d4150;

void SimGameBlink(void)
{
    static GameTimer timer(0);
    static int ticks;
    if (GameTimerExpired(&timer)) {
        GameTimerSet(&timer, 250);
        ticks++;
        if (g4d4150.x) {
            int color = 0;
            if ((ticks & 1) && g4d2498) color = g4a0350[g4d2498->color];
            SimMarkPosition(g4d4150.x, g4d4150.y, 2, color);
            MenuSetClip(&g4d3c9c, g4d4150.x, g4d4150.y, 2, 2);
        }
    }
}

void SimDrawHLine(long x1, long x2, long y)
{
    if (y < 6 || y > 0x83) return;
    int inset = 0;
    if (y < 0x25) inset = (0x25 - y) * 2;
    if (y > 0x64) inset = y * 2 - 200;
    ViewRect saved = viewportClipLeft;
    viewportClipLeft.left = inset + 10;
    viewportClipLeft.right = 0x87 - inset;
    MenuLine(&g513888, x1, y, x2, y);
    viewportClipLeft = saved;
}

void SimDrawVLine(long y1, long y2, long x)
{
    if (x < 10 || x > 0x87) return;
    int inset = (x <= 0x48 ? 0x49 - x : x - 0x48) / 2;
    ViewRect saved = viewportClipLeft;
    viewportClipTop = inset + 6;
    viewportClipBottom = 0x83 - inset;
    MenuLine(&g513888, x, y1, x, y2);
    viewportClipLeft = saved;
}

void SimGameDrawActors(SimGame *,int);
struct ViewSnapshot { long words[8]; ViewSnapshot(); };
inline ViewSnapshot::ViewSnapshot() {}
void SimGameDrawMap(SimGame *game)
{
    static ViewSnapshot previous;
    static GameTimer timer(0);
    static int pending;
    SimPoint at; int redraw=0;
    if(game->screen&2) redraw=2;
    if(!redraw && memcmp(&previous,game->view,32)) redraw|=4;
    if(!redraw) {
        if(game->screen&7) pending|=4;
        if(GameTimerExpired(&timer)) redraw=pending;
    }
    if(!redraw) { if(game->linked) SimGameBlink(); return; }
    previous=*(ViewSnapshot *)game->view;
    pending=0; viewportClipLeft=simPortraitRect;
    if(redraw&2) {
        PrepareDeathMap(&gFastTransform);
        PopDrawSurface(&g513888,0,1,8,8,128,128);
    } else {
        BriefBlit(&g513888,1,0,10,37,10,37,128,64);
        for(int row=1;row<32;row++) {
            int bytes=128-(row/2)*8;
            int x=((row*2)&~3)+10;
            memcpy((char *)gShellSurfacePixelsBefore+shellScanlines[37-row]+x,(char *)gShellDrawPixels+shellScanlines[37-row]+x,bytes);
            memcpy((char *)gShellSurfacePixelsBefore+shellScanlines[100+row]+x,(char *)gShellDrawPixels+shellScanlines[100+row]+x,bytes);
        }
    }
    MapPointToScreen(&gFastTransform,&at,game->view);
    int halfWidth=MapPointScale(&gFastTransform,DisplayScale(gView)*1000);
    int halfHeight=halfWidth*2/3;
    MenuSetColors(&g513888,gMenuColors[16],gMenuColors[16]);
    SimDrawHLine(at.x-halfWidth,at.x+halfWidth,at.y-halfHeight);
    SimDrawHLine(at.x-halfWidth,at.x+halfWidth,at.y+halfHeight);
    SimDrawVLine(at.y-halfHeight,at.y+halfHeight,at.x-halfWidth);
    SimDrawVLine(at.y-halfHeight,at.y+halfHeight,at.x+halfWidth);
    GuiResetClip(&g4d3c9c);
    SimGameDrawActors(game,2);
    if(game->linked) SimGameBlink();
    GameTimerSet(&timer,2000/(gGameTurn+1));
    viewportClipLeft=g4d381c;
}

void SimGameDrawActors(SimGame *game, int size)
{
    g4d4150.x = 0;
    for (ActorRecord *actor = ActorFindFree(); actor; actor = ActorFindNext(actor)) {
        SimPoint at;
        int half = size >> 1;
        unsigned long color = 0;
        if (actor->type == 0x11) {
            if (SimActorUnitId(actor->index) == 0) color = gMenuColors[0];
        } else if (actor->owner == game->player || !SimActorHidden(actor->index)) {
            color = g4a0350[actor->color];
            if (size == 2 && actor->blink) {
                actor->blink--;
                g4d2490 |= 4;
                color = 0x4b;
            }
        }
        if (color) {
            FastProject(&gFastTransform, &at, actor->position);
            if (size != 2) {
                MenuSetColors(&g513888, color, color);
                MenuFill(&g513888, at.x - half, at.y - half, size, size);
            } else {
                SimMarkPosition(at.x, at.y, 2, color);
                if (actor == game->focus) g4d4150 = at;
            }
        }
    }
}
