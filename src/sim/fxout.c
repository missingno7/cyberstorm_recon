// sim\fxout.c - weapon fire effects (beams, bolts, sprays) drawn over the battle view. Names are working labels.
#pragma pack(1)
struct FxPoint { int x, y; FxPoint() {} };
struct FxDef {                      // weapon effect definition (004a2c58[type])
    unsigned char kind;             // + 0x00 0 beam, 1 rail, 2 spray, 3 burst
    signed char anim, count;        // + 0x01, + 0x02 shots per burst
    char pad03, repeatSound;        // + 0x04
    char pad05[4];
    unsigned char colorRow, widthScale;   // + 0x09 row of boltColors, + 0x0a
    unsigned char jitterMin, jitterMax;   // + 0x0b (beam: step scale), + 0x0c
    unsigned char railSteps;        // + 0x0d (spray: glow animation)
    union { unsigned int segmentLength; struct { unsigned char glow, glowRepeat, part[5]; }; };   // + 0x0e (rail) / + 0x0e ..
};
struct Weapon { char at00; unsigned char type; char pad02[0x2d]; int at2f, at33; };
struct World { char pad00[0x2c]; unsigned char flags; char pad2d[0x959]; int localOwner; char pad98a[0x617]; int zoom; };   // + 0x986, + 0xfa1
struct FxOwner { char pad00[8]; int owner; char pad0c[0x14]; char action[6]; unsigned char at26; };   // + 0x20, + 0x26
struct FxHandle { int a, b; FxHandle(const FxHandle &); };      // copy constructor at 00429a04
struct FxLink : FxHandle { int c, d; };                            // 0x10 bytes
struct FxBytes { char c0, c1, c2, c3, c4, c5, c6, c7; int value; };   // 0x0c bytes
struct FxShape { short s; char a, b; int i, j; };                  // 0x0c bytes
struct FxSide {                     // one end of a shot (0x54 bytes)
    int cell;                       // + 0x00
    FxPoint at04, pos, at14;        // + 0x04, + 0x0c, + 0x14
    FxLink link;                    // + 0x1c
    FxBytes bytes;                  // + 0x2c
    FxShape shape;                  // + 0x38
    FxOwner *owner;                 // + 0x44
    int at48, at4c, at50;           // + 0x48
};
struct FxTarget { FxSide src, dst; int atA8, atAC, atB0; };   // shooter and target ends (0xb4 bytes), dst at + 0x54
struct ActorRecord { char pad00[0x20]; char action[5]; unsigned char kind; char pad26[0x72]; };   // 0x98 bytes: + 0x20, + 0x25
struct FxRect { char pad00[0x2b]; int x, y, w, h; };   // + 0x2b .. + 0x37
struct ViewRect { int left, top, right, bottom; };
struct ExploRecordFx { char pad00[8]; unsigned char fps; };   // sim\explo.c record (+ 8 fps)
struct FxPartVec { int x, y; FxPartVec(); };
inline FxPartVec::FxPartVec() {}
struct FxPart { char pad00[8]; FxPartVec vec, vec2; char pad18[0x34]; int hitSounded; char pad50[0x3c]; ~FxPart() {} };   // 0x8c bytes, + 0x4c
#pragma pack()

extern "C" void AssertFail(const char *, int, const char *);
extern "C" void *memset(void *, int, unsigned int);
extern "C" unsigned int next_link_seed(unsigned int *seed);
extern void WeaponSound(FxPoint *at, int sound, int kind, int value, int total);   // 0045dad0
extern void *ExploStart(int kind, FxTarget *target);  // 0045fa20 (sim\explo.c)
extern ExploRecordFx *ExploAddPart(char anim, int frames);                  // 0045fcd0 (sim\explo.c)
extern void ExploAddGlow(int anim, int value, FxOwner *actor, int layer, char repeat);   // 0045faf0
extern void PartInit(FxPart *p, FxDef *def, ExploRecordFx *e, FxPoint *from, FxPoint *to, Weapon *weapon, FxTarget *target, int index);   // 0045ada0
extern void PartSetup(FxPart *p, int a, int b, int c, int d, int e);       // 0045b0b0
extern int PartStep(FxPart *p, int mode);                                   // 0045b740
extern int PartHit(FxPart *p);                                              // 0045b7a0
extern unsigned int ISqrt(unsigned int);                    // 0047f8ea
extern void MenuSetColors(void *, unsigned long, long);
extern void MenuLine(void *, long, long, long, long);
extern void WorldDrawBegin(void *world), WorldDrawTerrain(void *world);    // 004554c0, 0044ad70
extern void ActorDrawEffect(void *action);                  // 00457f90
extern void ViewFlip(int);                                  // 0046cbdb
struct Shell; extern void ShellLimitFrameRate(Shell *); extern char modeCheckName[];   // 004d2460 is the shell object
extern FxDef *fxDefs[];                                     // 004a2c58
extern int weaponSoundTable[];                              // 004a32e8
extern int gLocalSpeed, gRemoteSpeed;                       // 004d3794, 004d3798
extern char g513888[];                                      // drawing surface
extern ViewRect viewportClipLeft, gViewRect;                // 005138fc, 004d382c
extern int gZoom;                                           // 004d3401
extern unsigned int gFxSeed;                                // 004d3e04
extern int actorCount; extern ActorRecord *actorTable;

