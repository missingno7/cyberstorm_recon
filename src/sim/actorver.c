/* Working names. Packed records follow the observed inventory layouts.
 * Unknown spans are provisional until the surrounding profile types are recovered. */
#include <string.h>
typedef unsigned char U8;
typedef unsigned long U32;
#pragma pack(push,1)
#include "actor.h"








struct Snapshot {
    char name[17];
    U8 slots[23];
    Snapshot *next;
};
struct UnitTemplate {
    int name;
    U8 header[20];
    U8 slots[23];
};
struct ItemRule {
    U8 head[1];
    U8 unknown[21];
    U32 mask;
};
struct ActorItem {
    U8 head[9];
    U8 level;
    U8 unknown0a;
    int cost;
    int unknown0f;
    int max;
    U8 rest[4];
};
struct TypeItem {
    U8 head[27];
    ItemRule rules[23];
    U8 rest[100];
};
struct ItemTable {
    int kind;
    char *items;
    int size,stringBase,count;
};
struct ListRow {
    char *name,*image;
    int cost;
    U8 type;
    int value,capacity,index;
};
struct Choice {
    int a,b,c,cost;
    ActorItem *item;
};
struct InventoryRow {
    int index;
    char *name;
    int condition;
};
struct GameSession {
    U8 header[0x43e];
    U8 level;
    U8 until44e[15];
    int capacity;
    U8 until30ab[0x2c59];
    ActorUnit hercs[60];
    U8 assignments[60][2];
    int pilotSelection,hercSelection,pilotCount,hercCount;
    U8 known[256];
    int credits,creditStep,stat70cf;
    Snapshot *snapshots;
};
#pragma pack(pop)
struct StrPool {
    char *GetString(long,int=1);
    char *GetSecondString(long,int=1);
};
extern StrPool g511ddc,gMenuPrefixStrings,gHercStrings,g511e18;
extern ItemTable HercItemTables[];
extern UnitTemplate unitTemplates[];
extern TypeItem unitTypes[];
extern char weaponTypes[],systemTypes[];
extern ListRow *g4c9338;
extern char *g4c933c;
extern int g4941d8,g4941dc;
extern char gDebugText[];
extern char reportToken_0049417c[], reportToken_00494184[], reportToken_0049419c[], reportToken_004941a8[], reportToken_004941c8[], reportBreak_0048b1e6[];
extern InventoryRow inventoryRows[23];
extern Choice upgradeRows[76],sortRows[76];
extern "C" int f_0043d152(GameSession *,int);
extern "C" void LoadActorData(ActorUnit *,UnitTemplate *,Snapshot * = 0);
extern "C" int f_0043ce5d(GameSession *,ActorUnit *);
extern int HercFreePower(ActorUnit *);
extern int HercMobility(ActorUnit *);
extern "C" const char *f_00439d2c(ActorUnit *);
extern "C" void f_0043c4bf(GameSession *,const char *,ActorUnit *);
extern int f_0043ac9c(ActorUnit *,int,int);
extern ActorItem *PopFindItem(ActorUnit *,int,int=-1);
struct SessionLevel;
extern int HercFillItems(ActorUnit *,int,SessionLevel *,Choice *,int);
extern "C" int f_004394e0(ActorUnit *,int);
extern char *HercFormatItem(ActorUnit *,ActorItem *,int,unsigned);
extern int HercInstallItem(ActorUnit *,int,ActorItem *);
extern int HercMountAllowed(ActorUnit *,int);
extern "C" int f_00439daf(ActorUnit *);
extern int StateInvoke(ActorUnit *,int *,int *);
extern int StateInvokeEnergy(ActorUnit *,int *,int *);
struct ActorState;
extern int ActorStateInvalid(ActorState *,int);
struct PlayerSlot;
extern PlayerSlot *GetModePlayer(void *,int);
extern "C" char *BriefFormat(const char *,...);
extern void DebugTextClear();
extern void DebugTextAppend(char *);
extern void f_00477295(char *);


extern int g494290,playerCount,gRemoteActorKind,modeSwitch,g4d3551,gLaunchBudget;
extern U8 g4c978a;
extern char modeCheckName[];






extern "C" int f_004422f0(ActorUnit *unit,int slot) {
    TypeItem type=*(TypeItem *)PopFindItem(unit,0);
    UnitSlot selected=unit->slots[slot];
    unsigned value=selected.item;
    U32 mask=type.rules[slot].mask;
    if(slot && value) {
        if(slot>=11 && slot<=18) {
            if(!(*(U32 *)(weaponTypes+value*70+0x31)&mask))return 1;
        }
        else if(slot>=19 && slot<=22) {
            if(!(*(U32 *)(systemTypes+value*71+0x32)&mask))return 1;
        }
        else {
            ActorItem item=*PopFindItem(unit,slot,value);
            if(!(mask&(1<<item.head[1])))return 1;
        }
    }
    return 0;
}
extern "C" int f_004423d7(ActorUnit *unit,int level) {
    int type=unit->slots[0].item;
    if(type<=0 || type>=34)return 1;
    if(type>=9 && type<=17)return 1;
    if(f_00439daf(unit)>level+1)return 1;
    for(int i=0; i<23; ++i) {
        if(!HercMountAllowed(unit,i) && unit->slots[i].item)return 1;
        if(unit->slots[i].condition>100)return 1;
        ActorItem *item=PopFindItem(unit,i);
        if(item->max && item->max<unit->slots[i].amount)return 1;
        if(f_004422f0(unit,i))return 1;
    }
    ActorItem *item=PopFindItem(unit,7);
    if(*(int *)((char *)unit+0x4d)>*(int *)((char *)item+0x1f))return 1;
    int a,b,c,d;
    StateInvoke(unit,&a,&b);
    StateInvokeEnergy(unit,&c,&d);
    return a>b || c>d;
}
extern "C" void f_004424d4(ActorRecord *actor) {
    if(!g494290 || playerCount<=2 || actor->owner==gRemoteActorKind)return;
    unsigned level;
    if(modeSwitch) level=gLaunchBudget; else level=g4c978a;
    if(g4d3551)level=14;
    int pilot=actor->pilot && ActorStateInvalid((ActorState *)actor->pilot,level);
    int unit=actor->unit && f_004423d7(actor->unit,level);
    if(pilot || unit) {
        f_00477295(BriefFormat("WARNING: I detect a possible cheater in our midst.\nPlayer: (#%d) %s\nHerc name: %s %s\nPilot name: %s %s\n",actor->owner,(char *)GetModePlayer(modeCheckName,actor->owner)+0x44,actor->unit,unit?"(QUESTIONABLE DATA)":"(looks fine)",actor->pilot,pilot?"(QUESTIONABLE DATA)":"(looks fine)"));
    }
}


