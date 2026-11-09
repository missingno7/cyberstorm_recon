/* Object 00452950..00454210 (cs11). DGROUP evidence: the file-name statics "sim\simfire.c",
 * "sim\simgad.c" and "sim\simmisc.c" follow sim\simgame.c's data; the trapezoid tables at
 * 004a042c/44/5c sit in the sim\simgad.c data block. No assert, so no line evidence.
 * Names are working labels. Profile ot-oe (orphan copies of small functions are inlined). */
typedef unsigned long U32;
struct GuiRect { long x, y, width, height; };
struct GuiPoint { long x, y; };
struct ViewRect { int left, top, right, bottom; };
struct MenuImage { U32 offset; unsigned short width, height; U32 flags; };
struct MenuImages { U32 count; MenuImage images[1]; };
struct StrPool { char *GetString(long, int = 0); };
struct SoundSettings;
struct GameTimer;
extern void GameTimerSet(GameTimer *, unsigned long);
extern int GameTimerExpired(GameTimer *);
struct GameTimer { unsigned long start, end, duration; GameTimer(unsigned long n) { GameTimerSet(this, n); } };

extern char g513888;
extern MenuImages *gMenuImages;
extern GuiRect gMenuClip;
extern ViewRect viewportClipLeft, gBriefDrawSource;
extern StrPool *gGuiStringPool;
extern long gGuiStringBase;
extern int SoundPlay(unsigned, const SoundSettings *);
extern int MenuControlChanged(void *, unsigned, U32 *);
extern void MenuDrawImage(void *, const MenuImage *, long, long);
extern void MenuSetClip(GuiRect *, long, long, long, long);
extern void MenuLine(void *, long, long, long, long);
extern void MenuDrawPanel(const MenuImage *, const GuiRect *);
extern void MenuDrawWrapped(const char *, long, long, long, long, const GuiRect *, int = 0, int = 0);
extern MenuImage *MenuImageData(void *, const MenuImage *);

struct GuiControlData : GuiRect { unsigned char style[4]; U32 attributes, shortcut; };
class GuiControl : public GuiControlData {
public:
    GuiControl(U32, U32); virtual ~GuiControl(); virtual void Reset(unsigned); virtual void Draw(unsigned);
    virtual int Event(unsigned, U32); virtual int HitTest(const GuiPoint *);
    void Clip() { long left = x, top = y, w = width, h = height; MenuSetClip(&gMenuClip, left, top, w, h); }
    void ResetCore() { attributes &= ~0x1060000UL; }
    int Contains(const GuiPoint *p) const { return p->x >= x && p->x < x + width && p->y >= y && p->y < y + height; }
};

struct SimTrapezoid { long top, topLeft, topRight, bottom, bottomLeft, bottomRight; };
extern SimTrapezoid g4a042c, g4a0444, g4a045c;

class SimMapGadget : public GuiControl {
public:
    U32 state; long kind, data;
    SimMapGadget(U32 style, long k, long d, U32 key);
    virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned, U32); virtual int HitTest(const GuiPoint *);
    void ClipFrame() { long top = y - 20, w = width + 40, h = height + 40, left = x - 20; MenuSetClip(&gMenuClip, left, top, w, h); }
};
class SimImageGadget : public GuiControl {
public:
    U32 state; long image, repeat;
    SimImageGadget(long n, long delay, U32 key);
    virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned, U32); virtual int HitTest(const GuiPoint *);
};
class SimValueGadget : public GuiControl {
public:
    U32 state; long value;
    SimValueGadget(U32 key);
    virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned, U32);
    long Value(); void SetValue(long);
};

struct SimGame;
extern int SimFire(SimGame *, void *, long, long, long);

int SimFireAt(SimGame *game, void *target, long *range, long mode)
{
    return SimFire(game, target, -*range, mode, -1);
}

int SimInTrapezoid(const SimTrapezoid *t, const GuiPoint *p)
{
    if (p->y >= t->top && p->y <= t->bottom) {
        long left = t->topLeft + (t->bottomLeft - t->topLeft) * (p->y - t->top) / (t->bottom - t->top);
        long right = t->topRight + (t->bottomRight - t->topRight) * (p->y - t->top) / (t->bottom - t->top);
        return left <= p->x && right >= p->x;
    }
    return 0;
}

