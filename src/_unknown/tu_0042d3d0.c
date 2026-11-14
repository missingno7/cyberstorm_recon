// Working labels for terrain drawing. The object starts with its switch table at 42d3d0.
typedef unsigned int uint;
typedef unsigned char byte;
struct TerrainCell { byte type,height,feature,featureType,overlay,overlayType,shade,sprite; uint edges; }; struct TerrainMarker { byte unknown[4],sprite,unknown5[3]; }; struct TerrainWorld { byte header[68]; TerrainCell *cells; TerrainMarker *markers; };
struct TerrainCanvas { byte unknown[0x60]; void *remap; byte unknown64[0x10]; int left,top,right,bottom; };

struct TerrainView { TerrainWorld *world; int unknown04[19],rotation,unknown54[5],size,half,threeHalves,doubleSize,heightScale,unknown7c[2]; uint flags; int alternate; };

struct Point { int x, y; Point() {} Point(int a, int b) { x=a; y=b; } Point(const Point &p) { x=p.x; y=p.y; } Point &operator+=(const Point &p) { x+=p.x; y+=p.y; return *this; } };
typedef Point DrawPoint;
struct MenuImage { int offset, width, height; };
struct ExploFrame { int offset, width, height; };
#pragma pack(push,1)
struct TileSprite { byte first, kind, reserved[3], stride; unsigned short animated; };
struct TerrainDefinition { byte sprite, reserved[38]; };
struct TerrainAnimation { byte first, pad[7]; unsigned short alternate; byte pad2[2], stride; };
#pragma pack(pop)
struct FeatureType { byte base, type, unknown, defense, cover, stride; unsigned short animated; }; extern FeatureType featureTypes[];
extern TerrainDefinition TerrainDefinitions[];
extern TerrainAnimation TerrainFrames;
extern byte TerrainOutlineColors[];
struct MapDetail { signed char left,front,right; byte unknown[3],outline,spare; }; extern MapDetail *MapDetails;
struct TerrainVisibilityState { char reserved[12]; char *values; char *explored; int Hidden(int index) { return !values[index]; } int Unexplored(int index) { return !explored[index]; } }; extern TerrainVisibilityState *TerrainVisibility;
extern void *terrainImage, *hexCursor;
extern byte terrainTypeRemap[], hexMask[], worldColorInfo[], worldColorExtras[], worldColors[];
extern byte *TerrainScratch;
extern unsigned long gMenuColors[];
extern unsigned long TerrainPalette[];
extern char g513888[];
extern DrawPoint pathDirections[6];
extern "C" int ResolveName(const char *, const char *);
extern "C" void BestLocSetData(void *, void *, void *);
extern void MenuSetColors(void *, unsigned long, long);
extern void MenuLine(void *, long, long, long, long);
extern void MenuFill(void *, long, long, long, long);
extern void DrawSprite(void *, ExploFrame *, int, int, int, int, int);
extern MenuImage *MenuImageData(void *, const MenuImage *);
extern void f_00435c5e(MenuImage *, int, int, int, int, int);
extern void f_00435830(int, int);
extern void f_00435084(void *, void *, void *, uint, int);
extern void f_00435b5c(MenuImage *, int, int, uint);
extern void f_0047f114(MenuImage *, int, int, uint, int, int);
extern void f_00435520(MenuImage *, int, int, int, int, int);
extern void f_00435610(MenuImage *, int, int, int, int, int);
extern void f_00435700(MenuImage *, int, int, int, int);
extern "C" void *memcpy(void *, const void *, uint);

#define TerrainFrame(sprite) ((MenuImage *)((char *)terrainImage + 4) + (sprite))

void TerrainDrawOverlay(TerrainView *view, uint location, int sprite, int type, int x, int y, int frame)
{
    int selected = -1;
    if (type <= 64) {
        FeatureType *def = &featureTypes[type - 1];
        switch (def->type) {
        case 0: case 2: {
            int image = sprite;
            if (frame && def->animated) image = def->animated - def->base + def->stride * (frame - 1) + sprite;
            if ((char)((int)(location & 2) >> 1)) {
                int half = view->size / 2;
                x += half;
                if (def->type == 2) DrawSprite(g513888, (ExploFrame *)TerrainFrame(image), x, y + view->size, 0, view->threeHalves, view->threeHalves);
                else DrawSprite(g513888, (ExploFrame *)TerrainFrame(image), x, y + half, 0, view->threeHalves, view->threeHalves);
            } else selected = image;
            break;
        }
        case 3: case 4:
            selected = sprite;
            if (frame && def->animated) selected = def->animated - def->base + def->stride * (frame - 1) + sprite;
            break;
        case 1: {
            int relative = sprite - def->base;
            selected = def->base + ((6 + relative / 3 - view->rotation) % 6) * 3 + relative % 3;
            if (frame && def->animated) selected += def->animated - def->base + def->stride * (frame - 1);
            break;
        }
        }
    } else if (type == 66) {
        selected = sprite;
        if (view->alternate) selected += 6;
    }
    if (selected >= 0) {
        MenuImage *image = MenuImageData(g513888, TerrainFrame(selected));
        f_00435c5e(image, x, y, 0, view->doubleSize, view->doubleSize);
    }
}

