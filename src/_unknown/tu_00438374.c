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
inline int HercMin(int a,int b){return a<b?a:b;}
#define HercClamp(a,lo,hi) ((a)<=(lo)?(lo):((a)>=(hi)?(hi):(a)))
/* 00438374 */
ActorItem *PopFindItem(ActorUnit *u,int slot,int value) {
 if(value<0) value=u->slots[slot].value;
 ItemTable *t=&HercItemTables[slot];
 return (ActorItem *)(t->items+t->size*value);
}
/* 004383b3 */
int HercMountAllowed(ActorUnit *u,int slot) {
 Chassis *c=(Chassis *)PopFindItem(u,0,-1);
 return c->mounts[slot].allowed!=0;
}
/* 004383e1 */
int PopItemReady(ActorUnit *u,int slot){
int r=HercMountAllowed(u,slot);if(r)return u->slots[slot].value!=0;return r;
}
/* 0043842f */
int HercItemEfficiency(ActorUnit *u,int slot) {
 ActorItem *p=PopFindItem(u,slot,-1); int condition=u->slots[slot].condition;
 if(condition<30){int type=f_00439d02(u);if(type>=9 && type<=17) condition=30;}
 if(condition<=p->minimum) return 0;
 int value=100;
 if(condition<p->nominal) value=condition*100/p->nominal;
 value=HercEfficiency[slot]*value/100;
 if(p->type==11) value=WeaponEfficiency[p->detail.chars[4]]*value/100;
 else if(p->type==19) value=SpecialEfficiency[p->detail.words[1]]*value/100;
 return value;
}
/* 004384f4 */
int HercClearField(ActorUnit *u){u->fieldE3=0;return 0;}
/* 00438505 */
int HercCalculateMass(ActorUnit *u){
 u->mass=0;
 for(int i=0;i<23;i++) u->mass+=PopFindItem(u,i,-1)->mass;
 int scale=HercMassScale; int slot=f_0043ac9c(u,5,1);
 if(slot){ActorItem *p=PopFindItem(u,slot,-1);scale-=p->detail.words[2];
 if(scale<p->detail.words[3])scale=HercMin(HercMassScale,p->detail.words[3]);}
 if(scale<100)scale=(scale+100)/2;
 if(scale<=20)scale=20;else if(scale>=500)scale=500;
 u->mass=u->mass*scale/100;
 u->mass=HercClamp(u->mass,100,0x7fffffff);
 return u->mass;
}
/* 00438602 */
int StateInvoke(ActorUnit *u,int *current,int *maximum){
 int value=u->slots[5].amount;if(current)*current=value;
 if(maximum)*maximum=PopFindItem(u,5,-1)->detail.words[0];return value;
}
/* 0043863d */
int StateInvokeEnergy(ActorUnit *u,int *current,int *maximum){
 int value=u->slots[6].amount;if(current)*current=value;
 if(maximum)*maximum=PopFindItem(u,6,-1)->detail.words[0];return value;
}
/* 00438678 */
int HercEnergy(ActorUnit *u){return u->slots[5].amount+u->slots[6].amount;}
/* 00438683 */
int HercCanSpend(ActorUnit *u,int amount){return HercEnergy(u)>=amount;}
/* 00438699 */
char *HercCategoryName(ActorItem *p){return gMenuPrefixStrings.GetString(p->type+1001,1);}
/* 004386b7 */
char *HercCategoryShort(ActorItem *p){return gMenuPrefixStrings.GetString(p->type+1025,1);}
/* 004386c8 */
char *HercCategoryPlural(ActorItem *p){return gMenuPrefixStrings.GetString(p->type+1049,1);}
/* 004386d9 */
char *HercCategoryUnits(ActorItem *p){return gMenuPrefixStrings.GetString(p->type+1073,1);}
/* 004386ea */
char *HercItemName(ActorItem *p){
 char *text=gHercStrings.GetString(HercItemTables[p->type].stringBase+p->id,1);
 if(text && !text[0] && text[1])text++;return text;
}
/* 00438730 */
char *PopItemValue(ActorItem *p){
 char *text=gHercStrings.GetString(HercItemTables[p->type].stringBase+p->id,1),*second=text;
 if(text){second+=strlen(text)+1;if(*second)text=second;}return text;
}
/* 0043877f */
char *HercItemDescription(ActorItem *p){return g511e18.GetString(HercItemTables[p->type].stringBase+p->id,1);}
/* 004387af */
void HercAppendAmount(HercSlot *slot,int id){
 if(slot) DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(id),slot->amount));
}

