// Working labels for line traversal and terrain occlusion. Separate object at 42eca0.
typedef unsigned int uint;
struct Point { int x, y; Point() {} Point(int a, int b) { x=a; y=b; } int &X() { return x; } int &Y() { return y; } };
typedef Point RayPoint;
inline int RaySign(int x) { if (x < 0) return -1; return x > 0; }
struct ApplyPoint { int x, y; };
extern "C" int abs(int);
extern "C" void *memset(void *, int, unsigned int);
extern "C" int ResolveName(const char *, const char *);
extern "C" void TargetObjectInit(const char *, void *, int);
extern "C" void BestLocSetData(void *, void *, void *);
extern unsigned int ISqrt(unsigned int);
extern int PointAngle(ApplyPoint *, ApplyPoint *);
extern void f_0047efb0(void *, int);
extern void MenuDrawImage(void *, const struct MenuImage *, long, long);
extern char g513888[];
extern int heightStep;
extern RayPoint pathDirections[6];
struct MenuImages; extern MenuImages *drawCursorImages;
extern int RayCachedOrigin, RayCachedTarget, RayCachedResult, RayCachedSample;
RayPoint RayHit;
int RayLocations[100];
int RayCount;
unsigned char RaySamples[100];
int *RayGrid;

// RayHit is a global class object; its empty constructor produces W?.fn_init.

void RayWalk(RayPoint *from, RayPoint *to, void (*visit)(RayPoint *))
{
    RayPoint delta(to->x - from->x, to->y - from->y);
    RayPoint step(delta.x < 0 ? -1 : delta.x > 0, delta.y < 0 ? -1 : delta.y > 0);
    RayPoint point = *from;
    if (!delta.x) {
        for (;;) {
            visit(&point);
            if (point.y == to->y) break;
            point.y += step.y;
        }
    } else if (!delta.y) {
        for (;;) {
            visit(&point);
            if (point.x == to->x) break;
            point.x += step.x;
        }
    } else {
        RayPoint twice(abs(delta.x) * 2, abs(delta.y) * 2);
        if (twice.x > twice.y) {
            int error = twice.y - (twice.x >> 1);
            for (;;) {
                visit(&point);
                if (point.x == to->x) break;
                if (error >= 0) { point.Y() += step.y; error -= twice.x; }
                point.x += step.x; error += twice.y;
            }
        } else {
            int error = twice.x - (twice.y >> 1);
            for (;;) {
                visit(&point);
                if (to->y == point.y) break;
                if (error >= 0) { point.x += step.x; error -= twice.y; }
                point.Y() += step.y; error += twice.x;
            }
        }
    }
}

void RayCollect(RayPoint *point)
{
    int direction = 0;
    RayPoint *delta=pathDirections; while (RayCount<100) {
        if (direction > 6) return;
        RayPoint candidate; candidate.x = point->x; candidate.y = point->y;
        if (candidate.x & 1) candidate.y |= 1; else candidate.y &= ~1;
        if (direction < 6) { candidate.x += delta->x; candidate.y += delta->y; }
        int location = ResolveName((const char *)RayGrid, (const char *)&candidate);
        if (location) {
            int i;
            for (i = 0; i < RayCount; ++i) if (RayLocations[i] == location) break;
            if (i == RayCount) { RayLocations[RayCount] = location; ++RayCount; }
        }
        ++delta; ++direction;
    }
}

