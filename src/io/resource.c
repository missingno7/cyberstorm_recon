#include <windows.h>
#include <mmsystem.h>
extern "C" char *FindChar(const char *, int);

extern "C" int MakePath(void *, const char *, const char *, int);
extern "C" void AssertFail(const char *, int, const char *); extern "C" int sprintf(char *, const char *, ...);

struct ResMgr {
    int count;
    int types[10];
    const char *paths[10];
    char entries[10][0x48];
    int loaded;
    int opened;
    char pad2[4];
    char file[1];
    int IsOpen();
    int Init(const char ** volatile, const char *);
    void Close();
    int FindPath(const char *, char *, int);
    int CloseFile();
};

struct MciState {
    unsigned int device;
    unsigned int pending;
    unsigned char pad[8];
    unsigned int state;
    MciState *StopClose();
    void Close();
};

extern "C" int f_c530() { return 0x530; }
extern "C" int f_c580() { return 0x580; }
int ResMgr::Init(const char ** volatile paths, const char *outDir)
{































    if (this == 0 || loaded != 0 || opened != 0)
        AssertFail(__FILE__, __LINE__, 0);
    register const char **source = paths;
    register int i = 0;
    register char *entry = (char *)entries;
    count = 0;
    while (*source) {
        count = i + 1;
        types[i] = 1;
        this->paths[i] = *source;
        if (FindChar(*source, '.')) {
            types[i] = 2;
            if (!MakePath(entry, *source, outDir, 1))
                AssertFail(__FILE__, __LINE__, "Unable to open game data file!");
        }
        ++source;
        ++i;
        entry += 0x48;
        if (i >= 10)
            break;
    }
    loaded = 1;
    return loaded;
}
extern "C" void IORelease(void *);
void ResMgr::Close()
{
    if (opened) {
        ResMgr *owner = this;
        if (opened)
            IORelease((char *)owner + 0x330);
        opened = 0;
    }
    if (loaded)
        loaded = 0;
}
extern "C" char *StringCopy(char *, const char *);
extern "C" char *StringAppend(char *, const char *);
extern "C" int FileExists(char *, char *, int);
extern "C" int FileOpen(char *, const char *, char *, int);
int ResMgr::FindPath(const char *suffix, char *destination, int flags)
{
    char buffer[256];
    register char *data;
    register char *dest = destination;
    register int i = 0;
    int n = count;
    if (n <= 0)
        return 0;
    int *entry = (int *)this;
    data = (char *)entries;
    do {
        if (entry[1] == 1) {
            StringCopy(buffer, *(const char **)((char *)entry + 0x2c));
            StringAppend(buffer, suffix);
            if (FileExists(dest, buffer, 0))
                return 1;
        } else if (entry[1] == 2) {
            if (FileOpen(data, suffix, dest, flags))
                return 1;
        }
        ++entry;
        ++i;
        data += 0x48;
    } while (i < count);
    return 0;
}
extern "C" int f_c7e0(ResMgr *self, const char *name, int required)
{
    char path[128];
    if (self->FindPath(name, (char *)self + 0x330, 0)) {
        self->opened = 1;
        return 1;
    }
    if (required) {
        char message[128];
        sprintf(message, "Unable to open %s", name);
        /* source line padding 1 */
        /* source line padding 2 */
        /* source line padding 3 */
        /* source line padding 4 */
        /* source line padding 5 */
        /* source line padding 6 */
        /* source line padding 7 */
        /* source line padding 8 */
        /* source line padding 9 */
        /* source line padding 10 */
        /* source line padding 11 */
        /* source line padding 12 */
        /* source line padding 13 */
        /* source line padding 14 */
        /* source line padding 15 */
        /* source line padding 16 */
        /* source line padding 17 */
        /* source line padding 18 */
        /* source line padding 19 */
        /* source line padding 20 */
        /* source line padding 21 */
        /* source line padding 22 */
        /* source line padding 23 */
        /* source line padding 24 */
        /* source line padding 25 */
        /* source line padding 26 */
        /* source line padding 27 */
        /* source line padding 28 */
        /* source line padding 29 */
        /* source line padding 30 */
        /* source line padding 31 */
        /* source line padding 32 */
        /* source line padding 33 */
        /* source line padding 34 */
        /* source line padding 35 */
        /* source line padding 36 */
        /* source line padding 37 */
        /* source line padding 38 */
        /* source line padding 39 */
        /* source line padding 40 */
        /* source line padding 41 */
        /* source line padding 42 */
        /* source line padding 43 */
        /* source line padding 44 */
        /* source line padding 45 */
        /* source line padding 46 */
        /* source line padding 47 */
        /* source line padding 48 */
        /* source line padding 49 */
        /* source line padding 50 */
        /* source line padding 51 */
        /* source line padding 52 */
        /* source line padding 53 */
        /* source line padding 54 */
        /* source line padding 55 */
        AssertFail(__FILE__, __LINE__, message);
    }
    return 0;
}
int ResMgr::CloseFile()
{
    if (opened)
        IORelease((char *)this + 0x330);
    opened = 0;
    return 1;
}
int ResMgr::IsOpen() { return opened; }
extern "C" int IoRead(void *, int, int);
extern "C" int IoClose(void *);
extern "C" int IoWrite(void *, int, int);
extern "C" int f_c8a0(void *self, int offset, int size)
{
    return IoRead((char *)self + 0x330, offset, size);
}
extern "C" int f_c8c0(void *self)
{
    return IoRead((char *)self + 0x330, 0, 1);
}
extern "C" int f_c8e0(void *self)
{
    return IoClose((char *)self + 0x330);
}
extern "C" int f_c900(void *self, int offset, int size)
{
    return IoWrite((char *)self + 0x330, offset, size);
}
extern "C" int ResMgr_Load(ResMgr *self) { return 0x920; }
extern "C" void MemFree(void *);
extern "C" void f_cb80(void *p)
{
    if (p)
        MemFree(p);
}
extern "C" int f_cba0() { return 0xba0; }
extern "C" void f_cbf0(void **p)
{
    if (*p) {
        f_cb80(*p);
        *p = 0;
    }
}
struct ResourceHandle { void *resource; };
extern "C" unsigned int f_cc10(ResourceHandle *p, const unsigned char *name)
{
    unsigned int sum;
    unsigned char ch;
    unsigned char *table;
    if (p->resource == 0)
        goto empty;
    if (name == 0)
        goto empty;
    sum = 0;
    ch = *name;
    table = *(unsigned char **)((char *)p->resource + 0x20);
    if (ch) {
        do {
            sum += table[*name++];
        } while (*name);
    }
    return sum;
empty:
    sum = 0;
    return sum;
}
extern "C" int f_cc50() { return 0xc50; }
extern "C" int f_cf80() { return 0xf80; }
extern "C" int f_cfc0() { return 0xfc0; }
extern "C" int f_d080() { return 0x1080; }
extern "C" int f_d0e0() { return 0x10e0; }
extern "C" void f_d110(MciState *s)
{
    s->device = 0;
    s->pending = 0;
    s->state = 0;
}
MciState *MciState::StopClose()
{
    register MciState *deviceState = this;
    if (device) {
        register MciState *statusState = this;
        if (device)
            mciSendCommandA(device, MCI_STOP, 0, 0);
        statusState->state = 0;
        mciSendCommandA(deviceState->device, MCI_CLOSE, 0, 0);
        deviceState->device = 0;
    }
    return this;
}
extern "C" int f_d180() { return 0x1180; }
void MciState::Close()
{
    if (device) {
        register MciState *statusState = this;
        if (device)
            mciSendCommandA(device, MCI_STOP, 0, 0);
        statusState->state = 0;
        mciSendCommandA(device, MCI_CLOSE, 0, 0);
        device = 0;
    }
}
extern "C" int f_d270(MciState *s)
{
    if (!s->device)
        return 0;
    MCI_STATUS_PARMS status;
    status.dwItem = 4;
    mciSendCommandA(s->device, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (unsigned long)&status);
    if (status.dwReturn == 0x20e)
        return 0;
    return 1;
}
extern "C" int f_d310(MciState *, int, int, int, int);
extern "C" void f_d2c0(MciState *s)
{
    if (s->device)
        mciSendCommandA(s->device, MCI_STOP, 0, 0);
    s->state = 0;
}
extern "C" int f_d2f0(MciState *s, int start)
{
    return f_d310(s, start, 0, start + 1, 0);
}
extern "C" int f_d310(MciState *s, int from, int flags, int to, int volume) { return 0x1310; }
