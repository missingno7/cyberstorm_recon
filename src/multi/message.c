/* multi\message.c -- multiplayer menu messages: building, checksums, sending, dispatch (cs11 004706b0..00471c78).
 * Names are working labels; MenuNetwork/PlayerSlot/MenuMessage* follow multi\mstart.c (lane s1). */
#include <windows.h>
#include <string.h>
#include <stdio.h>
void AssertFail(const char *file, int line, const char *message);
static char messageFile[] = __FILE__;
#define ASSERT(e) { if (!(e)) AssertFail(messageFile, __LINE__, 0); }
#define ASSERT_MSG(e, m) { if (!(e)) AssertFail(messageFile, __LINE__, m); }

struct Timer { unsigned long start, end, duration; };
void TimerSet(Timer *t, unsigned long duration);
struct PlayerSlot { int state, index; char unknown08[8]; char name[52], shortName[20], description[80]; int peer, kind, value, color; char unknownB8[4]; };
struct Options { char data[66]; };
struct GameInfo { char data[88]; };
struct Chat { char data[0x3d7a]; void Show(const char *name, const char *text); };
extern int actorCount;
struct MenuNetwork {
    int localSlot, host; int mapping[250]; PlayerSlot slots[10]; int ready[10]; int unknownB70, paused, hostLost; Timer timer; Chat chat;
    static int Active(); void DropPeer(int peer); int Count(); int AllocateSlot(int allowExtra); int BroadcastLocal(); void Finish(GameInfo *info);
    void SetMapping(int value, int index); void CopySlot(int index, PlayerSlot *info); void MarkReady(int slot, int source); void SetAllReady();
    int SlotForPeer(int peer); int LeaderPeer(); int CheckLeader(); void Pause();
    int LocalActor(int netId) { if (netId >= 0 && netId <= actorCount) return mapping[netId]; return -1; }
};
extern MenuNetwork gMenuNetwork;
extern Options gameOptions;
int StopMultiplayer(int code, int restart = 0);

/* Actors carried by actor messages. */
struct ActorBlock { char bytes[0x10b]; };
struct ActorListData { char bytes[0x7e]; };
struct ActorListObject : ActorListData { ActorListObject(); ~ActorListObject(); void Fixup(); };
struct ActorState { char bytes[0x1d]; };
struct ActorPosition { int x, y; };
struct ActorRecord {
    int id, netId, kind, unknown0c, target; char unknown14[4]; ActorBlock *block; ActorListObject *extra;
    char unknown20[0x12]; ActorPosition home, goal; int targetName, stance;
};
ActorRecord *ActorGetByIndex(int index); ActorRecord *ActorGetByUnit(int unit); void ActorActivate(ActorRecord *actor);
ActorRecord *ActorListAdd(int kind, ActorBlock *block, ActorListObject *extra, ActorPosition *position, int a, int b);
void ActorSaveState(ActorRecord *actor, ActorState *state);
int ActorLoad(ActorRecord *actor, ActorBlock *block, ActorListObject *extra, ActorState *state);
void ActorUpdate(ActorRecord *actor, ActorBlock *block, ActorListObject *extra, ActorState *state);
void LogicActorDelete(void *logic, int actor);
void TargetObjectInit(char *names, ActorPosition *position, int name); void BestLocSetData(char *names, ActorPosition *position, int name);
int ResolveName(char *names, ActorPosition *position);
extern char fallbackName[]; extern char g4dd0c0[]; extern int gLocalActorKind; extern int gRemoteActorKind;

