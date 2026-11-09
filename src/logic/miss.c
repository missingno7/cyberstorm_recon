typedef unsigned char u8;

struct MissionRef {
    int type;
    int item;
    int Resolve();
    int Address();
};

struct MissionRecord {
    int type;
    int item;
    int mode;
    int field0c;
    int field10;
    int time;
    int image;
    int field1c;
    int field20;
    int field24;
    int field28;
    int field2c;
    int field30;
    int field34;
    int field38;
    int field3c;
    int field40;
    int field44;
    int field48;
    int field4c;
    int field50;
    int field54;
};

extern int missLimitByType[];
extern int missOffsetByType[];
extern int missionCurrentOffset;
extern void *AllocateMissionImage(const char *image, int bytes, int mode = 1);
extern void *LoadMissionImage(const char *image, int bytes, int mode = 1);
extern const char missionImageName[];

extern void MissionLookup(int offset, const char *table);
extern int RandomRange(int *seed, int low, int high);

int MissionRef::Resolve()
{
    int type = this->type;
    int item = this->item;
    if (type <= 0)
        type = 0;
    else if (type >= 2)
        type = 2;
    int last = missLimitByType[type] - 1;
    if (item <= 0)
        item = 0;
    else if (item >= last)
        item = last;
    item = missOffsetByType[type] + item;
    MissionLookup(item, (const char *)0x4c9234);
    missionCurrentOffset = item;
    return item;
}

extern int TimeNow(void);
extern int MissionRandomTable[];
extern int missionPlayerCount;
extern int missionEstimateTable[];
extern void MissionDataInit(MissionRecord *, int, int, int, int, int, int);
extern void AssertFail(const char *file, int line, int condition);
extern void *MissionResourceLoad(void *manager, const char *file, int mode, void *buffer, int bytes);
extern int MissionRandomPercent(int *seed);
extern void MissionStateParse(void *table, void *record, void *output);
extern unsigned char missionRecordTable[];
extern int missionBonusByRank[];
extern char missionResourceManager[];
extern char missionTableFilename[];
extern int missionRandomSeed;
extern int missionRank;
extern int ClampRange(int value, int low, int high);
extern int sprintf(char *destination, const char *format, ...);
extern unsigned char missionStateOutput[];
extern char missionImageString[];
extern char missionImageBuffer[];
extern unsigned char missionCopyView[];
extern unsigned char missionCopyLayout[];
extern unsigned char missionCopyHeader[];
extern int missionCopyIndex;
extern unsigned char missionCopySeedA;
extern unsigned char missionCopySeedB;
extern unsigned char missionCopySeedC;
extern unsigned char missionCopySeedD;
extern unsigned char missionCopySeedE;

void MissionRecordInitialize(MissionRecord *record, int mode, int type, int item,
                             int field0c, int field10)
{
    record->type = type;
    record->item = item;
    record->mode = mode;
    record->field0c = field0c;
    record->field10 = field10;
    record->time = TimeNow() + 1;
    record->image = RandomRange(MissionRandomTable, 1, 3);
    MissionDataInit(record, 0, 0, 0, 0, 0, 0);
}

int MissionEstimate(MissionRecord *record, int difficulty, int *baseOut)
{
    int allowed;
    int iVar2;
    int iVar3;
    int iVar4;
    if (difficulty <= 1)
        difficulty = 1;
    else if (difficulty >= 0xd)
        difficulty = 0xd;
    int players = missionPlayerCount;
    if (players <= 1)
        iVar2 = 1;
    else {
        iVar2 = players;
        if (players >= 5)
            iVar2 = 5;
    }
    iVar2 = missionEstimateTable[iVar2 - 1 + (difficulty - 1) * 5];
    iVar3 = iVar2 / 10;
    if (record->field10 == 0) {
        iVar4 = iVar2 - iVar3;
        iVar3 = iVar2;
    } else {
        iVar4 = iVar2;
        if (record->field10 != 2) {
            iVar3 >>= 1;
            iVar4 = iVar2 - iVar3;
        }
        iVar3 = iVar3 + iVar2;
    }
    iVar3 = RandomRange(MissionRandomTable, iVar4, iVar3);
    if (record->field28 == 0 || record->field28 == 2)
        allowed = 1;
    else {
        allowed = 0;
        if (record->field28 == 2 || record->field28 == 3)
            allowed = 1;
        if (!allowed)
            iVar4 = 0xc0;
        else
            goto useExtra;
    }
useExtra:
    if (allowed)
        iVar4 = RandomRange(MissionRandomTable, 0xc0, 0x154);
    iVar3 = (record->field24 * (iVar3 * iVar4 >> 8)) / 100;
    if (iVar3 < 1)
        iVar3 = 0;
    else if (499999 < iVar3)
        iVar3 = 500000;
    if (baseOut)
        *baseOut = iVar2;
    return iVar3;
}

