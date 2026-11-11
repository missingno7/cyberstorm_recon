inline int MissionBound(int v,int lo,int hi) { if(v<=lo)return lo; if(v>=hi)return hi; return v; }

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
    int field54; int IsPassive() {return field28==0||field28==2;} int IsCombat() {return field28==2||field28==3;} int BoundedType() {int t=type; if(t<=0)t=0;else if(t>=2)t=2;return t;}
};

int MissionItemAddress(int,int); extern int missLimitByType[];
extern int missOffsetByType[];
extern int missionCurrentOffset;
extern void *AllocateMissionImage(const char *image, int bytes, int mode = 1);
struct StrPool { char *GetSecondString(long id,int mode=1); };
extern const char missionImageName[];

extern char g4c9234[]; extern void SimLoadPalette(int offset, const char *table);
extern int RandomRange(int *seed, int low, int high);

int MissionRef::Resolve()
{
    int offset=Address();
    SimLoadPalette(offset,g4c9234);
    missionCurrentOffset=offset;
    return offset;
}











extern int TimeNow(void);
extern int MissionRandomTable[];
extern int missionPlayerCount;
extern int missionEstimateTable[];
extern void MissionDataInit(MissionRecord *, int, int, int, int, int, int);
extern void AssertFail(const char *file, int line, const char *condition);
struct ResMgr { void *Load(const char *,int,void *,unsigned long); }; extern ResMgr gResMgr;
extern "C" unsigned int next_link_seed(unsigned int *);
extern char g4dd0c0[]; void LogicLoadActorState(unsigned long *,const unsigned long *,unsigned long *);
extern unsigned char missionRecordTable[];
extern int missionBonusByRank[];
extern char missionResourceManager[];
extern char missionTableFilename[];
extern int missionRandomSeed;
extern int missionRank;
extern int ClampRange(int value, int low=0, int high=2);
#include <stdio.h>
extern unsigned char missionStateOutput[];
extern char g511ddc[];
extern char missionImageBuffer[];
extern unsigned char missionCopyView[];
extern unsigned char missionCopyLayout[];
extern unsigned char missionCopyHeader[];
extern int HercMassScale;
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
    if(difficulty<=1) difficulty=1; else if(difficulty>=13) difficulty=13;
    int row = difficulty-1;
    int players=missionPlayerCount;
    if(players<=1) players=1; else if(players>=5) players=5;
    int column=--players;
    column+=row*5; int base=missionEstimateTable[column];
    int spread=base/10;
    int low, high;
    switch(record->field10) {
    case 0: low=base-spread; high=base; break;
    case 2: low=base; high=base+spread; break;
    default: spread >>= 1; low=base-spread; high=base+spread; break;
    }
    int estimate=RandomRange(MissionRandomTable,low,high);
    int factor=record->IsPassive() || record->IsCombat() ? RandomRange(MissionRandomTable,0xc0,0x154) : 0xc0;
    estimate *= factor;
    estimate >>= 8;
    estimate = record->field24 * estimate / 100;
    if(estimate<=0) estimate=0; else if(estimate>=500000) estimate=500000;
    if(baseOut) *baseOut=base;
    return estimate;
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
    MissionRandomTable[0] = record->time + 0x45;
    int type = record->type;
    int item = record->item;
    type = ClampRange(type, 0, 2);
    item = ClampRange(item, 0, missLimitByType[type] - 1);
    int offset = missOffsetByType[type] + item;
    SimLoadPalette(offset, g4c9234);
    missionCurrentOffset = offset;
    void *loaded = gResMgr.Load(missionTableFilename, 1,
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
    int iVar2=0;
    int iVar6=0x1f;

    if (record->mode == 3) {
        int group=record->type;
        if (group<=0) group=0; else if (group>=2) group=2;
        iVar2=group*8;
        iVar6=iVar2+7;
    }

    unsigned char *pbVar7 = localTable + record->mode * 0x20;
    unsigned int uVar5 = (unsigned int)next_link_seed((unsigned int *)MissionRandomTable) % 100;
    pbVar7 += iVar2;
    int index;
    for (index=iVar2; index<=iVar6; index++, pbVar7++) {
        int probability=*pbVar7;
        int id=index+1;
        if ((int)uVar5<probability) {
            record->field1c=(record->mode+1)*100+id;
            break;
        }
        uVar5-=probability;
    }
    if (index > iVar6) AssertFail(__FILE__, __LINE__, 0);
    int rank = missionRank;
    if (rank <= 0)
        rank = 0;
    else if (rank >= 2)
        rank = 2;
    record->field20 = rank;
    if (record->mode == 1)
        record->field20 = record->field20 + 3;
    LogicLoadActorState((unsigned long *)g4dd0c0, (const unsigned long *)record, (unsigned long *)workArea);
    record->field24 = *(int *)(workArea + 4);
    type = record->type;
    item = record->item;
    type = ClampRange(type, 0, 2);
    item = ClampRange(item, 0, missLimitByType[type] - 1);
    offset = missOffsetByType[type] + item;
    SimLoadPalette(offset, g4c9234);
    missionCurrentOffset = offset;
    record->field4c = MissionEstimate(record, record->field34, &record->field48);

    record->field50 = record->field4c / 0x14;
    int factor;
    int secondary;
    switch(record->field0c) {
    case 1:
        factor = record->field34 + 100 + record->type * 4;
        secondary = record->field34 + 0x6e + (record->type << 3);
        break;
    case 2:
        factor = record->field34 + 0x69 + record->type * 6;
        secondary = record->field34 + 0x82 + record->type * 10;
        break;
    default:
        factor = record->type * 2 + (record->field34 / 2 + 0x5f);
        secondary = record->type * 4 + (record->field34 / 2 + 0x5a);
        break;
    }
missionScales:
    record->field4c = factor * record->field4c / 100;
    record->field50 = record->field50 * secondary / 100;
    if (record->IsCombat()) {
        int bonus = record->field38;
        if (bonus <= 0)
            bonus = 0;
        else if (bonus >= 10)
            bonus = 10;
        record->field50 = missionBonusByRank[bonus];
    }
    record->field54 = 1000;
}

