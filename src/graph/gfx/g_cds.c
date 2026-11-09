#include <windows.h>
extern "C" void AssertFail(const char *, int, const char *);
extern "C" void *memcpy(void *, const void *, unsigned int);
#pragma function(memcpy)
struct Palette {
    unsigned short version, count;
    PALETTEENTRY entries[256];
    Palette(); ~Palette(); void ToRgb(RGBQUAD *);
};
typedef BOOL (WINAPI *WinGBitBltProc)(HDC,int,int,int,int,HDC,int,int);
typedef HBITMAP (WINAPI *WinGCreateBitmapProc)(HDC,BITMAPINFO *,void **);
typedef HDC (WINAPI *WinGCreateDCProc)();
typedef UINT (WINAPI *WinGColorTableProc)(HDC,UINT,UINT,const RGBQUAD *);
extern WinGCreateDCProc wingCreateDC;
extern WinGCreateBitmapProc wingCreateBitmap;
extern WinGBitBltProc wingBitBlt;
extern WinGColorTableProc wingSetDIBColorTable;
struct GFXCDSSurface {
    HBITMAP bitmap;
    void *pixels;
    HDC windowDC, memoryDC;
    HPALETTE currentPalette, oldPalette;
    HGDIOBJ oldBitmap;
    HWND window;
    Palette palette;
    GFXCDSSurface(); ~GFXCDSSurface();
    void SetWindow(HWND);
    static GFXCDSSurface *create(int,int,HWND);
    void Blit(int,int,int,int,int,int);
    void setPalette(const PALETTEENTRY *,int,int);
};
GFXCDSSurface::GFXCDSSurface()
{
    window = 0; windowDC = 0; memoryDC = 0; oldBitmap = 0;
    currentPalette = 0; oldPalette = 0; pixels = 0; bitmap = 0;
}
GFXCDSSurface::~GFXCDSSurface()
{
    if (bitmap) SelectObject(memoryDC, oldBitmap);
    if (currentPalette) {
        if (oldPalette) SelectPalette(windowDC, oldPalette, 0);
        DeleteObject(currentPalette);
    }
    DeleteDC(memoryDC);
    ReleaseDC(window, windowDC);
    if (bitmap) DeleteObject(bitmap);
}
void GFXCDSSurface::SetWindow(HWND hwnd)
{
    if (memoryDC) {
        if (bitmap) SelectObject(memoryDC, oldBitmap);
        DeleteDC(memoryDC); memoryDC = 0;
    }
    if (windowDC) { ReleaseDC(window,windowDC); windowDC = 0; }
    if (hwnd) {
        if (currentPalette) SelectPalette(windowDC, oldPalette, 0);
        windowDC = GetDC(hwnd); window = hwnd;
        if (currentPalette) oldPalette = SelectPalette(windowDC, currentPalette, 0);
        memoryDC = wingCreateDC();
        if (bitmap) oldBitmap = SelectObject(memoryDC, bitmap);
    }
}
GFXCDSSurface *GFXCDSSurface::create(int width, int height, HWND hwnd)
{
    struct { BITMAPINFO info; RGBQUAD extra[256]; } local;
    Palette palette;
    BITMAPINFO *info = &local.info;
    GFXCDSSurface *result = new GFXCDSSurface;
    GFXCDSSurface *surface = result;
    HDC dc;
    if (result && (dc = GetDC(hwnd)) != 0) {
        info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        info->bmiHeader.biPlanes = 1;
        info->bmiHeader.biBitCount = 8;
        info->bmiHeader.biSizeImage = 0;
        info->bmiHeader.biWidth = width;
        info->bmiHeader.biHeight = height;
        info->bmiHeader.biXPelsPerMeter = 0;
        info->bmiHeader.biYPelsPerMeter = 0;
        info->bmiHeader.biClrUsed = 0;
        info->bmiHeader.biClrImportant = 0;
        info->bmiHeader.biCompression = 0;
        if (GetSystemPaletteEntries(dc, 0, 256, palette.entries)) {
            palette.ToRgb(info->bmiColors);
            result->bitmap = wingCreateBitmap(dc,info,&result->pixels);
            if (result->bitmap) {
                ReleaseDC(hwnd, dc);
                result->SetWindow(hwnd);
                return result;
            }
        }
    }
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
    AssertFail(__FILE__, __LINE__, "GFXCDSSurface::create: Failed to create a surface.");
    if (surface) delete surface;
    return 0;
}
void GFXCDSSurface::Blit(int x,int y,int width,int height,int sx,int sy)
{
    wingBitBlt(windowDC,x,y,width,height,memoryDC,sx,sy);
}
void GFXCDSSurface::setPalette(const PALETTEENTRY *source, int start, int count)
{
    RGBQUAD rgb[256];
    if (!source) {
        if (currentPalette) SelectPalette(windowDC, currentPalette, 0);
        return;
    }
    if (start < 0 || start >= 256) {
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
// Original source-line spacing.
        AssertFail(__FILE__, __LINE__, "GFXCDSSurface::setPalette: start out of range.");
    }
    if (count <= 0 || count > 256) { AssertFail(__FILE__, __LINE__, "GFXCDSSurface::setPalette: count out of range.");
    }
    if (start + count > 256) { AssertFail(__FILE__, __LINE__, "GFXCDSSurface::setPalette: start+count out of range.");
    }
    memcpy(palette.entries + start, source + start, count * sizeof(PALETTEENTRY));
    HDC dc = GetDC(0);
    GetSystemPaletteEntries(dc,0,10,palette.entries);
    GetSystemPaletteEntries(dc,246,10,palette.entries+246);
    ReleaseDC(0,dc);
    { for (int j=0; j<10; ++j) {
        palette.entries[j].peFlags = 0;
        palette.entries[j+246].peFlags = 0;
    } }
    unsigned char flags;
    int j;
    for (int i=10; i<246; ++i) {
        flags = 4;
        if (i < 16) flags = 1;
        if (flags != 1) {
            for (j=0; j<10; ++j) {
                if (palette.entries[i].peRed == palette.entries[j].peRed &&
                    palette.entries[i].peGreen == palette.entries[j].peGreen &&
                    palette.entries[i].peBlue == palette.entries[j].peBlue) flags = 1;
            }
        }
        if (flags != 1) {
            for (j=246; j<256; ++j) {
                if (palette.entries[i].peRed == palette.entries[j].peRed &&
                    palette.entries[i].peGreen == palette.entries[j].peGreen &&
                    palette.entries[i].peBlue == palette.entries[j].peBlue) flags = 1;
            }
        }
        palette.entries[i].peFlags = flags;
    }

    palette.ToRgb(rgb);
    SelectObject(memoryDC,bitmap);
    wingSetDIBColorTable(memoryDC,start,count,rgb+start);
    HPALETTE created = CreatePalette((LOGPALETTE *)&palette);
    { HPALETTE previous = SelectPalette(windowDC,created,0);
    if (previous == currentPalette) DeleteObject(currentPalette);
    else oldPalette = previous; }
    currentPalette = created;
    RealizePalette(windowDC);
}

