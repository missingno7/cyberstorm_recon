// Unknown effects object, 00460800..00460c50. Labels and semantic field names are provisional.
#pragma pack(1)
struct WeaponInfo { char at00; unsigned char kind; char pad02[0x31]; int power; char pad37[8]; signed char factor; };
struct ActorRecord;
struct ExploActor;
struct ExploPoint { int x, y; };
struct ExploRecord { char beforeFrameStep[11]; unsigned char frameStep, delay; };
struct FxBurstDef { signed char kind, minCount, maxCount, minValue, maxValue, minDelay, maxDelay; unsigned char noRepeat : 1; unsigned char spareFlags : 7; };
struct FxHitDef { FxBurstDef *smoke, *blast; signed char smokeScale, blastScale, at0a, extra; };
#pragma pack()
extern FxHitDef fxHitDefinitions[]; // 004a35d8, 12-byte records
extern FxBurstDef fxExtraDefinitions[]; // 004a3590, 8-byte records
extern unsigned int gFxSeed;
extern int simLimit3, simLimit4, fxAlternateLimit;
extern const char fallbackName[];
extern "C" int RandomRange(unsigned int *, int, int);
extern "C" unsigned int next_link_seed(unsigned int *);
extern "C" void TargetObjectInit(const char *, void *, int);
void FxStirSeed(unsigned int *);
unsigned long TimeNow();
ExploRecord *ExploAddAt(int, ExploPoint *, int);
ExploRecord *ExploAddSmoke(int, ExploActor *, int, int, int);

void FxDamage(WeaponInfo *weapon, ActorRecord *actor, int shots, int damage)
{
    FxHitDef *definition = &fxHitDefinitions[weapon->kind];
    if (definition->smoke == 0 || !definition->smokeScale)
        return;
    FxBurstDef *effect = definition->smoke;
    if (!effect->kind)
        return;
    int count = RandomRange(&gFxSeed, effect->minCount, effect->maxCount);
    if (count < 0)
        count = shots;
    unsigned int previousSeed = gFxSeed; gFxSeed = TimeNow() + previousSeed;
    for (int i = 0; i < count; ++i) {
        int power = weapon->factor * weapon->power;
        FxStirSeed(&gFxSeed);
        int value = RandomRange(&gFxSeed, effect->minValue, effect->maxValue) * definition->smokeScale / 100;
        int delay = RandomRange(&gFxSeed, effect->minDelay, effect->maxDelay);
        next_link_seed(&gFxSeed);
        if (power != 0 && power > damage)
            value = ((damage * value) / power + value) / 2;
        ExploRecord *result = ExploAddSmoke(effect->kind, (ExploActor *)actor, value, 1, count);
        if (result != 0)
            result->delay = delay;
    }
}

void FxBlast(WeaponInfo *weapon, ExploPoint *position, int actor, int repeated)
{
    FxHitDef *definition = &fxHitDefinitions[weapon->kind];
    if (definition->blast == 0 || !definition->blastScale)
        return;
    FxBurstDef *effect = definition->blast;
    if ((effect->noRepeat) && repeated != 0)
        return;
    int count = RandomRange(&gFxSeed, effect->minCount, effect->maxCount);
    int spread = 700;
    if (actor == 0) {
        count = 1;
        spread = 0;
    }
    if (effect->minCount == 1 && effect->maxCount == 1 && repeated == 0)
        spread = 100;
    if (effect->noRepeat)
        spread = 0;
    unsigned int previousSeed = gFxSeed; gFxSeed = TimeNow() + previousSeed;
    int limit = fxAlternateLimit ? simLimit3 : simLimit4;
    if (effect->maxCount > 1) {
        count = (limit * count + 5) / 10;
        if (count < 1 && effect->minCount > 0)
            count = 1;
    }
    for (int i = 0; i < count; ++i) {
        ExploPoint point;
        point.x = position->x;
        point.y = position->y;
        if (actor != 0)
            TargetObjectInit(fallbackName, &point, actor);
        FxStirSeed(&gFxSeed);
        if (spread != 0) {
            point.x -= next_link_seed(&gFxSeed) % spread + spread / 2;
            point.y -= next_link_seed(&gFxSeed) % spread + spread / 2;
        }
        int value = RandomRange(&gFxSeed, effect->minValue, effect->maxValue) * definition->blastScale / 100;
        value *= 20;
        int delay = RandomRange(&gFxSeed, effect->minDelay, effect->maxDelay);
        next_link_seed(&gFxSeed);
        ExploRecord *result = ExploAddAt(effect->kind, &point, value);
        if (result != 0)
            result->delay = delay;
    }
}

void FxExtra(WeaponInfo *weapon, ExploPoint *position)
{
    FxHitDef *definition = &fxHitDefinitions[weapon->kind]; int index = definition->extra;
    if (index != 0) {
        FxBurstDef *effect = &fxExtraDefinitions[index];
        FxStirSeed(&gFxSeed);
        int value = RandomRange(&gFxSeed, effect->minValue, effect->maxValue);
        ExploRecord *result = ExploAddAt(effect->kind, position, value * 20);
        if (result != 0)
            result->frameStep = effect->minDelay;
    }
}


