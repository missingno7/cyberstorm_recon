// sim\simfire.c - weapon fire (working labels; names are guesses unless bound)
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
extern unsigned MissionRandomTable,simItemSeed,gFxSeed;
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
// Indexed by SimFire at 00452a38 (not yet used by the SimFire below); it precedes the path string in _DATA.
int simFireTable[13] = { 0, 182, 7, 182, 186, 184, 188, 196, 189, 197, 166, 182, 182 };
static char simfireSourceFile[] = __FILE__;


int SimFireAt(SimGame *game, void *target, long *range, long mode)
{
    return SimFire(game, (ActorRecord *)target, -*range, mode, -1);
}

int SimFire(SimGame *game, ActorRecord *actor, long destination, long slot, long forced)
{
    TargetRecord target;
    Weapon weaponBuffer;
    GridPoint fireAt, missWorld, screen, missTo;
    ActorRecord *other;
    int targetCell,angle,scatter;
    unsigned seed;
    int hit;
    Weapon *weapon;
    if (!destination) return 0;
    if (!actor || !actor->active) return 0;
    TargetInitializeRecord(game,&target,actor->id,destination);
    other=ActorGetByIndex(target.target.actorId);
    seed=MissionRandomTable;
    simItemSeed=seed;
    WeaponPrepare(game->Weapons(),&target,0);
    int status=WeaponStatus(game->Weapons(),slot);
    if (status && forced<0) {
        if (actor->owner==game->player) SoundTrigger(simFireTable[status],0);
        return 0;
    }
    angle=GridClamp(PointAngle(&target.source.position,&target.target.position));
    if (!actor->action.IsMoving() && angle*0x2aaaaabc!=actor->action.direction) {
        ActorSetHeading(actor,angle);
        game->flags|=4;
    }
    weapon=UnitFogObject(actor->unit,slot+11,&weaponBuffer);
    if (actor->owner!=game->localPlayer && !SimSpendOnActor(game,actor,weapon->energy) && forced<0) return 0;
    if (other) *(ActorUnit *)game->oldUnit=*other->unit;
    if (actor->owner==game->player) { simActions.Invalidate(); simActions.SetMode(0); }
    UnitSlot *unitSlot=&actor->unit->slots[slot+11];
    unitSlot->uses += unitSlot->uses!=250;
    unitSlot->amount -= unitSlot->amount!=0;
    unsigned chance=game->Weapons()->scores[slot].chance;
    if (forced>=0) chance=forced;
    hit=next_link_seed(&simItemSeed)%100<chance;
    if (game->network && ModeCheck(game)) ((PacketList *)modeLogName)->Fire((ActorRecord *)target.source.actor,target.target.actorId,(unsigned char)slot,seed,chance);
    AnimAction action=actor->action;
    AnimActionUpdateTrajectory((int *)&action,(int *)gView);
    AnimActionInterpolateDrawPos(&action,&fireAt,slot,PopFindItem(actor->unit,0,-1),gView);
    targetCell=action.resource;
    SimDrawFrame(game);
    WorldDrawTerrain(game);
    if (other) LogicCopyActorValues(&g4dd0c0,(int)actor,(int)other);
    if (hit) {
        SendEvent(actor->pilot,0x6e,(void *)(int)weapon->skill,gPilotSkill*40/100);
        SendEvent(actor->pilot,0x6f,(void *)40);
        screen.x=0; screen.y=0;
        if (other) {
            screen=other->action.screen;
            screen.x+=next_link_seed(&gFxSeed)%game->zoom-game->zoom/2;
            screen.y+=next_link_seed(&gFxSeed)%game->zoom-game->zoom/2;
            screen.y-=other->action.rect.height/5;
        } else ViewToScreen(gView,&screen,&target.target.position);
        FxFire(game,fireAt,screen,weapon,slot,&target,&target.target.position,1);
        ApplyDamage((int)game,target.target.actorId,targetCell,weapon);
        if (actor->owner==game->player) simActions.SetMode(2,gApplyTotalLoss,gApplyTotalDamage);
        if (other && other->owner==game->player) simPilot.SetOther(actor,gApplyTotalLoss,gApplyTotalDamage);
    } else {
        SendEvent(actor->pilot,0x6e,(void *)(int)weapon->skill,gPilotSkill*20/100);
        SendEvent(actor->pilot,0x6f,(void *)15);
        if (actor->owner==game->player) simActions.SetMode(1);
        if (other && other->owner==game->player) simPilot.SetOther(actor,-1,-1);
        int distance=VectorLength(target.target.position.x-target.source.position.x,target.target.position.y-target.source.position.y);
        angle=PointAngle(&target.source.position,&target.target.position);
        int error=FireClamp((int)((100-chance)*256)/100,0,256);
        scatter=(distance/10)*error>>8;
        distance+=RandomRange(&simItemSeed,-(distance/20+scatter),distance/20+scatter);
        int deviation=(error*15>>8)+5;
        angle+=RandomRange(&simItemSeed,-deviation,deviation)*0xb60b61;
        if (weapon->rings<=1 && !weapon->spread) distance+=distance/10+500;
        PointFromAngle(&missWorld,distance,angle);
        missWorld.x+=target.source.position.x; missWorld.y+=target.source.position.y;
        int cell=GridPointToIndex(fallbackName,&missWorld);
        ViewToScreen(gView,&missTo,&missWorld);
        FxFire(game,fireAt,missTo,weapon,slot,&target,&missWorld,0);
        if ((weapon->rings>1 || weapon->spread) && cell) {
            ApplyDamage((int)game,cell,targetCell,weapon);
            if (gApplyTotalLoss || gApplyTotalDamage) {
                if (actor->owner==game->player) simActions.SetMode(2,gApplyTotalLoss,gApplyTotalDamage);
                if (other && other->owner==game->player) simPilot.SetOther(actor,gApplyTotalLoss,gApplyTotalDamage);
            }
        }
    }
    if (other && other->owner==game->player) ReportSelect(&simPilot,other);
    if (other) { int visible=!other->hidden && other->action.state!=17; if (!visible) actor->targetId=0; }
    if (actor==game->focus) game->flags|=16;
    return 1;
}

