/* GUI base and shared drawing helpers. Names are working labels. */
#include <ctype.h>
typedef unsigned long U32;
struct GuiRect {long x,y,width,height;};
struct GuiPoint {long x,y;};
struct ViewRect {long left,top,right,bottom;};
struct GuiControlData:GuiRect {unsigned char style[4];U32 attributes,shortcut;};
class GuiControl:public GuiRect {public: unsigned char style[4];U32 attributes,shortcut;
 GuiControl(U32,U32); virtual ~GuiControl(); virtual void Reset(unsigned); virtual void Draw(unsigned){} virtual int Event(unsigned,U32){return 0;} virtual int HitTest(const GuiPoint *);
};
struct FontData {U32 unknown[3];long height;U32 more[4];unsigned char *widths;};
struct Font {FontData *data;};
struct MenuImage {U32 offset;unsigned short width,height;U32 flags;};
struct MenuImages {U32 count;MenuImage images[1];};
extern MenuImages *gMenuImages;
extern Font *gMenuFonts[5];
extern U32 gMenuColorMap[30],gMenuColors[30],gMenuTextMode,gMenuTextColor;
extern U32 *gMenuTextMap;
extern U32 gIn80,gIn84;
extern unsigned gGuiFocusedControl;
extern long gGuiHoveredControl;
extern char g513888;
extern ViewRect viewportClipLeft;
extern U32 menuClearColor;
extern void MenuSetColors(void *,U32,long);
extern void MenuFill(void *,long,long,long,long);
extern void DrawSprite(void *,const MenuImage *,int,int,int,int,int);
extern void MenuDrawImage(void *,const MenuImage *,long,long);
extern void MenuDrawText(Font *,long,long,const char *);
extern void SimAlertPrepare(void *,const GuiRect *);
GuiControl::GuiControl(U32 a,U32 key):attributes(a),shortcut(key) {}
GuiControl::~GuiControl() {}
int GuiControlStyle(GuiControl *c,unsigned fallback,U32 state) {
 int n=3;
 if(state&4)n=0;
 else if(state&8)n=1;
 else if(state&1)n=2;
 if(c->style[n])return c->style[n]-1;return fallback;
}
int MenuControlChanged(GuiControl *c,unsigned index,U32 *previous) {
 int changed=0;
 if(gGuiHoveredControl==index)c->attributes|=0x80000UL;else c->attributes&=~0x80000UL;
 if(gGuiFocusedControl==index)c->attributes|=0x100000UL;else c->attributes&=~0x100000UL;
 if(!(c->attributes&0x1000000UL))changed=1;
 if((c->attributes&0x7eff0000UL)!=(*previous&0x7eff0000UL))changed=1;
 if(c->attributes&0x800000UL)changed=0;
 c->attributes|=0x1000000UL;
 *previous=c->attributes;
 return changed;
}
inline U32 MenuScan(){return gIn80;}inline U32 MenuChar(){return gIn84;}
int GuiControlShortcut(GuiControl *c) {
 if(c->shortcut&&MenuScan()) {
  U32 scan=(c->shortcut>>8)&255;
  U32 key=c->shortcut&255;
  if(key&&key==MenuScan())return 1;
  if(scan&&scan==toupper(MenuChar()))return 1;
 }
 return 0;
}
void GuiControlSetShortcut(GuiControl *c,const char *s) {
 if(c->shortcut==0&&s&&(signed char)*s=='~') {
  ++s;
  if(*s&128)c->shortcut=(signed char)(*s&127);
  else c->shortcut=toupper((signed char)*s)<<8;
 }
}
void MenuClearRect(const GuiRect *r) {
 MenuSetColors(&g513888,menuClearColor,-1);
 MenuFill(&g513888,r->x,r->y,r->width,r->height);
}
void MenuDrawWrapped(const char *text,long ax,long ay,long font,long color,const GuiRect *r,int dx,int dy) {
 if(gMenuFonts[font]&&text&&(signed char)*text!=0) {
  Font *f=gMenuFonts[font];
  if((signed char)*text=='~') {
   do {++text;}while(*text&&(signed char)*text!='~');
   do {++text;}while((signed char)*text==' ');
  }
  long w=0;
  long h=f->data->height;
  gMenuTextMode=1;
  const char *s=text;
  for(;*s;++s){unsigned char c=*s;w+=f->data->widths[c];}
  long x=r->x+dx,y=r->y+dy;
  if(ax==-1)x+=4;
  if(ax==0)x+=(r->width-w)/2;
  if(ax==1)x+=r->width-w-4;
  if(ay==-1)++y;
  if(ay==0)y+=(r->height-h)/2;
  if(ay==1)y+=r->height-y-1;
  gMenuTextMap=&gMenuColorMap[color];
  gMenuTextColor=gMenuColors[color];
  MenuDrawText(f,x,y,text);
 }
}
void MenuDrawPanel(const MenuImage *images,const GuiRect *r) {
 long y=r->y;
 long right=r->x+r->width-1;
 long middle=r->height-images[0].height-images[6].height;
 for(int i=0;i<3;++i) {
  const MenuImage *left=images+i*3;
  const MenuImage *center=left+1;
  const MenuImage *last=left+2;
  long h=i==1?middle:left->height;
  DrawSprite(&g513888,left,r->x,y,0,left->width,h);
  DrawSprite(&g513888,center,r->x+left->width,y,0,r->width-left->width-last->width,h);
  DrawSprite(&g513888,last,right-last->width,y,0,last->width,h);
  y+=h;
 }
}
void MenuStretchImage(long index,const GuiRect *r) {
 MenuImages *images=gMenuImages;
 if(images&&index>=0&&(U32)index<images->count) {
  MenuImage *p=&images->images[index];
  DrawSprite(&g513888,p,r->x,r->y,0,r->width,r->height);
 }
}
void MenuCenterImage(long index,const GuiRect *r) {
 MenuImages *images=gMenuImages;
 if(images&&index>=0&&(U32)index<images->count) {
  MenuImage *p=&images->images[index];
  long x=r->x+(r->width-p->width)/2;
  long y=r->y+(r->height-p->height)/2;
  MenuDrawImage(&g513888,p,x,y);
 }
}
void MenuTileImage(long index,const GuiRect *r) {
 if(gMenuImages&&index>=0&&(U32)index<gMenuImages->count) {
  MenuImage *p=&gMenuImages->images[index];
  ViewRect old=viewportClipLeft;
  SimAlertPrepare(&g513888,r);
  for(long x=0;x<r->width;x+=p->width)
   for(long y=0;y<r->height;y+=p->height)MenuDrawImage(&g513888,p,r->x+x,r->y+y);
  viewportClipLeft=old;
 }
}

