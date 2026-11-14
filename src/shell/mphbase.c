/* Multiplayer host base screen. Layout and names are provisional. */
typedef unsigned long U32;
struct InputReq; struct StrPool { char *GetString(long, int = 1); };
struct GuiPoint { long x,y; }; struct GuiControl; struct Timer; void TimerSet(Timer *, U32); struct Timer { U32 start,end,duration; Timer(U32 n) { TimerSet(this,n); } };
struct GuiScreen { char layout[0x10]; InputReq *input; char rest[0x38]; GuiScreen(GuiControl **,unsigned,U32); ~GuiScreen(){input=0;} void Invalidate(); int Update(); void Draw(); long HitTest(const GuiPoint *); void LoadLayout(long); void Open(InputReq *, StrPool *, long, int); int IsActive() { return input != 0; } };
struct MenuButton { char prefix[20]; U32 flags; void SetVisibility(U32 state) { flags = (flags & 0xff1fffffUL) + (state << 21); } char rest[16]; MenuButton(const char *,U32); ~MenuButton(); };
struct MenuImage { char bytes[12]; };
struct Surface; struct Flic { char bytes[0x90]; Flic(); ~Flic(); int CurrentFrame() const { return *(const int *)(bytes + 0x44); } U32 Update(Surface *); void Close(); int Open(long,int = 100,int = 0); void SetLoops(long n) { *(long *)(bytes+0x4c)=n; } }; typedef Flic BriefPage;
#pragma pack(push, 1)
struct MultiplayerBase : public GuiScreen {
    char unknown4c[4]; int audioMode;
    unsigned char mode; U32 money; unsigned char players; char unknown5a[3];
    void *pool; int changed[2]; int selection[2];
    int previous[2]; int moneyValues[10]; unsigned char errors;
    int requested; char unknowna6[4]; Timer timerA,timerB; int canStart; unsigned char unknownc6;
    BriefPage mainPage; int unknown157; char unknown15b[0x30]; int busy;
    BriefPage pages[17]; MultiplayerBase(); int GetName(); unsigned char GetMode() { return mode; }
};
#pragma pack(pop)
extern char profileManager[];
extern char gShellBackground[];
extern void *gShellImages;
extern StrPool gShellStrings;
extern int g513888, g4b74f4;
extern char multiplayerName[];
void MenuLoadBackground(const char *, void *);
void SurfacePresent(void *);
void MenuDrawImage(void *, const MenuImage *, long, long);
int f_004773cc(char *, char * = multiplayerName, unsigned long = 12);
extern "C" unsigned int strlen(const char *);
extern "C" char *strcpy(char *, const char *);
extern "C" void f_cb80(void *);
extern "C" void BriefCloseScreen(void *);
extern "C" void BriefRender(int,int,int,int,const char *,int,int,int,int *,int *,int,int,int = 1,int = 0,int = 0,int = 0x7fffffff);
extern "C" int BriefDone(const char *);
void RefreshMultiplayerControls(MultiplayerBase *);
void RepaintMultiplayer(MultiplayerBase *);
struct ProfileManager; struct ProgressInfo;
void ApplyProgress(ProfileManager *, ProgressInfo *);
extern "C" int LogicCallMode(int, void *);
extern "C" int sprintf(char *, const char *, ...);

