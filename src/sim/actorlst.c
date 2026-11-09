typedef signed char Bool8;
struct ActorRecord {
    ActorRecord() : id(0) {}
    int id;
    int unused;
    int owner; int Owner() const {return owner;} int RelatedIndex() const {return relatedIndex;}
    int at0c;
    int at10;
    int at14;
    void *at18;
    void *at1c;
    char pad20[0x32];
    int at52;
    char pad56[0x31];
    Bool8 flag87;
    Bool8 flag88;
    Bool8 flag89;
    char pad8a[5];
    int relatedIndex;
    char rest93[5];
};
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

struct ActorListGlobalObject {
    char bytes[0x7e];
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

extern "C" void MemoryRelease(void *);
extern "C" void *ActorIdentity(void *, int); struct ActorUnitBlob { char bytes[0x10b]; }; struct ProfileManager { void *Mission(int); }; extern ProfileManager profileManager; extern "C" void *f_0043c6ed(void *,void *); struct ActorListObject { char bytes[0x7e]; ActorListObject(); };

int ActorListDestroy()
{
    if (actorTable != 0) {
        for (int i=0;i<actorCount;i++) {
            ActorRecord *entry=actorTable+i;
            if (entry->at18 && entry->at10==0) delete (char *)entry->at18;
            if (entry->at10<0) {
                if(entry->at18) delete (char *)entry->at18;
                if(entry->at1c) delete (ActorListGlobalObject *)entry->at1c;
            }
        }
        MemoryRelease(actorTable);
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
        if (entry->at10 <= -9999) {
            StreamWrite(stream, entry->at18, 0x10b);
            StreamWrite(stream, entry->at1c, 0x7e);
        } else if (entry->at10 == 0) {
            StreamWrite(stream, entry->at18, 0x10b);
        }
        if (entry->at52 != 0)
            StreamWrite(stream, (void *)(unsigned long)entry->at52, 0x494);
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
        AnimActionFreeBlock((char *)entry + 0x20);
        StreamRead(stream, entry, sizeof(ActorRecord));
        if (entry->at10 <= -9999) {
            entry->at18 = new ActorUnitBlob;
            entry->at1c = new ActorListObject;




























            /* ASSERT_LINE_140 */ ASSERT(entry->at18 != 0 && entry->at1c != 0);
            StreamRead(stream, entry->at18, 0x10b);
            StreamRead(stream, entry->at1c, 0x7e);
        } else if (entry->at10 == 0) {
            entry->at18 = new ActorUnitBlob;




            /* ASSERT_LINE_149 */ ASSERT(entry->at18 != 0);
            entry->at1c = &actorListGlobalObject; StreamRead(stream, entry->at18, 0x10b);
        } else {
            entry->at18 = profileManager.Mission(entry->at10)
                                            ;
            entry->at1c = f_0043c6ed(&profileManager, entry->at18);
        }
        if (entry->at52 != 0) {
            entry->at52 = 0;
            AnimActionCreateBlock((char *)entry + 0x20);
            StreamRead(stream, (void *)(unsigned long)entry->at52, 0x494);
            *(void **)((char *)(unsigned long)entry->at52 + 0x490) = entry;
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












inline int ActorTransferEligible(ActorRecord *entry) { return entry && leaderPlayer==entry->Owner() && entry->flag88 && !entry->flag89 && AnimActionHasMetadata((char *)entry+0x20); } inline ActorRecord *ActorGetByIndexInline(int index) { if (index == 0) return 0; if (index > 0 && index <= actorCount) return actorTable + (index - 1); return 0; }
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
                else { score=0; if(tail) score=-LogicCopyPoint((char *)tail+0x3a,(char *)entry+0x3a); }
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
        if(entry->flag88 && !entry->flag87 && !entry->flag89) found=entry;
        entry=ActorGetByIndex(entry->relatedIndex);
    }
    return found ? found : entry;
}
ActorRecord *ActorFindRelatedOther(ActorRecord *actor)
{
    ActorRecord *entry=ActorGetByIndex(actor->relatedIndex);
    while(entry) {
        if(entry->id==actor->id) break;
        if(entry->flag88 && !entry->flag87 && !entry->flag89) break;
        entry=ActorGetByIndex(entry->relatedIndex);
    }
    return entry;
}

ActorRecord *ActorFindFree()
{
    ActorRecord *entry = actorTable;
    int i = 0;
    for (; i < actorCount; ++i, ++entry) {
        if (entry->id != 0 && !entry->flag89)
            return entry;
    }
    return 0;
}

ActorRecord *ActorFindByOwner(int owner)
{
    ActorRecord *entry=actorTable;
    for(int i=0;i<actorCount;i++,entry++) {
        if(entry->id && entry->owner==owner && entry->flag88 && !entry->flag89) return entry;
    }
    return 0;
}
ActorRecord *ActorFindNext(ActorRecord *actor)
{
    if(!actor || actor->id>=actorCount) return 0;
    actor++;
    for(int index=actor->id;index<=actorCount;index++,actor++) {
        if(actor->id && !actor->flag89) return actor;
    }
    return 0;
}
ActorRecord *ActorFindOwnerNext(ActorRecord *actor,int owner)
{
    if(!actor || actor->id>=actorCount) return 0;
    actor++;
    for(int index=actor->id;index<=actorCount;index++,actor++) {
        if(actor->id && owner==actor->owner && actor->flag88 && !actor->flag89) return actor;
    }
    return 0;
}
ActorRecord *ActorCloseRecord(ActorRecord *actor)
{
    return (ActorRecord *)((char *)ActorClose((char *)actor + 0x20) - 0x20);
}



