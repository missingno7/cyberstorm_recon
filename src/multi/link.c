#include <windows.h>
#include <string.h>
// Named objects replace the unprovable aggregate map from the first pass.
struct Timer;
extern int sigsActive, networkEnabled, gLinkSigsMode, gLinkSigsAvailable;
extern int isHostPeer, hostPeer, localPeer;
extern HMODULE sigsModule;
extern unsigned int linkLastError;
extern HMODULE directplayModule;
class IDirectPlay;
typedef BOOL (__stdcall *EnumProvidersCallback)(GUID*,char*,DWORD,DWORD,void*);
typedef HRESULT (__stdcall *DirectPlayCreateProc)(GUID*,IDirectPlay**,IUnknown*);
typedef HRESULT (__stdcall *DirectPlayEnumerateProc)(EnumProvidersCallback,void*);
extern DirectPlayCreateProc gDirectPlayCreate;
extern DirectPlayEnumerateProc gDirectPlayEnumerate;
extern HANDLE gLinkEvent, gLinkThread;
extern DWORD gLinkThreadId;
extern Timer gLinkTimer;
extern unsigned int linkOptions[7];
extern char gPlayerFormalName[], gPlayerFriendlyName[];
struct Timer;
void TimerSet(Timer *, unsigned long);
extern "C" void *memset(void *, int, unsigned int);
void alternate_link_disconnect(void);
struct StrPool { int Lock(void); };
extern char resourcePool[];
extern const char snw_valid_dll[];
extern const char directplay_dll[];
extern const char directplay_enumerate[];
extern const char directplay_create[];
extern const char directplay_message[];
extern const char last_dialed_label[];
extern const char directplay_error_text[];
extern const char system_message_error_text[];
extern HWND g_mainWindow;
void AssertFail(const char *, int, const char *);
int link_is_ready(char *);
void link_force_shutdown(void);
void close_link_connection(void);
extern void *link_thread_proc_anchor;
int LinkDialog(char *);
void link_load_settings(void *);
void f_00477295(const char *);
void *AllocateMissionImage(void *, int, int);
typedef unsigned int (__stdcall *LinkThreadStart)(void *);
HANDLE LinkBeginThread(void *, unsigned int, LinkThreadStart, void *,
                       unsigned int, unsigned int, DWORD *);
int SigsSend(int, int, int, void *, unsigned int);
int link_alt_receive(void *, void *, int, void *, int);
int link_alt_disconnect(void);
int link_alt_set_options(void *, void *);
class IDirectPlay {
public:
 virtual int __stdcall QueryInterface(void*,void**) = 0;
 virtual int __stdcall AddRef() = 0;
 virtual int __stdcall Release() = 0;
 virtual int __stdcall AddPlayerToGroup(int,int) = 0;
 virtual int __stdcall Close() = 0;
 virtual int __stdcall CreatePlayer(unsigned int*,const char*,const char*,HANDLE*) = 0;
 virtual int __stdcall CreateGroup() = 0;
 virtual int __stdcall DeletePlayerFromGroup() = 0;
 virtual int __stdcall DestroyPlayer(int) = 0;
 virtual int __stdcall DestroyGroup() = 0;
 virtual int __stdcall EnableNewPlayers(int) = 0;
 virtual int __stdcall EnumGroupPlayers() = 0;
 virtual int __stdcall EnumGroups() = 0;
 virtual int __stdcall EnumPlayers() = 0;
 virtual int __stdcall EnumSessions() = 0;
 virtual int __stdcall GetCaps(void*) = 0;
 virtual int __stdcall GetMessageCount() = 0;
 virtual int __stdcall GetPlayerCaps() = 0;
 virtual int __stdcall GetPlayerName() = 0;
 virtual int __stdcall Initialize() = 0;
 virtual int __stdcall Open() = 0;
 virtual int __stdcall Receive(void*,void*,void*,void*,void*) = 0;
 virtual void __stdcall SaveSession(const char*) = 0;
 virtual int __stdcall Send(int,int,int,void*,unsigned int) = 0;
};
extern IDirectPlay *gDirectPlay;
 static char linkSourceFile[] = __FILE__;
void reset_link_timer(void)
{
    TimerSet(&gLinkTimer, 0);
}
int initialize_directplay(void)
{
    void *module;
    if (sigsActive != 0)
        return 1;
    module = LoadLibraryA(snw_valid_dll);
    if (module != 0) {
        gLinkSigsAvailable = 1;
        sigsActive = 1;
        FreeLibrary(module);
    }
    directplayModule = LoadLibraryA(directplay_dll);
    if (directplayModule != 0) {
        gDirectPlayEnumerate = (DirectPlayEnumerateProc)GetProcAddress(
            (void *)directplayModule, directplay_enumerate);
        gDirectPlayCreate = (DirectPlayCreateProc)GetProcAddress(
            (void *)directplayModule, directplay_create);
        if (gDirectPlayEnumerate == 0 || gDirectPlayCreate == 0)
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
// Original source-line padding.
            AssertFail(linkSourceFile, __LINE__, 0);
        gLinkEvent = CreateEventA(0, 0, 0, directplay_message);
        sigsActive = 1;
    }
    ((StrPool *)resourcePool)->Lock();
    return sigsActive;
}

