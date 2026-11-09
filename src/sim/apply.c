// sim\apply.c - applies a weapon's blast to every actor inside its damage rings.
// Names are working labels (guesses from use); field offsets, globals and line numbers are evidence.
#pragma pack(1)
struct ApplyRing { signed char damage; signed char count; };   // weapon + 0x3e + ring * 2
struct Weapon {
    char at00; unsigned char type; char pad02[0x1d];
    signed char armorClass;                                  // + 0x1f
    char pad20[4];
    signed char falloffNear, falloffFar;                     // + 0x24, + 0x25
    char pad26[9];
    int at2f, at33, at37;                                    // + 0x2f, + 0x33, + 0x37
    char pad3b;
    signed char rings;                                       // + 0x3c
    char pad3d;
    ApplyRing ring[4];                                       // + 0x3e
};
struct WeaponInfo { char pad00[0x33]; int at33; char pad37[0x0f]; };   // 0x46-byte records at 0049cb38
struct UnitSlot { char at00; unsigned char kind; unsigned char health; char at03; int at04; };
struct Unit { char pad00[0x11]; UnitSlot slot[23]; };    // slot[0].kind = unit class, slot[0].health = crew
struct PartRecord { char pad00[9]; int factor; char pad0d[0x0d]; };   // part + 0x1b + slot * 0x1a
struct PartDef { char pad00[2]; int scale; char pad06; unsigned char minimum; char pad08[0x13]; PartRecord record[1]; };
struct ArmorDef {
    char pad00[0x1b];
    signed char *typeFactor;                                 // + 0x1b
    int *classFactor;                                        // + 0x1f
    int bandFactor[6];                                       // + 0x23
    int at3b, at3f, at43, at47;                              // + 0x3b .. + 0x47
};
struct ActorRecord {
    char pad00[8]; int owner; char pad0c[0x0c];
    Unit *unit;                                              // + 0x18
    void *at1c;                                              // + 0x1c
    char at20[0x26];                                         // + 0x20 (world action block)
    int at46;                                                // + 0x46
    char pad4a[0x4d]; char at97;                             // + 0x97
};
struct ApplyPoint { int x, y; ApplyPoint() {} }; struct ApplyPair { int a, b; }; struct BestLocBuffer { char bytes[0x28]; };
struct BestLocCandidate { int at00; ApplyPoint pos; int actor; }; 
#pragma pack()

extern "C" void AssertFail(const char *, int, const char *);
extern long GridClamp(unsigned long);
extern void ReleaseState(void *);
extern "C" int WorldActionNotify(void *, int);
extern int SoundPlay(unsigned int, const void *);
extern int UnitDamage(Unit *unit, int range, int damage, Weapon *weapon);      // 0045b8c0
extern int UnitHealth(Unit *unit, int range);                                  // 0045b9e0
extern char *UnitPart(Unit *unit, int part, int index);                        // 00438374
extern int UnitClass(Unit *unit);                                              // 00439d02
extern int UnitSlotAt(Unit *unit, int range);                                  // 0043ae77
extern int Interpolate(int x0, int y0, int x1, int y1, int x);                 // 00453c70
extern void ModeReport(char *name, ActorRecord *actor, int value);             // 00453cb0
extern int ActorIsDead(ActorRecord *actor);                                    // 00455d10
extern void AnimActionConfigure(void *block, int size, int count);             // 00458c90
extern void WeaponSound(ApplyPoint *at, int sound, int kind, int value, int total);   // 0045dad0
extern void ExploMessage(int message, ActorRecord *actor);                        // 0045f930
extern void ExploAddSmoke(int kind, ActorRecord *actor, int a, int b, int c);        // 0045fc30
extern void FxDamage(WeaponInfo *info, ActorRecord *actor, int shots, int damage);    // 00460800
extern void VoicePlay(int voice, int delay);                                   // 0046cbc6
extern "C" unsigned int next_link_seed(unsigned int *seed);                    // 0047e160

extern char modeCheckName[]; extern int modeSwitch, gLocalActorKind; extern int gMission, gGameTime, gGameTurn, gRedrawFlags; extern unsigned int gRandomSeed;   // 004d3c98 004d374c 004d3784 004d248c 004d3e08