void SimDrawTrapezoid(const SimTrapezoid *t, long dx, long dy)
{
    MenuLine(&g513888, t->topLeft + dx, t->top + dy, t->topRight + dx, t->top + dy);
    MenuLine(&g513888, t->topRight + dx, t->top + dy, t->bottomRight + dx, t->bottom + dy);
    MenuLine(&g513888, t->bottomRight + dx, t->bottom + dy, t->bottomLeft + dx, t->bottom + dy);
    MenuLine(&g513888, t->bottomLeft + dx, t->bottom + dy, t->topLeft + dx, t->top + dy);
}

int SimImagePixel(const MenuImage *image, long x, long y)
{
    MenuImage *data = MenuImageData(&g513888, image);
    unsigned char *pixels = (unsigned char *)data + data->offset;
    return pixels[data->width * y + x] != 0;
}

SimMapGadget::SimMapGadget(U32 style, long k, long d, U32 key) : GuiControl(style, key), state(0), kind(k), data(d)
{
}

int SimMapGadget::HitTest(const GuiPoint *p)
{
    if (!Contains(p)) return 0;
    long dy = p->y - y, dx = p->x - x;
    GuiPoint at;
    at.x = dx;
    at.y = dy;
    switch (kind) {
    case 1:
        if (SimInTrapezoid(&g4a0444, &at) || SimInTrapezoid(&g4a042c, &at) || SimInTrapezoid(&g4a045c, &at)) return 1;
        return 0;
    case 6:
    case 7:
        return 1;
    }
    return 1;
}

void SimMapGadget::Draw(unsigned index)
{
    U32 previous = state;
    if (MenuControlChanged(this, index, &state) || ((kind == 6 || kind == 7) && (state & 0x80000UL))) {
        int lit = (state & 0x80000UL) != 0;
        viewportClipLeft = gBriefDrawSource;
        Clip();
        if ((state & 0x80000UL) && !(previous & 0x80000UL)) SoundPlay(392, 0);
        switch (kind) {
        case 1:
            MenuDrawImage(&g513888, &gMenuImages->images[lit + 0x5c], 6, 2);
            MenuSetClip(&gMenuClip, 6, 2, 0x94, 0x96);
            break;
        case 6:
            MenuDrawImage(&g513888, &gMenuImages->images[lit + 0x5e], x - 13, y - 6);
            ClipFrame();
            break;
        case 7:
            MenuDrawImage(&g513888, &gMenuImages->images[lit + 0x60], x - 10, y - 8);
            ClipFrame();
            break;
        }
    }
}

void SimMapGadget::Reset(unsigned)
{
    state = 0;
    ResetCore();
}

int SimMapGadget::Event(unsigned index, U32 event)
{
    static GameTimer timer(0);
    if (!(event & 1)) {
        if (event & 4) GameTimerSet(&timer, 400);
        else {
            if (!GameTimerExpired(&timer)) return 0;
            GameTimerSet(&timer, 200);
        }
    }
    return index + 1;
}

int SimValueGadget::Event(unsigned index, U32)
{
    return index + 1;
}

SimImageGadget::SimImageGadget(long n, long delay, U32 key) : GuiControl(28, key), state(0), image(n), repeat(delay)
{
}

int SimImageGadget::HitTest(const GuiPoint *p)
{
    if (!Contains(p)) return 0;
    return SimImagePixel(&gMenuImages->images[image], p->x - x, p->y - y);
}

void SimImageGadget::Draw(unsigned index)
{
    U32 previous = state;
    if (MenuControlChanged(this, index, &state)) {
        U32 lit = state & 0x80000UL, pressed = state & 0x20000UL;
        long n = image;
        if (lit) n += (pressed == 0) + 1;
        MenuDrawImage(&g513888, &gMenuImages->images[n], x, y);
        Clip();
        long color = (pressed == 0) + 0x13;
        const char *s = gGuiStringPool->GetString(gGuiStringBase + index, 0);
        if (s && *s) MenuDrawWrapped(s, 0, 0, 1, color, this, 0, 0);
        if ((state & 0x80000UL) && !(previous & 0x80000UL)) SoundPlay(392, 0);
    }
}

void SimImageGadget::Reset(unsigned)
{
    ResetCore();
    state &= ~0x1000000UL;
}

int SimImageGadget::Event(unsigned index, U32 event)
{
    static GameTimer repeatTimer(0);
    int result = index + 1;
    if (repeat) {
        if (event & 4) {
            GameTimerSet(&repeatTimer, repeat * 500 / 1000);
            return result;
        }
        if ((event & 8) && GameTimerExpired(&repeatTimer)) {
            GameTimerSet(&repeatTimer, repeat * 200 / 1000);
            return result;
        }
    }
    else if (event & 16) return result;
    return 0;
}

