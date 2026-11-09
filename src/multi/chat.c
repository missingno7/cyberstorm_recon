#include <windows.h>
extern char dialogOpenData[], g4dd0c0[]; int chatLogPending; char chatLogText[256];

extern const char chat_wildcard[];
extern const char reset_chat_wildcard[];
extern const char chat_file_path[];
extern char targetBriefResource[];
extern const char chat_player_state_table[];

extern "C" {

void *memset(void *, int, unsigned int);
char *strcpy(char *, const char *);
int close_chat_handle(unsigned int);
void core_chat(void *);
}

void LogicTextAppend(void *, const char *);

void *initialize_chat(void *self)
{
    InitializeCriticalSection((CRITICAL_SECTION *)dialogOpenData);
    *(unsigned int *)((char *)self + 0x3d76) = 0;
    return self;
}

int reset_chat_history(void *self)
{
    memset(self, 0, 0x3d7a);
    *(unsigned char *)self = 1;
    chatLogPending = 0;
    strcpy((char *)self + 0x262 + (*(unsigned int *)((char *)self + 0x3d62)) * 0x64, reset_chat_wildcard);
    return 1;
}

void clear_chat(void *self)
{
    memset(self, 0, 0x3d7a);
    *(unsigned char *)self = 1;
    chatLogPending = 0;
}







extern "C" {


char *strcat(char *, const char *);
unsigned int strlen(const char *);
int sprintf(char *, const char *, ...);



}

struct ChatPlayerState {int state; char rest[184];}; class MenuNetwork { public: int localSlot; int IsRemote(int); int PeerForSlot(int); };
struct MenuMessage { char bytes[0x1da]; MenuMessage(unsigned int,const char *); }; int MenuSend(MenuMessage *,int);
extern MenuNetwork gMenuNetwork;

void build_chat_menu_line(void *, unsigned int, const char *, unsigned int = 0xffffffff);
void *create_chat_popup(void *, int);
void check_chat_menu(void *);
void enable_chat_menu(void *, int, int, int);
int chat_menu_state(void *, int, int);
int chat_menu_valid(void *);
void set_chat_menu_item(void *, int, int);
int MenuClear(void *);
struct ChatMenuOwner {
    unsigned char prefix[0x3d76];
    void *menu;
};
void update_chat_menu(void *);
void *AllocateMissionImage(const char *, int, int = 0);
extern int g4faf40; inline int chat_suppressed(void) {return g4faf40;}
struct ModeSystem {int active; int IsActive(){return active;}}; extern ModeSystem modeCheckName; inline int chat_log_open(void) {return modeCheckName.IsActive();}
void AssertFail(const char *, int, const char *);

// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// source-line padding for the shared assertion
// Preserve the source location of the shared length assertion.
// Preserve the source location of the shared length assertion.
// Preserve the source location of the shared length assertion.
// Preserve the source location of the shared length assertion.
// Preserve the source location of the shared length assertion.
// Preserve the source location of the shared length assertion.
// Preserve the source location of the shared length assertion.
// Preserve the source location of the shared length assertion.
// Preserve the source location of the shared length assertion.
// Preserve the source location of the shared length assertion.
// Preserve the source location of the shared length assertion.
// Preserve the source location of the shared length assertion.
// Preserve the source location of the shared length assertion.
// Preserve the source location of the shared length assertion.
// Preserve the source location of the shared length assertion.
#define CHAT_LENGTH_ASSERT(s) do { if (strlen(s) >= 100) AssertFail(chat_file_path, 140, 0); } while (0)

struct ChatHistoryView {
    unsigned char flags[10];
    void *names[150];
    char messages[150][100];
    unsigned char gap[0x3d62 - (0x262 + 150 * 100)];
    int current; void Append(void *,const char *);
};

inline void ChatHistoryView::Append(void *name, const char *text)
{

    CHAT_LENGTH_ASSERT(text);
    names[current] = name;
    strcpy(messages[current], text);
    ++current;
    if (current >= 0x96)
        current = 0;
    strcpy(messages[current], chat_wildcard);
}

int process_chat(void *self)
{



    int i;
    register char *speaker_name;
    unsigned int name_id;
    EnterCriticalSection((CRITICAL_SECTION *)dialogOpenData);
    *((unsigned char *)self + 0x3d5d) = 0;
    MenuMessage local_text(0x1001, (char *)self + 0x3cfa);
    if (*(char *)self) {
        MenuSend(&local_text, 0);
    } else {
        for (i = 1; i < 9; ++i) {
            if (*((char *)self + i) && gMenuNetwork.IsRemote(i)) {
                name_id = gMenuNetwork.PeerForSlot(i);
                MenuSend(&local_text, name_id);
            }
        }
    }
    speaker_name = (char *)chat_player_state_table +
                   gMenuNetwork.localSlot * 0xbc + 0x10;
    ((ChatHistoryView*)self)->Append(speaker_name, (char *)self + 0x3cfa);
    *((char *)self + 0x3cfa) = 0;
    LeaveCriticalSection((CRITICAL_SECTION *)dialogOpenData);
    return 1;
}

