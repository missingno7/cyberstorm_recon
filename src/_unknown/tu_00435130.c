/* C++ object ends at 004358b8; the following ENTER routines are a
 * separate provisional assembly-origin object. Inferred rendering names. */
#include <string.h>
typedef unsigned char U8;
struct RayPoint {int x,y;};
struct SpanState {
    int integerStep;unsigned fractionStep;int skip;U8 *destination,*source;
    int width,step,verticalStep,size,doubleSize,tripleSize,quadSize;
    U8 left[256],right[256],shrinkLeft[1000],shrinkRight[1000],growLeft[1000],growRight[1000];
    int magic;void (*draw)(SpanState *);int rows;U8 *rowLeft,*rowRight;
} terrainSpan;
extern int viewportClipLeft,viewportClipTop,viewportClipRight,viewportClipBottom;
extern U8 *gShellSurfacePixels;
extern int *surfaceRowOffsets;
extern "C" int surfaceRowStride;
void RayWalk(RayPoint *,RayPoint *,void (*)(RayPoint *));
void DrawScaledSpan(SpanState *);void DrawSpan16(SpanState *);
void FillTerrainRows(U8 *,U8 *,int,int,int);
void FillTerrainRowsClipped(U8 *,U8 *,int,int,int);
void StoreTerrainEdge(RayPoint *point) {terrainSpan.left[point->y]=(U8)point->x;}
void SetTerrainSpanSize(int size) {
    
    if(size==terrainSpan.size) return;
    memset(&terrainSpan,0,0x11d4);
    terrainSpan.size=size;terrainSpan.doubleSize=size*2;
    terrainSpan.tripleSize=size*3;terrainSpan.magic=0x589ac;terrainSpan.quadSize=size*4;
    terrainSpan.draw=size==16 ? DrawSpan16 : DrawScaledSpan;
    int step=0x100000/size;
    terrainSpan.integerStep=(step&0xff0000)>>16;terrainSpan.fractionStep=(step&0xffff)<<16;terrainSpan.verticalStep=step;terrainSpan.step=step;
    RayPoint b,a;a.x=terrainSpan.size;a.y=0;b.x=0;b.y=terrainSpan.doubleSize;RayWalk(&a,&b,StoreTerrainEdge);
    for(int i=0;i<1000;++i) {
        terrainSpan.shrinkLeft[i]=0;terrainSpan.shrinkRight[i]=terrainSpan.size;
        terrainSpan.growLeft[i]=0;terrainSpan.growRight[i]=terrainSpan.size;
    }
    int j=1000-terrainSpan.doubleSize;
    for(i=0;i<terrainSpan.doubleSize;++i,++j) {
        int left=terrainSpan.left[i];
        int right=terrainSpan.quadSize-left-1;
        int k=terrainSpan.quadSize-i-1;
        terrainSpan.right[i]=right;
        terrainSpan.right[k]=right;
        terrainSpan.left[k]=left;
        terrainSpan.shrinkRight[i]=terrainSpan.shrinkLeft[j]=terrainSpan.size-left;
        terrainSpan.growRight[j]=left;terrainSpan.growLeft[i]=left;
    }
}
inline U8 *TerrainPixels(int *image){return (U8 *)image+*image;}
#define TERRAIN_PIXELS(image) (*(image)+(U8 *)(image))
#define SOURCE_ROW(row) ((row)<<16)
void DrawTerrainRows(int *image,int row,int x,int right,int top,int bottom) {
    if(top<viewportClipTop) {
        int delta=viewportClipTop-top;
        terrainSpan.rows-=delta;terrainSpan.rowLeft+=delta;terrainSpan.rowRight+=delta;
        top=viewportClipTop;
    }
    if(bottom>viewportClipBottom) {terrainSpan.rows-=bottom-viewportClipBottom;bottom=viewportClipBottom;}
    if(x<viewportClipLeft || right>viewportClipRight) {
        U8 *pixels=TERRAIN_PIXELS(image);
        U8 *destination=gShellSurfacePixels+surfaceRowOffsets[top];
        int sourceRow=SOURCE_ROW(row);
        for(int y=top;y<=bottom;++y) {
            int left=x+*terrainSpan.rowLeft++;
            int end=x+*terrainSpan.rowRight++;
            if(left<viewportClipLeft) {if(end<viewportClipLeft) continue;left=viewportClipLeft;}
            if(end>viewportClipRight) {if(left>viewportClipRight) continue;end=viewportClipRight;}
            terrainSpan.skip=left-x;terrainSpan.width=end-left+1;
            terrainSpan.destination=destination+left;
            terrainSpan.source=pixels+((sourceRow&0xff0000)>>12);
            sourceRow+=terrainSpan.step;terrainSpan.draw(&terrainSpan);
            destination+=surfaceRowStride;
        }
    } else {
        U8 *pixels=TERRAIN_PIXELS(image);
        U8 *destination=gShellSurfacePixels+surfaceRowOffsets[top];
        row<<=16;
        for(int y=top;y<=bottom;++y) {
            int skip=*terrainSpan.rowLeft++;
            int left=skip+x;
            int end=*terrainSpan.rowRight+++x;
            terrainSpan.skip=skip;terrainSpan.width=end-left+1;
            terrainSpan.destination=destination+left;
            terrainSpan.source=pixels+((row&0xff0000)>>12);
            row+=terrainSpan.step;terrainSpan.draw(&terrainSpan);
            destination+=surfaceRowStride;
        }
    }
}
void DrawTerrainTop(int *image,int row,int x,int top,int bottom,int texture) {
    if(x>viewportClipRight || top>viewportClipBottom || bottom<viewportClipTop) return;
    int right=x+terrainSpan.size;
    if(right<viewportClipLeft) return;
    terrainSpan.rows=bottom-top+1;
    terrainSpan.rowLeft=terrainSpan.shrinkLeft+(999-terrainSpan.rows);
    terrainSpan.rowRight=terrainSpan.shrinkRight+1;
    if(texture) {DrawTerrainRows(image,row,x,right,top,bottom);return;}
    if(x<viewportClipLeft || right>viewportClipRight)
        FillTerrainRowsClipped(terrainSpan.rowLeft,terrainSpan.rowRight,x,top,terrainSpan.rows);
    else FillTerrainRows(terrainSpan.rowLeft,terrainSpan.shrinkRight+1,x,top,terrainSpan.rows);
}
void DrawTerrainBottom(int *image,int row,int x,int top,int bottom,int texture) {
    if(x>viewportClipRight || top>viewportClipBottom || bottom<viewportClipTop) return;
    int right=x+terrainSpan.size;
    if(right<viewportClipLeft) return;
    terrainSpan.rows=bottom-top+1;
    terrainSpan.rowLeft=terrainSpan.growLeft+1;
    terrainSpan.rowRight=terrainSpan.growRight+(999-terrainSpan.rows);
    if(texture) {DrawTerrainRows(image,row,x,right,top,bottom);return;}
    if(x<viewportClipLeft || right>viewportClipRight)
        FillTerrainRowsClipped(terrainSpan.rowLeft,terrainSpan.rowRight,x,top,terrainSpan.rows);
    else FillTerrainRows(terrainSpan.growLeft+1,terrainSpan.rowRight,x,top,terrainSpan.rows);
}
void DrawTerrainMiddle(int *image,int row,int x,int top,int bottom) {
    if(x>viewportClipRight || top>viewportClipBottom || bottom<viewportClipTop) return;
    int right=x+terrainSpan.doubleSize-1;
    if(right<viewportClipLeft) return;
    if(bottom>viewportClipBottom) bottom=viewportClipBottom;
    if(top<viewportClipTop) top=viewportClipTop;
    terrainSpan.width=terrainSpan.doubleSize;terrainSpan.skip=0;
    if(x<viewportClipLeft) {terrainSpan.skip=viewportClipLeft-x;terrainSpan.width-=terrainSpan.skip;x=viewportClipLeft;}
    if(right>viewportClipRight) terrainSpan.width-=right-viewportClipRight;
    if(terrainSpan.width>0) {
        U8 *pixels=TERRAIN_PIXELS(image);
        terrainSpan.destination=gShellSurfacePixels+surfaceRowOffsets[top]+x;
        int sourceRow=SOURCE_ROW(row);
        for(int y=top;y<=bottom;++y) {
            terrainSpan.source=pixels+((sourceRow&0xff0000)>>11);
            sourceRow+=terrainSpan.step;terrainSpan.draw(&terrainSpan);
            terrainSpan.destination+=surfaceRowStride;
        }
    }
}
void FillTerrainDiamond(int x,int y) {
    if(x>viewportClipRight || y>viewportClipBottom) return;
    int right=x+terrainSpan.quadSize-1;
    int bottom=y+terrainSpan.quadSize-1;
    if(right<viewportClipLeft || bottom<viewportClipTop) return;
    if(x<viewportClipLeft || right>viewportClipRight)
        FillTerrainRowsClipped(terrainSpan.left,terrainSpan.right,x,y,terrainSpan.quadSize);
    else FillTerrainRows(terrainSpan.left,terrainSpan.right,x,y,terrainSpan.quadSize);
}









