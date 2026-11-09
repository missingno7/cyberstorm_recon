/* Working labels identify original functions by address until better names are known. */

struct Surface;
struct GuiPoint;
struct GuiControl;
extern GuiControl *gHercBuyControls[];
struct StrPool;
struct InputReq {
    unsigned long mask;
    unsigned long state;
    int HasState(unsigned long maskBits, unsigned long expected) const
    {
        return (state & maskBits) == expected;
    }
};
class Shell {
public:
    int UpdateCredits();
};
class Flic {
public:
    Flic(); ~Flic();
    unsigned long Update(Surface *surface);
    void Close();
    int Open(long source, int priority, int mode);
};
#pragma pack(push, 1)
class MenuNumber {
    char data[0x2c];
public:
    void SetValue(long value, signed char flash); MenuNumber(signed char font, signed char color); virtual ~MenuNumber();
};
struct MenuImage;
struct MenuPrefixButton {
    char data[0x28];
    MenuPrefixButton(const char *text = 0, signed char align = 1, signed char color = 2, signed char font = -1); virtual ~MenuPrefixButton();
    void SetText(const char *text); void SetColor(signed char color); void SetAlign(signed char align);
};
struct MenuLabel {
    char data[0x2d];
    MenuLabel(const char *text = 0, long align = -1, unsigned char font = 1, long color = 11, int background = 1); virtual ~MenuLabel();
    void SetText(const char *text); void SetColor(long color);
    void SetFont(unsigned char font); void SetBackground(int background); void SetAlign(long align);
};
#pragma pack(pop)
class GuiScreen {
public:
    char opaque[0x50]; ~GuiScreen(){ *(int *)(opaque+0x10)=0; }
    GuiScreen(GuiControl **controls, unsigned length, unsigned long flags);
    void LoadLayout(long resource);
    void Open(InputReq *input, StrPool *strings, long base, int navigate);
    void Invalidate();
    void Draw();
    long HitTest(GuiPoint *point);
    int Update();
};
class ResMgr {
public:
    void *Load(long resource, int flags, int first, int second);
};
class HercBuyCtor : public GuiScreen {
    char gap[0x0d];
    Flic animation;
public:
    HercBuyCtor(); ~HercBuyCtor();
};
extern int g511948; extern int gShellStrings;
extern int gMenuSavedSurface; extern int g513888;
extern int gShellImages; extern char uiSystem[];
extern int g4a706d; extern int g4b392c;
extern int g4b40dc; extern int g4b410c;
extern int g4b395c; extern int g4b3f78;
extern int g4bb78c; extern int g4a70b8;
extern int g4ae17f; extern int g5138fc;
extern int g51390c; extern int gMenuTextColor;
extern unsigned long *gMenuTextMap; extern int g4bc4f8;
extern int g4bc570; extern int g4c9772;
extern int g4b3fec; extern ResMgr gResMgr;
extern int profileManager[];
extern unsigned long gInDirty;
extern unsigned long gIn80;
inline unsigned long InputRead80() { return gIn80; }
extern int g4b38c8, g4b38f0, g4b39cc, g4b3978, g4b39a0;
extern int g4b4040, g4b3fec, g4b4014, g4b3f94, g4b3fbc, g4b3918;
extern int g4b4090, g4b40c0, g4b4150;
extern int g4b3fd0;
extern int g511dd4;
extern int g48ec0c, g48ec10;
extern long gGuiEffectPriority;
extern MenuPrefixButton g4b39e4[15];
extern MenuLabel g4b3c78[15];
extern "C" void BriefBlit(void *surface, int a, int b, int c, int d, int e, int f, int g, int h);
extern "C" int f_00406fc9(char *system);
extern "C" void f_0040627a(int a, int b, int c, int d);
extern "C" void f_00429a75(int *range, int value);
extern "C" void f_00429aa0(int *range, int value);
extern "C" unsigned long f_00441456(void *resource, int *count);
extern "C" void f_00476512(int value);
extern "C" void f_00477798(int value);
extern "C" void f_0043ba3e(void *first, void *second);
extern "C" char *f_004418a9(void *resource, unsigned index);
extern "C" void *f_0047c350(void *pool, int id, int load);
extern "C" void f_0047bff0(void *surface, int *image, int x, int y);
extern "C" void f_0047b3a0(void *surface);
extern "C" int f_0043c327(void);
extern "C" void f_00441b19(void *resource, unsigned index);
extern "C" void f_0046cbc6(int id, int mode);
extern "C" int f_004786d8(void *resource);
extern "C" int f_0047723b(const char *text);
extern "C" void f_004298fa(void *object, unsigned *value);
extern "C" void f_00416f43(void);
extern "C" int sprintf(char *destination, const char *format, ...);
void SurfaceCopyRegion(void *destination, void *source, long x, long y);
extern "C" void AssertFail(const char *file, int line, const char *message);
extern "C" void BriefDone(const char *name);
extern "C" void f_cb80(void *resource);
void InputPoll(InputReq *request);
void GuiPumpMessages();
void RichTextDraw(long left, long top, long width, long height, const unsigned char *text,
                  long scroll, long lineHeight, long paragraphHeight, long *totalHeight,
                  long *visibleHeight, const long *tabs, long color, int font = 1,
                  unsigned long ignore = 0, long firstCharacter = 0,
                  long lastCharacter = 0x7fffffff);
