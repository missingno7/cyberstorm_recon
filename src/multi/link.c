unsigned char link_data[0x60000];
#define DATA_WORD(address) (*(unsigned int *)(link_data + ((address) - 0x4a0000)))

struct Timer;
void TimerSet(Timer *, unsigned long);
extern "C" void *memset(void *, int, unsigned int);
void alternate_link_disconnect(void);
extern "C" {
__declspec(dllimport) void *__stdcall LoadLibraryA(const char *);
__declspec(dllimport) int __stdcall FreeLibrary(void *);
__declspec(dllimport) void *__stdcall GetProcAddress(void *, const char *);
__declspec(dllimport) void *__stdcall CreateEventA(void *, int, int, const char *);
__declspec(dllimport) int __stdcall CloseHandle(void *);
__declspec(dllimport) int __stdcall TerminateThread(void *, unsigned int);
__declspec(dllimport) void __stdcall Sleep(unsigned int);
__declspec(dllimport) int __stdcall PostMessageA(void *, unsigned int, unsigned int, int);
}
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
extern void *g_mainWindow;
void AssertFail(const char *, int, const char *);
int link_is_ready(const char *);
void link_force_shutdown(void);
void close_link_connection(void);
extern void *link_thread_proc_anchor;
void open_link_settings(const char *);
void link_load_settings(void *);
void run_mission_image(void *);
void *AllocateMissionImage(unsigned int, int, int);
typedef unsigned int (__stdcall *LinkThreadStart)(void *);
typedef unsigned long DWORD;
typedef void *HANDLE;
HANDLE LinkBeginThread(void *, unsigned int, LinkThreadStart, void *,
                       unsigned int, unsigned int, DWORD *);
int link_alt_send(int, int, int, int, int);
int link_alt_receive(void *, void *, int, int, int);
int link_alt_disconnect(void);
int link_alt_set_options(void *, void *);

struct LinkInterfaceVTable {
    void *methods[10];
    int (__stdcall *disconnect)(void *, int);
    void *methods_2c[4];
    int (__stdcall *set_options)(void *, void *);
    void *methods_40[5];
    int (__stdcall *receive)(void *, void *, void *, void *, void *, void *);
    void (__stdcall *get_last_dialed)(void *, const char *);
    int (__stdcall *send)(void *, int, int, int, int, int);
};
struct LinkInterface { LinkInterfaceVTable *vtable; };

struct LinkDecoderState {
    unsigned char *source;
    unsigned char *destination;
    unsigned char pad_08[8];
    unsigned int sample_number;
    int predictor;
    int difference;
    unsigned short encoded;
    unsigned short nibble;
    unsigned short step;
    unsigned short step_index;
    unsigned short format;
};

struct LinkDecodeParts { unsigned int low, high; };
union LinkDecodeResult { LinkDecodeParts parts; double value; };
struct LinkDecoderOwner {
    LinkDecodeResult __fastcall decode_link_samples(unsigned int, LinkDecoderState *, unsigned int);
};

