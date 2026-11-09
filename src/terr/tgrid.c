// terr\tgrid.c. Names and partial field layouts remain working labels.
struct GridPoint { long x, y; };
struct TerrainGrid { long fields[17]; void *cells, *neighbors; long PointToIndex(GridPoint *); };
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
    out->x=value&(grid->fields[8]-1);
    out->y=(value>>grid->fields[7])*2+((out->x&1)!=0);
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
inline long GridLookup(TerrainGrid *grid, const GridPoint *point) {
    long dx=GridAbs(grid->fields[4]-point->x);
    long dy=GridAbs(grid->fields[5]-point->y);
    long distance=GridMax(dx,(dx+dy)>>1);
    if(distance>grid->fields[2]) return 0;
    return ((point->y>>1)<<grid->fields[7])+point->x+1;
}

// Direction table is initialized by the global point constructors.
TerrainGrid *GridClear(TerrainGrid *p) { p->cells = 0; p->neighbors = 0; return p; }
TerrainGrid *GridRelease(TerrainGrid *p) {
    if (p->cells) {
        delete[] (char *)p->cells; p->cells=0;
        delete[] (char *)p->neighbors; p->neighbors=0;
    }
    return p;
}
void GridDestroy(TerrainGrid *p) {
    if (p->cells) {
        delete[] (char *)p->cells; p->cells=0;
        delete[] (char *)p->neighbors; p->neighbors=0;
    }
}
long GridInitialize(TerrainGrid *p, long spacing, long height) {
    // dimensions are recomputed through the grid fields.
    if (p->cells) {
        delete[] (char *)p->cells;
        p->cells = 0;
        delete[] (char *)p->neighbors;
        p->neighbors = 0;
    }
    p->fields[1] = spacing;
    p->fields[2] = height;
    long doubled = height * 2;
    p->fields[3] = doubled + 1;
    p->fields[4] = p->fields[2];
    p->fields[5] = p->fields[2] * 2 + ((p->fields[4] & 1) != 0);
    p->fields[7] = 0;
    for (long value = doubled; value > 0; value >>= 1) ++p->fields[7];
    p->fields[7] = GridMax(p->fields[7],1);
    p->fields[6] = (p->fields[2] << p->fields[7]) + p->fields[2] + 1;
    long power = GridPower(p->fields[7]);
    p->fields[8] = power;
    p->fields[9] = p->fields[8] - p->fields[3];
    p->fields[10] = p->fields[8] * p->fields[3];
    p->fields[11] = 0;
    memset(&p->fields[13], 0, 12);
    p->cells = new char[(p->fields[10] + 1) * 12];
    p->neighbors = new char[(p->fields[10] + 1) * 8];
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
    memset(p->cells, 0, (p->fields[10] + 1) * 12);
    memset(p->neighbors, 0, (p->fields[10] + 1) * 8);
    return 1;
}
void GridClearNeighbors(TerrainGrid *p) { memset(p->neighbors, 0, (p->fields[10] + 1) * 8); }
long GridPointToIndex(TerrainGrid *grid, GridPoint *point) {
    GridCoordLocal local(*point); GridCoordLocal remainder;
    local.x=(local.x*0x128)>>8;
    remainder.Set(abs(local.x)%grid->fields[1],abs(local.y)%grid->fields[1]);
    local.x/=grid->fields[1]; local.x+=grid->fields[4];
    remainder.x-=grid->fields[1]>>2;
    if(remainder.x>0) {
        long add;
        if(remainder.x < (grid->fields[1]>>1)) {
            int side=0;
            if(remainder.y > (grid->fields[1]>>1)) { remainder.y-=grid->fields[1]>>1; side=1; }
            if((local.x-grid->fields[4])&1)side=!side;
            add=side ? remainder.x>remainder.y : (grid->fields[1]>>1)-remainder.y<remainder.x;
            // Both cases produce the crossing predicate.
        } else add=1;
        if(add) { if(point->x<0)add=-1; else add=1; }
        local.x+=add;
    }
    long odd=(local.x-grid->fields[4])&1;
    if(!odd) local.y+=point->y<=0?-(grid->fields[1]>>1):(grid->fields[1]>>1);
    local.y/=grid->fields[1]; local.y=-local.y*2;
    if(odd) local.y+=point->y<=0?1:-1;
    local.y+=grid->fields[5];
    if(local.x&1)local.y|=1; else local.y&=~1;
    return GridLookup(grid,(GridPoint*)&local);



}











extern "C" GridPoint *TargetObjectInit(TerrainGrid *grid, GridPoint *out, long index) {
    GridDecode(grid,out,index);
    GridSubtract(out,(GridPoint*)&grid->fields[4]);
    GridScale(out,grid->fields[1]);
    out->x=(out->x*0xde)>>8;
    out->y=-out->y>>1;
    return out;
}












int ResolveName(const char *gridArg,const char *pointArg) { return GridLookup((TerrainGrid*)gridArg,(GridPoint*)pointArg); }









