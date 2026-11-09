#include <stdio.h>
#include <string.h>
static char animflufFile[] = __FILE__;
struct MenuImage { char bytes[12]; };
struct AnimFrames { unsigned int count; MenuImage frame[1]; };
struct SimAnim { AnimFrames *data; unsigned char kind, number, variant, frame; };
struct ResMgr { void *Load(long, int = 1, int = 0, int = 0); };
extern ResMgr gResMgr;
extern void ReleasePoolData(void *);
extern "C" void AssertFail(const char *, int, const char *);
void SimAnimClear(SimAnim *a);
void SimAnimSet(SimAnim *a, int kind, int number, int variant)
{
    char name[32];
    if (a->kind != kind || a->number != number || a->variant != variant) {
        SimAnimClear(a);
        a->kind = kind; a->number = number; a->variant = variant;
        switch (a->kind) {
        case 1: sprintf(name, "BGMMI%02d%c.ANX", a->number, a->variant + 64); break;
        case 2: sprintf(name, "DMGMIN%02d.BMX", a->number); break;
        }
        a->data = (AnimFrames *)gResMgr.Load((long)name);
    }
    a->frame = 0;
}
void SimAnimAdvance(SimAnim *a)
{
    if (!a->data || a->frame >= a->data->count)






































        AssertFail(animflufFile, __LINE__, 0);
    a->frame++;
    if (a->frame >= a->data->count) a->frame = 0;
}
MenuImage *SimAnimFrame(SimAnim *a, int frame)
{
    if (frame >= 0) a->frame = frame;
    if (!a->data || a->frame >= a->data->count)











        AssertFail(animflufFile, __LINE__, 0);
    return &a->data->frame[a->frame];
}
void SimAnimClear(SimAnim *a)
{
    if (a->data) ReleasePoolData(a->data);
    memset(a, 0, sizeof(*a));
}
