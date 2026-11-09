typedef unsigned long uint;
typedef unsigned char byte;

struct GuiControl
{
    char data[0x1c];
    GuiControl(unsigned long style, unsigned long flags);
};

extern void *PTR_f_00446804_0048dd04;
struct StrPool
{
    char *GetString(long id, int loadData);
    void Lock();
    void Unlock();
};
struct ResMgr
{
    int *Load(const char *name, int mode, int reserved, int flags);
};
extern ResMgr gResMgr;
extern StrPool *gGuiStringPool;
extern long gGuiStringBase;
extern StrPool gMenuPrefixStrings;
extern StrPool targetBriefResource;
extern unsigned long *gMenuTextMap;
extern long viewportClipLeft, viewportClipTop, viewportClipRight, viewportClipBottom;
struct GuiRect { long x, y, width, height; };
extern GuiRect gMenuClip;
struct Font;
extern char DAT_00511dc0[];
extern char g513888;
extern int DAT_004d1fa4;
extern char DAT_004d1f48, DAT_004d1f49, DAT_004d1f4a, DAT_004d1f4b;
extern long DAT_0051390c, DAT_00513910, DAT_00513914, DAT_00513918;
extern char s__3d___0048b30a[];
extern char DAT_0048b305[];
extern long DAT_004bb7a4, DAT_004bc4f8;
extern long g4bb78c;
extern long gInA4;
extern void *PTR_f_00446848_0048dcf0;
extern int f_00426d74(int, int, unsigned long *);
extern void f_0047bfa0(int, int, int, int, int, int, int);
extern byte *f_00438374(int, int, int);
extern void f_00438730(byte *);
extern int f_00446493(int, int, int *, int *);
extern byte *f_0047d420(byte *);
extern int f_0047cc10(int *, byte *);
extern void f_0047cf80(int *, int, int, byte *);
extern void f_0047b930(int, int, int);
extern void f_0047bad0(int, int, int, int, int);
extern void MenuSetClip(GuiRect *, long, long, long, long);
extern void MenuSetColors(void *, unsigned long, long);
extern void MenuFill(void *, long, long, long, long);
extern void MenuDrawText(Font *, long, long, const char *);
extern int MenuControlChanged(void *, unsigned long, unsigned long *);
extern long MenuTextWidth(Font *, const char *);
extern "C" void RenderFormattedResourceText(void *, int, int, const char *, ...);
extern "C" void LogicCallMode(int, int);
extern uint f_0043a7e8(int, uint);
extern long GridClamp(unsigned long);
struct GuiScreen { void Invalidate(); };
inline long AutoAdvanceEnabled() { return gInA4; }
extern void PopObjectConstruct(void *object);
extern "C" void BriefModuleOpen(void *object, int id, int delay, int flags);
extern int f_00439d48(int);
extern void f_00446350(int);
extern void f_00425e1a(int);
extern void f_00454440(int, int, int, int, int);
extern void f_0047ae20(unsigned long *);
extern int f_00445e23(int);
extern void f_0044a190(int, int);
extern void f_0044dd40(int, void *);
extern void f_0047e670(void *, void *, unsigned long);
extern int f_0047fce0(int);
extern void f_0047fca0(int *, int);
extern void f_0043ac02(int, int *, int *, int *, int *);
extern void f_0047c320(void *);
extern void f_0047bab0(int, int);
extern void f_0047b3a0(int);
extern void f_0047822d(int, int);
extern void f_004272cd(void *, long, long, long);
extern void f_0042732e(void *, long, long);
extern int f_00476512(int);
extern int f_00477798(int);
extern void f_0047eb40(void *);
extern void f_0041caf9(void *, int);
extern void f_00445c81(int);
extern char DAT_00511df0, DAT_00511948, DAT_00511958;
extern long DAT_00513890, DAT_00513898, DAT_005138ac, DAT_005138b0, DAT_00511960;
extern int DAT_004faf44;
extern "C" void ReleasePoolData(void *);
extern "C" void GlobalBlockFree(void *);
extern char g4b74f4;
extern char g511948, gMenuSavedSurface;
extern void GuiResetClip(void *);
extern "C" void BriefRenderUpdate(void *);
extern char modeCheckName[];
extern long DAT_005138ac;
extern char DAT_004a009c;
extern char DAT_004d1f4c, DAT_004d1f78;
extern int _DAT_004a00a4;
extern int modeSwitch;
extern char DAT_004faf48;
extern void f_00472723(void *, int, byte, byte, int);
extern int DAT_004faf40;
extern int TimeSleep(unsigned long);
extern void f_004259f7(void *, const char *);
extern void f_00425f87(void *, int, void *, int, int);
extern int f_0046c461(int, int *);
extern void f_0041c891(void *, int);
extern char s_DMGGUI_BMX_0048b310[], s_DMGHRC_BMX_0048b31b[], s_DMGGUI_BOX_0048b326[];
extern "C" void *memset(void *, int, unsigned long);
extern char DAT_004d1f63, DAT_004d1f8f;
extern int DAT_005138e8, DAT_005138f4, DAT_005138f8;
extern int DAT_004bc560, DAT_004bc4e8;
extern void f_0047bff0(void *, int *, int, int);
extern void f_0047cfc0(void *, int, int, int, byte *);

