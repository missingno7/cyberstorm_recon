extern "C" { void *memset(void *, int, unsigned int); char *strcpy(char *, const char *); } struct GuiControl; static inline int HotbriefHealth(int *info) { return *(int *)((char *)info + 0x12) > 0; } struct GuiPoint {}; struct HotbriefPoint { int x, y; void Move(int dx, int dy) { x += dx; y += dy; } }; static inline int HotbriefIsActive(void *s) { return ((int *)s)[4] != 0; }
extern GuiControl *gHotbriefResourceList[]; extern GuiPoint gHotbriefMousePoint; extern int gHotbriefHit;
extern int gDrawRemap; extern char gShellFont[]; extern char gShellResources[]; extern char fallbackName[], modeCheckName[]; extern char gShellFontBefore[];
extern char g4b74f4[]; extern unsigned long gBriefTimer; extern char g513888[];
extern char gMenuClip[]; extern char gHotbriefPool[]; extern char DAT_00511e54[];
extern char DAT_00511e7c[]; extern char gResMgr[]; extern char gMissionNamePool[];
extern int gHotbriefPanelState; extern int gInputPolling; extern int g5138ac;
extern int g5138b0; extern int playerCount; extern int gRemoteActorKind;
extern int gHotbriefStringIndex; extern int gHotbriefCounter; extern int gMenuTextColor;
extern char *gMenuTextMap; extern int viewportClipLeft; extern char gBriefDrawSource[];
extern char gBriefTextMap[]; extern int gBriefTextColor; extern char DAT_004d274e[];
extern char targetBriefResource[]; extern void AssertFail(const char *, int, const char *);
#define ASSERT(test) if (!(test)) AssertFail(__FILE__, __LINE__, 0)
void f_cb80(void *); void f_cbf0(void *); void f_0041cab0(void *);
void ReleasePoolData(void *); void GuiFadeOut(void); void ShellClearFrame(void);
int f_0042604d(void *, GuiPoint *); int f_00425ba1(void *); void SigsStart(void *, int);
void SurfacePresent(void *); void GuiResetClip(void *); void *ActorGetByIndex(int);
void *BriefWaitA(int); unsigned long BriefWaitB(int); void f_cba0(void *, const char *);
char *f_0047c350(void *, int, int);  
class ResMgr { public: void *Load(const char *, int, void * = 0, int = 0); }; void f_d2c0(void *); void HercSetMissionStart(void *, int);
void MenuDrawImage(void *, void *, int, int); void f_0047ddf0(void); void InputPoll(void *);
void f_0047c240(void *); void f_0047cfc0(void *, int, int, int, char *); void ShellServiceFrame(void *);
void ShellLimitFrameRate(void *); int f_0047e920(const char *); char *BriefFormat(void *, ...);
void f_00477295(char *); void f_00477295(void *); void *AllocateMissionImage(void *, int, int = 1);
void HotbriefSetup(void *); void f_00450b00(void *); void f_00450aa0(void *, int);
void TimerSet(unsigned long *, unsigned long); int TimerExpired(unsigned long *); void MenuDrawImage(void *, void *, int, int);
void MenuSetClip(void *, int, int, int, int); void BriefRenderUpdate(void *); int f_00438678(int);
char *f_00438374(int, int, int); void f_0042b290(void *, void *, int); int f_0042b420(int *, int *);
unsigned long f_0047e160(void *); void f_0045a9f0(void *, void *, int, int); int f_004394d8(void);
int f_0045d830(void *, void *, int, int); void f_00452950(void *, void *, void *, int); int f_0044eb00(void *);
void f_00451370(void *); void *ActorFindByOwner(void *); void *ActorFindOwnerNext(void *, void *);
void *f_00456b20(void); void *f_00456bb0(void *); int f_00464956(void *);
void *f_004641ac(void *); void *f_004641bb(void *, void *); void *f_00463c56(void *);
char *f_00463be8(void *); void f_00463c22(void *); int f_004771aa(const char *);
static char opfireSourceFile[]=__FILE__; int DAT_004a3a0c[]={70,70,66,63,56,49,42,35,28,21,17,14,10,7,3,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; extern unsigned char DAT_004c9265[]; extern int DAT_004d2dea;
extern int DAT_004d2de6; extern int DAT_004d3551; extern int DAT_004dd0b4;
extern int DAT_004dd0b8; extern int DAT_004f65e8; extern char DAT_004fb944[];
extern char DAT_00511ddc[]; extern char DAT_0048b5ac[]; extern char DAT_0048b5ae[];
extern char DAT_0048b5b2[]; extern char DAT_0048b5b4[]; extern char DAT_0051445c[];
extern int gHotbriefMissionFlags; extern char g511dc0[], hotbriefPlayerMaps[], hotbriefCountFormat[]; extern int hotbriefPlayerColors[];


int HotbriefCanSelectAction(void *actor, int rankLimit)
{
    if (HotbriefHealth(*(int **)((char *)actor + 0x1c))) {
        int mission = *(int *)((char *)actor + 0x18);
        char *slot = (char *)mission + 0x69;
        int skill = f_00438678(mission);
        for (int index = 0, row = 0xb; index < 8; ++index, ++row, slot += 8) {
            if (slot[1] && slot[2] &&
                ((*(unsigned char *)((char *)actor + 0x8e) & ((1 << index) == 0)) == 0)) {
                char *record = f_00438374(mission, row, -1);
                if ((unsigned char)slot[3] < (signed char)record[0x20] &&
                    DAT_004c9265[(signed char)record[0x1f]] &&
                    (*(int *)(record + 0x13) == 0 || *(int *)(slot + 4) != 0) &&
                    (DAT_004d2dea == *(int *)((char *)actor + 8) ||
                     *(int *)(record + 0x2b) <= skill) &&
                    rankLimit <= (signed char)record[0x21])
                    return 1;
            }
        }
    }
    return 0;
}

int HotbriefCheckAction(void *actorA, void *actorB, int location,
                        int priorLocation, int *target)
{
    if (!((signed char *)actorB)[0x88])
        return 0;
    unsigned long actorLocation[2];
    f_0042b290(fallbackName, actorLocation, location);
    int locationRank = f_0042b420((int *)actorLocation,
                                  (int *)((char *)actorB + 0x3a));
    if (locationRank >= 0x15)
        return 0;
    int chance = DAT_004a3a0c[locationRank];
    if (chance == 0)
        return 0;
    if (chance != 100 && chance < (int)(f_0047e160(DAT_0051445c) % 100))
        return 0;
    if (priorLocation != 0) {
        unsigned long previous[2];
        f_0042b290(fallbackName, previous, priorLocation);
        if (f_0042b420((int *)previous,
                       (int *)((char *)actorB + 0x3a)) < locationRank)
            return 0;
    }
    int bestRank = locationRank;
    int bestSkill = 0;
    int mission = *(int *)((char *)actorB + 0x18);
    int skill = 0;
    if (HotbriefHealth(*(int **)((char *)actorB + 0x1c))) {
        char *slot = (char *)mission + 0x69;
        skill = f_00438678(mission);
        for (int index = 0, row = 0xb; index < 8; ++index, ++row, slot += 8) {
            if (slot[1] && slot[2] &&
                ((*(unsigned char *)((char *)actorB + 0x8e) & ((1 << index) == 0)) == 0)) {
                char *record = f_00438374(mission, row, -1);
                if ((unsigned char)slot[3] < (signed char)record[0x20] &&
                    DAT_004c9265[(signed char)record[0x1f]] &&
                    (*(int *)(record + 0x13) == 0 || *(int *)(slot + 4) != 0) &&
                    (DAT_004d2dea == *(int *)((char *)actorB + 8) ||
                     *(int *)(record + 0x2b) <= skill) &&
                    bestRank <= (signed char)record[0x21]) {
                    bestSkill = 1;
                    break;
                }
            }
        }
    }
    if (!bestSkill)
        return 0;
    f_0045a9f0(modeCheckName, target, *(int *)actorB, -*((int *)actorA));
    if (target[0x2b] < 0x3c)
        return 0;
    return 1;
}

void HotbriefProcessOpposingActions(void *mission, int *actor, int location, int priorLocation)  {
    DAT_004dd0b8 = 0;
    int owner = DAT_004f65e8;
    int actorOwner;
    if (*(int *)((char *)mission + 0x2c2) == 0)  {
        owner = DAT_004dd0b4;
        if (*(int *)((char *)mission + 0x10f1) != 0) owner = *(int *)((char *)mission + 0x10f9);
    }
    if (owner != 0 && (actorOwner=actor[2]) == *(int *)((char *)mission + 0x97e) && (char)actor[0x22])  {
        int *other = (int *)f_00456b20();
        if (other != 0)  {
            int *targetRecord = (int *)((char *)mission + 0x9a6);
            int targetData[44];
            do  {
                if (actorOwner != other[2])  {
                    int *testActor=other;
                    int previousLocation=priorLocation;
                    int *sourceActor=actor;
                    int valid = 0;
                    if ((char)testActor[0x22])  {
                        unsigned long actorLocation[2];
                        f_0042b290(fallbackName, actorLocation, location);
                        int rank = f_0042b420((int *)actorLocation, (int *)((char *)testActor + 0x3a));
                        if (rank < 0x15)  {
                            int chance = DAT_004a3a0c[rank];
                            if (chance != 0 && (chance == 100 || (int)(f_0047e160(DAT_0051445c) % 100) <= chance))  {
                                if (previousLocation != 0)  {
                                    unsigned long previous[2];
                                    f_0042b290(fallbackName, previous, previousLocation);
                                    if (f_0042b420((int *)previous, (int *)((char *)testActor + 0x3a)) < rank) goto consider_next;
                                }
                                if (*(int *)(testActor[7] + 0x12) > 0)  {
                                    int missionData = testActor[6];
                                    char *slot = (char *)missionData + 0x69;
                                    int skill = f_00438678(missionData);
                                    for (int index = 0, row = 0xb; index < 8; ++index, ++row, slot += 8)  {
                                        if (slot[1] && slot[2] && ((*(unsigned char *)((char *)testActor + 0x8e) & ((1 << index) == 0)) == 0))  {
                                            unsigned int recordId = f_004394d8();
                                            char *record = f_00438374(missionData, row, recordId);
                                            if ((unsigned char)slot[3] < (signed char)record[0x20] && DAT_004c9265[(signed char)record[0x1f]] && (*(int *)(record + 0x13) == 0 || *(int *)(slot + 4) != 0) && (DAT_004d2dea == testActor[2] || *(int *)(record + 0x2b) <= skill) && rank <= (signed char)record[0x21])  {
                                                f_0045a9f0(modeCheckName, targetData, *testActor, -*sourceActor);
                                                if (targetData[0x2b] >= 0x3c) valid = 1;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    consider_next: if (valid)  {
                        DAT_004dd0b8 = 1;
                        int index;
                        while ((index = f_0045d830(targetRecord, targetData, 0x23, 0)) >= 0)  {
                            f_00452950(mission, other, actor, index);
                            DAT_004dd0b8 = 0;
                            if (actor[2] == *(int *)((char *)mission + 0x986) && f_0044eb00(mission) != 0)  {
                                ((unsigned char *)mission)[0x2c] |= 0x20;
                                f_00451370(mission);
                                ((unsigned char *)mission)[0x2c] &= 0xdf;
                            }
                            if (!(char)actor[0x22]) return;
                        }
                    }
                }
                other = (int *)f_00456bb0(other);
            }
            while (other != 0);
        }
    }
    DAT_004dd0b8 = 0;
}

void HotbriefEmpty(void)
{
}

