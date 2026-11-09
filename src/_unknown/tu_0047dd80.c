#include <windows.h>
extern "C" void RegisterAssertCallback(void (*)());
volatile int keyboardActive;
unsigned char keyDown[256];
unsigned int keyQueue[8];
volatile unsigned int keyRead, keyWrite;
extern "C" void f_47dde0();
extern "C" int FrameInputInit()
{
    for (int i = 0; i < 256; ++i) keyDown[i] = 0;
    keyWrite = 0; keyRead = keyWrite;
    keyboardActive = 1;
    RegisterAssertCallback(f_47dde0);
    return keyboardActive;
}
extern "C" void f_47dde0() { keyboardActive = 0; }
extern "C" void ResetFrameClock() { keyWrite = 0; keyRead = keyWrite; }
extern "C" int KeyQueuePending() { return keyRead != keyWrite; }
extern "C" unsigned int ReadKeyEvent()
{
    if (!keyboardActive || keyRead == keyWrite) return 0;
    unsigned int result = keyQueue[keyRead];
    keyRead = (keyRead + 1) & 7;
    return result;
}
extern "C" void QueueKeyEvent(unsigned int key, unsigned int flags, unsigned int repeat)
{
    if (keyboardActive) {
        keyQueue[keyWrite] = (unsigned short)(((flags & 255) << 8) | (key & 255)) | ((repeat & 65535) << 16);
        keyWrite = (keyWrite + 1) & 7;
        if (keyWrite == keyRead) keyRead = (keyRead + 1) & 7;
    }
}
extern "C" void MarkKeyDown(unsigned int key) { keyDown[key & 255] = 1; }
extern "C" void MarkKeyUp(unsigned int key) { keyDown[key & 255] = 0; }
extern "C" unsigned int GuiReadKey(unsigned int key) { return keyDown[key & 255]; }
extern "C" int ControlKeyDown() { return (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0; }
extern "C" int AltKeyDown() { return (GetAsyncKeyState(VK_MENU) & 0x8000) != 0; }
extern "C" int ShiftKeyDown() { return (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0; }