void MissionDataInit(MissionRecord *record, int field28, int field2c, int field30,
                     int field34, int field38, int field40)
{
    unsigned char localTable[0x120];
    char workArea[0x698];
    record->field28 = field28;
    record->field2c = field2c;
    record->field30 = field30;
    record->field34 = field34;
    record->field38 = field38;
    record->field40 = field40;
    missionRandomSeed = record->time + 0x45;
    int type = record->type;
    int item = record->item;
    type = ClampRange(type, 0, 2);
    item = ClampRange(item, 0, missLimitByType[type] - 1);
    int offset = missOffsetByType[type] + item;
    MissionLookup(offset, (const char *)0x4c9234);
    missionCurrentOffset = offset;
    void *loaded = MissionResourceLoad(missionResourceManager, missionTableFilename, 1,
                                       localTable, 0x120);
    /*
     * The source line below is deliberately kept at the assertion immediate.
     */
    /* ASSERT_PAD_14 */
    /* ASSERT_PAD_15 */
    /* ASSERT_PAD_16 */
    /* ASSERT_PAD_17 */
    /* ASSERT_PAD_18 */
    /* ASSERT_PAD_19 */
    /* ASSERT_PAD_20 */
    /* ASSERT_PAD_21 */
    /* ASSERT_PAD_22 */
    /* ASSERT_PAD_23 */
    /* ASSERT_PAD_24 */
    /* ASSERT_PAD_25 */
    /* ASSERT_PAD_26 */
    /* ASSERT_PAD_27 */
    /* ASSERT_PAD_28 */
    /* ASSERT_PAD_29 */
    /* ASSERT_PAD_30 */
    /* ASSERT_PAD_31 */
    /* ASSERT_PAD_32 */
    if (!loaded) AssertFail(__FILE__, __LINE__, 0);
    int iVar2 = 0;
    int iVar6 = 0x1f;
    if (record->mode == 3) {
        int group = record->type;
        group = group <= 0 ? 0 : (group >= 2 ? 2 : group);
        /* Preserve the second assertion's original __LINE__ immediate. */
        iVar2 = group * 8;
        iVar6 = group * 8 + 7;
    }
    unsigned char *pbVar7 = localTable + record->mode * 0x20;
    unsigned int uVar5 = (unsigned int)MissionRandomPercent(&missionRandomSeed) % 100;
    pbVar7 += iVar2;
    while (iVar2 <= iVar6) {
        int nextIndex = iVar2 + 1; unsigned char probability = *pbVar7;
        if ((int)uVar5 < (int)probability) {
            record->field1c = (record->mode + 1) * 100 + nextIndex;
            iVar2 = nextIndex;
            break;
        }
        uVar5 -= probability; ++pbVar7; iVar2 = nextIndex;
    }
    /* ASSERT_LINE_PAD_01 */
    /* ASSERT_LINE_PAD_02 */
    if (iVar6 < iVar2) AssertFail(__FILE__, __LINE__, 0);
    int rank = missionRank;
    if (rank < 1)
        rank = 0;
    else if (rank > 2)
        rank = 2;
    record->field20 = rank;
    if (record->mode == 1)
        record->field20 = rank + 3;
    MissionStateParse(missionRecordTable, record, workArea);
    record->field24 = *(int *)(workArea + 4);
    type = record->type;
    item = record->item;
    type = ClampRange(type, 0, 2);
    item = ClampRange(item, 0, missLimitByType[type] - 1);
    offset = missOffsetByType[type] + item;
    MissionLookup(offset, (const char *)0x4c9234);
    missionCurrentOffset = offset;
    int estimate = MissionEstimate(record, record->field34, &record->field48);
    record->field4c = estimate;
    record->field50 = estimate / 0x14;
    int factor;
    int secondary;
    int modeKind = record->field0c;
    if (modeKind == 0) {
        factor = record->type * 2 + record->field34 / 2 + 0x5f;
        secondary = record->type * 4;
        iVar6 = record->field34 / 2 + 0x5a;
    } else if (modeKind == 1) {
        factor = record->field34 + 100 + record->type * 4;
        iVar6 = record->field34 + 0x6e;
        secondary = record->type << 3;
    } else if (modeKind == 2) {
        factor = record->field34 + 0x69 + record->type * 6;
        secondary = record->field34 + 0x82 + record->type * 10;
        goto missionScales;
    } else {
        factor = record->type * 2 + record->field34 / 2 + 0x5f;
        secondary = record->type * 4;
        iVar6 = record->field34 / 2 + 0x5a;
    }
    secondary += iVar6;
missionScales:
    record->field4c = factor * record->field4c / 100;
    record->field50 = record->field50 * secondary / 100;
    if (record->field28 == 2 || record->field28 == 3) {
        int bonus = record->field38;
        if (bonus < 1)
            bonus = 0;
        else if (bonus > 9)
            bonus = 10;
        record->field50 = missionBonusByRank[bonus];
    }
    record->field54 = 1000;
}

