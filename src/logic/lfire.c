static char lfireSourceFile[]=__FILE__;
#include <string.h>
typedef unsigned char u8;
struct Point { int x, y; };
#pragma pack(push,1)
struct UnitInfo { u8 reserved[0x12]; int health; int Alive() { return health > 0; } };
struct UnitData { u8 reserved[0xeb]; u8 strength[32]; };
struct Unit {
    u8 reserved0[8]; int owner; u8 reservedC[12]; UnitData *data; UnitInfo *info;
    u8 reserved20[0x22]; int id; u8 reserved46[0x42]; u8 active, inactive;
    int Id() { return id; }
};
#pragma pack(pop)
struct LogicActor {
    Unit *unit; LogicActor *target; int reserved8; unsigned flags; int owner;
    int reserved14[7]; int count30, value34, value38, value3c, value40;
    int reserved44[9]; int rangeLow, rangeHigh; int reserved70[7]; int lastValue, lastTarget;
    int Available() { return unit && unit->active && unit->info->Alive() && !unit->inactive; }
    int Friendly() { return (flags & 1) == 0; }
};
class LogicActorList { public: LogicActor *First(); LogicActor *Next(LogicActor *); };
extern LogicActorList logicActorList;
extern const char *ActorName(LogicActor *);
extern int ActorValue(LogicActor *), ActorArrayGet(LogicActor *, int);
extern "C" int LogicCopyPoint(const char *, const char *);
extern int gLocalActorKind, g4d359c; static inline int WorldRadius() { return g4d359c; }
extern int gTargetOrigin, gCameraEnabled;
extern u8 *simActorVisibility; static inline int ActorHidden(LogicActor *a) { return !simActorVisibility[a->unit->Id()]; }
extern char fallbackName[], modeCheckName[], gFogBufferSource[], gView[];
extern int gRedrawFlags;
extern char weaponReport[], gTargetMode[];
struct FogData { int words[8]; };
extern FogData gFogDefaultData;
extern void BestLocSetData(void *, void *, int);
extern Unit *ActorGetByUnit(int);
extern int FogSetValue(void *, int, int), DisplayZoomArea(void *, void *, int, int, int);
extern void DisplayConfigure(void *, void *, int), SimSelectTarget(void *, int);
extern int f_0045d830(void *, void *, int, int);
extern void SimRandomFire(void *, int, int);

static inline int Product(int a, int b) { return a * b; }
static inline int ClampStrengthDistance(int n) { if (n <= 0) return 0; if (n >= 31) return 31; return n; }

int ActorStrengthAt(LogicActor *actor, const char *point)
{
    if (actor->unit) {
        int distance = LogicCopyPoint(point, ActorName(actor));
        distance = ClampStrengthDistance(distance);
        return actor->unit->data->strength[distance];
    }
    return 0;
}

int ActorNetStrengthAt(const char *point)
{
    int result = 0;
    LogicActor *actor = logicActorList.First();
    while (actor) {
        if (actor->Available()) {
            int distance = LogicCopyPoint(point, ActorName(actor));
            distance = ClampStrengthDistance(distance);
            int strength = actor->unit->data->strength[distance];
            if (actor->Friendly()) result += strength;
            else result -= strength;
        }
        actor = logicActorList.Next(actor);
    }

    return result;
}

LogicActor *ActorChooseNearest(LogicActor *actor, LogicActor *excluded)
{
    LogicActor *best = 0;
    int bestDistance;
    LogicActor *other = logicActorList.First();
    while (other) {
        if (other->Available() && actor->owner != other->owner && other != excluded) {
            int distance = LogicCopyPoint(ActorName(actor), ActorName(other));
            if (!best || distance < bestDistance) { best = other; bestDistance = distance; }
        }
        other = logicActorList.Next(other);
    }
    actor->target = best;
    return best;
}

