struct DisplayRect {
    int x, y, width, height;
    DisplayRect(int a,int b,int w,int h) { x=a; y=b; width=w; height=h; }
    int Contains(const int *p) const {
        if (p[0]>=x && p[0]<x+width && p[1]>=y && p[1]<y+height) return 1;
        return 0;
    }
};
struct AreaWalk { int x, y; unsigned int step; int phase, edge; };

extern "C" void __wcpp_2_ctor_array_(void *objects, int count, void *constructor);
extern int BestLocInitialize(void *, const char *, int);
extern void BestLocSetData(void *, void *, void *);



void DisplayInitialize(void *object, const char *name, int index)
{
    BestLocInitialize(object, name, index);
}

void *DisplayIdentity(void *value)
{
    return value;
}

void DisplaySetRect(void *object, const DisplayRect *source)
{
    DisplayRect *dest = (DisplayRect *)((char *)object + 0x28); *dest = *source;
    int *fields = (int *)object;
    fields[0x0e] = fields[0x0a] + (fields[0x0c] >> 1);
    fields[0x0f] = fields[0x0b] + (fields[0x0d] >> 1);
    fields[0x10] = fields[0x0a] + fields[0x0c] - 1;
    fields[0x11] = fields[0x0b] + fields[0x0d] - 1;
}

static const char DisplaySourcePath[] = "terr\\tdisplay.c";
extern "C" void AssertFail(const char *, int, const char *);
struct GuiPoint;
struct DisplayTile { unsigned char type, height; unsigned char reserved[10]; };
struct TerrainGrid {
    char reserved[0x10]; int origin[2]; char unknown18[0x2c]; DisplayTile *tiles;
    GuiPoint *Origin() { return (GuiPoint *)origin; }
    DisplayTile *Tile(int index) { return tiles+index; }
};
struct GuiPoint {
    long x, y;
    GuiPoint(); ~GuiPoint() {}
    GuiPoint(int a,int b) { x=a; y=b; }
    GuiPoint(const GuiPoint &p) { x=p.x; y=p.y; }
    long &Y() { return y; }
    GuiPoint &operator+=(const GuiPoint &p) { x+=p.x; y+=p.y; return *this; }
    GuiPoint &operator-=(const GuiPoint &p) { x-=p.x; y-=p.y; return *this; }
};
inline GuiPoint::GuiPoint() {}
extern long GridPointToIndex(TerrainGrid *, GuiPoint *);
extern void GridRotatePoint(GuiPoint *, long);
struct FastBuffer { void Update(long, long); };
extern void SetTerrainSpanSize(int); extern void f_0047efb0(void *, int);
extern "C" void TargetObjectInit(const char *, void *, int);
extern "C" int abs(int); extern GuiPoint DisplayPickPoint; extern int *DisplayCurrentView; extern int DisplayPickedIndex, DisplayPickedHeight; extern int DisplayUnraisedY; extern int heightStep; struct DisplayVisibilityState { char reserved[12]; char *values; int Hidden(int index) { return !values[index]; } }; extern DisplayVisibilityState *TerrainVisibility;
void DisplayGetBounds(int *, unsigned int *, unsigned int *);

