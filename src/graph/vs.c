/* Drawing surface object, unknown original source path. Names and spare fields are provisional. */
extern "C" void *memset(void *,int,unsigned long);
extern "C" void *memcpy(void *,const void *,unsigned long);
struct Rect { int left,top,right,bottom; };
struct GuiRect { int x,y,width,height; };
struct ClipState { Rect bounds,dirty; int pending,full; };
/* Spare descriptor/style fields have observed offsets but unknown meanings. */
struct SurfaceDesc {
    int *rows; int step,width,height,format,planes,count,pitch,bytes,x,y,spare[3];
};
struct DrawStyle { int mode,fill,line; unsigned char *map; };
struct MenuImage { int data; unsigned short width,height; unsigned char flags,encoding; unsigned short spare; };
struct GuiCanvas {
    int open; unsigned char *read,*write,*pages[3],*buffer;
    SurfaceDesc desc;
    DrawStyle style; int spare[4];
    Rect clip,bounds; ClipState dirty;
    GuiCanvas(const SurfaceDesc *);
    ~GuiCanvas();
};
extern void ClipInitialize(ClipState *,const Rect *);
extern void GuiResetClip(ClipState *);
extern void SimClipRestore(ClipState *), SimClipFlush(ClipState *);
extern void SurfacePresent(void *);
extern int useDirtyRects;
extern GuiCanvas g513888;
extern MenuImage decodedImage;
struct DecodeBuffer { int size; unsigned char *data; };
extern DecodeBuffer imageDecodeBuffer;
extern void DecodeImage(unsigned char,const void *,DecodeBuffer *);
extern void RenderImage(const MenuImage *,int,int,int,int,int);
extern void RenderSpecialImage(const MenuImage *,int,int);
extern void RenderFill(int,int,int,int);
extern void RenderVertical(int,int,int),RenderHorizontal(int,int,int),RenderLine(int,int,int,int);
extern int RenderClipLine(int *,int *,int *,int *,const Rect *);
extern void RenderCopy(const unsigned char *,unsigned char *,int,int,int,int);
struct ResMgr { void *Load(const char *,int=1,void * =0,unsigned long=0); };
extern ResMgr gResMgr;
extern void ReleasePoolData(void *);

void f_47b7c0(GuiCanvas *,const SurfaceDesc *);
GuiCanvas::GuiCanvas(const SurfaceDesc *d)
{
    memset(this,0,sizeof(*this));
    f_47b7c0(this,d);
}
GuiCanvas::~GuiCanvas() {}
void f_47b7c0(GuiCanvas *s,const SurfaceDesc *d)
{
    if (d) s->desc=*d;
    s->bounds.left=0; s->bounds.right=s->desc.width-1;
    s->bounds.top=0; s->bounds.bottom=s->desc.height-1;
    s->clip=s->bounds;
    ClipInitialize(&s->dirty,&s->bounds);
    GuiResetClip(&s->dirty);
}
int SurfaceGetPixel(GuiCanvas *s,int x,int y)
{
    if(x<s->clip.left || x>s->clip.right || y<s->clip.top || y>s->clip.bottom) return 0;
    return s->read[s->desc.rows[y]+x];
}
void SurfaceSetPixel(GuiCanvas *s,int x,int y,unsigned char color)
{
    if(x<s->clip.left || x>s->clip.right || y<s->clip.top || y>s->clip.bottom) return;
    s->write[s->desc.rows[y]+x]=color;
}
unsigned char *SurfacePixelAddress(GuiCanvas *s,int x,int y)
{
    if(x<s->clip.left) x=s->clip.left;
    if(y<s->clip.top) y=s->clip.top;
    if(x>s->clip.right) x=s->clip.right;
    if(y>s->clip.bottom) y=s->clip.bottom;
    return s->write+(x+s->desc.rows[y]);
}
void SurfaceSetStyle(GuiCanvas *s,const DrawStyle *style) { s->style=*style; }
void MenuSetColors(GuiCanvas *s,int line,int fill) { s->style.line=line; s->style.fill=fill; }
void SurfaceSetClip(GuiCanvas *s,const Rect *r)
{
    s->clip=*r;
    if(s->clip.left<s->bounds.left) s->clip.left=s->bounds.left;
    if(s->clip.right>s->bounds.right) s->clip.right=s->bounds.right;
    if(s->clip.top<s->bounds.top) s->clip.top=s->bounds.top;
    if(s->clip.bottom>s->bounds.bottom) s->clip.bottom=s->bounds.bottom;
}
void SimAlertPrepare(GuiCanvas *s,const GuiRect *r)
{
    s->clip.left=r->x; s->clip.right=r->x+r->width-1;
    s->clip.top=r->y; s->clip.bottom=r->y+r->height-1;
    if(s->clip.left<s->bounds.left) s->clip.left=s->bounds.left;
    if(s->clip.right>s->bounds.right) s->clip.right=s->bounds.right;
    if(s->clip.top<s->bounds.top) s->clip.top=s->bounds.top;
    if(s->clip.bottom>s->bounds.bottom) s->clip.bottom=s->bounds.bottom;
}
void SurfaceSetClipSize(GuiCanvas *s,int x,int y,int w,int h)
{
    s->clip.left=x; s->clip.right=x+w-1;
    s->clip.top=y; s->clip.bottom=y+h-1;
    if(s->clip.left<s->bounds.left) s->clip.left=s->bounds.left;
    if(s->clip.right>s->bounds.right) s->clip.right=s->bounds.right;
    if(s->clip.top<s->bounds.top) s->clip.top=s->bounds.top;
    if(s->clip.bottom>s->bounds.bottom) s->clip.bottom=s->bounds.bottom;
}