// The module's state (BSS 004dcbc0..004dcc27, declaration order = address order). The point objects have an
// empty constructor, which makes the compiler emit the empty static initializer at the start of the object.
// gApplyFreeShots is also used at 0044c54a/0044c803, gApplyTotalLoss/gApplyTotalDamage at 00452f71..00453278,
// so these are not file-static.
Weapon *gApplyWeapon;                // 004dcbc0
WeaponInfo *gApplyInfo;              // 004dcbc4
ApplyPoint gApplyAttackerPos;        // 004dcbc8
ApplyPoint gApplyAttackerLoc;        // 004dcbd0
ApplyPoint gApplyImpactPos;          // 004dcbd8
ApplyPoint gApplyImpactLoc;          // 004dcbe0
int gApplyDistance, gApplyPercent;   // 004dcbe8, 004dcbec
ActorRecord *gApplyAttacker;         // 004dcbf0
ActorRecord *gApplyActor, *gApplyLastActor;   // 004dcbf4, 004dcbf8
int gApplyBonus, gApplySound, gApplyFreeShots;      // 004dcbfc, 004dcc00, 004dcc04
int gApplyHits, gApplyLoss, gApplyDamage;           // 004dcc08, 004dcc0c, 004dcc10
int gApplyShots, gApplyUnused, gApplyTotalHits, gApplyTotalLoss, gApplyTotalDamage;   // 004dcc14 .. 004dcc24

// Original: both objects lie in CONST (004a25c4, 004a2620), one copy of the path for every ASSERT.
// #pragma data_seg("CONST") around the int table crashes wpp386, so they stay in _DATA here (same relative layout).
static const int slotWeight[23] = { 25, 0, 5, 15, 15, 5, 5, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 1, 1 };   // sums to 100
static const char applySourcePath[] = __FILE__;
#define ASSERT(e) if (!(e)) AssertFail(applySourcePath, __LINE__, 0)

static void ApplyFinish()
{
    int message = 0;
    if (UnitHealth(gApplyActor->unit, gApplyDistance - gApplyActor->at46)) {
        message = gApplyWeapon->at2f;
        if (message != 0 || gApplyWeapon->at37 != gApplyWeapon->at33) {
            if (gApplyLoss == 0)
                message = 0x1b;
            else if (gApplyLoss < 100)
                message = 0x1c;
            else
                message = 0x1d;
        }
    } else if (gApplyLoss != 0) {
        message = 0x1e;
        if (gApplyActor->owner == gLocalActorKind)
            VoicePlay(0xb5, 0);
    }
    if (message != 0) {
        ExploMessage(message, gApplyActor);
        message = gApplyWeapon->at2f * gApplyShots;
        WeaponSound(&gApplyImpactPos, gApplySound, 1, gApplyLoss, message);
    }
    if (gGameTurn > 20) {
        gApplyActor->at97 = gGameTime / 33 + 1;
        gRedrawFlags |= 4;
    }
    if (gApplyDamage > 75)
        AnimActionConfigure(gApplyActor->at20, (gApplyDamage > 150) + 1, 1);
    if (gApplyActor != 0 && gApplyActor != gApplyLastActor) {
        WeaponSound(&gApplyImpactPos, gApplySound, 2, gApplyDamage, gApplyShots * gApplyInfo->at33 + 1);
        FxDamage(gApplyInfo, gApplyActor, gApplyShots, gApplyDamage);
        gApplyLastActor = gApplyActor;
    }
}

