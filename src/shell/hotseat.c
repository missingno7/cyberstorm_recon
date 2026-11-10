/* shell\hotseat.c: object begins at XI target 004243e8. */















struct GuiControl;
struct GuiPoint { long x, y; }; struct MenuImage { long value[3]; };
#pragma pack(push,1)
struct HotseatDataView {char base[0x4c]; int count,unused50,images; char names[8][32],values[8][20],teams[8][4],profiles[8][512]; int loaded[8],team[8],value[8],last[8]; signed char colors[8]; int mode,funds,valueIndex,valueLimit,teamLimit,lastSetting,techIndex,techValue,map,run,option; char *ValueText(int i){return values[i];} char *TeamText(int i){return teams[i];} };
#pragma pack(pop)
typedef unsigned long U32;
struct GuiRect { long x,y,width,height; };
struct GuiControlData:GuiRect { unsigned char style[4]; U32 attributes,shortcut; };
class GuiControl:public GuiControlData { public: GuiControl(U32,U32); virtual ~GuiControl(); virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned,U32); virtual int HitTest(const GuiPoint *); void Visibility(U32 n){attributes=(attributes&0xff1fffffUL)+(n<<21);} };
#pragma pack(push,1)
class MenuLabel:public GuiControl { public: const char *text; long align; unsigned char font; int background; long color; MenuLabel(const char * =0,long=-1,unsigned char=1,long=11,int=1); virtual ~MenuLabel(); virtual void Reset(unsigned); virtual void Draw(unsigned); void SetFont(unsigned char); void SetText(const char *); void SetColor(long); void SetAlign(long); void SetBackground(int); };
class MenuPanel:public GuiControl { public: const char *text; MenuPanel(); virtual ~MenuPanel(); virtual void Draw(unsigned); };
class MenuHelpRegion:public GuiControl { public: long id; int activated; MenuHelpRegion(); virtual ~MenuHelpRegion(); virtual void Draw(unsigned); virtual int Event(unsigned,U32); };
class MenuNumber:public GuiControl { public: U32 unused; const char *text; long value; signed char font,color,flash,align; MenuNumber(signed char=-1,signed char=18); virtual ~MenuNumber(); virtual void Reset(unsigned); virtual void Draw(unsigned); void SetValue(long,signed char=0); void SetFont(signed char); };
class MenuPrefixButton:public GuiControl { public: const char *text; U32 state; signed char font,color,align,prefix; MenuPrefixButton(const char * =0,signed char=1,signed char=2,signed char=-1); virtual ~MenuPrefixButton(); virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned,U32); void SetText(const char *); void SetColor(signed char); void SetAlign(signed char); };
class MenuButton:public GuiControl { public: U32 state; const char *text; MenuButton(const char * =0,U32=0); virtual ~MenuButton(); virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned,U32); void SetText(const char *s){text=s;} };
#pragma aux range_ctor "BriefModuleInitC";
#pragma aux bar_ctor "BriefModuleInitD";
#pragma aux step_ctor "BriefModulePlace";
struct UpgradeRange { int value,maximum; __pragma("range_ctor") UpgradeRange(int,int); int GetValue(){return value;} };
struct UpgradeBar:GuiControl { UpgradeRange *range; int extra; __pragma("bar_ctor") UpgradeBar(UpgradeRange*,int); virtual ~UpgradeBar(); };
struct UpgradeArrow:GuiControl { UpgradeRange *range; int delta,key; __pragma("step_ctor") UpgradeArrow(UpgradeRange*,unsigned,int); virtual ~UpgradeArrow(); };
#pragma pack(pop)
extern GuiControl *PTR_DAT_0048f1dc[];
class GuiScreen
{
public:
    GuiScreen(GuiControl **controls = PTR_DAT_0048f1dc, unsigned count = 0x4c, unsigned long flags = 0);
    int HitTest(const GuiPoint *point);
    int Update(); void LoadLayout(long); void Open(void *,void *,long,int); int IsActive(){return *(int*)((char*)this+0x10)!=0;} void Invalidate();
};

