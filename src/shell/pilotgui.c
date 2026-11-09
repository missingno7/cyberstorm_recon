#pragma aux pilot_gui_screen_ctor "W?$ct:GuiScreen$n(pnpn$GuiControl$$uiul)_";
#pragma aux pilot_gui_flic_ctor "W?$ct:Flic$n()_";
#pragma aux pilot_gui_flic_dtor "W?$dt:Flic$n()_";
#pragma aux pilot_gui_surface_ctor "W?$ct:Surface$n(ul)_";
#pragma aux pilot_gui_surface_dtor "W?$dt:Surface$n()_";
extern unsigned char gPilotGuiResourceTemplate;
struct GuiRect { long x, y, width, height; };
struct GuiControlData : GuiRect { unsigned char style[4]; unsigned long attributes, shortcut; };
class GuiControl : public GuiControlData
{
public:
    virtual ~GuiControl();
    virtual void Reset(unsigned);
    virtual void Draw(unsigned);
    virtual int Event(unsigned, unsigned long);
    virtual int HitTest(const void *);
};
struct GuiScreen
{
    char data[0x4c];
    __pragma("pilot_gui_screen_ctor") GuiScreen(GuiControl *, unsigned long, unsigned long);
    ~GuiScreen() { *(long *)((char *)this + 0x10) = 0; }
};
struct Flic
{
    char data[0x8c];
    __pragma("pilot_gui_flic_ctor") Flic();
    __pragma("pilot_gui_flic_dtor") ~Flic();
};
extern void FlicClose(void *);
#pragma aux FlicClose "W?Close$:Flic$n()v";
struct Surface
{
    char data[0x10];
    __pragma("pilot_gui_surface_ctor") Surface(unsigned long);
    __pragma("pilot_gui_surface_dtor") ~Surface();
};
struct PilotGuiModule : GuiScreen
{
    Flic first_flic;
    char gap1[0x16];
    Flic second_flic;
    char gap2[4];
    Surface palette_surface;
    PilotGuiModule();
};

#pragma aux pilot_gui_open_ctor "BriefModuleOpen";
#pragma aux pilot_gui_open_dtor "W?$dt:BriefModule$n()_";

struct PilotGuiOpenObject
{
    char data[0x2c];
    __pragma("pilot_gui_open_ctor") PilotGuiOpenObject(int, unsigned long, int);
    __pragma("pilot_gui_open_dtor") ~PilotGuiOpenObject();
};

class MenuHelpRegion : public GuiControl
{
public:
    long id;
    int activated;
    MenuHelpRegion();
    virtual ~MenuHelpRegion();
};

#pragma pack(push, 1)
class MenuLabel : public GuiControl
{
public:
    const char *text;
    long align;
    unsigned char font;
    int background;
    long color;
    MenuLabel(const char * = 0, long = -1, unsigned char = 1, long = 0xb, int = 1);
    virtual ~MenuLabel();
};
class MenuPrefixButton : public GuiControl
{
public:
    const char *text;
    unsigned long state;
    signed char font, color, align, prefix;
    MenuPrefixButton(const char * = 0, signed char = 1, signed char = 2, signed char = -1);
    virtual ~MenuPrefixButton();
};
#pragma pack(pop)

