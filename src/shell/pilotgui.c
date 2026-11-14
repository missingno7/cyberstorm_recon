#pragma aux pilot_gui_screen_ctor "W?$ct:GuiScreen$n(pnpn$GuiControl$$uiul)_";
#pragma aux pilot_gui_flic_ctor "W?$ct:Flic$n()_";
#pragma aux pilot_gui_flic_dtor "W?$dt:Flic$n()_";
#pragma aux pilot_gui_surface_ctor "W?$ct:Surface$n(ul)_";
#pragma aux pilot_gui_surface_dtor "W?$dt:Surface$n()_";
int gPilotGuiBarImageIds[4] = {0,32,75,119}; static char pilotguiSourceFile[] = __FILE__; class GuiControl; extern GuiControl *gPilotGuiResourceTemplate[40];
struct GuiRect { long x, y, width, height; }; struct GuiControlData : GuiRect { unsigned char style[4]; unsigned long attributes, shortcut; }; class GuiControl : public GuiControlData { public:     virtual ~GuiControl();     virtual void Reset(unsigned);     virtual void Draw(unsigned);     virtual int Event(unsigned, unsigned long);     virtual int HitTest(const void *); };
struct GuiScreen {     char data[0x4c];     __pragma("pilot_gui_screen_ctor") GuiScreen(GuiControl *, unsigned long, unsigned long);     int IsActive() {return *(long *)((char *)this+0x10)!=0;}     ~GuiScreen() { *(long *)((char *)this + 0x10) = 0; } };
struct Flic {     char data[0x3c]; unsigned long *palettePixels; char remaining[0x50];     __pragma("pilot_gui_flic_ctor") Flic();     __pragma("pilot_gui_flic_dtor") ~Flic(); };
extern void FlicClose(void *);
#pragma aux FlicClose "W?Close$:Flic$n()v";
struct Surface {     char data[0x10];     __pragma("pilot_gui_surface_ctor") Surface(unsigned long);     __pragma("pilot_gui_surface_dtor") ~Surface(); };
struct ViewRect { int left, top, right, bottom; };
struct MenuImage { unsigned long offset; unsigned short width, height; unsigned long flags; };
struct MenuImages { unsigned count; MenuImage images[1]; };
struct PaletteColor { unsigned char red, green, blue, flags; };
struct Palette { PaletteColor entries[256]; unsigned char unknown[82]; };
#pragma pack(push,1)
struct PilotGuiFields {
 char unknown00[0x10]; long input; char unknown14[0x38]; Flic firstAnimation;
 long animationState,animationSpeed; short phase; char unknownE6[4]; long animationEnabled;
 Flic animation; Surface surface; MenuImage image; Palette palette; long paletteRows;
 unsigned char remap[256]; long paletteIndex; char profileFlag; long refresh;
 short rank,slideY; long modified,gray; void *selected;
};
#pragma pack(pop)
struct PilotGuiModule : GuiScreen {     Flic first_flic;     char gap1[0x12];     Flic second_flic;      Surface palette_surface;     PilotGuiModule(); };
#pragma aux pilot_gui_open_ctor "BriefModuleOpen";
#pragma aux pilot_gui_open_dtor "W?$dt:BriefModule$n()_";
struct PilotGuiOpenObject {     char data[0x2c];     __pragma("pilot_gui_open_ctor") PilotGuiOpenObject(int, unsigned long, int);     __pragma("pilot_gui_open_dtor") ~PilotGuiOpenObject(); };
class MenuHelpRegion : public GuiControl { public:     long id;     int activated;     MenuHelpRegion();     virtual ~MenuHelpRegion(); };
#pragma pack(push, 1)
class MenuLabel : public GuiControl { public:     const char *text;     long align;     unsigned char font;     int background;     long color;     MenuLabel(const char * = 0, long = -1, unsigned char = 1, long = 0xb, int = 1);     virtual ~MenuLabel();     void SetFont(unsigned char); void SetText(const char *); void SetColor(long); void SetAlign(long); void SetBackground(int); };
class MenuPrefixButton : public GuiControl { public:     const char *text;     unsigned long state;     signed char font, color, align, prefix;     MenuPrefixButton(const char * = 0, signed char = 1, signed char = 2, signed char = -1);     virtual ~MenuPrefixButton();     void SetColor(signed char); void SetFont(signed char); void SetAlign(signed char); };
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