extern "C" char *strncpy(char *destination, const char *source, unsigned long count);
extern const char *MenuFormatNumber(long value, long destination = 0, long length = 16);
void MenuDrawImage(void *surface, const MenuImage *image, long x, long y);
void SurfacePresent(void *surface);
void f_0041686f(void *object);
void f_0041733c(void *object);
void f_00417842(void *object);

#pragma pack(push, 1)
struct HercBuyDataView {
    unsigned char prefix00[0x4c];
    int resource;
    unsigned char state;
    unsigned char prefix51[0xed - 0x51];
    unsigned char entryCount;
    unsigned char middle[0xfce - 0xee];
    unsigned char * volatile records;
    unsigned char currentIndex;
    unsigned char selected;
    unsigned char lastSelected;
    int IsActive() const { return *(const int *)((const char *)this + 0x10) != 0; }
};
#pragma pack(pop)

void f_004166de(void)
{
    InputReq request;
    request.mask = 0x8004;
    while (1) {
        gInDirty = 1;
        InputPoll(&request);
        if (InputRead80() != 0 || request.HasState(0x50, 0x10) ||
            request.HasState(0xa0, 0x20))
            break;
        GuiPumpMessages();
    }
}

HercBuyCtor::HercBuyCtor()
    : GuiScreen(gHercBuyControls, 0x31, 0), animation()
{
}

void f_00416764(void *object, void *input)
{
    if (((HercBuyDataView *)object)->IsActive())
        return;

    f_00477798(3);
    f_0043ba3e(&g4a70b8, profileManager);
    *(int *)((char *)object + 0xf2) = 0;
    g4a706d = g4ae17f - 1;
    g4b3f78 = -1;
    *(int *)&g5138fc = *(int *)&g51390c;
    *((int *)&g5138fc + 1) = *((int *)&g51390c + 1);
    *((int *)&g5138fc + 2) = *((int *)&g51390c + 2);
    *((int *)&g5138fc + 3) = *((int *)&g51390c + 3);
    SurfaceCopyRegion(&gMenuSavedSurface, &g511948, 0, 0);
    void *resource = gResMgr.Load((long)"SHELLHRC.bmx", 1, 0, 0);
    *(int *)((char *)object + 0x4c) = (int)resource;
    if (resource == 0) {
        AssertFail(__FILE__, __LINE__, 0);
    }
    ((Flic *)((char *)object + 0x5d))->Open((long)"hb_buy_flx", 100, 0);
    *(int *)((char *)object + 0xa9) = 1;
    ((GuiScreen *)object)->LoadLayout((long)"hercbuy_box");
    ((GuiScreen *)object)->Open((InputReq *)input, (StrPool *)&gShellStrings, 0x76d, 1);
    *((unsigned char *)object + 0x50) = 0;
    *(int *)((char *)object + 0xee) = 0;
    f_0041686f(object);
}

