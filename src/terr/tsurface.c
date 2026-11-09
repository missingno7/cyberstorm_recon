typedef unsigned int uint;

extern "C" int RandomRange(unsigned int *, int, int);
extern "C" void MemoryRelease(void *);
extern void *MemoryAllocationRetry(unsigned long);
extern int MissionRandomTable[];
extern int SurfaceAverage(unsigned short, unsigned short, unsigned short, unsigned short);
extern int SurfaceMidpoint(unsigned short, unsigned short);
extern int SurfaceNoiseAt(int, int, int, int, int, int, unsigned char);
extern "C" void f_0042fc90(char *, int, int, int, unsigned char,
                             unsigned char, unsigned char, unsigned char,
                             unsigned char, unsigned char, unsigned char,
                             unsigned char, unsigned char, unsigned char);

extern "C" void f_0042f540(char *object)
{
    int depth = *(int *)(object + 0x68);
    int levelSpan = 1 << (*(int *)(object + 0x58) + 1);
    int side = depth * 2 - 3;
    unsigned int *rowStarts;
    rowStarts = (unsigned int *)new char[side * 16];
    unsigned char *expanded = (unsigned char *)new char[side * side];
    int row;
    int column;
    int outputColumn;
    unsigned int *rowPointer = rowStarts;
    unsigned int rowAddress = (unsigned int)expanded;
    for (row = 0; row < side; ++row) {
        *rowPointer = rowAddress;
        ++rowPointer;
        rowAddress += side;
    }

    unsigned char *source = *(unsigned char **)(object + 0x6c);
    int sourceRow = 1;
    int outputRow = 0;
    while (sourceRow < depth - 1) {
        int sourceColumn = 1;
        outputColumn = 0;
        while (sourceColumn < depth - 1) {
            unsigned char lowerRight = source[sourceRow * depth + sourceColumn];
            unsigned char lowerLeft = source[sourceRow * depth + sourceColumn - 1];
            unsigned char upperLeft = source[(sourceRow - 1) * depth + sourceColumn - 1];
            unsigned char upperRight = source[(sourceRow - 1) * depth + sourceColumn];
            *(unsigned char *)(rowStarts[outputRow] + outputColumn) =
                (unsigned char)SurfaceAverage(lowerRight, lowerLeft, upperLeft, upperRight);
            *(unsigned char *)(rowStarts[outputRow + 1] + outputColumn) =
                (unsigned char)SurfaceMidpoint(lowerLeft, lowerRight);
            *(unsigned char *)(rowStarts[outputRow] + outputColumn + 1) =
                (unsigned char)SurfaceMidpoint(lowerRight, upperRight);
            *(unsigned char *)(rowStarts[outputRow + 1] + outputColumn + 1) = lowerRight;
            ++sourceColumn;
            outputColumn += 2;
        }
        ++sourceRow;
        outputRow += 2;
    }

    int lowerRow = depth - 1;
    int upperRow = depth - 2;
    outputColumn = 0;
    for (column = 1; column < depth - 1; ++column) {
        unsigned char average = (unsigned char)(
            (source[lowerRow * depth + column] + source[upperRow * depth + outputColumn] +
             source[upperRow * depth + column] + source[lowerRow * depth + outputColumn]) >> 2);
        *(unsigned char *)(rowStarts[side - 1] + outputColumn) = average;
        *(unsigned char *)(rowStarts[side - 1] + outputColumn + 1) =
            (unsigned char)SurfaceMidpoint(source[lowerRow * depth + column],
                                           source[upperRow * depth + column]);
        ++outputColumn;
        ++outputColumn;
    }

    lowerRow = 1;
    upperRow = 0;
    unsigned int *rightEdge = rowStarts;
    for (row = 1; row < depth - 1; ++row) {
        unsigned char average = (unsigned char)(
            (source[lowerRow * depth + depth - 1] + source[upperRow * depth + depth - 2] +
             source[upperRow * depth + depth - 1] + source[lowerRow * depth + depth - 2]) >> 2);
        *(unsigned char *)(rightEdge[0] + side - 1) = average;
        *(unsigned char *)(rightEdge[1] + side - 1) =
            (unsigned char)SurfaceMidpoint(source[lowerRow * depth + depth - 1],
                                           source[lowerRow * depth + depth - 2]);
        lowerRow++;
        upperRow++;
        rightEdge += 2;
    }
    *(unsigned char *)(rowStarts[side - 1] - 4) = (unsigned char)SurfaceAverage(
        source[depth * depth - 1], source[(depth - 2) * depth - 2],
        source[(depth - 2) * depth - 1], source[depth * depth - 2]);

    for (unsigned int index = 0; index < 0x1eb; ++index) {
        int total = 0;
        int sample = 12;
        while (sample-- > 0)
            total += RandomRange((unsigned int *)MissionRandomTable, -32, 32);
        ((signed char *)0x4c64a8)[index] = (signed char)((short)total / 12);
    }
    *(unsigned int *)0x4c6694 = *(unsigned short *)(0x4904d4 + *(int *)(object + 0x60) * 2);
    unsigned int amplitude = (unsigned int)(*(int *)(object + 0x5c) * *(int *)(object + 0x5c) *
                                             *(int *)(object + 0x64)) /
                             *(unsigned int *)0x4c6694;
    for (row = 1; row < side - 1; row += 2) {
        for (column = 1; column < side - 1; column += 2) {
            unsigned int *top = rowStarts + row - 1;
            unsigned int *middle = top + 1;
            unsigned int *bottom = top + 2;
            f_0042fc90(object, levelSpan * ((row - 1) >> 1),
                       levelSpan * ((column - 1) >> 1), amplitude,
                       (unsigned char)*(int *)(object + 0x58),
                       *(unsigned char *)(top[0] + column - 1),
                       *(unsigned char *)(top[0] + column),
                       *(unsigned char *)(top[0] + column + 1),
                       *(unsigned char *)(middle[0] + column - 1),
                       *(unsigned char *)(middle[0] + column),
                       *(unsigned char *)(middle[0] + column + 1),
                       *(unsigned char *)(bottom[0] + column - 1),
                       *(unsigned char *)(bottom[0] + column),
                       *(unsigned char *)(bottom[0] + column + 1));
        }
    }
    MemoryRelease((void *)rowStarts[0]);
    MemoryRelease(rowStarts);
}

