struct GridPoint; struct TerrainGrid { long fields[17]; void *cells; void *neighbors; long PointToIndex(GridPoint *); };
struct GridState { long value0; long value1; long value2; long value3; long value4; long value5; long value6; long value7; long value8; long value9; long value10; long value11; };
extern GridState gGridState;
extern "C" void AssertFail(const char *, int, const char *);
extern "C" void MemoryRelease(void *);
extern "C" void *MemoryAllocationRetry(unsigned long);
extern "C" void *memset(void *, int, unsigned long);

void GridInitConfig() { long zero = 0; long minusTwo = -2; long one = 1; long minusOne = -1; gGridState.value0 = zero; gGridState.value1 = minusTwo; gGridState.value2 = one; gGridState.value3 = minusOne; gGridState.value4 = one; gGridState.value5 = one; gGridState.value6 = zero; gGridState.value8 = minusOne; gGridState.value9 = one; gGridState.value10 = minusOne; gGridState.value11 = minusOne; gGridState.value7 = 2; }

TerrainGrid *GridClear(TerrainGrid *p) { p->cells = 0; p->neighbors = 0; return p; }
TerrainGrid *GridRelease(TerrainGrid *p) { if (p->cells) { MemoryRelease(p->cells); register void *neighbors = p->neighbors; p->cells = 0; MemoryRelease(neighbors); p->neighbors = 0; } return p; }
void GridDestroy(TerrainGrid *p) { if (p->cells) { MemoryRelease(p->cells); register void *neighbors = p->neighbors; p->cells = 0; MemoryRelease(neighbors); p->neighbors = 0; } }
void GridClearNeighbors(TerrainGrid *p) { memset(p->neighbors, 0, (p->fields[10] + 1) * 8); }

long GridClamp(unsigned long value) { unsigned long divisor = 0x2aaaaabc; long result = (value + 0x1555555e) / divisor; if (result <= 0) return 0; if (result >= 5) return 5; return result; }

struct GridPoint { long x; long y; };
struct GridOffset { long x; long y; };
extern GridOffset gGridOffsets[6];
struct GridSearchState { GridPoint point; long target; long bestIndex; long bestDistance; };
extern GridSearchState gGridSearch;
int StreamRead(void *, void *, int);
int StreamWrite(void *, const void *, unsigned long);
extern "C" void StatsAdd(void *, const void *, int);
extern "C" long GridPointDistance(const GridPoint *, const GridPoint *); extern "C" int abs(int);
extern void GridShapeCreate(void *, TerrainGrid *, long);
extern void GridShapeReset(void *, long);
extern void GridShapeVisit(void *, long (*)(void *), long, long);
long GridMetric(GridPoint *a, GridPoint *b) { long dx = b->x - a->x; if (dx < 0) dx = -dx; long dy = b->y - a->y; if (dy < 0) dy = -dy; long result = (dx + dy) >> 1; if (result < dx) result = dx; return result; }
void GridGetIndexCoord(TerrainGrid *grid, unsigned long *out, int index) { unsigned long mask = grid->fields[8] - 1; unsigned long value = index - 1U; out[0] = value & mask; out[1] = ((int)value >> grid->fields[7]) * 2 + ((out[0] & 1) != 0); }
void GridToWorldCoord(TerrainGrid *grid, GridPoint *out, long index) { long value = index - 1; out->x = value & (grid->fields[8] - 1); out->y = (value >> grid->fields[7]) * 2 + ((out->x & 1) != 0); out->x -= grid->fields[4]; out->y -= grid->fields[5]; out->x *= grid->fields[1]; out->y *= grid->fields[1]; out->x = (out->x * 222) >> 8; out->y = (-out->y) >> 1; }
long GridIndexFromCoord(TerrainGrid *grid, GridPoint *point) { long dx = grid->fields[4] - point->x; if (dx < 0) dx = -dx; long dy = grid->fields[5] - point->y; if (dy < 0) dy = -dy; long distance = (dy + dx) >> 1; if (distance < dx) distance = dx; if (grid->fields[2] < distance) return 0; return ((point->y >> 1) << grid->fields[7]) + point->x + 1; }
long GridFindNeighbor(TerrainGrid *grid, long index, long wanted) { unsigned long x = (index - 1) & (grid->fields[8] - 1); long result = 0; for (long i = 0; i < 6; ++i) { GridPoint point; point.x = x + gGridOffsets[i].x; point.y = (((index - 1) >> grid->fields[7]) * 2) + ((x & 1) != 0) + gGridOffsets[i].y; long candidate = GridIndexFromCoord(grid, &point); if (candidate == wanted) break; ++result; if (result > 5) return result; } return result; }
void GridRotatePoint(GridPoint *point, long turns) { if (turns) { long limit = 6 - turns; for (long i = 0; i < limit; ++i) { GridPoint next; long middle = (point->x + point->y) >> 1; long x = point->x; next.y = x + middle; next.x = middle - point->y; *point = next; } } }
long GridInvalidIndex() { volatile long local[1]; register long result = -1; if (result) ((volatile unsigned short *)local)[0] = 0; return result; }

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
        AssertFail(__FILE__, __LINE__, 0);
    }
    memset(p->cells, 0, (p->fields[10] + 1) * 12);
    memset(p->neighbors, 0, (p->fields[10] + 1) * 8);
    return 1;
}