struct WorldView {
    TerrainGrid *grid;
    int reserved[4], radius, count, origin;
    GuiPoint center;
    DisplayRect rect;
    GuiPoint screenCenter;
    int right, bottom;
    GuiPoint focus;
    int rotation;
    GuiPoint offset, aligned;
    int index, scale, twice, triple, quad, heightScale, range, angle, flags, special;
};
struct DisplaySettings {GuiPoint point;int detail,scale,rotation,flags,unknown18,height;};
inline int DisplayClamp(int n,int low,int high){if(n<=low)return low;if(n>=high)return high;return n;} extern FastBuffer g4d35e4;
inline int DisplayMaxRef(int a,const int &b){return a>b?a:b;}
void DisplayConfigure(WorldView *view, DisplaySettings *settings, int reset)
{
    view->focus=settings->point;
    view->rotation=settings->rotation%6;if(view->rotation<0)view->rotation+=6;
    view->angle=view->rotation*0x2aaaaabc;view->flags=settings->flags;
    int scale=settings->scale;view->scale=scale;view->scale=DisplayMaxRef(scale,1);
    view->twice=view->scale*2;view->triple=view->scale+view->twice;view->quad=view->twice*2;
    SetTerrainSpanSize(view->scale);
    view->heightScale=view->scale*settings->height*heightStep/50000;
    if(!view->heightScale)view->flags&=~1;
    int estimate=(view->rect.width+view->triple-1)/view->triple+1;
    view->range=estimate>>1;view->range+=(estimate>>3)+2;
    view->range=DisplayClamp(view->range,5,view->radius*2);
    if(view->range>40)view->flags&=~1;
    if(!(view->flags&1))view->heightScale=0;
    view->index=GridPointToIndex(view->grid,&view->focus);


















    unsigned int assertedIndex=view->index;if(!assertedIndex)AssertFail(DisplaySourcePath,__LINE__,(const char *)assertedIndex);
    GuiPoint bounds;
    view->offset=GuiPoint(0,0);
    DisplayGetBounds((int *)view,(unsigned int *)&bounds,(unsigned int *)&view->focus);
    view->offset=bounds;view->offset-=view->screenCenter;
    if(view->flags&1) {
        GuiPoint saved=view->offset;view->offset=GuiPoint(0,0);
        int flags=view->flags;view->flags=flags&~1;
        DisplayGetBounds((int *)view,(unsigned int *)&bounds,(unsigned int *)&view->focus);
        view->flags=flags;view->offset=bounds;view->offset-=view->screenCenter;
        static GuiPoint shift(0,0);
        if(reset)view->offset+=shift;
        else {shift=saved;shift-=view->offset;view->offset=saved;}
    }
    if(!(view->scale&3))view->offset.x+=(view->screenCenter.x-view->offset.x)&3U;
    view->aligned=view->screenCenter;view->aligned-=view->offset;
    g4d35e4.Update(settings->rotation,settings->detail);
}

int DisplayZoomArea(int *view, unsigned int *result, int position, int candidate, int adjust)
{
    if (position == 0) return 0;
    GuiPoint origin;
    GuiPoint originPoint;
    BestLocSetData((void *)*view, &origin, (void *)position);
    TargetObjectInit((const char *)*view, &originPoint, position);
    int scaleCount = view[0x0d] / view[0x1d] + 1;
    int halfScale = scaleCount >> 1;
    GuiPoint target;
    GuiPoint targetPoint;
    int targetIndex;
    if(!candidate) {BestLocSetData((void *)*view,&target,(void *)view[0x19]);targetIndex=view[0x19];}
    else {BestLocSetData((void *)*view,&target,(void *)candidate);targetIndex=candidate;}
    TargetObjectInit((const char *)*view,&targetPoint,targetIndex);
    GuiPoint delta;delta.x=targetPoint.x-originPoint.x;delta.y=targetPoint.y-originPoint.y;
    f_0047efb0(&delta, view[0x20]);
    delta.x = (int)(delta.x * view[0x0d]) / view[0x0c];
    int vertical = abs(delta.y);
    int horizontal = abs((int)delta.x);
    if (horizontal <= vertical) horizontal = vertical;
    int rings = horizontal / *(int *)(*view + 4) + 1;
    if (candidate == 0) {
        if (((adjust == 0 || view[0x1a] == 0xe) && rings < halfScale)) return 0;
        result[0] = originPoint.x; result[1] = originPoint.y;
        if (adjust != 0) result[3] = 0xe;
    } else {
        if (rings < halfScale) {
            if (rings < (scaleCount >> 2) && adjust != 0) {
                unsigned int level = result[3];
                if ((int)level < 2) level = 1;
                result[3] = level;
                while (rings < (halfScale >> 1) && (int)result[3] < 0xe) {
                    ++result[3];
                    halfScale = view[0x0d] / (int)(result[3] * 4) + 1 >> 1;
                }
            }
        } else {
            unsigned int oldLevel = result[3];
            if (adjust != 0) {
                while (halfScale <= rings && (int)result[3] > 4) {
                    --result[3];
                    halfScale = view[0x0d] / (int)(result[3] * 4) + 1 >> 1;
                }
            }
            if (halfScale <= rings) {
                result[3] = oldLevel;
                return 0;
            }
        }
        result[0] = (originPoint.x + targetPoint.x) >> 1;
        result[1] = (originPoint.y + targetPoint.y) >> 1;
    }
    return 1;
}

int DisplayScale(int *object)
{
    int numerator = object[0x1c] - 1 + object[0x0c];
    int value = (numerator / object[0x1c] + 1) >> 1;
    int limit = object[5]; if (value <= 1) return 1;
    if (value >= limit) value = limit;
    return value;
}

int DisplayPointScale(int *object, int value)
{
    int numerator = value * object[0x1d];
    int *base = (int *)*object;
    int divisor = base[1];
    return numerator / divisor;
}