LinkDecodeResult __fastcall LinkDecoderOwner::decode_link_samples(
    unsigned int return_high, LinkDecoderState *state, unsigned int sample_count)
{
    unsigned char *source = state->source;
    unsigned char *destination = state->destination;
    unsigned int scaled_count = sample_count;
    LinkDecodeResult result;
    DATA_WORD(0x4a3ee6) = sample_count;
    if ((short)state->format == 0x10)
        scaled_count >>= 1;
    DATA_WORD(0x4a3ee2) = scaled_count;
    do {
        unsigned int nibble;
        if ((state->sample_number & 1) == 0) {
            state->encoded = *source;
            source++;
            nibble = state->encoded & 0xf;
            state->nibble = (unsigned short)nibble;
        } else {
            nibble = (state->encoded >> 4) & 0xf;
            state->nibble = (unsigned short)nibble;
        }
        state->difference = 0;
        if (nibble & 4)
            state->difference += state->step;
        if (nibble & 2)
            state->difference += state->step >> 1;
        if (nibble & 1)
            state->difference += state->step >> 2;
        state->difference += state->step >> 3;
        if (nibble & 8)
            state->difference = -state->difference;
        state->predictor += state->difference;
        if (state->predictor >= 0x7fff)
            state->predictor = 0x7fff;
        if (state->predictor <= -0x8000)
            state->predictor = -0x8000;
        if ((short)state->format == 0x10) {
            *(short *)destination = (short)state->predictor;
            destination += 2;
        } else {
            *destination = (unsigned char)(state->predictor >> 8) ^ 0x80;
            destination++;
        }
        state->step_index = (unsigned short)(state->step_index +
            *(unsigned short *)(link_data + (0x4a3e10 - 0x4a0000) + state->nibble * 2));
        if (state->step_index < 0x8000) {
            if (state->step_index > 0x58)
                state->step_index = 0x58;
        } else {
            state->step_index = 0;
        }
        state->sample_number++;
        state->step = *(unsigned short *)(link_data + (0x4a3e30 - 0x4a0000) + state->step_index * 2);
        DATA_WORD(0x4a3ee2)--;
    } while (DATA_WORD(0x4a3ee2) != 0);
    state->source = source;
    state->destination = destination;
    result.parts.low = DATA_WORD(0x4a3ee6);
    result.parts.high = return_high;
    return result;
}

void reset_link_timer(void)
{
    TimerSet((Timer *)(link_data + (0x4f5cfc - 0x4a0000)), 0);
}

int initialize_directplay(void)
{
    void *module;
    if (DATA_WORD(0x4f5cd4) != 0)
        return 1;
    module = LoadLibraryA(snw_valid_dll);
    if (module != 0) {
        DATA_WORD(0x4f5ce8) = 1;
        DATA_WORD(0x4f5cd4) = 1;
        FreeLibrary(module);
    }
    DATA_WORD(0x4f5d40) = (unsigned int)LoadLibraryA(directplay_dll);
    if (DATA_WORD(0x4f5d40) != 0) {
        DATA_WORD(0x4f5d48) = (unsigned int)GetProcAddress(
            (void *)DATA_WORD(0x4f5d40), directplay_enumerate);
        DATA_WORD(0x4f5d44) = (unsigned int)GetProcAddress(
            (void *)DATA_WORD(0x4f5d40), directplay_create);
        if (DATA_WORD(0x4f5d48) == 0 || DATA_WORD(0x4f5d44) == 0)
        // source line padding for the original assertion
        // source line padding for the original assertion
        // source line padding for the original assertion
        // source line padding for the original assertion
        // source line padding for the original assertion
        // source line padding for the original assertion
        // source line padding for the original assertion
        // source line padding for the original assertion
        // source line padding for the original assertion
        // source line padding for the original assertion
        // source line padding for the original assertion
        // source line padding for the original assertion
        // source line padding for the original assertion
        // source line padding for the original assertion
        // source line padding for the original assertion
        // source line padding for the original assertion
            AssertFail((const char *)(link_data + (0x4a3eec - 0x4a0000)), __LINE__, 0);
        DATA_WORD(0x4f5d30) = (unsigned int)CreateEventA(0, 0, 0, directplay_message);
        DATA_WORD(0x4f5cd4) = 1;
    }
    ((StrPool *)resourcePool)->Lock();
    return DATA_WORD(0x4f5cd4);
}

void shutdown_directplay(void)
{
    DATA_WORD(0x4f5cd4) = 0;
    DATA_WORD(0x4f5cf8) = 0;
    if (DATA_WORD(0x4f5ce4) != 0 || DATA_WORD(0x4fb7d4) != 0) {
        link_force_shutdown();
        return;
    }
    if (DATA_WORD(0x4f5cdc) != 0)
        close_link_connection();
    if (DATA_WORD(0x4f5d40) != 0) {
        FreeLibrary((void *)DATA_WORD(0x4f5d40));
        DATA_WORD(0x4f5d40) = 0;
    }
}

