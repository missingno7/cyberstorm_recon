// multi\mstart.c: working names and partial layouts inferred from code, cs11.
#include <windows.h>
#include <string.h>
#include <stdio.h>
struct PlayerSlot { int state,index; char unknown08[8]; char name[52],shortName[20],description[80]; int peer,kind,value,color; char unknownB8[4]; };
struct MenuMessage { char data[476]; MenuMessage() {} MenuMessage(unsigned,const char*); }; struct PlayerInfo { unsigned char kind; char unknown01[39]; int value; char unknown2C[4]; }; struct GameInfo { char data[88]; };
extern void TimerSet(void*,unsigned long); struct Timer { unsigned long start,end,duration; Timer() { TimerSet(this,0); } };
struct Chat { char data[0x3d7a]; Chat(); int Reset(); void Activate(); void Log(int,const char*); void Flush(); };
struct Options { char data[66]; Options(); void Reset(); }; struct CriticalSection { CRITICAL_SECTION lock; CriticalSection(); ~CriticalSection(); };
extern int modeSwitch; extern int multiplayerActive,playerCount,multiState,localPeer,hostPeer,networkEnabled,serverMode,canRestart,restartData,connectionType,isHostPeer,hostDropFlag,retryTimeout,savedRandom,lastPlayer,leaderPlayer,localPlayer; extern HANDLE networkThread; extern int MissionRandomTable[];
extern char modeCheckName[],modeLogName[],networkLog[],networkQueue[],networkSystem[],profileManager[],uiSystem[],resourcePool[],textPool[]; extern HWND g_mainWindow; extern void (*ShowStatus)(HWND,const char*,int,int); extern char *searchPaths[]; extern unsigned char playerColors[]; extern char startGuid[16],sessionGuid[16]; extern Options currentOptions; extern int globalReady[10];
extern void ResetPlayer(PlayerSlot*); extern void ValidateSlot(int); extern void MenuMessageInfo(MenuMessage*,int,int,void*); extern void MenuMessageSlot(MenuMessage*,int,int,PlayerSlot*); extern void MenuSend(const MenuMessage*,int);
extern void MenuMessageBasic(MenuMessage*,int); extern void MenuMessageData(MenuMessage*,int,int,int=0,int=0); extern void MenuMessageGame(MenuMessage*,int,GameInfo*); extern void MenuMessageStart(MenuMessage*,int,void*); extern void QueuePump(void*);
extern void AssertFail(const char*,int,const char*); extern void *AllocateMissionImage(void*,int,int=0); extern int FileExists(const char*); extern void GetPlayerInfo(void*,PlayerInfo*); extern int HasMission(void*); extern int LoadMission(const char*,int); extern void GetGameInfo(void*,Options*,GameInfo*); extern void ApplyGameInfo(void*,GameInfo*);
extern int StopMultiplayer(int,int=0); extern void NetworkReset(); extern int NetworkCreate(const char*); extern int NetworkJoin(const char*); extern void NetworkBegin(); extern void NetworkPoll(); extern void NetworkStop(); extern void NetworkShutdown(); extern void NetworkRefresh(); extern int PeerAlive(int); extern void NetworkCallback(); extern void GuiPumpMessages(); extern void CloseStartScreen(); extern void ShowMessage(const char*); extern void TimedMessage(const char*,int); extern int RestartNetwork();
extern PlayerSlot modePlayers[]; extern void OpenModeLog(void*); extern void ClearList(void*); extern void SetList(void*); extern void ResetQueue(void*); extern void ResetTransport(void*); extern PlayerSlot *GetModePlayer(void*,int); extern void RemoveModePlayer(void*,int); extern void ClearModePlayers(void*); extern void CommitModePlayers(void*); extern void AddModePlayer(void*,int,PlayerSlot*); extern void OpenMode(void*); extern void UpdateMode(void*); extern void FinishMode(void*); extern void UiEvent(void*,int);
#pragma pack(push,2)
struct SessionTail { int unknown4902,unknown4906; };
#pragma pack(pop)
struct MenuNetwork { int localSlot,host; int mapping[250]; PlayerSlot slots[10]; int ready[10]; int unknownB70,paused,hostLost; Timer timer; Chat chat; SessionTail tail;
 MenuNetwork(); ~MenuNetwork() {} void Reset(); int Start(const char*,int); void DropPeer(int); int Count(); int AllocateSlot(int); int BroadcastInfo(void*); int BroadcastLocal(); int BroadcastSlot(int,PlayerSlot*); int Begin(); void Finish(GameInfo*); int Active(); static DWORD __stdcall ThreadProc(void*); int ActorState(int); void SetMapping(int,int); void CompactPlayers(); void CopySlot(int,PlayerSlot*); void BroadcastAll(); void ResetReady(); void Synchronize(int,int,int=1); void Launch(); void MarkReady(int); int AllReady(); void SetAllReady(); void SetTimeout(); void SendStart(MenuNetwork*,int); int PeerForSlot(int); int SlotForPeer(int); int IsLeader(int); int LeaderPeer(); int LeaderSlot(); int CheckLeader(); int Connected(); int Poll(); int IsRemote(int); void Pause(); void Resume(); int IsRunning(); void PostStart();
 int MySlot() { return localSlot; } PlayerSlot *GetSlot(int i) { return &slots[i]; } int IsHost() { return host; } inline int CountCore(); inline int PeerCore(int); inline int FindPeer(int); inline int FindLeader(); inline int ReadyCore(); inline int ConnectedCore(); inline int AllocateCore(int); inline int BroadcastCore(int,PlayerSlot*); inline void ResetReadyCore(); inline void BroadcastAllCore(); inline void ResetCore();
};
inline MenuNetwork::MenuNetwork() { tail.unknown4906=0; unknownB70=0; }
CriticalSection connectionLock; Options startOptions,gameOptions; MenuNetwork gMenuNetwork;
#pragma data_seg("CONST")
extern const char mstartFile[]="multi\\mstart.c",startFormat[]="%sqstart\\MP%02d_%c.CBM",blankDescription[]=" ",startFailure[]="There was a problem in the multiplayer startup process.  Perhaps try again (more slowly) to make sure that all players are properly connected before launching the game.",hostGreeting[]="Caught in my web, I see!",joiningMessage[]="I'm in!",bailingMessage[]="I'm bailing!";
#pragma data_seg()
#define ASSERT(e) do { if(!(e)) AssertFail(mstartFile,__LINE__,0); } while(0)