int add_chat_message(void *self, void *name, const char *text)
{
    int row;
    EnterCriticalSection((CRITICAL_SECTION *)dialogOpenData);


    ((ChatHistoryView*)self)->Append(name, text);
    if (g4faf40 == 0 && chat_log_open() != 0) {
        chatLogPending = 1;
        sprintf(chatLogText,
                "[%s]: %s", name, text);
    }
    LeaveCriticalSection((CRITICAL_SECTION *)dialogOpenData);
    return 1;
}

void flush_chat_log(void)
{
    if (chatLogPending != 0) {
        chatLogPending = 0;
        LogicTextAppend(g4dd0c0,
                        chatLogText);
    }
}

void set_chat_mode(void *self, unsigned int mode, const char *text)
{
    memset(self, 0, 0xa);
    *((unsigned char *)self + mode) = 1;
    strcpy((char *)self + 0x3cfa, text);
    core_chat(self);
}

void append_chat_history(void *self, void *name, const char *text)
{
    ChatHistoryView *history = (ChatHistoryView *)self;
    CHAT_LENGTH_ASSERT(text);
    history->names[history->current] = name;
    strcpy(history->messages[history->current], text);
    ++history->current;
    if (history->current >= 0x96)
        history->current = 0;
    strcpy(history->messages[history->current], chat_wildcard);
}

int chat_ready(void)
{
    return 1;
}

void create_chat_menu(void *self)
{
    int popup_id = -1;
    ((ChatMenuOwner *)self)->menu =
        create_chat_popup(AllocateMissionImage(targetBriefResource, 0x57e), popup_id);
    update_chat_menu(self);
}

void update_chat_menu(void *self)
{
    int i;
    if (*(void **)((char *)self + 0x3d76) != 0) {
        register int first_item = 23000;
        build_chat_menu_line(*(void **)((char *)self + 0x3d76), first_item,
            (const char *)AllocateMissionImage(targetBriefResource, 0x57f));
        build_chat_menu_line(*(void **)((char *)self + 0x3d76), 0, 0);
        for (i = 1; i < 9; ++i) {
            if (gMenuNetwork.IsRemote(i) != 0)
                build_chat_menu_line(*(void **)((char *)self + 0x3d76), first_item + i,
                    chat_player_state_table + i * 0xbc + 0x10);
        }
        build_chat_menu_line(*(void **)((char *)self + 0x3d76), 0, 0);
        build_chat_menu_line(*(void **)((char *)self + 0x3d76), 0x59e5,
            (const char *)AllocateMissionImage(targetBriefResource, 0x580));
        memset(self, 0, 10);
        check_chat_menu(*(void **)((char *)self + 0x3d76));
        *(unsigned char *)self = 1;
        enable_chat_menu(*(void **)((char *)self + 0x3d76), first_item, 1, 3);
    }
}

void destroy_chat_menu(void *self)
{
    if (*(unsigned int *)((char *)self + 0x3d76) != 0) {
        int valid = chat_menu_valid(*(void **)((char *)self + 0x3d76));
        if (valid != 0)
            *(unsigned int *)((char *)self + 0x3d76) = 0;
    }
}

void handle_chat_menu_command(void *self, unsigned int item)
{
    if (item < 23000 || item > 0x59e5 ||
        *(void **)((char *)self + 0x3d76) == 0 || item == 0x59e5)
        return;
    unsigned int item_offset = item - 23000;
    int state = chat_menu_state(*(void **)((char *)self + 0x3d76), item, 3);
    if (item_offset == 0) {
        if (state == 0) {
            memset(self, 0, 10);
            check_chat_menu(*(void **)((char *)self + 0x3d76));
            *(unsigned char *)self = 1;
            enable_chat_menu(*(void **)((char *)self + 0x3d76), item, 1, 3);
            return;
        }
    } else {
        *(unsigned char *)self = 0;
        enable_chat_menu(*(void **)((char *)self + 0x3d76), 23000, 0, 3);
        set_chat_menu_item(*(void **)((char *)self + 0x3d76), item, 3);
        *((unsigned char *)self + item_offset) ^= 1;
        return;
    }
}

void refresh_chat_menu(void *self)
{
    for (int index=1; index<9; ++index) {
        if (((ChatPlayerState *)chat_player_state_table)[index].state == 1 &&
            ((ChatPlayerState *)chat_player_state_table)[index].state == 2)
            continue;
        MenuClear(*(void **)((char *)self + 0x3d76));
        update_chat_menu(self);
        break;
    }
}

char *format_chat_entry(void *self, char *destination, int age)
{
    int row;
    if (age < 0)



        AssertFail(chat_file_path, __LINE__, 0);
    row = *(int *)((char *)self + 0x3d62) - 1 - age;
    if (row < 0)
        row = 0x96 - row;
    strcpy(destination, "[");
    strcat(destination, *(char **)((char *)self + 10 + row * 4));
    strcat(destination, "]");
    strcat(destination, (char *)self + 0x262 + row * 100);
    return destination;
}