/* Message layout (packed, 0x1da bytes). */
struct MessageHeader { int type; unsigned checksum; int sequence; int ackLow; int ackHigh; unsigned size; int from; int to; };
struct DataBody { int a, b, c; };
struct SlotBody { int slot; PlayerSlot info; };
struct InfoBody { int a, b, slot; Options options; };
struct ActorBody { ActorBlock block; ActorListData extra; int netId, kind, index, targetName, stance; ActorState state; };
struct SyncBody { int v20, v24, v28, v2c, v30, v34, v38, v3c, v40, v44, v48, v4c, v50; };
struct SyncMessage : MessageHeader { SyncBody sync; };
struct MenuMessage : MessageHeader {
    union { DataBody data; SlotBody slot; InfoBody info; GameInfo game; ActorBody actor; char text[0x1ba]; };
    MenuMessage() {} MenuMessage(unsigned type, const char *text);
    unsigned Size(){return size;} int From(); int IsBroadcast(); void ApplyPackets(); void Clear();
};
MenuMessage *MenuMessageBasic(MenuMessage *m, int type); MenuMessage *MenuMessageData(MenuMessage *m, int type, int a, int b = 0, int c = 0);
MenuMessage *MenuMessageInfo(MenuMessage *m, int type, int slot, Options *options); MenuMessage *MenuMessageSlot(MenuMessage *m, int type, int slot, PlayerSlot *info);
MenuMessage *MenuMessageStart(MenuMessage *m, int type, ActorRecord *actor); int MenuSend(MenuMessage *m, int peer); int LocalPeer();

/* Transport (multi\link.c, multi\minipkt.c) and screens. */
int PacketType(void *packet); void PacketApply(void *packet); void Crc16(unsigned short *crc, void *data, int length);
int QueueSequence(char *queue, int peer); void QueueAcks(char *queue, int peer, int *high, int *low); void QueueSent(char *queue, MenuMessage *message);
void QueueAcknowledge(char *queue, int peer, int low, int high); void QueueStatus(char *queue, int peer, int *a, int *b, int *high, int *low);
void QueueUpdate(char *queue, int peer, int high, int low, int a, int b); void QueueResend(char *queue, int peer, int a, int b); void QueueRelease(char *queue, int a, int b);
int NetworkSend(int peer, void *data, unsigned size, int flags); void SetLinkTimer(int peer); struct TerrainGrid; long GridSerializeStats(TerrainGrid *grid, void *stats);
void ModeRefresh(char *mode); void ShowMessage(const char *text); void *AllocateMissionImage(void *pool, int id, int flags = 0);
void ScreenPlayerLeft(char *screen, int slot); void ValidateSlot(int slot); void ScreenPlayerText(char *screen, int slot, char *text);
extern char g4ba2bc[]; extern void *g_mainWindow; extern char modeCheckName[]; extern char resourcePool[]; extern char networkQueue[];
extern int localPeer; extern int multiState; extern int leaderPlayer; extern int MissionRandomTable[];
struct NetworkLog { MenuMessage message; int pending; }; extern NetworkLog networkLog;
extern char gPlayerFormalName[]; extern char gPlayerFriendlyName[]; extern char gDefaultPlanetName[];
struct QueueNode; class Queue { public: QueueNode *Add(unsigned size, void *source); }; extern Queue gReceiveQueue;

int MenuMessage::From()
{
    return from;
}

int MenuMessage::IsBroadcast()
{
    return to == 0;
}

void NetworkDropPeer(int peer)
{
    gMenuNetwork.DropPeer(peer);
}

void MenuMessage::ApplyPackets()
{
    int i = 0;
    char *packet = text;
    while (i < 16) {
        if (PacketType(packet) == 0)
            break;
        PacketApply(packet);
        packet += 0x14;
        i++;
    }
}

const char *MessageName()
{
    return "Unrecognized message";
}

void MessageActorMoved(MenuMessage *self, MenuMessage *m)
{
    ActorRecord *actor = ActorGetByUnit(m->actor.targetName);
    actor->netId = m->actor.netId;
    actor->stance = m->actor.stance;
    gMenuNetwork.SetMapping(actor->id, m->actor.netId);
}





