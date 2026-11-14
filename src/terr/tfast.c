struct FastItem { unsigned char raw[8]; };
struct FastBuffer { long count; FastItem *items; long current; long first; void Update(long width, long offset); void BuildFlags(); void BuildPixels(); };
struct FastCoord { long x; long y; };
struct FastGrid { unsigned long placeholder; };
extern "C" void AssertFail(const char *, int, const char *); extern "C" void *memset(void *, int, unsigned long); 
extern FastGrid fallbackName;
extern FastCoord gFastTransform;
void BestLocSetData(void *, void *, void *);
int ResolveName(const char *, const char *);
extern void GridConvert(FastCoord *, FastCoord *, FastCoord *);

 static char tfastSourceFile[] = __FILE__;
FastBuffer *FastBufferClear(FastBuffer *p) { p->items = 0; return p; }
FastBuffer *FastBufferFree(FastBuffer *p) { FastBuffer *owner = p; if (p->items) delete[] p->items; owner->items = 0; return p; }






























int FastBufferCreate(FastBuffer *p, long count) { FastBuffer *owner = p; p->count = count + 1; if (p->items) delete[] p->items; owner->items = 0; p->items = (FastItem *)new char[p->count * 8]; if (!p->items || !count) AssertFail(tfastSourceFile, __LINE__, 0); p->first = 0; p->current = p->first; memset(p->items, 0, p->count * 8); return 1; }

struct TerrainGrid { long fields[17]; void *cells; void *neighbors; };
struct GridPoint { long x; long y; };
struct GridOffset { long x; long y; };
struct FastPalette { unsigned char raw[256]; };
extern GridOffset gGridOffsets[6];
extern FastPalette gFastPalette;

void FastBufferDestroy(FastBuffer *p) { if (p->items) delete[] p->items; p->items = 0; }

void FastBufferReset(FastBuffer *p) { p->first = -1; p->current = p->first; }


void FastBuffer::Update(long width, long offset) { int changed = (width != current); current = width; first = offset; if (changed) { BuildFlags(); FastBuffer *owner = this; register long i = 1; FastItem *item; BuildPixels(); item = owner->items + 1; for (; i < owner->count; ++i, ++item) { FastCoord source; FastCoord result; BestLocSetData(&fallbackName, &source, (void *)i); GridConvert(&gFastTransform, &result, &source); item->raw[4] = (unsigned char)result.x; item->raw[5] = (unsigned char)result.y; } } }

struct FastCell { unsigned char raw[12]; };
struct TargetTriple { int x; int y; int z; }; extern TargetTriple *targetDefinitionTable;
extern long GridColorLookup(FastPalette *, FastCoord *, unsigned long, int);

struct Point { int x, y; };
extern Point pathDirections[6];
inline long FastNeighborCell(FastCoord *base, long direction) {
    Point neighbor; Point *d = &pathDirections[direction];
    neighbor.x = d->x; neighbor.y = d->y;
    neighbor.x += base->x;
    neighbor.y += base->y;
    return ResolveName((const char *)&fallbackName, (const char *)&neighbor);
}
void FastBuffer::BuildFlags() {
    FastItem *item = items + 1;
    for (long index = 1; index < count; index++, item++) {
        FastCoord base;
        BestLocSetData(&fallbackName, &base, (void *)index);
        int currentColor = ((unsigned char *)&targetDefinitionTable[index])[1];
        unsigned char flags = 0;
        for (long side = 0; side < 6; ++side) {
            long cell = FastNeighborCell(&base, (this->current + side) % 6);
            if (cell && ((unsigned char *)targetDefinitionTable)[cell * 12 + 1] < currentColor) flags |= (unsigned char)(1 << side);
        }
        item->raw[6] = flags;
    }
}

void FastBuffer::BuildPixels() {
    register FastItem *item = items + 1;
    register long index = 1;
    if (index < count) {
        long sourceOffset = 12;
        do {
            FastCoord coordinate;
            BestLocSetData(&fallbackName, &coordinate, (void *)index);
            unsigned long currentColor = ((unsigned char *)targetDefinitionTable)[sourceOffset + 1];
            long a = GridColorLookup(&gFastPalette, &coordinate, currentColor, 4);
            long b = GridColorLookup(&gFastPalette, &coordinate, currentColor, 3);
            long c = GridColorLookup(&gFastPalette, &coordinate, currentColor, 2);
            item->raw[0] = (unsigned char)a;
            item->raw[1] = (unsigned char)b;
            item->raw[2] = (unsigned char)c;
            sourceOffset += 12;
            ++index;
            ++item;
        } while (index < count);
    }

}

void FastBufferEmpty(FastBuffer *) {
}

void FastBufferMakeCoords(FastBuffer *p) {
    register FastItem *item = p->items + 1;
    register long index = 1;
    if (index < p->count) do {
        FastCoord source;
        FastCoord result;
        BestLocSetData(&fallbackName, &source, (void *)index);
        GridConvert(&gFastTransform, &result, &source);
        item->raw[4] = (unsigned char)result.x;
        item->raw[5] = (unsigned char)result.y;
        ++index;
        ++item;
    } while (index < p->count);
}