long GridPointToIndex(TerrainGrid *grid, GridPoint *point) {
    GridPoint input = *point;
    long x = (input.x * 0x128) >> 8;
    long y = input.y;
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

long GridVisitBest(void *argument) {
    long index = ((long *)argument)[3];
    if (index) {
        TerrainGrid *grid = *(TerrainGrid **)argument;
        GridPoint point;
        unsigned long x = (index - 1) & (grid->fields[8] - 1);
        point.x = ((x - grid->fields[4]) * grid->fields[1] * 0xde) >> 8;
        point.y = -(((((index - 1) >> grid->fields[7]) * 2 + ((x & 1) != 0)) - grid->fields[5]) * grid->fields[1]) >> 1;
        long distance = GridPointDistance(&gGridSearch.point, &point);
        distance = distance - (gGridSearch.target >> 1);
        if (distance < 0) distance = -distance;
        if (!gGridSearch.bestIndex || distance < gGridSearch.bestDistance) {
            gGridSearch.bestIndex = index;
            gGridSearch.bestDistance = distance;
        }
    }
    return 0;
}

long GridFindClosest(TerrainGrid *grid, unsigned long start, long target) {
    gGridSearch.bestIndex = 0;
    gGridSearch.target = target;
    unsigned long value = grid->fields[6] - 1;
    unsigned long x = value & (grid->fields[8] - 1);
    gGridSearch.point.x = ((x - grid->fields[4]) * grid->fields[1] * 0xde) >> 8;
    gGridSearch.point.y = -(((((long)value >> grid->fields[7]) * 2 + ((x & 1) != 0)) - grid->fields[5]) * grid->fields[1]) >> 1;
    long shape[10];
    GridShapeCreate(shape, grid, start);
    GridShapeReset(shape, grid->fields[6]);
    GridShapeVisit(shape, GridVisitBest, start, start);
    return gGridSearch.bestIndex;
}

long GridLoad(TerrainGrid *grid, void *stream) {
    if (!grid->cells) return 0;
    long count = grid->fields[10];
    int bytes = StreamRead(stream, grid, 0x40);
    if (bytes != 0x40) return 0;
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
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
// source-line padding
        AssertFail(__FILE__, __LINE__, 0);
    }
    unsigned long cellBytes = (grid->fields[10] + 1) * 12;
    if (StreamRead(stream, grid->cells, cellBytes) != (int)cellBytes) return 0;
    unsigned long neighborBytes = (grid->fields[10] + 1) * 8;
    if (StreamRead(stream, grid->neighbors, neighborBytes) != (int)neighborBytes) return 0;
    return 1;
}

long GridSave(TerrainGrid *grid, void *stream) {
    if (!grid->cells) return 0;
    if (StreamWrite(stream, grid, 0x40) != 0x40) return 0;
    unsigned long cellBytes = (grid->fields[10] + 1) * 12;
    if (StreamWrite(stream, grid->cells, cellBytes) != (int)cellBytes) return 0;
    unsigned long neighborBytes = (grid->fields[10] + 1) * 8;
    if (StreamWrite(stream, grid->neighbors, neighborBytes) != (int)neighborBytes) return 0;
    return 1;
}

long GridSerializeStats(TerrainGrid *grid, void *output) {
    unsigned short stats[9] = {0,0,0,0,0,0,0,0,0};
    unsigned char value[4] = {0,0,0,0};
    for (long i = 0; i < grid->fields[10]; ++i) {
        unsigned char *cell = (unsigned char *)grid->cells + i * 12;
        value[0] = cell[0]; StatsAdd(&stats[0], value, 1);
        value[0] = cell[2]; StatsAdd(&stats[1], value, 1);
        value[0] = cell[3]; StatsAdd(&stats[2], value, 1);
        value[0] = cell[4]; StatsAdd(&stats[3], value, 1);
        value[0] = cell[5]; StatsAdd(&stats[4], value, 1);
        value[0] = cell[1]; StatsAdd(&stats[5], value, 1);
        value[0] = cell[6]; StatsAdd(&stats[6], value, 1);
        StatsAdd(&stats[7], cell + 8, 4);
    }
    long *fields = (long *)output;
    fields[8] = stats[0]; fields[11] = stats[1]; fields[12] = stats[2]; fields[13] = stats[3];
    fields[19] = stats[4]; fields[15] = stats[5]; fields[17] = stats[6]; fields[20] = stats[7];
    return 1;
}