void MessageActorCreated(MenuMessage *self, MenuMessage *m)
{
    if (m->actor.kind == gLocalActorKind) {
        ActorRecord *actor = ActorGetByIndex(m->actor.index);
        actor->netId = m->actor.netId;
        actor->targetName = m->actor.targetName;
        TargetObjectInit(fallbackName, &actor->home, m->actor.targetName);
        BestLocSetData(fallbackName, &actor->goal, m->actor.targetName);
        gMenuNetwork.SetMapping(actor->id, actor->netId);
    } else if (m->actor.kind == gRemoteActorKind) {
        ActorRecord *actor = ActorGetByUnit(m->actor.targetName);
        ASSERT_MSG(actor, "Failed to find local instance of the Cybrid");
        if (actor) {
            actor->netId = m->actor.netId;
            actor->stance = m->actor.stance;
            gMenuNetwork.SetMapping(actor->id, m->actor.netId);
        }











    } else {
        ActorPosition position;
        ActorBlock *block = new ActorBlock;
        ActorListObject *extra = new ActorListObject;
        ASSERT(block && extra);
        *block = m->actor.block;
        *extra = *(ActorListObject *)&m->actor.extra;
        extra->Fixup();
        BestLocSetData(fallbackName, &position, m->actor.targetName);
        ActorRecord *actor = ActorListAdd(m->actor.kind, block, extra, &position, 1, 0);
        LogicActorDelete(g4dd0c0, (int)actor);
        actor->target = -1;
        actor->stance = m->actor.stance;
        ActorActivate(actor);
        if (multiState == 3) {
            m->actor.netId = actor->netId = actor->id;
            m->actor.targetName = ResolveName(fallbackName, &actor->goal);
            MenuSend(self, 0);
        } else {
            actor->netId = m->actor.netId;
        }
        gMenuNetwork.SetMapping(actor->id, m->actor.netId);
    }
}






















void MessageActorUpdate(MenuMessage *self, MenuMessage *m)
{
    ActorBlock *block = new ActorBlock;
    ActorListObject *extra = new ActorListObject;
    ASSERT(block && extra);
    *block = m->actor.block;
    *extra = *(ActorListObject *)&m->actor.extra;
    ActorRecord *actor = ActorGetByIndex(gMenuNetwork.LocalActor(m->actor.netId));
    ActorUpdate(actor, block, extra, &m->actor.state);
    delete block;
    delete extra;
}













void MessageJoinRequest(MenuMessage *m)
{
    MenuMessage reject;
    MenuMessage slotMessage;
    MenuMessage welcome;
    MenuMessage info;
    int slot;
    int peer = m->from;
    if (multiState == 2 && gMenuNetwork.Count() < 8) {
        ASSERT(peer <= 32);
        slot = gMenuNetwork.AllocateSlot(0);
        if (slot) {
            m->data.b = 2;
            gMenuNetwork.CopySlot(slot, (PlayerSlot *)&m->data.b);
            MenuMessageData(&welcome, 0x1010, 0x100d, 0, 0);
            MenuSend(&welcome, peer);
            for (int i = 1; i < 9; i++) {
                if (gMenuNetwork.slots[i].state == 0)
                    continue;
                MenuMessageSlot(&slotMessage, 0x1014, i, &gMenuNetwork.slots[i]);
                MenuSend(&slotMessage, peer);
            }
            info.type = 0x100e;
            info.size = 0x6e;
            info.info.options = gameOptions;
            info.info.a = 1;
            info.info.b = 1;
            info.info.slot = slot;
            MenuSend(&info, peer);
            return;
        }
    }
    MenuMessageBasic(&reject, 0x101a);
    MenuSend(&reject, peer);
}

void MenuMessage::Clear()
{
    memset(this, 0, 0x1da);
}






















void MessageCheckSync(MenuMessage *m)
{
    SyncMessage local;
    GridSerializeStats((TerrainGrid *)fallbackName, &local);
    SyncMessage *remote = (SyncMessage *)m;
    ASSERT(local.sync.v20 == remote->sync.v20);
    ASSERT(local.sync.v2c == remote->sync.v2c);
    ASSERT(local.sync.v30 == remote->sync.v30);
    ASSERT(local.sync.v34 == remote->sync.v34);
    ASSERT(local.sync.v3c == remote->sync.v3c);
    ASSERT(local.sync.v44 == remote->sync.v44);
    ASSERT(local.sync.v4c == remote->sync.v4c);
    ASSERT(local.sync.v50 == remote->sync.v50);
}

int IsSystemMessage(MenuMessage *m);
int MessageValid(MenuMessage *m);























































































































void MessageActorJoined(MenuMessage *self, MenuMessage *m);

void MessageSendStatus(int peer);

