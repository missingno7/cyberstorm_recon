// sim\simgad.c - simulation gadgets (working labels; names are guesses unless bound)
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
static char simgadSourceFile[] = __FILE__;
// Four unreferenced shapes precede the three used ones in this object's _DATA (004a03cc); grouping is a guess.
SimTrapezoid simGadgetShapes[4] = { { -3, -1, -1, 31, -1, 66 }, { -3, 62, 62, 31, -5, 62 }, { -1, -1, 62, 31, -1, -1 }, { -1, -1, 62, 31, 62, 62 } };
SimTrapezoid g4a042c = { 6, 70, 71, 37, 6, 133 }, g4a0444 = { 37, 6, 133, 100, 6, 133 }, g4a045c = { 101, 6, 133, 131, 70, 71 };

class SimMapGadget : public GuiControl {
public:
    U32 state; long kind, data;
    SimMapGadget(U32 style, long k, long d, U32 key);
    virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned, U32); virtual int HitTest(const GuiPoint *);
    void ClipFrame() { long top = y - 20, w = width + 40, h = height + 40, left = x - 20; MenuSetClip(&gMenuClip, left, top, w, h); }
};
#pragma aux shared_image_destructor "W?$dt:PilotImageButton$n()_";
class SimImageGadget : public GuiControl {
public:
    U32 state; long image, repeat;
    SimImageGadget(long n, long delay, U32 key); virtual __pragma("shared_image_destructor") ~SimImageGadget();
    virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned, U32); virtual int HitTest(const GuiPoint *);
};
class SimValueGadget : public GuiControl {
public:
    U32 state; long value;
    SimValueGadget(U32 key);
    virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned, U32);
    long Value(); void SetValue(long);
};

struct ActorRecord; struct ActorUnit; struct Context;
#pragma pack(push,1)
struct ExploPoint { long x,y; ExploPoint() {} ExploPoint(long a,long b) : x(a),y(b) {} ExploPoint(const ExploPoint &p) : x(p.x),y(p.y) {} };
struct AnimRect { long x,y,width,height; };
struct AnimAction {
    int id; unsigned char kind,state,variant,rate,frame,frames,flags; signed char dirty; unsigned short type;
    int deadline; ExploPoint position, destination; int target,heading; ExploPoint screen;
    int height; unsigned char color,phase,a38,a39,a3a; AnimRect rect; int scale; ExploPoint velocity,offset; int a5f,a63;
    int IsMoving() { return state != 1 && state != 7 && state != 14 && state != 17 && state != 18; }
};
/* ---- sim\simmisc.c part (working assumption) ---- */
struct SimGame {
    unsigned char unknown00[0x2c]; unsigned char flags; unsigned char unknown2d[0xb]; ActorRecord *focus; unsigned char unknown3c[0x44]; unsigned char oldUnit[0x10b]; unsigned char unknown18b[0x2c2-0x18b];
    int network;                        // +0x2c2
    unsigned char unknown2c6[0x986 - 0x2c6];
    int player;                         // +0x986
    int localPlayer; unsigned char unknown98e[0x9a6-0x98e]; unsigned char weaponContext[0xfa1-0x9a6]; Context *Weapons() { return (Context *)weaponContext; } int zoom; unsigned char unknownfa5[0x10e9-0xfa5];
    int quiet;                          // +0x10e9
};
struct ActorPosition { long x, y; };
struct ActorLevel { unsigned char unknown00[2]; unsigned char percent; unsigned char Percent() { return percent; } };
struct ActorUnit { unsigned char unknown00[0x12]; unsigned char type; unsigned char unknown13[0x61 - 0x13]; ActorLevel level; unsigned char unknown64[0x10b-0x64]; };
struct ActorItem { unsigned char unknown00[0x2b]; long full; long empty; };
struct ActorRecord {
    int id; unsigned char unknown04[4];
    int owner;                          // +0x08
    int targetId; unsigned char unknown10[8];
    ActorUnit *unit;                    // +0x18
    void *logic; void *Pilot(){return logic;}                        // +0x1c
    AnimAction action; char unknown87;  // action fields end at +0x86; byte +0x87 is unmodeled
    unsigned char alive; signed char hidden; unsigned char rest[0x98-0x8a];
};
struct SimSound { long priority, volume, pan, level; };

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

