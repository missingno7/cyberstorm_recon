/* Provisional briefing fragment, 0043e3ac..0043ecf8. It shares session and
 * report data with adjacent ProfileManager objects; exact TU boundaries remain
 * uncertain. No embedded path. All semantic names below are working labels. */
#include <string.h>
typedef unsigned char U8;
struct MissionRecord {unsigned long words[22];};
struct MissionInfo {int type,number,category,unknown0c,flags,mission,difficulty,value,value20,image,value28,value2c,value30,value34;int rest[9];};
struct ProfileManager {int fields[2];MissionRecord missions[10];U8 unknown378[0x6fc7-888],known[128],oldKnown[128];int MissionIsBlocked(MissionInfo &);};
struct StrPool {char *GetString(long,int=1);};
extern StrPool g511ddc,gHercStrings;
extern unsigned long gSessionTable[];
extern char gDebugText[];
extern int g4941e4,g4941e8;
extern char *g4c9344,*g4c9348;
struct MissionCategory {int name;char *text;};
extern MissionCategory missionCategories[];
struct EquipmentCategory {int first,count,unknown[3];};
extern EquipmentCategory equipmentCategories[];
extern char reportToken_0049417c[],reportToken_00494184[],reportToken_0049418c[],reportToken_004941a0[],reportToken_004941a8[],reportToken_004941b0[],reportToken_004941c8[];
void DecodeMission(MissionRecord *,MissionInfo *);
void *MissionAllocateType(int);
extern "C" void *MissionAllocate(int,int);
void FormatSetBuffer(void *,unsigned);
void DebugTextClear();void DebugTextAppend(char *);
extern "C" char *BriefFormat(const char *,...);