PilotGuiOpenObject gPilotGuiOpenA(0x33, 1000, 0);
PilotGuiOpenObject gPilotGuiOpenB(0x36, 1000, 0);
MenuHelpRegion gPilotGuiHelpRegion;
MenuLabel gPilotGuiLabel01(0, 1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel02(0, 1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel03(0, 1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel04(0, 0, 1, 0xb, 1);
MenuLabel gPilotGuiLabel05(0, 1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel06(0, 1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel07(0, 1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel08(0, 1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel09(0, 1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel10(0, 1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel11(0, 1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel12(0, 1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel13(0, 1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel14(0, 1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel15(0, -1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel16(0, -1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel17(0, -1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel18(0, -1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel19(0, -1, 1, 0xb, 1);
MenuLabel gPilotGuiLabelArray[8];
MenuLabel gPilotGuiLabel20(0, -1, 1, 0xb, 1);
MenuLabel gPilotGuiLabel21(0, -1, 1, 0xb, 1);
MenuPrefixButton gPilotGuiLabel22(0);
MenuPrefixButton gPilotGuiLabel23(0, 1, 2, -1);
MenuPrefixButton gPilotGuiLabel24(0, 1, 2, -1);
MenuPrefixButton gPilotGuiLabel25(0, 1, 2, -1);
MenuPrefixButton gPilotGuiLabel26(0, 1, 2, -1);
MenuPrefixButton gPilotGuiLabel27(0, 1, 2, -1);
MenuPrefixButton gPilotGuiLabel28(0, 1, 2, -1);
MenuPrefixButton gPilotGuiLabel29(0, 1, 2, -1);
PilotGuiModule g4b16a4;

extern unsigned char gPilotGuiLabel30;
extern unsigned char gPilotGuiLabel31;
extern unsigned char gPilotGuiLabel32;
extern unsigned char gPilotGuiLabel33;
extern int gPilotGuiBarImageIds[];
extern int g4affc2;

extern "C" void BriefModuleConstruct(void *);
extern "C" void BriefModuleOpen(void *, int, unsigned long, int);
extern void PilotMenuHelpRegionCtor(void *);
#pragma aux PilotMenuHelpRegionCtor "W?$ct:MenuHelpRegion$n()_";
extern void PilotMenuLabelCtor(void *, int, int, int, int, int);
#pragma aux PilotMenuLabelCtor "W?$ct:MenuLabel$n(pnxalucli)_";
extern void PilotMenuPrefixButtonCtor(void *, int, int, int, int);
#pragma aux PilotMenuPrefixButtonCtor "W?$ct:MenuPrefixButton$n(pnxaccc)_";
extern void PilotGuiCtorArray(void *, int, void *);
#pragma aux PilotGuiCtorArray "W?PopConstructorArray$n(pnvipnv)v";
extern void PilotGuiModuleConstructor(void *);
#pragma aux PilotGuiModuleConstructor "W?$ct:PilotGuiModule$n()_";

void PilotGuiBriefReset(char *p)
{
    *(long *)(p + 0x10) = 0;
}

PilotGuiModule::PilotGuiModule()
    : GuiScreen((GuiControl *)&gPilotGuiResourceTemplate, 0x28, 0), first_flic(), second_flic(), palette_surface(0)
{
}

extern void ReleasePoolData(void *);
extern void SurfaceFree(void *);
extern int chat_menu_valid(void *);
extern unsigned long gPilotChatMenu;

void PilotGuiCloseResources(void *p)
{
    if ((*(long *)((char *)p + 0x10) != 0) == 1) {
        ReleasePoolData(*(void **)((char *)p + 0x5ec));
        *(long *)((char *)p + 0x5ec) = 0;
        *(long *)((char *)p + 0xea) = 0;
        SurfaceFree((char *)p + 0x17e);
        FlicClose((char *)p + 0xee);
        *(long *)((char *)p + 0x6f0) = -1;
        *(long *)((char *)p + 0x10) = 0;
        if (gPilotChatMenu != 0) {
            chat_menu_valid((void *)gPilotChatMenu);
            gPilotChatMenu = 0;
        }
    }
}

struct ViewRect { int left, top, right, bottom; };
struct MenuImage { unsigned long offset; unsigned short width, height; unsigned long flags; };
struct PaletteColor { unsigned char red, green, blue, flags; };
struct Palette { PaletteColor entries[256]; unsigned char unknown[82]; };
extern GuiRect gMenuClip;
extern ViewRect viewportClipLeft, gBriefDrawSource;
extern char g513888;
extern unsigned char *gScreenImages;
extern char gShellGame;
extern unsigned char gPilotGuiProfileMode;
extern Palette gGuiPalette;
extern void *gDrawRemap;
extern void MenuSetClip(GuiRect *, long, long, long, long);
extern void MenuDrawImage(void *, const MenuImage *, long, long);
extern void f_0047b3d0(void *, long, long, long, long);
extern void PilotGuiLabelSetText(void *, const char *);
#pragma aux PilotGuiLabelSetText "W?SetText$:MenuLabel$n(pnxa)v";
extern void PilotGuiPrefixSetText(void *, const char *);
#pragma aux PilotGuiPrefixSetText "W?SetText$:MenuPrefixButton$n(pnxa)v";
extern unsigned char gPilotGuiFrameStep;
extern long gGuiEffectPriority;
extern void PilotGuiUpdateDetails(void *);
extern void PilotGuiRenderControls(void *);
extern void f_0041170c(void *);
extern int PilotProfileCount(void *);
#pragma aux PilotProfileCount "W?Count$:ProfileManager$n()i";
extern void *PilotProfileSelect(void *, int);
#pragma aux PilotProfileSelect "W?Select$:ProfileManager$n(i)pnv";
extern "C" int PilotValue(unsigned int, int);
extern int f_0043718a(unsigned int, int);
extern char *f_0043c73a(void *, unsigned int);
extern int FlicOpen(void *, long, int, int);
#pragma aux FlicOpen "W?Open$:Flic$n(lii)i";
extern unsigned long FlicUpdate(void *, Surface *);
#pragma aux FlicUpdate "W?Update$:Flic$n(pn$Surface$$)ul";
extern void PaletteCopyRange(Palette *, const PaletteColor *, int, int);
extern void PaletteSetColor(Palette *, PaletteColor *, int);
extern void ApplyPaletteData(void *, int, int);
extern unsigned char PaletteFindColor(Palette *, const PaletteColor *);
extern char *BriefFormat(const char *, ...);
extern char gPilotGuiFormat01[], gPilotGuiFormat02[], gPilotGuiFormat03[], gPilotGuiFormat04[];
extern char gPilotGuiFormat05[], gPilotGuiFormat06[], gPilotGuiFormat07[], gPilotGuiFormat08[];
extern char gPilotGuiFormat09[], gPilotGuiFormat10[], gPilotGuiFormat11[], gPilotGuiFormat12[];
extern char gPilotGuiFormat13[], gPilotGuiNameBuffer[];
extern char gPilotGuiNoPilots[], gPilotGuiUnlinked[];
extern "C" void SendEvent(void *, int, void *, int, int);
extern "C" int sprintf(char *, const char *, ...);
extern "C" void BriefStatus(void *);
extern "C" void BriefRenderUpdate(void *);
extern "C" void BriefBlit(void *, int, int, int, int, int, int, int, int);

void PilotGuiSelectionControlsRender(void *p)
{
    if ((*(long *)((char *)p + 0x10) != 0) == 1) {
        unsigned int mode = *(unsigned short *)((char *)p + 0xe4);
        if (mode == 2) {
            if (*(long *)((char *)p + 0x6fd) != 0)
                PilotGuiUpdateDetails(p);
            MenuDrawImage(&g513888, (const MenuImage *)((char *)gScreenImages + 0xdc), 0xb9, 9);
            PilotGuiRenderControls(p);
            MenuSetClip(&gMenuClip, 0, 0, 0x280, 0x96);
            BriefStatus(p);
            BriefRenderUpdate(p);
            return;
        }
        if (mode == 0) {
            *(short *)((char *)p + 0x6fb) += gPilotGuiFrameStep;
            if (*(short *)((char *)p + 0x6fb) > 9 || gGuiEffectPriority < 0x32) {
                *(short *)((char *)p + 0x6fb) = 9;
                *(short *)((char *)p + 0xe4) = 2;
                *(long *)((char *)p + 0x6f0) = -1;
            }
        } else if (mode == 1) {
            if (gGuiEffectPriority < 0x32) {
                *(short *)((char *)p + 0x6fb) = -0xa0;
                *(short *)((char *)p + 0xe4) = 3;
                return;
            }
            *(short *)((char *)p + 0x6fb) -= gPilotGuiFrameStep;
            if (*(short *)((char *)p + 0x6fb) < -0xa0) {
                *(short *)((char *)p + 0xe4) = 3;
                return;
            }
        } else {
            return;
        }
        BriefBlit(&g513888, 1, 0, 0x21a, 0x6b, 0x21a, 0x6b, 0x66, 0x1b);
        MenuSetClip(&gMenuClip, 0, 0, 0x280, 0x96);
        MenuDrawImage(&g513888, (const MenuImage *)((char *)gScreenImages + 0x10), 0xb5, 0);
        MenuDrawImage(&g513888, (const MenuImage *)((char *)gScreenImages + 4), 0xb9,
                      *(short *)((char *)p + 0x6fb));
    }
}

void PilotGuiDrawSelectionFrame(void)
{
    MenuSetClip(&gMenuClip, 0x18e, 0x15, 0x70, 0x70);
    viewportClipLeft = gBriefDrawSource;
    MenuDrawImage(&g513888, (const MenuImage *)((char *)gScreenImages + 0xd0), 0xb9, 9);
    f_0047b3d0(&g513888, 0x18e, 0x15, 0x70, 0x70);
    PilotGuiPrefixSetText(&gPilotGuiLabel22, " ");
    PilotGuiLabelSetText(&gPilotGuiLabel01, " ");
    PilotGuiLabelSetText(&gPilotGuiLabel02, " ");
    PilotGuiLabelSetText(&gPilotGuiLabel03, " ");
    PilotGuiLabelSetText(&gPilotGuiLabel13, " ");
    PilotGuiLabelSetText(&gPilotGuiLabel14, " ");
    PilotGuiLabelSetText(&gPilotGuiLabel04, " ");
    PilotGuiLabelSetText(&gPilotGuiLabel05, " ");
    PilotGuiLabelSetText(&gPilotGuiLabel06, " ");
    PilotGuiLabelSetText(&gPilotGuiLabel07, " ");
    PilotGuiLabelSetText(&gPilotGuiLabel08, " ");
    PilotGuiLabelSetText(&gPilotGuiLabel09, " ");
    PilotGuiLabelSetText(&gPilotGuiLabel10, " ");
    PilotGuiLabelSetText(&gPilotGuiLabel11, " ");
    PilotGuiLabelSetText(&gPilotGuiLabel12, " ");
}

void PilotGuiUpdateDetails(void *p)
{
    unsigned long color;
    void *selected = PilotProfileSelect(&gShellGame, -1);
    *(void **)((char *)p + 0x705) = selected;
    if (PilotProfileCount(&gShellGame) == 0 && gPilotGuiProfileMode != 2) {
        PilotGuiDrawSelectionFrame();
        f_0041170c(p);
    } else if (*(void **)((char *)p + 0x705) == 0 || *(long *)((char *)p + 0x6fd) == 0) {
        PilotGuiDrawSelectionFrame();
    } else {
        unsigned int rank = PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xca);
        *(short *)((char *)p + 0x6f9) = (short)rank;
        unsigned int wing = PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xd7);
        int paletteIndex = *(short *)((char *)p + 0x6f9) * 100 + (wing - 0x41);
        if (paletteIndex != *(long *)((char *)p + 0x6f0)) {
            f_0041170c(p);
            *(long *)((char *)p + 0x6f0) = paletteIndex;
            char *path = BriefFormat("BGMSH%02d%c.FLX", *(short *)((char *)p + 0x6f9) + 1,
                                     (char)(wing - 0x41 + 0x41));
            FlicOpen((char *)p + 0xee, (long)path, 100, 0);
            MenuSetClip(&gMenuClip, 0x18e, 0x15, 0x70, 0x70);
            viewportClipLeft = gBriefDrawSource;
            MenuDrawImage(&g513888, (const MenuImage *)((char *)gScreenImages + 0xd0), 0xb9, 9);
            f_0047b3d0(&g513888, 0x18e, 0x15, 0x70, 0x70);
            *(Palette *)((char *)p + 0x19a) = gGuiPalette;
            PaletteCopyRange((Palette *)((char *)p + 0x19a),
                (const PaletteColor *)(*(long *)((char *)p + 0x5ec) +
                    (*(short *)((char *)p + 0x6f9) * 3 + (wing - 0x41)) * 0x94), 0xab, 0x25);
            if (*(long *)((char *)p + 0x701) != 0) {
                for (int i = 0; i < 0x25; ++i) {
                    int shade = (i * 0xff) / 0x24;
                    PaletteColor *color = (PaletteColor *)((char *)p + 0x19a + 0x2ac + i * 4);
                    color->red = (unsigned char)shade;
                    color->green = (unsigned char)shade;
                    color->blue = (unsigned char)shade;
                    PaletteSetColor((Palette *)((char *)p + 0x19a), color, i + 0xab);
                }
            }
            ApplyPaletteData((char *)p + 0x19a, 0, 0x100);
        }
        if (FlicUpdate((char *)p + 0xee, (Surface *)((char *)p + 0x17e)) & 0x20) {
            unsigned long *pixels = *(unsigned long **)((char *)p + 0x12a);
            if (*(long *)((char *)p + 0x701) == 0) {
                for (int i = 0; i < 0x100; ++i)
                    *((unsigned char *)p + 0x5f0 + i) = PaletteFindColor(
                        (Palette *)((char *)p + 0x19a), (const PaletteColor *)(pixels + i));
            } else {
                for (int i = 0; i < 0x100; ++i) {
                    color = pixels[i];
                    unsigned char shade = (unsigned char)(((color >> 16) & 0xff) + (color & 0xff) +
                                                          ((color >> 8) & 0xff)) / 3;
                    ((unsigned char *)&color)[0] = shade;
                    ((unsigned char *)&color)[1] = shade;
                    ((unsigned char *)&color)[2] = shade;
                    *((unsigned char *)p + 0x5f0 + i) = PaletteFindColor(
                        (Palette *)((char *)p + 0x19a), (const PaletteColor *)&color);
                }
            }
        }
        gDrawRemap = (char *)p + 0x5f0;
        MenuDrawImage(&g513888, (const MenuImage *)((char *)p + 0x18e), 0x18e, 0x15);
        MenuSetClip(&gMenuClip, 0x18e, 0x15, 0x70, 0x70);
        gDrawRemap = 0;
    }
}

void f_0041170c(void *p)
{
    const char *lastText;
    if (PilotProfileCount(&gShellGame) > 0) {
        *(void **)((char *)p + 0x705) = PilotProfileSelect(&gShellGame, -1);
        SendEvent(*(void **)((char *)p + 0x705), 0x67, gPilotGuiNameBuffer, 0, 0);
        if (*(long *)((char *)p + 0x701) != 0)
            PilotGuiLabelSetText(&gPilotGuiLabel04, " ");
        else {
            char *link = f_0043c73a(&gShellGame, (unsigned int)*(void **)((char *)p + 0x705));
            if (link == 0)
                link = gPilotGuiUnlinked;
            PilotGuiLabelSetText(&gPilotGuiLabel04, link);
        }
        sprintf(gPilotGuiFormat01, "%2d/%2d",
                PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xc8),
                PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xc9));
        sprintf(gPilotGuiFormat02, "%2d/%2d",
                PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xcc),
                PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xcd));
        sprintf(gPilotGuiFormat03, "%2d/%2d",
                PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xcf),
                PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xd0));
        sprintf(gPilotGuiFormat04, "%2d", f_0043718a((unsigned int)*(void **)((char *)p + 0x705), 6));
        sprintf(gPilotGuiFormat05, "%2d", f_0043718a((unsigned int)*(void **)((char *)p + 0x705), 0));
        sprintf(gPilotGuiFormat06, "%2d", f_0043718a((unsigned int)*(void **)((char *)p + 0x705), 1));
        sprintf(gPilotGuiFormat07, "%2d", f_0043718a((unsigned int)*(void **)((char *)p + 0x705), 2));
        sprintf(gPilotGuiFormat08, "%2d", f_0043718a((unsigned int)*(void **)((char *)p + 0x705), 3));
        sprintf(gPilotGuiFormat09, "%2d", f_0043718a((unsigned int)*(void **)((char *)p + 0x705), 4));
        sprintf(gPilotGuiFormat10, "%2d", f_0043718a((unsigned int)*(void **)((char *)p + 0x705), 5));
        sprintf(gPilotGuiFormat11, "%2d", f_0043718a((unsigned int)*(void **)((char *)p + 0x705), 7));
        sprintf(gPilotGuiFormat12, "%2d/%2d",
                PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xd3),
                PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xd4));
        sprintf(gPilotGuiFormat13, "%2d", PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xd6));
        PilotGuiLabelSetText(&gPilotGuiLabel01, gPilotGuiFormat01);
        PilotGuiLabelSetText(&gPilotGuiLabel02, gPilotGuiFormat02);
        PilotGuiLabelSetText(&gPilotGuiLabel03, gPilotGuiFormat03);
        PilotGuiLabelSetText(&gPilotGuiLabel13, gPilotGuiFormat13);
        PilotGuiLabelSetText(&gPilotGuiLabel14, gPilotGuiFormat12);
        PilotGuiPrefixSetText(&gPilotGuiLabel22, gPilotGuiNameBuffer);
        PilotGuiLabelSetText(&gPilotGuiLabel05, gPilotGuiFormat04);
        PilotGuiLabelSetText(&gPilotGuiLabel06, gPilotGuiFormat05);
        PilotGuiLabelSetText(&gPilotGuiLabel07, gPilotGuiFormat06);
        PilotGuiLabelSetText(&gPilotGuiLabel08, gPilotGuiFormat07);
        PilotGuiLabelSetText(&gPilotGuiLabel09, gPilotGuiFormat08);
        PilotGuiLabelSetText(&gPilotGuiLabel10, gPilotGuiFormat09);
        PilotGuiLabelSetText(&gPilotGuiLabel11, gPilotGuiFormat10);
        lastText = gPilotGuiFormat11;
    } else {
        *(long *)((char *)p + 0x705) = 0;
        PilotGuiLabelSetText(&gPilotGuiLabel01, " ");
        PilotGuiLabelSetText(&gPilotGuiLabel02, " ");
        PilotGuiLabelSetText(&gPilotGuiLabel03, " ");
        PilotGuiLabelSetText(&gPilotGuiLabel13, " ");
        PilotGuiLabelSetText(&gPilotGuiLabel14, " ");
        PilotGuiPrefixSetText(&gPilotGuiLabel22, gPilotGuiNoPilots);
        PilotGuiLabelSetText(&gPilotGuiLabel04, " ");
        PilotGuiLabelSetText(&gPilotGuiLabel05, " ");
        PilotGuiLabelSetText(&gPilotGuiLabel06, " ");
        PilotGuiLabelSetText(&gPilotGuiLabel07, " ");
        PilotGuiLabelSetText(&gPilotGuiLabel08, " ");
        PilotGuiLabelSetText(&gPilotGuiLabel09, " ");
        PilotGuiLabelSetText(&gPilotGuiLabel10, " ");
        PilotGuiLabelSetText(&gPilotGuiLabel11, " ");
        lastText = " ";
    }
    PilotGuiLabelSetText(&gPilotGuiLabel12, lastText);
}

void PilotGuiDrawBars(void *p, int row, int value, int maximum)
{
    int bars = 0;
    if (maximum != 0) {
        bars = ((value * 120) / 4 + maximum / 2) / maximum;
    }
    if (bars <= 1)
        bars = 1;
    else if (bars >= 30)
        bars = 0x1e;
    int image = gPilotGuiBarImageIds[row];
    int i;
    for (i = 0; i < bars; ++i)
        MenuDrawImage(&g513888,
            (const MenuImage *)((char *)gScreenImages + (row * 2 + 9) * 0xc + 4),
            i * 4 + 0x1fe, image);
    for (i = bars; i < 0x1e; ++i)
        MenuDrawImage(&g513888,
            (const MenuImage *)((char *)gScreenImages + (row * 2 + 10) * 0xc + 4),
            i * 4 + 0x1fe, image);
}

void PilotGuiRenderControls(void *p)
{
    if (g4affc2 != 2 && g4affc2 != 1 &&
        g4affc2 != 3 && *(long *)((char *)p + 0x705) != 0) {
        PilotGuiDrawBars(p, 1,
            PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xc8),
            PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xc9));
        PilotGuiDrawBars(p, 2,
            PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xcc),
            PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xcd));
        PilotGuiDrawBars(p, 3,
            PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xcf),
            PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xd0));
    }
}