void f_00445238(void)
{
    PopObjectConstruct(&DAT_004a009c);
    BriefModuleOpen(&DAT_004d1f4c, 0x7f, 1000, 0x33);
    _DAT_004a00a4 = 1;
    BriefModuleOpen(&DAT_004d1f78, 0x82, 1000, 0x34);
    _DAT_004a00a4 = 2;
}

struct HercControl : GuiControl
{
    void *dispatch;
    unsigned long value;
    HercControl();
};

HercControl::HercControl() : GuiControl(0x18, 0)
{
    value = 0;
    dispatch = &PTR_f_00446804_0048dd04;
}

void f_004453fd(void *object)
{
    *(unsigned short *)((char *)object + 0x16) &= 0xfef9;
    *(unsigned long *)((char *)object + 0x20) = 0;
    return;
}

void f_0044540f(unsigned long *param_1, int param_2)
{
    uint local_18 = param_1[8];
    if (MenuControlChanged((void *)param_1, param_2, param_1 + 8)) {
        viewportClipLeft = DAT_0051390c;
        viewportClipTop = DAT_00513910;
        viewportClipRight = DAT_00513914;
        viewportClipBottom = DAT_00513918;
        MenuSetClip(&gMenuClip, *param_1, param_1[1], param_1[2], param_1[3]);
        f_0047bfa0(0x513888, 1, 0, *param_1, param_1[1], param_1[2], param_1[3]);
        int iVar4 = *(int *)(DAT_004d1fa4 + 0x5c3);
        uint uVar5 = (uint)*(byte *)(param_2 + 0xc83 + DAT_004d1fa4);
        DAT_004d1f4b = *(char *)(uVar5 + 0x5ab + DAT_004d1fa4);
        DAT_004d1f49 = DAT_004d1f4b + '\x04';
        DAT_004d1f4a = DAT_004d1f4b + '\x02';
        gMenuTextMap = (unsigned long *)&DAT_004d1f48;
        char *local_14;
        int iStack_1c;
        if (param_2 == 0x1a) {
            local_14 = gGuiStringPool->GetString(gGuiStringBase + 0x1a, 1);
        } else {
            local_14 = gMenuPrefixStrings.GetString(uVar5 + 0x44e, 0);
            byte *pbVar3 = f_00438374(iVar4, uVar5, 0xffffffff);
            f_00438730(pbVar3);
        }
        if (*(int *)(DAT_004d1fa4 + 0x5eb) != 0 && param_2 != 0x1a) {
            f_00446493(DAT_004d1fa4, uVar5, &iStack_1c, &iStack_1c);
        }
        viewportClipRight = *param_1 + 200;
        RenderFormattedResourceText((void *)DAT_00511dc0, *param_1 + 5, param_1[1] + 1, DAT_0048b305);
        viewportClipLeft = DAT_0051390c;
        viewportClipTop = DAT_00513910;
        viewportClipRight = DAT_00513914;
        viewportClipBottom = DAT_00513918;
        byte *pbVar3 = f_0047d420((byte *)s__3d___0048b30a);
        int iVar6 = param_1[1] + 1;
        uVar5 = *param_1;
        uint uVar1 = param_1[2];
        iVar4 = MenuTextWidth((Font *)DAT_00511dc0, (char *)pbVar3);
        MenuDrawText((Font *)DAT_00511dc0, (uVar5 + uVar1 + -5) - iVar4, iVar6, (char *)pbVar3);
        if ((*(byte *)((char *)param_1 + 0x22) & 8) != 0) {
            MenuSetColors(&g513888, DAT_004bc4f8, 0xffffffff);
            MenuFill(&g513888, *param_1, param_1[1], param_1[2], param_1[3]);
            if ((local_18 & 0x80000) == 0) {
                LogicCallMode(0x188, 0);
            }
        }
    }
}