void f_0041686f(void *object)
{
    int count;
    unsigned long records;
    unsigned char row;
    unsigned char entry;
    unsigned char i;

    records = f_00441456(&g4a70b8, &count);
    *(unsigned long *)((char *)object + 0xfce) = records;
    *((unsigned char *)object + 0xed) = (unsigned char)count;
    f_00429a75(&g4b395c, 0);
    if (count < 0x10) {
        g4b39cc = (g4b39cc & 0xff1fffff) + 0xa00000;
        g4b3978 = (g4b3978 & 0xff1fffff) + 0xa00000;
        g4b39a0 = (g4b39a0 & 0xff1fffff) + 0xa00000;
    } else {
        f_00429aa0(&g4b395c, count - 0xe);
        *((unsigned char *)object + 0xfd5) = 0;
        *((unsigned char *)object + 0xfd6) = *((unsigned char *)object + 0xfd5);
    }
    f_00429a75(&g4b3fd0, 0);
    *(unsigned short *)((char *)object + 0xfd7) = 0;
    *(unsigned short *)((char *)object + 0xfd9) = 99;
    *(int *)((char *)object + 0x55) = 0;
    *((unsigned char *)object + 0xfd2) = 0;
    *((unsigned char *)object + 0xfd4) = 99;
    *((unsigned char *)object + 0xfd3) = 1;

    for (row = 0; row < 0xf; ++row) {
        g4b39e4[row].SetColor(2);
        g4b39e4[row].SetAlign(1);
        entry = 0;
        while (1) {
            if (0xe < entry)
                break;
            g4b39e4[entry].SetText((const char *)object + 0xf6 +
                                   (unsigned long)entry * 0x11);
            g4b3c78[entry].SetText((const char *)object + 0x7fe +
                                   (unsigned long)entry * 0xc);
            entry = entry + 1;
        }
        g4b3c78[row].SetColor(2);
        g4b3c78[row].SetFont(1);
        g4b3c78[row].SetBackground(0);
        g4b3c78[row].SetAlign(1);
    }

    if (((Shell *)uiSystem)->UpdateCredits() != 0)
        ((MenuNumber *)&g4b392c)->SetValue(g4a706d, 0);

    for (i = 0; i < *((unsigned char *)object + 0xed); ++i) {
        strncpy((char *)object + 0xf6 + (unsigned long)i * 0x11,
                *(const char **)(records + (unsigned long)i * 0x19), 0x10);
        MenuFormatNumber(*(long *)(records + 8 + (unsigned long)i * 0x19),
                         (long)(unsigned long)((unsigned char *)object + 0x7fe +
                                               (unsigned long)i * 0xc),
                         0xb);
    }
    for (i = 0; i < *((unsigned char *)object + 0xed); ++i) {
        *((unsigned char *)object + 0x79a + i) =
            *(unsigned char *)(records + 0xc + (unsigned long)i * 0x19);
    }
    for (i = 0; i < *((unsigned char *)object + 0xed); ++i) {
        *(unsigned long *)((char *)object + 0xcae + (unsigned long)i * 4) =
            *(unsigned long *)(records + 0x11 + (unsigned long)i * 0x19);
        *(unsigned long *)((char *)object + 0xe3e + (unsigned long)i * 4) =
            *(unsigned long *)(records + 0x0d + (unsigned long)i * 0x19);
    }
    f_0041733c(object);
}

void f_0041686f(void *object);
void f_00416b76(void *object)
{
    unsigned char i;
    HercBuyDataView *screen = (HercBuyDataView *)object;

    for (i = 0; i < screen->entryCount; ++i)
        *((unsigned char *)screen + 0x79a + i) =
            screen->records[0x0c + i * 0x19];
}

void f_00416bad(void *object)
{
    unsigned char i;
    HercBuyDataView *screen = (HercBuyDataView *)object;

    for (i = 0; i < screen->entryCount; ++i) {
        unsigned long offset = (unsigned long)i * 0x19;
        *(unsigned long *)((char *)screen + 0xcae + (unsigned long)i * 4) =
            *(unsigned long *)(screen->records + 0x11 + offset);
        *(unsigned long *)((char *)screen + 0xe3e + (unsigned long)i * 4) =
            *(unsigned long *)(screen->records + 0x0d + offset);
    }
}

void f_00416bfa(void *object)
{
    if (((HercBuyDataView *)object)->IsActive()) {
        f_cb80((void *)*((int *)object + 0x13));
        *((int *)object + 0x13) = 0;
        BriefBlit(&g513888, 1, 0, 0, 0, 0, 0, 0x104, 0x1e0);
        SurfacePresent(&g513888);
        ((Flic *)((char *)object + 0x5d))->Close();
        *((int *)object + 4) = 0;
    }
}

void f_00416c65(void *object)
{
    if (((HercBuyDataView *)object)->IsActive()) {
        signed char current = (signed char)g4b395c;
        *((char *)object + 0xfd5) = (char)current;
        if ((char)current != *((char *)object + 0xfd6)) {
            *((char *)object + 0xfd6) = (char)current;
            *(int *)((char *)object + 0x55) = 1;
        }
        ((MenuLabel *)&g4b3c78[*(int *)((char *)object + 0xee)])->SetColor(2);
        ((MenuPrefixButton *)&g4b39e4[*(int *)((char *)object + 0xee)])->SetColor(2);
        MenuLabel *oldLabel = &g4b3c78[*(int *)((char *)object + 0xee)];
        oldLabel->data[0x17] &= 0xfe;
        MenuPrefixButton *oldButton = &g4b39e4[*(int *)((char *)object + 0xee)];
        oldButton->data[0x17] &= 0xfe;
        int index = ((GuiScreen *)object)->HitTest((GuiPoint *)&g4bb78c);
        if (index >= 0 && index <= 0xe) {
            char selected = (char)index;
            *((char *)object + 0xfd2) = *((char *)object + 0xfd5) + selected;
            ((MenuLabel *)&g4b3c78[index])->SetColor(0xf);
            ((MenuPrefixButton *)&g4b39e4[index])->SetColor(0xf);
            *(int *)((char *)object + 0xee) = index;
        }
        g4b3f78 = index;
        ((GuiScreen *)object)->Update();
    }
}

