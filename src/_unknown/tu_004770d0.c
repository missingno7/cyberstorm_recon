// Ordinal exports of the UI DLL and wrappers. Export signatures inferred from stack traffic.
#include <windows.h>
extern HMODULE g512580;
extern HWND g_mainWindow;
extern int g4faf44,g4faf40,networkEnabled;
extern unsigned long dialogInputUntil;
struct MenuNetwork { static int Active(); };
void ActivateMainWindow(); unsigned long TimeNow(); void TimeSleep(unsigned long);
extern "C" void LogicCallMode(int,int);
void HelpTextDisplay(const char*);
extern int (*dialogMessage)(HWND,const char*,int,int);
extern int (*ShowStatus)(HWND,const char*,int,int);
extern int (*dialogOpen)(HWND,int,int,int,int,int,int,int,int,int,char*);
extern int (*dialogChoose)(HWND,char*,char*,unsigned long,int);
extern int (*dialogEdit)(HWND,int,int,int);
extern int (*dialogInstall)(HWND,int,char*,char*,char*,const char*,const char*,const char*);
extern int (*dialogExport5)();
extern int (*dialogSelect)(HWND,int,int,int);
extern int (*dialogExport9)();
extern int (*gOptionsDialog)();
extern char dialogOpenData[];
void LoadDialogExports() {
 dialogOpen=(int (*)(HWND,int,int,int,int,int,int,int,int,int,char*))GetProcAddress(g512580,(const char*)1);
 dialogChoose=(int (*)(HWND,char*,char*,unsigned long,int))GetProcAddress(g512580,(const char*)2);
 dialogEdit=(int (*)(HWND,int,int,int))GetProcAddress(g512580,(const char*)3);
 dialogInstall=(int (*)(HWND,int,char*,char*,char*,const char*,const char*,const char*))GetProcAddress(g512580,(const char*)4);
 dialogExport5=(int (*)())GetProcAddress(g512580,(const char*)5);
 dialogSelect=(int (*)(HWND,int,int,int))GetProcAddress(g512580,(const char*)6);
 ShowStatus=(int (*)(HWND,const char*,int,int))GetProcAddress(g512580,(const char*)7);
 dialogMessage=(int (*)(HWND,const char*,int,int))GetProcAddress(g512580,(const char*)8);
 dialogExport9=(int (*)())GetProcAddress(g512580,(const char*)9);
 gOptionsDialog=(int (*)())GetProcAddress(g512580,(const char*)10);
}
inline int ReadNetworkEnabled() { return networkEnabled; }
inline void DialogEnd() { LogicCallMode(415,0); dialogInputUntil=TimeNow()+50; }
int f_0047723b(const char *text);
void DialogTimedMessage(const char *text);
int DialogConfirmPaused(const char *text) {
 int saved=g4faf44; g4faf44=0;
 if(MenuNetwork::Active() && g4faf40) TimeSleep(400);
 int result=f_0047723b(text); g4faf44=saved; return result;
}
int f_0047723b(const char *text) {
 int delay=ReadNetworkEnabled()?10000:0; ActivateMainWindow();
 int result=(ShowStatus(g_mainWindow,text,4,delay)==6); DialogEnd(); return result;
}
void f_00477295(const char *text) {
 if(ReadNetworkEnabled()) DialogTimedMessage(text);
 else { ActivateMainWindow(); ShowStatus(g_mainWindow,text,0,0); DialogEnd(); }
}
void DialogTimedMessage(const char *text) {
 int delay=ReadNetworkEnabled()?14000:0; ActivateMainWindow(); dialogMessage(g_mainWindow,text,0,delay); DialogEnd();
}
int f_477352(int a,char *b,char *c,char *d) {
 ActivateMainWindow(); int result=dialogInstall(g_mainWindow,a,b,c,d,"C:\\SIERRA\\CSTORM\\CSTORM.EXE","C:\\SIERRA\\CSTORM","C:\\");
 dialogInputUntil=TimeNow()+50; return result;
}
extern "C" void f_004773a1(int a,int b,int c) { ActivateMainWindow(); dialogEdit(g_mainWindow,a,b,c); dialogInputUntil=TimeNow()+50; }
int f_004773cc(char *a,char *b,unsigned long c) {
 ActivateMainWindow(); int result=(dialogChoose(g_mainWindow,a,b,c,1)==1); DialogEnd(); return result;
}
int DialogOpen(int a,int b,int c,int d,int e,int f,int g,int h,int i) {
 ActivateMainWindow(); return dialogOpen(g_mainWindow,a,b,c,d,e,f,g,h,i,dialogOpenData);
}
int LinkSelectDialog(int a,int b,int c) { ActivateMainWindow(); return dialogSelect(g_mainWindow,a,b,c); }
void HelpTextTimed(const char *text,unsigned long duration) { HelpTextDisplay(text); TimeSleep(duration); }