int RayOcclusion(int *grid, int origin, int target, int sample, RayPoint *hit)
{
    if (!origin || !target) return 0;
RayPoint impact, end, start, point, startCell, endCell;
    TargetObjectInit((const char *)grid, &start, origin);
    TargetObjectInit((const char *)grid, &end, target);
    int *occupants = (int *)grid[18]; int startOccupant = occupants[origin * 2]; int endOccupant = occupants[target * 2];
    if (RayCachedOrigin == origin && RayCachedTarget == target && RayCachedSample == sample) {
        if (hit) *hit = RayHit;
        return RayCachedResult;
    }
    RayCachedTarget = target; RayCachedSample = sample; RayCachedResult = 100; RayCachedOrigin = origin;
    RayHit = end; RayCount = 0; RayGrid = grid;
    memset(RaySamples, -1, 100);
    unsigned char *startTile = (unsigned char *)grid[17] + origin * 12;
    unsigned char *endTile = (unsigned char *)grid[17] + target * 12;
    impact = end;
    int sampleRow = sample / 10, sampleColumn = sample % 10;

    BestLocSetData(grid, &startCell, (void *)origin);
    BestLocSetData(grid, &endCell, (void *)target);
    unsigned char found = 0;
    RayWalk(&startCell, &endCell, RayCollect);
    if (!RayCount) return 100;
    uint distance = ISqrt((end.y - start.y) * (end.y - start.y) + (end.x - start.x) * (end.x - start.x));
    int angle = PointAngle((ApplyPoint *)&start, (ApplyPoint *)&end);
    int tenth = grid[1] / 10;
    if (!tenth || !distance) return 100;
    int result = 100;
    for (int i = 0; i < RayCount; ++i) {

        TargetObjectInit((const char *)grid, &point, RayLocations[i]);
        point.x -= start.x; point.y -= start.y;
        f_0047efb0(&point, -angle);
        if (point.y > 0 && point.y < (int)distance && abs(point.x) < grid[1]) {
            uint along;
            if (point.y <= 0) along = 0;
            else if (point.y >= (int)distance) along = distance;
            else along = point.y;
            int rise = ((unsigned char *)grid[17])[RayLocations[i] * 12 + 1] * heightStep -
                ((int)((uint)(endTile[1] - startTile[1]) * along * heightStep) / (int)distance + startTile[1] * heightStep);
            int occupant = ((int *)grid[18])[RayLocations[i] * 2];
            if (occupant && occupant != startOccupant && occupant != endOccupant) rise += 800;
            if (rise > 0) {
                int left, right;
                if (!occupant) {
                    left = (point.x - (grid[1] * 8 >> 4)) / tenth + 5;
                    right = (point.x + (grid[1] * 7 >> 4)) / tenth + 5;
                } else {
                    left = (point.x - (grid[1] * 4 >> 4)) / tenth + 5;
                    right = (point.x + (grid[1] * 3 >> 4)) / tenth + 5;
                }
                if (left < 1) left = 0; else if (left > 8) left = 9;
                if (right < 1) right = 0; else if (right > 8) right = 9;
                rise /= tenth;
                if (rise < 2) rise = 1; else if (rise > 9) rise = 10;
                for (int row = 10 - rise; row < 10; ++row) {
                    for (int column = left; column <= right; ++column) {
                        int offset = row * 10 + column;
                        if (RaySamples[offset]) {
                            if (!found && row == sampleRow && column == sampleColumn) {
                                impact.x = tenth * (sampleColumn - 5); found = 1;
                                int half=grid[1]>>1;
                                if(startTile[1]<endTile[1] && (impact.y=half*half-(impact.x-point.x)*(impact.x-point.x))>0) { impact.y=ISqrt(impact.y);impact.y=point.y-impact.y; }else impact.y=point.y;
                                f_0047efb0(&impact, angle);
                                impact.x += start.x; impact.y += start.y;
                            }
                            --result; RaySamples[offset] = 0;
                        }
                    }
                }
            }
        }
    }
    RayHit = impact; RayCachedResult = result;
    if (hit) *hit = impact;
    return result;
}

void RayDrawSamples(void *, int x, int y, int step)
{
    const struct MenuImage *image = (const struct MenuImage *)((char *)drawCursorImages + 0x544);
    for (int row = 0; row < 10; ++row) {
        int screenX = x;
        for (int column = 0; column < 10; ++column) {
            if (!RaySamples[row * 10 + column]) MenuDrawImage(g513888, image, screenX, y);
            screenX += step;
        }
        y += step;
    }
}