uint f_00445633(void *param_1, int param_2, byte param_3)
{
    uint uVar5;
    int iVar2, iVar3, iVar4;
    uint uVar1;
    if (param_2 != 0x1a)
        uVar5 = (uint)*(byte *)(DAT_004d1fa4 + param_2 + 0xc83);
    else
        uVar5 = 0x17;
    if (*(int *)(DAT_004d1fa4 + 0x5eb) == 0 && (param_3 & 8) != 0) {
        if (AutoAdvanceEnabled())
            iVar3 = 10;
        else
            iVar3 = 1;
        iVar4 = 0;
        for (iVar2 = 0; iVar2 < iVar3; ++iVar2) {
            uVar1 = f_0043a7e8(*(int *)(DAT_004d1fa4 + 0x5c3), uVar5);
            iVar4 += uVar1;
        }
        if (iVar4 != 0) {
            LogicCallMode(1, 0);
            ((GuiScreen *)DAT_004d1fa4)->Invalidate();
            *(unsigned long *)(DAT_004d1fa4 + 0x5e7) = 1;
        }
    }
    return 0;
}

struct HercDetailControl : GuiControl
{
    void *dispatch;
    HercDetailControl();
};

HercDetailControl::HercDetailControl() : GuiControl(8, 0)
{
    dispatch = &PTR_f_00446848_0048dcf0;
}

void f_004456f3(void)
{
}

uint f_004456f4(int *param_1)
{
    // A: keep the pointer live, capture the shared value, then load both fields.
    // E: compute the upper bound before the lower bound.
    int iVar2 = *param_1;
    int shared = g4bb78c;
    int range = param_1[2];
    int iVar3 = (range * 0x3a) / 100 + iVar2;
    int iVar1 = iVar2 + (range * 0x2a) / 100;
    int iVar4 = 0;
    if (shared < iVar1)
        iVar4 = ((iVar1 - shared) * 0x28) / (iVar1 - iVar2) + 1;
    else if (iVar3 < shared)
        iVar4 = ((shared - iVar3) * -0x28) / ((range + iVar2) - iVar3) + -1;
    if (iVar4 < -0x27)
        iVar4 = -0x28;
    else if (0x27 < iVar4)
        iVar4 = 0x28;
    *(int *)(DAT_004d1fa4 + 0x607) = iVar4;
    return 0;
}

void f_00446570(int *param_1)
{
    int *list = *(int **)((char *)param_1 + 0x60b);
    int iVar2 = list == 0 ? 0 : *list;
    if (iVar2 == 0) iVar2 = 0x28;
    int angle = ((*(int *)((char *)param_1 + 0x603) * 0x168) / iVar2 + 0x1e) % 0x168;
    *(int *)((char *)param_1 + 0x5df) = angle;
    *(uint *)((char *)param_1 + 0x5e3) = GridClamp(angle * 0xb60b61);
}