void f_00416d7f(void *object)
{
    ((Flic *)((char *)object + 0x5d))->Update((Surface *)&g511948);
    *((unsigned char *)object + 0x50) = 1;
}

void f_00416d9d(void *object)
{
    Flic *animation = (Flic *)((char *)object + 0x5d);

    if (gGuiEffectPriority > 0x32) {
        if ((animation->Update((Surface *)&g511948) & 4) == 0)
            return;
        animation->Close();
    } else {
        animation->Close();
        MenuDrawImage(&g513888, (const MenuImage *)(g511dd4 + 0x10c), 0x177, 0x2b);
        MenuDrawImage(&g513888, (const MenuImage *)(*(int *)((char *)object + 0x4c) + 4), 0, 0);
        MenuDrawImage(&g513888,
                      (const MenuImage *)(*(int *)((char *)object + 0x4c) + 4 +
                          *((unsigned char *)object + 0x79a + *((unsigned char *)object + 0xfd2)) * 0xc),
                      0, 0);
        SurfacePresent(&g513888);
    }
    f_0040627a(0xc, 0x1f, 0xd9, 0x1a9);
    BriefBlit(&g513888, 0, 1, 0, 0, 0, 0, 0x104, 0x1e0);
    *((unsigned char *)object + 0x50) = 2;
    f_0041733c(object);
    ((GuiScreen *)object)->Invalidate();
    ((GuiScreen *)object)->Draw();
}

void f_00416eae(void)
{
    BriefBlit(&g513888, 1, 0, 0, 0x26, 0, 0x26, 0x104, 0x192);
}

void f_00416eec(void *object)
{
    long totalHeight;
    long visibleHeight;
    HercBuyDataView *screenCopy;
    unsigned char baseIndex;
    unsigned char i;

    if (!((HercBuyDataView *)object)->IsActive())
        return;

    switch (*((unsigned char *)object + 0x50)) {
    case 0:
        ((Flic *)((char *)object + 0x5d))->Update((Surface *)&g511948);
        *((unsigned char *)object + 0x50) = 1;
        return;
    case 1:
        f_00416d9d(object);
        return;
    case 2:
        if (((Shell *)uiSystem)->UpdateCredits() != 0)
            ((MenuNumber *)&g4b392c)->SetValue(g4a706d, 0);

        if (*(int *)((char *)object + 0x55) != 0) {
            *(int *)((char *)object + 0x55) = 0;
            screenCopy = (HercBuyDataView *)object;
            baseIndex = *((unsigned char *)object + 0xfd5);
            for (i = 0; i < 0xf; ++i) {
                g4b39e4[i].SetText((const char *)object + 0xf6 +
                                   ((unsigned long)baseIndex + i) * 0x11);
                g4b3c78[i].SetText((const char *)object + 0x7fe +
                                   ((unsigned long)baseIndex + i) * 0xc);
            }
            ((GuiScreen *)screenCopy)->Invalidate();
            BriefBlit(&g513888, 1, 0, 0, 0x26, 0, 0x26, 0x104, 0x192);
        }

        if (*((unsigned char *)object + 0xfd2) != *((unsigned char *)object + 0xfd4) &&
            *((unsigned char *)object + 0xfd2) < *((unsigned char *)object + 0xed)) {
            *((unsigned char *)object + 0xfd4) = *((unsigned char *)object + 0xfd2);
            *((unsigned char *)object + 0xfd3) =
                *((unsigned char *)object + 0x79a + *((unsigned char *)object + 0xfd2));
            BriefBlit(&g513888, 1, 0, 0, 0x145, 0, 0x145, 0xe9, 0x4b);
            MenuDrawImage(&g513888, (const MenuImage *)(*(int *)((char *)object + 0x4c) + 4), 0, 0);
            MenuDrawImage(&g513888,
                          (const MenuImage *)(*(int *)((char *)object + 0x4c) + 4 +
                                              *((unsigned char *)object + 0x79a +
                                                *((unsigned char *)object + 0xfd2)) * 0xc),
                          0, 0);
            ((MenuNumber *)&g4b40dc)->SetValue(
                *(long *)((char *)object + 0xe3e + *((unsigned char *)object + 0xfd2) * 4), 0);
            ((MenuNumber *)&g4b410c)->SetValue(
                *(long *)((char *)object + 0xcae + *((unsigned char *)object + 0xfd2) * 4), 0);
            ((GuiScreen *)object)->Invalidate();
            ((GuiScreen *)object)->Draw();
            if (*((unsigned char *)object + 0xed) != 0) {
                RichTextDraw(0x13, 0x145, 0xd2, 0x4b,
                             *(const unsigned char **)(*(unsigned char **)((char *)object + 0xfce) +
                                 4 + *((unsigned char *)object + 0xfd2) * 0x19),
                             0, -1, 0, &totalHeight, &visibleHeight, 0, 0xf);
            }
            SurfacePresent(&g513888);
        }
        break;
    case 3:
        f_00417842(object);
        break;
    case 4:
    case 5:
        break;
    }
    ((GuiScreen *)object)->Draw();
}