inline PlayerSlot *ModeSlot(int index) { return &modePlayers[index-1]; } inline int ReadRandom() { return MissionRandomTable[0]; } inline int ReadLocalPeer() { return localPeer; } inline int CanContinue() { return canRestart==0; }
inline const char *DisconnectMessage() { if(connectionType) return (const char*)AllocateMissionImage(resourcePool,13,0); else return (const char*)AllocateMissionImage(resourcePool,12,0); }
inline void TimedCore(const char *message,int duration) { TimedMessage(message,duration); }
inline void StatusCore(const char *message,int duration) { ShowStatus(g_mainWindow,message,-1,duration); }
inline int MenuNetwork::CountCore() { int count=0; for(int i=1;i<9;i++) { if(slots[i].state==2 || slots[i].state==1) count++; } return count; }
inline int MenuNetwork::PeerCore(int slot) { ValidateSlot(slot); int state=slots[slot].state; if(state==2 || state==1) return slots[slot].peer; return -1; }
inline int MenuNetwork::FindPeer(int peer) { for(int i=0;i<=8;i++) if(peer==slots[i].peer && slots[i].state!=0) return slots[i].index; return -1; }
inline int MenuNetwork::FindLeader() { int i=1; while(slots[i].state==0) i++; return i; }
inline int MenuNetwork::ReadyCore() { int result=1; for(int i=1;i<=playerCount;i++) if(gMenuNetwork.ready[i]==0) result=0; return result; }
inline int MenuNetwork::AllocateCore(int allowExtra) { for(int i=1;i<9;i++) if(slots[i].state==0) { ResetPlayer(&slots[i]); slots[i].state=2; return i; } if(allowExtra) return 9; return 0; }
inline int MenuNetwork::BroadcastCore(int slot,PlayerSlot *info) { if(CountCore()<=1) return 0; MenuMessage packet; MenuMessageSlot(&packet,0x1014,slot,info); MenuSend(&packet,0); return 1; }
inline void MenuNetwork::ResetReadyCore() { memset(ready,0,sizeof(ready)); ready[0]=1; for(int i=1;i<=playerCount;i++) if(GetModePlayer(modeCheckName,i)->state!=2) ready[i]=1; }
inline void MenuNetwork::BroadcastAllCore() { for(int i=1;i<=9;i++) BroadcastCore(i,&slots[i]); }
inline void MenuNetwork::ResetCore() { memset(slots,0,sizeof(slots)); memset(mapping,0,sizeof(mapping)); memset(ready,0,sizeof(ready)); ClearList(modeLogName); ClearList(networkLog); SetList(networkLog); localSlot=0; host=0; NetworkReset(); gameOptions.Reset(); chat.Reset(); ResetQueue(networkQueue); paused=0; tail.unknown4902=0; hostLost=0; multiplayerActive=1; }