inline void PostNetworkEvent(int code)
{
    PostMessageA((HWND)g_mainWindow, WM_COMMAND, 0x7664, code);
}

int MessageDispatch(MenuMessage *m, int expected)
{
    MenuMessage request;
    MenuMessage removed;
    char text[100];

    int slot;
    if (!MessageValid(m))
        return 1;
    if (expected != 0x1025 && expected != m->type)
        return 0;
    if (m->type == 0x1007 || m->type == 0x1008 || !IsSystemMessage(m))
        SetLinkTimer(m->from);
    switch (m->type) {
    case 5:
        PostNetworkEvent(m->checksum);
        break;
    case 0x1016:
        PostNetworkEvent(m->from);
        break;
    case 0x1005:
        QueueRelease(networkQueue, m->data.a, m->data.b);
        break;
    case 0x1004:
        QueueResend(networkQueue, m->from, m->data.a, m->data.b);
        break;
    case 0x1013:
        QueueUpdate(networkQueue, m->from, m->ackHigh, m->ackLow, m->data.a, m->data.b);
        MessageSendStatus(m->from);
        break;
    case 0x1008:
        QueueUpdate(networkQueue, m->from, m->ackHigh, m->ackLow, m->data.a, m->data.b);
        break;
    case 0x1001: {
        slot = gMenuNetwork.SlotForPeer(m->from);
        const char *name = slot > 0 && slot < 9 ? gMenuNetwork.slots[slot].name : "????";
        gMenuNetwork.chat.Show(name, m->text);
        break;
    }
    case 0x1006:
        ModeRefresh(modeCheckName);
        break;
    case 0x1017:
        if (m->from != localPeer)
            MessageActorCreated(m, m);
        break;
    case 0x1022:
        if (m->from != localPeer)
            MessageActorMoved(m, m);
        break;
    case 0x101d:
        MessageCheckSync(m);
        break;
    case 0x100d:
        MessageJoinRequest(m);
        break;
    case 0x1011: {

        MenuMessageBasic(&request, 0x100d);
        MenuSend(&request, m->from);
        break;
    }
    case 0x100e:
        gameOptions = m->info.options;
        if (m->info.slot) {
            gMenuNetwork.localSlot = m->info.slot;
            gMenuNetwork.BroadcastLocal();
        }
        if (multiState == 5)
            multiState = 6;
        break;
    case 0x1012:
        multiState = 8;
        MissionRandomTable[0] = m->data.a;
        break;
    case 0x1014:
        slot = m->slot.slot;
        gMenuNetwork.CopySlot(slot, &m->slot.info);
        break;
    case 0x101c:
        slot = m->data.a;
        gMenuNetwork.MarkReady(slot, m->data.c);
        break;
    case 0x101e:
        gMenuNetwork.SetAllReady();
        break;
    case 0x1019:
        if (expected != 0x1019) {
            if (PacketType(m->text) != 0x16)
                return 0;
            if (leaderPlayer != gLocalActorKind)
                return 0;
        } else {
            networkLog.message = *m;
            networkLog.pending = 0;
        }
        break;
    case 0x1018:
        TimerSet(&gMenuNetwork.timer, m->data.a);
        break;
    case 0x101b:
        MessageActorJoined(m, m);
        break;
    case 0x1021:
        MessageActorUpdate(m, m);
        break;
    case 0x1020:
        slot = gMenuNetwork.SlotForPeer(m->from);
        ValidateSlot(slot);
        ScreenPlayerLeft(g4ba2bc, slot);
        break;
    case 0x1023: {

        int leader = gMenuNetwork.LeaderPeer();
        if (gMenuNetwork.CheckLeader())
            break;
        int peer = m->data.a;
        if (leader != peer)
            break;
        gMenuNetwork.DropPeer(peer);
        gMenuNetwork.slots[gMenuNetwork.SlotForPeer(peer)].state = 0;
        MenuMessageData(&removed, 0x1024, peer);
        MenuSend(&removed, 0);
        break;
    }
    case 0x1024:
        if (m->data.a == LocalPeer()) {
            
            sprintf(text, (const char *)AllocateMissionImage(resourcePool, 11), 30);
            ShowMessage(text);
            gMenuNetwork.slots[gMenuNetwork.localSlot].state = 0;
            StopMultiplayer(0);
            return 0;
        }
        NetworkDropPeer(m->data.a);
        break;
    case 0x101f:
        ShowMessage((const char *)AllocateMissionImage(resourcePool, 4));
        gMenuNetwork.slots[gMenuNetwork.localSlot].state = 7;
        StopMultiplayer(0);
        return 0;
    case 0x101a:
        multiState = 10;
        ShowMessage((const char *)AllocateMissionImage(resourcePool, 7));
        break;
    case 0x100f:
        multiState = 7;
        gMenuNetwork.Finish(&m->game);
        break;
    case 0x1007:
        MessageSendStatus(m->from);
        break;
    case 0x1002:
        slot = gMenuNetwork.SlotForPeer(m->from);
        ScreenPlayerText(g4ba2bc, slot, m->text);
        break;
    case 0x20:
    case 0x21:
    case 0x1000:
    case 0x1003:
    case 0x1010:
        break;
    case 1:
    case 2:
    case 3:
    case 7:
    case 0xe:
    case 0x31:
    case 0x1009:
    case 0x100a:
    case 0x100b:
    case 0x100c:
    default:
        break;
    }
    return 1;
}

