/* Assigned fragment of a larger ProfileManager TU; its true starting VA is not established.
 * Working labels; packed offsets are read from the cs11 callers. Unknown areas
 * remain temporary storage declarations until the surrounding ProfileManager TU is recovered. */
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned long U32;
#pragma pack(push,1)
struct MissionRecord { U32 words[22]; };
struct MissionInfo { int type, number, category, unknown0c, flags, mission, difficulty, value, value20, image, value28, value2c, value30, value34; int rest[9]; };
struct MissionSummary { byte type, number; ushort unused; int category, flags, difficulty, value, nextValue, blocked; void *typeData, *missionData; int image, value34; };
struct UnitRecord { char name[17];byte wounds;byte unknown12[8];int field1a;byte unknown1e[8];byte bytes26,bytes27;byte rest[86]; };
struct PilotSkill { byte active;byte rest[7]; };
struct PilotRecord { byte header[19];PilotSkill skills[23];byte rest[64]; };
struct RankCost { ushort cost; byte rest[14]; };
struct ProgressInfo { int money; byte rank; };
struct GameInfo;
class ProfileManager {
public:
    int field0,totalExperience; MissionRecord missions[9], quickMission; MissionSummary summary; MissionRecord specialMission;
    int mode, field400, missionType, field408, field40c; short choice;
    int killsTotal,objectivesTotal,missionsTotal,wins,streak; int field426, field42a; byte unknown42e[12]; int field43a;
    byte rank; byte unknown43f[2],previousRank; int progress, continuation;
    byte unknown44a[0xed9]; UnitRecord units[60]; PilotRecord pilots[60]; ushort pilotUnits[64]; int unitCount,pilotCount;
    byte unknown6fc7[256]; int money, oldMoney;
    UnitRecord *GetUnit(unsigned slot){return &units[slot-1];} void BeginMissions(); int IsSpecial(){return field426==2 || field426==3;} void SetMissionChoice(unsigned long); void RefreshMission(int);
    int GetProgress(){return progress;} int HasContinuation(); int MissionIsBlocked(MissionInfo &); ProfileManager *SetProfileMode(byte); void SelectMissionNumber(int,unsigned long &,unsigned long &); int LaunchMission(GameInfo *);
};
#pragma pack(pop)
extern RankCost rankCosts[];
inline ushort RankThreshold(int rank){return rankCosts[rank].cost;}
extern int MissionRandomTable[];
extern "C" unsigned long next_link_seed(int *);
int MissionBaseType();
int MissionCountVariants();
int MissionCountForType(int);
void MissionRecordInitialize(MissionRecord *, int, int, int, int, int);
void MissionDataInit(MissionRecord *, int, int, int, int, int, int);
void DecodeMission(MissionRecord *, MissionInfo *);
void *MissionAllocateType(int);
extern "C" void *MissionAllocate(int,int);
void UnitReset(UnitRecord *);
void ProfileAdvance(ProfileManager *, unsigned long);
void ProfileRefresh(ProfileManager *);

/* 0043ef34 */
void ResetProfileUnits(ProfileManager *p) {
    for (int i=0; i < p->unitCount; ++i) UnitReset(&p->units[i]);
}

/* 0043ef64: mission settlement, promotion, and debrief text. Field names are
 * inferred from arithmetic and report strings; unknown formatting tokens stay extern. */
