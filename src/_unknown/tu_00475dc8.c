// Shared resource pools and debug text. ResMgr layout remains opaque here.
#include <string.h>
struct Surface { int width,height; void *pixels; int flags; Surface(unsigned long); ~Surface(); };
struct StrPool { long id; char *buffer; long capacity; void *data; long size; StrPool(long,long,long); ~StrPool(); };
struct ResMgr { char unknown[0x34c]; ResMgr(); ~ResMgr(); };
extern char gDebugText[],sharedPoolBuffer[],smallDebugText[];
Surface g511948(1),gMenuSavedSurface(1);
/* Palette and font/image handles occupy the BSS gap before the string pools.
 * The palette layout follows the existing shell declaration; its final 82 bytes
 * remain opaque. BSS order/size alone is not asserted by TU COMPLETE. */
struct PaletteColor { unsigned char red, green, blue, flags; };
struct Palette { PaletteColor entries[256]; unsigned char unknown[82]; };
struct Font { void *data; };
struct MenuImages;
Palette gShellBackground;
Font shellFont,g511dc0,gDefaultFont,gShellFont,DAT_00511dcc;
MenuImages *gShellImages,*g511dd4;
unsigned char *gScreenImages;
StrPool g511ddc((long)"CoreTxt.bin",(long)sharedPoolBuffer,8192);
StrPool gMenuPrefixStrings((long)"HercTxt.bin",(long)sharedPoolBuffer,8192);
StrPool gHercStrings((long)"TechTxt.bin",(long)sharedPoolBuffer,8192);
StrPool g511e18((long)"ParaTech.bin",(long)sharedPoolBuffer,8192);
StrPool missionImageName((long)"WorldTxt.bin",(long)sharedPoolBuffer,8192);
StrPool promoStrings((long)"PromoTxt.bin",(long)sharedPoolBuffer,8192);
StrPool targetBriefResource((long)"SimTxt.bin",(long)sharedPoolBuffer,8192);
StrPool gShellStrings((long)"ShellTxt.bin",(long)sharedPoolBuffer,8192);
StrPool DAT_00511e7c((long)"ErrorTxt.bin",(long)sharedPoolBuffer,8192);
StrPool quickStrings((long)"QStart.bin",(long)sharedPoolBuffer,8192);
StrPool resourcePool((long)"MultiTxt.bin",(long)sharedPoolBuffer,8192);
StrPool helpLinks((long)"HelpLink.bin",(long)sharedPoolBuffer,8192);
ResMgr gResMgr;
extern char *cdPath;
extern char *resourceCdPath;
/* This initialized pointer is in the later resource-path object. Preserve this
 * TU's startup assignment without incorrectly allocating it in this BSS. */
struct ResourcePathInit { ResourcePathInit() { resourceCdPath=cdPath; } } resourcePathInit;
void DebugTextClear() { memset(gDebugText,0,65536); }
void SmallDebugTextClear() { memset(smallDebugText,0,8192); }
void DebugTextAppend(const char *text) {
 if(text) { unsigned a=strlen(gDebugText); unsigned b=strlen(text); if(a+b<=65535) strcat(gDebugText,text); }
}
