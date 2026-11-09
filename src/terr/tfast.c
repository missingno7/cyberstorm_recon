struct FastItem { unsigned char raw[8]; };
struct FastBuffer { long count; FastItem *items; long current; long first; void Update(long width, long offset); };
struct FastCoord { long x; long y; };
struct FastGrid { unsigned long placeholder; };
extern "C" void AssertFail(const char *, int, const char *); extern "C" void *memset(void *, int, unsigned long); extern "C" void MemoryRelease(void *);
extern FastGrid gFastGrid;
extern FastCoord gFastTransform;
extern void GridGet(FastGrid *, FastCoord *, long);
extern void GridConvert(FastCoord *, FastCoord *, FastCoord *);
extern void *MemoryAllocationRetry(unsigned long);

FastBuffer *FastBufferClear(FastBuffer *p) { p->items = 0; return p; }
FastBuffer *FastBufferFree(FastBuffer *p) { FastBuffer *owner = p; if (p->items) MemoryRelease(p->items); owner->items = 0; return p; }































int FastBufferCreate(FastBuffer *p, long count) { FastBuffer *owner = p; p->count = count + 1; if (p->items) MemoryRelease(p->items); owner->items = 0; p->items = (FastItem *)MemoryAllocationRetry(p->count * 8); if (!p->items || !count) AssertFail(__FILE__, __LINE__, 0); p->first = 0; p->current = p->first; memset(p->items, 0, p->count * 8); return 1; }

void FastBufferDestroy(FastBuffer *p) { if (p->items) MemoryRelease(p->items); p->items = 0; }

void FastBufferReset(FastBuffer *p) { p->first = -1; p->current = p->first; }


void FastBufferBuildFlags(FastBuffer *p);
void FastBufferBuildPixels(FastBuffer *p);

void FastBuffer::Update(long width, long offset) { int changed = (width != current); current = width; first = offset; if (changed) { FastBufferBuildFlags(this); FastBufferBuildPixels(this); FastItem *item = items + 1; for (long i = 1; i < count; ++i, ++item) { FastCoord source; FastCoord result; GridGet(&gFastGrid, &source, i); GridConvert(&gFastTransform, &result, &source); item->raw[4] = (unsigned char)result.x; item->raw[5] = (unsigned char)result.y; } } }