FxPoint gFxFrom, gFxTo;             // 004dcc38, 004dcc40
int gFxShade;                       // 004dcc48
unsigned char *gFxColors;           // 004dcc4c
int gFxWidth, gFxSegments;          // 004dcc50, 004dcc54
FxDef *gFxDef;                      // 004dcc58

static const signed char boltStep[8] = { 0, 0, 4, 2, 1, 1, 1, 1 };
static const char fxSourcePath[] = __FILE__;
static const unsigned char boltColors[15][9] = {
    { 0xd0, 0xd0, 0x4e, 0x4d, 0x4c, 0x4b, 0x4a, 0x49, 0x49 },
    { 0x4f, 0x4f, 0xd0, 0xd0, 0x4e, 0x4d, 0x4c, 0x4b, 0x4b },
    { 0x4d, 0x4d, 0x4c, 0x4b, 0x4a, 0x49, 0x48, 0x48, 0x48 },
    { 0x4f, 0xff, 0x4e, 0x4d, 0xff, 0x4b, 0xff, 0x49, 0xff },
    { 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xee },
    { 0xe7, 0xe7, 0xe8, 0xe8, 0xe9, 0xe9, 0xea, 0xea, 0xea },
    { 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xef, 0xef, 0xef, 0xf0 },
    { 0xe7, 0xeb, 0xe8, 0xec, 0xe9, 0xed, 0xea, 0xee, 0xff },
    { 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xde },
    { 0xd7, 0xd7, 0xd8, 0xd8, 0xd9, 0xd9, 0xda, 0xda, 0xda },
    { 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xdf, 0xdf, 0xdf, 0xe0 },
    { 0xd7, 0xdb, 0xd8, 0xdc, 0xd9, 0xdd, 0xda, 0xde, 0xff },
    { 0xd1, 0xd1, 0x90, 0x90, 0x91, 0x91, 0x58, 0x6a, 0xff },
    { 0xcf, 0xce, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xff },
    { 0x50, 0xd1, 0x91, 0x92, 0x94, 0x97, 0x99, 0x4d, 0xff },
};

