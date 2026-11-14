/* Shared working layouts. Gaps denote fields whose purpose remains unknown. */
#include <string.h>
typedef unsigned long U32;
typedef unsigned char byte;
#include "actor.h"


struct GuiPoint  {
    long x,y;
};
struct ViewRect  {
    long left,top,right,bottom;
};
struct InputReq;
struct StrPool  {
    char *GetString(long,int=0);
};
struct GuiControlData:GuiRect  {
    byte style[4];
    U32 attributes,shortcut;
};
class GuiControl:public GuiControlData  {
    public:
    GuiControl(U32,U32);
    virtual ~GuiControl();
    virtual void Reset(unsigned);
    virtual void Draw(unsigned);
    virtual int Event(unsigned,U32);
    virtual int HitTest(const GuiPoint *);
    void Visibility(U32 n)  {
        attributes=(attributes&~0xe00000UL)+(n<<21);
    }
    void Invalidate() {
        attributes&=~0x1000000UL;
    }
};
#pragma pack(push,1)
class MenuPrefixButton:public GuiControl  {
    public:
    const char *text;
    U32 state;
    signed char font,color,align,prefix;
    MenuPrefixButton(const char * =0,signed char=1,signed char=2,signed char=-1);
    virtual ~MenuPrefixButton();
    virtual void Reset(unsigned);
    virtual void Draw(unsigned);
    virtual int Event(unsigned,U32);
    void SetText(const char *);
    void SetColor(signed char);
    void SetAlign(signed char);
};
class MenuNumber:public GuiControl  {
    public:
    U32 unused;
    const char *text;
    long value;
    signed char font,color,flash,align;
    MenuNumber(signed char=-1,signed char=18);
    virtual ~MenuNumber();
    virtual void Reset(unsigned);
    virtual void Draw(unsigned);
    void SetValue(long,signed char=0);
    void SetFont(signed char);
    void SetAlignCore(int n) {
        align=n;
    }
};
class MenuButton:public GuiControl  {
    public: U32 state;
    const char *text;
    MenuButton(const char * =0,U32=0);
    virtual ~MenuButton();
    virtual void Reset(unsigned);
    virtual void Draw(unsigned);
    virtual int Event(unsigned,U32);
};
class MenuPanel:public GuiControl  {
    public:const char *text;
    MenuPanel();
    virtual ~MenuPanel();
    virtual void Draw(unsigned);
};
class MenuHelpRegion:public GuiControl  {
    public:long id;
    int activated;
    MenuHelpRegion();
    virtual ~MenuHelpRegion();
    virtual void Draw(unsigned);
    virtual int Event(unsigned,U32);
};
class MenuSpacer:public GuiControl  {
    public:MenuSpacer();
    virtual ~MenuSpacer();
    virtual void Draw(unsigned);
};
struct Surface;
struct Palette  {
    byte data[0x452];
};
struct FlicSound;
struct Flic  {
    byte unknown00[0x3c];
    Palette *palette;
    byte unknown40[12];
    long loops;
    byte unknown50[64];
    Flic();
    ~Flic();
    int Open(long,int=100,int=0);
    void Close();
    U32 Update(Surface *);
    void SetSounds(FlicSound *);
};
struct GuiScreen:GuiRect  {
    InputReq *input;
    byte unknown14[0x38];
    GuiScreen(GuiControl **,unsigned,U32);
    ~GuiScreen() {
        input=0;
    }
    int IsActive()const {
        return input!=0;
    }
    void LoadLayout(long);
    void Open(InputReq *,StrPool *,long,int);
    int Update();
    void Draw();
    void Invalidate();
    long HitTest(const GuiPoint *);
};
struct GameTimer;
void GameTimerSet(GameTimer *,U32);
int GameTimerExpired(GameTimer *);
struct GameTimer  {
    U32 start,end,length;
    GameTimer(U32 n) {
        GameTimerSet(this,n);
    }
};
struct MenuImage  {
    U32 offset;
    unsigned short width,height;
    U32 flags;
};
struct MenuImages  {
    U32 count;
    MenuImage image[1];
};



struct ActorLogic  {
    byte unknown00[0x12];
    int count;
    int HasPilots() {
        return count>0;
    }
}; struct ActorPilot : ActorLogic {};















