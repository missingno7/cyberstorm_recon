struct InputReq; struct StrPool; struct GuiPoint { long x,y; }; struct ActorUnit; struct ActorItem;
typedef unsigned long U32;
struct GuiRect { long x,y,width,height; };
class GuiControl:public GuiRect { public: unsigned char style[4]; U32 attributes,shortcut; virtual ~GuiControl(); virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned,U32); virtual int HitTest(const void *); };
struct GuiScreen { char before[0x10]; int active; char rest[0x38];
    GuiScreen(GuiControl **,unsigned,U32);
    long HitTest(const GuiPoint *); int Update(); void LoadLayout(long name); void Open(InputReq *request, StrPool *resource, long id, int flags);
};
#pragma aux pop_screen_constructor "W?PopLoadItem$n(i)i";
struct PopNode:GuiScreen {
    int changed;
    __pragma("pop_screen_constructor") PopNode();
    ~PopNode(){active=0;}
    int IsActive() const { return active != 0; }
    void Open(void *callback);
    int GetValue(int index);
};
struct SimGame; struct ActorRecord; struct PopSelection {};
extern int SimPreviewCost(SimGame *, int, int = 0);
extern int SimFocusReady(SimGame *);
extern int SimDistanceToDepot(SimGame *, ActorRecord *);
extern void PilotSetState(SimGame *, int);
extern int SimUseItem(SimGame *, int, int);
struct PopInput { int mask, state; int HelpPressed() { return (state & 0xa0) == 0x20; } };
struct PopFrameRect { int left, top, width, height; };
struct PopTableRect { int words[4]; };
struct PopManagerRoot {
    char *GetItems() const { return *(char **)((char *)this + 0x18); }
};

extern int PopLoadResource(int resource, const char *name, int count, int flags);
extern void PopNotifyChanged(PopNode *node); extern void PopRefreshNode(PopNode *node);


extern char targetBriefResource[];
extern int PopFindItem(void *manager, int id, int flags = -1); extern int PopItemValue(int item);
extern void PopRegisterItem(void *slot, int value); extern int PopItemReady(void *manager, int id);
extern char *HercFormatItem(ActorUnit *manager, ActorItem *item, int id, unsigned flags = 0xfffffaff);
extern "C" void f_00477304(void *text);
extern void PopUpdateItem(PopNode *node, int index);
struct SoundSettings; extern int SoundPlay(unsigned int id, const SoundSettings *settings);
extern void PopAssignScreen(void *screen, int state);
extern void PopStartItem(void *screen, int index, int flags);
extern int PopPrepareRect(PopFrameRect *destination, PopFrameRect *source);
extern void PopDrawSurface(void *surface, int a, int b, int c, int d, int e, int f);
extern char *AllocateMissionImage(int *pool, int id, int flags = 0);
extern int sprintf(char *destination, const char *format, ...);
extern void MenuDrawWrapped(const char *text, long a, long b, long c, long d, const PopFrameRect *rect,
                        int e = 0, int f = 0);
extern int BriefGetHandle(char *object, void *resource);
extern int PopSelectionIsReady(void *selection); extern int PopGetActiveIndex(void *selection, void *manager);
extern void AssertFail(const char *file, int line, const char *message);

extern void *g4d2498;  extern int g4d1d24[]; extern int g4d1d28[];
extern int targetLimits; extern char g4a001c[];
extern SimGame modeCheckName; extern char gDebugText[]; extern int g4bb78c;
extern int gIn80; extern long gInA4;
extern int g5138fc[4], g51390c[4], g51391c[4], g513888;


inline char *PopGetItemManager()
{
    return ((PopManagerRoot *)g4d2498)->GetItems();
}

/* Plain object: pop\psystem.c, initializer 00443f64 through callback 004444cd.
 * Generated array destructor at 00444538 and screen destructor at 00444550.
 * PopNodeReset at 00443f58 is retained for the published ownership ledger.
 * 004443d0 is the switch table for the chooser routine, not a separate object.
 */

void PopNodeReset(PopNode *node)
{
    node->active = 0;
}

class WeaponControl:public GuiControl {
public: U32 state; const char *text; long font,value,kind,cost,condition;
    WeaponControl(); virtual ~WeaponControl();
    virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned,U32);
};
WeaponControl g4d1cfc[4];
PopNode g4d1dec;
extern char gPopItemName[];
PopNode::PopNode():GuiScreen((GuiControl **)gPopItemName,4,0){}

struct PopListItem {
    char pad00[0x14];
    int flags;
    int GetFlags() const { return flags; }
};

void PopNode::Open(void *callback)
{
    if (IsActive())
        return;
    ((GuiScreen *)this)->LoadLayout((long)"POPSYS.BOX");
    ((GuiScreen *)this)->Open((InputReq *)callback, (StrPool *)targetBriefResource, 0x259, 0);
    void *manager = *(void **)((char *)g4d2498 + 0x18);
    for (int i = 0; i < 4; ++i) {
        int item = PopFindItem(manager, i + 0x13, -1);
        int value = PopItemValue(item);
        PopRegisterItem((PopListItem *)((char *)g4d1cfc + i * 0x3c), value);
        int slotOffset = i * 0x3c;
        PopListItem *slot = (PopListItem *)((char *)g4d1cfc + slotOffset);
        g4d1d24[i * 0xf] = 3;
        g4d1d28[i * 0xf] = -1;
        int flagValue = slot->GetFlags();
        flagValue = flagValue & 0xff1fffff;
        slot->flags = flagValue;
        if (!PopItemReady(manager, i + 0x13)) {
            flagValue = (slot->flags & 0xff1fffff) + 0xa00000;
            slot->flags = flagValue;
        }
    }
    changed = 1;
}

