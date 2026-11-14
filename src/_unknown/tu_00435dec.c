/* Unnamed actor-state object. All names and field meanings are working labels.
 * Packed offsets are evidenced by loads/stores; unknown fields stay explicit. */
typedef unsigned char U8;
typedef unsigned int U32;
extern "C" void *memset(void *, int, unsigned int);
extern "C" char *strncpy(char *, const char *, unsigned int);
extern "C" void *memcpy(void *, const void *, unsigned int);
extern const char actorListDescriptor[];
struct ActorState;
void RefreshActorState(ActorState *);
void RefreshActorFlags(ActorState *);
void PrepareActorDescriptor(void *); extern "C" void f_00435e43(ActorState *, U32);
struct PilotType; extern PilotType *gPilotTypes[];
struct Curve { U32 value[8]; };
void CurveInit(Curve *, int,int,int,int,int,int,int,int);
U32 CurveInverse(Curve *, U32);
U32 CurveForward(Curve *, U32);
U32 CurveSlope(Curve *, U32);
void ActorAdjustFatigue(ActorState *, int);
void ActorAdjustMorale(ActorState *, int);
void ActorRecoverInjuries(ActorState *, U32);
void ActorTick(ActorState *, int,int,int);
void ActorAdjustSkill(ActorState *, int, int);
void ActorAdjustSkills(ActorState *, int);
#pragma pack(push, 1)
struct ActorDescriptor {
    U8 marker; char name[12]; U8 endurance;
    U32 value; U8 health, morale, threshold, fatigue, skill;
    union { U8 baseSkills[0x80]; struct { U8 initialSkills[24]; U32 levelThresholds[26]; }; }; /* 0x17..0x96: exact fields not yet named */
    U8 initialLevel, maximumLevel; U8 unknown99[12]; U8 disabled;
    U8 *rules;
    U32 curves[64];
};
struct SkillValue { U32 value, maximum; };
struct ActorState {
    char name[16]; U8 unknown10; U8 damage;
    int health, morale, fatigue, skill; U32 experience;
    U8 level, originalLevel, ticks, active;
    U32 flags; signed char injuries[8]; SkillValue skills[8];
    ActorDescriptor *descriptor; int descriptorIndex; void ActorApplyDamage(U8); int Equal(const ActorState *);
};
struct ActorListObject : ActorState {
    ActorListObject(); void Fixup();
};
struct ActorListGlobalObject : ActorState {
    ActorListGlobalObject(const void *); ~ActorListGlobalObject();
};
#pragma pack(pop)