extern U8 HercBurstWeights[];
int HercMobility(ActorUnit *);
/* 004387e6 */
char *HercFormatItem(ActorUnit *u,ActorItem *p,int slot,unsigned flags){
 int wasOpen=gMenuPrefixStrings.data!=0;
 if(!wasOpen)gMenuPrefixStrings.Lock();
 HercSlot *installed=0;
 if(slot>=0 && (flags&0x20))installed=&u->slots[slot];
 DebugTextClear();
 if(flags&1){
 DebugTextAppend(gMenuPrefixStrings.GetString(slot+1001,0));
 DebugTextAppend("\n    ");DebugTextAppend(HercItemName(p));DebugTextAppend("\n\n");
 }
 if((flags&0x800) && installed){DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(1252,0),installed->condition));DebugTextAppend("\n\n");}
 if(flags&0x100){
 int before=HercMobility(u);
 ActorUnit copy=*u;HercSlot *s=&copy.slots[slot];
 s->value=p->id;s->condition=100;s->active=1;s->amount=0;
 int after=HercMobility(&copy);
 int id=after<before?1244:(after>before?1245:1246);
 if(flags&0x1000)DebugTextAppend(gMenuPrefixStrings.GetString(1238,0));
 DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(id,0),before,after));DebugTextAppend("\n\n");
 }
 if(flags&8){
 if(flags&0x4000)DebugTextAppend(gMenuPrefixStrings.GetString(1240,0));
 DebugTextAppend(HercItemDescription(p));DebugTextAppend("\n\n");
 }
 if(flags&0x10){
 if(flags&0x8000)DebugTextAppend(gMenuPrefixStrings.GetString(1241,0));
 switch(p->type){
 
case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:case 21:case 22:break;
case 0:DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(1226,0),200-p->field02,0,0,0,0,0,0));break;
 case 1:DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(1227,0),p->detail.words[8]+2*p->detail.words[10]));break;
 case 2:DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(1228,0),p->detail.words[0]));break;
 case 3:break;
 case 4:DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(1229,0),200-p->field02));break;
 case 5:HercAppendAmount(installed,1247);DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(1230,0),p->detail.words[0]));break;
 case 6:HercAppendAmount(installed,1248);DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(1231,0),p->detail.words[0]));break;
 case 7:HercAppendAmount(installed,1249);DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(1232,0),p->detail.words[1],p->detail.words[2],p->detail.words[3]));break;
 case 8:DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(1233,0),p->detail.words[1],gMenuPrefixStrings.GetString(1272+p->detail.words[2],0),p->detail.words[0]));break;
 case 9:DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(1234,0),p->detail.words[1],p->detail.words[2],p->detail.words[0]));break;
 case 10:DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(1235,0),p->detail.words[1],p->detail.words[2],p->detail.words[3],p->detail.words[0]));break;
 case 11:
 if(p->id){
 if(p->amount){HercAppendAmount(installed,1250);DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(1251,0),p->amount));}
 int shots=p->detail.chars[36];
 DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(1236,0),p->detail.chars[6],p->detail.chars[5],p->detail.words[4],p->detail.words[5]*shots,p->detail.words[6]*shots,p->detail.words[7]*shots,g511ddc.GetString(p->detail.chars[13]+124,0)));
 int burst=p->detail.chars[33];
 if(burst>1)DebugTextAppend(BriefFormat(gMenuPrefixStrings.GetString(1254,0),HercBurstWeights[burst]));
 if(p->detail.chars[11]==0)DebugTextAppend(gMenuPrefixStrings.GetString(1253,0));
 }break;
 }
 DebugTextAppend("\n");
 }
 if(flags&6){
 if(flags&0x2000)DebugTextAppend(gMenuPrefixStrings.GetString(1239,0));
 if(flags&2){
 if(p->type==19){DebugTextAppend(gMenuPrefixStrings.GetString(p->detail.words[1]+1257,0));DebugTextAppend("  ");}
 DebugTextAppend(HercCategoryPlural(p));DebugTextAppend("  ");
 }
 if(flags&4){
 DebugTextAppend(HercCategoryUnits(p));
 if(p->type==11){
 if(p->detail.chars[15]&1)DebugTextAppend(gMenuPrefixStrings.GetString(1097,0));
 if(p->detail.chars[15]&2)DebugTextAppend(gMenuPrefixStrings.GetString(1098,0));
 if(p->detail.chars[15]&4)DebugTextAppend(gMenuPrefixStrings.GetString(1099,0));
 if(p->detail.chars[15]&8)DebugTextAppend(gMenuPrefixStrings.GetString(1100,0));
 }
 }DebugTextAppend("\n\n");
 }
 if(flags&0x200){
 if(flags&0x400)DebugTextAppend("\x86" "1\x8f");
 DebugTextAppend(gMenuPrefixStrings.GetString(1242,0));DebugTextAppend(gMenuPrefixStrings.GetString(slot+1278,0));
 if(flags&0x400)DebugTextAppend("\x86" "0\x8f");
 }
 if(!wasOpen)gMenuPrefixStrings.Unlock();return gDebugText;
}
/* 00438dac */
void HercNoOperation(ActorUnit *){}
/* Later functions in the same plain-oe object. Formatter is reconstructed below. */
/* 004390e9 */
int HercPowerUse(ActorUnit *u,int damaged){
 int a=100,b=100,c=100,d=100;
 if(damaged){a=HercItemEfficiency(u,7);b=HercItemEfficiency(u,8);c=HercItemEfficiency(u,9);d=HercItemEfficiency(u,10);}
 int value=PopFindItem(u,7,-1)->detail.words[3]*a/100;
 value+=PopFindItem(u,8,-1)->detail.words[0]*b/100;
 value+=PopFindItem(u,9,-1)->detail.words[0]*c/100;
 value+=PopFindItem(u,10,-1)->detail.words[0]*d/100;return value;
}
/* 00439218 */
int HercFreePower(ActorUnit *u){
 int current,maximum;StateInvoke(u,&current,&maximum);
 int value=maximum-HercPowerUse(u,0);
 for(int i=0;i<4;i++){
 int slot=i+19; ActorItem *p=PopFindItem(u,slot,-1);
 if(p->detail.bytes[41]==4) value-=p->detail.words[7];
 }return value;
}