uint SurfaceAdjust(char *surface, unsigned short first, uint value, unsigned short second)
{
    if (*(int *)(surface + 0x54) != 0) {
        int low = (int)(unsigned short)value;
        int firstDifference = (int)first - low;
        int secondDifference = (int)second - low;
        int center = (firstDifference + secondDifference) / 2;
        value = ((low - center) * 2 + (int)first + (int)second) >> 2;
    }
    return value;
}

extern "C" void f_0042fc90(char *object, int x, int y, int amplitude,
                            unsigned char level, unsigned char p6,
                            unsigned char p7, unsigned char p8,
                            unsigned char p9, unsigned char p10,
                            unsigned char p11, unsigned char p12,
                            unsigned char p13, unsigned char p14)
{
    unsigned char *grid = *(unsigned char **)(object + 0x98);
    unsigned int width = *(unsigned int *)(object + 0x94);
    if (level == 0) {
        unsigned char *cell = grid + x * width + y;
        cell[0] = p6; cell[1] = p7; cell[2] = p8;
        cell += width;
        cell[0] = p9; cell[1] = p10; cell[2] = p11;
        cell += width;
        cell[0] = p12; cell[1] = p13; cell[2] = p14;
        return;
    }

    unsigned int half = 1U << (level - 1);
    unsigned int full = 1U << level;
    int delta = (int)(((unsigned int)(amplitude * 1000) / *(unsigned int *)0x4c6694) / 10);
    unsigned char upperLeft = p9;
    unsigned char topMiddle = p7;
    unsigned char lowerMiddle = p13;
    unsigned char upperRight = p11;
    if (*(int *)(object + 0x54) != 0) {
        upperLeft = (unsigned char)SurfaceAdjust(object, p6, p9, p12);
        topMiddle = (unsigned char)SurfaceAdjust(object, p6, p7, p8);
        lowerMiddle = (unsigned char)SurfaceAdjust(object, p12, p13, p14);
        upperRight = (unsigned char)SurfaceAdjust(object, p8, p11, p14);
    }
    unsigned int leftCenter = ((unsigned int)p9 + p10) >> 1;
    unsigned int rightCenter = ((unsigned int)p10 + p11) >> 1;
    unsigned int center = p10;
    if (*(int *)(object + 0x54) != 0)
        center = SurfaceAdjust(object, p9, p10, p11);
    int leftTop = ((int)p6 + p7) >> 1;
    int leftBottom = ((int)p12 + p13) >> 1;
    int centerTop = leftTop + (int)leftCenter;
    int centerBottom = (int)leftCenter + leftBottom;
    if (*(int *)(object + 0x54) != 0)
        leftCenter = SurfaceAdjust(object, leftTop, leftCenter, leftBottom);
    int rightTop = ((int)p7 + p8) >> 1;
    int rightBottom = ((int)p13 + p14) >> 1;
    int rightCenterTop = rightTop + (int)rightCenter;
    int rightCenterBottom = (int)rightCenter + rightBottom;
    if (*(int *)(object + 0x54) != 0)
        rightCenter = SurfaceAdjust(object, rightTop, rightCenter, rightBottom);
    unsigned int smoothCenter = center & 0xff;
    if (object[0x54 / 4] != 0)
        smoothCenter = SurfaceAdjust(object, topMiddle, center, lowerMiddle);

    int nTopLeft = SurfaceNoiseAt(x + (int)half * 2, y + (int)half, 0, 0, 0, delta,
                                   (unsigned char)leftCenter);
    int nTopRight = SurfaceNoiseAt(x + (int)half * 2, y + (int)half * 3, 0, 0, 0, delta,
                                    (unsigned char)rightCenter);
    int nLeft = SurfaceNoiseAt(x, y + (int)half, 0, 0, 0, delta, (unsigned char)leftTop) + leftTop;
    int nCenter = SurfaceNoiseAt(x + (int)half, y + (int)half, 0, 0, 0, delta,
                                  (unsigned char)(centerTop >> 1)) + (centerTop >> 1);
    int nBottomLeft = SurfaceNoiseAt(x + (int)half * 4, y + (int)half, 0, 0, 0, delta,
                                      (unsigned char)leftBottom) + leftBottom;
    int nBottomRight = SurfaceNoiseAt(x + (int)half * 3, y + (int)half, 0, 0, 0, delta,
                                       (unsigned char)(centerBottom >> 1)) + (centerBottom >> 1);
    int nRight = SurfaceNoiseAt(x, y + (int)half * 3, 0, 0, 0, delta,
                                 (unsigned char)rightTop) + rightTop;
    int nRightCenter = SurfaceNoiseAt(x + (int)half, y + (int)half * 3, 0, 0, 0, delta,
                                       (unsigned char)(rightCenterTop >> 1)) + (rightCenterTop >> 1);
    int nLowerRight = SurfaceNoiseAt(x + (int)half * 4, y + (int)half * 3, 0, 0, 0, delta,
                                      (unsigned char)rightBottom) + rightBottom;
    int nFarRight = SurfaceNoiseAt(x + (int)half * 3, y + (int)half * 3, 0, 0, 0, delta,
                                    (unsigned char)(rightCenterBottom >> 1)) + (rightCenterBottom >> 1);
    int nMiddleTop = SurfaceNoiseAt(x + (int)half, y + (int)half * 2, 0, 0, 0, delta,
                                     (unsigned char)((topMiddle + center) >> 1)) +
                     ((topMiddle + center) >> 1);
    int nMiddleBottom = SurfaceNoiseAt(x + (int)half * 3, y + (int)half * 2, 0, 0, 0, delta,
                                        (unsigned char)((center + lowerMiddle) >> 1)) +
                        ((center + lowerMiddle) >> 1);
    int nInnerLeft = SurfaceNoiseAt(x + (int)half, y, 0, 0, 0, delta,
                                     (unsigned char)(((int)p9 + p6) >> 1)) + (((int)p9 + p6) >> 1);
    int nInnerRight = SurfaceNoiseAt(x + (int)half, y + (int)full * 2, 0, 0, 0, delta,
                                      (unsigned char)(((int)p8 + p11) >> 1)) + (((int)p8 + p11) >> 1);
    int nBottom = SurfaceNoiseAt(x + (int)half * 3, y, 0, 0, 0, delta,
                                  (unsigned char)(((int)p12 + p9) >> 1)) + (((int)p12 + p9) >> 1);
    int nFarBottom = SurfaceNoiseAt(x + (int)half * 3, y + (int)full * 2, 0, 0, 0, delta,
                                     (unsigned char)(((int)p14 + p11) >> 1)) + (((int)p14 + p11) >> 1);

    f_0042fc90(object, x, y, delta, level - 1,
               p6, (unsigned char)nInnerLeft, topMiddle,
               (unsigned char)nLeft, (unsigned char)nCenter, (unsigned char)nMiddleTop,
               upperLeft, (unsigned char)nTopLeft, (unsigned char)smoothCenter);
    f_0042fc90(object, x, y + full, delta, level - 1,
               topMiddle, (unsigned char)nRight, p8,
               (unsigned char)nMiddleTop, (unsigned char)nRightCenter, (unsigned char)nInnerRight,
               (unsigned char)smoothCenter, (unsigned char)nTopRight, upperRight);
    f_0042fc90(object, x + full, y, delta, level - 1,
               upperLeft, (unsigned char)nTopLeft, (unsigned char)smoothCenter,
               (unsigned char)nBottom, (unsigned char)nBottomLeft, (unsigned char)nFarBottom,
               p12, (unsigned char)nBottomRight, lowerMiddle);
    f_0042fc90(object, x + full, y + full, delta, level - 1,
               (unsigned char)smoothCenter, (unsigned char)nTopRight, upperRight,
               (unsigned char)nFarBottom, (unsigned char)nFarRight, (unsigned char)nInnerRight,
               lowerMiddle, (unsigned char)nLowerRight, p14);
}