#pragma aux gShellGame "W?gShellGame$n$GameSession$$";
extern int PilotProfileCount(void *);
#pragma aux PilotProfileCount "W?Count$:ProfileManager$n()i";
extern void PilotProfileAdvance(void *, int);
#pragma aux PilotProfileAdvance "W?Advance$:ProfileManager$n(i)v";
extern void *PilotProfileSelect(void *, int);
#pragma aux PilotProfileSelect "W?Select$:ProfileManager$n(i)pnv";
extern void f_0041170c(void *);
extern void PilotProfileAdvance(void *, int);
#pragma aux PilotProfileAdvance "W?Advance$:ProfileManager$n(i)v";
extern "C" int BriefDone(const char *);
extern "C" int BriefGetHandle(void *, void *);
extern "C" void BriefStateSet(void *);
extern int f_0043bf81(void *, int);
extern void f_00476412(int);
struct PilotKey { unsigned long first, second; };
extern "C" void PlyGetKey(PilotKey *);
extern int f_0043d2b9(void *, const PilotKey *);
extern int f_0043d591(void *, const PilotKey *);
extern void PilotGuiContextMenu(void *, int);
extern char gPilotGuiPromptBuffer[], gPilotGuiSpecialKey[];
extern "C" int strcmp(const char *, const char *);
extern void f_0046c461(int, void *);
extern long gPilotGuiSelection;
extern unsigned long gPilotGuiModified;
struct GuiPoint { long x, y; };
extern GuiPoint g4bb78c;

