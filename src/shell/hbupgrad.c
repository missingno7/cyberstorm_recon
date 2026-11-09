#include <stdio.h>
#include <string.h>

class GuiControl {
    char pad[0x1c];
public:
    void *vtable;
    GuiControl(unsigned long, unsigned long);
};
extern char g48d394[];
GuiControl *GuiControlCtor(GuiControl *, unsigned long, unsigned long);
#pragma aux GuiControlCtor "W?$ct:GuiControl$n(ulul)_"
class GameSession;
extern GameSession gShellGame;
extern char gShellStrings[];
extern long g513888;
extern long g4a706d;
extern long g4ae17f;
extern int profileManager[];
extern int g4b709c;
extern int g4b7084;
extern int g4b7088;
extern int g4bb78c;
extern unsigned char g4b5f47;
extern unsigned char g4b5f1f;
extern unsigned char g4b5f97;
extern unsigned char g4b5f6f;
extern unsigned char g4b6dac;
extern int g4b6e34;
extern unsigned int g4b6d24;
extern char g4b5e30[];
extern char g4b5e3c[];
extern char g4b5e88[];
extern char g4b5e94[];
extern char g4b5dd4[];
extern char g4b6034[];
extern char g4b6428[];
extern char *g4b6028;
extern unsigned long g4b6ea4;
extern unsigned long g4b6e50;
extern unsigned long g4b6e78;
extern unsigned long g4b6b68;
extern unsigned long g4b6b90;
extern unsigned long g4b6bb8;
extern unsigned long g4b6be0;
extern unsigned long g4b6c44;
extern unsigned long g4b6c14;
extern unsigned long g4b6c78;
extern unsigned long g4b6b18;
extern unsigned long g4b6b40;
extern unsigned long g4b5fe4;
extern unsigned long g4b6018;
extern char g48ab38[];
extern char g48ab34[];
extern char g48ab3c[];
extern char g48ab40[];
extern char g48ab43[];
extern char *gShellImages;
struct PointPair { int x; int y; };
extern PointPair g48eec4;
extern unsigned char g50f944[];
extern unsigned char g4b6d43;
extern unsigned char g4b6f63;
extern unsigned char g4b6f93;

extern "C" void BriefBlit(void *, unsigned long, unsigned long, unsigned long,
                          unsigned long, unsigned long, unsigned long, unsigned long,
                          unsigned long);
extern "C" void f_0043ba3e(void *, void *);
extern "C" void f_0043cf2d(void *, unsigned int);
extern "C" int f_0043cec6(void *, unsigned int);
extern "C" int f_0043cef6(void *);
extern "C" char *f_0047c350(void *, unsigned int, int);
extern "C" int f_0047723b(char *);
extern "C" long f_0043cdcc(void *, unsigned int);
extern "C" void f_00419f3c(int *);
int SaveHercConfig(void *, unsigned int);
void *GlobalBlockFree(void *);
class StrPool { public: void Unlock(); };
extern StrPool gMenuPrefixStrings;
extern long g4ae07b;
int SoundTrigger(unsigned int, unsigned int);
void SurfacePresent(void *);

class Shell {
public:
    int UpdateCredits();
};
extern Shell uiSystem;
class MenuNumber {
public:
    void SetValue(long, char);
};
extern MenuNumber g4b6d2c;
extern char g4b5dc0[];
extern unsigned char g4b6d73;
class ProfileManager { public: void *Mission(int); };
class MenuPrefixButton { public: void SetText(const char *); };
void MenuPrefixSetText(MenuPrefixButton *, const char *);
#pragma aux MenuPrefixSetText "W?SetText$:MenuPrefixButton$n(pnxa)v"
extern MenuPrefixButton g4b6d5c;
extern "C" char *f_00439d2c(int);
class MenuLabel {
public:
    void SetText(const char *);
};
extern MenuLabel g4b6c30;
extern MenuLabel g4b6bcc;
extern MenuLabel g4b5fd0;

extern "C" void f_0042a7cc(int, int, int, int, unsigned char *, int, int, int, int *, int *, int *, int, int, int, int, int);
extern "C" void f_0047bff0(void *, int *, int, int);
extern "C" void f_0047b3a0(void *);
extern "C" int f_00406fc9(void *);
extern "C" void f_0041c096(void *);
extern "C" void f_00425e1a(void *);
extern "C" void *f_00441dad(void *, unsigned int, int *);
extern "C" unsigned int f_0042604d(void *, void *);
extern "C" void f_0041b37c(void *);
extern "C" void f_00425ba1(void *);
extern "C" void f_00425f1c(void *);













