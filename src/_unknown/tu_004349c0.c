/* Unnamed object starts at 004349c0: empty global-point initializer, then height editing callbacks. */
typedef unsigned char U8;
struct Point {int x,y;Point(){};};
struct Cell {U8 terrain,height,feature,featureKind,overlay,overlayKind,rotation,variant,unknown[4];};
struct TerrainGrid {int fields[2];int radius,pitch;Point center;int layout[4];int count,waterCount;int rest[5];Cell *cells;Cell *CellAt(int i){return cells+i;}};
struct Visit {TerrainGrid *grid;Point point;int index;int Index(){return index;}};
struct Search {int words[10];};
extern int heightSum,heightCount,heightTarget,heightInner,heightOuter,heightStrength;
Point heightCenter;
extern unsigned MissionRandomTable;
extern "C" int LogicCopyPoint(Point *,Point *);
extern void BestLocSetData(void *,void *,void *);
extern int BestLocInitialize(void *,const char *,int);
extern int BestLocSetCandidate(void *,void *);
extern void BestLocVisit(void *,int (*)(void *));
extern "C" int RandomRange(unsigned *,int,int);
void TerrainSetNoiseHeight(Point *,U8);

int TerrainSumHeight(void *data) {
Visit *v=(Visit *)data; int index=v->index; if(index){TerrainGrid *grid=v->grid;heightSum+=grid->cells[index].height;++heightCount;}return 0;
}
int TerrainAverageHeight(TerrainGrid *grid,int index,int radius)
{
    Search search;
    heightSum=0;heightCount=0;
    BestLocInitialize(&search,(const char *)grid,radius);
    BestLocSetCandidate(&search,(void *)index);
    BestLocVisit(&search,TerrainSumHeight);
    return heightCount>0?heightSum/heightCount:0;
}
int TerrainBlendHeight(Visit *v)
{
    int index=v->index;
    if(index) {
        TerrainGrid *grid=v->grid; Cell *cell=grid->CellAt(index);
        if(cell->height!=heightTarget) {
            int distance=LogicCopyPoint(&heightCenter,&v->point);
            int strength=heightStrength;
            if(distance>heightInner) strength=(heightOuter-distance)*strength/(heightOuter-heightInner);
            if(strength<100) {
                if(distance>heightInner) {
                    int range=(heightOuter-distance)*30/(heightOuter-heightInner)+25;
                    strength+=RandomRange(&MissionRandomTable,-range,range);
                    strength=strength<=0?0:(strength>=100?100:strength);
                }
                int oldHeight=cell->height;
                cell->height=(oldHeight*100+(heightTarget-oldHeight)*strength+50)/100;
            } else cell->height=heightTarget;
            TerrainSetNoiseHeight(&v->point,cell->height);
        }
    }
    return 0;
}
void TerrainAdjustHeight(TerrainGrid *grid,int index,int target,int inner,int outer,int strength)
{
    Search search;
    unsigned seed=MissionRandomTable; MissionRandomTable=0x12345678;
    if(!index||outer<=0||outer<=inner){MissionRandomTable=seed;return;}
    {
        BestLocSetData(grid,&heightCenter,(void *)index);
        heightOuter=outer;heightTarget=target;heightInner=inner;heightStrength=strength;
        BestLocInitialize(&search,(const char *)grid,outer-1);
        BestLocSetCandidate(&search,(void *)index);
        BestLocVisit(&search,(int (*)(void *))TerrainBlendHeight);
    }
    MissionRandomTable=seed;
}
int TerrainNoAction() { return 0; }

