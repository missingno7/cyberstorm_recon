/* Working names and packed layouts inferred from member accesses. No path evidence.
 * Provisional plain-oe object, 0043b8e8..0043e3aa. The preceding helper is separate.
 * unknown fields are temporary layout evidence, not claimed original declarations. */
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char U8;
typedef unsigned short U16;
#pragma pack(push,1)
struct ActorListObject { char name[16]; U8 unknown10[0x6a]; int type; ActorListObject(); ~ActorListObject(); };
typedef ActorListObject Pilot;
struct HercSlot { U8 value, condition; U8 unknown02[6]; };
struct ActorUnit { char name[18]; HercSlot slots[23]; U8 unknownca[0x41]; };
typedef ActorUnit Herc;
struct Assignment { U8 pilot, active; };
struct RankEntry { unsigned cost; int index; };
struct Snapshot { char name[16]; U8 type; U8 slots[23]; Snapshot *next; Snapshot(); };
struct GameSession {
    int active; char *name; U8 unknown008[1020]; int mission, stage, battle; short difficulty;
    int stat412, stat416, stat41a, stat41e, stat422, mode;
    int stat42a, stat42e, stat432, stat436, stat43a;
    U8 level, world, worldStage, previousLevel;
    int progress, enabled, pilotCapacity, hercCapacity;
    Pilot reserve[30]; U8 reserveMode; int reserveSelection, reserveCount, allPilots;
    Pilot pilots[60]; Herc hercs[60]; Assignment assignments[60];
    int pilotSelection, hercSelection, pilotCount, hercCount;
    U8 known[128], oldKnown[128]; int credits, creditStep, stat70cf;
    Snapshot *snapshots; U8 unknown70d7[0x5c];
    RankEntry rankedHercs[60], rankedPilots[60];
    GameSession(); ~GameSession();
};
#pragma pack(pop)
/* Existing names provide a compatible view of this packed session prefix. */
struct ProfileManager : GameSession { Pilot *Select(int=-1); Herc *Mission(int=-1); };

extern "C" void f_00435e43(Pilot *,int);
extern "C" void f_004372cd(Pilot *,char *);
extern "C" void f_004372b6(Pilot *,char *);
extern "C" void f_00439d31(Herc *,char *);
extern "C" char *f_00439d2c(Herc *);
extern "C" int f_004394e0(Herc *,int);
extern "C" int f_00439736(Herc *,int);
extern "C" int f_004397d3(Herc *,int);
extern "C" void f_00439706(Herc *,int);
struct ActorItem;
extern ActorItem *PopFindItem(ActorUnit *,int,int);
extern "C" int f_00441ca7(GameSession *,int,int);
extern "C" int f_00439d48(Herc *);
extern unsigned SimLogicFlags(void *);
extern "C" unsigned f_00436dcc(Pilot *,int);
extern long SendEvent(void *,int,void *,int,int);

extern int g4941d8,g4941dc,g4941e0,g4941e4,g4941e8;
extern void *g4c9338,*g4c933c,*g4c9340,*g4c9344,*g4c9348;
extern "C" void f_0043bac3(GameSession *);
extern "C" int f_0043bbcc(GameSession *,const char *,Pilot *);
extern "C" int f_0043bced(GameSession *,int);
extern "C" void f_0043bd78(GameSession *,int);

extern "C" void f_0043c33c(GameSession *,int);
extern "C" int f_0043c4bf(GameSession *,const char *,Herc *);
extern "C" int f_0043c844(GameSession *);
extern "C" void f_0043ca71(GameSession *,RankEntry *,unsigned,int);
extern "C" int f_0043cdcc(GameSession *,int);
extern "C" int f_0043ce5d(GameSession *,Herc *);
extern "C" void f_0043d1a6(GameSession *,int,int);
extern "C" void f_0043dad7(GameSession *);
extern "C" void f_0044052b(GameSession *);
extern int MissionBaseType();
#pragma pack(push,1)
struct LevelInfo { U16 bonus, hercs, pilots; short pilot; int world, stringId; };
struct ActorItem { U8 unknown00[9]; U8 level; U8 unknown0a; int cost; };
typedef ActorItem ItemInfo;
#pragma pack(pop)
extern LevelInfo gLevelInfo[];