extern "C" void BriefModuleOpen(void *, int, unsigned long, int);














PilotGuiModule::PilotGuiModule()
    : GuiScreen((GuiControl *)&gPilotGuiResourceTemplate, 0x28, 0), first_flic(), second_flic(), palette_surface(0)
{
}
extern void ReleasePoolData(void *);
extern void SurfaceFree(void *);
extern int chat_menu_valid(void *);
extern unsigned long gPilotChatMenu;

struct StrPool { char *GetString(long,int=0); }; extern StrPool gShellStrings;
struct ResMgr {void *Load(long,int=1,int=0,int=0);}; extern ResMgr gResMgr;
struct MenuImages; extern MenuImages *gMenuImages;
extern int SurfaceCreate(Surface *,long,long);
extern "C" void SurfaceGetFrame(Surface *,void *);
extern "C" void BriefObjectOpen(void *,const char *);
extern "C" void BriefObjectLoadResource(void *,void *,StrPool *,long,int);
extern "C" void AssertFail(const char *,int,const char *);
extern int PilotProfileCount(void *); extern void *PilotProfileSelect(void *,int);
extern void f_0041170c(void *); extern "C" void f_004120fe(void *,int);
extern char gShellGame; extern long gPilotGuiSelection;
extern unsigned char *gScreenImages; extern void *gShellImages;
void PilotGuiOpen(void *p, void *input)
{
    GuiScreen *screen=(GuiScreen *)p;
    if (screen->IsActive()) return;
    ((PilotGuiFields *)p)->gray=0;
    ((PilotGuiFields *)p)->animationEnabled=1;
    int made=SurfaceCreate(&((PilotGuiFields *)p)->surface,0x70,0x70);

















    if(!made) AssertFail(pilotguiSourceFile,__LINE__,(const char *)made);
    SurfaceGetFrame(&((PilotGuiFields *)p)->surface,(char *)&((PilotGuiFields *)p)->image);
    *(void **)((char *)p+0x5ec)=gResMgr.Load((long)"BGMPALS.PLX");
    ((PilotGuiFields *)p)->paletteIndex=-1;
    BriefObjectOpen(p,"pilotgui.box");
    BriefObjectLoadResource(p,input,&gShellStrings,0x961,1);
    gPilotGuiSelection=-1;
    gPilotGuiLabel04.SetFont(1); gPilotGuiLabel22.SetAlign(1);
    gPilotGuiLabel21.SetText(gShellStrings.GetString(0x980,1));
    gPilotGuiLabel15.SetText(gShellStrings.GetString(0x971,1));
    gPilotGuiLabel16.SetText(gShellStrings.GetString(0x972,1));
    gPilotGuiLabel17.SetText(gShellStrings.GetString(0x973,1));
    gPilotGuiLabel18.SetText(gShellStrings.GetString(0x974,1));
    gPilotGuiLabel19.SetText(gShellStrings.GetString(0x975,1));
    gPilotGuiLabel20.SetText(gShellStrings.GetString(0x97f,1));
    gPilotGuiLabel17.SetFont(0); gPilotGuiLabel18.SetFont(0); gPilotGuiLabel19.SetFont(0);
    gPilotGuiLabel17.SetBackground(0); gPilotGuiLabel18.SetBackground(0); gPilotGuiLabel19.SetBackground(0);
    gPilotGuiLabel13.SetBackground(0); gPilotGuiLabel14.SetBackground(0); gPilotGuiLabel04.SetBackground(0);
    gPilotGuiLabel21.SetBackground(0); gPilotGuiLabel15.SetBackground(0); gPilotGuiLabel16.SetBackground(0);
    gPilotGuiLabel20.SetBackground(0); gPilotGuiLabel01.SetBackground(0); gPilotGuiLabel02.SetBackground(0); gPilotGuiLabel03.SetBackground(0);
    for(int i=0;i<8;i++) {gPilotGuiLabelArray[i].SetFont(0);gPilotGuiLabelArray[i].SetText(gShellStrings.GetString(0x976+i,1));}
    gPilotGuiLabel21.SetAlign(0); gPilotGuiLabel15.SetAlign(0); gPilotGuiLabel16.SetAlign(0); gPilotGuiLabel22.SetFont(0);
    gPilotGuiLabel20.SetAlign(0); gPilotGuiLabel17.SetAlign(0); gPilotGuiLabel18.SetAlign(0); gPilotGuiLabel19.SetAlign(0); gPilotGuiLabel22.SetColor(0xb);
    GuiRect *prev=(GuiRect *)&gPilotGuiOpenA; prev->x=0x171; prev->y=0x8b;prev->width=0x14;prev->height=0x19;
    gMenuImages=(MenuImages *)gShellImages;
    GuiRect *next=(GuiRect *)&gPilotGuiOpenB; next->x=0x208; next->y=0x8b;next->width=0x14;next->height=0x19;
    ((PilotGuiFields *)p)->rank=0;*(void **)((char *)p+0x705)=0;
    if(PilotProfileCount(&gShellGame)>0) *(void **)((char *)p+0x705)=PilotProfileSelect(&gShellGame,-1);
    f_0041170c(p);
    ((PilotGuiFields *)p)->refresh=1;((PilotGuiFields *)p)->phase=3;((PilotGuiFields *)p)->slideY=-150;
    ((PilotGuiFields *)p)->animationSpeed=9;((PilotGuiFields *)p)->animationState=1;
    f_004120fe(p,0);
}
void PilotGuiCloseResources(void *p)
{
    if ((((PilotGuiFields *)p)->input != 0) == 1) {
        ReleasePoolData(*(void **)((char *)p + 0x5ec));
        ((PilotGuiFields *)p)->paletteRows = 0;
        ((PilotGuiFields *)p)->animationEnabled = 0;
        SurfaceFree((char *)&((PilotGuiFields *)p)->surface);
        FlicClose((char *)&((PilotGuiFields *)p)->animation);
        ((PilotGuiFields *)p)->paletteIndex = -1;
        ((PilotGuiFields *)p)->input = 0;
        if (gPilotChatMenu != 0) {
            chat_menu_valid((void *)gPilotChatMenu);
            gPilotChatMenu = 0;
        }
    }
}






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
extern int UnitCapability(void *, int);
extern char *f_0043c73a(void *, unsigned int);
extern int FlicOpen(void *, long, int = 100, int = 0);
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
extern void SendEvent(void *, int, void *, int = 0, int = 0);
extern "C" int sprintf(char *, const char *, ...);
extern "C" void BriefStatus(void *);
extern "C" void BriefRenderUpdate(void *);
extern "C" void BriefBlit(void *, int, int, int, int, int, int, int, int);

