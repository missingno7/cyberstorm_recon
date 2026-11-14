/* Packed layouts and names are working labels inferred from field accesses. */
#include <string.h>
typedef unsigned char U8;
typedef unsigned short U16;
#pragma pack(push,1)
#include "actor.h"




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

extern int f_0043ac9c(ActorUnit *,int,int);
extern int f_0043ad4f(ActorUnit *,int,int);
extern void Crc16(U16 *,void *,int);
extern void DebugTextClear();
extern void DebugTextAppend(char *);
extern "C" char *BriefFormat(char *,...);
extern char gDebugText[];

extern ActorItem *PopFindItem(ActorUnit *,int,int);
extern void SimAddMission(ActorUnit *);
extern signed char HercDamageWeights[];
extern "C" int f_0043d152(void *,int);
struct SessionLevel { char unknown[0x43e]; U8 level; int Level() { return level; } };
#pragma pack(push,1)
struct WeaponEntry : ItemHeader { int words[10]; U8 end[3]; };
struct SpecialEntry : ItemHeader { int words[10]; U8 end[4]; };
struct Choice { char *name,*shortName,*description; int flags; ActorItem *item; };
struct ChoiceNode { int key[6]; int flags; ActorItem *item; ChoiceNode *next; ChoiceNode(){next=0;} };
struct Loadout { int name; U8 unknown[20]; U8 values[23]; };
#pragma pack(pop)
extern WeaponEntry HercWeapons[];
extern SpecialEntry HercSpecials[];
void HercResetAmount(ActorUnit *,int);
inline int HercCondition(ActorUnit *u,int slot){return u->slots[slot].condition;}
/* 004394e0 */
extern "C" int f_004394e0(ActorUnit *u,int slot){
 int cost=0;
 if(slot==23){for(int i=0;i<23;i++)cost+=f_004394e0(u,i);}
 else {ActorItem *p=PopFindItem(u,slot,-1);
int damage=100-HercCondition(u,slot);damage*=p->repair;cost=damage*p->cost/100.0/100.0;
}
 return cost;
}
/* 00439563 */
int HercRepairUnitCost(ActorUnit *u,int slot){
 int cost=0;
 if(slot==23){for(int i=0;i<23;i++)cost+=HercRepairUnitCost(u,i);}
 else if(HercCondition(u,slot)!=100){ActorItem *p=PopFindItem(u,slot,-1);cost=p->repair*p->cost/10000;}
 return cost;
}
/* 004395c6 */
int HercResetAmountZero(ActorUnit *u,int slot){HercResetAmount(u,slot);return 0;}
/* 0043960a */
int HercSaleValue(ActorUnit *u,int slot){
 int value=0;
 if(slot==23){for(int i=0;i<23;i++)value+=HercSaleValue(u,i);}
 else {ActorItem *p=PopFindItem(u,slot,-1);HercResetAmountZero(u,slot);int cost=p->cost;int repair=f_004394e0(u,slot);return cost-repair;}
 return value;
}