void MissionCopy(int *source, int *destination)
{
    int item = source[1];
    int type = ClampRange(source[0], 0, 2);
    item = ClampRange(item, 0, missLimitByType[type] - 1);
    int offset = missOffsetByType[type] + item;
    MissionLookup(offset, (const char *)0x4c9234);
    missionCurrentOffset = offset;
    MissionStateParse(missionRecordTable, source, missionStateOutput);

    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
    destination[3] = source[3];
    destination[4] = source[4];
    destination[5] = source[10];
    destination[6] = source[6];
    destination[7] = source[0x13];
    destination[8] = source[0x15];

    char *banner;
    if (source[8] == 0) {
        banner = 0;
    } else {
        sprintf(missionImageBuffer,
                (const char *)AllocateMissionImage(missionImageString, 0xf5, 0), source[8]);
        banner = missionImageBuffer;
    }
    int scale = (source[0x13] * 6 + 200000) / 150000;
    if (scale < 2)
        scale = 1;
    else if (scale > 5)
        scale = 6;
    destination[9] = (int)missionCopyView;
    destination[10] = (int)banner;
    destination[11] = (int)missionCopyLayout;
    destination[12] = (int)AllocateMissionImage(missionImageName, scale + 0x22b);
    destination[13] = (int)missionCopyHeader;
    int index = missionCopyIndex;
    destination[14] = index;
    destination[15] = (int)AllocateMissionImage(missionImageName, index + 0x35);
    destination[16] = (int)AllocateMissionImage(missionImageName, offset + 1);
    destination[17] = (int)LoadMissionImage(missionImageName, offset + 1);
    destination[18] = (int)AllocateMissionImage(missionImageName, missionCopySeedA + 0x630);
    destination[19] = (int)AllocateMissionImage(missionImageName, missionCopySeedB + 0x63b);
    destination[20] = (int)AllocateMissionImage(missionImageName, missionCopySeedC + 0x650);
    destination[21] = (int)AllocateMissionImage(missionImageName, missionCopySeedD + 0x618);
    destination[22] = (int)AllocateMissionImage(missionImageName, missionCopySeedE + 0x621);
}

int MissionRef::Address()
{
    register MissionRef *ref = this;
    int type = ref->type;
    int item = ref->item;
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
    int boundedType;
    if (type <= 0)
        boundedType = 0;
    else if (type >= 2)
        boundedType = 2;
    else
        boundedType = type;
    int last = missLimitByType[boundedType] - 1;
    int boundedItem = item <= 0 ? 0 : (item >= last ? last : item);
    return missOffsetByType[boundedType] + boundedItem;
}

void *MissionAllocateType(int type)
{
    if (type <= 0)
        type = 0;
    else if (type >= 2)
        type = 2;
    return AllocateMissionImage(missionImageName, type + 0x32, 1);
}

void MissionAllocateItem(register int type, int item)
{
    register int typeIndex = type;
    if (typeIndex <= 0)
        typeIndex = 0;
    else if (typeIndex >= 2)
        typeIndex = 2;
    AllocateMissionImage(missionImageName,
        missOffsetByType[typeIndex] +
        (item <= 0 ? 0 : (item >= missLimitByType[typeIndex] - 1 ?
        missLimitByType[typeIndex] - 1 : item)) + 1);
}

void MissionLoadItem(register int type, int item)
{
    register int typeIndex = type;
    if (typeIndex <= 0)
        typeIndex = 0;
    else if (typeIndex >= 2)
        typeIndex = 2;
    LoadMissionImage(missionImageName,
        missOffsetByType[typeIndex] +
        (item <= 0 ? 0 : (item >= missLimitByType[typeIndex] - 1 ?
        missLimitByType[typeIndex] - 1 : item)) + 1);
}
