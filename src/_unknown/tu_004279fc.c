/* TU at 004279fc..00427c68 (cs11). Original file name unknown (no __FILE__ evidence). PARTIAL: 6 of 7 functions (00427a40 missing).
 * Names and layouts are working labels; the code is evidence. Profile 'plain' (-4s -s). */

struct InputReq {
    unsigned long mask;
    unsigned long state;
};

extern unsigned long gInDirty;
extern unsigned long gInA, gInB, gIn7c, gIn80, gIn84, gIn9c, gInA0, gInA4;

void InputUpdate();

void InputNop()
{
}

void InputSetRange(InputReq *r, unsigned long v)
{
    r->state &= ~0x3f0;
    r->state |= v & 0x3f0;
}

unsigned long InputGetHi(InputReq *r)
{
    return (r->state >> 16) & 0xf;
}

void InputSetLoHi(InputReq *r, unsigned long v)
{
    r->state &= ~0xf;
    r->state |= v & 0xf;
    r->state &= ~0xf0000;
    r->state |= (v & 0xf) << 16;
}

void InputPoll(InputReq *r)
{
    if (gInDirty) {
        InputUpdate();
        gInDirty = 0;
    }
    r->state = 0;
    if (r->mask & 0x4)
        r->state |= gInA;
    if (r->mask & 0x8000)
        r->state |= gInB;
}

void InputReset(InputReq *r)
{
    gInDirty = 0;
    r->state = 0;
    gIn7c = 0;
    gIn80 = 0;
    gIn84 = 0;
    gInA = 0;
    gInA0 = 0;
    gInA4 = 0;
    gIn9c = 0;
    gInB = 0;
}
