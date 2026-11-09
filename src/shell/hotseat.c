extern "C" void f_004243e4(void)
{
}

extern "C" int f_004243e5(void)
{
    return 0;
}

extern "C" void f_004243e7(void)
{
}

struct GuiControl;
struct GuiPoint { long x, y; };
class MenuLabel { public: void SetColor(long color); };
extern GuiControl *PTR_DAT_0048f1dc[];
class GuiScreen
{
public:
    GuiScreen(GuiControl **controls = PTR_DAT_0048f1dc, unsigned count = 0x4c, unsigned long flags = 0);
    int HitTest(const GuiPoint *point);
    int Update();
};

class HotseatScreen : public GuiScreen
{
public:
    HotseatScreen();
};

class HotseatObject : public GuiScreen
{
    char state[0x12cf];
};

HotseatScreen::HotseatScreen() : GuiScreen(PTR_DAT_0048f1dc, 0x4c, 0)
{
}

extern int MissionRandomTable[];
extern void f_0047e200(int *table);
extern "C" unsigned int next_link_seed(unsigned int *seed);
extern "C" void *memset(void *dest, int value, unsigned long size);
extern "C" void GuiFadeOut(void);
extern void ReleasePoolData(void *);
extern "C" void f_cbf0(void *);
extern "C" void BriefCloseAudio(void);
extern void GuiResetClip(void *);
extern void MenuDrawImage(void *, void *, long, long);
extern char DAT_00511dcc, gMenuClip, g513888;
struct StrPool { char *GetString(long id, int loadData); };
extern StrPool DAT_00511e7c, DAT_00511e68, gShellStrings;
extern long viewportClipLeft, DAT_00513900, DAT_00513904, DAT_00513908;
extern long DAT_0051390c, DAT_00513910, DAT_00513914, DAT_00513918;
extern int _DAT_004bb6f0, DAT_004bac94;
extern GuiPoint g4bb78c;
extern void MenuHelpOpenTopic(long);

struct ClipCopy { long value[4]; };
extern int _DAT_004bb430, _DAT_004bb458, _DAT_004bb480;
extern char DAT_004bb423, DAT_004bb44b, DAT_004bb473;
extern int f_004773cc(char *, char *, unsigned long);
extern void f_00425f1c(int);
extern "C" char *strcpy(char *, const char *);
extern int f_0047723b(char *);
extern void f_00477295(char *);
extern int f_0042500d(int);
extern void f_004254d5(int);
extern char DAT_00511dd0[];
extern void f_00427e60(int, int);
extern void f_00428f33(int, int, int);
extern void f_0042539a(int);
extern void f_00425e1a(int);
extern void f_0047bf50(int, int, int, int, int, int, int, int, int);
extern short DAT_0048f334[];
extern int DAT_0048f30c[];
extern int DAT_004bb50c, DAT_004bb61c, DAT_004bb594, DAT_004bb484;
extern char DAT_004bb0fc, DAT_004bb284;
extern char *gShellImages;
extern char DAT_004bab8b, DAT_004babeb, DAT_004bab5b;
extern char DAT_004babbb, DAT_004bac1b;
extern int _DAT_004babc8, _DAT_004bac28;
extern char DAT_0048acf2[], DAT_0048acf5[], DAT_0048acf8[], DAT_0048acfb[];
extern "C" int sprintf(char *, const char *, ...);
extern int *gShellHotseat;
extern int *Allocate(int);
extern void f_004010da(int);
extern void f_00424a07(int *, int);
extern int atoi(const char *);
extern int f_0043dcfd(int, int);
extern int LoadMission(const char *, int);
extern void GetPlayerInfo(void *, void *);
extern char profileManager;
extern char gDebugText[];
extern "C" unsigned long strlen(const char *);
extern "C" void AssertFail(const char *, int, const char *);

void f_00424cf4(int param_1)
{
    unsigned int uVar1;
    int iVar2;
    int iVar3;
    int aiStack_2c[8];
    f_0047e200(MissionRandomTable);
    memset(aiStack_2c, 0, 0x20);
    iVar3 = 0;
    while (iVar3 < *(int *)(param_1 + 0x4c)) {
        uVar1 = next_link_seed((unsigned int *)MissionRandomTable);
        iVar2 = (int)(uVar1 % 8);
        if (aiStack_2c[iVar2] == 0) {
            aiStack_2c[iVar2] = 1;
            *(char *)(param_1 + 0x1298 + iVar3) = (char)iVar2;
            ++iVar3;
        }
    }
}

