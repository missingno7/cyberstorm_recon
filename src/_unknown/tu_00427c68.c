#include <string.h>
typedef unsigned long U32;
struct GuiRect {long left,top,right,bottom;};
struct Surface {long width,height;void *pixels;U32 flags; void *Pixels(long row=0) { if (row) return (char*)pixels + row*width; return pixels; }};
struct Palette {unsigned char bytes[0x452];};
struct MenuImage {U32 offset;unsigned short width,height;U32 flags;};
struct MenuImages {U32 count;MenuImage images[1];};
struct ResMgr {int OpenFile(long,int=0);int CloseFile();void *Load(long,int=1,int=0,int=0);};
extern ResMgr gResMgr;
extern "C" unsigned long f_c900(ResMgr *,void *,unsigned long);
extern "C" unsigned long f_c8a0(ResMgr *,long,int);
extern void ReleasePoolData(void *);
extern void DecodeLaunchPalette(Palette *,const void *);
extern void MenuDrawImage(void *,const MenuImage *,long,long);
extern char g513888;
extern long g5138ac,g5138b0;
extern GuiRect viewportClipLeft,gBriefDrawSource;
#pragma pack(push,1)
struct BitmapHeader {char signature[2];unsigned long size;unsigned short reserved[2];unsigned long pixels;unsigned long headerSize;long width,height;unsigned short planes;short depth;unsigned long compression,imageSize;long xResolution,yResolution;unsigned long used,important;};
#pragma pack(pop)
int MenuReadBitmap(long resource,Surface *surface,void *palette){
 int error=1;
 BitmapHeader header;
 if(gResMgr.OpenFile(resource,0)){
  if(f_c900(&gResMgr,&header,54)&&header.signature[0]=='B'&&header.signature[1]=='M'&&header.depth==8&&g5138ac==header.width&&g5138b0==header.height){
   if(palette){
    f_c900(&gResMgr,palette,1024);
    for(int i=0;i<256;++i){unsigned char *color=(unsigned char *)palette+i*4;
unsigned char b=color[0];color[0]=color[2];color[2]=b;
}
    memset(palette,0,4);
   }else f_c8a0(&gResMgr,1024,1);
   if(surface)f_c900(&gResMgr,surface->Pixels(),surface->width*surface->height);
   error=0;
  }
  gResMgr.CloseFile();
 }
 return error;
}
int MenuLoadBackground(long resource,Palette *palette){
 MenuImages *data=(MenuImages *)gResMgr.Load(resource,0);
 void *loaded=data;
 if(!data)return 0;
 GuiRect old=viewportClipLeft;
 viewportClipLeft=gBriefDrawSource;
 MenuDrawImage(&g513888,&data->images[0],0,0);
 if(palette&&data->count>1)DecodeLaunchPalette(palette,&data->images[1]);
 viewportClipLeft=old;
 ReleasePoolData(loaded);
 return 1;
}

