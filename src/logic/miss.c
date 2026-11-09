typedef unsigned char u8;

extern int missLimitByType[];
extern int missOffsetByType[];
extern int missionImage[];
extern void *AllocateMissionImage(const char *image, int bytes, int mode);
extern void *LoadMissionImage(const char *image, int bytes, int mode);
extern const char missionImageName[];

struct MissionRef {
    int type;
    int item;
    int Address();
};

int MissionRef::Address()
{
    int rawType = this->type;
    int type = rawType;
    int item = this->item;
    if (rawType <= 0)
        type = 0;
    else if (rawType >= 2)
        type = 2;
    int last = missLimitByType[type] - 1;
    if (item <= 0)
        item = 0;
    else if (item >= last)
        item = last;
    return missOffsetByType[type] + item;
}

int MissionBaseType(void)
{
    return 3;
}

int MissionRowCount(void)
{
    return 0x12;
}

int MissionCountForType(int type)
{
    if (type <= 0)
        type = 0;
    else if (type >= 2)
        type = 2;
    return missLimitByType[type];
}

int MissionItemAddress(int type, int item)
{
    if (type <= 0)
        type = 0;
    else if (type >= 2)
        type = 2;
    int last = missLimitByType[type] - 1;
    if (item <= 0)
        item = 0;
    else if (item >= last)
        item = last;
    return missOffsetByType[type] + item;
}

void *MissionAllocateType(int type)
{
    if (type <= 0)
        type = 0;
    else if (type >= 2)
        type = 2;
    return AllocateMissionImage(missionImageName, type + 0x32, 1);
}

void *MissionAllocateItem(int type, int item)
{
    if (type <= 0)
        type = 0;
    else if (type >= 2)
        type = 2;
    if (item <= 0)
        item = 0;
    else if (item >= missLimitByType[type] - 1)
        item = missLimitByType[type] - 1;
    return AllocateMissionImage(missionImageName, item + 1, 1);
}

void *MissionLoadItem(int type, int item)
{
    if (type <= 0)
        type = 0;
    else if (type >= 2)
        type = 2;
    if (item <= 0)
        item = 0;
    else if (item >= missLimitByType[type] - 1)
        item = missLimitByType[type] - 1;
    return LoadMissionImage(missionImageName, item + 1, 1);
}