void TimerSet(Timer *, U32); int TimerExpired(Timer *);
struct MenuLabel { char base[20]; U32 flags; void SetVisibility(U32 state) { flags = (flags & 0xff1fffffUL) + (state << 21); } char rest[28]; MenuLabel(const char *,long,unsigned char,long,int); ~MenuLabel(); void SetText(const char *); void SetColor(long); void SetBackground(int); };
struct MenuNumber { char base[20]; U32 flags; void SetVisibility(U32 state) { flags = (flags & 0xff1fffffUL) + (state << 21); } char rest[24]; MenuNumber(signed char,signed char); ~MenuNumber(); void SetFont(signed char); void SetValue(long, signed char); };
struct MenuPrefixButton { char base[20]; U32 flags; void SetVisibility(U32 state) { flags = (flags & 0xff1fffffUL) + (state << 21); } char rest[20]; MenuPrefixButton(const char * = 0,signed char = 1,signed char = 2,signed char = -1); ~MenuPrefixButton(); void SetAlign(signed char); };
extern MenuPrefixButton multiplayerButtons[11];
extern MenuLabel g4b7f9c,g4b8004,g4b8038,g4b806c,g4b80a0,g4b8108,g4b813c,g4b8274;
extern MenuNumber g4b7ee4,g4b7f14,g4b7f6c,g4b82cc;
extern int multiplayerHelp;
extern char multiplayerPlayerText[4], multiplayerMoneyText[20], multiplayerProfileText[20];
extern char multiplayerCountText[8], multiplayerRosterText[8], multiplayerMissionText[40], multiplayerPrompt[96];
extern int g4d030f;
struct ProfileManager { int Count(); };
extern "C" void BriefWaitA(int);
extern "C" void BriefWaitB(int);
void ResetMultiplayerInput();
void SoundStop(unsigned int); void GuiFadeOut(); void ShellClearFrame();
void f_0041cab0(void *);
void ValidateMultiplayer(MultiplayerBase *);
int HasMission(void *);
int SoundTrigger(unsigned int,int);
extern "C" void HercSetMissionStart(void *, int);
extern "C" int f_0043d846(void *);
struct PlayerInfo { char bytes[48]; };
void GetPlayerInfo(void *, PlayerInfo *);
extern char g511948;
extern "C" unsigned int BriefSwitch(void *, void *);
struct GuiRect { long x,y,w,h; };
struct ViewRect { long left,top,right,bottom; };
extern GuiRect gMenuClip;
extern ViewRect viewportClipLeft,gBriefDrawSource;
void GuiResetClip(void *);
extern GuiPoint g4bb78c;
int multiplayerHover=-999;
static char mphbaseSourceFile[]=__FILE__;
void MultiplayerUpdateValues(MultiplayerBase *);