/* 004392a8 */
int HercMobility(ActorUnit *u){
 int base=PopFindItem(u,2,-1)->detail.words[0];
 ActorItem *p=PopFindItem(u,5,-1);
 int available=p->detail.words[0]-HercPowerUse(u,0);
 int mass=HercCalculateMass(u);
 if(!base)return 0;
 base=f_0043ad4f(u,2,base);return available*base*10/(mass*16);
}

/* 00439333 */
int HercUnsupported(){return -1;}
/* 00439339 */
int BlockChecksum(ActorUnit *u){U16 sum[2];sum[0]=0;Crc16(sum,u,sizeof(*u));return sum[0];}
/* 00439363 */
int HercEqual(ActorUnit *a,ActorUnit *b){
 int same=1;
 if(memcmp(a->name,b->name,sizeof(a->name)))same=0;
 if(memcmp(a->damage,b->damage,sizeof(a->damage)))same=0;
 if(a->mass!=b->mass)same=0;
 if(a->fieldE3!=b->fieldE3)same=0;
 if(a->flagC9!=b->flagC9)same=0;
 if(a->flagCA!=b->flagCA)same=0;
 for(int i=0;i<6;i++)if(a->armor[i]!=b->armor[i])same=0;
 for(i=0;i<23;i++)if(memcmp(&a->slots[i],&b->slots[i],sizeof(HercSlot)))same=0;
 return same;
}
/* 00439438 */
void HercCopy(ActorUnit *d,ActorUnit *s){
 strcpy(d->name,s->name);memcpy(d->damage,s->damage,sizeof(d->damage));
 d->mass=s->mass;d->fieldE3=s->fieldE3;d->flagC9=s->flagC9;d->flagCA=s->flagCA;
 for(int i=0;i<6;i++)d->armor[i]=s->armor[i];
 for(i=0;i<23;i++)memcpy(&d->slots[i],&s->slots[i],sizeof(HercSlot));
}





