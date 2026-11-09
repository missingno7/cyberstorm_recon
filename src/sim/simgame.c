// sim\simgame.c - simulation game loop (working labels; names are guesses unless bound)
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
extern void GameTimerSet(GameTimer *, unsigned long);
extern int GameTimerExpired(GameTimer *);
struct GameTimer { unsigned long start, end, duration; GameTimer(unsigned long n) { GameTimerSet(this, n); } };
struct SimPoint { long x, y; };
struct ActorRecord {
    unsigned char unknown00[8];
    int owner;                          // +0x08
    unsigned char unknown0c[0x18 - 0xc];
    UnitState *unit;                    // +0x18
    unsigned char unknown1c[0x25 - 0x1c];
    unsigned char type;                 // +0x25
    unsigned char variant;              // +0x26
    unsigned char unknown27[0x32 - 0x27];
    unsigned char position[0x42 - 0x32];  // +0x32
    int index;                          // +0x42
    unsigned char unknown46[0x5a - 0x46];
    signed char color;                  // +0x5a
    unsigned char unknown5b[0x97 - 0x5b];
    unsigned char blink;                // +0x97
};
struct SimAnim { unsigned char unknown00[6]; unsigned char mode; unsigned char frame; };
struct FileRecord;
struct TerrainGrid;
struct FogBuffer;
struct LogicSystem { int Save(FileRecord *); int Load(FileRecord *); };
struct FastCoord;
struct PlayerSlot;
struct SimGame {
    int active;                         // +0x00
    int IsActive() { return active; }
    unsigned char unknown04[0xc];
    unsigned char saved[0x2c - 0x10];   // +0x10: start of the saved block
    int screen;                         // +0x2c, -1 = not drawn yet; flag bits
    unsigned char unknown30[0x4];
    int focusIndex;                     // +0x34
    ActorRecord *focus;                 // +0x38
    unsigned char unknown3c[0x44 - 0x3c];
    SimStat stats[3];                   // +0x44
    SimAnim portrait;                   // +0x68
    SimAnim face;                       // +0x70
    SimAnim status;                     // +0x78
    unsigned char unknown80[0x18b - 0x80];
    UnitState focusState;               // +0x18b, copy of the focus unit's state
    unsigned char unknown296[0x2d6 - 0x296];
    int palette;                        // +0x2d6
    unsigned char unknown2da[0x97a - 0x2da];
    struct PlayerSlot *slot;            // +0x97a
    int mode;                           // +0x97e
    unsigned char unknown982[0x986 - 0x982];
    int player;                         // +0x986
    unsigned char unknown98a[0x1029 - 0x98a];
    long healthTrend;                   // +0x1029
    long energyTrend;                   // +0x102d
    unsigned char unknown1031[0x10f1 - 0x1031];
    int extraSaved;                     // +0x10f1
    unsigned char unknown10f5[0x112d - 0x10f5];
    unsigned char savedEnd;             // +0x112d
};
#pragma pack(pop)

extern GuiCanvas g513888;
extern ViewRect viewportClipLeft;       // 005138fc
extern ViewRect g4d381c, g4d385c;
extern GuiScreen g4d4094;
extern FastBuffer g4d35e4;
extern char gShellBackground;
extern char g4c9234, g4c68b4;
extern StrPool missionImageName, targetBriefResource;
extern ProfileManager profileManager;
extern int g4d030f;
extern MenuImages *g4d3744;
extern int g4d3485;
extern long g4a0338, g4a033c;           // panel origin
extern GuiRect g4d3cec, g4d3c9c;
extern unsigned long gMenuColors[30], gMenuColorMap[30];
extern unsigned long gMenuTextColor;
extern unsigned long *gMenuTextMap;
extern Font g511dc0;
extern SimPoint g4d4150;              // marked position of the focus actor
extern ActorRecord *g4d2498;
extern unsigned char g4a0350[];
extern int gGameTurn;
extern TerrainGrid fallbackName;        // 004d3594 (label from symbols.json)
extern FogBuffer g4d35f4;
extern LogicSystem g4dd0c0;
extern int *actorIndexTable;
extern unsigned char *g4d3604;          // actor visibility flags
extern unsigned char g4d2490;
extern FastCoord gFastTransform;

