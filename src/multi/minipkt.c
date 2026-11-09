/* multi\minipkt.c -- multiplayer "mini packets": small 20-byte game actions batched into one menu message
 * (cs11 00471ce8..004742bc). Names are working labels; packet type names come from the name table
 * (NULLMSG, HERCREPAIR, ...). MenuMessage/MenuNetwork follow multi\message.c and multi\mstart.c. */
#include <windows.h>

void AssertFail(const char *file, int line, const char *message);
#define ASSERT(e) { if (!(e)) AssertFail(minipktFile, __LINE__, 0); }

struct Timer;
void TimerSet(Timer *t, unsigned long duration);
struct Timer { unsigned long start, end, duration; Timer() { TimerSet(this, 0); } };
int TimerExpired(Timer *t);

#pragma pack(1)
struct Packet {
    int type;
    int id;
    int IsNull() { return type == 0; }
    union {
        struct { unsigned char slot; char value; int amount; } repair;
        struct { int heading; signed char mode; } face;
        struct { int target; int seed; unsigned char weapon; signed char aim; } fire;
        struct { signed char a, b, c; } rtnfire;
        struct { unsigned char front, back; } shields;
        struct { unsigned char system; } toggle;
        struct { int block, extra, actor; } check;
        struct { int x, y; } move;
        struct { int seed, arg; } turn;
    };
};
struct PayloadSlot { char unknown0[2]; char value; char unknown3; int amount; };
struct Payload { char unknown00[0x11]; PayloadSlot slots[23]; char front, back; };
#pragma pack()

struct ActorRecord;
struct MenuMessage {
    int type; unsigned checksum; int sequence; int ackLow; int ackHigh; unsigned size; int from; int to;
    union { Packet packets[16]; char text[0x1ba]; };
    MenuMessage();
    MenuMessage(int type);
    MenuMessage(int type, ActorRecord *actor);
    MenuMessage(int type, int a, int b = 0, int c = 0);
    void Clear();
};
int MenuSend(MenuMessage *m, int peer);
int LocalPeer(MenuMessage *m);

struct PlayerSlot { int state, index; char unknown08[8]; char name[52], shortName[20], description[80]; int peer, kind, value, color; char unknownB8[4]; };
extern int actorCount;
struct MenuNetwork {
    int localSlot, host; int mapping[250];
    static int Active(); void ResetReady(int serial); int AllReady(); int Poll(); void Resume(); int IsRunning();
    int LeaderSlot(); void Synchronize(int target, int source, int serial);
    int LocalActor(int netId) { if (netId >= 0 && netId <= actorCount) return mapping[netId]; return -1; }
};
extern MenuNetwork gMenuNetwork;

struct ActorMode {
    char unknown0[5]; unsigned char state; unsigned char stance;
    int Busy() { if (state != 1 && state != 7 && state != 14 && state != 17 && state != 18) return 1; return 0; }
};
struct Pilot { char unknown00[0x12]; int health; int Alive() { return health > 0; } };
struct ActorRecord {
    int id, netId, kind, target; char unknown10[8]; Payload *payload; Pilot *pilot; ActorMode mode;
    char unknown27[0x61]; char hasPilot; char unknown89[4]; char moved;
};

