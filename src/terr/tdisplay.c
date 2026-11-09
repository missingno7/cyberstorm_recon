struct DisplayRect { int x, y, width, height; };
struct AreaWalk { int x, y; unsigned int step; int phase, edge; };

extern "C" void __wcpp_2_ctor_array_(void *objects, int count, void *constructor);
extern int BestLocInitialize(void *, const char *, int);
extern void BestLocSetData(void *, void *, void *);

void DisplayConstructArray()
{
    __wcpp_2_ctor_array_((void *)0x4bc62c, 2000, (void *)0x48daf4);
}

void DisplayInitialize(void *object, const char *name, int index)
{
    BestLocInitialize(object, name, index);
}

void *DisplayIdentity(void *value)
{
    return value;
}

void DisplaySetRect(void *object, const DisplayRect *source)
{
    *(DisplayRect *)((char *)object + 0x28) = *source;
    int *fields = (int *)object;
    fields[0x0e] = fields[0x0a] + (fields[0x0c] >> 1);
    fields[0x0f] = fields[0x0b] + (fields[0x0d] >> 1);
    fields[0x10] = fields[0x0a] + fields[0x0c] - 1;
    fields[0x11] = fields[0x0b] + fields[0x0d] - 1;
}

static const char DisplaySourcePath[] = "terr\\tdisplay.c";
extern "C" void AssertFail(const char *, int, const char *);
struct TerrainGrid; struct GridPoint { int x, y; };
extern long GridPointToIndex(TerrainGrid *, GridPoint *);
extern void GridRotatePoint(GridPoint *, long);
struct FastBuffer { void Update(long, long); };
extern void f_00435140(int); extern int f_0047d474(void *, int); extern void f_0047efb0(void *, int);
extern "C" void TargetObjectInit(const char *, void *, int);
extern "C" int abs(int);
void DisplayGetBounds(int *, unsigned int *, unsigned int *);

