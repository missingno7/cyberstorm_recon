unsigned char link_data[0x60000];
#define DATA_WORD(address) (*(unsigned int *)(link_data + ((address) - 0x4a0000)))
extern "C" {
void *memset(void *, int, unsigned int);
__declspec(dllimport) unsigned int __stdcall GetTickCount(void);
__declspec(dllimport) void __stdcall Sleep(unsigned int);
__declspec(dllimport) unsigned int __stdcall WaitForSingleObject(void *, unsigned int);
__declspec(dllimport) int __stdcall MessageBeep(unsigned int);
__declspec(dllimport) int __stdcall FlashWindow(void *, int);
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
void initialize_link_packet(unsigned int *);
int receive_link_packet(int *, int *, unsigned int *, int, int);
void expire_link_session(void);
void deliver_link_packet(void *, unsigned int *);
struct LinkEntry { char pad[8]; void *name; };
struct Queue {
    void *head;
    void *tail;
    Queue();
    void Lock(void);
    int IsEmpty(void);
    LinkEntry *First(void);
    void Remove(LinkEntry *);
    void Unlock(void);
};
extern Queue gReceiveQueue;
#define LINK_QUEUE (&gReceiveQueue)
inline void *operator new(unsigned int, void *where) { return where; }
void BriefModuleConstruct(void *);
void PopConstructorArray(void *, int, void *);
extern unsigned char timer_constructor_table;

void initialize_linkapp_globals(void)
{
    BriefModuleConstruct(link_data + (0x4a3f00 - 0x4a0000));
    new ((Queue *)0x4f5d50) Queue;
    DATA_WORD(0x4a3f08) = 1;
    new ((Queue *)0x4f5d58) Queue;
    DATA_WORD(0x4a3f08) = 2;
    PopConstructorArray(link_data + (0x4f5d60 - 0x4a0000), 0x21,
                        &timer_constructor_table);
}

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
        register unsigned int active = DATA_WORD(0x4f5cdc);
        if (active == 0)
            return;
        process_link_events();
        if (DATA_WORD(0x4f5ce4) != 0) {
            Sleep(150);
            continue;
        }
        if (WaitForSingleObject((void *)DATA_WORD(0x4f5d30), 0xffffffff) == 0x80)
            return;
    }
}

void clear_link_entries(void)
{
    DATA_WORD(0x4f5d4c) = 0;
    memset(link_data + (0x4f5d60 - 0x4a0000), 0, 0x39c);
}

int retry_link_timer(int index)
{
    int active_player = DATA_WORD(0x4f5cf4);
    if (active_player == 0 || index == active_player)
        return 1;
    int timer_offset = index * 28;
    if (TimerExpired((Timer *)(link_data + (0x4f5d60 - 0x4a0000) + timer_offset)) == 0)
        return 1;
    int old_count = DATA_WORD(0x4f5d6c + timer_offset);
    DATA_WORD(0x4f5d6c + timer_offset) += 1;
    if (old_count >= 15)
        return 0;
    if ((int)DATA_WORD(0x4f5d6c + timer_offset) * 2 <= 15)
        goto fast_retry;
    MessageSendPing(index);
    goto retry_done;
fast_retry:
    MessageRequestStatus(index);
retry_done:
    int delay = 2000;
    if (DATA_WORD(0x4f5ce4) != 0)
        delay = 3000;
    unsigned int random_value = next_link_seed((unsigned int *)(link_data + (0x4d3e04 - 0x4a0000)));
    TimerSet((Timer *)(link_data + (0x4f5d60 - 0x4a0000) + index * 28),
             (random_value & 0x3ff) + delay);
    return 1;
}

void set_link_timer(int index)
{
    int delay = 2000;
    if (DATA_WORD(0x4f5ce4) != 0)
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
    unsigned int timeout = (next_link_seed((unsigned int *)(link_data + (0x4d3e04 - 0x4a0000))) & 0x3ff) + delay;
    TimerSet((Timer *)(link_data + (0x4f5d60 - 0x4a0000) + index * 28), timeout);
    *(unsigned int *)(link_data + (0x4f5d6c - 0x4a0000) + index * 28) = 0;
}

void process_link_events(void)
{
    char packet[0x1da];
    int player;
    int event_id;
    int skip_delivery;
    initialize_link_packet((unsigned int *)packet);
    for (;;) {
parse_packet:
        do {
            if (receive_link_packet(&event_id, &player, (unsigned int *)packet, sizeof(packet), 1) == 0)
                goto no_packet;
        } while (player != (int)DATA_WORD(0x4f5cf4) && player != 0);
        if (MessageValid((MenuMessage *)packet) != 0)
            goto check_busy;
skip_packet:
        skip_delivery = 1;
        goto check_delivery;
check_busy:
        skip_delivery = IsSystemMessage((MenuMessage *)packet);
        if (skip_delivery != 0) {
            MessageDispatch((MenuMessage *)packet, 0x1025);
            goto skip_packet;
        }
check_delivery:
        if (skip_delivery != 0)
            goto parse_packet;
        // Source-line padding preserves the original AssertFail line number.
        // Source-line padding preserves the original AssertFail line number.
        if (*(int *)(packet + 0x14) == 0)
            AssertFail(__FILE__, __LINE__, 0);
        deliver_link_packet(link_data + (0x4fb32c - 0x4a0000), (unsigned int *)packet);
    }
no_packet:
    {
        unsigned int elapsed = (GetTickCount() - DATA_WORD(0x4f6100)) / 1000;
        if (elapsed > 10) {
            if (DATA_WORD(0x4f6104) + 3 <= elapsed) {
                MessageBeep(0);
                FlashWindow(*(void * volatile *)0x51253c, 0);
                MessageBeep(0);
                DATA_WORD(0x4f6104) = elapsed;
            }
            if (elapsed > 14)
                expire_link_session();
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
    DATA_WORD(0x4f6100) = GetTickCount();
    DATA_WORD(0x4f6104) = 0;
}

void drain_link_queue(void)
{
    register int has_entries;
    LinkEntry *entry;
    int cancelled;
    LINK_QUEUE->Lock();
    do {
        has_entries = LINK_QUEUE->IsEmpty() == 0;
        do {
            if (has_entries == 0) {
                LINK_QUEUE->Unlock();
                DATA_WORD(0x4f6100) = GetTickCount();
                DATA_WORD(0x4f6104) = 0;
                return;
            }
            entry = LINK_QUEUE->First();
            cancelled = MessageDispatch((MenuMessage *)entry->name, 0x1025);
            has_entries = 0;
        } while (cancelled == 0);
        LINK_QUEUE->Remove(entry);
    } while (1);
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
