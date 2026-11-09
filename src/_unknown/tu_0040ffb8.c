extern void PopObjectConstruct(void *);
extern unsigned long gBriefInitState;
extern unsigned char gBriefModuleBase;
extern unsigned char gBriefButtonA;
extern unsigned char gBriefButtonB;
extern unsigned char gBriefButtonC;
extern unsigned char gBriefButtonD;
extern unsigned char gBriefButtonE;
extern unsigned char gBriefButtonF;
extern unsigned char gBriefHelpRegion;
extern unsigned char gBriefNumber;
extern unsigned char gBriefPanel;
extern unsigned char gBriefResourceDesc;
class Scene {};
extern Scene g4b0e18;
extern void BriefMenuButtonCtor(void *, unsigned long, unsigned long);
#pragma aux BriefMenuButtonCtor "W?$ct:MenuButton$n(pnxaul)_";
extern void BriefMenuHelpCtor(void *);
#pragma aux BriefMenuHelpCtor "W?$ct:MenuHelpRegion$n()_";
extern void BriefMenuNumberCtor(void *, signed char, signed char);
#pragma aux BriefMenuNumberCtor "W?$ct:MenuNumber$n(cc)_";
extern void BriefMenuPanelCtor(void *);
#pragma aux BriefMenuPanelCtor "W?$ct:MenuPanel$n()_";
extern int BriefLoadResource(void *, void *, int, int);
#pragma aux BriefLoadResource "W?PopLoadResource$n(ipnxaii)i";

void PilotBriefLoadResources(void *);

void PilotBriefStaticInit(void)
{
    PopObjectConstruct(&gBriefModuleBase);
    BriefMenuButtonCtor(&gBriefButtonA, 0, 0);
    gBriefInitState = 1;
    BriefMenuButtonCtor(&gBriefButtonB, 0, 0);
    gBriefInitState = 2;
    BriefMenuButtonCtor(&gBriefButtonC, 0, 0);
    gBriefInitState = 3;
    BriefMenuButtonCtor(&gBriefButtonD, 0, 0);
    gBriefInitState = 4;
    BriefMenuButtonCtor(&gBriefButtonE, 0, 0);
    gBriefInitState = 5;
    BriefMenuButtonCtor(&gBriefButtonF, 0, 0);
    gBriefInitState = 6;
    BriefMenuHelpCtor(&gBriefHelpRegion);
    gBriefInitState = 7;
    BriefMenuNumberCtor(&gBriefNumber, (signed char)0xff, 0x12);
    gBriefInitState = 8;
    BriefMenuPanelCtor(&gBriefPanel);
    gBriefInitState = 9;
    PilotBriefLoadResources(&g4b0e18);
    gBriefInitState = 10;
}

void PilotBriefLoadResources(void *p)
{
    BriefLoadResource(p, &gBriefResourceDesc, 9, 0);
}

static inline int PilotBriefIsOpen(void *p)
{
    return *(long *)((char *)p + 0x10) != 0;
}

extern void PilotBriefLoadLayout(void *, unsigned long);
#pragma aux PilotBriefLoadLayout "W?LoadLayout$:GuiScreen$n(l)v";
extern void PilotBriefOpen(void *, void *, void *, unsigned long, int);
#pragma aux PilotBriefOpen "W?Open$:GuiScreen$n(pn$InputReq$$pn$StrPool$$li)v";
extern unsigned char gShellStrings;
#pragma aux gShellStrings "W?gShellStrings$n$StrPool$$";
inline const char *StandbyLayoutName() { return "Standby.box"; }
inline const char *BioroomResourceName() { return "bioroom"; }
extern unsigned long g4ae17f;
extern unsigned long g4b0dbc;
extern unsigned char g4b0df3;
struct GuiPoint { long x, y; };
extern GuiPoint g4bb78c;
extern unsigned long g4aff6c;
extern int screenChanged;
extern int PilotBriefGetHandle(void *, void *);
#pragma aux PilotBriefGetHandle "BriefGetHandle";
extern int PilotBriefFinish(void *);
#pragma aux PilotBriefFinish "W?Update$:GuiScreen$n()i";
extern int PilotBriefDone(const char *);
#pragma aux PilotBriefDone "BriefDone";
extern "C" int f_0040c63e(void *, int, int);
struct Shell
{
    int UpdateCredits();
};
extern Shell uiSystem;
extern char gShellGame;
#pragma aux gShellGame "W?gShellGame$n$GameSession$$";
extern void *PilotBriefSelectProfile(void *, int);
#pragma aux PilotBriefSelectProfile "W?Select$:ProfileManager$n(i)pnv";
extern int g4a706d;
extern "C" void HercSetDisplay(void *, int, int);

void PilotBriefOpenStandby(void *p, void *input)
{
    if (!PilotBriefIsOpen(p)) {
        g4b0dbc = 0xffffffffUL;
        PilotBriefLoadLayout(p, (unsigned long)StandbyLayoutName());
        PilotBriefOpen(p, input, &gShellStrings, 0x641, 1);
        int enabled = 1;
        g4b0df3 = enabled;
        g4a706d = g4ae17f - enabled;
        *(long *)((char *)p + 0x4e) = (long)PilotBriefSelectProfile(&gShellGame, -1);
        if (uiSystem.UpdateCredits())
            HercSetDisplay(&gBriefNumber, g4a706d, 0);
        *(short *)((char *)p + 0x52) = (short)0xff56;
        *(short *)((char *)p + 0x4c) = 3;
    }
}

