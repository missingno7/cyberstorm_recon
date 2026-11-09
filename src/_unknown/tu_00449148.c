/* pop\pdeath.c: names and partial class layouts inferred from callers. */
#include <stdio.h>
typedef unsigned long U32; typedef unsigned char U8;
struct InputReq { U32 mask,state; int LeftPressed(){return (state&0x50)==0x10;} int RightPressed(){return (state&0xa0)==0x20;} int LeftReleased(){return (state&0x50)==0x40;} int RightReleased(){return (state&0xa0)==0x80;} };
struct Shell { char prefix[8]; InputReq input; InputReq *Input(){return &input;} };
struct Surface { long width,height;U8 *pixels;U32 flags;Surface(U32=0);~Surface(); };
struct Palette;struct FlicSound { unsigned short id,frame;U8 volume,pan,pitch,flags; };
struct Flic { char unknown00[0x3c];Palette *palette;char unknown40[12];int repeatCount;char unknown50[64];Flic();~Flic();int Open(long,int=100,int=0);U32 Update(Surface *);void ApplyPalette();void SetSounds(FlicSound *); };
struct GuiRect { long x,y,w,h; }; struct ViewRect { int x,y,w,h; };
struct Font; struct ExploFrame { U32 data[3]; };struct MenuImage;struct SimGame;
struct FastCoord {long x,y;}; struct DeathPoint { int x,y; DeathPoint(){} DeathPoint(int a,int b){x=a;y=b;} }; struct ViewState { int flags,x,y,scale; U32 data; ViewState(){} ViewState(int f,DeathPoint p,int z,U32 d){flags=f;x=p.x;y=p.y;scale=z;data=d;} void Set(int f,const DeathPoint &p,int z,U32 d){flags=f;x=p.x;y=p.y;scale=z;data=d;} };
struct ResMgr {void *Load(const char *,int=1,void * =0,U32=0);};struct StrPool {char *GetString(long,int=0);};
extern ResMgr gResMgr;extern StrPool targetBriefResource;
extern char gShellFont;extern Font gDefaultFont;
extern char g513888;extern int g5138ac,g5138b0;extern void *gDrawRemap;
extern GuiRect gMenuClip;extern ViewRect viewportClipLeft,gBriefDrawSource;
extern U32 gMenuColors[],gMenuTextColor,*gMenuTextMap;
extern U32 deathTextColor,deathTextMap[],deathMapColor;
extern U32 gIn80,gInDirty;extern int deathMovies;
extern FlicSound deathSounds[];extern FastCoord gFastTransform;extern ViewState simTransform;
extern char modeCheckName[],g4dd0c0[];
extern void InputPoll(InputReq *);extern long SendEvent(void *,int,void *,int,int);
extern int SoundTrigger(unsigned,int);extern void FlicInitBuffers(U32,U32);extern void FlicFreeBuffers();
inline U32 InputAbort(){return gIn80;} inline int ReadScreenWidth(){return g5138ac;} extern int SurfaceCreate(void *,long,long);extern "C" void SurfaceGetFrame(Surface *,ExploFrame *);
extern "C" int BriefWaitA(int);extern "C" int BriefWaitB(int);
extern "C" void f_cba0(void *,const char *);extern "C" void f_cbf0(void *);extern "C" void f_cb80(void *);
extern void SigsStart(void *,int);extern void SurfacePresent(void *);extern void SurfaceFlush(void *);
extern void GuiResetClip(void *);extern void GuiFilterPalette(Palette *,int);extern void GuiFadeOut();
extern long MenuTextWidth(Font *,const char *);extern void MenuDrawText(Font *,long,long,const char *);
extern void MenuSetClip(GuiRect *,long,long,long,long);
extern void DrawSprite(void *,ExploFrame *,int,int,int,int,int);
extern void MenuDrawImage(void *,MenuImage *,int,int);
extern "C" char *BriefFormat(const char *,...);extern "C" void ResetFrameClock();
extern "C" int PilotValue(void *,int);extern void ShellServiceFrame(Shell *);extern void ShellLimitFrameRate(Shell *);
extern "C" void ShellRestore(Shell *);extern void SimSetMenuState(SimGame *,int);
extern "C" void SetFastView(FastCoord *,const ViewState *);extern "C" void PrepareDeathMap(FastCoord *);extern "C" void DrawDeathMap(FastCoord *,U32,int);
extern void SimGameDrawActors(SimGame *,int);extern void *LogicGetScriptData(void *);
extern void DeathTimerDraw(Shell *,int,int,int,int);extern int SimHotReadTimer(char *);extern "C" void DeathInput(Shell *);
extern "C" void AssertFail(const char *,int,const char *);
struct DeathScreen {int fields[4];int active;};
void DeathSummary(Shell *shell)
{
    char text[200];ViewState settings;DeathPoint origin;int x,y;int previous;
    previous=BriefWaitB(31);BriefWaitA(0);SigsStart(&g513888,0);
    viewportClipLeft=gBriefDrawSource;
    char *image=(char *)gResMgr.Load("BGGRNHEX.BMX");
    MenuDrawImage(&g513888,(MenuImage *)(image+4),(g5138ac-640)/2,(g5138b0-480)/2);
    f_cb80(image);GuiResetClip(&gMenuClip);
    y=(g5138b0-360)/2;x=(g5138ac-252)/2;
    origin.x=x;origin.y=y;settings.Set(0,origin,4,*(U32 *)((char *)shell+0xfa5));
    SetFastView(&gFastTransform,&settings);PrepareDeathMap(&gFastTransform);DrawDeathMap(&gFastTransform,deathMapColor,1);
    SimGameDrawActors((SimGame *)modeCheckName,4);SetFastView(&gFastTransform,&simTransform);
    gMenuTextMap=deathTextMap;gMenuTextColor=deathTextColor;
    y+=270;
    if(!*(int *)((char *)shell+0x2c2)) {
        sprintf(text,targetBriefResource.GetString(1100,0),*(int *)((char *)shell+0x2be)+1);
        f_cba0(&gShellFont,"font3.fnx");
        int textX=(ReadScreenWidth()-MenuTextWidth((Font *)&gShellFont,text))/2; MenuDrawText((Font *)&gShellFont,textX,y,text);
        f_cbf0(&gShellFont);
    }
    y+=40;
    sprintf(text,targetBriefResource.GetString(1101,0));
    int textX=(ReadScreenWidth()-MenuTextWidth(&gDefaultFont,text))/2; MenuDrawText(&gDefaultFont,textX,y,text);
    y+=20;
    sprintf(text,(char *)LogicGetScriptData(g4dd0c0));
    textX=(ReadScreenWidth()-MenuTextWidth(&gDefaultFont,text))/2; MenuDrawText(&gDefaultFont,textX,y,text);
    SurfacePresent(&g513888);ResetFrameClock();gInDirty=1;InputPoll(&shell->input);
    for(;;) {
        DeathTimerDraw(shell,g5138ac-75,20,1,-1);
        if(SimHotReadTimer((char *)shell)<10000)break;
        gInDirty=1;InputPoll(&shell->input);DeathInput(shell);
        if(InputAbort()||shell->input.LeftReleased()||shell->input.RightReleased())break;
        SurfaceFlush(&g513888);ShellServiceFrame(shell);ShellLimitFrameRate(shell);
    }
    GuiFadeOut();SigsStart(&g513888,0);SurfacePresent(&g513888);BriefWaitB(previous);
    SimSetMenuState((SimGame *)shell,1);ShellRestore(shell);
}