inline int MenuNetwork::ConnectedCore() { if(!networkEnabled) return 0; Poll(); int result=0; if(slots[1].state!=0) result=PeerAlive(1); return result; }
void MenuNetwork::DropPeer(int peer) {
 int slot=FindPeer(peer); char message[100]; if(slot<1) return;
 


// Slot records begin at +0x3f0, have stride 0xbc; ready flags begin at +0xb48.
// SessionTail is packed to 2; the enclosing MenuNetwork retains 4-byte alignment.
// Unknown byte fields describe unrecovered data layouts. Names are working labels.
// The empty destructor makes Watcom emit the independently matched constructor.
ASSERT(peer<=32);
 ASSERT(slot<=8);
    EnterCriticalSection(&connectionLock.lock);
    if(slots[slot].state==0) {
        LeaveCriticalSection(&connectionLock.lock);
        return;
    }
    slots[slot].state=0;
    LeaveCriticalSection(&connectionLock.lock);
    sprintf(message,(const char*)AllocateMissionImage(resourcePool,10),slot,slots[slot].name);
    chat.Log(ReadLocalPeer(),message);
    if(slot>=1) ready[slot]=1;
    if(slot==1 && multiState==6) {
        CloseStartScreen();
        ShowMessage((const char*)AllocateMissionImage(resourcePool,5,0));
        slots[localSlot].state=0;
        StopMultiplayer(0);
        return;
    }
    RemoveModePlayer(modeCheckName,slot);
    if(CountCore()==1 && multiState!=2) StopMultiplayer(1);
    if(slot<gMenuNetwork.localSlot) {
        int found=0;
        for(int i=1;i<gMenuNetwork.localSlot;i++) if(slots[i].state==2) found=1;
        if(!found) {
            int extra=lastPlayer;
            GetSlot(extra)->state=3;
            ModeSlot(extra)->state=3;
            if(leaderPlayer==lastPlayer) {
                if(multiplayerActive) OpenModeLog(modeLogName);
                else OpenMode(modeCheckName);
            }
        }
    }
    chat.Flush();
}
void StartStatus(const char *message,int duration) {
    ShowStatus(g_mainWindow,message,-1,duration);
}
int StartMultiplayer(const char *name,int joining) {
    gMenuNetwork.ResetCore();
    memcpy(startGuid,sessionGuid,16);
    if(joining) {
        if(!NetworkJoin(name)) return 0;
    }
    else {
        if(!NetworkCreate(name)) return 0;
    }
    NetworkBegin();
    if(!HasMission(profileManager)) {
        char filename[256];
        for(int i=0;searchPaths[i];i++) {
            sprintf(filename,startFormat,searchPaths[i],1,'A');
            if(FileExists(filename) && LoadMission(filename,1)) break;
        }
    }
    gMenuNetwork.host=(isHostPeer==1);
    if(gMenuNetwork.host) {
        savedRandom=MissionRandomTable[0];
        multiState=2;
        hostDropFlag=0;
        gMenuNetwork.localSlot=gMenuNetwork.AllocateCore(0);
        PlayerInfo info;
        GetPlayerInfo(profileManager,&info);
        PlayerSlot *p=&gMenuNetwork.slots[gMenuNetwork.localSlot];
        strcpy(p->name,name);
        strcpy(p->shortName,name);
        strcpy(p->description,blankDescription);
        p->index=1;
        p->peer=localPeer;
        strcpy(p->name,name);
        p->kind=info.kind;
        p->value=info.value;
    }
    else {
        multiState=5;
        MenuMessage packet(0x100d,name);
        MenuSend(&packet,1);
        while(multiState!=6 && multiState!=10 && PeerAlive(1)) {
            Sleep(100);
            GuiPumpMessages();
            NetworkPoll();
        }















































































































































        if(gMenuNetwork.localSlot<=0) AssertFail(mstartFile,__LINE__,startFailure);
        if(multiState==10) {
            NetworkStop();
            return 0;
        }
    }
    return 1;
}
int MenuNetwork::Count() {
    return CountCore();
}
int MenuNetwork::AllocateSlot(int allowExtra) {
    for(int i=1;i<9;i++) if(slots[i].state==0) {
        ResetPlayer(&slots[i]);
        slots[i].state=2;
        return i;
    }
    if(allowExtra) return 9;
    return 0;
}
int MenuNetwork::BroadcastInfo(void *info) {
    if(!IsHost()) return 0;
    if(CountCore()<=1) return 0;
    MenuMessage packet;
    MenuMessageInfo(&packet,0x100e,0,info);
    MenuSend(&packet,0);
    return 1;
}
int MenuNetwork::BroadcastLocal() {
    PlayerSlot *p=&slots[localSlot];
    p->index=localSlot;
    p->peer=localPeer;
    PlayerInfo info;
    GetPlayerInfo(profileManager,&info);
    p->kind=info.kind;
    p->value=info.value;
    return BroadcastCore(localSlot,p);
}
int MenuNetwork::BroadcastSlot(int slot,PlayerSlot *info) {
    if(CountCore()<=1) return 0;
    MenuMessage packet;
    MenuMessageSlot(&packet,0x1014,slot,info);
    MenuSend(&packet,0);
    return 1;
}
int MenuNetwork::Begin() {
    NetworkRefresh();
    if(*(int*)(gameOptions.data+24)>=9) *(int*)(gameOptions.data+28)=1;
    GameInfo info;
    GetGameInfo(profileManager,&gameOptions,&info);
    BroadcastAllCore();
    MenuMessage packet;
    MenuMessageGame(&packet,0x100f,&info);
    MenuSend(&packet,0);
    multiState=3;
    chat.Log(0,hostGreeting);
    StatusCore((const char*)AllocateMissionImage(resourcePool,14,0),2000);
    MissionRandomTable[0]=savedRandom;
    ApplyGameInfo(profileManager,&info);
    gMenuNetwork.chat.Activate();
    gMenuNetwork.chat.Log(0,joiningMessage);
    UiEvent(uiSystem,8);
    return 1;
}
void MenuNetwork::Finish(GameInfo *info) {
    StatusCore((const char*)AllocateMissionImage(resourcePool,14,0),2000);
    MissionRandomTable[0]=savedRandom;
    ApplyGameInfo(profileManager,info);
    gMenuNetwork.chat.Activate();
    gMenuNetwork.chat.Log(0,joiningMessage);
    UiEvent(uiSystem,8);
}
int MenuNetwork::Active() {
    return multiplayerActive;
}
int StopMultiplayer(int notify,int restart) {
    int result;
    if(!multiplayerActive) result=0;
    else {
    if(gMenuNetwork.IsHost()) currentOptions=gameOptions;
    if(restart && serverMode && restartData) {
        multiplayerActive=0;
        modeSwitch=0;
        ResetTransport(networkSystem);
        ClearList(modeLogName);
        ClearList(networkLog);
        ResetQueue(networkQueue);
        if(!RestartNetwork()) {
            NetworkStop();
            NetworkShutdown();
        }
    }
    else if(serverMode && notify) {
        modeSwitch=0;
        if(networkThread) {
            Sleep(300);
            TerminateThread(networkThread,0);
            CloseHandle(networkThread);
            networkThread=0;
        }
        ResetTransport(networkSystem);
        ClearList(modeLogName);
        ClearList(networkLog);
        ResetQueue(networkQueue);
    }
    else {
        if(notify && CanContinue()) ShowMessage((const char*)AllocateMissionImage(resourcePool,3,0));
        else {
            gMenuNetwork.chat.Log(0,bailingMessage);
            ClearModePlayers(modeCheckName);
            MenuMessage packet;
            MenuMessageBasic(&packet,0x1016);
            MenuSend(&packet,0);
            TimedCore(DisconnectMessage(),1500);
        }
        multiplayerActive=0;
        modeSwitch=0;
        NetworkStop();
        ResetTransport(networkSystem);
        ClearList(modeLogName);
        ClearList(networkLog);
        ResetQueue(networkQueue);
        CommitModePlayers(modeCheckName);
        NetworkShutdown();
    }
    result=1;
    }
    return result;
}
DWORD __stdcall MenuNetwork::ThreadProc(void*) {
    NetworkCallback();
    return 0;
}
// Constructor emitted through the inline member definition above.
void MenuNetwork::Reset() {
    ResetCore();
}
struct ActorRecord {
    int unknown,state;
}
;
extern ActorRecord *ActorGetByIndex(int);
int GetActorState(int actor) {
    return ActorGetByIndex(actor)->state;
}
void MenuNetwork::SetMapping(int value,int index) {
    mapping[index]=value;
}
void MenuNetwork::CompactPlayers() {
    int peer=PeerCore(localSlot);
    int i=1;
    {
        PlayerSlot *p=GetSlot(1);
        while(i++<=9) {
            if(p->state==6) p->state=0;
            p++;
        }
    }
    i=1;
    int dest=i;
    while(i<=9 && slots[i].state!=0) {
        slots[i].color=playerColors[i];
        i++;
        dest++;
    }
    while(i<=9) {
        if(slots[i].state!=0) {
            slots[i].color=playerColors[i];
            slots[i].index=dest;
            slots[dest++]=slots[i];
            slots[i].state=0;
        }
        i++;
    }
    localSlot=FindPeer(peer);
    slots[localSlot].state=1;
    int last;
    for(last=9;last>0 && slots[last].state==0;last--) {
    }
    int extra=last+1;
    lastPlayer=extra;
    PlayerSlot *p=&slots[extra];
    p->state=3;
    playerColors[lastPlayer]=16;
    p->color=16;
    strcpy(p->name,(const char*)AllocateMissionImage(textPool,0x9d,0));
    p=GetSlot(1);
    for(int n=1;n<=extra;n++,p++) AddModePlayer(modeCheckName,p->state,p);
    localPlayer=localSlot;
}
void MenuNetwork::CopySlot(int index,PlayerSlot *info) {
    slots[index]=*info;
    slots[index].index=index;
}
void MenuNetwork::BroadcastAll() {
    for(int i=1;i<=9;i++) BroadcastCore(i,&slots[i]);
}
void MenuNetwork::ResetReady() {
    memset(ready,0,sizeof(ready));
    ready[0]=1;
    for(int i=1;i<=playerCount;i++) if(GetModePlayer(modeCheckName,i)->state!=2) ready[i]=1;
}
void MenuNetwork::Synchronize(int target,int source,int) {
    MenuMessage packet,ack;
    MenuMessageData(&packet,0x101c,source,source,0);
    MenuMessageBasic(&ack,0x101e);
    ResetReadyCore();
    if(source!=target) MenuSend(&packet,GetModePlayer(modeCheckName,target)->peer);
    int count=0;
    while(!ReadyCore() && Poll()) {
        if(source!=target && ++count>100) {
            count=0;
            MenuSend(&packet,GetModePlayer(modeCheckName,target)->peer);
        }
        Sleep(100);
        GuiPumpMessages();
        NetworkPoll();
    }
    if(source==target) MenuSend(&ack,0);
}
void MenuNetwork::Launch() {
    StatusCore((const char*)AllocateMissionImage(resourcePool,15,0),2000);
    int previous=leaderPlayer;
    leaderPlayer=1;
    Synchronize(1,localSlot);
    if(multiState==3) {
        FinishMode(modeLogName);
        MenuMessage packet;
        MenuMessageData(&packet,0x1012,ReadRandom(),0,0);
        MenuSend(&packet,0);
    }
    else while(multiState!=8 && ConnectedCore()) {
        Sleep(100);
        GuiPumpMessages();
        NetworkPoll();
        QueuePump(networkLog);
    }
    leaderPlayer=previous;
}
void MenuNetwork::MarkReady(int slot) {
    if(slot>=1) ready[slot]=1;
}
int MenuNetwork::AllReady() {
    return ReadyCore();
}
void MenuNetwork::SetAllReady() {
    for(int i=1;i<=playerCount;i++) gMenuNetwork.ready[i]=1;
}
void MenuNetwork::SetTimeout() {
    if(retryTimeout>0) {
        unsigned long delay=retryTimeout*1000;
        MenuMessage packet;
        MenuMessageData(&packet,0x1018,delay);
        MenuSend(&packet,0);
        TimerSet(&timer,delay);
    }
}
void MenuNetwork::SendStart(MenuNetwork *state,int restart) {
    int target;
    if(multiState==3) {
        state->host=state->localSlot;
        gMenuNetwork.mapping[state->localSlot]=state->localSlot;
        target=0;
    }
    else target=hostPeer;
    MenuMessage packet;
    int type;
    if(restart) type=0x1022;
    else type=0x1017;
    MenuMessageStart(&packet,type,state);
    MenuSend(&packet,target);
}
int MenuNetwork::PeerForSlot(int slot) {
    ValidateSlot(slot);
    int state=slots[slot].state;
    if(state==2 || state==1) return slots[slot].peer;
    return -1;
}
int MenuNetwork::SlotForPeer(int peer) {
    for(int i=0;i<=8;i++) if(peer==slots[i].peer && slots[i].state!=0) return slots[i].index;
    return -1;
}
int MenuNetwork::IsLeader(int slot) {
    ValidateSlot(slot);
    int i;
    i=1;
    while(slots[i].state==0) i++;
    return i==slot;
}
int MenuNetwork::LeaderPeer() {
    return PeerCore(FindLeader());
}
int MenuNetwork::LeaderSlot() {
    return FindLeader();
}
int MenuNetwork::CheckLeader() {
    int slot=FindLeader();
    if(slot==localSlot) return 1;
    if(slots[slot].state==2) {
        int peer=slots[slot].peer;
        if(PeerAlive(peer)) {
            hostLost=0;
            return 1;
        }
        if(hostLost==0) {
            hostLost=1;
            MenuMessage failure,removed;
            if(CountCore()>2) {
                MenuMessage &packet=failure;
                MenuMessageData(&packet,0x1023,peer);
                MenuSend(&packet,0);
            }
            else {
                DropPeer(peer);
                slots[slot].state=0;
                MenuMessage &packet=removed;
                MenuMessageData(&packet,0x1024,peer);
                MenuSend(&packet,0);
            }
        }
    }
    return 0;
}
int MenuNetwork::Connected() {
    return ConnectedCore();
}
int MenuNetwork::Poll() {
    if(!networkEnabled) return 0;
    for(int i=2;i<=9;i++) {
        int peer=slots[i].peer;
        if(slots[i].state==2 && !PeerAlive(peer) && FindLeader()==localSlot) {
            DropPeer(peer);
            slots[i].state=0;
            MenuMessage packet;
            MenuMessageData(&packet,0x1024,peer);
            MenuSend(&packet,0);
        }
    }
    CheckLeader();
    return 1;
}
int MenuNetwork::IsRemote(int slot) {
    if(slot!=localSlot && slots[slot].state==2) return 1;
    return 0;
}
void MenuNetwork::PostStart() {
    PostMessage(g_mainWindow,0x111,0x7669,0);
}
void MenuNetwork::Pause() {
    paused=1;
}
void MenuNetwork::Resume() {
    paused=0;
}
int MenuNetwork::IsRunning() {
    return paused==0;
}