extern "C" void BriefModuleInitC(void *,int,int);
extern "C" void BriefModuleInitD(void *,void *,int);
extern "C" void BriefModulePlace(void *,void *,int,int);
struct MultiplayerSelection {
    U32 value; char rest[4];
    MultiplayerSelection(int count) { BriefModuleInitC(this,count,0); }
};
struct UpgradeBar {
    char prefix[20]; U32 flags; void SetVisibility(U32 state) { flags = (flags & 0xff1fffffUL) + (state << 21); } char rest[16];
    UpgradeBar(MultiplayerSelection *s) { BriefModuleInitD(this,s,0); }
    ~UpgradeBar();
};
struct UpgradeArrow { char prefix[20]; U32 flags; void SetVisibility(U32 state) { flags = (flags & 0xff1fffffUL) + (state << 21); } char rest[20]; UpgradeArrow(MultiplayerSelection *s,int step,int key) { BriefModulePlace(this,s,step,key); } ~UpgradeArrow(); };
struct MenuHelpRegion { char prefix[20]; U32 flags; void SetVisibility(U32 state) { flags = (flags & 0xff1fffffUL) + (state << 21); } char rest[16]; MenuHelpRegion(); ~MenuHelpRegion(); };
struct MenuPanel { char prefix[20]; U32 flags; void SetVisibility(U32 state) { flags = (flags & 0xff1fffffUL) + (state << 21); } char rest[12]; MenuPanel(); ~MenuPanel(); };
extern GuiControl *multiplayerControlList[31];
MenuPrefixButton multiplayerButtons[11];
MenuButton g4b7cbc(0,0),g4b7ce4(0,0),g4b7d0c(0,0),g4b7d34(0,0),g4b7d5c(0,0),g4b7d84(0,0),g4b7dac(0,0);
MultiplayerSelection multiplayerMoneySelectorObject(10); UpgradeBar multiplayerMoneyControl(&multiplayerMoneySelectorObject);
UpgradeArrow g4b7e04(&multiplayerMoneySelectorObject,1,54),g4b7e30(&multiplayerMoneySelectorObject,-1,51);
MultiplayerSelection multiplayerPlayerSelectorObject(13); UpgradeBar multiplayerPlayerControl(&multiplayerPlayerSelectorObject);
UpgradeArrow g4b7e8c(&multiplayerPlayerSelectorObject,1,54),g4b7eb8(&multiplayerPlayerSelectorObject,-1,51);
MenuNumber g4b7ee4(-1,18),g4b7f14(-1,18);
MenuHelpRegion multiplayerHelpRegion;
MenuNumber g4b7f6c(-1,18);
MenuLabel g4b7f9c(0,-1,1,11,1),g4b7fd0(0,-1,1,11,1),g4b8004(0,-1,1,11,1),g4b8038(0,-1,1,11,1),g4b806c(0,-1,1,11,1),g4b80a0(0,-1,1,11,1),g4b80d4(0,-1,1,11,1),g4b8108(0,-1,1,11,1),g4b813c(0,-1,1,11,1),g4b8170(0,-1,1,11,1),g4b81a4(0,-1,1,11,1),g4b81d8(0,-1,1,11,1),g4b820c(0,-1,1,11,1),g4b8240(0,-1,1,11,1),g4b8274(0,-1,1,11,1);
MenuPanel multiplayerPanel;
MenuNumber g4b82cc(-1,18);
MultiplayerBase g4b82fc;
GuiControl *multiplayerControlList[31]={
 (GuiControl *)&multiplayerPanel,
 (GuiControl *)&multiplayerButtons[0],
 (GuiControl *)&multiplayerButtons[1],
 (GuiControl *)&multiplayerButtons[2],
 (GuiControl *)&multiplayerButtons[3],
 (GuiControl *)&multiplayerButtons[4],
 (GuiControl *)&g4b7d34,
 (GuiControl *)&g4b7d5c,
 (GuiControl *)&g4b7d84,
 (GuiControl *)&g4b7d0c,
 (GuiControl *)&g4b7ce4,
 (GuiControl *)&g4b7cbc,
 (GuiControl *)&g4b7dac,
 (GuiControl *)&g4b7e04,
 (GuiControl *)&multiplayerMoneyControl,
 (GuiControl *)&g4b7e30,
 (GuiControl *)&g4b7e8c,
 (GuiControl *)&multiplayerPlayerControl,
 (GuiControl *)&g4b7eb8,
 (GuiControl *)&g4b7ee4,
 (GuiControl *)&g4b7f14,
 (GuiControl *)&multiplayerHelpRegion,
 (GuiControl *)&g4b7f6c,
 (GuiControl *)&g4b7f9c,
 (GuiControl *)&g4b82cc,
 (GuiControl *)&g4b8038,
 (GuiControl *)&g4b806c,
 (GuiControl *)&g4b80a0,
 (GuiControl *)&g4b80d4,
 (GuiControl *)&g4b8108,
 (GuiControl *)&g4b813c
};
MultiplayerBase::MultiplayerBase() : GuiScreen(multiplayerControlList,31,0),timerA(0),timerB(0) {}
void MultiplayerSetMoneyValues(MultiplayerBase *);
void MultiplayerWait(MultiplayerBase *);
void MultiplayerConfigure(MultiplayerBase *);
struct ResMgr { void *Load(long, int = 1, int = 0, int = 0); };
extern ResMgr gResMgr;
extern int gGuiPaletteFaded;
void ShellLoadPalette(const char *);
void SurfaceCopyRegion(void *, void *, int, int);
extern char gMenuSavedSurface;

