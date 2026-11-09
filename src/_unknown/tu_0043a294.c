/* Packed layouts and names are working labels inferred from field accesses. */
#include <string.h>
typedef unsigned char U8;
typedef unsigned short U16;
#pragma pack(push,1)
struct HercSlot { U8 type,value,condition,active; int amount; };
struct ActorUnit {
    char name[17]; HercSlot slots[23]; U8 flagC9,flagCA;
    int armor[6]; int fieldE3,mass; U8 damage[32];
};
struct ItemHeader {
    U8 type,id; int field02; U8 nominal,minimum,field08,level,repair;
    int cost,field0f,amount,mass;
};
struct ActorItem : ItemHeader { union { int words[16]; U8 bytes[64]; signed char chars[64]; } detail; };
struct ItemTable { int kind; char *items; int size,stringBase,count; };
struct Mount { U8 allowed,field01; U8 weights[6]; U8 chance; U8 rest[13]; unsigned mask; };
struct Chassis : ItemHeader { Mount mounts[23]; U8 rest[100]; };
struct StrPool { long id; char *buffer; long capacity; void *data; long size;
 int Lock(); void Unlock(); char *GetString(long,int=0); char *GetSecondString(long,int); };
#pragma pack(pop)
extern ItemTable HercItemTables[];
extern U8 HercEfficiency[],WeaponEfficiency[],SpecialEfficiency[];
extern int HercMassScale;
extern StrPool gMenuPrefixStrings,gHercStrings,g511e18,g511ddc;
extern "C" int f_00439d02(ActorUnit *);
extern int f_0043ac9c(ActorUnit *,int,int);
extern int f_0043ad4f(ActorUnit *,int,int);
extern void Crc16(U16 *,void *,int);
extern void DebugTextClear();
extern void DebugTextAppend(char *);
extern "C" char *BriefFormat(char *,...);
extern char gDebugText[];

