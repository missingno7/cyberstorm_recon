typedef void (*AssertCallback)();
extern int g_assertCallbackCount;
extern int g_assertMessageActive;
extern AssertCallback g_assertCallbacks[50];
extern void *g_mainWindow;
extern "C" int sprintf(char *buffer, const char *format, ...);
extern "C" __declspec(dllimport) int __stdcall MessageBeep(unsigned int type);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void *window, const char *text, const char *caption, unsigned int type);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int code);

void DrainAssertCallbacks()
{
    while (g_assertCallbackCount != 0) {
        --g_assertCallbackCount;
        g_assertCallbacks[g_assertCallbackCount]();
    }
}

void RegisterAssertCallback(AssertCallback callback)
{
    int wasActive;
    if (g_assertCallbackCount >= 50) {
        wasActive = g_assertMessageActive;
        if (wasActive == 0) {
            char buffer[1024];
            g_assertMessageActive = 1;
            sprintf(buffer, "%s\r\n\r\nProgram: %s\r\nVersion: %s %s\r\nFile: %s (#%d)\r\n\r\n%s",
                (const char *)wasActive, "CyberStorm", "1.10A", "Dec 13 1996", __FILE__, 104, "Assertion failed");
            MessageBeep(wasActive);
            MessageBoxA(g_mainWindow, buffer, "Problem!", 0x2000);
            g_assertMessageActive = wasActive;
        }
    }
    g_assertCallbacks[g_assertCallbackCount++] = callback;
}

void AssertFail(const char *file, int line, const char *message)
{
    if (g_assertMessageActive == 0) {
        char buffer[1024];
        g_assertMessageActive = 1;
        sprintf(buffer, "%s\r\n\r\nProgram: %s\r\nVersion: %s %s\r\nFile: %s (#%d)\r\n\r\n%s",
            message, "CyberStorm", "1.10A", "Dec 13 1996", file, line, "Assertion failed");
        MessageBeep(0);
        MessageBoxA(g_mainWindow, buffer, "Problem!", 0x2000);
        g_assertMessageActive = 0;
    }
}

void AssertExit(unsigned int code)
{
    while (g_assertCallbackCount != 0) {
        --g_assertCallbackCount;
        g_assertCallbacks[g_assertCallbackCount]();
    }
    ExitProcess(code);
}