extern "C" char *BriefFormat(const char *, ...);
extern "C" void *memset(void *, int, unsigned int);
void ShellMultiplayerOpen(MultiplayerBase *s, InputReq *input, int host) {
    if (s->IsActive()) return;
    gGuiPaletteFaded = 0; s->busy = 0;
    memset(multiplayerPlayerText, 0, 4);
    memset(multiplayerMoneyText, 0, 20);
    memset(multiplayerProfileText, 0, 20);
    multiplayerHelp = -1;
    s->pool = gResMgr.Load((long)"BASELITE.BMX");
    BriefWaitB(8);
    s->LoadLayout((long)"mphbase.box");
    s->Open(input, &gShellStrings, 0xdad, 1);
    ResetMultiplayerInput();
    MultiplayerSetMoneyValues(s);
    PlayerInfo info;
    GetPlayerInfo(profileManager, &info);
    if (host) {
        s->mode = 0;
        MenuLoadBackground("BGBASE.BMX", gShellBackground);
        s->mainPage.Open((long)"drop1.flx",100,0); s->mainPage.SetLoops(1);
        ShellLoadPalette("mainmenu.plx");
        s->audioMode = 0; s->unknown157 = 0;
        BriefWaitA(6);
    } else {
        s->mode = 1;
        ShellLoadPalette("mainmenu.plx");
        MenuLoadBackground("BGBASE.BMX", gShellBackground);
        SurfaceCopyRegion(&gMenuSavedSurface, &g511948, 0, 0);
        s->audioMode = 1; s->unknown157 = 0;
        sprintf(multiplayerMoneyText, "%d", *(int *)(info.bytes + 0x2c));
        BriefWaitA(8);
    }
    *(int *)s->unknowna6 = 0; s->requested = 0;
    for (int i=0;i<17;++i) s->pages[i].Open((long)BriefFormat("HB_AMB%d.FLX",i+1),100,1);
    SoundTrigger(0x31, 500);
    if (s->audioMode) HercSetMissionStart(&g4b74f4, 2);
    TimerSet((Timer *)((char *)s + 0xaa), 1000);
    s->errors = 15;
    ((int *)s->previous)[1] = 99; ((int *)s->previous)[0] = 99;
    MultiplayerConfigure(s);
    RefreshMultiplayerControls(s);
    if (s->mode == 0) MultiplayerWait(s);
    sprintf(multiplayerProfileText, "%d", f_0043d846(profileManager));
    g4b813c.SetText(multiplayerProfileText);
}

extern MenuButton g4b7cbc;
extern MenuButton g4b7ce4;
extern MenuButton g4b7d0c;
extern MenuButton g4b7d34;
extern MenuButton g4b7d5c;
extern MenuButton g4b7d84;
extern MenuButton g4b7dac;








