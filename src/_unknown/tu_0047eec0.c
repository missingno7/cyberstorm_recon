struct Point { int x, y; Point() {} Point(int a, int b) { x=a; y=b; } };
extern "C" int VectorAtan(int, int);
extern "C" int FixedCos(int);
extern "C" int FixedSin(int);
extern "C" int FixedMultiplyShift(int, int, int);
extern "C" void RotatePoint(Point *, int);
extern "C" int ApproachFixed(int from, int to, int rate)
{
    int delta = (to - from) >> 16;
    if (delta) {
        rate >>= 16;
        if ((delta < 0 ? -delta : delta) > rate) delta = delta < 0 ? -rate : rate;
        from += delta << 16;
    }
    return from;
}
extern "C" int VectorAngle(Point *p) { return VectorAtan(p->x, p->y) - 0x4000001a; }
extern "C" int PointAngle(Point *a, Point *b) { return VectorAtan(b->x - a->x, b->y - a->y) - 0x4000001a; }
extern "C" Point *PointFromAngle(Point *p, int length, int angle)
{
    p->x = FixedMultiplyShift(length, FixedCos(angle), 16);
    p->x = -p->x;
    p->y = FixedMultiplyShift(length, FixedSin(angle), 16);
    return p;
}
Point *f_0047efb0(Point *p, int angle) { RotatePoint(p, angle); return p; }
extern "C" int PointInConvexPolygon(Point *point, Point *vertices, int count)
{
    Point *previous = vertices + (count - 1);
    for (int i = 0; i < count; ++i) {
        Point edge(vertices->y - previous->y, previous->x - vertices->x);
        Point relative(point->x - vertices->x, point->y - vertices->y);
        if (edge.y * relative.y + relative.x * edge.x > 0) return 0;
        previous = vertices++;
    }
    return 1;
}