void PilotGuiSelectionControlsRender(void *p)
{
    if ((((PilotGuiFields *)p)->input != 0) == 1) {
        unsigned int mode = (unsigned short)((PilotGuiFields *)p)->phase;
        if (mode == 2) {
            if (((PilotGuiFields *)p)->modified != 0)
                PilotGuiUpdateDetails(p);
            MenuDrawImage(&g513888, (const MenuImage *)((char *)gScreenImages + 0xdc), 0xb9, 9);
            PilotGuiRenderControls(p);
            MenuSetClip(&gMenuClip, 0, 0, 0x280, 0x96);
            BriefStatus(p);
            BriefRenderUpdate(p);
            return;
        }
        if (mode == 0) {
            ((PilotGuiFields *)p)->slideY += gPilotGuiFrameStep;
            if (((PilotGuiFields *)p)->slideY > 9 || gGuiEffectPriority < 0x32) {
                ((PilotGuiFields *)p)->slideY = 9;
                ((PilotGuiFields *)p)->phase = 2;
                ((PilotGuiFields *)p)->paletteIndex = -1;
            }
        } else if (mode == 1) {
            if (gGuiEffectPriority < 0x32) {
                ((PilotGuiFields *)p)->slideY = -0xa0;
                ((PilotGuiFields *)p)->phase = 3;
                return;
            }
            ((PilotGuiFields *)p)->slideY -= gPilotGuiFrameStep;
            if (((PilotGuiFields *)p)->slideY < -0xa0) {
                ((PilotGuiFields *)p)->phase = 3;
                return;
            }
        } else {
            return;
        }
        BriefBlit(&g513888, 1, 0, 0x21a, 0x6b, 0x21a, 0x6b, 0x66, 0x1b);
        MenuSetClip(&gMenuClip, 0, 0, 0x280, 0x96);
        MenuDrawImage(&g513888, (const MenuImage *)((char *)gScreenImages + 0x10), 0xb5, 0);
        MenuDrawImage(&g513888, (const MenuImage *)((char *)gScreenImages + 4), 0xb9,
                      ((PilotGuiFields *)p)->slideY);
    }
}

