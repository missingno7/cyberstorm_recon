struct HercBay {
    char pad00[0x10];
    int active;
    int IsActive() const { return active != 0; }
    void Start(void *callback);
};

int HercObjectCreate(int id, void *resource, int count, int flags);
int HercObjectRelease(char *object);
extern "C" void HercCloseScreen(char *object);
extern "C" int HercGetHandle(char *object, void *resource);
extern "C" void HercRefresh(char *object);
extern "C" void HercUpdate(char *object);
extern "C" void HercSelect(int mode);
extern "C" int HercOpenFile(char *object, const char *name);
extern "C" void HercOpenResource(char *object, void *callback, void *resource, int id, int flags);
extern "C" void HercWait(int value);
extern "C" void HercResetAudio();
extern "C" void HercPrepareAssets();
extern "C" int HercLoadResource(void *manager, const char *name, int a, int b, int c);
extern "C" int HercLockStrings(void *pool);
extern "C" int HercGetResource(void *manager, int id, int flags);
extern "C" void HercSetResource(void *control, int resource);
extern "C" void HercSetState(void *control, int state);
extern "C" void HercSetControlState(void *control, int state);
extern "C" void HercPlaySound(int id, int flags);
extern "C" void HercSetMission(void *control, int value);
extern "C" void HercAssertFail(const char *file, int line, const char *message);
extern "C" void HercSetDisplay(void *display, int a, int b);
extern "C" void HercDraw16(int a, int b, int c, int d, int e, int f, int g, int h,
                             int i, int j, int k, int l, int m = 1, int n = 0,
                             int o = 0, int p = 0x7fffffff);
extern "C" void HercPlayAnimation(char *object, const char *name, int frames, int flags);
extern "C" void HercDone(const char *name);

extern int g4b3748;
extern int g48eb10;
extern int g4b37bc;
extern int g4bb78c;
extern int g4c9772;
extern int g511e68;
extern int g4d030f;
extern int g4d0413;
extern int g4b36b0;
extern int g4b36dc;
extern int g4b3708;
extern int g4b3734;
extern int g4b369c;
extern int g4b36c8;
extern int g4b36f4;
extern char g4b3777;
extern int g511ecc;
extern int g511dd4;
extern int g511e18;
extern int g511e68;
extern int g4b74f4;
extern char g48aa2c[];
extern int g48eaf4, g48eafc, g4b3720, g4b3778, g4b379c, g4b37c4;
extern "C" void HercModuleConstruct(void *object);
extern "C" void HercModuleInitWidget(void *object, int a, int b, int c, int d);
extern "C" void HercModuleInitA(void *object, int a, int b);
extern "C" void HercModuleInitB(void *object, int a, int b);
extern "C" void HercModuleInitC(void *object);
extern "C" void HercModuleInitD(void *object);
int HercBayGetObject(int id);

void HercBayModuleInit()
{
    HercModuleConstruct(&g48eaf4);
    HercModuleInitWidget(&g4b369c, 0, 1, 2, -1); g48eafc = 1;
    HercModuleInitWidget(&g4b36c8, 0, 1, 2, -1); g48eafc = 2;
    HercModuleInitWidget(&g4b36f4, 0, 1, 2, -1); g48eafc = 3;
    HercModuleInitA(&g4b3720, 0, 0); g48eafc = 4;
    HercModuleInitB(&g4b3748, -1, 0x12); g48eafc = 5;
    HercModuleInitC(&g4b3778); g48eafc = 6;
    HercModuleInitD(&g4b379c); g48eafc = 7;
    HercBayGetObject((int)&g4b37c4); g48eafc = 8;
}

int HercBayGetObject(int id)
{
    char *object = (char *)HercObjectCreate(id, &g48eb10, 7, 0);
    return HercObjectRelease(object + 0x58) - 0x58;
}

