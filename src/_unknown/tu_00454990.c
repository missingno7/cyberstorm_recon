/* Drawing object 00454990..004556f0; split from the preceding timer/player object.
 * Unknown structure ranges are temporary padding. Labels are descriptive guesses. */
#pragma pack(push,1)
struct GridPoint { int x,y; GridPoint() {} };
struct GuiRect { long x,y,width,height; GuiRect() {}
    int Moved(const GuiRect &r) { return x != r.x || y != r.y; }
};
struct AnimAction {
    int id;
    unsigned char kind,state,variant,rate,frame,frames,flags;
    signed char dirty;
    unsigned short type;
    unsigned long deadline;
    GridPoint position;
    char unknown1a[8];
    int target;
    int heading;
    char unknown2a[0x3b-0x2a];
    GuiRect rect;
    char unknown4b[0x68-0x4b];
    void SetHeading(int n) { heading = n * 0x2aaaaabc; }
    int IsMoving() {
        return state != 1 && state != 7 && state != 14 && state != 17 && state != 18;
    }
};
struct UnitState { unsigned char unknown00[0x12]; unsigned char type; unsigned char unknown13[0x10b-0x13]; };
struct ActorRecord {
    int id;
    unsigned char kind,subtype,variant,unknown07;
    int owner,targetId,unknown10,targetName;
    UnitState *unit;
    void *world;
    AnimAction action;
    signed char active,hidden;
    char unknown8a[3];
    signed char stance;
    unsigned char color;
    char unknown8f[4];
    int related;
    unsigned char blink;
    int Active() { return active; }
    int Hidden() { return hidden; }
    int Stance() { return stance; }
    int Color() { return color; }
};

struct SimGame {
    char unknown00[0x2c]; unsigned short flags; char unknown2e[10];
    ActorRecord *focus;
    char unknown3c[0x2c2-0x3c]; int timed; char unknown2c6[4];
    int tick; char unknown2ce[4]; int cursor;
    char unknown2d6[0x986-0x2d6]; int viewer; char unknown98a[8]; int localTurn;
    char unknown996[0xf9d-0x996]; int detail, cursorScale;
    char unknownfa5[0x10cd-0xfa5]; ActorRecord *target;
    char unknown10d1[0x10ed-0x10d1]; int remaining,timedExtra;
};
struct ViewRect { int left,top,right,bottom; };
struct ClipState { GuiRect rect; int extra[6]; };
struct WorldAction {
    int depth; unsigned char kind,subtype,variant; char unknown07[0x22-7];
    int target; char unknown26[0x3b-0x26]; GuiRect rect; char unknown4b[0x6f-0x4b];
};
struct ExploRecord {
    int depth; signed char type; char unknown05[0x2b-5]; GuiRect rect; char unknown3b[8];
};
struct DrawEntry { int depth; unsigned char kind; };
struct TargetTriple { char unknown00[4]; unsigned char marker; char unknown05[7]; };
struct MenuImage { unsigned long offset; unsigned short width,height; unsigned long flags; };
struct MenuImages { unsigned long count; MenuImage images[1]; };
struct DrawGaps { int value[5]; };
struct AnimType { char unknown00[0x14]; int period; char unknown18[0x3c-0x18]; };
#pragma pack(pop)
extern unsigned char drawOverlayFlags;
extern unsigned char *simVisibleTypes,*simActorVisibility;
extern char fallbackName[],gView[],g513888,g4dd0c0[];
extern TargetTriple *targetDefinitionTable;
extern MenuImages *terrainImage,*drawCursorImages;
extern int gZoom,gGameTurn,drawFrame;
extern ClipState simMainClip,simMainSaved,simDetailClip,simDetailSaved,drawDetailChanged;
extern ClipState simSmallClip;
extern ViewRect viewportClipLeft,gViewRect;
extern int g5138ac;
extern int actorCount,worldActionCount,exploCount;
extern ActorRecord *actorTable;
extern WorldAction *worldActions;
extern ExploRecord *explos;
extern DrawEntry **drawQueue;
extern DrawGaps drawSortGaps;
extern signed char drawDetailRates[];
extern AnimType drawAnimationTypes[];
extern int drawCursorTarget;
static int drawPreviousTick;
static int drawQueueCount;
static GridPoint drawCursorPoint;
static GuiRect drawCursorRect;

