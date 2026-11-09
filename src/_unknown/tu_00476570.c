// GDI help/status text. Names are provisional; APIs and layouts are evidenced by calls.
#include <windows.h>
#include <string.h>
extern "C" void RegisterAssertCallback(void (*)());
extern HDC g512538;
extern HWND g_mainWindow;
extern int gViewX,gViewY,g5138ac,g5138b0;
extern char helpTitle[];
extern HFONT helpFont;
extern int helpTextHeight,helpTextMode;
void GdiTextClose();
int GdiTextInit() {
 if(helpTextMode&2) helpFont=CreateFont(17,8,0,0,0,0,0,0,0,0,0,0,0,"Modaerne");
 RegisterAssertCallback(GdiTextClose);
 return helpFont!=0;
}
int GdiTextReady() { return helpFont!=0; }
void GdiTextClose() { if(helpFont) DeleteObject(helpFont); helpFont=0; }
void GdiTextSetMode(int mode) { helpTextMode=mode; }
int f_4765e6() {
 HDC dc=g512538;
 if(!(helpTextMode&2) || !helpFont || !dc) return 0;
 char text[300]; SIZE extent;
 for(unsigned i=0;i<300;i++) text[i]=i+1;
 SetTextColor(dc,0xffff); SetBkColor(dc,0); SelectObject(dc,GetStockObject(4)); SetTextAlign(dc,2);
 SelectObject(dc,helpFont); helpTextHeight=0;
 if(GetTextExtentPoint32(dc,text,256,&extent)) helpTextHeight=extent.cy+2;
 return helpTextHeight;
}
void HelpTextDraw(const char *message);
void HelpTextDisplay(const char *message) {
 char title[256];
 if(helpTextMode&1) {
  if(message) { strcpy(title,"CyberStorm          "); strcat(title,message); SetWindowText(g_mainWindow,title); }
  else SetWindowText(g_mainWindow,helpTitle);
 }
 if(helpTextMode&2) HelpTextDraw(message);
}
inline int HelpTextWidth() { return g5138ac; }
inline int HelpTextBottom() { return g5138b0+gViewY+2; }
inline unsigned HelpTextLength(const char *s) { return strlen(s); }
void HelpTextDraw(const char *message) {
 HDC dc=g512538; int x=gViewX,y=HelpTextBottom();
 Rectangle(dc,x,y,2000,2000);
 if(message) { x+=HelpTextWidth(); TextOut(dc,x,y,message,HelpTextLength(message)); }
}
void HelpTextWindowTitle(const char *message) {
 char title[256];
 if(message) { strcpy(title,"CyberStorm          "); strcat(title,message); SetWindowText(g_mainWindow,title); }
 else SetWindowText(g_mainWindow,helpTitle);
}

