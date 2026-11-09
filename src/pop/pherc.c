typedef unsigned long uint;
typedef unsigned char byte;
struct GuiControl {
    long x,y,width,height; byte style[4]; uint attributes,shortcut; int X(){return x;}int Y(){return y;}int Width(){return width;}int Height(){return height;}void SetState(uint n){attributes=(attributes&0xff1fffffUL)+(n<<21);}void Clip();void Background();void Invalidate(){attributes&=~0x1000000UL;}
    GuiControl(unsigned long style, unsigned long flags);
    virtual ~GuiControl();
};
extern void *PTR_f_00446804_0048dd04;
struct StrPool {
    char *GetString(long id, int loadData);
    void Lock();
    void Unlock();
};
struct FileRecord;
struct ResMgr {
    int FindPath(const char *,FileRecord *,int);
    int *Load(const char *name, int mode=1, int reserved=0, int flags=0);
};
extern ResMgr gResMgr; extern StrPool *gGuiStringPool; extern long gGuiStringBase; extern StrPool gMenuPrefixStrings; extern StrPool targetBriefResource; extern unsigned long *gMenuTextMap;
struct ViewRect {int left,top,right,bottom;};extern ViewRect viewportClipLeft,gBriefDrawSource;
struct GuiRect { long x, y, width, height; };
extern GuiRect gMenuClip;
struct Font;
extern char DAT_00511dc0[]; extern char g513888; extern int DAT_004d1fa4; extern char DAT_004d1f48, DAT_004d1f49, DAT_004d1f4a, DAT_004d1f4b;
extern long DAT_0051390c, DAT_00513910, DAT_00513914, DAT_00513918; extern char s__3d___0048b30a[]; extern char DAT_0048b305[]; extern long DAT_004bb7a4, DAT_004bc4f8; extern long g4bb78c;
extern long gInA4; extern void *PTR_f_00446848_0048dcf0; extern int f_00426d74(int, int, unsigned long *); extern void f_0047bfa0(int, int, int, int, int, int, int);
extern byte *f_00438374(int, int, int); extern void f_00438730(byte *); extern int f_00446493(int, int, int *, int *); extern byte *f_0047d420(byte *); extern int f_0047cc10(int *, byte *);
extern void f_0047cf80(int *, int, int, byte *); extern void f_0047b930(int, int, int); extern void f_0047bad0(int, int, int, int, int); extern void MenuSetClip(GuiRect *, long, long, long, long);
extern void MenuSetColors(void *, unsigned long, long); extern void MenuFill(void *, long, long, long, long); extern void MenuDrawText(Font *, long, long, const char *);
extern int MenuControlChanged(void *, unsigned long, unsigned long *); extern long MenuTextWidth(Font *, const char *);
extern "C" void RenderFormattedResourceText(void *,int,int,const char *,const char *,const char *); extern "C" void LogicCallMode(int, int); extern uint f_0043a7e8(int, uint); extern long GridClamp(unsigned long);
struct GuiScreen {char header[16];int active;char rest[56];GuiScreen(GuiControl **,unsigned,uint);void Reset(GuiControl **,unsigned,uint);long HitTest(const void *);void Invalidate();int IsActive(){return active!=0;}int Update();};
inline long AutoAdvanceEnabled() { return gInA4; }
extern void PopObjectConstruct(void *object); extern "C" void BriefModuleOpen(void *object, int id, int delay, int flags); extern int f_00439d48(int); extern void f_00446350(int);
extern void f_00425e1a(int); struct SimGame;struct ActorRecord;extern void SimDrawTimer(SimGame *,int,int,int=1,int=-1); extern void f_0047ae20(unsigned long *); struct HercModule;extern int f_00445e23(HercModule *); extern void PilotSetState(SimGame *,int);
extern void SimSetFocus(SimGame *,ActorRecord *); extern void f_0047e670(void *, void *, unsigned long); extern int f_0047fce0(int); extern void f_0047fca0(int *, int);
extern void f_0043ac02(int, int *, int *, int *, int *); extern void f_0047c320(void *); extern void SigsStart(void *,int); extern void SurfacePresent(void *); extern void SetGameWindowSize(int=-1,int=-1);
extern void f_004272cd(void *, long, long, long); extern void f_0042732e(void *, long, long); extern "C" int BriefWaitB(int); extern "C" int BriefWaitA(int); extern void f_0047eb40(void *);
extern void f_0041caf9(void *, int); extern void f_00445c81(int); extern char DAT_00511df0, DAT_00511948, DAT_00511958;
extern long DAT_00513890, DAT_00513898, g5138ac, DAT_005138b0, DAT_00511960; extern int DAT_004faf44; extern "C" void ReleasePoolData(void *); extern "C" void GlobalBlockFree(void *);
extern char g4b74f4; extern char g511948, gMenuSavedSurface; extern void GuiResetClip(void *); extern "C" void BriefRenderUpdate(void *); extern char modeCheckName[]; inline int SurfaceWidth(){return g5138ac;}inline int SurfaceHeight(){return DAT_005138b0;}inline void *SurfacePixels(){return (void *)DAT_00513890;}
extern char DAT_004a009c; extern char DAT_004d1f4c, DAT_004d1f78; extern int _DAT_004a00a4; extern int modeSwitch; extern char DAT_004faf48;
struct ActorRecordCopy {byte data[267];};
struct ActorRecord;struct PacketList {void Repair(ActorRecord *,char,char,int);};extern PacketList modeLogName;
extern int DAT_004faf40; extern int TimeSleep(unsigned long); extern void f_004259f7(void *, const char *); extern void f_00425f87(void *, void *, void *, int, int); extern int f_0046c461(int, int *);
extern void f_0041c891(void *, int); extern char s_DMGGUI_BMX_0048b310[], s_DMGHRC_BMX_0048b31b[], s_DMGGUI_BOX_0048b326[]; extern "C" void *memset(void *, int, unsigned long);
extern char DAT_004d1f63, DAT_004d1f8f; extern int DAT_005138e8, DAT_005138f4, DAT_005138f8; extern int DAT_004bc560, DAT_004bc4e8; extern void f_0047bff0(void *, int *, int, int);
extern void f_0047cfc0(void *, int, int, int, byte *);
struct HercControl : GuiControl {
    unsigned long value;
    HercControl();
};
struct HercDetailControl : GuiControl {
    HercDetailControl();
};
struct FrameList {int count;int Count(){return count;}};
#pragma aux file_init_name "W?FileInit$n(pn$FileRecord$$)v";
struct FileRecord {uint handle,open,flags,error;byte direct,counted,pad[2];uint position,length;__pragma("file_init_name") FileRecord();uint IsOpen(){return open;}};
struct GameTimer {uint start,end,delay;GameTimer(uint n);};
void GameTimerSet(GameTimer *,uint);int GameTimerExpired(GameTimer *);
inline GameTimer::GameTimer(uint n){GameTimerSet(this,n);}
#pragma pack(push,1)
struct MenuButton:GuiControl {uint state;const char *text;MenuButton(const char * =0,uint=0);virtual ~MenuButton();};
struct MenuHelpRegion:GuiControl {long id;int activated;MenuHelpRegion();virtual ~MenuHelpRegion();};
struct MenuNumber:GuiControl {uint unused;const char *text;long value;signed char font,color,flash,align;MenuNumber(signed char=-1,signed char=18);virtual ~MenuNumber();};
struct MenuImageLabel:GuiControl {const char *text;long image;signed char ax,ay,font,unused,color,bevel;MenuImageLabel(long=0,signed char=0,signed char=0,signed char=1,signed char=0,signed char=11,signed char=0);virtual ~MenuImageLabel();void SetStyle(signed char,signed char,signed char,signed char,signed char,signed char);};
struct HercModule:GuiScreen {
 byte flag4c;ActorRecord *actor;int *guiImages,*hercImages;byte palette[0x452],remap[256],colors[24];
 ActorRecordCopy *unit;int portrait,selected,previous,savedManual,savedAudio,unknown5db,angle,grid,changed,showDamage,points,total;
 void *buffer;int totalBytes,loaded,frame,speed;FrameList *frames;byte unknown60f;GameTimer timer;FileRecord file;ActorRecordCopy backup;
 MenuButton button;MenuHelpRegion help;HercDetailControl detail;MenuNumber number;MenuImageLabel labels[4];HercControl controls[24];GuiControl *controlList[34];byte ids[34];
 HercModule();void Update();byte Color(int n){return colors[n];}int HelpPressed(){return ((*(uint **)((char *)this+16))[1]&0xa0)==0x20;}int SavedManual(){return savedManual;}int SavedAudio(){return savedAudio;}int Speed(){return speed;}FrameList *Frames(){return frames;}ActorRecordCopy *Unit(){return unit;}int Loaded(){return loaded;}int Width(){return *(int *)((char *)this+8);}
};
#pragma pack(pop)
struct ActorUnit;struct ActorItem;struct SoundSettings;extern char *PopItemValue(ActorItem *);
extern int HercMountAllowed(ActorUnit *,int);extern "C" int f_00439d02(void *);
extern void DecodeLaunchPalette(void *,void *);extern void ApplyPaletteData(void *,int=0,int=256);extern void GuiFilterPalette(void *,int=0); extern "C" void AssertFail(const char *,int,const char *);
extern "C" byte *BriefFormat(const char *,...); extern int IORelease(FileRecord *);extern uint IoClose(FileRecord *);extern void *GlobalBlockAlloc(uint,unsigned);
extern int next_link_seed(uint *);extern uint gFxSeed; extern void PilotManagerPoll(void *);extern int HercRepairPoints(ActorUnit *);extern int HercRepairNeeded(ActorUnit *,int);
extern ActorItem *PopFindItem(ActorUnit *,int,int);extern char *HercFormatItem(ActorUnit *,ActorItem *,int,unsigned);
extern void DialogTimedMessage(const char *);extern char gDebugText[];extern StrPool gShellMiniStrings;
extern void HelpSetDirty(int);extern ActorRecord *ActorGetByIndex(int);extern ActorRecord *ActorFindRelated(ActorRecord *);extern ActorRecord *ActorFindRelatedOther(ActorRecord *);
extern unsigned long gIn80,gIn7c,gInA0;extern int SimShortcutMode(void *); extern "C" int sprintf(char *,const char *,...);
void f_0044591f(HercModule *,int);void f_00446570(int *);void f_00446253(int);
void f_004465d6(int);
inline int Percent(int n,int p){return n*p/100;}inline int ClampValue(int n,int lo,int hi){if(n<=lo)return lo;if(n>=hi)return hi;return n;}
inline void GuiControl::Clip(){int left=x;int top=y;int w=width;int h=height;MenuSetClip(&gMenuClip,left,top,w,h);}
inline void GuiControl::Background(){f_0047bfa0((int)&g513888,1,0,x,y,width,height);}
void f_00445238(void) {
    PopObjectConstruct(&DAT_004a009c);
    BriefModuleOpen(&DAT_004d1f4c, 0x7f, 1000, 0x33);
    _DAT_004a00a4 = 1;
    BriefModuleOpen(&DAT_004d1f78, 0x82, 1000, 0x34);
    _DAT_004a00a4 = 2;
}
HercModule::HercModule():GuiScreen(0,0,0),timer(0),file(),button(),help(),detail(),number() {
 controlList[2]=&number;controlList[1]=&detail;controlList[0]=&help;
 controlList[27]=(GuiControl *)&DAT_004d1f4c;controlList[28]=(GuiControl *)&DAT_004d1f78;controlList[29]=&button;
 for(int i=0;i<24;i++)controlList[i+3]=&controls[i];
 for(i=0;i<4;i++){labels[i].SetStyle(-1,-1,0,0,11,0);controlList[i+30]=&labels[i];}
 Reset(controlList,34,0);
}
HercControl::HercControl() : GuiControl(0x18, 0), value(0) {
}
void f_004453fd(void *object) {
    *(unsigned short *)((char *)object + 0x16) &= 0xfef9;
    *(unsigned long *)((char *)object + 0x20) = 0;
    return;
}
void f_0044540f(HercControl *control,int id) {
 int unused;uint prior=control->value;
 if(MenuControlChanged(control,id,&control->value)){
  viewportClipLeft=gBriefDrawSource;control->Clip();
  control->Background();
  HercModule *panel=(HercModule *)DAT_004d1fa4;ActorUnit *unit=(ActorUnit *)panel->Unit();unsigned index=panel->ids[id];
  byte color=panel->Color(index);DAT_004d1f49=color+4;DAT_004d1f4a=color+2;DAT_004d1f4b=color;gMenuTextMap=(uint *)&DAT_004d1f48;
  char *title;char *value;int condition;
  if(id==26){title=gGuiStringPool->GetString(gGuiStringBase+id,1);value="";condition=((HercModule *)DAT_004d1fa4)->total;}
  else{condition=*((byte *)unit+index*8+0x13);long textId=index+1102;title=gMenuPrefixStrings.GetString(textId,0);value=PopItemValue(PopFindItem(unit,index,-1));}
  if(((HercModule *)DAT_004d1fa4)->showDamage){if(id==26)condition=100;else condition=f_00446493(DAT_004d1fa4,index,&unused,&unused);}
  viewportClipLeft.right=control->x+200;
  RenderFormattedResourceText(DAT_00511dc0,control->x+5,control->y+1,DAT_0048b305,title,value);
  viewportClipLeft=gBriefDrawSource;
  const char *text=(char *)BriefFormat(s__3d___0048b30a,condition);
  MenuDrawText((Font *)DAT_00511dc0,control->x+control->width-5-MenuTextWidth((Font *)DAT_00511dc0,text),control->y+1,text);
  if(control->value&0x80000){MenuSetColors(&g513888,DAT_004bc4f8,-1);MenuFill(&g513888,control->x,control->y,control->width,control->height);if(!(prior&0x80000))LogicCallMode(0x188,0);}
 }
}
uint f_00445633(void *param_1, int param_2, byte param_3) {
    uint uVar5;
    int iVar2, iVar3, iVar4;
    uint uVar1;
    if (param_2 != 0x1a)
        uVar5 = (uint)*(byte *)(DAT_004d1fa4 + param_2 + 0xc83);
    else
        uVar5 = 0x17;
    if (*(int *)(DAT_004d1fa4 + 0x5eb) == 0 && (param_3 & 8) != 0) {
        if (AutoAdvanceEnabled())
            iVar3 = 10;
        else
            iVar3 = 1;
        iVar4 = 0;
        for (iVar2 = 0; iVar2 < iVar3; ++iVar2) {
            uVar1 = f_0043a7e8(*(int *)(DAT_004d1fa4 + 0x5c3), uVar5);
            iVar4 += uVar1;
        }
        if (iVar4 != 0) {
            LogicCallMode(1, 0);
            ((GuiScreen *)DAT_004d1fa4)->Invalidate();
            *(unsigned long *)(DAT_004d1fa4 + 0x5e7) = 1;
        }
    }
    return 0;
}
HercDetailControl::HercDetailControl() : GuiControl(8, 0) {
}
void f_004456f3(void) {
}
uint f_004456f4(int *box) {
 int mouse=g4bb78c;
 int left=box[0];
 int widthLow=box[2];int low=left+widthLow*42/100;
 int widthHigh=box[2];int high=widthHigh*58/100+left;
 int right=box[2]+left;
 int speed=0;
 if(mouse<low)speed=(low-mouse)*40/(low-left)+1;
 else if(mouse>high)speed=-(mouse-high)*40/(right-high)-1;
 speed=ClampValue(speed,-40,40);
 *(int *)(DAT_004d1fa4+0x607)=speed;
 return 0;
}
void f_0044579c(int *param_1, void *param_2) {
    unsigned int priorActive = ((GuiScreen *)param_1)->IsActive();
    if (!priorActive) {
        DAT_004faf44 = priorActive;
        if (modeSwitch != 0 && DAT_004faf40 != 0)
            TimeSleep(0xfa);
        memset(param_1 + 0x13, 0,
               (unsigned long)((char *)param_1 + 0x60f - (char *)(param_1 + 0x13)));
        *(int *)((char *)param_1 + 0x5d7) = BriefWaitA(0xc);
        *(int *)((char *)param_1 + 0x5d3) = BriefWaitB(0x1d);
        SetGameWindowSize(640,480);
        SigsStart(&g513888,0);
        SurfacePresent(&g513888);
        f_004272cd(&g511948,(long)SurfacePixels(),SurfaceWidth(),SurfaceHeight());
        f_0042732e(&gMenuSavedSurface,SurfaceWidth(),SurfaceHeight());
        DAT_00513898 = DAT_00511960;
        int *gui = gResMgr.Load(s_DMGGUI_BMX_0048b310);
        *(int **)((char *)param_1 + 0x51) = gui;
        int *herc = gResMgr.Load(s_DMGHRC_BMX_0048b31b);
        *(int **)((char *)param_1 + 0x55) = herc;
        f_004259f7(param_1, s_DMGGUI_BOX_0048b326);
        f_00425f87(param_1, param_2, &targetBriefResource, 0x12d, 0);
        f_0046c461(0x1e, 0);
        ((StrPool *)&DAT_00511df0)->Lock();
        *(int *)((char *)param_1 + 0x5f7) = 0;
        *(int *)((char *)param_1 + 0x5eb) = 0;
        f_0041c891(&g4b74f4, 0x927);
    }
}
void f_0044591f(HercModule *self,int actor) {
 self->actor=(ActorRecord *)actor;self->unit=*(ActorRecordCopy **)(actor+0x18);
 self->backup=*self->unit;self->previous=-1;self->selected=self->previous;self->changed=1;
 f_004259f7(self,"DMGGUI.BOX");self->Invalidate();
 DecodeLaunchPalette(self->palette,(byte *)self->guiImages+16);ApplyPaletteData(self->palette);GuiFilterPalette(self->palette);
 for(int i=0;i<256;i++)self->remap[i]=(byte)i;
 f_004465d6((int)self);memset(self->ids,0,34);
 for(i=0;i<23;i++){
  short allowed=(short)HercMountAllowed((ActorUnit *)self->unit,i);HercControl *control=&self->controls[i];if(allowed){self->ids[i+3]=(byte)i;control->SetState(0);}else control->SetState(5);
 }
 self->number.text=(const char *)self->unit;self->number.align=2;
 if(self->buffer)f_0047eb40(self->buffer);
 if(self->file.IsOpen())IORelease(&self->file);
 self->totalBytes=0;self->buffer=0;self->frames=0;
 self->portrait=f_00439d02(self->unit);
 if((unsigned)self->portrait>=(unsigned)*self->hercImages)self->portrait=0;
 if(self->portrait){
  if(gResMgr.FindPath((char *)BriefFormat("DMGHRC%02d.BMX",self->portrait),&self->file,1)){
   self->totalBytes=IoClose(&self->file);self->loaded=0;self->buffer=GlobalBlockAlloc(self->totalBytes,0);

















































































   if(!self->buffer)AssertFail(__FILE__,__LINE__,(const char *)self->buffer);
   memset(self->buffer,0,self->totalBytes);self->frames=(FrameList *)self->buffer;GameTimerSet(&self->timer,0);
   if(next_link_seed(&gFxSeed)%2)self->speed=5;else self->speed=-5;
   self->frame=0;
  }
 }
 viewportClipLeft=gBriefDrawSource;
 SigsStart(&g513888,0);GuiResetClip(&gMenuClip);f_0047bff0(&g513888,self->guiImages+1,0,0);
 f_0047bfa0((int)&g513888,0,1,0,0,g5138ac,DAT_005138b0);
 if(self->portrait){DAT_005138e8=(int)self->remap;f_0047bff0(&g513888,(int *)((byte *)self->hercImages+4+self->portrait*12),260,0);DAT_005138e8=0;}
}

