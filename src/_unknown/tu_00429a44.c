/* Shared range widgets; legacy C bindings BriefModuleInitC/D, BriefSetMode,
 * BriefAdjust and BriefModulePlace are reconstructed as native C++ members. */
typedef unsigned long U32;
struct GuiRect {long x,y,width,height;};struct GuiPoint{long x,y;};
struct GuiControlData:GuiRect{unsigned char style[4];U32 attributes,shortcut;};
class GuiControl:public GuiControlData{public:GuiControl(U32,U32);virtual ~GuiControl();virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);virtual int HitTest(const GuiPoint*);void Clip();int Pressed(){return (attributes&0x20000UL)!=0;}};
struct MenuImage{U32 offset;unsigned short width,height;U32 flags;};struct MenuImages{U32 count;MenuImage images[1];};
extern MenuImages *gMenuImages;extern char g513888;extern GuiRect gMenuClip;
extern GuiPoint g4bb78c;
extern int MenuControlChanged(void *,unsigned,U32 *);
extern void MenuSetClip(GuiRect *,long,long,long,long);
extern void MenuDrawImage(void *,const MenuImage *,long,long);
extern void DrawSprite(void *,const MenuImage *,int,int,int,int,int);
extern int LogicCallMode(int,void *);
struct Timer;extern void TimerSet(Timer *,U32);extern int TimerExpired(Timer *);
struct Timer{U32 start,end,duration;Timer(U32 t){TimerSet(this,t);}};
inline int RangeClamp(int v,int low,int high){if(v<=low)return low;if(v<high)return v;return high;}
struct UpgradeRange{
 int current,maximum;
 UpgradeRange(int,int);
 void Set(int);
 void Resize(int);
 int Count(){return maximum;} int Value(){return current;}
};
class UpgradeBar:public GuiControl{public:
 UpgradeRange *range;int value;
 UpgradeBar(UpgradeRange *,int);virtual ~UpgradeBar(){}virtual void Draw(unsigned);virtual int Event(unsigned,U32);
};
class UpgradeArrow:public GuiControl{public:
 UpgradeRange *range;int step,image;
 UpgradeArrow(UpgradeRange *,int,int);virtual ~UpgradeArrow(){}virtual void Draw(unsigned);virtual int Event(unsigned,U32);
};
UpgradeRange::UpgradeRange(int max,int v):maximum(max){Set(v);}
void UpgradeRange::Set(int v){if(maximum>0)current=RangeClamp(v,0,maximum-1);else current=0;}
void UpgradeRange::Resize(int n){maximum=n;current=0;}
UpgradeBar::UpgradeBar(UpgradeRange *r,int v):GuiControl(12,0),range(r),value(v){}
void UpgradeBar::Draw(unsigned index){
 U32 state=0;MenuControlChanged(this,index,&state);
 long left=x,top=y,w=width,h=height;MenuSetClip(&gMenuClip,left,top,w,h);
 if(height>width){
  MenuDrawImage(&g513888,&gMenuImages->images[60],x,y);
  MenuDrawImage(&g513888,&gMenuImages->images[62],x,y+height-3);
  DrawSprite(&g513888,&gMenuImages->images[61],x,y+3,0,width,height-6);
 }else{
  MenuDrawImage(&g513888,&gMenuImages->images[63],x,y);
  MenuDrawImage(&g513888,&gMenuImages->images[65],x+width-3,y);
  DrawSprite(&g513888,&gMenuImages->images[64],x+3,y,0,width-6,height);
 }
 int image=57;if(Pressed())++image;else if(state&0x80000UL)image+=2;
 MenuImage *p=&gMenuImages->images[image];
 if(range->Count()>1){
  int n=range->Count()-1;
  if(height>width){long yy=(height-width)*range->Value()/n+y;DrawSprite(&g513888,p,x,yy,0,width,width);}
  else {long xx=(width-height)*range->Value()/n+x;DrawSprite(&g513888,p,xx,y,0,height,height);}
 }
}
int UpgradeBar::Event(unsigned,U32){
 if(range->Count()>1){
  int n=range->Count()-1,v;
  if(height>width)v=(g4bb78c.y-((width>>1)+y))*n/(height-width);
  else v=(g4bb78c.x-((height>>1)+x))*n/(width-height);
  range->Set(v);
 }
 return 0;
}
UpgradeArrow::UpgradeArrow(UpgradeRange *r,int n,int i):GuiControl(12,0),range(r),step(n),image(i){}
void UpgradeArrow::Draw(unsigned index){
 U32 state=0;MenuControlChanged(this,index,&state);
 long left=x,top=y,w=width,h=height;MenuSetClip(&gMenuClip,left,top,w,h);
 int n=image;if(Pressed())++n;else if(state&0x80000UL)n+=2;
 DrawSprite(&g513888,&gMenuImages->images[n],x,y,0,width,height);
}
int UpgradeArrow::Event(unsigned,U32 event){
 static Timer repeatTimer(0);
 if(event&4){LogicCallMode(415,0);range->Set(range->Value()+step);TimerSet(&repeatTimer,250);}
 else if(TimerExpired(&repeatTimer)){range->Set(range->Value()+step);TimerSet(&repeatTimer,25);}
 return 0;
}