void f_00419d52(char *);
class FileRecord;
int IORelease(FileRecord *);
FileRecord *FileInitReturn(FileRecord *);
#pragma aux FileInitReturn "W?FileInit$n(pn$FileRecord$$)v"
unsigned long StreamRead(FileRecord *, void *, unsigned long);
class Timer;
void TimerSet(Timer *, unsigned long = 0);
int TimerExpired(Timer *);
class GuiRect;
void MenuSetClip(GuiRect *, int, int, int, int);
class MenuImage;
void MenuDrawImage(void *, MenuImage *, int, int);
extern unsigned long g5138fc;
extern unsigned long g513900;
extern unsigned long g513904;
extern unsigned long g513908;
extern unsigned long g51390c;
extern unsigned long g513910;
extern unsigned long g513914;
extern unsigned long g513918;
extern unsigned long g51391c;
extern void *g5138e8;
class Flic;
Flic *FlicCtor(Flic *);
#pragma aux FlicCtor "W?$ct:Flic$n()_"
class GuiScreen;
GuiScreen *GuiScreenCtor(GuiScreen *, const char *, unsigned int, unsigned long);
#pragma aux GuiScreenCtor "W?$ct:GuiScreen$n(pnpn$GuiControl$$uiul)_"
extern char g48ed3c[];

int f_004193a2(int param_1)
{
    register GuiScreen *object = (GuiScreen *)param_1;
    GuiScreen *screen = GuiScreenCtor(object, g48ed3c, 0x62, 0);
    FileRecord *file = FileInitReturn((FileRecord *)((char *)screen + 0x4c));
    char *timer = (char *)file + 0x34;
    TimerSet((Timer *)timer);
    char *timer2 = timer + 0x116;
    TimerSet((Timer *)timer2, 0);
    timer = timer2;
    timer += 0x29b;
    Flic *flic = FlicCtor((Flic *)timer);
    return (int)flic - 0x431;
}

void f_004193fa(void *param_1)
{
    GuiControlCtor((GuiControl *)param_1, 8, 0)->vtable = g48d394;
}

void f_00419412(void)
{
}

int f_00419413(int *param_1)
{
    if (g4b709c) {
        int threshold = g4bb78c;
        int base = param_1[0];
        int percent = param_1[2];
        int low = base + (percent * 0x2a) / 100;
        int high = base + (param_1[2] * 0x3a) / 100;
        int total = param_1[2] + base;
        int difference;
        if (threshold < low) {
            g4b7084 = 1;
            difference = threshold - base;
        } else if (threshold <= high) {
            g4b7084 = 0;
            g4b7088 = 0x53;
            return 0;
        } else {
            g4b7084 = -1;
            difference = total - threshold;
        }
        g4b7088 = (difference * 0x96) / 100 + 5;
    }
    return 0;
}

unsigned long f_00419875(char *param_1)
{
    register unsigned long result = *(long *)(param_1 + 0x10) != 0;
    if (result) {
        char *file = param_1;
        if (*(void **)(param_1 + 0x68) != 0) {
            GlobalBlockFree(*(void **)(param_1 + 0x68));
        }
        IORelease((FileRecord *)(file + 0x4c));
        *(long *)(param_1 + 0x10) = 0;
        gMenuPrefixStrings.Unlock();
        result = g4ae07b;
    }
    return result;
}

void f_004198c3(void)
{
    BriefBlit(&g513888, 1, 0, 0, 0, 0, 0, 0x104, 0x1e0);
}

void f_004198e7(void)
{
    BriefBlit(&g513888, 1, 0, 0xf5, 0x172, 0xf5, 0x172, 0x6b, 0x6e);
}