#define ASSERT(e) if (!(e)) AssertFail(fxSourcePath, __LINE__, 0)
#define COLOR_INDEX(i) ((i) <= 0 ? 0 : (i) >= 8 ? 8 : (i))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void FxDrawBolt(int x0, int y0, int x1, int y1, int shade, int width, unsigned char *colors)
{
    int step = width >= 0 && width < 8 ? boltStep[width] : 1;
    unsigned int color;
    if (width >= 6) {
        color = colors[COLOR_INDEX(step * 5 + shade)];
        if (color != 0xff) {
            MenuSetColors(g513888, color, -1);
            MenuLine(g513888, x0 - 2, y0 - 2, x1 - 2, y1 - 2);
            MenuLine(g513888, x0 + 2, y0 - 2, x1 + 2, y1 - 2);
            MenuLine(g513888, x0 + 2, y0 + 2, x1 + 2, y1 + 2);
            MenuLine(g513888, x0 - 2, y0 + 2, x1 - 2, y1 + 2);
        }
    }
    if (width >= 5) {
        color = colors[COLOR_INDEX(step * 4 + shade)];
        if (color != 0xff) {
            MenuSetColors(g513888, color, -1);
            MenuLine(g513888, x0 - 1, y0 - 2, x1 - 1, y1 - 2);
            MenuLine(g513888, x0, y0 - 2, x1, y1 - 2);
            MenuLine(g513888, x0 + 1, y0 - 2, x1 + 1, y1 - 2);
            MenuLine(g513888, x0 - 1, y0 + 2, x1 - 1, y1 + 2);
            MenuLine(g513888, x0, y0 + 2, x1, y1 + 2);
            MenuLine(g513888, x0 + 1, y0 + 2, x1 + 1, y1 + 2);
            MenuLine(g513888, x0 - 2, y0 - 1, x1 - 2, y1 - 1);
            MenuLine(g513888, x0 - 2, y0, x1 - 2, y1);
            MenuLine(g513888, x0 - 2, y0 + 1, x1 - 2, y1 + 1);
            MenuLine(g513888, x0 + 2, y0 - 1, x1 + 2, y1 - 1);
            MenuLine(g513888, x0 + 2, y0, x1 + 2, y1);
            MenuLine(g513888, x0 + 2, y0 + 1, x1 + 2, y1 + 1);
        }
    }
    if (width >= 4) {
        color = colors[COLOR_INDEX(step * 3 + shade)];
        if (color != 0xff) {
            MenuSetColors(g513888, color, -1);
            MenuLine(g513888, x0 - 1, y0 - 1, x1 - 1, y1 - 1);
            MenuLine(g513888, x0 + 1, y0 - 1, x1 + 1, y1 - 1);
            MenuLine(g513888, x0 + 1, y0 + 1, x1 + 1, y1 + 1);
            MenuLine(g513888, x0 - 1, y0 + 1, x1 - 1, y1 + 1);
        }
    }
    if (width >= 3) {
        color = colors[COLOR_INDEX(step * 2 + shade)];
        if (color != 0xff) {
            MenuSetColors(g513888, color, -1);
            MenuLine(g513888, x0, y0 - 1, x1, y1 - 1);
            MenuLine(g513888, x0, y0 + 1, x1, y1 + 1);
            MenuLine(g513888, x0 - 1, y0, x1 - 1, y1);
            MenuLine(g513888, x0 + 1, y0, x1 + 1, y1);
        }
    }
    if (width == 2) {
        color = colors[COLOR_INDEX(shade)];
        if (color != 0xff) {
            MenuSetColors(g513888, color, -1);
            MenuLine(g513888, x0, y0, x1, y1);
            MenuLine(g513888, x0 + 1, y0, x1 + 1, y1);
            MenuLine(g513888, x0, y0 + 1, x1, y1 + 1);
            MenuLine(g513888, x0 + 1, y0 + 1, x1 + 1, y1 + 1);
        }
    } else {
        color = colors[COLOR_INDEX(shade)];
        if (color != 0xff) {
            MenuSetColors(g513888, color, -1);
            MenuLine(g513888, x0, y0, x1, y1);
        }
    }
}

void FxDrawCurrent()
{
    FxDrawBolt(gFxFrom.x, gFxFrom.y, gFxTo.x, gFxTo.y, gFxShade, gFxWidth, gFxColors);
}

void FxDrawJagged()
{
    FxPoint from = gFxFrom;
    int spread = (gFxDef->jitterMax - gFxDef->jitterMin) * gZoom / 16;
    int half = -spread / 2;
    for (int i = 1; i <= gFxSegments; i++) {
        FxPoint to;
        to.x = gFxFrom.x + (gFxTo.x - gFxFrom.x) * i / gFxSegments;
        to.y = gFxFrom.y + (gFxTo.y - gFxFrom.y) * i / gFxSegments;
        to.x += next_link_seed(&gFxSeed) % spread + half;
        to.y += next_link_seed(&gFxSeed) % spread + half;
        FxDrawBolt(from.x, from.y, to.x, to.y, 0, gFxWidth, gFxColors);
        from = to;
    }
}

void FxBounds(FxRect *rect)
{
    int margin = 5;
    if (gFxDef != 0)
        margin = (gFxDef->jitterMax - gFxDef->jitterMin) * gZoom / 16 + 5;
    int left, right, top, bottom;
    if (gFxFrom.x < gFxTo.x) {
        left = gFxFrom.x;
        right = gFxTo.x;
    } else {
        left = gFxTo.x;
        right = gFxFrom.x;
    }
    if (gFxFrom.y < gFxTo.y) {
        top = gFxFrom.y;
        bottom = gFxTo.y;
    } else {
        top = gFxTo.y;
        bottom = gFxFrom.y;
    }
    rect->x = left - margin;
    rect->y = top - margin;
    rect->w = right - left + margin * 2;
    rect->h = bottom - top + margin * 2;
}

static void FxRedraw(void *world)
{
    WorldDrawBegin(world);
    WorldDrawTerrain(world);
    viewportClipLeft = gViewRect;
    for (int i = 0; i < actorCount; i++) {
        if (actorTable[i].kind == 12)
            ActorDrawEffect(actorTable[i].action);
    }
    ViewFlip(0);
    ShellLimitFrameRate((Shell *)modeCheckName);
}



















