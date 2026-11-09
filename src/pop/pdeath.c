/* pop\pdeath.c: names and partial class layouts inferred from callers. */
#include <stdio.h>
static char pdeathFile[] = __FILE__; typedef unsigned long U32; typedef unsigned char U8;
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
void DeathScreenClose(DeathScreen *screen){screen->active=0;}

void DeathMovie(Shell *shell,void *actor)
{
    if(!deathMovies) { SendEvent(actor,127,0,0,0); SoundTrigger(151,10); }
    else {
        U8 remap[256]; Flic flic; char title[64]; Surface surface; ExploFrame frame; int previous; U32 update;
        previous=BriefWaitA(0);
        f_cba0(&gShellFont,"font3.fnx");FlicInitBuffers(64000,64000);
        int ok=SurfaceCreate(&surface,112,112);
        if(!ok) AssertFail(pdeathFile,__LINE__,(const char *)ok);
        SurfaceGetFrame(&surface,&frame);
        SendEvent(actor,103,title,0,0);
        SigsStart(&g513888,0);SurfacePresent(&g513888);GuiResetClip(&gMenuClip);
        viewportClipLeft=gBriefDrawSource;gDrawRemap=0;
        int x=(g5138ac-336)/2,y=(g5138b0-336)/2;
        int mission=PilotValue(actor,202)+1;
        char *filename=BriefFormat("BGMSH%02dD.FLX",mission); flic.Open((long)filename);flic.repeatCount=1;
        SendEvent(actor,127,0,0,0);ResetFrameClock();
        if(mission==25)flic.SetSounds(deathSounds);
        for(;;) {
            gInDirty=1;InputPoll(&shell->input);
            if(InputAbort()||shell->input.LeftPressed()||shell->input.RightPressed())break;
            update=flic.Update(&surface);if(update&4)break;
            if(update&32) {
                flic.ApplyPalette();GuiFilterPalette(flic.palette,0);
                gMenuTextMap=deathTextMap;gMenuTextColor=deathTextColor;
                int textX=(ReadScreenWidth()-MenuTextWidth((Font *)&gShellFont,title))/2; MenuDrawText((Font *)&gShellFont,textX,y+356,title);
                U32 black=gMenuColors[0]; for(int i=0;i<256;i++)remap[i]=i<10?black:i;
            }
            if(update&1) {
                gDrawRemap=remap;DrawSprite(&g513888,&frame,x,y,0,336,336);
                MenuSetClip(&gMenuClip,x,y,336,336);gDrawRemap=0;
            }
            SurfaceFlush(&g513888);ShellServiceFrame(shell);ShellLimitFrameRate(shell);
        }
        f_cbf0(&gShellFont);FlicFreeBuffers();GuiFadeOut();SigsStart(&g513888,0);SurfacePresent(&g513888);
        BriefWaitA(previous);ShellRestore(shell);
    }
}