/* 00439706 */
extern "C" void f_00439706(ActorUnit *u,int slot){
 if(slot==23){for(int i=0;i<23;i++)f_00439706(u,i);return;}
 u->slots[slot].condition=100;
}
/* 00439736 */
extern "C" int f_00439736(ActorUnit *u,int slot){
 int step=HercCondition(u,slot);
 step=step+10<=100?10:100-step;
 ActorItem *p=PopFindItem(u,slot,-1);
 int cost=step*p->repair*p->cost/100.0/100.0;
 if(cost<=0)cost=(step*2)*p->repair*p->cost/100.0/100.0;
 return cost;
}
/* 004397d3 */
extern "C" int f_004397d3(ActorUnit *u,int slot){
 if(slot==23){for(int i=0;i<23;i++)f_004397d3(u,i);return 0;}
 int step;
 if(u->slots[slot].condition+10<=100){step=10;ActorItem *p=PopFindItem(u,slot,-1);int cost=10*p->repair*p->cost/100.0/100.0;if(cost<=0)step=20;}
 else step=100-u->slots[slot].condition;
 U8 *condition=&u->slots[slot].condition;
 if(*condition<100)*condition+=step;
 *condition=*condition<=100?*condition:100;
 return *condition;
}
/* 004398a8 */
void HercResetAmount(ActorUnit *u,int slot){
 if(slot==23){for(int i=0;i<23;i++)HercResetAmount(u,i);return;}
 ActorItem *p=PopFindItem(u,slot,-1);u->slots[slot].amount=p->amount;
}
/* 004398e4 */
void HercAddChoiceNode(ActorUnit *u,ChoiceNode **list,int slot,int value,int *count){
 if(!*list || (*list)->item->id!=value){
 ChoiceNode *p=new ChoiceNode;memset(p,0,sizeof(*p));
 ActorItem *item=PopFindItem(u,slot,value);int id=HercItemTables[slot].stringBase+value;
 p->key[1]=id;p->key[3]=id;p->key[5]=id;p->flags=-1;p->item=item;
 if(!*list)*list=p;
 else {ChoiceNode *tail=*list;while(tail->next)tail=tail->next;tail->next=p;}
 ++*count;
 }
}
/* 00439997 */
ChoiceNode *HercListItems(ActorUnit *u,int slot,SessionLevel *session,int *count){
 Chassis chassis; ItemHeader item; ChoiceNode *list; unsigned mask;
 if(!slot || slot<0 || slot>=23)return 0;
 {
 list=0;*count=0;
 chassis=*(Chassis *)PopFindItem(u,0,-1);
 if(!chassis.mounts[slot].allowed)return 0;
 int current=u->slots[slot].item;
 if(current)HercAddChoiceNode(u,&list,slot,current,count);
 if((slot>=11 && slot<=18) || (slot>=19 && slot<=22))HercAddChoiceNode(u,&list,slot,0,count);
 mask=chassis.mounts[slot].mask;
 if(slot>=11 && slot<=18){
 for(int i=1;i<76;i++){
 
 if((mask&HercWeapons[i].words[0]) && f_0043d152(session,HercWeapons[i].id+651) && session->Level()>=HercWeapons[i].level)HercAddChoiceNode(u,&list,slot,i,count);
 }return list;
 }
 if(slot>=19 && slot<=22){
 for(int i=1;i<35;i++){
 
 if((mask&HercSpecials[i].words[0]) && f_0043d152(session,HercSpecials[i].id+801) && session->Level()>=HercSpecials[i].level)HercAddChoiceNode(u,&list,slot,i,count);
 }return list;
 }
 for(int i=1;i<32;i++){
 if(i>=HercItemTables[slot].count)break;
 item=*(ItemHeader *)PopFindItem(u,slot,i);
 if((mask&(1<<item.id)) && f_0043d152(session,HercItemTables[slot].stringBase+i) && session->level>=item.level)HercAddChoiceNode(u,&list,slot,i,count);
 }return list;
 }
}
/* 00439cad */
int HercInstallItem(ActorUnit *u,int slot,ActorItem *p){
 UnitSlot *s=&u->slots[slot];s->type=p->type;s->item=p->id;s->condition=100;s->uses=0;s->amount=p->amount;
 SimAddMission(u);return HercItemTables[slot].stringBase+s->item;
}
/* 00439d02 */
extern "C" int f_00439d02(ActorUnit *u){return u->slots[0].item;}
/* 00439d0c */
char *HercChassisName(ActorUnit *u){int id=HercItemTables[0].stringBase+u->slots[0].item;return gHercStrings.GetSecondString(id,1);}
/* 00439d2c */
extern "C" char *f_00439d2c(ActorUnit *u){return u->name;}
/* 00439d31 */
extern "C" void f_00439d31(ActorUnit *u,char *s){strncpy(u->name,s,17);}
inline int DamageClamp(int damage){return damage<=0?0:(damage>=100?100:damage);}
/* 00439d48 */
extern "C" int f_00439d48(ActorUnit *u){
 int value=10000;for(int i=0;i<23;i++){
 int damage=DamageClamp(100-u->slots[i].condition);
 value-=damage*value*HercDamageWeights[i]/10000;
 }return (value+50)/100;
}
/* 00439daf */
extern "C" int f_00439daf(ActorUnit *u){
 unsigned maximum=0;for(int i=0;i<23;i++){ActorItem *p=(ActorItem *)(HercItemTables[i].items+HercItemTables[i].size*u->slots[i].item);if(p->level>maximum)maximum=p->level;}return maximum;
}
/* 00439df6 */
extern "C" void LoadActorData(ActorUnit *u,Loadout *loadout,char *name){
 memset(u,0,sizeof(*u));for(int i=0;i<23;i++){
 UnitSlot *s=&u->slots[i];s->type=i;s->item=loadout->values[i];s->condition=100;s->uses=0;s->amount=PopFindItem(u,i,-1)->amount;
 }SimAddMission(u);
 if(!name){char *text=gHercStrings.GetSecondString(loadout->name,1);strncpy(u->name,text,16);}
 else strncpy(u->name,name,16);
}
/* 00439e9a */
void HercAddChoice(ActorUnit *u,Choice *list,int slot,int value,int *count,int capacity){
 if(list && *count<capacity){
 for(int i=0;i<capacity;i++){if(!list[i].item)break;if(list[i].item->id==value)return;}
 Choice *p=&list[*count];ActorItem *item=PopFindItem(u,slot,value);int id=HercItemTables[slot].stringBase+value;
 p->name=gHercStrings.GetSecondString(id,1);p->shortName=gHercStrings.GetString(id,1);p->description=g511e18.GetString(id,1);p->flags=-1;p->item=item;++*count;
 }
}


/* 00439f6e */
int HercFillItems(ActorUnit *u,int slot,SessionLevel *session,Choice *list,int capacity){
 Chassis chassis; ItemHeader item; int count; unsigned mask;
 if(!slot || slot<0 || slot>=23)return 0;
 {
 
 chassis=*(Chassis *)PopFindItem(u,0,-1);
 if(!chassis.mounts[slot].allowed)return 0;
 count=0;
 int current=u->slots[slot].item;
 if(current)HercAddChoice(u,list,slot,current,&count,capacity);
 if((slot>=11 && slot<=18) || (slot>=19 && slot<=22))HercAddChoice(u,list,slot,0,&count,capacity);
 mask=chassis.mounts[slot].mask;
 if(slot>=11 && slot<=18){
 for(int i=1;i<76;i++){
 
 if((mask&HercWeapons[i].words[0]) && f_0043d152(session,HercWeapons[i].id+651) && session->Level()>=HercWeapons[i].level)HercAddChoice(u,list,slot,i,&count,capacity);
 }return count;
 }
 if(slot>=19 && slot<=22){
 for(int i=1;i<35;i++){
 
 if((mask&HercSpecials[i].words[0]) && f_0043d152(session,HercSpecials[i].id+801) && session->Level()>=HercSpecials[i].level)HercAddChoice(u,list,slot,i,&count,capacity);
 }return count;
 }
 for(int i=1;i<32;i++){
 if(i>=HercItemTables[slot].count)break;
 item=*(ItemHeader *)PopFindItem(u,slot,i);
 if((mask&(1<<item.id)) && f_0043d152(session,HercItemTables[slot].stringBase+i) && session->level>=item.level)HercAddChoice(u,list,slot,i,&count,capacity);
 }return count;
 }
}



