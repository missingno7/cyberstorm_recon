typedef unsigned int uint; typedef int SurfaceDepth; extern signed char SurfaceNoiseTable[491]; extern unsigned short SurfaceScaleValues[];

extern "C" int RandomRange(unsigned int *, int, int);


extern int MissionRandomTable[];
extern unsigned short SurfaceAverage(unsigned short, unsigned short, unsigned short, unsigned short);
extern unsigned short SurfaceMidpoint(unsigned short, unsigned short);
extern int SurfaceNoiseAt(int, int, int, int, int, int, unsigned char);
extern "C" void f_0042fc90(char *, int, int, int, unsigned char,
                             unsigned char, unsigned char, unsigned char,
                             unsigned char, unsigned char, unsigned char,
                             unsigned char, unsigned char, unsigned char);

inline unsigned char SurfaceSourceAt(char *surface, unsigned int row, unsigned int col) { return (*(unsigned char **)(surface+0x6c))[row * *(unsigned int *)(surface+0x68)+col]; }
extern void SurfaceBuildNoiseTable();
extern unsigned int SurfaceNoiseDivisor;
extern "C" void f_0042f540(char *object)
{
    unsigned int span = (1U << (*(int *)(object+0x58)+1)) + 1;
    int depth=*(int *)(object+0x68);
    unsigned int side=depth*2-3;
    unsigned int *rows=(unsigned int *)new char[side*16];
    unsigned int address=(unsigned int)new char[side*side];
    if (side) { unsigned int *r=rows; do { *r=address; ++r; address+=side; } while (r < rows+side); }
    unsigned int sourceRow=1, previousRow=0, rowSlot=2;
    for (; sourceRow < depth-1; ++sourceRow, ++previousRow, rowSlot+=2) {
        unsigned int previousColumn=0;
        unsigned int outColumn=0;
        for (unsigned int column=1; column < depth-1; ++column, ++previousColumn, outColumn+=2) {
            unsigned int *r=rows+rowSlot-2;
            *(unsigned char *)(r[0]+outColumn)=SurfaceAverage(SurfaceSourceAt(object,sourceRow,column),SurfaceSourceAt(object,sourceRow,previousColumn),SurfaceSourceAt(object,previousRow,column),SurfaceSourceAt(object,previousRow,previousColumn));
            *(unsigned char *)(r[1]+outColumn)=SurfaceMidpoint(SurfaceSourceAt(object,sourceRow,previousColumn),SurfaceSourceAt(object,sourceRow,column));
            *(unsigned char *)(r[0]+outColumn+1)=SurfaceMidpoint(SurfaceSourceAt(object,sourceRow,column),SurfaceSourceAt(object,previousRow,column));
            *(unsigned char *)(r[1]+outColumn+1)=SurfaceSourceAt(object,sourceRow,column);
        }
    }
    unsigned int lastRow=depth-1;
    unsigned int previousColumn=0, outColumn=0;
    for (unsigned int column=1; column<depth-1; ++column, ++previousColumn, outColumn+=2) {
        *(unsigned char *)(rows[lastRow*2-2]+outColumn)=SurfaceAverage(SurfaceSourceAt(object,lastRow,column),SurfaceSourceAt(object,depth-2,previousColumn),SurfaceSourceAt(object,depth-2,column),SurfaceSourceAt(object,lastRow,previousColumn));
        *(unsigned char *)(rows[lastRow*2-2]+outColumn+1)=SurfaceMidpoint(SurfaceSourceAt(object,lastRow,column),SurfaceSourceAt(object,depth-2,column));
    }
    previousRow=0;
    unsigned int *edge=rows;
    for (sourceRow=1;sourceRow<depth-1;++sourceRow,++previousRow,edge+=2) {
        *(unsigned char *)(edge[0]+lastRow*2-2)=SurfaceAverage(SurfaceSourceAt(object,sourceRow,lastRow),SurfaceSourceAt(object,previousRow,depth-2),SurfaceSourceAt(object,previousRow,lastRow),SurfaceSourceAt(object,sourceRow,depth-2));
        *(unsigned char *)(edge[1]+lastRow*2-2)=SurfaceMidpoint(SurfaceSourceAt(object,sourceRow,lastRow),SurfaceSourceAt(object,sourceRow,depth-2));
    }
    *(unsigned char *)(rows[lastRow*2-2]-4)=SurfaceAverage(SurfaceSourceAt(object,lastRow,(unsigned int)-1),SurfaceSourceAt(object,depth-2,(unsigned int)-2),SurfaceSourceAt(object,depth-2,(unsigned int)-1),SurfaceSourceAt(object,lastRow,(unsigned int)-2));
    SurfaceBuildNoiseTable();
    SurfaceNoiseDivisor=SurfaceScaleValues[*(int *)(object+0x60)];
    unsigned int amplitude=(unsigned int)(*(int *)(object+0x5c) * *(int *)(object+0x5c) * *(int *)(object+0x64))/SurfaceNoiseDivisor;
    unsigned int origin=0;
    unsigned int *middle=rows+1;
    for (; origin < side-1; origin+=2, middle+=2) {
        unsigned int previous=0;
        for (unsigned int col=1; col<side; col+=2, previous+=2) {
            unsigned int top=middle[-1], mid=middle[0], bottom=middle[1];
            f_0042fc90(object,(span-1)*(origin>>1),(previous>>1)*(span-1),amplitude,*(unsigned char *)(object+0x58),
                *(unsigned char *)(top+col-1),*(unsigned char *)(top+col),*(unsigned char *)(top+col+1),
                *(unsigned char *)(mid+col-1),*(unsigned char *)(mid+col),*(unsigned char *)(mid+col+1),
                *(unsigned char *)(bottom+col-1),*(unsigned char *)(bottom+col),*(unsigned char *)(bottom+col+1));
        }
    }
    delete[] (char *)rows[0];
    delete[] (char *)rows;
}

