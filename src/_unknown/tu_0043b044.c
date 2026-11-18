#include <string.h>

struct ActorUnit;
struct ActorItem;
struct ItemTable {
    int kind;
    char *items;
    int size, stringBase, count;
};

extern ItemTable HercItemTables[];
ActorItem *PopFindItem(ActorUnit *, int slot, int item = -1);
int HercItemEfficiency(ActorUnit *, int slot);
int f_0043ad4f(ActorUnit *, int slot, int value);

#pragma pack(push, 1)
struct WeaponStats {
    char header[0x20];
    signed char threshold, range, nearRange, farRange, falloffNear, falloffFar;
    char unknown26[3];
    signed char accuracy;
    unsigned char scaledFields;
    int energy, armorDamage, directDamage, penetrationDamage;
};

struct FogObject {
    char header[0x1b];
    union {
        int speed;
        int generation;
        struct { int capacity, recharge; } battery;
        struct { int field0, capacity, recharge, drain; } shield;
        struct { int strength, range; } sensor;
        int values[4];
    };
};
#pragma pack(pop)

// Equipment records share a packed header. Each slot interprets the statistics after it.
#define ITEM_STAT(buffer, member) (((FogObject *)(buffer))->member)
#define SCALE_FIELD(field, percent) ((field) = ((field) * (percent) + 50) / 100)

inline int ItemSize(int slot)
{
    return HercItemTables[slot].size;
}

inline int Lesser(int a, int b)
{
    return a < b ? a : b;
}

FogObject *UnitFogObject(ActorUnit *unit, int slot, void *buffer)
{
    ActorItem *item = PopFindItem(unit, slot);
    int efficiency = HercItemEfficiency(unit, slot);
    // The drive, shield and sensor also receive equipment bonuses at full efficiency.
    const unsigned bonusSlots = (1U << 2) | (1U << 7) | (1U << 8);
    if (efficiency == 100 && !((1U << slot) & bonusSlots))
        return (FogObject *)item;

    memcpy(buffer, item, ItemSize(slot));
    switch (slot) {
    case 2: {
        int left = HercItemEfficiency(unit, 3);
        int right = HercItemEfficiency(unit, 4);
        int legs = Lesser(left, right);
        efficiency = Lesser(legs, efficiency);
        ITEM_STAT(buffer, speed) = f_0043ad4f(unit, 2, ITEM_STAT(buffer, speed));
        SCALE_FIELD(ITEM_STAT(buffer, speed), efficiency);
        break;
    }
    case 5:
        SCALE_FIELD(ITEM_STAT(buffer, generation), efficiency);
        break;
    case 6:
        SCALE_FIELD(ITEM_STAT(buffer, battery.capacity), efficiency);
        SCALE_FIELD(ITEM_STAT(buffer, battery.recharge), efficiency);
        break;
    case 7:
        SCALE_FIELD(ITEM_STAT(buffer, shield.drain), efficiency);
        ITEM_STAT(buffer, shield.recharge) = f_0043ad4f(unit, 7, ITEM_STAT(buffer, shield.recharge));
        SCALE_FIELD(ITEM_STAT(buffer, shield.recharge), efficiency);
        break;
    case 8:
        ITEM_STAT(buffer, sensor.range) = f_0043ad4f(unit, 8, ITEM_STAT(buffer, sensor.range));
        SCALE_FIELD(ITEM_STAT(buffer, sensor.strength), efficiency);
        SCALE_FIELD(ITEM_STAT(buffer, sensor.range), efficiency);
        if (ITEM_STAT(buffer, sensor.range) < 3)
            ITEM_STAT(buffer, sensor.range) = 3;
        break;
    case 9:
        SCALE_FIELD(ITEM_STAT(buffer, values[0]), efficiency);
        SCALE_FIELD(ITEM_STAT(buffer, values[1]), efficiency);
        SCALE_FIELD(ITEM_STAT(buffer, values[2]), efficiency);
        break;
    case 10:
        SCALE_FIELD(ITEM_STAT(buffer, values[0]), efficiency);
        SCALE_FIELD(ITEM_STAT(buffer, values[1]), efficiency);
        SCALE_FIELD(ITEM_STAT(buffer, values[2]), efficiency);
        SCALE_FIELD(ITEM_STAT(buffer, values[3]), efficiency);
        break;
    case 11: case 12: case 13: case 14:
    case 15: case 16: case 17: case 18: {
        WeaponStats &w = *(WeaponStats *)buffer;
        unsigned char fields = w.scaledFields;
        if ((unsigned)fields & 1)
            SCALE_FIELD(w.accuracy, efficiency);
        if ((unsigned)fields & 2)
            SCALE_FIELD(w.threshold, efficiency);
        if ((unsigned)fields & 4) {
            SCALE_FIELD(w.range, efficiency);
            SCALE_FIELD(w.nearRange, efficiency);
            SCALE_FIELD(w.farRange, efficiency);
            SCALE_FIELD(w.falloffNear, efficiency);
            SCALE_FIELD(w.falloffFar, efficiency);
        }
        if ((unsigned)fields & 8) {
            SCALE_FIELD(w.armorDamage, efficiency);
            SCALE_FIELD(w.directDamage, efficiency);
            SCALE_FIELD(w.penetrationDamage, efficiency);
        }
        break;
    }
    case 0: case 1: case 3: case 4:
    case 19: case 20: case 21: case 22:
    default:
        break;
    }
    return (FogObject *)buffer;
}
