/* Working names. Packed records follow the observed inventory layouts.
 * Unknown spans are provisional until the surrounding profile types are recovered. */
#include <string.h>
typedef unsigned char U8;
typedef unsigned long U32;
#pragma pack(push,1)
struct Slot {
    U8 kind, value, condition, unknown;
    int amount;
};
struct ActorUnit {
    char name[17];
    Slot slots[23];
    U8 rest[66];
};
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
extern "C" void MemoryRelease(void *);
extern void *MemoryAllocationRetry(U32);
extern int g494290,playerCount,gRemoteActorKind,modeSwitch,g4d3551,gLaunchBudget;
extern U8 g4c978a;
extern char modeCheckName[];
struct ActorRecord {
    int a,b,id;
    U8 rest[12];
    ActorUnit *unit;
    void *pilot;
};
extern "C" int f_004413e0(GameSession *s,UnitTemplate *t) {
    TypeItem *type=&unitTypes[t->slots[0]];
    for(int i=0; i<23; ++i) {
        if(type->rules[i].head[0]) {
            unsigned value=t->slots[i];
            if(value) {
                if(!f_0043d152(s,HercItemTables[i].stringBase+value)) return 0;
            }
        }
    }
    return 1;
}
extern "C" ListRow *f_00441456(GameSession *s,int *count) {
    *count=0;
    int standard=0,custom=0,i=0;
    for(; unitTemplates[i].name; ++i) if(f_004413e0(s,&unitTemplates[i])) ++standard;
    Snapshot *p=s->snapshots;
    while(p) {
        ++custom;
        p=p->next;
    }
    *count=standard+custom;
    if(!*count)return 0;
    s->stat70cf=*count;
    if(!g4941d8)delete [] g4c9338;
    else g4941d8=0;
    g4c9338=new ListRow[*count+1];
    memset(g4c9338,0,(*count+1)*sizeof(ListRow));
    int n=0;
    for(i=0; unitTemplates[i].name; ++i) {
        UnitTemplate t=unitTemplates[i];
        if(n<*count && f_004413e0(s,&unitTemplates[i])) {
            g4c9338[n].name=gHercStrings.GetSecondString(t.name);
            g4c9338[n].image=g511e18.GetString(t.name);
            g4c9338[n].type=t.slots[0];
            g4c9338[n].index=i;
            ActorUnit unit;
            LoadActorData(&unit,&unitTemplates[i],0);
            g4c9338[n].cost=f_0043ce5d(s,&unit);
            g4c9338[n].value=HercFreePower(&unit);
            g4c9338[n].capacity=HercMobility(&unit);
            ++n;
        }
    }
    for(p=s->snapshots,i=1; p; p=p->next,++i) {
        g4c9338[n].name=(char *)p;
        g4c9338[n].image=g511ddc.GetString(150);
        g4c9338[n].type=p->slots[0];
        g4c9338[n].index=-i;
        UnitTemplate t;
        memcpy(t.slots,p->slots,23);
        ActorUnit unit;
        LoadActorData(&unit,&t,p);
        g4c9338[n].cost=f_0043ce5d(s,&unit);
        g4c9338[n].value=HercFreePower(&unit);
        g4c9338[n].capacity=HercMobility(&unit);
        ++n;
    }
    return g4c9338;
}
extern "C" char *f_004418a9(GameSession *s,int index) {
    int i; if(index<0 || index>=s->stat70cf)return 0;
    UnitTemplate t;
    int id=g4c9338[index].index;
    if(id>=0)t=unitTemplates[id];
    else {
        int n=-id-1;
        Snapshot *p=s->snapshots;
        while(p && --n!=-1)p=p->next;
        if(!p)return 0;
        memcpy(t.slots,p->slots,23);
    }
    DebugTextClear();
    DebugTextAppend(BriefFormat("%s%s%s%s\n\n",reportToken_004941c8,reportToken_0049419c,reportToken_00494184,g4c9338[index].name));
    DebugTextAppend(BriefFormat("%s",reportToken_004941c8));
    TypeItem *type=&unitTypes[t.slots[0]];
    for(i=0; i<23; ++i) {
        if(type->rules[i].head[0]) {
            DebugTextAppend(BriefFormat("%s%s%s%s%s\n",reportToken_0049417c,gMenuPrefixStrings.GetString(i+1025),reportToken_004941a8,reportToken_00494184,gHercStrings.GetString(HercItemTables[i].stringBase+t.slots[i])));
        }
        if(i==10 || i==18)DebugTextAppend(reportBreak_0048b1e6);
    }
    DebugTextAppend("\n\n");
    DebugTextAppend(BriefFormat("%s",reportToken_004941c8));
    DebugTextAppend(id>=0?g511e18.GetString(t.name):g511ddc.GetString(150));
    if(!g4941dc)delete [] g4c933c;
    else g4941dc=0;
    int len=strlen(gDebugText);
    g4c933c=new char[len+32];
    memset(g4c933c,0,len+32);
    strcpy(g4c933c,gDebugText);
    return g4c933c;
}

