// terr\tgrid.c. Names and partial field layouts remain working labels.
struct GridPoint { long x, y; };
struct GridCell { unsigned char terrain, water, ground, feature, height, flags, region, unused; long actor; }; struct GridNeighbors { unsigned short index[4]; }; struct TerrainGrid { long unknown00,spacing,radius,diameter; GridPoint center; long centerIndex,rowShift,rowWidth,rowSlack,cellCount,state,unknown30,counts[3],unknown40; GridCell *cells; GridNeighbors *neighbors; long PointToIndex(GridPoint *); };
struct Point {
    long x, y;
    Point(long ax, long ay) : x(ax), y(ay) {}
};
Point pathDirections[6] = {
    Point(0,-2), Point(1,-1), Point(1,1), Point(0,2), Point(-1,1), Point(-1,-1)
};
struct GridIndex { unsigned short value; GridIndex(unsigned short n) : value(n) {} };
// Provisional local-coordinate class; the unmatched conversion uses scalar copy construction.
struct GridCoordLocal {
    long x, y;
    GridCoordLocal() {}
    GridCoordLocal(const GridPoint &p) : x(p.x), y(p.y) {}
    void Set(long ax, long ay) { x=ax; y=ay; }
    void Add(const GridPoint *p) { x+=p->x; y+=p->y; }
};
struct GridSearchState { GridPoint point; long target, bestIndex, bestDistance; };
extern GridSearchState gGridSearch;
extern "C" void AssertFail(const char *, int, const char *);
static const char GridSourcePath[] = "terr\x5ctgrid.c";
extern "C" void *memset(void *, int, unsigned long);
extern "C" int abs(int);
int StreamRead(void *, void *, int);
int StreamWrite(void *, const void *, unsigned long);
void Crc16(unsigned short *, const void *, unsigned int);
struct ApplyPoint; int PointAngle(ApplyPoint *, ApplyPoint *);
int BestLocInitialize(void *, const char *, int);
int BestLocSetCandidate(void *, void *);
void AreaEnumerate(int *, void *, unsigned int, unsigned int);
inline long GridPower(long shift) { return 1<<shift; }
inline long GridAbs(long a) { return a<0 ? -a : a; }
inline long GridMax(long a, long b) { return a>b ? a : b; }
inline GridPoint *GridDecode(TerrainGrid *grid, GridPoint *out, long index) {
    long value=index-1;
    out->x=value&(grid->rowWidth-1);
    out->y=(value>>grid->rowShift)*2+((out->x&1)!=0);
    return out;
}
inline GridPoint *GridSubtract(GridPoint *p, const GridPoint *q) {
    p->x-=q->x; p->y-=q->y;
    return p;
}
inline GridPoint *GridScale(GridPoint *p, long amount) {
    p->x*=amount; p->y*=amount;
    return p;
}
inline long GridDistance(const GridPoint *a, const GridPoint *b) {
    long dx=GridAbs(a->x-b->x); long dy=GridAbs(a->y-b->y); return GridMax(dx,(dx+dy)>>1); }