uint SurfaceAdjust(char *surface,unsigned short first,uint value,unsigned short second)
{
 if(!*(int *)(surface+0x54)) return value;
 int low=(unsigned short)value;
 int firstDifference=(int)first-low;
 int secondDifference=(int)second-low;
 int center=(secondDifference+firstDifference)/2;
 int adjusted=(low-center)*2; adjusted+=first; adjusted+=second; return adjusted>>2;
}

extern "C" void f_0042fc90(char *object, int x, int y, int amplitude,
                            unsigned char level, unsigned char p6,
                            unsigned char p7, unsigned char p8,
                            unsigned char p9, unsigned char p10,
                            unsigned char p11, unsigned char p12,
                            unsigned char p13, unsigned char p14)
{
    if (level == 0) {
        unsigned char *cell = *(unsigned char **)(object + 0x98) + y + x * *(unsigned int *)(object + 0x94);
        cell[0]=p6; cell[1]=p7; cell[2]=p8;
        cell += *(unsigned int *)(object + 0x94);
        cell[0]=p9; cell[1]=p10; cell[2]=p11;
        cell += *(unsigned int *)(object + 0x94);
        cell[0]=p12; cell[1]=p13; cell[2]=p14;
        return;
    }
    unsigned char half = 1U << (level - 1);
    unsigned char full = 1U << level;
    int delta = (int)(((unsigned int)(amplitude * 1000) / SurfaceNoiseDivisor) / 10);
    --level;
    unsigned char upperLeft = p9;
    if (*(int *)(object+0x54)) upperLeft = SurfaceAdjust(object,p6,p9,p12);
    unsigned int topMiddle = p7;
    if (*(int *)(object+0x54)) topMiddle = SurfaceAdjust(object,p6,p7,p8);
    unsigned int lowerMiddle = p13;
    if (*(int *)(object+0x54)) lowerMiddle = SurfaceAdjust(object,p12,p13,p14);
    unsigned char upperRight = p11;
    if (*(int *)(object+0x54)) upperRight = SurfaceAdjust(object,p8,p11,p14);
    unsigned int leftCenter = ((int)p9 + p10) >> 1;
    unsigned int rightCenter = ((int)p10 + p11) >> 1;
    unsigned int center = p10;
    if (*(int *)(object+0x54)) center = SurfaceAdjust(object,p9,p10,p11);
    int leftTop = ((int)p7+p6)>>1;
    int centerTop = leftTop+leftCenter;
    int leftBottom = ((int)p12+p13)>>1;
    int centerBottom = leftCenter+leftBottom;
    if (*(int *)(object+0x54)) leftCenter = SurfaceAdjust(object,leftTop,leftCenter,leftBottom);
    int rightTop = ((int)p8+p7)>>1;
    int rightCenterTop = rightTop+rightCenter;
    int rightBottom = ((int)p13+p14)>>1;
    int rightCenterBottom = rightCenter+rightBottom;
    if (*(int *)(object+0x54)) rightCenter = SurfaceAdjust(object,rightTop,rightCenter,rightBottom);
    unsigned int smoothCenter=center&255;
    if (*(int *)(object+0x54)) smoothCenter=SurfaceAdjust(object,(unsigned char)topMiddle,smoothCenter,(unsigned char)lowerMiddle);
    unsigned char middle = smoothCenter;
    int nTopLeft=SurfaceNoiseAt(x,y,2,1,half,amplitude,(unsigned char)leftCenter);
    int nTopRight=SurfaceNoiseAt(x,y,2,3,half,amplitude,(unsigned char)rightCenter);
    int nLeft=SurfaceNoiseAt(x,y,0,1,half,amplitude,(unsigned char)leftTop);
    int nCenter=SurfaceNoiseAt(x,y,1,1,half,amplitude,(unsigned char)(centerTop>>1));
    int nBottomLeft=SurfaceNoiseAt(x,y,4,1,half,amplitude,(unsigned char)leftBottom);
    int nBottomRight=SurfaceNoiseAt(x,y,3,1,half,amplitude,(unsigned char)(centerBottom>>1));
    int nRight=SurfaceNoiseAt(x,y,0,3,half,amplitude,(unsigned char)rightTop);
    int nRightCenter=SurfaceNoiseAt(x,y,1,3,half,amplitude,(unsigned char)(rightCenterTop>>1));
    int nLowerRight=SurfaceNoiseAt(x,y,4,3,half,amplitude,(unsigned char)rightBottom);
    int nFarRight=SurfaceNoiseAt(x,y,3,3,half,amplitude,(unsigned char)(rightCenterBottom>>1));
    int nMiddleTop=SurfaceNoiseAt(x,y,1,2,half,amplitude,(unsigned char)(((unsigned char)topMiddle+(unsigned char)center)>>1));
    int nMiddleBottom=SurfaceNoiseAt(x,y,3,2,half,amplitude,(unsigned char)(((unsigned char)center+(unsigned char)lowerMiddle)>>1));
    int nInnerLeft=SurfaceNoiseAt(x,y,1,0,half,amplitude,(unsigned char)(((int)p9+p6)>>1));
    f_0042fc90(object,x,y,delta,level,p6,nLeft,topMiddle,nInnerLeft,nCenter,nMiddleTop,upperLeft,nTopLeft,middle);
    int nInnerRight=SurfaceNoiseAt(x,y,1,4,half,amplitude,(unsigned char)(((int)p8+p11)>>1));
    f_0042fc90(object,x,y+full,delta,level,topMiddle,nRight,p8,nMiddleTop,nRightCenter,nInnerRight,middle,nTopRight,upperRight);
    int nBottom=SurfaceNoiseAt(x,y,3,0,half,amplitude,(unsigned char)(((int)p12+p9)>>1));
    f_0042fc90(object,x+full,y,delta,level,upperLeft,nTopLeft,middle,nBottom,nBottomRight,nMiddleBottom,p12,nBottomLeft,lowerMiddle);
    int nFarBottom=SurfaceNoiseAt(x,y,3,4,half,amplitude,(unsigned char)(((int)p14+p11)>>1));
    f_0042fc90(object,x+full,y+full,delta,level,middle,nTopRight,upperRight,nMiddleBottom,nFarRight,nFarBottom,lowerMiddle,nLowerRight,p14);
}