void f_00419901(char *param_1)
{
    unsigned char second;
    unsigned char first;
    *(void **)(param_1 + 0x3cc) = f_00441dad(&gShellGame,
        (unsigned int)*(unsigned char *)(param_1 + 0x3dc), (int *)(param_1 + 0x3d8));
    for (first = 0; (int)first < *(int *)(param_1 + 0x3d8); ++first) {
        MenuPrefixSetText(
            (MenuPrefixButton *)(g4b6034 + (unsigned int)first * 0x2c),
            *(const char **)((char *)*(void **)(param_1 + 0x3cc) + 4 + (unsigned int)first * 0xc));
    }
    for (second = 0; (int)second < *(int *)(param_1 + 0x3d8); ++second) {
        char *text = g4b5dd4 + (unsigned int)second * 4;
        sprintf(text, g48ab34,
            *(int *)((char *)*(void **)(param_1 + 0x3cc) + 8 + (unsigned int)second * 0xc));
        MenuPrefixSetText((MenuPrefixButton *)(g4b6428 + (unsigned int)second * 0x2c), text);
    }
}

void f_004199e8(char *param_1)
{
    if (*(int *)(param_1 + 0x10) != 0) {
        unsigned int value = f_0042604d(param_1, &g4bb78c);
        if (value != *(unsigned int *)(param_1 + 0x4c5)) {
            if (value == 0) {
                *(unsigned char *)(param_1 + 0x4c9) = 1;
            } else if ((int)value < 1 || *(int *)(param_1 + 0x3d8) < (int)value) {
                if ((int)value < 0x18 || *(int *)(param_1 + 0x3d8) + 0x17 <= (int)value)
                    *(unsigned char *)(param_1 + 0x4c9) = 0;
                else
                    *(unsigned char *)(param_1 + 0x4c9) = 3;
            } else {
                *(unsigned char *)(param_1 + 0x4c9) = 2;
            }
            f_0041b37c(param_1);
        }
        *(unsigned int *)(param_1 + 0x4c5) = value;
        if (*(unsigned char *)(param_1 + 0x1a2) == 5) {
            char current = (char)g4b6dac;
            *(char *)(param_1 + 0x3eb) = current;
            if (current != *(char *)(param_1 + 0x3ec)) {
                *(char *)(param_1 + 0x3ec) = current;
                *(int *)(param_1 + 0x4ce) = 1;
            }
            int selected = g4b6e34;
            *(int *)(param_1 + 0x421) = selected;
            if (selected != *(int *)(param_1 + 0x425)) {
                *(int *)(param_1 + 0x425) = selected;
                *(int *)(param_1 + 0x429) = 1;
            }
        }
        g4b6d24 = value;
        f_00425ba1(param_1);
    }
}

void f_00419ae3(char *param_1)
{
    if (uiSystem.UpdateCredits() != 0) {
        g4b6d2c.SetValue(g4a706d, 0);
    }
    *(unsigned char *)(param_1 + 0x1a3) = *(unsigned char *)(param_1 + 0x1a2);
    *(unsigned char *)(param_1 + 0x1a2) = 2;
}

void f_00419b3c(char *param_1)
{
    if (*(int *)(param_1 + 0x10) != 0) {
        switch (((unsigned char)*(unsigned char *)(param_1 + 0x1a2)) - 1) {
        case 0:
            if (uiSystem.UpdateCredits() != 0) g4b6d2c.SetValue(g4a706d, 0);
            *(unsigned char *)(param_1 + 0x1a3) = *(unsigned char *)(param_1 + 0x1a2);
            *(unsigned char *)(param_1 + 0x1a2) = 2;
            return;
        case 1:
            f_00419d52(param_1);
            g4b5f47 &= 0xfe;
            g4b5f1f &= 0xfe;
            g4b5f97 &= 0xfe;
            g4b5f6f &= 0xfe;
            g4b6d43 &= 0xfe;
            g4b6f63 &= 0xfe;
            g4b6f93 &= 0xfe;
            f_00425e1a(param_1);
            if (uiSystem.UpdateCredits() != 0) g4b6d2c.SetValue(g4a706d, 0);
            g4b6d73 &= 0xfe;
            break;
        case 2:
        case 4:
            f_00425e1a(param_1);
            return;
        case 3:
        case 5:
            f_00419d52(param_1);
            if (*(int *)(param_1 + 0x4ce) != 0) {
                *(int *)(param_1 + 0x4ce) = 0;
                BriefBlit(&g513888, 1, 0, 0, 0x32, 0, 0x32, 0xf0, 0x84);
                f_0041c096(param_1);
            }
            if (*(int *)(param_1 + 0x429) != 0) {
                *(int *)(param_1 + 0x429) = 0;
                BriefBlit(&g513888, 1, 0, 0, 0xb4, 0, 0xb4, 0xe0, 0x106);
                PointPair local;
                local = g48eec4;
                f_0042a7cc(0x12, 0xcd, 200, 0xe6, g50f944,
                    *(int *)(param_1 + 0x421), -1, 6,
                    (int *)(param_1 + 0x419), (int *)(param_1 + 0x41d),
                    (int *)&local, 0xe, 1, 0, 0, 0x7fffffff);
            }
            f_00425f1c(param_1);
            break;
        }
        f_00425e1a(param_1);
    }
}