inline long GridLookup(TerrainGrid *grid, const GridPoint *point) {
    const GridPoint *center=&grid->center; long dx=GridAbs(center->x-point->x); long dy=GridAbs(center->y-point->y);
    if(GridMax(dx,(dx+dy)>>1)>grid->radius) return 0;
    return ((point->y>>1)<<grid->rowShift)+point->x+1;
}
// Direction table is initialized by the global point constructors.
TerrainGrid *GridClear(TerrainGrid *p) { p->cells = 0; p->neighbors = 0; return p; }
TerrainGrid *GridRelease(TerrainGrid *p) {
    if (p->cells) {
        delete[] p->cells; p->cells=0;
        delete[] p->neighbors; p->neighbors=0;
    }
    return p;
}
void GridDestroy(TerrainGrid *p) {
    if (p->cells) {
        delete[] p->cells; p->cells=0;
        delete[] p->neighbors; p->neighbors=0;
    }
}
long GridInitialize(TerrainGrid *p, long spacing, long height) {
    // dimensions are recomputed through the grid fields.
    if (p->cells) {
        delete[] p->cells;
        p->cells = 0;
        delete[] p->neighbors;
        p->neighbors = 0;
    }
    p->spacing = spacing;
    p->radius = height;
    long doubled = height * 2;
    p->diameter = doubled + 1;
    p->center.x = p->radius;
    p->center.y = p->radius * 2 + ((p->center.x & 1) != 0);
    p->rowShift = 0;
    for (long value = doubled; value > 0; value >>= 1) ++p->rowShift;
    p->rowShift = GridMax(p->rowShift,1);
    p->centerIndex = (p->radius << p->rowShift) + p->radius + 1;
    long power = GridPower(p->rowShift);
    p->rowWidth = power;
    p->rowSlack = p->rowWidth - p->diameter;
    p->cellCount = p->rowWidth * p->diameter;
    p->state = 0;
    memset(&p->counts[0], 0, 12);
    p->cells = new GridCell[p->cellCount + 1];
    p->neighbors = new GridNeighbors[p->cellCount + 1];
    if (!p->cells || !p->neighbors) {
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        AssertFail(GridSourcePath, __LINE__, 0);
    }
    memset(p->cells, 0, (p->cellCount + 1) * 12);
    memset(p->neighbors, 0, (p->cellCount + 1) * 8);
    return 1;
}
void GridClearNeighbors(TerrainGrid *p) { memset(p->neighbors, 0, (p->cellCount + 1) * 8); }
long GridPointToIndex(TerrainGrid *grid, GridPoint *point) {
    GridCoordLocal local(*point); GridCoordLocal remainder;
    local.x=(local.x*0x128)>>8;
    remainder.Set(abs(local.x)%grid->spacing,abs(local.y)%grid->spacing);
    local.x/=grid->spacing; local.x+=grid->center.x;
    remainder.x-=grid->spacing>>2;
    if(remainder.x>0) {
        long add;
        if(remainder.x < (grid->spacing>>1)) {
            int side=0;
            if(remainder.y > (grid->spacing>>1)) { remainder.y-=grid->spacing>>1; side=1; }
            if((local.x-grid->center.x)&1)side=!side;
            add=side ? remainder.x>remainder.y : (grid->spacing>>1)-remainder.y<remainder.x;
            // Both cases produce the crossing predicate.
        } else add=1;
        if(add) { if(point->x<0)add=-1; else add=1; }
        local.x+=add;
    }
    long odd=(local.x-grid->center.x)&1;
    if(!odd) local.y+=point->y<=0?-(grid->spacing>>1):(grid->spacing>>1);
    local.y/=grid->spacing; local.y=-local.y*2;
    if(odd) local.y+=point->y<=0?1:-1;
    local.y+=grid->center.y;
    if(local.x&1)local.y|=1; else local.y&=~1;
    return GridLookup(grid,(GridPoint*)&local);



}











extern "C" GridPoint *TargetObjectInit(TerrainGrid *grid, GridPoint *out, long index) {
    GridDecode(grid,out,index);
    GridSubtract(out,&grid->center);
    GridScale(out,grid->spacing);
    out->x=(out->x*0xde)>>8;
    out->y=-out->y>>1;
    return out;
}












extern "C" int ResolveName(TerrainGrid *grid,GridPoint *point) {
    if(GridDistance(&grid->center,point)>grid->radius) return 0; return ((point->y>>1)<<grid->rowShift)+point->x+1; }








long *BestLocSetData(TerrainGrid *grid, long *out, long index) {
    --index;
    out[0]=index&(grid->rowWidth-1);
    out[1]=(index>>grid->rowShift)*2+((out[0]&1)!=0);
    return out;
}
long GridFindNeighbor(TerrainGrid *grid,long index,long wanted) {
    GridPoint origin;
    GridDecode(grid,&origin,index);
    long result;
    for(result=0; result<6; ++result) {
        GridCoordLocal point(origin);
        point.Add((const GridPoint *)&pathDirections[result]);
        long candidate;
        if(GridDistance(&grid->center,(GridPoint *)&point)>grid->radius) candidate=0;
        else candidate=((point.y>>1)<<grid->rowShift)+point.x+1;
        if(candidate==wanted) break;
    }
    return result;
}
long GridClamp(unsigned long value) {
    unsigned long divisor=0x2aaaaabc;
    long result=(value+0x1555555e)/divisor;
    if(result<=0) return 0;
    if(result>=5) return 5;
    return result;
}
void GridRotatePoint(GridPoint *point,long turns) {
    if(turns) {
        long limit=6-turns;
        for(long i=0;i<limit;++i) {
            GridPoint next;
            long middle=(point->x+point->y)>>1;
            long x=point->x;
            next.y=x+middle;
            next.x=middle-point->y;
            *point=next;
        }
    }
}
extern "C" int LogicCopyPoint(int *left, int *right) {
    long dx = GridAbs(right[0]-left[0]);
    long dy = GridAbs(right[1]-left[1]);
    return GridMax(dx, (dx+dy)>>1);
}







