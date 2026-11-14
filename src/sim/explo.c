// sim\explo.c - explosion / effect animation records (500 x 0x43 bytes). Names are working labels.
extern "C" void *memset(void *, int, unsigned int);
#pragma pack(1)
struct ExploPoint { int x, y; ExploPoint(); }; inline ExploPoint::ExploPoint() {}
struct ExploRect { int x, y, w, h; ExploRect(int ax, int ay, int aw, int ah) { x = ax; y = ay; w = aw; h = ah; } };
struct ExploFrame { char pad00[4]; unsigned short scaleX, scaleY; char pad08[2]; unsigned char anchorX, anchorY; };   // + 4, + 6, + 0x0a, + 0x0b
struct ExploRecord {
    int depth;                                      // + 0x00 draw order key
    signed char type, anim;                         // + 0x04 (0 = free, negative kinds), + 0x05
    char positioned; signed char repeat;            // + 0x06, + 0x07
    unsigned char fps, frame, frames, frameStep;    // + 0x08 .. + 0x0b
    unsigned char delay, life; signed char layer;   // + 0x0c, + 0x0d (0xff = endless), + 0x0e
    unsigned int nextTime;                          // + 0x0f
    int target, value;                              // + 0x13, + 0x17
    ExploPoint pos, pos2;                           // + 0x1b, + 0x23
    int rectX, rectY, rectW, rectH;                 // + 0x2b
    ExploFrame *frameData;                          // + 0x3b
    int sound;                                      // + 0x3f
};
struct ExploActor { int id; char pad04[0x28]; unsigned short sound; char pad2e[4]; ExploPoint pos, pos2; };   // + 0x2c, + 0x32, + 0x3a
struct ExploTarget { char pad00[0x0c]; ExploPoint pos; char pad14[0x4c]; ExploPoint dest; };   // + 0x0c, + 0x60
struct FileRecord;
#pragma pack()

extern "C" void AssertFail(const char *, int, const char *);
extern "C" int RandomRange(unsigned int *seed, int low, int high);
extern unsigned long StreamRead(FileRecord *, void *, unsigned long);
extern unsigned long StreamWrite(FileRecord *, const void *, unsigned long);
extern "C" void TargetObjectInit(const char *, void *, int);
extern void BestLocSetData(void *, void *, void *);
extern long GridPointToIndex(const char *, ExploPoint *);
extern const char fallbackName[];
extern int PointAngle(ExploPoint *a, ExploPoint *b);           // 0047ef20
extern void ViewToScreen(void *view, ExploPoint *out, ExploPoint *in);   // 0042bf30
extern int ViewScale(void *view, int value);                   // 0042bf10
extern char gView[];                                           // 004d360c
extern int gZoom;                                              // 004d3401
extern unsigned int gFxSeed;                                   // 004d3e04
extern int exploSoundTable[];                                  // 004a0cd8 (15 ints per entry)

extern ExploRecord *ExploAlloc(int kind);

int exploCount;                     // 004dcc7c
ExploRecord *explos;                // 004dcc80
struct ExploRemap { unsigned char map[256]; ExploRemap() {} };
ExploRemap exploRemap;              // 004dcc84

#define EXPLO_RESET(e, kind) memset(e, 0, sizeof(ExploRecord)), (e)->value = 100, (e)->fps = 60, (e)->life = 0xff,     (e)->frameStep = 1, (e)->frames = 1, (e)->type = kind
static const char exploSourcePath[] = __FILE__;
#define ASSERT(e) if (!(e)) AssertFail(exploSourcePath, __LINE__, 0)

static ExploRecord *ExploFindReset(int kind, int target)
{
    ExploRecord *e = explos;
    for (int i = 0; i < exploCount; i++, e++) {
        if (e->type == kind && target == e->target) {
            EXPLO_RESET(e, kind);
            return e;
        }
    }
    return 0;
}

ExploRecord *ExploMessage(int message, ExploActor *actor)
{
    if (message == 0 || actor == 0)
        return 0;
    ExploRecord *e = ExploFindReset(-6, -actor->id);
    if (e == 0)
        e = ExploAlloc(-6);
    if (e == 0)
        return 0;
    if (e != 0) {
        e->positioned = 1;
        e->anim = message;
        e->target = actor->id;
        e->target = -e->target;
        e->value = exploSoundTable[actor->sound * 15];
        e->pos2 = actor->pos2;
        e->pos = actor->pos;
    }
    return e;
}













