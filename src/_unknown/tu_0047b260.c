/* Display backend and palette management. Original source path unknown. */
#define WIN32_LEAN_AND_MEAN
#define _STRING_H_INCLUDED
#include <windows.h>
extern "C" void *memset(void *,int,unsigned long);
extern "C" void *memcpy(void *,const void *,unsigned long);
struct Rect { int left,top,right,bottom; };
struct ClipState { Rect bounds,dirty; int pending,full; };
/* Spare descriptor fields have observed offsets but unknown meanings. */
struct SurfaceDesc { int *rows; int step,width,height,format,planes,count,pitch,bytes,x,y,spare[3]; };
struct DrawStyle { int mode,fill,line; unsigned char *map; };
struct GuiCanvas {
    int open; unsigned char *read,*write,*pages[3],*buffer; SurfaceDesc desc;
    DrawStyle style; int spare[4]; Rect clip,bounds; ClipState dirty;
    GuiCanvas(const SurfaceDesc *); ~GuiCanvas();
};
struct WinGSurface { int open; unsigned char *pixels; HDC dc; int spare; HPALETTE palette; ~WinGSurface(); };
extern SurfaceDesc defaultSurfaceDesc;
GuiCanvas g513888(&defaultSurfaceDesc);
extern WinGSurface *displaySurface;
extern HWND g_mainWindow;
extern HDC g512538;
extern int scanlineStorage[1200];
extern int InitializeWinG(void);
extern WinGSurface *CreateWinGSurface(int,int,HWND);
extern void BlitWinGSurface(WinGSurface *,int,int,int,int,int,int);
extern void SetWinGPalette(WinGSurface *,const void *,unsigned,unsigned);
extern "C" void RegisterAssertCallback(void (*)(void));
#pragma pack(push,1)
struct PaletteCycle { unsigned char first,count,delay,timer,current; };
struct CycleConfig { unsigned char group,delay,mode; };
struct Palette { PALETTEENTRY colors[256]; PaletteCycle cycles[6]; PALETTEENTRY saved[12]; int mode; };
#pragma pack(pop)
extern Palette gGuiPalette;

int SurfaceBackendReady(void) { return InitializeWinG(); }
void SurfaceBackendClose(void)
{
    if(displaySurface) delete displaySurface;
    displaySurface=0;
}
/* 0047b2d0: complete body, parked at a 17-byte register-allocation residue. */
int f_47b2d0(GuiCanvas *s)
{
    if(!InitializeWinG()) return 0;
    if(s->open) SurfaceBackendClose();
    int width=s->desc.width, height=s->desc.height;
    s->desc.pitch=width; s->desc.count=1; s->desc.bytes=width*height;
    displaySurface=CreateWinGSurface(s->desc.width,s->desc.height,g_mainWindow);
    s->write=s->read=s->pages[0]=s->pages[1]=s->pages[2]=s->buffer=displaySurface->pixels;
    memset(scanlineStorage,0,sizeof(scanlineStorage));
    int pitch=s->desc.pitch; s->desc.step=pitch; s->desc.step=-pitch;
    int offset=0;
    for(int row=0;row<s->desc.height;row++) { s->desc.rows[s->desc.height-1-row]=offset; offset+=s->desc.pitch; }
    if(!s->open) RegisterAssertCallback(SurfaceBackendClose);
    s->open=1;
    return 1;
}

void SurfacePresent(void *canvas)
{ GuiCanvas *s=(GuiCanvas *)canvas; BlitWinGSurface(displaySurface,s->desc.x,s->desc.y,s->desc.width,s->desc.height,0,0); }
void f_0047b3d0(GuiCanvas *s,int x,int y,int w,int h)
{ BlitWinGSurface(displaySurface,s->desc.x+x,s->desc.y+y,w,h,x,y); }
void SurfacePaletteReset(void)
{ if(displaySurface) SetWinGPalette(displaySurface,0,0,0); }
void ApplyPaletteData(const PALETTEENTRY *data,unsigned first,int count)
{
    int last=first+count-1;
    if(first<10) first=10;
    if(last>245) last=245;
    count=last-first+1;
    memcpy(gGuiPalette.colors+first,data+first,count*4);
    SetWinGPalette(displaySurface,&gGuiPalette,first,count);
}
/* 0047b4a0: complete body; group comparison and cursor allocation unresolved. */
void PaletteAttach(Palette *p,const CycleConfig *cfg)
{
    memset(p->cycles,0,sizeof(p->cycles));
    unsigned char group=cfg->group;
    int modeSet=0;
    unsigned char count=0,first=0;
    PaletteCycle *cycle=p->cycles;
    for(int i=0;i<6;i++,cycle++,cfg++) {
        if(cfg->mode!=0) {
            p->colors[10+i].peFlags=1;
            if(!modeSet) { modeSet=1; p->mode=cfg->mode; }
        }
        cycle->delay=cycle->timer=cfg->delay;
        cycle->current=i;
        if(p->mode==1) {
            if(group==cfg->group) { cycle->first=first; count++; }
            else {
                for(int n=0;n<count;n++) p->cycles[first+n].count=count;
                count=1; first=i; group=cfg->group; cycle->first=first;
            }
        } else { cycle->first=0; cycle->count=10; }
    }
    if(p->mode==1) for(int n=0;n<count;n++) p->cycles[first+n].count=count;
}
void PaletteBlend(Palette *p,const Palette *source,int unused,unsigned count)
{
    int i;
    for(i=0;i<count;i++) { p->saved[i]=source->colors[i+10]; p->saved[i].peFlags=p->colors[i+10].peFlags; }
    if(p->mode==2) {
        for(i=0;i<count-2;i++) {
            unsigned char index=(10-i)+(count-2);
            p->saved[i+count]=source->colors[index];
            p->saved[i+count].peFlags=p->colors[index].peFlags;
        }
    }
}
/* 0047b650: complete body; loop/block layout and index temporary unresolved. */
void SimPaletteCycle(Palette *p)
{
    unsigned char changed=0;
    PaletteCycle *cycle=p->cycles;
    PALETTEENTRY *color=p->colors;
    do {
        if((int)color[10].peFlags==1) {
            cycle->timer--;
            if((int)cycle->timer<=0) {
                cycle->timer=cycle->delay;
                int end=cycle->first+cycle->count;
                cycle->current++;
                unsigned char current=cycle->current;
                if((int)current>=end) current=cycle->current=cycle->first;
                color[10]=p->saved[current];
                changed=1;
            }
        }
        color++; cycle++;
    } while(color!=p->colors+6);
    if(changed) {
        HPALETTE palette=displaySurface->palette;
        HPALETTE old=SelectPalette(g512538,palette,0);
        AnimatePalette(palette,10,6,p->colors+10);
        RealizePalette(g512538);
        SelectPalette(g512538,old,1);
    }
}


