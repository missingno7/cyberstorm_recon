extern "C" unsigned long AnimClockNow(void);

void AnimActionClear(void *action)
{
    unsigned char *p = (unsigned char *)action;
    p[4] = 0;
    *(volatile unsigned char *)(p + 5) = 0;
}

void AnimActionInitialize(void *action, unsigned char type, unsigned short duration,
                          unsigned char rate, unsigned char initialFlags)
{
    unsigned char *p = (unsigned char *)action;
    unsigned char t = type;
    union FlagWord { unsigned short word; struct { unsigned char low; unsigned char high; } bytes; } flags;
    p[8] = 0;
    p[4] = t;
    *(volatile unsigned short *)(p + 12) = duration;
    p[7] = rate;
    p[10] = initialFlags;
    *(volatile unsigned char *)(p + 5) = 0;
    *(volatile unsigned long *)(p + 14) = 0;
    flags.bytes.low = p[10];
    p[9] = 0;
    flags.bytes.low &= 0x70;
    p[10] = flags.bytes.low;
    *(volatile unsigned char *)(p + 11) = 1;
    flags.bytes.high = flags.bytes.low;
    flags.bytes.high |= 0x80;
    p[10] = flags.bytes.high;
}

void WorldActionNotify(void *action, unsigned char state)
{
    unsigned char *p = (unsigned char *)action;
    union FlagWord { unsigned short word; struct { unsigned char low; unsigned char high; } bytes; } flags;
    p[8] = 0;
    p[9] = 0;
    p[5] = state;
    flags.bytes.low = p[10];
    *(volatile unsigned long *)(p + 14) = 0;
    flags.bytes.low &= 0x70;
    p[10] = flags.bytes.low;
    *(volatile unsigned char *)(p + 11) = 1;
    flags.bytes.high = flags.bytes.low;
    flags.bytes.high |= 0x80;
    p[10] = flags.bytes.high;
}

int AnimActionIsSet(void *action)
{
    unsigned long value = 0;
    unsigned char *p = (unsigned char *)action;
    value = p[10];
    return value & 1;
}

int AnimActionAdvance(void *action)
{
    unsigned char *p = (unsigned char *)action;
    unsigned long now = AnimClockNow();
    unsigned long deadline = *(volatile unsigned long *)(p + 14);
    if (now >= deadline && p[7]) {
        *(volatile unsigned long *)(p + 14) = now + 1000 / (int)(unsigned char)p[7];
        return 1;
    }
    return 0;
}

int AnimActionStep(unsigned char *p)
{
    unsigned long now = AnimClockNow();
    unsigned long deadline = *(volatile unsigned long *)(p + 14);
    int ready = 0;
    if (now >= deadline && p[7] != 0) {
        *(volatile unsigned long *)(p + 14) = now + 1000 / (int)(unsigned char)p[7];
        ready = 1;
    }
    if (ready && p[9] != 0 && p[5] != 0) {
        unsigned char flags = p[10];
        if ((flags & 0x20) != 0) {
            unsigned char frame = (unsigned char)(p[8] + 1);
            p[8] = frame;
            if (p[9] <= frame)
                p[8] = 0;
        } else {
            unsigned char frame = (unsigned char)(p[8] + 1);
            p[8] = frame;
            if (p[9] <= frame) {
                p[8] = (unsigned char)(p[9] - 1);
                p[10] |= 1;
                p[10] = (p[10] & 0x7f) | 1;
            }
        }
        p[10] &= 0xfb;
    }
    return 0;
}

struct AnimPair { unsigned long first; unsigned long second; };

void ActorClose(void *action)
{
    volatile unsigned char *p = (volatile unsigned char *)action;
    AnimPair pair;
    unsigned char bits;
    p[4] = 0;
    p[5] = 0;
    pair.first = 0;
    pair.second = 0;
    *(AnimPair *)(p + 0x12) = pair;
    p[8] = 0;
    p[9] = 0;
    p[11] = 1;
    p[0x36] = 0;
    *(volatile unsigned long *)(p + 0x26) = 0;
    p[7] = 50;
    p[10] = 0;
    p[8] = 0;
    p[9] = 0;
    p[11] = 1;
    *(volatile unsigned short *)(p + 0xc) = 0;
    *(volatile unsigned long *)(p + 0xe) = 0;
    p[4] = 1;
    p[5] = 0;
    *(volatile unsigned long *)(p + 0x32) = 0;
    bits = p[10] & 0x70;
    p[10] = bits;
    *(volatile unsigned long *)(p + 0xe) = 0;
    p[10] = bits | 0x80;
    p[5] = 1;
}

