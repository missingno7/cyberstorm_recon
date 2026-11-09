#include <windows.h>
typedef void (*AssertCallback)();
     
#pragma data_seg("assertFormat", "DATA")
static char assertFormat[]="%s\r\n\r\nProgram: %s\r\nVersion: %s %s\r\nFile: %s (#%d)\r\n\r\n%s";
#pragma data_seg()
#pragma data_seg("assertProgram", "DATA")
static char assertProgram[]="CyberStorm";
#pragma data_seg()
#pragma data_seg("assertVersion", "DATA")
static char assertVersion[]="##########################################################################";
#pragma data_seg()
#pragma data_seg("assertDate", "DATA")
static char assertDate[]="Dec 13 1996";
#pragma data_seg()
#pragma data_seg("assertFooter", "DATA")
static char assertFooter[]="------------------------------------------------------------------------------------------------------\r\nThis problem prevents CyberStorm from proceeding.\r\n\r\nUseful Tips:\r\n     The above text may seem cryptic, but often contains a useful sentence.  Scan the above text for any useful information, such as asking you to 'clear some disk space'.\r\n     After pressing the 'OK' box, restart your computer and run Cyberstorm again.  If you have an previously saved game available, try loading that game and continuing from that point.\r\n";
#pragma data_seg()
#pragma data_seg("assertTitle", "DATA")
static char assertTitle[]="Problem!";
#pragma data_seg()
extern int g_assertCallbackCount;
extern int g_assertMessageActive;
extern AssertCallback g_assertCallbacks[50];
extern void *g_mainWindow;
int sprintf(char *buffer, const char *format, ...);

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
            MessageBoxA((HWND)g_mainWindow, buffer, assertTitle, 0x2000);
            g_assertMessageActive = wasActive;
        }
    }
    {register int oldCount = g_assertCallbackCount;
    register AssertCallback *slot = g_assertCallbacks + oldCount;
    g_assertCallbackCount = oldCount + 1;
    *slot = callback;}
}

void AssertFail(const char *file, int line, const char *message)
{
    if (g_assertMessageActive == 0) {
        char buffer[1024];
        g_assertMessageActive = 1;
        sprintf(buffer, assertFormat,
            message, assertProgram, assertVersion, assertDate, file, line, assertFooter);
        MessageBeep(0);
        MessageBoxA((HWND)g_mainWindow, buffer, assertTitle, 0x2000);
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





