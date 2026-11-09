/* shell\shell.c, partial reconstruction of the single supported object cluster.
 * The census also contains switch blocks/tables: see REPORT.md before splitting.
 * Shell fields marked unknown are unobserved layout, not initialized storage. */
typedef unsigned long U32;
struct Surface {long width,height;unsigned char *pixels;U32 flags;Surface(U32);~Surface();};
struct FileRecord {U32 handle,open,flags,error;signed char direct,counted;unsigned char reserved[2];U32 position,length;
    FileRecord();~FileRecord();U32 IsOpen(){return open;}};
struct FlicSound {unsigned short id,frame;unsigned char volume,pan,pitch,flags;};
struct SoundSettings {int loop,pan,pitch,volume;};

struct FrameHeader {long size;short magic,chunks;long reserved[2];};
struct FlicPalette;
class Flic {
public:
    Surface *target;
    Surface bitmap;
    U32 unknown14,unknown18; /* unobserved; not fabricated initialized fields */
    FlicSound *sounds;
    FileRecord file;
    FlicPalette *palette;
    U32 status;
    U32 currentFrame;
    U32 deadline;
    long loops;
    int active;
    U32 memoryPosition;
    unsigned char *resource;
    FrameHeader pending;
    U32 pendingRead;
    long fileSize,magic,frames,width,height,delay;
    U32 firstFrame,secondFrame;
    Flic();~Flic();int Open(long,int=100,int=0);void Close();void ApplyPalette();U32 Update(Surface *);
};

struct GuiRect {long x,y,width,height;};
struct PaletteColor {unsigned char red,green,blue,flags;};
struct Palette {PaletteColor entries[256];unsigned char unknown[82];};
struct Timer;extern void TimerSet(Timer *,U32);struct Timer {U32 start,end,duration;Timer(U32 n){TimerSet(this,n);}};
struct InputReq {U32 mask,state;InputReq(U32 m):mask(m){} int NewPress(){return (state&0x50)==0x10;}int HelpPressed(){return (state&0xa0)==0x20;}};
extern void TimerSet(Timer *,U32);
extern int TimerExpired(Timer *);
extern long gGuiEffectPriority;
extern long gShellCreditStep;
extern Timer gShellCreditTimer;
extern char g513888,g511948,gMenuSavedSurface;
extern GuiRect gMenuClip;
extern Palette gGuiPalette;
extern unsigned char *gSurfaceColorRemap;
extern int gShellOverlayPaletteActive,gShellTintModeActive;
extern PaletteColor gShellOverlayColors[];
extern "C" void AssertFail(const char *,int,const char *);
/* A TU-local filename follows the C++ registration record in original _DATA. */
static const char ShellSourceFile[]=__FILE__;
struct ResMgr {void *Load(const char *,int=1,void * =0,U32=0);};
extern ResMgr gResMgr;
extern void SigsStart(void *,unsigned char);
extern void SurfacePresent(void *);
extern void GuiResetClip(void *);
extern void SurfaceCopyRegion(void *,void *,long,long);
extern void PaletteCopyRange(Palette *,const PaletteColor *,int,int);
extern void GuiFilterPalette(Palette *,int);
extern void ApplyPaletteData(void *,int=0,int=256);
extern unsigned char PaletteFindColor(Palette *,const PaletteColor *);
extern void SurfaceRemapRect(void *,long,long,long,long);
extern void GuiRemapClipRect(GuiRect *,long,long,long,long);

/* Opaque screen instances are passed by address only in the close-all routine. */
struct Scene;struct MissBrief {void Begin(void *,unsigned char);};extern MissBrief g4b3390;
extern Scene g4aff6c,g4b2e30,g4b3598,g4b37c4,g4b416c,g4b5b50;
extern Scene g4b700c,g4aebe0,g4afa84,g4afbfc,g4b7904,g4b82fc,g4ba2bc;
extern Scene *gShellHotseat;
extern void ShellSelectionClose(Scene *);
extern void MissBriefClose(MissBrief *);
extern void CommsScreenClose(Scene *);
extern void ShellMissionClose(Scene *);
extern void HercBayShutdown(Scene *);
extern void HercBuyClose(Scene *);
extern void ShellStorageClose(Scene *);
extern void HercUpgradeClose(Scene *);
extern void ShellMainMenuClose(Scene *);
extern void ShellIntroClose(Scene *);
extern void ShellMapClose(Scene *);
extern void ShellBriefingClose(Scene *);
extern void ShellMultiplayerClose(Scene *);
extern void MultiplayerLaunchClose(Scene *);
extern void HotseatClose(Scene *);
extern void SoundClearContext();