#pragma pack(push,1)
struct PilotType {U8 unknown00[0x99];U8 level;};
struct ItemCatalog {char *items;int size,first,count,unknown10;};
struct SessionSummary {U8 level,world,worldStage,previousLevel;char *title,*name;int stat412,stat416,stat41a,stat41e,stat422;U8 hercs,hercCapacity,pilots,pilotCapacity;int credits,cost,creditStep;};
#pragma pack(pop)
extern PilotType *gPilotTypes[];
extern ItemCatalog gItemCatalog[];
struct StrPool {char *GetString(long,int);}; extern StrPool g511ddc;
struct Shell {unsigned long NextCreditBreakpoint(unsigned long);}; extern Shell uiSystem;
extern int g4d0840;
extern char g4c929c[],g4c92b8[];
/* Existing adjacent width values are copied as one aggregate. */
struct Columns { int widths[2]; };
extern Columns g4941ec;
extern int g4941f0;
extern int (*g512500)(HWND,const char *,char *,int,int);
extern int (*dialogExport9)(HWND,const char *,int,int,int,int *,int);

extern char *HercChassisName(Herc *);
extern "C" int f_004379ba(Pilot *);
extern "C" void f_00435e70(Pilot *,int);
extern "C" void f_00441c2c(GameSession *,int);
extern "C" void f_00441456(GameSession *,int *);
extern "C" void f_00441b19(GameSession *,int);
extern "C" void f_0043e21a(GameSession *,int);

extern HWND g_mainWindow;
extern "C" U8 f_00439daf(Herc *);
extern "C" void f_0043c17c(GameSession *);
extern "C" int f_0043d152(GameSession *,int);
extern "C" void f_004773a1(Pilot *, int, int);

inline int SessionIsSpecial(GameSession *s) {return s->mode==2 || s->mode==3;}
extern "C" void f_0043b7e0(GameSession *,int);

/* Coordinate conversion is written as a macro: lexical parameter use is visible in register allocation. */
#define CONVERT_CLIENT_POINT(point) {RECT client,window;GetWindowRect(g_mainWindow,&window);GetClientRect(g_mainWindow,&client); \
int windowHeight=window.bottom-window.top;int clientHeight=client.bottom-client.top;int top=window.top; \
point->x+=window.left;point->y+=top+(windowHeight-clientHeight);}
GameSession profileManager;

Snapshot::Snapshot() {}

extern "C" Snapshot *f_0043b912(Snapshot *s, Herc *h)
{
    strncpy(s->name,h->name,16);
    s->type=f_00439daf(h);
    for (int i=0; i<23; ++i) s->slots[i]=h->slots[i].value;
    s->next=0;
    return s;
}
extern "C" void f_0043b956(POINT *point)
{
    RECT client,window;
    GetWindowRect(g_mainWindow,&window);
    GetClientRect(g_mainWindow,&client);
    int height=window.bottom-window.top;
    int clientHeight=client.bottom-client.top;
    point->x+=window.left;
    point->y=point->y+window.top+(height-clientHeight);
}
GameSession::GameSession() {memset(this,0,sizeof(*this));active=0;}
GameSession::~GameSession() {f_0043bac3(this);}

