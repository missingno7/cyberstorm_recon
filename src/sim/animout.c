static char animoutSourceFile[]=__FILE__;
extern "C" void *memset(void *, int, unsigned int); extern "C" void *memcpy(void *, const void *, unsigned int);

extern unsigned long animDrawState[10];
extern int animDrawX0;
extern int animDrawX1;
extern int animDrawRowStart;
extern int animDrawRowLast;
extern int animDrawRowLimit;
extern int animDrawWidth;
extern unsigned char *animDrawBuffer;
extern int animDrawX1End;

void ResetAnimDrawState(void)
{
    animDrawState[0] = 0;
    animDrawState[1] = 0;
    animDrawState[2] = 0;
    animDrawState[3] = 0;
    animDrawState[4] = 0;
    animDrawState[5] = 0;
    animDrawState[6] = 0;
    animDrawState[7] = 0;
    animDrawState[8] = 0;
    animDrawState[9] = 0;
}

void AnimClearBetweenRows(void)
{
    int row = animDrawRowStart;
    unsigned long right = ((unsigned long)animDrawX0 << 16) | 0x8000;
    unsigned long left = ((unsigned long)animDrawX1 << 16) | 0x8000;
    int height = animDrawRowLast;
    int step = 0;
    height -= row;
    if (height != 0)
        step = ((animDrawX1End - animDrawX1) * 0x10000) / height;
    while (row < animDrawRowLimit) {
        if (row >= 0) {
            int rightPixel = (int)left >> 16;
            int leftPixel = (int)right >> 16;
            if (rightPixel >= 0 && leftPixel < animDrawWidth) {
                if (animDrawWidth - 1 < rightPixel)
                    rightPixel = animDrawWidth - 1;
                if (leftPixel < 0)
                    leftPixel = 0;
                    memset(animDrawBuffer + (animDrawWidth * row + leftPixel), 0,
                       (rightPixel - leftPixel) + 1);
            }
        }
        ++row;
        if (height-- > 0) {
            right -= step;
            left += step;
        }
    }
}

extern unsigned char *targetDefinitionTable;
extern unsigned char animFallbackName[];
extern int gZoom;
extern int g_004d3405;
extern int gView[];
extern int g_004d4320;
extern int g_004d4324;
extern int g_004d4328;
extern int g_004d432c;
extern int g_004d433c;
extern int g_004d4338;
extern int g_004d4344;
extern int g_004d4348;
extern int g_004d434c;
extern int g_004d3690;
extern int g_004d42b0;
extern int g_004d42b4;
extern int g_004d42b8;
extern unsigned char gDebugText[];
extern void *gDrawRemap;
struct MenuImage;
extern char g513888;
extern MenuImage *MenuImageData(void *manager, const MenuImage *image); extern void RenderImage(const MenuImage *, int, int, int, int, int);
extern int animViewportX;
extern int animViewportY;
extern int animViewportWidth;
extern int animViewportHeight;
extern int ResolveName(unsigned long name, int *point);
extern void f_0042c040(int *source, int *result, int *point, int target);
extern void f_0047f114(int *bitmap, int x, int y, int a, int width, int height);

struct DrawPoint { int x; int y; DrawPoint() {} DrawPoint(const DrawPoint &p) { x = p.x; y = p.y; } }; struct DrawDefaults {
    DrawPoint steps[6];
    DrawDefaults()
    {
        steps[0].x = 1;
        steps[0].y = 1;
        steps[1].x = 0;
        steps[1].y = 2;
        steps[2].x = -1;
        steps[2].y = 1;
        steps[3].x = -1;
        steps[3].y = -1;
        steps[4].x = 0;
        steps[4].y = -2;
        steps[5].x = 1;
        steps[5].y = -1;
    }
};