void HercBay::Start(void *callback)
{
    if (IsActive())
        return;
    HercSelect(g4c9772 == 1 ? 2 : 9);
    HercWait(0xc);
    HercResetAudio();
    HercPrepareAssets();
    g4b37bc = -1;
    int loaded = HercLoadResource(&g511ecc, "hercgui.bmx", 1, 0, 0);
    g511dd4 = loaded;
    if (loaded == 0)
    {


        HercAssertFail(__FILE__, 101, 0);
    }
    HercOpenFile((char *)this, "hercbay.box");
    HercOpenResource((char *)this, callback, &g511e68, 0x709, 1);
    HercPlayAnimation((char *)this + 0x58, "hb_idle.flx", 0x64, 0);
    *(int *)((char *)this + 0xac) = -1;
    HercLockStrings(&g511e18);
    *(int *)((char *)this + 0xec) = 0;
    *(int *)((char *)this + 0xe8) = 0;
    HercSetResource(&g4b369c, HercGetResource(&g511e68, 0x709, 1));
    HercSetResource(&g4b36c8, HercGetResource(&g511e68, 0x70a, 1));
    HercSetResource(&g4b36f4, HercGetResource(&g511e68, 0x70b, 1));
    HercSetState(&g4b369c, 2);
    HercSetState(&g4b36c8, 2);
    HercSetState(&g4b36f4, 2);
    g4b3777 = 1;
    g4b3734 = (g4b3734 & 0xff1fffff) + 0xa00000;
    HercSetDisplay(&g4b3748, g4d0413, 0);
    *(int *)((char *)this + 0x4c) = g4d030f;
    HercPlaySound(0x33, 0);
    HercSetMission(&g4b74f4, 0x927);
}

void HercBayConfigureControls()
{
    HercSetResource(&g4b369c, HercGetResource(&g511e68, 0x709, 1));
    HercSetResource(&g4b36c8, HercGetResource(&g511e68, 0x70a, 1));
    HercSetResource(&g4b36f4, HercGetResource(&g511e68, 0x70b, 1));
    HercSetControlState(&g4b369c, 2);
    HercSetControlState(&g4b36c8, 2);
    HercSetControlState(&g4b36f4, 2);
    g4b3777 = 1;
    g4b3734 = (g4b3734 & 0xff1fffff) + 0xa00000;
}

void HercBayAltStart(HercBay *bay, void *callback)
{
    HercSelect(g4c9772 == 1 ? 2 : 9);
    g4b37bc = -1;
    HercOpenFile((char *)bay, "hercbay.box");
    HercOpenResource((char *)bay, callback, &g511e68, 0x709, 1);
    HercUpdate((char *)bay);
    HercPlayAnimation((char *)bay + 0x58, "hb_idle.flx", 0x64, 0);
    *(int *)((char *)bay + 0xa4) = -1;
    HercSetDisplay(&g4b3748, g4d0413, 0);
}

void HercBayClose(HercBay *bay)
{
    if (bay->IsActive()) {
        HercCloseScreen((char *)bay + 0x58);
        bay->active = 0;
    }
}

void HercBaySetStatus(HercBay *bay)
{
    *(int *)((char *)bay + 0x4c) = g4d030f;
    if (bay->IsActive()) {
        g4b37bc = HercGetHandle((char *)bay, &g4bb78c);
        HercRefresh((char *)bay);
    }
}

void HercBaySetDisplay()
{
    register int value = g4d0413;
    HercSetDisplay(&g4b3748, value, 0);
}

class HercFrame {
public:
    void Draw(int display);
};

void HercFrame::Draw(int display)
{
    int local[2];
    HercDraw16(0xe6, 0xb9, 0xaa, 0x4b, display, 0, -1, 0,
               (int)&local[0], (int)&local[1], 0, 0xf);
}

inline int Read4b36b0() { return g4b36b0; }
inline int Read4b36dc() { return g4b36dc; }
inline int Read4b3708() { return g4b3708; }

class HercBayActions {
public:
    void Reactivate(HercBay *bay);
};

void HercBayActions::Reactivate(HercBay *bay)
{
    g4b3734 = (g4b3734 & 0xff1fffff) + 0xa00000;
    g4b36b0 = Read4b36b0() & 0xff1fffff;
    g4b36dc = Read4b36dc() & 0xff1fffff;
    g4b3708 = Read4b3708() & 0xff1fffff;
    *(int *)((char *)bay + 0xec) = 1;
    HercPlayAnimation((char *)bay + 0x58, "hb_idle.flx", 0x64, 0);
    HercUpdate((char *)bay);
}

void HercBayDone()
{
    HercDone(g48aa2c);
}