extern "C" GameSession *f_0043ba3e(GameSession *s,GameSession *source)
{
    Snapshot *last;
    f_0043bac3(s);
    memcpy(s,source,sizeof(*s));
    s->snapshots=0;
    Snapshot *old=source->snapshots;
    while(old) {
        Snapshot *copy=new Snapshot;
        *copy=*old;
        copy->next=0;
        if(!s->snapshots) {last=copy;s->snapshots=copy;}
        else {last->next=copy;last=copy;}
        old=old->next;
    }
    return s;
}
extern "C" void f_0043bac3(GameSession *s)
{
    Snapshot *node=s->snapshots;
    while(node) {Snapshot *old=node;node=node->next;delete old;}
    s->snapshots=0;
    if(!g4941d8) delete[] g4c9338;g4941d8=1;g4c9338=0;
    if(!g4941dc) delete[] g4c933c;g4941dc=1;g4c933c=0;
    if(!g4941e0) delete[] g4c9340;g4941e0=1;g4c9340=0;
    if(!g4941e4) delete[] g4c9344;g4941e4=1;g4c9344=0;
    if(!g4941e8) delete[] g4c9348;g4941e8=1;g4c9348=0;
    memset(s,0,sizeof(*s));
}
extern "C" int f_0043bbcc(GameSession *s,const char *name,Pilot *pilot)
{
    char base[28]; char result[28];
    int suffix=0;
    strcpy(base,name);
    char *colon=strrchr(base,':');
    if(colon) {int n=atoi(colon+1);if(n) {suffix=n;*colon=0;}}
    for(int i=0;i<s->pilotCount;++i) {
        Pilot *other=&s->pilots[i];
        if(other!=pilot) {
            f_004372cd(other,result);
            if(strstr(result,base)) {
                colon=strrchr(result,':');
                if(colon) {int n=atoi(colon+1);if(suffix<=n) suffix=n+1;}
                else if(!suffix) suffix=1;
            }
        }
    }
    if(suffix>0) {
        char *end=base+strlen(base);--end;if(*end==':') *end=0;
        sprintf(result,"%s:%d",base,suffix);
    } else strcpy(result,name);
    f_004372b6(pilot,result);
    return 1;
}
extern "C" int f_0043bced(GameSession *s,int kind)
{
    if(s->pilotCount==s->pilotCapacity) return 0;
    Pilot *p=&s->pilots[s->pilotCount];
    f_00435e43(p,kind);
    char name[28];f_004372cd(p,name);
    f_0043bbcc(s,name,p);
    ++s->pilotCount;s->pilotSelection=s->pilotCount;
    return s->pilotCount;
}

extern "C" int f_0043bd5f(GameSession *s) { return s->pilotCount < s->pilotCapacity; }
extern "C" void f_0043bd78(GameSession *s,int index)
{
    if(index==-1) index=s->pilotSelection;
    if(((ProfileManager *)s)->Select(index)) {
        for(int i=index-1;i<s->pilotCount-1;++i) s->pilots[i]=s->pilots[i+1];
        memset(&s->pilots[s->pilotCount-1],0,sizeof(Pilot));
        if(s->pilotSelection>index) --s->pilotSelection;
        else if(s->pilotSelection==index && index==s->pilotCount) --s->pilotSelection;
        --s->pilotCount;
        for(i=0;i<s->hercCount;++i) {
            if(s->assignments[i].pilot==index) s->assignments[i].pilot=0;
            else if(s->assignments[i].pilot>index) --s->assignments[i].pilot;
        }
    }
}

Pilot *ProfileManager::Select(int index)
{
    if(reserveMode) {
        if(index==-1) index=reserveSelection;
        if(index<1 || index>reserveCount) return 0;
        return &reserve[index-1];
    } else {
        if(index==-1) index=pilotSelection;
        if(index<1 || index>pilotCount) return 0;
        return &pilots[index-1];
    }
    return 0;
}
extern "C" void f_0043bed7(GameSession *s,int index)
{
    if(s->reserveMode) s->reserveSelection=index;
    else s->pilotSelection=index;
}
extern "C" void f_0043befa(GameSession *s,int delta)
{
    if(s->reserveMode) {
        s->reserveSelection+=delta;
        if(s->reserveSelection>s->reserveCount) s->reserveSelection=1;
        if(s->reserveSelection<1) s->reserveSelection=s->reserveCount;
    } else {
        s->pilotSelection+=delta;
        if(s->pilotSelection>s->pilotCount) s->pilotSelection=1;
        if(s->pilotSelection<1) s->pilotSelection=s->pilotCount;
    }
}
extern "C" int f_0043bf81(GameSession *s,Pilot *pilot)
{
    if(s->reserveMode) {
        for(int i=0; i<s->reserveCount; ++i) if(pilot==&s->reserve[i]) return i+1;
    } else {
        for(int i=0; i<s->pilotCount; ++i) if(pilot==&s->pilots[i]) return i+1;
    }
    return 0;
}
extern "C" int f_0043bfe1(GameSession *s)
{ if(s->reserveMode) return s->reserveCount; return s->pilotCount; }
extern "C" int f_0043bffc(GameSession *s,int index)
{
    if(index>=1 && index<=s->pilotCount)
        for(int i=0;i<s->hercCount;++i) if(s->assignments[i].pilot==index) return i+1;
    return 0;
}
extern "C" int f_0043c036(GameSession *s,int index)
{
    if(index<1 || index>s->hercCount) return 0;
    return s->assignments[index-1].pilot;
}
extern "C" int f_0043c05b(GameSession *s)
{
    int count=0;
    U8 again=1; U8 start=0;
    while(again) {
        again=0;
        for(int i=start;i<s->pilotCount;++i) if(!(SimLogicFlags(&s->pilots[i])&1)) {
            f_0043bd78(s,i+1);again=1;++count;if(start>0) start=i-1;break;
        }
    }
    again=1;start=0;
    while(again) {
        again=0;
        for(int i=start;i<s->hercCount;++i) if(!f_00439d48(&s->hercs[i])) {
            f_0043c33c(s,i+1);again=1;if(start>0) start=i-1;break;
        }
    }
    return count;
}

