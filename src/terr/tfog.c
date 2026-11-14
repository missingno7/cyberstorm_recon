struct FogSource { long unused[10]; long count; };
struct FogBuffer { long mode; FogSource *source; long count; signed char *lower; signed char *upper; long state; int Resize(); void Initialize(int mode = 0, int initialize = 1);  };
extern "C" void AssertFail(const char *, int, const char *); static const char FogSourcePath[] = "terr\\tfog.c"; extern "C" void *memcpy(void *, const void *, unsigned long); extern "C" void *memset(void *, int, unsigned long); void FogInitialize(FogBuffer *, int mode = 0, int initialize = 1);


void FogNoOp() {}
FogBuffer *FogSetSource(FogBuffer *p, FogSource *source) { p->lower = 0; p->upper = 0; p->source = source; return p; }
void FogRelease(FogBuffer *p) { if (p->lower) delete[] p->lower; if (p->upper) delete[] p->upper; p->lower = 0; p->upper = 0; }






















































int FogBuffer::Resize() { FogBuffer *releaseTarget = this; if (releaseTarget->lower) delete[] releaseTarget->lower; if (releaseTarget->upper) delete[] releaseTarget->upper; releaseTarget->lower = 0; releaseTarget->upper = 0; count = source->count; lower = (signed char *)new char[count + 1]; upper = (signed char *)new char[count + 1]; if (!lower || !upper) AssertFail(FogSourcePath, __LINE__, 0); Initialize(); return 1; }









void FogCopy(FogBuffer *dst, FogBuffer *src) { if (!dst->lower || !dst->upper) AssertFail(FogSourcePath, __LINE__, 0); memcpy(dst->lower, src->lower, dst->count + 1); memcpy(dst->upper, src->upper, dst->count + 1); dst->mode = src->mode; }

void FogMerge(FogBuffer *dst, FogBuffer *src) { dst->mode = 0; for (long i = 0; i <= dst->count; ++i) { if (dst->lower[i] > src->lower[i]) dst->lower[i] = src->lower[i]; if (dst->upper[i] > src->upper[i]) dst->upper[i] = src->upper[i]; } }

void FogBuffer::Initialize(int mode,int initialize) {this->mode=mode;state=1;char fillLower=(this->mode==1||this->mode==3||this->mode==4);char fillUpper=(this->mode==4);if(this->mode==5){fillUpper=0;fillLower=0;}if(fillLower||initialize)memset(lower,fillLower,count+1);if(fillUpper||initialize)memset(upper,fillUpper,count+1);}

void FogResetMode(FogBuffer *p) { if (p->mode == 4 || p->mode == 3 || p->mode == 2 || p->mode == 5) return; p->state = 1; memset(p->upper, 0, p->count + 1); }

int FogSetValue(FogBuffer *p, long index, int value) { register long v; if (p->mode == 4) return 0; if (p->mode == 5) return 0; v = (value > 0); if (*(p->lower + index) != v || *(p->upper + index) != v) { p->state = 1; *(p->lower + index) = (signed char)v; *(p->upper + index) = (signed char)v; return 1; } return 0; }

struct FogCellRecord; struct TerrainGrid { long fields[17]; FogCellRecord *cells; long *neighbors; };
struct GridPoint { long x; long y; };
struct FogCellRecord { unsigned char type,height,unknown[10]; };
struct FogVisit { TerrainGrid *grid; GridPoint point; long index; };
#pragma pack(push, 1)
struct FogObject { char pad00[8]; long owner; char pad0c[0x13]; long range; long scale; char pad27[0x1b]; long index; char pad46[0x42]; signed char visible; char pad89[7]; };
#pragma pack(pop)
struct FogCastGlobals { FogObject *owner; FogBuffer *active; union { long flagsWord; unsigned char flags; }; long index; GridPoint source; GridPoint target; signed char mask[64]; long limit; };
extern FogCastGlobals gFogCast;
extern unsigned char *gTerrainCellData;
extern long gTerrainCellState; extern int heightStep,gLocalActorKind;
extern long gFogScale[16];
extern unsigned char gFogMaskWidths[6];
extern long gFogGlobalMode;
extern "C" void TargetObjectInit(const char *, void *, int);
extern "C" int LogicCopyPoint(int *, int *);
int PointAngle(GridPoint *,GridPoint *);
long GridIndexFromCoord(TerrainGrid *, GridPoint *);
int BestLocInitialize(void *, const char *, int);
int BestLocSetCandidate(void *, void *);
void BestLocVisit(void *, int (*)(void *));
void BestLocSetData(void *, void *, void *);
int StreamRead(void *, void *, int);
int StreamWrite(void *, const void *, unsigned long);
extern FogObject *CreateState(int);

int FogUpdateCell(void *argument) {
    long index = ((FogVisit *)argument)->index;
    if (index) {
        int visible = (gFogCast.limit <= 7);
        unsigned long band;
        int blocked = 0; int range;
        if (visible) {
            GridPoint coordinate;
            TargetObjectInit((const char *)((FogVisit *)argument)->grid, &coordinate, (int)index);
            long distance = PointAngle(&gFogCast.target, &coordinate);
            band = ((unsigned long)distance + 0x1fU) >> 0x1a & 63;
            range = LogicCopyPoint((int *)&gFogCast.source, (int *)&((FogVisit *)argument)->point);
            blocked = gFogCast.mask[band];

        }
        if (!blocked) {
            if (gFogGlobalMode == 0) {
                signed char *upper=gFogCast.active->upper; upper[index] = 1;
                gFogCast.active->lower[index] = upper[index];
            } else {
                long objectId = ((FogVisit *)argument)->grid->neighbors[index*2];
                if(objectId) { FogObject *object = CreateState((int)objectId);
                if (object && (int)object->visible && index == object->index && object->owner != gLocalActorKind) gFogCast.flags |= 4; }
            }
            if (visible && gFogCast.limit <= ((FogVisit *)argument)->grid->cells[index].height) {
                if (range>5 || range<1) gFogCast.mask[band]=1; else {
                    unsigned long count = gFogMaskWidths[range];
                    long start = ((long)band) - ((long)count >> 1);
                    if (start < 0) start += 64;
                    while (count--) {
                        gFogCast.mask[start++] = 1;


                        if (start >=64) start = 0;
                    }
                }
            }
        }
    }
    return 0;
}

long FogCast(FogBuffer *buffer, long index, FogObject *object) {
    if (buffer->mode == 4 || buffer->mode == 5) return 0;
    gFogCast.flagsWord = 0;
    buffer->state = 1;
    if (index && object->range > 0) {
        gFogCast.active = buffer;
        gFogCast.owner = object;
        gFogCast.index = index;
        BestLocSetData(buffer->source, &gFogCast.source, (void *)index);
        TargetObjectInit((const char *)buffer->source, &gFogCast.target, (int)gFogCast.index);
        long shape[10];
        BestLocInitialize(shape, (const char *)buffer->source, (int)object->range);
        BestLocSetCandidate(shape, (void *)gFogCast.index);
        long half = (heightStep + 500) / 2;
        long scale = gFogScale[object->scale] * 500;
        int threshold=(scale + half / 2) / half; memset(gFogCast.mask, 0, 0x40);
        gFogCast.limit = threshold + ((TerrainGrid *)buffer->source)->cells[gFogCast.index].height;
        BestLocVisit(shape, FogUpdateCell);
        gFogCast.flags |= 3;
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

