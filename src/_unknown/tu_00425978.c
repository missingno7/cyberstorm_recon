/* 00425978..004260c4. No original source-path/class-name string survives here.
 * GuiScreen/GuiControl are shared working names, replacing shell-specific aliases.
 * Layouts and methods follow the common control-list, input, and drawing callers. */
typedef unsigned long U32;
struct GuiRect { long x, y, width, height; };
struct GuiPoint { long x, y; };
struct Timer; extern void TimerSet(Timer *,U32);
struct Timer { U32 start,end,duration;Timer(U32 length){TimerSet(this,length);} };
struct InputReq {
    U32 mask, state;
    int ButtonDown() const { return (state & 0x10) != 0; }
    int NewPress() const { return (state & 0x50) == 0x10; }
};
struct StrPool { char *GetString(long id, int load); };
struct ResMgr { int OpenFile(long id, int flags); int CloseFile(); };
extern ResMgr gResMgr;
extern "C" U32 f_c8e0(ResMgr *);
extern "C" U32 f_c900(ResMgr *, void *, U32);
extern void *MemoryAllocationRetry(U32);
extern void MemoryRelease(void *);
extern void TimerSet(Timer *, U32);
extern int TimerExpired(Timer *);
extern U32 InputGetHi(InputReq *);
extern int GuiReadKey(unsigned key);

struct GuiControlData : GuiRect {
    unsigned char style[4];
    U32 attributes;
    U32 shortcut;
};
class GuiControl : public GuiControlData {
public:
    virtual ~GuiControl();
    virtual void Reset(unsigned index);
    virtual void Draw(unsigned index);
    virtual int Event(unsigned index, U32 event);
    virtual int HitTest(const GuiPoint *point);
    int Enabled() const { return attributes != 0 && (attributes & 0xe00000UL) == 0; }
    int Hidden() const { return (attributes & 0x800000UL) != 0; }
    int Wants(U32 mask) const { return (attributes & mask) != 0; }
    int Pressed() const { return (attributes & 0x20000UL) != 0; }
    void Toggle() { attributes |= 0x20000UL; attributes ^= 0x40000UL; }
    void Release() { attributes &= ~0x20000UL; }
    void Invalidate() { attributes &= ~0x1000000UL; }
};
extern int GuiControlShortcut(GuiControl *);
extern int GuiControlStyle(GuiControl *, unsigned index, U32 event);
extern void GuiControlSetShortcut(GuiControl *, const char *);
extern void GuiSetControlClip(void *, const GuiRect *);
extern char g513888;
extern GuiPoint g4bb78c;
extern InputReq *gGuiCurrentInput;
extern StrPool *gGuiStringPool;
extern unsigned gGuiFocusedControl;
extern long gGuiHoveredControl;
extern long gGuiStringBase;
extern GuiRect gGuiClipRect;

class GuiScreen : public GuiRect {
public:
    InputReq *input;
    GuiControl **controls;
    StrPool *strings;
    unsigned count;
    unsigned focus;
    long hovered;
    unsigned pressed;
    U32 options;
    long firstString;
    int navigation;
    int delayed;
    unsigned delayedControl;
    Timer repeatTimer;
    GuiScreen(GuiControl **list, unsigned length, U32 flags);
    GuiScreen *Reset(GuiControl **list, unsigned length, U32 flags);
    void LoadLayout(long resource);
    void SetLayout(int first, int last, const GuiRect *bounds, const GuiRect *rects);
    int Activate(unsigned index);
    int Update();
    void Draw();
    void SetStrings(long base);
    void Invalidate();
    void ResetControls();
    void Open(InputReq *, StrPool *, long base, int navigate);
    long HitTest(const GuiPoint *);
    void ResetControlsCore(){
    if (IsActive()) {
        for (unsigned i = 0; i < count; ++i) {
            Control(i)->Reset(i);
            Control(i)->Invalidate();
        }
    }
}
    void SetStringsCore(long base){
    firstString = base;
    if (IsActive() && strings) {
        for (unsigned i = 0; i < count; ++i) {
            const char *text=strings->GetString(firstString+i,1);
            GuiControlSetShortcut(controls[i],text);
        }
    }
}
    void DrawControlCore(unsigned i){GuiSetControlClip(&g513888,controls[i]);controls[i]->Draw(i);}
    int ActivateCore(unsigned index){
    int result = 0;
    if (input && index < count) {
        GuiControl *item = controls[index];
        item->Toggle();
        U32 event = 5;
        if (item->Wants(event)) result = item->Event(index, event);
    }
    return result;
}
    int IsActive() const { return input != 0; }
    GuiControl *Control(unsigned index) { return controls[index]; }
};

GuiScreen::GuiScreen(GuiControl **list, unsigned length, U32 flags)
 : input(0),controls(list),count(length),focus(0),hovered(-1),pressed(0),options(flags),repeatTimer(0)
{
}

GuiScreen *GuiScreen::Reset(GuiControl **list, unsigned length, U32 flags)
{
    controls = list;
    count = length;
    options = flags;
    input = 0;
    focus = 0;
    hovered = -1;
    pressed = 0;
    return this;
}

