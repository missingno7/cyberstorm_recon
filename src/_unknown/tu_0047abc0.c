/* TU at 0047abc0..0047ac73 (cs11). Original file name unknown: no __FILE__ string references this code.
 * Function and type names are working labels (no symbols); the code is evidence. Profile 'ot'. */
#include <windows.h>

struct Timer {
    DWORD start;
    DWORD end;
    DWORD duration;
};

DWORD TimeNow(void)
{
    return GetTickCount();
}

void TimeSleep(DWORD ms)
{
    Sleep(ms);
}

void TimerSet(Timer *t, DWORD duration)
{
    t->start = GetTickCount();
    t->end = t->start + duration;
    t->duration = duration;
}

void TimerNext(Timer *t, DWORD duration)
{
    t->start = t->end;
    t->end = t->start + duration;
    t->duration = duration;
}

int TimerExpired(Timer *t)
{
    return GetTickCount() >= t->end;
}

DWORD TimerElapsed(Timer *t)
{
    return GetTickCount() - t->start;
}

DWORD TimerRemaining(Timer *t)
{
    DWORD elapsed = GetTickCount() - t->start;
    if (elapsed < t->duration)
        return t->duration - elapsed;
    return 0;
}