extern "C" int f_00441b19(GameSession *s,int index) {
    if(index<0 || index>=s->stat70cf)return 0;
    if(s->hercCount==s->capacity)return 0;
    ActorUnit *unit=&s->hercs[s->hercCount];
    int id=g4c9338[index].index;
    UnitTemplate t;
    Snapshot *p;
    if(id>=0) {
        LoadActorData(unit,&unitTemplates[id],0);
    }
    else {
        int n=-id-1;
        p=s->snapshots;
        while(p && --n!=-1)p=p->next;
        if(!p)return 0;
        memcpy(t.slots,p->slots,23);
        LoadActorData(unit,&t,p);
    }
    char name[28];
    strcpy(name,f_00439d2c(unit));
    f_0043c4bf(s,name,unit);
    ++s->hercCount;
    s->credits-=g4c9338[index].cost;
    s->hercSelection=index+1;
    return s->hercCount;
}

extern "C" int f_00441c2c(GameSession *s,unsigned index) {
    ActorUnit *unit=&s->hercs[s->hercCount];
    unsigned n;
    for(n=0; unitTemplates[n].name; ++n);
    if(index>=n)return -1;
    UnitTemplate *t=&unitTemplates[index];
    LoadActorData(unit,t);
    s->hercSelection=++s->hercCount;
    return s->hercCount;
}
extern "C" int f_00441ca7(GameSession *s,int value,int levels) {
    if(levels>0) {
        if(levels>1)value=f_00441ca7(s,value,levels-1);
        value=value*80/100;
    }
    return value;
}
extern "C" int f_00441ce1(GameSession *s,int index) {
    if(index==-1)index=s->hercSelection;
    if(index<1 || index>s->hercCount)return -1;
    return HercFreePower(&s->hercs[index-1]);
}
extern "C" int f_00441d1f(GameSession *s,int index) {
    if(index==-1)index=s->hercSelection;
    if(index<1 || index>s->hercCount)return -1;
    return HercMobility(&s->hercs[index-1]);
}
extern "C" int f_00441d5d(GameSession *s) {
    int found=0;
    for(int i=0; i<s->hercCount; ++i)if((unsigned)s->assignments[i][0]) {
        if(f_0043ac9c(&s->hercs[i],1,0)) {
            found=1;
            break;
        }
    }
    return found;
}
extern "C" InventoryRow *f_00441dad(GameSession *s,int index,int *count) {
    *count=0;
    if(index<0 || index>=s->hercCount)return 0;
    memset(inventoryRows,0,sizeof(inventoryRows));
    ActorUnit *unit=&s->hercs[index];
    TypeItem type=*(TypeItem *)PopFindItem(unit,0);
    int n=0;
    for(int i=0; i<23; ++i) {
        InventoryRow *row=&inventoryRows[n];
        if(type.rules[i].head[0]) {
            unsigned value=unit->slots[i].value;
            int id=HercItemTables[i].stringBase;
            row->index=i;
            row->name=gHercStrings.GetSecondString(id+value);
            row->condition=unit->slots[i].condition;
            ++n;
        }
    }
    for(i=n; i<23; ++i)inventoryRows[i].index=-1;
    *count=n;
    return inventoryRows;
}
extern "C" void f_00441ec5(GameSession *,Choice *list,int count) {
    if(!list || !list[1].item || !list[2].item)return;
    memset(sortRows,0,sizeof(sortRows));
    sortRows[0]=list[0];
    sortRows[1]=list[1];
    int n=2;
    for(int i=2; list[i].item && i<count && n<count; ++i) {
        int level=list[i].item->level;
        int cost=list[i].cost;
        int j=1;
        while(j<n && level<sortRows[j].item->level)++j;
        while(j<n && level==(int)sortRows[j].item->level && cost<sortRows[j].cost)++j;
        if(j<n)memmove(&sortRows[j+1],&sortRows[j],(count-j-1)*20L);
        sortRows[j]=list[i];
        ++n;
    }
    memcpy(list,sortRows,count*sizeof(Choice));
}
extern "C" Choice *f_00442045(GameSession *s,int index,int slot,int *count) {
    *count=0;
    if(index<0 || index>=s->hercCount)return 0;
    int max=76;
    memset(upgradeRows,0,sizeof(upgradeRows));
    ActorUnit *unit=&s->hercs[index];
    *count=HercFillItems(unit,slot,(SessionLevel *)s,upgradeRows,max);
    if(!*count)return 0;
    ActorItem *item=PopFindItem(unit,slot);
    int cost;
    if(slot<11)cost=item->cost;
    else {
        int levels=(int)s->level-(int)item->level;
        cost=f_00441ca7(s,item->cost,levels);
    }
    int remaining=cost-f_004394e0(unit,slot);
    if(remaining<0)remaining=0;
    for(int i=0; upgradeRows[i].item && i<max; ++i) {
        item=upgradeRows[i].item;
        upgradeRows[i].cost=(slot<11?item->cost:f_00441ca7(s,item->cost,(int)s->level-(int)item->level))-remaining;
        if(i==0 && cost-f_004394e0(unit,slot)<0)upgradeRows[0].cost=f_004394e0(unit,slot);
    }
    f_00441ec5(s,upgradeRows,max);
    return upgradeRows;
}
extern "C" void *f_0044220e(GameSession *s,int index,int slot,Choice *entry) {
    if(index<0 || index>=s->hercCount)return 0;
    if(slot<0 || slot>=23)return 0;
    return HercFormatItem(&s->hercs[index],entry->item,slot,-39);
}
extern "C" char *f_00442258(GameSession *s,int index,int slot,int choice) {
    if(index<0 || index>=s->hercCount || slot<0 || slot>=23)return 0;
    int count;
    Choice *list=f_00442045(s,index,slot,&count);
    if(!count || !list || choice>count)return 0;
    int id=HercInstallItem(&s->hercs[index],slot,list[choice].item);
    char *name=gHercStrings.GetSecondString(id);
    s->credits-=list[choice].cost;
    return name;
}
extern "C" int f_004422f0(ActorUnit *unit,int slot) {
    TypeItem type=*(TypeItem *)PopFindItem(unit,0);
    Slot selected=unit->slots[slot];
    unsigned value=selected.value;
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
    int type=unit->slots[0].value;
    if(type<=0 || type>=34)return 1;
    if(type>=9 && type<=17)return 1;
    if(f_00439daf(unit)>level+1)return 1;
    for(int i=0; i<23; ++i) {
        if(!HercMountAllowed(unit,i) && unit->slots[i].value)return 1;
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
    if(!g494290 || playerCount<=2 || actor->id==gRemoteActorKind)return;
    unsigned level=modeSwitch?gLaunchBudget:g4c978a;
    if(g4d3551)level=14;
    int pilot=actor->pilot && ActorStateInvalid((ActorState *)actor->pilot,level);
    int unit=actor->unit && f_004423d7(actor->unit,level);
    if(pilot || unit) {
        extern char reportWarning[];
        f_00477295(BriefFormat(reportWarning,actor->id,(char *)GetModePlayer(modeCheckName,actor->id)+0x44,actor->unit,unit?"(QUESTIONABLE DATA)":"(looks fine)",actor->pilot,pilot?"(QUESTIONABLE DATA)":"(looks fine)"));
    }
}