void GuiScreen::LoadLayout(long resource)
{
    gResMgr.OpenFile(resource, 0);
    U32 size = f_c8e0(&gResMgr);
    void *storage = MemoryAllocationRetry(size);
    unsigned char *cursor = (unsigned char *)storage;
    f_c900(&gResMgr, storage, size);
    gResMgr.CloseFile();
    x = *(long *)cursor; cursor += 4;
    y = *(long *)cursor; cursor += 4;
    width = *(long *)cursor; cursor += 4;
    height = *(long *)cursor; cursor += 8; /* one reserved header dword */
    for (unsigned i = 0; i < count; ++i) {
        GuiControl *item = controls[i];
        item->x = x + *(long *)cursor; cursor += 4;
        item->y = y + *(long *)cursor; cursor += 4;
        item->width = *(long *)cursor; cursor += 4;
        item->height = *(long *)cursor; cursor += 4;
        for (int j = 0; j < 4; ++j)
            item->style[j] = *cursor++;
    }
    MemoryRelease(storage);
}

void GuiScreen::SetLayout(int first, int last, const GuiRect *bounds, const GuiRect *rects)
{
    x = bounds->x; y = bounds->y; width = bounds->width; height = bounds->height;
    for (int i = first; i <= last; ++i) {
        GuiControl *item = controls[i];
        item->x = rects[i].x + x;
        item->y = rects[i].y + y;
        item->width = rects[i].width;
        item->height = rects[i].height;
        for (int j = 0; j < 4; ++j) item->style[j] = 0;
    }
}

int GuiScreen::Activate(unsigned index)
{
    int result = 0;
    if (input && index < count) {
        GuiControl *item = controls[index];
        item->Toggle();
        U32 event = 5;
        if (item->Wants(event)) result = item->Event(index, event);
    }
    return result;
}

int GuiScreen::Update()
{
    if (IsActive()) {
        gGuiCurrentInput = input;
        hovered = pressed ? pressed - 1 : -1;
        if (navigation) {
            U32 key = InputGetHi(input);
            if (key && !input->ButtonDown()) GuiControlStyle(controls[focus], focus, key);
            GuiReadKey(0x1c);
        }
        int result = 0;
        for (unsigned i = 0; i < count && !result && IsActive(); ++i) {
            GuiControl *item = controls[i];
            if (item->Enabled()) {
                U32 event;
                if (GuiControlShortcut(item)) event = 0x10015;
                else if (item->HitTest(&g4bb78c)) {
                    event = 1;
                    hovered = pressed ? pressed - 1 : i;
                    if (input->ButtonDown()) {
                        if (item->Pressed()) event |= 8;
                        else if (input->NewPress()) {
                            event |= 4;
                            item->Toggle();
                            pressed = i + 1;
                        }
                    } else if (item->Pressed()) {
                        event |= 0x10;
                        item->Release();
                        pressed = 0;
                    }
                } else {
                    event = 2;
                    if (item->Pressed()) {
                        if (input->ButtonDown()) event |= 8;
                        else { item->Release(); pressed = 0; }
                    }
                }
                if (item->Wants(event)) {
                    result = item->Event(i, event);
                    if (!result && (event & 1)) focus = i;
                }
            }
        }
        if (!result && delayed && TimerExpired(&repeatTimer)) {
            result = ActivateCore(delayedControl);
            delayed = 0;
        }
        gGuiCurrentInput = 0;
        return result;
    }
    return 0;
}

void GuiScreen::Draw()
{
    if (IsActive()) {
        gGuiHoveredControl = hovered;
        gGuiFocusedControl = focus;
        gGuiStringPool = strings;
        gGuiStringBase = firstString;
        GuiRect saved = gGuiClipRect;
        for (unsigned i = 0; i < count; ++i) {
            if (!Control(i)->Hidden()) {
                DrawControlCore(i);
            }
        }
        gGuiStringPool = 0;
        gGuiClipRect = saved;
    }

}

void GuiScreen::SetStrings(long base)
{
    firstString = base;
    if (IsActive() && strings) {
        for (unsigned i = 0; i < count; ++i) {
            const char *text=strings->GetString(firstString+i,1);
            GuiControlSetShortcut(controls[i],text);
        }
    }
}

void GuiScreen::Invalidate()
{
    if (IsActive()) for (unsigned i = 0; i < count; ++i) Control(i)->Invalidate();
}

void GuiScreen::ResetControls()
{
    if (IsActive()) {
        for (unsigned i = 0; i < count; ++i) {
            Control(i)->Reset(i);
            Control(i)->Invalidate();
        }
    }
}

void GuiScreen::Open(InputReq *request, StrPool *pool, long base, int navigate)
{
    if (!IsActive()) {
        input = request;
        strings = pool;
        navigation = navigate;
        ResetControlsCore();
        SetStringsCore(base);
        hovered = -1;
        delayed = 0;
    }
}

long GuiScreen::HitTest(const GuiPoint *point)
{
    if (pressed) return hovered = pressed - 1;
    for (unsigned i = 0; i < count; ++i) {
        if (Control(i)->Enabled() && Control(i)->HitTest(point)) {
            hovered = i;
            return i;
        }
    }
    hovered = -1;
    return hovered;
}

typedef char GuiControlSizeMustBe32[sizeof(GuiControl) == 0x20 ? 1 : -1];
typedef char GuiScreenSizeMustBe76[sizeof(GuiScreen) == 0x4c ? 1 : -1];
