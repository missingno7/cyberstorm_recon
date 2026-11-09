typedef signed char Bool8;
struct ActorRecord {
    ActorRecord() : id(0) {}
    int id;
    int unused;
    ActorRecord *owner;
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
extern int *actorIndexTable; extern int actorTransferMode; extern int actorOwnerId; extern "C" int ActorTransferCheck(void *); extern "C" int ActorTransferScore(void *, void *);
extern "C" int ActorCleanup(void *);
extern "C" int StreamRead(void *, void *, unsigned int);
extern "C" int StreamWrite(void *, void *, unsigned int);
extern "C" int ActorClose(void *);
extern "C" int ActorMap(int);
extern "C" void AssertFail(const char *, int, const char *);
extern "C" void *memset(void *, int, unsigned int); extern "C" void *Allocate(unsigned int); extern "C" void ActorListStateInit(void *); extern "C" void ActorListStatePrepare(void *); extern "C" void *ActorListCopyConstruct(void *); extern "C" void *ActorStringCreate(const void *, void *); extern "C" void *ActorStringClone(const void *, void *); extern const char actorStringDescriptor[];
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
extern "C" void *ActorIdentity(void *, int);

int ActorListDestroy()
{
    if (actorTable != 0) {
        register int count = actorCount;
        int i = 0;
        if (count > 0) {
            do {
                ActorRecord *entry = actorTable + i;
                if (entry->at18 && entry->at10 == 0)
                    delete (char *)entry->at18;
                if (0 > *(volatile int *)&entry->at10) {
                    if (entry->at18)
                        delete (char *)entry->at18;
                    if (entry->at1c)
                        delete (char *)ActorIdentity(entry->at1c, 0);
                }
                ++i;
            } while (i < actorCount);
        }
        MemoryRelease(actorTable);
    }
    actorTable = 0;
    actorCount = 0;
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
    register int i = 0;
    while (i < actorCount) {
        ActorRecord *entry = actorTable + i;
        ActorListStateInit((char *)entry + 0x20);
        StreamRead(stream, entry, sizeof(ActorRecord));
        if (entry->at10 <= -9999) {
            entry->at18 = Allocate(0x10b);
            void *second = Allocate(0x7e);
            if (second != 0)
                second = ActorListCopyConstruct(second);
            entry->at1c = second;

























            /* ASSERT_LINE_140 */ ASSERT(entry->at18 != 0 && entry->at1c != 0);
            StreamRead(stream, entry->at18, 0x10b);
            StreamRead(stream, entry->at1c, 0x7e);
        } else if (entry->at10 == 0) {
            entry->at18 = Allocate(0x10b);
            entry->at1c = &actorListGlobalObject;



            /* ASSERT_LINE_149 */ ASSERT(entry->at18 != 0);
            StreamRead(stream, entry->at18, 0x10b);
        } else {
            entry->at18 = ActorStringCreate(actorStringDescriptor,
                                            (void *)(unsigned long)entry->at10);
            entry->at1c = ActorStringClone(actorStringDescriptor, entry->at18);
        }
        if (entry->at52 != 0) {
            entry->at52 = 0;
            ActorListStatePrepare((char *)entry + 0x20);
            StreamRead(stream, (void *)(unsigned long)entry->at52, 0x494);
            *(void **)((char *)(unsigned long)entry->at52 + 0x490) = entry;
        }
        ++i;
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
    int index;
    if (unit == 0)
        goto invalidUnit;
    index = actorIndexTable[unit * 2];
    if (index <= 0)
        goto invalidUnit;
    if (index == 0)
        goto invalidIndex;
    if (index <= 0)
        goto invalidIndex;
    if (index > actorCount)
        goto invalidIndex;
    return (ActorRecord *)((char *)actorTable + (index - 1) * sizeof(ActorRecord));
invalidIndex:
    return (ActorRecord *)(unsigned long)(index ^ index);
invalidUnit:
    return 0;
}
inline ActorRecord *ActorGetByIndexInline(int index) { if (index == 0) return 0; if (index > 0 && index <= actorCount) return actorTable + (index - 1); return 0; }
ActorRecord *ActorListTransfer(int requestedIndex)
{
    int selected = 0;
    if (actorTransferMode != 0) {
        ActorRecord *entry = ActorGetByIndexInline(requestedIndex);
        if (entry != 0 && entry->owner == (ActorRecord *)(unsigned long)actorOwnerId &&
            entry->flag88 != 0 && entry->flag89 == 0 &&
            ActorTransferCheck((char *)entry + 0x20))
            selected = requestedIndex;
    }

    if (selected == 0) {
        int i = 1;
        while (i <= actorCount) {
            ActorRecord *entry = ActorGetByIndexInline(i);
            if (entry != 0 && entry->owner == (ActorRecord *)(unsigned long)actorOwnerId &&
                entry->flag88 != 0 && entry->flag89 == 0 &&
                ActorTransferCheck((char *)entry + 0x20)) {
                selected = i;
                break;
            }
            ++i;
        }
    }

    if (selected == 0) {
        int i = 1;
        while (i <= actorCount) {
            ActorRecord *entry = ActorGetByIndexInline(i);
            if (entry != 0 && entry->owner == (ActorRecord *)(unsigned long)actorOwnerId) {
                selected = i;
                break;
            }
            ++i;
        }
    }















    /* ASSERT_LINE_252 */ ASSERT(selected != 0);
    int eligibleCount = 0;
    int i = 1;
    while (i <= actorCount) {
        ActorRecord *entry = ActorGetByIndexInline(i);
        if (entry != 0) {
            entry->relatedIndex = 0;
            if (entry->owner == (ActorRecord *)(unsigned long)actorOwnerId &&
                entry->flag88 != 0 && entry->flag89 == 0 &&
                ActorTransferCheck((char *)entry + 0x20))
                ++eligibleCount;
        }
        ++i;
    }
    if (eligibleCount < 2)
        return ActorGetByIndexInline(selected);

    ActorRecord *root = ActorGetByIndexInline(selected);
    ActorRecord *head = 0;
    ActorRecord *tail = 0;
    ActorRecord *anchor = root;
    for (;;) {
        ActorRecord *best = 0;
        int bestScore = 0;
        i = 1;
        while (i <= actorCount) {
            ActorRecord *entry = ActorGetByIndexInline(i);
            if (entry != 0 && entry->owner == (ActorRecord *)(unsigned long)actorOwnerId &&
                entry->flag88 != 0 && entry->flag89 == 0 &&
                ActorTransferCheck((char *)entry + 0x20) && entry->relatedIndex == 0) {
                int score;
                if (entry == anchor) {
                    score = 1;
                } else if (anchor != 0) {
                    score = -ActorTransferScore((char *)entry + 0x3a,
                                                (char *)anchor + 0x3a);
                } else {
                    score = 0;
                }
                if (best == 0 || score > bestScore) {
                    best = entry;
                    bestScore = score;
                }
            }
            ++i;
        }
        if (best == 0)
            break;
        if (head == 0)
            head = best;
        if (tail != 0)
            tail->relatedIndex = best->id;
        tail = best;
        best->relatedIndex = -1;
        anchor = best;
    }
    if (head != 0 && tail != 0)
        tail->relatedIndex = head->id;
    return ActorGetByIndexInline(selected);
}

ActorRecord *ActorFindRelated(ActorRecord *actor)
{
    int index = actor->relatedIndex;
    ActorRecord *entry = 0;
    ActorRecord *found = 0;
    if (index != 0) {
        if (index > 0) {
            if (index <= actorCount)
                entry = actorTable + (index - 1);
        }
    }
    while (entry != 0) {
        if (actor->id == entry->id)
            break;
        if (entry->flag88 != 0 && entry->flag87 == 0 && entry->flag89 == 0)
            found = entry;
        index = entry->relatedIndex;
        if (index == 0 || index <= 0 || index > actorCount)
            entry = 0;
        else
            entry = (ActorRecord *)((char *)actorTable + (index - 1) * 0x98);
    }
    if (found != 0)
        return found;
    return entry;
}

ActorRecord *ActorFindRelatedOther(ActorRecord *actor)
{
    int index = actor->relatedIndex;
    ActorRecord *entry = 0;
    if (index != 0) {
        if (index > 0) {
            if (index <= actorCount)
                entry = (ActorRecord *)((char *)actorTable + (index - 1) * 0x98);
        }
    }
    while (entry != 0) {
        if (actor->id == entry->id)
            return entry;
        if (entry->flag88 == 0)
            goto nextRelated;
        if (entry->flag87 != 0)
            goto nextRelated;
        if (entry->flag89 == 0)
            return entry;
    nextRelated:
        index = entry->relatedIndex;
        if (index <= 0 || index > actorCount)
            return 0;
        entry = actorTable + (index - 1);
    }
    return entry;
}

ActorRecord *ActorFindFree()
{
    ActorRecord *entry = actorTable;
    int i = 0;
    for (; i < actorCount; ++i, ++entry) {
        if (entry->id != 0 && entry->flag89 == 0)
            return entry;
    }
    return 0;
}

ActorRecord *ActorFindByOwner(register ActorRecord *owner)
{
    register int firstCount = actorCount;
    if (firstCount <= 0)
        return 0;
    ActorRecord *entry = actorTable;
    int i = 0;
    int limit = *(volatile int *)&actorCount;
    do {
        if (entry->id == 0)
            goto next;
        if (entry->owner != owner)
            goto next;
        if (entry->flag88 == 0)
            goto next;
        if (entry->flag89 != 0)
            goto next;
        return entry;
    next:
        ++i;
        ++entry;
    } while (i < limit);
    return 0;
}

ActorRecord *ActorFindNext(ActorRecord *actor)
{
    int initialCount;
    ActorRecord *entry;
    int index;
    if (actor == 0)
        return 0;
    initialCount = actorCount;
    if (initialCount <= actor->id)
        return 0;
    entry = actor + 1;
    index = entry->id;
    if (index > initialCount)
        return 0;
    int limit = actorCount;
    do {
        if (entry->id != 0 && entry->flag89 == 0)
            return entry;
        ++index;
        ++entry;
    } while (index <= limit);
    return 0;
}

ActorRecord *ActorFindOwnerNext(ActorRecord *actor, ActorRecord *owner)
{
    register ActorRecord *ownerPtr = owner;
    register int initialCount;
    ActorRecord *entry;
    int index;
    if (actor == 0)
        return 0;
    initialCount = actorCount;
    if (initialCount <= actor->id)
        return 0;
    entry = actor + 1;
    index = entry->id;
    if (index > initialCount)
        return 0;
    register int limit = *(volatile int *)&actorCount;
    do {
        if (entry->id == 0)
            goto next;
        if (entry->owner != ownerPtr)
            goto next;
        if (entry->flag88 == 0)
            goto next;
        if (entry->flag89 != 0)
            goto next;
        return entry;
next:
        ++index;
        ++entry;
    } while (index <= limit);
    return 0;
}

ActorRecord *ActorCloseRecord(ActorRecord *actor)
{
    return (ActorRecord *)((char *)ActorClose((char *)actor + 0x20) - 0x20);
}