ExploRecord *ExploStart(int kind, ExploTarget *target)
{
    ExploRecord *e = ExploAlloc(kind);
    ASSERT(e != 0 && kind != 0);
    if (e != 0) {
        ExploPoint from, to;
        ViewToScreen(gView, &from, &target->pos);
        ViewToScreen(gView, &to, &target->dest);
        e->anim = 1;
        int angle = (PointAngle(&from, &to) / 0xb60b61 + 720) % 360;
        if (angle > 120 && angle < 240)
            e->depth = from.y - gZoom * 2;
        else
            e->depth = 20000;
    }
    return e;
}

// Declarations used by the drawing and positioning code at the end of the file.
#pragma pack(1)
struct ExploBox { int x, y, w, h; };
#include "actor.h"

struct ExploAnim { unsigned char count; char pad01[3]; ExploFrame frames[1]; };                   // frames are 12 bytes
#pragma pack()
extern ExploAnim *AnimFrames(int anim);                        // 00459e60
extern ActorRecord *ActorGetByIndex(int);
extern char *UnitPart(ActorUnit *unit, int part = 0, int index = -1);        // 00438374
extern void ActorAttachPoint(AnimAction *s, ExploPoint *out, int layer, char *part, void *view);   // 004579e0
extern void ExploIdle();                                       // 0047682c
extern void FxBounds(ExploRecord *e);                          // 0045e310 (simxout.c)
extern void FxDrawCurrent();                                   // 0045e190
extern void FxDrawJagged();                                    // 0045e1e0
extern void DrawSprite(void *surface, ExploFrame *frame, int x, int y, int flags, int w, int h);   // 0047c0f0
extern int AnimSetViewportRect(int *, int *, int, int, int, int);
extern void SoundAtPoint(ExploPoint *at, int sound);           // 0045d9f0
extern unsigned int TimerNow();                                // 0047fc80
extern char g513888[];
extern void *gDrawRemap;                                       // 005138e8


ExploRecord *ExploAddGlow(int anim, int value, ExploActor *actor, char layer, char repeat)
{
    if (anim == 0 || actor == 0)
        return 0;
    ExploRecord *e = ExploAlloc(-10);
    if (e == 0)
        return 0;
    if (e != 0) {
        e->value = value;
        e->anim = anim;
        e->repeat = repeat;
        e->target = actor->id;
        e->target = -actor->id;
        e->value = value << 4;
        e->layer = layer;
        e->pos2 = actor->pos2;
        e->pos.x = actor->pos.x;
        e->pos.y = actor->pos.y;
    }
    return e;
}

ExploRecord *ExploAddAt(int anim, ExploPoint *pos, int value)
{
    if (anim == 0)
        return 0;
    ExploRecord *e = ExploAlloc(-9);
    if (e == 0)
        return 0;
    {
        int index = GridPointToIndex(fallbackName, pos);
        if (index == 0)
            return 0;
        if (e != 0) {
            e->value = value;
            e->target = index;
            e->anim = anim;
            e->pos = *pos;
            BestLocSetData((void *)fallbackName, &e->pos2, (void *)index);
        }
    }
    return e;
}

ExploRecord *ExploAddTarget(int anim, int target, int value)
{
    if (anim == 0 || target == 0)
        return 0;
    ExploRecord *e = ExploAlloc(-5);
    if (e == 0)
        return 0;
    if (e != 0) {
        e->anim = anim;
        e->value = value;
        TargetObjectInit(fallbackName, &e->pos, target);
    }
    return e;
}

ExploRecord *ExploAddSmoke(int anim, ExploActor *actor, int value, int unused, int size)
{
    if (anim == 0 || actor == 0)
        return 0;
    ExploRecord *e = ExploAlloc(-8);
    if (e == 0)
        return 0;
    if (e != 0) {
        e->anim = anim;
        e->value = value;
        e->target = actor->id;
        e->target = -e->target;
        e->pos2 = actor->pos2;
        if (size > 1) {
            e->pos.x = RandomRange(&gFxSeed, 30, 70);
            e->pos.y = RandomRange(&gFxSeed, 30, 70);
        } else {
            e->pos.x = RandomRange(&gFxSeed, 40, 60);
            e->pos.y = RandomRange(&gFxSeed, 40, 60);
        }
    }
    return e;
}









