int SurfaceMidpoint(unsigned short first, unsigned short second)
{
    return (int)((uint)first + (uint)second) >> 1;
}

int SurfaceAverage(unsigned short first, unsigned short second,
                   unsigned short third, unsigned short fourth)
{
    int total = (int)((uint)first + (uint)second);
    total = (int)third + total;
    total = (int)fourth + total;
    return total >> 2;
}

extern "C" int RandomRange(unsigned int *seed, int low, int high);
extern "C" void MemoryRelease(void *memory);
extern void *MemoryAllocationRetry(unsigned long bytes);
extern int MissionRandomTable[];
struct ResMgr { int *Load(const char *, int, int *, unsigned int); };
extern ResMgr gResMgr;
extern "C" void AssertFail(const char *, int, const char *);
extern "C" unsigned int strlen(const char *);
extern "C" char *strcpy(char *, const char *);
extern "C" void *memcpy(void *, const void *, unsigned int);
extern void ReleasePoolData(void *);
static const char SurfaceSourcePath[] = "terr\\surface.c";

void SurfaceBuildNoiseTable()
{
    unsigned short index = 0;
    while (index < 0x1eb) {
        int total = 0;
        int sample = 0;
        while (sample != 12) {
            total += RandomRange((unsigned int *)MissionRandomTable, -32, 32);
            ++sample;
        }
        ((signed char *)0x4c64a8)[index] = (signed char)((short)total / 12);
        ++index;
    }
}

