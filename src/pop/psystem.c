struct InputReq; struct StrPool;
struct GuiScreen { int Update(); void LoadLayout(long name); void Open(InputReq *request, StrPool *resource, long id, int flags); }; struct PopNode {
    char pad00[0x10]; int active; char pad14[0x38]; int changed;
    int IsActive() const { return active != 0; }
    void Open(void *callback);
    int GetValue(int index);
};
struct PopSelection { void Update(int value, int flags = 0); };
struct PopFrameRect { int left, top, width, height; };
struct PopTableRect { int words[4]; };
struct PopManagerRoot {
    char *GetItems() const { return *(char **)((char *)this + 0x18); }
};

extern int PopLoadResource(int resource, const char *name, int count, int flags);
extern void PopNotifyChanged(PopNode *node); extern void PopRefreshNode(PopNode *node);
extern void PopObjectConstruct(void *object);
extern void PopConstructorArray(void *objects, int count, void *constructor);
extern char targetBriefResource[];
extern int PopFindItem(void *manager, int id, int flags); extern int PopItemValue(int item);
extern void PopRegisterItem(void *slot, int value); extern int PopItemReady(void *manager, int id);
extern void PopSetItem(void *manager, int item, int id, int flags);
extern void PopPlaySelection(void *sound);
extern void PopUpdateItem(PopNode *node, int index);
struct SoundSettings; extern int SoundPlay(unsigned int id, SoundSettings *settings);
extern void PopAssignScreen(void *screen, int state);
extern void PopStartItem(void *screen, int index, int flags);
extern void PopPrepareRect(PopFrameRect *destination, PopFrameRect *source);
extern void PopDrawSurface(void *surface, int a, int b, int c, int d, int e, int f);
extern char *AllocateMissionImage(int *pool, int id, int flags = 0);
extern int sprintf(char *destination, const char *format, ...);
extern void PopDrawText(char *text, int a, int b, int c, int d, int *rect,
                        int e = 0, int f = 0);
extern int BriefGetHandle(char *object, void *resource);
extern int PopSelectionIsReady(void *selection); extern int PopGetActiveIndex(void *selection, void *manager);
extern void AssertFail(const char *file, int line, const char *message);

extern void *g4d2498; extern char g4d1cfc[]; extern int g4d1d24[]; extern int g4d1d28[];
extern int g4a0010, g48dc00, g4a0018, g4d1dec, g4d388c; extern char g4a001c[];
extern PopSelection g4d2460; extern int g4fb944, g4bb78c;
extern int gIn80, gInA4;
extern int g5138fc[4], g51390c[4], g51391c[4], g513888;
int PopLoadItem(int resource);

inline char *PopGetItemManager()
{
    return ((PopManagerRoot *)g4d2498)->GetItems();
}

/* One plain-profile object: pop\psystem.c, 00443f58..00444538.
 * 004443d0 is the switch table for the chooser routine, not a separate object.
 */

void PopNodeReset(PopNode *node)
{
    node->active = 0;
}

void PopModuleInit()
{
    PopObjectConstruct(&g4a0010);
    PopConstructorArray(&g4d1cfc, 4, &g48dc00);
    g4a0018 = 1;
    PopLoadItem((int)&g4d1dec);
    g4a0018 = 2;
}

int PopLoadItem(int resource)
{
    extern char gPopItemName[];
    return PopLoadResource(resource, gPopItemName, 4, 0);
}

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
        PopRegisterItem((PopListItem *)(g4d1cfc + i * 0x3c), value);
        int slotOffset = i * 0x3c;
        PopListItem *slot = (PopListItem *)(g4d1cfc + slotOffset);
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
        int selection = BriefGetHandle((char *)node, &g4bb78c);
        if (selection >= 0) {
            g4d388c = selection + 500;
            if (selection < 4) {
                g4d2460.Update(node->GetValue(selection));
                if ((*(unsigned int *)((char *)node + 0x14) & 0xa0) == 0x20) {
                    void *manager = *(void **)((char *)g4d2498 + 0x18);
                    int itemId = selection + 0x13;
                    int item = PopFindItem(manager, itemId, -1);
                    PopSetItem(manager, item, itemId, 0xfffffaff);
                    PopPlaySelection(&g4fb944);
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
    PopTableRect *entry = (PopTableRect *)(g4d1cfc + index * 0x3c);
    rect.left = entry->words[0];
    rect.top = entry->words[1];
    rect.width = entry->words[2];
    rect.height = entry->words[3];
    rect.top += 0x14;
    PopPrepareRect((PopFrameRect *)g51391c, &rect);
    PopDrawSurface(&g513888, 1, 0, rect.left, rect.top, rect.width, rect.height);

    register void *root = g4d2498;
    register char *manager = *(char **)((char *)root + 0x18);
    unsigned char *item = (unsigned char *)PopFindItem(manager, index + 0x13, -1);
    unsigned char *row = (unsigned char *)manager + index * 8 + 0xa9;
    text[0] = 0;
    int label = row[2] == 'd' ? 10 : 15;
    if (row[2] > item[7]) {
        if (item[0x44] == 4 ||
            (item[0x44] == 2 && *(int *)(row + 4) != 0)) {
            label = 0x260;
        } else if (row[3] == 0) {
            label = 0x25e;
        } else {
            label = 0x25d;
        }
        char *format = AllocateMissionImage((int *)targetBriefResource, label);
        sprintf(text, format);
    } else {
        label = 0xe;
        char *format = AllocateMissionImage((int *)targetBriefResource, 0x25f);
        sprintf(text, format);
    }
    PopDrawText(text, -1, 0, 1, label, (int *)&rect);

    int state = *(int *)(item + 0x1f);
    int value = state == 1 ? *(int *)(item + 0x2b) : *(int *)(item + 0x13);
    int rowValue = *(int *)(row + 4);
    int templateId = (rowValue == 0) * 2 + state * 3 + 0x261 +
                     (value == rowValue);
    char *format = AllocateMissionImage((int *)targetBriefResource, templateId);
    sprintf(text, format);
    PopDrawText(text, 1, 0, 0, 10, (int *)&rect);
}

int PopNode::GetValue(int index)
{
    if (PopSelectionIsReady(&g4d2460) == 0)
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
        if (row[3] != 0)
            return 0;
        break;
    case 2: {
        if (*(int *)(row + 4) == 0)
            return 0;
        int activeIndex = PopGetActiveIndex(&g4d2460, g4d2498);
        if (activeIndex < 0 || *(int *)(item + 0x27) < activeIndex)
            return 0;
        return activeIndex * *(int *)(item + 0x23) + *(int *)(item + 0x33);
    }
    default:
        break;
    }
    return *(int *)(item + 0x33);
}
void PopSystemCallback(void * volatile callback)
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
        PopAssignScreen(callback, 3);
        PopStartItem(callback, index, -1);
        break;
    }
}