void PilotGuiDrawSelectionFrame(void *p)
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
    void *selected = PilotProfileSelect(&gShellGame, -1);
    *(void **)((char *)p + 0x705) = selected;
    if (PilotProfileCount(&gShellGame) == 0 && gPilotGuiProfileMode != 2) {
        PilotGuiDrawSelectionFrame(p);
        f_0041170c(p);
    } else if (*(void **)((char *)p + 0x705) != 0 && ((PilotGuiFields *)p)->modified != 0) {
        unsigned int rank = PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xca);
        ((PilotGuiFields *)p)->rank = (short)rank;
        int wing = PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xd7) - 0x41;
        int paletteIndex = ((PilotGuiFields *)p)->rank * 100 + wing;
        if (paletteIndex != ((PilotGuiFields *)p)->paletteIndex) {
            f_0041170c(p);
            ((PilotGuiFields *)p)->paletteIndex = paletteIndex;
            char *path = BriefFormat("BGMSH%02d%c.FLX", ((PilotGuiFields *)p)->rank + 1,
                                     wing + 0x41);
            FlicOpen((char *)&((PilotGuiFields *)p)->animation, (long)path);
            MenuSetClip(&gMenuClip, 0x18e, 0x15, 0x70, 0x70);
            viewportClipLeft = gBriefDrawSource;
            MenuDrawImage(&g513888, (const MenuImage *)((char *)gScreenImages + 0xd0), 0xb9, 9);
            f_0047b3d0(&g513888, 0x18e, 0x15, 0x70, 0x70);
            ((PilotGuiFields *)p)->palette = gGuiPalette;
            PaletteCopyRange(&((PilotGuiFields *)p)->palette,
                (const PaletteColor *)(((PilotGuiFields *)p)->paletteRows +
                    (((PilotGuiFields *)p)->rank * 3 + wing) * 0x94), 0xab, 0x25);
            if (((PilotGuiFields *)p)->gray != 0) {
                for (int i = 0; i < 0x25; ++i) {
                    int shade = (i * 0xff) / 0x24;
                    PaletteColor *color = (PaletteColor *)((char *)&((PilotGuiFields *)p)->palette + 0x2ac + i * 4);
                    color->red = color->green = color->blue = (unsigned char)shade;
                    PaletteSetColor(&((PilotGuiFields *)p)->palette, color, i + 0xab);
                }
            }
            ApplyPaletteData((char *)&((PilotGuiFields *)p)->palette, 0, 0x100);
        }
        if (FlicUpdate((char *)&((PilotGuiFields *)p)->animation, &((PilotGuiFields *)p)->surface) & 0x20) {
            unsigned long *pixels = ((PilotGuiFields *)p)->animation.palettePixels;
            if (((PilotGuiFields *)p)->gray != 0) {
                for (int i = 0; i < 0x100; ++i) {
                    PaletteColor color = *(PaletteColor *)(pixels+i);
                    color.red = color.green = color.blue = (color.red + color.green + color.blue) / 3;
                    ((PilotGuiFields *)p)->remap[i] = PaletteFindColor(
                        &((PilotGuiFields *)p)->palette, (const PaletteColor *)&color);
                }
            } else {
                for (int i = 0; i < 0x100; ++i)
                    ((PilotGuiFields *)p)->remap[i] = PaletteFindColor(
                        &((PilotGuiFields *)p)->palette, (const PaletteColor *)(pixels + i));
            }
        }
        gDrawRemap = (char *)((PilotGuiFields *)p)->remap;
        MenuDrawImage(&g513888, &((PilotGuiFields *)p)->image, 0x18e, 0x15);
        MenuSetClip(&gMenuClip, 0x18e, 0x15, 0x70, 0x70);
        gDrawRemap = 0;
    } else { PilotGuiDrawSelectionFrame(p); }
}