void DisplayGetBounds(int *view, unsigned int *output, unsigned int *point)
{
    *(GuiPoint *)output = *(GuiPoint *)point;
    f_0047efb0(output, view[0x20]);
    ((int *)output)[0] = (int)(((int *)output)[0] * 0x128) >> 8;
    ((int *)output)[1] = -((int *)output)[1] * 2;
    ((int *)output)[0] *= view[0x1c];
    ((int *)output)[1] *= view[0x1b];
    int tileSize = *(int *)(*view + 4);
    ((int *)output)[0] = (int)((int *)output)[0] / tileSize;
    ((int *)output)[1] = (int)((int *)output)[1] / tileSize;
    if ((((unsigned char *)(view + 0x21))[0] & 1) != 0) {
        int index = GridPointToIndex((TerrainGrid *)*view, (GuiPoint *)point);
        if (index != 0 && (TerrainVisibility == 0 || !TerrainVisibility->Hidden(index))) {
            ((GuiPoint *)output)->Y() -= ((TerrainGrid *)*view)->tiles[index].height * view[0x1e];
        }
    }
    *(GuiPoint *)output += *(GuiPoint *)(view + 0x0e);
    *(GuiPoint *)output -= *(GuiPoint *)(view + 0x15);
}

void DisplayPointToScreen(int *view, int *output, int *point, int index)
{
    *(GuiPoint *)output = *(GuiPoint *)point;
    GuiPoint *origin = (GuiPoint *)(*view + 0x10); ((int *)output)[0] -= origin->x; ((int *)output)[1] -= origin->y;
    GridRotatePoint((GuiPoint *)output, view[0x14]);
    ((int *)output)[0] *= view[0x1c];
    output[1] *= view[0x1b];
    if ((((unsigned char *)(view + 0x21))[0] & 1) != 0) {
        DisplayUnraisedY = output[1];
        if (index != 0 && (TerrainVisibility == 0 || !TerrainVisibility->Hidden(index))) {
            ((GuiPoint *)output)->Y() -= ((TerrainGrid *)*view)->tiles[index].height * view[0x1e];
        }
    }
    *(GuiPoint *)output += *(GuiPoint *)(view + 0x0e);
    *(GuiPoint *)output -= *(GuiPoint *)(view + 0x15);
}

int DisplayPointToScreenScaled(int *view, unsigned int *output, unsigned int *point)
{
    *(GuiPoint *)output = *(GuiPoint *)point;
    f_0047efb0(output, view[0x20]);
    ((int *)output)[0] = (int)(((int *)output)[0] * 0x128) >> 8;
    ((int *)output)[1] = -((int *)output)[1] * 2;
    ((int *)output)[0] *= view[0x1c]; ((int *)output)[1] *= view[0x1b];
    int tileSize = *(int *)(*view + 4);
    ((int *)output)[0] = (int)((int *)output)[0] / tileSize; ((int *)output)[1] = (int)((int *)output)[1] / tileSize;
    *(GuiPoint *)output += *(GuiPoint *)(view + 0x0e);
    *(GuiPoint *)output -= *(GuiPoint *)(view + 0x15);
    return view[0x1e];
}

int DisplayRectOutside(int viewAddress, int *rect)
{
 int *view=(int *)viewAddress;
 if (rect[0] - view[0x1d] >= view[0x0a] + view[0x0c] || rect[1] - view[0x1b] >= view[0x0b] + view[0x0d] || rect[0] + view[0x1d] < view[0x0a] || rect[1] + view[0x1b] + view[0x1d] < view[0x0b]) return 1; return 0;
}

extern void f_0042d640(int *, unsigned int, int *, unsigned int);
extern int f_0042b3d0(int *, int);

