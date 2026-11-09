// Locate a CyberStorm volume, optionally retrying through the CD prompt.
#include <windows.h>
#include <string.h>
extern HWND g_mainWindow;
char cdDrive[]="?:";
extern char cdMissingCaption[],cdInsertPrompt[];
char FindGameCD(int prompt) {
 char volume[64];
 do {
  cdDrive[0]='C'; unsigned long drives=GetLogicalDrives()>>2;
  while(drives) {
   if(drives&1) {
    GetVolumeInformation(cdDrive,volume,63,0,0,0,0,0);
    if(!strcmp("CYBERSTORM1",volume)) return cdDrive[0];
    if(!strcmp("CYBERSTORM2",volume)) return cdDrive[0];
   }
   drives>>=1; cdDrive[0]++;
  }
  if(prompt) {
   int result=MessageBox(g_mainWindow,cdInsertPrompt,cdMissingCaption,0x12035);
   if(result==4) Sleep(5000);
   if(result==2) prompt=0;
  }
 } while(prompt);
 return 0;
}