void f_00419d21(void *param_1)
{
    BriefBlit(&g513888, 1, 0, 0, 0x32, 0, 0x32, 0xf0, 0x84);
    f_0041c096(param_1);
}

void f_00419d52(char *param_1)
{
    int length = *(int *)(param_1 + 0x6c);
    if (length != 0) {
        if (*(int *)(param_1 + 0x90) == 0) {
            if (*(int *)(param_1 + 0x70) < length) {
                unsigned long amount = (unsigned long)(length - *(int *)(param_1 + 0x70));
                if (amount > 0x4000) amount = 0x4000;
                StreamRead((FileRecord *)(param_1 + 0x4c),
                           (void *)(*(int *)(param_1 + 0x68) + *(int *)(param_1 + 0x70)),
                           amount);
                *(int *)(param_1 + 0x70) += amount;
                *(short *)(param_1 + 0x194) += 1;
            } else {
                *(int *)(param_1 + 0x90) = 1;
            }
            if (*(unsigned short *)(param_1 + 0x194) < 0xc) return;
            if (*(int *)(param_1 + 0x70) - 0x7fff <
                *(int *)(*(int *)(param_1 + 0x74) * 0xc + *(int *)(param_1 + 0x8c) + 4)) return;
        }
        if (TimerExpired((Timer *)(param_1 + 0x80))) {
            TimerSet((Timer *)(param_1 + 0x80), *(unsigned long *)(param_1 + 0x7c));
            g5138fc = g51390c;
            g513900 = g513910;
            g513904 = g513914;
            g513908 = g513918;
            BriefBlit(&g513888, 1, 0, 0xf8, 0, 0xf8, 0, 0x188, 0x1e0);
            MenuSetClip((GuiRect *)&g51391c, 0xfa, 0x14, 0x186, 0x1cc);
            g5138e8 = param_1 + 0x94;
            MenuDrawImage(&g513888,
                (MenuImage *)(*(int *)(param_1 + 0x8c) + 4 + *(int *)(param_1 + 0x74) * 0xc),
                0x104, 10);
            g5138e8 = 0;
            if (*(int *)(param_1 + 0x78) < 1) {
                if (*(int *)(param_1 + 0x78) < 0) {
                    int index = *(int *)(param_1 + 0x74) - 1;
                    *(int *)(param_1 + 0x74) = index;
                    if (index < 0) *(int *)(param_1 + 0x74) = **(int **)(param_1 + 0x8c) - 1;
                }
            } else {
                unsigned int index = *(int *)(param_1 + 0x74) + 1;
                *(unsigned int *)(param_1 + 0x74) = index;
                if (**(unsigned int **)(param_1 + 0x8c) <= index) {
                    *(int *)(param_1 + 0x74) = 0;
                    return;
                }
            }
        }
    }
}

void f_00419ee4(void *param_1)
{
    g4b5f47 &= 0xfe;
    g4b5f1f &= 0xfe;
    g4b5f97 &= 0xfe;
    g4b5f6f &= 0xfe;
    g4b6d43 &= 0xfe;
    g4b6f63 &= 0xfe;
    g4b6f93 &= 0xfe;
    f_00425e1a(param_1);
}

void f_0041a9da(char *param_1)
{
    g4b6ea4 = (g4b6ea4 & 0xff1fffff) + 0xa00000;
    g4b6e50 = (g4b6e50 & 0xff1fffff) + 0xa00000;
    g4b6e78 = (g4b6e78 & 0xff1fffff) + 0xa00000;
    if (*(char *)(param_1 + 0x1a2) != 6)
        BriefBlit(&g513888, 1, 0, 0xea, 0xbe, 0xea, 0xbe, 0x18, 0x122);
}