static void ApplyOne(int damage)
{
    ASSERT(gApplyActor != 0 && gApplyActor->unit != 0 && gApplyActor->at1c != 0);
    ActorRecord *actor = gApplyActor;
    int range = gApplyDistance - actor->at46;
    Unit *unit = actor->unit;
    int band = GridClamp(range);
    int before = UnitHealth(unit, range);
    int hit = UnitDamage(unit, range, damage, gApplyWeapon) + gApplyBonus;
    int after = UnitHealth(unit, range);
    gApplyHits++;
    gApplyLoss += before - after;
    gApplyDamage += hit;
    gApplyTotalHits++;
    gApplyTotalLoss += before - after;
    gApplyTotalDamage += hit;
    if (hit != 0) {
        int slot = UnitSlotAt(unit, range);
        if (gApplyBonus != 0)
            slot = 0;
        PartRecord *record = &((PartDef *)UnitPart(unit, 0, -1))->record[slot];
        ArmorDef *armor = (ArmorDef *)UnitPart(unit, 1, -1);
        PartDef *part = (PartDef *)UnitPart(unit, slot, -1);
        UnitSlot *entry = &unit->slot[slot];
        int amount = hit * 100;
        int health = entry->health;
        int resist = Interpolate(0, armor->at3f, 100, armor->at3b, health);
        amount -= Interpolate(0, armor->at47, 100, armor->at43, health) * 100;
        amount -= resist * amount / 100;
        if (amount < 0)
            amount = 0;
        amount = part->scale * amount / 100;
        amount = record->factor * amount / 100;
        amount = armor->bandFactor[band] * amount / 100;
        amount = armor->classFactor[gApplyWeapon->armorClass] * amount / 100;
        amount = armor->typeFactor[gApplyWeapon->type] * amount / 50;
        if (amount < 0)
            amount = 0;
        if (unit->slot[0].kind == 9) {
            if (gMission == 410)
                amount = amount * 50 / 100;
            if (gMission == 418)
                amount = amount * 25 / 100;
        }
        int fraction = amount % 100;
        int points = amount / 100;
        if (modeSwitch == 0 && next_link_seed(&gRandomSeed) % 100 < fraction)
            points++;
        int old = health;
        health -= points;
        if (modeSwitch == 0 && UnitClass(unit) == 0x1f && health < 50)
            health = 50;
        if (health <= 0 || part->minimum > health) {
            entry->at03 = 0;
            entry->at04 = 0;
            health = 0;
            if (slot == 0) {
                WorldActionNotify(actor->at20, 0x11);
                ReleaseState(actor);
                if (old > 0) {
                    ExploAddSmoke(8, actor, 0x1e, 1, 0);
                    SoundPlay(0x18, 0);
                    int kind = UnitClass(unit);
                    if (kind >= 9 && kind <= 17 && gApplyAttacker != 0 && gLocalActorKind == gApplyAttacker->owner)
                        VoicePlay(0x1a2, 10);
                }
            }
        }
        entry->health = health;
        if (slot == 10)
            ModeReport(modeCheckName, actor, points);
        if (!ActorIsDead(actor)) {
            int crew = unit->slot[0].health;
            if (crew != 0 && crew < 40)
                WorldActionNotify(actor->at20, 0x12);
        }
    }
}

// The original file has more lines between ApplyOne and ApplyDamage than this reconstruction;
// the blank lines below keep the ASSERT in ApplyDamage on its original line (447).


































































































































// Declarations used by ApplyVisit and ApplyDamage.
inline int Square(int v) { return v * v; }
extern "C" void TargetObjectInit(const char *, void *, int);
extern void BestLocSetData(void *, void *, void *);
extern int ResolveName(const char *, const char *);
extern int BestLocInitialize(void *, const char *, int);
extern int BestLocSetCandidate(void *, void *);
extern void BestLocVisit(void *, int (*)(void *));
extern "C" int LogicCopyPoint(void *, void *);
extern ActorRecord *ActorGetByIndex(int);
extern ActorRecord *ActorGetByUnit(int);
extern void ModeEvent(char *name, ActorRecord *actor, int a, int b);           // 00453d40
extern void WeaponRingHit(Weapon *weapon, ApplyPair *pair, int actor, int ring);      // 00460980
extern void StatsRecord(void *stats, ActorRecord *attacker, ActorRecord *actor);       // 00463973
extern int PointAngle(ApplyPoint *a, ApplyPoint *b);                        // 0047ef20
extern unsigned int ISqrt(unsigned int);                                       // 0047f8ea
extern const char fallbackName[];
extern int *actorIndexTable;
extern char g4dd0c0[];
extern WeaponInfo weaponInfoTable[];        // 0049cb38
extern int weaponSoundTable[];              // 004a32e8

