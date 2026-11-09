struct FogSource { long unused[10]; long count; };
struct FogBuffer { long mode; FogSource *source; long count; signed char *lower; signed char *upper; long state; int Resize(); };
extern "C" void AssertFail(const char *, int, const char *); extern "C" void MemoryRelease(void *); extern "C" void *MemoryAllocationRetry(unsigned long); extern "C" void *memcpy(void *, const void *, unsigned long); extern "C" void *memset(void *, int, unsigned long); void FogInitialize(FogBuffer *, int mode = 0, int initialize = 1);


void FogNoOp() {}
FogBuffer *FogSetSource(FogBuffer *p, FogSource *source) { p->lower = 0; p->upper = 0; p->source = source; return p; }
void FogRelease(FogBuffer *p) { if (p->lower) MemoryRelease(p->lower); if (p->upper) MemoryRelease(p->upper); p->lower = 0; p->upper = 0; }






















































int FogBuffer::Resize() { FogBuffer *owner = this; if (lower) { MemoryRelease(lower); signed char *upper = owner->upper; owner->lower = 0; MemoryRelease(upper); owner->upper = 0; } count = source->count; lower = (signed char *)MemoryAllocationRetry(count + 1); upper = (signed char *)MemoryAllocationRetry(count + 1); if (!lower || !upper) AssertFail(__FILE__, __LINE__, 0); FogInitialize(this); return 1; }









void FogCopy(FogBuffer *dst, FogBuffer *src) { if (!dst->lower || !dst->upper) AssertFail(__FILE__, __LINE__, 0); memcpy(dst->lower, src->lower, dst->count + 1); memcpy(dst->upper, src->upper, dst->count + 1); dst->mode = src->mode; }

void FogMerge(FogBuffer *dst, FogBuffer *src) { dst->mode = 0; for (long i = 0; i <= dst->count; ++i) { if (dst->lower[i] > src->lower[i]) dst->lower[i] = src->lower[i]; if (dst->upper[i] > src->upper[i]) dst->upper[i] = src->upper[i]; } }

void FogInitialize(FogBuffer *p, int mode, int initialize) { p->mode = mode; p->state = 1; int fillLower = mode == 1 || mode == 3 || mode == 4; char fillUpper = (char)(mode == 4); if (mode == 5) fillLower = 0; if (fillLower || initialize) memset(p->lower, fillLower, p->count + 1); if (fillUpper || initialize) memset(p->upper, fillUpper, p->count + 1); }

void FogResetMode(FogBuffer *p) { if (p->mode == 4 || p->mode == 3 || p->mode == 2 || p->mode == 5) return; p->state = 1; memset(p->upper, 0, p->count + 1); }

int FogSetValue(FogBuffer *p, long index, int value) { long v; signed char *lower; if (p->mode == 4) goto unchanged; if (p->mode == 5) goto unchanged; v = (value > 0); lower = p->lower; if (lower[index] != v) goto changed; if (p->upper[index] == v) goto unchanged; changed: p->state = 1; p->lower[index] = (signed char)v; p->upper[index] = (signed char)v; return 1; unchanged: return 0; }