long *BestLocSetData(TerrainGrid *grid, long *out, long index) {
    --index;
    out[0]=index&(grid->fields[8]-1);
    out[1]=(index>>grid->fields[7])*2+((out[0]&1)!=0);
    return out;
}
long GridFindNeighbor(TerrainGrid *grid,long index,long wanted) {
    GridPoint origin;
    long value=index-1;
    origin.x=value&(grid->fields[8]-1);
    origin.y=(value>>grid->fields[7])*2+((origin.x&1)!=0);
    long result=0;
    Point *offset=pathDirections;
    while(result<6) {
        GridPoint point=origin;
        point.x+=offset->x; point.y+=offset->y;
        long dx=GridAbs(grid->fields[4]-point.x);
        long dy=GridAbs(grid->fields[5]-point.y);
        long distance=GridMax(dx,(dx+dy)>>1);
        long candidate;
        if(distance>grid->fields[2]) candidate=0;
        else candidate=((point.y>>1)<<grid->fields[7])+point.x+1;
        if(candidate==wanted) break;
        ++result; ++offset;
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
            point.x=value&(grid->fields[8]-1);
            point.y=(value>>grid->fields[7])*2+((point.x&1)!=0);
            GridSubtract(&point,(GridPoint*)&grid->fields[4]);
            GridScale(&point,grid->fields[1]);
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
        long value=grid->fields[6]-1;
        gGridSearch.point.x=value&(grid->fields[8]-1);
        gGridSearch.point.y=(value>>grid->fields[7])*2+((gGridSearch.point.x&1)!=0);
        GridSubtract(&gGridSearch.point,(GridPoint*)&grid->fields[4]);
        GridScale(&gGridSearch.point,grid->fields[1]);
        gGridSearch.point.x=(gGridSearch.point.x*0xde)>>8;
        gGridSearch.point.y=-gGridSearch.point.y>>1;
    }




    long shape[10];
    BestLocInitialize(shape, (const char *)grid, (int)start);
    BestLocSetCandidate(shape, (void *)grid->fields[6]);
    AreaEnumerate((int*)shape, (void*)GridVisitBest, start, start);
    return gGridSearch.bestIndex;
}
long GridLoad(TerrainGrid *grid, void *stream) {
    if (!grid->cells) return 0;
    long count = grid->fields[10];
    unsigned long headerBytes = (unsigned long)((char *)&grid->fields[16] - (char *)grid); int bytes = StreamRead(stream, grid, headerBytes);
    if (bytes != (int)headerBytes) return 0;
    if (count != grid->fields[10]) {
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
    unsigned long cellBytes = (grid->fields[10] + 1) * 12;
    if (StreamRead(stream, grid->cells, cellBytes) != (int)cellBytes) return 0;
    unsigned long neighborBytes = (grid->fields[10] + 1) * 8;
    if (StreamRead(stream, grid->neighbors, neighborBytes) != (int)neighborBytes) return 0;
    return 1;
}
long GridSave(TerrainGrid *grid, void *stream) {
    if (!grid->cells) return 0;
    unsigned long headerBytes = (unsigned long)((char *)&grid->fields[16] - (char *)grid); if (StreamWrite(stream, grid, headerBytes) != (int)headerBytes) return 0;
    unsigned long cellBytes = (grid->fields[10] + 1) * 12;
    if (StreamWrite(stream, grid->cells, cellBytes) != (int)cellBytes) return 0;
    unsigned long neighborBytes = (grid->fields[10] + 1) * 8;
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
    unsigned char *cell = (unsigned char *)grid->cells;
    if (i < grid->fields[10]) {
        do {
            scratch.value[0] = cell[0]; Crc16(&scratch.slot[7].count, scratch.value, 1);
            scratch.value[0] = cell[2]; Crc16(&scratch.slot[4].count, scratch.value, 1);
            scratch.value[0] = cell[3]; Crc16(&scratch.slot[6].count, scratch.value, 1);
            scratch.value[0] = cell[4]; Crc16(&scratch.slot[1].count, scratch.value, 1);
            scratch.value[0] = cell[5]; Crc16(&scratch.slot[3].count, scratch.value, 1);
            scratch.value[0] = cell[1]; Crc16(&scratch.slot[5].count, scratch.value, 1);
            scratch.value[0] = cell[6]; Crc16(&scratch.slot[0].count, scratch.value, 1);
            Crc16(&scratch.slot[8].count, cell+8, 4);
            ++i; cell += 12;
        } while (i < grid->fields[10]);
    }
    long *fields = (long *)output;
    fields[8] = scratch.slot[7].count; fields[11] = scratch.slot[4].count;
    fields[12] = scratch.slot[6].count; fields[13] = scratch.slot[1].count;
    fields[19] = scratch.slot[3].count; fields[15] = scratch.slot[5].count;
    fields[17] = scratch.slot[0].count; fields[20] = scratch.slot[8].count;
    return 1;
}