extern ActorItem *PopFindItem(ActorUnit *,int,int=-1);
extern int HercItemEfficiency(ActorUnit *,int);
extern int HercCalculateMass(ActorUnit *);
extern int HercCanSpend(ActorUnit *,int);
extern int HercPowerUse(ActorUnit *,int);
extern void HercResetAmount(ActorUnit *,int);
extern int gRedrawFlags,HercRepairScale,HercSensorBase;
extern "C" void LogicCallMode(int,void * =0);
extern int UpdatePayload(void *,int=0);
extern void SendEvent(void *,int,void * =0,int=0,int=0);
extern void *UnitFogObject(ActorUnit *,int,void *);
extern "C" unsigned next_link_seed(unsigned *);
extern unsigned HercRandomSeed;
extern int HercHitBias[];
struct SixBytes { U8 bytes[6]; };
extern SixBytes HercConditionColors[];
extern unsigned long gSessionTable[];
#define CLAMP(v,lo,hi) ((v)<=(lo)?(lo):((v)>=(hi)?(hi):(v)))
int HercWeaponDamage(ActorUnit *,int,int);
int HercDamageAtRange(ActorUnit *,int);
void HercBuildDamage(ActorUnit *);
/* 0043a294 */
int UnitSpend(ActorUnit *u,int amount){
 int *power=&u->slots[5].amount,*battery=&u->slots[6].amount;
 gRedrawFlags|=0x40;
 if(amount<=*power){*power-=amount;return 1;}
 else {amount-=*power;if(amount<=*battery){*power=0;*battery-=amount;return 1;}}return 0;
}
/* 0043a2d5 */
void HercAdvanceTurn(ActorUnit *u,void *actor){
 HercCalculateMass(u);
 ActorItem *p=PopFindItem(u,6,-1);int efficiency=HercItemEfficiency(u,6);
 int batteryMax=p->detail.words[0]*efficiency/100;
 p=PopFindItem(u,5,-1);efficiency=HercItemEfficiency(u,5);
 int generation=p->detail.words[0]*efficiency/100;
 p=PopFindItem(u,6,-1);efficiency=HercItemEfficiency(u,6);
 u->slots[6].amount+=p->detail.words[1]*efficiency/100+u->slots[5].amount;
 u->slots[5].amount=generation;
 u->slots[6].amount=CLAMP(u->slots[6].amount,0,batteryMax);
 if(actor){p=PopFindItem(u,10,-1);efficiency=HercItemEfficiency(u,10);
 SendEvent(actor,122,(void *)((p->detail.words[1]*efficiency+50)/100),(p->detail.words[2]*efficiency+50)/100+HercSensorBase,(p->detail.words[3]*efficiency+50)/100);}
 char shield[56];p=(ActorItem *)UnitFogObject(u,7,shield);
 u->slots[7].amount+=p->detail.words[2];u->slots[7].amount=CLAMP(u->slots[7].amount,0,p->detail.words[1]);
 u->slots[5].amount-=HercPowerUse(u,1);u->slots[5].amount=CLAMP(u->slots[5].amount,0,0x7fffffff);
 for(int i=0;i<8;i++)u->slots[i+11].active=0;
 for(i=0;i<4;i++){
 U8 *active=&u->slots[i+19].active;U8 old=*active;
 if(old && old!=255)*active=old-1;
 if(*active){int cost=PopFindItem(u,i+19,-1)->detail.words[7];if(HercCanSpend(u,cost))UnitSpend(u,cost);else *active=0;}
 if(old && !*active)LogicCallMode(PopFindItem(u,i+19)->detail.words[9]);
 }
 HercBuildDamage(u);
}
void HercBuildDamageCopy(ActorUnit *);
/* 0043a597 */
void HercBuildDamage(ActorUnit *u){HercBuildDamageCopy(u);}
/* 0043a606 */
void HercBuildDamageCopy(ActorUnit *u){memset(u->damage,0,32);for(int i=0;i<32;i++)u->damage[i]=HercDamageAtRange(u,i);}
/* 0043a670 */
int HercDamageAtRange(ActorUnit *u,int distance){int value=0;for(int i=0;i<8;i++)value+=HercWeaponDamage(u,distance,i);return value;}
/* 0043a69c */
int HercWeaponDamage(ActorUnit *u,int distance,int weapon){
 int value=0;weapon=CLAMP(weapon,0,7);ActorItem *p=PopFindItem(u,weapon+11,-1);
 if(p->id){
 value=(p->detail.words[5]+p->detail.words[6]+p->detail.words[7])>>1;
 int low=p->detail.chars[9];
 if(distance>low){int high=p->detail.chars[10];if(distance>=high)value=0;else {int span=high-low;value=(high-distance)*value/span;}}
 low=p->detail.chars[7];
 if(distance>low){int high=p->detail.chars[8];if(distance>=high)value=0;else {int span=high-low;value=(high-distance)*value/span;}}
 }return value;
}
/* 0043a739 */
int HercRepairPoints(ActorUnit *u){int value=0;for(int i=0;i<4;i++){ActorItem *p=PopFindItem(u,i+19,-1);if(p->detail.words[1]==2 && HercItemEfficiency(u,i+19))value+=u->slots[i+19].amount;}return value;}
inline int HercCondition(ActorUnit *u,int slot){return u->slots[slot].condition;}
/* 0043a782 */
int HercRepairNeeded(ActorUnit *u,int slot){
 if(slot==23){int value=0;for(int i=0;i<23;i++){int n=HercRepairNeeded(u,i);if(n!=100000)value+=n;}return value;}
 if(slot==1)return 100000;
 U8 condition=u->slots[slot].condition;if(!condition)return 100000;
 if(condition<100)return 1;return 0;
}
/* 0043a7e8 */
int f_0043a7e8(ActorUnit *u,int slot){
 if(slot==23){int value=0;for(int i=0;i<23;i++)value|=f_0043a7e8(u,i);return value;}
 int points=HercRepairPoints(u),need=HercRepairNeeded(u,slot);
 if(points<need || !need)return 0;
 u->slots[slot].condition++;
 while(--need!=-1){int repair=f_0043ac9c(u,2,1);if(repair){int *amount=&u->slots[repair].amount;if(*amount>0)--*amount;}}
 return 1;
}
/* 0043a90e */
int HercAmmoCost(ActorUnit *u,int n){
 int slot=f_0043ac9c(u,1,1);
 if(slot){ActorItem *p=PopFindItem(u,slot,-1);if(u->slots[slot].amount>=p->detail.words[4])return 0;
 int cost=(((p->detail.words[3]*n+p->detail.words[2])/100)*HercRepairScale+50)/100;
 if(cost<1)cost=1;return cost;}return 0;
}
/* 0043a97f */
int HercAddAmmo(ActorUnit *u,int n){
 int slot=f_0043ac9c(u,1,1);
 if(slot){ActorItem *p=PopFindItem(u,slot,-1);int percent=p->detail.words[5];n=percent*n/100;
 u->slots[slot].amount+=n;if(u->slots[slot].amount>=p->detail.words[4])u->slots[slot].amount=p->detail.words[4];return 1;}return 0;
}

/* 0043a9e7 */
int HercNeedsAmmo(ActorUnit *u){int slot=f_0043ac9c(u,1,1);if(slot){ActorItem *p=PopFindItem(u,slot,-1);if(u->slots[slot].amount>=p->detail.words[4])return 0;return 1;}return 0;}
/* 0043aa29 */
int StateAction(ActorUnit *u){int value=0;for(int i=0;i<4;i++){ActorItem *p=PopFindItem(u,i+19,-1);if(p->detail.words[1]==1)value+=u->slots[i+19].amount;}return value;}