extern "C" void f_0043c167(GameSession *s) { s->reserveMode=1; f_0043c17c(s); }
extern "C" void f_0043c17c(GameSession *s)
{
    s->reserveCount=0;
    U8 limit=20;
    if(s->allPilots) limit=30;
    for(int i=0;i<limit;++i) {
        if(gPilotTypes[i]->level<=s->level || f_0043d152(s,i+51) || SessionIsSpecial(s) || (s->allPilots && i>=20)) {
            Pilot *p=&s->reserve[s->reserveCount++];f_00435e43(p,i);f_0043d1a6(s,i+51,1);
        }
    }
    s->reserveSelection=1;
}

extern "C" int f_0043c2ba(GameSession *s,int kind)
{
    if(s->hercCount==s->hercCapacity) return 0;
    Herc *h=&s->hercs[s->hercCount];h->slots[0].value=(U8)kind;
    char name[28];strcpy(name,f_00439d2c(h));
    f_0043c4bf(s,name,h);
    ++s->hercCount;return s->hercCount;
}

extern "C" int f_0043c327(GameSession *s) { return s->hercCount < s->hercCapacity; }
extern "C" void f_0043c33c(GameSession *s,int index)
{
    if(index==-1) index=s->hercSelection;
    for(int i=index-1;i<s->hercCount-1;++i) {
        s->hercs[i]=s->hercs[i+1];s->assignments[i]=s->assignments[i+1];
    }
    memset(&s->hercs[s->hercCount-1],0,sizeof(Herc));
    memset(&s->assignments[s->hercCount-1],0,sizeof(Assignment));
    if(s->hercSelection>index) --s->hercSelection;
    else if(s->hercSelection==index && index==s->hercCount) --s->hercSelection;
    --s->hercCount;
}

Herc *ProfileManager::Mission(int index)
{
    if(index==-1) index=hercSelection;
    if(index<1 || index>hercCount) return 0;
    return &hercs[index-1];
}
extern "C" void f_0043c441(GameSession *s,int index) {s->hercSelection=index;}
extern "C" void f_0043c450(GameSession *s,int delta)
{
    s->hercSelection+=delta;
    if(s->hercSelection>s->hercCount) s->hercSelection=1;
    if(s->hercSelection<1) s->hercSelection=s->hercCount;
}
extern "C" int f_0043c490(GameSession *s,Herc *herc)
{
    for(int i=0;i<s->hercCount;++i) if(herc==&s->hercs[i]) return i+1;
    return 0;
}
extern "C" int f_0043c4bf(GameSession *s,const char *name,Herc *herc)
{
    char base[28],result[28];
    int suffix=0;
    strcpy(base,name);
    char *colon=strrchr(base,':');
    if(colon) {int n=atoi(colon+1);if(n) {suffix=n;*colon=0;}}
    for(int i=0;i<s->hercCount;++i) {
        Herc *other=&s->hercs[i];
        if(other!=herc) {
            strcpy(result,(char *)other);
            if(strstr(result,base)) {
                colon=strrchr(result,':');
                if(colon) {int n=atoi(colon+1);if(suffix<=n) suffix=n+1;}
                else if(!suffix) suffix=1;
            }
        }
    }
    if(suffix>0) {
        int length=strlen(base)-1;char *end=base+length;if(*end==':') *end=0;
        sprintf(result,"%s:%d",base,suffix);
    } else strcpy(result,name);
    f_00439d31(herc,result);
    return 1;
}

extern "C" int f_0043c5d6(GameSession *s,POINT *point)
{
    CONVERT_CLIENT_POINT(point);
    Herc *h=((ProfileManager *)s)->Mission();
    strcpy(g4c929c,f_00439d2c(h));
    char name[52];strcpy(name,g4c929c);
    int result=g512500(g_mainWindow,g511ddc.GetString(250,1),name,12,1);
    if(result==1 && stricmp(name,g4c929c) && strlen(name)) {
        name[12]=0;return f_0043c4bf(s,name,h);
    }
    return 0;
}