int ModeCheck(const char *mode);
extern char modeCheckName[];
void *CreateState(int actor);
void ReleaseState(void *state);
void ModeEvent(char *mode, ActorRecord *actor, int a, int b);
void SendEvent(void *target, int event, void *data = 0, int a = 0, int b = 0);
int UpdatePayload(void *payload, int stance);
ActorRecord *ActorFindByOwner(ActorRecord *owner);
PlayerSlot *GetModePlayer(void *mode, int player);
int cancel_current_link_entry(unsigned type);
void GuiPumpMessages(); void NetworkPoll();
int BlockChecksum(void *block); int PilotChecksum(void *pilot); int ActorChecksum(ActorRecord *actor);
void ActorFace(ActorRecord *actor, char mode);
void PayloadHeading(void *payload, int heading);
void ActorCrouch(ActorRecord *actor, int now); void ActorStand(ActorRecord *actor, int now);
void ActorMove(ActorRecord *actor, int x, int y);
void ModeFire(char *mode, ActorRecord *actor, int target, int weapon, int aim);
void ModeReturnFire(char *mode, int a, int b, int c);
void ModeToggleSystem(char *mode, int system, int value);
void ModeGiveFocus(char *mode, ActorRecord *actor);
void ModeRemoveFocus(char *mode);
void ModeEndTurn(char *mode, int arg); void ModeNextTurn(char *mode);
void ModeBeginDiagnose();
extern int leaderPlayer, gLocalActorKind, gRemoteActorKind;
extern int MissionRandomTable[];
inline int ReadRandom() { return MissionRandomTable[0]; }
extern void *g4d2498;

class PacketList : public MenuMessage {
public:
    int count;
    Timer timer;

    PacketList();
    void Reset();
    void Stop();
    Packet *Add();
    void Poll();
    void Send(int loopback);
    int Step();
    void Process();
    void Repair(ActorRecord *actor, char slot, char value, int amount);
    void Simple(int type, ActorRecord *actor);
    void RepairCurrent(char slot, char value, int amount);
    void JackUp(ActorRecord *actor);
    void Placidex(ActorRecord *actor);
    void Remove(ActorRecord *actor);
    void Crouch(ActorRecord *actor);
    void Stand(ActorRecord *actor);
    void Face(ActorRecord *actor, int mode, int heading);
    void Shields(ActorRecord *actor, char front, char back);
    void ShieldsCurrent(char front, char back);
    void Toggle(ActorRecord *actor, char system);
    void ToggleCurrent(char system);
    void Focus(ActorRecord *actor);
    void Move(ActorRecord *actor, int x, int y);
    void DeadPilot(ActorRecord *actor);
    void DeadActor(ActorRecord *actor);
    void Fire(ActorRecord *actor, int target, int weapon, int seed, int aim);
    void ReturnFire(ActorRecord *actor, char a, char b, char c);
    void EndTurn(int arg);
    void EnsureCybridEndTurn();
    void Diagnose();
    void Check(int type, ActorRecord *actor);
};

static char *packetNames[] = {
    "NULLMSG", "HERCREPAIR", "PILOTJACKUP", "PILOTPLACIDEX", "MOVEFACE", "CROUCH", "STAND", "FIRE", "RTNFIRE",
    "ADJUSTSHIELDS", "SYSTEMTOGGLE", "GIVEFOCUS", "MOVE", "ENDTURN", "CHECKACTOR", "BEGINDIAGNOSE",
    "ENDDIAGNOSE", "CHECKHERC", "CHECKPILOT", "DEADACTOR", "DEADPILOT", "DOSYNCH", "ENSURECYBRIDENDTURN",
    "REMOVEACTOR", 0
};
static char minipktFile[] = __FILE__;

PacketList modeLogName;
PacketList networkLog;
ActorRecord *gCheckedActor;
int gCheckActors;
int gDiagnoseSerial;

int PacketApply(Packet *p);

int PacketType(void *packet)
{
    return ((Packet *)packet)->type;
}

void PacketList::Process()
{
    while (Step())
        ;
}

int PacketList::Step()
{
    if (count == 16)
        cancel_current_link_entry(0x1019);
    if (count == 16)
        return 0;

    if (!MenuNetwork::Active()) {
        count = 16;
        return 1;
    }
    Packet *p = &packets[count];
    if (p->IsNull()) {
        count = 16;
        return 1;
    }
    if (PacketApply(p)) {
        count++;
        return 1;
    }
    return 0;
}

void CheckActor(Packet *p, ActorRecord *actor);