void f_004171ac(void *object)
{
    long totalHeight;
    long visibleHeight;
    if (*((unsigned char *)object + 0xed) != 0) {
        RichTextDraw(0x13, 0x145, 0xd2, 0x4b,
                     *(const unsigned char **)(*(unsigned char **)((char *)object + 0xfce) +
                         4 + *((unsigned char *)object + 0xfd2) * 0x19),
                     0, -1, 0, &totalHeight, &visibleHeight, 0, 0xf);
    }
}

void f_00417219(void *object)
{
    unsigned char row;
    unsigned char entry;

    for (row = 0; row < 0xf; ++row) {
        MenuPrefixButton *button = &g4b39e4[row];
        button->SetColor(2);
        button->SetAlign(1);
        entry = 0;
        while (1) {
            if (0xe < entry)
                break;
            g4b39e4[entry].SetText((const char *)object + 0xf6 +
                                   (unsigned long)entry * 0x11);
            g4b3c78[entry].SetText((const char *)object + 0x7fe +
                                   (unsigned long)entry * 0xc);
            entry = entry + 1;
        }
        MenuLabel *label = &g4b3c78[row];
        label->SetColor(2);
        label->SetFont(1);
        label->SetBackground(0);
        label->SetAlign(1);
    }
}

void f_0041733c(void *object);
void f_0041733c(void *object)
{
    int i;

    switch (*((unsigned char *)object + 0x50)) {
    case 0:
    case 1:
    case 2:
        f_00476512(0xd);
        for (i = 0; i < *((unsigned char *)object + 0xed) && i < 0xf; ++i) {
            *(unsigned long *)((char *)&g4b39e4[i] + 0x14) &= 0xff1fffff;
            *(unsigned long *)((char *)&g4b3c78[i] + 0x14) &= 0xff1fffff;
        }
        for (; i < 0xf; ++i) {
            *(unsigned long *)((char *)&g4b39e4[i] + 0x14) =
                (*(unsigned long *)((char *)&g4b39e4[i] + 0x14) & 0xff1fffff) + 0xa00000;
            *(unsigned long *)((char *)&g4b3c78[i] + 0x14) =
                (*(unsigned long *)((char *)&g4b3c78[i] + 0x14) & 0xff1fffff) + 0xa00000;
        }
        g4b38c8 &= 0xff1fffff;
        g4b38f0 &= 0xff1fffff;
        if (0xf < *((unsigned char *)object + 0xed)) {
            g4b39cc &= 0xff1fffff;
            g4b3978 &= 0xff1fffff;
            g4b39a0 &= 0xff1fffff;
        }
        g4b4040 = (g4b4040 & 0xff1fffff) + 0xa00000;
        g4b3fec = (g4b3fec & 0xff1fffff) + 0xa00000;
        g4b4014 = (g4b4014 & 0xff1fffff) + 0xa00000;
        g4b3f94 = (g4b3f94 & 0xff1fffff) + 0xa00000;
        g4b3fbc = (g4b3fbc & 0xff1fffff) + 0xa00000;
        g4b3918 = (g4b3918 & 0xff1fffff) + 0xa00000;
        g4b4090 &= 0xfe1fffff;
        g4b40c0 &= 0xfe1fffff;
        g4b4150 = ((g4b4150 & 0xff1fffff) + 0xa00000) & 0xfeffffff;
        return;
    case 3:
        f_00476512(0xe);
        for (i = 0; i < 0xf; ++i) {
            *(unsigned long *)((char *)&g4b39e4[i] + 0x14) =
                (*(unsigned long *)((char *)&g4b39e4[i] + 0x14) & 0xff1fffff) + 0xa00000;
            *(unsigned long *)((char *)&g4b3c78[i] + 0x14) =
                (*(unsigned long *)((char *)&g4b3c78[i] + 0x14) & 0xff1fffff) + 0xa00000;
        }
        g4b38c8 = (g4b38c8 & 0xff1fffff) + 0xa00000;
        g4b38f0 = (g4b38f0 & 0xff1fffff) + 0xa00000;
        g4b39cc = (g4b39cc & 0xff1fffff) + 0xa00000;
        g4b3978 = (g4b3978 & 0xff1fffff) + 0xa00000;
        g4b39a0 = (g4b39a0 & 0xff1fffff) + 0xa00000;
        g4b4090 = (g4b4090 & 0xff1fffff) + 0xa00000;
        g4b4040 &= 0xff1fffff;
        g4b3fec &= 0xff1fffff;
        g4b4014 &= 0xff1fffff;
        g4b3f94 &= 0xff1fffff;
        g4b3fbc &= 0xff1fffff;
        g4b3918 = (g4b3918 & 0xff1fffff) + 0xa00000;
        g4b40c0 = ((g4b40c0 & 0xff1fffff) + 0xa00000) & 0xfeffffff;
        g4b4150 &= 0xfe1fffff;
        return;
    case 4:
        g4b4040 = (g4b4040 & 0xff1fffff) + 0x200000;
        g4b3fec = (g4b3fec & 0xff1fffff) + 0x200000;
        g4b4014 = (g4b4014 & 0xff1fffff) + 0x200000;
        g4b3f94 = (g4b3f94 & 0xff1fffff) + 0x200000;
        g4b3fbc = (g4b3fbc & 0xff1fffff) + 0x200000;
    case 5:
        g4b3918 &= 0xfe1fffff;
        return;
    default:
        return;
    }
}