void DisplayConfigure(int *view, int *settings, int reset)
{
    view[0x12] = settings[0]; view[0x13] = settings[1];
    int ring = settings[4] % 6; view[0x14] = ring; if (ring < 0) view[0x14] = ring += 6;
    view[0x20] = view[0x14] * 0x2aaaaabc; view[0x21] = settings[5];
    int scale = settings[3]; view[0x1a] = scale; if (scale < 2) scale = 1; view[0x1a] = scale;
    view[0x1b] = scale * 2; view[0x1c] = scale + view[0x1b]; view[0x1d] = view[0x1b] * 2;
    f_00435140(scale);
    int resolution = (*(int *)0x4c7e50 * scale * settings[7]) / 50000; view[0x1e] = resolution;
    if (resolution == 0) ((unsigned char *)(view + 0x21))[0] &= 0xfe;
    int estimate = (view[0x0c] + view[0x1c] - 1) / view[0x1c] + 1;
    int radius = (estimate >> 1) + (estimate >> 3) + 2; view[0x1f] = radius;
    if (radius < 6) radius = 5; else if (view[5] * 2 <= radius) radius = view[5] * 2;
    view[0x1f] = radius;
    if (radius > 0x28) ((unsigned char *)(view + 0x21))[0] &= 0xfe;
    if ((((unsigned char *)(view + 0x21))[0] & 1) == 0) view[0x1e] = 0;
    view[0x19] = GridPointToIndex((TerrainGrid *)view[0], (GridPoint *)(view + 0x12));
    // Original source line padding keeps the assert's immediate stable.
    // Padding line 2.
    // Padding line 3.
    // Padding line 4.
    // Padding line 5.
    // Padding line 6.
    // Source-line padding 49.
    // Source-line padding 48.
    // Source-line padding 47.
    // Source-line padding 46.
    // Source-line padding 45.
    // Source-line padding 44.
    // Source-line padding 43.
    // Source-line padding 42.
    // Source-line padding 41.
    // Source-line padding 40.
    // Source-line padding 39.
    // Source-line padding 38.
    // Source-line padding 37.
    // Source-line padding 36.
    // Source-line padding 35.
    // Source-line padding 34.
    // Source-line padding 33.
    // Source-line padding 32.
    // Source-line padding 31.
    // Source-line padding 30.
    // Source-line padding 29.
    // Source-line padding 28.
    // Source-line padding 27.
    // Source-line padding 26.
    // Source-line padding 25.
    // Source-line padding 24.
    // Source-line padding 23.
    // Source-line padding 22.
    // Source-line padding 21.
    // Source-line padding 20.
    // Source-line padding 19.
    // Source-line padding 18.
    // Source-line padding 17.
    // Source-line padding 16.
    // Source-line padding 15.
    // Source-line padding 14.
    // Source-line padding 13.
    // Source-line padding 12.
    // Source-line padding 11.
    // Source-line padding 10.
    // Source-line padding 9.
    // Source-line padding 8.
    // Source-line padding 7.
    // Source-line padding 6.
    // Source-line padding 5.
    // Source-line padding 4.
    unsigned int assertedIndex = view[0x19]; if (assertedIndex == 0) AssertFail(DisplaySourcePath, __LINE__, (const char *)assertedIndex);
    unsigned int bounds[2]; unsigned int *point = (unsigned int *)(view + 0x12);
    unsigned int savedX = 0; unsigned int savedY = 0;
    view[0x15] = 0; view[0x16] = 0; DisplayGetBounds(view, bounds, point);
    view[0x15] = bounds[0]; view[0x16] = bounds[1];
    view[0x15] -= view[0x0e]; view[0x16] -= view[0x0f];
    if ((((unsigned char *)(view + 0x21))[0] & 1) != 0) {
        savedX = view[0x15]; savedY = view[0x16];
        bounds[0] = 0; bounds[1] = 0; view[0x15] = 0; view[0x16] = 0;
        unsigned char flags = ((unsigned char *)(view + 0x21))[0]; ((unsigned char *)(view + 0x21))[0] = flags & 0xfe;
        DisplayGetBounds(view, bounds, point); view[0x21] = flags;
        view[0x15] = bounds[0] - view[0x0e]; view[0x16] = bounds[1] - view[0x0f];
        if (!f_0047d474((void *)0x4bc618, 1)) { *(int *)0x4bc610 = 0; *(int *)0x4bc614 = 0; }
        if (reset == 0) {
            *(int *)0x4bc610 = savedX - view[0x15]; *(int *)0x4bc614 = savedY - view[0x16];
            view[0x15] = savedX; view[0x16] = savedY;
        } else { view[0x15] += *(int *)0x4bc610; view[0x16] += *(int *)0x4bc614; }
    }
    if ((((unsigned char *)(view + 0x1a))[0] & 3) == 0) view[0x15] += (view[0x0e] - view[0x15]) & 3U;
    view[0x17] = view[0x0e] - view[0x15]; view[0x18] = view[0x0f] - view[0x16];
    ((FastBuffer *)0x4d35e4)->Update(settings[4], settings[2]);
}

int DisplayZoomArea(int *view, unsigned int *result, int position, int candidate, int adjust)
{
    if (position == 0) return 0;
    unsigned int origin[4];
    int originPoint[2];
    BestLocSetData((void *)*view, origin, (void *)position);
    TargetObjectInit((const char *)*view, originPoint, position);
    int scaleCount = view[0x0d] / view[0x1d] + 1;
    int halfScale = scaleCount >> 1;
    unsigned int target[4];
    int targetPoint[2];
    int targetIndex = candidate == 0 ? view[0x19] : candidate;
    BestLocSetData((void *)*view, target, (void *)targetIndex);
    TargetObjectInit((const char *)*view, targetPoint, targetIndex);
    unsigned int dx = (unsigned int)targetPoint[0] - (unsigned int)originPoint[0];
    int dy = targetPoint[1] - originPoint[1];
    f_0047efb0(&dx, view[0x20]);
    dx = (int)(dx * view[0x0d]) / view[0x0c];
    int vertical = abs(dy);
    int horizontal = abs((int)dx);
    if (horizontal <= vertical) horizontal = vertical;
    int rings = horizontal / *(int *)(*view + 4) + 1;
    if (candidate == 0) {
        if (((adjust == 0 || view[0x1a] == 0xe) && rings < halfScale)) return 0;
        result[0] = originPoint[0]; result[1] = originPoint[1];
        if (adjust != 0) result[3] = 0xe;
    } else {
        if (rings < halfScale) {
            if (rings < (scaleCount >> 2) && adjust != 0) {
                unsigned int level = result[3];
                if ((int)level < 2) level = 1;
                result[3] = level;
                while (rings < (halfScale >> 1) && (int)result[3] < 0xe) {
                    ++result[3];
                    halfScale = view[0x0d] / (int)(result[3] * 4) + 1 >> 1;
                }
            }
        } else {
            unsigned int oldLevel = result[3];
            if (adjust != 0) {
                while (halfScale <= rings && (int)result[3] > 4) {
                    --result[3];
                    halfScale = view[0x0d] / (int)(result[3] * 4) + 1 >> 1;
                }
            }
            if (halfScale <= rings) {
                result[3] = oldLevel;
                return 0;
            }
        }
        result[0] = (originPoint[0] + targetPoint[0]) >> 1;
        result[1] = (originPoint[1] + targetPoint[1]) >> 1;
    }
    return 1;
}

