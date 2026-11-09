struct MissBrief {
    char pad00[0x10];
    int active;
    int IsActive() const { return active != 0; }
    void Begin(void *callback, unsigned char number);
};

extern "C" void TimerSet(void *timer, int value); extern "C" int BriefObjectCreate(void *owner, void *descriptor, int count, int flags); extern "C" int BriefObjectRelease(char *object);
extern "C" void BriefCloseWait(); extern "C" void BriefCloseAudio(); extern "C" void BriefCloseScreen(char *object); extern "C" int BriefGetHandle(char *object, void *resource); extern "C" void BriefStateSet(char *object); extern "C" int BriefSwitch(char *object, void *resource);
extern "C" void BriefBlit(void *surface, int a, int b, int c, int d, int e, int f, int g, int h); extern "C" int BriefLookup(int table, int number); extern "C" int BriefMissionFile(int table, int number); extern "C" int BriefFormat(const char *format, ...);
extern "C" int BriefRender(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p); extern "C" void BriefStatus(char *object); extern "C" void BriefRenderUpdate(char *object); extern "C" void BriefDone(const char *name);
extern "C" void BriefWaitA(int value); extern "C" void BriefWaitB(int value); extern "C" void AssertFail(const char *file, int line, const char *message); extern "C" int BriefOpenFile(char *object, const char *name); extern "C" void BriefOpenResource(char *object, void *callback, void *resource, int a, int b);
extern "C" void BriefSetMode(void *control, int mode); extern "C" void BriefAdjust(int surface, int y); extern "C" void BriefPlayAnimation(char *object, const char *name, int frames, int flags); extern "C" void BriefCopyHeader(void *object, void *format); extern "C" void BriefDrawCurrent(void *surface, int a, int b, int c, int d, int e, int f, int g, int h); extern "C" void BriefProcessData(void *data);

void MissBriefDrawMap(MissBrief *screen);

extern int g48ea70, g48ea50, g4bb78c, g513888;
extern int g48ea28[2], g48ea2c[2];
extern int gBriefStatus, gBriefSurface, gBriefTable, gBriefLookupTable;
extern int gBriefResource, gBriefCanvas, gBriefControlA, gBriefDrawTarget, gBriefDrawSource;
extern int gBriefDrawA, gBriefDrawB, gBriefHeaderFormat;
extern char gBriefDataBuffer[];
extern char g48a9df[];
extern int g48ea1c, g48ea24, g4b3290, g4b32b8, g4b32e0;
extern int g4b3308, g4b3310, g4b3338, g4b3364, g4b3390;
extern "C" void BriefModuleConstruct(void *object);
extern "C" void BriefModuleInitA(void *object, int a, int b);
extern "C" void BriefModuleInitB(void *object);
extern "C" void BriefModuleInitC(void *object, int a, int b);
extern "C" void BriefModuleInitD(void *object, void *surface, int value);
extern "C" void BriefModulePlace(void *object, void *surface, int x, int y);

class BriefFactory {
public:
    int GetObject();
};

void MissBriefModuleInit()
{
    BriefModuleConstruct(&g48ea1c);
    BriefModuleInitA(&g4b3290, 0, 0);
    g48ea24 = 1; BriefModuleInitA(&g4b32b8, 0, 0);
    g48ea24 = 2; BriefModuleInitB(&g4b32e0);
    g48ea24 = 3; BriefModuleInitC(&g4b3308, 0x32, 0);
    BriefModuleInitD(&g4b3310, &g4b3308, 0);
    g48ea24 = 4; BriefModulePlace(&g4b3338, &g4b3308, 0x14, 0x30); g48ea24 = 5;
    BriefModulePlace(&g4b3364, &g4b3308, -0x14, 0x2d); g48ea24 = 6;
    ((BriefFactory *)&g4b3390)->GetObject(); g48ea24 = 7;
}