extern void ApplyPaletteData(void *, int = 0, int = 256);
extern void PopDrawSurface(void *surface, int a, int b, int c, int d, int e, int f);
extern void MenuLine(void *, long, long, long, long);
extern void FastBufferReset(FastBuffer *);
extern void SimLoadPalette(int, void *);
extern void SimLoadImage(void *, int, void *);
extern void SimSetMode(SimGame *, int);
extern void SimSetView(SimGame *, int);
extern void SimAddMission(void *);
extern void MenuDrawImage(void *, MenuImage *, long, long);
extern void MenuSetClip(GuiRect *, long, long, long, long);
extern int StateInvoke(UnitState *, long *, long *);
extern int StateInvokeEnergy(UnitState *, long *, long *);
extern void *gDrawRemap;
extern Font g511dbc;
extern int g4a031c[3];
extern void *SimUnitRemap(UnitState *, UnitState *);
extern void *SimUnitRemapVariant(UnitState *, int, UnitState *);
extern void MenuDrawText(Font *, long, long, const char *);
extern "C" int BriefFormat(const char *format, ...);
extern void MenuDrawTextAt(Font *, long, long, int, const char *);
extern void SimMarkPosition(long, long, int, int);
extern MenuImage *SimAnimFrame(SimAnim *, int);
extern void SimAnimAdvance(SimAnim *);
extern void SimPaletteCycle(void *);
extern unsigned long StreamWrite(FileRecord *, const void *, unsigned long);
extern void ExploCompact(int);
extern int ActorListRead(void *);
extern int WorldActionRemove(void *);
extern int ExploSave(FileRecord *);
extern long GridSave(TerrainGrid *, void *);
extern int FogWrite(FogBuffer *, void *);
extern void SimSaveExtra(FileRecord *);
extern void SimLoadExtra(FileRecord *);
extern unsigned long StreamRead(FileRecord *, void *, unsigned long);
extern int ActorListWrite(void *);
extern int WorldActionClear(void *);
extern int ExploLoad(FileRecord *);
extern long GridLoad(TerrainGrid *, void *);
extern int FogRead(FogBuffer *, void *);
extern "C" void *memset(void *, int, unsigned int);
extern PlayerSlot *GetModePlayer(void *, int);
extern ActorRecord *ActorGetByIndex(int);
extern void SimSetFocus(SimGame *, ActorRecord *);
extern void SimSetUpdate(SimGame *, int);
extern ActorRecord *ActorFindFree(void);
extern ActorRecord *ActorFindNext(ActorRecord *);
extern void FastProject(FastCoord *, SimPoint *, void *);
extern void MenuSetColors(void *, unsigned long, long);
extern void MenuFill(void *, long, long, long, long);

void SimGameDrawPanel(SimGame *game);
inline int Clamp(int value, int low, int high)
{
    if (value <= low) return low;
    if (value >= high) return high;
    return value;
}
inline void SimRestoreBackground(long x, long y, long w, long h) { PopDrawSurface(&g513888, 1, 0, x, y, w, h); }
void SimGameDrawStatus(SimGame *game);
void SimGameLeave(SimGame *game);

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
    if (filled) MenuDrawImage(&g513888, &g4d3744->images[image], x, y);
    if (value != maximum) {
        viewportClipLeft.left = x + filled;
        MenuDrawImage(&g513888, &g4d3744->images[image + 2], x, y);
    }
    if (g4d3485) {
        viewportClipLeft.left = x + trend * width / maximum;
        viewportClipLeft.right = x + filled;
        MenuDrawImage(&g513888, &g4d3744->images[image + 1], x, y);
    }
    viewportClipLeft = g4d381c;
    MenuDrawImage(&g513888, &g4d3744->images[image + 3 + (trend < 0)], x - 10, y);
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
        long x = g4a0338 + 0x94, y = g4a033c;
        viewportClipLeft = g4d385c;
        SimRestoreBackground(x, y, 0x50, 0x50);
        MenuImage *frame = SimAnimFrame(&game->face, -1);
        gDrawRemap = SimUnitRemap(game->focus->unit, &game->focusState);
        MenuDrawImage(&g513888, frame, x, y);
        gDrawRemap = 0;
        MenuSetClip(&g4d3cec, x, y, 1000, 1000);
        gDrawRemap = SimUnitRemapVariant(game->focus->unit, game->focus->variant, &game->focusState);
        MenuDrawImage(&g513888, &g4d3744->images[90], g4a0338 + 0x95, g4a033c + 2);
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
            for (n = 0; n < filled; n++) MenuDrawImage(&g513888, &g4d3744->images[image], left + n * 3, top);
            for (; n < 19; n++) MenuDrawImage(&g513888, &g4d3744->images[image + 1], left + n * 3, top);
            top -= 3;
            MenuDrawText(&g511dbc, left + 0x3e, top, targetBriefResource.GetString(text, 0));
            MenuDrawTextAt(&g511dbc, left + 0x62, top, 1, (const char *)BriefFormat("%d", game->stats[k].value));
        }
        if (!(blink & 1) || game->stats[1].value >= 0) image++;
        MenuDrawImage(&g513888, &g4d3744->images[image], g4a0338 + 0x181, g4a033c + 0x26);
        image = 0x7d;
        if (!(blink & 1) || game->stats[2].value <= game->stats[2].maximum) image++;
        MenuDrawImage(&g513888, &g4d3744->images[image], g4a0338 + 0x1be, g4a033c + 0x31);
        viewportClipLeft = g4d381c;
    }
}