#include <string.h>
struct DebriefStats { U32 outcome,field4,field8; int totalKills,kills,damaged,value,objectives,totalObjectives,field24; };
struct StrPool { char *GetString(long,int=1); };
extern StrPool reportStrings;
extern char g4dd0c0[];
extern U32 gSessionTable[];
extern char gDebugText[];
extern int missionExperienceScale, reportStatic;
extern char *savedReport;
extern char reportToken_0049417c[],reportToken_00494184[],reportToken_0049418c[],reportToken_00494194[],reportToken_004941a0[],reportToken_004941a4[],reportToken_004941a8[],reportToken_004941b0[],reportToken_004941b8[],reportToken_004941c0[],reportToken_004941c8[];
extern char reportBreak_0048af46[],reportBreak_0048af94[],reportBreak_0048afad[],reportBreak_0048afc2[],reportBreak_0048b078[],reportBreak_0048b07a[],reportBreak_0048b07c[],reportBreak_0048b142[];
struct RankDescriptor { ushort skip; byte unknown[10]; int name; };
extern RankDescriptor rankDescriptors[];
void LogicGetMissionSummary(unsigned char *,U32 *);
int ProfileUnitIsAssigned(ProfileManager *,int);
void UnitClearA(UnitRecord *); void UnitClearB(UnitRecord *);
void UnitAddExperience(UnitRecord *,int);
void PilotReset(void *,int);
unsigned SimLogicFlags(void *);
void ReportBuffer(void *,U32);
void DebugTextClear(); void DebugTextAppend(char *);
extern "C" char *BriefFormat(const char *,...);
extern "C" char *LogicTextPointer(void *);
byte PilotHealth(void *); byte UnitGrade(UnitRecord *);
unsigned UnitStat(UnitRecord *,unsigned);
void ProfileCleanup(ProfileManager *);


void GenerateMissions(ProfileManager *); void ReplaceMission(ProfileManager *);