struct DisplayRenderItem { GuiPoint point; int key; unsigned int index; DisplayRenderItem(); };
DisplayRenderItem DisplayItems[2000]; extern DisplayRenderItem *DisplaySorted[2000];
extern int DisplayItemCount; extern void *DisplayDrawArg1,*DisplayDrawArg2,*DisplayDrawArg3;
extern char g513888[]; extern void MenuSetColors(void *,unsigned long,long); extern void MenuFill(void *,long,long,long,long);
struct DrawPoint; extern void TerrainDrawCliff(int *,int,DrawPoint *,int); extern void TerrainDrawTile(int *,unsigned int,DrawPoint *,unsigned int);
extern WorldView gView; struct TargetTriple {int x,y,z;}; extern TargetTriple *targetDefinitionTable; extern int logicWorldOffsetX,logicWorldOffsetY; extern unsigned char *simVisibleTypes;
inline int DisplayMainHidden(unsigned int index) {return !simVisibleTypes[index];}
inline unsigned int DisplayMainHeight(unsigned int index) {if(index)return ((DisplayTile *)targetDefinitionTable)[index].height;return 0;}
int DisplaySubmitElement(int *element)
{
    unsigned int objectId=element[3];
    unsigned int height=DisplayMainHeight(objectId);
    GuiPoint point=*(GuiPoint *)(element+1);
    point.x-=logicWorldOffsetX;point.y-=logicWorldOffsetY;
    GridRotatePoint(&point,gView.rotation);
    point.x*=gView.triple;point.y*=gView.twice;
    unsigned int lower=0;
    if((gView.flags&1) && objectId && !DisplayMainHidden(objectId))lower=height;
    point+=gView.aligned;
    DisplayUnraisedY=point.y;
    if(lower)point.y-=gView.heightScale*lower;
    if(point.x-gView.twice<=gView.right && point.y-gView.twice<=gView.bottom && gView.rect.x<=point.x+gView.twice && gView.rect.y<=DisplayUnraisedY+gView.twice) {
        if((int)lower>0) {
            if(DisplayItemCount<2000) {
                DisplayRenderItem *record=DisplayItems+DisplayItemCount++;
                record->point=point;
                int key=DisplayUnraisedY*16+height;record->index=objectId;record->key=key;record->key=-key;
            }
        } else TerrainDrawTile((int *)&gView,objectId,(DrawPoint *)&point,height);
    }
    return 0;
}

inline void DisplaySortItems(void **items, int count)
{
    int gaps[5]={9,5,3,2,1};
    for (int gapIndex=0;gapIndex<5;++gapIndex) {
        int gap=gaps[gapIndex];
        for (int i=gap; i<count; ++i) {
            int index=i-gap;
            void *item=items[i];
            for (; index>=0; index-=gap) { void *prior=items[index]; if (*(int *)((char *)item+8)<*(int *)((char *)prior+8)) break; items[index+gap]=prior; }
            items[index+gap]=item;
        }
    }
}
void DisplaySort(void **items, int count)
{
    int gaps[5]={9,5,3,2,1};
    for (int gapIndex=0;gapIndex<5;++gapIndex) {
        int gap=gaps[gapIndex];
        for (int i=gap; i<count; ++i) {
            int index=i-gap;
            void *item=items[i];
            for (; index>=0; index-=gap) { void *prior=items[index]; if (*(int *)((char *)item+8)<*(int *)((char *)prior+8)) break; items[index+gap]=prior; }
            items[index+gap]=item;
        }
    }
}

extern void AreaEnumerate(int *, void *, unsigned int, unsigned int);
extern void f_0047b930(int, int, int);
extern void f_0047bad0(int, int, int, int, int);
extern void f_0042de30(int *, int, int *, unsigned int);
extern "C" void __wcpp_2_ctor_array_(void *, int, void *);
extern "C" int PointInConvexPolygon(int *,void *,int);
extern int ResolveName(const char *, const char *);
void DisplaySetCandidate(int *, int);

void DisplayProcess(int *view,void *argument1,void *argument2,void *argument3)
{
 DisplaySetCandidate(view,view[0x19]);DisplayCurrentView=view;DisplayDrawArg1=argument1;DisplayDrawArg2=argument2;DisplayDrawArg3=argument3;DisplayItemCount=0;
 if(view[0x1a]<=8){MenuSetColors(g513888,0,0);MenuFill(g513888,view[10],view[11],view[12],view[13]);}
 AreaEnumerate(view,(void *)DisplaySubmitElement,0,view[0x1f]);
 int i=0;DisplayRenderItem *item=DisplayItems;DisplayRenderItem **out=DisplaySorted;for(;i<DisplayItemCount;++i,++out,++item)*out=item;
 DisplaySortItems((void **)DisplaySorted,DisplayItemCount);
 int index=0;DisplayRenderItem **current=DisplaySorted;
 if(DisplayItemCount>0)do {DisplayRenderItem *item=*current;int location=item->index;unsigned int height=((TerrainGrid *)*view)->tiles[location].height;
 TerrainDrawCliff(view,location,(DrawPoint *)&item->point,height);
 ++index;++current;TerrainDrawTile(view,item->index,(DrawPoint *)&item->point,height);
 }while(index<DisplayItemCount);
}

