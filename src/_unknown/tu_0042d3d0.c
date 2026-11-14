// Working labels for terrain drawing. The object starts with its switch table at 42d3d0.
typedef unsigned int uint;
typedef unsigned char byte;
struct TerrainCell { byte type,height,rest[10]; }; struct TerrainWorld { byte header[68]; TerrainCell *cells; };

struct Point { int x, y; Point() {} Point(int a, int b) { x=a; y=b; } Point(const Point &p) { x=p.x; y=p.y; } Point &operator+=(const Point &p) { x+=p.x; y+=p.y; return *this; } };
typedef Point DrawPoint;
struct MenuImage { int offset, width, height; };
struct ExploFrame { int offset, width, height; };
#pragma pack(push,1)
struct TileSprite { byte first, kind, reserved[3], stride; unsigned short animated; };
struct TerrainDefinition { byte sprite, reserved[38]; };
struct TerrainAnimation { byte first, pad[7], alternate, pad2[3], stride; };
#pragma pack(pop)
struct FeatureType { byte base, type, unknown, defense, cover, stride; unsigned short animated; }; extern FeatureType featureTypes[];
extern TerrainDefinition TerrainDefinitions[];
extern TerrainAnimation TerrainFrames;
extern byte TerrainOutlineColors[];
extern byte *MapDetails;
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

void TerrainDrawOverlay(int *view, uint location, int sprite, int type, int x, int y, int frame)
{
    int selected = -1;
    if (type <= 64) {
        FeatureType *def = &featureTypes[type - 1];
        switch (def->type) {
        case 0: case 2: {
            int image = sprite;
            if (frame && def->animated) image = def->animated - def->base + def->stride * (frame - 1) + sprite;
            if ((char)((int)(location & 2) >> 1)) {
                int half = view[26] / 2;
                x += half;
                if (def->type == 2) DrawSprite(g513888, (ExploFrame *)TerrainFrame(image), x, y + view[26], 0, view[28], view[28]);
                else DrawSprite(g513888, (ExploFrame *)TerrainFrame(image), x, y + half, 0, view[28], view[28]);
            } else selected = image;
            break;
        }
        case 3: case 4:
            selected = sprite;
            if (frame && def->animated) selected = def->animated - def->base + def->stride * (frame - 1) + sprite;
            break;
        case 1: {
            int relative = sprite - def->base;
            selected = def->base + ((6 + relative / 3 - view[20]) % 6) * 3 + relative % 3;
            if (frame && def->animated) selected += def->animated - def->base + def->stride * (frame - 1);
            break;
        }
        }
    } else if (type == 66) {
        selected = sprite;
        if (view[34]) selected += 6;
    }
    if (selected >= 0) {
        MenuImage *image = MenuImageData(g513888, TerrainFrame(selected));
        f_00435c5e(image, x, y, 0, view[29], view[29]);
    }
}

void TerrainDrawTile(int *view, uint location, DrawPoint *point, uint orientation)
{
    int x = point->x - view[27], y = point->y - view[27];
    if (y + view[29] < *(int *)(g513888 + 0x78)) return;
    uint frame = orientation & 7;
    int hidden = 0;
    if (location && TerrainVisibility) hidden = !((char **)((char *)TerrainVisibility + 12))[0][location];
    uint flags = view[33];
    byte *tile = (byte *)((int *)view[0])[17] + location * 12;
    if (!location || !(flags & 2) || hidden) {
        uint color = 0;
        if (hidden) { MenuSetColors(g513888, gMenuColors[8], gMenuColors[0]); flags &= ~0x30; }
        else if (!location) { MenuSetColors(g513888, gMenuColors[0], gMenuColors[0]); flags &= ~0x30; }
        else { color = worldColorInfo[tile[0] * 8 + frame]; MenuSetColors(g513888, color, color); }
        if (view[26] > 8 || color) f_00435830(x, y);
        if (hidden) {
            MenuLine(g513888, x + view[26], y, x + view[28], y);
            MenuLine(g513888, x + view[28], y, x + view[29], y + view[27]);
            MenuLine(g513888, x + view[29], y + view[27], x + view[28], y + view[29]);
            MenuLine(g513888, x + view[28], y + view[29], x + view[26], y + view[29]);
            MenuLine(g513888, x + view[26], y + view[29], x, y + view[27]);
            MenuLine(g513888, x, y + view[27], x + view[26], y);
        }
    } else {
        TerrainDefinition *def = TerrainDefinitions + tile[0];
        int animation = 0;
        if (frame && TerrainFrames.alternate) animation = TerrainFrames.alternate - TerrainFrames.first + (frame - 1) * TerrainFrames.stride;
        MenuImage *image = TerrainFrame(tile[7] + def->sprite + animation);
        byte *pixels = (byte *)image + image->offset;
        if (*(uint *)(tile + 8) && (view[33] & 0x40)) {
            memcpy(TerrainScratch, pixels, 0x1000);
            uint edges = *(uint *)(tile + 8);
            for (int edge = 0; edge < 6 && edges; ++edge, edges >>= 4) {
                if (edges & 15) {
                    MenuImage *layer = TerrainFrame(TerrainDefinitions[terrainTypeRemap[edges & 15]].sprite + animation);
                    f_00435084(TerrainScratch, (byte *)layer + layer->offset,
                        hexMask + ((6 - view[20] + edge) % 6) * 0x200, tile[6], 0);
                }
            }
            pixels = TerrainScratch;
        }
        *(void **)(g513888 + 0x60) = 0;
        if (location && TerrainVisibility && !((char **)((char *)TerrainVisibility + 16))[0][location])
            *(void **)(g513888 + 0x60) = worldColors;
        MenuImage local; int width=image->width,height=image->height; local.offset=(int)pixels-(int)&local; local.width=width; local.height=height;
        int fast = view[29] == 64 && !*(void **)(g513888 + 0x60) &&
            *(int *)(g513888 + 0x78) <= y && *(int *)(g513888 + 0x74) <= x &&
            y + 64 <= *(int *)(g513888 + 0x80) && x + view[29] <= *(int *)(g513888 + 0x7c);
        if (fast && !(def->reserved[5] & 2)) f_00435b5c(&local, x, y, tile[6]);
        else f_0047f114(&local, x, y, tile[6], view[29] + 1, view[29]);
        if (view[33] & 0x80) {
            if (tile[2]) TerrainDrawOverlay(view, location, tile[2], tile[3], x, y, frame);
            if (tile[4]) TerrainDrawOverlay(view, location, tile[4], tile[5], x, y, frame);
        }
        *(void **)(g513888 + 0x60) = 0;
    }
    if (flags & 0x30) {
        byte edges = flags & 0x10 ? 255 : MapDetails[location * 8 + 6];
        if (edges) {
            unsigned long color = gMenuColors[0];
            if (TerrainOutlineColors[tile[0]] > 29) color = TerrainPalette[TerrainOutlineColors[tile[0]]];
            MenuSetColors(g513888, color, -1);
            if (edges & 1) MenuLine(g513888, x + view[26], y, x + view[28], y);
            if (edges & 2) MenuLine(g513888, x + view[28], y, x + view[29], y + view[27]);
            if (edges & 4) MenuLine(g513888, x + view[29], y + view[27], x + view[28], y + view[29]);
            if (edges & 8) MenuLine(g513888, x + view[28], y + view[29], x + view[26], y + view[29]);
            if (edges & 16) MenuLine(g513888, x + view[26], y + view[29], x, y + view[27]);
            if (edges & 32) MenuLine(g513888, x, y + view[27], x + view[26], y);
        }
    }
    byte marker = ((byte *)((int *)view[0])[18])[location * 8 + 4];
    if (marker && (view[33] & 8)) DrawSprite(g513888, (ExploFrame *)((char *)hexCursor + 4) + marker, x, y, 0, view[29], view[29]);
}