ActorListObject::ActorListObject() { memset(this, 0, 0x7e); }
ActorListGlobalObject::~ActorListGlobalObject() {}
ActorListGlobalObject::ActorListGlobalObject(const void *source) {
    memset(this, 0, 0x7e);
    descriptor = (ActorDescriptor *)source;
    descriptorIndex = -1;
    RefreshActorState(this);
}
int ActorBaseHealth(ActorState *, ActorDescriptor *d) {
    int value = d->health;
    if (value > 99) value = 99;
    return value;
}
void f_00435e43(ActorState *s, U32 index) {
    if (index < 30) {
        s->descriptorIndex = index;
        index = (U32)gPilotTypes[index];
    } else s->descriptorIndex = -1;
    s->descriptor = (ActorDescriptor *)index;
    RefreshActorState(s);
}
void ActorBindDescriptor(ActorState *s, int index) {
    if (index == -1) { s->descriptor = (ActorDescriptor *)actorListDescriptor; return; }
    s->descriptorIndex = index;
    s->descriptor = (ActorDescriptor *)gPilotTypes[index];
    PrepareActorDescriptor(s->descriptor);
}
void RefreshActorState(ActorState *s) {
    strncpy(s->name, s->descriptor->name, 12);
    s->level = s->originalLevel = s->descriptor->initialLevel;
    s->damage = 0; s->fatigue = 0; s->experience = 0;
    int health = s->descriptor->health;
    if (health > 99) health = 99;
    health = health + s->level - 1;
    if (health >= 100) health = 99;
    s->health = health;
    int morale = s->descriptor->morale + s->level - 1;
    if (morale >= 100) morale = 99;
    s->morale = morale;
    s->skill = (int)s->descriptor->skill / 10;
    s->active = 1; s->flags = 0;
    memset(s->injuries, 0, 8);
    s->ticks = 0; s->active = 1;
    memset(s->descriptor->curves, 0, 0x100);
    s->skills[0].value = (U32)s->descriptor->baseSkills[0] << 12;
    s->skills[0].maximum = (U32)s->descriptor->baseSkills[1] << 12;
    s->skills[1].value = (U32)s->descriptor->baseSkills[4] << 12;
    s->skills[1].maximum = (U32)s->descriptor->baseSkills[5] << 12;
    s->skills[2].value = (U32)s->descriptor->baseSkills[8] << 12;
    s->skills[2].maximum = (U32)s->descriptor->baseSkills[9] << 12;
    s->skills[3].value = (U32)s->descriptor->baseSkills[12] << 12;
    s->skills[3].maximum = (U32)s->descriptor->baseSkills[13] << 12;
    s->skills[4].value = (U32)s->descriptor->baseSkills[16] << 12;
    s->skills[4].maximum = (U32)s->descriptor->baseSkills[17] << 12;
    s->skills[5].value = (U32)s->descriptor->baseSkills[20] << 12;
    s->skills[5].maximum = (U32)s->descriptor->baseSkills[21] << 12;
    s->skills[6].value = (U32)s->descriptor->baseSkills[24] << 12;
    s->skills[6].maximum = (U32)s->descriptor->baseSkills[25] << 12;
    PrepareActorDescriptor(s->descriptor);
    RefreshActorFlags(s);
}
int ActorStateInvalid(ActorState *s) {
    int level = s->level;
    if (level < 0 || level > 26 || level > s->descriptor->maximumLevel + 10) return 1;
    if (s->health > 200 || s->morale > 200) return 1;
    if (s->skill != 0 && !s->descriptor->skill) return 1;
    if (s->skill > 6) return 1;
    return 0;
}