int DisplayResolveCandidate(int *view, int *point, int index)
{
    if(index) {
        GuiPoint screen;
        DisplayPointToScreen(view,(int *)&screen,point,index);
        int left=screen.x-view[0x1b],top=screen.y-view[0x1b];
        DisplayRect rect(left,top,view[0x1d],view[0x1d]);
        if(rect.Contains((int *)&DisplayPickPoint)) {
            GuiPoint corners[6]; GuiPoint *out=corners;
            { GuiPoint &dst=*out; dst=GuiPoint(left+view[0x1a],top); ++out; }
            { GuiPoint &dst=*out; dst=GuiPoint(left+view[0x1c],top); ++out; }
            { GuiPoint &dst=*out; dst=GuiPoint(left+view[0x1d],top+view[0x1b]); ++out; }
            { GuiPoint &dst=*out; dst=GuiPoint(left+view[0x1c],top+view[0x1d]); ++out; }
            { GuiPoint &dst=*out; dst=GuiPoint(left+view[0x1a],top+view[0x1d]); ++out; }
            { GuiPoint &dst=*out; dst=GuiPoint(left,top+view[0x1b]); ++out; }
            return PointInConvexPolygon((int *)&DisplayPickPoint,corners,6);
        }
    }
    return 0;
}

int DisplayConsiderElement(int *item)
{
    int index = item[3];
    if (index != 0 && DisplayResolveCandidate(DisplayCurrentView, item + 1, index) != 0) {
        unsigned int height = 0;
        if (!TerrainVisibility || !TerrainVisibility->Hidden(index)) {
            height = ((TerrainGrid *)*item)->tiles[index].height;
        }
        if (DisplayPickedIndex == 0 || (int)DisplayPickedHeight < (int)height) {
            DisplayPickedIndex = index;
            DisplayPickedHeight = height;
        }
    }
    return 0;
}

int DisplayPickAt(int *view, int *point)
{
    DisplayPickedIndex=0;
    DisplayRect *rect=(DisplayRect *)(view+10);
    if (rect->Contains(point)) {
        DisplayCurrentView=view;
        DisplayPickPoint=*(GuiPoint *)point;
        GuiPoint gridPoint=*(GuiPoint *)point;
        gridPoint += *(GuiPoint *)(view+0x15);
        gridPoint -= *(GuiPoint *)(view+0x0e);
        if (((unsigned char *)(view+0x21))[0]&1) gridPoint.y += view[0x1e]*3;
        gridPoint.y /= view[0x1b];
        gridPoint.x /= view[0x1c];
        GridRotatePoint(&gridPoint,-view[0x14]);
        GuiPoint *origin=((TerrainGrid *)*view)->Origin(); gridPoint += *origin;
        int index=ResolveName((const char *)*view,(const char *)&gridPoint);
        if (index) {
            DisplaySetCandidate(view,index);
            AreaEnumerate(view,(void *)DisplayConsiderElement,0,4);
        }
    }
    return DisplayPickedIndex;
}

/* GuiPoint destructor is the shared COMDAT at 0042c9d0. */
/* DisplayRenderItem destructor is compiler generated. */
inline DisplayRenderItem::DisplayRenderItem() {}

void DisplayWalkInit(AreaWalk *walk, unsigned int step)
{
    walk->step = step;
    int twice = ((volatile AreaWalk *)walk)->step * 2;
    walk->x = 0;
    walk->y = twice;
    walk->edge = twice;
    walk->phase = walk->step > 0;
}

extern "C" void DisplayStep(AreaWalk *walk)
{
    switch ((unsigned int)walk->phase) {
    case 1:
        walk->phase *= 2;
        if (walk->x) { walk->x = -walk->x; return; }
    case 2:
        ++walk->phase;
        if (walk->y) { walk->y = -walk->y; return; }
    case 3:
        walk->phase = 0;
        if (walk->x && walk->y) { walk->x = -walk->x; return; }
    }
    walk->y = -walk->y;
    if (walk->y <= 1) {
        walk->x = 0;
        walk->edge += 2;
        walk->y = walk->edge;
        ++walk->step;
    } else if (walk->y > walk->x) {
        ++walk->x; --walk->y;
    } else walk->y -= 2;
    ++walk->phase;
}


int DisplayCount(int);
int BestLocInitialize(void *object, const char *name, int radius)
{
    *(const char **)object = name;
    ((int *)object)[5] = radius;
    int count = radius < 0 ? 0 : ((radius + 1) * radius >> 1) * 6 + 1;
    ((int *)object)[6] = count;
    return (int)object;
}

int DisplayCount(int value)
{
    if (value < 0) return 0;
    return (value * (value + 1) >> 1) * 6 + 1;
}

void DisplaySetCandidate(int *object, int value)
{
    object[7] = value;
    BestLocSetData((void *)*object, object + 8, (void *)object[7]);
}

int DisplaySetRadius(int *object,int value) {object[5]=value; if(value<0) value=0; else value=((value+1)*value>>1)*6+1; return object[6]=value;}












