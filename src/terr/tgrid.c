struct TerrainGrid { long fields[17]; void *cells; void *neighbors; };
extern "C" void MemoryRelease(void *);
extern "C" void *memset(void *, int, unsigned long);

TerrainGrid *GridClear(TerrainGrid *p) { p->cells = 0; p->neighbors = 0; return p; }
TerrainGrid *GridRelease(TerrainGrid *p) { if (p->cells) { MemoryRelease(p->cells); register void *neighbors = p->neighbors; p->cells = 0; MemoryRelease(neighbors); p->neighbors = 0; } return p; }
void GridDestroy(TerrainGrid *p) { if (p->cells) { MemoryRelease(p->cells); register void *neighbors = p->neighbors; p->cells = 0; MemoryRelease(neighbors); p->neighbors = 0; } }
void GridClearNeighbors(TerrainGrid *p) { memset(p->neighbors, 0, (p->fields[10] + 1) * 8); }

long GridClamp(unsigned long value) { unsigned long divisor = 0x2aaaaabc; long result = (value + 0x1555555e) / divisor; if (result <= 0) return 0; if (result >= 5) return 5; return result; }

struct GridPoint { long x; long y; };
long GridMetric(GridPoint *a, GridPoint *b) { long dx = b->x - a->x; if (dx < 0) dx = -dx; long dy = b->y - a->y; if (dy < 0) dy = -dy; long result = (dx + dy) / 2; if (dx > result) result = dx; return result; }
void GridGetIndexCoord(TerrainGrid *grid, GridPoint *out, long index) { long mask = grid->fields[8] - 1; long value = index - 1; long x = value & mask; out->x = x; out->y = (value >> grid->fields[7]) * 2 + ((out->x & 1) != 0); }
