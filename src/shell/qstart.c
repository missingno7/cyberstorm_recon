/* Quick-start screen; names and unobserved layout fields are provisional. */
typedef unsigned long U32;
struct GuiPoint {long x,y;}; struct GuiControl; struct InputReq;
struct StrPool {char *GetString(long,int=1); char *GetSecondString(long,int=1);};
struct GuiScreen {char first[16]; InputReq *input; char rest[0x38];
 GuiScreen(GuiControl **,unsigned,U32); ~GuiScreen(){input=0;}
 void LoadLayout(long); void Open(InputReq *,StrPool *,long,int); void Invalidate(); int Update(); void Draw(); long HitTest(const GuiPoint *);
 int IsActive(){return input!=0;}
};
struct GuiControl {char first[20]; U32 flags,shortcut; virtual ~GuiControl(); void SetVisibility(U32 n){flags=(flags&0xff1fffffUL)+(n<<21);}};
struct MenuButton:GuiControl {char rest[8]; MenuButton(const char *,U32); virtual ~MenuButton();};
struct MenuNumber:GuiControl {char rest[16]; MenuNumber(signed char,signed char); virtual ~MenuNumber(); void SetFont(signed char);};
struct MenuPanel:GuiControl {char rest[4]; MenuPanel(); virtual ~MenuPanel();};
struct MenuHelpRegion:GuiControl {int id,activated; MenuHelpRegion(); virtual ~MenuHelpRegion();};
extern "C" void BriefModuleInitC(void *,int,int);
extern "C" void BriefModuleInitD(void *,void *,int);
extern "C" void BriefModulePlace(void *,void *,int,int);
#pragma aux quick_selection_ctor "BriefModuleInitC"
#pragma aux quick_bar_ctor "BriefModuleInitD"
#pragma aux quick_arrow_ctor "BriefModulePlace"
struct QuickSelection {U32 current; int maximum; __pragma("quick_selection_ctor") QuickSelection(int,int=0); U32 Get(){return current&255;}};
struct UpgradeBar:GuiControl {char rest[8]; __pragma("quick_bar_ctor") UpgradeBar(QuickSelection *,int=0); virtual ~UpgradeBar();};
struct UpgradeArrow:GuiControl {char rest[12]; __pragma("quick_arrow_ctor") UpgradeArrow(QuickSelection *,int,int); virtual ~UpgradeArrow();};
extern GuiControl *quickControls[22];
typedef struct QuickStartScreen Scene;
struct QuickStartScreen:GuiScreen {
 char unknown4c[4]; int options[4]; char unknown60[4];
 int x[4],y[4],width[4],height[4],textBase[4]; char unknownb4[16];
 int changed[4]; char unknownd4[4]; void *pool; int selected[4],previous[4];
 QuickStartScreen();
};
MenuButton quickBack(0,0),quickRandom(0,0),quickLaunch(0,0);
MenuNumber quickNumber0(-1,18),quickNumber1(-1,18),quickNumber2(-1,18),quickNumber3(-1,18),quickNumber4(-1,18);
MenuPanel quickPanel; MenuButton quickConfirm(0,0); MenuHelpRegion quickHelp;
QuickSelection quickChoice0(3); UpgradeBar quickBar0(&quickChoice0); UpgradeArrow quickNext0(&quickChoice0,1,54),quickPrev0(&quickChoice0,-1,51);
QuickSelection quickChoice1(23); UpgradeBar quickBar1(&quickChoice1); UpgradeArrow quickNext1(&quickChoice1,1,54),quickPrev1(&quickChoice1,-1,51);
QuickSelection quickChoice2(14); UpgradeBar quickBar2(&quickChoice2); UpgradeArrow quickNext2(&quickChoice2,1,54),quickPrev2(&quickChoice2,-1,51);
QuickSelection quickChoice3(3); UpgradeBar quickBar3(&quickChoice3); UpgradeArrow quickNext3(&quickChoice3,1,54),quickPrev3(&quickChoice3,-1,51);
QuickStartScreen g4b7904;
static char qstartSourceFile[]=__FILE__;
GuiControl *quickControls[22]={
 (GuiControl *)&quickBack,(GuiControl *)&quickRandom,(GuiControl *)&quickLaunch,
 (GuiControl *)&quickNumber1,(GuiControl *)&quickNumber2,(GuiControl *)&quickNumber3,(GuiControl *)&quickNumber4,
 (GuiControl *)&quickPanel,(GuiControl *)&quickPrev0,(GuiControl *)&quickBar0,(GuiControl *)&quickNext0,
 (GuiControl *)&quickPrev1,(GuiControl *)&quickBar1,(GuiControl *)&quickNext1,
 (GuiControl *)&quickPrev2,(GuiControl *)&quickBar2,(GuiControl *)&quickNext2,
 (GuiControl *)&quickPrev3,(GuiControl *)&quickBar3,(GuiControl *)&quickNext3,
 (GuiControl *)&quickConfirm,(GuiControl *)&quickHelp};
