/* multi\linkdlg.c -- DirectPlay provider/session selection for the multiplayer link (cs11 0046d9ac..0046e0c0).
 * Names are working labels; the DirectPlay 1 interface layout follows the DirectX 2 SDK dplay.h. */
#include <windows.h>
#include <string.h>

struct Timer;
void TimerSet(Timer *t, unsigned long duration);
int TimerExpired(Timer *t);
void AssertFail(const char *file, int line, const char *message);
/* DirectPlay 1 (dplay.h, DirectX 2) */
struct DPSESSIONDESC {
    DWORD dwSize;
    GUID guidSession;
    DWORD dwSession;
    DWORD dwMaxPlayers;
    DWORD dwCurrentPlayers;
    DWORD dwFlags;
    char szSessionName[32];
    char szUserField[16];
    DWORD dwReserved1;
    char szPassword[16];
    DWORD dwReserved2;
    DWORD dwUser1, dwUser2, dwUser3, dwUser4;
};

typedef BOOL (__stdcall *EnumSessionsCallback)(DPSESSIONDESC *desc, void *context, DWORD *timeout, DWORD flags);
typedef BOOL (__stdcall *EnumProvidersCallback)(GUID *guid, char *name, DWORD major, DWORD minor, void *context);

class IDirectPlay {
public:
    virtual HRESULT __stdcall QueryInterface(const GUID &iid, void **object) = 0;
    virtual ULONG __stdcall AddRef() = 0;
    virtual ULONG __stdcall Release() = 0;
    virtual HRESULT __stdcall AddPlayerToGroup(DWORD group, DWORD player) = 0;
    virtual HRESULT __stdcall Close() = 0;
    virtual HRESULT __stdcall CreatePlayer(DWORD *player, char *friendlyName, char *formalName, HANDLE *event) = 0;
    virtual HRESULT __stdcall CreateGroup(DWORD *group, char *friendlyName, char *formalName) = 0;
    virtual HRESULT __stdcall DeletePlayerFromGroup(DWORD group, DWORD player) = 0;
    virtual HRESULT __stdcall DestroyPlayer(DWORD player) = 0;
    virtual HRESULT __stdcall DestroyGroup(DWORD group) = 0;
    virtual HRESULT __stdcall EnableNewPlayers(BOOL enable) = 0;
    virtual HRESULT __stdcall EnumGroupPlayers(DWORD group, void *callback, void *context, DWORD flags) = 0;
    virtual HRESULT __stdcall EnumGroups(DWORD session, void *callback, void *context, DWORD flags) = 0;
    virtual HRESULT __stdcall EnumPlayers(DWORD session, void *callback, void *context, DWORD flags) = 0;
    virtual HRESULT __stdcall EnumSessions(DPSESSIONDESC *desc, DWORD timeout, EnumSessionsCallback callback, void *context, DWORD flags) = 0;
    virtual HRESULT __stdcall GetCaps(void *caps) = 0;
    virtual HRESULT __stdcall GetMessageCount(DWORD player, DWORD *count) = 0;
    virtual HRESULT __stdcall GetPlayerCaps(DWORD player, void *caps) = 0;
    virtual HRESULT __stdcall GetPlayerName(DWORD player, char *friendlyName, DWORD *friendlyLength, char *formalName, DWORD *formalLength) = 0;
    virtual HRESULT __stdcall Initialize(GUID *guid) = 0;
    virtual HRESULT __stdcall Open(DPSESSIONDESC *desc) = 0;
};

#define LINK_ERROR_COMMAND 0x7665

/* multi\link.c */
extern int networkEnabled;
extern int gLinkIsSigs;
inline int LinkIsSigs() { return gLinkIsSigs; }
extern int gLinkSigsMode;
extern int gLinkSigsAvailable;
extern int isHostPeer;
extern DWORD localPeer;
extern Timer gLinkTimer;
extern HANDLE gLinkThread;
extern DWORD gLinkThreadId;
extern IDirectPlay *gDirectPlay;
extern HANDLE gLinkEvent;
extern HRESULT (__stdcall *gDirectPlayCreate)(GUID *provider, IDirectPlay **directPlay, IUnknown *outer);
extern HRESULT (__stdcall *gDirectPlayEnumerate)(EnumProvidersCallback callback, void *context);
extern void *gLinkDialogParam;
void NetworkStop(void);
void LinkSetupPlayer(void);

