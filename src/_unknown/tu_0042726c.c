/* Surface storage. Ends at 004275ba; the polygon tables at 004275bc
 * belong to the next object. Original path unknown. */
#include <string.h>
typedef unsigned long U32;
struct GuiRect {long x,y,width,height;GuiRect(long w,long h,long left=0,long top=0):x(left),y(top),width(w),height(h){}};
struct GuiPoint {long x,y;};
struct MenuImage {U32 offset;unsigned short width,height;U32 flags;};
extern GuiRect gMenuClip;
extern long g5138ac,g5138b0;
extern void GuiResetClip(void *);
extern void *GlobalBlockFree(void *);
extern void *GlobalBlockAlloc(U32,unsigned=0);
class Surface {public:
 long width,height; signed char *pixels;U32 flags;
 Surface(U32);~Surface();
 void FreeCore(){if(pixels){if(flags&2)flags&=~2UL;else GlobalBlockFree(pixels);}height=0;width=height;pixels=0;}
};
Surface::Surface(U32 f):flags(f){height=0;width=height;pixels=0;}
Surface::~Surface(){FreeCore();}
int SurfaceAttach(Surface *s,void *pixels,long w,long h){
 s->FreeCore();s->pixels=(signed char *)pixels;
 if(pixels){s->flags|=2;s->width=w;s->height=h;return 1;}return 0;
}
int SurfaceCreate(Surface *s,long w,long h){
 s->FreeCore();long bytes=w*h;s->pixels=(signed char *)GlobalBlockAlloc(bytes);
 if(s->pixels){s->width=w;s->height=h;return 1;}return 0;
}
void SurfaceFree(Surface *s){if(s->pixels){if(s->flags&2)s->flags&=~2UL;else GlobalBlockFree(s->pixels);}s->height=0;s->width=s->height;s->pixels=0;}
int SurfaceGetPixel(Surface *s,long x,long y){
 if(s->pixels){long row=(s->flags&1)?s->height-y-1:y;return s->pixels[row*s->width+x];}return 0;
}
signed char *SurfaceGetRow(Surface *s,long y){
 if(s->pixels){long row=(s->flags&1)?s->height-y-1:y;return s->pixels+row*s->width;}return 0;
}
void SurfaceClear(Surface *s){
 if(s->pixels)memset(s->pixels,0,s->width*s->height);GuiResetClip(&gMenuClip);
}
int MenuRestore(Surface *dst,Surface *src,const GuiRect &r,long x,long y){
 GuiResetClip(&gMenuClip);
 int result;
 if(dst->pixels&&r.width>0&&r.height>0&&x+r.width<=dst->width&&y+r.height<=dst->height){
  long sy=(src->flags&1)?src->height-r.y-1:r.y;
  signed char *sp;{long sourceOffset=sy*src->width;sp=sourceOffset+(src->pixels+r.x);}
  long srcStep=(src->flags&1)?-src->width:src->width;
  long dy=(dst->flags&1)?dst->height-y-1:y;
  long destOffset=dst->width*dy;signed char *dp=dst->pixels+x+destOffset;
  long dstStep=(dst->flags&1)?-dst->width:dst->width;
  long rows=r.height;
  while(rows--){memcpy(dp,sp,r.width);sp+=srcStep;dp+=dstStep;}
  result=1;
 }else result=0;
 return result;
}
void SurfaceCopyRegion(Surface *dst,Surface *src,long x,long y){
 MenuRestore(dst,src,GuiRect(src->width,src->height),x,y);
}
extern "C" void SurfaceGetFrame(Surface *s,MenuImage *frame){
 memset(frame,0,12);frame->offset=(char *)s->pixels-(char *)frame;frame->width=s->width;frame->height=s->height;
}