void f_00445c81(int object) {
 if(modeSwitch){
  byte *current=(byte *)(*(int *)(object+0x5c3)+0x11);
  byte *previous=(byte *)(object+0x649);
  for(int i=0;i<23;i++,current+=8,previous+=8){
   if(current[2]!=previous[2]||*(int *)(current+4)!=*(int *)(previous+4))
    modeLogName.Repair(*(ActorRecord **)(object+0x4d),(byte)i,current[2],*(int *)(current+4));
  }
  *(ActorRecordCopy *)(object+0x638)=**(ActorRecordCopy **)(object+0x5c3);
 }
}
void f_00445cfe(int param_1) {
    if (((GuiScreen *)param_1)->IsActive()) {
        f_00445c81(param_1);
        gMenuPrefixStrings.Unlock();
        ReleasePoolData(*(void **)(param_1 + 0x51));
        ReleasePoolData(*(void **)(param_1 + 0x55));
        *(int *)(param_1 + 0x51) = 0;
        *(int *)(param_1 + 0x55) = 0;
        SigsStart(&g513888,0);
        SurfacePresent(&g513888);
        SetGameWindowSize();
        f_004272cd(&g511948,DAT_00513890,SurfaceWidth(),SurfaceHeight());
        f_0042732e(&gMenuSavedSurface,SurfaceWidth(),SurfaceHeight());
        DAT_00513898 = DAT_00511960;
        BriefWaitB(((HercModule *)param_1)->SavedManual());
        BriefWaitA(*(uint *)(param_1+0x5d7));
        if (*(int *)(param_1 + 0x5f7) != 0)
            GlobalBlockFree(*(void **)(param_1 + 0x5f7));
        *(int *)(param_1 + 0x5f7) = 0;
        f_0041caf9(&g4b74f4, 0x927);
        *(int *)(param_1 + 0x10) = 0;
        DAT_004faf44 = 1;
    }
}
int f_00445e23(HercModule *self) {
 if(!self->IsActive())return 0;
 PilotManagerPoll(&g4b74f4);f_00446570((int *)self);
 int oldPoints=self->points;self->points=HercRepairPoints((ActorUnit *)self->unit);
 long hit=self->HitTest(&g4bb78c);
 self->help.id=hit;
 if(hit<3||hit>25)self->selected=-1;else self->selected=self->ids[hit];
 if(hit==26)self->selected=23;
 if(self->HelpPressed()&&self->selected>=0&&self->selected<23){
  HercFormatItem((ActorUnit *)self->unit,PopFindItem((ActorUnit *)self->unit,self->selected,-1),self->selected,0xfffffaff);
  DialogTimedMessage(gDebugText);self->changed=1;
 }
 int damage=gInA0;if(!self->totalBytes)damage=0;
 if(damage||damage!=self->showDamage){self->changed=1;self->Invalidate();}self->showDamage=damage;
 if(self->selected>=0){
  if(!self->showDamage){
   if(self->points){int need=HercRepairNeeded((ActorUnit *)self->unit,self->selected);if(need){
    if(need<100000){if(oldPoints!=self->points)HelpSetDirty(1);self->help.id=self->selected+38;
     char *destination=gShellMiniStrings.GetString(339,0);char *format=targetBriefResource.GetString(338,0);sprintf(destination,format,need,self->points);
    }else self->help.id=self->selected+62;
   }}
  }else self->help.id=36;
 }
 f_00446253((int)self);
 if(gIn7c>=20000&&gIn7c<=20199){ActorRecord *actor=ActorGetByIndex(gIn7c-20000);f_00445c81((int)self);f_0044591f(self,(int)actor);LogicCallMode(6,0);}
 int result=(gIn80==1||gIn80==57);
 switch(self->GuiScreen::Update()){
 case 28:{ActorRecord *a=ActorFindRelated(self->actor);if(a){f_00445c81((int)self);f_0044591f(self,(int)a);LogicCallMode(6,0);}else LogicCallMode(7,0);break;}
 case 29:{ActorRecord *a=ActorFindRelatedOther(self->actor);if(a){f_00445c81((int)self);f_0044591f(self,(int)a);LogicCallMode(6,0);}else LogicCallMode(7,0);break;}
 case 30:result=1;break;
 }
 switch(gIn80){
 case 1:case 57:result=1;break;
 case 75:{int *rate=&self->speed;int step;if(AutoAdvanceEnabled())step=5;else{step=self->speed;if(step<0)step=-step;step=step/5+1;}*rate+=step;break;}
 case 76:self->speed=0;break;
 case 77:{int *rate=&self->speed;int step;if(AutoAdvanceEnabled())step=5;else{step=self->speed;if(step<0)step=-step;step=step/5+1;}*rate-=step;break;}
 }
 int shortcut=SimShortcutMode((void *)self->active);if(shortcut&&shortcut!=5)result=-shortcut;
 int speed=self->speed;if(speed<=-40)speed=-40;else if(speed>=40)speed=40;self->speed=speed;
 return result;
}
void f_00446253(int object) {
 if(*(int *)(object+0x5fb)){
  int loaded=*(int *)(object+0x5ff);
  if(loaded<*(int *)(object+0x5fb)){
   int bytes=*(int *)(object+0x5fb)-loaded;if(bytes>0x1800)bytes=0x1800;
   f_0047e670((void *)(object+0x61c),(void *)(*(int *)(object+0x5f7)+*(int *)(object+0x5ff)),bytes);
   *(int *)(object+0x5ff)+=bytes;
  }else if(f_0047fce0(object+0x610)){
   int speed=*(int *)(object+0x607);
   if(speed<0)f_0047fca0((int *)(object+0x610),-500/speed);
   else if(speed>0)f_0047fca0((int *)(object+0x610),500/speed);
   else f_0047fca0((int *)(object+0x610),100);
   f_00446350(object);
   if(*(int *)(object+0x607)>0){
    int frame=++*(int *)(object+0x603);
    if((unsigned)frame>=**(unsigned **)(object+0x60b))*(int *)(object+0x603)=0;
   }else if(*(int *)(object+0x607)<0){
    int frame=--*(int *)(object+0x603);
    if(frame<0)*(int *)(object+0x603)=**(int **)(object+0x60b)-1;
   }
  }
 }
}
void f_00446350(int object) {
 HercModule *self=(HercModule *)object;
 f_0047bfa0((int)&g513888,1,0,280,0,360,40);f_0047bfa0((int)&g513888,1,0,260,40,380,440);
 MenuSetClip(&gMenuClip,260,0,380,480);DAT_004d1f63&=0xfe;DAT_004d1f8f&=0xfe;self->button.Invalidate();DAT_005138e8=(int)self->remap;
 if(!self->totalBytes||self->loaded<self->totalBytes)f_0047bff0(&g513888,(int *)((byte *)self->hercImages+4+self->portrait*12),260,0);
 else f_0047bff0(&g513888,(int *)((byte *)self->frames+4+self->frame*12),260,0);
 DAT_005138e8=0;
 if(self->points){DAT_005138f8=(int)&DAT_004bc560;DAT_005138f4=DAT_004bc4e8;
 f_0047cfc0(DAT_00511dc0,475,453,0,BriefFormat(targetBriefResource.GetString(337,0),self->points));}
}
int f_00446493(int param_1, int param_2, int *param_3, int *param_4) {
    int local_28, local_24, local_20;
    int local_1c;
    int local_14;
    int iVar6;
    int iVar5;
    byte *base;
    int iVar4;
    f_0043ac02(*(int *)(param_1 + 0x5df), &local_28, &local_24, &local_20, &local_1c);
    local_14=0;iVar6=0;iVar5=0;
    base = (byte *)PopFindItem((ActorUnit *)*(ActorRecordCopy **)(param_1+0x5c3),0,-1);
    for (iVar4 = 0; iVar4 < 0x17; ++iVar4) {
        char *pcVar3 = ((char *)base + 0x1b) + iVar4 * 0x1a;
        if (*pcVar3 != '\0') {
            int iVar2=((int)(byte)pcVar3[local_28+2]*local_20+50)/100+((int)(byte)pcVar3[local_24+2]*local_1c+50)/100;
            iVar6 += iVar2;
            if (iVar4 == param_2) local_14 = iVar2;
            if (iVar2 > iVar5) iVar5 = iVar2;
        }
    }
    *param_3 = iVar5;
    *param_4 = iVar6;
    return (local_14 * 100) / iVar6;
}
void f_00446570(int *param_1) {
    FrameList *list=((HercModule *)param_1)->Frames();
    int iVar2;
    if(!list||!(iVar2=list->Count()))iVar2=40;
    int angle = ((*(int *)((char *)param_1 + 0x603) * 0x168) / iVar2 + 0x1e) % 0x168;
    *(int *)((char *)param_1 + 0x5df) = angle;
    *(uint *)((char *)param_1 + 0x5e3) = GridClamp((uint)angle * 0xb60b61);
}
void f_004465d6(int param_1) {
    int iVar1, iVar2, iVar4;
    int uVar3;
    int iStack_18, local_14, local_10;
    for(iVar4=0;iVar4<=23;iVar4++){
        if (iVar4 == 0x17)
            uVar3 = *(uint *)(param_1 + 0x5f3);
        else
            uVar3 = (int)*(byte *)(*(int *)(param_1 + 0x5c3) + 0x13 + iVar4 * 8);
        if (*(int *)(param_1 + 0x5eb) != 0) {
            if (iVar4 == 0x17)
                uVar3 = 100;
            else {
                iVar1 = f_00446493(param_1, iVar4, &local_14, &iStack_18);
                uVar3=100-(iVar1*100)/16;
                if(uVar3<=0)uVar3=0;
                else if(uVar3>=100)uVar3=100;
            }
        }
        local_10=(100-uVar3)*15/100;
        local_10=local_10*8+16;
        *(char *)(iVar4 + 0x5ab + param_1) = (char)local_10;
        iVar1=*(int *)(param_1+0x5cb);
        if (iVar1 >= 0 && iVar4 != iVar1 && iVar1 < 0x17 && iVar1 != 1)
            local_10 = 0x90;
        for(int j=0;j<6;++j)
            *(char *)(param_1 + 0x4bb + (iVar4*6+j)) = (char)local_10+(char)j;
    }
}
void HercModule::Update() {
 if(IsActive()){
  if(selected!=previous||changed){
   if(changed){changed=0;total=f_00439d48((int)unit);}
   previous=selected;f_004465d6((int)this);f_00446350((int)this);GuiResetClip(&gMenuClip);
  }
  BriefRenderUpdate(this);SimDrawTimer((SimGame *)modeCheckName,g5138ac-75,20);
 }
}
void f_00446781(int param_1) {
    int result = f_00445e23((HercModule *)DAT_004d1fa4);
    if (result != 0) {
        void *puVar1 = *(void **)(DAT_004d1fa4 + 0x4d);
        PilotSetState((SimGame *)param_1, *(int *)(param_1 + 0x1c));
        SimSetFocus((SimGame *)param_1,(ActorRecord *)puVar1);
        if (result < 0) {
            PilotSetState((SimGame *)param_1, -result);
            return;
        }
        return;
    }
    ((HercModule *)DAT_004d1fa4)->Update();
}