struct Shell; extern void DeathMovie(Shell *, void *);
extern "C" int sprintf(char *, const char *, ...);
extern void LogicTextAppend(LogicSystem *, const char *);
extern void ViewToScreen(void *, ExploPoint *, ExploPoint *);
extern void TargetObjectInit(char *, ActorPosition *, int);
extern ActorRecord *ActorFindByOwner(int);
extern ActorRecord *ActorFindOwnerNext(ActorRecord *, int);
extern unsigned ISqrt(unsigned);
void ModeEvent(SimGame *game, ActorRecord *actor, int amount, int message);
void SimMessage(SimGame *game, const char *text);
void SimActorKilled(SimGame *game, ActorRecord *actor, int message, int report);
void SimSoundSetup(SimSound *sound, ExploPoint *at);

#pragma pack(pop)
#pragma pack(push,1)
struct TargetSide { int id; ExploPoint cell,position; char unknown14[0x44-0x14]; ActorRecord *actor; int actorId,heading,band; };
struct TargetRecord { TargetSide source,target; int distance,elevation,modifier; };
struct Weapon { char unknown00[0x28]; signed char skill; char unknown29[2]; int energy; char unknown2f[0xd]; signed char rings,spread; char unknown3e[8]; };
struct WeaponScore { int state,accuracy,power,chance,rangeFactor,maximum,expected,ammoFactor; Weapon weapon; char unknown66[8]; };
struct Context { int id; TargetRecord target; char fog[0x27]; WeaponScore scores[8]; };
struct UnitSlot { unsigned char id,kind,health,charge; unsigned ammo; };
#pragma pack(pop)
struct WeaponPopup { void Invalidate(); void SetMode(int,int=0,int=0); }; extern WeaponPopup simActions;
struct ReportScreen { void SetOther(ActorRecord *,int,int); }; extern ReportScreen simPilot;
struct PacketList { void Fire(ActorRecord *,int,int,int,int); void DeadPilot(ActorRecord *); }; 
extern unsigned MissionRandomTable,gShotSeed,gFxSeed;
extern int weaponStatusSounds[],gPilotSkill,gApplyTotalLoss,gApplyTotalDamage;
void TargetInitializeRecord(void *,TargetRecord *,int,int);
ActorRecord *ActorGetByIndex(int);
void WeaponPrepare(Context *,TargetRecord *,int);
int WeaponStatus(Context *,int);
Weapon *UnitFogObject(ActorUnit *,int,void *);
int SimSpendOnActor(SimGame *,ActorRecord *,int);
int PointAngle(ExploPoint *,ExploPoint *);
int GridClamp(int);
void ActorSetHeading(ActorRecord *,int);
void AnimActionUpdateTrajectory(int *,int *);
void AnimActionInterpolateDrawPos(void *,ExploPoint *,int,void *,void *);
void SimDrawFrame(SimGame *);
void WorldDrawTerrain(SimGame *);
void LogicCopyActorValues(void *,int,int);
void FxFire(SimGame *,ExploPoint,ExploPoint,Weapon *,int,TargetRecord *,ExploPoint *,int);
void ApplyDamage(int,int,int,Weapon *);
void ReportSelect(ReportScreen *,ActorRecord *);
extern "C" unsigned next_link_seed(unsigned *);
int RandomRange(unsigned *,int,int);
extern "C" ExploPoint *PointFromAngle(ExploPoint *,int,int);
int VectorLength(int,int);
long GridPointToIndex(const char *,ExploPoint *);
inline int FireClamp(int n,int lo,int hi) { return n<=lo?lo:n>=hi?hi:n; }

struct SimGame;
extern int SimFire(SimGame *, ActorRecord *, long, long, long);

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