unsigned short SurfaceMidpoint(unsigned short first, unsigned short second)
{
    return (first + second) >> 1;
}

unsigned short SurfaceAverage(unsigned short first, unsigned short second,
                   unsigned short third, unsigned short fourth)
{
    return (first + second + third + fourth) >> 2;
}

extern "C" int RandomRange(unsigned int *seed, int low, int high);


extern int MissionRandomTable[];
struct ResMgr { int *Load(char *, unsigned int = 1, int * = 0, unsigned int = 0); };
extern ResMgr gResMgr;
extern "C" void AssertFail(const char *, int, const char *);
extern "C" unsigned int strlen(const char *);
extern "C" char *strcpy(char *, const char *);
extern "C" void *memcpy(void *, const void *, unsigned int);
extern void ReleasePoolData(void *);
static const char SurfaceSourcePath[] = "terr\\tsurface.c";

void SurfaceBuildNoiseTable()
{
    unsigned short index=0;
    
    for (;;) {
        int total=0;
        unsigned char sample=0;
        for(;sample<12;++sample) total+=RandomRange((unsigned int *)MissionRandomTable,-32,32);
        SurfaceNoiseTable[index]=(signed char)((short)total/12);
        if(++index>=0x1ebU) return;
    }
}

int SurfaceNoiseAt(int x, int y, int xStep, int yStep, int scale,
                   int amplitude, unsigned char base)
{
    x = xStep * scale + x;
    y = yStep * scale + y;
    unsigned int index = (unsigned int)(x * 0x29 + y * 0x2f + x * 0xb * x + y * 0x11 * y) % 0x1ea;
    int value = (unsigned int)base + ((int)SurfaceNoiseTable[index] * amplitude >> 7);
    if (value > 0xff) value = 0xff;
    if (value < 0) value = 0;
    return value;
}