void PilotGuiHandleInput(void *p)
{
    if ((*(long *)((char *)p + 0x10) != 0) == 1) {
        unsigned int key = BriefGetHandle(p, &g4bb78c);
        gPilotGuiSelection = key;
        if ((int)key < 0) {
            key = (unsigned int)BriefDone("bioroom");
            if ((int)key >= 0)
                gPilotGuiSelection = key + 100;
        }
        if (key == 0x20) {
            int helpPressed = ((*(unsigned int *)(*(unsigned long *)((char *)p + 0x10) + 4) & 0xa0) == 0x20);
            if (helpPressed) {
                gPilotGuiSelection = 0x981;
                if (PilotProfileCount(&gShellGame) > 0) {
                    f_0043bf81(&gShellGame, (int)*(void **)((char *)p + 0x705));
                    f_00476412(*(short *)((char *)p + 0x6f9) + 0x20);
                }
            }
        }
        BriefStateSet(p);
    }
}

void PilotGuiDone(void)
{
    BriefDone("bioroom");
}

void PilotGuiNextProfile(void *p)
{
    if (PilotProfileCount(&gShellGame) > 0) {
        PilotProfileAdvance(&gShellGame, 1);
        *(long *)((char *)p + 0x6f5) = 1;
        f_0041170c(p);
    }
}