void TerrainDrawTile(TerrainView *view, uint location, DrawPoint *point, uint frame)
{
    int x = point->x - view->half, y = point->y - view->half;
    if (y + view->doubleSize < ((TerrainCanvas *)g513888)->top) return;
    frame &= 7;
    int hidden = 0;
    if (location && TerrainVisibility) hidden = TerrainVisibility->Hidden(location);
    uint flags = view->flags;
    TerrainCell *tile = &(view->world)->cells[location];
    if (location && (flags & 2) && !hidden) {
        TerrainDefinition *def = TerrainDefinitions + tile->type;
        int animation = 0;
        if (frame && TerrainFrames.alternate) animation = TerrainFrames.alternate - TerrainFrames.first + (frame - 1) * TerrainFrames.stride;
        MenuImage *image = TerrainFrame(tile->sprite + def->sprite + animation);
        byte *pixels = (byte *)image + image->offset;
        if (tile->edges && (view->flags & 0x40)) {
            memcpy(TerrainScratch, pixels, 0x1000);
            uint edges = tile->edges;
            for (int edge = 0; edge < 6 && edges; ++edge, edges >>= 4) {
                if (edges & 15) {
                    MenuImage *layer = TerrainFrame(TerrainDefinitions[terrainTypeRemap[edges & 15]].sprite + animation);
                    f_00435084(TerrainScratch, (byte *)layer + layer->offset,
                        hexMask + ((6 - view->rotation + edge) % 6) * 0x200, tile->shade, 0);
                }
            }
            pixels = TerrainScratch;
        }
        ((TerrainCanvas *)g513888)->remap = 0;
        if (location && TerrainVisibility && TerrainVisibility->Unexplored(location))
            ((TerrainCanvas *)g513888)->remap = worldColors;
        MenuImage local = *image; local.offset=(int)pixels-(int)&local;
        int fast = view->doubleSize == 64 && !((TerrainCanvas *)g513888)->remap &&
            ((TerrainCanvas *)g513888)->top <= y && ((TerrainCanvas *)g513888)->left <= x &&
            y + 64 <= ((TerrainCanvas *)g513888)->bottom && x + view->doubleSize <= ((TerrainCanvas *)g513888)->right;
        if (fast) {
            if ((unsigned)def->reserved[5] & 2) f_0047f114(&local, x, y, tile->shade, view->doubleSize + 1, view->doubleSize);
            else f_00435b5c(&local, x, y, tile->shade);
        } else f_0047f114(&local, x, y, tile->shade, view->doubleSize + 1, view->doubleSize);
        if (view->flags & 0x80) {
            if (tile->feature) TerrainDrawOverlay(view, location, tile->feature, tile->featureType, x, y, frame);
            if (tile->overlay) TerrainDrawOverlay(view, location, tile->overlay, tile->overlayType, x, y, frame);
        }
        ((TerrainCanvas *)g513888)->remap = 0;
    }
    else {
        uint color = 0;
        if (hidden) { MenuSetColors(g513888, gMenuColors[8], gMenuColors[0]); flags &= ~0x30; }
        else if (!location) { MenuSetColors(g513888, gMenuColors[0], gMenuColors[0]); flags &= ~0x30; }
        else { color = worldColorInfo[tile->type * 8 + frame]; MenuSetColors(g513888, color, color); }
        if (view->size > 8 || color) f_00435830(x, y);
        if (hidden) {
            MenuLine(g513888, x + view->size, y, x + view->threeHalves, y);
            MenuLine(g513888, x + view->threeHalves, y, x + view->doubleSize, y + view->half);
            MenuLine(g513888, x + view->doubleSize, y + view->half, x + view->threeHalves, y + view->doubleSize);
            MenuLine(g513888, x + view->threeHalves, y + view->doubleSize, x + view->size, y + view->doubleSize);
            MenuLine(g513888, x + view->size, y + view->doubleSize, x, y + view->half);
            MenuLine(g513888, x, y + view->half, x + view->size, y);
        }
    }
    if (flags & 0x30) {
        byte edges = flags & 0x10 ? 255 : MapDetails[location].outline;
        if (edges) {
            unsigned long color = gMenuColors[0];
            if (TerrainOutlineColors[tile->type] > 29) color = TerrainPalette[TerrainOutlineColors[tile->type]];
            MenuSetColors(g513888, color, -1);
            if (edges & 1) MenuLine(g513888, x + view->size, y, x + view->threeHalves, y);
            if (edges & 2) MenuLine(g513888, x + view->threeHalves, y, x + view->doubleSize, y + view->half);
            if (edges & 4) MenuLine(g513888, x + view->doubleSize, y + view->half, x + view->threeHalves, y + view->doubleSize);
            if (edges & 8) MenuLine(g513888, x + view->threeHalves, y + view->doubleSize, x + view->size, y + view->doubleSize);
            if (edges & 16) MenuLine(g513888, x + view->size, y + view->doubleSize, x, y + view->half);
            if (edges & 32) MenuLine(g513888, x, y + view->half, x + view->size, y);
        }
    }
    byte marker = (view->world)->markers[location].sprite;
    if (marker && (view->flags & 8)) DrawSprite(g513888, (ExploFrame *)((char *)hexCursor + 4) + marker, x, y, 0, view->doubleSize, view->doubleSize);
}

