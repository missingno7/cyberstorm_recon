#include <stdio.h>
struct BitmapAction { int id; unsigned char kind,state,variant,rate,frame,frames,flags,dirty; unsigned short type; };struct BitmapKind { unsigned long mode; unsigned char frameTypes[56]; };
struct ResMgr { char pad[0x324]; int open; void *Load(long id, int a = 1, int b = 0, int c = 0); };
extern ResMgr gResMgr;
extern void AssertFail(const char *file, int line, const char *message);
extern unsigned long currentFrameCounter;
extern unsigned long animationKindTable[];
extern unsigned char animationFrameTypeTable[];
extern unsigned long animationDefinitionTable[];
extern int g_004d2dee;
extern "C" {
    char animNameBuffer[16];
    int *animationResourceTable[3990];
    unsigned long animationLastUseTable[3990];
    int *effectResourceTable[70];
    unsigned long effectLastUseTable[70];
    unsigned long currentAnimationFrame;
    unsigned long currentAnimationGroup;
}
char animBmpSourcePath[] = "sim\\animbmp.c";
#pragma data_seg("CONST")
static const char animFormatPool[] =
    "HRC%03d%c%d.ANX\0" "FX%03d.BMX\0" "HRC023B0.ANX\0" "FX%03d.BMX";
#pragma data_seg()

char *AnimBitmapName(void *animation)
{
    unsigned char *p = (unsigned char *)animation;
    switch (((BitmapAction *)p)->kind) {
    case 1:
        sprintf(animNameBuffer, animFormatPool,
                ((BitmapAction *)p)->type, currentAnimationFrame + 64,
                currentAnimationGroup);
        break;
    case 4:
        sprintf(animNameBuffer, animFormatPool + 0x10, ((BitmapAction *)p)->state);
        break;
    }
    return animNameBuffer;
}

int *AnimBitmapFrame(unsigned char *p)
{
// Retain the source lines of the original assert immediates.
    unsigned long mode;
    unsigned long modeFlags;
    unsigned long frameOffset;
    unsigned long moduloFrame;
    int **resource;
    unsigned long resourceCount;
    switch (((BitmapAction *)p)->kind) { case 1: {
        mode = animationKindTable[((BitmapAction *)p)->type * 15];
        moduloFrame = ((BitmapAction *)p)->variant % 6;
        switch (mode) { case 0: {
            modeFlags = animationDefinitionTable[((BitmapAction *)p)->state * 6];
            currentAnimationFrame = modeFlags & 15;
            currentAnimationGroup = (modeFlags & 16) ? 0 : moduloFrame;


            if (currentAnimationFrame == 7 && g_004d2dee != 0)
                currentAnimationFrame = 6;
        break; } case 1: {
            currentAnimationFrame = animationDefinitionTable[((BitmapAction *)p)->state * 6] & 15;
            currentAnimationGroup = moduloFrame;
            if (animationFrameTypeTable[((BitmapAction *)p)->type * 60 + currentAnimationFrame] != 6)
                currentAnimationGroup = 0;
            modeFlags = 0;
        break; } case 2: {
            currentAnimationFrame = 1;
            currentAnimationGroup = 0;
            modeFlags = 0;
        }
        } resource = animationResourceTable + ((BitmapAction *)p)->type * 42 + currentAnimationFrame * 6 + currentAnimationGroup;
        animationLastUseTable[((BitmapAction *)p)->type * 42 + currentAnimationFrame * 6 + currentAnimationGroup] = currentFrameCounter;
        if (*resource == 0) {
            if (((BitmapAction *)p)->kind == 1) {
                sprintf(animNameBuffer, animFormatPool, ((BitmapAction *)p)->type, currentAnimationFrame + 64, currentAnimationGroup);
            } else if (((BitmapAction *)p)->kind == 4) {
                sprintf(animNameBuffer, animFormatPool + 0x10, ((BitmapAction *)p)->state);
            }
            *resource = (int *)gResMgr.Load((long)animNameBuffer, 0, 0, 0);
            if (*resource == 0)
            *resource = (int *)gResMgr.Load((long)(animFormatPool + 0x1b));
            if (*resource == 0) {
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
                AssertFail(animBmpSourcePath, __LINE__, 0);
            }
        }
        if ((modeFlags & 16) == 0)
            ((BitmapAction *)p)->frames = (unsigned char)**resource;
        else
            ((BitmapAction *)p)->frames = 1;
        frameOffset = ((BitmapAction *)p)->frame;
        if (mode == 2) {
            switch (((BitmapAction *)p)->frames) {
            case 1: frameOffset = 0; break;
            case 2: frameOffset = ((BitmapAction *)p)->state != 1; break;
            case 6: frameOffset = moduloFrame; break;
            case 7: frameOffset = moduloFrame + (((BitmapAction *)p)->state != 1); break;
            }
        }
        if (modeFlags != 0) {
            if (modeFlags & 16)
                frameOffset = moduloFrame;
            if (modeFlags & 32)
                frameOffset = ((BitmapAction *)p)->frames - ((BitmapAction *)p)->frame - 1;
        }
        resourceCount = (unsigned long)**resource;
        if (resourceCount <= frameOffset || (int)frameOffset < 0)
            frameOffset = 0;
        return (int *)((char *)*resource + 4 + frameOffset * 12);
    }
    case 4: {
        resource = effectResourceTable + ((BitmapAction *)p)->state;
        if (*resource == 0) {
                sprintf(animNameBuffer, animFormatPool + 0x10, ((BitmapAction *)p)->state);
            *resource = (int *)gResMgr.Load((long)animNameBuffer, 1, 0, 0);
            if (*resource == 0) {
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
                AssertFail(animBmpSourcePath, __LINE__, 0);
            }
        }
        effectLastUseTable[((BitmapAction *)p)->state] = currentFrameCounter;
        ((BitmapAction *)p)->frames = *(unsigned char *)effectResourceTable[((BitmapAction *)p)->state];
        return (int *)((char *)effectResourceTable[((BitmapAction *)p)->state] + 4 + ((BitmapAction *)p)->frame * 12);
    }
// Original assertion source-line padding.
// Original assertion source-line padding.
    default: AssertFail(animBmpSourcePath, __LINE__, 0);
    ((BitmapAction *)p)->frames = 0;
    return 0; }
}