void FxBeam(World *world, FxPoint from, FxPoint to, Weapon *weapon, int flags, FxTarget *target, FxPoint *impact, int quiet)
{
    FxDef *def = fxDefs[weapon->type];
    int sound = weaponSoundTable[weapon->type];
    gFxWidth = (def->widthScale * world->zoom + 8) / 16;
    gFxWidth = gFxWidth <= 1 ? 1 : gFxWidth >= 5 ? 5 : gFxWidth;
    gFxColors = (unsigned char *)boltColors[def->colorRow];
    gFxSegments = 0;
    gFxDef = 0;
    int speed = target->src.owner->owner == world->localOwner ? gLocalSpeed : gRemoteSpeed;
    int steps = speed * def->jitterMin / 20 + 1;
    int count = def->count;
    FxPoint aim;
    aim.x = to.x;
    aim.y = to.y;
    ASSERT(ExploStart(-2, target) != 0);
    for (int n = 0; n < count; n++) {
        if (n == 0 || def->repeatSound)
            WeaponSound(&from, sound, 0, 0, MAX(weapon->at33, weapon->at2f));
        if (count > 1) {
            int jitter = world->zoom / 3;
            int half = jitter / 2;
            to.x = aim.x - half + next_link_seed(&gFxSeed) % jitter;
            to.y = aim.y - half + next_link_seed(&gFxSeed) % jitter;
        }
        for (int i = 1; i <= steps; i++) {
            gFxFrom = from;
            gFxTo.x = (to.x - from.x) * i / steps + from.x;
            gFxTo.y = (to.y - from.y) * i / steps + from.y;
            gFxShade = steps - i;
            FxRedraw(world);
        }
        if (count > 1 && n != count - 1) {
            gFxShade = 8;
            FxRedraw(world);
        }
    }
}



























inline int Distance2(FxPoint *a, FxPoint *b)
{
    return (b->x - a->x) * (b->x - a->x) + (b->y - a->y) * (b->y - a->y);
}

void FxRail(World *world, FxPoint from, FxPoint to, Weapon *weapon, int flags, FxTarget *target, FxPoint *impact, int quiet)
{
    FxDef *def = fxDefs[weapon->type];
    int sound = weaponSoundTable[weapon->type];
    gFxWidth = (def->widthScale * world->zoom + 8) / 16;
    gFxWidth = gFxWidth <= 1 ? 1 : gFxWidth >= 5 ? 5 : gFxWidth;
    gFxColors = (unsigned char *)boltColors[def->colorRow];
    gFxSegments = ISqrt(Distance2(&target->src.pos, &target->dst.pos)) / def->segmentLength + 2;
    int speed = world->localOwner == target->src.owner->owner ? gLocalSpeed : gRemoteSpeed;
    int steps = speed * def->railSteps / 10 + 1;
    signed char count=def->count;
    ASSERT(ExploStart(-3, target) != 0);
    for (int n = 0; n < count; n++) {
        if (n == 0 || def->repeatSound)
            WeaponSound(&from, sound, 0, 0, MAX(weapon->at33, weapon->at2f));
        for (int i = 0; i < steps; i++) {
            gFxFrom = from;
            gFxTo = to;
            gFxShade = i;
            gFxDef = def;
            FxRedraw(world);
        }
    }
}






























void FxSpray(World *world, FxPoint from, FxPoint to, Weapon *weapon, int flags, FxTarget *target, FxPoint *impact, int quiet)
{
    FxDef *def = fxDefs[weapon->type];
    int sound = weaponSoundTable[weapon->type];
    if (!def->repeatSound)
        WeaponSound(&from, sound, 0, 0, MAX(weapon->at33, weapon->at2f));
    FxTarget aim = *target;
    aim.dst.pos = *impact;
    FxPart parts[20];
    memset(parts, 0, sizeof(parts));
    int count = def->count;
    for (int i = 0; i < count; i++) {
        ExploRecordFx *e = ExploAddPart(def->colorRow, def->widthScale);
        ASSERT(e != 0);
        PartInit(&parts[i], def, e, &from, &to, weapon, &aim, i);
        PartSetup(&parts[i], def->part[0], def->part[1], def->part[2], def->part[3], def->part[4]);
    }
    int glow=def->railSteps; FxOwner *actor=target->src.owner;
    if (glow)
        glow += actor->at26;
    ExploAddGlow(def->jitterMin, def->jitterMax, target->src.owner, flags, 0);
    ExploAddGlow(glow, def->glow, target->src.owner, flags, def->glowRepeat);
    for (;;) {
        int done = 1;
        for (int j = 0; j < count; j++) {
            if (!PartStep(&parts[j], 0))
                done = 0;
            if (def->repeatSound && PartHit(&parts[j]) && !parts[j].hitSounded) {
                WeaponSound(&from, sound, 0, 0, MAX(weapon->at33, weapon->at2f));
                parts[j].hitSounded = 1;
            }
        }
        if (done)
            break;
        FxRedraw(world);
    }
}





