int DisplayScale(int *object)
{
    int numerator = object[0x1c] + object[0x0c] - 1;
    int value = (numerator / object[0x1c] + 1) >> 1;
    if (value < 2) return 1;
    if (object[5] <= value) value = object[5];
    return value;
}

int DisplayPointScale(int *object, int value)
{
    int numerator = value * object[0x1d];
    int *base = (int *)*object;
    int divisor = base[1];
    return numerator / divisor;
}

void DisplayGetBounds(int *view, unsigned int *output, unsigned int *point)
{
    output[0] = point[0];
    output[1] = point[1];
    f_0047efb0(output, view[0x20]);
    output[0] = (int)(output[0] * 0x128) >> 8;
    output[1] *= -2;
    output[0] *= view[0x1c];
    output[1] *= view[0x1b];
    int tileSize = *(int *)(*view + 4);
    output[0] = (int)output[0] / tileSize;
    output[1] = (int)output[1] / tileSize;
    if ((((unsigned char *)(view + 0x21))[0] & 1) != 0) {
        int index = GridPointToIndex((TerrainGrid *)*view, (GridPoint *)point);
        unsigned int list = *(unsigned int *)0x4c6274;
        if (index != 0 && (list == 0 || *(char *)(*(int *)(list + 0xc) + index) != 0)) {
            output[1] -= *(unsigned char *)(*(int *)(*view + 0x44) + 1 + index * 0xc) * view[0x1e];
        }
    }
    output[0] += view[0x0e];
    output[1] += view[0x0f];
    output[0] -= view[0x15];
    output[1] -= view[0x16];
}

void DisplayPointToScreen(int *view, int *output, int *point, int index)
{
    output[0] = point[0]; output[1] = point[1];
    output[0] -= *(int *)(*view + 0x10); output[1] -= *(int *)(*view + 0x14);
    GridRotatePoint((GridPoint *)output, view[0x14]);
    output[0] *= view[0x1c];
    int height = output[1] * view[0x1b]; output[1] = height;
    if ((((unsigned char *)(view + 0x21))[0] & 1) != 0) {
        *(int *)0x4c6284 = height;
        unsigned int list = *(unsigned int *)0x4c6274;
        if (index != 0 && (list == 0 || *(char *)(*(int *)(list + 0xc) + index) != 0)) {
            output[1] -= *(unsigned char *)(index * 0xc + 1 + *(int *)(*view + 0x44)) * view[0x1e];
        }
    }
    output[0] += view[0x0e]; output[1] += view[0x0f];
    output[0] -= view[0x15]; output[1] -= view[0x16];
}

int DisplayPointToScreenScaled(int *view, unsigned int *output, unsigned int *point)
{
    output[0] = point[0]; output[1] = point[1];
    f_0047efb0(output, view[0x20]);
    output[0] = (int)(output[0] * 0x128) >> 8;
    output[1] *= -2;
    output[0] *= view[0x1c]; output[1] *= view[0x1b];
    int tileSize = *(int *)(*view + 4);
    output[0] = (int)output[0] / tileSize; output[1] = (int)output[1] / tileSize;
    output[0] += view[0x0e]; output[1] += view[0x0f];
    output[0] -= view[0x15]; output[1] -= view[0x16];
    return view[0x1e];
}