void SimGameNothing(void)
{
}

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
    viewportClipLeft.top = inset + 6;
    viewportClipLeft.bottom = 0x83 - inset;
    MenuLine(&g513888, x, y1, x, y2);
    viewportClipLeft = saved;
}

void SimGameDrawActors(SimGame *game, int size)
{
    g4d4150.x = 0;
    for (ActorRecord *actor = ActorFindFree(); actor; actor = ActorFindNext(actor)) {
        SimPoint at;
        int half = size >> 1;
        unsigned long color = 0;
        if (actor->type == 0x11) {
            if (actorIndexTable[actor->index * 2] == 0) color = gMenuColors[0];
        } else if (actor->owner == game->player || g4d3604[actor->index] != 0) {
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

int SimGameLoad(SimGame *game, FileRecord *file)
{
    if (!game->IsActive()) return 1;
    unsigned long size = (unsigned char *)&game->savedEnd - game->saved;
    if (StreamRead(file, game->saved, size) != size) return 0;
    ActorListWrite(file);
    WorldActionClear(file);
    ExploLoad(file);
    GridLoad(&fallbackName, file);
    FogRead(&g4d35f4, file);
    if (game->extraSaved) SimLoadExtra(file);
    g4dd0c0.Load(file);
    memset(&game->portrait, 0, sizeof(SimAnim));
    memset(&game->face, 0, sizeof(SimAnim));
    memset(&game->status, 0, sizeof(SimAnim));
    SimGameLeave(game);
    game->slot = GetModePlayer(game, game->mode);
    SimSetFocus(game, ActorGetByIndex(game->focusIndex));
    SimSetUpdate(game, 1);
    return 1;
}

int SimGameSave(SimGame *game, FileRecord *file)
{
    if (!game->IsActive()) return 1;
    unsigned long size = (unsigned char *)&game->savedEnd - game->saved;
    if (StreamWrite(file, game->saved, size) != size) return 0;
    ExploCompact(1);
    ActorListRead(file);
    WorldActionRemove(file);
    ExploSave(file);
    GridSave(&fallbackName, file);
    FogWrite(&g4d35f4, file);
    if (game->extraSaved) SimSaveExtra(file);
    g4dd0c0.Save(file);
    return 1;
}

void SimGameLeave(SimGame *game)
{
    SimLoadPalette(game->palette, &g4c9234);
    SimLoadImage(&g4c68b4, game->palette, &gShellBackground);
    ApplyPaletteData(&gShellBackground);
    FastBufferReset(&g4d35e4);
    SimSetMode(game, 0);
    SimSetView(game, 0);
    game->screen = -1;
}

void SimGameLoadMissions(void)
{
    missionImageName.Lock();
    profileManager.BeginMissions();
    for (int i = 0; i < g4d030f; i++)
        SimAddMission(profileManager.Mission(i + 1));
}

void SimGameEnd(void)
{
}