int open_link_connection(const char *address)
{
    register LinkInterface *interface;
    int result;
    LinkInterface *alternate_interface;
    const char *current_address = address;
    if (DATA_WORD(0x4f5cdc) == 0 && initialize_directplay() != 0 &&
        (open_link_settings(current_address),
         (DATA_WORD(0x4f5d2c) != 0 || DATA_WORD(0x4f5ce4) != 0))) {
        alternate_interface = (LinkInterface *)DATA_WORD(0x4f5ce4);
        if (alternate_interface == 0) {
            interface = (LinkInterface *)DATA_WORD(0x4f5d2c);
            DATA_WORD(0x4f5cf0) = 1;
            result = ((int (__stdcall *)(void *, unsigned int *, const char *,
                                         const char *, void *))interface->vtable->methods[5])(
                interface, (unsigned int *)(link_data + (0x4f5cf4 - 0x4a0000)),
                (const char *)(link_data + (0x4a3f50 - 0x4a0000)),
                (const char *)(link_data + (0x4a3f40 - 0x4a0000)),
                link_data + (0x4f5d30 - 0x4a0000));
            if (result == 0) {
                DATA_WORD(0x4f5cdc) = 1;
                memset(link_data + (0x4f5d08 - 0x4a0000), 0, 0x1c);
                DATA_WORD(0x4f5d08) = 0x1c;
                interface->vtable->set_options(interface,
                    link_data + (0x4f5d08 - 0x4a0000));
                if (DATA_WORD(0x4f5cec) == 2 && DATA_WORD(0x4f5d1c) < 0x90) {
                    close_link_connection();
                    run_mission_image(AllocateMissionImage(0x511ea4, 6, 0));
                } else {
                    interface->vtable->get_last_dialed(interface, last_dialed_label);
                    DATA_WORD(0x4f5d24) = (unsigned int)LinkBeginThread(
                        0, 0, (LinkThreadStart)link_thread_proc_anchor, 0, 0, 0,
                        (DWORD *)(link_data + (0x4f5d28 - 0x4a0000)));
                    if (DATA_WORD(0x4f5d24) != 0)
                        return 1;
                    close_link_connection();
                }
            } else {
                ((int (__stdcall *)(void *))interface->vtable->methods[4])(interface);
                ((int (__stdcall *)(void *))interface->vtable->methods[2])(interface);
                DATA_WORD(0x4f5d2c) = (unsigned int)alternate_interface;
            }
        } else {
            if (link_is_ready(current_address) == 0)
                return 0;
            if (DATA_WORD(0x4f5cec) == 2)
                Sleep(DATA_WORD(0x4f5cf4) * 300 + 3000);
            link_load_settings(link_data + (0x4f5d08 - 0x4a0000));
            DATA_WORD(0x4f5cdc) = 1;
            DATA_WORD(0x4f5d24) = (unsigned int)LinkBeginThread(
                0, 0, (LinkThreadStart)link_thread_proc_anchor, 0, 0, 0,
                (DWORD *)(link_data + (0x4f5d28 - 0x4a0000)));
            if (DATA_WORD(0x4f5d24) != 0)
                return 1;
            link_force_shutdown();
        }
    }
    return 0;
}