void f_004251df(int param_1)
{
    if (param_1 != 0) {
        int active = *(int *)(param_1 + 0x10) != 0;
        if (active) {
        ReleasePoolData(*(void **)(param_1 + 0x54));
        f_cbf0(&DAT_00511dcc);
        GuiFadeOut();
        BriefCloseAudio();
        *(int *)(param_1 + 0x10) = 0;
        }
    }
}

void f_004255bf(int param_1)
{
    int iVar1;
    GuiResetClip(&gMenuClip);
    for (iVar1 = 0; iVar1 < *(int *)(param_1 + 0x4c); iVar1 = iVar1 + 1) {
        MenuDrawImage(&g513888,
                      (void *)(((*(signed char *)(param_1 + 0x1298 + iVar1) + 0x19) * 0xc) +
                               *(int *)(param_1 + 0x54) + 4),
                      0x87, iVar1 * 0x1a + 0x32);
    }
}

int f_00425616(int param_1)
{
    if (f_0042500d(param_1) != 0) {
        char *prompt = DAT_00511e7c.GetString(0x19f, 1);
        int accepted = f_0047723b(prompt);
        *(int *)(param_1 + 0x12c4) = accepted;
        return 1;
    }
    char *prompt = DAT_00511e7c.GetString(0x1a1, 1);
    f_00477295(prompt);
    return 0;
}

void f_0042515a(int param_1)
{
    int resourceId = *(int *)(param_1 + 0x12a0) + 0xf8a;
    _DAT_004bb430 = (int)gShellStrings.GetString(resourceId, 0);
    DAT_004bb423 &= 0xfe;
    resourceId = *(int *)(param_1 + 0x12a4) + 0xf85;
    _DAT_004bb458 = (int)gShellStrings.GetString(resourceId, 0);
    DAT_004bb44b &= 0xfe;
    resourceId = *(int *)(param_1 + 0x12c8) + 0xfed;
    _DAT_004bb480 = (int)gShellStrings.GetString(resourceId, 0);
    DAT_004bb473 &= 0xfe;
    f_0042500d(param_1);
}

int f_00425222(int param_1)
{
    if (param_1 != 0) {
        int active = *(int *)(param_1 + 0x10) != 0;
        if (active) goto active_screen;
    }
    return 0;
active_screen:
    *(ClipCopy *)&viewportClipLeft = *(ClipCopy *)&DAT_0051390c;
    int mode = ((GuiScreen *)param_1)->HitTest(&g4bb78c);
    _DAT_004bb6f0 = mode;
    int helpAvailable = ((*(unsigned long *)(*(int *)(param_1 + 0x10) + 4) & 0xa0) == 0x20);
    if (helpAvailable) {
        if (mode == 0x49)
            MenuHelpOpenTopic(DAT_004bac94 + 0xf0d);
    }
    return ((GuiScreen *)param_1)->Update();
}

int f_0042500d(int param_1)
{
    int result = 1;
    for (int i = 0; i < *(int *)(param_1 + 0x4c); ++i) {
        int y = i * 0x1a + 0x2f;
        if (*(int *)(param_1 + 0x12ac) < *(int *)(param_1 + 0x1258 + i * 4)) {
            MenuDrawImage(&g513888, gShellImages + 0x334, 0x21d, y);
            ((MenuLabel *)((char *)&DAT_004bb0fc + i * 0x31))->SetColor(0xe);
            result = 0;
        } else {
            MenuDrawImage(&g513888, gShellImages + 0x340, 0x21d, y);
            ((MenuLabel *)((char *)&DAT_004bb0fc + i * 0x31))->SetColor(0x12);
        }
        y = i * 0x1a + 0x2f;
        if (*(int *)(param_1 + 0x12b0) < *(int *)(param_1 + 0x1238 + i * 4)) {
            MenuDrawImage(&g513888, gShellImages + 0x34c, 0x1e0, y);
            ((MenuLabel *)((char *)&DAT_004bb284 + i * 0x31))->SetColor(0xe);
            result = 0;
        } else {
            MenuDrawImage(&g513888, gShellImages + 0x358, 0x1e0, y);
            ((MenuLabel *)((char *)&DAT_004bb284 + i * 0x31))->SetColor(0x12);
        }
    }
    f_00425f1c(param_1);
    GuiResetClip(&gMenuClip);
    return result;
}