void PilotGuiPreviousProfile(void *p)
{
    if (PilotProfileCount(&gShellGame) > 1) {
        PilotProfileAdvance(&gShellGame, -1);
        *(long *)((char *)p + 0x6f5) = 1;
        f_0041170c(p);
    }
}

extern "C" void f_0043bed7(void *, int);
void PilotGuiPreviousProfileByKey(void *p)
{
    if (PilotProfileCount(&gShellGame) > 1) {
        PilotKey key;
        PilotKey search;
        PlyGetKey(&key);
        search.first = key.first;
        search.second = key.second;
        int index = f_0043d2b9(&gShellGame, &search);
        if (index >= 0) {
            f_0043bed7(&gShellGame, index + 1);
            *(void **)((char *)p + 0x705) = PilotProfileSelect(&gShellGame, -1);
            f_0041170c(p);
        }
    }
}

void PilotGuiSelectProfileByKey(void *p)
{
    if (PilotProfileCount(&gShellGame) > 0) {
        PilotKey key;
        PilotKey search;
        PlyGetKey(&key);
        search.first = key.first;
        search.second = key.second;
        if (f_0043d591(&gShellGame, &search)) {
            PilotGuiContextMenu(p, 1);
            f_0041170c(p);
            char *profile = (char *)PilotProfileSelect(&gShellGame, -1);
            *(char **)((char *)p + 0x705) = profile;
            if (profile != 0) {
                SendEvent(profile, 0x67, gPilotGuiPromptBuffer, 0, 0);
                if (strcmp(gPilotGuiPromptBuffer, gPilotGuiSpecialKey) == 0)
                    f_0046c461(0x15d, 0);
            }
        }
        gPilotGuiModified = 1;
    }
}

