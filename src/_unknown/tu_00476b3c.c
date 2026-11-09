// System and resource cursors. Indices follow the original table stores.
#include <windows.h>
extern HINSTANCE cursorInstance;
extern int customCursors,cursorDirty,currentCursor;
extern HCURSOR cursorHandles[34];
extern "C" void RegisterAssertCallback(void (*)());
void CursorClose();
void SimSetCursor(int,int);
void CursorInit() {
 int custom=customCursors;
 cursorHandles[0]=0;
 cursorHandles[1]=LoadCursor(0,(const char*)32512);
 cursorHandles[2]=LoadCursor(0,(const char*)32514);
 cursorHandles[3]=LoadCursor(0,(const char*)32648);
 cursorHandles[10]=LoadCursor(0,(const char*)32641);
 cursorHandles[11]=LoadCursor(0,(const char*)32646);
 cursorHandles[4]=cursorHandles[25]=LoadCursor(cursorInstance,"MCursor1");
 cursorHandles[7]=cursorHandles[28]=LoadCursor(cursorInstance,"MCursor3");
 cursorHandles[8]=cursorHandles[29]=LoadCursor(cursorInstance,"MCursor4");
 cursorHandles[9]=cursorHandles[30]=LoadCursor(cursorInstance,"MCursor5");
 if(custom) {
  cursorHandles[1]=cursorHandles[22]=LoadCursor(cursorInstance,"MCursor6");
  cursorHandles[11]=cursorHandles[32]=LoadCursor(cursorInstance,"MCursor7");
 }
 cursorHandles[5]=cursorHandles[1]; cursorHandles[6]=cursorHandles[1];
 cursorHandles[12]=cursorHandles[11]; cursorHandles[13]=cursorHandles[11]; cursorHandles[14]=cursorHandles[11]; cursorHandles[15]=cursorHandles[11];
 cursorHandles[16]=cursorHandles[11]; cursorHandles[17]=cursorHandles[11]; cursorHandles[18]=cursorHandles[11]; cursorHandles[19]=cursorHandles[11];
 cursorHandles[20]=cursorHandles[1];
 RegisterAssertCallback(CursorClose); SimSetCursor(1,0);
}
void CursorClose() {}
void SimSetCursor(int cursor,int force) {
 if(cursorHandles[cursor]!=cursorHandles[currentCursor] || force) cursorDirty=1;
 currentCursor=cursor;
}
void CursorUpdate() {
 if(cursorDirty) { cursorDirty=0; if(cursorHandles[currentCursor]) SetCursor(cursorHandles[currentCursor]); }
}
int CursorCurrent() { return currentCursor; }
