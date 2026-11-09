unsigned char chat_data[0x60000];
#define CHAT_WORD(address) (*(unsigned int *)(chat_data + ((address) - 0x4a0000)))
extern const char chat_wildcard[];
extern const char reset_chat_wildcard[];
extern const char chat_file_path[];
extern char targetBriefResource[];
extern const char chat_player_state_table[];

extern "C" {
__declspec(dllimport) void __stdcall InitializeCriticalSection(void *);
void *memset(void *, int, unsigned int);
char *strcpy(char *, const char *);
int close_chat_handle(unsigned int);
void core_chat(void *);
}

void LogicTextAppend(void *, const char *);

void *initialize_chat(void *self)
{
    InitializeCriticalSection(chat_data + (0x4f64a0 - 0x4a0000));
    *(unsigned int *)((char *)self + 0x3d76) = 0;
    return self;
}

int reset_chat_history(void *self)
{
    memset(self, 0, 0x3d7a);
    *(unsigned char *)self = 1;
    CHAT_WORD(0x4f64b8) = 0;
    strcpy((char *)self + 0x262 + (*(unsigned int *)((char *)self + 0x3d62)) * 0x64, reset_chat_wildcard);
    return 1;
}

void clear_chat(void *self)
{
    memset(self, 0, 0x3d7a);
    *(unsigned char *)self = 1;
    CHAT_WORD(0x4f64b8) = 0;
}

void flush_chat_log(void)
{
    if (CHAT_WORD(0x4f64b8) != 0) {
        CHAT_WORD(0x4f64b8) = 0;
        LogicTextAppend((char *)chat_data + (0x4dd0c0 - 0x4a0000),
                        (char *)chat_data + (0x4f64bc - 0x4a0000));
    }
}

void set_chat_mode(void *self, unsigned int mode, const char *text)
{
    memset(self, 0, 0xa);
    *((unsigned char *)self + mode) = 1;
    strcpy((char *)self + 0x3cfa, text);
    core_chat(self);
}

int chat_ready(void)
{
    return 1;
}

extern "C" {
__declspec(dllimport) void __stdcall EnterCriticalSection(void *);
__declspec(dllimport) void __stdcall LeaveCriticalSection(void *);
char *strcat(char *, const char *);
unsigned int strlen(const char *);
int sprintf(char *, const char *, ...);
void parse_chat_text(unsigned int *, unsigned int, const char *);
int chat_entry_name(void *, int);
void append_chat_name(unsigned int *, int);
}

class MenuNetwork { public: int IsRemote(int); };
extern MenuNetwork gMenuNetwork;

void build_chat_menu_line(void *, unsigned int, const char *, unsigned int = 0xffffffff);
void *create_chat_popup(void *, int);
void check_chat_menu(void *);
void enable_chat_menu(void *, int, int, int);
int chat_menu_state(void *, int, int);
int chat_menu_valid(void *);
void set_chat_menu_item(void *, int, int);
void refresh_chat_selection(void *);
struct ChatMenuOwner {
    unsigned char prefix[0x3d76];
    void *menu;
};
void update_chat_menu(void *);
void *AllocateMissionImage(const char *, int, int = 0);
int chat_suppressed(void);
int chat_log_open(void);
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
// source-line padding for the shared assertion
#define CHAT_LENGTH_ASSERT(s) do { if (strlen(s) >= 100) AssertFail(chat_file_path, 140, 0); } while (0)

int process_chat(void *self)
{
    unsigned int local_text[0x77];
    register unsigned int *parsed_names = local_text;
    char *input_text = (char *)self + 0x3cfa;
    int i;
    register char *speaker_name;
    unsigned int name_id;
    EnterCriticalSection(chat_data + (0x4f64a0 - 0x4a0000));
    *((unsigned char *)self + 0x3d5d) = 0;
    parse_chat_text(local_text, 0x1001, input_text);
    if (*(char *)self == 0) {
        for (i = 1; i < 9; ++i) {
            if (*((char *)self + i) != 0 &&
                gMenuNetwork.IsRemote(i) != 0) {
                name_id = chat_entry_name((void *)0x4f6624, i);
                append_chat_name(parsed_names, name_id);
            }
        }
    } else {
        append_chat_name(parsed_names, 0);
    }
    speaker_name = (char *)chat_data + (0x4f6a14 - 0x4a0000) +
                   CHAT_WORD(0x4f6624) * 0xbc + 0x10;
    CHAT_LENGTH_ASSERT(input_text);
    *(unsigned int *)((char *)self + 10 +
        *(unsigned int *)((char *)self + 0x3d62) * 4) =
        (unsigned int)speaker_name;
    strcpy((char *)self + 0x262 +
        *(unsigned int *)((char *)self + 0x3d62) * 100,
        input_text);
    i = *(unsigned int *)((char *)self + 0x3d62) + 1;
    *(unsigned int *)((char *)self + 0x3d62) = i;
    if (i >= 0x96)
        *(unsigned int *)((char *)self + 0x3d62) = 0;
    strcpy((char *)self + 0x262 +
        *(unsigned int *)((char *)self + 0x3d62) * 100, chat_wildcard);
    *input_text = 0;
    LeaveCriticalSection(chat_data + (0x4f64a0 - 0x4a0000));
    return 1;
}

int add_chat_message(void *self, void *name, const char *text)
{
    int row;
    EnterCriticalSection(chat_data + (0x4f64a0 - 0x4a0000));
    register const char *source_text = text;
    register void *entry_name = name;
    CHAT_LENGTH_ASSERT(source_text);
    *(unsigned int *)((char *)self + 10 +
        *(unsigned int *)((char *)self + 0x3d62) * 4) =
        (unsigned int)entry_name;
    strcpy((char *)self + 0x262 +
        *(unsigned int *)((char *)self + 0x3d62) * 100, source_text);
    row = *(unsigned int *)((char *)self + 0x3d62) + 1;
    *(unsigned int *)((char *)self + 0x3d62) = row;
    if (row >= 0x96)
        *(unsigned int *)((char *)self + 0x3d62) = 0;
    strcpy((char *)self + 0x262 +
        *(unsigned int *)((char *)self + 0x3d62) * 100, chat_wildcard);
    if (chat_suppressed() == 0 && chat_log_open() != 0) {
        CHAT_WORD(0x4f64b8) = 1;
        sprintf((char *)chat_data + (0x4f64bc - 0x4a0000),
                "[%s]: %s", source_text, source_text);
    }
    LeaveCriticalSection(chat_data + (0x4f64a0 - 0x4a0000));
    return 1;
}

struct ChatHistoryView {
    unsigned char flags[10];
    void *names[150];
    char messages[150][100];
    unsigned char gap[0x3d62 - (0x262 + 150 * 100)];
    int current;
};

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
    int index = 1;
    int state;
    while (index < 9) {
        state = *(int *)(chat_data + (0x4f6a14 - 0x4a0000) + index * 0xbc);
        if (state == 1 && state == 2) {
            index += state;
            continue;
        } else {
            refresh_chat_selection(*(void **)((char *)self + 0x3d76));
            update_chat_menu(self);
            return;
        }
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