void ProfileManager::BeginMissions() {
    MissionInfo initial;
    DebriefStats stats;
    int regenerate=0, promote=0;
    LogicGetMissionSummary((byte *)g4dd0c0,(U32 *)&stats);
    specialMission=missions[choice];
    DecodeMission(&missions[choice],&initial);
    int rate = initial.category==1 ? 80 : 30;
    int damageBonus=stats.value*((U32)(rate*stats.damaged)/100);
    int killBonus=stats.kills*stats.value;
    int baseBonus=initial.value;
    int reward=killBonus;
    if(stats.objectives==stats.totalObjectives) reward+=damageBonus;
    if(IsSpecial()) reward*=10;
    int objectiveBonus=stats.objectives*initial.value20;
    if(initial.category==1 && stats.kills==0 && stats.damaged==0) baseBonus=0;
    int earned=baseBonus+reward+objectiveBonus;
    int possible=initial.value+stats.totalObjectives*initial.value20+stats.totalKills*stats.value;
    ++field40c; ++missionsTotal;
    int experience=(initial.difficulty*missionExperienceScale+50)/100;
    for(int i=0;i<unitCount;++i) {
        UnitRecord *unit=&units[i];
        if(!ProfileUnitIsAssigned(this,i+1)) {UnitClearA(unit);UnitClearB(unit);unit->field1a=0;}
        else UnitAddExperience(unit,experience);
    }
    totalExperience+=initial.difficulty;
    for(i=0;i<pilotCount;++i) PilotReset(&pilots[i],23);
    switch(stats.outcome) {
    case 1:
        ++field408;++wins;++streak;
        killsTotal+=stats.kills;objectivesTotal+=stats.objectives;
        regenerate=1;money+=earned;
        if(IsSpecial() || initial.category!=2) ++progress; else progress+=3;
        if(rank!=14) {
            if(IsSpecial()) {
                if(rank!=13 && progress>=3) {promote=1;continuation=1;progress=0;initial.category=2;}
            } else if(initial.category==3) {
                progress=0;
                if(missionType<MissionBaseType()-1) {++missionType;field408=0;field40c=0;}
                regenerate=2;promote=1;continuation=1;
            } else if(rankCosts[rank].cost<=progress && rankCosts[rank].cost!=0) {promote=1;continuation=1;}
        }
        break;
    case 3: RefreshMission(choice); break;
    default: RefreshMission(choice); break;
    }
    if(initial.mission==0 || initial.mission==2 || initial.mission==3) regenerate=0;
    if(stats.outcome!=1) {
        for(i=0;i<pilotCount;++i) {
            unsigned unit=pilotUnits[i]&255;
            if(unit && !(SimLogicFlags(&units[unit-1])&1)) {
                for(int j=0;j<23;++j) pilots[i].skills[j].active=0;
            }
        }
    }
    ReportBuffer(gSessionTable,0x4000);DebugTextClear();
    DebugTextAppend(BriefFormat("%s%s%s%s\n",reportToken_004941c8,reportToken_004941a4,reportToken_0049418c,reportStrings.GetString(0x98)));
    DebugTextAppend(reportBreak_0048af46);
    DebugTextAppend(BriefFormat("%s%s%s: %s%s%s\n",reportToken_004941c8,reportToken_0049417c,reportStrings.GetString(0x99),reportToken_00494184,reportToken_004941b8,MissionAllocateType(initial.type)));
    DebugTextAppend(BriefFormat("%s%s: %s%s%s\n",reportToken_0049417c,reportStrings.GetString(0x9a),reportToken_00494184,reportToken_004941b8,MissionAllocate(initial.type,initial.number)));
    DebugTextAppend(BriefFormat("%s%s: %s%s%d %s\n",reportToken_0049417c,reportStrings.GetString(0xb5),reportToken_00494184,reportToken_004941b8,initial.difficulty,reportStrings.GetString(0xb6)));
    DebugTextAppend(BriefFormat("%s%s:",reportToken_0049417c,reportStrings.GetString(0xc5)));
    if(stats.outcome==3) {
        DebugTextAppend(BriefFormat("%s%s%s%s%s\n",reportToken_00494194,reportToken_004941a0,reportToken_004941b8,reportStrings.GetString(0xe2),reportToken_004941c8));
        if(!IsSpecial()) DebugTextAppend(BriefFormat("%s%s\n%s%s\n",reportToken_004941c8,reportToken_00494184,LogicTextPointer(g4dd0c0),reportToken_004941c8));
        DebugTextAppend(reportBreak_0048af94);
    } else if(stats.outcome==2) {
        DebugTextAppend(BriefFormat("%s%s%s%s%s\n",reportToken_00494194,reportToken_004941a0,reportToken_004941b8,reportStrings.GetString(0xe3),reportToken_004941c8));
        if(!IsSpecial()) DebugTextAppend(BriefFormat("%s%s\n%s%s\n",reportToken_004941c8,reportToken_00494184,LogicTextPointer(g4dd0c0),reportToken_004941c8));
        DebugTextAppend(reportBreak_0048afad);
    } else {
        DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_00494184,reportToken_004941b8,(char *)initial.image));
        if(!IsSpecial()) DebugTextAppend(BriefFormat("%s%s\n%s%s\n",reportToken_004941c8,reportToken_00494184,LogicTextPointer(g4dd0c0),reportToken_004941c8));
        DebugTextAppend(reportBreak_0048afc2);
        DebugTextAppend(BriefFormat("\n%s%s:\n",reportToken_0049417c,reportStrings.GetString(0xbc)));
        DebugTextAppend(BriefFormat("%s%s%s: %s%s%d %s%s\n",reportToken_004941a8,reportToken_0049417c,reportStrings.GetString(0xa0),reportToken_00494184,reportToken_004941b8,killBonus,reportToken_004941c0,reportStrings.GetString(0xb1)));
        if(stats.objectives==stats.totalObjectives) DebugTextAppend(BriefFormat("%s%s%s: %s%s%d %s%s\n",reportToken_004941a8,reportToken_0049417c,reportStrings.GetString(0xa2),reportToken_00494184,reportToken_004941b8,damageBonus,reportToken_004941c0,reportStrings.GetString(0xb1)));
        DebugTextAppend(BriefFormat("%s%s%s: %s%s%d %s%s\n",reportToken_004941a8,reportToken_0049417c,reportStrings.GetString(0xbf),reportToken_00494184,reportToken_004941b8,objectiveBonus,reportToken_004941c0,reportStrings.GetString(0xb1)));
        DebugTextAppend(BriefFormat("%s%s%s: %s%s%d %s%s\n",reportToken_004941a8,reportToken_0049417c,reportStrings.GetString(0xc2),reportToken_00494184,reportToken_004941b8,baseBonus,reportToken_004941c0,reportStrings.GetString(0xb1)));
        DebugTextAppend(BriefFormat("\n\n%s%s:\n",reportToken_0049417c,reportStrings.GetString(0xb8)));
        int percent=stats.kills && stats.totalKills ? stats.kills*100/stats.totalKills : 0;
        DebugTextAppend(BriefFormat("%s%s%s: %s%s%d%s%d%%\n",reportToken_004941a8,reportToken_0049417c,reportStrings.GetString(0xa1),reportToken_00494184,reportToken_004941b8,stats.kills,reportToken_004941c0,percent));
        percent=stats.objectives && stats.totalObjectives ? stats.objectives*100/stats.totalObjectives : 0;
        DebugTextAppend(BriefFormat("%s%s%s: %s%s%d%s%d%%\n",reportToken_004941a8,reportToken_0049417c,reportStrings.GetString(0x9e),reportToken_00494184,reportToken_004941b8,stats.objectives,reportToken_004941c0,percent));
        percent=earned && possible ? earned*100/possible : 0;
        DebugTextAppend(BriefFormat("\n%s%s%s: %s%s%d%s%s%d%%\n",reportToken_004941a8,reportToken_0049417c,reportStrings.GetString(0xb9),reportToken_00494194,reportToken_004941b8,earned,reportToken_004941c0,reportToken_00494184,percent));
        if(promote) {
            byte oldRank=rank,newRank=rank;
            if(initial.category==3) {do {++newRank;} while(rankDescriptors[newRank].skip>0);} else newRank=oldRank+1;
            char *name=reportStrings.GetString(rankDescriptors[newRank].name);
            DebugTextAppend(BriefFormat("\n%s%s%s%s",reportToken_00494194,reportToken_004941a8,reportStrings.GetString(0xf7),reportToken_004941b8));
            byte levels=newRank-oldRank;
            if(levels==1) DebugTextAppend(BriefFormat(reportStrings.GetString(0xf8),name));
            else DebugTextAppend(BriefFormat(reportStrings.GetString(0xf9),levels,name));
            DebugTextAppend(reportBreak_0048b078);
        }
        DebugTextAppend(reportBreak_0048b07a);
    }
    DebugTextAppend(reportBreak_0048b07c);
    DebugTextAppend(BriefFormat("%s%s%s:\n",reportToken_004941c8,reportToken_004941a4,reportStrings.GetString(0xb3)));
    DebugTextAppend(BriefFormat("%s\n",reportToken_004941c8));
    for(i=0;i<pilotCount;++i) {
        PilotRecord *pilot=&pilots[i];
        unsigned index=pilotUnits[i]&255;
        if(index) {
            
            unsigned flags=SimLogicFlags(GetUnit(index));
            DebugTextAppend(BriefFormat("%s%s%s  %s%s\n",reportToken_00494184,(char *)pilot,reportToken_0049417c,reportToken_004941c0,reportStrings.GetString(0xa5)));
            DebugTextAppend(BriefFormat("%s%s%s: %s%s",reportToken_0049417c,reportToken_004941a8,reportStrings.GetString(0xa8),reportToken_00494184,reportToken_004941b0));
            byte health=PilotHealth(pilot);
            if(health) DebugTextAppend(BriefFormat("%d%%\n",health));
            else DebugTextAppend(BriefFormat("%s%s\n",reportToken_00494194,reportStrings.GetString(0xaa)));
            DebugTextAppend(BriefFormat("%s%s%s  %s%s\n",reportToken_00494184,(char *)GetUnit(index),reportToken_0049417c,reportToken_004941c0,reportStrings.GetString(0xa6)));
            DebugTextAppend(BriefFormat("%s%s%s: %s",reportToken_0049417c,reportToken_004941a8,reportStrings.GetString(0x79),reportToken_004941b0));
            byte grade=UnitGrade(GetUnit(index));
            if(grade=='D') {
                if(flags&1) DebugTextAppend(BriefFormat("%s%s\n",reportToken_00494194,reportStrings.GetString(0xaf)));
                else DebugTextAppend(BriefFormat("%s%s\n",reportToken_00494194,reportStrings.GetString(0xc6)));
            } else if(grade=='C') DebugTextAppend(BriefFormat("%s%s\n",reportToken_00494194,reportStrings.GetString(0xae)));
            else if(grade=='B') DebugTextAppend(BriefFormat("%s%s\n",reportToken_00494184,reportStrings.GetString(0xad)));
            else DebugTextAppend(BriefFormat("%s%s\n",reportToken_00494184,reportStrings.GetString(0xac)));
            if(!(flags&1)) {
                if(flags&0x10000) DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_00494194,reportToken_004941a8,reportStrings.GetString(0x75)));
                else if(flags&0x100000) DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_00494194,reportToken_004941a8,reportStrings.GetString(0x76)));
                else DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_00494194,reportToken_004941a8,reportStrings.GetString(0x77)));
            } else {
                DebugTextAppend(BriefFormat("%s%s%s: %s",reportToken_0049417c,reportToken_004941a8,reportStrings.GetString(0x66),reportToken_004941b0));
                int damage=UnitStat(GetUnit(index),0xd4)*10-GetUnit(index)->wounds;
                char *description=reportStrings.GetString(damage==1 ? 0x71 : 0x72);
                if(flags&0x40000) {
                    DebugTextAppend(BriefFormat("%s%d %s\n",reportToken_00494194,damage,description));
                    DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_004941b0,reportToken_00494194,reportStrings.GetString(0x74)));
                } else if(flags&0x20000) {
                    DebugTextAppend(BriefFormat("%s%d %s\n",reportToken_00494184,damage,description));
                    DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_004941b0,reportToken_00494184,reportStrings.GetString(0x73)));
                } else DebugTextAppend(BriefFormat("%s%d %s\n",reportToken_00494184,damage,description));
            }
            if(GetUnit(index)->bytes26 != GetUnit(index)->bytes27) DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_004941a8,reportToken_00494184,reportStrings.GetString(0xab)));
            DebugTextAppend("\n\n");
        }
    }
    DebugTextAppend(reportBreak_0048b142);
    DebugTextAppend(BriefFormat("%s%s%s:\n",reportToken_004941c8,reportToken_004941a4,reportStrings.GetString(0xa7)));
    DebugTextAppend(BriefFormat("%s\n",reportToken_004941c8));
    int any=0;
    for(i=0;i<unitCount;++i) {
        unsigned number=i+1;
        if(!ProfileUnitIsAssigned(this,number)) {
            any=1; UnitRecord *base=units; UnitRecord *unit=&base[i];
            DebugTextAppend(BriefFormat("%s%s%s  %s%s\n",reportToken_00494184,(char *)unit,reportToken_0049417c,reportToken_004941c0,reportStrings.GetString(0xa6)));
            unsigned flags=SimLogicFlags(unit);
            if(!(flags&1)) {
                if(flags&0x10000) DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_00494194,reportToken_004941a8,reportStrings.GetString(0x75)));
                else if(flags&0x100000) DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_00494194,reportToken_004941a8,reportStrings.GetString(0x76)));
                else DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_00494194,reportToken_004941a8,reportStrings.GetString(0x77)));
            } else {
                DebugTextAppend(BriefFormat("%s%s%s: %s",reportToken_0049417c,reportToken_004941a8,reportStrings.GetString(0x66),reportToken_004941b0));
                int damage=UnitStat(unit,0xd4)*10-base[number-1].wounds;
                char *description=reportStrings.GetString(damage==1 ? 0x71 : 0x72);
                if(flags&0x40000) {
                    DebugTextAppend(BriefFormat("%s%d %s\n",reportToken_00494194,damage,description));
                    DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_004941b0,reportToken_00494194,reportStrings.GetString(0x74)));
                } else if(flags&0x20000) {
                    DebugTextAppend(BriefFormat("%s%d %s\n",reportToken_00494184,damage,description));
                    DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_004941b0,reportToken_00494184,reportStrings.GetString(0x73)));
                } else DebugTextAppend(BriefFormat("%s%d %s\n",reportToken_00494184,damage,description));
            }
            DebugTextAppend("\n\n");
        }
    }

    if(!any) DebugTextAppend(BriefFormat("%s%s\n",reportToken_004941c8,reportStrings.GetString(0xc6)));
    ProfileCleanup(this);
    if(promote) {
        previousRank=rank;
        if(initial.category==3) {do {++rank;ProfileAdvance(this,rank);} while(rankDescriptors[rank].skip>0);}
        else {++rank;ProfileAdvance(this,rank);}
    }
    ProfileRefresh(this);
    if(regenerate==2) GenerateMissions(this); else if(regenerate==1) ReplaceMission(this);
    if(!reportStatic) delete [] savedReport; else reportStatic=0;
    unsigned length=strlen(gDebugText)+1;
    savedReport=new char[length];
    memset(savedReport,0,length);strcpy(savedReport,gDebugText);
    field43a=(int)savedReport;choice=-1;
}


