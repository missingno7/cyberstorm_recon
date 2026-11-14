#include "actor.h"
typedef signed char Bool8;



















extern int actorCount;
extern ActorRecord *actorTable;
extern int *actorIndexTable; extern int actorTransferMode; extern int leaderPlayer; extern int AnimActionHasMetadata(void *); extern "C" int LogicCopyPoint(void *, void *);
extern "C" int ActorCleanup(void *);
extern "C" int StreamRead(void *, void *, unsigned int);
extern "C" int StreamWrite(void *, void *, unsigned int);
extern "C" int ActorClose(void *);
extern "C" int ActorMap(int);
extern "C" void AssertFail(const char *, int, const char *);
extern "C" void *memset(void *, int, unsigned int); extern "C" void *Allocate(unsigned int); extern "C" void AnimActionFreeBlock(void *); extern "C" void AnimActionCreateBlock(void *); extern "C" void *ActorListCopyConstruct(void *); extern "C" void *ActorStringCreate(const void *, void *); extern "C" void *ActorStringClone(const void *, void *); extern const char actorStringDescriptor[];
#define ASSERT(e) if (!(e)) AssertFail(__FILE__, __LINE__, 0)
extern const char actorListDescriptor[];

struct ActorPilot { char bytes[0x7e]; }; /* pilot record, 0x7e bytes (StreamRead size) */ struct ActorListGlobalObject : ActorPilot {

    ActorListGlobalObject(const void *descriptor);
    ~ActorListGlobalObject();
};
ActorListGlobalObject actorListGlobalObject(actorListDescriptor);

int ActorListConstruct()
{
    actorCount = 0;
    actorTable = new ActorRecord[400];

    /* ASSERT_LINE_47 */ ASSERT(actorTable != 0);
    memset(actorTable, 0, 0xed80);
    return actorTable != 0;
}


extern "C" void *ActorIdentity(void *, int); struct ProfileManager { ActorUnit *Mission(int); }; extern ProfileManager profileManager; extern "C" ActorPilot *f_0043c6ed(void *,void *); struct ActorListObject : ActorPilot { ActorListObject(); };

int ActorListDestroy()
{
    if (actorTable != 0) {
        for (int i=0;i<actorCount;i++) {
            ActorRecord *entry=actorTable+i;
            if (entry->unit && entry->source==0) delete (char *)entry->unit;
            if (entry->source<0) {
                if(entry->unit) delete (char *)entry->unit;
                if(entry->pilot) delete (ActorListGlobalObject *)entry->pilot;
            }
        }
        delete[] actorTable;
    }
    actorTable=0; actorCount=0;
    return 1;
}










int ActorListRead(void *stream)
{
    StreamWrite(stream, &actorCount, 4);
    int i = 0;
    while (i < actorCount) {
        ActorRecord *entry = actorTable + i;
        StreamWrite(stream, entry, sizeof(ActorRecord));
        if (entry->source <= -9999) {
            StreamWrite(stream, entry->unit, 0x10b);
            StreamWrite(stream, entry->pilot, 0x7e);
        } else if (entry->source == 0) {
            StreamWrite(stream, entry->unit, 0x10b);
        }
        if (entry->action.block != 0)
            StreamWrite(stream, entry->action.block, 0x494);
        ++i;
    }
    return 1;
}

int ActorListWrite(void *stream)
{
    StreamRead(stream, &actorCount, 4);
    int i;
    for(i=0;i<actorCount;i++) {
        ActorRecord *entry = actorTable + i;
        AnimActionFreeBlock(&entry->action);
        StreamRead(stream, entry, sizeof(ActorRecord));
        if (entry->source <= -9999) {
            entry->unit = new ActorUnit;
            entry->pilot = new ActorListObject;




























            /* ASSERT_LINE_140 */ ASSERT(entry->unit != 0 && entry->pilot != 0);
            StreamRead(stream, entry->unit, 0x10b);
            StreamRead(stream, entry->pilot, 0x7e);
        } else if (entry->source == 0) {
            entry->unit = new ActorUnit;




            /* ASSERT_LINE_149 */ ASSERT(entry->unit != 0);
            entry->pilot = &actorListGlobalObject; StreamRead(stream, entry->unit, 0x10b);
        } else {
            entry->unit = profileManager.Mission(entry->source)
                                            ;
            entry->pilot = f_0043c6ed(&profileManager, entry->unit);
        }
        if (entry->action.block != 0) {
            entry->action.block = 0;
            AnimActionCreateBlock(&entry->action);
            StreamRead(stream, entry->action.block, 0x494);
            entry->action.block->actor = entry;
        }
    }
    return 1;
}

