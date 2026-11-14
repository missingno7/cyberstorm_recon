// terr\trender.c: XI-anchored map view and its two initialized color tables.
struct GridPoint { int x, y; GridPoint() {} };
struct ViewRect { GridPoint top; int width, height; ViewRect() {} ViewRect(int x, int y, int w, int h) { top.x=x; top.y=y; width=w; height=h; } void Center(GridPoint *p) { p->x=top.x+(width>>1); p->y=top.y+(height>>1); } };
struct ViewState { int mode; GridPoint position; int scale, rotation, extra1, extra2; };
struct FastCoord {
    int *grid; int reserved[4]; int radius, count, origin;
    GridPoint center; int mode; GridPoint position; int scale;
    ViewRect bounds; GridPoint offset; int rotation, angle;
};
struct VisitPoint { int *grid; GridPoint point; int index; };
inline int MapOrigin(FastCoord *self) { return self->grid[6]; }
extern int BestLocInitialize(void *, const char *, int);
extern int BestLocSetCandidate(void *, void *);
extern void BestLocVisit(void *, int (*)(void *));
extern void GridRotatePoint(GridPoint *, long);
extern void f_0047efb0(void *, int);
extern void MenuSetColors(void *, unsigned long, long);
extern void MenuFill(void *, long, long, long, long);
extern void MenuLine(void *, long, long, long, long);
extern void SimMarkPosition(long, long, int, int);
extern unsigned int ISqrt(unsigned int);
extern int f_00432d60(int *, int);
extern unsigned long gMenuColors[];
extern unsigned char worldColorInfo[];
extern char g513888[];
extern unsigned char *MapDetails;
static char trenderSourceFile[] = __FILE__;
unsigned char MapHeightColors[8] = {237,238,239,240,241,242,243,244};
unsigned char MapThreatColors[11] = {241,239,238,237,236,235,234,234,234,0,0};
// BSS fields belong to this object. Addresses are inferred only by the comparator.
int MapPick, MapBestDistance;
GridPoint MapPickPoint;
FastCoord *MapView;
void *MapVisibility;
int MapDrawScale;

// MapPickPoint is a global class object; its constructor generates the XI initializer.
extern "C" void SetFastView(FastCoord *, const ViewState *);

FastCoord *MapInitialize(FastCoord *self, const char *name, int radius)
{
    self = (FastCoord *)BestLocInitialize(self, name, radius);
    self->mode = 0;
    ViewState state;
    state.extra1 = 0; state.extra2 = 0;
    state.mode = 0; state.position.x = 0; state.position.y = 0;
    state.scale = 2; state.rotation = 0;
    SetFastView(self, &state);
    return self;
}

void *MapIdentity(void *self) { return self; }
void MapSetVisibility(FastCoord *, void *visibility) { MapVisibility = visibility; }

void MapGetView(FastCoord *self, ViewState *state)
{
    state->mode = self->mode;
    GridPoint *src = &self->position; state->position = *src;
    state->scale = self->scale;
    state->rotation = self->rotation;
}

extern "C" void SetFastView(FastCoord *self, const ViewState *state)
{
    self->mode = state->mode;
    self->position = state->position;
    int scale = state->scale;
    if (scale <= 2) scale = 2;
    else if (scale >= 8) scale = 8;
    self->scale = scale;
    self->rotation = state->rotation % 6;
    if (self->rotation < 0) self->rotation += 6;
    self->angle = self->rotation * 0x2aaaaabc;
    int side = (self->radius * 2 + 1) * self->scale;
    ViewRect rect(self->position.x, self->position.y, side, side);
    self->bounds = rect;
    self->offset.x = self->bounds.top.x + (self->bounds.width >> 1);
    self->offset.y = self->bounds.top.y + (self->bounds.height >> 1);
    --self->offset.x; --self->offset.y;
}

int MapPointScale(FastCoord *self, int value)
{
    int numerator = value * self->scale;
    int *grid = self->grid; int divisor = grid[1];
    return numerator / divisor;
}

void MapPointToScreen(FastCoord *self, GridPoint *out, GridPoint *point)
{
    *out = *point;
    f_0047efb0(out, self->angle);
    out->x = out->x * 0x128 >> 8;
    out->y = -out->y * 2;
    out->x *= self->scale; out->y *= self->scale >> 1;
    int divisor = self->grid[1];
    out->x /= divisor; out->y /= divisor;
    GridPoint *offset = &self->offset;
    out->x += offset->x; out->y += offset->y;
}

void GridConvert(FastCoord *self, GridPoint *out, GridPoint *point)
{
    *out = *point;
    GridPoint *origin = (GridPoint *)(self->grid + 4);
    out->x -= origin->x; out->y -= origin->y;
    GridRotatePoint(out, self->rotation);
    out->x *= self->scale; out->y *= self->scale >> 1;
    GridPoint *offset = &self->offset;
    out->x += offset->x; out->y += offset->y;
}

