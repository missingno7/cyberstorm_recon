/* pop\ppilot.c, plain-oe. Working names; unknown fields are provisional layout gaps. */
#pragma aux pilot_image_constructor "W?$ct:SimImageGadget$n(llul)_"; /* Reuse the bound constructor; this class has its own observed type descriptor. */
#include <string.h>
#include <stdio.h>
typedef unsigned long U32; typedef unsigned char byte;
#include "actor.h"
struct GuiPoint { long x,y; }; struct ViewRect {long left,top,right,bottom;}; struct InputReq; struct GuiControlData:GuiRect { byte style[4]; U32 attributes,shortcut; };
class GuiControl:public GuiControlData { public: GuiControl(U32,U32); virtual ~GuiControl(); virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned,U32); virtual int HitTest(const GuiPoint *); };
#pragma pack(push,1)
class MenuImageLabel:public GuiControl { public: const char *text; long image; signed char ax,ay,font,unused,color,bevel; MenuImageLabel(long=0,signed char=0,signed char=0,signed char=1,signed char=0,signed char=11,signed char=0); virtual ~MenuImageLabel(); virtual void Draw(unsigned); void SetStyle(signed char,signed char,signed char,signed char,signed char,signed char); void SetTextCore(const char *t){text=t;} };
class MenuSpacer:public GuiControl { public: MenuSpacer(); virtual ~MenuSpacer(); virtual void Draw(unsigned); };
class MenuNumber:public GuiControl { public: U32 unused; const char *text; long value; signed char font,color,flash,align; MenuNumber(signed char=-1,signed char=18); virtual ~MenuNumber(); virtual void Reset(unsigned); virtual void Draw(unsigned); void SetFont(signed char); void SetValueCore(long n){text=(const char *)n;} };
class MenuButton:public GuiControl { public: U32 state; const char *text; MenuButton(const char * = 0,U32=0); virtual ~MenuButton(); virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned,U32); };
class MenuHelpRegion:public GuiControl { public: long id; int activated; MenuHelpRegion(); virtual ~MenuHelpRegion(); virtual void Draw(unsigned); virtual int Event(unsigned,U32); void SetTopic(long n){id=n;} };
class PilotImageButton:public GuiControl { public: U32 state; long image,repeat; __pragma("pilot_image_constructor") PilotImageButton(long,long,U32); };
struct GameTimer; extern void GameTimerSet(GameTimer *,U32); extern int GameTimerExpired(GameTimer *); struct GameTimer { U32 start,end,length; GameTimer(U32 t){GameTimerSet(this,t);} };
struct FlicSound; struct Flic { char unknown[0x4c]; long loops; char rest[0x40]; void SetLoops(long n){loops=n;} void *GetPalette(){return *(void **)((char *)this+60);} void SetSounds(FlicSound *); Flic(); int Open(long,int=100,int=0); U32 Update(void *); }; struct Surface { long width,height; byte *pixels; U32 flags; Surface(U32); };
struct PilotEquipmentStats {byte header[31];int values[3];}; struct Palette { byte data[0x452]; }; struct MenuImage { U32 offset; unsigned short width,height; U32 flags; }; struct MenuImages { U32 count; MenuImage image[1]; };
struct ActorLogic {}; struct ActorPilot : ActorLogic {}; struct SimGame { byte unknown[0x1c]; int state; };
struct StrPool { char *GetString(long,int=0); }; struct ResMgr { void *Load(const char *,int=1,void * = 0,U32=0); }; extern ResMgr gResMgr;
struct GuiScreen { char unknown[0x10]; void *active; char rest[0x38]; GuiScreen(GuiControl ** = 0,unsigned=0,U32=0); GuiScreen *Reset(GuiControl **,unsigned,U32); int IsActive()const{return active!=0;} int Update(); void LoadLayout(long); void Open(InputReq *,StrPool *,long,int); };
struct PilotScreen:GuiScreen { ActorRecord *actor; ActorLogic *logic; ActorUnit *unit; MenuImages *background; Flic animation; Surface surface; MenuImage surfaceImage; Palette palette; int paletteChanged; byte remap[256]; int portrait,mood,overridePortrait,overrideMood,status,statusColor; int shown[16],target[16],maximum[16],changed[16],frames[3],effect; GameTimer timer; int tick,savedAudio,savedWait,refresh,stopped; MenuNumber numbers[2]; MenuImageLabel labels[13]; MenuSpacer spacers[8]; MenuNumber statusNumber; MenuButton button; MenuHelpRegion help; GuiControl *controls[30]; PilotScreen(); void Begin(void *); void Select(ActorRecord *); void Close(); int Event(); void Refresh(); void DrawValues(); void DrawDetails(); void Draw(); void Next(); void Previous(); void DrawBar(GuiRect *,int,int,int,int); };
#pragma pack(pop)
extern char g513888,g511948,gMenuSavedSurface,g4b74f4; extern long gShellSurfacePixels,gShellDrawPixels,g5138ac,g5138b0,savedSurfacePixels; extern int g4faf44,g4faf40,modeSwitch; extern StrPool targetBriefResource,gMenuPrefixStrings,g511ddc; extern PilotScreen *gPilotScreen;
extern ViewRect viewportClipLeft,gBriefDrawSource; extern GuiRect gMenuClip; extern char g511dc0; extern U32 gMenuColors[],gMenuColorMap[]; extern U32 gMenuTextColor; extern U32 *gMenuTextMap; extern void *gDrawRemap;
inline long SurfaceWidth(){return g5138ac;} inline long SurfaceHeight(){return g5138b0;} extern unsigned long gIn80,gIn7c; inline unsigned long PilotKey(){return gIn80;} inline long PilotSelection(){return gIn7c;} extern unsigned long gInA4; extern char modeCheckName[]; extern byte statusColorEmpty,statusColorLow; extern FlicSound deathSounds[];
inline void *SurfacePixels(){return (void *)gShellSurfacePixels;} inline int ClampPilot(int n,int lo,int hi){if(n<=lo)return lo;if(n>=hi)return hi;return n;} extern void TimeSleep(U32); extern "C" int BriefWaitA(int); extern "C" int BriefWaitB(int); extern void SetGameWindowSize(int,int); extern void SigsStart(void *,int); extern void SurfacePresent(void *); extern void SurfaceAttach(void *,void *,long,long); extern int SurfaceCreate(void *,long,long); extern "C" void SurfaceGetFrame(void *,MenuImage *);
extern void FlicInitBuffers(U32,U32); extern void FlicFreeBuffers(); extern void AssertFail(const char *,int,const char *); extern "C" void PaletteCopyRange(void *,const void *,unsigned,unsigned); extern void GuiFilterPalette(void *,int); extern void ApplyPaletteData(void *,int,int); extern "C" void HercSetMissionStart(void *,int); extern "C" void HercSetMission(void *,int); extern void ReleasePoolData(void *);
extern void MenuDrawImage(void *,const MenuImage *,long,long); extern void PopDrawSurface(void *,int,int,long,long,long,long); extern "C" void BriefBlit(void *,int,int,long,long,long,long,unsigned,unsigned); extern "C" char *BriefFormat(const char *,...); extern void MenuDrawWrapped(const char *,long,long,long,long,const GuiRect *,int=0,int=0); extern void MenuDrawTextAt(void *,long,long,int,const char *);
extern void GuiResetClip(void *); extern void MenuSetClip(GuiRect *,long,long,long,long); extern void MenuPresentRect(void *,long,long,long,long); extern void MenuSetColors(void *,U32,long); extern void MenuFill(void *,long,long,long,long); extern void MenuDrawMapped(void *,const MenuImage *,long,long,int,int,int);
extern long PilotValue(void *,int); extern int UnitCapability(void *,int); extern void SendEvent(void *,int,void *,int,int); extern void VoicePlay(int,int); 
extern void *PopFindItem(void *,int,int); struct UnitState;struct FogObject;extern FogObject *UnitFogObject(UnitState *,int,void *); extern char *PilotItemName(void *); extern ActorRecord *ActorFindRelated(ActorRecord *); extern ActorRecord *ActorFindRelatedOther(ActorRecord *); extern ActorRecord *ActorGetByIndex(int); extern void PilotManagerPoll(void *); extern int SimShortcutMode(void *); extern "C" int BriefGetHandle(void *,void *); extern GuiPoint g4bb78c; extern "C" void LogicCallMode(int,int); extern void PilotNext(void *,ActorRecord *,int=gInA4,int=0); extern void PilotPrevious(void *,ActorRecord *,int=gInA4,int=0); extern void PilotSetState(SimGame *,int); extern void SimSetFocus(SimGame *,ActorRecord *); extern void PilotManagerDraw(void *,int,int,int,int); extern "C" void BriefStatus(void *); extern "C" void BriefRenderUpdate(void *);
PilotImageButton pilotNext(0x33,1000,0x33),pilotPrevious(0x36,1000,0x34),pilotButtonA(0xa0,1000,0),pilotButtonB(0xa3,1000,0);
void PilotDrawFlashing(PilotScreen *,int,int,int,int,int); void PilotDrawFrame(PilotScreen *,GuiRect *,int,int); static signed char frameTable[5][4]={{5,5,2,2},{5,5,5,5},{4,4,4,4},{3,3,3,3},{3,3,2,2}};
PilotScreen::PilotScreen():GuiScreen(),animation(),surface(0),timer(0),statusNumber(),button(),help() {
    controls[0]=&numbers[0]; controls[1]=&numbers[1]; controls[15]=&statusNumber; controls[16]=&pilotNext; controls[17]=&pilotPrevious; controls[18]=&button; controls[19]=&pilotButtonA; controls[20]=&pilotButtonB; controls[21]=&help;
    int i; for(i=0;i<13;++i){controls[i+2]=&labels[i]; labels[i].SetStyle(-1,-1,1,0,11,0);}
    for(i=0;i<8;++i) controls[i+22]=&spacers[i];
    for(i=0;i<8;++i) labels[i+2].SetTextCore(g511ddc.GetString(i+0x7b,1));
    statusNumber.SetFont(0); help.SetTopic(-1); Reset(controls,30,0);
}
void PilotScreen::Begin(void *callback) {
    if(IsActive())return;
    g4faf44=0; if(modeSwitch&&g4faf40)TimeSleep(250);
    actor=0;logic=0;unit=0; savedWait=BriefWaitA(12);savedAudio=BriefWaitB(30);
    SetGameWindowSize(640,480);SigsStart(&g513888,0);SurfacePresent(&g513888);
    SurfaceAttach(&g511948,SurfacePixels(),SurfaceWidth(),SurfaceHeight());SurfaceCreate(&gMenuSavedSurface,SurfaceWidth(),SurfaceHeight());gShellDrawPixels=savedSurfacePixels;
    FlicInitBuffers(64000,64000); int result=SurfaceCreate(&surface,112,112);
    if(!result)












































        AssertFail(__FILE__,__LINE__,(const char *)result);
    SurfaceGetFrame(&surface,&surfaceImage); background=(MenuImages *)gResMgr.Load("BGMGUI.BMX"); viewportClipLeft=gBriefDrawSource;
    SigsStart(&g513888,0);MenuDrawImage(&g513888,&background->image[0],0,0);MenuDrawImage(&g513888,&background->image[14],350,10);
    PopDrawSurface(&g513888,0,1,0,0,SurfaceWidth(),SurfaceHeight());
    MenuImage *pal=&background->image[1];PaletteCopyRange(&palette,(char *)pal+pal->offset,pal->width,pal->height);GuiFilterPalette(&palette,0);ApplyPaletteData(&palette,0,256);
    paletteChanged=0;for(int i=0;i<256;++i)remap[i]=(i<10)?176:i;
    HercSetMissionStart(&g4b74f4,0x928); LoadLayout((long)"BGMGUI.BOX");Open((InputReq *)callback,&targetBriefResource,0x191,1);
}
void PilotScreen::Select(ActorRecord *selected) {
    actor=selected;
    if(!selected||!selected->pilot||!selected->unit)




























        AssertFail(__FILE__,__LINE__,0);
    logic=actor->pilot;
    unit=actor->unit;
    effect=0;
    tick=0;
    refresh=1;
    stopped=0;
    overrideMood=-1;
    portrait=mood=overridePortrait=overrideMood;
    Refresh();
    memcpy(shown,target,64);
    memset(changed,0,64);
    long name=(long)logic;
    statusNumber.SetValueCore(name);
    GameTimerSet(&timer,0);
}
void PilotScreen::Close() {
    if(IsActive()) {
        FlicFreeBuffers();
        SigsStart(&g513888,0);
        SurfacePresent(&g513888);
        SetGameWindowSize(-1,-1);
        SurfaceAttach(&g511948,(void *)gShellSurfacePixels,SurfaceWidth(),SurfaceHeight());
        SurfaceCreate(&gMenuSavedSurface,SurfaceWidth(),SurfaceHeight());
        gShellDrawPixels=savedSurfacePixels;
        if(background)ReleasePoolData(background);
        background=0;
        BriefWaitB(savedAudio);
        BriefWaitA(savedWait);
        HercSetMission(&g4b74f4,0x928);
        active=0;
        g4faf44=1;
    }
}
int PilotScreen::Event()  {
    if(!IsActive())return 0;
    PilotManagerPoll(&g4b74f4);
    help.SetTopic(BriefGetHandle(this,&g4bb78c));
    int action=Update()-1, result=0;
    if(action<0) {
        switch(gIn80) {
            case 26:action=19;
            break;
            case 27:action=20;
            break;
        }
    }
    int gameEvent=SimShortcutMode(active);
    if(gameEvent && gameEvent!=6)result=-gameEvent;
    switch(action) {
        case 18:LogicCallMode(6,0);
        result=1;
        break;
        case 16: {
            ActorRecord *next=ActorFindRelated(actor);
            if(next) {
                Select(next);
                LogicCallMode(6,0);
            }
            else LogicCallMode(7,0);
            break;
        }
        case 17: {
            ActorRecord *next=ActorFindRelatedOther(actor);
            if(next) {
                Select(next);
                LogicCallMode(6,0);
            }
            else LogicCallMode(7,0);
            break;
        }
        case 19:Next();
        break;
        case 20:Previous();
        break;
    }
    if(PilotSelection()>=20000 && PilotSelection()<=20199) {
        Select(ActorGetByIndex(PilotSelection()-20000));
        LogicCallMode(6,0);
    }
    if(PilotKey()==1||PilotKey()==57)result=1;
    return result;
}
void PilotScreen::Refresh()  {
    int dead;
    int newPortrait=PilotValue(logic,202),newMood=PilotValue(logic,215)-65;
    if(overridePortrait>=0)newPortrait=overridePortrait;
    if(overrideMood>=0)newMood=overrideMood;
    if(newPortrait!=portrait||newMood!=mood) {
        switch(newMood) {
            case 1:VoicePlay(156,0);
            break;
            case 2:VoicePlay(157,0);
            break;
            case 3:if(portrait!=-1)SendEvent(logic,127,0,0,0);
            VoicePlay(0,3000);
            VoicePlay(159,0);
            break;
        }
        portrait=newPortrait;
        mood=newMood;
        char *file=BriefFormat("BGMSH%02d%c.FLX",portrait+1,newMood+'A');
        animation.Open((long)file);
        animation.SetLoops((mood==3)?1:-1);
        if(portrait==24&&newMood==3)animation.SetSounds(deathSounds);
    }
    dead=newMood==3;
    target[0]=PilotValue(logic,200);
    target[1]=PilotValue(logic,204);
    target[2]=PilotValue(logic,207);
    target[14]=PilotValue(logic,214);
    target[15]=PilotValue(logic,211);
    target[3]=target[0]/4;
    target[4]=PilotValue(logic,206);
    target[5]=PilotValue(logic,209);
    target[6]=UnitCapability(logic,6);
    target[7]=UnitCapability(logic,0);
    target[8]=UnitCapability(logic,1);
    target[9]=UnitCapability(logic,2);
    target[10]=UnitCapability(logic,3);
    target[11]=UnitCapability(logic,4);
    target[12]=UnitCapability(logic,5);
    target[13]=PilotValue(logic,7);
    maximum[15]=PilotValue(logic,212);
    maximum[0]=PilotValue(logic,201);
    maximum[1]=PilotValue(logic,205);
    maximum[2]=PilotValue(logic,208);
    status=unit->slots[10].condition;
    statusColor=0;
    if(status==0)statusColor=statusColorEmpty;
    else if(status<100)statusColor=statusColorLow;
    for(int i=0;i<=2;++i) {
        int scale=(i==0)+3;
        int max=maximum[i];
        int current=target[i];
        if(i==2)current=max-current;
        int frame=current*scale/max;
        frame=ClampPilot(frame,0,scale)+(scale==3);
        if(current<=0||dead)frame=0;
        frames[i]=frame;
    }
}
void PilotScreen::DrawValues()  {
    int w,x,h;
    w=55;
    x=labels[0].x+labels[0].width-w;
    h=labels[0].height;
    for(int i=0;i<8;++i) {
        int y=labels[i+2].y;
        PopDrawSurface(&g513888,1,0,x,y,w,h);
        int color=shown[i+6]==target[i+6]?11:16;
        MenuDrawWrapped(BriefFormat("%d %%",shown[i+6]),1,-1,1,color,&labels[i+2]);
    }
    DrawBar(&labels[10],18,shown[0],target[0],maximum[0]);
    DrawBar(&labels[11],21,shown[1],target[1],maximum[1]);
    DrawBar(&labels[12],24,shown[2],target[2],maximum[2]);
}
void PilotScreen::DrawDetails()  {
    int rank=shown[14]-1;
    int letter=rank/5+65,number=rank%5+1;
    MenuDrawWrapped(BriefFormat("%c%d",letter,number),1,-1,1,11,&labels[0]);
    MenuDrawWrapped(BriefFormat("%d / %d",shown[15],maximum[15]),1,-1,1,11,&labels[1]);
    #pragma pack(push,1)
    struct SkillItem { byte header[31]; int values[3]; };
#pragma pack(pop)
    byte stats[52];
    ActorUnit *u=unit;
    SkillItem *original=(SkillItem *)PopFindItem(u,10,-1);
    SkillItem *current=(SkillItem *)UnitFogObject((UnitState *)unit,10,stats);
    char *name=PilotItemName(PopFindItem(unit,10,-1));
    int *originalValues=original->values,*currentValues=current->values;
    byte health=unit->GetStatus();
    status=health;
    MenuDrawWrapped(name,-1,-1,1,11,&spacers[0],10,10);
    MenuDrawWrapped(targetBriefResource.GetString(434),-1,-1,1,11,&spacers[0],10,30);
    MenuDrawWrapped(BriefFormat("%d %%",status),1,-1,1,11,&spacers[0],-10,30);
    for(int i=0;i<3;++i) {
        char text[32];
        sprintf(text,"+ %d",currentValues[i]);
        if(originalValues[i]!=currentValues[i])strcat(text,BriefFormat(" %s %d",g511ddc.GetString(103),originalValues[i]));
        int y=60+i*20;
        MenuDrawWrapped(targetBriefResource.GetString(431+i),-1,-1,1,11,&spacers[0],10,y);
        MenuDrawWrapped(text,1,-1,1,11,&spacers[0],-10,y);
    }
}
void PilotDrawFlashing(PilotScreen *s,int x,int y,int on,int tick,int image) {
    MenuImage *p=&s->background->image[image];
    int show=on&&(tick+image)%6<3;
    if(show)MenuDrawImage(&g513888,p,x,y);
    else BriefBlit(&g513888,1,0,x,y,x,y,p->width,p->height);
}
void PilotDrawFrame(PilotScreen *s,GuiRect *r,int frame,int tick) {
    int frameTime=(tick/2)%4;
    int image=frameTable[frame][frameTime];
    MenuDrawImage(&g513888,&s->background->image[image],r->x,r->y);
}
void PilotScreen::Draw()  {
    if(!IsActive())return;
    if(refresh) {
        Refresh();
        PopDrawSurface(&g513888,1,0,0,0,SurfaceWidth(),SurfaceHeight());
        GuiResetClip((void *)&gMenuClip);
        DrawDetails();
        BriefStatus(this);
    }
    U32 flags=0;
    if(!stopped)flags=animation.Update(&surface);
    if(flags&4)refresh=stopped=1;
    else if(flags&32) {
        MenuSetColors(&g513888,0,0);
        MenuFill(&g513888,371,31,220,230);
        GuiResetClip((void *)&gMenuClip);
        palette=*(Palette *)animation.GetPalette();
        paletteChanged=1;
    }
    else if((flags&1)||refresh||paletteChanged) {
        if(paletteChanged)ApplyPaletteData(&palette,0,176);
        paletteChanged=0;
        gDrawRemap=remap;
        MenuDrawMapped(&g513888,&surfaceImage,371,31,0,220,230);
        MenuSetClip(&gMenuClip,371,31,220,230);
        gDrawRemap=0;
    }
    int update=0;
    if(GameTimerExpired(&timer)) {
        ++tick;
        GameTimerSet(&timer,150);
        MenuSetClip(&gMenuClip,20,300,600,180);
        if(effect) {
            PopDrawSurface(&g513888,1,0,332,375,30,50);
            if(--effect)MenuDrawImage(&g513888,&background->image[15+(3-effect)],332,375);
        }
        if(statusColor) {
            int color=(tick%4!=0)?14:4;
            
            const char *text=statusColor>=255?targetBriefResource.GetString(436):BriefFormat("%d",statusColor);
            MenuDrawWrapped(targetBriefResource.GetString(435),-1,-1,1,color,&spacers[0],10,130);
            MenuDrawWrapped(text,1,-1,1,color,&spacers[0],-10,130);
        }
        PilotDrawFrame(this,&spacers[5],frames[0],tick+1);
        PilotDrawFrame(this,&spacers[6],frames[1],tick+3);
        PilotDrawFrame(this,&spacers[7],frames[2],tick+5);
        PilotDrawFlashing(this,365,390,shown[1]<0,tick,22);
        PilotDrawFlashing(this,583,440,shown[2]>maximum[2],tick,25);
        for(int i=0;i<16;++i) {
            changed[i]=0;
            if(shown[i]!=target[i]) {
                int delta=shown[i]-target[i];
                int step=delta<=8&&delta>=-8?1:4;
                if(target[i]<shown[i])step=-step;
                int *value=&shown[i];
                *value+=step;
                changed[i]=1;
                update=1;
            }
        }
    }
    if(update)LogicCallMode(6,0);
    if(refresh||update) {
        DrawValues();
        GuiResetClip((void *)&gMenuClip);
        refresh=0;
    }
    PilotManagerDraw(modeCheckName,285,65,1,-1);
    BriefRenderUpdate(this);
}
void PilotScreen::Next() {
    if(!stopped) {
        PilotNext(modeCheckName,actor);
        Refresh();
        effect=4;
    }
}
void PilotScreen::Previous() {
    if(!stopped) {
        PilotPrevious(modeCheckName,actor);
        refresh=1;
        Refresh();
        effect=4;
    }
}
void PilotScreen::DrawBar(GuiRect *r,int image,int shownValue,int currentValue,int maxValue){
    viewportClipLeft=gBriefDrawSource;gMenuTextMap=&gMenuColorMap[shownValue==currentValue?14:16];gMenuTextColor=gMenuColors[shownValue==currentValue?14:16];
    PopDrawSurface(&g513888,1,0,r->width/2+r->x,r->y,r->width/2,21);
    MenuDrawTextAt((void *)&g511dc0,r->x+r->width-1,r->y,1,BriefFormat("    %d / %d",shownValue,maxValue));
    int x=r->x,y=r->y+20; int n=(shownValue*34+maxValue/2)/maxValue;
    int minimum=shownValue>=0;n=minimum>=n?minimum:n>=34?34:n;
    int i;for(i=0;i<n;++i)MenuDrawImage(&g513888,(MenuImage *)((char *)background+image*12+4),x+4+i*6,y);
    for(;i<34;++i)MenuDrawImage(&g513888,&background->image[(image+2)],x+4+i*6,y);
}
void PilotScreenUpdate(SimGame *game) {
    int result=gPilotScreen->Event();
    if(result) {
        ActorRecord *actor=gPilotScreen->actor;
        PilotSetState(game,game->state);
        SimSetFocus(game,actor);
        if(result<0) {
            PilotSetState(game,-result);
            return;
        }
    }
    else gPilotScreen->Draw();
}