class HotseatScreen : public GuiScreen
{
public:
    HotseatScreen();
};

class HotseatObject : public GuiScreen
{
    char state[0x12cf]; public: 
};

MenuPanel hotPanel;
MenuPrefixButton hotMode,hotFunds,hotOption;
MenuNumber hotNumber0(-1,18);
MenuNumber hotNumber1(-1,18);
MenuNumber hotNumber2(-1,18);
MenuNumber hotNumber3(-1,18);
MenuNumber hotNumber4(-1,18);
MenuNumber hotNumber5(-1,18);
MenuNumber hotNumber6(-1,18);
MenuNumber hotNumber7(-1,18);
MenuNumber hotNumber8(-1,18);
MenuNumber hotNumber9(-1,18);
MenuNumber hotNumber10(-1,18);
MenuNumber hotNumber11(-1,18);
UpgradeRange hotMapRange(23,0);
UpgradeBar hotMapBar(&hotMapRange,0);
UpgradeArrow hotMapNext(&hotMapRange,1,0x36),hotMapPrevious(&hotMapRange,-1,0x33);
MenuButton hotNames[8],hotProfiles[8];
MenuPrefixButton hotActions[8];
MenuLabel hotTeamLabels[8],hotValueLabels[8];
MenuButton hotSetting0,hotSetting1,hotSetting2;
UpgradeRange hotRange0(10,0); UpgradeBar hotBar0(&hotRange0,0); UpgradeArrow hotNext0(&hotRange0,1,0x36),hotPrevious0(&hotRange0,-1,0x33);
UpgradeRange hotRange1(13,0); UpgradeBar hotBar1(&hotRange1,0); UpgradeArrow hotNext1(&hotRange1,1,0x36),hotPrevious1(&hotRange1,-1,0x33);
UpgradeRange hotRange2(11,0); UpgradeBar hotBar2(&hotRange2,0); UpgradeArrow hotNext2(&hotRange2,1,0x36),hotPrevious2(&hotRange2,-1,0x33);
UpgradeRange hotRange3(13,0); UpgradeBar hotBar3(&hotRange3,0); UpgradeArrow hotNext3(&hotRange3,1,0x36),hotPrevious3(&hotRange3,-1,0x33);
MenuPrefixButton hotDone;
MenuHelpRegion hotHelp;
 static char hotseatSourceFile[] = __FILE__;
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
extern void MenuDrawImage(void *, const MenuImage *, long, long);
extern char DAT_00511dcc,g513888; extern char gMenuClip[];
extern const char hotseatLayoutName[],hotseatFontName[],hotseatImagesName[],hotseatPlayerFormat[];
struct Palette; extern Palette gShellBackground; extern char g511948,gMenuSavedSurface;
struct ResMgr {void *Load(long,int=1,int=0,int=0);}; extern ResMgr gResMgr;
extern "C" void f_cba0(void*,const char*); extern "C" void BriefWaitA(int); extern "C" void BriefWaitB(int);
void DecodeLaunchPalette(Palette*,const void*); void ShellApplyPalette(Palette*); void SurfaceCopyRegion(void*,void*,long,long);
void f_00424d60(int); extern "C" void BriefSetMode(void*,int);
struct StrPool { char *GetString(long id, int loadData=0); };
extern StrPool DAT_00511e7c, DAT_00511e68, gShellStrings;
extern long viewportClipLeft, DAT_00513900, DAT_00513904, DAT_00513908;
extern long DAT_0051390c, DAT_00513910, DAT_00513914, DAT_00513918;
extern int _DAT_004bb6f0, DAT_004bac94;
extern GuiPoint g4bb78c;
extern void MenuHelpOpenTopic(long);

struct ClipCopy { long value[4]; };
extern int _DAT_004bb430, _DAT_004bb458, _DAT_004bb480;
extern char DAT_004bb423, DAT_004bb44b, DAT_004bb473;
extern int f_004773cc(char *, char *, unsigned long=0x12);