struct ProfileManager  {
    ActorUnit *Mission(int);
};
#pragma pack(pop)
struct ResMgr  {
    void *Load(const char *,int=1,void * =0,U32=0);
};
extern ResMgr gResMgr;
extern ProfileManager profileManager;
extern char gShellGame[];
extern StrPool gShellStrings,targetBriefResource;
extern char g513888,g511948,gMenuSavedSurface;
extern Palette gShellBackground;
extern ViewRect viewportClipLeft,gBriefDrawSource;
extern GuiRect gMenuClip,simClip;
extern GuiPoint g4bb78c;
extern MenuImages *g511dd4;
extern void *gDrawRemap;
extern long gGuiEffectPriority,g4ae07b,g4ae17f,g4a706d;
extern int logicMode;
extern "C" void BriefWaitA(int);
extern "C" void BriefWaitB(int);
extern "C" void f_0043ba3e(void *,void *);
extern "C" const char *f_00439d2c(ActorUnit *);
extern "C" int f_00439d02(ActorUnit *);
extern "C" long f_00441d1f(ProfileManager *,int);
extern "C" long f_00441ce1(ProfileManager *,int);
extern "C" void f_cb80(void *);
void SurfaceCopyRegion(void *,void *,long,long);
void ShellApplyPalette(Palette *);
void ShellShadeRect(long,long,long,long);
void MenuDrawImage(void *,const MenuImage *,long,long);
void SurfacePresent(void *);
void MenuSetClip(GuiRect *,long,long,long,long);
extern "C" void BriefBlit(void *,int,int,long,long,long,long,unsigned,unsigned);
extern "C" void BriefDone(const char *);
/* Report popup. POPREPT.BOX is a resource name, not source-path evidence. */
struct SimAnim  {
    byte data[7];
    byte frame;
};
void SimAnimSet(SimAnim *,int,int,int);
MenuImage *SimAnimFrame(SimAnim *,int);
void ReportAnimClose(SimAnim *);
struct PlayerSlot  {
    byte unknown00[12];
    int actor;
    char name[1];
};
struct TerrainGrid;
extern TerrainGrid fallbackName;
extern PlayerSlot *GetModePlayer(void *,int);
extern ActorRecord *ActorGetByIndex(int);
extern int GridClearSelection(TerrainGrid *);
extern void ReportSelectMission(TerrainGrid *,int,int);
extern "C" void LogicCallMode(int,int);
extern int gLocalActorKind,gRedrawFlags,modeSwitch,targetLimits;
extern char modeCheckName[];
extern MenuImages *drawCursorImages;
extern void *HercConditionDiagram(ActorUnit *,ActorUnit *);
extern void *ReportMoodRemap(ActorUnit *,unsigned,ActorUnit *);
extern long PilotValue(void *,int);
extern void DrawSprite(void *,MenuImage *,int,int,int,int,int);
extern void PopDrawSurface(void *,int,int,long,long,long,long);
extern void GuiResetClip(void *);
struct Font;
extern Font reportFont,g511dc0,gDefaultFont;
extern U32 reportTextColor,reportPlayerTextColor[],reportTextMap[],deathTextColor,deathTextMap[],gMenuColorMap[],gMenuTextColor,*gMenuTextMap,gMenuTextMode;
long reportBarY[3] = {0,22,44};
long reportHeadingY=8,reportNameY=18,reportValueY1=38,reportValueY2=53,reportErrorY=44;
static char preportSourceFile[] = __FILE__;
void MenuDrawText(Font *,long,long,const char *);
void MenuDrawTextAt(Font *,long,long,int,const char *);
extern "C" char *BriefFormat(const char *,...);
#pragma pack(push,1)
struct ReportScreen:GuiScreen  {
    int initial;
    GameTimer timer;
    int dirty,tick;
    ActorRecord *selected,*other;
    ActorUnit *unit;
    ActorLogic *logic;
    int enemy,first,second;
    SimAnim portrait,unitImage;
    ActorUnit copy;
    ReportScreen();
    void SetOther(ActorRecord *,int,int);
};
#pragma pack(pop)
MenuNumber reportNumber,reportUnit,reportPilot;
MenuSpacer reportHeading,reportDiagram,reportStats;
ReportScreen simPilot;
extern GuiControl *reportControls[];
void ReportSelect(ReportScreen *,ActorRecord *);
void ReportDrawUnit(ReportScreen *);
void ReportDrawOther(ReportScreen *);
ReportScreen::ReportScreen():GuiScreen(reportControls,6,0),timer(0) {
}
void ReportOpen(ReportScreen *s,InputReq *input) {
    if(s->IsActive())return;
    memset(&s->initial,0,(int *)((char *)&s->copy+sizeof(s->copy))-&s->initial);
    s->LoadLayout((long)"POPREPT.BOX");
    s->Open(input,&targetBriefResource,901,0);
    reportNumber.SetAlignCore(2);
    ActorRecord *actor=0;
    PlayerSlot *p=GetModePlayer(modeCheckName,gLocalActorKind);
    int index=p->actor;
    if(index)actor=ActorGetByIndex(index);
    s->selected=0;
    ReportSelect(s,actor);
    s->SetOther(0,-1,-1);
}
void ReportMission(ReportScreen *s,int mission) {
    if(s->IsActive()&&s->selected) {
        ActorRecord *a=s->selected;
        if(a->owner==gLocalActorKind&&a->target!=mission) {
            a->target=mission;
            LogicCallMode(6,0);
            GridClearSelection(&fallbackName);
            ReportSelectMission(&fallbackName,mission,7);
            gRedrawFlags|=1;
        }
    }
}
void ReportClose(ReportScreen *s) {
    if(s->IsActive()) {
        ReportAnimClose(&s->portrait);
        ReportAnimClose(&s->unitImage);
        s->input=0;
    }
}
void ReportInput(ReportScreen *s) {
    if(s->IsActive()) {
        long hit=s->HitTest(&g4bb78c);
        if(hit>=0)targetLimits=hit+800;
        s->Update();
    }
}
void SimPilotDraw(ReportScreen *s) {
    if(s->IsActive()) {
        if(GameTimerExpired(&s->timer)) {
            s->tick++;
            GameTimerSet(&s->timer,400);
            s->dirty=1;
        }
        if(s->dirty) {
            s->dirty=0;
            PopDrawSurface(&g513888,1,0,s->x,s->y,s->width,s->height);
            GuiResetClip(&simClip);
            ReportDrawUnit(s);
            ReportDrawOther(s);
            s->Invalidate();
        }
        s->Draw();
    }
}
void ReportDirty(ReportScreen *s) {
    s->dirty=1;
}
void ReportSelect(ReportScreen *s,ActorRecord *a) {
    if(s->IsActive()) {
        if(a) {
            int valid=!a->hidden&&a->action.state!=17;
            if(!valid)a=0;
        }
        if(a!=s->selected) {
            if(GridClearSelection(&fallbackName))gRedrawFlags|=1;
            s->dirty=1;
            s->Invalidate();
            s->unit=0;
            s->logic=0;
            s->selected=a;
            reportUnit.Visibility(5);
            reportPilot.Visibility(5);
            if(s->selected) {
                s->unit=s->selected->unit;
                s->logic=s->selected->Pilot();
                reportUnit.Visibility(0);
                reportPilot.Visibility(0);
                reportUnit.text=(const char *)s->unit;
                reportPilot.text=(const char *)s->logic;
                reportUnit.Invalidate();
                reportPilot.Invalidate();
                SimAnimSet(&s->unitImage,2,f_00439d02(s->unit),1);
                if(s->selected->owner!=gLocalActorKind) {
                    s->enemy=1;
                    reportPilot.text=GetModePlayer(modeCheckName,s->selected->owner)->name;
                }
                else s->enemy=0;
            }
        }
        if(s->selected)s->copy=*s->unit;
    }
}
void ReportScreen::SetOther(ActorRecord *a,int n,int m) {
    if(IsActive()) {
        if(!a||a!=other)dirty=1;
        other=a;
        first=n;
        second=m;
    }
}
inline int ReportClamp(int n,int lo,int hi) {
    if(n<=lo)return lo;
    if(n>=hi)return hi;
    return n;
}
void ReportDrawUnit(ReportScreen *s) {
    if(s->selected) {
        viewportClipLeft=gBriefDrawSource;
        long ix=reportDiagram.x+35,iy=reportDiagram.y+5;
        MenuImage *frame=SimAnimFrame(&s->unitImage,-1);
        gDrawRemap=HercConditionDiagram(s->unit,&s->copy);
        DrawSprite(&g513888,frame,ix+3,iy+3,0,76,76);
        gDrawRemap=ReportMoodRemap(s->unit,s->selected->action.angle,&s->copy);
        MenuDrawImage(&g513888,&drawCursorImages->image[90],ix,iy);
        gDrawRemap=0;
        s->copy=*s->unit;
        if(!s->enemy) {
            long value[3],maximum[3];
            int portrait;
            long px=reportStats.x,py=reportStats.y;
            int row;
            int id=PilotValue(s->logic,202)+1;
            portrait=PilotValue(s->logic,215)-64;
            if(!s->logic->HasPilots())portrait=4;
            SimAnimSet(&s->portrait,1,id,5);
            s->portrait.frame=portrait-1;
            MenuImage *portraitFrame=SimAnimFrame(&s->portrait,-1);
            MenuDrawImage(&g513888,portraitFrame,px,py);
            value[0]=PilotValue(s->logic,200);
            value[1]=PilotValue(s->logic,204);
            value[2]=PilotValue(s->logic,207);
            maximum[0]=PilotValue(s->logic,201);
            maximum[1]=PilotValue(s->logic,205);
            maximum[2]=PilotValue(s->logic,208);
            if(value[0]<0)value[0]=0;
            gMenuTextMap=reportTextMap;
            gMenuTextColor=reportTextColor;
            for(row=0;row<3;row++) {
                int current=value[row], top=maximum[row]; int filled=(current*19+top/2)/top;
                long bx=px+86,by=py+reportBarY[row]+27; filled=ReportClamp(filled,current>=0,19);
                int image=row*2+117;
                int j;
                for(j=0;j<filled;j++)MenuDrawImage(&g513888,&drawCursorImages->image[image],bx+j*3,by);
                for(;j<19;j++)MenuDrawImage(&g513888,&drawCursorImages->image[image+1],bx+j*3,by);
                MenuDrawText(&reportFont,bx+21,by-11,targetBriefResource.GetString(row+1300));
                MenuDrawTextAt(&reportFont,bx+57,by-11,1,BriefFormat("%d",value[row]));
            }
            U32 flash=s->tick&1;
            int image=123;
            if(!flash||value[1]>=0)image++;
            MenuDrawImage(&g513888,&drawCursorImages->image[image],px+82,py+49);
            image=125;
            if(!flash||value[2]<=maximum[2])image++;
            MenuDrawImage(&g513888,&drawCursorImages->image[image],px+143,py+71);
        }
    }
}
void ReportDrawOther(ReportScreen *s) {
    if(s->other) {
        const char *name;
        long x;
        long y;
        long x2;
        gMenuTextMode=1;
        x=reportHeading.x+8;
        x2=reportHeading.x+140;
        y=reportHeading.y;
        name=(const char *)s->other->unit;
        gMenuTextMap=reportTextMap;
        gMenuTextColor=reportTextColor;
        if(modeSwitch) {
            gMenuTextMap=&gMenuColorMap[s->other->owner+20];
            gMenuTextColor=reportPlayerTextColor[s->other->owner];
            name=GetModePlayer(modeCheckName,s->other->owner)->name;
        }
        MenuDrawTextAt(&reportFont,x,y+reportHeadingY,-1,targetBriefResource.GetString(908));
        MenuDrawTextAt(&g511dc0,x,y+reportNameY,-1,name);
        if(s->first>=0&&s->second>=0) {
            gMenuTextMap=reportTextMap;
            gMenuTextColor=reportTextColor;
            MenuDrawTextAt(&g511dc0,x,y+reportValueY1,-1,targetBriefResource.GetString(719));
            MenuDrawTextAt(&g511dc0,x,y+reportValueY2,-1,targetBriefResource.GetString(720));
            MenuDrawTextAt(&g511dc0,x2,y+reportValueY1,1,BriefFormat("%d",s->first));
            MenuDrawTextAt(&g511dc0,x2,y+reportValueY2,1,BriefFormat("%d",s->second));
        }
        else {
            gMenuTextMap=deathTextMap;
            gMenuTextColor=deathTextColor;
            MenuDrawText(&gDefaultFont,x,y+reportErrorY,targetBriefResource.GetString(718));
        }
    }
}
void ReportGlobalInput() {
    if(simPilot.IsActive()) {
        long hit=simPilot.HitTest(&g4bb78c);
        if(hit>=0)targetLimits=hit+800;
        simPilot.Update();
    }
}

GuiControl *reportControls[] = {&reportNumber,&reportUnit,&reportPilot,&reportHeading,&reportDiagram,&reportStats};
