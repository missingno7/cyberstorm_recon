/* Provisional fragment of the ProfileManager object; no embedded path.
 * Packed layouts shared with the published tu_0043b8e8. Names are working labels. */
#include <string.h>
typedef unsigned char U8;
#pragma pack(push,1)
struct ActorListObject { char name[16]; U8 unknown10[0x6a]; int type; };
#include "actor.h"

struct Snapshot { char name[16]; U8 type; U8 slots[23]; Snapshot *next; Snapshot(); };
struct GameSession {
    U8 unknown000[0x3fc]; int mission; U8 unknown400[0x3e]; U8 level;
    U8 unknown43f[0x13]; ActorListObject reserve[30]; U8 reserveMode;
    int reserveSelection,reserveCount,allPilots;
    ActorListObject pilots[60]; ActorUnit hercs[60]; U8 assignments[120];
    int pilotSelection,hercSelection,pilotCount,hercCount;
    U8 known[128],oldKnown[128]; int credits,creditStep,stat70cf;
    Snapshot *snapshots; U8 unknown70d7[0x2d]; U8 fileVersion;
};
#pragma pack(pop)
struct FileRecord;
unsigned long StreamRead(FileRecord *,void *,unsigned long);
unsigned long StreamWrite(FileRecord *,const void *,unsigned long);
extern "C" void f_0043bac3(GameSession *);
extern "C" void f_00435e43(ActorListObject *,unsigned long);
struct ActorState;
void ActorBindDescriptor(ActorState *,int);
extern "C" Snapshot *f_0043b912(Snapshot *,ActorUnit *);
extern int g4941d8,g4941dc,g4941e0,g4941e4,g4941e8;
extern void *g4c9338,*g4c933c,*g4c9340,*g4c9344,*g4c9348;

int LoadSession(GameSession *s,FileRecord *file) {
    f_0043bac3(s);
    if(StreamRead(file,s,0x74f3)!=0x74f3) return 0;
    for(int i=0;i<s->reserveCount;++i) {
        ActorListObject *p=&s->reserve[i]; f_00435e43(p,p->type);
    }
    for(i=0;i<s->pilotCount;++i) {
        ActorListObject *p=&s->pilots[i]; ActorBindDescriptor((ActorState *)p,p->type);
    }
    s->snapshots=0;
    g4c9338=0;g4c9348=0;g4c9344=0;g4c9340=0;g4c933c=0;
    g4941e8=1;g4941e4=1;g4941e0=1;g4941dc=1;g4941d8=1;
    if(s->fileVersion!=0x69) {s->fileVersion=0x69;s->mission=1;}
    return 1;
}
int SaveSession(GameSession *s,FileRecord *file) {
    return StreamWrite(file,s,0x74f3)==0x74f3;
}
int f_43ee17(GameSession *s,FileRecord *file,int index) {
    char snapshot[44];
    if(index<0 || index>=s->hercCount) return 0;
    f_0043b912((Snapshot *)snapshot,&s->hercs[index]);
    if(StreamWrite(file,&snapshot,0x2c)!=0x2c) return 0;
    return 1;
}
int f_43ee71(void *object,FileRecord *file) {
    GameSession *s=(GameSession *)object;
    Snapshot *snapshot=new Snapshot;
    memset(snapshot,0,0x2c);
    if(StreamRead(file,snapshot,0x2c)!=0x2c) {delete snapshot;return -1;}
    snapshot->next=0;
    /* Equipment transfer record: prefix and stride remain provisional. Only
     * the 23 slots at 0x18 and the type byte at 0x30 are evidenced here. */
    struct EquipmentTransfer { U8 unknown[24],slots[24],type; } equipment;
    memcpy(equipment.slots,snapshot->slots,23);
    equipment.type=snapshot->type;
    if(s->level<equipment.type) {delete snapshot;return equipment.type;}
    if(!s->snapshots) s->snapshots=snapshot;
    else {
        Snapshot *p=s->snapshots;
        while(p->next) p=p->next;
        p->next=snapshot;
    }
    return 0;
}