ExploRecord *ExploAddPart(char anim, int frames)
{
    ExploRecord *e = ExploAlloc(-1);
    ASSERT(e != 0);
    if (e != 0) {
        e->anim = anim;
        e->repeat = -1;
        e->value = frames * 10;
    }
    return e;
}

extern void ExploSetupFrame(ExploRecord *e, ExploPoint *pos);

ExploRecord *ExploAddTrail(int anim, int frames, int step, ExploRecord *source, ExploPoint *pos)
{
    if (anim == 0 || frames == 0)
        return 0;
    ExploRecord *e = ExploAlloc(-11);
    if (e == 0)
        return 0;
    e->anim = anim;
    e->value = frames * 10;
    e->repeat = 0;
    e->depth = source->depth;
    e->pos2 = source->pos2;
    e->frameStep = step ? step : 1;
    ExploSetupFrame(e, pos);
    e->pos = *pos;
    return e;
}



















ExploRecord *ExploAlloc(int kind)
{
    ASSERT(explos != 0);
    ExploRecord *e = 0;
    if (exploCount >= 500) {
        ExploRecord *p = explos;
        for (int i = 0; i < exploCount; i++, p++) {
            if (!p->type || p->type < kind)
                e = p;
        }
        if (e == 0)
            return 0;
    } else
        e = &explos[exploCount++];
    EXPLO_RESET(e, kind);
    return e;
}

int ExploFree(ExploRecord *e)
{
    if (e == 0)
        return 0;
    e->type = 0;
    return 1;
}

void ExploCompact(int all)
{
    ExploRecord *p = explos;
    for (int i = 0; i < exploCount; ) {
        signed char type = p->type;
        if (type == 0 || type == -1 || type == -2 || type == -3 || type == -4 || (type == -11 && all)) {
            explos[i] = explos[exploCount - 1];
            exploCount--;
        } else {
            i++;
            p++;
        }
    }
}








































































int ExploInit()
{
    ASSERT(explos == 0);
    explos = new ExploRecord[500];
    ASSERT(explos != 0);
    if (explos != 0)
        memset(explos, 0, 500 * sizeof(ExploRecord));
    exploCount = 0;
    memset(&exploRemap, 0x40, sizeof(exploRemap));
    exploRemap.map[0] = 0;
    return explos != 0;
}

static void ExploClear()
{
    if (explos != 0)
        memset(explos, 0, 500 * sizeof(ExploRecord));
    exploCount = 0;
}

void ExploShutdown()
{
    ExploClear();
    if (explos != 0)
        delete[] explos;
    explos = 0;
    exploCount = 0;
}




















int ExploSave(FileRecord *file)
{
    ASSERT(explos != 0);
    StreamWrite(file, &exploCount, 4);
    ASSERT(exploCount >= 0 && exploCount < 500);
    if (exploCount != 0)
        StreamWrite(file, explos, exploCount * sizeof(ExploRecord));
    return 1;
}







int ExploLoad(FileRecord *file)
{
    ASSERT(explos != 0);
    ExploClear();
    StreamRead(file, &exploCount, 4);
    ASSERT(exploCount >= 0 && exploCount < 500);
    if (exploCount != 0)
        StreamRead(file, explos, exploCount * sizeof(ExploRecord));
    return 1;
}











void ExploSetupFrame(ExploRecord *e, ExploPoint *pos)
{
    if (!e->anim || e->delay) {
        static ExploRect hidden(-10, -10, 1, 1);
        e->frameData = 0;
        *(ExploRect *)&e->rectX = hidden;
    } else {
        ExploAnim *anim = AnimFrames(e->anim);
        e->frames = anim->count;
        ASSERT(e->frames);
        e->frameData = &anim->frames[e->frame % e->frames];
        int scale;
        if (e->value > 0)
            scale = ViewScale(gView, e->value);
        else
            scale = -e->value;
        ExploFrame *f = e->frameData;
        unsigned int anchorX = f->anchorX;
        unsigned int anchorY = f->anchorY;
        int w = f->scaleX * scale / 64;
        int h = f->scaleY * scale / 64;
        e->rectX = pos->x;
        e->rectY = pos->y;
        e->rectW = w;
        e->rectH = h;
        if (e->frameData->scaleX)
            e->rectX -= w * anchorX / e->frameData->scaleX;
        if (e->frameData->scaleY)
            e->rectY -= anchorY * h / e->frameData->scaleY;
    }
}

