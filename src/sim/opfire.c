// sim\opfire.c: opposing (reaction) fire during the hot-seat briefing. Working names unless bound.
#pragma pack(push,1)
#include "actor.h"
struct ActorPilot { char unknown00[0x12]; int health; };

struct ActorItem;

struct ActorItem { char unknown00[0x13]; int ammo; char unknown17[8]; signed char category, maxUses, rank; char unknown22[9]; int skill; };




struct TargetRecord;
struct SimGame {
    char unknown00[0x2c]; unsigned char screen; char unknown2d[0x2c2-0x2d]; int multi;
    char unknown2c6[0x97e-0x2c6]; int mode; char unknown982[4]; int player; char unknown98a[0x9a6-0x98a]; char targets[0x10f1-0x9a6];
    int extraSaved; char unknown10f5[4]; int remoteEnabled;
};
#pragma pack(pop)
struct TerrainGrid;
extern TerrainGrid fallbackName;
extern char modeCheckName[];
extern unsigned long MissionRandomTable[];
extern int gRemoteActorKind, DAT_004dd0b4, DAT_004dd0b8, gLaunchOption105;
extern unsigned char WeaponEfficiency[];
long *BestLocSetData(TerrainGrid *, long *, long);
extern "C" int LogicCopyPoint(long *, GridPoint *);
extern "C" unsigned long next_link_seed(unsigned long *);
ActorItem *PopFindItem(ActorUnit *, int, int);
int HercEnergy(ActorUnit *);
inline int HercUnsupported2() { return -1; }
inline ActorItem *UnitItem(ActorUnit *unit, int row) { return PopFindItem(unit, row, HercUnsupported2()); }
void TargetInitializeRecord(void *, int *, int, int);
ActorRecord *ActorFindFree();
ActorRecord *ActorFindNext(ActorRecord *);
int f_0045d830(void *, void *, int, int);
void f_00452950(SimGame *, ActorRecord *, ActorRecord *, int);
int ModeCheck(const char *);
void SimGameDrawStatus(SimGame *);
inline int HotbriefOpposingEnabled(SimGame *s) { if (s->multi) return gLaunchOption105; if (s->extraSaved) return s->remoteEnabled; return DAT_004dd0b4; }
inline int HotbriefHealth(ActorPilot *pilot) { return pilot->health > 0; }
static char opfireSourceFile[]=__FILE__; int DAT_004a3a0c[]={70,70,66,63,56,49,42,35,28,21,17,14,10,7,3,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int HotbriefCanSelectAction(ActorRecord *actor, int rankLimit)
{
    if (!HotbriefHealth(actor->pilot))
        return 0;
    ActorUnit *unit = actor->unit;
    UnitSlot *slot = &unit->slots[11];
    int skill = HercEnergy(unit);
    for (int index = 0, row = 11; index < 8; index++, row++, slot++) {
        if (slot->item && slot->condition && !(actor->returnFire & 1 << index == 0)) {
            ActorItem *item = UnitItem(unit, row);
            if (slot->uses < item->maxUses && WeaponEfficiency[item->category] &&
                (!item->ammo || slot->amount) &&
                (actor->owner == gRemoteActorKind || skill >= item->skill) &&
                rankLimit <= item->rank)
                return 1;
        }
    }
    return 0;
}

int HotbriefCheckAction(ActorRecord *source, ActorRecord *actor, int location, int priorLocation, int *target)
{
    if (actor->active == 0)
        return 0;
    long point[2];
    BestLocSetData(&fallbackName, point, location);
    int rank = LogicCopyPoint(point, &actor->action.target);
    if (rank > 20)
        return 0;
    int chance = DAT_004a3a0c[rank];
    if (chance == 0)
        return 0;
    if (chance != 100 && (int)(next_link_seed(MissionRandomTable) % 100) > chance)
        return 0;
    if (priorLocation) {
        long previous[2];
        BestLocSetData(&fallbackName, previous, priorLocation);
        if (LogicCopyPoint(previous, &actor->action.target) < rank)
            return 0;
    }
    if (!HotbriefCanSelectAction(actor, rank))
        return 0;
    TargetInitializeRecord(modeCheckName, target, actor->id, -source->id);
    if (target[0x2b] < 60)
        return 0;
    return 1;
}

void HotbriefProcessOpposingActions(SimGame *s, ActorRecord *actor, int location, int priorLocation)
{
    DAT_004dd0b8 = 0;
    if (s->multi) {
        if (!gLaunchOption105) return;
    } else if (s->extraSaved) {
        if (!s->remoteEnabled) return;
    } else if (!DAT_004dd0b4) return;
    int owner = actor->owner;
    if (owner != s->mode || actor->active == 0) return;
    for (ActorRecord *other = ActorFindFree(); other; other = ActorFindNext(other)) {
        int target[44];
        if (owner != other->owner && HotbriefCheckAction(actor, other, location, priorLocation, target)) {
            DAT_004dd0b8 = 1;
            int index;
            while ((index = f_0045d830(s->targets, target, 0x23, 0)) >= 0) {
                f_00452950(s, other, actor, index);
                DAT_004dd0b8 = 0;
                if (actor->owner == s->player && ModeCheck((const char *)s)) {
                    s->screen |= 0x20;
                    SimGameDrawStatus(s);
                    s->screen &= ~0x20;
                }
                if (actor->active == 0) return;
            }
        }
    }
    DAT_004dd0b8 = 0;
}

void HotbriefEmpty(void)
{
}