void MessageActorJoined(MenuMessage *self, MenuMessage *m)
{
    MenuMessage reply;
    ActorBlock *block = new ActorBlock;
    ActorListObject *extra = new ActorListObject;
    ASSERT(block && extra);
    *block = m->actor.block;
    *extra = *(ActorListObject *)&m->actor.extra;
    ActorRecord *actor = ActorGetByIndex(gMenuNetwork.LocalActor(m->actor.netId));

    ASSERT(actor);
    if (ActorLoad(actor, block, extra, &m->actor.state) == 0) {
        int peer = self->from;
        MenuMessageStart(&reply, 0x1021, actor);
        MenuSend(&reply, peer);
        gMenuNetwork.Pause();
    }
    delete block;
    delete extra;
}

int LocalPeer()
{
    return localPeer;
}

unsigned MessageChecksum(MenuMessage *m)
{
    unsigned short sum[2];
    sum[0] = 0;
    Crc16(sum, &m->type, 4);
    Crc16(sum, &m->sequence, 4);
    Crc16(sum, &m->ackLow, 4);
    Crc16(sum, &m->ackHigh, 4);
    Crc16(sum, &m->size, 4);
    Crc16(sum, &m->from, 4);
    Crc16(sum, &m->to, 4);
    return sum[0];
}

int MessageValid(MenuMessage *m)
{

    if (m->type == 2 || m->type == 1 || m->type == 3 || m->type == 5 || m->type == 7 || m->type == 0xe || m->type == 0x20
        || m->type == 0x21 || m->type == 0x31 || m->type == 0x484b)
        return 1;
    if (m->Size() > 0x1da)
        return 0;
    if (m->type < 0x1001 || m->type > 0x1025)
        return 0;
    if (MessageChecksum(m) != m->checksum)
        return 0;
    return 1;
}

int IsSystemMessage(MenuMessage *m)
{
    if ((m->type >= 0x1001 && m->type <= 0x100a) || m->type == 3 || m->type == 5)
        return 1;
    return 0;
}

int MessageTransmit(MenuMessage *m)
{
    return NetworkSend(m->to, m, m->size, 0);
}

int MenuSend(MenuMessage *m, int peer)
{
    if (!MenuNetwork::Active())
        return 0;
    m->ackLow = 0;
    m->ackHigh = 0;
    m->to = peer;
    m->from = localPeer;
    m->sequence = IsSystemMessage(m) ? 0 : QueueSequence(networkQueue, peer);
    QueueAcks(networkQueue, peer, &m->ackHigh, &m->ackLow);
    m->checksum = MessageChecksum(m);
    if (peer == LocalPeer())
        return gReceiveQueue.Add(0x1da, m) != 0;
    QueueSent(networkQueue, m);
    return MessageTransmit(m);
}