QuickStartScreen::QuickStartScreen():GuiScreen(quickControls,22,0) {}
struct ViewRect {long left,top,right,bottom;};
extern ViewRect viewportClipLeft,gBriefDrawSource;
extern StrPool gShellStrings,quickStrings;
extern GuiPoint g4bb78c;
extern char gShellBackground[],g511948,gMenuSavedSurface,g513888;
extern void *gShellImages;
struct MenuImage {U32 offset;unsigned short width,height;U32 flags;};
void ShellClearFrame(); void GuiFadeOut(); void SurfacePresent(void *);
void MenuDrawImage(void *,const MenuImage *,long,long);
void SurfaceCopyRegion(void *,void *,int,int); void ShellApplyPalette(void *);
void DecodeLaunchPalette(void *,void *); void ReleasePoolData(void *);
struct ResMgr {void *Load(long,int=1,int=0,int=0);}; extern ResMgr gResMgr;
extern "C" char *BriefFormat(const char *,...);
extern "C" void BriefWaitB(int);
extern "C" void BriefBlit(void *,unsigned long,unsigned long,unsigned long,unsigned long,unsigned long,unsigned long,unsigned long,unsigned long);
extern "C" void BriefRender(int,int,int,int,const char *,int,int,int,int *,int *,int,int,int=1,int=0,int=0,int=0x7fffffff);
void SoundClearContext(); int SoundTrigger(unsigned,unsigned);
extern int MissionRandomTable[]; void f_0047e200(int *); extern "C" unsigned next_link_seed(int *);
extern "C" void BriefSetMode(void *,int);
void QuickDrawOptions(Scene *);