void RefreshMultiplayerControls(MultiplayerBase *s) {
 int iVar1; switch(s->mode) {
  case 0:
    g4b7cbc.SetVisibility(5);
    g4b7ce4.SetVisibility(5);
    g4b7d0c.SetVisibility(5);
    g4b7d84.SetVisibility(5);
    g4b7d34.SetVisibility(5);
    g4b7d5c.SetVisibility(5);
    g4b8108.SetVisibility(5);
    g4b813c.SetVisibility(5);
    g4b806c.SetVisibility(5);
    g4b80a0.SetVisibility(5);
    g4b7f6c.SetVisibility(5);
    g4b82cc.SetVisibility(5);
    g4b80d4.SetVisibility(5);
    g4b7f9c.SetVisibility(5);
    g4b8038.SetVisibility(5);
    for (iVar1 = 0; iVar1 < 5; iVar1 = iVar1 + 1) {
      multiplayerButtons[iVar1].SetVisibility(5);
    }
    g4b7dac.SetVisibility(0);
    g4b7e04.SetVisibility(0);
    multiplayerMoneyControl.SetVisibility(0);
    g4b7e30.SetVisibility(0);
    g4b7e8c.SetVisibility(0);
    multiplayerPlayerControl.SetVisibility(0);
    g4b7eb8.SetVisibility(0);
    g4b7ee4.SetVisibility(0);
    g4b7f14.SetVisibility(0);
    return;
  case 3:
    g4b7ce4.flags = g4b7ce4.flags & 0xfe1fffff;
    g4b7d0c.flags = g4b7d0c.flags & 0xfe1fffff;
    return;
  case 2:
    for (iVar1 = 0; iVar1 < 5; iVar1 = iVar1 + 1) {
      multiplayerButtons[iVar1].SetVisibility(5);
    }
    g4b7d34.flags = g4b7d34.flags & 0xfe1fffff;
    g4b7d5c.flags = g4b7d5c.flags & 0xfe1fffff;
    g4b7d84.flags = g4b7d84.flags & 0xfe1fffff;
    return;
  case 1:
for (iVar1 = 0; iVar1 < 5; iVar1 = iVar1 + 1) {
    multiplayerButtons[iVar1].SetVisibility(0);
  }
  multiplayerButtons[2].SetVisibility(5);
  g4b80d4.SetVisibility(5);
  g4b7f9c.SetVisibility(5);
  g4b7cbc.SetVisibility(5);
  g4b7ce4.SetVisibility(5);
  g4b7d0c.SetVisibility(5);
  g4b8038.SetVisibility(5);
  g4b7d84.SetVisibility(5);
  g4b7d34.SetVisibility(5);
  g4b7d5c.SetVisibility(5);
  g4b7dac.SetVisibility(5);
  g4b7e04.SetVisibility(5);
  multiplayerMoneyControl.SetVisibility(5);
  g4b7e30.SetVisibility(5);
  g4b7e8c.SetVisibility(5);
  multiplayerPlayerControl.SetVisibility(5);
  g4b7eb8.SetVisibility(5);
  g4b7ee4.SetVisibility(5);
  g4b7f14.SetVisibility(5);
  g4b8108.SetVisibility(0);
  g4b813c.SetVisibility(0);
  g4b806c.SetVisibility(0);
  g4b80a0.SetVisibility(0);
  g4b7f6c.SetVisibility(0);
  g4b82cc.SetVisibility(5);
  GuiResetClip(&gMenuClip);
  BriefWaitA(8);
  return;
  case 4:
    for (iVar1 = 0; iVar1 < 5; iVar1 = iVar1 + 1) {
      multiplayerButtons[iVar1].SetVisibility(5);
    }
    g4b8108.SetVisibility(5);
    g4b813c.SetVisibility(5);
    g4b806c.SetVisibility(5);
    g4b80a0.SetVisibility(5);
    g4b7f6c.SetVisibility(5);
    g4b82cc.SetVisibility(5);
    g4b80d4.SetVisibility(5);
    g4b7f9c.SetVisibility(5);
    g4b8038.SetVisibility(5);
    return;
 default: return;
 }
}

extern "C" void BriefSetMode(void *, int);
extern char multiplayerMoneySelector[], multiplayerPlayerSelector[];
void MultiplayerConfigure(MultiplayerBase *s) {
    for (int i = 0; i < 5; ++i) multiplayerButtons[i].SetAlign(2);
    g4b806c.SetText(gShellStrings.GetString(0xdc8, 1));
    g4b80a0.SetText(gShellStrings.GetString(0xdc9, 1));
    g4b7f6c.SetFont(0);
    g4b806c.SetBackground(0); g4b8004.SetBackground(0); g4b8038.SetBackground(0);
    g4b80a0.SetBackground(0); g4b8274.SetBackground(0);
    g4b82cc.SetFont(0);
    g4b806c.SetColor(11); g4b80a0.SetColor(11); g4b8004.SetColor(11); g4b7f9c.SetColor(11);
    g4b8108.SetText(multiplayerPlayerText);
    s->selection[0] = 0; s->selection[1] = 0;
    BriefSetMode(&multiplayerMoneySelectorObject, 0); BriefSetMode(&multiplayerPlayerSelectorObject, 0);
    multiplayerButtons[2].flags = (multiplayerButtons[2].flags & 0xff1fffffUL) + 0xa00000;
}

