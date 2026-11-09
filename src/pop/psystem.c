struct PopNode {
    char pad00[0x10];
    int active;
    char pad14[0x38];
    int changed;
    int IsActive() const { return active != 0; }
};

extern int PopLoadResource(int resource, const char *name, int count, int flags);
extern void PopNotifyChanged(PopNode *node);
extern void PopRefreshNode(PopNode *node);
extern void PopUpdateItem(PopNode *node, int index);
extern void PopObjectConstruct(void *object);
extern void PopConstructorArray(void *objects, int count, void *constructor);
extern int PopOpenBox(char *object, const char *name);
extern void PopOpenResource(char *object, void *callback, void *resource, int id, int flags);
extern int PopFindItem(void *manager, int id, int flags);
extern int PopItemValue(int item);
extern void PopRegisterItem(void *slot, int value);
extern int PopItemReady(void *manager, int id);
extern void *g4d2498;
extern char g4d1cfc[];
extern int g511e54;
extern int g4a0010;
extern int g48dc00;
extern int g4a0018;
extern int g4d1dec;
int PopLoadItem(int resource);

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
    char pad18[0x0c];
    int disabled;
};

void PopSystemOpen(PopNode *node, void *callback)
{
    if (node->IsActive())
        return;
    PopOpenBox((char *)node, "POPSYS.BOX");
    PopOpenResource((char *)node, callback, &g511e54, 0x259, 0);
    void * volatile manager = *(void **)((char *)g4d2498 + 0x18);
    for (int i = 0; i < 4; ++i) {
        int itemId = i + 0x13;
        int item = PopFindItem(manager, itemId, -1);
        int value = PopItemValue(item);
        PopListItem *slot = (PopListItem *)(g4d1cfc + i * 0x3c);
        PopRegisterItem(slot, value);
        slot->flags = 3;
        slot->disabled = -1;
        slot->flags &= 0xff1fffff;
        if (!PopItemReady(manager, itemId))
            slot->flags = (slot->flags & 0xff1fffff) + 0xa00000;
    }
    node->changed = 1;
}

void PopNodeClear(PopNode *node)
{
    if (node->IsActive())
        node->active = 0;
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
