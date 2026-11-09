#include <windows.h>
extern "C" void RegisterAssertCallback(void (*)());
extern int g5138ac, g5138b0;
struct MousePoint {
    int x, y;
    MousePoint() { x = 0; y = 0; }
};
volatile MousePoint mousePosition, pressPosition, releasePosition, savedPosition;
volatile int mouseButtons, pressButtons, releaseButtons;
extern "C" void f_47d710();
extern "C" int MousePresent() { return GetSystemMetrics(SM_MOUSEPRESENT); }
extern "C" int InputInit()
{
    mouseButtons = 0;
    mousePosition.y = mouseButtons;
    mousePosition.x = mousePosition.y;
    if (MousePresent()) {
        RegisterAssertCallback(f_47d710);
        return 1;
    }
    return 0;
}
extern "C" void f_47d710() {}
extern "C" void ClampMousePoint(MousePoint *p)
{
    if (p->x < -1) p->x = -1;
    else if (p->x > g5138ac) p->x = g5138ac;
    if (p->y < -1) p->y = -1;
    else if (p->y > g5138b0) p->y = g5138b0;
}
extern "C" int PlyGetKey(MousePoint *p)
{
    p->x = mousePosition.x;
    p->y = mousePosition.y;
    ClampMousePoint(p);
    return mouseButtons;
}
extern "C" int ReadMousePosition(MousePoint *p)
{
    p->x = mousePosition.x;
    p->y = mousePosition.y;
    return mouseButtons;
}
extern "C" int ReadMouseRelease(MousePoint *p)
{
    int buttons = releaseButtons;
    if (buttons) {
        p->x = releasePosition.x; p->y = releasePosition.y;
        releaseButtons = 0;
        ClampMousePoint(p);
    }
    return buttons;
}
extern "C" int ReadMousePress(MousePoint *p)
{
    int buttons = pressButtons;
    if (buttons) {
        p->x = pressPosition.x; p->y = pressPosition.y;
        pressButtons = 0;
        ClampMousePoint(p);
    }
    return buttons;
}
extern "C" int MousePoll() { return 0; }
extern "C" int MouseButtonDown(int mask) { return (mouseButtons & mask) != 0; }
extern "C" void UpdateMouseState(int x, int y, int buttons)
{
    int previous = mouseButtons;
    mousePosition.x = x; mousePosition.y = y;
    mouseButtons = buttons;
    // Preserve the original precedence: bitwise AND of a boolean and a mask.
    if (mouseButtons & (previous == 0)) {
        pressPosition.x = x; pressPosition.y = y; pressButtons = mouseButtons;
    } else if (previous & (mouseButtons == 0)) {
        releasePosition.x = x; releasePosition.y = y; releaseButtons = mouseButtons;
    }
}
extern "C" void SetMousePosition(int x, int y) { mousePosition.x = x; mousePosition.y = y; }


