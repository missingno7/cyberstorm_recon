struct FogSource { long unused[10]; long count; };
struct FogBuffer { long mode; FogSource *source; long count; signed char *lower; signed char *upper; long state; int Resize(); void Initialize(int mode = 0, int initialize = 1);  };
extern "C" void AssertFail(const char *, int, const char *); static const char FogSourcePath[] = "terr\\fog.c"; extern "C" void MemoryRelease(void *); extern "C" void *MemoryAllocationRetry(unsigned long); extern "C" void *memcpy(void *, const void *, unsigned long); extern "C" void *memset(void *, int, unsigned long); void FogInitialize(FogBuffer *, int mode = 0, int initialize = 1);


void FogNoOp() {}
FogBuffer *FogSetSource(FogBuffer *p, FogSource *source) { p->lower = 0; p->upper = 0; p->source = source; return p; }
void FogRelease(FogBuffer *p) { if (p->lower) MemoryRelease(p->lower); if (p->upper) MemoryRelease(p->upper); p->lower = 0; p->upper = 0; }






















































int FogBuffer::Resize() { FogBuffer *releaseTarget = this; if (releaseTarget->lower) MemoryRelease(releaseTarget->lower); if (releaseTarget->upper) MemoryRelease(releaseTarget->upper); releaseTarget->lower = 0; releaseTarget->upper = 0; count = source->count; lower = (signed char *)MemoryAllocationRetry(count + 1); upper = (signed char *)MemoryAllocationRetry(count + 1); if (!lower || !upper) AssertFail(FogSourcePath, __LINE__, 0); Initialize(); return 1; }









void FogCopy(FogBuffer *dst, FogBuffer *src) { if (!dst->lower || !dst->upper) AssertFail(FogSourcePath, __LINE__, 0); memcpy(dst->lower, src->lower, dst->count + 1); memcpy(dst->upper, src->upper, dst->count + 1); dst->mode = src->mode; }

void FogMerge(FogBuffer *dst, FogBuffer *src) { dst->mode = 0; for (long i = 0; i <= dst->count; ++i) { if (dst->lower[i] > src->lower[i]) dst->lower[i] = src->lower[i]; if (dst->upper[i] > src->upper[i]) dst->upper[i] = src->upper[i]; } }

void FogBuffer::Initialize(int mode, int initialize) { volatile long *modeField = (volatile long *)&this->mode; *modeField = mode; long effectiveMode = *modeField; this->state = 1; int fillLower = 0; switch (effectiveMode) { case 1: case 3: case 4: fillLower = 1; break; case 5: fillLower = 0; break; } int fillUpper = (effectiveMode == 4); if (fillLower || initialize) memset(this->lower, fillLower, this->count + 1); if (fillUpper || initialize) memset(this->upper, fillUpper, this->count + 1); }

void FogResetMode(FogBuffer *p) { if (p->mode == 4 || p->mode == 3 || p->mode == 2 || p->mode == 5) return; p->state = 1; memset(p->upper, 0, p->count + 1); }

int FogSetValue(FogBuffer *p, long index, int value) { register long v; signed char *lower; if (p->mode == 4) goto unchanged; if (p->mode == 5) goto unchanged; v = (value > 0); lower = p->lower; if (lower[index] != v) goto changed; if (p->upper[index] == v) goto unchanged; changed: p->state = 1; p->lower[index] = (signed char)v; p->upper[index] = (signed char)v; return 1; unchanged: return 0; }

struct TerrainGrid { long fields[17]; void *cells; void *neighbors; };
struct GridPoint { long x; long y; };
struct FogCellRecord { unsigned char raw[12]; };
struct FogObject { unsigned char raw[0x90]; };
struct FogCastGlobals { FogObject *owner; FogBuffer *active; union { long flagsWord; unsigned char flags; }; long index; GridPoint source; GridPoint target; unsigned char mask[64]; long limit; };
extern FogCastGlobals gFogCast;
extern unsigned char *gTerrainCellData;
extern long gTerrainCellState;
extern long gFogScale[16];
extern unsigned char gFogMaskWidths[6];
extern long gFogGlobalMode;
void GridToWorldCoord(TerrainGrid *, GridPoint *, long);
long GridMetric(GridPoint *, GridPoint *);
long GridPointDistance(const GridPoint *, const GridPoint *);
long GridIndexFromCoord(TerrainGrid *, GridPoint *);
void GridShapeCreate(void *, TerrainGrid *, long);
void GridShapeReset(void *, long);
void GridShapeVisit(void *, long (*)(void *), long, long);
void GridGetIndexCoord(TerrainGrid *, GridPoint *, long);
int StreamRead(void *, void *, int);
int StreamWrite(void *, const void *, unsigned long);
extern "C" FogObject *FogObjectLookup(long);