void f_004252a1(int param_1)
{
    if (param_1 != 0) {
        int active = *(int *)(param_1 + 0x10) != 0;
        if (active) {
        int oldCurrent = *(int *)(param_1 + 0x12c0);
        *(int *)(param_1 + 0x12c0) = DAT_004bac94;
        int oldScore = *(int *)(param_1 + 0x12b0);
        int newScore = DAT_004bb50c + 1;
        *(int *)(param_1 + 0x12b0) = newScore;
        int oldTech = *(int *)(param_1 + 0x12b8);
        *(int *)(param_1 + 0x12b8) = DAT_004bb61c;
        int oldDefense = *(int *)(param_1 + 0x12b4);
        *(int *)(param_1 + 0x12b4) = DAT_004bb594;
        int oldWeapon = *(int *)(param_1 + 0x12a8);
        *(int *)(param_1 + 0x12a8) = DAT_004bb484;
        if ((char)((oldCurrent != DAT_004bac94) + (oldScore != newScore) +
                   (oldTech != DAT_004bb61c) + (oldDefense != DAT_004bb594) +
                   (oldWeapon != DAT_004bb484)) != 0) {
            f_0042539a(param_1);
            f_004254d5(param_1);
        }
        *(ClipCopy *)&viewportClipLeft = *(ClipCopy *)&DAT_0051390c;
        f_00425e1a(param_1);
        *(ClipCopy *)&viewportClipLeft = *(ClipCopy *)&DAT_0051390c;
        }
    }
}

void f_004254d5(int param_1)
{
    for (int i = 0; i < *(int *)(param_1 + 0x4c); ++i) {
        int localName = param_1 + 0x1f8 + i * 4;
        char *text;
        if (*(int *)(param_1 + 0x1218 + i * 4) == 0) {
            text = gShellStrings.GetString(0xe78, 0);
            sprintf((char *)(param_1 + 0x218 + i * 0x200), text);
            sprintf((char *)localName, DAT_0048acf8);
            text = DAT_0048acfb;
        } else {
            sprintf((char *)localName, DAT_0048acf2);
            text = DAT_0048acf5;
        }
        sprintf((char *)(param_1 + 0x158 + i * 0x14), text);
    }
    f_00425f1c(param_1);
    f_0042500d(param_1);
}

void f_0042539a(int param_1)
{
    f_0047bf50(0x513888, 1, 0, 500, 0x154, 500, 0x154, 0x8c, 0x78);
    MenuDrawImage(&g513888,
                  (void *)(*(int *)(param_1 + 0x54) + 4 + (*(int *)(param_1 + 0x12c0) + 2) * 0xc),
                  0x203, 0x154);
    _DAT_004babc8 = (int)gShellStrings.GetString(*(int *)(param_1 + 0x12c0) + 0xee3, 0);
    DAT_004babbb &= 0xfe;
    f_00428f33(0x4bab74, *(int *)(param_1 + 0x12b0), 0);
    DAT_004bab8b &= 0xfe;
    *(int *)(param_1 + 0x12bc) = DAT_0048f334[*(int *)(param_1 + 0x12b8)];
    _DAT_004bac28 = (int)gShellStrings.GetString(*(int *)(param_1 + 0x12b8) + 0xefa, 0);
    DAT_004bac1b &= 0xfe;
    f_00428f33(0x4babd4, *(int *)(param_1 + 0x12b4), 0);
    DAT_004babeb &= 0xfe;
    int value = DAT_0048f30c[*(int *)(param_1 + 0x12a8)];
    *(int *)(param_1 + 0x12ac) = value;
    f_00428f33(0x4bab44, value, 0);
    DAT_004bab5b &= 0xfe;
    GuiResetClip(&gMenuClip);
}

int f_0042490e(int param_1)
{
    if (gShellHotseat == 0) {
        HotseatObject *object = new HotseatObject;
        gShellHotseat = (int *)object;
    }
    if (gShellHotseat != 0) goto have_screen;
    return 0;

have_screen: ;
    char countText[32];
    memset(countText, 0, 0x20);
    int accepted = f_004773cc(DAT_00511e7c.GetString(0x19d, 1), countText, 0x14);
    if (accepted == 0)
        return 0;
    gShellHotseat[0x13] = atoi(countText);
    if (gShellHotseat[0x13] < 2) goto bad_count;
    if (gShellHotseat[0x13] <= 8) goto valid_count;
bad_count:
    f_00477295(DAT_00511e7c.GetString(0x19e, 1));
    return 0;
valid_count:
    f_004010da(param_1);
    f_00424a07(gShellHotseat, param_1 + 0x43);
    *(int *)(param_1 + 0x4f) = 0x26;
    return 1;
}

