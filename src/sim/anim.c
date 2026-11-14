#include <string.h>
#include <stdlib.h>
char animSourcePath[] = "sim\\anim.c";
extern void AssertFail(const char *file, int line, const char *message);
struct AnimBlockAllocation { unsigned char bytes[0x494]; };
extern "C" unsigned int TimerNow(void);
struct AnimPair { long first; long second; AnimPair() {} AnimPair(long x,long y) : first(x),second(y) {} void Copy(const AnimPair &p) { *this = p; } int Equal(const AnimPair &p) const { return first == p.first && second == p.second; }
    int operator!=(const AnimPair &p) const { return first != p.first || second != p.second; } };
#pragma pack(push, 1)
/* Path block (0x494 bytes, AnimActionCreateBlock) and the network-visible state record (AnimActionCopyState). */
struct AnimActor; struct AnimBlock { long path[128]; long cost[128]; unsigned char marks[0x80]; long budget, count, index, unknown48c; AnimActor *actor; }; struct ExploPoint { int x; int y; }; struct AnimState { unsigned char header[5]; AnimPair position; AnimPair target; long resource; long direction; };
/* Working layout of the 0x67-byte animation action (member of ActorRecord at +0x20). */
struct AnimAction { long depth; unsigned char type, state, angle, rate, frame, count, flags, ready; unsigned short kind; unsigned long time;
    AnimPair position; AnimPair target; long resource; long direction; union { struct { long x, y; }; ExploPoint point; }; AnimBlock *block; unsigned char b36, b37; signed char b38, b39; unsigned char b3a;
    long drawX, drawY, drawWidth, drawHeight; int *bitmap; AnimPair from; AnimPair to; long step, steps; AnimAction(); void SetState(int s) { state = s; } };
struct AnimPilot { char pad[0x12]; int health; int Alive(){return health>0;} }; struct AnimActor { long id,spare,owner,type,spare10,target; void *unit; AnimPilot *pilot; char action[0x67]; signed char done,active,disabled; char pad8a[3]; unsigned char spent,returnFire; char pad8f[4]; long link; int Active(){return active;} }; inline int AnimActionBusy(AnimAction *a) { return a->state != 1 && a->state != 7 && a->state != 14 && a->state != 17 && a->state != 18; }
#pragma pack(pop)
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
extern int DisplayPointToScreenScaled(void *actor, void *from, void *to); struct AnimBitmap { unsigned char head[4]; unsigned short width, height; unsigned char pad[2], xOffset, yOffset; };
extern int FixedSin(int angle);
extern int f_0042afb0(unsigned long name, void *point);
extern int *AnimBitmapFrame(void *action);
extern int animationMetaAt0cc4[];
extern int animationMetaAt0cc8[];
extern unsigned char *targetDefinitionTable;
extern long GridClamp(unsigned long);
extern int ViewScale(void *actor, int value);

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
extern unsigned long AnimActionCommitTrajectory(AnimAction *action);
extern void f_00455890(int actor, int direction);
extern void f_00462920(unsigned long resource, int *actor, int current, int previous);
extern unsigned char animationStatusFlags[];
extern signed char animationModeStep[];
extern "C" void BestLocSetData(void *name, void *point, void *index);
extern int BestLocSetup(void *entries, int id, int reset);
extern int SearchGetPath(void *entries, int id, void *buffer, int size);
extern void BuildTerrainPath(unsigned long name, int index, int id, void *buffer, int size);
extern void SearchGetCost(void *entries, int id);
extern void TargetInitAt(unsigned long name, unsigned short *result, int actor, int id);
extern int StateInvoke(int state, void *first, void *second);
extern unsigned char bestLocEntries[];
extern int bestLocCursor;
extern void *g4d2498;
extern int gLocalActorKind;
extern unsigned char *simVisibleTypes; inline int SimTypeHidden(int t) { return !simVisibleTypes[t]; } inline int SimActorHidden(int t) { return !simActorVisibility[t]; }
extern int animationMetaAt0cbc[];
extern unsigned long animationKindTable[];
extern "C" int f_00434a20(unsigned long name, int index, int variant);
extern "C" void f_00434bd0(unsigned long name, int index, int value,
                           int variant, int flags, int duration);