void PopNodeClear(PopNode *node)
{
    if (node->IsActive())
        node->active = 0;
}

int PopNodeHandleSelection(PopNode *node)
{
    if (node->IsActive()) {
        int selection = ((GuiScreen *)node)->HitTest((const GuiPoint *)&g4bb78c);
        if (selection >= 0) {
            targetLimits = selection + 500;
            if (selection >= 0 && selection <= 3) {
                SimPreviewCost((SimGame *)&modeCheckName, node->GetValue(selection));
                if (((PopInput *)node->active)->HelpPressed()) {
                    void *manager = *(void **)((char *)g4d2498 + 0x18);
                    int itemId = selection + 0x13;
                    HercFormatItem((ActorUnit *)manager, (ActorItem *)PopFindItem(manager, itemId), itemId);
                    f_00477304(&gDebugText);
                }
            }
        }
        return ((GuiScreen *)node)->Update();
    }
    return 0;
}
void PopNodeUpdate(PopNode *node)
{
    if (node->IsActive()) {
        if (node->changed) {
            node->changed = 0;
            void *manager = *(void **)((char *)g4d2498 + 0x18);
            for (int i = 0; i < 4; ++i) {
                if (PopItemReady(manager, i + 0x13))
                    PopUpdateItem(node, i);
            }
        }
        PopRefreshNode(node);
    }
}

void PopNodeChanged(PopNode *node)
{
    node->changed = 1;
    PopNotifyChanged(node);
}
void PopUpdateItem(PopNode *node, int index)
{
    char text[64];
    PopFrameRect rect;
    *(PopFrameRect *)g5138fc = *(PopFrameRect *)g51390c;
    PopTableRect *entry = (PopTableRect *)((char *)g4d1cfc + index * 0x3c);
    rect.left = entry->words[0];
    rect.top = entry->words[1];
    rect.width = entry->words[2];
    rect.height = entry->words[3];
    rect.top += 0x14;
    PopPrepareRect((PopFrameRect *)g51391c, &rect);
    PopDrawSurface(&g513888, 1, 0, rect.left, rect.top, rect.width, rect.height);
    char *manager = PopGetItemManager();
    unsigned char *item = (unsigned char *)PopFindItem(manager, index + 0x13, -1);
    int rowOffset = index * 8 + 0x98;
    manager += 0x11;
    unsigned char *row = (unsigned char *)manager + rowOffset;
    text[0] = 0;
    long label = row[2] == 100 ? 10 : 15;
    if (row[2] <= item[7]) {
        label = 0xe;
        sprintf(text, AllocateMissionImage((int *)targetBriefResource, 0x25f), label);
    } else {
        if (item[0x44] == 4)
            sprintf(text, AllocateMissionImage((int *)targetBriefResource, 0x260));
        else if (item[0x44] == 2 && *(int *)(row + 4))
            sprintf(text, AllocateMissionImage((int *)targetBriefResource, 0x260));
        else if (row[3])
            sprintf(text, AllocateMissionImage((int *)targetBriefResource, 0x25d));
        else
            sprintf(text, AllocateMissionImage((int *)targetBriefResource, 0x25e));
    }
    MenuDrawWrapped(text, -1, 0, 1, label, &rect);
    if (*(int *)(item + 0x1f) != 1) label = *(int *)(item + 0x13);
    else label = *(int *)(item + 0x2b);
    int templateId = *(int *)(item + 0x1f) * 3 + 0x261 +
        (label == *(int *)(row + 4)) + (*(int *)(row + 4) == 0) * 2;
    sprintf(text, AllocateMissionImage((int *)targetBriefResource, templateId), *(int *)(row + 4));
    MenuDrawWrapped(text, 1, 0, 0, 10, &rect);
}
int PopNode::GetValue(int index)
{
    if (SimFocusReady((SimGame *)&modeCheckName) == 0)
        return 0;
    if (g4d2498 == 0 || *(void **)((char *)g4d2498 + 0x18) == 0) {
        AssertFail(g4a001c, 211, 0);
    }
    void *manager = *(void **)((char *)g4d2498 + 0x18);
    if (index >= 4)
        return 0;
    if (!PopItemReady(manager, index + 0x13))
        return 0;
    unsigned char *item = (unsigned char *)PopFindItem(manager, index + 0x13, -1);
    int rowOffset = index * 8 + 0x98;
    manager = (char *)manager + 0x11;
    char *row = (char *)manager + rowOffset;
    int rowLevel = (unsigned char)row[2];
    int itemLevel = (unsigned char)item[7];
    if (rowLevel <= itemLevel)
        return 0;
    switch (item[0x44]) {
    case 1:
        if (row[3])
            return 0;
        break;
    case 2: {
        if (*(int *)(row + 4) == 0)
            return 0;
        int activeIndex = SimDistanceToDepot((SimGame *)&modeCheckName, (ActorRecord *)g4d2498);
        if (activeIndex < 0 || *(int *)(item + 0x27) < activeIndex)
            return 0;
        return activeIndex * *(int *)(item + 0x23) + *(int *)(item + 0x33);
    }
    case 0: case 3: case 4:
    default:
        break;
    }
    return *(int *)(item + 0x33);
}
void PopSystemCallback(SimGame *callback)
{
    int selection = PopNodeHandleSelection((PopNode *)&g4d1dec);
    int index = selection - 1;
    if (index < 0) {
        int control = gIn80;
        if (control >= 2 && control <= 5) {
            void *ready = (void *)gInA4;
            if (ready != 0)
                index = control - 2;
        }
        if (index < 0)
            return;
    }
    SoundPlay(9, 0);
    switch (index) {
    case 0:
    case 1:
    case 2:
    case 3:
        PilotSetState(callback, 3);
        SimUseItem(callback, index, -1);
        break;
    }
}