/* 004404a2 */
void ProfileManager::RefreshMission(int slot) {
    MissionInfo info;
    DecodeMission(&missions[slot], &info);
    unsigned long rank = this->rank;
    if (info.category == 3) {
        while ((int)rank < 14 && rankCosts[rank].cost != 0) ++rank;
    }
    MissionDataInit(&missions[slot], field426, field40c, field408, rank, field42a, 1);
}

/* 0044052b: nine explicit offerings, with an inlined RefreshMission per slot. */
extern int missionFinalIndex[];
void GenerateMissions(ProfileManager *p) {
    byte count=MissionCountForType(p->missionType);
    int category=p->missionType!=0;
    MissionRecord *mission;
    byte slot=0;
    mission=&p->missions[slot];
    MissionRecordInitialize(mission,category,p->missionType,next_link_seed(MissionRandomTable)%count,p->mode,0);
    p->RefreshMission(slot++);
    mission=&p->missions[slot];
    MissionRecordInitialize(mission,1,p->missionType,next_link_seed(MissionRandomTable)%count,p->mode,0);
    p->RefreshMission(slot++);
    mission=&p->missions[slot];
    MissionRecordInitialize(mission,1,p->missionType,next_link_seed(MissionRandomTable)%count,p->mode,1);
    p->RefreshMission(slot++);
    mission=&p->missions[slot];
    MissionRecordInitialize(mission,1,p->missionType,next_link_seed(MissionRandomTable)%count,p->mode,2);
    p->RefreshMission(slot++);
    mission=&p->missions[slot];
    MissionRecordInitialize(mission,2,p->missionType,next_link_seed(MissionRandomTable)%count,p->mode,0);
    p->RefreshMission(slot++);
    mission=&p->missions[slot];
    MissionRecordInitialize(mission,2,p->missionType,next_link_seed(MissionRandomTable)%count,p->mode,1);
    p->RefreshMission(slot++);
    mission=&p->missions[slot];
    MissionRecordInitialize(mission,2,p->missionType,next_link_seed(MissionRandomTable)%count,p->mode,2);
    p->RefreshMission(slot++);
    mission=&p->missions[slot];
    MissionRecordInitialize(mission,2,p->missionType,next_link_seed(MissionRandomTable)%count,p->mode,2);
    p->RefreshMission(slot++);
    next_link_seed(MissionRandomTable);
    mission=&p->missions[slot];
    MissionRecordInitialize(mission,3,p->missionType,missionFinalIndex[p->missionType],p->mode,2);
    p->RefreshMission(slot);
}

