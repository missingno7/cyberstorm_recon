/* Weapon popup; inferred types and names. No original __FILE__ string. */
#include <stdio.h>
typedef unsigned char U8;
typedef unsigned long U32;
struct GuiRect {
    long x,y,width,height;
};
struct GuiPoint {
    long x,y;
    GuiPoint(long a=0,long b=0):x(a),y(b) {
    }
};
struct ViewRect {
    int left,top,right,bottom;
};
struct InputReq {
    U32 mask,state;
    int HelpPressed() {
        return (state&0xa0)==0x20;
    }
};
struct Timer;
extern void TimerSet(Timer *,U32);
extern int TimerExpired(Timer *);
struct Timer {
    U32 start,end,duration;
    Timer(U32 n) {
        TimerSet(this,n);
    }
};
struct StrPool {
    char *GetString(long,int=0);
};
struct Font {
    void *data;
};
struct MenuImage {
    U32 offset;
    unsigned short width,height;
    U32 flags;
};
struct MenuImages {
    U32 count;
    MenuImage images[1];
};
class GuiControl:public GuiRect {
    public:U8 style[4];
    U32 attributes,shortcut;
    GuiControl(U32,U32);
    virtual ~GuiControl();
    virtual void Reset(unsigned);
    virtual void Draw(unsigned);
    virtual int Event(unsigned,U32);
    virtual int HitTest(const GuiPoint *);
    void Clip();
    int GetFlags() {
        return attributes;
    }
};
class GuiScreen:public GuiRect {
    public:InputReq *input;
    GuiControl **controls;
    StrPool *strings;
    unsigned count,focus;
    long hovered;
    unsigned pressed;
    U32 options;
    long firstString;
    int navigation,delayed;
    unsigned delayedControl;
    U32 repeatTimer[3];
    GuiScreen(GuiControl **,unsigned,U32);
    void LoadLayout(long);
    void Open(InputReq *,StrPool *,long,int);
    int Update();
    void Draw();
    void Invalidate();
    long HitTest(const GuiPoint *);
    int IsActive() {
        return input!=0;
    }
};
class MenuImageButton:public GuiControl {
    public:U32 state;
    long image,repeat;
    MenuImageButton(long=0,long=0,U32=0);
    virtual ~MenuImageButton();
    virtual void Reset(unsigned);
    virtual void Draw(unsigned);
    virtual int Event(unsigned,U32);
};
/* The old SimMapGadget typesig hypothesis names the 0x28-byte value
 * gadget at 0048df88. This 0x2c-byte map gadget owns 0048dbe8. Reuse
 * the independently bound constructor/destructor linkage explicitly. */
