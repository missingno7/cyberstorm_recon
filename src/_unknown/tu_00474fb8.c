// MCI AVI playback and its window helpers. Names are provisional.
#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
extern HWND g_mainWindow,movieWindow;
extern HDC g512538;
extern HPALETTE moviePalette;
// moviePlaying 004fb924; movieSkipped 004fb928; movieWindow 004fb938.
extern int moviePlaying,movieSkipped,gViewX,gViewY;
extern char *searchPaths[];
int FileExists(const char*); void f_46b9bd(); int SoundInit();
extern "C" void ResetFrameClock();
// ReadFrameInput is the working label for 0047de30.
int ReadFrameInput(); void GuiPumpMessages(); void HelpClearTopic(); void SimSetCursor(int,int); void CursorUpdate();
extern "C" int BriefWaitA(int);
struct Palette; struct GuiRect;
extern Palette gGuiPalette; extern GuiRect gMenuClip;
void ApplyPaletteData(void*,int=0,int=256); void GuiResetClip(void*);
void MovieCapture() {
 SetCapture(g_mainWindow); HMENU menu=GetSystemMenu(g_mainWindow,0);
 if(menu) { EnableMenuItem(menu,0xf030,1); EnableMenuItem(menu,0xf020,1); EnableMenuItem(menu,0xf020,1); EnableMenuItem(menu,0xf060,1); }
}
void MovieRelease() { GetSystemMenu(g_mainWindow,1); ReleaseCapture(); }
void MovieNotify(int state) { if(state==1) moviePlaying=0; }
void MovieCreatePalette() {
 LOGPALETTE *p=(LOGPALETTE*)LocalAlloc(0x40,0x3b8); p->palVersion=0x300; p->palNumEntries=236;
 for(int i=0;i<236;i++) { p->palPalEntry[i].peRed=i+10; p->palPalEntry[i].peGreen=0; p->palPalEntry[i].peBlue=0; p->palPalEntry[i].peFlags=4; }
 moviePalette=CreatePalette(p); LocalFree(p);
}
int MovieInit(int palette) { if(palette) MovieCreatePalette(); return mciSendString("open avivideo",0,0,0)==0; }
void MovieClose() {
 char error[128]; MCIERROR status=mciSendString("close mov",0,0,0);
 if(status) { mciGetErrorString(status,error,128); MessageBox(0,error,"close avi ",0x30); }
}
void MovieTerminate() {
 char error[128]; MCIERROR status=mciSendString("close avivideo",0,0,0);
 if(status) { mciGetErrorString(status,error,128); MessageBox(0,error,"term avi ",0x30); }
}
void MovieResize(HWND window,RECT *rect) {
 RECT client; GetClientRect(window,&client); int x,y,width,height;
 if(rect) { x=rect->left; y=rect->top; width=rect->right; height=rect->bottom; }
 else { y=0; x=0; width=client.right; height=client.bottom; }
 MoveWindow(window,x,y,width,height,1);
}
int MovieIsPlaying() { return moviePlaying; }
inline void MovieSetRect(RECT *r,int x,int y,int width,int height) { r->left=x; r->top=y; r->right=width; r->bottom=height; }
void MoviePlace() { RECT rect; MovieSetRect(&rect,gViewX,gViewY,640,480); MovieResize(movieWindow,&rect); }
void ShellPlayMovie(const char *name) {
 char command[256],path[128]; HWND owner=g_mainWindow; HDC dc=g512538;
 SetBkColor(dc,0); Rectangle(dc,0,0,2000,2000); moviePlaying=0;
 if(!MovieInit(0)) return;
 int found=0;
 for(int i=0;!found && searchPaths[i];i++) { sprintf(path,"%sAVI\\%s.AVI",searchPaths[i],name); if(FileExists(path)) found=1; }
 if(!found) return;
 f_46b9bd(); sprintf(command,"open %s alias mov style child parent %d",path,owner);
 MCIERROR status=mciSendString(command,0,0,0);
 if(!status) {
  status=mciSendString("status mov window handle",command,255,0);
  if(!status) { movieWindow=(HWND)atoi(command); scanf(command,"%d",&movieWindow); }
  else { mciGetErrorString(status,command,255); MessageBox(owner,command,0,0x30); }
  MoviePlace(); wsprintf(command,"setvideo mov palette handle to %d",moviePalette);
  status=mciSendString(command,0,0,0); char error[100]; sprintf(error,"retVal is %d",status);
  if(status) { mciGetErrorString(status,error,100); MessageBox(owner,error,"string palette setting error",0x30); }
  MovieCapture(); mciSendString("play mov window notify",0,0,owner); GuiPumpMessages(); ResetFrameClock();
  moviePlaying=1; int previous=BriefWaitA(0); SimSetCursor(1,1); CursorUpdate(); HelpClearTopic();
  while(moviePlaying) {
   GuiPumpMessages();
   if(ReadFrameInput() || (GetAsyncKeyState(1)&0x8000) || (GetAsyncKeyState(2)&0x8000)) { moviePlaying=0; movieSkipped=1; }
   else Sleep(100);
  }
  MovieClose(); MovieRelease(); BriefWaitA(previous);
 }
 MovieTerminate();
 do { GuiPumpMessages(); } while((GetAsyncKeyState(1)&0x8000) || (GetAsyncKeyState(2)&0x8000));
 ResetFrameClock(); SetBkColor(dc,0); Rectangle(dc,0,0,2000,2000);
 ApplyPaletteData(&gGuiPalette); GuiResetClip(&gMenuClip); SoundInit(); moviePlaying=0;
}

