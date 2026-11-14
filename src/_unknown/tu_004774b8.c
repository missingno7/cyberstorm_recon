// Main menu policy and dynamic menu helpers. Working names; command IDs retained.
#include <windows.h>
extern HMENU g512544;
extern HWND g_mainWindow;
// menuReady 00512524; menuMode 00512528.
extern int menuReady,menuMode,windowOriginX,windowOriginY,gViewX,gViewY;
extern "C" void RegisterAssertCallback(void (*)());
void ReadMousePosition(POINT*);
inline HMENU MenuEnableAll(HMENU menu,int disabled) {
 int count; unsigned flags; int i;
 flags=disabled!=0; count=GetMenuItemCount(menu);
 for(i=0;i<count;i++) EnableMenuItem(menu,i,flags|0x400);
 return menu;
}
inline void MenuMainFlags(int save,int load,int always,int mode3,int mode4) {
 unsigned a=always?1:0,b=mode3?1:0,c=mode4?1:0,d=load?1:0,e=save?1:0;
 EnableMenuItem(g512544,0x7531,e); EnableMenuItem(g512544,0x7532,d); EnableMenuItem(g512544,0x7535,a);
 EnableMenuItem(g512544,0x7538,b); EnableMenuItem(g512544,0x7537,c);
}
inline int MenuFileDisabled(int mode) { if(mode<10 || mode>10) return 1; return 0; }
inline int MenuAlwaysFlag(int mode) { if(mode==0 || mode==1 || mode==11) return 0; return 0; }
void MenuApplyMode(int mode) {
 HMENU menu=GetSubMenu(g512544,1);
 int disabled;
 disabled=MenuFileDisabled(mode);
 if(mode==13) disabled=0;
 menu=MenuEnableAll(menu,disabled);
 if(mode==11 || mode==14) { EnableMenuItem(menu,5,0x400); EnableMenuItem(menu,8,0x400); }
 if(mode==12) EnableMenuItem(menu,4,0x400);
 menu=GetSubMenu(g512544,2);
 menu=MenuEnableAll(menu,mode!=2);
 int always=0;
 if(mode==0 || mode==1 || mode==11) always=0;
 int mode3=(mode!=3),mode4=(mode!=4),load=1,save=1;
 switch(mode) { case 1: load=0;break;case 2: case 7: case 8: case 10: load=0; break; }
 switch(mode) { case 2:save=0;break;case 7: case 8: case 9:save=0;break;case 10: save=0; break; }
 MenuMainFlags(save,load,always,mode3,mode4);
 if(mode==8 || mode==9) { EnableMenuItem(g512544,0x75fc,0); EnableMenuItem(g512544,0x75fb,0); EnableMenuItem(g512544,0x75fa,0); }
}
void MenuTrackContext() {
 HMENU menu=GetSubMenu(g512544,1); if(menu) { menu=GetSubMenu(menu,5); if(menu) { POINT p; ReadMousePosition(&p); TrackPopupMenu(menu,0,p.x+windowOriginX+gViewX,p.y+windowOriginY+gViewY,0,g_mainWindow,0); } }
}
void MenuShutdown() { menuReady=0; }
void MenuInit() { if(!menuReady) { RegisterAssertCallback(MenuShutdown); menuReady=1; } }
extern "C" int BriefWaitA(int mode) { MenuInit(); MenuApplyMode(mode); DrawMenuBar(g_mainWindow); int old=menuMode; menuMode=mode; return old; }
void *create_chat_popup(void *title,int position) {
 HMENU menu=CreatePopupMenu(); if(!menu) return 0;
 if(position==-1) position=GetMenuItemCount(g512544)-1;
 if(!InsertMenu(g512544,position,0x411,(UINT)menu,(const char*)title)) return 0;
 DrawMenuBar(g_mainWindow); return menu;
}
int chat_menu_valid(HMENU menu) {
 if(!menu) return 0;
 int result=0;
 for(int i=0;i<GetMenuItemCount(g512544);i++) if(GetSubMenu(g512544,i)==menu) result=DeleteMenu(g512544,i,0x400);
 DrawMenuBar(g_mainWindow); return result;
}
int MenuClear(HMENU menu) {
 if(!menu) return 0;
 while(GetMenuItemCount(menu)) DeleteMenu(menu,0,0x400);
 return 1;
}
int build_chat_menu_line(HMENU menu,unsigned id,const char *text,unsigned columnSize) {
 if(!menu) return 0;
 unsigned count=GetMenuItemCount(menu);
 unsigned char flags=((int)columnSize>0 && count>0 && count%columnSize==0)?0x20:0;
 if(!AppendMenu(menu,flags,id,text)) return 0;
 if(!count) {
  int i; for(i=0;i<GetMenuItemCount(g512544);i++) if(GetSubMenu(g512544,i)==menu) break;
  if(GetMenuState(g512544,i,0x400)|1) EnableMenuItem(g512544,i,0x400);
 }
 DrawMenuBar(g_mainWindow); return 1;
}
int MenuDeleteLine(HMENU menu,unsigned id) {
 if(!menu) return 0; int result=0;
 for(int i=0;i<GetMenuItemCount(menu);i++) if(GetMenuItemID(menu,i)==id) { result=DeleteMenu(menu,i,0x400); break; }
 if(GetMenuItemCount(menu)==0) for(int j=0;j<GetMenuItemCount(g512544);j++) if(GetSubMenu(g512544,j)==menu) EnableMenuItem(g512544,j,0x401);
 DrawMenuBar(g_mainWindow); return result;
}
int enable_chat_menu(HMENU menu,int item,int checked,int lookup) {
 if(!menu) return 0;
 int count=GetMenuItemCount(menu); if(lookup==4 && item>=count) return 0;
 unsigned flags=0; if(checked) flags=8; if(lookup!=3) flags|=0x400;
 return (int)CheckMenuItem(menu,item,flags)>=0;
}
int check_chat_menu(HMENU menu) {
 if(!menu) return 0;
 int count=GetMenuItemCount(menu); if(!count) return 0;
 for(int i=0;i<count;i++) enable_chat_menu(menu,i,0,2);
 return 1;
}
int chat_menu_state(HMENU menu,int item,int lookup) {
 if(!menu) return 0;
 int count=GetMenuItemCount(menu); if(lookup==4 && item>=count) return 0;
 unsigned flags=0; if(lookup!=3) flags=0x400;
 return (GetMenuState(menu,item,flags)&8)>1;
}
void set_chat_menu_item(HMENU menu,int item,int lookup) {
 if(!menu) return;
 int count=GetMenuItemCount(menu); if(lookup==4 && item>=count) return;
 unsigned flags=0; if(lookup!=3) flags=0x400;
 if(!(GetMenuState(menu,item,flags)&8)) flags|=8;
 CheckMenuItem(menu,item,flags);
}
int MenuCount(HMENU menu) { if(!menu) return 0; return GetMenuItemCount(menu); }