extern "C" Pilot *f_0043c6ed(GameSession *s,Herc *herc)
{
    unsigned index=s->assignments[f_0043c490(s,herc)-1].pilot;
    if(index) return ((ProfileManager *)s)->Select(index);
    return 0;
}
extern "C" Herc *f_0043c73a(GameSession *s,Pilot *pilot)
{
    int index=f_0043bf81(s,pilot);
    for(int i=0;i<s->hercCount;++i) if(s->assignments[i].pilot==index) return ((ProfileManager *)s)->Mission(i+1);
    return 0;
}
extern "C" int f_0043c7eb(GameSession *s,int herc,unsigned pilot)
{
    for(int i=0;i<s->hercCount;++i) if(s->assignments[i].pilot==pilot) {
        s->assignments[i].pilot=0; s->assignments[i].active=0;
    }
    s->assignments[herc-1].pilot=(U8)pilot;
    s->assignments[herc-1].active=1;
    return 1;
}
extern "C" int f_0043c844(GameSession *s)
{
    unsigned hercs=0,pilots=0;
    memset(s->rankedHercs,0,sizeof(s->rankedHercs));memset(s->rankedPilots,0,sizeof(s->rankedPilots));
    for(int i=1;i<=s->hercCount;++i) if(s->assignments[i-1].pilot==0) {
        f_0043ca71(s,s->rankedHercs,f_0043cdcc(s,i-1),i);++hercs;
    }
    for(i=1;i<=s->pilotCount;++i) {
        U8 unassigned=1;
        for(int j=0;j<s->hercCount;++j) if(s->assignments[j].pilot==i) {unassigned=0;break;}
        if(unassigned) {
            Pilot *p=((ProfileManager *)s)->Select(i);
            if(p && SendEvent(p,101,0,0,0)) {f_0043ca71(s,s->rankedPilots,f_00436dcc(p,203),i);++pilots;}
        }
    }
    if(!hercs || !pilots) return 0;
    for(i=0;i<hercs && i<pilots;++i) f_0043c7eb(s,s->rankedHercs[i].index,s->rankedPilots[i].index);
    return 1;
}

extern "C" void f_0043ca71(GameSession *, RankEntry *table,unsigned cost,int index)
{
    for(int i=0;i<60;++i) if(table[i].cost<=cost) {
        for(int j=59;j>i;--j) {table[j].cost=table[j-1].cost;table[j].index=table[j-1].index;}
        table[i].cost=cost; table[i].index=index; return;
    }
}
extern "C" int f_0043cab9(GameSession *s)
{
    int count=0;
    for(int i=0;i<s->hercCount;++i) count+=(f_0043c6ed(s,&s->hercs[i])!=0);
    return count;
}
extern "C" int f_0043cb47(GameSession *s)
{
    int pilots=0,hercs=0;
    for(int i=1;i<=60;++i) {
        if(i<=s->pilotCount && !f_0043c73a(s,&s->pilots[i-1])) ++pilots;
        if(i<=s->hercCount && !f_0043c6ed(s,&s->hercs[i-1])) ++hercs;
    }
    return hercs<pilots?hercs:pilots;
}