int MessageDeliver(MenuMessage *m)
{
    int to = m->to;
    if (to == LocalPeer())
        return gReceiveQueue.Add(0x1da, m) != 0;
    return MessageTransmit(m);
}

void MessageAcknowledge(MenuMessage *m, char *queue)
{
    QueueAcknowledge(queue, m->from, m->ackLow, m->ackHigh);
}

void MessageRequestStatus(int peer)
{
    MenuMessage reply;
    if (localPeer != peer) {
        MenuMessageBasic(&reply, 0x1013);
        QueueStatus(networkQueue, peer, &reply.data.a, &reply.data.b, &reply.ackHigh, &reply.ackLow);
        MenuSend(&reply, peer);
    }
}

void MessageSendStatus(int peer)
{
    MenuMessage reply;
    if (peer != LocalPeer()) {
        MenuMessageBasic(&reply, 0x1008);
        QueueStatus(networkQueue, peer, &reply.data.a, &reply.data.b, &reply.ackHigh, &reply.ackLow);
        MenuSend(&reply, peer);
    }
}

void MessageSendPing(int peer)
{
    MenuMessage reply;
    if (localPeer != peer) {
        MenuMessageBasic(&reply, 0x1007);
        MenuSend(&reply, peer);
    }
}

MenuMessage *MenuMessageNull(MenuMessage *m)
{
    m->type = 0x1000;
    m->size = 0x20;
    return m;
}

MenuMessage *MenuMessageBasic(MenuMessage *m, int type)
{
    m->type = type;
    m->size = 0x20;
    if (type == 0x100d) {
        strcpy(m->slot.info.name, gPlayerFormalName);
        strcpy(m->slot.info.shortName, gPlayerFriendlyName);
        strcpy(m->slot.info.description, gDefaultPlanetName);
        m->slot.info.peer = localPeer;
        m->size = 0xe0;
    } else if (m->type == 0x1008 || m->type == 0x1013) {
        m->size = 0x28;
    }
    if (type == 0x101d)
        m->size = 0x54;
    return m;
}

MenuMessage *MenuMessageStart(MenuMessage *m, int type, ActorRecord *actor)
{
    m->type = type;
    ActorBlock *block = &m->actor.block;
    *block = *actor->block;
    ActorListData *extra = &m->actor.extra;
    *extra = *actor->extra;
    m->actor.kind = actor->kind;
    m->actor.index = actor->id;
    m->actor.netId = actor->netId;
    m->actor.targetName = actor->targetName;
    m->actor.stance = actor->stance;
    if (type != 0x1017)
        ActorSaveState(actor, &m->actor.state);
    m->size = 0x1da;
    return m;
}

MenuMessage *MenuMessageData(MenuMessage *m, int type, int a, int b, int c)
{
    m->type = type;
    m->data.a = a;
    m->data.b = b;
    m->data.c = c;
    m->size = 0x2c;
    return m;
}

MenuMessage *MenuMessageInfo(MenuMessage *m, int type, int slot, Options *options)
{
    m->type = type;
    m->size = 0x6e;
    Options *dest = &m->info.options;
    *dest = *options;
    m->info.a = 1;
    m->info.b = 1;
    m->info.slot = slot;
    return m;
}

MenuMessage *MenuMessageGame(MenuMessage *m, int type, GameInfo *game)
{
    m->type = type;
    m->size = 0x78;
    GameInfo *dest = &m->game;
    *dest = *game;
    return m;
}

MenuMessage *MenuMessageSlot(MenuMessage *m, int type, int slot, PlayerSlot *info)
{
    m->type = type;
    m->size = 0xe0;
    m->slot.slot = slot;
    m->slot.info = *info;
    return m;
}

MenuMessage::MenuMessage(unsigned type, const char *text)
{
    if (type == 0x1001 || type == 0x1002) {
        strlen(text);
        this->type = type;
        size = strlen(text) + 0x21;
        strcpy(this->text, text);
        return;
    }
    this->type = type;
    size = 0x20;
    strcpy(slot.info.name, text);
    strcpy(slot.info.shortName, text);
    strcpy(slot.info.description, " ");
    slot.info.peer = localPeer;
    size = 0xe0;
}