int SurfaceNoiseAt(int x, int y, int xStep, int yStep, int scale,
                   int amplitude, unsigned char base)
{
    x = xStep * scale + x;
    y = yStep * scale + y;
    unsigned int index = (unsigned int)(x * 0x29 + y * 0x2f + x * 0xb * x + y * 0x11 * y) % 0x1ea;
    int value = (unsigned int)base + ((int)((signed char *)0x4c64a8)[index] * amplitude >> 7);
    if (value > 0xff) value = 0xff;
    if (value < 0) value = 0;
    return value;
}

int *SurfaceAllocateGrid(int width, int height, int depth)
{
    int *rows = (int *)new char[width << 4];
    char *data = new char[height * width * depth];
    if ((unsigned int)width > 0) {
        int *row = rows;
        do {
            *row = (int)data;
            ++row;
            data += height * depth;
        } while (row < rows + width);
    }
    return rows;
}

void SurfaceReleaseGrid(void * volatile *rows)
{
    delete[] (char *)rows[0];
    delete[] (char *)rows;
}

int SurfaceStaticValue(int index)
{
    return *(unsigned short *)(0x4904d4 + index * 2);
}

int SurfaceLoadDefinition(char *object, const char *name)
{
    int *resource = gResMgr.Load(name, 1, 0, 0);
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    if (resource == 0) AssertFail(SurfaceSourcePath, __LINE__, 0);
    int *header = resource;
    // Source-line padding.
    // Source-line padding.
    if (*header != (int)0x88888888) AssertFail(SurfaceSourcePath, __LINE__, 0);
    char *text = (char *)(header + 1);
    unsigned int textLength = strlen(text);
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    if (textLength >= 0x50) AssertFail(SurfaceSourcePath, __LINE__, 0);
    strcpy(object, text);
    ++textLength;
    text += textLength;
    unsigned int *fields = (unsigned int *)text;
    *(unsigned int *)(object + 0x54) = fields[0];
    *(unsigned int *)(object + 0x58) = fields[1];
    *(unsigned int *)(object + 0x5c) = fields[2];
    *(unsigned int *)(object + 0x60) = fields[3];
    *(unsigned int *)(object + 0x64) = fields[4];
    *(unsigned int *)(object + 0x68) = fields[5];
    unsigned int width = fields[6];
    *(unsigned int *)(object + 0x94) = width;
    *(int *)(object + 0x50) = 0;
    int level = (int)width >> 6;
    while (level > 1) {
        ++*(int *)(object + 0x50);
        level >>= 1;
    }
    unsigned int depth = *(unsigned int *)(object + 0x68);
    char *allocation = new char[depth * depth * 3 + width * width];
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    // Source-line padding.
    if (allocation == 0) AssertFail(SurfaceSourcePath, __LINE__, 0);
    char *payload = (char *)(fields + 7);
    *(char **)(object + 0x98) = allocation;
    *(char **)(object + 0x6c) = allocation + width * width;
    *(char **)(object + 0x70) = allocation + width * width + depth * depth;
    memcpy(*(void **)(object + 0x6c), payload, depth * depth);
    payload += depth * depth;
    memcpy(*(void **)(object + 0x70), payload, depth * depth * 2);
    payload += depth * depth * 2;
    memcpy(object + 0xbc, payload, 0x100);
    memcpy(object + 0x74, payload + 0x100, 0x20);
    memcpy(object + 0x9c, payload + 0x120, 0x20);
    ReleasePoolData(resource);
    return 1;
}