void shutdown_directplay(void)
{
    sigsActive = 0;
    linkLastError = 0;
    if (gLinkSigsMode != 0 || sigsModule != 0) {
        link_force_shutdown();
        return;
    }
    if (networkEnabled != 0)
        close_link_connection();
    if (directplayModule != 0) {
        FreeLibrary((void *)directplayModule);
        directplayModule = 0;
    }
}

int request_link_connection(char *address);
int open_link_connection(char *address)
{
    if (networkEnabled || !initialize_directplay()) return 0;
    LinkDialog(address);
    if (!gDirectPlay && !gLinkSigsMode) return 0;
    if (gLinkSigsMode) {
        return request_link_connection(address);
    } else {
        hostPeer = 1;
        if (gDirectPlay->CreatePlayer((unsigned int *)&localPeer,gPlayerFriendlyName,gPlayerFormalName,&gLinkEvent)) {
            gDirectPlay->Close();
            gDirectPlay->Release();
            gDirectPlay = 0;
            return 0;
        }
        networkEnabled = 1;
        memset(linkOptions,0,0x1c);
        linkOptions[0] = 0x1c;
        gDirectPlay->GetCaps(linkOptions);
        if (isHostPeer == 2 && linkOptions[5] < 0x90) {
            close_link_connection();
            f_00477295((const char *)AllocateMissionImage((void *)resourcePool,6,0));
            return 0;
        }
        gDirectPlay->SaveSession(last_dialed_label);
        gLinkThread = LinkBeginThread(0,0,(LinkThreadStart)link_thread_proc_anchor,0,0,0,&gLinkThreadId);
        if (!gLinkThread) { close_link_connection(); return 0; }
        return 1;
    }
}

void close_link_connection(void)
{
    if (gLinkSigsMode || sigsModule) {
        link_force_shutdown();
        if (gLinkThread) {
            Sleep(100);
            TerminateThread(gLinkThread, 0);
            CloseHandle(gLinkThread);
            gLinkThread = 0;
        }
    } else if (networkEnabled) {
        networkEnabled = 0;
        if (localPeer)
            gDirectPlay->DestroyPlayer(localPeer);
        localPeer = 0;
        if (gDirectPlay) {
            gDirectPlay->Close();
            gDirectPlay->Release();
            gDirectPlay = 0;
        }
        if (gLinkEvent) {
            CloseHandle(gLinkEvent);
            gLinkEvent = 0;
        }
        if (gLinkThread) {
            Sleep(100);
            TerminateThread(gLinkThread, 0);
            CloseHandle(gLinkThread);
            gLinkThread = 0;
        }
    }
}

int send_link_packet(int first, void *second, unsigned int third, int fourth)
{
    int result;
    if (gLinkSigsMode != 0) {
        result = SigsSend(localPeer, first, fourth, (void *)second,
                               third);
        if (result != 0)
            return 0x80004005;
        return 0;
    } else {
        if (gDirectPlay == 0)
            return 0;
        result = gDirectPlay->Send(
                                         localPeer,
                                         first, fourth, second, third);
        if (result > 0) {
            linkLastError = result;
            return 1;
        }
        if (result != 0 && result == (int)0x88770096) {
            if (first == 0)
                PostMessageA(g_mainWindow, 0x111, 0x7663, first);
            else
                PostMessageA(g_mainWindow, 0x111, 0x7664, first);
        }
        return result == 0;
    }
}

int receive_link_packet(int *event_id, int *player, unsigned int *packet,
                        unsigned int packet_size, int flags)
{
    int result;
    if (gLinkSigsMode != 0) {
        result = link_alt_receive(event_id, player, flags, packet, packet_size);
    } else {
        if (gDirectPlay == 0)
            return 0;
        result = gDirectPlay->Receive( event_id, player,
            (void *)flags, packet, &packet_size);
    }
    return result == 0;
}

const char *dp_error_not_recognized(void)
{
    return directplay_error_text;
}

const char *system_message_not_recognized(void)
{
    return system_message_error_text;
}

void disconnect_link_interface(void)
{
    if (gLinkSigsMode != 0) {
        alternate_link_disconnect();
    } else {
        if (gDirectPlay != 0)
            gDirectPlay->EnableNewPlayers(0);
    }
}

void reset_link_options(void)
{
    unsigned int *options = (unsigned int *)(linkOptions);
    memset(options, 0, 0x1c);
    *options = 0x1c;
    IDirectPlay *connection = (IDirectPlay *)gDirectPlay;
    connection->GetCaps( options);
}

int request_link_connection(char *address)
{
    if (link_is_ready(address) == 0)
        return 0;
    if (isHostPeer == 2)
        Sleep(localPeer * 300 + 3000);
    link_load_settings(linkOptions);
    networkEnabled = 1;
    gLinkThread = LinkBeginThread(
        0, 0, (LinkThreadStart)link_thread_proc_anchor, 0, 0, 0,
        &gLinkThreadId);
    if (gLinkThread == 0) {
        link_force_shutdown();
        return 0;
    }
    return 1;
}