void ActorWork(ActorState *); void ActorRest(ActorState *);
int ActorScale(ActorState *, int, U32);
void ActorAdjustScalar(ActorState *, U32 *, int, U32);
void ActorChangeMorale(ActorState *, int);
void ActorTrainSkill(ActorState *, U32, int); void ActorGainExperience(ActorState *, int);
int ActorTrainingCost(ActorState *, U32, int);
int ActorHealthCost(ActorState *); void ActorRestoreHealth(ActorState *);
int ActorMoraleCost(ActorState *); void ActorRestoreMorale(ActorState *);
int ActorFatigueCost(ActorState *); int ActorRemainingValue(ActorState *);
void ActorRememberLevel(ActorState *); 
void ActorDamageHealth(ActorState *, int); void ActorZeroVitals(ActorState *); void ActorVoice(ActorState *);
long SendEvent(void *object, int eventCode, void *argument, int arg2, int arg3) {
    ActorState *s=(ActorState *)object;
    U32 event=eventCode;
    int value=(int)argument;
    if (s->descriptor->disabled && event > 103) { RefreshActorFlags(s); return 0; }
    switch(event) {
    case 100: f_00435e43(s, value); break;
    case 101: return (int)s->descriptor;
    case 102: strncpy(s->name, (char *)value, 16); break;
    case 103: strncpy((char *)value, s->name, 16); break;
    case 104: ActorWork(s); break;
    case 105: ActorRest(s); break;
    case 106:
        ActorAdjustScalar(s, (U32 *)&s->morale, -ActorScale(s, s->descriptor->rules[9], 217), 0);
        if (s->morale <= 0 && s->descriptor->health > 99) s->morale = 1;
        break;
    case 107:
        ActorAdjustScalar(s, (U32 *)&s->morale, -ActorScale(s, s->descriptor->rules[10], 217), 0);
        if (s->morale <= 0 && s->descriptor->health > 99) s->morale = 1;
        break;
    case 108: ActorChangeMorale(s, value); break;
    case 109: s->active = 0; break;
    case 110: ActorTrainSkill(s, value, arg2); break;
    case 111: ActorGainExperience(s, value); break;
    case 113: case 114: return ActorTrainingCost(s, value, event);
    case 115: return ActorHealthCost(s);
    case 116: ActorRestoreHealth(s); break;
    case 117: return ActorMoraleCost(s);
    case 118: ActorRestoreMorale(s); break;
    case 119: return ActorFatigueCost(s);
    case 120: s->fatigue = 0; break;
    case 121: return ActorRemainingValue(s);
    case 122: ActorTick(s, value, arg2, arg3); break;
    case 123: ActorRememberLevel(s); break;
    case 124: s->ActorApplyDamage(value); break;
    case 125: ActorDamageHealth(s, value); break;
    case 126: ActorZeroVitals(s); break;
    case 127: ActorVoice(s); break;
    case 128: ((ActorListObject *)s)->Fixup(); break;
    }
    RefreshActorFlags(s);
    return 0;
}
void ActorListObject::Fixup() {
    if (descriptorIndex == -1) descriptor = (ActorDescriptor *)actorListDescriptor;
    else { descriptor = (ActorDescriptor *)gPilotTypes[descriptorIndex]; PrepareActorDescriptor(descriptor); }
}
void ActorAdjustFixed(ActorState *, U32 *value, int delta, U32 maximum) {
    if (delta < 0) {
        delta = -delta;
        *value -= delta << 12;
    }
    else *value += delta << 12;
    *value = *value > maximum ? maximum : *value;
}
void ActorAdjustScalar(ActorState *, U32 *value, int delta, U32 maximum) {
    *value += delta;
    if (delta >= 0) *value = maximum < *value ? maximum : *value;
    else *value = (int)maximum > (int)*value ? maximum : *value;
}
int ActorScale(ActorState *s, int value, U32 event) {
    int result;
    switch (event) {
    case 201: case 205: case 209: {
        int adjusted = s->level + value - 1;
        result = adjusted < 100 ? adjusted : 99;
        break;
    }
    case 217: {
        int adjusted = (value * 10000 -
            ((s->level * 100) / (int)s->descriptor->maximumLevel) *
            ((value * 100) / 2)) / 10000;
        result = adjusted > 0 ? adjusted : 1;
        break;
    }
    default: result = 0; break;
    }
    return result;
}
void RefreshActorFlags(ActorState *s) {
    if (s->health) s->flags |= 1;
    else {
        s->morale = 0; s->fatigue = 0; s->skill = 0; s->flags = 0;
        if (s->damage >= (U32)s->descriptor->endurance * 10) s->flags |= 0x10000;
        else s->flags |= 0x100000;
        memset(s->skills, 0, 0x40);
    }
    if (s->morale < s->descriptor->threshold + s->descriptor->rules[11]) s->flags |= 2;
    if (s->morale > s->descriptor->threshold) s->flags |= 4;
}
unsigned int SimLogicFlags(void *object) {
    RefreshActorFlags((ActorState *)object);
    return ((ActorState *)object)->flags;
}

