// Help topics and the external manual viewer. Unknown original path.
#include <windows.h>
#include <string.h>
#include <stdio.h>
struct StrPool { long id; char *buffer; long capacity; void *data; long size; StrPool(long,long,long); ~StrPool(); int Lock(); void Unlock(); char *GetString(long,int); };
extern char gDebugText[];
extern char helpDefaultTitle[];
StrPool gShellMiniStrings((long)"miniSh.bin",(long)gDebugText,65536);
StrPool gShellParaStrings((long)"paraSh.bin",(long)gDebugText,65536);
StrPool gMiniMenuStrings((long)"miniMenu.bin",(long)gDebugText,65536);
extern int helpActive,lastHelpTopic,helpDirty,helpTopic,manualTopic;
extern char helpTitle[],helpCustomText[];
extern StrPool helpLinks;
extern char *searchPaths[];
int FileExists(const char*); void Problem(unsigned long,int);
extern "C" void RegisterAssertCallback(void (*)());
void HelpTextDisplay(const char*); void f_00477295(const char*);
void HelpCloseViewer() { HWND viewer=FindWindow("MS_VIEWER",0); if(viewer) PostMessage(viewer,0x112,0xf060,0); }
void HelpShutdown(); void HelpUpdate();
void HelpInit() {
 if(!helpActive) {
  helpActive=1; gShellMiniStrings.Lock(); manualTopic=2; helpTopic=0;
  strcpy(helpTitle,helpDefaultTitle); helpDirty=1; HelpUpdate(); RegisterAssertCallback(HelpShutdown);
 }
}
void HelpShutdown() { HelpCloseViewer(); helpActive=0; }
int HelpGetTopic() { return helpTopic; }
void MenuHelpSetTopic(int topic) { helpTopic=topic; }
void HelpSetText(const char *text) {
 if(helpTopic<0 && !strcmp(helpCustomText,text)) return;
 strncpy(helpCustomText,text,128); helpTopic=-1; helpDirty=1;
}
void HelpClearTopic() { helpTopic=0; helpDirty=1; HelpUpdate(); }
void HelpSetDirty(int dirty) { helpDirty=dirty; }
void HelpUpdate() {
 if(helpActive && (lastHelpTopic!=helpTopic || helpDirty)) {
  lastHelpTopic=helpTopic; helpDirty=0; char *text;
  if(helpTopic) {
   if(helpTopic<0) text=helpCustomText;
   else { text=gShellMiniStrings.GetString(helpTopic,1); if(!text) text=gMiniMenuStrings.GetString(helpTopic,1); }
   HelpTextDisplay(text);
  } else HelpTextDisplay(0);
 }
}
void HelpSetTitle(const char *text) {
 if(text) sprintf(helpTitle,"%s%s",helpDefaultTitle,text);
 else strcpy(helpTitle,helpDefaultTitle);
 helpDirty=1;
}
void f_00476412(int topic);
void HelpOpenDefault() { f_00476412(2); }
void HelpIdle() {}
void f_00476412(int topic) {
 char command[512]; if(topic<0) topic=manualTopic;
 char *entry=helpLinks.GetString(topic,1);
 if(entry && *entry) {
  HelpCloseViewer(); int found=0;
  for(int i=0;!found && searchPaths[i];i++) {
   sprintf(command,"%sMANUAL\\MVIEWER2.EXE",searchPaths[i]);
   if(FileExists(command)) {
    sprintf(command,"%sMANUAL\\MVIEWER2 -i %s METALSTO.MVB",searchPaths[i],entry+1);
    if((int)WinExec(command,1)>31) found=1;
   }
  }
  if(!found) Problem(304,0);
 }
}
void OptionsDlgProc(int topic) { f_00476412(topic+62); }
extern "C" int BriefWaitB(int topic) { int old=manualTopic; manualTopic=topic; return old; }
int HelpManualTopic() { return manualTopic; }
void MenuHelpOpenTopic(long topic) {
 if(topic) { gShellParaStrings.Lock(); char *text=gShellParaStrings.GetString(topic,1); if(text) f_00477295(text); gShellParaStrings.Unlock(); }
}
