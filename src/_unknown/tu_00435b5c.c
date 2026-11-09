/* Separate apparent assembly object. Full C++ raster reconstruction. */
typedef unsigned char U8;typedef unsigned long U32;
struct Image {int pixels;};
struct TileRun {U8 start,leading,words,trailing;};
extern TileRun tileRuns[];
extern int tileStarts[4],tileRowSteps[4],tileRowStep;
extern int viewportClipLeft,viewportClipTop,viewportClipRight,viewportClipBottom;
extern U8 *gShellSurfacePixels,*gDrawRemap;
extern int *surfaceRowOffsets;extern int surfaceStride;
void DrawTileFallback(Image *,int,int,int,int,int);
void DrawTerrainTile(Image *image,int x,int y,int direction) {
    U8 *destination=gShellSurfacePixels+surfaceRowOffsets[y]+x;
    U8 *source=(U8 *)image+image->pixels+tileStarts[direction];
    tileRowStep=tileRowSteps[direction];
    TileRun *run=tileRuns;
    if(!(direction&1)) {
        while(run->start!=255) {
            U8 *s=source+run->start,*d=destination+run->start;
            U8 count=run->leading;
            if(count) do {*d++=*s++;} while(--count);
            count=run->words;
            do {*(U32 *)d=*(U32 *)s;s+=4;d+=4;} while(--count);
            count=run->trailing;
            if(count) do {*d++=*s++;} while(--count);
            source+=tileRowStep;destination+=surfaceStride;++run;
        }
    } else {
        while(run->start!=255) {
            U8 *s=source-run->start,*d=destination+run->start;
            U8 count=run->leading;
            if(count) do {*d++=*s--;} while(--count);
            count=run->words;
            do {
                U32 word=*(U32 *)(s-3);
                *(U32 *)d=(word>>24)|((word>>8)&0xff00)|((word<<8)&0xff0000)|(word<<24);
                s-=4;d+=4;
            } while(--count);
            count=run->trailing;
            if(count) do {*d++=*s--;} while(--count);
            source+=tileRowStep;destination+=surfaceStride;++run;
        }
    }
}
void DrawTerrainTileClipped(Image *image,int x,int y,int direction,int width,int height) {
    if(width!=64 || height!=64 || direction!=0) {DrawTileFallback(image,x,y,direction,width,height);return;}
    int right=x+63;
    if(right<viewportClipLeft || x>viewportClipRight) return;
    if(x<viewportClipLeft || right>viewportClipRight) {DrawTileFallback(image,x,y,direction,width,height);return;}
    if(y>viewportClipBottom) return;
    int bottom=y+63;
    if(bottom<viewportClipTop) return;
    int skip=0,delta=y-viewportClipTop;
    if(delta<0) {height+=delta;skip=(-delta)<<6;y=viewportClipTop;}
    delta=bottom-viewportClipBottom;
    if(delta>0) height-=delta;
    if(height<=0) return;
    U8 *destination=gShellSurfacePixels+surfaceRowOffsets[y]+x;
    U8 *source=(U8 *)image+image->pixels+skip;
    if(!gDrawRemap) {
        do {
            U8 *d=destination;
            for(int words=16;words;--words) {
                U32 pixels=*(U32 *)source;
                if(pixels) {
                    U32 old=*(U32 *)d;
                    for(int byte=0;byte<4;++byte) {
                        unsigned shift=byte*8;U32 pixel=(pixels>>shift)&255;
                        if(pixel) old=(old&~(255UL<<shift))|(pixel<<shift);
                    }
                    *(U32 *)d=old;
                }
                source+=4;d+=4;
            }
            destination+=surfaceStride;
        } while(--height);
    } else {
        do {
            U8 *d=destination;
            for(int words=16;words;--words) {
                U32 pixels=*(U32 *)source;
                if(pixels) {
                    U32 old=*(U32 *)d;
                    for(int byte=0;byte<4;++byte) {
                        unsigned shift=byte*8;U32 pixel=(pixels>>shift)&255;
                        if(pixel) old=(old&~(255UL<<shift))|((U32)gDrawRemap[pixel]<<shift);
                    }
                    *(U32 *)d=old;
                }
                source+=4;d+=4;
            }
            destination+=surfaceStride;
        } while(--height);
    }
}
