// Shared resource pools and debug text. ResMgr layout remains opaque here.
#include <string.h>
struct Surface { void *pixels; int width,height,flags; Surface(unsigned long); ~Surface(); };
struct StrPool { long id; char *buffer; long capacity; void *data; long size; StrPool(long,long,long); ~StrPool(); };
struct ResMgr { char unknown[0x34c]; ResMgr(); ~ResMgr(); };
extern char gDebugText[],sharedPoolBuffer[],smallDebugText[];
Surface g511948(1),gMenuSavedSurface(1);
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
char *resourceCdPath=cdPath;
void DebugTextClear() { memset(gDebugText,0,65536); }
void SmallDebugTextClear() { memset(smallDebugText,0,8192); }
void DebugTextAppend(const char *text) {
 if(text) { unsigned a=strlen(gDebugText); unsigned b=strlen(text); if(a+b<=65535) strcat(gDebugText,text); }
}