int ActorCondition(ActorState *);
int ActorQuery(ActorState *s, U32 event) {
    int result;
    if (event >= 216) return 0;
    switch(event) {
    case 200: return s->health;
    case 201: return ActorScale(s, s->descriptor->health, 201);
    case 202: return s->descriptor->marker;
    case 203: return s->descriptor->value;
    case 204:
        result = s->morale - s->descriptor->threshold;
        if (!s->health) result = 0;
        return result < 100 ? result : 99;
    case 205:
        result = ActorScale(s, s->descriptor->morale, 205) - s->descriptor->threshold;
        return result < 100 ? result : 99;
    case 207: return s->fatigue < 100 ? s->fatigue : 99;
    case 208: return ActorScale(s, s->descriptor->fatigue, 209);
    case 215: return ActorCondition(s);
    case 211: result = (U8)(s->damage / 10); return result < 100 ? result : 99;
    case 212: return s->descriptor->endurance;
    case 7: return s->skill * 10;
    case 213: return s->experience;
    case 214: return s->level;
    case 209: return 0;
    case 210: if (s->originalLevel != s->level) return s->level; return 0;
    default: return 0;
    }
}
int ActorEffectiveSkill(ActorState *s, U32 index) {
    int result = 0;
    if (index == 7) result = s->skill * 10;
    else if (index <= 8) result = s->skills[index].value >> 12;
    int firstThreshold = s->descriptor->threshold;
    int maximum = s->descriptor->morale;
    maximum = s->level + maximum - 1;
    if (maximum >= 100) maximum = 99;
    int middle = (maximum-firstThreshold)/2;
    middle += s->descriptor->threshold;
    middle = (U8)middle;
    if (s->morale <= middle) result -= middle - s->morale + 1;
    int threshold = s->descriptor->threshold;
    if (s->morale <= threshold) result -= (threshold - s->morale + 1) * 6;
    if (result < 0) result = 0;
    if (result > 99) result = 99;
    return result;
}
ActorDescriptor *ActorGetDescriptor(ActorState *s) { return s->descriptor; }
int ActorCondition(ActorState *s) {
    if (s->health == 0) return 0x44;
    int threshold = s->descriptor->threshold;
    if (s->morale <= threshold) return 0x43;
    int maximum = s->descriptor->morale; maximum = s->level + maximum - 1; if (maximum >= 100) maximum = 99;
    int offset = (maximum - threshold) / 2; int middle = offset + s->descriptor->threshold; middle = (U8)middle;
    if (s->morale <= middle) return 0x42;
    return 0x41;
}
void ActorSetName(ActorState *s, const char *name) { strncpy(s->name, name, 16); }
void ActorGetName(ActorState *s, char *name) { strncpy(name, s->name, 16); }
void ActorWork(ActorState *s) {
if (s->fatigue < ActorScale(s, s->descriptor->fatigue, 209)) ActorRecoverInjuries(s, s->descriptor->rules[12]);
    s->fatigue += ActorScale(s, s->descriptor->rules[13], 217);
    ActorAdjustScalar(s, (U32 *)&s->morale, -ActorScale(s, s->descriptor->rules[14], 217), 0);
    if (s->morale <= 0 && s->descriptor->health > 99) s->morale = 1;
    ActorAdjustFatigue(s, 0);
}
void ActorRest(ActorState *s) {
++s->ticks;
    if (s->fatigue < ActorScale(s, s->descriptor->fatigue, 209)) {
        if (s->ticks == s->descriptor->rules[18]) {
            ActorAdjustScalar(s, (U32 *)&s->morale, s->descriptor->rules[17], ActorScale(s, s->descriptor->morale, 205));
            s->fatigue += ActorScale(s, s->descriptor->rules[16], 217);
            s->ticks = 0;
        }
    } else if (s->ticks == s->descriptor->rules[18]) {
        s->fatigue += ActorScale(s, s->descriptor->rules[16], 217);
        s->ticks = 0;
    }
    ActorAdjustFatigue(s, 0);
}
void ActorChangeMorale(ActorState *s, int delta) {
    ActorAdjustScalar(s, (U32 *)&s->morale, delta, ActorScale(s, s->descriptor->morale, 205));
}
void ActorTrainSkill(ActorState *s, U32 index, int count) {
    if (index <= 8) {
        U32 *value = &s->skills[index].value;
        int scale = count * 10;
        U32 rate = CurveSlope((Curve *)(s->descriptor->curves + index * 8), *value);
        *value += ((rate / 100) * scale) >> 12;
    }
}
void ActorGainExperience(ActorState *s, int amount) {
    s->experience += ((U32)(amount * 20) / 100) << 12;
    U32 integral = s->experience >> 12;
    if (integral >= s->descriptor->levelThresholds[s->level] &&
        s->level <= s->descriptor->maximumLevel) {
        ++s->level;
        ActorAdjustSkills(s, 1);
    }
}