int DisplayRectOutside(int viewAddress, int *rect)
{
    if (rect[0] - *(int *)(viewAddress + 0x74) < *(int *)(viewAddress + 0x28) + *(int *)(viewAddress + 0x30) &&
        *(int *)(viewAddress + 0x2c) + *(int *)(viewAddress + 0x34) >
            rect[1] - *(int *)(viewAddress + 0x6c) &&
        *(int *)(viewAddress + 0x28) <= rect[0] + *(int *)(viewAddress + 0x74) &&
        *(int *)(viewAddress + 0x2c) <= rect[1] + *(int *)(viewAddress + 0x6c) + *(int *)(viewAddress + 0x74)) return 0;
    return 1;
}

extern void f_0042d640(int *, unsigned int, int *, unsigned int);
extern int f_0042b3d0(int *, int);

int DisplaySubmitElement(int *element)
{
    unsigned int objectId = *(unsigned int *)(element + 3);
    unsigned int height = 0;
    if (objectId != 0) height = *(unsigned char *)(*(int *)0x4d35d8 + 1 + objectId * 0xc);
    int point[2];
    point[0] = element[1] - *(int *)0x4d35a4;
    point[1] = element[2] - *(int *)0x4d35a8;
    GridRotatePoint((GridPoint *)point, *(int *)0x4d365c);
    unsigned int lower = 0;
    if ((*(unsigned int *)0x4d3690 & 1) != 0 && objectId != 0 && *(char *)(objectId + *(int *)0x4d3600) != 0) lower = height;
    point[0] = point[0] * *(int *)0x4d367c + *(int *)0x4d3668;
    *(int *)0x4c6284 = point[1] * *(int *)0x4d3678 + *(int *)0x4d366c;
    point[1] = *(int *)0x4c6284;
    if (lower != 0) point[1] = *(int *)0x4c6284 - *(int *)0x4d3684 * lower;
    if (point[0] - *(int *)0x4d3678 <= *(int *)0x4d364c &&
        point[1] - *(int *)0x4d3678 <= *(int *)0x4d3650 &&
        *(int *)0x4d3634 <= point[0] + *(int *)0x4d3678 &&
        *(int *)0x4d3638 <= *(int *)0x4c6284 + *(int *)0x4d3678) {
        if (lower == 0) f_0042d640((int *)0x4d360c, objectId, point, height);
        else if (*(int *)0x4c626c < 2000) {
            int offset = *(int *)0x4c626c * 0x10;
            ++*(int *)0x4c626c;
            *(int *)(0x4bc62c + offset) = point[0];
            *(int *)(0x4bc630 + offset) = point[1];
            int encodedHeight = *(int *)0x4c6284 * 0x10 + height;
            *(unsigned int *)(0x4bc638 + offset) = objectId;
            *(int *)(0x4bc634 + offset) = encodedHeight;
            *(int *)(0x4bc634 + offset) = -encodedHeight;
        }
    }
    return 0;
}

void DisplaySort(void **items, int count)
{
    int gaps[5];
    int *source = (int *)0x490470;
    for (int i = 0; i < 5; ++i) gaps[i] = source[i];
    for (int gapIndex = 0; gapIndex < 5; ++gapIndex) {
        int gap = gaps[gapIndex];
        if (gap < count) {
            int i = gap;
            do {
                int index = i - gap;
                void *item = items[i];
                while (index >= 0 && *(int *)((char *)items[index] + 8) <= *(int *)((char *)item + 8)) {
                    int destination = index + gap;
                    void *prior = items[index];
                    index -= gap;
                    items[destination] = prior;
                }
                items[index + gap] = item;
                ++i;
            } while (i < count);
        }
    }
}