void f_004256eb(int param_1, int param_2)
{
    int selected = (f_0047723b(DAT_00511e7c.GetString(0x1a0, 1)) == 0);
    if (selected != 0) {
        f_0043dcfd((int)&profileManager, 3);
        if (LoadMission(0, 0) == 0) selected = 0;
        f_0043dcfd((int)&profileManager, 1);
    }
    int slot = param_1 + param_2 * 4;
    if (selected != 0) {
        *(int *)(slot + 0x1218) = 1;
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserved source line for ASSERT
        // preserve ASSERT immediate line
        if (strlen(gDebugText) >= 500) AssertFail(__FILE__, __LINE__, 0);
        strcpy((char *)(param_1 + 0x218 + param_2 * 0x200), gDebugText);
        struct PlayerInfo { char first; char rest[39]; int value; int unused; } info;
        GetPlayerInfo(&profileManager, &info);
        *(unsigned int *)(param_1 + 0x1238 + param_2 * 4) = (unsigned char)info.first;
        *(int *)(param_1 + 0x1258 + param_2 * 4) = info.value;
    } else {
        *(int *)(slot + 0x1218) = 0;
        *(int *)(slot + 0x1238) = *(int *)(param_1 + 0x12b0);
        *(int *)(slot + 0x1258) = *(int *)(param_1 + 0x12ac);
        *(int *)(slot + 0x1278) = *(int *)(param_1 + 0x12a8);
    }
    f_004254d5(param_1);
}

void f_00425815(int param_1, int param_2)
{
    // Hypothesis 1: signed versus default-char loads explain the first byte-width difference.
    int local_10 = (int)*(signed char *)(param_1 + param_2 + 0x1298);
    int local_30[8];
    memset(local_30, 0, 0x20);
    int iVar2 = 0;
    int marker = 1;
    // Hypothesis 2: a pointer walk changes the counter register at the first loop body.
    for (; iVar2 < param_2; ++iVar2) {
        int slot = (int)*(signed char *)(param_1 + iVar2 + 0x1298);
        if (slot >= 0 && slot < 8)
            local_30[slot] = 1;
    }
    // Hypothesis 3: re-reading the active player's slot instead of reusing local_10 changes the reload.
    iVar2 = (int)*(char *)(param_1 + param_2 + 0x1298);
    do {
        // Hypothesis 4: the target uses signed division for modulo eight.
        iVar2 = (iVar2 + 1) % 8;
        *(char *)(param_2 + 0x1298 + param_1) = (char)iVar2;
    } while (local_30[iVar2] != 0);
    // Hypothesis 5: a copied index loop matches the target's EDX loop counter.
    char replacement = (char)local_10;
    for (int laterIndex = param_2; ++laterIndex < *(int *)(param_1 + 0x4c);) {
        if (*(signed char *)(param_1 + laterIndex + 0x1298) == iVar2)
            *(char *)(param_1 + laterIndex + 0x1298) = replacement;
    }
    GuiResetClip(&gMenuClip);
    for (iVar2 = 0; iVar2 < *(int *)(param_1 + 0x4c); ++iVar2) {
        MenuDrawImage(&g513888, (void *)(*(int *)(param_1 + 0x54) + 4 + (*(signed char *)(param_1 + iVar2 + 0x1298) + 0x19) * 0xc), 0x87, iVar2 * 0x1a + 0x32);
    }
}

void f_00425906(int param_1)
{
    char *prompt = DAT_00511e7c.GetString(0x1a6, 1);
    if (f_0047723b(prompt) != 0) {
        int iVar3;
        for (iVar3 = 0; iVar3 < *(int *)(param_1 + 0x4c); ++iVar3) {
            *(int *)(param_1 + 0x1218 + iVar3 * 4) = 0;
            *(int *)(param_1 + 0x1238 + iVar3 * 4) = *(int *)(param_1 + 0x12b0);
            *(int *)(param_1 + 0x1258 + iVar3 * 4) = *(int *)(param_1 + 0x12ac);
            *(int *)(param_1 + 0x1278 + iVar3 * 4) = *(int *)(param_1 + 0x12a8);
        }
        f_004254d5(param_1);
    }
}

void f_00425674(int param_1, int param_2)
{
    char local_28[32] = {0};
    char *name = (char *)(param_1 + 0x58 + param_2 * 0x20);
    strcpy(local_28, name);
    int accepted = f_004773cc(gShellStrings.GetString(0xdd3, 0), local_28, 0x12);
    if (accepted != 0)
        strcpy(name, local_28);
    f_00425f1c(param_1);
}