void SigsStart(GuiCanvas *s,unsigned char color) { memset(s->write,color,s->desc.bytes); }
void MenuLine(GuiCanvas *,int,int,int,int);
/* 0047bad0: complete body; border temporary allocation unresolved. */
void MenuFill(GuiCanvas *s,int x,int y,int w,int h)
{
    {
    int right=x+w-1,bottom=y+h-1,left=x,top=y;
    if(w<=0 || h<=0 || x>s->clip.right || y>s->clip.bottom || right<s->clip.left || bottom<s->clip.top) return;
    if(left<s->clip.left) left=s->clip.left;
    if(left>s->clip.right) left=s->clip.right;
    if(right<s->clip.left) right=s->clip.left;
    if(right>s->clip.right) right=s->clip.right;
    if(top<s->clip.top) top=s->clip.top;
    if(top>s->clip.bottom) top=s->clip.bottom;
    if(bottom<s->clip.top) bottom=s->clip.top;
    if(bottom>s->clip.bottom) bottom=s->clip.bottom;
    bottom=bottom-top+1; right=right-left+1;
    if(s->style.fill!=-1) RenderFill(left,top,right,bottom);
    }
    if(s->style.fill==-1 || s->style.fill!=s->style.line) {
        int right=x+w-1;
        MenuLine(s,x,y,right,y); int bottom=y+h-1; MenuLine(s,right,y,right,bottom);
        MenuLine(s,right,bottom,x,bottom); MenuLine(s,x,bottom,x,y);
    }
}

/* 0047bc10: complete body; loop shape and shared preceding epilogue unresolved. */
void SurfaceRemapRect(GuiCanvas *s,int x,int y,int w,unsigned h)
{
    unsigned char *map=s->style.map;
    unsigned row=0;
    if(h==0) return;
    int yy=y;
    do {
        unsigned char *p=SurfacePixelAddress(s,x,yy);
        int n=w;
        for(;;) {
            if(n<=0) break;
            *p=map[*p];
            p++;
            n--;
        }
        row++;
        yy++;
    } while(row<h);
}

void MenuLine(GuiCanvas *s,int x1,int y1,int x2,int y2)
{
    if(x1==x2) {
        if(y2<y1) { int t=y1; y1=y2; y2=t; }
        if(x1<s->clip.left || x1>s->clip.right || y2<s->clip.top || y1>s->clip.bottom) return;
        if(y1<s->clip.top) y1=s->clip.top;
        if(y2>s->clip.bottom) y2=s->clip.bottom;
        RenderVertical(x1,y1,y2-y1+1);
    } else if(y1==y2) {
        if(x2<x1) { int t=x1; x1=x2; x2=t; }
        if(y1<s->clip.top || y1>s->clip.bottom || x2<s->clip.left || x1>s->clip.right) return;
        if(x1<s->clip.left) x1=s->clip.left;
        if(x2>s->clip.right) x2=s->clip.right;
        RenderHorizontal(x1,y1,x2-x1+1);
    } else {
        if(RenderClipLine(&x1,&y1,&x2,&y2,&s->clip)) RenderLine(x1,y1,x2,y2);
    }
}
int SurfaceArea(GuiCanvas *,int w,int h) { return w*h; }
void SurfaceReadRect(GuiCanvas *s,unsigned char *pixels,int x,int y,unsigned w,unsigned h)
{
    unsigned char *p=pixels;
    for(unsigned row=0;row<h;row++) {
        memcpy(p,s->read+s->desc.rows[y+row]+x,w);
        p+=w;
    }
}