int ActorTrainingCost(ActorState *s, U32 index, int event) {
    if (index <= 8) {
        if (index == 7) {
            if (s->descriptor->skill == 0) return 0;
            if (s->skill < 4) {
                if (event == 0x71) {
                    int cost = (s->skill + 1) * 5000;
                    if (s->morale > 85) cost -= (s->morale - 85) * 500;
                    return cost;
                }
                ++s->skill; return 0;
            }
        } else {
            Curve curve;
            U32 *points = s->descriptor->curves + index * 8;
            U32 current = s->skills[index].value >> 12; U32 middle;
            U32 low = points[2], high = points[5];
            U32 difference = high - low;
            middle = low + (difference >> 1);
            CurveInit(&curve, 2, 35, low, 0, 0, high, 100, 100);
            U32 step = (U32)(difference * 0.2);
            if (step < 1) step = 1;
            if (current + step <= middle) {
                if (event == 0x71) return CurveForward(&curve, current + step) * 8;
                s->skills[index].value += step << 12;
            }
        }
    }
    return 0;
}
void ActorRestoreHealth(ActorState *s) {
    int value = s->level + s->descriptor->health - 1;
    if (value >= 100) value = 99;
    s->health = value;
}
int ActorHealthCost(ActorState *s) {
int maximum = ActorScale(s, s->descriptor->health, 201);
    if (maximum == s->health) return 0;
    int difference = 100 - maximum;
    Curve curve;
    CurveInit(&curve, 2, 35, 0, difference, difference, maximum, difference * 10, difference * 10);
    return CurveInverse(&curve, s->health) * 5;
}
void ActorRestoreMorale(ActorState *s) {
    int value = s->level + s->descriptor->morale - 1;
    if (value >= 100) value = 99;
    s->morale = value;
}
int ActorMoraleCost(ActorState *s) {
int maximum = ActorScale(s, s->descriptor->morale, 205);
    if (maximum == s->morale) return 0;
    int difference = 100 - maximum;
    Curve curve;
    CurveInit(&curve, 2, 35, 0, difference, difference, maximum, difference * 10, difference * 10);
    return CurveInverse(&curve, s->morale) * 5;
}
int ActorFatigueCost(ActorState *s) {
    if (!s->fatigue) return 0;
    int difference = 100 - ActorScale(s, s->descriptor->fatigue, 209);
    Curve curve;
    CurveInit(&curve, 2, -35, 0, difference, difference, 100, difference * 10, difference * 10);
    return CurveForward(&curve, s->fatigue) * 5;
}
int ActorRemainingValue(ActorState *s) {
    float proportion = (float)s->damage / (float)(s->descriptor->endurance * 10);
    int result = (int)((float)s->descriptor->value -
        (float)s->descriptor->value * proportion);
    if (result < 0) return 0;
    return result;
}

