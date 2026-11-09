typedef void (*AssertCallback)();
static const char assertFormat[]="%s\r\n\r\nProgram: %s\r\nVersion: %s %s\r\nFile: %s (#%d)\r\n\r\n%s"; static const char assertProgram[]="CyberStorm"; static const char assertVersion[]="#########################################################################"; static const char assertDate[]="Dec 13 1996"; static const char assertFooter[]="------------------------------------------------------------------------------------------------------\r\nThis program prevents CyberStorm from proceeding.\r\n\r\nUseful Tips:\r\n    The above text may seem cryptic, but often contains useful sentences.  Scan the above text for any useful information, such as asking you to 'clear some disk space'.\r\n    After pressing the 'OK' box, restart your computer and run CyberStorm again. If you have a previous saved game available, try loading that game and continue from that point.\r\n"; static const char assertTitle[]="Problem!";
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
            sprintf(buffer, assertFormat,
                (const char *)wasActive, assertProgram, assertVersion, assertDate, __FILE__, 104, assertFooter);
            MessageBeep(wasActive);
            MessageBoxA(g_mainWindow, buffer, assertTitle, 0x2000);
            g_assertMessageActive = wasActive;
        }
    }
    register int oldCount = g_assertCallbackCount;
    register AssertCallback *slot = g_assertCallbacks + oldCount;
    g_assertCallbackCount = oldCount + 1;
    *slot = callback;
}

void AssertFail(const char *file, int line, const char *message)
{
    if (g_assertMessageActive == 0) {
        char buffer[1024];
        g_assertMessageActive = 1;
        sprintf(buffer, assertFormat,
            message, assertProgram, assertVersion, assertDate, file, line, assertFooter);
        MessageBeep(0);
        MessageBoxA(g_mainWindow, buffer, assertTitle, 0x2000);
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