/* 00440bf5 */
void ReplaceMission(ProfileManager *p) {
    unsigned long count=MissionCountForType(p->missionType);
    byte choices=(byte)count;
    MissionInfo info;
    if (p->missionType==0) {
        DecodeMission(&p->missions[0],&info);
        if(info.category==0) {
            MissionRecordInitialize(&p->missions[0],1,p->missionType,next_link_seed(MissionRandomTable)%(byte)count,p->mode,0);
            p->RefreshMission(0);
        }
    }
    DecodeMission(&p->missions[p->choice],&info);
    MissionRecordInitialize(&p->missions[p->choice],info.category,p->missionType,next_link_seed(MissionRandomTable)%choices,p->mode,info.flags);
    p->RefreshMission(p->choice);
}

/* 00440dea */
void ProfileManager::SetMissionChoice(unsigned long selected) {
    if ((int)selected < 0 || (int)selected >= 9) selected = 0;
    choice = (short)selected;
}

/* 00440e05: this member-shaped helper has an unused first argument. */
void ProfileManager::SelectMissionNumber(int number, unsigned long &type, unsigned long &index) {
    type = 0; index = 0;
    int types = MissionBaseType();
    if (number < 0) {
        type = next_link_seed(MissionRandomTable) % types;
        int count = MissionCountVariants();
        index = next_link_seed(MissionRandomTable) % count;
    } else {
        byte base = 0;
        byte count = MissionCountForType(type);
        while ((int)type < types) {
            if (number < base + count) { index = number - base; break; }
            base += count;
            ++type;
            count = MissionCountForType(type);
        }
    }
}