extern void AreaEnumerate(int *, void *, unsigned int, unsigned int);
extern void f_0047b930(int, int, int);
extern void f_0047bad0(int, int, int, int, int);
extern void f_0042de30(int *, int, int *, unsigned int);
extern "C" void __wcpp_2_ctor_array_(void *, int, void *);
extern int f_0047efd0(int *, void *, int);
extern int ResolveName(const char *, const char *);
void DisplaySetCandidate(int *, int);

void DisplayProcess(int *view, void *argument1, void *argument2, void *argument3)
{
    DisplaySetCandidate(view, view[0x19]);
    *(int *)0x4c6270 = (int)view;
    *(int *)0x4c6278 = (int)argument1;
    *(int *)0x4c627c = (int)argument2;
    *(int *)0x4c6280 = (int)argument3;
    *(int *)0x4c626c = 0;
    if (view[0x1a] < 9) {
        f_0047b930(0x513888, 0, 0);
        f_0047bad0(0x513888, view[10], view[11], view[12], view[13]);
    }
    AreaEnumerate(view, (void *)DisplaySubmitElement, 0, (unsigned int)view[0x1f]);
    void **sorted = (void **)0x4c432c;
    char *items = (char *)0x4bc62c;
    int count = 0;
    if (*(int *)0x4c626c > 0) {
        do { sorted[count++] = items; items += 0x10; } while (count < *(int *)0x4c626c);
    }
    int itemCount = *(int *)0x4c626c;
    int gaps[5];
    int *gapSource = (int *)0x490470;
    for (int i = 0; i < 5; ++i) gaps[i] = gapSource[i];
    int gapIndex = 0;
    do {
        int gap = *(int *)((char *)gaps + gapIndex);
        if (gap < itemCount) {
            int index = gap;
            void **current = sorted + gap;
            do {
                int prior = index - gap;
                void *item = *current;
                while (prior >= 0 && *(int *)((char *)sorted[prior] + 8) <= *(int *)((char *)item + 8)) {
                    int destination = prior + gap;
                    void *previous = sorted[prior];
                    prior -= gap;
                    sorted[destination] = previous;
                }
                sorted[prior + gap] = item;
                ++index;
                ++current;
            } while (index < itemCount);
        }
        gapIndex += 4;
    } while (gapIndex != 0x14);
    void **current = sorted;
    int index = 0;
    if (itemCount > 0) {
        do {
            int *item = (int *)*current;
            unsigned int height = *(unsigned char *)(item[3] * 0xc + 1 + *(int *)(*view + 0x44));
            f_0042de30(view, item[3], item, height);
            ++index;
            ++current;
            f_0042d640(view, item[3], item, height);
        } while (index < itemCount);
    }
}

int DisplayResolveCandidate(int *view, int *point, int index)
{
    if (index != 0) {
        int rect[2];
        DisplayPointToScreen(view, rect, point, index);
        int left = rect[0] - view[0x1b];
        int top = rect[1] - view[0x1b];
        int size = view[0x1d];
        if (*(int *)0x4bc624 >= left && *(int *)0x4bc624 < left + size &&
            *(int *)0x4bc628 >= top && *(int *)0x4bc628 < top + size) {
            int corners[30];
            __wcpp_2_ctor_array_(corners, 6, (void *)0x48daa4);
            corners[0] = left + view[0x1a]; corners[2] = left + view[0x1c];
            corners[4] = left + view[0x1d]; corners[5] = top + view[0x1b];
            corners[6] = left + view[0x1c]; corners[7] = top + view[0x1d];
            corners[8] = left + view[0x1a]; corners[9] = top + view[0x1d];
            corners[20] = left; corners[11] = top + view[0x1b];
            corners[10] = left; corners[3] = top;
            corners[16] = corners[2]; corners[17] = top;
            corners[21] = corners[11]; corners[22] = corners[4];
            corners[23] = corners[5]; corners[24] = corners[6];
            corners[25] = corners[7]; corners[26] = corners[0];
            corners[27] = top; corners[28] = corners[8]; corners[29] = corners[9];
            return f_0047efd0((int *)0x4bc624, corners, 6);
        }
    }
    return 0;
}