long GridVisitBest(void *argument) {
    long index = ((long *)argument)[3];
    if (index) {
        TerrainGrid *grid = *(TerrainGrid **)argument;
        GridPoint point;
        {
            long value=index-1;
            point.x=value&(grid->rowWidth-1);
            point.y=(value>>grid->rowShift)*2+((point.x&1)!=0);
            GridSubtract(&point,&grid->center);
            GridScale(&point,grid->spacing);
            point.x=(point.x*0xde)>>8;
            point.y=-point.y>>1;
        }


        long distance = PointAngle((ApplyPoint*)&gGridSearch.point, (ApplyPoint*)&point);
        distance = abs((distance - gGridSearch.target) >> 1);
        // target takes abs after the signed half-difference
        if (!gGridSearch.bestIndex || distance < gGridSearch.bestDistance) {
            gGridSearch.bestIndex = index;
            gGridSearch.bestDistance = distance;
        }
    }
    return 0;
}
long GridFindClosest(TerrainGrid *grid, unsigned long start, long target) {
    gGridSearch.bestIndex = 0; gGridSearch.target = target;
    {
        long value=grid->centerIndex-1;
        gGridSearch.point.x=value&(grid->rowWidth-1);
        gGridSearch.point.y=(value>>grid->rowShift)*2+((gGridSearch.point.x&1)!=0);
        GridSubtract(&gGridSearch.point,&grid->center);
        GridScale(&gGridSearch.point,grid->spacing);
        gGridSearch.point.x=(gGridSearch.point.x*0xde)>>8;
        gGridSearch.point.y=-gGridSearch.point.y>>1;
    }




    long shape[10];
    BestLocInitialize(shape, (const char *)grid, (int)start);
    BestLocSetCandidate(shape, (void *)grid->centerIndex);
    AreaEnumerate((int*)shape, (void*)GridVisitBest, start, start);
    return gGridSearch.bestIndex;
}







long GridLoad(TerrainGrid *grid, void *stream) {
    if (!grid->cells) return 0;
    long count = grid->cellCount;
    unsigned long headerBytes = (unsigned long)((char *)&grid->unknown40 - (char *)grid); int bytes = StreamRead(stream, grid, headerBytes);
    if (bytes != (int)headerBytes) return 0;
    if (count != grid->cellCount) {
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        // source-line padding
        AssertFail(GridSourcePath, __LINE__, 0);
    }
    unsigned long cellBytes = (grid->cellCount + 1) * 12;
    if (StreamRead(stream, grid->cells, cellBytes) != (int)cellBytes) return 0;
    unsigned long neighborBytes = (grid->cellCount + 1) * 8;
    if (StreamRead(stream, grid->neighbors, neighborBytes) != (int)neighborBytes) return 0;
    return 1;
}
long GridSave(TerrainGrid *grid, void *stream) {
    if (!grid->cells) return 0;
    unsigned long headerBytes = (unsigned long)((char *)&grid->unknown40 - (char *)grid); if (StreamWrite(stream, grid, headerBytes) != (int)headerBytes) return 0;
    unsigned long cellBytes = (grid->cellCount + 1) * 12;
    if (StreamWrite(stream, grid->cells, cellBytes) != (int)cellBytes) return 0;
    unsigned long neighborBytes = (grid->cellCount + 1) * 8;
    if (StreamWrite(stream, grid->neighbors, neighborBytes) != (int)neighborBytes) return 0;
    return 1;
}
long GridInvalidIndex() { GridIndex local(0); return -1; }
long GridSerializeStats(TerrainGrid *grid, void *output) {
    struct StatSlot { unsigned short count; unsigned short unused; };
    struct StatsScratch { StatSlot slot[9]; unsigned char value[4]; };
    StatsScratch scratch;
    scratch.slot[7].count = 0; scratch.slot[4].count = 0; scratch.slot[6].count = 0;
    scratch.slot[1].count = 0; scratch.slot[3].count = 0; scratch.slot[5].count = 0;
    scratch.slot[0].count = 0; scratch.slot[2].count = 0; scratch.slot[8].count = 0;
    scratch.value[0] = 0;
    long i = 0;
    // Each grid cell occupies twelve bytes.
    GridCell *cell = grid->cells;
    if (i < grid->cellCount) {
        do {
            scratch.value[0] = cell->terrain; Crc16(&scratch.slot[7].count, scratch.value, 1);
            scratch.value[0] = cell->ground; Crc16(&scratch.slot[4].count, scratch.value, 1);
            scratch.value[0] = cell->feature; Crc16(&scratch.slot[6].count, scratch.value, 1);
            scratch.value[0] = cell->height; Crc16(&scratch.slot[1].count, scratch.value, 1);
            scratch.value[0] = cell->flags; Crc16(&scratch.slot[3].count, scratch.value, 1);
            scratch.value[0] = cell->water; Crc16(&scratch.slot[5].count, scratch.value, 1);
            scratch.value[0] = cell->region; Crc16(&scratch.slot[0].count, scratch.value, 1);
            Crc16(&scratch.slot[8].count, &cell->actor, 4);
            ++i; ++cell;
        } while (i < grid->cellCount);
    }
    long *fields = (long *)output;
    fields[8] = scratch.slot[7].count; fields[11] = scratch.slot[4].count;
    fields[12] = scratch.slot[6].count; fields[13] = scratch.slot[1].count;
    fields[19] = scratch.slot[3].count; fields[15] = scratch.slot[5].count;
    fields[17] = scratch.slot[0].count; fields[20] = scratch.slot[8].count;
    return 1;
}