LogicActor *ActorChooseByMode(LogicActor *actor, int origin, int radius)
{
    if (!origin) origin = gTargetOrigin;
    if (!radius) radius = WorldRadius() * 2;
    Point point;
    BestLocSetData(fallbackName, &point, origin);
    int bestValue;
    LogicActor *best = 0;
    LogicActor *other = logicActorList.First();
    while (other) {
        if (other->Available() && actor->owner != other->owner) {
            int hidden = 0;
            if (actor->owner == gLocalActorKind) hidden = !((char *)simActorVisibility)[other->unit->Id()];
            int distance = LogicCopyPoint((const char *)&point, ActorName(other));
            if (!hidden && distance <= radius) {
                int value;
                int divisor;
                switch (ActorArrayGet(actor, 7)) {
                case 5: value = ActorNetStrengthAt(ActorName(other)); break;
                case 8:
                    value = other->value34 * other->value34;
                    divisor = Product(other->value38, other->count30);
                    if (divisor > 1) value /= divisor;
                    break;
                case 9:
                    value = other->value34 * other->value34;
                    divisor = Product(other->value38, other->count30);
                    if (divisor > 1) value /= divisor;
                    divisor = LogicCopyPoint(ActorName(actor), ActorName(other));
                    if (divisor > 1) value /= divisor;
                    break;
                case 10: value = Product(other->value40, other->value3c); break;
                default: value = -LogicCopyPoint(ActorName(actor), ActorName(other)); break;
                }
                if (!best || value > bestValue) { best = other; bestValue = value; }
            }
        }
        other = logicActorList.Next(other);
    }
    actor->target = best;
    return best;
}


int ActorUpdateView(LogicActor *actor, int target)
{
    if (!gCameraEnabled) return 0;
    int changed = 0;
    if (ActorValue(actor) != actor->lastValue || (target && target != actor->lastTarget)) {
        actor->lastValue = ActorValue(actor);
        actor->lastTarget = target;
        if (target && ActorHidden(actor)) {
            Unit *unit = ActorGetByUnit(target);
            if (unit && unit->owner == gLocalActorKind) {
                gRedrawFlags |= FogSetValue(gFogBufferSource, actor->lastValue, 99);
                changed = 1;
            }
        }
        if (gCameraEnabled && (target || !ActorHidden(actor))) {
            FogData data; data.words[0] = gFogDefaultData.words[0]; data.words[1] = gFogDefaultData.words[1]; data.words[2] = gFogDefaultData.words[2]; data.words[3] = gFogDefaultData.words[3]; data.words[4] = gFogDefaultData.words[4]; data.words[5] = gFogDefaultData.words[5]; data.words[6] = gFogDefaultData.words[6]; data.words[7] = gFogDefaultData.words[7];
            if (DisplayZoomArea(gView, &data, actor->lastValue, target, 1) && memcmp(&data, &gFogDefaultData, 32)) {
                gFogDefaultData = data;
                DisplayConfigure(gView, &data, 0);
                gRedrawFlags |= 1;
                changed = 1;
            }
        }
    }
    return changed;
}


int ActorTryTargetAction(LogicActor *actor, int attack)
{
    int action; LogicActor *target; int result = 1;
    if ((ActorArrayGet(actor, 7) != 7 || !attack) && actor->target) {

        if (attack) {
            int distance = LogicCopyPoint(ActorName(actor), ActorName(actor->target));
            if (distance >= actor->rangeLow && distance <= actor->rangeHigh) action = 25;
            else action = 35;
        } else action = 15;
        target = actor->target;
        SimSelectTarget(modeCheckName, ActorValue(target));
        int choice = f_0045d830(weaponReport, gTargetMode, action, 0);
        if (choice < 0) {
            LogicActor *saved = actor->target;
            target = ActorChooseNearest(actor, saved);
            actor->target = saved;
            if (target) {
                SimSelectTarget(modeCheckName, ActorValue(target));
                choice = f_0045d830(weaponReport, gTargetMode, action + 10, 0);
            }
        }
        if (choice >= 0) {
            if (!ActorUpdateView(actor, ActorValue(target))) SimRandomFire(modeCheckName, choice, 0);
            result = 0;
        }
    }
    return result;
}





