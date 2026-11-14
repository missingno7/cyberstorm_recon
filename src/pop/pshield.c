/* Shield popup. Working names; object starts at 0044455c, not 00444550.
 * Packed unit fields and control layouts follow observed member offsets. */
#include <stdio.h>
typedef unsigned char U8;
typedef unsigned long U32;
#include "actor.h"
struct GuiPoint { long x,y; };
struct AimPoint:GuiPoint { AimPoint(long a=0,long b=0){x=a;y=b;} };
struct ViewRect { long left,top,right,bottom; };
struct InputReq;
struct StrPool { char *GetString(long,int=0); };
class GuiControl:public GuiRect {
public: U8 style[4]; U32 attributes,shortcut;
    GuiControl(U32,U32); virtual ~GuiControl();
    virtual void Reset(unsigned); virtual void Draw(unsigned);
    virtual int Event(unsigned,U32); virtual int HitTest(const GuiPoint *);
    int HasFlags(U32 mask) { return (attributes&mask)!=0; }
};
#pragma pack(push,1)
class MenuNumber:public GuiControl {
public: U32 unused; const char *text; long value; signed char font,color,flash,align;
    MenuNumber(signed char=-1,signed char=18); virtual ~MenuNumber();
    virtual void Reset(unsigned); virtual void Draw(unsigned);
    void SetAlign(int value){align=value;}
};
class MenuButton:public GuiControl {
public: U32 state; const char *text;
    MenuButton(const char * = 0,U32=0); virtual ~MenuButton();
    virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned,U32);
};






#pragma pack(pop)
struct SimGame { U8 unknown[0x38]; ActorRecord *focus; };
struct ModeSystem;
struct MenuImage { U32 offset; unsigned short width,height; U32 flags; };
struct MenuImages { U32 count; MenuImage image[1]; };
struct Font { void *data; };
struct SoundSettings { int flags,rate,pan,volume; };
struct GameTimer;
extern void GameTimerSet(GameTimer *,U32); extern int GameTimerExpired(GameTimer *);
struct GameTimer { U32 start,end,length; GameTimer(U32 n){GameTimerSet(this,n);} };
struct GuiScreen:GuiRect {
    InputReq *input; GuiControl **controls; StrPool *strings;
    unsigned count,focus; long hovered; unsigned pressed; U32 options;
    long firstString; int navigation,delayed; unsigned delayedControl; U32 timer[3];
    GuiScreen(GuiControl **,unsigned,U32);
    int IsActive(){return input!=0;}
    void LoadLayout(long); void Open(InputReq *,StrPool *,long,int);
    void ResetControls(); int Update(); void Draw();
    long HitTest(const GuiPoint *);
};
struct ResMgr { void *Load(const char *,int=1,void * = 0,U32=0); };
struct SimAnim { U8 bytes[8]; };
extern ActorRecord *g4d2498;
extern char modeCheckName[];
extern int modeSwitch;
struct PacketList { void Shields(ActorRecord *,char,char); }; extern PacketList modeLogName;
extern int ModeCheck(const char *);