void ActorRememberLevel(ActorState *s) { s->originalLevel = s->level; RefreshActorFlags(s); }
void ActorState::ActorApplyDamage(U8 amount) { ActorState *s = this;
    while (s->health > 0) {
        int total = 0;
        for (int i = 0; i < 8; ++i) total += s->injuries[i];
        if (!total) break;
        ActorTick(s, 2, 2, 1);
    }
    s->damage += amount;
    U32 maximum = s->descriptor->endurance * 10;
    if (s->damage > maximum) s->damage = maximum;
    if (s->damage >= maximum && s->descriptor->health <= 99) {
        s->health = 0; s->morale = 0;
    } else {
        U8 *rules = s->descriptor->rules;
        if (s->damage >= (U32)(((100.0 - rules[2]) / 100.0) * maximum)) {
            ActorAdjustScalar(s, (U32 *)&s->health, -(int)rules[3], 0);
            ActorAdjustScalar(s, (U32 *)&s->morale, -(int)s->descriptor->rules[3], 0);
            s->flags |= 0x40000;
        } else if (s->damage >= (U32)(((100.0 - rules[0]) / 100.0) * maximum)) {
            ActorAdjustScalar(s, (U32 *)&s->health, -(int)rules[1], 0);
            ActorAdjustScalar(s, (U32 *)&s->morale, -(int)s->descriptor->rules[1], 0);
            s->flags |= 0x20000;
        }
        if (s->morale <= 0 && s->descriptor->health > 99) s->morale = 1;
        if (s->health <= 0 && s->descriptor->health > 99) s->health = 1;
    }
    RefreshActorFlags(s);
}
void ActorTick(ActorState *s, int health, int morale, int fatigue) {
    if ((health == -1 && morale == -1 && fatigue == -1) || !s->active || !s->health) {
        if (s->descriptor->health <= 99) {
            s->health = 0;
            return;
        }
        s->health = 1;
    }
    if (fatigue) ActorAdjustScalar(s, (U32 *)&s->fatigue, -fatigue, 0);
    s->fatigue = s->fatigue < 0 ? 0 : s->fatigue;
    ActorAdjustFatigue(s, health);
    ActorAdjustMorale(s, morale);
    for (int i = 0; i < 8; ++i) {
        int injury = s->injuries[i];
        if (injury > 0) {
            int step = s->descriptor->rules[15];
            if (injury < step) { ActorAdjustSkill(s, i, -injury); s->injuries[i] = 0; }
            else { ActorAdjustSkill(s, i, -step); s->injuries[i] -= step; }
        }
    }
}
void ActorDamageHealth(ActorState *s, int damage) {
    s->health -= damage;
    if (s->health <= 0) {
        s->health = s->descriptor->health > 99;
    }
    int maximum = s->level + s->descriptor->health - 1;
    if (maximum >= 100) maximum = 99;
    if (s->health > maximum) s->health = maximum;
}
void ActorZeroVitals(ActorState *s) { s->health = 0; s->morale = 0; s->fatigue = 0; }
extern int actorVoiceCycle, actorVoiceCycle2;
extern int actorVoices[6], actorVoices2[3];
extern "C" int LogicCallMode(int, void *);
void ActorVoice(ActorState *s) {
    int id = *(int *)((char *)s->descriptor + 0x9b);
    if (id) {
        if (id == -1) id = actorVoices[actorVoiceCycle++ % 6];
        if (id == -2) id = actorVoices2[actorVoiceCycle2++ % 3];
        LogicCallMode(id, 0);
    }
}
void ActorAdjustFatigue(ActorState *s, int delta) {
    if (s->fatigue > ActorScale(s, s->descriptor->fatigue, 209))
        ActorAdjustScalar(s, (U32 *)&s->health, -(int)s->descriptor->rules[4], 0);
    if (s->morale < s->descriptor->threshold)
        ActorAdjustScalar(s, (U32 *)&s->health, -(int)s->descriptor->rules[5], 0);
    ActorAdjustScalar(s, (U32 *)&s->health, delta, ActorScale(s, s->descriptor->health, 201));
    if (s->health <= 0) {
        if (s->descriptor->health > 99) s->health = 1;
        else s->health = 0;
    }
}
void ActorAdjustMorale(ActorState *s, int delta) {
    if (s->fatigue > ActorScale(s, s->descriptor->fatigue, 209))
        ActorAdjustScalar(s, (U32 *)&s->morale, -(int)s->descriptor->rules[8], 0);
    ActorAdjustScalar(s, (U32 *)&s->morale, delta, ActorScale(s, s->descriptor->morale, 205));
    if (s->morale <= 0) {
        if (s->descriptor->health > 99) s->morale = 1;
        else s->morale = 0;
    }
}
void ActorRecoverInjuries(ActorState *s, U32 step) {
for (int i = 0; i < 8; ++i) {
        U32 difference = (s->skills[i].maximum >> 12) - (s->skills[i].value >> 12);
        if (difference >= step) s->injuries[i] += (char)step;
        else s->injuries[i] += (char)difference;
        ActorAdjustFixed(s, &s->skills[i].value, step, s->skills[i].maximum);
    }
}
void ActorAdjustSkills(ActorState *s, int delta) {
    for (int i = 0; i < 8; ++i) ActorAdjustFixed(s, &s->skills[i].value, delta, s->skills[i].maximum);
}
void ActorAdjustSkill(ActorState *s, int index, int delta) {
    if (index >= 0 || index < 8) ActorAdjustFixed(s, &s->skills[index].value, delta, s->skills[index].maximum);
}
void Crc16(unsigned short *, const void *, unsigned int);
int PilotChecksum(void *object) {
    ActorState *s = (ActorState *)object;
    if (!(s->flags & 1)) return 0xbc614e;
    unsigned short crc[2]; crc[0] = 0;
    unsigned int length = (char *)&s->descriptor - (char *)s;
    Crc16(crc, s, length);
    unsigned int remaining = sizeof(*s) - length - 4;
    char *tail = (char *)&s->descriptorIndex; Crc16(crc, tail, remaining);
    return crc[0];
}
int ActorState::Equal(const ActorState *other) {
    const char *a = (const char *)this;
    const char *b = (const char *)other;
    int equal = 1;
    for (U32 i = 0; i < 0x7e; ++a, ++b, ++i) if (*a != *b) equal = 0;
    return equal;
}
void ActorCopyState(ActorState *dest, ActorState *source) {
    memcpy(dest, source, 0x76);
    dest->descriptorIndex = source->descriptorIndex;
}
