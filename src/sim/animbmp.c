#include <stdio.h>

struct ResMgr { char pad[0x324]; int open; void *Load(long id, int a = 1, int b = 0, int c = 0); };
extern ResMgr gResMgr;
extern void AssertFail(const char *file, int line, const char *message);
extern unsigned long currentFrameCounter;
extern unsigned long animationKindTable[];
extern unsigned char animationFrameTypeTable[];
extern unsigned long animationDefinitionTable[];
extern int modeSwitch;
extern "C" {
    unsigned char animationCachePadding[0x1000];
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
    switch (p[4]) {
    case 1:
        sprintf(animNameBuffer, animFormatPool,
                *(unsigned short *)(p + 12), currentAnimationFrame + 64,
                currentAnimationGroup);
        break;
    case 4:
        sprintf(animNameBuffer, animFormatPool + 0x10, p[5]);
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
    if (p[4] == 1) {
        mode = animationKindTable[*(unsigned short *)(p + 12) * 15];
        moduloFrame = p[6] % 6;
        if (mode == 0) {
            modeFlags = animationDefinitionTable[p[5] * 6];
            currentAnimationFrame = modeFlags & 15;
            currentAnimationGroup = moduloFrame;
            if (modeFlags & 16)
                currentAnimationGroup = 0;
            if (currentAnimationFrame == 7 && modeSwitch != 0)
                currentAnimationFrame = 6;
        } else if (mode < 2) {
            currentAnimationFrame = animationDefinitionTable[p[5] * 6] & 15;
            currentAnimationGroup = moduloFrame;
            if (animationFrameTypeTable[*(unsigned short *)(p + 12) * 60 + currentAnimationFrame] != 6)
                currentAnimationGroup = 0;
            modeFlags = 0;
        } else if (mode == 2) {
            currentAnimationFrame = 1;
            currentAnimationGroup = 0;
            modeFlags = 0;
        }
        resource = animationResourceTable + *(unsigned short *)(p + 12) * 42 + currentAnimationFrame * 6 + currentAnimationGroup;
        animationLastUseTable[*(unsigned short *)(p + 12) * 42 + currentAnimationFrame * 6 + currentAnimationGroup] = currentFrameCounter;
        if (*resource == 0) {
            if (p[4] == 1) {
                sprintf(animNameBuffer, animFormatPool, *(unsigned short *)(p + 12), currentAnimationFrame + 64, currentAnimationGroup);
            } else if (p[4] == 4) {
                sprintf(animNameBuffer, animFormatPool + 0x10, p[5]);
            }
            *resource = (int *)gResMgr.Load((long)animNameBuffer, 0, 0, 0);
            if (*resource == 0)
            *resource = (int *)gResMgr.Load((long)(animFormatPool + 0x1b));
            if (*resource == 0) {
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
                // line number is pinned to the original assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
// source line padding for the original line-120 assertion
                AssertFail(animBmpSourcePath, __LINE__, 0);
            }
        }
        if ((modeFlags & 16) == 0)
            p[9] = (unsigned char)**resource;
        else
            p[9] = 1;
        frameOffset = p[8];
        if (mode == 2) {
            switch (p[9]) {
            case 1: frameOffset = 0; break;
            case 2: frameOffset = p[5] != 1; break;
            case 6: frameOffset = moduloFrame; break;
            case 7: frameOffset = moduloFrame + (p[5] != 1); break;
            }
        }
        if (modeFlags != 0) {
            if (modeFlags & 16)
                frameOffset = moduloFrame;
            if (modeFlags & 32)
                frameOffset = p[9] - p[8] - 1;
        }
        resourceCount = (unsigned long)**resource;
        if (resourceCount <= frameOffset || (int)frameOffset < 0)
            frameOffset = 0;
        return (int *)((char *)*resource + 4 + frameOffset * 12);
    }
    if (p[4] == 4) {
        resource = effectResourceTable + p[5];
        if (*resource == 0) {
                sprintf(animNameBuffer, animFormatPool + 0x10, p[5]);
            *resource = (int *)gResMgr.Load((long)animNameBuffer, 1, 0, 0);
            if (*resource == 0) {
// source line padding for the original line-157 assertion
// source line padding for the original line-157 assertion
// source line padding for the original line-157 assertion
// source line padding for the original line-157 assertion
                AssertFail(animBmpSourcePath, __LINE__, 0);
            }
        }
        effectLastUseTable[p[5]] = currentFrameCounter;
        p[9] = *(unsigned char *)effectResourceTable[p[5]];
        return (int *)((char *)effectResourceTable[p[5]] + 4 + p[8] * 12);
    }
// source line padding for the original line-166 assertion
// source line padding for the original line-166 assertion
    AssertFail(animBmpSourcePath, __LINE__, 0);
    p[9] = 0;
    return 0;
}

void *AnimEnsureFX(int index)
{
    char name[32];
    if (effectResourceTable[index] == 0) {
        sprintf(name, animFormatPool + 0x28, index);
        void *loaded = gResMgr.Load((long)name);
        effectResourceTable[index] = (int *)loaded;
        if (loaded == 0) {
// source line padding for the original line-187 assertion
// source line padding for the original line-187 assertion
// source line padding for the original line-187 assertion
// source line padding for the original line-187 assertion
// source line padding for the original line-187 assertion
// source line padding for the original line-187 assertion
// source line padding for the original line-187 assertion
// source line padding for the original line-187 assertion
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
    int group = 0;
    int row;
    do {
        row = 0;
        int **rowResources = groupResources;
        int rowEnd = group * 0xa8;
        do {
            rowEnd += 0x18;
            int timestampOffset = group * 0xa8 + row * 0x18;
            int **resource = rowResources;
            do {
                if (*resource != 0) {
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
                        f_cb80(*resource);
                        *resource = 0;
                    }
                }
                timestampOffset += 4;
                ++resource;
            } while (timestampOffset != rowEnd);
            rowResources += 6;
            ++row;
        } while (row < 7);
        groupResources += 42;
        ++group;
    } while (group < 0x5f);

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