void PilotBriefResetIfOpen(void *p)
{
    if (PilotBriefIsOpen(p))
        *(long *)((char *)p + 0x10) = 0;
}

int PilotBriefAcceptProfile(void *p)
{
    unsigned long mode = *(unsigned short *)((char *)p + 0x4c);
    if (mode != 2)
        return 0;
    g4b0dbc = (unsigned long)PilotBriefGetHandle(p, &g4bb78c);
    if ((long)g4b0dbc < 0) {
        int done = PilotBriefDone(BioroomResourceName());
        f_0040c63e(&g4aff6c, done, 1);
        if (done >= 0)
            g4b0dbc = (unsigned long)(done + 100);
    }
    return PilotBriefFinish(p);
}

void PilotBriefDoneRoom(void)
{
    PilotBriefDone(BioroomResourceName());
}

extern unsigned char g4affbc;
extern unsigned char screenSlideStep;
extern long gGuiEffectPriority;
#pragma aux gGuiEffectPriority "W?gGuiEffectPriority$nl";
extern int screenChanged;
extern void PilotBriefDrawImage(void *, void *, int, int);
#pragma aux PilotBriefDrawImage "W?MenuDrawImage$n(pnvpnx$MenuImage$$ll)v";
extern unsigned char g513888;
#pragma aux g513888 "W?g513888$ni";
extern unsigned char *gScreenImages;
extern void PilotBriefStatus(void *);
#pragma aux PilotBriefStatus "BriefStatus";
extern void PilotBriefRenderUpdate(void *);
#pragma aux PilotBriefRenderUpdate "BriefRenderUpdate";
extern "C" void f_004120fe(void *, int);
extern "C" void f_0041170c(void *);
extern "C" void f_0041207d(void *);
extern Scene g4b16a4;

void ShellBiodermDetailsRender(void *p)
{
    unsigned short mode = *(unsigned short *)((char *)p + 0x4c);
    short y;
    switch (mode) {
    case 2:
        {
            int pending = screenChanged;
            if (pending != 0) {
                *(long *)((char *)p + 0x4e) = (long)PilotBriefSelectProfile(&gShellGame, -1);
                if (uiSystem.UpdateCredits())
                    HercSetDisplay(&gBriefNumber, g4a706d, 0);
                screenChanged = 0;
            }
            PilotBriefDrawImage(&g513888, (void *)(gScreenImages + 0x70), 0x23, 0x38);
            PilotBriefStatus(p);
            PilotBriefRenderUpdate(p);
            return;
        }
    case 3:
        return;
    case 0:
        {
            unsigned char flags = g4affbc;
            flags &= 0xf7;
            if (flags)
                return;
        }
        {
        *(short *)((char *)p + 0x52) += screenSlideStep;
        y = *(short *)((char *)p + 0x52);
        if ((int)y > 0x24 || gGuiEffectPriority < 0x32) {
            *(short *)((char *)p + 0x52) = 0x24;
            *(short *)((char *)p + 0x4c) = 2;
            g4affbc = 8;
            *(long *)((char *)p + 0x4e) = (long)PilotBriefSelectProfile(&gShellGame, -1);
            if (uiSystem.UpdateCredits())
                HercSetDisplay(&gBriefNumber, g4a706d, 0);
            f_004120fe(&g4b16a4, 1);
            f_0041207d(&g4b16a4);
            PilotBriefDrawImage(&g513888, (void *)(gScreenImages + 0x7c), 0, 0);
            return;
        }
        PilotBriefDrawImage(&g513888, (void *)(gScreenImages + 0x7c), 0, 0);
        PilotBriefDrawImage(&g513888, (void *)(gScreenImages + 0x70), *(short *)((char *)p + 0x52), 0x38);
        return;
        }
    case 1:
        if (gGuiEffectPriority > 0x32) {
            *(short *)((char *)p + 0x52) -= screenSlideStep;
            y = *(short *)((char *)p + 0x52);
            if ((int)y < -170) {
                *(short *)((char *)p + 0x4c) = 3;
                g4affbc = 0;
                return;
            }
            PilotBriefDrawImage(&g513888, (void *)(gScreenImages + 0x7c), 0, 0);
            PilotBriefDrawImage(&g513888, (void *)(gScreenImages + 0x70), *(short *)((char *)p + 0x52), 0x38);
            return;
        } else {
            *(short *)((char *)p + 0x52) = (short)0xff56;
            *(short *)((char *)p + 0x4c) = 3;
            g4affbc = 0;
            PilotBriefDrawImage(&g513888, (void *)(gScreenImages + 0x7c), 0, 0);
            return;
        }
    default:
        return;
    }
}



