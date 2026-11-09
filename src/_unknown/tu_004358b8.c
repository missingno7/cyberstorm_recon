/* Separate assembly-origin raster object, 004358b8..00435b5c.
 * Full C++ reconstruction; the original ENTER frames remain unmatched. */
typedef unsigned char U8;typedef unsigned long U32;
struct SpanState {int integerStep;U32 fractionStep;int skip;U8 *destination,*source;int width,step;};
extern int viewportClipLeft,viewportClipTop,viewportClipRight,viewportClipBottom;
extern U8 *gShellSurfacePixels;
extern int *surfaceRowOffsets;extern int surfaceStride;
extern unsigned long gMenuBackground;
inline U8 SampleScaled(U8 *&source,U32 &fraction,int integer,U32 step) {
    U8 pixel=*source;U32 previous=fraction;fraction+=step;
    source+=integer+(fraction<previous);return pixel;
}
void DrawScaledSpan(SpanState *span) {
    int integer=span->integerStep;U32 step=span->fractionStep;
    U8 *source=span->source,*destination=span->destination;
    int count=span->width;U32 fraction;
    if(span->skip) {
        U32 initial=span->step*span->skip+0x8000;
        fraction=initial<<16;source+=(initial>>16)&255;
    } else fraction=0x80000000;
    if(count>=12) {
        while((U32)destination&3) {*destination++=SampleScaled(source,fraction,integer,step);--count;}
        do {
            U32 pixels=SampleScaled(source,fraction,integer,step);
            pixels|=(U32)SampleScaled(source,fraction,integer,step)<<8;
            pixels|=(U32)SampleScaled(source,fraction,integer,step)<<16;
            pixels|=(U32)SampleScaled(source,fraction,integer,step)<<24;
            *(U32 *)destination=pixels;destination+=4;count-=4;
        } while(count>=4);
    }
    while(count>0) {*destination++=SampleScaled(source,fraction,integer,step);--count;}
}
void DrawSpan16(SpanState *span) {
    U8 *source=span->source+span->skip,*destination=span->destination;
    int count=span->width;
    if(count>=8) {
        while((U32)destination&3) {*destination++=*source++;--count;}
        do {*(U32 *)destination=*(U32 *)source;count-=4;source+=4;destination+=4;} while(count>=4);
    }
    while(count>0) {*destination++=*source++;--count;}
}
inline U32 BackgroundWord() {
    U32 color=(gMenuBackground&0xffff0000)|((gMenuBackground&255)*0x101);
    return (color<<16)|(color>>16)|color;
}
inline void FillSpan(U8 *destination,int count,U32 color) {
    if(count<=0) return;
    if(count>=12) {
        unsigned align=-(U32)destination&3;
        count-=align;
        while(align--) *destination++=(U8)color;
        unsigned words=(unsigned)count>>2;count&=3;
        while(words--) {*(U32 *)destination=color;destination+=4;}
    }
    while(count--) *destination++=(U8)color;
}
void FillTerrainRows(U8 *left,U8 *right,int x,int top,int rows) {
    if(top>viewportClipBottom) return;
    int bottom=top+rows-1;
    if(bottom<viewportClipTop) return;
    int delta=top-viewportClipTop;
    if(delta<0) {rows+=delta;left-=delta;right-=delta;top=viewportClipTop;}
    delta=bottom-viewportClipBottom;if(delta>0) rows-=delta;
    if(rows<=0) return;
    U8 *destination=gShellSurfacePixels+surfaceRowOffsets[top];
    U32 color=BackgroundWord();
    do {
        int begin=*left+++x,end=*right+++x;
        FillSpan(destination+begin,end-begin+1,color);
        destination+=surfaceStride;
    } while(--rows);
}
void FillTerrainRowsClipped(U8 *left,U8 *right,int x,int top,int rows) {
    if(top>viewportClipBottom) return;
    int bottom=top+rows-1;
    if(bottom<viewportClipTop) return;
    int delta=top-viewportClipTop;
    if(delta<0) {rows+=delta;left-=delta;right-=delta;top=viewportClipTop;}
    delta=bottom-viewportClipBottom;if(delta>0) rows-=delta;
    if(rows<=0) return;
    U8 *destination=gShellSurfacePixels+surfaceRowOffsets[top];
    U32 color=BackgroundWord();
    do {
        int begin=*left+++x,end=*right+++x;
        if(begin<viewportClipLeft) begin=viewportClipLeft;
        if(end>viewportClipRight) end=viewportClipRight;
        FillSpan(destination+begin,end-begin+1,color);
        destination+=surfaceStride;
    } while(--rows);
}


