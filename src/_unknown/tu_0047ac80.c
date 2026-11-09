/* Unknown original path. Working names describe clipping and dirty rectangles. */
struct Rect { int left, top, right, bottom; };
struct GuiRect { int x, y, width, height; };
struct ClipState { Rect bounds, dirty; int pending, full; };
extern int g513888;
extern void f_0047b3d0(void *,long,long,long,long);
extern void SimAlertPrepare(void *,const GuiRect *);
extern void SurfaceSetClipSize(void *,int,int,int,int);
extern void SurfaceSetClip(void *,const Rect *);

int MenuSetClip(ClipState *c,int x,int y,int w,int h)
{
    if (!c->full) {
        int r=x+w-1, b=y+h-1;
        if (x>c->bounds.right || y>c->bounds.bottom || r<c->bounds.left || b<c->bounds.top) return 0;
        if (x<c->dirty.left) c->dirty.left=x;
        if (y<c->dirty.top) c->dirty.top=y;
        if (r>c->dirty.right) c->dirty.right=r;
        if (b>c->dirty.bottom) c->dirty.bottom=b;
        c->pending=1;
    }
    return 1;
}
int GuiRemapClipRect(ClipState *c,int x,int y,int r,int b)
{
    if (!c->full) {
        if (x>c->bounds.right || y>c->bounds.bottom || r<c->bounds.left || b<c->bounds.top) return 0;
        if (x<c->dirty.left) c->dirty.left=x;
        if (y<c->dirty.top) c->dirty.top=y;
        if (r>c->dirty.right) c->dirty.right=r;
        if (b>c->dirty.bottom) c->dirty.bottom=b;
        c->pending=1;
    }
    return 1;
}
int ClipAddRect(ClipState *c,const Rect *r)
{ return GuiRemapClipRect(c,r->left,r->top,r->right,r->bottom); }
int ClipAddDirty(ClipState *c,const ClipState *s)
{
    if (s->pending) return GuiRemapClipRect(c,s->dirty.left,s->dirty.top,s->dirty.right,s->dirty.bottom);
    return 1;
}
int ClipAddSize(ClipState *c,const GuiRect *r)
{
    if (!c->full) return GuiRemapClipRect(c,r->x,r->y,r->x+r->width-1,r->y+r->height-1);
    return 1;
}
void GuiResetClip(ClipState *c)
{ c->dirty=c->bounds; c->pending=c->full=1; }
void SimClipFlush(ClipState *c)
{ c->dirty.left=0x7fffffff; c->dirty.top=0x7fffffff; c->dirty.right=-1; c->dirty.bottom=-1; c->pending=c->full=0; }
void ClipInitializeSize(ClipState *c,const GuiRect *r)
{
    c->bounds.left=r->x; c->bounds.top=r->y;
    c->bounds.right=r->x+r->width-1; c->bounds.bottom=r->y+r->height-1;
    GuiResetClip(c);
}
void ClipInitialize(ClipState *c,const Rect *r)
{ c->bounds=*r; GuiResetClip(c); }
int ClipGetSize(ClipState *c,GuiRect *out)
{
    if (!c->pending || c->dirty.right<c->dirty.left || c->dirty.bottom<c->dirty.top) return 0;
    if (c->dirty.right<c->bounds.left || c->dirty.left>c->bounds.right || c->dirty.bottom<c->bounds.top || c->dirty.top>c->bounds.bottom) return 0;
    Rect r=c->dirty;
    if (r.left<c->bounds.left) r.left=c->bounds.left;
    if (r.right<c->bounds.left) r.right=c->bounds.left;
    if (r.top<c->bounds.top) r.top=c->bounds.top;
    if (r.bottom<c->bounds.top) r.bottom=c->bounds.top;
    if (r.left>c->bounds.right) r.left=c->bounds.right;
    if (r.right>c->bounds.right) r.right=c->bounds.right;
    if (r.top>c->bounds.bottom) r.top=c->bounds.bottom;
    if (r.bottom>c->bounds.bottom) r.bottom=c->bounds.bottom;
    out->x=r.left; out->y=r.top;
    out->width=r.right-r.left+1; out->height=r.bottom-r.top+1;
    return 1;
}
int f_0047b000(ClipState *c,GuiRect *out)
{
    if (!ClipGetSize(c,out)) return 0;
    int x=out->x;
    out->x &= ~3;
    out->width=(x+out->width-out->x+3)&~3;
    return 1;
}
void SimClipRestore(ClipState *c)
{
    GuiRect r;
    if (f_0047b000(c,&r)) {
        f_0047b3d0(&g513888,r.x,r.y,r.width,r.height);
        SimClipFlush(c);
    }
}
int SimClipIntersects(const ClipState *c,const ClipState *s)
{
    if (!s->pending) return 1;
    if (c->pending && c->dirty.left<=s->dirty.left && c->dirty.top<=s->dirty.top && c->dirty.right>=s->dirty.right && c->dirty.bottom>=s->dirty.bottom) return 1;
    return 0;
}
int ClipIntersectsSize(ClipState *c,int x,int y,int w,int h)
{
    if (x<=c->bounds.right && y<=c->bounds.bottom && x+w-1>=c->bounds.left && y+h-1>=c->bounds.top) return 1;
    return 0;
}
int ClipIntersectsRect(ClipState *c,const GuiRect *r)
{
    if (r->x<=c->bounds.right && r->y<=c->bounds.bottom && r->x+r->width-1>=c->bounds.left && r->y+r->height-1>=c->bounds.top) return 1;
    return 0;
}
void ClipApply(ClipState *c)
{
    GuiRect r;
    if (f_0047b000(c,&r)) SimAlertPrepare(&g513888,&r);
    else SurfaceSetClipSize(&g513888,c->bounds.left,c->bounds.top,1,1);
}
void ClipApplyRect(const Rect *r)
{ SurfaceSetClip(&g513888,r); }