int PacketApply(Packet *p)
{
    ActorRecord *actor = (ActorRecord *)CreateState(gMenuNetwork.LocalActor(p->id));
    if (ModeCheck(modeCheckName) && p->type != 13 && p->type != 15 && p->type != 16 && p->type != 22 && p->type != 21)
        return 1;
    switch (p->type) {
    case 0:
        break;
    case 20:
        ModeEvent(modeCheckName, actor, 1000000, 0);
        break;
    case 19:
        ReleaseState(actor);
        break;
    case 1: {
        PayloadSlot *slot = &actor->payload->slots[p->repair.slot];
        slot->value = p->repair.value;
        slot->amount = p->repair.amount;
        break;
    }
    case 2:
        SendEvent(actor->pilot, 0x68);
        if (!actor->pilot->Alive())
            actor->hasPilot = 0;
        break;
    case 3:
        SendEvent(actor->pilot, 0x69);
        if (!actor->pilot->Alive())
            actor->hasPilot = 0;
        break;
    case 4:
        if (actor->mode.Busy())
            return 0;
        ActorFace(actor, p->face.mode);
        if (p->face.heading) {
            PayloadHeading(actor->payload, p->face.heading);
            actor->moved = 1;
        }
        break;
    case 5:
        if (actor->mode.Busy())
            return 0;
        ActorCrouch(actor, 1);
        break;
    case 6:
        if (actor->mode.Busy())
            return 0;
        ActorStand(actor, 1);
        break;
    case 9:
        actor->payload->front = p->shields.front;
        actor->payload->back = p->shields.back;
        UpdatePayload(actor->payload, actor->mode.stance);
        break;
    case 10:
        ModeToggleSystem(modeCheckName, p->toggle.system, -1);
        break;
    case 11:
        ModeGiveFocus(modeCheckName, actor);
        break;
    case 12:
        if (actor->mode.Busy())
            return 0;
        ActorMove(actor, p->move.x, p->move.y);
        break;
    case 13:
        MissionRandomTable[0] = p->turn.seed;
        ModeEndTurn(modeCheckName, p->turn.arg);
        ModeNextTurn(modeCheckName);
        break;
    case 7:
        if (actor->mode.Busy())
            return 0;
        actor->target = p->fire.target;
        MissionRandomTable[0] = p->fire.seed;
        ModeFire(modeCheckName, actor, actor->target, p->fire.weapon, p->fire.aim);
        break;
    case 8:
        ModeReturnFire(modeCheckName, p->rtnfire.a, p->rtnfire.b, p->rtnfire.c);
        break;
    case 15:
        ModeBeginDiagnose();
        gCheckActors = 1;
        gCheckedActor = 0;
        break;
    case 16: {
        gCheckActors = 0;
        MenuMessage reply(0x101c, gLocalActorKind, gLocalActorKind, p->turn.arg);
        int leader = leaderPlayer;
        if (leader == gRemoteActorKind)
            leader = gMenuNetwork.LeaderSlot();
        MenuSend(&reply, GetModePlayer(modeCheckName, leader)->peer);
        break;
    }
    case 21: {
        ModeBeginDiagnose();
        int leader = leaderPlayer;
        if (leader == gRemoteActorKind)
            leader = gMenuNetwork.LeaderSlot();
        gMenuNetwork.Synchronize(leader, gLocalActorKind, p->turn.arg);
        break;
    }
    case 14:
        if (actor) {
            if (actor->mode.Busy())
                return 0;
            if (p->id >= 0)
                CheckActor(p, actor);
        }
        break;
    case 22:
        if (leaderPlayer == gRemoteActorKind)
            ModeNextTurn(modeCheckName);
        break;
    case 23:
        ModeGiveFocus(modeCheckName, actor);
        ModeRemoveFocus(modeCheckName);
        break;
    }
    return 1;
}

void CheckActor(Packet *p, ActorRecord *actor)
{
    int block = BlockChecksum(actor->payload);
    int pilot = PilotChecksum(actor->pilot);
    int state = ActorChecksum(actor);
    if (p->check.actor == 0xbe1a || state == 0xbe1a)
        return;
    if (block == p->check.block && pilot == p->check.extra && state == p->check.actor)
        return;
    gCheckedActor = actor;
    if (gCheckActors) {
        MenuMessage m(0x101b, actor);
        MenuSend(&m, GetModePlayer(modeCheckName, leaderPlayer)->peer);
    }
}

