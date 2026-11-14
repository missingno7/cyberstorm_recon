// sim/exptbl.c, 00460800..00460c50. Labels and semantic field names are provisional.
#pragma pack(1)
struct WeaponInfo { char at00; unsigned char kind; char pad02[0x31]; int power; char pad37[8]; signed char factor; };
struct ActorRecord;
struct ExploActor;
struct ExploPoint { int x, y; };
struct ExploRecord { char beforeFrameStep[11]; unsigned char frameStep, delay; };
struct FxBurstDef { signed char kind, minCount, maxCount, minValue, maxValue, minDelay, maxDelay; unsigned char flags; };
struct FxHitDef { FxBurstDef *smoke, *blast; signed char smokeScale, blastScale, at0a, extra; };
#pragma pack()
static char exptblSourceFile[]=__FILE__;
FxBurstDef fxBurstDefinitions[]={
    {1,1,1,90,110,0,0,0},
    {2,1,1,90,110,0,0,0},
    {3,1,1,90,110,0,0,0},
    {4,1,1,90,110,0,0,0},
    {10,1,1,90,110,0,0,0},
    {5,1,1,90,110,0,0,0},
    {12,1,1,90,110,0,0,0},
    {4,3,3,90,110,0,5,0},
    {4,3,8,25,35,0,20,0},
    {11,10,15,90,100,0,20,0},
    {9,1,1,90,110,0,0,0},
    {15,1,1,90,110,0,0,0},
    {16,1,1,90,110,0,0,0},
    {17,1,1,90,110,0,0,0},
    {18,1,1,90,110,0,0,0},
    {19,1,1,90,110,0,0,0},
    {20,1,1,90,110,0,0,0},
    {11,1,1,90,110,0,0,0},
    {12,1,1,90,110,0,0,0},
    {23,0,2,90,110,0,8,0},
    {23,0,4,90,110,0,15,0},
    {13,1,1,90,110,0,0,1},
    {14,1,1,90,110,0,0,1},
    {6,1,1,90,110,0,6,0},
};
FxBurstDef fxExtraDefinitions[]={
    {0,0,0,0,0,0,0,0},
    {26,1,1,10,20,4,4,0},
    {26,1,1,20,30,3,3,0},
    {26,1,1,30,50,2,2,0},
    {26,1,1,50,80,1,1,0},
    {26,1,1,80,115,1,1,0},
    {24,1,1,30,45,2,2,0},
    {24,1,1,50,65,1,1,0},
    {24,1,1,70,85,1,1,0},
};
FxHitDef fxHitDefinitions[]={
    {0,0,100,100,0,0},
    {&fxBurstDefinitions[1],0,20,100,0,0},
    {&fxBurstDefinitions[1],0,30,100,0,0},
    {&fxBurstDefinitions[1],0,40,100,0,1},
    {&fxBurstDefinitions[16],0,25,100,0,0},
    {&fxBurstDefinitions[16],0,30,100,0,6},
    {&fxBurstDefinitions[16],&fxBurstDefinitions[19],50,30,0,0},
    {&fxBurstDefinitions[8],0,50,100,0,0},
    {&fxBurstDefinitions[8],0,70,100,0,0},
    {&fxBurstDefinitions[8],0,90,100,0,0},
    {0,0,100,100,0,0},
    {&fxBurstDefinitions[5],0,27,100,0,0},
    {&fxBurstDefinitions[5],0,40,100,0,0},
    {&fxBurstDefinitions[0],0,100,100,0,1},
    {&fxBurstDefinitions[5],0,50,100,0,0},
    {&fxBurstDefinitions[6],0,80,100,0,2},
    {&fxBurstDefinitions[6],0,70,100,0,0},
    {&fxBurstDefinitions[12],&fxBurstDefinitions[22],80,120,0,0},
    {&fxBurstDefinitions[0],&fxBurstDefinitions[20],60,20,0,0},
    {&fxBurstDefinitions[0],0,60,100,0,2},
    {&fxBurstDefinitions[1],0,50,100,0,1},
    {&fxBurstDefinitions[3],0,30,100,0,1},
    {&fxBurstDefinitions[3],0,45,100,0,2},
    {&fxBurstDefinitions[3],0,60,100,0,2},
    {&fxBurstDefinitions[0],0,40,100,0,6},
    {&fxBurstDefinitions[17],0,80,100,0,2},
    {&fxBurstDefinitions[17],0,100,100,0,2},
    {&fxBurstDefinitions[16],0,60,100,0,7},
    {0,0,100,100,0,1},
    {&fxBurstDefinitions[5],0,45,100,0,0},
    {&fxBurstDefinitions[0],&fxBurstDefinitions[20],60,25,0,0},
    {&fxBurstDefinitions[12],&fxBurstDefinitions[21],80,60,0,0},
    {&fxBurstDefinitions[8],0,50,100,0,0},
    {&fxBurstDefinitions[6],0,80,100,0,2},
    {&fxBurstDefinitions[4],0,20,100,0,1},
    {&fxBurstDefinitions[4],0,25,100,0,2},
    {&fxBurstDefinitions[4],0,30,100,0,3},
    {&fxBurstDefinitions[4],0,35,100,0,4},
    {&fxBurstDefinitions[2],0,80,100,0,1},
    {&fxBurstDefinitions[3],0,80,100,0,1},
    {&fxBurstDefinitions[0],&fxBurstDefinitions[19],70,30,0,0},
    {&fxBurstDefinitions[15],0,75,100,0,8},
    {&fxBurstDefinitions[7],0,30,100,0,0},
    {&fxBurstDefinitions[7],0,40,100,0,0},
    {&fxBurstDefinitions[7],0,50,100,0,2},
    {&fxBurstDefinitions[14],0,80,100,0,0},
    {&fxBurstDefinitions[7],0,45,100,0,0},
    {&fxBurstDefinitions[16],0,70,100,0,7},
    {&fxBurstDefinitions[17],0,80,100,0,1},
    {&fxBurstDefinitions[12],&fxBurstDefinitions[22],80,120,0,0},
    {&fxBurstDefinitions[8],0,50,100,0,0},
    {&fxBurstDefinitions[17],0,-126,100,0,0},
    {&fxBurstDefinitions[9],0,30,100,0,4},
    {&fxBurstDefinitions[10],0,50,100,0,2},
    {&fxBurstDefinitions[10],0,60,100,0,2},
    {&fxBurstDefinitions[10],0,65,100,0,2},
    {&fxBurstDefinitions[14],0,95,100,0,1},
    {&fxBurstDefinitions[7],0,45,100,0,2},
    {&fxBurstDefinitions[15],0,90,100,0,8},
    {0,0,100,100,0,0},
    {&fxBurstDefinitions[0],&fxBurstDefinitions[19],110,45,0,0},
    {&fxBurstDefinitions[12],&fxBurstDefinitions[22],80,120,0,0},
    {&fxBurstDefinitions[8],&fxBurstDefinitions[22],80,120,0,0},
    {&fxBurstDefinitions[11],0,75,30,0,2},
    {&fxBurstDefinitions[11],0,100,40,0,4},
    {&fxBurstDefinitions[1],&fxBurstDefinitions[23],80,60,0,0},
    {&fxBurstDefinitions[0],&fxBurstDefinitions[19],80,35,0,0},
    {&fxBurstDefinitions[18],0,120,100,0,3},
    {&fxBurstDefinitions[10],0,60,100,0,5},
    {&fxBurstDefinitions[13],&fxBurstDefinitions[19],120,80,5,0},
    {0,0,100,100,0,0},
    {0,0,100,100,0,0},
    {0,0,100,100,0,0},
    {0,0,100,100,0,0},
    {0,0,100,100,0,0},
    {0,0,100,100,0,0},
};

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
    if (((unsigned)effect->flags & 1) && repeated != 0)
        return;
    int count = RandomRange(&gFxSeed, effect->minCount, effect->maxCount);
    int spread = 700;
    if (actor == 0) {
        count = 1;
        spread = 0;
    }
    if (effect->minCount == 1 && effect->maxCount == 1 && repeated == 0)
        spread = 100;
    if ((unsigned)effect->flags & 1)
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


