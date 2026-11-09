struct GridPoint; struct TerrainGrid { long fields[17]; void *cells; void *neighbors; long PointToIndex(GridPoint *); };
struct GridState { long value0; long value1; long value2; long value3; long value4; long value5; long value6; long value7; long value8; long value9; long value10; long value11; };
extern GridState gGridState;
extern "C" void AssertFail(const char *, int, const char *); static const char GridSourcePath[] = "terr\x5ctgrid.c";
extern "C" void MemoryRelease(void *);
extern "C" void *MemoryAllocationRetry(unsigned long);
extern "C" void *memset(void *, int, unsigned long);
struct GridPoint { long x; long y; };
struct GridOffset { long x; long y; };
extern GridOffset gGridOffsets[6];
struct GridSearchState { GridPoint point; long target; long bestIndex; long bestDistance; };
extern GridSearchState gGridSearch;
int StreamRead(void *, void *, int);
int StreamWrite(void *, const void *, unsigned long);
extern "C" void StatsAdd(void *, const void *, int);
extern "C" long GridPointDistance(const GridPoint *, const GridPoint *); extern "C" int abs(int);
extern int BestLocInitialize(void *, const char *, int);
extern int BestLocSetCandidate(void *, void *);
extern void GridShapeVisit(void *, long (*)(void *), long, long);