extern "C" int f_0043ccc6(GameSession *s,int index) {return s->assignments[index-1].active!=0;}
extern "C" void f_0043cce3(GameSession *s,int index,int active) {s->assignments[index-1].active=active!=0;}
extern "C" int f_0043ccff(GameSession *s)
{int total=0;for(int i=0;i<s->hercCount;++i) total+=s->assignments[i].active;return total;}
extern "C" void f_0043cd28(GameSession *s,unsigned index)
{
    if(index==-1) index=s->pilotSelection;
    for(int i=0;i<s->hercCount;++i) if(s->assignments[i].pilot==index) {
        s->assignments[i].pilot=0;s->assignments[i].active=0;break;
    }
}
extern "C" void f_0043cd6c(GameSession *s)
{for(int i=0;i<s->hercCount;++i) {s->assignments[i].pilot=0;s->assignments[i].active=0;}}
extern "C" void f_0043cd92(GameSession *s,unsigned herc,unsigned slot,unsigned condition)
{if(herc<(unsigned)s->hercCount && slot<23 && condition<=99) s->hercs[herc].slots[slot].condition=(U8)condition;}
extern "C" int f_0043cdcc(GameSession *s,int index)
{
    if(index<0 || index>=s->hercCount) return 0;
    return f_0043ce5d(s,&s->hercs[index]);
}
extern "C" int f_0043ce5d(GameSession *s,Herc *h)
{
    int total=0;
    for(int i=0;i<23;++i) {
        ItemInfo *item=(ItemInfo *)PopFindItem(h,i,-1);
        int price;
        if(i<11) price=item->cost;
        else price=f_00441ca7(s,item->cost,s->level-item->level);
        price-=f_004394e0(h,i);
        if(price>0) total+=price;
    }
    return total;
}
extern "C" int f_0043cec6(GameSession *s,int index)
{if(index<0 || index>=s->hercCount) return 0; return f_004394e0(&s->hercs[index],23);}
extern "C" int f_0043cef6(GameSession *s)
{int total=0;for(int i=0;i<s->hercCount;++i) total+=f_004394e0(&s->hercs[i],23);return total;}
extern "C" int f_0043cf2d(GameSession *s,int index)
{
    if(index<0 || index>=s->hercCount) return -1;
    {
        int amount=f_0043cdcc(s,index);f_0043c33c(s,index+1);s->credits+=amount;return s->credits;
    }
    return -1;
}
extern "C" int f_0043cffc(GameSession *s,int index,int slot)
{
    if(index<0 || index>=s->hercCount || slot<0 || slot>=23) return -1;
    {
        Herc *h=&s->hercs[index];int price=f_00439736(h,slot);
        if(price>s->credits) return -1;
        s->credits-=price;return f_004397d3(h,slot);
    }
    return -1;
}
extern "C" int f_0043d066(GameSession *s,int index)
{
    if(index<0 || index>=s->hercCount) return -1;
    {
        int price=f_004394e0(&s->hercs[index],23);
        if(price>s->credits) return -1;
        f_00439706(&s->hercs[index],23);s->credits-=price;return s->credits;
    }
    return -1;
}
extern "C" int f_0043d0c8(GameSession *s)
{
    int cost=f_0043cef6(s);
    if(cost>s->credits) return -1;
    for(int i=0;i<s->hercCount;++i) f_00439706(&s->hercs[i],23);
    s->credits-=cost;return s->credits;
}

extern "C" int f_0043d152(GameSession *s,int index)
{if(index>1024) return 0;
int byte=index/8; int mask=1<<(index%8); return (s->known[byte]&mask)!=0;
}
extern "C" void f_0043d1a6(GameSession *s,int index,int value)
{
    if(index<=1024) {
        int byte=index/8;
        U8 mask=(U8)(1<<(index%8));
        if(value) s->known[byte]|=mask;
        else s->known[byte]&=~mask;
    }
}
extern "C" int f_0043d1fd(GameSession *s)
{int total=0;for(int i=0;i<1024;++i) total+=f_0043d152(s,i);return total;}
extern "C" void f_0043d25f(GameSession *s,int value)
{for(int i=0;i<1024;++i) f_0043d1a6(s,i,value);}
extern "C" int f_0043d2b9(GameSession *s,POINT *point)
{
    if(s->pilotCount<1) return -1;
    f_0043b956(point);
    char *list=new char[2048];memset(list,0,2048);
    for(int i=0;i<s->pilotCount;++i) {sprintf(g4c92b8,"%s\n",s->pilots[i].name);strcat(list,g4c92b8);}
    int result=dialogExport9(g_mainWindow,list,point->x,point->y,16,0,0);
    if(result<0 || result>=s->pilotCount) result=-1;
    delete[] list;return result;
}
extern "C" int f_0043d3cc(GameSession *s,POINT *point)
{
    if(s->hercCount<1) return -1;
    f_0043b956(point);
    char pilotName[16];
    char *list=new char[2048];memset(list,0,2048);
    for(int i=0;i<s->hercCount;++i) {
        unsigned pilot=s->assignments[i].pilot;
        if(pilot) strcpy(pilotName,s->pilots[pilot-1].name); else strcpy(pilotName,g511ddc.GetString(198,1));
        sprintf(g4c92b8,"%s\t%s\t%s\n",s->hercs[i].name,HercChassisName(&s->hercs[i]),pilotName);
        strcat(list,g4c92b8);
    }
    Columns columns = g4941ec;
    int result=dialogExport9(g_mainWindow,list,point->x,point->y,60,columns.widths,2);
    if(result>=0 && result<s->hercCount) f_0043c7eb(s,result+1,s->pilotSelection);
    else result=-1;
    delete [] list;return result;
}