void f_00459020(DrawPoint point, int phase)
{
    static DrawDefaults defaults;
    int target = ResolveName((unsigned long)animFallbackName, (int *)&point);
    if (target == 0)
        return;

    int phaseOffset = (phase + g_004d3405) % 6;
    unsigned char *targetEntry = targetDefinitionTable + target * 12;
    unsigned char oldType = targetEntry[1];
    DrawPoint step = defaults.steps[phaseOffset];
    point.x += step.x;
    point.y += step.y;

    phaseOffset = (g_004d3405 + 1) % 6;
    step = defaults.steps[phaseOffset];
    g_004d432c = 99999;

    int result[2];
    if (phase == 1 && targetEntry[4]) {
        f_0042c040(gView, result, (int *)&point, target);
        result[1] -= gZoom / 8;
        if (result[1] < g_004d432c) {
            g_004d4328 = result[0];
            g_004d432c = result[1];
        }
    }

    int scan = 0;
    do {
        int next = ResolveName((unsigned long)animFallbackName, (int *)&point);
        if (next != 0 && oldType < targetDefinitionTable[next * 12 + 1]) {
            f_0042c040(gView, result, (int *)&point, next);
            if (result[1] < g_004d432c) {
                g_004d4328 = result[0];
                g_004d432c = result[1];
            }
        }
        point.x += step.x;
        ++scan;
        point.y += step.y;
    } while (scan < 3);

    if (g_004d432c != 99999) {
        int halfWidth = animDrawWidth / 2;
        int halfHeight = animDrawRowLimit / 2;
        animDrawX0 = (((g_004d4328 - g_004d4320 - animViewportX) * animDrawWidth) + halfWidth) / animViewportWidth;
        animDrawX1 = (((g_004d4328 + g_004d4320 - animViewportX) * animDrawWidth) + halfWidth) / animViewportWidth;
        g_004d4338 = (((g_004d4328 - g_004d4324 - animViewportX) * animDrawWidth) + halfWidth) / animViewportWidth;
        g_004d4348 = (((g_004d4328 + g_004d4324 - animViewportX) * animDrawWidth) + halfWidth) / animViewportWidth;
        g_004d4344 = (((g_004d432c - g_004d4324 - animViewportY) * animDrawRowLimit) + halfHeight) / animViewportHeight;
        g_004d433c = (((g_004d432c - animViewportY) * animDrawRowLimit) + halfHeight) / animViewportHeight;
        g_004d434c = g_004d433c;
        unsigned long right = ((unsigned long)animDrawX1 << 16) | 0x8000;
        unsigned long left = ((unsigned long)animDrawX0 << 16) | 0x8000;
        register int row = g_004d4344;
        int height = g_004d433c - row;
        animDrawRowStart = row;
        int slope = 0;
        if (height != 0)
            slope = ((g_004d4348 - animDrawX1) * 0x10000) / height;
        while (row < animDrawRowLimit) {
            if (row >= 0) {
                int rightPixel = (int)right >> 16;
                int leftPixel = (int)left >> 16;
                if (rightPixel >= 0 && leftPixel < animDrawWidth) {
                    if (animDrawWidth - 1 < rightPixel)
                        rightPixel = animDrawWidth - 1;
                    if (leftPixel < 0)
                        leftPixel = 0;
                    memset(animDrawBuffer + animDrawWidth * row + leftPixel,
                           0, (rightPixel - leftPixel) + 1);
                }
            }
            ++row;
            int previousHeight = height;
            --height;
            if (previousHeight > 0) {
                left -= slope;
                right += slope;
            }
        }
    }
}

extern int *AnimBitmapFrame(void *action);


struct MenuImage { unsigned long offset; unsigned short width, height; unsigned char kind, compressed; char unknown0a[2]; };
extern MenuImage animImageHeader;
inline int AnimClearEnabled() { return (g_004d3690 & 1) != 0; }
extern "C" void AnimDrawBitmap(MenuImage *image, DrawPoint *position)
{
    if (image != 0) {
        if (image->compressed)
            image = MenuImageData(&g513888, image);
        if (AnimClearEnabled()) {
            if (!image->compressed) {
                animImageHeader = *image;
                memcpy(gDebugText, (char *)image + animImageHeader.offset, image->width * image->height);
                animImageHeader.offset = gDebugText - (unsigned char *)&animImageHeader;
                image = &animImageHeader;
            }
            animDrawBuffer = (unsigned char *)image + image->offset;
            animDrawWidth = image->width;
            animDrawRowLimit = image->height;
            g_004d4320 = gZoom;
            g_004d4324 = gZoom * 2;
            
            f_00459020(*position, 0);
            
            f_00459020(*position, 1);
            
            f_00459020(*position, 2);
        }
        RenderImage(image, animViewportX, animViewportY, 0,
                   animViewportWidth, animViewportHeight);
    }
}











extern int viewportClipRight;
extern int viewportClipBottom;
extern int viewportClipLeft;
extern int viewportClipTop;
extern int animViewportX;
extern int animViewportY;
extern int animViewportWidth;
extern int animViewportHeight;
extern "C" void AnimDrawBitmap(MenuImage *image, DrawPoint *position);

int AnimSetViewportRect(int *bitmap, int *position, int x, int y, int width, int height)
{
    int result;
    int positionCopy[2];
    animViewportWidth = width;
    animViewportHeight = height;
    animViewportX = x;
    animViewportY = y;
    if (viewportClipRight < x || viewportClipBottom < y ||
        x + width < viewportClipLeft || y + height < viewportClipTop ||
        width == 0 || height == 0) {
        result = 0;
    } else {
        positionCopy[0] = position[0];
        positionCopy[1] = position[1];
        AnimDrawBitmap((MenuImage *)bitmap, (DrawPoint *)positionCopy);
        result = 1;
    }
    return result;
}

extern int animationMetaAt0cbc[];
extern int animationMetaAt0cc0[];
extern int animationKindTable[];
extern short *f_0045a050(unsigned char *action);
struct ExploPoint { int x; int y; };
extern void SoundAtPoint(ExploPoint *position, int sound);
extern void f_0045a290(unsigned char *action);
struct ExploFrame;
extern void DrawSprite(void *surface, ExploFrame *frame, int x, int y,
                       int transparent, int width, int height);