extern void Crc16(unsigned short *checksum, void *data, int length);
extern void AssertFail(const char *file, int line, const char *message);
void AnimActionClear(AnimAction *action);
void AnimActionInitialize(AnimAction *, int type, int kind,
 int rate, int flags); inline void AnimActionSetState(AnimAction *a, int s) { a->state = s; }
void WorldActionNotify(AnimAction *action, int state);
int AnimActionIsSet(AnimAction *action);
int AnimActionAdvance(AnimAction *action);
int AnimActionStep(AnimAction *action);

unsigned long AnimActionRelease(void *action);
void AnimActionAssign(void *unused, unsigned long value, int index, int count);
void AnimActionClearGroup(AnimAction *action);
void AnimActionReplace(AnimAction *action, unsigned long value);
void AnimActionUpdateTrajectory(AnimAction *action, int *actor);
void AnimActionInterpolateDrawPos(AnimAction *action, int *position, int frame,
                                  void *drawInfo, void *actor);
unsigned long AnimActionCommitTrajectory(AnimAction *action);
void ActorDrawEffect(AnimAction *action);
void AnimActionCreateBlock(void *action);
void AnimActionFreeBlock(void *action);
int AnimActionPrepareBlock(AnimAction *action, int actor, int sourceId);
void AnimActionSetExternalBlock(AnimAction *action, int actor, int sourceId,
                                unsigned long budget);
void AnimActionInitTargetActor(AnimAction *action);
int AnimActionHasMetadata(AnimAction *action);
void AnimActionConfigure(AnimAction *action, int frame, int index);
int AnimActionGetMetadata(void *action);
void AnimActionPublish(void *action);
void AnimActionChecksum(void *action, unsigned short *checksum);
void AnimActionCopyState(void *action, void *output);
int AnimActionEqual(AnimAction *action, AnimState *state);
void AnimActionCopyChangedFields(AnimAction *action, AnimState *state);
void AnimActionClear(AnimAction *a) {
    a->type = 0; a->state = 0;
}
void AnimActionInitialize(AnimAction *a, int type, int kind, int rate, int flags) {
    a->type = type; a->kind = kind; a->rate = rate; a->flags = flags; WorldActionNotify(a, 0);
}
void WorldActionNotify(AnimAction *a, int state) {
    a->frame = 0; a->count = 0; a->state = state; a->time = 0; a->ready = 1;
    a->flags &= 0x70; a->flags |= 0x80;
}
int AnimActionIsSet(AnimAction *a) {
    return (unsigned)a->flags & 1;
}
int AnimActionAdvance(AnimAction *a) {
    unsigned long now = TimerNow();
    if (now >= a->time && a->rate) { a->time = now + 1000 / a->rate; return 1; }
    return 0;
}
int AnimActionStep(AnimAction *a) {
    if (!AnimActionAdvance(a) || !a->count || a->state == 0) return 0;
    if ((unsigned)a->flags & 0x20) {
        if (++a->frame >= a->count) a->frame = 0;
    } else if (++a->frame >= a->count) {
        a->frame = a->count - 1; a->flags |= 1; a->flags &= 0x7f;
    }
    a->flags &= 0xfb;
    return 0;
}


















































