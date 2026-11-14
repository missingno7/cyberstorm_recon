/* actor.h: the actor record cluster (ActorRecord 0x98, AnimAction 0x67 at +0x20, AnimBlock 0x494, ActorUnit
 * 0x10b). Lane ty1 pilot. Offsets are proven by original instructions (table in candidates/ty1/REPORT.md);
 * names are working labels. unknownXX ranges are not yet proven by any instruction.
 * No function prototypes here: only types and accessors that exist as inlined code in the original. */
#ifndef ACTOR_H_TY1
#define ACTOR_H_TY1
#pragma pack(push, 1)

/* Map/screen point (8 bytes). simdo/simgad/simmain/actor.c declared the member-wise copy constructor; no
 * converted file changes bytes with or without it (anim.c measured both). Assignment is the implicit one
 * (anim.c constructor: movsd pair). */
struct GridPoint {
    long x, y;
    GridPoint() {}
    GridPoint(long px, long py) : x(px), y(py) {}
    GridPoint(const GridPoint &p) : x(p.x), y(p.y) {}
    void Copy(const GridPoint &p) { *this = p; }
    int Equal(const GridPoint &p) const { return x == p.x && y == p.y; }
    int operator!=(const GridPoint &p) const { return x != p.x || y != p.y; }
};

/* Rectangle (draw rectangle of an action at +0x3b). The empty default constructor is needed: without it
 * simactor SimInvalidateActors (static GuiRect locals) loses STRICT. */
struct GuiRect {
    long x, y, width, height;
    GuiRect() {}
    int Moved(const GuiRect &r) const { return x != r.x || y != r.y; }
};

struct ActorRecord;

/* Path search block owned by an action (+0x32), 0x494 bytes (AnimActionCreateBlock). */
struct AnimBlock {
    long path[128];             /* +000 */
    long cost[128];             /* +200 */
    unsigned char marks[0x80];  /* +400 */
    long budget;                /* +480 */
    long count;                 /* +484 */
    long index;                 /* +488 */
    long unknown48c;            /* +48c */
    ActorRecord *actor;         /* +490 */
};

/* Animation action, 0x67 bytes, member of ActorRecord at +0x20; constructor 00457510 (sim\anim.c). */
struct AnimAction {
    long depth;                 /* +00 draw depth */
    unsigned char type;         /* +04 */
    unsigned char state;        /* +05 */
    unsigned char angle;        /* +06 */
    unsigned char rate;         /* +07 */
    unsigned char frame;        /* +08 */
    unsigned char count;        /* +09 */
    unsigned char flags;        /* +0a */
    signed char ready;          /* +0b */
    unsigned short kind;        /* +0c */
    unsigned long time;         /* +0e */
    GridPoint position;         /* +12 */
    GridPoint target;           /* +1a */
    long resource;              /* +22 grid index of position */
    long direction;             /* +26 */
    GridPoint screen;           /* +2a */
    AnimBlock *block;           /* +32 */
    unsigned char special;      /* +36 anim.c: pending special move */
    unsigned char random;       /* +37 random phase (actoradd seed % 100; simactor frame offset) */
    signed char b38, b39;       /* +38 */
    signed char color;          /* +3a player color (actoradd; simmini indexes a table with it: movsx) */
    GuiRect rect;               /* +3b */
    int *bitmap;                /* +4b */
    GridPoint from;             /* +4f */
    GridPoint to;               /* +57 */
    long step;                  /* +5f */
    long steps;                 /* +63 */
    AnimAction();
    void SetState(int s) { state = s; }
    long Width() { return rect.width; }
    void SetHeading(int n) { direction = n * 0x2aaaaabc; }
    int IsMoving() { return state != 1 && state != 7 && state != 14 && state != 17 && state != 18; }
};

/* Herc equipment slot (8 bytes) and herc unit record (0x10b bytes). */
struct UnitSlot {
    unsigned char type;         /* +0 */
    unsigned char item;         /* +1 installed item id (0 = empty slot); slot 0: chassis type */
    unsigned char condition;    /* +2 condition percent (0 = destroyed) */
    unsigned char uses;         /* +3 uses/charge this turn */
    int amount;                 /* +4 ammunition / energy units */
    unsigned char Condition() { return condition; } /* inlined in SimDamageActor: lea this; mov al,[this+2] */
};
struct ActorUnit {
    char name[17];              /* +000 */
    UnitSlot slots[23];         /* +011 */
    unsigned char shieldDirection; /* +0c9 shield facing (pshield) */
    unsigned char shieldRange;  /* +0ca shield range (pshield) */
    int armor[6];               /* +0cb per-facing armor / shield arcs */
    int fieldE3;                /* +0e3 */
    int mass;                   /* +0e7 */
    unsigned char damage[32];   /* +0eb */
    void SetDirection(int value) { shieldDirection = value; }
    unsigned short GetStatus() { return slots[10].condition; } /* ppilot DrawDetails: wider return than the byte */
};

/* Pilot record: outside this cluster; each file completes it. */
struct ActorPilot;

/* Actor record, 0x98 bytes (actorTable entries). */
struct ActorRecord {
    long id;                    /* +00 */
    long previous;              /* +04 */
    long owner;                 /* +08 */
    long targetId;              /* +0c */
    long source;                /* +10 unit source: <= -9999 custom, 0 own, > 0 mission (actorlst) */
    long target;                /* +14 */
    ActorUnit *unit;            /* +18 */
    ActorPilot *pilot;          /* +1c */
    AnimAction action;          /* +20 */
    signed char done;           /* +87 */
    signed char active;         /* +88 */
    signed char hidden;         /* +89 */
    unsigned char flag8a;       /* +8a set to 1 by ActorListAdd */
    unsigned char flag8b;       /* +8b set to 1 by ActorListAdd */
    unsigned char unknown8c;    /* +8c */
    signed char spent;          /* +8d action spent this turn (actor.c crouch/stand, simdo) */
    unsigned char returnFire;   /* +8e return-fire slot mask (simdo) */
    long relatedIndex;          /* +8f */
    long related;               /* +93 */
    unsigned char blink;        /* +97 */
    ActorUnit *Unit() { return unit; }
    ActorPilot *Pilot() { return pilot; }
    long Owner() const { return owner; }
    long Index() { return action.resource; }
    long RelatedIndex() const { return relatedIndex; }
    int Active() { return active; }
    int Hidden() { return hidden; }
    int Ready() const { return active && !hidden; }
    int IsVisible() { return !hidden && action.state != 17; }
    int Spent() { return spent; }
    int ReturnFire() { return returnFire; }
    const char *UnitText() const { return (const char *)unit; }
    const char *PilotText() const { return (const char *)pilot; }
};

#pragma pack(pop)
#endif