void PilotGuiEnable(void *p)
{
    *(long *)((char *)p + 0x701) = 1;
    *(char *)((char *)p + 0x6f4) = 1;
}

extern unsigned char gPilotGuiFlags;
void PilotGuiDisable(void *p)
{
    *(char *)((char *)p + 0x6f4) = 0;
    *(long *)((char *)p + 0x701) = 0;
    gPilotGuiFlags &= 0xfe;
}

extern unsigned long gPilotGuiPalette;
void PilotGuiPaletteForward(void)
{
    gPilotGuiPalette = (gPilotGuiPalette & 0xff1fffff) + 0x200000;
}
void PilotGuiPaletteBackward(void)
{
    unsigned long palette = gPilotGuiPalette & 0xff1fffffUL;
    gPilotGuiPalette = palette;
}

void PilotGuiSelectProfileAt(void *p, int index)
{
    if (index > 0) {
        if (index <= PilotProfileCount(&gShellGame)) {
            f_0043bed7(&gShellGame, index);
            *(long *)((char *)p + 0x705) = (long)PilotProfileSelect(&gShellGame, -1);
            *(long *)((char *)p + 0x6f5) = 1;
            f_0041170c(p);
        }
    }
}

extern int chat_menu_valid(void *);
extern int f_0043c73a(void *, int);
extern void *f_00477a08(const char *, int);
extern void f_00477ad2(void *);
extern void f_00477b03(void *, int, const char *, int);
void PilotGuiReleaseChatMenu(void)
{
    if (gPilotChatMenu) {
        chat_menu_valid((void *)gPilotChatMenu);
        gPilotChatMenu = 0;
    }
}