void SurfaceClearAllocation(void *object)
{
    *(int *)((char *)object + 0x98) = 0;
}

int *SurfaceReleaseAllocation(int *object)
{
    if ((unsigned int)object[0x26] != 0) {
        delete[] (char *)object[0x26];
        object[0x26] = 0;
    }
    return object;
}

extern "C" unsigned int next_link_seed(unsigned int *seed);
extern "C" int sprintf(char *, const char *, ...);
extern "C" void *memset(void *, int, unsigned int);
extern "C" void f_0042f540(char *);

void SurfaceApplyNoise(int *object)
{
    unsigned char *grid = (unsigned char *)object[0x1b];
    unsigned char *weights = (unsigned char *)object[0x1c];
    unsigned int row = 0;
    if (object[0x1a] != 0) {
        do {
            unsigned int column = 0;
            if (object[0x1a] != 0) {
                do {
                    int value = RandomRange((unsigned int *)MissionRandomTable, weights[1], weights[0]) + *grid;
                    if (value < 0) *grid = 0;
                    else if (value < 0x100) *grid = (unsigned char)value;
                    else *grid = 0xff;
                    ++column;
                    ++grid;
                    weights += 2;
                } while (column < (unsigned int)object[0x1a]);
            }
            ++row;
        } while (row < (unsigned int)object[0x1a]);
    }
    unsigned char *descriptor = (unsigned char *)(object + 0x1d);
    int depth = object[0x1a];
    for (int i = 0;
        i != 8;
        ++i, descriptor += 4) {
        unsigned char count = descriptor[0];
        int value = RandomRange((unsigned int *)MissionRandomTable, descriptor[2], descriptor[3]);
        while (count > 0) {
            unsigned int index = next_link_seed((unsigned int *)MissionRandomTable) % (depth * depth);
            ((unsigned char *)object[0x1b])[index] = (unsigned char)value;
            --count;
        }
    }
}