int *SurfaceAllocateGrid(int width, int height, int depth)
{
    int *rows = (int *)new char[width << 4];
    char *data = new char[height * width * depth];
    if ((unsigned int)width > 0) {
        int *row = rows;
        do {
            *row = (int)data;
            ++row;
            data += height * depth;
        } while (row < rows + width);
    }
    return rows;
}

void SurfaceReleaseGrid(void * volatile *rows)
{
    delete[] (char *)rows[0];
    delete[] (char *)rows;
}

int SurfaceStaticValue(int index)
{
    return SurfaceScaleValues[index];
}

int SurfaceLoadDefinition(char *object, const char *name)
{
    int *resource = gResMgr.Load((char *)name);
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.



































































































    if (!resource) AssertFail(SurfaceSourcePath, __LINE__, (const char *)resource);
    unsigned int *header = (unsigned int *)resource;
    // Original source line padding.
    // Original source line padding.
    if (*header != 0x88888888U) AssertFail(SurfaceSourcePath, __LINE__, 0);
    char *cursor = (char *)(header + 1);
    int textLength = strlen(cursor);
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    if (textLength >= 0x50) AssertFail(SurfaceSourcePath, __LINE__, 0);
    strcpy(object, cursor);
    cursor += textLength + 1;
    *(unsigned int *)(object + 0x54) = ((unsigned int *)cursor)[0];
    *(unsigned int *)(object + 0x58) = ((unsigned int *)cursor)[1];
    *(unsigned int *)(object + 0x5c) = ((unsigned int *)cursor)[2];
    *(unsigned int *)(object + 0x60) = ((unsigned int *)cursor)[3];
    *(unsigned int *)(object + 0x64) = ((unsigned int *)cursor)[4];
    *(SurfaceDepth *)(object + 0x68) = ((unsigned int *)cursor)[5];
    *(unsigned int *)(object + 0x94) = ((unsigned int *)cursor)[6];
    cursor += 28;
    int level = *(unsigned int *)(object + 0x94) >> 6;
    *(int *)(object + 0x50) = 0;
    while (level > 1) { ++*(int *)(object + 0x50); level >>= 1; }
    char *allocation = new char[*(unsigned int *)(object + 0x94) * *(unsigned int *)(object + 0x94) + *(SurfaceDepth *)(object + 0x68) * *(SurfaceDepth *)(object + 0x68) * 3];
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    // Original source line padding.
    if (!allocation) AssertFail(SurfaceSourcePath, __LINE__, (const char *)allocation);
    *(char **)(object + 0x98) = allocation;
    allocation += *(unsigned int *)(object + 0x94) * *(unsigned int *)(object + 0x94);
    *(char **)(object + 0x6c) = allocation;
    allocation += *(SurfaceDepth *)(object + 0x68) * *(SurfaceDepth *)(object + 0x68);
    *(char **)(object + 0x70) = allocation;
    memcpy(*(void **)(object + 0x6c), cursor, *(SurfaceDepth *)(object + 0x68) * *(SurfaceDepth *)(object + 0x68));
    cursor += *(SurfaceDepth *)(object + 0x68) * *(SurfaceDepth *)(object + 0x68);
    memcpy(*(void **)(object + 0x70), cursor, (*(SurfaceDepth *)(object + 0x68) * *(SurfaceDepth *)(object + 0x68)) * 2);
    cursor += (*(SurfaceDepth *)(object + 0x68) * *(SurfaceDepth *)(object + 0x68)) * 2;
    memcpy(object + 0xbc, cursor, 0x100); cursor += 0x100;
    memcpy(object + 0x74, cursor, 0x20); cursor += 0x20;
    memcpy(object + 0x9c, cursor, 0x20);
    ReleasePoolData(resource);
    return 1;
}

