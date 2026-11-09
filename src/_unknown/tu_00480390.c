#include <windows.h>
typedef BOOL (WINAPI *WinGBitBltProc)(HDC,int,int,int,int,HDC,int,int);
typedef HBITMAP (WINAPI *WinGCreateBitmapProc)(HDC,BITMAPINFO *,void **);
typedef BOOL (WINAPI *WinGRecommendDIBFormatProc)(BITMAPINFO *);
typedef HDC (WINAPI *WinGCreateDCProc)();
typedef UINT (WINAPI *WinGColorTableProc)(HDC,UINT,UINT,const RGBQUAD *);
typedef BOOL (WINAPI *WinGStretchBltProc)(HDC,int,int,int,int,HDC,int,int,int,int);
volatile int wingInitialized;
WinGCreateDCProc wingCreateDC;
WinGCreateBitmapProc wingCreateBitmap;
WinGRecommendDIBFormatProc wingRecommendDIBFormat;
WinGColorTableProc wingSetDIBColorTable, wingGetDIBColorTable;
WinGBitBltProc wingBitBlt;
WinGStretchBltProc wingStretchBlt;
extern "C" int IsWin32s()
{
    DWORD version = GetVersion();
    return (int)(version & 255) < 4 && (version & 0x80000000) != 0;
}
extern "C" int LoadWinG()
{
    int success = 0;
    HMODULE module = LoadLibraryA("WING32.DLL");
    if (module) {
        success = (wingBitBlt = (WinGBitBltProc)GetProcAddress(module, "WinGBitBlt")) != 0;
        success = success && (wingCreateBitmap = (WinGCreateBitmapProc)GetProcAddress(module, "WinGCreateBitmap")) != 0;
        success = success && (wingRecommendDIBFormat = (WinGRecommendDIBFormatProc)GetProcAddress(module, "WinGRecommendDIBFormat")) != 0;
        success = success && (wingCreateDC = (WinGCreateDCProc)GetProcAddress(module, "WinGCreateDC")) != 0;
        success = success && (wingSetDIBColorTable = (WinGColorTableProc)GetProcAddress(module, "WinGSetDIBColorTable")) != 0;
        success = success && (wingGetDIBColorTable = (WinGColorTableProc)GetProcAddress(module, "WinGGetDIBColorTable")) != 0;
        success = success && (wingStretchBlt = (WinGStretchBltProc)GetProcAddress(module, "WinGStretchBlt")) != 0;
    }
    return success;
}
extern "C" BOOL WINAPI GdiRecommendDIBFormat(BITMAPINFO *info)
{
    info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info->bmiHeader.biPlanes = 1;
    info->bmiHeader.biCompression = BI_RGB;
    info->bmiHeader.biSizeImage = 0;
    info->bmiHeader.biClrUsed = 0;
    info->bmiHeader.biHeight = -1;
    return TRUE;
}
extern "C" HDC WINAPI GdiCreateDC() { return CreateCompatibleDC(0); }
extern "C" HBITMAP WINAPI GdiCreateBitmap(HDC dc, BITMAPINFO *info, void **bits) { return CreateDIBSection(dc, info, DIB_RGB_COLORS, bits, 0, 0); }
extern "C" UINT WINAPI GdiSetDIBColorTable(HDC dc, UINT first, UINT count, const RGBQUAD *table) { return SetDIBColorTable(dc, first, count, table); }
extern "C" UINT WINAPI GdiGetDIBColorTable(HDC dc, UINT first, UINT count, const RGBQUAD *table) { return GetDIBColorTable(dc, first, count, (RGBQUAD *)table); }
extern "C" BOOL WINAPI GdiBitBlt(HDC dc,int x,int y,int w,int h,HDC source,int sx,int sy) { return BitBlt(dc,x,y,w,h,source,sx,sy,SRCCOPY); }
extern "C" BOOL WINAPI GdiStretchBlt(HDC dc,int x,int y,int w,int h,HDC source,int sx,int sy,int sw,int sh) { return StretchBlt(dc,x,y,w,h,source,sx,sy,sw,sh,SRCCOPY); }
extern "C" int UseGdiFallback()
{
    wingBitBlt = GdiBitBlt;
    wingCreateBitmap = GdiCreateBitmap;
    wingRecommendDIBFormat = GdiRecommendDIBFormat;
    wingCreateDC = GdiCreateDC;
    wingSetDIBColorTable = GdiSetDIBColorTable;
    wingGetDIBColorTable = GdiGetDIBColorTable;
    wingStretchBlt = GdiStretchBlt;
    return 1;
}
extern "C" int InitializeWinG()
{
    if (!wingInitialized) {
        if (IsWin32s()) {
            if (!LoadWinG()) return 0;
            wingInitialized = 1;
            return wingInitialized;
        }
        wingInitialized = UseGdiFallback();
    }
    return wingInitialized;
}