void SurfaceApplyNoiseDetails(int *object)
{
    unsigned char *descriptor = (unsigned char *)(object + 0x27);
    unsigned int width = object[0x25];
    for (int n = 0; n < 8; ++n, descriptor += 4) {
        unsigned int count = descriptor[0];
        int value = RandomRange((unsigned int *)MissionRandomTable, descriptor[2], descriptor[3]);
        do {
            if (!count) break;
            ((unsigned char *)object[0x26])[next_link_seed((unsigned int *)MissionRandomTable) % (width * width)] = (unsigned char)value;
            --count;
        } while (1);
    }
}

void SurfaceInitializeProcedural(int *object, int fileNumber)
{
    char filename[32];
    sprintf(filename, "scape%d.dat", fileNumber);
    if ((unsigned int)object[0x26] != 0) {
        delete[] (char *)object[0x26];
        object[0x26] = 0;
    }
    SurfaceLoadDefinition((char *)object, filename);
    SurfaceApplyNoise(object);
    f_0042f540((char *)object);
    unsigned char *descriptor = (unsigned char *)(object + 0x27);
    int width = object[0x25];
    for (int i = 0;
        i < 8;
        ++i, descriptor += 4) {
        unsigned char value = (unsigned char)RandomRange((unsigned int *)MissionRandomTable, descriptor[2], descriptor[3]);
        int count = descriptor[0];
        do {
            int remaining = count--;
            if (remaining <= 0) break;
            unsigned int index = next_link_seed((unsigned int *)MissionRandomTable) % (width * width);
            ((unsigned char *)object[0x26])[index] = (unsigned char)value;
        } while (1);
    }
}

void SurfaceClearAllocationAndRelease(int *object)
{
    if ((unsigned int)object[0x26] != 0) {
        delete[] (char *)object[0x26];
        object[0x26] = 0;
    }
}

unsigned char SurfaceGridValue(int *object, unsigned int *point)
{
    unsigned int x = (point[0] & 0x3f) << *(unsigned char *)((char *)object + 0x50);
    unsigned int y = (point[1] & 0x3f) << *(unsigned char *)((char *)object + 0x50);
    if (x >= (unsigned int)object[0x25]) x = object[0x25] - 1;
    if (y >= (unsigned int)object[0x25]) y = object[0x25] - 1;
    return *(unsigned char *)(object[0x26] + x * object[0x25] + y);
}