void f_00446253(int param_1)
{
    int iVar2;
    unsigned long count = *(unsigned long *)(param_1 + 0x5fb);
    if (count == 0) return;
    if (*(unsigned long *)(param_1 + 0x5ff) < count) {
        unsigned long bytes = count - *(unsigned long *)(param_1 + 0x5ff);
        if (0x1800 < bytes) bytes = 0x1800;
        f_0047e670((void *)(param_1 + 0x61c),
                   (void *)(*(int *)(param_1 + 0x5f7) + *(int *)(param_1 + 0x5ff)), bytes);
        *(unsigned long *)(param_1 + 0x5ff) += bytes;
        return;
    }
    if (!f_0047fce0(param_1 + 0x610)) return;
    iVar2 = *(int *)(param_1 + 0x607);
    int amount;
    if (iVar2 < 0) amount = -500;
    else if (iVar2 < 1) amount = 100;
    else amount = 500;
    if (iVar2 != 0) amount /= iVar2;
    f_0047fca0((int *)(param_1 + 0x610), amount);
    f_00446350(param_1);
    if (*(int *)(param_1 + 0x607) < 1) {
        if (*(int *)(param_1 + 0x607) < 0) {
            iVar2 = *(int *)(param_1 + 0x603) - 1;
            *(int *)(param_1 + 0x603) = iVar2;
            if (iVar2 < 0)
                *(int *)(param_1 + 0x603) = **(int **)(param_1 + 0x60b) - 1;
        }
    } else {
        unsigned long next = *(int *)(param_1 + 0x603) + 1;
        *(unsigned long *)(param_1 + 0x603) = next;
        if (**(unsigned long **)(param_1 + 0x60b) <= next) {
            *(int *)(param_1 + 0x603) = 0;
            return;
        }
    }
}

void f_004465d6(int param_1)
{
    int iVar1, iVar2, iVar4;
    uint uVar3;
    int iStack_18, local_14, local_10;
    iVar4 = 0;
    do {
        if (0x17 < iVar4)
            return;
        if (iVar4 == 0x17)
            uVar3 = *(uint *)(param_1 + 0x5f3);
        else
            uVar3 = (uint)*(byte *)(*(int *)(param_1 + 0x5c3) + 0x13 + iVar4 * 8);
        if (*(int *)(param_1 + 0x5eb) != 0) {
            if (iVar4 == 0x17)
                uVar3 = 100;
            else {
                iVar1 = f_00446493(param_1, iVar4, &local_14, &iStack_18);
                iVar2 = iVar1 * 100 >> 0x1f;
                uVar3 = 100 - ((int)((iVar1 * 100 + iVar2 * -0x10) - (uint)(iVar2 << 3 < 0)) >> 4);
                if ((int)uVar3 < 1)
                    uVar3 = 0;
                else if (99 < (int)uVar3)
                    uVar3 = 100;
            }
        }
        local_10 = ((int)((100 - uVar3) * 0xf) / 100) * 8 + 0x10;
        *(char *)(iVar4 + 0x5ab + param_1) = (char)local_10;
        iVar1 = *(int *)(param_1 + 0x5cb);
        if (-1 < iVar1 && iVar4 != iVar1 && iVar1 < 0x17 && iVar1 != 1)
            local_10 = 0x90;
        for (iVar1 = 0; iVar1 < 6; ++iVar1)
            *(char *)(param_1 + 0x4bb + iVar4 * 6 + iVar1) = (char)local_10 + (char)iVar1;
        ++iVar4;
    } while (1);
}

void f_00445c81(int param_1)
{
    int iVar1, iVar2, iVar4;
    unsigned long *puVar3;
    unsigned long *puVar5;
    if (modeSwitch != 0) {
        iVar2 = *(int *)(param_1 + 0x5c3) + 0x11;
        iVar4 = param_1 + 0x649;
        for (iVar1 = 0; iVar1 < 0x17; ++iVar1) {
            if (*(char *)(iVar2 + 2) != *(char *)(iVar4 + 2) ||
                *(int *)(iVar2 + 4) != *(int *)(iVar4 + 4)) {
                f_00472723(&DAT_004faf48, *(int *)(param_1 + 0x4d), (byte)iVar1,
                           *(byte *)(iVar2 + 2), *(int *)(iVar2 + 4));
            }
            iVar2 += 8;
            iVar4 += 8;
        }
        puVar3 = *(unsigned long **)(param_1 + 0x5c3);
        puVar5 = (unsigned long *)(param_1 + 0x638);
        for (iVar4 = 0x42; iVar4 != 0; --iVar4) {
            *puVar5 = *puVar3;
            ++puVar3;
            ++puVar5;
        }
        *(unsigned short *)puVar5 = *(unsigned short *)puVar3;
        *((unsigned char *)puVar5 + 2) = *((unsigned char *)puVar3 + 2);
    }
}