inline int MissionSelect(MissionRecord *record)
{
    int type = record->type;
    int item = record->item;
    type = ClampRange(type);
    item = ClampRange(item, 0, missLimitByType[type] - 1);
    int offset = missOffsetByType[type] + item;
    SimLoadPalette(offset,g4c9234);
    missionCurrentOffset = offset;
    return offset;
}
void MissionCopy(MissionRecord *record,int *destination)
{
    int offset = MissionSelect(record);
    LogicLoadActorState((unsigned long *)g4dd0c0, (const unsigned long *)record, (unsigned long *)missionStateOutput);

    destination[0] = record->type;
    destination[1] = record->item;
    destination[2] = record->mode;
    destination[3] = record->field0c;
    destination[4] = record->field10;
    destination[5] = record->field28;
    destination[6] = record->image;
    destination[7] = record->field4c;
    destination[8] = record->field54;

    char *banner;
    if (record->field20 != 0) {
        sprintf(missionImageBuffer,
                (const char *)AllocateMissionImage(g511ddc, 0xf5, 0), record->field20);
        banner = missionImageBuffer;
    } else {
        banner = 0;
    }
    int scale = (record->field4c * 6 + 200000) / 150000;
    if (scale <= 1)
        scale = 1;
    else if (scale >= 6)
        scale = 6;
    destination[9] = (int)missionCopyView;
    destination[10] = (int)banner;
    destination[11] = (int)missionCopyLayout;
    destination[12] = (int)AllocateMissionImage(missionImageName, scale + 0x22b);
    destination[13] = (int)missionCopyHeader;
    int index = HercMassScale;
    destination[14] = index;
    destination[15] = (int)AllocateMissionImage(missionImageName, index + 0x35);
    destination[16] = (int)AllocateMissionImage(missionImageName, offset + 1);
    destination[17] = (int)((StrPool *)missionImageName)->GetSecondString(offset + 1);
    destination[18] = (int)AllocateMissionImage(missionImageName, missionCopySeedA + 0x630);
    destination[19] = (int)AllocateMissionImage(missionImageName, missionCopySeedB + 0x63b);
    destination[20] = (int)AllocateMissionImage(missionImageName, missionCopySeedC + 0x650);
    destination[21] = (int)AllocateMissionImage(missionImageName, missionCopySeedD + 0x618);
    destination[22] = (int)AllocateMissionImage(missionImageName, missionCopySeedE + 0x621);
}

int MissionRef::Address()
{
    int t = type;
    int i = item;
    t = MissionBound(t, 0, 2);
    i = MissionBound(i, 0, missLimitByType[t] - 1);
    return missOffsetByType[t] + i;
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

int MissionItemAddress(int type,int item)
{
    int boundedType=MissionBound(type,0,2);
    int last=missLimitByType[boundedType]-1;
    int boundedItem;
    if (item<=0) boundedItem=0;
    else if (item>=last) boundedItem=last;
    else boundedItem=item;
    int offset=missOffsetByType[boundedType];
    offset+=boundedItem;
    return offset;
}

void *MissionAllocateType(int type)
{
    if (type <= 0)
        type = 0;
    else if (type >= 2)
        type = 2;
    return AllocateMissionImage(missionImageName, type + 0x32, 1);
}

void MissionAllocateItem(int type,int item)
{
    AllocateMissionImage(missionImageName,MissionItemAddress(type,item)+1);
}
void MissionLoadItem(int type,int item)
{
    ((StrPool *)missionImageName)->GetSecondString(MissionItemAddress(type,item)+1);
}