void SurfaceGridSetValue(int *object, unsigned int *point, unsigned char value)
{
    if (object[0x26] != 0) {
        unsigned int x = (point[0] & 0x3f) << *(unsigned char *)((char *)object + 0x50);
        unsigned int y = (point[1] & 0x3f) << *(unsigned char *)((char *)object + 0x50);
        if (x >= (unsigned int)object[0x25]) x = object[0x25] - 1;
        if (y >= (unsigned int)object[0x25]) y = object[0x25] - 1;
        *(unsigned char *)(object[0x26] + x * object[0x25] + y) = value;
    }
}

void SurfaceApplyColorMap(int *object)
{
    unsigned char *data = (unsigned char *)object[0x26];
    for (unsigned int row = 0; row < (unsigned int)object[0x25]; ++row)
        for (unsigned int column = 0; column < (unsigned int)object[0x25]; ++column, ++data)
            *data = ((unsigned char *)object)[0xbc + *data];
}

unsigned int SurfaceRandomize(int *object, void *unused, int fillGrid)
{
    unsigned int result = 0;
    char filename[32];
    sprintf(filename, "scape_d.dat", unused);
    if ((unsigned int)object[0x26] != 0) {
        delete[] (char *)object[0x26];
        ((char *)object)[0x98] = 0;
        ((char *)object)[0x99] = 0;
        ((char *)object)[0x9a] = 0;
        ((char *)object)[0x9b] = 0;
    }
    SurfaceLoadDefinition((char *)object, filename);
    unsigned char *grid = (unsigned char *)object[0x1b];
    unsigned char lowGrid = grid[0];
    unsigned char highGrid = grid[3];
    unsigned char noiseLow = grid[2];
    unsigned char noiseHigh = grid[1];
    unsigned char scatterLow = grid[4];
    unsigned char scatterHigh = grid[5];
    unsigned int count = object[0x1a] * object[0x1a];
    for (unsigned int cell = 0; cell < count; ++cell)
        grid[cell] = (unsigned char)RandomRange((unsigned int *)MissionRandomTable, noiseLow, noiseHigh);
    unsigned char scatterCount = (unsigned char)RandomRange((unsigned int *)MissionRandomTable, lowGrid, highGrid);
    for (unsigned int i = 0; i < scatterCount; ++i) {
        unsigned int index = next_link_seed((unsigned int *)MissionRandomTable) % count;
        int value = RandomRange((unsigned int *)MissionRandomTable, scatterLow, scatterHigh);
        grid[index] = (unsigned char)value;
    }
    f_0042f540((char *)object);
    unsigned char *descriptor = (unsigned char *)(object + 0x27);
    unsigned int width = object[0x25];
    for (int descriptorIndex = 0; descriptorIndex != 8; ++descriptorIndex, descriptor += 4) {
        unsigned char number = descriptor[0];
        int value = RandomRange((unsigned int *)MissionRandomTable, descriptor[2], descriptor[3]);
        while (number > 0) {
            unsigned int index = next_link_seed((unsigned int *)MissionRandomTable) % (width * width);
            ((unsigned char *)object[0x26])[index] = (unsigned char)value;
            --number;
        }
    }
    if (fillGrid != 0) {
        unsigned int cells = width * width;
        unsigned char base = (unsigned char)RandomRange((unsigned int *)MissionRandomTable, noiseLow, noiseHigh);
        memset((void *)object[0x26], base, cells);
        for (unsigned int fillIndex = 0; fillIndex < scatterCount; ++fillIndex) {
            unsigned int index = next_link_seed((unsigned int *)MissionRandomTable) % cells;
            int value = RandomRange((unsigned int *)MissionRandomTable, scatterLow, scatterHigh);
            ((unsigned char *)object[0x26])[index] = (unsigned char)value;
        }
        result = base;
    }
    return result;
}