void f_00445cfe(int param_1)
{
    int active = *(int *)(param_1 + 0x10) != 0;
    if (active) {
        f_00445c81(param_1);
        gMenuPrefixStrings.Unlock();
        ReleasePoolData(*(void **)(param_1 + 0x51));
        ReleasePoolData(*(void **)(param_1 + 0x55));
        *(int *)(param_1 + 0x51) = 0;
        *(int *)(param_1 + 0x55) = 0;
        f_0047bab0(0x513888, 0);
        f_0047b3a0(0x513888);
        f_0047822d(-1, -1);
        f_004272cd(&g511948, DAT_00513890, DAT_005138ac, DAT_005138b0);
        f_0042732e(&gMenuSavedSurface, DAT_005138ac, DAT_005138b0);
        DAT_00513898 = DAT_00511960;
        f_00476512(*(int *)(param_1 + 0x5d3));
        f_00477798(*(uint *)(param_1 + 0x5d7));
        if (*(int *)(param_1 + 0x5f7) != 0)
            GlobalBlockFree(*(void **)(param_1 + 0x5f7));
        *(int *)(param_1 + 0x5f7) = 0;
        f_0041caf9(&g4b74f4, 0x927);
        *(int *)(param_1 + 0x10) = 0;
        DAT_004faf44 = 1;
    }
}

struct HercModule
{
    void Update();
};

void HercModule::Update()
{
    int param_1 = (int)this;
    int iVar1;
    int active = *(int *)(param_1 + 0x10) != 0;
    if (active) {
        if (*(int *)(param_1 + 0x5cb) != *(int *)(param_1 + 0x5cf) ||
            *(int *)(param_1 + 0x5e7) != 0) {
            if (*(int *)(param_1 + 0x5e7) != 0) {
                *(int *)(param_1 + 0x5e7) = 0;
                iVar1 = f_00439d48(*(int *)(param_1 + 0x5c3));
                *(int *)(param_1 + 0x5f3) = iVar1;
            }
            *(int *)(param_1 + 0x5cf) = *(int *)(param_1 + 0x5cb);
            f_004465d6(param_1);
            f_00446350(param_1);
            GuiResetClip(&gMenuClip);
        }
        BriefRenderUpdate((void *)param_1);
        f_00454440((int)modeCheckName, DAT_005138ac - 0x4b, 0x14, 1, -1);
    }
}

void f_00446781(int param_1)
{
    int result = f_00445e23(DAT_004d1fa4);
    if (result != 0) {
        void *puVar1 = *(void **)(DAT_004d1fa4 + 0x4d);
        f_0044a190(param_1, *(int *)(param_1 + 0x1c));
        f_0044dd40(param_1, puVar1);
        if (result < 0) {
            f_0044a190(param_1, -result);
            return;
        }
        return;
    }
    ((HercModule *)DAT_004d1fa4)->Update();
}