void close_link_connection(void)
{
    register unsigned int zero;
    register unsigned int alternate_state = DATA_WORD(0x4f5ce4);
    register unsigned int thread_handle;
    if (alternate_state == 0 && DATA_WORD(0x4fb7d4) == 0) {
        if (DATA_WORD(0x4f5cdc) != 0) {
            zero = 0;
            DATA_WORD(0x4f5cdc) = alternate_state;
            if (DATA_WORD(0x4f5cf4) != 0)
                ((int (__stdcall *)(void *, unsigned int))
                    ((void **)*(unsigned int **)(link_data + (0x4f5d2c - 0x4a0000)))[8])(
                        (void *)DATA_WORD(0x4f5d2c), DATA_WORD(0x4f5cf4));
            DATA_WORD(0x4f5cf4) = 0;
            if (DATA_WORD(0x4f5d2c) != 0) {
                LinkInterface *interface = (LinkInterface *)DATA_WORD(0x4f5d2c);
                ((int (__stdcall *)(void *))interface->vtable->methods[4])(interface);
                ((int (__stdcall *)(void *))interface->vtable->methods[2])(interface);
                DATA_WORD(0x4f5d2c) = zero;
            }
            if (DATA_WORD(0x4f5d30) != 0) {
                CloseHandle((void *)DATA_WORD(0x4f5d30));
                DATA_WORD(0x4f5d30) = 0;
            }
            thread_handle = DATA_WORD(0x4f5d24);
            if (thread_handle != 0) {
                Sleep(100);
                TerminateThread((void *)thread_handle, 0);
                CloseHandle((void *)thread_handle);
                DATA_WORD(0x4f5d24) = zero;
            }
        }
    } else {
        link_force_shutdown();
        if (DATA_WORD(0x4f5d24) != 0) {
            Sleep(100);
            TerminateThread((void *)DATA_WORD(0x4f5d24), 0);
            CloseHandle((void *)DATA_WORD(0x4f5d24));
            DATA_WORD(0x4f5d24) = 0;
            return;
        }
    }
}

unsigned int send_link_packet(unsigned int first, int second, int third, int fourth)
{
    int result;
    if (DATA_WORD(0x4f5ce4) != 0) {
        result = link_alt_send(DATA_WORD(0x4f5cf4), first, fourth, second,
                               third);
        if (result != 0)
            return 0x80004005;
        return 0;
    } else {
        if (DATA_WORD(0x4f5d2c) == 0)
            return 0;
        result = ((LinkInterface *)DATA_WORD(0x4f5d2c))->vtable->send(
                                         (LinkInterface *)DATA_WORD(0x4f5d2c),
                                         DATA_WORD(0x4f5cf4),
                                         first, fourth, second, third);
        if (result > 0) {
            DATA_WORD(0x4f5cf8) = result;
            return 1;
        }
        if (result == (int)0x88770096) {
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
    if (DATA_WORD(0x4f5ce4) != 0) {
        result = link_alt_receive(event_id, player, flags, (int)packet, packet_size);
    } else {
        if (DATA_WORD(0x4f5d2c) == 0)
            return 0;
        result = ((LinkInterface *)DATA_WORD(0x4f5d2c))->vtable->receive(
            (LinkInterface *)DATA_WORD(0x4f5d2c), event_id, player,
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
    if (DATA_WORD(0x4f5ce4) != 0) {
        alternate_link_disconnect();
    } else {
        if (DATA_WORD(0x4f5d2c) != 0)
            ((LinkInterface *)DATA_WORD(0x4f5d2c))->vtable->disconnect(
                (LinkInterface *)DATA_WORD(0x4f5d2c), 0);
    }
}

void reset_link_options(void)
{
    unsigned int *options = (unsigned int *)(link_data + (0x4f5d08 - 0x4a0000));
    memset(options, 0, 0x1c);
    *options = 0x1c;
    LinkInterface *interface = (LinkInterface *)DATA_WORD(0x4f5d2c);
    interface->vtable->set_options(interface, options);
}

int request_link_connection(const char *address)
{
    if (link_is_ready(address) == 0)
        return 0;
    if (DATA_WORD(0x4f5cec) == 2)
        Sleep(DATA_WORD(0x4f5cf4) * 300 + 3000);
    link_load_settings(link_data + (0x4f5d08 - 0x4a0000));
    DATA_WORD(0x4f5cdc) = 1;
    DATA_WORD(0x4f5d24) = (unsigned int)LinkBeginThread(
        0, 0, (LinkThreadStart)link_thread_proc_anchor, 0, 0, 0,
        (DWORD *)(link_data + (0x4f5d28 - 0x4a0000)));
    if (DATA_WORD(0x4f5d24) == 0) {
        link_force_shutdown();
        return 0;
    }
    return 1;
}