#pragma pack(push,1)

extern U32 gInDirty;
extern void InputPoll(InputReq *);
extern void SurfaceClear(void *);
extern void ShellPlayMovie(const char *);
extern void ShellIntroStart(Scene *,InputReq *,int);
struct ProfileManager {int Count();void *Select(int);void Advance(int);long Value(int);};
extern ProfileManager profileManager;
extern int g4d030f;
extern long SendEvent(void *,int,void * =0,int=0,int=0);
class ShellBase {
public:
    long active;long IsActive()const{return active;}
    ShellBase():active(0){}
    virtual ~ShellBase();
    virtual void Open();
    virtual void Close(int);
    virtual void Update()=0;
    virtual void Event(unsigned char)=0;
};
class Shell:public ShellBase {
public:
    Shell():ShellBase(),input(0x8004){}
    virtual ~Shell();
    virtual void Open();
    virtual void Close(int);
    virtual void Update();
    virtual void Event(unsigned char);
    unsigned char unknown08[8];
    long status;
    unsigned char difficulty;
    long displayedCredits;
    U32 unknown19,unknown1d;
    char sessionName[30];
    U32 unknown3f;
    InputReq input;
    long auxiliaryState,screen;
    unsigned char unknown53[12];
    long SumRosterResponses();
    long ProfileValueTotal();
    InputReq *Request(){return &input;}
    int PlayTakeoff(int=0);
    void PlayTravel(U32);
    void BeginBriefing(unsigned char);
    void ShowIntro();
    void ReplayIntro();
    void ShowIntroCore();
    int UpdateCredits();
    U32 NextCreditBreakpoint(U32);
};
#pragma pack(pop)


/* Class extents are independently present in the original destructor type records. */
struct Rate {U32 count,last,interval;Rate(U32);~Rate();};
#pragma pack(push,1)
struct GameSession {unsigned char unknown00[0x70c7];long credits;unsigned char unknown70cb[0x428];GameSession();~GameSession();long Credits(){return credits;}};
#pragma pack(pop)
long gShellCreditStep;
Rate gShellRate(5);
Timer gShellIdleTimer(0);
Timer gShellCreditTimer(0);
Shell uiSystem;
GameSession gShellGame;
Timer gShellAnimationTimer(0);


struct StrPool {void Unlock();};
extern StrPool gShellStrings,missionImageName;
extern void *gShellImages;
extern char gShellFont,g4b74f4,gShellResources;
extern void ShellMultiplayerShutdown(void *);
extern void ShellSoundShutdown();
extern void FlicFreeBuffers();
extern void SurfaceFree(void *);
extern "C" void f_cbf0(void *);
extern "C" void f_d2c0(void *);
extern "C" void f_cb80(void *);
extern int gShellExitCode;
inline void ShellCloseScreensCore() {

    ShellSelectionClose(&g4aff6c);
    MissBriefClose(&g4b3390);
    CommsScreenClose(&g4b2e30);
    ShellMissionClose(&g4b3598);
    HercBayShutdown(&g4b37c4);
    HercBuyClose(&g4b416c);
    ShellStorageClose(&g4b5b50);
    HercUpgradeClose(&g4b700c);
    ShellMainMenuClose(&g4aebe0);
    ShellIntroClose(&g4afa84);
    ShellMapClose(&g4afbfc);
    ShellBriefingClose(&g4b7904);
    ShellMultiplayerClose(&g4b82fc);
    MultiplayerLaunchClose(&g4ba2bc);
    if(gShellHotseat)HotseatClose(gShellHotseat);
    SoundClearContext();

}

inline void ShellClearFrameCore(){
    SigsStart(&g513888,0);
    SurfacePresent(&g513888);
    GuiResetClip(&gMenuClip);
}
void ShellClearFrame()
{
    SigsStart(&g513888,0);
    SurfacePresent(&g513888);
    GuiResetClip(&gMenuClip);
}