void f_004176b1(void *object)
{
    char i;

    for (i = 0; i < *((unsigned char *)object + 0xed); ++i) {
        strncpy((char *)object + 0xf6 + (unsigned long)i * 0x11,
                *(const char **)(*(int *)((char *)object + 0xfce) +
                                 (unsigned long)i * 0x19), 0x10);
        MenuFormatNumber(*(int *)(*(int *)((char *)object + 0xfce) + 8 +
                                  (unsigned long)i * 0x19),
                         (long)(unsigned long)((unsigned char *)object + 0x7fe +
                                               (unsigned long)i * 0xc),
                         0xb);
    }
}

void f_00417732(void *object, unsigned char baseIndex)
{
    unsigned int i;

    i = 0;
    while (1) {
        if (0xe < i)
            break;
        g4b39e4[i].SetText((const char *)object + 0xf6 +
                           ((unsigned long)baseIndex + i) * 0x11);
        g4b3c78[i].SetText((const char *)object + 0x7fe +
                           ((unsigned long)baseIndex + i) * 0xc);
        i = i + 1;
    }
}

void f_004177af(void *object, char amount);
void f_004177af(void *object, char amount)
{
    char *text;
    *((char *)object + 0xfe7) = *((char *)object + 0xfd5) + (amount - 1);
    *((unsigned char *)object + 0x50) = 3;
    f_0041733c(object);
    ((GuiScreen *)object)->Invalidate();
    *(unsigned short *)((char *)object + 0xfd9) = 999;
    text = f_004418a9(&g4a70b8, *((unsigned char *)object + 0xfe7));
    *(char **)((char *)object + 0xfe3) = text;
    if (text == 0) {
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        /* original ASSERT line immediate */
        AssertFail(__FILE__, __LINE__, "no text for purchase window");
    }
    f_00417842(object);
    f_00429aa0(&g4b3fd0, *(int *)((char *)object + 0xfdb) - 0x174);
}

void f_00417842(void *object)
{
    long tabs[2];
    tabs[0] = g48ec0c;
    tabs[1] = g48ec10;
    short selected = (short)g4b3fd0;
    *(short *)((char *)object + 0xfd7) = selected;
    if (selected != *(short *)((char *)object + 0xfd9)) {
        BriefBlit(&g513888, 1, 0, 0, 0x2b, 0, 0x2b, 0x104, 0x188);
        *(unsigned short *)((char *)object + 0xfd9) =
            *(unsigned short *)((char *)object + 0xfd7);
        gMenuTextMap = (unsigned long *)&g4bc570;
        gMenuTextColor = g4bc4f8;
        RichTextDraw(0xd, 0x2b, 0xd9, 0x188,
                     *(const unsigned char **)((char *)object + 0xfe3),
                     *(unsigned short *)((char *)object + 0xfd7), -1, 6,
                     (long *)((char *)object + 0xfdb),
                     (long *)((char *)object + 0xfdf), tabs, 0xf);
        *((unsigned char *)&g4b3fec + 3) &= 0xfe;
        ((GuiScreen *)object)->Draw();
        SurfacePresent(&g513888);
        f_00416f43();
    }
}

