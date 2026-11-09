#include <string.h>
#include <stdlib.h>
char animSourcePath[] = "sim\\anim.c";
extern void AssertFail(const char *file, int line, const char *message);
struct AnimBlockAllocation { unsigned char bytes[0x494]; };
extern "C" unsigned int TimerNow(void);
struct AnimPair { unsigned long first; unsigned long second; void Copy(const AnimPair &p) { *this = p; } int Equal(const AnimPair &p) const { return first == p.first && second == p.second; } };
extern int *actorIndexTable; static inline unsigned long *AnimResourceSlot(int index) { return (unsigned long *)((char *)actorIndexTable+index*8); }
extern unsigned long animReleaseSentinel;
extern const char fallbackName[];
extern int animationMetaAt0cc0[];
extern "C" void AnimReleaseResource(unsigned long name, unsigned long id,
                                    unsigned long kind);
extern int BestLocInitialize(void *list, const char *name, int count);
extern int BestLocSetCandidate(void *list, void *candidate);
extern void BestLocVisit(void *list, int (*callback)(void *));
extern int animationMetaAt0cbc[];
extern "C" int AnimResourceResolve(unsigned long name, int *entry);
extern "C" void AnimResourceLoadEntry(unsigned long name, unsigned long *entry,
                                       int index);
extern int f_0042c130(void *actor, void *from, void *to);
extern int f_0047f9d9(int angle);
extern int f_0042afb0(unsigned long name, void *point);
extern int *AnimBitmapFrame(void *action);
extern int animationMetaAt0cc4[];
extern int animationMetaAt0cc8[];
extern unsigned char *targetDefinitionTable;
extern long GridClamp(unsigned long);
extern int ViewScale(void *actor, int value);
struct ExploPoint { int x; int y; };
extern long GridPointToIndex(const char *name, ExploPoint *point);
extern void ViewToScreen(void *view, ExploPoint *out, ExploPoint *in);
extern int PointAngle(ExploPoint *from, ExploPoint *to);
extern unsigned int ISqrt(unsigned int square);
extern int StateInvoke(int state, void *first, void *second);
extern int *ActorGetByUnit(int unit);
extern void f_0043a294(int state, int value);
extern void f_0044e940(unsigned long resource, int value);
extern int ModeCheck(unsigned long resource);
extern void f_004731e8(void *name, int *actor, int target, int value);
extern int f_00455c40(int *actor);
extern int f_0047723b(const char *text);
extern void f_00477295(const char *text);
extern int animationModeRate[];
extern int modeSwitch;
extern int g_004d2dee;
extern int g_004d2df2;
extern int g_004d271a;
extern unsigned char *simActorVisibility;
extern int g_004c7e50;
extern char modeLogName[], modeCheckName[];
extern char targetBriefResource[];
struct StrPool { char *GetString(long id, int flags); };
extern "C" void TargetObjectInit(const char *name, void *position,
                                  int objectId);
extern unsigned long AnimActionCommitTrajectory(void *action);
extern void f_00455890(int actor, int direction);
extern void f_00462920(unsigned long resource, int *actor, int current, int previous);
extern unsigned char animationStatusFlags[];
extern signed char animationModeStep[];
extern "C" void BestLocSetData(void *name, void *point, void *index);
extern int BestLocSetup(void *entries, int id, int reset);
extern int f_0042d2b0(void *entries, int id, void *buffer, int size);
extern void f_004324d0(unsigned long name, int index, int id, void *buffer, int size);
extern void f_0042d370(void *entries, int id);
extern void TargetInitAt(unsigned long name, unsigned short *result, int actor, int id);
extern int StateInvoke(int state, void *first, void *second);
extern unsigned char bestLocEntries[];
extern int bestLocCursor;
extern void *g4d2498;
extern int gLocalActorKind;
extern unsigned char *simVisibleTypes;
extern int animationMetaAt0cbc[];
extern unsigned long animationKindTable[];
extern "C" int f_00434a20(unsigned long name, int index, int variant);
extern "C" void f_00434bd0(unsigned long name, int index, int value,
                           int variant, int flags, int duration);
extern void Crc16(unsigned short *checksum, void *data, int length);
extern void AssertFail(const char *file, int line, const char *message);
void AnimActionClear(void *action);
void AnimActionInitialize(void *action, unsigned char type, unsigned short duration,
                          unsigned char rate, unsigned char initialFlags);
void WorldActionNotify(void *action, unsigned char state);
int AnimActionIsSet(void *action);
int AnimActionAdvance(void *action);
int AnimActionStep(unsigned char *p);
void ActorClose(void *action);
unsigned long AnimActionRelease(void *action);
void AnimActionAssign(void *unused, unsigned long value, int index, int count);
void AnimActionClearGroup(void *action);
void AnimActionReplace(unsigned char *action, unsigned long value);
void AnimActionUpdateTrajectory(int *action, int *actor);
void AnimActionInterpolateDrawPos(void *action, int *position, int frame,
                                  void *drawInfo, void *actor);
unsigned long AnimActionCommitTrajectory(void *action);
void ActorDrawEffect(void * action);
void AnimActionCreateBlock(void *action);
void AnimActionFreeBlock(void *action);
int AnimActionPrepareBlock(void *action, int actor, int sourceId);
void AnimActionSetExternalBlock(void *action, int actor, int sourceId,
                                unsigned long budget);
