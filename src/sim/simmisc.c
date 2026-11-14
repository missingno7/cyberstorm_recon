// sim\simmisc.c - simulation helpers (working labels; names are guesses unless bound)
/* Object 00452950..00454210 (cs11). DGROUP evidence: the file-name statics "sim\simfire.c",
 * "sim\simgad.c" and "sim\simmisc.c" follow sim\simgame.c's data; the trapezoid tables at
 * 004a042c/44/5c sit in the sim\simgad.c data block. No assert, so no line evidence.
 * Names are working labels. Profile ot-oe (orphan copies of small functions are inlined). */
typedef unsigned long U32;
#include "actor.h"
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


struct ActorItem { unsigned char unknown00[0x2b]; long full; long empty; };









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
extern void ViewToScreen(void *, GridPoint *, GridPoint *);
extern void TargetObjectInit(char *, ActorPosition *, int);
extern ActorRecord *ActorFindByOwner(int);
extern ActorRecord *ActorFindOwnerNext(ActorRecord *, int);
extern unsigned ISqrt(unsigned);
void ModeEvent(SimGame *game, ActorRecord *actor, int amount, int message);
void SimMessage(SimGame *game, const char *text);
void SimActorKilled(SimGame *game, ActorRecord *actor, int message, int report);
void SimSoundSetup(SimSound *sound, GridPoint *at);

#pragma pack(pop)
#pragma pack(push,1)
struct TargetSide { int id; GridPoint cell,position; char unknown14[0x44-0x14]; ActorRecord *actor; int actorId,heading,band; };
struct TargetRecord { TargetSide source,target; int distance,elevation,modifier; };
struct Weapon { char unknown00[0x28]; signed char skill; char unknown29[2]; int energy; char unknown2f[0xd]; signed char rings,spread; char unknown3e[8]; };
struct WeaponScore { int state,accuracy,power,chance,rangeFactor,maximum,expected,ammoFactor; Weapon weapon; char unknown66[8]; };
struct Context { int id; TargetRecord target; char fog[0x27]; WeaponScore scores[8]; };

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
int PointAngle(GridPoint *,GridPoint *);
int GridClamp(int);
void ActorSetHeading(ActorRecord *,int);
void AnimActionUpdateTrajectory(int *,int *);
void AnimActionInterpolateDrawPos(void *,GridPoint *,int,void *,void *);
void SimDrawFrame(SimGame *);
void WorldDrawTerrain(SimGame *);
void LogicCopyActorValues(void *,int,int);
void FxFire(SimGame *,GridPoint,GridPoint,Weapon *,int,TargetRecord *,GridPoint *,int);
void ApplyDamage(int,int,int,Weapon *);
void ReportSelect(ReportScreen *,ActorRecord *);
extern "C" unsigned next_link_seed(unsigned *);
int RandomRange(unsigned *,int,int);
extern "C" GridPoint *PointFromAngle(GridPoint *,int,int);
int VectorLength(int,int);
long GridPointToIndex(const char *,GridPoint *);
inline int FireClamp(int n,int lo,int hi) { return n<=lo?lo:n>=hi?hi:n; }

struct SimGame;
extern int SimFire(SimGame *, ActorRecord *, long, long, long);
static char simmiscSourceFile[] = __FILE__;

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
    int level = Clamp(unit->slots[10].Condition(), 0, 100);
    int damage = Interpolate(0, item->empty, 100, item->full, level) * percent / 100;
    if (damage) ModeEvent(game, actor, damage, 0x4b6);
}

void ModeEvent(SimGame *game, ActorRecord *actor, int amount, int message)
{
    void *logic = actor->pilot;
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
    actor->active = 0;
    void *logic = actor->pilot;
    if (game->network && ModeCheck(game)) {
        ((PacketList *)modeLogName)->DeadPilot(actor);
        ModeEvent(&modeCheckName, actor, 1000000, 0);
    }
    if (actor->owner == game->player) {
        if (report) DeathMovie((Shell *)game, logic);
        SimRefreshPanel(game, 0);
        if (message) {
            const char *format = targetBriefResource.GetString(message, 0);
            sprintf(text, format, actor->Pilot());
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
    SimSoundSetup(sound, &actor->action.position);
    if (actor != g4d2498) sound->level = sound->level * 75 / 100;
}

void SimSoundAtTile(SimSound *sound, int tile)
{
    ActorPosition at;
    TargetObjectInit(fallbackName, &at, tile);
    SimSoundSetup(sound, (GridPoint *)&at);
}

void SimSoundSetup(SimSound *sound, GridPoint *at)
{
    GridPoint screen;
    ViewToScreen(gView, &screen, at);
    sound->priority = 0;
    sound->volume = Clamp(sound->volume, 0, 200);
    sound->level = Clamp(sound->level, 0, 100);
    sound->pan = Clamp(sound->pan, -100, 100);
    GridPoint center((gViewRect.left + gViewRect.right)/2,(gViewRect.top + gViewRect.bottom)/2);
    VectorLength(screen.x - center.x, screen.y - center.y);
    if (gViewRect.top > screen.y) sound->level -= (gViewRect.top - screen.y) / 100;
    if (gViewRect.bottom < screen.y) sound->level -= (screen.y - gViewRect.bottom) / 100;
    if (gViewRect.left > screen.x) sound->level -= (gViewRect.left - screen.x) / 100;
    if (gViewRect.right < screen.x) sound->level -= (screen.x - gViewRect.right) / 100;
    sound->pan = (screen.x - center.x) / 100;
    sound->level = sound->level * gZoom / 16;
    if (g4d2498 && g4d2498->unit != g4d249c) sound->level /= 2;
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
        if (a->unit->slots[0].item == 0x1f)
            return Clamp((ActorDistance((ActorPosition *)&actor->action.position, (ActorPosition *)&a->action.position) + 100) / 1000 - 2, 0, 100);
    }
    return -1;
}