int ExploPosition(ExploRecord *e)
{
    if (!e->anim || e->delay)
        return 0;
    {
        ExploPoint at;
        ActorRecord *actor;
        switch (e->type) {
        case 0:
            ExploIdle();
            break;
        case -2:
        case -3:
            FxBounds(e);
            break;
        case -11:
            ExploSetupFrame(e, &e->pos);
            break;
        case -6: {
            actor = ActorGetByIndex(-e->target);
            if (actor == 0)
                break;
            static ExploPoint spot;
            AnimAction *sprite = &actor->action;
            e->value = sprite->rect.height;
            e->value = -sprite->rect.height;
            spot.x = sprite->screen.x;
            spot.y = sprite->rect.height / 2 + sprite->rect.y;
            e->depth = actor->action.depth + 10;
            ExploSetupFrame(e, &spot);
            break;
        }
        case -7:
        case -8: {
            actor = ActorGetByIndex(-e->target);
            GuiRect *box = &actor->action.rect;
            at.x = box->width * e->pos.x / 100 + box->x;
            at.y = box->height * e->pos.y / 100 + box->y;
            int value = e->value;
            e->value = box->height * value / 100;
            e->value = -e->value;
            ExploSetupFrame(e, &at);
            e->value = value;
            e->depth = actor->action.depth + 1;
            break;
        }
        case -9:
            ViewToScreen(gView, &at, &e->pos);
            e->depth = at.y + gZoom * 2;
            ExploSetupFrame(e, &at);
            break;
        case -5:
            ViewToScreen(gView, &at, &e->pos);
            e->depth = 40000;
            ExploSetupFrame(e, &at);
            break;
        case -10:
            actor = ActorGetByIndex(-e->target);
            ActorAttachPoint(&actor->action, &at, e->layer, UnitPart(actor->unit), gView);
            e->depth = actor->action.depth + 20000;
            ExploSetupFrame(e, &at);
            break;
        }
        return 1;
    }
}

void ExploDraw(ExploRecord *e)
{
    gDrawRemap = 0;
    switch (e->type) {
    case 0:
        ExploIdle();
        break;
    case -2:
        FxDrawCurrent();
        break;
    case -3:
        FxDrawJagged();
        break;
    case -4:
        gDrawRemap = &exploRemap;
    case -11:
    case -10:
    case -9:
    case -8:
    case -7:
    case -6:
    case -5:
    case -1:
        if (e->anim >= 24 && e->anim <= 26)
            gDrawRemap = (void *)1;
        if (e->frameData != 0) {
            if (e->depth > 30000)
                DrawSprite(g513888, e->frameData, e->rectX, e->rectY, 0, e->rectW, e->rectH);
            else
                AnimSetViewportRect((int *)e->frameData, (int *)&e->pos2, e->rectX, e->rectY, e->rectW, e->rectH);
        }
        break;
    }
    gDrawRemap = 0;
}

void ExploAnimate(ExploRecord *e)
{
    int type = e->type;
    if (type == 0 || type == -3 || type == -2)
        return;
    unsigned int now = TimerNow();
    if (now < e->nextTime || !e->fps || !e->frames)
        return;
    if (e->delay) {
        e->delay--;
        return;
    }
    if (e->frame == 0 && e->sound != 0) {
        SoundAtPoint(&e->pos, e->sound);
        e->sound = 0;
    }
    if (e->life != 0xff) {
        if (!e->life--) {
            e->type = 0;
            return;
        }
    }
    e->nextTime = now + 1000 / e->fps;
    e->frame += e->frameStep;
    if (e->frame < e->frames)
        return;
    if (e->repeat == -1) {
        e->frame = 0;
        return;
    }
    if (e->repeat-- > 0) {
        e->frame = 0;
        return;
    }
    e->type = 0;
}

void ExploAnimateAll()
{
    ExploRecord *e = explos;
    for (int i = 0; i < exploCount; i++, e++) {
        if (e->type)
            ExploAnimate(e);
    }
}