/* 0043aa62 */
void SimAddMission(ActorUnit *u){
 HercResetAmount(u,23);extern void HercNoOperation(ActorUnit *);HercNoOperation(u);
 int raw=HercItemEfficiency(u,7);int efficiency;if(raw<=0)efficiency=0;else if(raw>=100)efficiency=100;else efficiency=raw;
 ActorItem *p=PopFindItem(u,7,-1);u->slots[7].amount=p->detail.words[1]*efficiency/100;
 p=PopFindItem(u,5,-1);efficiency=HercItemEfficiency(u,5);u->slots[5].amount=p->detail.words[0]*efficiency/100;
 p=PopFindItem(u,6,-1);efficiency=HercItemEfficiency(u,6);u->slots[6].amount=p->detail.words[0]*efficiency/100;
 u->flagCA=0;u->flagC9=u->flagCA;UpdatePayload(u);
 for(int i=0;i<4;i++)u->slots[i+19].active=PopFindItem(u,i+19,-1)->detail.bytes[42];
 HercAdvanceTurn(u,0);
}

/* 0043ab6e */
void *HercConditionDiagram(ActorUnit *u,ActorUnit *old){
 memset(gSessionTable,0,256);
 for(int i=0;i<23;i++){int condition=HercCondition(u,i),index=condition*4/100;
 if(old){U8 previous=old->slots[i].condition;if(condition!=previous)index=6;}
 memcpy((char *)gSessionTable+(16+i*6),&HercConditionColors[index],6);}
 return gSessionTable;
}


/* 0043ac02 */
void HercAngleWeights(int angle,int *a,int *b,int *x,int *y){
 *a=(angle%360)/60;*b=(*a+1)%6;*y=(angle%60)*100/60;*x=100-*y;
}
/* 0043ac9c */
int f_0043ac9c(ActorUnit *u,int type,int active){
 int best=0,result=0;
 for(int i=0;i<4;i++){
 ActorItem *p=PopFindItem(u,i+19,-1);
 if(type==p->detail.words[1] && (!active || u->slots[i+19].active || p->detail.bytes[41]==4) && HercItemEfficiency(u,i+19)){
 int amount=u->slots[i+19].amount,score=0;
 switch(type){case 0:break;case 1:score=p->detail.words[4]-amount;break;
 case 3:case 7:score=p->detail.words[2];break;case 2:case 8:score=amount;break;
 case 4:case 5:case 6:case 9:case 10:score=1;break;}
 if(score>best)result=i+19;
 }}return result;
}
/* 0043ad4f */
int f_0043ad4f(ActorUnit *u,int type,int base){
 int result=base;
 for(int i=0;i<4;i++){
 ActorItem *p=PopFindItem(u,i+19,-1);
 if(p->detail.words[1]==9 && type==p->detail.words[2] && (u->slots[i+19].active || p->detail.bytes[41]==4)){
 if(HercItemEfficiency(u,i+19))result+=base*p->detail.words[4]/100+p->detail.words[3]-base;
 }}return CLAMP(result,0,100000);
}
/* 0043adf6 */
int HercDefense(ActorUnit *u){
 int total=0,left=100;for(int i=0;i<4;i++){ActorItem *p=PopFindItem(u,i+19,-1);
 if(p->detail.words[1]==3 && (u->slots[i+19].active || p->detail.bytes[41]==4) && HercItemEfficiency(u,i+19)){
 int n=p->detail.words[2]*left/100;total+=n;left-=n;
 }}return total;
}
inline int HercHeavyModel(ActorUnit *u){return u->slots[0].value>8;}

/* 0043ae77 */
int HercSelectHit(ActorUnit *u,int direction){
 if(HercHeavyModel(u))return 0;
 int thresholds[23],a,b,x,y;
 HercAngleWeights(direction/0xb60b61,&a,&b,&x,&y);
 unsigned total=0;Chassis *c=(Chassis *)PopFindItem(u,0,-1);
 for(int i=0;i<23;i++){
 Mount *m=&c->mounts[i];
 if(!m->allowed || !u->slots[i].condition)thresholds[i]=0x7fffffff;
 else {int weight=m->weights[a]*x/100+m->weights[b]*y/100+HercHitBias[i];thresholds[i]=total;total+=weight;}
 }
 if(!total)return 0;
 int random=next_link_seed(&HercRandomSeed)%total,selected=0;
 for(i=0;i<23;i++)if(random>=thresholds[i])selected=i;
 int chance=c->mounts[selected].chance;
 if(chance){chance=chance*HercItemEfficiency(u,0)/100;if(next_link_seed(&HercRandomSeed)%100<chance)selected=0;}
 return selected;
}