void MultiplayerRelease(MultiplayerBase *s) {
    if (s->pool) f_cb80(s->pool);
    s->pool = 0;
    for (int i = 0; i < 17; ++i) BriefCloseScreen(&s->pages[i]);
    BriefCloseScreen(&s->mainPage);
    s->input = 0;
}
void ShellMultiplayerClose(MultiplayerBase *s) {
    if (s->IsActive()) {
        MultiplayerRelease(s);
        SoundStop(12);
        GuiFadeOut(); ShellClearFrame(); ResetMultiplayerInput();
        f_0041cab0(&g4b74f4);
    }
}
void ValidateMultiplayer(MultiplayerBase *s) {
    sprintf(multiplayerPrompt, "%s", gShellStrings.GetString(0x266, 1));
    s->errors &= ~8;
    int count = g4d030f;
    sprintf(multiplayerCountText, "%3d", count);
    if (!count) s->errors |= 1; else s->errors &= ~1;
    sprintf(multiplayerRosterText, "%3d", ((ProfileManager *)profileManager)->Count());
    if (!((ProfileManager *)profileManager)->Count()) s->errors |= 2; else s->errors &= ~2;
    sprintf(multiplayerMissionText, "%3d", HasMission(profileManager));
    g4b8274.SetText(multiplayerMissionText);
    if (!HasMission(profileManager)) { g4b8274.SetColor(4); s->errors |= 4; }
    else { g4b8274.SetColor(2); s->errors &= ~4; }
    if (s->errors == 0) {
        g4b8038.SetColor(2);
        g4b8038.SetText(gShellStrings.GetString(0x279, 1));
        s->canStart = 1;
    } else {
        g4b8038.SetColor(4);
        g4b8038.SetText(gShellStrings.GetString(0x27a, 1));
        s->canStart = 0;
    }
}
void MultiplayerEnter(MultiplayerBase *s) {
    s->busy = 0;
    MenuLoadBackground("BGBASE.BMX", gShellBackground);
    SurfacePresent(&g513888);
    s->mode = 1;
    RefreshMultiplayerControls(s);
    s->Invalidate();
}
int MultiplayerBusy(MultiplayerBase *s) { return s->busy; }
int HasMission(void *);
void MultiplayerDrawText(MultiplayerBase *s, const char *text);
int MultiplayerHost(MultiplayerBase *s) { if(HasMission(profileManager)>0) {
s->busy = 1;
    MenuDrawImage(&g513888, (MenuImage *)((char *)gShellImages + 0x31c), 200, 140);
    s->mode = 2;
    RefreshMultiplayerControls(s);
    s->unknown157 = 0;
    MultiplayerDrawText(s, gShellStrings.GetString(0x272, 1));
    SurfacePresent(&g513888);
    return 1;
} return 0; }
void MultiplayerRequest(MultiplayerBase *s) { s->requested = 1; }
int MultiplayerBase::GetName() {
    if (f_004773cc(gShellStrings.GetString(0xdd3, 1))) {
        if (strlen(multiplayerName) == 0) strcpy(multiplayerName, "[anon.]");
        return 1;
    }
    return 0;
}