void ShellCloseAllScreens()
{
    ShellSelectionClose(&g4aff6c);
    MissBriefClose(&g4b3390);
    CommsScreenClose(&g4b2e30);
    ShellMissionClose(&g4b3598);
    HercBayShutdown(&g4b37c4);
    HercBuyClose(&g4b416c);
    ShellStorageClose(&g4b5b50);
    HercUpgradeClose(&g4b700c);
    ShellMainMenuClose(&g4aebe0);
    ShellIntroClose(&g4afa84);
    ShellMapClose(&g4afbfc);
    ShellBriefingClose(&g4b7904);
    ShellMultiplayerClose(&g4b82fc);
    MultiplayerLaunchClose(&g4ba2bc);
    if(gShellHotseat)HotseatClose(gShellHotseat);
    SoundClearContext();
}


void Shell::Close(int exitCode)
{
    if(IsActive()){
        gShellStrings.Unlock();
        missionImageName.Unlock();
        f_cb80(gShellImages);gShellImages=0;
        f_cbf0(&gShellFont);
        ShellCloseScreensCore();
        ShellMultiplayerShutdown(&g4b74f4);
        f_d2c0(&gShellResources);
        ShellSoundShutdown();
        FlicFreeBuffers();
        SurfaceFree(&gMenuSavedSurface);
        active=0;gShellExitCode=exitCode;
    }
}


void Shell::BeginBriefing(unsigned char number)
{
    CommsScreenClose(&g4b2e30);
    screen=28;
    g4b3390.Begin(&input,number);
}
long Shell::SumRosterResponses()
{

    long total=0;
    int count=profileManager.Count();
    if(!count)return 0;
    void *current=profileManager.Select(-1);
    if(!current)return 0;
    for(int i=0;i<count;++i){
        total+=SendEvent(current,0x79);
        profileManager.Advance(1);
        current=profileManager.Select(-1);
    }
    return total;

}
long Shell::ProfileValueTotal()
{
    long total=0;
    int count=g4d030f;
    for(int i=0;i<count;++i)total+=profileManager.Value(i);
    return total;
}

inline void Shell::ShowIntroCore()
{
    ShellClearFrameCore();
    ShellPlayMovie("csintro");
    gInDirty=1;
    InputPoll(Request());
}
void Shell::ShowIntro()
{
    ShowIntroCore();
}
void Shell::ReplayIntro()
{
    ShellIntroClose(&g4afa84);
    SurfaceClear(&g511948);
    SurfacePresent(&g513888);
    ShowIntroCore();
    SurfaceClear(&g511948);
    SurfacePresent(&g513888);
    ShellIntroStart(&g4afa84,&input,0);
}
inline void ShellLoadPaletteCore(const char *name)
{
    Palette palette;
    void *loaded=gResMgr.Load(name,1,&palette,sizeof(palette));





































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































    if(!loaded)AssertFail(ShellSourceFile,__LINE__,0);
    SurfaceCopyRegion(&gMenuSavedSurface,&g511948,0,0);
    if(gShellOverlayPaletteActive)PaletteCopyRange(&palette,gShellOverlayColors,171,37);
    GuiFilterPalette(&palette,gShellTintModeActive?3:1);
    ApplyPaletteData(&palette);
}
void ShellLoadPalette(const char *name){ShellLoadPaletteCore(name);}
extern U32 gIn80;inline U32 ShellInputKey(){return gIn80;}
extern void HelpClearTopic();
extern void ShellSetMenuState(int);
extern void MenuLoadBackground(const char *,void *);
extern char gShellBackground;
extern void SurfaceFlush(void *);
extern void ShellServiceFrame(Shell *);
extern void ShellLimitFrameRate(Shell *);
extern void GuiFadeOut();
extern void SoundStop(unsigned);
struct SoundSettings;extern int SoundPlay(unsigned,const SoundSettings * =0);


void ShellApplyPalette(Palette *palette)
{
    SurfaceCopyRegion(&gMenuSavedSurface,&g511948,0,0);
    if(gShellOverlayPaletteActive)PaletteCopyRange(palette,gShellOverlayColors,171,37);
    GuiFilterPalette(palette,gShellTintModeActive?3:1);
    ApplyPaletteData(palette);
}