int TerrainVisibleDrop(int *view, DrawPoint *point, int height, int direction)
{
    
    DrawPoint neighbour = pathDirections[(direction + view[20]) % 6];
    neighbour += *point;
    int location = ResolveName((const char *)view[0], (const char *)&neighbour);
    if (location) {
        uint other = 0;
        if (!TerrainVisibility || !TerrainVisibility->Hidden(location))
            other=((TerrainWorld *)view[0])->cells[location].height;
        if ((int)other < height) return height - other; else return 0;
    }
    return height;
}

void TerrainDrawCliff(int *view, int location, DrawPoint *point, int level)
{
    int obscured=0; if(location && TerrainVisibility && TerrainVisibility->Unexplored(location)) obscured=1;
    int textured; if(!obscured && (view[33]&4))textured=1;else textured=0;
    int x = point->x - view[27], y = point->y - view[27];
    DrawPoint cell; BestLocSetData((void *)view[0], &cell, (void *)location);
    byte *tile = (byte *)((int *)view[0])[17] + location * 12;
    int sprite = TerrainDefinitions[tile[0]].reserved[1];
    int shade = 112 - level * 16;
    int height = ((signed char *)MapDetails)[location * 8] * view[30];
    if (height > 0) {
        if (!textured) {
            uint color = obscured ? gMenuColors[8] : worldColorExtras[tile[0] * 3];
            MenuSetColors(g513888, 0, color);
            f_00435520(0, shade, x, view[27] + y, view[29] + y + height, 0);
            if (view[33] & 0x200) {
                MenuLine(g513888, x, view[27] + y, x, view[27] + y + height);
                MenuLine(g513888, x, view[27] + y + height, view[26] + x, view[29] + y + height);
            }
        } else f_00435520(TerrainFrame(sprite), shade, x, view[27] + y, view[29] + y + height, 1);
    }
    height = ((signed char *)MapDetails)[location * 8 + 2] * view[30];
    if (height > 0) {
        if (!textured) {
            uint color = obscured ? gMenuColors[8] : worldColorExtras[tile[0] * 3 + 2];
            MenuSetColors(g513888, 0, color);
            f_00435610(0, shade, view[28] + x, view[27] + y, view[29] + y + height, 0);
            if (view[33] & 0x200) {
                MenuLine(g513888, view[29] + x, view[27] + y, view[29] + x, view[27] + y + height);
                MenuLine(g513888, view[29] + x, view[27] + y + height, view[28] + x, view[29] + y + height);
            }
        } else f_00435610(TerrainFrame(sprite + 2), shade, view[28] + x, view[27] + y, view[29] + y + height, 1);
    }
    height = ((signed char *)MapDetails)[location * 8 + 1] * view[30];
    if (height > 0) {
        if (!textured) {
            uint color = obscured ? gMenuColors[8] : worldColorExtras[tile[0] * 3 + 1];
            uint firstColor=view[33]&0x200 ? 0 : color; MenuSetColors(g513888,firstColor,color);
            MenuFill(g513888, x + view[26], y + view[29], view[27] + 1, height + 1);
        } else f_00435700(TerrainFrame(sprite + 1), shade, x + view[26], y + view[29], height + y + view[29]);
    }
}