int DisplayConsiderElement(int *item)
{
    int index = item[3];
    if (index != 0 && DisplayResolveCandidate((int *)*(int *)0x4c6270, item + 1, index) != 0) {
        unsigned int height = 0;
        unsigned int list = *(unsigned int *)0x4c6274;
        if (list == 0 || *(char *)(*(int *)(list + 0xc) + index) != 0) {
            height = *(unsigned char *)(*(int *)(*item + 0x44) + 1 + index * 0xc);
        }
        if (*(int *)0x4bc61c == 0 || (int)*(unsigned int *)0x4bc620 < (int)height) {
            *(int *)0x4bc61c = index;
            *(unsigned int *)0x4bc620 = height;
        }
    }
    return 0;
}

int DisplayPickAt(int *view, int *point)
{
    *(int *)0x4bc61c = 0;
    if (point[0] >= view[10] && point[0] < view[10] + view[12] &&
        point[1] >= view[11] && point[1] < view[11] + view[13]) {
        *(int *)0x4c6270 = (int)view;
        *(int *)0x4bc624 = point[0]; *(int *)0x4bc628 = point[1];
        int y = point[1] + view[0x16] - view[0x0f];
        if ((((unsigned char *)(view + 0x21))[0] & 1) != 0) y = view[0x1e] * 3 + y;
        y /= view[0x1b];
        int x = (point[0] + view[0x15] - view[0x0e]) / view[0x1c];
        GridRotatePoint((GridPoint *)&x, -view[0x14]);
        x += *(int *)(*view + 0x10); y += *(int *)(*view + 0x14);
        int index = ResolveName((const char *)*view, (const char *)&x);
        if (index != 0) {
            DisplaySetCandidate(view, index);
            AreaEnumerate(view, (void *)DisplayConsiderElement, 0, 4);
        }
    }
    return *(int *)0x4bc61c;
}

void *DisplayIdentityA(void *value) { return value; }
void *DisplayIdentityB(void *value) { return value; }
void *DisplayIdentityC(void *value) { return value; }

void DisplayWalkInit(AreaWalk *walk, unsigned int step)
{
    walk->step = step;
    int twice = ((volatile AreaWalk *)walk)->step * 2;
    walk->x = 0;
    walk->y = twice;
    walk->edge = twice;
    walk->phase = walk->step > 0;
}

extern "C" void DisplayStep(AreaWalk *walk)
{
    unsigned int phase = walk->phase;
    if (phase < 2) {
        if (phase == 1) {
            walk->phase = 2;
            if (walk->x) { walk->x = -walk->x; return; }
        } else goto transition;
    } else if (phase < 3) goto phase_two;
    else if (phase != 3) goto transition;
    walk->phase = 0;
    if (walk->x && walk->y) { walk->x = -walk->x; return; }
    goto transition;
phase_two:
    ++walk->phase;
    if (walk->y) { walk->y = -walk->y; return; }
transition:
    {
        int y = walk->y;
        int opposite = -y;
        walk->y = opposite;
        if (y == -1 || opposite <= 0) {
            int edge = walk->edge;
            walk->x = 0;
            walk->edge = edge + 2;
            walk->y = edge + 2;
            ++walk->step;
        } else {
            int x = walk->x;
            if (x + y != 0 && x <= opposite) {
                ++walk->x;
                --walk->y;
                ++walk->phase;
                return;
            }
            walk->y = opposite - 2;
        }
        ++walk->phase;
    }
}

int BestLocInitialize(void *object, const char *name, int radius)
{
    int count = 0;
    if (radius >= 0) count = ((radius + 1) * radius >> 1) * 6 + 1;
    *(const char **)object = name;
    ((int *)object)[5] = radius;
    ((int *)object)[6] = count;
    return (int)object;
}

int DisplayCount(int value)
{
    if (value < 0) return 0;
    return (value * (value + 1) >> 1) * 6 + 1;
}

void DisplaySetCandidate(int *object, int value)
{
    object[7] = value;
    BestLocSetData((void *)*object, object + 8, (void *)value);
}

void DisplaySetRadius(int *object, int value) { object[5] = value; if (value < 0) { object[6] = 0; return; } object[6] = ((value + 1) * value >> 1) * 6 + 1; }


