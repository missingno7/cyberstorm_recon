// logic\lgen.c - mission force generator: picks unit configurations and places actors. Names are working labels.
#include <windows.h>
extern "C" void AssertFail(const char *file, int line, const char *message);
#define ASSERT(e) if (!(e)) AssertFail(__FILE__, __LINE__, 0)
extern "C" unsigned int next_link_seed(unsigned int *seed);
extern unsigned int MissionRandomTable;                             // 0051445c random seed
int RandomRange(unsigned int *seed, int low, int high);
extern "C" char *BriefFormat(const char *format, ...);              // 0047d420 (vsprintf into a static buffer)
struct ScriptPoint { long x, y; };
#pragma pack(1)
struct UnitInfo {                       // unit configuration record (0x2f bytes)
    char *name;                         // + 0x00 (0 ends a table)
    char pad04[4];
    unsigned char side;                 // + 0x08
    char pad09;
    unsigned char weight;               // + 0x0a
    char pad0b;
    long cost;                          // + 0x0c
    unsigned char level;                // + 0x10
    unsigned char sight, speed;         // + 0x11, + 0x12
    unsigned char skill;                // + 0x13
    char pad14[4];
    unsigned char chassis;              // + 0x18 herc chassis + 8 / vehicle animation
    char pad19[0x16];
};
inline long GenUnitKind(unsigned char *unit) { return unit[0x12]; }
struct GenRecord { char pad00[8]; long owner; char pad0c[0xc]; unsigned char *unit; char pad1c[0x1e]; ScriptPoint at; char pad42[4]; long angle; };
struct GenActor {                       // logic actor (logic\lactor.c)
    GenRecord *rec;                     // + 0x00
    char pad04[8];
    unsigned char flags;                // + 0x0c
    char pad0d[3];
    long owner;                         // + 0x10
    long id;                            // + 0x14
    char pad18[4];
    long type, index, delay;            // + 0x1c, + 0x20, + 0x24
    char pad28[0x40];
    long sight, speed;                  // + 0x68, + 0x6c
    char where[8];                      // + 0x70
    long angle;                         // + 0x78
    long squad;                         // + 0x7c
    long follower;                      // + 0x80
    long weight;                        // + 0x84
};
#pragma pack()
struct LogicSystem { char pad00[0xc]; long mode; char pad10[0x10]; long campaign; char pad24[0x14]; long level; };
extern LogicSystem g4dd0c0;                                         // logic\lsys.c system object
#define gLogic g4dd0c0
class Script { public: char pad00[0x30]; long time; long Now(){return time;} };
extern Script gScript;                                              // logic\sinterp.c
struct MenuNetwork { char pad00[4]; long active; long IsActive(){return active;} void SendStart(GenRecord *rec, int drop); };
extern MenuNetwork gMenuNetwork;
extern UnitInfo gHercInfo[], gVehicleInfo[], unitTemplates[], gBuildingInfo[], gDropInfo[];
extern long g4c928c;                                                // highest herc chassis available
extern int modeSwitch, playerCount, gLocalActorKind;
extern char fallbackName[];                                         // the map
extern "C" int ResolveName(char *map, ScriptPoint *at);
int LogicStoreActorLink(void *logic, void *cell);                     // 00462fff
struct ActorPoint; struct LogicActor;
struct LogicActorList { LogicActor *CreateActor(int,ActorPoint*,int,const signed char*); };
inline GenActor *GenCreateActor(void *list,int type,ScriptPoint *at,int index,int side) {
    return (GenActor*)((LogicActorList*)list)->CreateActor(type,(ActorPoint*)at,index,(const signed char*)side);
}  // 00464218
GenRecord *ActorCreate(long owner, UnitInfo *info, void *where, int flag);
extern "C" void ActorArraySet(GenActor *actor, int index, long value);
extern "C" long ActorArrayGet(GenActor *actor, int index);
int PopFindItem(void *unit, int part, int index = -1);             // 00438374 unit part record
inline int GenFindPart(void *unit,int part) { return PopFindItem(unit,part); }
struct ActorUnit; int HercDamageAtRange(ActorUnit *unit, int kind);                              // 0043a670
extern ScriptPoint gHexDirection[6];
extern long *actorIndexTable;
struct AnimMeta { long radius; char pad[0x38]; };
extern AnimMeta animationMetaAt0cbc[];
inline void GenSetSquad(GenActor *actor,int side) { actor->squad=side; }
inline void GenSetWeight(GenActor *actor,long weight) { actor->weight=weight; }
inline int GenChassis(int index){return gHercInfo[index].chassis;} inline long GenVehicleCost(int index) { return gVehicleInfo[index].cost; }
inline long GenVehicleWeight(int index) { return gVehicleInfo[index].weight; }
inline long GenInfoCost(UnitInfo *info) {return info->cost;}
inline long Min(long a, long b) { return a < b ? a : b; }
inline long Max(long a, long b) { return a > b ? a : b; }
struct GenAnchor { long kind; ScriptPoint at; long radius; GenAnchor(); ~GenAnchor() {} };
inline GenAnchor::GenAnchor() {}

