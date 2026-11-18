#include <windows.h>
#include <process.h>

typedef unsigned (__stdcall *LinkThreadStart)(void *);

HANDLE LinkBeginThread(void *security, unsigned stackSize, LinkThreadStart start,
                       void *argument, unsigned flags, unsigned priority, DWORD *threadId)
{
    HANDLE thread = (HANDLE)_beginthreadex(security, stackSize, start, argument,
                                           flags, (unsigned *)threadId);
    SetThreadPriority(thread, priority);
    return thread;
}

BOOL LinkSetThreadPriority(HANDLE thread, int priority)
{
    return SetThreadPriority(thread, priority);
}

DWORD LinkSuspendThread(HANDLE thread)
{
    return SuspendThread(thread);
}

DWORD LinkResumeThread(HANDLE thread)
{
    return ResumeThread(thread);
}
