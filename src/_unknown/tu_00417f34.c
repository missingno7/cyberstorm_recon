/* Shared working layouts. Gaps denote fields whose purpose remains unknown. */
#include <string.h>
typedef unsigned long U32;
typedef unsigned char byte;
struct GuiRect  {
    long x,y,width,height;
};
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
    void SetLoops(long n) {
        loops=n;
    }
    Palette *GetPalette() {
        return palette;
    }
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
struct ActorUnit  {
    byte data[0x10b];
};
struct ActorLogic  {
    byte unknown00[0x12];
    int count;
};
struct ActorRecord  {
    byte unknown00[8];
    int owner;
    byte unknown0c[8];
    int mission;
    ActorUnit *unit;
    ActorLogic *logic;
    byte unknown20[5];
    byte kind,mood;
    byte unknown27[0x62];
    byte dead;
};
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
inline int LogicMode() {
    return logicMode;
}
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
/* Storage screen; no embedded source-path evidence. */
#pragma aux range_ctor "BriefModuleInitC";
#pragma aux bar_ctor "BriefModuleInitD";
#pragma aux step_ctor "BriefModulePlace";
extern "C" void BriefSetMode(void *,int);
extern "C" void BriefAdjust(void *,int);
#pragma pack(push,1)
struct UpgradeRange  {
    int value,maximum;
    int GetValue() {
        return value;
    }
    __pragma("range_ctor") UpgradeRange(int,int);
};
struct UpgradeBar:GuiControl  {
    UpgradeRange *range;
    int extra;
    __pragma("bar_ctor") UpgradeBar(UpgradeRange *,int);
    virtual ~UpgradeBar();
};
struct UpgradeArrow:GuiControl  {
    UpgradeRange *range;
    int delta,key;
    __pragma("step_ctor") UpgradeArrow(UpgradeRange *,int,int);
    virtual ~UpgradeArrow();
};
struct StorageScreen:GuiScreen  {
    MenuImages *images;
    byte selected;
    int color[23];
    byte phase;
    byte unknownae[4];
    int redraw;
    byte unknownb6[4];
    Flic animation;
    byte count;
    int pending;
    byte remap[256],image[24],last,offset,oldOffset;
    StorageScreen();
    ~StorageScreen() {
    }
    int Count() {
        return count;
    }
    void Names(byte);
    void Ramp();
    void SetCount(int n) {
        count=n;
    }
};
#pragma pack(pop)
MenuPrefixButton storageLeft[24],storageRight[24];
MenuNumber storageNumber;
MenuButton storageButton;
UpgradeRange storageRange(50,0);
UpgradeBar storageBar(&storageRange,0);
UpgradeArrow storageNext(&storageRange,1,48),storagePrevious(&storageRange,-1,45);
MenuPanel storagePanel;
MenuNumber storageNumberA,storageNumberB,storageNumberC;
MenuHelpRegion storageHelp;
StorageScreen g4b5b50;
extern GuiControl *storageControls[];
extern FlicSound storageSounds[];
void StorageSetup();
void StorageVisibility(StorageScreen *);
void StorageColors(StorageScreen *);
void StorageVisibleNames(StorageScreen *,byte);
void StorageDetails(StorageScreen *);
void StorageStart(StorageScreen *);
void StorageAnimate(StorageScreen *);
StorageScreen::StorageScreen():GuiScreen(storageControls,58,0),animation() {
}
void ShellStorageOpen(StorageScreen *s,InputReq *input)  {
    if(s->IsActive())return;
    if(LogicMode()==1)BriefWaitA(2);
    else BriefWaitA(9);
    BriefWaitB(15);
    storageHelp.id=-1;
    SurfaceCopyRegion(&gMenuSavedSurface,&g511948,0,0);
    f_0043ba3e(gShellGame,&profileManager);
    g4a706d=g4ae17f;
    s->animation.Open((long)"hb_upg.flx");
    s->animation.SetLoops(1);
    s->animation.SetSounds(storageSounds);
    viewportClipLeft=gBriefDrawSource;
    s->animation.Update((Surface *)&g511948);
    gShellBackground=*s->animation.GetPalette();
    ShellApplyPalette(&gShellBackground);
    s->images=(MenuImages *)gResMgr.Load("DMGHRC.bmx");
    int count=g4ae07b;
    s->SetCount(count);
    storageNumber.SetAlignCore(1);
    BriefSetMode(&storageRange,0);
    BriefAdjust(&storageRange,count-23);
    s->offset=0;
    s->oldOffset=s->offset;
    s->redraw=0;
    s->phase=0;
    s->selected=0;
    s->last=99;
    s->pending=0;
    StorageSetup();
    storageNumber.SetValue(g4a706d);
    s->Names(0);
    s->LoadLayout((long)"hbmanage.box");
    s->Open(input,&gShellStrings,2001,1);
    StorageVisibility(s);
    StorageColors(s);
}
void StorageReopen(StorageScreen *s,InputReq *input) {
    BriefBlit(&g513888,1,0,0,0,0,0,260,480);
    storageHelp.id=-1;
    g4a706d=g4ae17f;
    if(LogicMode()==1)BriefWaitA(2);
    else BriefWaitA(9);
    int count=g4ae07b;
    s->SetCount(count);
    s->images=(MenuImages *)gResMgr.Load("DMGHRC.bmx");
    BriefSetMode(&storageRange,0);
    BriefAdjust(&storageRange,count-23);
    s->offset=0;
    s->oldOffset=s->offset;
    s->redraw=0;
    s->phase=3;
    s->selected=0;
    s->last=99;
    s->pending=0;
    StorageColors(s);
    storageNumber.SetValue(g4a706d);
    s->Names(0);
    s->LoadLayout((long)"hbmanage.box");
    s->Open(input,&gShellStrings,2001,1);
    StorageVisibility(s);
}
void StorageClose(StorageScreen *s) {
    if(s->IsActive()) {
        BriefBlit(&g513888,1,0,0,0,0,0,260,480);
        s->animation.Close();
        f_cb80(s->images);
        s->images=0;
        s->input=0;
    }
}
void StorageInput(StorageScreen *s) {
    if(s->IsActive()) {
        s->offset=storageRange.GetValue();
        if(s->offset!=s->oldOffset) {
            s->oldOffset=s->offset;
            s->redraw=1;
        }
        int hit=s->HitTest(&g4bb78c);
        if(hit>=0&&hit<=23)s->selected=s->offset+hit;
        storageHelp.id=hit;
        s->Update();
    }
}
byte StorageImage(StorageScreen *s) {
    return s->image[s->selected-s->offset]-1;
}
void StorageDetails(StorageScreen *s) {
    viewportClipLeft=gBriefDrawSource;
    BriefBlit(&g513888,1,0,250,0,250,0,390,480);
    MenuSetClip(&gMenuClip,250,0,390,480);
    gDrawRemap=s->remap;
    byte index=s->selected-s->offset;
    MenuDrawImage(&g513888,&s->images->image[s->image[index]-1],260,10);
    gDrawRemap=0;
    long a=f_00441d1f(&profileManager,s->selected+1),b=f_00441ce1(&profileManager,s->selected+1);
    storageNumberC.SetValue(a);
    storageNumberB.SetValue(b);
}
void StorageStart(StorageScreen *s) {
    if(gGuiEffectPriority>50)s->animation.Update((Surface *)&g511948);
    else MenuDrawImage(&g513888,&g511dd4->image[21],244,0);
    s->phase=1;
    StorageVisibility(s);
}
void StorageAnimate(StorageScreen *s) {
    if(gGuiEffectPriority>50) {
        if(s->animation.Update((Surface *)&g511948)&4) {
            ShellShadeRect(17,31,212,425);
            BriefBlit(&g513888,0,1,250,0,250,0,390,480);
            s->animation.Close();
            s->phase=2;
        }
    }
    else  {
        ShellShadeRect(17,31,212,425);
        BriefBlit(&g513888,0,1,250,0,250,0,390,480);
        s->phase=2;
    }
}
void ShellStorageRender(StorageScreen *s) {
    if(s->IsActive()) {
        switch(s->phase) {
            case 0:StorageStart(s);
            return;
            case 1:StorageAnimate(s);
            return;
            case 3:s->phase=2;
            return;
            case 2:
            storageButton.Invalidate();
            if(s->redraw) {
                BriefBlit(&g513888,1,0,0,0,0,0,260,448);
                ShellShadeRect(17,31,212,425);
                s->redraw=0;
                s->Names(s->offset);
                StorageVisibleNames(s,s->offset);
                s->Invalidate();
                s->Draw();
                SurfacePresent(&g513888);
            }
            if(s->selected!=s->last&&s->selected<s->count) {
                s->last=s->selected;
                StorageColors(s);
                s->Ramp();
                StorageDetails(s);
                s->Invalidate();
                s->Draw();
                SurfacePresent(&g513888);
            }
            /* fall through */
            default:break;
        }
        s->Draw();
    }
}
void StorageSetup() {
    for(byte i=0;i<24;i++) {
        storageLeft[i].SetColor(2);
        storageLeft[i].SetAlign(1);
        storageRight[i].SetColor(2);
        storageRight[i].SetAlign(1);
    }
    storageNumberB.SetAlignCore(1);
    storageNumberC.SetAlignCore(1);
    storageNumber.SetAlignCore(1);
    storageNumberA.SetFont(-1);
}
void StorageVisibility(StorageScreen *s) {
    switch(s->phase) {
        default:return;
        case 0:case 1:case 2:case 3:;
        int i;
        for(i=0;i<s->count&&i<24;i++) {
            storageLeft[i].Visibility(0);
            storageRight[i].Visibility(0);
        }
        while(1) {
            if(i>=24)break;
            storageLeft[i].Visibility(5);
            storageRight[i].Visibility(5);
            i++;
        }
        storageButton.Visibility(0);
        if(s->count>24) {
            storagePrevious.Visibility(0);
            storageBar.Visibility(0);
            storageNext.Visibility(0);
        }
        else {
            storagePrevious.Visibility(5);
            storageBar.Visibility(5);
            storageNext.Visibility(5);
        }
    }
}
void StorageColors(StorageScreen *s) {
    if(s->count!=0) {
        ActorUnit *u=((ProfileManager *)gShellGame)->Mission(s->selected+1);
        for(int i=0;i<23;i++) {
            int n=u->data[0x13+i*8];
            if(n==100)s->color[i]=16;
            else if(n>75)s->color[i]=22;
            else if(n>50)s->color[i]=28;
            else if(n>25)s->color[i]=34;
            else s->color[i]=40;
        }
    }
}
void StorageScreen::Names(byte base) {
    for(unsigned short i=0;i<this->count-base&&i<24;i++) {
        ActorUnit *u=((ProfileManager *)gShellGame)->Mission(i+base+1);
        storageLeft[i].SetText(f_00439d2c(u));
        this->image[i]=f_00439d02(u)+1;
    }
}
void StorageVisibleNames(StorageScreen *s,byte base) {
    unsigned short i=0;
    while(1) {
        if(i>=24)break;
        if(i<s->Count()-base) {
            storageLeft[i].Visibility(0);
            storageRight[i].Visibility(0);
        }
        else {
            storageLeft[i].Visibility(5);
            storageRight[i].Visibility(5);
        }
        i++;
    }
}
void StorageRedraw(StorageScreen *s) {
    s->phase=2;
    StorageVisibility(s);
    s->Invalidate();
}
void StorageCredits() {
    storageNumber.SetValue(g4a706d);
}
void StorageSave(StorageScreen *s) {
    if(s->pending) {
        s->pending=0;
        f_0043ba3e(&profileManager,gShellGame);
    }
}
void StorageRestore(StorageScreen *s) {
    if(s->pending) {
        s->pending=0;
        f_0043ba3e(gShellGame,&profileManager);
    }
}
void StorageScreen::Ramp() {
    for(int i=0;i<23;i++)for(int j=0;j<6;j++) {
        byte c=this->color[i]+j;
        this->remap[i*6+j+16]=c;
    }
}
byte StorageSelected(StorageScreen *s) {
    return s->selected;
}
void StorageDone() {
    BriefDone("hercbuy");
}