void MultiplayerWait(MultiplayerBase *s) {
    s->busy = 1;
    MenuDrawImage(&g513888, (MenuImage *)((char *)gShellImages + 0x328), 160, 142);
    s->unknown157 = 0;
    MultiplayerDrawText(s, gShellStrings.GetString(0xdd2, 1));
    SurfacePresent(&g513888);
}
void MultiplayerSearch(MultiplayerBase *s) {
    s->busy = 1;
    MenuDrawImage(&g513888, (MenuImage *)((char *)gShellImages + 0x31c), 200, 140);
    s->mode = 3;
    RefreshMultiplayerControls(s);
    s->unknown157 = 0;
    MultiplayerDrawText(s, gShellStrings.GetString(0x271, 1));
    SurfacePresent(&g513888);
}
void SigsJoin(char *object) {
    MultiplayerBase *s = (MultiplayerBase *)object;
    s->busy = 1;
    MenuDrawImage(&g513888, (MenuImage *)((char *)gShellImages + 0x31c), 200, 140);
    s->mode = 4;
    RefreshMultiplayerControls(s);
    s->unknown157 = 0;
    MultiplayerDrawText(s, gShellStrings.GetString(0xdd4, 1));
    SurfacePresent(&g513888);
}
int MultiplayerAttemptStart(MultiplayerBase *s) {
    ValidateMultiplayer(s);
    if (s->canStart) {
        if (!s->GetName()) return 0;
        SoundTrigger(0x1a4, 0);
        MultiplayerRelease(s);
        if (!s->audioMode) { GuiFadeOut(); ShellClearFrame(); s->audioMode = 0; }
        return 1;
    }
    s->busy = 1;
    MenuDrawImage(&g513888, (MenuImage *)((char *)gShellImages + 0x31c), 200, 140);
    { for (int i = 0; i < 5; ++i) multiplayerButtons[i].flags = (multiplayerButtons[i].flags & 0xff1fffffUL) + 0xa00000; }
    /* Clear the first ordinary button's visibility and clean bits. */
    extern U32 multiplayerFirstButtonFlags;
    g4b7cbc.flags &= 0xfe1fffffUL;
    s->unknown157 = 0;
    unsigned char a = s->errors & 1, b = s->errors & 2, c = s->errors & 4, d = s->errors & 8;
    if (a) MultiplayerDrawText(s, gShellStrings.GetString(0x274));
    else if (b) MultiplayerDrawText(s, gShellStrings.GetString(0x275));
    else if (c) MultiplayerDrawText(s, gShellStrings.GetString(0x277));
    else if (d) { MultiplayerDrawText(s, gShellStrings.GetString(0x276)); SoundTrigger(0x90, 0); }
    SurfacePresent(&g513888);
    return 0;
}
void MultiplayerDrawText(MultiplayerBase *s, const char *text) {
    int x,y,width,height;
    switch(s->GetMode()) {case 0:x=180;y=150;width=280;height=100;break;default:x=215;y=150;width=225;height=100;break;}
    int first, last;
    BriefRender(x,y,width,height,text,0,-1,0,&first,&last,0,15);
}

/* The unused third parameter is evidenced by caller pushes 1/0 and 12-byte cleanup. */
struct MenuImageSet {int count; MenuImage images[1];};
void MultiplayerDrawHover(MultiplayerBase *s, int hover, int force = 1) {
    if (!s->busy && s->pool && hover != multiplayerHover) {
        U32 mask = ~0UL;
        if (!s->audioMode) {
            if (s->mainPage.CurrentFrame() < 50) mask = ~4UL;
            mask &= ~8UL;
        }
        GuiResetClip(&gMenuClip);
        viewportClipLeft = gBriefDrawSource;
        int i=0; int selected=hover; for (;i<4;++i) {
            if (mask & (1UL << i)) {
                U32 imageIndex = (i == hover) + i * 2; MenuDrawImage(&g513888, &((MenuImageSet *)s->pool)->images[imageIndex], 0, 0);
                if (i == 3) { U32 imageIndex = (i == selected) + 8; MenuDrawImage(&g513888, &((MenuImageSet *)s->pool)->images[imageIndex], 0, 0); }
            }
        }
        multiplayerHover = hover;
    }
}