void PilotGuiContextMenu(void *p, int menu)
{
    char item[128];
    char profileName[64];
    volatile int selected;
    int count = PilotProfileCount(&gShellGame);
    if (count < 2) {
        if (gPilotChatMenu != 0) {
            chat_menu_valid((void *)gPilotChatMenu);
            gPilotChatMenu = 0;
        }
    }
    else {
        void *profile = PilotProfileSelect(&gShellGame, -1);
        *(void **)((char *)p + 0x705) = profile;
        selected = f_0043bf81(&gShellGame, (int)profile);
        if (menu == 0) {
            if (gPilotChatMenu != 0) {
                chat_menu_valid((void *)gPilotChatMenu);
                gPilotChatMenu = (unsigned long)menu;
            }
        }
        if (gPilotChatMenu == 0)
            gPilotChatMenu = (unsigned long)f_00477a08("Pilots", -1);
        if (gPilotChatMenu != 0) {
            f_00477ad2((void *)gPilotChatMenu);
            f_0043bed7(&gShellGame, 1);
            int i = 0;
            while (i < PilotProfileCount(&gShellGame)) {
                char *current = (char *)PilotProfileSelect(&gShellGame, -1);
                SendEvent(current, 0x67, profileName, 0, 0);
                if (*(char *)((char *)p + 0x6f4) == 1) {
                    sprintf(item, "%s", profileName);
                }
                else {
                    char *description = (char *)f_0043c73a(&gShellGame, (int)current);
                    if (description == 0)
                        sprintf(item, "%s (%s)", profileName, " unlinked");
                    else
                        sprintf(item, "%s (%s)", profileName, description);
                }
                f_00477b03((void *)gPilotChatMenu, i + 0x4e21, item, 0x28);
                PilotProfileAdvance(&gShellGame, 1);
                ++i;
            }
        }
        if (selected == 0)
            selected = 1;
        f_0043bed7(&gShellGame, selected);
    }
}

void PilotGuiClearProfileModified(void *p)
{
    *(long *)((char *)p + 0x6fd) = 0;
}
void PilotGuiSetProfileModified(void *p)
{
    *(long *)((char *)p + 0x6fd) = 1;
}