void f_00459640(unsigned char *);
void BestLocSetData(void *,void *,void *);
void DisplayPointToScreen(int *,int *,int *,int);
int AnimSetViewportRect(int *,int *,int,int,int,int);
void MenuSetClip(GuiRect *,long,long,long,long);
void GuiResetClip(void *);
void SimClipFlush(void *);
int SimHotReadTimer(char *);
int f_0047add0(ClipState *,GuiRect *);
int f_0047b170(ClipState *,GuiRect *);
void f_0047ad90(ClipState *,ClipState *);
void f_0047b1c0(ClipState *);
int f_0047b000(ClipState *,GuiRect *);
void AnimActionUpdateTrajectory(int *,int *);
int f_00460340(ExploRecord *);
void ExploDraw(ExploRecord *);
void ExploAnimateAll();
int Interpolate(int,int,int,int,int);
void DisplayProcess(int *,void * = 0,int = 0,int = 0);
int f_00438678(UnitState *);
void PopDrawSurface(void *,int,int,int,int,int,int);
void LogicDrawConsole(void *);
void f_004517c0(SimGame *);
inline void DrawColor(unsigned char *p) { f_00459640(p); }
inline unsigned char Hidden(int target,int type) { return !(type == 17 ? simVisibleTypes : simActorVisibility)[target]; }

inline void DrawActionBody(AnimAction *action)
{
    f_00459640((unsigned char *)action);
    if (drawOverlayFlags & 0x80) {
        int target = action->target;
        if (action->state != 4 || action->variant != 0) {
            unsigned char image = targetDefinitionTable[target].marker;
            if (image) {
                GridPoint world,screen;
                BestLocSetData(fallbackName,&world,(void *)target);
                DisplayPointToScreen((int *)gView,(int *)&screen,(int *)&world,target);
                int zoom = gZoom*2;
                int x = screen.x - zoom; int y = screen.y - zoom;
                if (target & 2)
                    AnimSetViewportRect((int *)&terrainImage->images[image],(int *)&world,x+gZoom/2,y+gZoom,gZoom*3,gZoom*3);
                else
                    AnimSetViewportRect((int *)&terrainImage->images[image],(int *)&world,x,y,zoom*2,zoom*2);

            }
        }
    }
}

void SimDrawAction(AnimAction *action) { DrawActionBody(action); }

void SimInvalidateActors(SimGame *game)
{
    static GuiRect previousFocus;
    static GuiRect previousTarget;
    int tick = game->tick/512;
    if (game->localTurn && game->focus) {
        GuiRect *rect = &game->focus->action.rect;
        if (tick != drawPreviousTick || previousFocus.Moved(*rect)) {
            MenuSetClip(&simDetailClip.rect,previousFocus.x-5,previousFocus.y-5,previousFocus.width+10,previousFocus.height+10);
            MenuSetClip(&simDetailClip.rect,rect->x-5,rect->y-5,rect->width+10,rect->height+10);
            previousFocus = *rect;
        }
        if (game->target) {
            rect = &game->target->action.rect;
            if (tick != drawPreviousTick || previousTarget.Moved(*rect)) {
                MenuSetClip(&simDetailClip.rect,previousTarget.x-5,previousTarget.y-5,previousTarget.width+10,previousTarget.height+10);
                MenuSetClip(&simDetailClip.rect,rect->x-5,rect->y-5,rect->width+10,rect->height+10);
                previousTarget = *rect;
            }
        }
    }
    drawPreviousTick = tick;
    if (game->timed || game->timedExtra) {
        int left = SimHotReadTimer((char *)game);
        if (left != game->remaining) MenuSetClip(&simDetailClip.rect,g5138ac-145,0,145,40);
        game->remaining = left;
    } else game->remaining = 0x7fffffff;
}

