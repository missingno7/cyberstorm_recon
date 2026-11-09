/* TU at 004279fc..00427c68 (cs11). Original file name unknown (no __FILE__ evidence). All seven main code extents are reconstructed.
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

/* Keypad directions and repeated-button timers, reconstructed from 00427a40. */
struct GuiPoint { long x, y; };
extern GuiPoint g4bb78c;
struct InputKeyBinding { unsigned long scan, direction; };
static InputKeyBinding inputKeys[] = {
    {0x4b,4}, {0x4d,8}, {0x48,1}, {0x50,2},
    {0x47,5}, {0x4f,6}, {0x49,9}, {0x51,10}, {0,0}
};
extern "C" unsigned long GuiReadKey(unsigned long);
extern "C" unsigned long ReadKeyEvent();
extern "C" unsigned long AltKeyDown(), ControlKeyDown(), ShiftKeyDown();
extern "C" unsigned long MousePoll(GuiPoint *), ReadMousePress(GuiPoint *), PlyGetKey(GuiPoint *);
struct Timer;
void TimerSet(Timer *, unsigned long);
int TimerExpired(Timer *);
struct Timer {
    unsigned long start, end, duration;
    Timer(unsigned long interval) { TimerSet(this, interval); }
};

void InputUpdate()
{
    gInA = (gInA & 0x30) << 2;
    gInB = (gInB & 0x30) << 2;
    for (int i=0; inputKeys[i].scan; ++i) {
        if (GuiReadKey(inputKeys[i].scan)) gInA |= inputKeys[i].direction;
    }
    unsigned long key=ReadKeyEvent();
    gIn84=(key >> 8) & 255;
    gIn7c=key >> 16;
    gIn80=key & 255;
    gIn9c=AltKeyDown();
    gInA0=ControlKeyDown();
    gInA4=ShiftKeyDown();
    unsigned long buttons=MousePoll(&g4bb78c);
    if (buttons) gInB |= (buttons & 3) << 8;
    else {
        buttons=ReadMousePress(&g4bb78c);
        if (buttons) gInB |= (buttons & 3) << 4;
        else gInB |= (PlyGetKey(&g4bb78c) & 3) << 4;
    }
    static Timer doubleClick(0);
    if ((gInA & 0x50)==0x10) {
        if (!TimerExpired(&doubleClick)) gInA |= 0x100;
        TimerSet(&doubleClick,250);
    }
    static Timer repeat(0);
    if (TimerExpired(&repeat) && (gInA & 15)) {
        TimerSet(&repeat,250);
        gInA |= (gInA & 15) << 16;
    } else gInA &= ~0xf0000UL;
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