void GridInitConfig() { long zero = 0; long minusTwo = -2; long one = 1; long minusOne = -1; gGridState.value0 = zero; gGridState.value1 = minusTwo; gGridState.value2 = one; gGridState.value3 = minusOne; gGridState.value4 = one; gGridState.value5 = one; gGridState.value6 = zero; gGridState.value8 = minusOne; gGridState.value9 = one; gGridState.value10 = minusOne; gGridState.value11 = minusOne; gGridState.value7 = 2; }
TerrainGrid *GridClear(TerrainGrid *p) { p->cells = 0; p->neighbors = 0; return p; }
TerrainGrid *GridRelease(TerrainGrid *p) { if (p->cells) { MemoryRelease(p->cells); register void *neighbors = p->neighbors; p->cells = 0; MemoryRelease(neighbors); p->neighbors = 0; } return p; }
void GridDestroy(TerrainGrid *p) { if (p->cells) { MemoryRelease(p->cells); register void *neighbors = p->neighbors; p->cells = 0; MemoryRelease(neighbors); p->neighbors = 0; } }
long GridInitialize(TerrainGrid *p, long spacing, long height) {
    volatile long one;
    if (p->cells) {
        MemoryRelease(p->cells);
        p->cells = 0;
        MemoryRelease(p->neighbors);
        p->neighbors = 0;
    }
    p->fields[1] = spacing;
    p->fields[2] = height;
    long doubled = height * 2;
    p->fields[3] = doubled + 1;
    p->fields[4] = height;
    p->fields[5] = doubled + ((height & 1) != 0);
    p->fields[7] = 0;
    for (long value = doubled; value > 0; value >>= 1) ++p->fields[7];
    one = 1; if (p->fields[7] < one) p->fields[7] = one;
    p->fields[6] = (height << p->fields[7]) + height + 1;
    long power = 1 << p->fields[7];
    p->fields[8] = power;
    p->fields[9] = power - p->fields[3];
    p->fields[10] = power * p->fields[3];
    p->fields[11] = 0;
    memset(&p->fields[13], 0, 12);
    p->cells = MemoryAllocationRetry((p->fields[10] + 1) * 12);
    p->neighbors = MemoryAllocationRetry((p->fields[10] + 1) * 8);
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
    memset(p->cells, 0, (p->fields[10] + 1) * 12);
    memset(p->neighbors, 0, (p->fields[10] + 1) * 8);
    return 1;
}
void GridClearNeighbors(TerrainGrid *p) { memset(p->neighbors, 0, (p->fields[10] + 1) * 8); }
long GridPointToIndex(TerrainGrid *grid, GridPoint *point) {
    long inputX = point->x; long inputY = point->y;
    long x = (inputX * 0x128) >> 8;
    long y = inputY;
    long row = abs(y);
    row %= grid->fields[1];
    long column = abs(x);
    
    long cellX = x / grid->fields[1] + grid->fields[4];
    long delta = column % grid->fields[1] - (grid->fields[1] >> 2);
    if (delta > 0) {
        long half = grid->fields[1] >> 1;
        long adjust = 0;
        if (delta < half) {
            adjust = row;
            if (half < row) adjust = row - half;
            long side = (half < row);
            if (((cellX - grid->fields[4]) & 1) != 0) side = !side;
            if (!side) adjust = half - adjust;
            side = adjust < delta;
            if (side) cellX += point->x < 0 ? -1 : 1;
        } else cellX += point->x < 0 ? -1 : 1;
    }
    long odd = (cellX - grid->fields[4]) & 1;
    long outputY = y;
    if (!odd) outputY += point->y < 1 ? -(grid->fields[1] >> 1) : (grid->fields[1] >> 1);
    outputY = (outputY / grid->fields[1]) * -2;
    if (odd) outputY += point->y < 1 ? 1 : -1;
    outputY += grid->fields[5];
    if ((cellX & 1) == 0) outputY &= 0xfffffffe;
    else outputY |= 1;
    long dx = grid->fields[4] - cellX;
    if (dx < 0) dx = -dx;
    long dy = grid->fields[5] - outputY;
    if (dy < 0) dy = -dy;
    long distance = (dy + dx) >> 1;
    if (distance < dx) distance = dx;
    if (grid->fields[2] < distance) return 0;
    return (((outputY >> 1) << grid->fields[7]) + cellX + 1);
}
extern "C" void TargetObjectInit(int gridArg, unsigned long *out, int index) {
    int *grid = (int *)gridArg;
    unsigned long value = (unsigned long)index - 1U;
    out[0] = value & (unsigned long)(grid[8] - 1);
    out[1] = ((int)value >> grid[7]) * 2 + ((out[0] & 1) != 0);
    out[0] -= grid[4];
    out[1] -= grid[5];
    int scale = grid[1];
    out[0] *= scale;
    out[1] *= scale;
    out[0] = (int)(out[0] * 0xde) >> 8;
    out[1] = (int)-out[1] >> 1;
}
int ResolveName(const char *gridArg, const char *pointArg) {
    int *grid = (int *)gridArg; int *point = (int *)pointArg;
    int dx = grid[4] - point[0]; if (dx < 0) dx = -dx;
    int dy = grid[5] - point[1]; if (dy < 0) dy = -dy;
    int average = (dy + dx) >> 1;
    volatile int savedAverage = average;
    if (savedAverage < dx) savedAverage = dx;
    if (grid[2] < savedAverage) return 0;
    return ((point[1] >> 1) << ((unsigned char *)gridArg)[0x1c]) + point[0] + 1;
}
void BestLocSetData(void *gridArg, void *outArg, void *indexArg) { TerrainGrid *grid = (TerrainGrid *)gridArg; unsigned long *out = (unsigned long *)outArg; unsigned long mask = grid->fields[8] - 1; unsigned long value = (unsigned long)indexArg - 1U; out[0] = value & mask; out[1] = ((int)value >> grid->fields[7]) * 2 + ((out[0] & 1) != 0); }
long GridFindNeighbor(TerrainGrid *grid, long index, long wanted) { volatile unsigned long x = (index - 1) & (grid->fields[8] - 1); long row = (((index - 1) >> grid->fields[7]) * 2) + ((x & 1) != 0); long result = 0; GridOffset *offset = gGridOffsets; while (result < 6) { GridPoint point; point.x = x + offset->x; point.y = row + offset->y; long dx = grid->fields[4] - point.x; if (dx < 0) dx = -dx; long dy = grid->fields[5] - point.y; if (dy < 0) dy = -dy; volatile long distance = (dy + dx) >> 1; if (dx <= distance) dx = distance; long candidate; if (grid->fields[2] < dx) candidate = 0; else candidate = ((point.y >> 1) << grid->fields[7]) + point.x + 1; if (candidate == wanted) break; ++result; ++offset; } return result; }
long GridClamp(unsigned long value) { unsigned long divisor = 0x2aaaaabc; long result = (value + 0x1555555e) / divisor; if (result <= 0) return 0; if (result >= 5) return 5; return result; }
void GridRotatePoint(GridPoint *point, long turns) { if (turns) { long limit = 6 - turns; for (long i = 0; i < limit; ++i) { GridPoint next; long middle = (point->x + point->y) >> 1; long x = point->x; next.y = x + middle; next.x = middle - point->y; *point = next; } } }
extern "C" int LogicCopyPoint(int *left, int *right) {
    int dx = left[0] - right[0]; if (dx < 0) dx = -dx;
    int dy = left[1] - right[1]; if (dy < 0) dy = -dy;
    volatile int sum = dx + dy;
    int average = sum >> 1;
    if (average < dx) average = dx;
    return average;
}
long GridVisitBest(void *argument) {
    long index = ((long *)argument)[3];
    if (index) {
        TerrainGrid *grid = *(TerrainGrid **)argument;
        GridPoint point;
        unsigned long x = (index - 1) & (grid->fields[8] - 1);
        point.x = (((long)x - grid->fields[4]) * grid->fields[1] * 0xde) >> 8;
        point.y = -(((((index - 1) >> grid->fields[7]) * 2 + ((x & 1) != 0)) - grid->fields[5]) * grid->fields[1]) >> 1;
        long distance = GridPointDistance(&gGridSearch.point, &point);
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
    unsigned long value = grid->fields[6] - 1;
    gGridSearch.point.x = value & (grid->fields[8] - 1);
    gGridSearch.point.y = ((long)value >> grid->fields[7]) * 2 + ((gGridSearch.point.x & 1) != 0);
    gGridSearch.point.x = ((gGridSearch.point.x - grid->fields[4]) * grid->fields[1] * 0xde) >> 8;
    gGridSearch.point.y = -((gGridSearch.point.y - grid->fields[5]) * grid->fields[1]) >> 1;
    long shape[10];
    BestLocInitialize(shape, (const char *)grid, (int)start);
    BestLocSetCandidate(shape, (void *)grid->fields[6]);
    GridShapeVisit(shape, GridVisitBest, start, start);
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
long GridInvalidIndex() { volatile long local[1]; register long result = -1; if (result) ((volatile unsigned short *)local)[0] = 0; return result; }
long GridSerializeStats(TerrainGrid *grid, void *output) {
    struct StatSlot { unsigned short count; unsigned short unused; };
    struct StatsScratch { StatSlot slot[9]; unsigned char value[4]; };
    StatsScratch scratch;
    scratch.slot[7].count = 0; scratch.slot[4].count = 0; scratch.slot[6].count = 0;
    scratch.slot[1].count = 0; scratch.slot[3].count = 0; scratch.slot[5].count = 0;
    scratch.slot[0].count = 0; scratch.slot[2].count = 0; scratch.slot[8].count = 0;
    scratch.value[0] = 0;
    unsigned char *cell = (unsigned char *)grid->cells;
    unsigned char *wide = cell + 8;
    long i = 0;
    if (i < grid->fields[10]) {
        do {
            scratch.value[0] = cell[0]; StatsAdd(&scratch.slot[7].count, scratch.value, 1);
            scratch.value[0] = cell[2]; StatsAdd(&scratch.slot[4].count, scratch.value, 1);
            scratch.value[0] = cell[3]; StatsAdd(&scratch.slot[6].count, scratch.value, 1);
            scratch.value[0] = cell[4]; StatsAdd(&scratch.slot[1].count, scratch.value, 1);
            scratch.value[0] = cell[5]; StatsAdd(&scratch.slot[3].count, scratch.value, 1);
            scratch.value[0] = cell[1]; StatsAdd(&scratch.slot[5].count, scratch.value, 1);
            scratch.value[0] = cell[6]; StatsAdd(&scratch.slot[0].count, scratch.value, 1);
            StatsAdd(&scratch.slot[8].count, wide, 4);
            ++i; cell += 12; wide += 12;
        } while (i < grid->fields[10]);
    }
    long *fields = (long *)output;
    fields[8] = scratch.slot[7].count; fields[11] = scratch.slot[4].count;
    fields[12] = scratch.slot[6].count; fields[13] = scratch.slot[1].count;
    fields[19] = scratch.slot[3].count; fields[15] = scratch.slot[5].count;
    fields[17] = scratch.slot[0].count; fields[20] = scratch.slot[8].count;
    return 1;
}
