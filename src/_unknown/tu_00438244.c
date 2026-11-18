struct Curve {
    int mode, curvature;
    int lowX, lowY, below;
    int highX, highY, above;
};
void CurveInit(Curve *, int, int, int, int, int, int, int, int);

#pragma pack(push, 1)
struct SkillRange {
    unsigned char low, high;
    short curvature;
};

struct ActorDescriptor {
    char header[0x17];
    SkillRange ranges[7];
    char otherFields[0xaa - 0x33];
    Curve curves[7];
};
#pragma pack(pop)

// The descriptor stores seven packed range definitions and seven expanded curves.
#define PREPARE_SKILL(actor, i) \
    CurveInit(&(actor)->curves[i], 2, (actor)->ranges[i].curvature, (actor)->ranges[i].low, \
              0, 0, (actor)->ranges[i].high, 100, 100)

void PrepareActorDescriptor(void *data)
{
    ActorDescriptor *actor = (ActorDescriptor *)data;
    PREPARE_SKILL(actor, 0);
    PREPARE_SKILL(actor, 1);
    PREPARE_SKILL(actor, 2);
    PREPARE_SKILL(actor, 3);
    PREPARE_SKILL(actor, 4);
    PREPARE_SKILL(actor, 5);
    PREPARE_SKILL(actor, 6);
}