int MultiplayerDone(MultiplayerBase *);
void MultiplayerUpdate(MultiplayerBase *s) {
    if (s->IsActive()) {
        if (s->mode == 0) {
            extern unsigned char multiplayerMoneySelection, multiplayerPlayerSelection;
            s->selection[0] = multiplayerPlayerSelectorObject.value & 255;
            s->selection[1] = multiplayerMoneySelectorObject.value & 255;
            for (int i=0;i<2;++i) { if(s->selection[i] != s->previous[i]) { s->changed[i]=1; s->previous[i]=s->selection[i]; } else s->changed[i]=0; }
        }
        int hover = s->HitTest(&g4bb78c);
        multiplayerHelp = hover;
        if (!s->busy) for (int i = 0; i < 11; ++i) multiplayerButtons[i].SetVisibility(0);
        if (hover < 0) {
            hover = MultiplayerDone(s);
            MultiplayerDrawHover(s, hover);
            if (hover >= 0) {
                multiplayerHelp = hover + 100;
                switch(hover) { case 0: multiplayerButtons[0].SetVisibility(2); break; case 1: multiplayerButtons[1].SetVisibility(2); break; case 3: multiplayerButtons[3].SetVisibility(2); break; }
            }
        } else if (hover >= 1 && hover <= 4) MultiplayerDrawHover(s, hover - 1, 0);
        s->Update();
    }
}


void ShellMultiplayerRender(MultiplayerBase *s) {
    int i; if (s->IsActive()) {
        if (s->unknown157) {
            U32 result = s->mainPage.Update((Surface *)&g511948);
            if (result & 4) { s->audioMode = 1; BriefCloseScreen(&s->mainPage); HercSetMissionStart(&g4b74f4, 2); }
            g4b7f6c.flags &= ~0x1000000UL;
        }
        
if (!s->busy) for (i=0;i<17;++i) (s->pages+i)->Update((Surface *)&g511948);

        Timer *blink = &s->timerA; if (TimerExpired(blink) && s->canStart) {
            if (s->unknownc6 == 0) { s->unknownc6 = 1; g4b8038.SetColor(2); TimerSet(blink,800); }
            else { s->unknownc6 = 0; g4b8038.SetColor(10); TimerSet(blink,600); }
            g4b8038.flags &= ~0x1000000UL;
        }
        MultiplayerUpdateValues(s);
        Timer *refresh = &s->timerB; if (TimerExpired(refresh)) {
            PlayerInfo info;
            GetPlayerInfo(profileManager, &info);
            sprintf(multiplayerPlayerText, "%2d", (unsigned char)info.bytes[0]);
            sprintf(multiplayerProfileText, "%d", f_0043d846(profileManager));
            g4b813c.flags &= ~0x1000000UL; g4b8108.flags &= ~0x1000000UL;
            TimerSet(refresh, 1000);
        }
        s->Draw();
    }
}

int MultiplayerDone(MultiplayerBase *s) {
    if (s->busy) return -1;
    return BriefDone("mpbase");
}
void MultiplayerSetMoneyValues(MultiplayerBase *s) {
    s->moneyValues[0] = 20000; s->moneyValues[1] = 50000;
    s->moneyValues[2] = 100000; s->moneyValues[3] = 200000;
    s->moneyValues[4] = 400000; s->moneyValues[5] = 750000;
    s->moneyValues[6] = 1000000; s->moneyValues[7] = 3000000;
    s->moneyValues[8] = 7500000; s->moneyValues[9] = 15000000;
}
void MultiplayerUpdateValues(MultiplayerBase *s) {
    if (s->mode == 0) {
        for (unsigned char i = 0; i < 2; ++i) {
            if (s->changed[i]) {
                switch(i) {
                case 0:
                    sprintf(multiplayerPlayerText, "%2d", s->selection[0] + 1);
                    g4b7f14.SetValue(s->selection[0] + 1, 0);
                    g4b7f14.flags &= ~0x1000000UL; break;
                case 1:
                    sprintf(multiplayerMoneyText, "%d", s->moneyValues[s->selection[1]]);
                    g4b7ee4.SetValue(s->moneyValues[s->selection[1]], 0);
                    g4b7ee4.flags &= ~0x1000000UL; break;
                }
            }
        }
    }
}
void MultiplayerSaveOptions(MultiplayerBase *s) {
    s->money = s->moneyValues[s->selection[1]];
    s->players = s->selection[0] + 1;
    ApplyProgress((ProfileManager *)profileManager, (ProgressInfo *)&s->money);
    s->unknown157 = 1;
    LogicCallMode(12, 0);
}