extern "C" int f_0043d591(GameSession *s,POINT *point)
{
    CONVERT_CLIENT_POINT(point);
    Pilot *p=((ProfileManager *)s)->Select();f_004372cd(p,g4c929c);
    char name[52];strcpy(name,g4c929c);
    int result=g512500(g_mainWindow,g511ddc.GetString(251,1),name,12,1);
    if(result==1 && stricmp(name,g4c929c) && strlen(name)) {
        name[12]=0;result=f_0043bbcc(s,name,p);
        if(result && !strcmp(name,"Kurosh")) f_00435e43(p,21);
    } else result=0;
    return result;
}

extern "C" int f_0043d6b1(GameSession *s,int cost)
{
    if(cost>s->credits) {s->credits=0;return 0;}
    s->credits-=cost;return 1;
}
extern "C" void f_0043d6e3(GameSession *s,int reset)
{
    if(reset || !s->active) {
        f_0043bac3(s);s->active=1;s->reserveMode=0;s->allPilots=0;
        s->mission=0;s->stage=0;s->battle=0;s->stat412=0;s->stat416=0;s->stat41a=0;s->stat41e=0;s->stat422=0;
        s->difficulty=-1;s->mode=1;s->stat42a=-1;s->stat42e=-1;s->stat432=-1;s->stat436=-1;
        s->progress=0;s->level=0;s->previousLevel=s->level;s->worldStage=0;s->creditStep=0;
        s->unknown70d7[0x2d]=105; /* observed marker at +0x7104 */
        if(g4d0840) {s->level=1;s->worldStage=1;f_0043e21a(s,7);s->credits=45000;s->difficulty=0;}
        f_0043b7e0(s,s->level);s->enabled=1;f_0043dad7(s);f_0043c17c(s);f_0044052b(s);
    }
}
extern "C" int f_0043d846(GameSession *s)
{
    int total=0;
    for(int i=0;i<s->hercCount;++i) {
        if(!SessionIsSpecial(s) || f_0043c036(s,i+1)) total+=f_0043cdcc(s,i);
    }
    for(i=0;i<s->pilotCount;++i) {
        if(!SessionIsSpecial(s) || f_0043bffc(s,i+1)) total+=f_004379ba(&s->pilots[i]);
    }
    return total;
}
extern "C" void f_0043d9e8(GameSession *s,SessionSummary *out)
{
    out->level=s->level;out->world=s->world;out->worldStage=s->worldStage;out->previousLevel=s->previousLevel;
    out->title=g511ddc.GetString(gLevelInfo[s->level].stringId,1);out->name=s->name;
    out->stat412=s->stat412;out->stat416=s->stat416;out->stat41a=s->stat41a;out->stat41e=s->stat41e;out->stat422=s->stat422;
    out->hercs=(U8)s->hercCount;out->hercCapacity=(U8)s->hercCapacity;out->pilots=(U8)s->pilotCount;out->pilotCapacity=(U8)s->pilotCapacity;
    out->credits=s->credits;out->cost=f_0043d846(s);out->creditStep=s->creditStep;
}
extern "C" void f_0043dab1(GameSession *s) {s->creditStep=uiSystem.NextCreditBreakpoint(f_0043d846(s));}
extern "C" void f_0043dad7(GameSession *s)
{
    memcpy(s->oldKnown,s->known,128);
    for(int i=0;i<23;++i) {
        char *item=gItemCatalog[i].items;
        for(int j=0;j<gItemCatalog[i].count;++j) {
            if(s->level>=((ItemInfo *)item)->level) f_0043d1a6(s,gItemCatalog[i].first+j,1);
            item+=gItemCatalog[i].size;
        }
    }
    f_0043c17c(s);
}
extern "C" int f_0043dbb2(GameSession *s,Herc *source,Pilot *pilot)
{
    if(s->hercCount==s->hercCapacity) return -1;
    if(s->pilotCount==s->pilotCapacity) return -1;
    Herc *h=&s->hercs[s->hercCount];*h=*source;
    char name[28];strcpy(name,f_00439d2c(h));f_0043c4bf(s,name,h);
    ++s->hercCount;s->hercSelection=s->hercCount;
    Pilot *p=&s->pilots[s->pilotCount];*p=*pilot;
    f_00435e70(p,p->type);f_004372cd(p,name);f_0043bbcc(s,name,p);
    ++s->pilotCount;s->pilotSelection=s->pilotCount;
    f_0043c7eb(s,s->hercSelection,s->pilotSelection);return s->hercSelection;
}
extern "C" void f_0043dcfd(GameSession *s,unsigned mode)
{
    s->mode=mode;
    switch(mode) {
    case 0:case 1:
        if(s->level==0 && s->pilotCount==0 && s->hercCount==0) {
            f_00441c2c(s,0);f_0043bced(s,17);f_0043c7eb(s,s->hercSelection,s->pilotSelection);
            f_00441c2c(s,3);f_0043bced(s,18);f_0043c7eb(s,s->hercSelection,s->pilotSelection);
            s->credits=10000;s->difficulty=0;
        }
        break;
    case 2:case 3:s->credits=0;s->allPilots=1;break;
    }
}
extern "C" void f_0043deeb(GameSession *s)
{
    U16 world=(U16)gLevelInfo[s->level].world;
    while(world==(unsigned)gLevelInfo[s->level].world) {++s->level;f_0043b7e0(s,s->level);}
    f_0043dad7(s);
}
extern "C" void f_0043df40(GameSession *s)
{
    if(s->level<13) {
        ++s->level;f_0043b7e0(s,s->level);f_0043dad7(s);
        s->credits+=gLevelInfo[s->level].bonus*2500;
        if(gLevelInfo[s->level].bonus==0) {
            s->credits+=20000;
            if(s->mission<MissionBaseType()-1) {++s->mission;s->stage=0;s->battle=0;}
            f_0044052b(s);
        }
    }
}