void ShellShadeRect(long x,long y,long width,long height)
{
    unsigned char map[256];
    map[0]=0;
    for(int i=10;i<=246;++i){
        PaletteColor color=gGuiPalette.entries[i];
        color.red=color.red*6U/10U;
        color.green=color.green*6U/10U+24;
        color.blue=color.blue*6U/10U+24;
        map[i]=PaletteFindColor(&gGuiPalette,&color);
    }
    gSurfaceColorRemap=map;
    SurfaceRemapRect(&g513888,x,y,width,height);
    GuiRemapClipRect(&gMenuClip,x,y,width,height);
    gSurfaceColorRemap=0;
}

/* Diagnostic: byte-equal, but the inlined palette literal is still UNBOUND. */
int Shell::PlayTakeoff(int)
{
    Flic movie;
    int skipped=0;
    ShellLoadPaletteCore("mainmenu.plx");
    MenuLoadBackground("BGBASE.BMX",&gShellBackground);
    SurfaceCopyRegion(&gMenuSavedSurface,&g511948,0,0);
    movie.Open((long)"takeoff.flx");
    movie.loops=1;
    SoundPlay(13);
    while(1){
        gInDirty=1;
        InputPoll(Request());
        if(ShellInputKey()||Request()->NewPress()||Request()->HelpPressed()){
            skipped=1;
            break;
        }
        if(movie.Update((Surface *)&g511948)&4)break;
        SurfaceFlush(&g513888);
        ShellServiceFrame(this);
        ShellLimitFrameRate(this);
    }
    movie.Close();
    GuiFadeOut();
    ShellClearFrameCore();
    SoundStop(13);
    return skipped;
}
/* Diagnostic: branch placement still differs; not an accepted function. */
void Shell::PlayTravel(U32 route)
{
    Flic movie;
    ShellSetMenuState(6);
    HelpClearTopic();
    if(gGuiEffectPriority>50){
        if(!PlayTakeoff()){
            switch(route){
            case 0:movie.Open((long)"icemoon.flx");break;
            case 1:SoundPlay(38);movie.Open((long)"whoosh.flx");break;
            default:return;
            }
            movie.loops=1;
            while(1){
                gInDirty=1;
                InputPoll(Request());
                if(ShellInputKey()||Request()->NewPress()||Request()->HelpPressed())break;
                U32 status=movie.Update((Surface *)&g511948);
                if(status&0x20)movie.ApplyPalette();
                if(status&4)break;
                SurfaceFlush(&g513888);
                ShellServiceFrame(this);
                ShellLimitFrameRate(this);
            }
            SoundStop(38);
        }
    }
    movie.Close();
    GuiFadeOut();
    gInDirty=1;
    InputPoll(Request());
}
int Shell::UpdateCredits()
{
    long credits=gShellGame.Credits();
    if(credits==displayedCredits){
        gShellCreditStep=0;
        if(TimerExpired(&gShellCreditTimer)){
            TimerSet(&gShellCreditTimer,1000);
            return 1;
        }
        return 0;
    }
    if(gGuiEffectPriority<100){displayedCredits=credits;gShellCreditStep=0;return 1;}
    long decrease=displayedCredits-credits;
    long increase=credits-displayedCredits;
    if(increase>0){
        if(!gShellCreditStep){gShellCreditStep=increase/6;if(!gShellCreditStep){displayedCredits=credits;return 1;}}
        if(increase<gShellCreditStep)displayedCredits=credits;else displayedCredits+=gShellCreditStep;
        return 1;
    }
    if(!gShellCreditStep){gShellCreditStep=decrease/6;if(!gShellCreditStep){displayedCredits=credits;return 1;}}
    if(decrease<gShellCreditStep)displayedCredits=credits;else displayedCredits-=gShellCreditStep;
    return 1;
}

U32 Shell::NextCreditBreakpoint(U32 credits)
{
    if(credits<=20000)return 20000;
    if(credits<=50000)return 50000;
    if(credits<=100000)return 100000;
    if(credits<=200000)return 200000;
    if(credits<=400000)return 400000;
    if(credits<=750000)return 750000;
    if(credits<=1000000)return 1000000;
    if(credits<=3000000)return 3000000;
    if(credits<=7500000)return 7500000;
    return 15000000;
}