/* 00440ea6 */
struct GameInfo { int fields[12]; };
void GetGameInfo(ProfileManager *p, GameInfo *g, MissionRecord *out) {
    unsigned long type=0, index=0;
    p->SelectMissionNumber(g->fields[11],type,index);
    int category;
    if(g->fields[6]>=10)category=7; else if(g->fields[6]==9)category=6; else if(g->fields[6]==8)category=5; else category=8;
    MissionRecordInitialize(out,category,type,index,0,1);
    MissionDataInit(out,g->fields[3],p->field40c,p->field408,g->fields[10],g->fields[6],g->fields[1]);
}

/* 00440fb6 */
void ApplyGameInfo(ProfileManager *p, MissionRecord *in) {
    MissionInfo info;
    DecodeMission(in,&info);
    p->missionType=info.type;
    p->field426=info.mission;
    p->quickMission=*in;
    p->choice=9;
}

/* 00441004 */
#include <stdio.h>
extern char *searchPaths[];
int FileExists(const char *);
int LoadMission(const char *,int);
void Problem(unsigned long,int);
int ProfileManager::LaunchMission(GameInfo *g) {
    char path[128];
    int found=0;
    for(int i=0; !found && searchPaths[i]; ++i) {
        sprintf(path,"%sqstart\\qs%03d.cbs",searchPaths[i],g->fields[3]);
        if(FileExists(path)) found=1;
    }
    if(!found) {Problem(0x12f,0);return 0;}
    if(!LoadMission(path,0)) {Problem(0x12f,0);return 0;}
    unsigned long type,index;
    SelectMissionNumber(g->fields[2],type,index);
    MissionRecord *mission=&quickMission;
    MissionRecordInitialize(mission,g->fields[1],type,index,0,g->fields[4]);
    MissionDataInit(mission,0,0,0,g->fields[3],0,1);
    choice=9;field426=0;return 1;
}