extern void AnimActionInitialize(void *action, unsigned char type,
                                 unsigned short duration, unsigned char rate,
                                 unsigned char initialFlags);
extern void WorldActionNotify(void *action, unsigned char state);
extern int AnimActionStep(unsigned char *action);
extern int *AnimBitmapFrame(unsigned char *action);
extern void f_00459020(DrawPoint point, int phase);

struct AnimDrawAction {
    AnimDrawAction();

    unsigned char bytes[0x12];
};

typedef DrawPoint AnimDrawPoint;

#pragma pack(push,1)
struct DrawAction { int id; unsigned char kind, state, variant, rate, frame, frames, flags, dirty; unsigned short type; int deadline; DrawPoint position, destination; int target, heading; DrawPoint screen; int height; unsigned char color, phase, a38, a39, a3a; int clipX, clipY, clipWidth, clipHeight; int scale; DrawPoint velocity, offset; int a5f,a63; };
#pragma pack(pop)
void f_00459640(unsigned char *p)
{
    if (((DrawAction *)p)->state != 1 || ((DrawAction *)p)->state != 7) {
        short *offset = f_0045a050(p);
        SoundAtPoint((ExploPoint *)(p + 0x12), (int)offset);
    }
    animViewportWidth = ((DrawAction *)p)->clipWidth;
    animViewportHeight = ((DrawAction *)p)->clipHeight;
    animViewportX = ((DrawAction *)p)->clipX;
    animViewportY = ((DrawAction *)p)->clipY;
    if (animViewportX > viewportClipRight ||
        animViewportY > viewportClipBottom ||
        animViewportX + animViewportWidth < viewportClipLeft ||
        animViewportY + animViewportHeight < viewportClipTop)
        return;
    {
        int record = *(unsigned short *)(p + 12) * 15;
        int *metadata = animationKindTable + record;
        f_0045a290(p);
        int *bitmap = AnimBitmapFrame(p);

        if (metadata[3] > 1) {
            DrawSprite(&g513888, (ExploFrame *)bitmap, animViewportX, animViewportY,
                       0, animViewportWidth, animViewportHeight);
        } else {
            AnimDrawPoint mainPosition=((DrawAction *)p)->destination;
            AnimDrawBitmap((MenuImage *)bitmap, (DrawPoint *)&mainPosition);
        }
        gDrawRemap = 0;
        if (((DrawAction *)p)->state == 0x11) {
            static AnimDrawAction actions[3];
            static int actionIndex;
            while (actionIndex != 3) {
                AnimActionInitialize(actions[actionIndex].bytes, 4, 1, 99, 0x20);
                WorldActionNotify(actions[actionIndex].bytes, 0x15);
                actions[actionIndex].bytes[8] = (unsigned char)actionIndex * 3;
            }




            if (metadata[4] == 0) {
                animViewportWidth = ((DrawAction *)p)->clipWidth;
                animViewportHeight = ((DrawAction *)p)->clipHeight;
                animViewportX = ((DrawAction *)p)->screen.x - ((animViewportWidth << 15) >> 16);
                animViewportY = ((DrawAction *)p)->screen.y -
                                (animViewportWidth * 0x7000 >> 16);
                AnimDrawPoint firstPosition = ((DrawAction *)p)->destination;
                AnimDrawBitmap((MenuImage *)AnimBitmapFrame(actions[0].bytes),
                               (DrawPoint *)&firstPosition);
                animViewportX = ((DrawAction *)p)->screen.x -
                                (animViewportWidth * 0x7000 >> 16);
                animViewportY = ((DrawAction *)p)->screen.y - ((animViewportWidth << 15) >> 16);
                AnimDrawPoint secondPosition = ((DrawAction *)p)->destination;
                AnimDrawBitmap((MenuImage *)AnimBitmapFrame(actions[1].bytes),
                               (DrawPoint *)&secondPosition);
                int diagonal = animViewportWidth * 0x9000 >> 16;
                animViewportX = ((DrawAction *)p)->screen.x - diagonal;
                animViewportY = ((DrawAction *)p)->screen.y - diagonal;
                AnimDrawPoint thirdPosition = ((DrawAction *)p)->destination;
                AnimDrawBitmap((MenuImage *)AnimBitmapFrame(actions[2].bytes),
                               (DrawPoint *)&thirdPosition);
            } else {
                animViewportWidth = ((DrawAction *)p)->clipWidth;
                animViewportHeight = ((DrawAction *)p)->clipHeight;
                animViewportX = ((DrawAction *)p)->clipX;
                animViewportY = ((DrawAction *)p)->clipY;
                AnimDrawPoint fallbackPosition = ((DrawAction *)p)->destination;
                AnimDrawBitmap((MenuImage *)AnimBitmapFrame(actions[0].bytes),
                               (DrawPoint *)&fallbackPosition);
            }
            AnimActionStep(actions[0].bytes);
            AnimActionStep(actions[1].bytes);
            AnimActionStep(actions[2].bytes);
        }
    }
}

















