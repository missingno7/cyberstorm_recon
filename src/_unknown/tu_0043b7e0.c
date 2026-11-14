/* Working names and packed layouts inferred from member accesses. No path evidence.
 * Provisional separate plain object, 0043b7e0..0043b8e7. The session object starts at 0043b8e8.
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
#include "actor.h"

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
struct ProfileManager : GameSession { Pilot *Select(int); Herc *Mission(int); };

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

extern HWND g_mainWindow;
extern "C" U8 f_00439daf(Herc *);
extern "C" void f_0043c17c(GameSession *);
extern "C" void f_004773a1(Pilot *, int, int);

inline int IsSpecial(GameSession *s) {return s->mode==2 || s->mode==3;}
#define Larger(a,b) ((a)>(b)?(a):(b))
extern "C" void f_0043b7e0(GameSession *s,int level)
{
    s->hercCapacity=Larger((int)gLevelInfo[level].hercs,s->hercCapacity);
    s->pilotCapacity=Larger((int)gLevelInfo[level].pilots,s->pilotCapacity);
    int special=IsSpecial(s);
    if(special && s->hercCapacity>12) s->hercCapacity=12;
    s->world=(U8)gLevelInfo[level].world;
    if(level>0 && gLevelInfo[level].world!=gLevelInfo[level-1].world) s->worldStage=1;
    else ++s->worldStage;
    short pilot=gLevelInfo[level].pilot;
    if(pilot>=0 && pilot<30 && !IsSpecial(s)) {f_0043bced(s,pilot);f_0043d1a6(s,pilot+51,1);}
}