struct VisibilityMap { int reserved[3]; char *visible; char *threat; };
inline VisibilityMap *Visibility() { return (VisibilityMap *)MapVisibility; }
inline int MapHidden(int index) { return !Visibility()->visible[index]; }
int MapDrawPoint(VisitPoint *visit)
{
    int index = visit->index;
    if (index) {
        unsigned char *tile = (unsigned char *)visit->grid[17] + index * 12;
        GridPoint screen;
        GridPoint *point = &visit->point;
        if (MapDrawScale == 2) {
            unsigned char *detail = &MapDetails[index * 8 + 4];
            screen.x = detail[0];
            screen.y = detail[1];
        } else GridConvert(MapView, &screen, point);
        unsigned int color;
        if (MapVisibility && MapHidden(index))
            color = gMenuColors[0];
        else switch (MapView->mode) {
        case 0: color = MapHeightColors[7 - (tile[1] & 7)]; break;
        case 1:
            if (tile[3] == 0x42) color = gMenuColors[6];
            else if (tile[3] == 0x41) color = gMenuColors[1];
            else color = MapHeightColors[(tile[0] >> 1) & 7];
            break;
        case 2:
            if (tile[3] == 0x42) color = gMenuColors[6];
            else if (tile[3] == 0x41) color = gMenuColors[1];
            else color = worldColorInfo[tile[0] * 8 + tile[1]];
            break;
        case 3: color = Visibility()->threat[index] != 0 ? 0x4b : 0x48; break;
        case 4: color = MapThreatColors[f_00432d60(visit->grid, index) / 10]; break;
        }
        if (MapDrawScale != 2) {
            MenuSetColors(g513888, color, color);
            int side = MapView->scale;
            MenuFill(g513888, screen.x - (side >> 1), screen.y - (side >> 1), side, side);
        } else SimMarkPosition(screen.x, screen.y, 2, color);
    }
    return 0;
}

extern "C" void PrepareDeathMap(FastCoord *self)
{
    BestLocSetCandidate(self, (void *)MapOrigin(self));
    MapView = self; MapDrawScale = self->scale;
    BestLocVisit(self, (int (*)(void *))MapDrawPoint);
}

// The original Point class has a non-elided default constructor for arrays.
struct GuiPoint { long x, y; GuiPoint(); GuiPoint(int a, int b) { x=a; y=b; } };
inline GuiPoint::GuiPoint() {}
typedef GuiPoint OutlinePoint;
extern "C" void DrawDeathMap(FastCoord *self, unsigned long color, int padding)
{
    int half = self->grid[2] * (self->scale >> 1);
    int twice = half * 2, triple = half + twice, whole = twice * 2;
    int x = self->position.x, y = self->position.y;
    OutlinePoint vertices[6];
    vertices[0] = OutlinePoint(twice + x, y - padding);
    vertices[1] = OutlinePoint(padding + whole + x, half + y);
    vertices[2] = OutlinePoint(padding + whole + x, triple + y);
    vertices[3] = OutlinePoint(twice + x, y + whole + padding);
    vertices[4] = OutlinePoint(x - padding, triple + y);
    vertices[5] = OutlinePoint(x - padding, half + y);
    MenuSetColors(g513888, color, -1);
    int previous = 5;
    for (int i = 0; i < 6; ++i) {
        MenuLine(g513888, vertices[previous].x, vertices[previous].y, vertices[i].x, vertices[i].y);
        previous = i;
    }
}

int MapContainsPoint(FastCoord *self, GridPoint *point, int index)
{
    if (index) {
        GridPoint screen;
        GridConvert(self, &screen, point);
        ViewRect rect(screen.x - (self->scale >> 1), screen.y - (self->scale >> 1), self->scale, self->scale);
        int inside = rect.top.x <= MapPickPoint.x && rect.top.x + rect.width > MapPickPoint.x &&
                     rect.top.y <= MapPickPoint.y && rect.top.y + rect.height > MapPickPoint.y;
        if (inside) return 1;
    }
    return 0;
}

int MapPickVisit(void *item)
{
    VisitPoint *visit = (VisitPoint *)item;
    int index = visit->index;
    if (MapContainsPoint(MapView, &visit->point, index)) { MapPick = index; return 1; }
    return 0;
}

int MapPickAt(FastCoord *self, GridPoint *point)
{
    MapPickPoint = *point; MapPick = 0;
    ViewRect *rect = &self->bounds;
    int inside = point->x >= rect->top.x && point->x < rect->top.x + rect->width &&
                 point->y >= rect->top.y && point->y < rect->top.y + rect->height;
    if (inside) {
        BestLocSetCandidate(self, (void *)MapOrigin(self)); MapView = self;
        BestLocVisit(self, MapPickVisit);
    }
    return MapPick;
}

int MapNearestVisit(void *item)
{
    VisitPoint *visit = (VisitPoint *)item;
    GridPoint screen;
    GridConvert(MapView, &screen, &visit->point);
    int dx = MapPickPoint.x - screen.x, dy = MapPickPoint.y - screen.y;
    int distance = ISqrt(dx * dx + dy * dy);
    if (distance < MapBestDistance) { MapBestDistance = distance; MapPick = visit->index; }
    return 0;
}

int MapNearestAt(FastCoord *self, GridPoint *point)
{
    MapPickPoint = *point; MapPick = 0; MapBestDistance = 0x7fffffff;
    BestLocSetCandidate(self, (void *)MapOrigin(self)); MapView = self;
    BestLocVisit(self, MapNearestVisit);
    return MapPick;
}