void ShellBriefingOpen(Scene *s,InputReq *input) {
 if(s->IsActive()) return;
 viewportClipLeft=gBriefDrawSource;
 ShellClearFrame(); quickHelp.id=-1;
 s->LoadLayout((long)"qstart.box"); s->Open(input,&gShellStrings,0xd49,1);
 BriefWaitB(7); s->pool=0;
 char *name=BriefFormat("qstart.bmx"); s->pool=gResMgr.Load((long)name);
 DecodeLaunchPalette(gShellBackground,(char *)s->pool+16); ShellApplyPalette(gShellBackground);
 MenuDrawImage(&g513888,(MenuImage *)((char *)s->pool+4),0,0);
 SurfaceCopyRegion(&gMenuSavedSurface,&g511948,0,0);
 quickNumber1.SetFont(0); quickNumber2.SetFont(0); quickNumber3.SetFont(0); quickNumber4.SetFont(0);
 s->x[0]=39; s->y[0]=50; s->width[0]=244; s->height[0]=52;
 s->x[3]=348; s->y[3]=50; s->width[3]=244; s->height[3]=50;
 s->x[1]=39; s->y[1]=190; s->width[1]=244; s->height[1]=100;
 s->x[2]=348; s->y[2]=190; s->width[2]=244; s->height[2]=180;
 *(int *)s->unknownb4=1;
 s->textBase[0]=36; s->textBase[1]=40; s->textBase[2]=16; s->textBase[3]=31;
 for(int i=0;i<4;i++){s->changed[i]=0;s->previous[i]=99;}
 quickConfirm.SetVisibility(5);
}
void ShellBriefingClose(Scene *s) {
 if(s->IsActive()){ReleasePoolData(s->pool); GuiFadeOut(); ShellClearFrame(); s->input=0;}
}
void ShellBriefingExit(Scene *s) {
 int changed=0;
 if(s->IsActive()) {
 s->selected[0]=quickChoice0.Get(); s->selected[1]=quickChoice1.Get(); s->selected[2]=quickChoice2.Get(); s->selected[3]=quickChoice3.Get();
 for(int i=0;i<4;i++) {
  if(s->selected[i]!=s->previous[i]) {s->changed[i]=1;s->previous[i]=s->selected[i];changed=1;} else s->changed[i]=0;
 }
 if(changed){s->options[0]=s->selected[0]+1; s->options[1]=s->selected[1];s->options[2]=s->selected[2];s->options[3]=s->selected[3]-1;}
 quickHelp.id=s->HitTest(&g4bb78c);
 s->Update();
 }

}
void ShellBriefingRender(Scene *s){if(s->IsActive()){QuickDrawOptions(s);s->Draw();}}
void ShellBriefingBack(Scene *) {
 f_0047e200(MissionRandomTable);
 unsigned a=next_link_seed(MissionRandomTable)%3;
 unsigned b=next_link_seed(MissionRandomTable)%23;
 unsigned c=next_link_seed(MissionRandomTable)%14;
 unsigned d=next_link_seed(MissionRandomTable)%3;
 BriefSetMode(&quickChoice0,a);BriefSetMode(&quickChoice1,b);BriefSetMode(&quickChoice2,c);BriefSetMode(&quickChoice3,d);
}
void QuickDrawOptions(Scene *s) {
 for(unsigned char i=0;i<4;i++) {
  if(s->changed[i]) {
   if(i==1) {SoundClearContext();SoundTrigger(0,1000);SoundTrigger(s->selected[i]+318,0);
    MenuDrawImage(&g513888,(MenuImage *)((char *)s->pool+4)+(s->selected[i]+2),120,292);SurfacePresent(&g513888);}
   BriefBlit(&g513888,1,0,s->x[i],s->y[i],s->x[i],s->y[i],s->width[i],s->height[i]);
   const char *first=quickStrings.GetString(s->textBase[i]+s->selected[i],1);
   const char *second=quickStrings.GetSecondString(s->textBase[i]+s->selected[i],1);
   int begin,end;
   BriefRender(s->x[i],s->y[i],s->width[i],s->height[i],first,0,-1,0,&begin,&end,0,15);
   BriefRender(s->x[i],s->y[i]+14,s->width[i],s->height[i]-14,second,0,-1,0,&begin,&end,0,2,0);
  }
 }
}
void QuickShowMessage(Scene *,const char *);
void ShellBriefingContinue(Scene *s) {
 quickBack.SetVisibility(5);quickRandom.SetVisibility(5);quickLaunch.SetVisibility(5);
 quickPrev0.SetVisibility(5);quickBar0.SetVisibility(5);quickNext0.SetVisibility(5);
 quickPrev1.SetVisibility(5);quickBar1.SetVisibility(5);quickNext1.SetVisibility(5);
 quickPrev2.SetVisibility(5);quickBar2.SetVisibility(5);quickNext2.SetVisibility(5);
 quickPrev3.SetVisibility(5);quickBar3.SetVisibility(5);quickNext3.SetVisibility(5);
 quickConfirm.flags&=0xfe1fffffUL;
 QuickShowMessage(s," Unable to load mission file. ");
}
void QuickShowMessage(Scene *,const char *text) {
 MenuDrawImage(&g513888,(MenuImage *)((char *)gShellImages+0x31c),200,150);SurfacePresent(&g513888);
 int begin,end; BriefRender(220,165,170,75,text,0,-1,0,&begin,&end,0,15);
}
void QuickDrawText(Scene *,const char *text) {
 int begin,end; BriefRender(220,165,170,75,text,0,-1,0,&begin,&end,0,15);
}
void ShellBriefingAbort(Scene *s) {
 quickConfirm.SetVisibility(5);
 quickBack.SetVisibility(0);quickRandom.SetVisibility(0);quickLaunch.SetVisibility(0);
 quickPrev0.SetVisibility(0);quickBar0.SetVisibility(0);quickNext0.SetVisibility(0);
 quickPrev1.SetVisibility(0);quickBar1.SetVisibility(0);quickNext1.SetVisibility(0);
 quickPrev2.SetVisibility(0);quickBar2.SetVisibility(0);quickNext2.SetVisibility(0);
 quickPrev3.SetVisibility(0);quickBar3.SetVisibility(0);quickNext3.SetVisibility(0);
 s->Invalidate();BriefBlit(&g513888,1,0,200,150,200,150,250,150);
 for(int i=0;i<4;i++){s->changed[i]=0;s->previous[i]=99;}
}