static signed char skillTable[18][3] = {
    {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
    {2, 2, 1}, {2, 2, 1}, {2, 2, 1}, {2, 2, 1},
    {3, 3, 1}, {3, 3, 1}, {3, 3, 1}, {3, 3, 1},
    {4, 3, 1}, {0, 0, 0}, {4, 4, 2}, {5, 5, 2}
};
static signed char squadDirection[10] = {0, 0, 1, 3, 5, 0, 4, 2, 0, 0};
static long dropList[7];
static GenAnchor anchors[30];

int LogicGenSkill(void *list, int type)
{
    int level = gLogic.level;
    if (gLogic.campaign == 0x192) level = 15;
    if (gLogic.campaign == 0x19a) level = 16;
    if (gLogic.campaign == 0x1a2) level = 17;
    if (type >= 14 && type <= 16) return skillTable[level][1];
    if (type >= 10 && type <= 16) return skillTable[level][2];
    if (type >= 17 && type <= 24) return 0;
    if (type >= 25 && type <= 29) return 0;
    if (type == 30) return 0;
    return skillTable[level][0];
}

int LogicGenFindType(void *list, int type, long least, long most = 0, int side = 0)
{
    // each table has its own selection.
    if (type >= 10 && type <= 16) {
        int best; int level = gLogic.level;
        int i;
        long bestCost = least;
        best = -1;
        for (i = 0; gVehicleInfo[i].name; i++) {
            int skill = gVehicleInfo[i].skill;
            if ((!most || !skill || skill >= LogicGenSkill(list, type))
                && gVehicleInfo[i].side == side && gVehicleInfo[i].level <= level) {
                long cost = gVehicleInfo[i].cost;
                if ((most && cost <= most && cost > bestCost) || (!most && (best == -1 || cost < bestCost))) {
                    best = i;
                    bestCost = cost;
                }
            }
        }
        return best;
    } else if (type >= 17 && type <= 24)
        return type - 17;
    else if (type >= 25 && type <= 29)
        return type - 25;
    else if (type == 30)
        return 0;
    else {
        int best; int chassis = g4c928c;
        int level, top, i;
        long bestCost;
        if (chassis <= 1) chassis = 1;
        else if (chassis >= 9) chassis = 9;
        top = chassis + 1;
        if (gLogic.mode == 2 || gLogic.mode == 3) top++;
        level = gLogic.level;
        if (type != -1) level = 99;
        if (gLogic.campaign == 0x19a) {
            top = 8;
            level += 3;
        }
        if (gLogic.campaign == 0x1a2) {
            top += 2;
            level += 10;
        }
        bestCost = least;
        best = -1;
        for (i = 0; gHercInfo[i].name; i++) {
            int kind = gHercInfo[i].chassis - 8;
            int skill;
            if (kind < 1 || kind > 9) continue;
            if (type == -1 && kind <= top && kind != 1) kind = type;
            skill = gHercInfo[i].skill;
            if ((!most || !skill || LogicGenSkill(list, 1) <= skill)
                && kind == type && gHercInfo[i].level <= level) {
                long cost = gHercInfo[i].cost;
                if ((most && cost <= most && cost > bestCost) || (!most && (best == -1 || cost < bestCost))) {
                    best = i;
                    bestCost = cost;
                }
            }
        }
        if (type == 1) {
            if (gLogic.campaign == 0x19a) best++;
            if (gLogic.campaign == 0x1a2) best += 2;
        }
        return best;
    }
    // table branches return directly.
}

long LogicGenVehicle(void *list, int type, long budget, ScriptPoint *at);

















































































long LogicMapPointUpdate(void *list, int type, long most, ScriptPoint *at)
{
    int index, side;
    GenActor *actor;

    if (type <= 0 || type >= 32) AssertFail(__FILE__, __LINE__, "unknown actor type");
    if (type >= 10 && type <= 16) return LogicGenVehicle(list, type, most, at);
    if (type == 30 || type == 31) {








        if (!LogicStoreActorLink(&gLogic, (void *)ResolveName(fallbackName, at))) AssertFail(__FILE__, __LINE__, "Too many drop locations");
        if (type == 31) return 0;
    }
    index = LogicGenFindType(list, type, 0, most);
    if (index == -1) index = LogicGenFindType(list, type, 0);
    side = 0;
    if (type == 30 && modeSwitch) {
        side = most;
        if (most < 1 || most > playerCount) side = gLocalActorKind;
    }
    actor = GenCreateActor(list, type, at, index, side);
    return actor ? actor->id : 0;
}

long *LogicMapViewUpdate(void *list, long budget, ScriptPoint *at)
{
    long spent[6], chosen[6];
    int i, n;

    if (gLogic.campaign == 0x19a || gLogic.campaign == 0x1a2) {
        if (budget <= 200) budget = 200;
        else if (budget >= 2500) budget = 2500;
    } else {
        if (budget <= 200) budget = 200;
        else if (budget >= 1200) budget = 1200;
    }
    for (i = 0; i < 6; i++) {
        chosen[i] = -1;
        spent[i] = 0;
    }
    for (i = 0; i < 20 && budget > 0; i++) {
        unsigned int slot = next_link_seed(&MissionRandomTable) % 6;
        long have = 0;
        long amount = Min(RandomRange(&MissionRandomTable, 25, 300), budget);
        budget -= amount;
        if (chosen[slot] != -1) have = gHercInfo[chosen[slot]].cost;
        if (have < amount) {
            int index = LogicGenFindType(list, -1, have, amount, 0);
            if (index != -1) {
                chosen[slot] = index;
                spent[slot] = amount;
            }
        }
    }
    n = 0;
    for (i = 0; ; i++) { int next=n+1; if (i>=6) break;
        if (chosen[i] != -1) {
            GenActor *actor = GenCreateActor(list, GenChassis(chosen[i]) - 8, at, chosen[i], 0);
            if (actor) { dropList[n] = actor->id; n=next; }
        }
    }
    dropList[n] = -1;
    return dropList;
}

long LogicGenVehicleRadius(int index)
{
    return animationMetaAt0cbc[gVehicleInfo[index].chassis].radius;
}

GenActor *LogicGenSpawn(void *list, int type, long *budget, ScriptPoint *at, int side, GenActor *leader, int index)
{
    GenActor *actor;
    if (index == -1) {
        index = LogicGenFindType(list, type, 0, *budget, side);
        if (index == -1) return 0;
    }
    actor = GenCreateActor(list, type, at, index, 0);
    if (!actor) return 0; {
        GenSetSquad(actor,side);
        GenSetWeight(actor,gVehicleInfo[index].weight);
        *budget -= gVehicleInfo[index].cost;
        *budget = Max(*budget, 0);
        if (leader) leader->follower = actor->id;
    }
    return actor;
}





























































































































































long LogicGenVehicle(void *list, int type, long budget, ScriptPoint *at)
{
    int count, most = 7, extra = 0, side, index, i;
    long bonus = 0, id, radius;
    GenActor *leader;

    if (gLogic.campaign == 0x19a) {
        most = 8;
        extra = 12;
        bonus = budget / 3;
    } else if (gLogic.campaign == 0x1a2) {
        most = 9;
        extra = 15;
        bonus = budget * 60 / 100;
    } else
        budget = budget <= 0 ? 0 : budget >= 1500 ? 1500 : budget;
    side = 0;
    if (type >= 14) side = type - 12;
    index = LogicGenFindType(list, type, 0, budget, side);
    if (index == -1) index = LogicGenFindType(list, type, 0, 0, side);
    if (index == -1) AssertFail(__FILE__, __LINE__, BriefFormat("Config Not Found: type=%d", side));
    leader = LogicGenSpawn(list, type, &budget, at, side, 0, index);
    if (!leader) return 0;
    id = leader->id;
    radius = LogicGenVehicleRadius(index);
    if (type >= 14) return id;
    count = (budget + 99) / 200 + 1;
    if (count <= 1) most = 1;
    else if (most > count) most = count;
    count = most;
    anchors[0].kind = 0;
    anchors[0].at = *at;
    anchors[0].radius = radius;
    for (i = 1; i < count; i++) {
        int roll, kind, found;
        anchors[i].kind = -1;
        roll = budget / 30 + next_link_seed(&MissionRandomTable) % 100;
        if (roll < 30)
            kind = 5;
        else if (roll < 65)
            kind = 7;
        else if (roll < 90)
            kind = 6;
        else if (roll < 115)
            kind = 8;
        else
            kind = 1;
        found = LogicGenFindType(list, type, 0, budget, kind);
        if (found != -1) {
            long size = LogicGenVehicleRadius(found);
            long steps = size + radius;
            ScriptPoint p;
            int j;
            p.x = at->x;
            p.y = at->y;
            for (j = 0; j < steps; j++) {
                p.x += gHexDirection[squadDirection[i + 1]].x;
                p.y += gHexDirection[squadDirection[i + 1]].y;
            }
            if (ResolveName(fallbackName, &p)) {
                GenActor *actor = LogicGenSpawn(list, type, &budget, &p, kind, leader, found);
                if (actor) {
                    leader = actor;
                    actor->angle = (6 - squadDirection[i + 1]) * 0x2aaaaabc;
                    anchors[i].kind = kind;
                    anchors[i].at = p;
                    anchors[i].radius = size;
                }
            }
        }
    }
    if (extra) count = extra;
    budget += bonus;
    for (i = 1; i < count; i++) {
        int kind = 2;
        unsigned int roll = next_link_seed(&MissionRandomTable) % 100;
        if (roll >= 50) {
            kind = 3;
            if (roll >= 75) kind = 4;
        }
        if (gLogic.campaign == 0x1a2) next_link_seed(&MissionRandomTable);
        if (anchors[i].kind == -1) {
            ScriptPoint p;
            int j;
            p.x = at->x;
            p.y = at->y;
            for (j = 0; j < radius; j++) {
                p.x += gHexDirection[squadDirection[i + 1]].x;
                p.y += gHexDirection[squadDirection[i + 1]].y;
            }
            if (ResolveName(fallbackName, &p) && next_link_seed(&MissionRandomTable) % 100 < 75) {
                GenActor *actor = LogicGenSpawn(list, type, &budget, &p, kind, leader, -1);
                if (actor) {
                    leader = actor;
                    actor->angle = (6 - squadDirection[i + 2]) * 0x2aaaaabc;
                }
            }
        } else {
            int direction = squadDirection[i + 1];
            long size = anchors[i].radius;
            int turn;
            for (turn = -1; turn <= 1; turn++) {
                int d = direction + turn;
                ScriptPoint p;
                int j, cell;
                if (d < 0) d += 6;
                else if (d >= 6) d -= 6;
                p.x = anchors[i].at.x;
                p.y = anchors[i].at.y;
                for (j = 0; j <= size; j++) {
                    p.x += gHexDirection[d].x;
                    p.y += gHexDirection[d].y;
                }
                cell = ResolveName(fallbackName, &p);
                if (cell && !actorIndexTable[cell * 2] && next_link_seed(&MissionRandomTable) % 100 < 75) {
                    GenActor *actor = LogicGenSpawn(list, type, &budget, &p, kind, leader, -1);
                    if (actor) {
                        leader = actor;
                        actor->angle = (6 - d) * 0x2aaaaabc;
                    }
                }
            }
        }
    }
    return id;
}





























































































void LogicGenSetup(GenActor *actor)
{
    long armor, power, speed;
    ASSERT(actor->rec && actor->rec->unit);
    armor = *(long *)((char *)GenFindPart(actor->rec->unit, 5) + 0x1b);
    power = HercDamageAtRange((ActorUnit*)actor->rec->unit, actor->sight);
    speed = *(long *)((char *)GenFindPart(actor->rec->unit, 1) + 0x3b);
    ActorArraySet(actor, 2, armor);
    ActorArraySet(actor, 3, power);
    ActorArraySet(actor, 4, speed);
    ActorArraySet(actor, 10, actor->rec->owner);
}












void LogicGenSpawnActor(GenActor *actor)
{
    UnitInfo *info;
    if (actor->rec) return;
    if (gScript.Now() < actor->delay + ActorArrayGet(actor, 1)) return;
    if (modeSwitch && gScript.Now() > 2) return;
    if (actor->type >= 1 && actor->type <= 9)
        info = &gHercInfo[actor->index];
    else if (actor->type >= 10 && actor->type <= 16)
        info = &gVehicleInfo[actor->index];
    else if (actor->type >= 17 && actor->type <= 24)
        info = &unitTemplates[actor->index];
    else if (actor->type >= 25 && actor->type <= 29)
        info = &gBuildingInfo[actor->index];
    else if (actor->type == 30)
        info = &gDropInfo[actor->index];
    else
        AssertFail(__FILE__, __LINE__, BriefFormat("unknown type = %d", actor->type));
    actor->rec = ActorCreate(actor->owner, info, actor->where, 1);

    ASSERT(actor->rec && actor->rec->owner == actor->owner);
    actor->rec->angle = actor->angle;
    if (modeSwitch && gMenuNetwork.IsActive()) gMenuNetwork.SendStart(actor->rec, actor->type == 30);
    actor->sight = info->sight;
    actor->speed = info->speed;
    ActorArraySet(actor, 6, info->cost);
    LogicGenSetup(actor);
}

struct GenUnitInfo { char pad[0x2d5]; };
extern char g494aa3[]; inline long GenUnitCost(int kind) { return *(long *)(g494aa3+kind*0x2d5); }









void LogicDeleteActorFromList(void *list, GenRecord *rec)
{
    int kind, type;
    GenActor *actor;
    ASSERT(rec && rec->unit);
    kind = GenUnitKind(rec->unit);
    if (kind == 31)
        type = 30;
    else {






        ASSERT(kind >= 1 && kind <= 8);
        type = kind + 16;
    }
    actor = (GenActor*)((LogicActorList*)list)->CreateActor(type,(ActorPoint*)&rec->at,0,(const signed char*)rec->owner);


    ASSERT(actor);
    actor->flags |= 1;
    actor->rec = rec;
    actor->sight = 5;
    actor->speed = 5;
    ActorArraySet(actor, 6, GenUnitCost(kind));
    ActorArraySet(actor, 7, 0);
    LogicGenSetup(actor);
}