extern GuiPoint g4bb78c;
extern U32 gIn80; extern long gInA4;
inline int ShieldShift(){return gInA4;}
extern int targetLimits;
extern StrPool targetBriefResource;
extern ResMgr gResMgr;
extern char g513888;
extern ViewRect viewportClipLeft,gBriefDrawSource;
extern GuiRect gMenuClip;
extern U32 gMenuTextColor,*gMenuTextMap;
extern void *gDrawRemap;
extern Font shellFont;
extern SoundSettings shieldSoundDefaults;
extern MenuImages *shieldImages,*shieldImagesDim,*drawCursorImages;
extern int shieldFacing,shieldImageX,shieldImageY;
extern int DAT_004bc560,DAT_004bc4e8;
extern SimAnim shieldAnim;
extern ActorUnit shieldPreview;
extern GuiControl *shieldControls[];
int shieldLabelX[6] = {65,-4,-4,65,134,134};
int shieldLabelY[6] = {-13,19,84,114,84,19};
signed char shieldLabelAlign[6] = {1,0,0,1,2,2};
static char pshieldSourceFile[] = __FILE__;
extern int MenuControlChanged(void *,unsigned,U32 *);
extern int SoundPlay(unsigned,const SoundSettings *);
extern void SoundStop(unsigned); extern void SoundSetSettings(unsigned,const SoundSettings *);
extern void SoundTrigger(unsigned,int);
extern unsigned ISqrt(unsigned); extern long GridClamp(U32);
extern "C" int f_0047ef00(GuiPoint *);
extern void PointPolar(GuiPoint *,int,int);
extern "C" char *BriefFormat(const char *,...);
extern int MenuTextWidth(Font *,const char *);
extern void MenuDrawText(Font *,long,long,const char *);
extern void MenuDrawImage(void *,const MenuImage *,long,long);
extern void DrawSprite(void *,MenuImage *,int,int,int,int,int);
extern int MenuSetClip(GuiRect *,long,long,long,long);
extern void PopDrawSurface(void *,int,int,int,int,int,int);
extern void MenuDrawWrapped(const char *,long,long,long,long,const GuiRect *,int=0,int=0);
extern void ReleasePoolData(void *);
extern void *PopFindItem(ActorUnit *,int,int=-1);
extern "C" void *f_0045bb30(ActorUnit *,int,ActorUnit *);
extern MenuImage *SimAnimFrame(SimAnim *,int);
extern void UpdatePayload(ActorUnit *,int);
class ShieldControl:public GuiControl {
public: U32 state; long frame,step,changed; SoundSettings sound;
    ShieldControl(); virtual ~ShieldControl(){}
    virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned,U32);
    void Commit(); int IsPressed();
};
class ShieldPopup:public GuiScreen {
public: void *images,*dimImages; int facing,previousFacing; GuiPoint cursor;
    ShieldPopup(); ~ShieldPopup(){input=0;}
    void OpenShield(InputReq *); void Refresh(); void SetAim(int,int);
    int Process(); void Commit(); int AllowsWeapons();
};
MenuNumber shieldNumber(-1,18);
MenuButton shieldButton(0,0);
ShieldControl shieldControl;
ShieldPopup simInventory;
ShieldControl::ShieldControl():GuiControl(31,0) {
    sound=shieldSoundDefaults;
    state=0; changed=0; step=0; frame=-1;
}
void ShieldControl::Reset(unsigned) {
    attributes&=~0x1060000UL; state=0; changed=0;
}
void ShieldControl::Draw(unsigned index) {
    static GameTimer timer(0);
    if(GameTimerExpired(&timer)) {
        GameTimerSet(&timer,50); attributes&=~0x1000000UL;
        if(frame>=0)++frame;
        if((unsigned)frame>=shieldImages->count)frame=0;
        simInventory.Refresh();
    }
    if(MenuControlChanged(this,index,&state) && frame>=0) {
        MenuImage *image;
        if(HasFlags(0x80000))image=&shieldImages->image[frame];
        else image=&shieldImagesDim->image[frame];
        int left=shieldImageX+(width/2+x)-(int)image->width/2;
        int top=shieldImageY+(height/2+y)-(int)image->height/2;
        MenuDrawImage(&g513888,image,left,top);
    }
}
inline int ShieldLimit(int value,int low,int high){if(value<=low)return low;if(value>=high)return high;return value;}
int ShieldControl::Event(unsigned index,U32 event) {
    int result=0,updateSound=0;
    if(event&4) {
        frame=0; sound=shieldSoundDefaults; sound.volume=20; step=10;
        SoundPlay(239,&sound);
    }
    if(event&12) {
        int dx=g4bb78c.x,dy=g4bb78c.y; dx-=width/2+x;dy-=height/2+y;
        sound.pan=dx*30/width;
        sound.rate=(int)g4d2498->unit->shieldRange/5+100;
        updateSound=1; changed=1;
        AimPoint point(dx,dy);
        int angle=f_0047ef00(&point)+shieldFacing*0x2aaaaabc;
        int distance=ISqrt(point.x*point.x+point.y*point.y);
        if(distance>48)distance=48;
        if(ShieldShift())angle=GridClamp(angle)*0x2aaaaabc;
        g4d2498->unit->SetDirection(angle>>24);
        g4d2498->unit->shieldRange=distance;
        shieldControl.attributes&=~0x1000000UL;
        result=index+1;
    }
    if(!HasFlags(0x20000))step=-5;
    if((event&1)&&frame<0){frame=0;attributes&=~0x1000000UL;}
    if(step) {
        updateSound=1;
        sound.volume=ShieldLimit(sound.volume+step,0,100);
        if(!sound.volume){step=0;SoundStop(239);}
        if(sound.volume==100)step=0;
    }
    if(updateSound)SoundSetSettings(239,&sound);
    state=event;return result;
}
void ShieldControl::Commit() {
    if(changed&&modeSwitch&&ModeCheck(modeCheckName))
        modeLogName.Shields(g4d2498,g4d2498->unit->shieldDirection,g4d2498->unit->shieldRange);
    changed=0;
}
int ShieldControl::IsPressed(){return HasFlags(0x20000);}
ShieldPopup::ShieldPopup():GuiScreen(shieldControls,3,0){}
void ShieldPopup::OpenShield(InputReq *request) {
    if(!IsActive()) {
        LoadLayout((long)"POPSHLD.BOX");Open(request,&targetBriefResource,801,0);ResetControls();
        images=gResMgr.Load("popShld1.bmx");dimImages=gResMgr.Load("popShld1.bmx");
        shieldNumber.SetAlign(1); shieldControl.frame=0;
        previousFacing=999;Refresh();
    }
}
void SimInventoryClose(ShieldPopup *p) {
    if(p->IsActive()) {
        if(shieldControl.changed&&modeSwitch&&ModeCheck(modeCheckName))
            modeLogName.Shields(g4d2498,g4d2498->unit->shieldDirection,g4d2498->unit->shieldRange);
        shieldControl.changed=0;
        ReleasePoolData(p->images);ReleasePoolData(p->dimImages);SoundStop(239);p->input=0;
    }
}
void ShieldPopup::Refresh() {
    if(g4d2498) {
        ActorUnit *unit=g4d2498->unit; shieldPreview=*unit;
        if(IsActive()) {
            viewportClipLeft=gBriefDrawSource;
            PopDrawSurface(&g513888,1,0,x,y+60,width,190);
            long left=x,top=y,w=width,h=height;
            MenuSetClip(&gMenuClip,left,top,w,h);
            int current=unit->slots[7].amount;
            int maximum=*(int *)((char *)PopFindItem(unit,7)+0x1f);
            char *text=BriefFormat(targetBriefResource.GetString((current!=0)+804+(current==maximum)),
                current,maximum,(current*100+50)/maximum);
            MenuDrawWrapped(text,-1,-1,1,11,this,10,60);
            facing=g4d2498->action.angle;
            PointPolar(&cursor,unit->shieldRange,unit->shieldDirection*0x1000000-facing*0x2aaaaabc);
            gMenuTextMap=(U32 *)&DAT_004bc560;gMenuTextColor=DAT_004bc4e8;
            for(int i=0;i<6;++i) {
                char *text=BriefFormat("%d",unit->armor[(i+6-facing)%6]);
                int width=MenuTextWidth(&shellFont,text);
                MenuDrawText(&shellFont,shieldControl.x+shieldLabelX[i]-shieldLabelAlign[i]*width/2,
                             shieldControl.y+shieldLabelY[i],text);
            }
            gDrawRemap=0;
            SimAnim anim=shieldAnim;
            DrawSprite(&g513888,SimAnimFrame(&anim,facing+6),shieldControl.x+24,shieldControl.y+16,0,80,80);
            gDrawRemap=f_0045bb30(unit,facing,0);
            MenuDrawImage(&g513888,(MenuImage *)((char *)drawCursorImages+0x448),shieldControl.x,shieldControl.y);
            gDrawRemap=0;shieldControl.attributes&=~0x1000000UL;
        }
    }
}
void ShieldPopup::SetAim(int x,int y) {
    AimPoint point(x,y);
    int angle=f_0047ef00(&point);angle+=facing*0x2aaaaabc;
    int distance=ISqrt(point.x*point.x+point.y*point.y);
    if(distance>48)distance=48;
    if(ShieldShift())angle=GridClamp(angle)*0x2aaaaabc;
    g4d2498->unit->shieldDirection=angle>>24;
    g4d2498->unit->shieldRange=distance;
    shieldControl.attributes&=~0x1000000UL;
}
int ShieldPopup::Process() {
    if(IsActive()) {
        int hovered=HitTest(&g4bb78c);
        if(hovered>=0)targetLimits=hovered+700;
        facing=g4d2498->action.angle;
        if(facing!=previousFacing){previousFacing=facing;Refresh();}
        if(!shieldControl.HasFlags(0x20000)) {
            if(shieldControl.changed&&modeSwitch&&ModeCheck(modeCheckName))
                modeLogName.Shields(g4d2498,g4d2498->unit->shieldDirection,g4d2498->unit->shieldRange);
            shieldControl.changed=0;
        }
        return Update();
    }
    return 0;
}
void SimInventoryDraw(ShieldPopup *p){if(p->IsActive())p->Draw();}
void ShieldPopup::Commit() {
    if(modeSwitch&&ModeCheck(modeCheckName))
        modeLogName.Shields(g4d2498,g4d2498->unit->shieldDirection,g4d2498->unit->shieldRange);
}
int ShieldPopup::AllowsWeapons(){if(IsActive()&&shieldControl.HasFlags(0x20000))return 0;return 1;}
void SimInventoryEvent(SimGame *s) {
    int selection=simInventory.Process()-1;
    if(selection<0) {
        switch(gIn80){case 13:selection=1;break;case 12:selection=4;break;}
        if(selection<0)return;
    }
    ActorRecord *actor=s->focus;
    ActorUnit *unit=actor->unit;
    long facing=actor->action.angle;
    switch(selection) {
    case 2:
        UpdatePayload(unit,facing);simInventory.Refresh();break;
    case 1:
        SoundPlay(9,0);SoundTrigger(210,0);
        simInventory.SetAim(0,0);
        UpdatePayload(unit,facing);simInventory.Refresh();
        if(modeSwitch&&ModeCheck(modeCheckName))
            modeLogName.Shields(g4d2498,g4d2498->unit->shieldDirection,g4d2498->unit->shieldRange);
        break;
    case 4:
        SoundPlay(9,0);unit->shieldDirection=128;unit->shieldRange=24;
        UpdatePayload(unit,facing);simInventory.Refresh();
        if(modeSwitch&&ModeCheck(modeCheckName))
            modeLogName.Shields(g4d2498,g4d2498->unit->shieldDirection,g4d2498->unit->shieldRange);
        shieldControl.attributes&=~0x1000000UL;break;
    }
}



GuiControl *shieldControls[] = {&shieldNumber,&shieldButton,&shieldControl};