void SurfaceClearAllocation(void *object)
{
    *(int *)((char *)object + 0x98) = 0;
}

int *SurfaceReleaseAllocation(int *object)
{
    if ((unsigned int)object[0x26] != 0) {
        delete[] (char *)object[0x26];
        object[0x26] = 0;
    }
    return object;
}

extern "C" unsigned int next_link_seed(unsigned int *seed);
extern "C" int sprintf(char *, const char *, ...);
extern "C" void *memset(void *, int, unsigned int);
extern "C" void f_0042f540(char *);

void SurfaceApplyNoise(int *object)
{
    int i;
    unsigned int row;
    unsigned char *grid=(unsigned char *)object[0x1b];
    signed char *weights=(signed char *)object[0x1c];
    for (row=0; row < (unsigned int)object[0x1a]; ++row) {
        for (unsigned int column=0; column < (unsigned int)object[0x1a]; ++column, ++grid, weights+=2) {
            int value=RandomRange((unsigned int *)MissionRandomTable,weights[1],weights[0])+*grid;
            if (value<0) *grid=0;
            else if (value>255) *grid=255;
            else *grid=value;
        }
    }
    unsigned char *descriptor=(unsigned char *)(object+0x1d);
    unsigned int depth=object[0x1a]*object[0x1a];
    for (i=0;i<8;++i,descriptor+=4) {
        int count=descriptor[0];
        unsigned char value=(unsigned char)RandomRange((unsigned int *)MissionRandomTable,descriptor[2],descriptor[3]);
        while (count-- > 0) {
            unsigned int index=next_link_seed((unsigned int *)MissionRandomTable) % depth;
            unsigned char *data=(unsigned char *)object[0x1b];
            data+=index;
            *data=value;
        }
    }
}
struct SurfaceScatterGrid { char gap[0x98]; unsigned char *data; unsigned char &At(unsigned int i) { return data[i]; } };
void SurfaceApplyNoiseDetails(int *object)
{
    unsigned int width = object[0x25]*object[0x25];
    unsigned char *descriptor = (unsigned char *)(object + 0x27);
    for (int n = 0; n < 8; ++n, descriptor += 4) {
        int count = descriptor[0];
        unsigned char value = (unsigned char)RandomRange((unsigned int *)MissionRandomTable, descriptor[2], descriptor[3]);
        while (count-- > 0) {
            ((SurfaceScatterGrid *)object)->At(next_link_seed((unsigned int *)MissionRandomTable)%width)=value;
        }
    }
}
void SurfaceInitializeProcedural(int *object, int fileNumber)
{
    char filename[32];
    sprintf(filename, "scape%d.dat", fileNumber);
    if ((unsigned int)object[0x26] != 0) {
        delete[] (char *)object[0x26];
        object[0x26] = 0;
    }
    SurfaceLoadDefinition((char *)object, filename);
    SurfaceApplyNoise(object);
    f_0042f540((char *)object);
    SurfaceApplyNoiseDetails(object);
}

void SurfaceClearAllocationAndRelease(int *object)
{
    if ((unsigned int)object[0x26] != 0) {
        delete[] (char *)object[0x26];
        object[0x26] = 0;
    }
}