extern "C" char *strcpy(char *, const char *);
extern int f_0047723b(char *);
extern void f_00477295(char *);
extern int f_0042500d(HotseatDataView*);
extern void f_004254d5(HotseatDataView*);
extern char DAT_00511dd0[];
extern void f_00427e60(int, int);
extern void f_00428f33(int, int, int);
extern void f_0042539a(int);
extern void f_00425e1a(int);
struct GuiCanvas; void SurfaceCopyAligned(GuiCanvas*,int,int,int,int,int,int,int,int);
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

inline unsigned int HotRandom(unsigned int bound){return next_link_seed((unsigned int*)MissionRandomTable)%bound;}
void f_00424cf4(int); void f_0042515a(int); void f_004255bf(int);


















inline int HotChange(int *field,int value){int old=*field; *field=value; return old!=value;}

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

void f_00424a07(int *screen,int input)
{
    if (((GuiScreen*)screen)->IsActive()) return;
    memset((char*)screen+0x50,0,((char*)screen+0x12cc)-((char*)screen+0x50));
    BriefWaitA(0);
    *(ClipCopy*)&viewportClipLeft=*(ClipCopy*)&DAT_0051390c;
    BriefCloseAudio(); _DAT_004bb6f0=-1;
    ((GuiScreen*)screen)->LoadLayout((long)hotseatLayoutName);
    ((GuiScreen*)screen)->Open((void*)input,&gShellStrings,0xfa1,1);
    f_cba0(&DAT_00511dcc,hotseatFontName);
    screen[0x15]=(int)gResMgr.Load((long)hotseatImagesName);
    DecodeLaunchPalette(&gShellBackground,(void*)(screen[0x15]+16)); ShellApplyPalette(&gShellBackground);
    MenuDrawImage(&g513888, (const MenuImage*)((screen[0x15]+4)),0,0);
    SurfaceCopyRegion(&gMenuSavedSurface,&g511948,0,0); BriefWaitB(9);
    screen[0x4b2]=1; f_00424d60((int)screen); f_00424cf4((int)screen);
    BriefSetMode(&hotMapRange,HotRandom(7)); BriefSetMode(&hotRange3,12); BriefSetMode(&hotRange2,0); BriefSetMode(&hotRange1,2); BriefSetMode(&hotRange0,2);
    for(int i=0;i<screen[0x13];i++){ screen[i+0x48e]=3; screen[i+0x49e]=2; screen[i+0x496]=DAT_0048f30c[2]; }
    f_0042515a((int)screen); f_0042539a((int)screen); f_004254d5((HotseatDataView*)screen); f_004255bf((int)screen);
}

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
        iVar2 = (int)HotRandom(8);
        if (aiStack_2c[iVar2] == 0) {
            aiStack_2c[iVar2] = 1;
            *(char *)(param_1 + 0x1298 + iVar3) = (char)iVar2;
            ++iVar3;
        }
    }
}

void f_00424d60(int screen)
{
    hotMode.SetAlign(2); hotFunds.SetAlign(2); hotOption.SetAlign(2);
    hotNumber7.SetFont(0); hotNumber0.SetFont(0); hotNumber1.SetFont(0); hotNumber2.SetFont(0); hotNumber3.SetFont(0); hotNumber4.SetFont(0); hotNumber7.SetFont(0); hotNumber10.SetFont(0); hotNumber11.SetFont(0);
    int i;
    for (i=0;i<8;i++) {
        hotNames[i].SetText((char*)(screen+0x58+i*32));
        hotProfiles[i].SetText((char*)(screen+0x218+i*512));
        hotValueLabels[i].SetText((char*)(screen+0x1f8+i*4));
        hotTeamLabels[i].SetText((char*)(screen+0x158+i*20));
        hotTeamLabels[i].SetAlign(0); hotValueLabels[i].SetAlign(0); hotTeamLabels[i].SetBackground(0); hotValueLabels[i].SetBackground(0);
    }
    for (i=0;i<8;i++) {
        sprintf((char*)(screen+0x58+i*32),hotseatPlayerFormat,i+1);
        *(char*)(screen+0x1298+i)=(char)i;
        hotNames[i].Visibility(0); hotProfiles[i].Visibility(0); hotActions[i].Visibility(0); hotTeamLabels[i].Visibility(0); hotValueLabels[i].Visibility(0);
        if (i>=*(int*)(screen+0x4c)) {
            *(char*)(screen+0x1298+i)=-1;
            hotNames[i].Visibility(5); hotProfiles[i].Visibility(5); hotActions[i].Visibility(5); hotTeamLabels[i].Visibility(5); hotValueLabels[i].Visibility(5);
        }
    }
}