const char *PacketName(Packet *p)
{
    return packetNames[p->type];
}

PacketList::PacketList()
{
    Reset();
}

void PacketList::Reset()
{
    count = 0;
    Clear();
    type = 0x1019;
    size = 0x160;
    TimerSet(&timer, 2000);
}

void PacketList::Stop()
{
    count = 16;
}

Packet *PacketList::Add()
{
    if (count == 16)
        Send(0);
    ASSERT(ModeCheck(modeCheckName));
    return &packets[count++];
}

/*
 * Flushes a partly filled list once its two second timer has run out.
 *
 */
void PacketList::Poll()
{
    if (count > 0 && TimerExpired(&timer))
        Send(0);
}

/*
 * Terminates the list with a NULLMSG packet, sends it to every peer (and to the
 * local peer too when asked) and starts a new list.
 *
 */
void PacketList::Send(int loopback)
{
    if (!ModeCheck(modeCheckName)) {
        Reset();
        return;
    }
    if (count < 16)
        packets[count].type = 0;
    MenuSend(this, 0);
    if (loopback)
        MenuSend(this, LocalPeer(this));
    Reset();
}

/*
 * HERCREPAIR: new value and repair amount of one payload slot.
 *
 */
void PacketList::Repair(ActorRecord *actor, char slot, char value, int amount)
{
    Packet *p = Add();
    p->type = 1;
    p->id = actor->netId;
    p->repair.slot = slot;
    p->repair.value = value;
    p->repair.amount = amount;
}

/*
 * Packets that carry nothing but the actor.
 *
 */
void PacketList::Simple(int type, ActorRecord *actor)
{
    Packet *p = Add();
    p->type = type;
    p->id = actor->netId;
}

/*
 * HERCREPAIR for the current actor.
 *
 */
void PacketList::RepairCurrent(char slot, char value, int amount)
{
    Repair((ActorRecord *)g4d2498, slot, value, amount);
}

/*
 * PILOTJACKUP
 *
 */
void PacketList::JackUp(ActorRecord *actor)
{
    Simple(2, actor);
}

/*
 * PILOTPLACIDEX
 *
 */
void PacketList::Placidex(ActorRecord *actor)
{
    Simple(3, actor);
}

/*
 * REMOVEACTOR
 *
 */
void PacketList::Remove(ActorRecord *actor)
{
    Simple(23, actor);
}

/*
 * CROUCH
 *
 */
void PacketList::Crouch(ActorRecord *actor)
{
    Simple(5, actor);
}

/*
 * STAND
 */
void PacketList::Stand(ActorRecord *actor)
{
    Simple(6, actor);
}

/*
 * MOVEFACE: facing mode and an optional new heading.
 */
void PacketList::Face(ActorRecord *actor, int mode, int heading)
{
    Packet *p = Add();
    p->type = 4;
    p->id = actor->netId;
    p->face.mode = mode;
    p->face.heading = heading;
}

/*
 * ADJUSTSHIELDS: front/back shield split.
 */
void PacketList::Shields(ActorRecord *actor, char front, char back)
{
    Packet *p = Add();
    p->type = 9;
    p->id = actor->netId;
    p->shields.front = front;
    p->shields.back = back;
}

/*
 * ADJUSTSHIELDS for the current actor.
 */
void PacketList::ShieldsCurrent(char front, char back)
{
    Shields((ActorRecord *)g4d2498, front, back);
}

/*
 * SYSTEMTOGGLE
 */
void PacketList::Toggle(ActorRecord *actor, char system)
{
    Packet *p = Add();
    p->type = 10;
    p->id = actor->netId;
    p->toggle.system = system;
}

/*
 * SYSTEMTOGGLE for the current actor.
 */