extern "C" void f_0043e007(GameSession *s)
{s->level=13;s->mission=MissionBaseType()-1;s->progress=100;f_0044052b(s);memset(s->known,255,128);}
extern "C" void f_0043e04d(GameSession *s)
{
    if(s->level==13 && s->pilotCount>=gLevelInfo[13].pilots && s->hercCount>=gLevelInfo[13].hercs && s->mission==MissionBaseType()-1) return;
    while(s->level<14) {f_0043b7e0(s,s->level);++s->level;}
    s->level=13;s->mission=MissionBaseType()-1;s->progress=100;f_0044052b(s);
    memset(s->known,255,128);s->credits=9000000;
    int kind=0;
    while(s->pilotCount<s->pilotCapacity) {f_0043bced(s,kind);if(++kind==20) kind=0;}
    f_0043bed7(s,1);
    int count;f_00441456(s,&count);kind=0;
    while(s->hercCount<s->hercCapacity) {f_00441b19(s,kind);if(++kind==count) kind=0;}
    s->hercSelection=1;f_0043c844(s);s->credits=9000000;
}

extern "C" void f_0043e1dd(GameSession *s) {++s->progress;}
extern "C" void f_0043e1e8(GameSession *s) {s->progress+=3;}
extern "C" void f_0043e1f4(GameSession *s) {memset(s->known,255,128);}
extern "C" void f_0043e211(GameSession *s) {memset(s->known,0,128);}
extern "C" void f_0043e21a(GameSession *s,int requested)
{
int kind,count,groups;memset(s->known,255,128);s->credits=9000000;s->pilotCapacity=60;s->hercCapacity=60;for(kind=0;kind<20;++kind)f_0043bced(s,kind);f_0043bed7(s,1);f_00441456(s,&count);kind=0;if(count>=requested)groups=1;else groups=requested/count;
    if(count>requested) count=requested;
    {for(int group=0;group<groups;++group) for(kind=0;kind<count;++kind) f_00441b19(s,kind);}
    for(kind=0;kind<requested-count*groups;++kind) f_00441b19(s,kind);
    s->hercSelection=1;f_0043c844(s);s->credits=9000000;
}

extern "C" void f_0043e37c(GameSession *s) {s->allPilots=1;f_0043c17c(s);}
extern "C" void f_0043e38f(GameSession *s) {f_004773a1(s->pilots,s->pilotCount,sizeof(Pilot));}