/* 004411a8 */
void ApplyProgress(ProfileManager *p, ProgressInfo *info) {
    for (p->rank=1; p->rank <= info->rank; ++p->rank) ProfileAdvance(p,p->rank);
    --p->rank;
    ProfileRefresh(p);
    p->money=info->money;
    p->oldMoney=info->money;
}

/* 00441202 */
int ProfileConstant() { return 18; }

/* 00441208 */
extern "C" MissionSummary *BriefLookup(ProfileManager *p, int selected) {
    MissionInfo info;
    if (selected == -9) DecodeMission(&p->specialMission,&info);
    else {
        if (selected < 0 || selected >= 9) selected=0;
        DecodeMission(&p->missions[selected],&info);
    }
    p->summary.type=(byte)info.type;
    p->summary.number=(byte)info.number;
    p->summary.category=info.category;
    p->summary.difficulty=info.difficulty;
    p->summary.value=info.value;
    p->summary.nextValue=info.value;
    p->summary.flags=info.flags;
    p->summary.blocked=p->MissionIsBlocked(info);
    p->summary.typeData=MissionAllocateType(info.type);
    p->summary.missionData=MissionAllocate(info.type,info.number);
    p->summary.image=info.image;
    p->summary.value34=info.value34;
    return &p->summary;
}

/* 00441331 */
int ProfileValue(ProfileManager *p) { return p->field43a; }

/* 0044133c */
int ProfileManager::MissionIsBlocked(MissionInfo &info) {
    if (rank==14) return 0;
    if (info.category==3) return 1;
    byte cost=1;
    if (info.category==2) cost=3;
    int threshold=RankThreshold(rank);
    int increment=cost;
    int next=progress;
    if (next+increment>=threshold && threshold) return 1;
    return 0;
}
/* 00441399 */
int ProfileManager::HasContinuation() {
    if (continuation != 0) { continuation=0; return 1; }
    return 0;
}

/* 004413b9 */
ProfileManager *ProfileManager::SetProfileMode(byte mode) {
    if (mode == 2) this->mode=mode; else if (mode == 1) this->mode=mode; else this->mode=0; return this;
}

