void f_0041170c(void *p)
{
    const char *lastText;
    if (PilotProfileCount(&gShellGame) > 0) {
        *(void **)((char *)p + 0x705) = PilotProfileSelect(&gShellGame, -1);
        SendEvent(*(void **)((char *)p + 0x705), 0x67, gPilotGuiNameBuffer);
        if (((PilotGuiFields *)p)->gray != 0)
            PilotGuiLabelSetText(&gPilotGuiLabel04, " ");
        else {
            char *link = f_0043c73a(&gShellGame, (unsigned int)*(void **)((char *)p + 0x705));
            if (!link) PilotGuiLabelSetText(&gPilotGuiLabel04, gPilotGuiUnlinked); else PilotGuiLabelSetText(&gPilotGuiLabel04, link);
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
        sprintf(gPilotGuiFormat04, "%2d", UnitCapability(*(void **)((char *)p + 0x705), 6));
        sprintf(gPilotGuiFormat05, "%2d", UnitCapability(*(void **)((char *)p + 0x705), 0));
        sprintf(gPilotGuiFormat06, "%2d", UnitCapability(*(void **)((char *)p + 0x705), 1));
        sprintf(gPilotGuiFormat07, "%2d", UnitCapability(*(void **)((char *)p + 0x705), 2));
        sprintf(gPilotGuiFormat08, "%2d", UnitCapability(*(void **)((char *)p + 0x705), 3));
        sprintf(gPilotGuiFormat09, "%2d", UnitCapability(*(void **)((char *)p + 0x705), 4));
        sprintf(gPilotGuiFormat10, "%2d", UnitCapability(*(void **)((char *)p + 0x705), 5));
        sprintf(gPilotGuiFormat11, "%2d", UnitCapability(*(void **)((char *)p + 0x705), 7));
        sprintf(gPilotGuiFormat12, "%2d/%2d",
                PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xd3),
                PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xd4));
        int rating = PilotValue((unsigned int)*(void **)((char *)p + 0x705), 0xd6) - 1;
        sprintf(gPilotGuiFormat13, "%c%d", rating / 5 + 0x41, rating % 5 + 1);
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
        PilotGuiLabelSetText(&gPilotGuiLabel12, gPilotGuiFormat11);
    } else {
        ((PilotGuiFields *)p)->selected = 0;
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
        PilotGuiLabelSetText(&gPilotGuiLabel12, " ");
    }
    // Branches can own their final label update.
}

void PilotGuiDrawBars(void *p, int row, int value, int maximum)
{
    int bars = 0;
    if (maximum != 0) {
        bars = ((value * 120) / 4 + maximum / 2) / maximum;
    }
    bars = bars <= 1 ? 1 : bars >= 30 ? 30 : bars;
    int x = 0x1fe; int image = gPilotGuiBarImageIds[row]; int imageIndex = row * 2 + 9;
    int i;
    for (i = 0; i < bars; ++i)
        MenuDrawImage(&g513888,
            &((MenuImages *)gScreenImages)->images[imageIndex],
            i * 4 + x, image);
    for (i = bars; i < 0x1e; ++i)
        MenuDrawImage(&g513888,
            &((MenuImages *)gScreenImages)->images[imageIndex+1],
            i * 4 + x, image);
}