void FxBurst(World *world, FxPoint from, FxPoint to, Weapon *weapon, int flags, FxTarget *target, FxPoint *impact, int quiet)
{
    FxDef *def = fxDefs[weapon->type];
    int sound = weaponSoundTable[weapon->type];
    if (!def->repeatSound)
        WeaponSound(&from, sound, 0, 0, MAX(weapon->at33, weapon->at2f));
    FxTarget aim = *target;
    aim.dst.pos = *impact;
    FxPart parts[20];
    memset(parts, 0, sizeof(parts));
    int count = def->count;
    for (int i = 0; i < count; i++) {
        ExploRecordFx *e = ExploAddPart(def->widthScale, def->jitterMin);
        ASSERT(e != 0);
        e->fps = 0;
        PartInit(&parts[i], def, e, &from, &to, weapon, &aim, i);
        PartSetup(&parts[i], def->jitterMax, def->railSteps, def->glow, def->glowRepeat, def->part[0]);
    }
    for (;;) {
        int done = 1;
        for (int j = 0; j < count; j++) {
            if (!PartStep(&parts[j], 1))
                done = 0;
            if (def->repeatSound && PartHit(&parts[j]) && !parts[j].hitSounded) {
                WeaponSound(&from, sound, 0, 0, MAX(weapon->at33, weapon->at2f));
                parts[j].hitSounded = 1;
            }
        }
        if (done)
            break;
        FxRedraw(world);
    }
}

// Declarations used by FxFire.
extern char *gFog;                                          // 004d3604
extern int gLocalActorKind;
extern void ExploCompact(int all = 0);                      // 0045fe90 (sim\explo.c)
extern void AnimActionConfigure(void *, int, int);
extern void WeaponRingHit(Weapon *weapon, FxPoint *impact, int actor = 0, int ring = 1);   // 00460980
extern void WeaponHitMarks(Weapon *weapon, FxPoint *impact, int actor);            // 00460bc0
inline int CellUnseen(int cell) { return !gFog[cell]; }

void FxFire(World *world, FxPoint from, FxPoint to, Weapon *weapon, int flags, FxTarget *target, FxPoint *impact, int quiet)
{
    int type = weapon->type;
    FxDef *def = fxDefs[type];
    int sound = weaponSoundTable[type];
    int hidden = 0;
    if (CellUnseen(target->src.cell) && CellUnseen(target->dst.cell))
        hidden = 1;
    if (from.x < gViewRect.left && to.x < gViewRect.left)
        hidden = 1;
    if (from.x > gViewRect.right && to.x > gViewRect.right)
        hidden = 1;
    if (from.y < gViewRect.top && to.y < gViewRect.top)
        hidden = 1;
    if (from.y > gViewRect.bottom && to.y > gViewRect.bottom)
        hidden = 1;
    if (hidden) {
        if (sound)
            WeaponSound(&from, sound, 0, 0, MAX(weapon->at33, weapon->at2f));
        return;
    }
    ExploCompact(1);
    viewportClipLeft = gViewRect;
    world->flags &= ~1;
    viewportClipLeft = gViewRect;
    if (def->anim && target->src.owner)
        AnimActionConfigure(target->src.owner->action, def->anim, -1);
    switch (def->kind) {
    case 0:
        FxBeam(world, from, to, weapon, flags, target, impact, quiet);
        break;
    case 1:
        FxRail(world, from, to, weapon, flags, target, impact, quiet);
        break;
    case 2:
        FxSpray(world, from, to, weapon, flags, target, impact, quiet);
        break;
    case 3:
        FxBurst(world, from, to, weapon, flags, target, impact, quiet);
        break;
    }
    if (quiet == 0)
        WeaponRingHit(weapon, impact);
    int owner = target->src.owner ? target->src.owner->owner : 0;
    int speed = owner == gLocalActorKind ? gLocalSpeed : gRemoteSpeed;
    if (speed >= 10 && quiet == 0)
        WeaponHitMarks(weapon, impact, 0);
    ExploCompact();
    WorldDrawBegin(world);
    WorldDrawTerrain(world);
}