extern unsigned long *animResourcePointerTable;
extern unsigned long animReleaseSentinel;
extern unsigned char animFallbackName[];
extern "C" void AnimReleaseResource(unsigned long name, unsigned long id,
                                    unsigned long kind);

unsigned long AnimActionRelease(void *action)
{
    unsigned long id = *(unsigned long *)((char *)action + 12);
    if (id != 0) {
        AnimReleaseResource((unsigned long)animFallbackName, id, 3);
        unsigned long offset = id * 8;
        unsigned long *table = animResourcePointerTable;
        unsigned long sentinel = animReleaseSentinel;
        *(unsigned long *)((char *)table + offset) = sentinel;
    }
    return 0;
}

extern "C" void AnimResourceFind(void *list, unsigned char *name, int count);
extern "C" void AnimResourceUpdate(void *list, int index);
extern "C" void AnimResourceIterate(void *list, void (*callback)(void *));
extern int animationMetaAt0cbc[];

void AnimActionAssign(void *unused, unsigned long value, int index, int count)
{
    int resourceList[10];
    if (count > 1) {
        animReleaseSentinel = value;
        AnimResourceFind(resourceList, animFallbackName, count - 1);
        AnimResourceUpdate(resourceList, index);
        AnimResourceIterate(resourceList, (void (*)(void *))AnimActionRelease);
        return;
    }
    *(unsigned long *)((char *)animResourcePointerTable + index * 8) = value;
}

void AnimActionClearGroup(void *action)
{
    unsigned char *p = (unsigned char *)action;
    int count = animationMetaAt0cbc[*(unsigned short *)(p + 12) * 15];
    int index = *(int *)(p + 0x22);
    int resourceList[10];
    if (count > 1) {
        animReleaseSentinel = 0;
        AnimResourceFind(resourceList, animFallbackName, count - 1);
        AnimResourceUpdate(resourceList, index);
        AnimResourceIterate(resourceList, (void (*)(void *))AnimActionRelease);
        return;
    }
    unsigned long *table = animResourcePointerTable;
    table[index * 2] = 0;
}

extern "C" int AnimResourceResolve(unsigned long name, int *entry);
extern "C" void AnimResourceLoadEntry(unsigned long name, unsigned long *entry,
                                       int index);

void AnimActionReplace(void *action, unsigned long value)
{
    unsigned char *p = (unsigned char *)action;
    int count = animationMetaAt0cbc[*(unsigned short *)(p + 12) * 15];
    int index = *(int *)(p + 0x22);
    int resourceListA[10];
    int resourceListB[10];
    if (count < 2) {
        animResourcePointerTable[index * 2] = 0;
    } else {
        animReleaseSentinel = 0;
        AnimResourceFind(resourceListB, animFallbackName, count - 1);
        AnimResourceUpdate(resourceListB, index);
        AnimResourceIterate(resourceListB, (void (*)(void *))AnimActionRelease);
    }
    index = AnimResourceResolve((unsigned long)animFallbackName,
                                (int *)(p + 0x12));
    *(int *)(p + 0x22) = index;
    AnimResourceLoadEntry((unsigned long)animFallbackName,
                          (unsigned long *)(p + 0x1a), index);
    index = *(int *)(p + 0x22);
    if (count < 2) {
        animResourcePointerTable[index * 2] = value;
    } else {
        animReleaseSentinel = value;
        AnimResourceFind(resourceListA, animFallbackName, count - 1);
        AnimResourceUpdate(resourceListA, index);
        AnimResourceIterate(resourceListA, (void (*)(void *))AnimActionRelease);
    }
}

extern int animationMetaAt0cc0[];
extern int animationMetaAt0cbc[];

int AnimActionHasMetadata(void *action)
{
    return animationMetaAt0cc0[*(unsigned short *)((char *)action + 12) * 15] != 0;
}

int AnimActionGetMetadata(void *action)
{
    return animationMetaAt0cbc[*(unsigned short *)((char *)action + 12) * 15];
}

void AnimActionMarkReady(void *action)
{
    *(unsigned char *)((char *)action + 11) = 1;
}

void AnimActionCopyState(void *action, void *output)
{
    struct Pair { int x; int y; };
    struct Pair *a = (struct Pair *)((char *)action + 0x12);
    struct Pair *b = (struct Pair *)((char *)output + 5);
    b[0] = a[0];
    b[1] = a[1];
    *(int *)((char *)output + 0x15) = *(int *)((char *)action + 0x22);
    *(int *)((char *)output + 0x19) = *(int *)((char *)action + 0x26);
}

