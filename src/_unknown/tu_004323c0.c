/* Provisional terrain tail object. All field names are working labels. */
typedef unsigned char U8;
struct Point {int x,y;};
struct Cell {U8 terrain,height,feature,featureKind,overlay,overlayKind,rotation,variant;unsigned raised;};
struct TerrainGrid {int prefix[10];int count;int rest[6];Cell *cells;};
extern "C" void BestLocSetData(TerrainGrid *,Point *,int);
extern "C" int ResolveName(TerrainGrid *,Point *);
extern Point pathDirections[6];
extern U8 terrainTypeOrder[];
extern "C" void TerrainRaise(TerrainGrid *grid) {
    unsigned unusedDirections[6];Point point;int height,neighborHeight;
    Cell *cell=grid->cells+1;
    for(int index=1;index<grid->count;++index,++cell) {
        cell->raised=0;Point *direction=pathDirections;
        for(int i=0;i!=6;++i,++direction) {
            unusedDirections[i]=0;
            BestLocSetData(grid,&point,index);
            point.x+=direction->x;point.y+=direction->y;
            int neighbor=ResolveName(grid,&point);
            if(neighbor) {
                Cell *other=grid->cells+neighbor;
                if(terrainTypeOrder[cell->terrain]<terrainTypeOrder[other->terrain]) {
                    height=cell->height;neighborHeight=other->height;
                    if(height<=neighborHeight) cell->raised|=(unsigned)terrainTypeOrder[other->terrain]<<(i*4);
                }
            }
        }
    }
}

