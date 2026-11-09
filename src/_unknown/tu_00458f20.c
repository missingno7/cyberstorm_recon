#include <string.h>

extern volatile unsigned long animDrawState[10];
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
    int step = 0;
    int row = animDrawRowStart;
    unsigned long left = ((unsigned long)animDrawX1 << 16) | 0x8000;
    unsigned long right = ((unsigned long)animDrawX0 << 16) | 0x8000;
    int height = animDrawRowLast - row;
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
                memset(animDrawBuffer + animDrawWidth * row + leftPixel, 0,
                       (rightPixel - leftPixel) + 1);
            }
        }
        ++row;
        if (height > 0) {
            --height;
            left += step;
            right -= step;
        } else {
            --height;
        }
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
extern "C" void AnimDrawBitmap(int *bitmap, int *position);

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
        AnimDrawBitmap(bitmap, positionCopy);
        result = 1;
    }
    return result;
}

unsigned long UnknownIdentityThunk(unsigned long value)
{
    return value;
}