SimValueGadget::SimValueGadget(U32 key) : GuiControl(16, key), state(0), value(0)
{
}

void SimValueGadget::Reset(unsigned)
{
    ResetCore();
    value = 0;
    state = value;
}

void SimValueGadget::Draw(unsigned index)
{
    U32 previous = state;
    if (MenuControlChanged(this, index, &state)) {
        long image = 18, color = 20;
        if (state & 0x80000UL) {
            image = 36;
            if (state & 0x20000UL) {
                image = 27;
                color = 19;
                if (!(previous & 0x20000UL)) SoundPlay(415, 0);
            }
            if (!(state & 0x20000UL) && !(previous & 0x80000UL)) SoundPlay(392, 0);
        }
        if (value) {
            image = 27;
            color = 19;
        }
        MenuDrawPanel(&gMenuImages->images[image], this);
        Clip();
        const char *s = gGuiStringPool->GetString(gGuiStringBase + index, 0);
        if (s) MenuDrawWrapped(s, 0, 0, 1, color, this, 0, 0);
    }
}

long SimValueGadget::Value()
{
    return value;
}

void SimValueGadget::SetValue(long v)
{
    value = v;
    attributes &= ~0x1000000UL;
}

/* ---- sim\simmisc.c part (working assumption) ---- */
struct SimGame {
    unsigned char unknown00[0x2c2];
    int network;                        // +0x2c2
    unsigned char unknown2c6[0x986 - 0x2c6];
    int player;                         // +0x986
    unsigned char unknown98a[0x10e9 - 0x98a];
    int quiet;                          // +0x10e9
};
struct ActorPosition { long x, y; };
struct ActorLevel { unsigned char unknown00[2]; unsigned char percent; unsigned char Percent() { return percent; } };
struct ActorUnit { unsigned char unknown00[0x12]; unsigned char type; unsigned char unknown13[0x61 - 0x13]; ActorLevel level; };
struct ActorItem { unsigned char unknown00[0x2b]; long full; long empty; };
struct ActorRecord {
    unsigned char unknown00[8];
    int owner;                          // +0x08
    unsigned char unknown0c[0x18 - 0xc];
    ActorUnit *unit;                    // +0x18
    void *logic;                        // +0x1c
    unsigned char unknown20[0x32 - 0x20];
    ActorPosition position;             // +0x32
    unsigned char unknown3a[0x88 - 0x3a];
    unsigned char alive;                // +0x88
};
struct SimSound { long priority, volume, pan, level; };
struct ExploPoint { long x, y; };
struct LogicSystem;
extern LogicSystem g4dd0c0;
extern SimGame modeCheckName;           // 004d2460, the running game (label from symbols.json)
extern char modeLogName[];
extern char fallbackName[];
extern char gView[];
extern StrPool targetBriefResource;
extern ViewRect gViewRect;
extern int gZoom;
extern ActorRecord *g4d2498;
extern void *g4d249c;
extern ActorItem *PopFindItem(ActorUnit *, int, int);
extern void SendEvent(void *, int, void * = 0, int = 0, int = 0);
extern int SoundTrigger(unsigned, int);
extern unsigned SimLogicFlags(void *);
extern void SimRefreshPanel(SimGame *, int);
extern int ModeCheck(SimGame *);
extern void ModeLogActor(char *, ActorRecord *);
extern void SimReportLoss(SimGame *, void *);
extern "C" int sprintf(char *, const char *, ...);
extern void LogicTextAppend(LogicSystem *, const char *);
extern void ViewToScreen(void *, ExploPoint *, ExploPoint *);
extern void SimSoundDirection(long, long);
extern void TargetObjectInit(char *, ActorPosition *, int);
extern ActorRecord *ActorFindByOwner(int);
extern ActorRecord *ActorFindOwnerNext(ActorRecord *, int);
extern unsigned ISqrt(unsigned);
void ModeEvent(SimGame *game, ActorRecord *actor, int amount, int message);
void SimMessage(SimGame *game, const char *text);
void SimActorKilled(SimGame *game, ActorRecord *actor, int message, int report);
void SimSoundSetup(SimSound *sound, ExploPoint *at);

int Interpolate(int x0, int y0, int x1, int y1, int x)
{
    if (x <= x0) return y0;
    if (x >= x1) return y1;
    return (x - x0) * (y1 - y0) / (x1 - x0) + y0;
}