void AnimActionInitTargetActor(void *action);
int AnimActionHasMetadata(void *action);
void AnimActionConfigure(void *action, unsigned char frame, int index);
int AnimActionGetMetadata(void *action);
void AnimActionMarkReady(void *action);
void AnimActionPublish(void *action);
void AnimActionChecksum(void *action, unsigned short *checksum);
void AnimActionCopyState(void *action, void *output);
int AnimActionEqual(void *action, void *state);
void AnimActionCopyChangedFields(void *action, void *state);
void AnimActionClear(void *action) {
    unsigned char *p = (unsigned char *)action;
    p[4] = 0;
    *(volatile unsigned char *)(p + 5) = 0;
}
void AnimActionInitialize(void *action, unsigned char type, unsigned short duration,
                          unsigned char rate, unsigned char initialFlags) {
    unsigned char *p = (unsigned char *)action;
    unsigned char t = type;
    union FlagWord { unsigned short word; struct { unsigned char low; unsigned char high; } bytes; } flags;
    p[8] = 0;
    p[4] = t;
    *(volatile unsigned short *)(p + 12) = duration;
    p[7] = rate;
    p[10] = initialFlags;
    unsigned long zero = 0;
    *(volatile unsigned char *)(p + 5) = (unsigned char)zero;
    *(volatile unsigned long *)(p + 14) = zero;
    flags.word = (unsigned short)zero;
    flags.bytes.low = *(volatile unsigned char *)(p + 10);
    *(volatile unsigned char *)(p + 9) = 0;
    flags.bytes.low &= 0x70;
    p[10] = flags.bytes.low;
    flags.bytes.high = flags.bytes.low;
    p[11] = 1;
    flags.bytes.high |= 0x80;
    p[10] = flags.bytes.high;
}
void WorldActionNotify(void *action, unsigned char state) {
    unsigned char *p = (unsigned char *)action;
    union FlagWord { unsigned short word; struct { unsigned char low; unsigned char high; } bytes; } flags;
    p[8] = 0;
    p[9] = 0;
    p[5] = state;
    flags.bytes.high = 0;
    flags.bytes.low = p[10];
    *(volatile unsigned long *)(p + 14) = 0;
    flags.bytes.low &= 0x70;
    p[10] = flags.bytes.low;
    flags.bytes.high = flags.bytes.low;
    *(volatile unsigned char *)(p + 11) = 1;
    flags.bytes.high |= 0x80;
    p[10] = flags.bytes.high;
}
int AnimActionIsSet(void *action) {
    unsigned long value = 0;
    unsigned char *p = (unsigned char *)action;
    value = p[10];
    return value & 1;
}
int AnimActionAdvance(void *action) {
unsigned char *p = (unsigned char *)action; unsigned long now = TimerNow(); if (now >= *(unsigned long *)(p + 14) && p[7]) { *(unsigned long *)(p + 14) = now + 1000 / (int)(unsigned char)p[7]; return 1; } return 0;
}
int AnimActionStep(unsigned char *p) {
int advanced=AnimActionAdvance(p); if (!advanced) return 0; if (!p[9]) return 0; if ((int)p[5]==0) return 0; 









    if (p[10] & 0x20) {
        unsigned char frame = p[8];
        ++frame;
        p[8] = frame;
        if (frame >= p[9])
            p[8] = 0;
    } else {
        unsigned char frame = p[8];
        ++frame;
        unsigned char count = p[9];
        p[8] = frame;
        if (frame >= count) {
            unsigned char flags = p[10];
            --count;
            flags |= 1;
            p[8] = count;
            p[10] = flags;
            p[10] = (p[10] & 0x7f) | 1;
        }
    }
    p[10] &= 0xfb;
    return 0;
}
void ActorClose(void *action) {
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
unsigned long AnimActionRelease(void *action) {
    unsigned long id = *(unsigned long *)((char *)action + 12);
    if (id != 0) {
        AnimReleaseResource((unsigned long)fallbackName, id, 3);
        unsigned long offset = id * 8;
        unsigned long *table = (unsigned long *)actorIndexTable;
        unsigned long sentinel = animReleaseSentinel;
        *(unsigned long *)((char *)table + offset) = sentinel;
    }
    return 0;
}
void AnimActionAssign(void *unused, unsigned long value, int index, int count) {
    int resourceList[10];
    if (count > 1) {
        animReleaseSentinel = value;
        BestLocInitialize(resourceList, fallbackName, count - 1);
        BestLocSetCandidate(resourceList, (void *)index);
        BestLocVisit(resourceList, (int (*)(void *))AnimActionRelease);
        return;
    }
    *(unsigned long *)((char *)actorIndexTable + index * 8) = value;
}
void AnimActionClearGroup(void *action) {
    unsigned char *p = (unsigned char *)action;
    int count = animationMetaAt0cbc[*(unsigned short *)(p + 12) * 15];
    int index = *(int *)(p + 0x22);
    int resourceList[10];
    if (count > 1) {
        animReleaseSentinel = 0;
        BestLocInitialize(resourceList, fallbackName, count - 1);
        BestLocSetCandidate(resourceList, (void *)index);
        BestLocVisit(resourceList, (int (*)(void *))AnimActionRelease);
        return;
    }
    actorIndexTable[index * 2] = 0;
}
void AnimActionReplace(unsigned char *action, unsigned long value)  {
    int count=AnimActionGetMetadata(action);
    int index = *(int *)(action + 0x22);
    int resourceListA[10];
    int resourceListB[10];
    if (count > 1)  {
        animReleaseSentinel = 0;
        BestLocInitialize(resourceListA, fallbackName, count - 1);
        BestLocSetCandidate(resourceListA, (void *)index);
        BestLocVisit(resourceListA, (int (*)(void *))AnimActionRelease);
    }
    else  {
        actorIndexTable[index * 2] = 0;
    }
    index = AnimResourceResolve((unsigned long)fallbackName, (int *)(action + 0x12));
    *(int *)(action + 0x22) = index;
    AnimResourceLoadEntry((unsigned long)fallbackName, (unsigned long *)(action + 0x1a), index);
    index = *(int *)(action + 0x22);
    if (count > 1)  {
        animReleaseSentinel = value;
        BestLocInitialize(resourceListB, fallbackName, count - 1);
        BestLocSetCandidate(resourceListB, (void *)index);
        BestLocVisit(resourceListB, (int (*)(void *))AnimActionRelease);
    }
    else  {
        *AnimResourceSlot(index)=value;
    }
}

void AnimActionUpdateTrajectory(int *action, int *actor) {
    unsigned char *p = (unsigned char *)action;
    int distance = 0;
    int angle;
    int *frame;
    if (p[5] == 10) {
        distance = f_0042c130(actor, p + 0x2a, p + 0x12);
        angle = f_0047f9d9(((*(int *)(p + 0x5f) * 0xb4) /
                             *(int *)(p + 0x63)) * 0xb60b61);
        int first = GridPointToIndex(fallbackName, (ExploPoint *)(p + 0x4f));
        int firstDistance = targetDefinitionTable[first * 12 + 1] * distance;
        int second = GridPointToIndex(fallbackName, (ExploPoint *)(p + 0x57));
        distance = firstDistance +
            (*(int *)(p + 0x5f) *
             (targetDefinitionTable[second * 12 + 1] * distance - firstDistance)) /
             *(int *)(p + 0x63) + ((distance * angle * 2) >> 16);
    } else {
        ViewToScreen(actor, (ExploPoint *)(p + 0x2a),
                     (ExploPoint *)(p + 0x12));
    }
    *(int *)p = *(int *)(p + 0x2e);
    if (p[5] == 0x11)
        *(int *)p -= 20000;
    *(int *)(p + 0x2e) -= distance;
    p[6] = (unsigned char)GridClamp(
        *(int *)((char *)actor + 0x80) + *(int *)(p + 0x26));
    frame = AnimBitmapFrame(p);
    *(int *)(p + 0x4b) = (int)frame;
    unsigned char xOffset = ((unsigned char *)frame)[10];
    unsigned int yOffset = ((unsigned char *)frame)[11];
    int width = *(unsigned short *)((char *)frame + 4);
    int scaledWidth = ViewScale(actor,
        animationMetaAt0cc4[*(unsigned short *)(p + 12) * 15]) * width;
    scaledWidth /= 64;
    int height = *(unsigned short *)(*(unsigned char **)(p + 0x4b) + 6);
    int scaledHeight = ViewScale(actor,
        animationMetaAt0cc8[*(unsigned short *)(p + 12) * 15]) * height;
    scaledHeight /= 64;
    *(int *)(p + 0x43) = scaledWidth;
    *(int *)(p + 0x3b) = *(int *)(p + 0x2a);
    *(int *)(p + 0x3f) = *(int *)(p + 0x2e);
    *(int *)(p + 0x47) = scaledHeight;
    width = *(unsigned short *)(*(unsigned char **)(p + 0x4b) + 4);
    if (width != 0)
        *(int *)(p + 0x3b) -= ((unsigned int)xOffset * scaledWidth) / width;
    height = *(unsigned short *)(*(unsigned char **)(p + 0x4b) + 6);
    if (height != 0)
        *(int *)(p + 0x3f) -= (yOffset * scaledHeight) / height;
    p[11] = 0;
}
void AnimActionInterpolateDrawPos(void *action, int *position, int frame,
                                  void *drawInfo, void *actor) {
    struct Point { int x; int y; };
    unsigned char *p = (unsigned char *)action;
    *(Point *)position = *(Point *)(p + 0x2a);
    position[1] -= *(int *)(p + 0x47) / 4;
    unsigned char *frameData = *(unsigned char **)((char *)drawInfo + 0x2d1);
    if (frameData != 0) {
        frameData += p[6] * 18;
        int ends[2];
        ends[0] = frameData[frame * 2 + 2];
        ends[1] = frameData[frame * 2 + 3];
        if (ends[0] != 0 || ends[1] != 0) {
            ends[0] -= frameData[0]; ends[1] -= frameData[1];
            ((int *)position)[0] = *(int *)(p + 0x2a);
            ((int *)position)[1] = *(int *)(p + 0x2e);
            int scaleX = ViewScale(actor,
                animationMetaAt0cc4[*(unsigned short *)(p + 12) * 15]);
            position[0] += (ends[0] * scaleX) / 64;
            int scaleY = ViewScale(actor,
                animationMetaAt0cc8[*(unsigned short *)(p + 12) * 15]);
            position[1] += (ends[1] * scaleY) / 64;
        }
    }
}
unsigned long AnimActionCommitTrajectory(void *action) {
    unsigned char *p = (unsigned char *)action;
    int listA[10];
    int listB[10];
    int distance = PointAngle((ExploPoint *)(p + 0x4f), (ExploPoint *)(p + 0x57));
    unsigned long direction = (unsigned long)GridClamp(distance) * 0x2aaaaabcUL;
    
    int tileScale = animationMetaAt0cc0[(*(unsigned short *)(p + 12)) * 15];
    int rate = animationModeRate[g_004d2dee];
    int halfFrame = tileScale * rate >> 1;
    int dx = *(int *)(p + 0x57) - *(int *)(p + 0x4f);
    int dy = *(int *)(p + 0x5b) - *(int *)(p + 0x53);
    unsigned long travel = ISqrt(dy * dy + dx * dx);
    *(int *)(p + 0x5f) = 1;
    p[11] = 1;
    unsigned char *block = *(unsigned char **)(p + 0x32);
    *(unsigned long *)(p + 99) = (travel + halfFrame) / (tileScale * rate);
    if (block == 0)
        return 0;
    int *actor = *(int **)(block + 0x490);
    if (actor == 0)
        return 0;
    int state = actor[6];
    int event = *(int *)(block + 0x200 + *(int *)(block + 0x488) * 4);
    int continueState = 1;
    if (StateInvoke(state, 0, 0) < event && *((char *)actor + 0x8d) != 0)
        continueState = 0;
    int target = *(int *)(block + *(int *)(block + 0x488) * 4);
    int *oldActor = 0;
    if (continueState != 0 && actorIndexTable[target * 2] != 0) {
        oldActor = ActorGetByUnit(target);
        if (oldActor != 0 && *(char *)(oldActor[6] + 0x12) == 0x1f &&
            oldActor[2] == actor[2]) {
            if (g_004d2df2 == 0)
                goto success;
            StrPool *pool = (StrPool *)targetBriefResource;
            char *message = pool->GetString(0x4b2, 0);
            if (f_0047723b(message) != 0)
                goto success;
        }
        oldActor = 0;
        continueState = 0;
    }
success:
    if (continueState != 0) {
        f_0043a294(state, event);
        *((unsigned char *)actor + 0x8d) = 1;
        *(int *)(p + 0x26) = (int)direction;
        if (g_004d2df2 != 0 && target != 0 && simActorVisibility[target] == 0)
            f_0044e940((unsigned long)modeCheckName, 0);
        if (modeSwitch != 0 && ModeCheck((unsigned long)modeCheckName) != 0)
            f_004731e8(modeLogName, actor, target, event);
        ++*(int *)(block + 0x488);
        if (oldActor != 0) {
            if (f_00455c40(actor) != 0) {
                if (g_004d2df2 != 0) {
                    StrPool *pool = (StrPool *)targetBriefResource;
                    f_00477295(pool->GetString(0x4b3, 0));
                }
                f_00455c40(oldActor);
            }
            *(int *)((char *)actor + 0x93) = *oldActor;
        }
        int index = *(int *)(p + 0x22);
        int resourceCount = animationMetaAt0cbc[(*(unsigned short *)(p + 12)) * 15];
        if (resourceCount < 2) {
            actorIndexTable[index * 2] = 0;
        } else {
            animReleaseSentinel = 0;
            BestLocInitialize(listB, fallbackName, resourceCount - 1);
            BestLocSetCandidate(listB, (void *)index);
            BestLocVisit(listB, (int (*)(void *))AnimActionRelease);
        }
        if (*((unsigned char *)actor + 0x89) == 0) {
            resourceCount = animationMetaAt0cbc[(*(unsigned short *)(p + 12)) * 15];
            *(int *)(p + 0x22) = target;
            if (resourceCount < 2) {
                actorIndexTable[target * 2] = actor[0];
            } else {
                animReleaseSentinel = (unsigned long)actor[0];
                BestLocInitialize(listA, fallbackName, resourceCount - 1);
                BestLocSetCandidate(listA, (void *)target);
                BestLocVisit(listA, (int (*)(void *))AnimActionRelease);
            }
        }
        if (g_004d2dee == 4 || g_004d271a == 2) {
            p[8] = 0;
            *(int *)(p + 0x12) = *(int *)(p + 0x57);
            *(int *)(p + 0x16) = *(int *)(p + 0x5b);
            *(int *)(p + 0x5f) = *(int *)(p + 99);
        } else {
            int startIndex = GridPointToIndex(fallbackName,
                                               (ExploPoint *)(p + 0x4f));
            unsigned int startGroup = targetDefinitionTable[startIndex * 12 + 1];
            int endIndex = GridPointToIndex(fallbackName,
                                            (ExploPoint *)(p + 0x57));
            int groupDistance = abs((int)startGroup -
                (int)targetDefinitionTable[endIndex * 12 + 1]);
            if (799 < groupDistance * g_004c7e50) {
                p[5] = 9;
                p[8] = 0;
                rate = animationModeRate[g_004d2dee];
                dx = *(int *)(p + 0x57) - *(int *)(p + 0x4f);
                halfFrame = rate * 0x32 >> 1;
                dy = *(int *)(p + 0x5b) - *(int *)(p + 0x53);
                travel = ISqrt(dx * dx + dy * dy);
                *(unsigned long *)(p + 99) = (travel + halfFrame) / (rate * 0x32);
            }
        }
        return 1;
    }
    p[8] = 0;
    p[9] = 0;
    p[11] = 1;
    *(unsigned long *)(p + 14) = 0;
    p[8] = 0;
    unsigned char flags = p[10] & 0x70;
    p[10] = flags;
    p[5] = 1;
    p[10] = flags | 0x80;
    *(int *)(p + 0x12) = *(int *)(p + 0x4f);
    *(int *)(p + 0x16) = *(int *)(p + 0x53);
    return 0;
}
void ActorDrawEffect(void * action) {
    unsigned char *p = (unsigned char *)action;
    AnimBitmapFrame(action);
    if (animationStatusFlags[p[5]]) p[11] = 1;
    switch ((unsigned char)p[5]) {
case 4: {
            unsigned char *block = *(unsigned char **)(p + 0x32);
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
            if (!block || !*(int *)(block + 0x490)) AssertFail(animSourcePath, __LINE__, 0);
            int current = *(int *)(p + 0x5f), count = *(int *)(p + 99); if (current < count) {
                *(int *)(p + 0x12) = current * (*(int *)(p + 0x57) - *(int *)(p + 0x4f)) / count + *(int *)(p + 0x4f);
                *(int *)(p + 0x16) = *(int *)(p + 0x53) + current * (*(int *)(p + 0x5b) - *(int *)(p + 0x53)) / count;
                signed char step = animationModeStep[g_004d2dee * 4]; *(int *)(p + 0x5f) = current + 1; step = p[8] + step;
                
                p[8] = (unsigned char)step; } else { int slot = *(int *)(block + 0x488); int target = *(int *)(block + slot * 4); if (!target) {
                    *(int *)(p + 0x12) = *(int *)(p + 0x57); *(int *)(p + 0x16) = *(int *)(p + 0x5b);
                    f_00455890(*(int *)(block + 0x490), 1); if (slot > 0 && ModeCheck((unsigned long)modeCheckName)) {
                        f_00462920((unsigned long)modeCheckName, *(int **)(block + 0x490), *(int *)(block + slot * 4 - 4), *(int *)(block + slot * 4 - 8));
                        unsigned long active = *(unsigned long *)(block + 0x490);
                        if (*(char *)(active + 0x88) == 0) return; if (*(int *)(*(int *)(active + 0x1c) + 0x12) < 1) {
                            p[8]=0; p[9]=0; *(unsigned long *)(p+14)=0; p[11]=1; unsigned char flags=p[10]&0x70; p[5]=3; p[10]=flags; p[10]=flags|0x80; return;
                        } } p[8]=0; p[9]=0; *(unsigned long *)(p+14)=0; p[11]=1;
                    unsigned char flags=p[10]&0x70; p[5]=3; p[10]=flags;  p[10]=flags|0x80; } else {
                    *(int *)(p + 0x12)=*(int *)(p+0x57); *(int *)(p+0x16)=*(int *)(p+0x5b); f_00455890(*(int *)(block+0x490),0); if (slot>0 && ModeCheck((unsigned long)modeCheckName)) {
                        f_00462920((unsigned long)modeCheckName,*(int **)(block+0x490),*(int *)(block+slot*4),*(int *)(block+slot*4-4));
                        unsigned long active=*(unsigned long *)(block+0x490); if (*(char *)(active+0x88)==0) return; if (*(int *)(*(int *)(active+0x1c)+0x12)<1) {
                            p[8]=0; p[9]=0; p[5]=3; *(unsigned long *)(p+14)=0;  p[11]=1;
                            unsigned char flags=p[10]&0x70; p[10]=flags; p[10]=flags|0x80; return;
                        } } signed char special=*(signed char *)(p+0x36); if (!special) { *(int *)(p+0x4f)=*(int *)(p+0x12); *(int *)(p+0x53)=*(int *)(p+0x16);
                        TargetObjectInit(fallbackName, p + 0x57, *(int *)(block + slot * 4)); p[8]=(unsigned char)(p[8]+animationModeStep[g_004d2dee*4]);
                        AnimActionCommitTrajectory(action); } else {  if (special==3) {
                            p[8]=0; p[9]=0; *(unsigned long *)(p+14)=0; p[11]=1; p[5]=(unsigned char)special;
                            unsigned char flags=p[10]&0x70; p[10]=flags;  p[10]=flags|0x80;
                        } p[0x36]=0; } } } break; } case 9: { unsigned char frame = (unsigned char)(p[8] + 1);
             p[8] = frame; if (3 < frame) { p[8] = 0; p[9] = 0; p[5] = 10; *(unsigned long *)(p + 14) = 0;
                unsigned char flags = p[10] & 0xf0; p[11] = 1; p[10] = flags; p[10] = flags | 0x80; } break; } case 10: {
            unsigned int current=*(unsigned int *)(p+0x5f); int count=*(int *)(p+99); if ((int)current<count) {
                *(int *)(p+0x12)=(int)current*(*(int *)(p+0x57)-*(int *)(p+0x4f))/count+*(int *)(p+0x4f);
                *(int *)(p+0x16)=(int)current*(*(int *)(p+0x5b)-*(int *)(p+0x53))/count+*(int *)(p+0x53);
                signed char step=animationModeStep[g_004d2dee*4]; *(int *)(p+0x5f)=current+1;
                step=p[8]+step;  p[8]=(unsigned char)step;
            } else { *(int *)(p+0x12)=*(int *)(p+0x57); *(int *)(p+0x16)=*(int *)(p+0x5b); p[8]=0; p[9]=0; *(unsigned long *)(p+14)=0; p[11]=1; unsigned char flags=p[10]&0xf0;
                p[10]=flags; p[5]=0xb; p[10]=flags|0x80; } break; } case 11: { unsigned char *block=*(unsigned char **)(p+0x32);
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
            if (!block || !*(int *)(block+0x490)) AssertFail(animSourcePath, __LINE__,0);
            p[8]=0; p[9]=0; *(unsigned long *)(p+14)=0; p[11]=1;
            unsigned char flags=p[10]&0x70;  p[10]=flags; p[5]=4; p[10]=flags|0x80; break;
        }
        case 1:
            if (*(unsigned char **)(p + 0x32)) {
                delete *(unsigned char **)(p + 0x32);
                *(unsigned long *)(p + 0x32) = 0;
            }
            break;
        case 7:
            if (*(unsigned char **)(p + 0x32)) {
                delete *(unsigned char **)(p + 0x32);
                *(unsigned long *)(p + 0x32) = 0;
            }
            break;
        case 17:
            if (*(unsigned char **)(p + 0x32)) {
                delete *(unsigned char **)(p + 0x32);
                *(unsigned long *)(p + 0x32) = 0;
            }
            break;
        case 14:
            if (*(unsigned char **)(p + 0x32)) {
                delete *(unsigned char **)(p + 0x32);
                *(unsigned long *)(p + 0x32) = 0;
            }
            break;
        case 2: {
            p[8] = 0; p[9] = 0; *(unsigned long *)(p + 14) = 0; p[11] = 1;
            unsigned char flags = p[10] & 0xf0; p[10] = flags; p[5] = 4;
            p[10] = flags | 0x80; break;
        }
        case 3: {
            unsigned char *block = *(unsigned char **)(p + 0x32);
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
            if (!block || !*(int *)(block + 0x490)) AssertFail(animSourcePath, __LINE__, 0);
            BestLocSetData((void *)fallbackName, p + 0x1a,
                           (void *)*(int *)(p + 0x22));
            TargetObjectInit(fallbackName, p + 0x12,
                             *(int *)(p + 0x22));
            p[8] = 0; p[9] = 0; *(unsigned long *)(p + 14) = 0; p[11] = 1;
            unsigned char flags = p[10] & 0x70; p[10] = flags; p[5] = 1;
            p[10] = flags | 0x80;  break;
        }
        case 6: { unsigned char frame = (unsigned char)(p[8] + 1);  p[8] = frame; if (3 < frame) { p[8] = 0; p[9] = 0;
                *(unsigned long *)(p + 14) = 0; p[11] = 1; unsigned char flags = p[10] & 0xf0; p[5] = 7; p[10] = flags;
                p[10] = flags | 0x80; } break; } case 8: { unsigned char frame = (unsigned char)(p[8] + 1);
             p[8] = frame; if (3 < frame) { p[8] = 0; p[9] = 0; p[5] = 1; *(unsigned long *)(p + 14) = 0;
                unsigned char flags = p[10] & 0xf0; p[10] = flags; p[11] = 1; p[10] = flags | 0x80; } break; } case 12: {
            signed char direction=*(signed char *)(p+0x39);
            if (direction<1) {
                if (!p[8]) {
                    *(unsigned long *)(p+14)=0; p[11]=1; p[8]=0; p[9]=0; unsigned char flags=p[10]&0x70;
                     p[10]=flags; p[5]=1; p[10]=flags|0x80;
                } else {  p[8]=(unsigned char)(p[8]-1); }
            } else {
                unsigned char frame=(unsigned char)(p[8]+1); p[8]=frame; 
                if (p[0x38] <= frame) p[0x39]=0xff;
            }
            break;
        }
        case 13: {
            p[8]=0; p[9]=0; p[5]=0xe; *(unsigned long *)(p+14)=0; unsigned char flags=p[10]&0x70;
             p[11]=1; p[10]=flags; p[10]=flags|0x80; break;
        }
        case 15: {
            p[8]=0; p[9]=0; *(unsigned long *)(p+14)=0; p[11]=1; unsigned char flags=p[10]&0x70;
            p[5]=1; p[10]=flags;  p[10]=flags|0x80; break;
        }
        case 16: {
            p[8]=0; p[9]=0; *(unsigned long *)(p+14)=0;  p[11]=1; unsigned char flags=p[10]&0x70;
            p[5]=0x11; p[10]=flags; p[10]=flags|0x80; break;
        }
        case 19: {
            unsigned char *block=*(unsigned char **)(p+0x32);
            if (!block) {
                p[8]=0; p[9]=0; *(unsigned long *)(p+14)=0;  p[11]=1; unsigned char flags=p[10]&0x70;
                p[5]=1; p[10]=flags; p[10]=flags|0x80;
            } else {
                int slot=*(int *)(block+0x488); int *actor=ActorGetByUnit(*(int *)(block+slot*4));
                if (!actor) {
                    p[8]=0; p[9]=0; *(unsigned long *)(p+14)=0; p[11]=1; unsigned char flags=p[10]&0x70;
                    p[5]=4; p[10]=flags;  p[10]=flags|0x80;
                } else {
                    signed char kind=*(signed char *)((char *)actor+0x25);
                    if (kind==1 || kind==7 || kind==0xe || kind==0x11 || kind==0x12) {
                        p[8]=0; p[9]=0; p[5]=1; *(unsigned long *)(p+14)=0; unsigned char flags=p[10]&0x70;
                         p[11]=1; p[10]=flags; p[10]=flags|0x80;
                    }
                }
            }
            break;
        }
    }
    
    if (*(unsigned char **)(p+0x32)) {
        int index = GridPointToIndex(fallbackName, (ExploPoint *)(p + 0x12));
        BestLocSetData((void *)fallbackName, p + 0x1a, (void *)index);
    }
    unsigned short frames=*(unsigned short *)(p+8);
    unsigned char frame=(unsigned char)frames, count=(unsigned char)(frames>>8);
    if (count<=frame) {
        if (count) { p[8]=(unsigned char)(frame%count); return; }
        p[8]=0;
    }
    return;
}
void AnimActionCreateBlock(void *action) {
    if (*(unsigned long *)((char *)action + 0x32) == 0)
        *(unsigned long *)((char *)action + 0x32) =
            (unsigned long)new AnimBlockAllocation;
    if (*(unsigned long *)((char *)action + 0x32) == 0)
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
        AssertFail(animSourcePath, __LINE__, 0);
    memset(*(void **)((char *)action + 0x32), 0, 0x494);
}
void AnimActionFreeBlock(void *action) {
    struct ActionResource { unsigned char pad[0x32]; unsigned char *resource; };
    ActionResource *object = (ActionResource *)action;
    if (object->resource != 0) {
        delete object->resource;
        object->resource = 0;
    }
}
int AnimActionPrepareBlock(void *action, int actor, int sourceId) {
    unsigned char *p = (unsigned char *)action;
    if (actor != 0 && sourceId != 0) {
        unsigned char type = p[5];
        if (type == 1 || type == 7 || type == 0x0e || type == 0x11 || type == 0x12) {
            if ((void *)actor == g4d2498)
                goto valid;
        }
    }
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
    AssertFail(animSourcePath, __LINE__, 0);
valid:
    int id = *(int *)(p + 0x22);
    if (bestLocCursor != id)
        BestLocSetup(bestLocEntries, id, 0);
    AnimActionCreateBlock(action);
    unsigned char *block = *(unsigned char **)(p + 0x32);
    int found = f_0042d2b0(bestLocEntries, sourceId, block + 4, 0x80);
    if (!found)
        f_004324d0((unsigned long)fallbackName, id, sourceId, block + 4, 0x80);
    else
        f_0042d370(bestLocEntries, sourceId);
    *(int *)block = id;
    block[0x400] = 6;
    *(int *)(block + 0x200) = 0;
    *(int *)(block + 0x484) = 1;
    *(int *)(block + 0x488) = 1;
    *(int *)(block + 0x490) = actor;
    int entryIndex = 1;
    int threshold = StateInvoke(*(int *)(actor + 0x18), 0, 0);
    int currentId = id;
    for (int offset = 4; *(int *)(block + offset) != 0; offset += 4) {
        unsigned short result[6];
        TargetInitAt((unsigned long)fallbackName, result,
                     *(int *)(block + offset), currentId);
        ++*(int *)(block + 0x484);
        *(int *)(block + 0x480) += result[0];
        *(unsigned int *)(block + 0x200 + offset) = result[0];
        block[0x400 + entryIndex] =
            (threshold < *(int *)(block + 0x480)) ? 9 : 7;
        currentId = *(int *)(block + offset);
        if (*(int *)(actor + 8) == gLocalActorKind) {
            int resource = *(int *)(actorIndexTable + currentId * 2);
            if (resource < 0 && simVisibleTypes[currentId] != 0)
                block[0x400 + entryIndex] = 9;
            if (0 < resource && simActorVisibility[currentId] != 0)
                block[0x400 + entryIndex] = 9;
        }
        ++entryIndex;
    }
    return 1;
}
void AnimActionSetExternalBlock(void *action, int actor, int sourceId,
                                unsigned long budget) {
    unsigned char *p = (unsigned char *)action;
    unsigned char type = p[5];
    int allowed = type == 1 || type == 7 || type == 0x0e || type == 0x11 || type == 0x12;
    if (actor && sourceId && allowed)
        goto valid;
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
    AssertFail(animSourcePath, __LINE__, 0);
valid:
    AnimActionCreateBlock(action);
    unsigned char *block = *(unsigned char **)(p + 0x32);
    *(unsigned long *)(block + 0x484) = 2;
    *(unsigned long *)(block + 0x488) = 1;
    *(unsigned long *)(block + 0x480) = budget;
    *(unsigned long *)(block + 0x490) = actor;
    *(unsigned long *)block = *(unsigned long *)(p + 0x22);
    block[0x400] = 0;
    *(unsigned long *)(block + 0x200) = 0;
    *(unsigned long *)(block + 4) = sourceId;
    block[0x401] = 0;
    *(unsigned long *)(block + 0x204) = budget;
}
void AnimActionInitTargetActor(void *action) {
    unsigned char *p = (unsigned char *)action;
    unsigned char *block = *(unsigned char **)(p + 0x32);
    if (block != 0) {
        if (*(int *)(block + 0x488) == 1) {
            if (*(int *)(block + 4) != 0)
                goto valid;
        }
    }
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
    AssertFail(animSourcePath, __LINE__, 0);
valid:
TargetObjectInit(fallbackName,p+0x57,*(int *)(*(unsigned char **)(p+0x32)+4));((AnimPair *)(p+0x4f))->Copy(*(AnimPair *)(p+0x12));



    p[8] = 0;
    p[5] = 4;
    AnimActionCommitTrajectory(action);
}
int AnimActionHasMetadata(void *action) {
    return animationMetaAt0cc0[*(unsigned short *)((char *)action + 12) * 15] != 0;
}
void AnimActionConfigure(void *action, unsigned char frame, int index) {
    unsigned char *p = (unsigned char *)action;
    if (p[5] == 1) {
        unsigned short metadataIndex = *(unsigned short *)(p + 12);
        if (animationMetaAt0cc0[metadataIndex * 15] == 0)
            return;
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
int AnimActionGetMetadata(void *action) {
    return animationMetaAt0cbc[*(unsigned short *)((char *)action + 12) * 15];
}
void AnimActionMarkReady(void *action) {
    *(unsigned char *)((char *)action + 11) = 1;
}
void AnimActionPublish(void *action) {
    unsigned char *p = (unsigned char *)action;
    unsigned char *entry = (unsigned char *)animationKindTable;
    entry += *(unsigned short *)(p + 12) * 60;
    if (!entry[8])
        return;
    int value = f_00434a20((unsigned long)fallbackName,
                           *(int *)(p + 0x22), *(signed char *)(entry + 9));
    f_00434bd0((unsigned long)fallbackName, *(int *)(p + 0x22), value,
               *(signed char *)(entry + 9),
               *(signed char *)(entry + 10), 100);
}
void AnimActionChecksum(void *action, unsigned short *checksum) {
    Crc16(checksum, (char *)action + 0x12, 8);
    Crc16(checksum, (char *)action + 0x1a, 8);
    Crc16(checksum, (char *)action + 0x22, 4);
    Crc16(checksum, (char *)action + 0x26, 4);
}
void AnimActionCopyState(void *action, void *output)  {
    ((AnimPair *)((char *)output+5))->Copy(*(AnimPair *)((char *)action+0x12));
    ((AnimPair *)((char *)output+0xd))->Copy(*(AnimPair *)((char *)action+0x1a));
    *(unsigned long *)((char *)output+0x15)=*(unsigned long *)((char *)action+0x22);
    *(unsigned long *)((char *)output+0x19)=*(unsigned long *)((char *)action+0x26);
}

int AnimActionEqual(void *action, void *state) {
    unsigned char *p = (unsigned char *)action;
    unsigned char *q = (unsigned char *)state;
    int equal = 1;
    if (*(int *)(q + 5) != *(int *)(p + 0x12) ||
        *(int *)(q + 9) != *(int *)(p + 0x16))
        equal = 0;
    if (*(int *)(q + 0x0d) != *(int *)(p + 0x1a) ||
        *(int *)(q + 0x11) != *(int *)(p + 0x1e))
        equal = 0;
    if (*(int *)(q + 0x15) != *(int *)(p + 0x22))
        equal = 0;
    if (*(int *)(q + 0x19) != *(int *)(p + 0x26))
        equal = 0;
    return equal;
}
void AnimActionCopyChangedFields(void *action, void *state) {
    unsigned char *p = (unsigned char *)action;
    unsigned char *q = (unsigned char *)state;
    struct Pair { unsigned long first; unsigned long second; };
    Pair *first = (Pair *)(p + 0x12);
    Pair *firstSource = (Pair *)(q + 5);
    if (first->first != firstSource->first ||
        first->second != firstSource->second)
        *first = *firstSource;
    Pair *second = (Pair *)(p + 0x1a);
    Pair *secondSource = (Pair *)(q + 0x0d);
    if (second->first != secondSource->first ||
        second->second != secondSource->second)
        *second = *secondSource;
    if (*(int *)(q + 0x15) != *(int *)(p + 0x22))
        *(int *)(p + 0x22) = *(int *)(q + 0x15);
    if (*(int *)(q + 0x19) != *(int *)(p + 0x26))
        *(int *)(p + 0x26) = *(int *)(q + 0x19);
}