int TerrainVisibleDrop(TerrainView *view, DrawPoint *point, int height, int direction)
{
    
    DrawPoint neighbour = pathDirections[(direction + view->rotation) % 6];
    neighbour += *point;
    int location = ResolveName((const char *)view->world, (const char *)&neighbour);
    if (location) {
        uint other = 0;
        if (!TerrainVisibility || !TerrainVisibility->Hidden(location))
            other=(view->world)->cells[location].height;
        if ((int)other < height) return height - other; else return 0;
    }
    return height;
}

void TerrainDrawCliff(TerrainView *view, int location, DrawPoint *point, int level)
{
    int obscured=0; if(location && TerrainVisibility && TerrainVisibility->Unexplored(location)) obscured=1;
    int textured; if(!obscured && (view->flags&4))textured=1;else textured=0;
    int x = point->x - view->half, y = point->y - view->half;
    DrawPoint cell; BestLocSetData((void *)view->world, &cell, (void *)location);
    TerrainCell *tile = &(view->world)->cells[location];
    int sprite = TerrainDefinitions[tile->type].reserved[1];
    int shade = 112 - level * 16;
    int height = MapDetails[location].left * view->heightScale;
    if (height > 0) {
        if (textured) f_00435520(TerrainFrame(sprite), shade, x, view->half + y, view->doubleSize + y + height, 1);
        else {
            uint color = obscured ? gMenuColors[8] : worldColorExtras[tile->type * 3];
            MenuSetColors(g513888, 0, color);
            f_00435520(0, shade, x, view->half + y, view->doubleSize + y + height, 0);
            if (view->flags & 0x200) {
                MenuLine(g513888, x, view->half + y, x, view->half + y + height);
                MenuLine(g513888, x, view->half + y + height, view->size + x, view->doubleSize + y + height);
            }
        }
    }
    height = MapDetails[location].right * view->heightScale;
    if (height > 0) {
        if (textured) f_00435610(TerrainFrame(sprite + 2), shade, view->threeHalves + x, view->half + y, view->doubleSize + y + height, 1);
        else {
            uint color = obscured ? gMenuColors[8] : worldColorExtras[tile->type * 3 + 2];
            MenuSetColors(g513888, 0, color);
            f_00435610(0, shade, view->threeHalves + x, view->half + y, view->doubleSize + y + height, 0);
            if (view->flags & 0x200) {
                MenuLine(g513888, view->doubleSize + x, view->half + y, view->doubleSize + x, view->half + y + height);
                MenuLine(g513888, view->doubleSize + x, view->half + y + height, view->threeHalves + x, view->doubleSize + y + height);
            }
        }
    }
    height = MapDetails[location].front * view->heightScale;
    if (height > 0) {
        if (textured) f_00435700(TerrainFrame(sprite + 1), shade, x + view->size, y + view->doubleSize, height + y + view->doubleSize);
        else {
            uint color = obscured ? gMenuColors[8] : worldColorExtras[tile->type * 3 + 1];
            if (view->flags&0x200) MenuSetColors(g513888,0,color);
            else MenuSetColors(g513888,color,color);
            MenuFill(g513888, x + view->size, y + view->doubleSize, view->half + 1, height + 1);
        }
    }
}