extern const GUID startGuid;
extern const GUID gSigsProviderGuid;
extern unsigned (__stdcall *const gLinkThreadProc)(void *);
extern void *g_mainWindow;
extern int g513888;
extern char gDebugText[];
extern DWORD gSessionTable[];
extern char g4b82fc[];
extern char networkQueue[];

void DebugTextClear(void);
void DebugTextAppend(char *text);
unsigned long LinkSelectDialog(char *text, char *(*select)(int index), void *param);
void SigsStart(int *state, int mode);
void SigsJoin(char *state);
int SigsStop(void);
void ResetQueue(char *queue);
struct MenuNetwork { int Count(); };
extern MenuNetwork gMenuNetwork;
HANDLE LinkBeginThread(void *security, unsigned stack, unsigned (__stdcall *start)(void *), void *arg, unsigned flags, unsigned priority, DWORD *id);

/* One retry timer per peer (array of 33 in multi\linkapp.c). */
struct LinkPeerTimer {
    DWORD timer[3];
    int retries;
    DWORD reserved[3];
    LinkPeerTimer();
    ~LinkPeerTimer();
};

struct ProviderEntry { char *name; GUID *guid; char buffer[0x50]; };

struct ProviderList {
    ProviderEntry entries[10];
    int count;

    void Clear();
    void Add(char *name, GUID *guid);
    char *Dump();
};

LinkPeerTimer::LinkPeerTimer()
{
    TimerSet((Timer *)timer, 0);
}

void ProviderList::Clear()
{
    count = 0;
}

void ProviderList::Add(char *name, GUID *guid)
{
    if (count < 10) {
        char *buffer = entries[count].buffer;
        memset(buffer, 0, sizeof(entries[count].buffer));
        strncpy(buffer, name, sizeof(entries[count].buffer) - 2);
        entries[count].name = buffer;
        entries[count].guid = guid;
        count++;
    }
}

char *ProviderList::Dump()
{
    int i;
    DebugTextClear();
    for (i = 0; i < count; i++) {
        DebugTextAppend(entries[i].name);
        DebugTextAppend("\n");
    }
    return gDebugText;
}

char gSessionName[] = "CyberStorm: ............         ";
char gPlayerFormalName[] = "Immodius Rex";
char gPlayerFriendlyName[] = "Rexie";
char gDefaultPlanetName[] = "Sol III";
static char linkdlgFile[] = __FILE__;
DWORD *gSessionIds = gSessionTable;
DWORD gEnumTimeout = 1000;

ProviderList gProviders;
int gSessionCount;
GUID *gLinkProvider;

BOOL __stdcall EnumProviders(GUID *guid, char *name, DWORD major, DWORD minor, void *context)
{
    gProviders.Add(name, guid);
    return TRUE;
}

BOOL __stdcall EnumSessions(DPSESSIONDESC *desc, void *context, DWORD *timeout, DWORD flags)
{
    if (flags & 1)
        return FALSE;
    if (gSessionCount >= 255)
        return FALSE;
    if (desc->szSessionName[0]) {
        DebugTextAppend(desc->szSessionName);
        DebugTextAppend("\n");
        gSessionIds[gSessionCount++] = desc->dwSession;
    }
    return TRUE;
}

char *SelectProvider(int index)
{
    DPSESSIONDESC desc;
    if (gDirectPlay == 0) {
        if (gProviders.entries[index].guid == 0) {
            gLinkSigsMode = 1;
            SigsStart(&g513888, 0);
            return "__SIGS__";
        }
        GUID *guid = gProviders.entries[index].guid;
        gLinkIsSigs = memcmp(guid, &gSigsProviderGuid, sizeof(GUID)) == 0;
        TimerSet(&gLinkTimer, 180000);
        HRESULT result = gDirectPlayCreate(guid, &gDirectPlay, 0);
        if (result != 0) {
            PostMessageA((HWND)g_mainWindow, WM_COMMAND, LINK_ERROR_COMMAND, result);
            return 0;
        }
        if (gDirectPlay == 0)
            return 0;
        gLinkProvider = guid;
    }
    gSessionCount = 0;
    DebugTextClear();
    memset(&desc, 0, sizeof(desc));
    desc.dwSize = sizeof(desc);
    desc.guidSession = startGuid;
    DWORD flags;
    if (LinkIsSigs())
        flags = 4;
    else
        flags = 0;
    gDirectPlay->EnumSessions(&desc, gEnumTimeout, EnumSessions, 0, flags);
    if (gEnumTimeout < 4000)
        gEnumTimeout *= 2;
    return gDebugText;
}