void SimBuildDrawQueue(SimGame *game)
{
    signed char all = (game->flags & 0x801) != 0;
    if (all) { GuiResetClip(&simMainClip); GuiResetClip(&simDetailClip); }
    else { SimClipFlush(&simMainClip); SimClipFlush(&simDetailClip); }
    DrawEntry **out = drawQueue;
    drawQueueCount = 0;
    int animationRate = Interpolate(0,2000,100,150,gGameTurn);
    animationRate = drawDetailRates[game->detail]*animationRate/10;
    ActorRecord *actor = actorTable;
    for (int i=0; i<actorCount; ++i,++actor) {
        AnimAction *action = &actor->action;
        if (!actor->hidden) {
            int hidden = 0;
            if (actor->owner != game->viewer) hidden = Hidden(action->target,action->state);
            if (hidden) {
                if (all || action->dirty) AnimActionUpdateTrajectory((int *)action,(int *)gView);
            } else {
                int dirty = action->dirty;
                if (all || dirty) {
                    f_0047add0(&simDetailClip,&action->rect);
                    AnimActionUpdateTrajectory((int *)action,(int *)gView);
                    if (f_0047add0(&simDetailClip,&action->rect) || dirty) {
                        *out++ = (DrawEntry *)action;
                        ++drawQueueCount;
                    }
                } else if (f_0047b170(&simMainClip,&action->rect)) {
                    int period = drawAnimationTypes[action->type].period * animationRate/100;
                    if ((!period || (drawFrame + (unsigned char)*((char *)action+0x37))%period == 0) && gGameTurn)
                        f_0047add0(&simMainClip,&action->rect);
                    *out++ = (DrawEntry *)action;
                    ++drawQueueCount;
                }
            }
        }
    }
    for (i=0; i<worldActionCount; ++i) {
        WorldAction *action = &worldActions[i];
        if (simVisibleTypes[action->target]) {
            if (all) AnimActionUpdateTrajectory((int *)action,(int *)gView);
            if (f_0047b170(&simMainClip,&action->rect)) {
                *out++ = (DrawEntry *)action;
                ++drawQueueCount;
            }
        }
    }
    for (i=0; i<exploCount; ++i) {
        ExploRecord *effect = &explos[i];
        if (effect->type) {
            f_0047add0(&simDetailClip,&effect->rect);
            if (f_00460340(effect) && f_0047add0(&simDetailClip,&effect->rect)) {
                *out++ = (DrawEntry *)effect;
                ++drawQueueCount;
            }
        }
    }
    if (game->localTurn) {
        if (drawCursorTarget != game->cursor) f_0047add0(&simDetailClip,&drawCursorRect);
        if (game->cursor) {
            BestLocSetData(fallbackName,&drawCursorPoint,(void *)game->cursor);
            DisplayPointToScreen((int *)gView,(int *)&drawCursorRect,(int *)&drawCursorPoint,game->cursor);
            int radius = (game->cursorScale*28+8)/16;
            drawCursorRect.x -= radius;
            drawCursorRect.y -= radius;
            drawCursorRect.width = radius*2;
            drawCursorRect.height = radius*2;
            if (drawCursorTarget != game->cursor) f_0047add0(&simDetailClip,&drawCursorRect);
        }
        drawCursorTarget = game->cursor;
    } else drawCursorTarget = 0;
    SimInvalidateActors(game);
    f_0047ad90(&simMainClip,&simDetailClip);
    ClipState *source = &simMainClip;
    simSmallClip = *source;
    f_0047ad90(&simSmallClip,&simMainSaved);
    source = &simDetailClip;
    drawDetailChanged = *source;
    f_0047ad90(&drawDetailChanged,&simDetailSaved);
}

void SimSortDrawQueue(SimGame *)
{
    DrawGaps gaps = drawSortGaps;
    for (int pass=0; pass<5; ++pass) {
        int gap = gaps.value[pass];
        for (int i=gap; i<drawQueueCount; ++i) {
            DrawEntry *entry = drawQueue[i];
            int j = i-gap;
            for (;j>=0;j-=gap) {
                if (entry->depth > drawQueue[j]->depth) break;
                drawQueue[j+gap] = drawQueue[j];
            }
            drawQueue[j+gap] = entry;
        }
    }
}

void SimDrawQueue(SimGame *game)
{
    SimSortDrawQueue(game);
    f_0047b1c0(&simSmallClip);
    for (int i=0; i<drawQueueCount; ++i) {
        DrawEntry *entry = drawQueue[i];
        if (entry->kind == 1) DrawActionBody((AnimAction *)entry);
        else ExploDraw((ExploRecord *)entry);
    }
    viewportClipLeft = gViewRect;
}

void SimDrawBackground(SimGame *game)
{
    viewportClipLeft = gViewRect;
    GuiRect rect;
    int changed,full;
    if (game->flags & 1) {
        extern char bestLocEntries[];
        extern int bestLocCount;
        if (game->focus && game->focus->owner == *(int *)((char *)game+0x97e)) {
            int unit = f_00438678(game->focus->unit);
            DisplayProcess((int *)gView,bestLocEntries,unit,bestLocCount);
        } else DisplayProcess((int *)gView);
        PopDrawSurface(&g513888,0,1,gViewRect.left,gViewRect.top,gViewRect.right-gViewRect.left+1,gViewRect.bottom-gViewRect.top+1);
    } else {
        if (f_0047b000(&drawDetailChanged,&rect))
            PopDrawSurface(&g513888,1,0,rect.x,rect.y,rect.width,rect.height);
    }
}

void SimDrawCursor()
{
    if (drawCursorTarget)
        AnimSetViewportRect((int *)&drawCursorImages->images[166],(int *)&drawCursorPoint,drawCursorRect.x,drawCursorRect.y,drawCursorRect.width,drawCursorRect.height);
}

extern void *gDrawRemap;
void SimDrawFrame(SimGame *game)
{
    viewportClipLeft = gViewRect;
    if (gGameTurn) ++drawFrame;
    SimBuildDrawQueue(game);
    SimDrawBackground(game);
    SimDrawCursor();
    SimDrawQueue(game);
    gDrawRemap = 0;
    viewportClipLeft = gViewRect;
    LogicDrawConsole(g4dd0c0);
    f_004517c0(game);
    ExploAnimateAll();
    viewportClipLeft = gViewRect;
}

