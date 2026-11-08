/* TU at 004297d4..00429a04 (cs11). Original file name unknown (no __FILE__ evidence). PARTIAL: 2 of 5 functions.
 * Names and layouts are working labels; the code is evidence. Profile 'plain' (-4s -s). */

struct PlyEntry { char data[0x84]; };
struct PlyKey { long a; long b; };

int PlyMatch(PlyEntry *e, long a, long b);
void PlyGetKey(PlyKey *k);

extern PlyEntry gPly[8];
extern int gPlyCount;
extern PlyKey gPlyKey;

int PlyFind(PlyKey *k)
{
    int i;
    PlyEntry *e = gPly;
    for (i = 0; i < gPlyCount; i++, e++) {
        if (PlyMatch(e, k->a, k->b))
            return i;
    }
    return -1;
}

int PlyFindCurrent()
{
    int i;
    PlyEntry *e;
    PlyGetKey(&gPlyKey);
    e = gPly;
    for (i = 0; i < gPlyCount; i++, e++) {
        if (PlyMatch(e, gPlyKey.a, gPlyKey.b))
            return i;
    }
    return -1;
}