int RecreateSession(void);

void LinkSigsPoll(void)
{
    if (LinkIsSigs()) {
        if (gMenuNetwork.Count() > 1) {
            TimerSet(&gLinkTimer, 0);
        } else if (TimerExpired(&gLinkTimer)) {
            RecreateSession();
            TimerSet(&gLinkTimer, 90000);
        }
    }
}

int RecreateSession(void)
{
    DPSESSIONDESC desc;
    const GUID *provider = &gSigsProviderGuid;
    if (gDirectPlay) {
        NetworkStop();
        ResetQueue(networkQueue);
    }
    if (gDirectPlay) AssertFail(linkdlgFile, __LINE__, 0);
    HRESULT result = gDirectPlayCreate((GUID *)provider, &gDirectPlay, 0);
    if (result == 0) {
        memset(&desc, 0, sizeof(desc));
        desc.dwSize = sizeof(desc);
        desc.dwMaxPlayers = 32;
        desc.dwFlags = 2;
        desc.guidSession = startGuid;
        strcpy(desc.szSessionName, gSessionName);
        if (gDirectPlay->Open(&desc) == 0
            && gDirectPlay->CreatePlayer(&localPeer, gPlayerFriendlyName, gPlayerFormalName, &gLinkEvent) == 0) {
            networkEnabled = 1;
            LinkSetupPlayer();
            gLinkThread = LinkBeginThread(0, 0, gLinkThreadProc, 0, 0, 0, &gLinkThreadId);
            if (gLinkThread)
                return 1;
        }
    }
    PostMessageA((HWND)g_mainWindow, WM_COMMAND, LINK_ERROR_COMMAND, result);
    gDirectPlay->Close();
    gDirectPlay->Release();
    gDirectPlay = 0;
    return 0;
}

int LinkDialog(char *name)
{
    DPSESSIONDESC join;
    DPSESSIONDESC create;
    HRESULT result;
    unsigned long choice;
    unsigned selection;
    int command;
    int session;
    gProviders.Clear();
    gDirectPlayEnumerate(EnumProviders, 0);
    if (gProviders.count == 0 && gLinkSigsAvailable == 0)
        return 0;
    if (gLinkSigsAvailable) {
        int n = gProviders.count++;
        gProviders.entries[n].name = gProviders.entries[n].buffer;
        gProviders.entries[n].guid = 0;
        strcpy(gProviders.entries[n].buffer, "Sierra Internet Gaming System (SIGS)");
    }
    gProviders.Dump();
    choice = LinkSelectDialog(gDebugText, SelectProvider, gLinkDialogParam);
    if ((gDirectPlay == 0) & (gLinkSigsMode == 0))
        choice = 0;
    command = HIWORD(choice);
    selection = LOWORD(choice);
    switch (command) {
    case 0:
        if (gLinkSigsMode) {
            gLinkSigsMode = SigsStop();
            break;
        }
        if (gDirectPlay) {
            gDirectPlay->Close();
            gDirectPlay->Release();
            gDirectPlay = 0;
        }
        return 0;
    case 1:
        isHostPeer = 1;
        memset(&create, 0, sizeof(create));
        create.dwSize = sizeof(create);
        create.dwMaxPlayers = 32;
        create.dwFlags = 2;
        create.guidSession = startGuid;
        strncpy(gSessionName + 12, name, strlen(gSessionName) - 13);
        strcpy(create.szSessionName, gSessionName);
        result = gDirectPlay->Open(&create);
        if (result == 0)
            break;
        PostMessageA((HWND)g_mainWindow, WM_COMMAND, LINK_ERROR_COMMAND, result);
        gDirectPlay->Close();
        gDirectPlay->Release();
        gDirectPlay = 0;
        return 0;
    case 2:
        if (LinkIsSigs())
            SigsJoin(g4b82fc);
        isHostPeer = 2;
        session = (selection & 0xff00) >> 8;
        memset(&join, 0, sizeof(join));
        join.dwSize = sizeof(join);
        join.guidSession = startGuid;
        join.dwFlags = 1;
        join.dwSession = gSessionIds[session];
        result = gDirectPlay->Open(&join);
        if (result == 0)
            break;
        PostMessageA((HWND)g_mainWindow, WM_COMMAND, LINK_ERROR_COMMAND, result);
        gDirectPlay->Close();
        gDirectPlay->Release();
        gDirectPlay = 0;
        return 0;
    }
    return 1;
}