void PacketList::ToggleCurrent(char system)
{
    Toggle((ActorRecord *)g4d2498, system);
}

/*
 * GIVEFOCUS: carries the actor's checksums like CHECKACTOR.
 */
void PacketList::Focus(ActorRecord *actor)
{
    Check(11, actor);
}

/*
 * MOVE: destination cell.
 */
void PacketList::Move(ActorRecord *actor, int x, int y)
{
    Packet *p = Add();
    p->type = 12;
    p->id = actor->netId;
    p->move.x = x;
    p->move.y = y;
}

/*
 * DEADPILOT
 */
void PacketList::DeadPilot(ActorRecord *actor)
{
    Packet *p = Add();
    p->type = 20;
    p->id = actor->netId;
}

/*
 * DEADACTOR
 */
void PacketList::DeadActor(ActorRecord *actor)
{
    Packet *p = Add();
    p->type = 19;
    p->id = actor->netId;
}

/*
 * FIRE: target, weapon, random seed and aim of one shot.
 */
void PacketList::Fire(ActorRecord *actor, int target, int weapon, int seed, int aim)
{
    Packet *p = Add();
    p->type = 7;
    p->id = actor->netId;
    p->fire.seed = seed;
    p->fire.target = target;
    p->fire.weapon = weapon;
    p->fire.aim = aim;
}

/*
 * RTNFIRE
 */
void PacketList::ReturnFire(ActorRecord *actor, char a, char b, char c)
{
    Packet *p = Add();
    p->type = 8;
    p->id = actor->netId;
    p->rtnfire.a = a;
    p->rtnfire.b = b;
    p->rtnfire.c = c;
}

/*
 * ENDTURN: tells the other players, runs the diagnose handshake and sends the
 * turn end with the current random seed.
 */
void PacketList::EndTurn(int arg)
{
    MenuMessage m(0x1006);
    MenuSend(&m, 0);
    Diagnose();
    Packet *p = Add();
    p->type = 13;
    p->id = -1;
    p->turn.seed = ReadRandom();
    p->turn.arg = arg;
    Send(0);
}

/*
 * ENSURECYBRIDENDTURN: flushes pending packets first and loops the packet back.
 */
void PacketList::EnsureCybridEndTurn()
{
    if (count != 0)
        Send(0);
    Packet *p = Add();
    p->type = 22;
    p->id = -1;
    Send(1);
}

/*
 * BEGINDIAGNOSE/ENDDIAGNOSE handshake: every machine compares its actors with
 * the CHECKACTOR packets, then all players wait for each other and DOSYNCH.
 */
void PacketList::Diagnose()
{
    ASSERT(ModeCheck(modeCheckName));
    if (leaderPlayer == gRemoteActorKind && !ActorFindByOwner((ActorRecord *)gRemoteActorKind))
        return;
    ModeBeginDiagnose();
    gMenuNetwork.Resume();
    Packet *p = Add();
    p->type = 15;
    p->id = -1;
    p->turn.seed = 0;
    p = Add();
    p->type = 16;
    p->id = -1;
    p->turn.arg = ++gDiagnoseSerial;
    Send(0);
    gMenuNetwork.ResetReady(gDiagnoseSerial);
    while (!gMenuNetwork.AllReady() && gMenuNetwork.Poll()) {
        Sleep(100);
        GuiPumpMessages();
        NetworkPoll();
    }
    if (gMenuNetwork.IsRunning())
        return;
    p = Add();
    p->type = 21;
    p->id = -1;
    p->turn.arg = 99;
    Send(0);
    gMenuNetwork.Synchronize(gLocalActorKind, gLocalActorKind, 99);
}

void PacketList::Check(int type, ActorRecord *actor)
{
    Packet *p = Add();
    p->type = type;
    p->id = actor->netId;
    p->check.block = BlockChecksum(actor->payload);
    p->check.extra = PilotChecksum(actor->pilot);
    p->check.actor = ActorChecksum(actor);
}
