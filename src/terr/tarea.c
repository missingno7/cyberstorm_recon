struct AreaWalk {
    int x;
    int y;
    unsigned int step;
    unsigned int phase;
    int edge;
};
struct AreaPoint { int x; int y; AreaPoint &operator+=(const AreaPoint &p) { x+=p.x; y+=p.y; return *this; } AreaPoint &operator-=(const AreaPoint &p) { x-=p.x; y-=p.y; return *this; } };

static const char AreaSourcePath[] = "terr\\tarea.c";
extern "C" void AssertFail(const char *, int, const char *);
extern "C" void BestLocSetData(void *, void *, void *);
extern "C" int LogicCopyPoint(int *, int *);
extern "C" void DisplayStep(AreaWalk *);
extern int ResolveName(const char *, const char *);
extern void AreaVisit(void *, void *);

inline int AreaRingCount(int radius) { if (radius < 0) return 0; return (((radius + 1) * radius) >> 1) * 6 + 1; }
int AreaFind(int *grid, int position)
{
    int point[2];
    if (position != 0) {
        BestLocSetData((void *)grid[0], point, (void *)position);
        int *center = grid + 8;
        int ring = LogicCopyPoint(point, center);
        if (ring <= grid[5]) {
            point[0] -= center[0];
            point[1] -= center[1];
            AreaWalk walk;
            walk.step = (unsigned int)ring;
            walk.x = 0;
            walk.y = ring * 2;
            walk.edge = walk.y;
            walk.phase = ((unsigned int)ring > 0);
            int index = AreaRingCount(ring - 1);
            ++index;
            while (walk.step <= (unsigned int)grid[5]) {
                if (walk.x == point[0] && walk.y == point[1]) return index;
                DisplayStep(&walk);
                ++index;
            }
            // The assert's line number is an immediate in the original call.
            // Keep this call at source line 143.
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            // line-number padding
            AssertFail(AreaSourcePath, __LINE__, 0);
        }
    }
    return 0;
}


int AreaIndexToPoint(int *grid, int index)
{
    int savedIndex = index;
    AreaPoint point = *(AreaPoint *)(grid + 8);
    if (savedIndex != 0) {
        int ring = index + 4;
        ring /= 6;
        int sum = 0;
        int r = 0;
        if (ring > 0) { do { ++r; sum += r; } while (sum < ring); }
        AreaWalk walk;
        walk.step = r;
        walk.x = 0;
        walk.y = r * 2;
        walk.edge = walk.y;
        walk.phase = ((unsigned int)r > 0);
        int current = AreaRingCount(r - 1);
        ++current;
        while (walk.step <= (unsigned int)grid[5]) {
            if (current == savedIndex) {
                point += *(AreaPoint *)&walk;
                break;
            }
            DisplayStep(&walk);
            ++current;
        }
    }
    return ResolveName((const char *)grid[0], (const char *)&point);
}

int *AreaGetPoint(int *grid, int *point, int index)
{
    *(AreaPoint *)point = *(AreaPoint *)(grid + 8);
    if (index != 0) {
        int ring = index + 4;
        ring /= 6;
        int sum = 0;
        int r = 0;
        if (ring > 0) { do { ++r; sum += r; } while (sum < ring); }
        AreaWalk walk;
        walk.step = r;
        walk.x = 0;
        walk.y = r * 2;
        walk.edge = walk.y;
        walk.phase = ((unsigned int)r > 0);
        int current = AreaRingCount(r - 1);
        ++current;
        while (walk.step <= (unsigned int)grid[5]) {
            if (current == index) {
                *(AreaPoint *)point += *(AreaPoint *)&walk;
                break;
            }
            DisplayStep(&walk);
            ++current;
        }
    }
    return point;
}

void AreaEnumerate(int *record, void *callback, unsigned int ring, unsigned int maxRing)
{
    AreaWalk walk;
    walk.step = ring;
    walk.x = 0;
    walk.y = ring * 2;
    walk.edge = walk.y;
    walk.phase = ((unsigned int)ring > 0);
    int index = AreaRingCount((int)ring - 1);
    ++index;
    while (walk.step <= maxRing) {
        record[1] = record[8] + walk.x;
        record[2] = record[9] + walk.y;
        record[3] = ResolveName((const char *)record[0], (const char *)(record + 1));
        record[4] = index;
        if (((int (*)(void *))callback)(record) != 0) return;
        DisplayStep(&walk);
        ++index;
    }
}

void AreaEnumerateAll(int *record, void *callback)
{
    AreaEnumerate(record, callback, 0, (unsigned int)record[5]);
}