AnimAction::AnimAction() {
    AnimActionClear(this);
    position.Copy(AnimPair(0,0));
    frame = 0; count = 0; ready = 1; b36 = 0; time = 0; direction = 0;
    AnimActionInitialize(this, 1, 0, 50, 0); time = 0; block = 0;
    SetState(1);
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
void AnimActionClearGroup(AnimAction *a) {
    int list[10];
    int count = animationMetaAt0cbc[a->kind * 15];
    int index = a->resource;
    if (count > 1) {
        animReleaseSentinel = 0;
        BestLocInitialize(list, fallbackName, count - 1);
        BestLocSetCandidate(list, (void *)index);
        BestLocVisit(list, (int (*)(void *))AnimActionRelease);
    } else actorIndexTable[index * 2] = 0;
}
void AnimActionReplace(AnimAction *a, unsigned long value) {
    int list[10];
    int count = animationMetaAt0cbc[a->kind * 15];
    int index = a->resource;
    if (count > 1) {
        animReleaseSentinel = 0;
        BestLocInitialize(list, fallbackName, count - 1);
        BestLocSetCandidate(list, (void *)index);
        BestLocVisit(list, (int (*)(void *))AnimActionRelease);
    } else actorIndexTable[index * 2] = 0;
    a->resource = GridPointToIndex(fallbackName, (ExploPoint *)&a->position);
    BestLocSetData((void *)fallbackName, &a->target, (void *)a->resource);
    AnimActionAssign(a, value, a->resource, count);
}


















void AnimActionUpdateTrajectory(AnimAction *a, int *actor) {
    int state = a->state; int distance = 0; if (state == 10) {
        distance = DisplayPointToScreenScaled(actor, &a->point, &a->position);
        int sine = FixedSin(a->step * 180 / a->steps * 0xb60b61);
        int first = targetDefinitionTable[GridPointToIndex(fallbackName, (ExploPoint *)&a->from) * 12 + 1] * distance;
        int second = targetDefinitionTable[GridPointToIndex(fallbackName, (ExploPoint *)&a->to) * 12 + 1] * distance;
        int curve = distance * sine * 2 >> 16; distance = first + a->step * (second - first) / a->steps + curve;
    } else {
        ViewToScreen(actor, &a->point, (ExploPoint *)&a->position);
    }
    a->depth = a->y;
    if (a->state == 0x11) a->depth -= 20000;
    a->y -= distance;
    a->angle = (unsigned char)GridClamp(actor[0x20] + a->direction);
    a->bitmap = AnimBitmapFrame(a);
    unsigned yOffset, xOffset = ((AnimBitmap *)a->bitmap)->xOffset; yOffset = ((AnimBitmap *)a->bitmap)->yOffset;

    int width = ViewScale(actor, animationMetaAt0cc4[a->kind * 15]) * ((AnimBitmap *)a->bitmap)->width / 64;
    int height = ViewScale(actor, animationMetaAt0cc8[a->kind * 15]) * ((AnimBitmap *)a->bitmap)->height / 64;
    a->drawWidth = width;
    a->drawX = a->x;
    a->drawY = a->y;
    a->drawHeight = height;
    if (((AnimBitmap *)a->bitmap)->width) a->drawX -= xOffset * width / ((AnimBitmap *)a->bitmap)->width;
    if (((AnimBitmap *)a->bitmap)->height) a->drawY -= yOffset * height / ((AnimBitmap *)a->bitmap)->height;
    a->ready = 0;
}























void AnimActionInterpolateDrawPos(AnimAction *p, int *position, int frame,
                                  void *drawInfo, void *actor) {
    *(ExploPoint *)position=p->point;
    position[1]-=p->drawHeight/4;
    unsigned char *frameData=*(unsigned char **)((char *)drawInfo+0x2d1);
    if(frameData) {
        frameData+=p->angle*18;
        int y, x;
        x=frameData[frame*2+2]; y=frameData[frame*2+3];
        if(x || y) {
            x-=frameData[0]; y-=frameData[1];
            *(ExploPoint *)position=p->point;
            position[0]+=x*ViewScale(actor,animationMetaAt0cc4[p->kind*15])/64;
            position[1]+=y*ViewScale(actor,animationMetaAt0cc8[p->kind*15])/64;
        }
    }
}








unsigned long AnimActionCommitTrajectory(AnimAction *p) {

    int listA[10];
    int listB[10];
    int distance = PointAngle((ExploPoint *)(&p->from), (ExploPoint *)(&p->to));
    unsigned long direction = (unsigned long)GridClamp(distance) * 0x2aaaaabcUL;
    
    int tileScale = animationMetaAt0cc0[(p->kind) * 15];
    int rate = animationModeRate[g_004d2dee];
    int halfFrame = tileScale * rate >> 1;
    int dx = p->to.first - p->from.first;
    int dy = p->to.second - p->from.second;
    unsigned long travel = ISqrt(dy * dy + dx * dx);
    p->step = 1;
    p->ready = 1;
    AnimBlock *block=p->block;
    p->steps = (travel + halfFrame) / (tileScale * rate);
    if (block == 0)
        return 0;
    int *actor = (int *)block->actor;
    if (actor == 0)
        return 0;
    int state = actor[6];
    int event = block->cost[block->index];
    int continueState = 1;
    if (StateInvoke(state, 0, 0) < event && *((char *)actor + 0x8d) != 0)
        continueState = 0;
    int target = block->path[block->index];
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
        p->direction = (int)direction;
        if (g_004d2df2 != 0 && target != 0 && simActorVisibility[target] == 0)
            f_0044e940((unsigned long)modeCheckName, 0);
        if (modeSwitch != 0 && ModeCheck((unsigned long)modeCheckName) != 0)
            f_004731e8(modeLogName, actor, target, event);
        ++block->index;
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
        AnimActionClearGroup(p);









        if (*((unsigned char *)actor + 0x89) == 0) {
            int resourceCount = animationMetaAt0cbc[(p->kind) * 15];
            p->resource = target;
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
            p->frame = 0;
            p->position.first = p->to.first;
            p->position.second = p->to.second;
            p->step = p->steps;
        } else {
            int startIndex = GridPointToIndex(fallbackName,
                                               (ExploPoint *)(&p->from));
            unsigned int startGroup = targetDefinitionTable[startIndex * 12 + 1];
            int endIndex = GridPointToIndex(fallbackName,
                                            (ExploPoint *)(&p->to));
            int groupDistance = abs((int)startGroup -
                (int)targetDefinitionTable[endIndex * 12 + 1]);
            if (799 < groupDistance * g_004c7e50) {
                p->state = 9;
                p->frame = 0;
                rate = animationModeRate[g_004d2dee];
                dx = p->to.first - p->from.first;
                halfFrame = rate * 0x32 >> 1;
                dy = p->to.second - p->from.second;
                travel = ISqrt(dx * dx + dy * dy);
                p->steps = (travel + halfFrame) / (rate * 0x32);
            }
        }
        return 1;
    }
    p->frame = 0;
    p->count = 0;
    p->ready = 1;
    p->time = 0;
    p->frame = 0;
    unsigned char flags = p->flags & 0x70;
    p->flags = flags;
    p->state = 1;
    p->flags = flags | 0x80;
    p->position.first = p->from.first;
    p->position.second = p->from.second;
    return 0;
}
void ActorDrawEffect(AnimAction *p) {

    AnimBitmapFrame(p);
    if (animationStatusFlags[p->state]) p->ready = 1;
    switch ((unsigned char)p->state) {
case 4: {
            
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
            if (!p->block || !p->block->actor) AssertFail(animSourcePath, __LINE__, 0);
            if (p->step < p->steps) {
                p->position.first = p->step * (p->to.first - p->from.first) / p->steps + p->from.first;
                p->position.second = p->from.second + p->step * (p->to.second - p->from.second) / p->steps;
                signed char step = animationModeStep[g_004d2dee * 4]; p->step = p->step + 1; step = p->frame + step;
                
                p->frame = (unsigned char)step; } else {  int target = p->block->path[p->block->index]; if (target) {
                    p->position.Copy(p->to); f_00455890((int)p->block->actor,0); if (p->block->index>0 && ModeCheck((unsigned long)modeCheckName)) {
                        f_00462920((unsigned long)modeCheckName,(int *)p->block->actor,p->block->path[p->block->index],p->block->path[p->block->index-1]);
                         if (!p->block->actor->Active()) return; if (!p->block->actor->pilot->Alive()) {
                            WorldActionNotify(p, 3);
 return;
                        } } signed char special=p->b36; if (special) { if (special==3) {
                            WorldActionNotify(p, special);
                        } p->b36=0; } else { p->from.Copy(p->position);
                        TargetObjectInit(fallbackName, &p->to, p->block->path[p->block->index]); p->frame=(unsigned char)(p->frame+animationModeStep[g_004d2dee*4]);
                        AnimActionCommitTrajectory(p);
 } } else {
                    p->position.Copy(p->to);
                    f_00455890((int)p->block->actor, 1); if (p->block->index > 0 && ModeCheck((unsigned long)modeCheckName)) {
                        f_00462920((unsigned long)modeCheckName, (int *)p->block->actor, p->block->path[p->block->index-1], p->block->path[p->block->index-2]);
                        
                        if (!p->block->actor->Active()) return; if (!p->block->actor->pilot->Alive()) {
                            WorldActionNotify(p, 3); return;
                        } } WorldActionNotify(p, 3);
 } } break; } case 9: { int frame = (unsigned char)(p->frame + 1);
             p->frame = frame; if (3 < frame) { p->frame = 0; p->count = 0; p->SetState(10); p->time = 0;
                unsigned char flags = p->flags & 0xf0; p->ready = 1; p->flags = flags; p->flags = flags | 0x80; } break; } case 10: {
            if (p->step < p->steps) {
                p->position.first=p->step*(p->to.first-p->from.first)/p->steps+p->from.first;
                p->position.second=p->step*(p->to.second-p->from.second)/p->steps+p->from.second;
                signed char step=animationModeStep[g_004d2dee*4]; p->step=p->step+1;
                step=p->frame+step;  p->frame=(unsigned char)step;
            } else { p->position.Copy(p->to); p->frame=0; p->count=0; p->time=0; p->ready=1; unsigned char flags=p->flags&0xf0;
                p->flags=flags; p->SetState(0xb); p->flags=flags|0x80; } break; } case 11: { 
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
            if (!p->block || !p->block->actor) AssertFail(animSourcePath, __LINE__,0);
            WorldActionNotify(p, 4);
 break;
        }
        case 1:
            if (p->block) {
                delete p->block;
                p->block = 0;
            }
            break;
        case 7:
            if (p->block) {
                delete p->block;
                p->block = 0;
            }
            break;
        case 17:
            if (p->block) {
                delete p->block;
                p->block = 0;
            }
            break;
        case 14:
            if (p->block) {
                delete p->block;
                p->block = 0;
            }
            break;
        case 2: {
            p->frame = 0; p->count = 0; p->time = 0; p->ready = 1;
            unsigned char flags = p->flags & 0xf0; p->flags = flags; p->SetState(4);
            p->flags = flags | 0x80; break;
        }
        case 3: {
            
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
            if (!p->block || !p->block->actor) AssertFail(animSourcePath, __LINE__, 0);
            BestLocSetData((void *)fallbackName, &p->target,
                           (void *)p->resource);
            TargetObjectInit(fallbackName, &p->position,
                             p->resource);
            WorldActionNotify(p, 1);

  break;
        }
        case 6: { int frame = (unsigned char)(p->frame + 1);  p->frame = frame; if (3 < frame) { p->frame = 0; p->count = 0;
                p->time = 0; p->ready = 1; unsigned char flags = p->flags & 0xf0; p->SetState(7); p->flags = flags;
                p->flags = flags | 0x80; } break; } case 8: { int frame = (unsigned char)(p->frame + 1);
             p->frame = frame; if (3 < frame) { p->frame = 0; p->count = 0; p->SetState(1); p->time = 0;
                unsigned char flags = p->flags & 0xf0; p->flags = flags; p->ready = 1; p->flags = flags | 0x80; } break; } case 12: {
            signed char direction=p->b39;
            if (direction<1) {
                if (!p->frame) {
                    p->time=0; p->ready=1; WorldActionNotify(p, 1);

                } else {  p->frame=(unsigned char)(p->frame-1); }
            } else {
                int frame=(unsigned char)(p->frame+1); p->frame=frame; 
                if (p->b38 <= frame) p->b39=0xff;
            }
            break;
        }
        case 13: {
            WorldActionNotify(p, 0xe);
 break;
        }
        case 15: {
            WorldActionNotify(p, 1);
 break;
        }
        case 16: {
            WorldActionNotify(p, 0x11);
 break;
        }
        case 19: {
            
            if (!p->block) {
                WorldActionNotify(p, 1);

            } else {
                 int *actor=ActorGetByUnit(p->block->path[p->block->index]);
                if (!actor) {
                    WorldActionNotify(p, 4);

                } else {
                    
                    if (!AnimActionBusy((AnimAction *)((char *)actor+0x20))) {
                        WorldActionNotify(p, 1);

                    }
                }
            }
            break;
        }
    }
    
    if (p->block) {
        int index = GridPointToIndex(fallbackName, (ExploPoint *)&p->position);
        BestLocSetData((void *)fallbackName, &p->target, (void *)index);
    }
    unsigned char frame=p->frame, count=p->count;
    
    if (count<=frame) {
        if (count) { p->frame=(unsigned char)(frame%count); return; }
        p->frame=0;
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
int AnimActionPrepareBlock(AnimAction *a, int actor, int sourceId) {













    if (!(actor && sourceId && !AnimActionBusy(a) && (void *)actor == g4d2498)) AssertFail(animSourcePath, __LINE__, 0);
    if (bestLocCursor != a->resource) BestLocSetup(bestLocEntries, a->resource, 0);
    AnimActionCreateBlock(a);
    if (SearchGetPath(bestLocEntries, sourceId, &a->block->path[1], 0x80)) SearchGetCost(bestLocEntries, sourceId);
    else BuildTerrainPath((unsigned long)fallbackName, a->resource, sourceId, &a->block->path[1], 0x80);
    a->block->path[0] = a->resource;
    a->block->marks[0] = 6;
    a->block->cost[0] = 0;
    a->block->count = 1;
    a->block->index = 1;
    a->block->actor = (AnimActor *)actor;
    int threshold = StateInvoke(((int *)actor)[6], 0, 0);
    int previous = a->resource;
    for (int i = 1; a->block->path[i]; i++) {
        unsigned short result[6];
        TargetInitAt((unsigned long)fallbackName, result, a->block->path[i], previous);
        a->block->count++;
        a->block->budget += result[0];
        a->block->cost[i] = result[0];
        a->block->marks[i] = a->block->budget > threshold ? 9 : 7;
        previous = a->block->path[i];
        if (((int *)actor)[2] == gLocalActorKind) {
            int resource = actorIndexTable[previous * 2];
            if (resource < 0 && !SimTypeHidden(previous)) a->block->marks[i] = 9;
            if (resource > 0 && !SimActorHidden(previous)) a->block->marks[i] = 9;
        }
    }
    return 1;
}













void AnimActionSetExternalBlock(AnimAction *a, int actor, int sourceId, unsigned long budget) {

























    if (!(actor && sourceId && !AnimActionBusy(a))) AssertFail(animSourcePath, __LINE__, 0);
    AnimActionCreateBlock(a);

    a->block->count = 2;
    a->block->index = 1;
    a->block->budget = budget;
    a->block->actor = (AnimActor *)actor;
    a->block->path[0] = a->resource;
    a->block->marks[0] = 0;
    a->block->cost[0] = 0;
    a->block->path[1] = sourceId;
    a->block->marks[1] = 0;
    a->block->cost[1] = budget;
}

void AnimActionInitTargetActor(AnimAction *a) {
























    if (!a->block || a->block->index != 1 || !a->block->path[1]) AssertFail(animSourcePath, __LINE__, 0);
    TargetObjectInit(fallbackName, &a->to, a->block->path[1]); a->from.Copy(a->position);
    a->frame = 0;
    a->state = 4;
    AnimActionCommitTrajectory(a);
}




int AnimActionHasMetadata(AnimAction *a) {
    return animationMetaAt0cc0[a->kind * 15] != 0;
}
void AnimActionConfigure(AnimAction *a, int frame, int index) {
    if (a->state == 1 && AnimActionHasMetadata(a)) {
        WorldActionNotify(a, 12);
        a->frame = (index < 0) ? frame : 0;
        a->b39 = index; a->b38 = frame;
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

int AnimActionEqual(AnimAction *a, AnimState *s) {
    int equal = 1;
    AnimPair *pair = &s->position;
    if (*pair != a->position) equal = 0;
    pair = &s->target;
    if (*pair != a->target) equal = 0;
    if (s->resource != a->resource) equal = 0;
    if (s->direction != a->direction) equal = 0;
    return equal;
}
void AnimActionCopyChangedFields(AnimAction *a, AnimState *s) {
    AnimPair *pair = &s->position;
    if (*pair != a->position) a->position.Copy(*pair);
    pair = &s->target;
    if (*pair != a->target) a->target.Copy(*pair);
    if (s->resource != a->resource) a->resource = s->resource;
    if (s->direction != a->direction) a->direction = s->direction;
}