#pragma aux weapon_map_constructor "W?$ct:SimMapGadget$n(ulllul)_";
#pragma aux weapon_map_destructor "W?$dt:SimMapGadget$n()_";
class WeaponMapGadget:public GuiControl {
    public:U32 state;
    long kind,data;
    __pragma("weapon_map_constructor") WeaponMapGadget(U32,long,long,U32);
    virtual __pragma("weapon_map_destructor") ~WeaponMapGadget() {
    }
    virtual void Reset(unsigned);
    virtual void Draw(unsigned);
    virtual int Event(unsigned,U32);
    virtual int HitTest(const GuiPoint *);
};
class WeaponControl:public GuiControl {
    public:U32 state;
    const char *text;
    long font,value,kind,cost,condition;
    WeaponControl();
    virtual ~WeaponControl() {
    }
    virtual void Reset(unsigned);
    void SetText(const char *);
    virtual void Draw(unsigned);
    virtual int Event(unsigned,U32);
};
#pragma pack(push,1)
struct ActorUnit {
    char name[17];
    U8 slots[184];
    U8 tail[66];
};
struct ActorItem {
    U8 head[9];
    U8 level;
    U8 unknown0a;
    int cost;
    int unknown0f;
    int max;
    U8 tail[4];
};
struct ActorRecord {
    int id,next,index,kind;
    U8 unknown10[8];
    ActorUnit *unit;
    U8 unknown1c[9];
    U8 type,face;
    U8 unknown27[31];
    int angle;
    U8 unknown4a[63];
    U8 dead;
    U8 unknown8a[4];
    U8 weapons;
    int IsVisible() {
        return !dead && type!=17;
    }
    U8 WeaponMask() {
        return weapons;
    }
};
struct SimGame {
    U8 head[0x38];
    ActorRecord *focus;
};
struct TargetRecord {
    U8 bytes[180];
};
struct WeaponStatus {
    int kind;
    U8 unknown4[8];
    int cost,condition;
    U8 unknown14[55];
    int target;
    U8 unknown4f[31];
};
#pragma pack(pop)
struct WeaponReport {
    U8 head[223];
    WeaponStatus weapons[8];
};
struct ApplyPoint {
    int x,y,z;
};
struct TargetScene {
    U8 head[0xc];
    ApplyPoint source;
    U8 unknown18[0x3c];
    U32 model;
    U8 unknown58[8];
    ApplyPoint destination;
    U8 unknown6c[0x34];
    int angle;
    int unusedA4,distance,unusedAC,heading;
};
struct SimAnim {
    U8 bytes[8];
};
struct FogData {
    int v[8];
};
extern GuiControl *weaponControls[];
extern ActorRecord *g4d2498;
struct ModeSystem;
extern ModeSystem modeCheckName;
extern GuiPoint g4bb78c;
extern int targetLimits;
extern int gIn80,gInA4;
inline int InputKey() {
    return gIn80;
}
inline int InputShift() {
    return gInA4;
}
extern StrPool targetBriefResource,gShellMiniStrings;
extern char gDebugText[];
extern MenuImages *gMenuImages;
extern char g513888;
extern ViewRect viewportClipLeft,gBriefDrawSource;
extern GuiRect gMenuClip,simClip;
extern U32 gMenuTextMode,gMenuTextColor;
extern U32 *gMenuTextMap;
extern U32 deathTextMap[],infoTextMap[],warningTextMap[],deathTextColor,infoTextColor,warningTextColor;
extern Font g511dc0,gDefaultFont;
extern ActorUnit previewUnit;
extern SimAnim previewAnim;
extern U8 previewDirection;
extern TargetScene targetScene;
extern WeaponReport weaponReport;
extern int targetSceneActive,targetActorId,modeSwitch,previewMode;
extern MenuImages *weaponImages;
extern char fallbackName[],gView[];
extern FogData gFogDefaultData;
extern int MenuControlChanged(void *,unsigned,U32 *);
extern void MenuSetClip(GuiRect *,long,long,long,long);
extern void MenuDrawImage(void *,const MenuImage *,long,long);
extern void MenuDrawWrapped(const char *,long,long,long,long,const GuiRect *,int=0,int=0);
struct SoundSettings;
extern int SoundPlay(unsigned,const SoundSettings *);
extern int HercMountAllowed(ActorUnit *,int);
extern ActorItem *PopFindItem(ActorUnit *,int,int=-1);
extern char *PopItemValue(ActorItem *);
extern void PopRegisterItem(void *,int);
extern void TargetInitializeRecord(void *,TargetRecord *,int,int);
extern "C" void f_0045d6b0(WeaponReport *,TargetScene *,int);
extern "C" char *BriefFormat(const char *,...);
extern int SimPreviewCost(SimGame *,int,int);
extern char *HercFormatItem(ActorUnit *,ActorItem *,int,unsigned);
extern "C" void f_00477304(char *);
extern void PopDrawSurface(void *,int,int,int,int,int,int);
extern void GuiResetClip(void *);
extern ActorRecord *ActorGetByIndex(int);
extern "C" int f_00439d02(ActorUnit *);
extern void SimAnimSet(SimAnim *,int,int,int);
extern long GridClamp(U32);
extern void *HercConditionDiagram(ActorUnit *,ActorUnit *);
extern void *gDrawRemap;
extern MenuImage *SimAnimFrame(SimAnim *,int);
extern void DrawSprite(void *,MenuImage *,int,int,int,int,int);
extern "C" void f_0042f490(void *,int,int,int);
extern "C" void *f_0045bb30(ActorUnit *,int,ActorUnit *);
extern int PointAngle(ApplyPoint *,ApplyPoint *);
extern signed char weaponMarkerOffsets[][2];
extern void SetTerrainView(void *,const FogData *,int);
extern "C" void f_0042d640(void *,U32,GuiPoint,int);
struct PlayerSlot;
extern PlayerSlot *GetModePlayer(void *,int);
extern void MenuDrawTextAt(Font *,long,long,int,const char *);
extern void MenuDrawText(Font *,long,long,const char *);
extern char simInventory[];
extern "C" int f_00444f8f(void *);
extern "C" int f_0044f290(SimGame *,int=-1,int=1);
extern void SimSetReturnFire(SimGame *,int,int,int);
struct PopNode;
#pragma aux weapon_screen_destructor "W?PopNodeReset$n(pn$PopNode$$)v";
class WeaponPopup:public GuiScreen {
    public:int changed;
    Timer detailTimer,previewTimer;
    int mode,a,b,top;
    char labels[8][60];
    WeaponPopup();
    __pragma("weapon_screen_destructor") ~WeaponPopup() { input=0; }
    void OpenWeapons(InputReq *);
    void Close();
    void UpdateWeapons();
    int Process();
    void UpdatePreview();
    void Invalidate();
    void SetMode(int,int,int);
};
WeaponControl weaponRows[8];
MenuImageButton weaponButtons[8];
WeaponMapGadget weaponMap(16,0,0,0);
WeaponPopup simActions;
WeaponControl::WeaponControl():GuiControl(16,0),state(0),text(0) {
}
void WeaponControl::Reset(unsigned) {
    attributes&=~0x1060000UL;
    state=0;
}
void WeaponControl::SetText(const char *p) {
    text=p;
}
void WeaponControl::Draw(unsigned index) {
    U32 previous=state;
    if(MenuControlChanged(this,index,&state)) {
        long left=x,top=y,w=width,h=height;
        MenuSetClip(&gMenuClip,left,top,w,h);
        int imageIndex=(state&0x80000)?110:font+108;
        MenuImages *images=gMenuImages;
        MenuImage *image=&images->images[imageIndex];
        MenuDrawImage(&g513888,image,x+width-(int)image->width,y);
        ViewRect save=viewportClipLeft;
        viewportClipLeft.right-=(int)image->width/2;
        MenuDrawImage(&g513888,image,x,y);
        viewportClipLeft=save;
        long color=font==1?14:10;
        if(text)MenuDrawWrapped(text,-1,-1,0,color,this,0,1);
        if(value>=0) {
            char buffer[12];
            sprintf(buffer,"%d",value);
            MenuDrawWrapped(buffer,1,-1,0,color,this,0,2);
        }
        if((state&0x80000) && !(previous&0x80000))SoundPlay(392,0);
    }
}
int WeaponControl::Event(unsigned index,U32) {
    return index+1;
}
WeaponPopup::WeaponPopup():GuiScreen(weaponControls,17,0),detailTimer(0),previewTimer(0) {
}
void WeaponPopup::OpenWeapons(InputReq *request) {
    if(!IsActive()) {
        changed=1;
        mode=0;
        a=0;
        b=0;
        changed=1;
        LoadLayout((long)"POPWEAP.BOX");
        Open(request,&targetBriefResource,701,0);
        changed=1;
        top=0;
        ActorUnit *unit=g4d2498->unit;
        for(int i=0; i<8; ++i) {
            int ready=HercMountAllowed(unit,i+11);
            MenuImageButton *button=&weaponButtons[i];
            WeaponControl *row=&weaponRows[i];
            if(!ready) {
                int flags=row->GetFlags();
                flags=(flags&0xff1fffff)+0xa00000;
                row->attributes=flags;
                flags=button->GetFlags();
                flags=(flags&0xff1fffff)+0xa00000;
                button->attributes=flags;
            }
            else {
                int flags=row->GetFlags();
                flags=flags&0xff1fffff;
                row->attributes=flags;
                flags=button->GetFlags();
                flags=flags&0xff1fffff;
                button->attributes=flags;
                int bottom=weaponRows[i].y+weaponRows[i].height+1;
                if(top<bottom)top=bottom;
            }
        }
    }
}
void WeaponPopup::Close() {
    if(IsActive())input=0;
}
void WeaponPopup::UpdateWeapons() {
    ActorUnit *unit=g4d2498->unit;
    if(targetSceneActive)TargetInitializeRecord(&modeCheckName,(TargetRecord *)&targetScene,g4d2498->id,g4d2498->kind);
    f_0045d6b0(&weaponReport,&targetScene,0);
    for(int i=0; i<8; ++i) {
        int kind=weaponReport.weapons[i].kind;
        sprintf(labels[i],"%d - %s",i+1,PopItemValue(PopFindItem(unit,i+11)));
        weaponRows[i].text=labels[i];
        weaponRows[i].font=kind?1:3;
        weaponRows[i].value=-1;
        weaponRows[i].kind=kind;
        weaponRows[i].condition=weaponReport.weapons[i].cost;
        weaponRows[i].cost=weaponReport.weapons[i].condition;
        if(PopFindItem(unit,i+11)->max)weaponRows[i].value=*(int *)((char *)unit+0x6d+i*8);
        U8 mask=g4d2498->weapons;
        int bit=1<<i;
        int image=(mask&bit)?115:113;
        weaponButtons[i].image=image;
    }
    GuiScreen::Invalidate();
}
int WeaponPopup::Process() {
    if(IsActive()) {
        if(changed || TimerExpired(&previewTimer)) {
            TimerSet(&previewTimer,2500);
            UpdateWeapons();
        }
        if(changed || TimerExpired(&detailTimer)) {
            TimerSet(&detailTimer,750);
            UpdatePreview();
        }
        changed=0;
        long hovered=HitTest(&g4bb78c);
        if(hovered>=0) {
            targetLimits=hovered+600;
            int selection=hovered;
            if(selection>=0 && selection<8) {
                int kind=weaponRows[selection].kind;
                targetLimits=kind+623;
                if(!kind) {
                    targetLimits=selection+636;
                    char *buffer=gShellMiniStrings.GetString(737);
                    int stringId=(weaponRows[selection].cost!=100)+722;
                    char *format=targetBriefResource.GetString(stringId);
                    sprintf(buffer,format,weaponRows[selection].condition,weaponRows[selection].cost);
                    SimPreviewCost((SimGame *)&modeCheckName,weaponReport.weapons[selection].target,0);
                }
                if(input->HelpPressed()) {
                    ActorUnit *unit=g4d2498->unit;
                    HercFormatItem(unit,PopFindItem(unit,selection+11),selection+11,-1281);
                    f_00477304(gDebugText);
                }
            }
        }
        return Update();
    }
    return 0;
}
void WeaponPopup::UpdatePreview() {
    viewportClipLeft=gBriefDrawSource;
    viewportClipLeft.top=top;
    if(top<330)PopDrawSurface(&g513888,1,0,0,330,148,130);
    else PopDrawSurface(&g513888,1,0,0,339,148,121);
    GuiResetClip(&simClip);
    int x=36,y=342,size=80;
    TargetScene *scene=&targetScene;
    ActorRecord *actor=ActorGetByIndex(targetActorId);
    if(actor && actor->IsVisible()) {
        ActorUnit *unit=actor->unit;
        if(!previewUnit.slots[1])previewUnit=*unit;
        SimAnimSet(&previewAnim,2,f_00439d02(unit),1);
        previewDirection=GridClamp(scene->angle-scene->heading);
        gDrawRemap=HercConditionDiagram(unit,&previewUnit);
        DrawSprite(&g513888,SimAnimFrame(&previewAnim,-1),x+2,y+2,0,size-4,size-4);
        gDrawRemap=0;
        f_0042f490(fallbackName,x,y,8);
        MenuDrawImage(&g513888,(MenuImage *)((char *)weaponImages+0x7d8),x,y);
        gDrawRemap=f_0045bb30(unit,actor->face,&previewUnit);
        MenuDrawImage(&g513888,(MenuImage *)((char *)weaponImages+0x43c),x-1,y-1);
        gDrawRemap=0;
        int direction=(actor->face+GridClamp(PointAngle(&scene->destination,&scene->source)-actor->angle))%6;
        MenuDrawImage(&g513888,&weaponImages->images[direction+168],x+weaponMarkerOffsets[direction][0],y+weaponMarkerOffsets[direction][1]);
        previewUnit=*unit;
    }
    else if(scene->model) {
        FogData fog=gFogDefaultData;
        fog.v[3]=16;
        fog.v[5]=-57;
        SetTerrainView(gView,&fog,0);
        f_0042d640(gView,scene->model,GuiPoint(x+40,y+40),0);
        SetTerrainView(gView,&gFogDefaultData,0);
    }
    if(!mode && actor) {
        gMenuTextMap=infoTextMap;
        gMenuTextColor=infoTextColor;
        int distance=(scene->distance+50)/100;
        char *text=BriefFormat("%d.%d",distance/10,distance%10);
        char *name=(char *)actor->unit;
        if(modeSwitch || previewMode)name=(char *)GetModePlayer(&modeCheckName,actor->index)+16;
        MenuDrawTextAt(&g511dc0,74,y+90,0,name);
        MenuDrawTextAt(&g511dc0,25,y+103,-1,targetBriefResource.GetString(721));
        MenuDrawTextAt(&g511dc0,123,y+103,1,text);
    }
    else {
        gMenuTextMode=1;
        if(mode==2) {
            gMenuTextMap=deathTextMap;
            gMenuTextColor=deathTextColor;
            MenuDrawTextAt(&g511dc0,10,y+90,-1,targetBriefResource.GetString(719));
            MenuDrawTextAt(&g511dc0,10,y+103,-1,targetBriefResource.GetString(720));
            MenuDrawTextAt(&g511dc0,138,y+90,1,BriefFormat("%d",a));
            MenuDrawTextAt(&g511dc0,138,y+103,1,BriefFormat("%d",b));
        }
        else if(mode==1) {
            gMenuTextMap=warningTextMap;
            gMenuTextColor=warningTextColor;
            MenuDrawText(&gDefaultFont,10,y+95,targetBriefResource.GetString(718));
            viewportClipLeft=gBriefDrawSource;
            return;
        }
    }
    viewportClipLeft=gBriefDrawSource;
}
void SimActionsDraw(WeaponPopup *p) {
    if(p->IsActive())p->Draw();
}
void WeaponPopup::Invalidate() {
    changed=1;
}
void WeaponPopup::SetMode(int m,int x,int y) {
    mode=m;
    a=x;
    b=y;
    changed=1;
}
void WeaponCommand(SimGame *s) {
    if(!f_00444f8f(simInventory))return;
    int selection=simActions.Process()-1;
    if(selection<0) {
        int key=InputKey();
        if(key>=2 && key<=9 && !InputShift())selection=key-2;
        else if(key==30 && !InputShift())f_0044f290(s);
        else if(key==30) {
            TargetRecord record;
            TargetInitializeRecord(s,&record,g4d2498->id,g4d2498->kind);
            ActorRecord *p=ActorGetByIndex(*(int *)(record.bytes+156));
            while(p && p->IsVisible()) {
                if(!f_0044f290(s,-1,0))break;
            }
        }
        if(selection<0)return;
    }
    SoundPlay(9,0);
    switch(selection) {
        case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:if(s->focus) {
            if(HercMountAllowed(s->focus->unit,selection+11))f_0044f290(s,selection,1);
        }
        break;
        case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:SimSetReturnFire(s,selection-8,gInA4,-1);
        simActions.UpdateWeapons();
        break;
    }
}

