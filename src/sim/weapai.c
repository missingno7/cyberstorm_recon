// sim\weapai.c: weapon scoring and selection. Field names are provisional.
#pragma pack(1)
struct Point { int x,y; Point() {} Point(const Point &p) { x=p.x;y=p.y; } };
#include "actor.h"

struct ActorItem { char pad00[0x279]; int defense; };
struct Weapon { char pad00[0x13]; int ammo; char pad17[8]; signed char armorClass, threshold, range, nearRange, farRange, falloffNear, falloffFar, power, limit, skill; char pad29[2]; int energy; int armorDamage, directDamage, penetrationDamage; signed char overflow, rings, at3d, ringDamage, ringCount; char pad40[6]; };
struct WeaponScore { int state, accuracy, power, chance, rangeFactor, maximum, expected, ammoFactor; Weapon weapon; char pad66[8]; };
struct TargetRecord { int sourceId; char pad04[0x29]; unsigned char sourceHeight; char pad2e[0xd]; signed char sourceSkill; char pad3c[8]; ActorRecord *sourceActor; int sourceIndex, sourceDirection, sourceBand; int targetId; char pad58[0x29]; unsigned char targetHeight; char pad82[0xc]; signed char targetSkill; char pad8f[9]; ActorRecord *targetActor; int targetIndex, targetDirection, targetBand; int distance, elevation, modifier; };
struct FogObject { char bytes[0x1f]; int accuracy, elevation; };
struct Context { int id; TargetRecord target; FogObject fog; WeaponScore score[8]; };
struct PlayerSlot { int at00, at04, accuracy; };
struct AreaCandidate { int kind; Point at; int actor; };
struct Area { int words[10]; };
struct TargetTriple { char at00; unsigned char height; char pad02[10]; };
#pragma pack()
extern "C" void AssertFail(const char *,int,const char *);
extern FogObject *UnitFogObject(ActorUnit *, int, void *);
extern int HercEnergy(ActorUnit *);
extern int UnitCapability(void *,int);
extern "C" int f_00439d02(ActorUnit *);
extern PlayerSlot *GetModePlayer(void *,int);
extern int HercDefense(ActorUnit *);
extern int AnimActionGetMetadata(void *);
extern ActorItem *PopFindItem(ActorUnit *,int,int = -1);
extern int Interpolate(int,int,int,int,int);
extern long GridClamp(unsigned long);
extern void UnitDamageSplit(ActorUnit *,int,int,Weapon *,int &,int &);
extern "C" void TargetObjectInit(const char *,void *,int);
extern void BestLocSetData(void *,void *,void *);
extern int ResolveName(const char *,const char *);
extern int PointAngle(Point *,Point *);
extern int BestLocInitialize(void *,const char *,int);
extern int BestLocSetCandidate(void *,void *);
extern void AreaEnumerate(int *,void *,unsigned int,unsigned int);
extern "C" int TargetActorById(int);
extern "C" unsigned int next_link_seed(unsigned int *);
extern char fallbackName[], modeCheckName[];
extern int gRemoteActorKind, modeSwitch, leaderPlayer, gMissionKind, gMission, weaponRangeBase, weaponHalfAccuracy;
extern unsigned char *simVisibleTypes;
extern signed char WeaponEfficiency[], weaponMissionRanges[], weaponClassRanges[], weaponVisibleRanges[];
extern int weaponActionDefense[];
extern unsigned char weaponTerrainWeights[];
extern Point pathDirections[];
extern TargetTriple *targetDefinitionTable;
extern unsigned int MissionRandomTable;
int weaponTerrainPenalty, weaponAreaOwner, weaponRingDamage, weaponAreaScore;
inline int IsRestrictedMode(AnimAction *a) { return a->state!=1 && a->state!=7 && a->state!=14 && a->state!=17 && a->state!=18; }
inline int IsTypeHidden(int type) { return !simVisibleTypes[type]; }
inline int HealthPercent(ActorUnit *u) { return u->slots[0].condition; }
static inline int Clamp(int value,int low,int high) { if(value<=low) return low; if(value>=high) return high; return value; }
int WeaponEnabled(Context *c,int index)
{
    if (c->target.sourceActor == 0)
















































































        AssertFail(__FILE__, __LINE__, 0);
    return c->target.sourceActor->returnFire & ((1 << index) != 0);
}
int WeaponAlwaysEnabled() { return 1; }
int WeaponStatus(Context *c,int index)
{
    if (!c->target.sourceActor || !c->target.targetId) return 2;
    ActorUnit *u=c->target.sourceActor->unit;
    UnitSlot *slot=&u->slots[index+11];
    if (!slot->item) return 1;
    if (!slot->condition) return 3;
    WeaponScore *r=&c->score[index];
    Weapon temporary;
    Weapon *w=(Weapon *)UnitFogObject(u,index+11,&temporary);
    r->weapon=*w;
    if (!WeaponEfficiency[w->armorClass]) return 11;
    if (r->weapon.ammo && !slot->amount) return 6;
    if ((int)slot->uses >= r->weapon.threshold) return r->weapon.ammo == 0 ? 5 : 4;
    if (HercEnergy(u) < r->weapon.energy && c->target.sourceActor->owner != gRemoteActorKind) return 7;
    if (!UnitCapability(c->target.sourceActor->pilot,r->weapon.skill)) return 10;
    if (c->target.distance-100 > r->weapon.range*1000) return 8;
    if (r->weapon.limit >= c->target.elevation) return 9;
    if (modeSwitch) {
        if (IsRestrictedMode(&c->target.sourceActor->action)) return 12;

    }
    if (!modeSwitch && gRemoteActorKind == c->target.sourceActor->owner) {
        int kind=f_00439d02(c->target.sourceActor->unit); int visible=!IsTypeHidden(c->target.sourceActor->action.resource);
        int range=weaponMissionRanges[gMissionKind]+weaponClassRanges[kind]+weaponRangeBase;
        if (gMission==410) range+=2;
        if (gMission==418) range+=3;
        if (visible) range+=weaponVisibleRanges[kind];
        if (HealthPercent(c->target.sourceActor->unit)!=100) range+=100;
        if (c->target.distance-100 > range*1000) return 8;
    }
    return 0;
}
int WeaponAccuracy(Context *c,int index)
{
    if (!c->target.sourceActor || !c->target.sourceActor->unit || !c->target.sourceActor->pilot)























































































        AssertFail(__FILE__, __LINE__, 0);
    WeaponScore *r=&c->score[index];
    ActorRecord *target=c->target.targetActor, *source=c->target.sourceActor;
    void *pilot=source->pilot; int skill=UnitCapability(pilot,r->weapon.skill);
    int range=Interpolate(r->weapon.nearRange*1000,135,r->weapon.farRange*1000,0,c->target.distance);
    int sourceSkill=c->target.sourceSkill, targetSkill=c->target.targetSkill;
    if (source->action.state==7) sourceSkill-=20;
    if (target && target->action.state==7 && c->target.targetHeight >= c->target.sourceHeight) targetSkill+=20;
    int heights=(int)c->target.sourceHeight-c->target.targetHeight; int modifiers=sourceSkill-targetSkill+heights*5;
    int bonus=c->fog.accuracy+GetModePlayer(modeCheckName,source->owner)->accuracy;
    if (target) {
        bonus-=HercDefense(target->unit);
        bonus-=(UnitCapability(target->pilot,6)-50)/4;
        int action=AnimActionGetMetadata(&target->action);
        ActorItem *part=PopFindItem(target->unit,0);
        bonus+=part->defense+weaponActionDefense[action];
    }
    int chance=range+modifiers; chance+=bonus; chance=chance*skill/100;
    chance=Clamp(chance,0,100);
    if (weaponHalfAccuracy) chance=chance*50/100;
    return chance;
}
int WeaponPower(Context *c,int index)
{
    if (!c->target.sourceActor || !c->target.sourceActor->unit)








































































        AssertFail(__FILE__, __LINE__, 0);
    WeaponScore *r=c->score; r+=index;
    int value=Interpolate(r->weapon.limit,0,r->weapon.power,100,(c->target.elevation*(c->fog.elevation+100))/100);
    if (r->weapon.power == 0) value-=weaponTerrainPenalty;
    return Clamp(value,0,100);
}
inline int Larger(int a,const int &b) { if (a > b) return a; return b; }
int WeaponMaximum(Context *c,int index)
{
    WeaponScore *r=&c->score[index];
    int damage=r->weapon.armorDamage+r->weapon.penetrationDamage;
    int direct=r->weapon.directDamage;
    int best=Larger(damage,direct);
    best*=r->weapon.ringCount;
    return best;
}
int WeaponExpected(Context *c,int index)
{
    ActorUnit *unit; int direction; ActorUnit *other;
    if (!c->target.sourceActor || !c->target.sourceActor->unit || !c->target.targetActor || !c->target.targetActor->unit) return 0;
    unit=c->target.sourceActor->unit; other=c->target.targetActor->unit;
    WeaponScore *r=&c->score[index];
    int falloff=Interpolate(r->weapon.falloffNear,100,r->weapon.falloffFar,0,(c->target.distance+100)/1000);
    int factor=Clamp(falloff,10,100);
    r->rangeFactor=factor;
    direction=c->target.modifier+0x80000034-c->target.targetActor->action.direction;
    int damage=factor*r->weapon.ringCount;
    int armor,body;
    UnitDamageSplit(other,direction,damage,(Weapon *)PopFindItem(unit,index+11),armor,body);
    int result=((armor+body)*r->chance+50)/100;
    if (c->target.sourceActor->owner==leaderPlayer && r->weapon.armorDamage > r->weapon.directDamage) {
        if (armor) {
            result=r->weapon.armorDamage*4*r->chance/100;
            if (armor >= ((int *)((char *)other+0xcb))[GridClamp(direction)]) result*=4;
        } else result/=2;
    }
    return result;
}
int WeaponAmmoFactor(Context *c,int index)
{
    WeaponScore *r=&c->score[index];
    int ammo=100;
    if (r->weapon.ammo) {
        if (c->target.sourceActor == 0)





































































            AssertFail(__FILE__, __LINE__, 0);
#define LEFT c->target.sourceActor->unit->slots[index+11].amount
        int left=LEFT;
        ammo=left*100/r->weapon.ammo+r->weapon.ammo/2;
    }
    int factor=0;
    if (r->maximum) factor=ammo*r->expected/r->maximum;
    return Clamp(factor,0,500);
}
int WeaponEvaluate(Context *c,int index,int checkEnabled)
{
    WeaponScore *r=&c->score[index];
    r->maximum=0; r->expected=0; r->ammoFactor=0; r->accuracy=0; r->power=0; r->chance=0;
    if (!c->target.sourceActor || !c->target.targetId) { r->state=2; return 0; }
    if (checkEnabled && !WeaponEnabled(c,index)) return 0;
    r->state=WeaponStatus(c,index);
    if (r->state) return 0;
    r->accuracy=WeaponAccuracy(c,index);
    r->power=WeaponPower(c,index);
    r->chance=r->accuracy*r->power/100;
    r->maximum=WeaponMaximum(c,index);
    r->expected=WeaponExpected(c,index);
    r->ammoFactor=WeaponAmmoFactor(c,index);
    return 1;
}
int WeaponTerrain(int source,int target)
{
    int total=0;
    if (source && target) {
        Point targetGrid,sourcePoint,targetPoint;
        TargetObjectInit(fallbackName,&sourcePoint,source);
        TargetObjectInit(fallbackName,&targetPoint,target);
        int facing=GridClamp(PointAngle(&targetPoint,&sourcePoint));
        BestLocSetData(fallbackName,&targetGrid,(void *)target);
        unsigned int height=targetDefinitionTable[target].height;
        Point *offset=pathDirections, *end=offset+6;
        int weightIndex=facing*6;
        for (; offset!=end; ++offset,++weightIndex) {
            Point neighbor=targetGrid;
            neighbor.x+=offset->x;neighbor.y+=offset->y;
            int cell=ResolveName(fallbackName,(const char *)&neighbor);
            if (cell) { int delta=targetDefinitionTable[cell].height-height; if (delta>0) total+=delta*weaponTerrainWeights[weightIndex]; }
        }
    }
    return total;
}
void WeaponPrepare(Context *c,TargetRecord *target,int checkEnabled)
{
    c->target=*target;
    if (c->target.sourceActor && c->target.sourceActor->unit) c->fog=*UnitFogObject(c->target.sourceActor->unit,9,&c->fog);
    weaponTerrainPenalty=0;
    if (c->target.targetId) weaponTerrainPenalty=WeaponTerrain(c->target.sourceId,c->target.targetId);
    for (int i=0;i<8;i++) WeaponEvaluate(c,i,checkEnabled);
    weaponTerrainPenalty=0;
}
int WeaponAreaCallback(AreaCandidate *candidate)
{
    int actorId=candidate->actor; if(actorId) {
        ActorRecord *actor=(ActorRecord *)TargetActorById(candidate->actor);
        if (actor) {
            if (actor->owner==weaponAreaOwner) { weaponAreaScore+=weaponRingDamage; return 0; }
            weaponAreaScore-=weaponRingDamage;
        }
    }
    return 0;
}
int WeaponAreaValue(int owner,Weapon *w,int target)
{
    Area area; Point at;
    weaponAreaScore=0; weaponAreaOwner=owner;
    if (target) {
        BestLocSetData(fallbackName,&at,(void *)target);
        Weapon *ring=w;
        for(int i=0;i<w->rings;i++,ring=(Weapon *)((char *)ring+2)) {
            weaponRingDamage=ring->ringDamage;
            BestLocInitialize(&area,fallbackName,i);
            BestLocSetCandidate(&area,(void *)target);
            AreaEnumerate(area.words,(void *)WeaponAreaCallback,i,i);
        }
    }
    return weaponAreaScore;
}
int WeaponChoose(Context *c,TargetRecord *target,int minimum,int checkEnabled)
{
    int weights[8];
    WeaponPrepare(c,target,checkEnabled);
    ActorRecord *actor=target->sourceActor;
    if (!actor)




























































































































        AssertFail(__FILE__, __LINE__, 0);
    unsigned int total=0;
    for(int i=0;i<8;i++) {
        int weight=c->score[i].ammoFactor;
        weights[i]=0;
        if (weight>=minimum) {
            ActorUnit *unit=actor->unit; Weapon *w=(Weapon *)PopFindItem(unit,i+11);
            if (w->rings<=1 || WeaponAreaValue(actor->owner,w,target->targetId)<0)
            { weights[i]=weight; total+=weight; }
        }
    }
    if(total) {
        unsigned int roll=next_link_seed(&MissionRandomTable)%total;
        int choice=roll;
        for(int i=0;i<8;i++) { choice-=weights[i]; if(choice<0) return i; }
    }
    return -1;
}