void PilotBriefRefreshSelection(void *p)
{
    *(long *)((char *)p + 0x4e) = (long)PilotBriefSelectProfile(&gShellGame, -1);
    if (uiSystem.UpdateCredits())
        HercSetDisplay(&gBriefNumber, g4a706d, 0);
}

extern int PilotBriefProfileCount(void *);
#pragma aux PilotBriefProfileCount "W?Count$:ProfileManager$n()i";
extern void PilotBriefSurfacePresent(void *);
#pragma aux PilotBriefSurfacePresent "W?SurfacePresent$n(pnv)v";
struct PlyKey { unsigned long a, b; };
struct PlyKeyPair { PlyKey copied, original; };
extern void PilotBriefGetKey(PlyKey *);
#pragma aux PilotBriefGetKey "W?PlyGetKey$n(pn$PlyKey$$)v";
extern int f_0043d3cc(void *, void *);

extern int PilotBriefHasMission(void *);
#pragma aux PilotBriefHasMission "W?HasMission$n(pnv)i";
extern int PilotBriefSoundTrigger(unsigned int, unsigned int);
#pragma aux PilotBriefSoundTrigger "W?SoundTrigger$n(uii)i";
extern "C" void LogicCallMode(int, int);
extern "C" int f_0043c844(void *);
extern "C" void f_0043cd6c(void *);
extern int g4affb8;
extern int f_0043bf81(void *, int);
extern int f_0043bffc(void *, int);
extern int f_0043cd28(void *, int);

void PilotBriefReviewKey(void)
{
    if (PilotBriefProfileCount(&gShellGame) > 0) {
        PilotBriefDrawImage(&g513888, (void *)(gScreenImages + 0x7c), 0, 0);
        PilotBriefSurfacePresent(&g513888);
        PlyKeyPair key;
        PilotBriefGetKey(&key.original);
        key.copied.a = key.original.a;
        key.copied.b = key.original.b;
        int index = f_0043d3cc(&gShellGame, &key.copied);
        if (index >= 0) {
            f_004120fe(&g4b16a4, 1);
            f_0041170c(&g4b16a4);
            g4affb8 = 1;
            LogicCallMode(0xec, 0);
            PilotBriefSoundTrigger(0, 500);
            PilotBriefSoundTrigger(0x61, 0);
        }
    }
}

class PilotBriefModule
{
public:
    void InstallMissionProfile();
};

void PilotBriefModule::InstallMissionProfile()
{
    if (PilotBriefHasMission(&gShellGame) > 0) {
        int profile = *(long *)((char *)this + 0x4e) =
            (long)PilotBriefSelectProfile(&gShellGame, -1);
        int profileCheck = f_0043bf81(&gShellGame, profile);
        if (f_0043bffc(&gShellGame, profileCheck) != 0) {
            LogicCallMode(0x19e, 0);
            PilotBriefSoundTrigger(0, 500);
            PilotBriefSoundTrigger(0x62, 0);
            f_0043cd28(&gShellGame, -1);
            f_004120fe(&g4b16a4, 1);
            f_0041170c(&g4b16a4);
            g4affb8 = 1;
        }
    }
}

void PilotBriefSwitchProfileMode(void)
{
    if (PilotBriefHasMission(&gShellGame) > 0) {
        LogicCallMode(0x19e, 0);
        PilotBriefSoundTrigger(0, 500);
        PilotBriefSoundTrigger(99, 0);
            f_0043cd6c(&gShellGame);
            f_004120fe(&g4b16a4, 1);
            f_0041170c(&g4b16a4);
            g4affb8 = 1;
    }
}

void PilotBriefResetProfileMode(void)
{
    if (f_0043c844(&gShellGame) != 0) {
        LogicCallMode(399, 0);
        PilotBriefSoundTrigger(0, 500);
        f_004120fe(&g4b16a4, 1);
        f_0041170c(&g4b16a4);
        g4affb8 = 1;
        PilotBriefSoundTrigger(100, 0);
    }
}

extern char profileManager;
#pragma aux profileManager "W?profileManager$n$ProfileManager$$";
extern "C" void f_0043ba3e(void *, void *);
void PilotBriefRestoreProfile(void)
{
    if (g4affb8) {
        f_0043ba3e(&profileManager, &gShellGame);
        g4affb8 = 0;
    }
}

extern void *PilotBriefAllocateMissionImage(void *, int, int);
#pragma aux PilotBriefAllocateMissionImage "W?AllocateMissionImage$n(pnxaii)pnv";
extern int PilotBriefImageLoaded(void *);
#pragma aux PilotBriefImageLoaded "f_0047723b";

void PilotBriefSaveOrRestore(void)
{
    PilotBriefSoundTrigger(0x65, 0);
    if (g4affb8) {
        void *image = PilotBriefAllocateMissionImage(&gShellStrings, 0xf8d, 0);
        if (PilotBriefImageLoaded(image)) {
            g4affb8 = 1;
            f_0043ba3e(&profileManager, &gShellGame);
            g4affb8 = 0;
        } else {
            f_0043ba3e(&gShellGame, &profileManager);
            g4affb8 = 0;
            f_0041170c(&g4b16a4);
            g4a706d = g4ae17f - 1;
        }
    }
}