void f_0041aa54(void)
{
    unsigned long value = g4b6ea4;
    value &= 0xff1fffff;
    g4b6ea4 = value;
    value = g4b6e50;
    value &= 0xff1fffff;
    g4b6e50 = value;
    value = g4b6e78;
    value &= 0xff1fffff;
    g4b6e78 = value;
}

void f_0041aa82(void)
{
    unsigned long value = g4b6b68;
    value &= 0xff1fffff;
    g4b6b68 = value;
    value = g4b6b90;
    value &= 0xff1fffff;
    g4b6b90 = value;
    value = g4b6bb8;
    value &= 0xff1fffff;
    g4b6bb8 = value;
    value = g4b6be0;
    value &= 0xff1fffff;
    g4b6be0 = value;
    value = g4b6c44;
    value &= 0xff1fffff;
    g4b6c44 = value;
    value = g4b6c14;
    value &= 0xff1fffff;
    g4b6c14 = value;
    value = g4b6c78;
    value &= 0xff1fffff;
    g4b6c78 = value;
}

void f_0041aaec(void)
{
    unsigned long value = g4b6b68;
    value = (value & 0xff1fffff) + 0xa00000;
    g4b6b68 = value;
    value = g4b6b90;
    value = (value & 0xff1fffff) + 0xa00000;
    g4b6b90 = value;
    value = g4b6bb8;
    value = (value & 0xff1fffff) + 0xa00000;
    g4b6bb8 = value;
    value = g4b6be0;
    value = (value & 0xff1fffff) + 0xa00000;
    g4b6be0 = value;
    value = g4b6c14;
    value = (value & 0xff1fffff) + 0xa00000;
    g4b6c14 = value;
    value = g4b6c44;
    value = (value & 0xff1fffff) + 0xa00000;
    g4b6c44 = value;
    value = g4b6c78;
    value = (value & 0xff1fffff) + 0xa00000;
    g4b6c78 = value;
}

void f_0041ab78(void)
{
    unsigned long value = g4b6b18;
    value &= 0xff1fffff;
    g4b6b18 = value;
    value = g4b6b40;
    value &= 0xff1fffff;
    g4b6b40 = value;
    value = g4b5fe4;
    value &= 0xff1fffff;
    g4b5fe4 = value;
    value = g4b6018;
    value &= 0xff1fffff;
    g4b6018 = value;
}

void f_0041abb5(void)
{
    unsigned long value = g4b5fe4;
    value = (value & 0xff1fffff) + 0xa00000;
    value = (value & 0xff1fffff) + 0xa00000;
    g4b5fe4 = value;
    value = g4b6018;
    value = (value & 0xff1fffff) + 0xa00000;
    value = (value & 0xff1fffff) + 0xa00000;
    g4b6018 = value;
    value = g4b6b18;
    value = (value & 0xff1fffff) + 0xa00000;
    g4b6b18 = value;
    value = g4b6b40;
    value = (value & 0xff1fffff) + 0xa00000;
    g4b6b40 = value;
}
void f_0041b539(int *param_1)
{
    if (*(long *)((char *)param_1 + 0x4ca) != 0) {
        *(long *)((char *)param_1 + 0x4ca) = 0;
        f_0043ba3e(profileManager, &gShellGame);
    }
}

void f_0041b563(int *param_1)
{
    if (*(long *)((char *)param_1 + 0x4ca) != 0) {
        char *mission = f_0047c350(gShellStrings, 0xf8d, 0);
        if (f_0047723b(mission)) {
            *(long *)((char *)param_1 + 0x4ca) = 1;
            if (*(long *)((char *)param_1 + 0x4ca) != 0) {
                *(long *)((char *)param_1 + 0x4ca) = 0;
                f_0043ba3e(profileManager, &gShellGame);
                return;
            }
        } else {
            *(long *)((char *)param_1 + 0x4ca) = 0;
            f_0043ba3e(&gShellGame, profileManager);
            g4a706d = g4ae17f - 1;
            SoundTrigger(0x2f, 0);
        }
    }
}