extern "C" char *BriefMissionFile(ProfileManager *p,int index) {
    MissionInfo info;
    if(index<0 || index>9) return 0;
    DecodeMission(&p->missions[index],&info);
    int blocked=p->MissionIsBlocked(info);
    FormatSetBuffer(gSessionTable,0x4000);DebugTextClear();
    DebugTextAppend(BriefFormat("%s%s%s%s\n",reportToken_004941c8,reportToken_004941a0,reportToken_0049418c,g511ddc.GetString(0x97)));
    DebugTextAppend("\n");
    DebugTextAppend(BriefFormat("%s%s%s:%s%s%s\n",reportToken_004941c8,reportToken_0049417c,g511ddc.GetString(0x99),reportToken_004941b0,reportToken_00494184,MissionAllocateType(info.type)));
    DebugTextAppend(BriefFormat("%s%s:%s%s%s\n",reportToken_0049417c,g511ddc.GetString(0x9a),reportToken_004941b0,reportToken_00494184,MissionAllocate(info.type,info.number)));
    DebugTextAppend(BriefFormat("%s%s:%s%s%s\n",reportToken_0049417c,g511ddc.GetString(0xc7),reportToken_004941b0,missionCategories[info.category].text,(info.category<0 || info.category>=4 ? g511ddc.GetString(0xc9) : g511ddc.GetString(missionCategories[info.category].name))));
    DebugTextAppend(BriefFormat("%s%s:%s%s%s\n",reportToken_0049417c,g511ddc.GetString(0xb4),reportToken_004941b0,reportToken_00494184,info.image));
    DebugTextAppend(BriefFormat("%s%s:%s%s%d %s\n",reportToken_0049417c,g511ddc.GetString(0xb5),reportToken_004941b0,reportToken_00494184,info.difficulty,g511ddc.GetString(0xb6)));
    DebugTextAppend(BriefFormat("%s%s:%s%s",reportToken_0049417c,g511ddc.GetString(0xb7),reportToken_004941b0,reportToken_00494184));
    if(blocked) DebugTextAppend(BriefFormat("%s\n",g511ddc.GetString(0xba)));
    else DebugTextAppend(BriefFormat("%s\n",g511ddc.GetString(0xc6)));
    DebugTextAppend("\n");
    DebugTextAppend(BriefFormat("%s%s:\n",reportToken_0049417c,g511ddc.GetString(0xbc)));
    if(info.value28) DebugTextAppend(BriefFormat("%s%s%s:%s%s%s\n",reportToken_0049417c,reportToken_004941a8,g511ddc.GetString(0x9f),reportToken_004941b0,reportToken_00494184,info.value28));
    DebugTextAppend(BriefFormat("%s%s%s:%s%s%d %s\n",reportToken_0049417c,reportToken_004941a8,g511ddc.GetString(0xbf),reportToken_004941b0,reportToken_00494184,info.value20,g511ddc.GetString(0xc0)));
    if(info.value) {
        DebugTextAppend("\n");
        DebugTextAppend(BriefFormat("%s%s:\n",reportToken_0049417c,g511ddc.GetString(0xc2)));
        DebugTextAppend(BriefFormat("%s%s%s:%s%s%d %s\n",reportToken_0049417c,reportToken_004941a8,g511ddc.GetString(0xc3),reportToken_004941b0,reportToken_00494184,info.value,g511ddc.GetString(0xb1)));
        if(info.value2c) DebugTextAppend(BriefFormat("%s%s%s:%s%s%s\n",reportToken_0049417c,reportToken_004941a8,g511ddc.GetString(0xc4),reportToken_004941b0,reportToken_00494184,info.value2c));
    }
    if(info.value30) {
        DebugTextAppend("\n");
        DebugTextAppend(BriefFormat("%s",reportToken_004941c8));
        DebugTextAppend((char *)info.value30);DebugTextAppend("\n");
    }
    if(info.value34) {
        DebugTextAppend("\n");
        DebugTextAppend(BriefFormat("%s%s\n",reportToken_004941c8,info.value34));
    }
    DebugTextAppend("\n");DebugTextAppend("\n");
    DebugTextAppend(BriefFormat("%s%s%s: %s\n",reportToken_0049417c,reportToken_004941a0,g511ddc.GetString(0xdc),info.rest[2]));
    DebugTextAppend("\n");
    DebugTextAppend(BriefFormat("%s",reportToken_004941c8));
    DebugTextAppend((char *)info.rest[3]);DebugTextAppend("\n");
    DebugTextAppend("\n");
    DebugTextAppend(BriefFormat("%s%s:\n",reportToken_0049417c,g511ddc.GetString(0xde)));
    DebugTextAppend(BriefFormat("%s%s%d%% %s(%s)\n",reportToken_00494184,reportToken_004941a8,info.rest[0],reportToken_0049417c,info.rest[1]));
    DebugTextAppend("\n");
    DebugTextAppend(BriefFormat("%s%s:\n",reportToken_0049417c,g511ddc.GetString(0xdf)));
    DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_0049417c,reportToken_004941a8,info.rest[7]));
    DebugTextAppend("\n");
    DebugTextAppend(BriefFormat("%s%s:\n",reportToken_0049417c,g511ddc.GetString(0xe0)));
    DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_0049417c,reportToken_004941a8,info.rest[8]));
    DebugTextAppend("\n");
    if(!g4941e4) delete [] g4c9344;else g4941e4=0;
    unsigned length=strlen(gDebugText)+1;
    g4c9344=new char[length];memset(g4c9344,0,length);strcpy(g4c9344,gDebugText);
    return g4c9344;
}
char *BriefNewEquipment(ProfileManager *p) {
    int count=128,first=1,lastCategory=-1;
    int lower=equipmentCategories[0].first,upper=lower+equipmentCategories[0].count;
    for(int i=0;i<count;++i) {
        U8 changed=p->known[i]^p->oldKnown[i];
        if(!changed) continue;
        if(first) {
            DebugTextAppend(BriefFormat("\n\n%s%s%s%s\n",reportToken_004941c8,reportToken_004941a0,g511ddc.GetString(0xa4),reportToken_004941c8));first=0;
        }
        for(int bit=0;bit<8;++bit) {
            if(!(changed&(1<<bit))) continue;
            int item=i*8+bit;
            if(item>=lower && item<=upper) continue;
            if(item>=51 && item<81 && lastCategory!=50) {
                lastCategory=50;
                DebugTextAppend(BriefFormat("\n%s%s\n",reportToken_004941c8,gHercStrings.GetString(lastCategory)));
            } else {
                for(int category=0;category<23;++category) {
                    int begin=equipmentCategories[category].first;
                    if(item>=begin && item<begin+equipmentCategories[category].count && lastCategory!=begin-1) {
                        lastCategory=begin-1;
                        DebugTextAppend(BriefFormat("\n%s%s\n",reportToken_004941c8,gHercStrings.GetString(lastCategory)));break;
                    }
                }
            }
            DebugTextAppend(BriefFormat("%s%s%s\n",reportToken_004941a8,reportToken_00494184,gHercStrings.GetString(item)));
        }
    }
    if(!g4941e8) delete [] g4c9348;else g4941e8=0;
    unsigned length=strlen(gDebugText)+1;
    g4c9348=new char[length];memset(g4c9348,0,length);strcpy(g4c9348,gDebugText);
    return g4c9348;
}