ActorRecord *ActorGetByIndex(int index)
{
    if (index == 0)
        goto invalid;
    if (index > 0) {
        if (index > actorCount)
            goto invalid;
        return actorTable + (index - 1);
    }
invalid:
    return 0;
}

ActorRecord *ActorGetByUnit(int unit)
{
    if (unit) {
        int index=actorIndexTable[unit*2];
        if(index>0) return ActorGetByIndex(index);
    }
    return 0;
}












inline int ActorTransferEligible(ActorRecord *entry) { return entry && leaderPlayer==entry->Owner() && entry->active && !entry->hidden && AnimActionHasMetadata(&entry->action); } inline ActorRecord *ActorGetByIndexInline(int index) { if (index == 0) return 0; if (index > 0 && index <= actorCount) return actorTable + (index - 1); return 0; }
ActorRecord *ActorListTransfer(int requestedIndex)
{
    int selected=0;
    if(actorTransferMode && ActorTransferEligible(ActorGetByIndex(requestedIndex))) selected=requestedIndex;
    for(int i=1;!selected && i<=actorCount;i++) {
        if(ActorTransferEligible(ActorGetByIndex(i))) selected=i;
    }
    for(i=1;!selected && i<=actorCount;i++) {
        ActorRecord *entry=ActorGetByIndex(i);
        if(entry && leaderPlayer==entry->Owner()) selected=i;
    }









































    /* ASSERT_LINE_252 */ ASSERT(selected != 0);
    int eligibleCount=0;
    for(i=1;i<=actorCount;i++) {
        ActorRecord *entry=ActorGetByIndex(i);
        if(entry) {
            entry->relatedIndex=0;
            if(ActorTransferEligible(entry)) ++eligibleCount;
        }
    }
    if(eligibleCount<2) return ActorGetByIndex(selected);
    ActorRecord *head=0,*tail=0;
    for(;;) {
        ActorRecord *best=0;
        int bestScore;
        for(i=1;i<=actorCount;i++) {
            ActorRecord *entry=ActorGetByIndex(i);
            if(ActorTransferEligible(entry) && !entry->RelatedIndex()) {
                int score;
                if(entry==ActorGetByIndex(selected)) score=1;
                else { score=0; if(tail) score=-LogicCopyPoint(&tail->action.target,&entry->action.target); }
                if(!best || score>bestScore) { best=entry; bestScore=score; }
            }
        }
        if(!best) break;
        if(!head) head=best;
        if(tail) tail->relatedIndex=best->id;
        tail=best; best->relatedIndex=-1;
    }
    if(head && tail) tail->relatedIndex=head->id;
    return ActorGetByIndex(selected);
}

ActorRecord *ActorFindRelated(ActorRecord *actor)
{
    ActorRecord *found=0;
    ActorRecord *entry=ActorGetByIndex(actor->relatedIndex);
    while(entry) {
        if(entry->id==actor->id) break;
        if(entry->active && !entry->done && !entry->hidden) found=entry;
        entry=ActorGetByIndex(entry->relatedIndex);
    }
    return found ? found : entry;
}
ActorRecord *ActorFindRelatedOther(ActorRecord *actor)
{
    ActorRecord *entry=ActorGetByIndex(actor->relatedIndex);
    while(entry) {
        if(entry->id==actor->id) break;
        if(entry->active && !entry->done && !entry->hidden) break;
        entry=ActorGetByIndex(entry->relatedIndex);
    }
    return entry;
}

ActorRecord *ActorFindFree()
{
    ActorRecord *entry = actorTable;
    int i = 0;
    for (; i < actorCount; ++i, ++entry) {
        if (entry->id != 0 && !entry->hidden)
            return entry;
    }
    return 0;
}

ActorRecord *ActorFindByOwner(int owner)
{
    ActorRecord *entry=actorTable;
    for(int i=0;i<actorCount;i++,entry++) {
        if(entry->id && entry->owner==owner && entry->active && !entry->hidden) return entry;
    }
    return 0;
}
ActorRecord *ActorFindNext(ActorRecord *actor)
{
    if(!actor || actor->id>=actorCount) return 0;
    actor++;
    for(int index=actor->id;index<=actorCount;index++,actor++) {
        if(actor->id && !actor->hidden) return actor;
    }
    return 0;
}
ActorRecord *ActorFindOwnerNext(ActorRecord *actor,int owner)
{
    if(!actor || actor->id>=actorCount) return 0;
    actor++;
    for(int index=actor->id;index<=actorCount;index++,actor++) {
        if(actor->id && owner==actor->owner && actor->active && !actor->hidden) return actor;
    }
    return 0;
}