void f_0041b5f8(int *param_1)
{
    int credits = f_0043cec6(&gShellGame, (unsigned int)*(unsigned char *)((char *)param_1 + 0x3dc));
    sprintf(g4b5e30, g48ab38, credits);
    g4b6c30.SetText(g4b5e30);
    sprintf(g4b5e3c, g48ab3c, f_0043cef6(&gShellGame));
    g4b6bcc.SetText(g4b5e3c);
    SurfacePresent(&g513888);
    *(unsigned char *)((char *)param_1 + 0x1a3) = *(unsigned char *)((char *)param_1 + 0x1a2);
    *(unsigned char *)((char *)param_1 + 0x1a2) = 3;
    f_00419f3c(param_1);
    SoundTrigger(0x30, 0);
}

void f_0041b69c(int *param_1)
{
    *(unsigned char *)((char *)param_1 + 0x1a3) = *(unsigned char *)((char *)param_1 + 0x1a2);
    *(unsigned char *)((char *)param_1 + 0x1a2) = 2;
    f_00419f3c(param_1);
}

void f_0041b6bd(int *param_1)
{
    char *mission = f_0047c350(gShellStrings, 0x8cf, 1);
    sprintf(g4b5e94, g48ab40, mission);
    g4b6028 = g4b5e94;
    long value = f_0043cdcc(&gShellGame, (unsigned int)*(unsigned char *)((char *)param_1 + 0x3dc));
    sprintf(g4b5e88, g48ab43, value);
    g4b5fd0.SetText(g4b5e88);
    *(unsigned char *)((char *)param_1 + 0x1a3) = *(unsigned char *)((char *)param_1 + 0x1a2);
    *(unsigned char *)((char *)param_1 + 0x1a2) = 4;
    f_00419f3c(param_1);
}

void f_0041b74c(int *param_1)
{
    f_0043cf2d(&gShellGame, (unsigned int)*(unsigned char *)((char *)param_1 + 0x3dc));
    if (uiSystem.UpdateCredits() != 0) {
        g4b6d2c.SetValue(g4a706d, 0);
    }
    *(long *)((char *)param_1 + 0x4ca) = 1;
    SoundTrigger(0x74, 0);
}

void f_0041c3ca(char *param_1)
{
    PointPair local;
    BriefBlit(&g513888, 1, 0, 0, 0xb4, 0, 0xb4, 0xe0, 0x106);
    local = g48eec4;
    f_0042a7cc(0x12, 0xcd, 200, 0xe6, g50f944,
               *(int *)(param_1 + 0x421), -1, 6,
               (int *)(param_1 + 0x419), (int *)(param_1 + 0x41d),
               (int *)&local, 0xe, 1, 0, 0, 0x7fffffff);
}

void f_0041c456(char *param_1, unsigned char *param_2)
{
    int local_10;
    int local_c;
    MenuDrawImage(&g513888, (MenuImage *)(gShellImages + 0x31c), 0xaa, 0xdc);
    SurfacePresent(&g513888);
    *(unsigned char *)(param_1 + 0x1a3) = *(unsigned char *)(param_1 + 0x1a2);
    *(unsigned char *)(param_1 + 0x1a2) = 6;
    f_00419f3c((int *)param_1);
    f_0042a7cc(0xbe, 0xeb, 0xaa, 0x4b, param_2, 0, -1, 0,
               &local_10, &local_c, 0, 0xf, 1, 0, 0, 0x7fffffff);
}

void f_0041c4ea(void *param_1, unsigned char *param_2)
{
    int local_c;
    int local_8;
    f_0042a7cc(0xbe, 0xeb, 0xaa, 0x4b, param_2, 0, -1, 0,
               &local_c, &local_8, 0, 0xf, 1, 0, 0, 0x7fffffff);
}

void f_0041c66e(char *param_1)
{
    BriefBlit(&g513888, 1, 0, 0, 0, 0, 0, 0xf5, 0x19);
    strncpy(g4b5dc0,
            f_00439d2c((int)((ProfileManager *)&gShellGame)->Mission(
                (unsigned int)*(unsigned char *)(param_1 + 0x3dc) + 1)),
            0x12);
    g4b6d5c.SetText(g4b5dc0);
    g4b6d73 &= 0xfe;
}

void f_0041c7a3(int *param_1)
{
    SaveHercConfig(&gShellGame, (unsigned int)*(unsigned char *)((char *)param_1 + 0x3dc));
}