void f_00417842(void *object);
void f_0041791b(void *object);
void f_0041791b(HercBuyDataView *screen)
{
    if (screen->selected == 0)
        screen->selected = 1;
    BriefBlit(&g513888, 1, 0, 0, 0, 0, 0, 0x104, 0x1e0);
    MenuDrawImage(&g513888,
                  (const MenuImage *)(screen->resource + 4), 0, 0);
    MenuDrawImage(&g513888,
                  (const MenuImage *)(screen->resource + 4 + (screen->selected * 4 - screen->selected) * 4),
                  0, 0);
    f_00429a75(&g4b3fd0, 0);
    screen->state = 2;
    f_0041733c(screen);
    ((GuiScreen *)screen)->Invalidate();
    ((GuiScreen *)screen)->Draw();
    SurfacePresent(&g513888);
}

void f_004179d7(void)
{
    if (f_00406fc9(uiSystem) != 0)
        ((MenuNumber *)&g4b392c)->SetValue(g4a706d, 0);
}

void f_004179ff(void)
{
    if (f_00406fc9(uiSystem) != 0)
        ((MenuNumber *)&g4b392c)->SetValue(g4a706d, 0);
}

int f_00417a27(void *object)
{
    long totalA, visibleA, totalB, visibleB, totalC, visibleC;
    unsigned char *text;
    unsigned long price;
    unsigned long credits;
    int result;
    price = *(unsigned long *)(*(int *)((char *)object + 0xfce) + 8 +
                               *((unsigned char *)object + 0xfe7) * 0x19);
    credits = g4ae17f;
    if (price > credits) {
        *((unsigned char *)object + 0x50) = 4;
        text = (unsigned char *)f_0047c350(&gShellStrings, 0x79d, 1);
        MenuDrawImage(&g513888, (const MenuImage *)(gShellImages + 0x31c), 0x118, 0xaa);
        SurfacePresent(&g513888);
        f_0041733c(object);
        RichTextDraw(0x122, 0xb4, 0xd7, 0x50, text, 0, -1, 0,
                     &totalA, &visibleA, 0, 0xf);
        result = 0x37;
    } else {
        result = f_0043c327();
        if (result != 0) {
            f_00441b19(&g4a70b8, *((unsigned char *)object + 0xfe7));
            if (((Shell *)uiSystem)->UpdateCredits() != 0)
                ((MenuNumber *)&g4b392c)->SetValue(g4a706d, 0);
            *(int *)((char *)object + 0xf2) = 1;
            f_0046cbc6(0x66, 0);
            return 1;
        }
        *((unsigned char *)object + 0x50) = 4;
        if (g4c9772 == 0 || g4c9772 == 1) {
            text = (unsigned char *)f_0047c350(&gShellStrings, 0x79e, 1);
            MenuDrawImage(&g513888, (const MenuImage *)(gShellImages + 0x31c), 0x118, 0xaa);
            SurfacePresent(&g513888);
            f_0041733c(object);
            RichTextDraw(0x122, 0xb4, 0xd7, 0x50, text, 0, -1, 0,
                         &totalB, &visibleB, 0, 0xf);
        } else {
            text = (unsigned char *)f_0047c350(&gShellStrings, 0x79f, 1);
            MenuDrawImage(&g513888, (const MenuImage *)(gShellImages + 0x31c), 0x118, 0xaa);
            SurfacePresent(&g513888);
            f_0041733c(object);
            RichTextDraw(0x122, 0xb4, 0xd7, 0x50, text, 0, -1, 0,
                         &totalC, &visibleC, 0, 0xf);
        }
        result = 0x72;
    }
    f_0046cbc6(result, 0);
    return 0;
}

int f_00417a27(void *object);
void f_00417c72(HercBuyDataView *screen)
{
    f_0041791b(screen);
}

void f_00417c80(void *object, const unsigned char *text)
{
    long total, visible;
    MenuDrawImage(&g513888, (const MenuImage *)(gShellImages + 0x31c), 0x118, 0xaa);
    SurfacePresent(&g513888);
    f_0041733c(object);
    RichTextDraw(0x122, 0xb4, 0xd7, 0x50, text, 0, -1, 0,
                 &total, &visible, 0, 0xf);
}

void f_00417c80(void *object, const unsigned char *text);
void f_00417cf9(void *unused, const unsigned char *text)
{
    long total, visible;
    RichTextDraw(0x122, 0xb4, 0xd7, 0x50, text, 0, -1, 0,
                 &total, &visible, 0, 0xf);
}

