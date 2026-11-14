static char projSourceFile[]=__FILE__;
// Unit directional armor, redistribution and indicator colors. Names are provisional.
#pragma pack(1)
struct UnitArmor { int at00, total; };
struct Unit { char pad00[0x49]; UnitArmor armor; char pad51[0x78]; unsigned char direction, bias; int facingArmor[6]; };
struct ActorUnit;
struct ActorItem { char pad00[0x1b]; signed char *typeFactor; int strength; };
struct Weapon { char at00; unsigned char kind; char pad02[0x2d]; int armorDamage, directDamage, penetrationDamage; signed char overflow; };
#pragma pack()
struct DamagePoint { int x,y; DamagePoint() {} DamagePoint(int a,int b) { x=a; y=b; } };
extern ActorItem *PopFindItem(ActorUnit *, int, int = -1);
extern long GridClamp(unsigned long);
extern unsigned int ISqrt(unsigned int);
extern void PointPolar(DamagePoint *, int, int);
extern unsigned int TimerNow();
unsigned char damageLevelColors[]={224,223,222,221,220,219,218,217,216,215}, damageFlashColors[]={79,78};
extern unsigned char damageIndicator[];
inline int UnitScaledDamage(Unit *u, int damage, Weapon *w) { return PopFindItem((ActorUnit *)u, 7)->typeFactor[w->kind] * damage / 50; }
void UnitDamageSplit(Unit *u, int range, register int damage, register Weapon *w, int &armor, int &body)
{
    damage = UnitScaledDamage(u, damage, w);
    int shield = w->armorDamage * damage / 100; int direct = w->directDamage * damage / 100; int penetrate = w->penetrationDamage * damage / 100;


    int overflow = w->overflow;
    int health = u->facingArmor[GridClamp(range)];
    if (health == 0) { armor = 0; body = direct; }
    else {
        armor = shield; body = penetrate;
        if (shield > health) { body = penetrate + (shield - health) * overflow / 100; armor = health; }
    }
}
int UnitDamage(Unit *u, int range, register int damage, register Weapon *w)
{
    damage = PopFindItem((ActorUnit *)u, 7)->typeFactor[w->kind] * damage / 50;
    int shield = w->armorDamage * damage / 100; int direct = w->directDamage * damage / 100; int penetrate = w->penetrationDamage * damage / 100;


    int overflow = w->overflow;
    int facing = GridClamp(range);
    int health = u->facingArmor[facing];
    int removed = 0, result;
    if (health == 0) result = direct;
    else {
        removed = shield; result = penetrate;
        if (shield > health) { result = penetrate + (shield - health) * overflow / 100; removed = health; }
    }
    if (removed) {
        u->facingArmor[facing] -= removed;
        if (u->facingArmor[facing] <= 0) u->facingArmor[facing] = 0;
        u->armor.total -= removed;
        if (u->armor.total <= 0) u->armor.total = 0;
    }
    return result;
}
int UnitHealth(Unit *u, int range) { return u->facingArmor[GridClamp(range)]; }
void UpdatePayload(Unit *u, int unused)
{
    static DamagePoint centers[6] = { DamagePoint(0,64), DamagePoint(56,32), DamagePoint(56,-32), DamagePoint(0,-64), DamagePoint(-56,-32), DamagePoint(-56,32) };
    int weights[6]; DamagePoint at;
    PointPolar(&at, u->bias, (unsigned int)u->direction << 24);
    int total = u->armor.total, sum = 0;
    DamagePoint *p = centers;
    for (int i=0; i<6; i++,p++) {
        int x=at.x-p->x, y=at.y-p->y;
        int d=ISqrt(x*x+y*y);
        weights[i]=d*d; sum += weights[i];
    }
    for (int j=0; j<6; j++) u->facingArmor[j] = (weights[j] * total + sum/2) / sum;
}
unsigned char *UnitDamageIndicator(Unit *u, int direction, Unit *previous)
{
    unsigned int time=TimerNow()/1500; unsigned int phase=time&1;
    int maximum = PopFindItem((ActorUnit *)u,7)->strength / 4;
    Unit *p=u, *old=previous;
    Unit *end=(Unit *)((char *)u+24);
    do {
        int level=p->facingArmor[0] * 10 / maximum;
        if (level >= 10) level=9;
        unsigned char color = p->facingArmor[0] == 0 ? 0 : damageLevelColors[level];
        if (previous && old->facingArmor[0] != p->facingArmor[0]) color=255;
        damageIndicator[1+direction%6]=color;
        unsigned char flash = p->facingArmor[0] != 0 ? 0 : damageFlashColors[phase];
        damageIndicator[7+direction%6]=flash;
        p=(Unit *)((char *)p+4);old=(Unit *)((char *)old+4);direction++;
    } while (p!=end);
    return damageIndicator;
}
