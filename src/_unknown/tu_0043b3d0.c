struct Curve {
    int mode, curvature;
    int lowX, lowY, below;
    int highX, highY, above;
};

int FixedSin(int angle);

// FixedSin uses a full 32-bit turn and returns a 16.16 sine.
enum { AngleUnitsPerDegree = 0xb60b61 };

void CurveInit(Curve *c, int mode, int curvature, int lowX, int lowY, int below,
               int highX, int highY, int above)
{
    c->mode = mode;
    c->curvature = curvature;
    c->lowX = lowX;
    c->lowY = lowY;
    c->below = below;
    c->highX = highX;
    c->highY = highY;
    c->above = above;
}

// The offset type selects signed division for scalar curves and unsigned division for skills.
#define BENT_VALUE(T, c, x, result) \
    do { \
        int amplitude = (c)->lowY > (c)->highY ? (c)->lowY - (c)->highY : (c)->highY - (c)->lowY; \
        int width = (c)->highX - (c)->lowX; \
        T offset = (x) - (c)->lowX; \
        int degrees = offset * 180 / width; \
        (result) = ((c)->highY - (c)->lowY) * offset / width; \
        int sine = FixedSin((unsigned)degrees * AngleUnitsPerDegree); \
        (result) += sine * amplitude * (c)->curvature / 100 / 65536; \
    } while (0)

unsigned CurveForward(Curve *c, unsigned x)
{
    if (!c->mode)
        return x;
    if ((int)x <= c->lowX)
        return c->below;
    if ((int)x >= c->highX)
        return c->above;
    if (c->highX <= c->lowX)
        return c->lowY;

    switch (c->mode) {
    case 1:
        return ((int)x - c->lowX) * (c->highY - c->lowY) / (c->highX - c->lowX);
    case 2: {
        int value;
        BENT_VALUE(int, c, (int)x, value);
        if (value < c->lowY)
            return c->lowY;
        if (value > c->highY)
            return c->highY;
        return value;
    }
    }
    return 0;
}

unsigned CurveInverse(Curve *c, unsigned x)
{
    if (!c->mode)
        return x;
    if ((int)x <= c->lowX)
        return c->highY;
    if ((int)x >= c->highX)
        return c->lowY;
    if (c->highX <= c->lowX)
        return c->highY;

    switch (c->mode) {
    case 1:
        return ((int)x - c->lowX) * (c->highY - c->lowY) / (c->highX - c->lowX);
    case 2: {
        int value;
        BENT_VALUE(int, c, (int)x, value);
        if (value < c->lowY)
            return c->highY;
        if (value > c->highY)
            return c->lowY;
        return c->highY - value;
    }
    }
    return 0;
}

unsigned CurveSlope(Curve *c, unsigned fixedX)
{
    unsigned x = fixedX >> 12;
    int result = 0;
    if (!c->mode)
        return x << 12;
    if (x <= c->lowX)
        return (unsigned)c->highY << 12;
    if (x >= c->highX)
        return (unsigned)c->lowY << 12;
    if (c->highX <= c->lowX)
        return (unsigned)c->highY << 12;

    switch (c->mode) {
    case 1:
        result = (x - c->lowX) * (c->highY - c->lowY) / (c->highX - c->lowX);
        // Fall through to the bent curve.
    case 2:
        int value;
        BENT_VALUE(unsigned, c, x, value);
        if (value < c->lowY)
            result = c->highY;
        else if (value > c->highY)
            result = c->lowY;
        else
            result = c->highY - value;
        break;
    }
    return (unsigned)result << 12;
}