int f_0042500d(HotseatDataView *screen)
{
    int result=1;
    for(int i=0;i<screen->count;++i) {
        if(screen->value[i]<=screen->valueLimit) {
            MenuDrawImage(&g513888,(const MenuImage*)(gShellImages+0x340),0x21d,i*26+47); hotTeamLabels[i].SetColor(18);
        } else {
            MenuDrawImage(&g513888,(const MenuImage*)(gShellImages+0x334),0x21d,i*26+47); hotTeamLabels[i].SetColor(14); result=0;
        }
        if(screen->team[i]<=screen->teamLimit) {
            MenuDrawImage(&g513888,(const MenuImage*)(gShellImages+0x358),0x1e0,i*26+47); hotValueLabels[i].SetColor(18);
        } else {
            MenuDrawImage(&g513888,(const MenuImage*)(gShellImages+0x34c),0x1e0,i*26+47); hotValueLabels[i].SetColor(14); result=0;
        }
    }
    ((GuiScreen*)screen)->Invalidate(); GuiResetClip(&gMenuClip); return result;
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
    f_0042500d((HotseatDataView*)param_1);
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

void f_004252a1(int param_1)
{
    if (param_1 && ((GuiScreen*)param_1)->IsActive()) {
        int old=*(int*)(param_1+0x12c0); int now=hotMapRange.GetValue(); *(int*)(param_1+0x12c0)=now; long changed=old!=now;
        old=*(int*)(param_1+0x12b0); now=hotRange1.GetValue()+1; *(int*)(param_1+0x12b0)=now; changed+=old!=now;
        old=*(int*)(param_1+0x12b8); now=hotRange3.GetValue(); *(int*)(param_1+0x12b8)=now; changed+=old!=now;
        old=*(int*)(param_1+0x12b4); now=hotRange2.GetValue(); *(int*)(param_1+0x12b4)=now; changed+=old!=now;
        old=*(int*)(param_1+0x12a8); now=hotRange0.GetValue(); *(int*)(param_1+0x12a8)=now; changed+=old!=now;
        if (changed) { f_0042539a(param_1); f_004254d5((HotseatDataView*)param_1); }
        *(ClipCopy*)&viewportClipLeft=*(ClipCopy*)&DAT_0051390c;
        f_00425e1a(param_1);
        *(ClipCopy*)&viewportClipLeft=*(ClipCopy*)&DAT_0051390c;
    }
}

void f_0042539a(int param_1)
{
    SurfaceCopyAligned((GuiCanvas*)&g513888, 1, 0, 500, 0x154, 500, 0x154, 0x8c, 0x78);
    MenuDrawImage(&g513888, (const MenuImage*)(((*(int *)(param_1 + 0x12c0) + 2) * 12 + (*(int *)(param_1 + 0x54) + 4))),
                  0x203, 0x154);
    int id=*(int *)(param_1 + 0x12c0)+0xee3;
    _DAT_004babc8 = (int)gShellStrings.GetString(id,0);
    DAT_004babbb &= 0xfe;
    hotNumber6.SetValue(*(int *)(param_1 + 0x12b0), 0);
    DAT_004bab8b &= 0xfe;
    *(int *)(param_1 + 0x12bc) = DAT_0048f334[*(int *)(param_1 + 0x12b8)];
    id=*(int *)(param_1 + 0x12b8)+0xefa;
    _DAT_004bac28 = (int)gShellStrings.GetString(id,0);
    DAT_004bac1b &= 0xfe;
    hotNumber8.SetValue(*(int *)(param_1 + 0x12b4), 0);
    DAT_004babeb &= 0xfe;
    int value = DAT_0048f30c[*(int *)(param_1 + 0x12a8)];
    *(int *)(param_1 + 0x12ac) = value;
    hotNumber5.SetValue(value, 0);
    DAT_004bab5b &= 0xfe;
    GuiResetClip(&gMenuClip);
}



void f_004254d5(HotseatDataView *screen)
{
    for(int i=0;i<screen->count;i++) {
        
        if(screen->loaded[i]) { sprintf(screen->teams[i],DAT_0048acf2,screen->team[i]); sprintf(screen->values[i],DAT_0048acf5,screen->value[i]); }
        else { sprintf(screen->profiles[i],gShellStrings.GetString(0xe78,0)); sprintf(screen->teams[i],DAT_0048acf8,screen->team[i]); sprintf(screen->values[i],DAT_0048acfb,screen->value[i]); }
    }
    ((GuiScreen*)screen)->Invalidate(); f_0042500d((HotseatDataView*)screen);
}

void f_004255bf(int param_1)
{
    int iVar1;
    GuiResetClip(&gMenuClip);
    for (iVar1 = 0; iVar1 < *(int *)(param_1 + 0x4c); iVar1 = iVar1 + 1) {
        MenuDrawImage(&g513888, (const MenuImage*)(((*(signed char *)(param_1 + 0x1298 + iVar1) + 0x19) * 0xc +
                               (*(int *)(param_1 + 0x54) + 4))),
                      0x87, iVar1 * 0x1a + 0x32);
    }
}

int f_00425616(int param_1)
{
    if (f_0042500d((HotseatDataView*)param_1) != 0) {
        char *prompt = DAT_00511e7c.GetString(0x19f, 1);
        int accepted = f_0047723b(prompt);
        *(int *)(param_1 + 0x12c4) = accepted;
        return 1;
    }
    char *prompt = DAT_00511e7c.GetString(0x1a1, 1);
    f_00477295(prompt);
    return 0;
}

void f_00425674(int param_1, int param_2)
{
    char local_28[32]; memset(local_28,0,sizeof(local_28));
    char *name = (char *)(param_1 + 0x58 + param_2 * 0x20);
    strcpy(local_28, name);
    int accepted = f_004773cc(gShellStrings.GetString(0xdd3), local_28);
    if (accepted != 0)
        strcpy(name, local_28);
    ((GuiScreen*)param_1)->Invalidate();
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
        // preserve ASSERT immediate line
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
        if (strlen(gDebugText) >= 500) AssertFail(hotseatSourceFile, __LINE__, 0);
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
    f_004254d5((HotseatDataView*)param_1);
}

void f_00425815(int param_1, int param_2)
{
    int local_10 = (int)*(signed char *)(param_1 + param_2 + 0x1298);
    int local_30[8];
    memset(local_30, 0, 0x20);
    int iVar2 = 0;
    for (; iVar2 < param_2; ++iVar2) {
        int slot = (int)*(signed char *)(param_1 + iVar2 + 0x1298);
        if (slot >= 0 && slot < 8)
            local_30[slot] = 1;
    }
    iVar2 = (int)*(signed char *)(param_1 + param_2 + 0x1298);
    do {
        iVar2 = (iVar2 + 1) % 8;
        *(char *)(param_2 + 0x1298 + param_1) = (char)iVar2;
    } while (local_30[iVar2] != 0);
    char replacement = (char)local_10;
    for (int laterIndex = param_2; ++laterIndex < *(int *)(param_1 + 0x4c);) {
        if (*(signed char *)(param_1 + laterIndex + 0x1298) == iVar2)
            *(char *)(param_1 + laterIndex + 0x1298) = replacement;
    }
    GuiResetClip(&gMenuClip);
    for (iVar2 = 0; iVar2 < *(int *)(param_1 + 0x4c); ++iVar2) {
        MenuDrawImage(&g513888, (const MenuImage*)((*(int *)(param_1 + 0x54) + 4 + (*(signed char *)(param_1 + iVar2 + 0x1298) + 0x19) * 0xc)), 0x87, iVar2 * 0x1a + 0x32);
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
        f_004254d5((HotseatDataView*)param_1);
    }
}



GuiControl *PTR_DAT_0048f1dc[]={
 (GuiControl *)&hotPanel,
 (GuiControl *)&hotMode,
 (GuiControl *)&hotFunds,
 (GuiControl *)&hotOption,
 (GuiControl *)&hotNumber0,
 (GuiControl *)&hotNumber1,
 (GuiControl *)&hotNumber2,
 (GuiControl *)&hotNumber3,
 (GuiControl *)&hotNumber4,
 (GuiControl *)&hotNumber5,
 (GuiControl *)&hotNumber6,
 (GuiControl *)&hotNumber7,
 (GuiControl *)&hotNumber8,
 (GuiControl *)&hotNumber9,
 (GuiControl *)&hotNumber10,
 (GuiControl *)&hotNumber11,
 (GuiControl *)&hotTeamLabels[0],
 (GuiControl *)&hotTeamLabels[1],
 (GuiControl *)&hotTeamLabels[2],
 (GuiControl *)&hotTeamLabels[3],
 (GuiControl *)&hotTeamLabels[4],
 (GuiControl *)&hotTeamLabels[5],
 (GuiControl *)&hotTeamLabels[6],
 (GuiControl *)&hotTeamLabels[7],
 (GuiControl *)&hotValueLabels[0],
 (GuiControl *)&hotValueLabels[1],
 (GuiControl *)&hotValueLabels[2],
 (GuiControl *)&hotValueLabels[3],
 (GuiControl *)&hotValueLabels[4],
 (GuiControl *)&hotValueLabels[5],
 (GuiControl *)&hotValueLabels[6],
 (GuiControl *)&hotValueLabels[7],
 (GuiControl *)&hotNames[0],
 (GuiControl *)&hotNames[1],
 (GuiControl *)&hotNames[2],
 (GuiControl *)&hotNames[3],
 (GuiControl *)&hotNames[4],
 (GuiControl *)&hotNames[5],
 (GuiControl *)&hotNames[6],
 (GuiControl *)&hotNames[7],
 (GuiControl *)&hotProfiles[0],
 (GuiControl *)&hotProfiles[1],
 (GuiControl *)&hotProfiles[2],
 (GuiControl *)&hotProfiles[3],
 (GuiControl *)&hotProfiles[4],
 (GuiControl *)&hotProfiles[5],
 (GuiControl *)&hotProfiles[6],
 (GuiControl *)&hotProfiles[7],
 (GuiControl *)&hotActions[0],
 (GuiControl *)&hotActions[1],
 (GuiControl *)&hotActions[2],
 (GuiControl *)&hotActions[3],
 (GuiControl *)&hotActions[4],
 (GuiControl *)&hotActions[5],
 (GuiControl *)&hotActions[6],
 (GuiControl *)&hotActions[7],
 (GuiControl *)&hotSetting0,
 (GuiControl *)&hotSetting1,
 (GuiControl *)&hotNext0,
 (GuiControl *)&hotBar0,
 (GuiControl *)&hotPrevious0,
 (GuiControl *)&hotNext1,
 (GuiControl *)&hotBar1,
 (GuiControl *)&hotPrevious1,
 (GuiControl *)&hotNext2,
 (GuiControl *)&hotBar2,
 (GuiControl *)&hotPrevious2,
 (GuiControl *)&hotNext3,
 (GuiControl *)&hotBar3,
 (GuiControl *)&hotPrevious3,
 (GuiControl *)&hotMapPrevious,
 (GuiControl *)&hotMapBar,
 (GuiControl *)&hotMapNext,
 (GuiControl *)&hotDone,
 (GuiControl *)&hotHelp,
 (GuiControl *)&hotSetting2
};
int DAT_0048f30c[]={20000,50000,100000,200000,400000,750000,1000000,3000000,7500000,15000000};
short DAT_0048f334[]={30,45,60,90,120,180,240,300,480,600,900,1200,-1};