struct SurfaceGridLayout { char prefix[0x50]; int shift; char middle[0x40]; unsigned int width; unsigned char *data; };
unsigned char SurfaceGridValue(SurfaceGridLayout *object, unsigned int *point)
{
    int x = point[0] & 63;
    int y = point[1] & 63;
    x <<= object->shift; y <<= object->shift;
    if ((unsigned int)x >= object->width) x = object->width - 1;
    if ((unsigned int)y >= object->width) y = object->width - 1;
    return object->data[x * object->width + y];
}

void SurfaceGridSetValue(SurfaceGridLayout *object, unsigned int *point, unsigned char value)
{
    if (object->data) {
        unsigned int y;
        unsigned int x;
        x = point[0] & 63;
        y = point[1] & 63;
        x <<= object->shift; y <<= object->shift;
        if (x >= object->width) x = object->width - 1;
        if (y >= object->width) y = object->width - 1;
        object->data[y + x * object->width] = value;
    }
}
void SurfaceApplyColorMap(int *object)
{
    unsigned char *data = (unsigned char *)object[0x26];
    for (unsigned int row = 0; row < (unsigned int)object[0x25]; ++row)
        for (unsigned int column = 0; column < (unsigned int)object[0x25]; ++column, ++data)
            *data = ((unsigned char *)object)[0xbc + *data];
}

unsigned int SurfaceRandomize(int *object, int fileNumber, int fillGrid)
{
    unsigned int result = 0;
    char filename[32];
    sprintf(filename, "scape%d.dat", fileNumber);
    if ((unsigned int)object[0x26] != 0) {
        delete[] (char *)object[0x26];
        object[0x26] = 0;
    }
    SurfaceLoadDefinition((char *)object, filename);
    unsigned char *grid = (unsigned char *)object[0x1b];
    unsigned char lowGrid = grid[0];
    unsigned char highGrid = grid[1];
    unsigned char noiseLow = grid[2];
    unsigned char noiseHigh = grid[3];
    unsigned char scatterLow = grid[4];
    unsigned char scatterHigh = grid[5];
    int count = object[0x1a] * object[0x1a];
    unsigned char *cell = grid; for (int cellNumber = 0; cellNumber < count; ++cellNumber, ++cell) *cell = (unsigned char)RandomRange((unsigned int *)MissionRandomTable, noiseLow, noiseHigh);
    unsigned char scatterCount = (unsigned char)RandomRange((unsigned int *)MissionRandomTable, lowGrid, highGrid);
    for (int i = 0; i < scatterCount; ++i) {
        unsigned int index = next_link_seed((unsigned int *)MissionRandomTable) % count;
        int value = RandomRange((unsigned int *)MissionRandomTable, scatterLow, scatterHigh);
        grid[index] = (unsigned char)value;
    }
    f_0042f540((char *)object);
    unsigned char *descriptor = (unsigned char *)(object + 0x27);
    unsigned int width = object[0x25];
    for (int descriptorIndex = 0; descriptorIndex != 8; ++descriptorIndex, descriptor += 4) {
        int number = descriptor[0];
        unsigned char value = (unsigned char)RandomRange((unsigned int *)MissionRandomTable, descriptor[2], descriptor[3]);
        while (number > 0) {
            unsigned int index = next_link_seed((unsigned int *)MissionRandomTable) % (width * width);
            ((unsigned char *)object[0x26])[index] = (unsigned char)value;
            --number;
        }
    }
    if (fillGrid != 0) {
        unsigned int cells = object[0x25] * object[0x25]; unsigned char *target = (unsigned char *)object[0x26];
        unsigned char base = (unsigned char)RandomRange((unsigned int *)MissionRandomTable, noiseLow, noiseHigh);
        memset(target, base, cells);
        for (int fillIndex = 0; fillIndex < scatterCount; ++fillIndex) {
            unsigned int index = next_link_seed((unsigned int *)MissionRandomTable) % cells;
            int value = RandomRange((unsigned int *)MissionRandomTable, scatterLow, scatterHigh);
            target[index] = (unsigned char)value;
        }
        result = base;
    }
    return result;
}