int f_00446493(int param_1, int param_2, int *param_3, int *param_4)
{
    int local_28, local_24, local_20;
    int local_1c[2];
    int local_14 = 0;
    int iVar6 = 0;
    int iVar5 = 0;
    byte *base;
    int iVar4;
    f_0043ac02(*(int *)(param_1 + 0x5df), &local_28, &local_24, &local_20, local_1c);
    base = f_00438374(*(int *)(param_1 + 0x5c3), 0, -1);
    for (iVar4 = 0; iVar4 < 0x17; ++iVar4) {
        char *pcVar3 = (char *)base + iVar4 * 0x1a + 0x1b;
        if (*pcVar3 != '\0') {
            int iVar2 = ((unsigned int)(byte)pcVar3[local_24 + 2] * local_1c[0] + 0x32) / 100 +
                        ((unsigned int)(byte)pcVar3[local_28 + 2] * local_20 + 0x32) / 100;
            iVar6 += iVar2;
            if (iVar4 == param_2) local_14 = iVar2;
            if (iVar5 < iVar2) iVar5 = iVar2;
        }
    }
    *param_3 = iVar5;
    *param_4 = iVar6;
    return (local_14 * 100) / iVar6;
}

void f_0044579c(int *param_1, int param_2)
{
    unsigned int priorActive = (param_1[4] != 0);
    if (param_1[4] == 0) {
        DAT_004faf44 = priorActive;
        if (modeSwitch != 0 && DAT_004faf40 != 0)
            TimeSleep(0xfa);
        memset(param_1 + 0x13, 0,
               (unsigned long)((char *)param_1 + 0x60f - (char *)(param_1 + 0x13)));
        *(int *)((char *)param_1 + 0x5d7) = f_00477798(0xc);
        *(int *)((char *)param_1 + 0x5d3) = f_00476512(0x1d);
        f_0047822d(0x280, 0x1e0);
        f_0047bab0(0x513888, 0);
        f_0047b3a0(0x513888);
        f_004272cd(&DAT_00511948, DAT_00513890, DAT_005138ac, DAT_005138b0);
        f_0042732e(&DAT_00511958, DAT_005138ac, DAT_005138b0);
        DAT_00513898 = DAT_00511960;
        int *gui = gResMgr.Load(s_DMGGUI_BMX_0048b310, 1, 0, 0);
        *(int **)((char *)param_1 + 0x51) = gui;
        int *herc = gResMgr.Load(s_DMGHRC_BMX_0048b31b, 1, 0, 0);
        *(int **)((char *)param_1 + 0x55) = herc;
        f_004259f7(param_1, s_DMGGUI_BOX_0048b326);
        f_00425f87(param_1, param_2, &targetBriefResource, 0x12d, 0);
        f_0046c461(0x1e, 0);
        ((StrPool *)&DAT_00511df0)->Lock();
        *(int *)((char *)param_1 + 0x5f7) = 0;
        *(int *)((char *)param_1 + 0x5eb) = 0;
        f_0041c891(&g4b74f4, 0x927);
    }
}

void f_00446350(int param_1)
{
    f_0047bfa0(0x513888, 1, 0, 0x118, 0, 0x168, 0x28);
    f_0047bfa0(0x513888, 1, 0, 0x104, 0x28, 0x17c, 0x1b8);
    MenuSetClip(&gMenuClip, 0x104, 0, 0x17c, 0x1e0);
    DAT_004d1f63 &= 0xfe;
    DAT_004d1f8f &= 0xfe;
    *(byte *)(param_1 + 0x75a) &= 0xfe;
    DAT_005138e8 = param_1 + 0x4ab;
    int imageBase;
    int frame;
    if (*(int *)(param_1 + 0x5fb) == 0 ||
        *(int *)(param_1 + 0x5ff) < *(int *)(param_1 + 0x5fb)) {
        imageBase = *(int *)(param_1 + 0x55);
        frame = *(int *)(param_1 + 0x5c7);
    } else {
        imageBase = *(int *)(param_1 + 0x60b);
        frame = *(int *)(param_1 + 0x603);
    }
    f_0047bff0(&g513888, (int *)(frame * 0xc + imageBase + 4), 0x104, 0);
    DAT_005138e8 = 0;
    if (*(int *)(param_1 + 0x5ef) != 0) {
        DAT_005138f8 = (int)&DAT_004bc560;
        DAT_005138f4 = DAT_004bc4e8;
        byte *text = (byte *)targetBriefResource.GetString(0x151, 0);
        text = f_0047d420(text);
        f_0047cfc0(DAT_00511dc0, 0x1db, 0x1c5, 0, text);
    }
}