void SurfaceWriteRect(GuiCanvas *s,const unsigned char *pixels,int x,int y,unsigned w,unsigned h)
{
    const unsigned char *p=pixels;
    for(unsigned row=0;row<h;row++) {
        memcpy(s->write+s->desc.rows[y+row]+x,p,w);
        p+=w;
    }
}

void BriefBlit(GuiCanvas *s,int from,int to,int x,int y,int dx,int dy,unsigned w,unsigned h)
{
    if(!(x&3) && !(w&3)) { RenderCopy(s->pages[from],s->pages[to],x,y,w,h); return; }
    unsigned char *source=s->pages[from]+s->desc.rows[y]+x;
    unsigned char *dest=s->pages[to]+s->desc.rows[dy]+dx;
    int step=s->desc.step;
    for(unsigned row=0;row<h;row++) { memcpy(dest,source,w); source+=step; dest+=step; }
}
void SurfaceCopyAligned(GuiCanvas *s,int from,int to,int x,int y,int dx,int dy,int w,int h)
{ w+=((x&3)+3); w&=~3; x&=~3; RenderCopy(s->pages[from],s->pages[to],x,y,w,h); }
void PopDrawSurface(GuiCanvas *s,int from,int to,int x,int y,int w,int h)
{ w+=((x&3)+3); w&=~3; x&=~3; RenderCopy(s->pages[from],s->pages[to],x,y,w,h); }
void DrawSprite(GuiCanvas *,const MenuImage *,int,int,int,int,int);
void MenuDrawImage(GuiCanvas *s,const MenuImage *image,int x,int y)
{
    if((image->encoding&15)==13) {
        unsigned char type=image->encoding>>4;
        if(type) {
            MenuImage local=*image;
            DecodeImage(type,(char *)image+local.data,&imageDecodeBuffer);
            local.data=imageDecodeBuffer.data-(unsigned char *)&local;
            RenderSpecialImage(&local,x,y);
        } else RenderSpecialImage(image,x,y);
    } else {
        DrawSprite(s,image,x,y,0,image->width,image->height);
    }
}
void DrawSprite(GuiCanvas *,const MenuImage *image,int x,int y,int mode,int w,int h)
{
    if(image->encoding) {
        decodedImage=*image;
        DecodeImage(decodedImage.encoding,(char *)image+decodedImage.data,&imageDecodeBuffer);
        decodedImage.data=imageDecodeBuffer.data-(unsigned char *)&decodedImage;
        image=&decodedImage;
    }
    RenderImage(image,x,y,mode,w,h);
}
MenuImage *MenuImageData(GuiCanvas *,const MenuImage *image)
{
    decodedImage=*image;
    DecodeImage(decodedImage.encoding,(char *)image+decodedImage.data,&imageDecodeBuffer);
    decodedImage.data=imageDecodeBuffer.data-(unsigned char *)&decodedImage;
    return &decodedImage;
}
int SurfaceLoadBackground(GuiCanvas *s,const char *name)
{
    int *data=(int *)gResMgr.Load(name,0);
    if(!data) return 0;
    Rect saved=s->clip;
    s->clip=s->bounds;
    MenuDrawImage(&g513888,(MenuImage *)(data+1),0,0);
    s->clip=saved;
    ReleasePoolData(data);
    return 1;
}
void SurfaceFlush(GuiCanvas *s)
{
    if(!useDirtyRects) { SurfacePresent(s); return; }
    SimClipRestore(&s->dirty); SimClipFlush(&s->dirty);
}

static char xSourceFile[] = __FILE__;