inline int Clamp(int value, int low, int high)
{
    if (value <= low) return low;
    if (value >= high) return high;
    return value;
}

void SimDamageActor(SimGame *game, ActorRecord *actor, int percent)
{
    ActorUnit *unit = actor->unit;
    ActorItem *item = PopFindItem(unit, 10, -1);
    int level = Clamp(unit->level.Percent(), 0, 100);
    int damage = Interpolate(0, item->empty, 100, item->full, level) * percent / 100;
    if (damage) ModeEvent(game, actor, damage, 0x4b6);
}

void ModeEvent(SimGame *game, ActorRecord *actor, int amount, int message)
{
    void *logic = actor->logic;
    int before = SimLogicFlags(logic) & 1;
    if (amount >= 1000000) SendEvent(logic, 0x7e);
    else if (amount) {
        SendEvent(logic, 0x7d, (void *)amount);
        SendEvent(logic, 0x6b);
        SendEvent(logic, 0x6f, (void *)0xfa);
        if (message == 0x1b3) SoundTrigger(0xa1, 0);
    } else {
        SendEvent(logic, 0x6a);
        SendEvent(logic, 0x6f, (void *)0xf);
    }
    int after = SimLogicFlags(logic) & 1;
    if (actor->owner == game->player) SimRefreshPanel(game, 0);
    if (before && !after) SimActorKilled(game, actor, message, 1);
}

void SimActorKilled(SimGame *game, ActorRecord *actor, int message, int report)
{
    char text[192];
    actor->alive = 0;
    void *logic = actor->logic;
    if (game->network && ModeCheck(game)) {
        ModeLogActor(modeLogName, actor);
        ModeEvent(&modeCheckName, actor, 1000000, 0);
    }
    if (actor->owner == game->player) {
        if (report) SimReportLoss(game, logic);
        SimRefreshPanel(game, 0);
        if (message) {
            const char *format = targetBriefResource.GetString(message, 0);
            sprintf(text, format, actor->logic);
            SimMessage(game, text);
        }
    }
}

void SimMessage(SimGame *game, const char *text)
{
    if (!game->quiet) LogicTextAppend(&g4dd0c0, text);
}

void SimSoundAtActor(SimSound *sound, ActorRecord *actor)
{
    SimSoundSetup(sound, (ExploPoint *)&actor->position);
    if (actor != g4d2498) sound->level = sound->level * 75 / 100;
}

void SimSoundAtTile(SimSound *sound, int tile)
{
    ActorPosition at;
    TargetObjectInit(fallbackName, &at, tile);
    SimSoundSetup(sound, (ExploPoint *)&at);
}

void SimSoundSetup(SimSound *sound, ExploPoint *at)
{
    ExploPoint center, screen;
    ViewToScreen(gView, &screen, at);
    sound->priority = 0;
    sound->volume = Clamp(sound->volume, 0, 200);
    sound->level = Clamp(sound->level, 0, 100);
    sound->pan = Clamp(sound->pan, -100, 100);
    center.y = (gViewRect.top + gViewRect.bottom) / 2;
    center.x = (gViewRect.left + gViewRect.right) / 2;
    SimSoundDirection(screen.x - center.x, screen.y - center.y);
    if (screen.y < gViewRect.top) sound->level -= (gViewRect.top - screen.y) / 100;
    if (screen.y > gViewRect.bottom) sound->level -= (screen.y - gViewRect.bottom) / 100;
    if (screen.x < gViewRect.left) sound->level -= (gViewRect.left - screen.x) / 100;
    if (screen.x > gViewRect.right) sound->level -= (screen.x - gViewRect.right) / 100;
    sound->pan = (screen.x - center.x) / 100;
    sound->level = sound->level * gZoom / 16;
    if (g4d2498 && g4d249c != g4d2498->unit) sound->level /= 2;
}

inline unsigned ActorDistance(const ActorPosition *a, const ActorPosition *b)
{
    long dx = a->x - b->x, dy = a->y - b->y;
    return ISqrt(dx * dx + dy * dy);
}

int SimDistanceToDepot(SimGame *game, ActorRecord *actor)
{
    int owner = actor->owner;
    for (ActorRecord *a = ActorFindByOwner(owner); a; a = ActorFindOwnerNext(a, owner)) {
        if (a->unit->type == 0x1f)
            return Clamp((ActorDistance(&actor->position, &a->position) + 100) / 1000 - 2, 0, 100);
    }
    return -1;
}