void f_00417cf9(void *unused, const unsigned char *text);
void f_00417d3d(void *object)
{
    if (*(int *)((char *)object + 0xf2) != 0) {
        *(int *)((char *)object + 0xf2) = 0;
        f_0043ba3e(&profileManager, &g4a70b8);
    }
}

void f_00417d3d(void *object);
void f_00417d67(void *object);
void f_00417d67(void *object)
{
    char *text;
    if (*(int *)((char *)object + 0xf2) != 0) {
        text = (char *)f_0047c350(&gShellStrings, 0xf8d, 0);
        if (f_0047723b(text) != 0) {
            *(int *)((char *)object + 0xf2) = 1;
            if (*(int *)((char *)object + 0xf2) == 0)
                return;
            *(int *)((char *)object + 0xf2) = 0;
            f_0043ba3e(&profileManager, &g4a70b8);
        } else {
            *(int *)((char *)object + 0xf2) = 0;
            f_0043ba3e(&g4a70b8, &profileManager);
        }
    }
}

void f_00417dd9(void *object);
void f_00417dd9(void *object)
{
    BriefDone("hercbuy");
}

void f_00417de7(void *object);
void f_00417de7(void *object)
{
    unsigned char buffer[0x400];
    long total, visible;
    unsigned result;
    if (*((unsigned char *)object + 0xed) >= 100) {
        unsigned char *format = (unsigned char *)f_0047c350(&gShellStrings, 0x7a0, 1);
        sprintf((char *)buffer, (const char *)format);
        *((unsigned char *)object + 0x50) = 5;
        MenuDrawImage(&g513888, (const MenuImage *)(gShellImages + 0x31c), 0x118, 0xaa);
        SurfacePresent(&g513888);
        f_0041733c(object);
        RichTextDraw(0x122, 0xb4, 0xd7, 0x50, buffer, 0, -1, 0,
                     &total, &visible, 0, 0xf);
        return;
    }
    result = f_004786d8(&g4a70b8);
    if ((int)result < 0) {
        *((unsigned char *)object + 0x50) = 5;
        return;
    }
    *((unsigned char *)object + 0x50) = 2;
    f_0041686f(object);
    *(int *)((char *)object + 0x55) = 1;
    *(int *)((char *)object + 0xf2) = 1;
}

#pragma pack(push, 1)
class HercBuyInitButton {
    char data[0x24];
public:
    HercBuyInitButton(const char *, unsigned long);
    virtual ~HercBuyInitButton();
};
class HercBuyInitBar {
    char data[0x24];
public:
    HercBuyInitBar(void *, int);
    virtual ~HercBuyInitBar();
};
class HercBuyInitRange {
    int current, maximum;
public:
    HercBuyInitRange(int, int);
};
class HercBuyInitChild {
    char data[0x28];
public:
    HercBuyInitChild(void *, unsigned int, int);
    virtual ~HercBuyInitChild();
};
class HercBuyInitHelp {
    char data[0x24];
public:
    HercBuyInitHelp();
    virtual ~HercBuyInitHelp();
};
class HercBuyInitPanel {
    char data[0x20];
public:
    HercBuyInitPanel();
    virtual ~HercBuyInitPanel();
};
class HercBuyInitScreen {
    char data[0x68];
public:
    HercBuyInitScreen();
    virtual ~HercBuyInitScreen();
};
#pragma pack(pop)

HercBuyInitButton initButton0(0, 0);
HercBuyInitButton initButton1(0, 0);
HercBuyInitButton initButton2(0, 0);
MenuNumber initNumber0(-1, 0x12);
HercBuyInitRange initRange0(0x32, 0);
HercBuyInitBar initBar0(&initRange0, 0);
HercBuyInitChild initChild0(&initRange0, 1, 0x30);
HercBuyInitChild initChild1(&initRange0, -1, 0x2d);
MenuPrefixButton g4b39e4[15];
MenuLabel g4b3c78[15];
char initPadAfterLabels;
HercBuyInitHelp initHelp;
HercBuyInitButton initButton3(0, 0);
HercBuyInitButton initButton4(0, 0);
HercBuyInitRange initRange1(1000, 0);
HercBuyInitBar initBar1(&initRange1, 0);
HercBuyInitChild initChild2(&initRange1, 10, 0x30);
HercBuyInitChild initChild3(&initRange1, -10, 0x2d);
HercBuyInitPanel initPanel;
MenuNumber initNumber1(-1, 0x12);
MenuNumber initNumber2(-1, 0x12);
MenuNumber initNumber3(-1, 0x12);
MenuNumber initNumber4(-1, 0x12);
MenuNumber initNumber5(-1, 0x12);
HercBuyInitScreen initScreen;


HercBuyCtor::~HercBuyCtor() {} 