void *AnimEnsureFX(int index)
{
    char name[32];
    if (effectResourceTable[index] == 0) {
        sprintf(name, animFormatPool + 0x28, index);
        void *loaded = gResMgr.Load((long)name);
        effectResourceTable[index] = (int *)loaded;
        if (loaded == 0) {
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
// Original assertion source-line padding.
            AssertFail(animBmpSourcePath, __LINE__, 0);
        }
    }
    effectLastUseTable[index] = currentFrameCounter;
    return effectResourceTable[index];
}

extern "C" void f_cb80(void *data);

int AnimBitmapSweep(int rowEarlyAge, int rowLateAge, int outerAge,
                    int innerAge, int effectAge)
{
int **groupResources = animationResourceTable;
    int group;
    int row;
    for (group = 0; group < 0x5f; ++group, groupResources += 42) {
        row = 0;
        int **rowResources = groupResources;
        int rowEnd = group * 0xa8;
        do {
            rowEnd += 0x18;
            int timestampOffset = group * 0xa8 + row * 0x18;
            int **resource = rowResources;
            do {
                int *loaded = *resource;
                if (loaded != 0) {
                    int ordinaryGroup = (group < 8) || (group > 0x1a);
                    int rowAge = rowLateAge;
                    if (row < 3)
                        rowAge = rowEarlyAge;
                    int groupAge = innerAge;
                    if (ordinaryGroup)
                        groupAge = outerAge;
                    int expiration = *(int *)((char *)animationLastUseTable + timestampOffset)
                                   + rowAge + groupAge;
                    if (expiration < (int)currentFrameCounter) {
                        f_cb80(loaded);
                        *resource = 0;
                    }
                }
                timestampOffset += 4;
                ++resource;
            } while (timestampOffset != rowEnd);
            rowResources += 6;
            ++row;
        } while (row < 7);
    }

    group = 0;
    do {
        int **resource = (int **)((char *)effectResourceTable + group);
        if (*resource != 0 &&
            (row = *(int *)((char *)effectLastUseTable + group) + effectAge) <
                (int)currentFrameCounter) {
            f_cb80(*resource);
            *resource = 0;
        }
        group += 4;
    } while (group != 0x118);
    return row;
}