int ApplyVisit(BestLocCandidate *candidate)
{
    int actorId = candidate->actor;
    if (actorId == 0)
        return 0;
    int ring = LogicCopyPoint(&gApplyImpactLoc, &candidate->pos);
    int damage = gApplyWeapon->ring[ring].damage;
    int count = gApplyWeapon->ring[ring].count;
    if (count < 1)
        count = 1;
    ApplyPair hit;
    hit.a = 0;
    hit.b = 0;
    WeaponRingHit(gApplyWeapon, &hit, actorId, ring);
    if (ring == 0) {
        ApplyPair direct;
        direct.a = ring;
        direct.b = ring;
        WeaponRingHit(gApplyWeapon, &direct, actorId, ring + 1);
        WeaponSound(&gApplyImpactPos, gApplySound, 2, 40, 100);
    }
    int index = actorIndexTable[actorId * 2];
    if (index <= 0)
        return 0;
    gApplyActor = ActorGetByIndex(index);
    if (ring > 0) {
        ApplyPoint pos;
        TargetObjectInit(fallbackName, &pos, candidate->actor);
        gApplyDistance = PointAngle(&pos, &gApplyImpactPos);
    }
    if (count >= 10)
        count = count * gApplyPercent / 100;
    else
        damage = damage * gApplyPercent / 100;
    gApplyBonus = 0;
    if (gLocalActorKind == gApplyAttacker->owner && gApplyFreeShots != 0) {
        gApplyBonus = 20000;
        gApplyFreeShots--;
    }
    gApplyShots = count;
    gApplyHits = 0;
    gApplyLoss = 0;
    gApplyDamage = 0;
    gApplyUnused = 0;
    for (int i = 0; i < count; i++)
        ApplyOne(damage);
    StatsRecord(g4dd0c0, gApplyAttacker, gApplyActor);
    ApplyFinish();
    Unit *unit = gApplyActor->unit;
    if (unit->slot[0].kind > 8) {
        for (int j = 1; j < 23; j++)
            unit->slot[j].health = unit->slot[0].health;
    } else {
        int sum = 0;
        for (int j = 0; j < 23; j++)
            sum += unit->slot[j].health * slotWeight[j];
        char average = sum / 100;
        if (unit->slot[1].health > average)
            unit->slot[1].health = average;
    }
    return 0;
}

void ApplyDamage(int unused, int impact, int attacker, Weapon *weapon)
{
    if (impact == 0 || attacker == 0)
        return;
    TargetObjectInit(fallbackName, &gApplyAttackerPos, attacker);
    BestLocSetData((void *)fallbackName, &gApplyAttackerLoc, (void *)attacker);
    TargetObjectInit(fallbackName, &gApplyImpactPos, impact);
    BestLocSetData((void *)fallbackName, &gApplyImpactLoc, (void *)impact);
    gApplyWeapon = weapon;
    gApplyInfo = &weaponInfoTable[weapon->type];
    gApplySound = weaponSoundTable[weapon->type];
    gApplyAttacker = ActorGetByUnit(attacker);
    gApplyActor = 0;
    gApplyLastActor = 0;
    gApplyDistance = PointAngle(&gApplyImpactPos, &gApplyAttackerPos);
    gApplyTotalHits = 0;
    gApplyTotalLoss = 0;
    gApplyTotalDamage = 0;
    if (gApplyAttacker == 0)
        return;
    gApplyPercent = Interpolate(weapon->falloffNear, 100, weapon->falloffFar, 0,
        (ISqrt(Square(gApplyAttackerPos.x - gApplyImpactPos.x) + Square(gApplyAttackerPos.y - gApplyImpactPos.y)) + 100) / 1000);
    if (gApplyPercent < 10)
        gApplyPercent = 10;
    int start = ResolveName(fallbackName, (const char *)&gApplyImpactLoc);
    BestLocBuffer loc;
    BestLocInitialize(&loc, fallbackName, weapon->rings - 1);
    BestLocSetCandidate(&loc, (void *)start);
    ASSERT(weapon->rings >= 1 && weapon->rings <= 4);
    BestLocVisit(&loc, (int (*)(void *))ApplyVisit);
    if (gApplyActor != 0 && gApplyTotalDamage != 0)
        ModeEvent(modeCheckName, gApplyActor, 0, 0x4b7);
}