int BriefFactory::GetObject()
{
    register void *owner = this;
    char *timer = (char *)BriefObjectCreate(owner, &g48ea70, 6, 0) + 0x54;
    TimerSet(timer, 0);
    return BriefObjectRelease(timer + 0x0c) - 0x60;
}
/* AssertFail remains on source line 68. */ struct BriefMissionEntry { unsigned long words[11]; };
void MissBrief::Begin(void *callback, unsigned char number)
{
    if (IsActive())
        return;
    BriefWaitA(6);
    BriefWaitB(0x19);
    BriefCloseAudio();
    gBriefStatus = -1;
    if (number > 9)
        AssertFail(__FILE__, 68, 0);
    ((unsigned char *)this)[0x50] = number;
    BriefMissionEntry *entry = (BriefMissionEntry *)BriefLookup(gBriefTable, number);
    *(BriefMissionEntry *)((char *)this + 0x100) = *entry;
    ((unsigned char *)this)[0x53] = ((unsigned char *)this)[0x101];
    ((unsigned char *)this)[0x51] = ((unsigned char *)this)[0x100] + 1;
    *(int *)((char *)this + 0x12c) = BriefMissionFile(gBriefLookupTable,
                                                       ((unsigned char *)this)[0x50]);
    *(int *)((char *)this + 0xf0) = 0;
    BriefOpenFile((char *)this, "missbrf.box");
    register void *callbackCopy = callback;
    BriefOpenResource((char *)this, callbackCopy, &gBriefResource, 0xa8d, 1);
    BriefSetMode(&gBriefSurface, 0);
    *(int *)((char *)this + 0xf4) = 0;
    MissBriefDrawMap(this);
    BriefBlit(&gBriefCanvas, 0, 1, 0x140, 0, 0x140, 0, 0x11d, 0x1e0);
    MissBriefDrawMap(this);
    BriefAdjust(gBriefControlA, *(int *)((char *)this + 0xf8) - 0x165);
}

void MissBriefClose(MissBrief *screen)
{
    if (screen->IsActive()) {
        screen->active = 0;
        BriefCloseWait();
        BriefCloseAudio();
        BriefCloseScreen((char *)screen + 0x60);
    }
}

void MissBriefExit(MissBrief *screen)
{
    if (screen->IsActive()) {
        *(int *)((char *)screen + 0xf4) = 1;
        gBriefStatus = BriefGetHandle((char *)screen, &g4bb78c);
        BriefStateSet((char *)screen);
    }
}

void MissBriefUpdate(MissBrief *screen)
{
    if (screen->IsActive()) {
        extern int g511948;
        BriefSwitch((char *)screen + 0x60, &g511948);
        int oldSurface = *(int *)((char *)screen + 0xf0);
        int newSurface = gBriefSurface;
        *(int *)((char *)screen + 0xf0) = newSurface;
        if (oldSurface != newSurface) {
            BriefBlit(&g513888, 1, 0, 0x140, 0, 0x140, 0, 0x11d, 0x1e0);
            MissBriefDrawMap(screen);
            BriefStatus((char *)screen);
            BriefRenderUpdate((char *)screen);
        }
        BriefRenderUpdate((char *)screen);
    }
}

void MissBriefOrphan()
{
    BriefDone(g48a9df);
}

void MissBriefDrawMap(MissBrief *screen)
{
    int tileStep = 2;
    for (int i = 0; ; i += 2) {
        int empty = (g48ea28[i] | g48ea2c[i]) == 0;
        BriefRender(g48ea28[i] + 0x140, g48ea2c[i] + 0x1e, 0x11d, 0x179,
                    *(int *)((char *)screen + 0x12c),
                    *(int *)((char *)screen + 0xf0), -1, 6,
                    (int)((char *)screen + 0xf8),
                    (int)((char *)screen + 0xfc), (int)&g48ea50,
                    empty ? 0x10 : 0, tileStep, !empty, 0, 0x7fffffff);
        if (empty)
            break;
    }
}

struct BriefHeader { unsigned long words[4]; };
struct BriefCopyArea { unsigned long words[0x114]; unsigned short tail; };

void MissBriefLoadBackground(MissBrief *screen)
{
    char *name = (char *)BriefFormat("BG_%1d%02d%c.FLX",
                                     ((unsigned char *)screen)[0x51],
                                     ((unsigned char *)screen)[0x53],
                                     ((unsigned char *)screen)[0x52] + 0x41);
    BriefPlayAnimation((char *)screen + 0x60, name, 0x64, 0);
    BriefCloseWait();
    BriefCloseAudio();
    char *region = (char *)screen + 0x80;
    if (*(int *)(region + 4) == 0)
        return;
    *(BriefHeader *)&gBriefDrawTarget = *(BriefHeader *)&gBriefDrawSource;
    BriefCopyHeader((char *)screen + 0x60, &gBriefHeaderFormat);
    BriefDrawCurrent(&gBriefCanvas, 0, 1, 0, 0, 0, 0,
                     gBriefDrawA, gBriefDrawB);
    char *source = *(char **)(region + 0x3c);
    *(BriefCopyArea *)gBriefDataBuffer = *(BriefCopyArea *)source;
    BriefProcessData(gBriefDataBuffer);
    *(int *)(region + 0x2c) = -1;
}