long FogUpdateCell(void *argument) {
    long index = ((long *)argument)[3];
    if (index) {
        volatile unsigned long visible = (gFogCast.limit <= 7);
        unsigned long band;
        char blocked = 0;
        if (visible) {
            GridPoint coordinate;
            GridToWorldCoord(*(TerrainGrid **)argument, &coordinate, index);
            long distance = GridPointDistance(&gFogCast.target, &coordinate);
            band = ((unsigned long)distance + 0x1fU) >> 0x1a;
            long range = GridMetric(&gFogCast.source, (GridPoint *)((unsigned char *)argument + sizeof(long)));
            blocked = gFogCast.mask[band];
            if (range < 0) blocked = 0;
        }
        if (!blocked) {
            if (gFogGlobalMode == 0) {
                gFogCast.active->upper[index] = 1;
                gFogCast.active->lower[index] = gFogCast.active->upper[index];
            } else {
                long objectId = ((long *)*(TerrainGrid **)argument)[18 + index * 2];
                FogObject *object = objectId ? FogObjectLookup(objectId) : 0;
                if (object && object->raw[0x88] && index == *(long *)(object->raw + 0x42) && gTerrainCellState != *(long *)(object->raw + 8)) gFogCast.flags |= 4;
            }
            if (visible && gFogCast.limit <= ((unsigned char *)((*(TerrainGrid **)argument)->cells))[index * 12 + 1]) {
                if (gFogCast.index > 0 && gFogCast.index < 6) {
                    unsigned long count = gFogMaskWidths[gFogCast.index];
                    long start = ((long)(index & 63)) - ((long)count >> 1);
                    if (start < 0) start += 64;
                    while (count--) {
                        long next = start + 1;
                        gFogCast.mask[start] = 1;
                        start = next;
                        if (start > 63) start = 0;
                    }
                } else gFogCast.mask[band] = 1;
            }
        }
    }
    return 0;
}

long FogCast(FogBuffer *buffer, long index, FogObject *object) {
    if (buffer->mode == 4 || buffer->mode == 5) return 0;
    gFogCast.flagsWord = 0;
    buffer->state = 1;
    if (index && *(long *)(object->raw + 0x1f) > 0) {
        gFogCast.active = buffer;
        gFogCast.owner = object;
        gFogCast.index = index;
        GridGetIndexCoord((TerrainGrid *)buffer->source, &gFogCast.source, index);
        GridToWorldCoord((TerrainGrid *)buffer->source, &gFogCast.target, gFogCast.index);
        long shape[10];
        GridShapeCreate(shape, (TerrainGrid *)buffer->source, *(long *)(object->raw + 0x1f));
        GridShapeReset(shape, gFogCast.index);
        long half = (gTerrainCellState + 500) / 2;
        long scale = gFogScale[*(long *)(object->raw + 0x23)];
        memset(gFogCast.mask, 0, 0x40);
        gFogCast.limit = ((unsigned char *)((TerrainGrid *)buffer->source)->cells)[gFogCast.index * 12 + 1] + (scale * 500 + half / 2) / half;
        GridShapeVisit(shape, FogUpdateCell, index, index);
        gFogCast.flagsWord |= 3;
    }
    return gFogCast.flagsWord;
}

void FogCastAlternate(FogBuffer *buffer, long index, FogObject *object) { gFogGlobalMode = 1; FogCast(buffer, index, object); gFogGlobalMode = 0; }

int FogRead(FogBuffer *p, void *stream) {
    int count;
    int bytes = StreamRead(stream, &count, 4);
    if (bytes != 4 || count != p->count) return 0;
    int size = p->count + 1;
    if (StreamRead(stream, p->lower, size) != size) return 0;
    if (StreamRead(stream, p->upper, size) != size) return 0;
    return 1;
}

int FogWrite(FogBuffer *p, void *stream) {
    int bytes = StreamWrite(stream, &p->count, 4);
    if (bytes != 4) return 0;
    unsigned long size = p->count + 1;
    if (StreamWrite(stream, p->lower, size) != (int)size) return 0;
    if (StreamWrite(stream, p->upper, size) != (int)size) return 0;
    return 1;
}