void AnimActionConfigure(void *action, int frame, int index)
{
    unsigned char *p = (unsigned char *)action;
    unsigned short animationId = *(unsigned short *)(p + 12);
    if (p[5] == 1 && animationMetaAt0cc0[animationId * 15] != 0) {
        p[8] = 0;
        p[9] = 0;
        p[5] = 0x0c;
        unsigned char flags = p[10];
        *(volatile unsigned long *)(p + 14) = 0;
        p[10] = flags & 0x70;
        *(volatile unsigned char *)(p + 11) = 1;
        p[10] = (flags & 0x70) | 0x80;
        p[8] = (index < 0) ? frame : 0;
        *(char *)(p + 0x39) = (char)index;
        *(unsigned char *)(p + 0x38) = frame;
    }
}

void AnimActionFreeBlock(void *action)
{
    register unsigned char *p = (unsigned char *)action;
    unsigned char *resource = *(unsigned char **)(p + 0x32);
    if (resource != 0) {
        delete resource;
        *(unsigned long *)(p + 0x32) = 0;
    }
}

extern unsigned char animationMetadataRecords[];
extern "C" int AnimResourceIndex(unsigned long name, int index, int variant);
extern "C" void AnimResourceSet(unsigned long name, int index, int value,
                                int variant, int flags, int duration);

void AnimActionPublish(void *action)
{
    unsigned char *p = (unsigned char *)action;
    unsigned long record = *(unsigned short *)(p + 12) * 60;
    if (animationMetadataRecords[record] != 0) {
        int index = *(int *)(p + 0x22);
        int variant = (signed char)animationMetadataRecords[record + 1];
        int value = AnimResourceIndex((unsigned long)animFallbackName, index, variant);
        AnimResourceSet((unsigned long)animFallbackName, index, value, variant,
                        (signed char)animationMetadataRecords[record + 2], 100);
    }
}

extern void Crc16(unsigned short *checksum, void *data, int length);

void AnimActionChecksum(void *action, unsigned short *checksum)
{
    Crc16(checksum, (char *)action + 0x12, 8);
    Crc16(checksum, (char *)action + 0x1a, 8);
    Crc16(checksum, (char *)action + 0x22, 4);
    Crc16(checksum, (char *)action + 0x26, 4);
}

int AnimActionEqual(void *action, void *state)
{
    unsigned char *p = (unsigned char *)action;
    unsigned char *q = (unsigned char *)state;
    int changedFirst;
    int changedSecond;
    int equal = 1;
    if (*(int *)(q + 5) == *(int *)(p + 0x12) &&
        *(int *)(q + 9) == *(int *)(p + 0x16))
        changedFirst = 0;
    else
        changedFirst = 1;
    if (changedFirst)
        equal = 0;
    if (*(int *)(q + 0x0d) == *(int *)(p + 0x1a) &&
        *(int *)(q + 0x11) == *(int *)(p + 0x1e))
        changedSecond = 0;
    else
        changedSecond = 1;
    if (changedSecond)
        equal = 0;
    if (*(int *)(q + 0x15) != *(int *)(p + 0x22))
        equal = 0;
    if (*(int *)(q + 0x19) != *(int *)(p + 0x26))
        equal = 0;
    return equal;
}

void AnimActionCopyChangedFields(void *action, void *state)
{
    unsigned char *p = (unsigned char *)action;
    unsigned char *q = (unsigned char *)state;
    if (*(int *)(q + 5) != *(int *)(p + 0x12) ||
        *(int *)(q + 9) != *(int *)(p + 0x16)) {
        *(int *)(p + 0x12) = *(int *)(q + 5);
        *(int *)(p + 0x16) = *(int *)(q + 9);
    }
    if (*(int *)(q + 0x0d) != *(int *)(p + 0x1a) ||
        *(int *)(q + 0x11) != *(int *)(p + 0x1e)) {
        *(int *)(p + 0x1a) = *(int *)(q + 0x0d);
        *(int *)(p + 0x1e) = *(int *)(q + 0x11);
    }
    if (*(int *)(q + 0x15) != *(int *)(p + 0x22))
        *(int *)(p + 0x22) = *(int *)(q + 0x15);
    if (*(int *)(q + 0x19) != *(int *)(p + 0x26))
        *(int *)(p + 0x26) = *(int *)(q + 0x19);
}