void PilotGuiRenderControls(void *p)
{
    if (g4affc2 != 2 && g4affc2 != 1 &&
        g4affc2 != 3 && ((PilotGuiFields *)p)->selected != 0) {
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
extern "C" void f_004120fe(void *, int);
extern char gPilotGuiPromptBuffer[], gPilotGuiSpecialKey[];
extern "C" int strcmp(const char *, const char *);
extern int LogicCallMode(int, void *);
extern long gPilotGuiSelection;
extern unsigned long g4affb8;
struct GuiPoint { long x, y; };
extern GuiPoint g4bb78c;

void PilotGuiHandleInput(void *p)
{
    if ((((PilotGuiFields *)p)->input != 0) == 1) {
        unsigned int key = BriefGetHandle(p, &g4bb78c);
        gPilotGuiSelection = key;
        if ((int)key < 0) {
            key = (unsigned int)BriefDone("bioroom");
            if ((int)key >= 0)
                gPilotGuiSelection = key + 100;
        }
        if (key == 0x20) {
            int helpPressed = ((*(unsigned int *)(((PilotGuiFields *)p)->input + 4) & 0xa0) == 0x20);
            if (helpPressed) {
                gPilotGuiSelection = 0x981;
                if (PilotProfileCount(&gShellGame) > 0) {
                    f_0043bf81(&gShellGame, (int)*(void **)((char *)p + 0x705));
                    f_00476412(((PilotGuiFields *)p)->rank + 0x20);
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
        ((PilotGuiFields *)p)->refresh = 1;
        f_0041170c(p);
    }
}

void PilotGuiPreviousProfile(void *p)
{
    if (PilotProfileCount(&gShellGame) > 1) {
        PilotProfileAdvance(&gShellGame, -1);
        ((PilotGuiFields *)p)->refresh = 1;
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
            f_004120fe(p, 1);
            f_0041170c(p);
            char *profile = (char *)PilotProfileSelect(&gShellGame, -1);
            *(char **)((char *)p + 0x705) = profile;
            if (profile != 0) {
                SendEvent(profile, 0x67, gPilotGuiPromptBuffer);
                if (strcmp(gPilotGuiPromptBuffer, gPilotGuiSpecialKey) == 0)
                    LogicCallMode(0x15d, 0);
            }
        }
        g4affb8 = 1;
    }
}

void PilotGuiEnable(void *p)
{
    ((PilotGuiFields *)p)->gray = 1;
    ((PilotGuiFields *)p)->profileFlag = 1;
}

extern unsigned char gPilotGuiFlags;
void PilotGuiDisable(void *p)
{
    ((PilotGuiFields *)p)->profileFlag = 0;
    ((PilotGuiFields *)p)->gray = 0;
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
            ((PilotGuiFields *)p)->selected = PilotProfileSelect(&gShellGame, -1);
            ((PilotGuiFields *)p)->refresh = 1;
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

extern "C" void f_004120fe(void *p, int menu)
{
    char item[128];
    char profileName[64];
    int selected;
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
                SendEvent(current, 0x67, profileName);
                if (((PilotGuiFields *)p)->profileFlag != 1) {
                    char *description = (char *)f_0043c73a(&gShellGame, (int)current);
                    if (description == 0) sprintf(item, "%s (%s)", profileName, " unlinked");
                    else sprintf(item, "%s (%s)", profileName, description);
                } else sprintf(item, "%s", profileName);
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
    ((PilotGuiFields *)p)->modified = 0;
}
void PilotGuiSetProfileModified(void *p)
{
    ((PilotGuiFields *)p)->modified = 1;
}








GuiControl *gPilotGuiResourceTemplate[40] = {
    (GuiControl *)&gPilotGuiOpenB,
    (GuiControl *)&gPilotGuiOpenA,
    (GuiControl *)&gPilotGuiLabel01,
    (GuiControl *)&gPilotGuiLabel02,
    (GuiControl *)&gPilotGuiLabel03,
    (GuiControl *)&gPilotGuiLabel04,
    (GuiControl *)&gPilotGuiLabel05,
    (GuiControl *)&gPilotGuiLabel06,
    (GuiControl *)&gPilotGuiLabel07,
    (GuiControl *)&gPilotGuiLabel08,
    (GuiControl *)&gPilotGuiLabel09,
    (GuiControl *)&gPilotGuiLabel10,
    (GuiControl *)&gPilotGuiLabel11,
    (GuiControl *)&gPilotGuiLabel12,
    (GuiControl *)&gPilotGuiLabel13,
    (GuiControl *)&gPilotGuiLabel14,
    (GuiControl *)&gPilotGuiLabel15,
    (GuiControl *)&gPilotGuiLabel16,
    (GuiControl *)&gPilotGuiLabel17,
    (GuiControl *)&gPilotGuiLabel18,
    (GuiControl *)&gPilotGuiLabel19,
    (GuiControl *)&gPilotGuiLabelArray[0],
    (GuiControl *)&gPilotGuiLabelArray[1],
    (GuiControl *)&gPilotGuiLabelArray[2],
    (GuiControl *)&gPilotGuiLabelArray[3],
    (GuiControl *)&gPilotGuiLabelArray[4],
    (GuiControl *)&gPilotGuiLabelArray[5],
    (GuiControl *)&gPilotGuiLabelArray[6],
    (GuiControl *)&gPilotGuiLabelArray[7],
    (GuiControl *)&gPilotGuiLabel22,
    (GuiControl *)&gPilotGuiLabel20,
    (GuiControl *)&gPilotGuiLabel21,
    (GuiControl *)&gPilotGuiLabel23,
    (GuiControl *)&gPilotGuiLabel24,
    (GuiControl *)&gPilotGuiLabel25,
    (GuiControl *)&gPilotGuiLabel26,
    (GuiControl *)&gPilotGuiLabel27,
    (GuiControl *)&gPilotGuiLabel28,
    (GuiControl *)&gPilotGuiLabel29,
    (GuiControl *)&gPilotGuiHelpRegion
};
