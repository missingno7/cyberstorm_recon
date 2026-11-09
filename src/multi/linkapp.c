#include <windows.h>
extern int networkEnabled, gLinkSigsMode, localPeer, multiState; extern void *gLinkEvent; extern unsigned int gFxSeed; extern char networkQueue[]; extern HWND volatile g_mainWindow; extern int linkTimerReserved; // Four BSS bytes between the timer array and the clocks.
extern unsigned int linkLastTick, linkLastAlert;

extern "C" {
void *memset(void *, int, unsigned int);





}
struct MenuMessage;
int MessageValid(MenuMessage *);
int IsSystemMessage(MenuMessage *);
int MessageDispatch(MenuMessage *, int);
void MessageSendPing(int);
void MessageRequestStatus(int);
MenuMessage *MenuMessageBasic(MenuMessage *, int);
int MenuSend(MenuMessage *, int);

void process_link_events(void);
struct Timer;
void TimerSet(Timer *, unsigned long);
int TimerExpired(Timer *);
void AssertFail(const char *, int, const char *);
extern "C" unsigned int next_link_seed(unsigned int *);
void MenuMessageNull(MenuMessage *);
int receive_link_packet(int *, int *, unsigned int *, int, int);
void PostCornerClick(void);
void QueueReceive(void *, unsigned int *);
struct LinkEntry { char pad[8]; void *name; void *Name() {return name;} };
struct Queue {
    void *head;
    void *tail;
    Queue(); ~Queue();
    void Lock(void);
    int IsEmpty(void); int HasEntries() {return IsEmpty()==0;}
    LinkEntry *First(void);
    void Remove(LinkEntry *);
    void Unlock(void);
};
extern Queue gReceiveQueue;
#define LINK_QUEUE (&gReceiveQueue)
struct Timer { unsigned long start,end,duration; };
struct LinkPeerTimer { Timer timer; int attempts; int reserved[3]; LinkPeerTimer(); ~LinkPeerTimer() {} };
Queue gReceiveQueue, linkSendQueue;
LinkPeerTimer linkPeerTimers[33];
int linkTimerReserved; // Four BSS bytes between the timer array and the clocks.
unsigned int linkLastTick, linkLastAlert;

int wait_for_name_release(void *name)
{
    if (MessageValid((MenuMessage *)name) != 0)
        goto check_busy;
return_one:
    return 1;
check_busy:
    if (IsSystemMessage((MenuMessage *)name) == 0)
        goto return_zero;
    MessageDispatch((MenuMessage *)name, 0x1025);
    goto return_one;
return_zero:
    return 0;
}

void wait_for_link_shutdown(void)
{
    for (;;) {
        register unsigned int active = networkEnabled;
        if (active == 0)
            return;
        process_link_events();
        if (gLinkSigsMode != 0) {
            Sleep(150);
            continue;
        }
        if (WaitForSingleObject((void *)gLinkEvent, 0xffffffff) == 0x80)
            return;
    }
}

void clear_link_entries(void)
{
    multiState = 0;
    memset((char *)linkPeerTimers, 0, 0x39c);
}

int retry_link_timer(int index)
{
    int active_player = localPeer;
    if (active_player == 0 || index == active_player)
        return 1;
    int timer_offset = index * 28;
    if (TimerExpired((Timer *)((char *)linkPeerTimers + timer_offset)) == 0)
        return 1;
    int old_count = (*(unsigned int *)((char *)linkPeerTimers + 12 + timer_offset));
    (*(unsigned int *)((char *)linkPeerTimers + 12 + timer_offset)) += 1;
    if (old_count >= 15)
        return 0;
    if ((int)(*(unsigned int *)((char *)linkPeerTimers + 12 + timer_offset)) * 2 <= 15)
        goto fast_retry;
    MessageSendPing(index);
    goto retry_done;
fast_retry:
    MessageRequestStatus(index);
retry_done:
    int delay = 2000;
    if (gLinkSigsMode != 0)
        delay = 3000;
    unsigned int random_value = next_link_seed((unsigned int *)((char *)&gFxSeed));
    TimerSet((Timer *)((char *)linkPeerTimers + index * 28),
             (random_value & 0x3ff) + delay);
    return 1;
}

void set_link_timer(int index)
{
    int delay = 2000;
    if (gLinkSigsMode != 0)
        delay = 3000;
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.
    // Source-line padding preserves the original AssertFail line number.







    if (index > 32)
        AssertFail(__FILE__, __LINE__, 0);
    if (index < 1)
        return;
    unsigned int timeout = (next_link_seed((unsigned int *)((char *)&gFxSeed)) & 0x3ff) + delay;
    TimerSet((Timer *)((char *)linkPeerTimers + index * 28), timeout);
    *(unsigned int *)((char *)linkPeerTimers + 12 + index * 28) = 0;
}

void process_link_events(void)
{
    char packet[0x1da];
    int player;
    int event_id;
    // The validation helper is inlined by this object's -oe profile.
    MenuMessageNull((MenuMessage *)packet);
    for (;;) {
parse_packet:
        do {
            if (receive_link_packet(&event_id, &player, (unsigned int *)packet, sizeof(packet), 1) == 0)
                goto no_packet;
        } while (player != (int)localPeer && player != 0);
        if (wait_for_name_release(packet)) goto parse_packet;
        // Source-line padding preserves the original AssertFail line number.
        // Source-line padding preserves the original AssertFail line number.













        if (*(int *)(packet + 0x14) == 0)
            AssertFail(__FILE__, __LINE__, 0);
        QueueReceive(networkQueue, (unsigned int *)packet);
    }
no_packet:
    {
        unsigned int elapsed = (GetTickCount() - linkLastTick) / 1000;
        if (elapsed > 10) {
            if (elapsed >= linkLastAlert + 3) {
                MessageBeep(0);
                FlashWindow(g_mainWindow, 0);
                MessageBeep(0);
                linkLastAlert = elapsed;
            }
            if (elapsed >= 15)
                PostCornerClick();
        }
    }
}

void format_link_name(int destination, int source)
{
    char buffer[0x1dc];
    MenuMessageBasic((MenuMessage *)buffer, source);
    MenuSend((MenuMessage *)buffer, destination);
}

void save_link_tick(void)
{
    linkLastTick = GetTickCount();
    linkLastAlert = 0;
}

void drain_link_queue(void)
{
    LINK_QUEUE->Lock();
    int ready=LINK_QUEUE->HasEntries();
    while (ready) {
        LinkEntry *entry=LINK_QUEUE->First();
        void *name=entry->Name();
        ready=MessageDispatch((MenuMessage *)name,0x1025);
        if (ready) {
            LINK_QUEUE->Remove(entry);
            ready=LINK_QUEUE->HasEntries();
        }
    }
    LINK_QUEUE->Unlock();
    save_link_tick();
}

int cancel_current_link_entry(unsigned int reason)
{
    LinkEntry *entry;
    int cancelled = 0;
    LINK_QUEUE->Lock();
    if (LINK_QUEUE->IsEmpty() == 0) {
        entry = LINK_QUEUE->First();
        void *name = entry->name;
        cancelled = MessageDispatch((MenuMessage *)name, reason);
        if (cancelled != 0)
            LINK_QUEUE->Remove(entry);
    }
    LINK_QUEUE->Unlock();
    return cancelled;
}


