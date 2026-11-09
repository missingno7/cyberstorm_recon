// Special option menu strings. Plain object begins after the save-header object.
#include <string.h>
char *DecodeOption(char *text) { while(*text) { *text^=0xbf; text++; } return text; }
extern void *specialMenu;
extern int specialSeparator;
void *create_chat_popup(void*,int);
void build_chat_menu_line(void*,unsigned,const char*,unsigned);
int chat_menu_valid(void*);
void enable_chat_menu(void*,int,int,int);
void AddSpecialOption(int id,char *text) {
 if((signed char)*text==0) { if(specialMenu) specialSeparator=1; return; }
 if(!specialMenu) specialMenu=create_chat_popup((void*)"Special",-1);
 if(specialMenu) {
  if(specialSeparator) build_chat_menu_line(specialMenu,0,0,21);
  specialSeparator=0; build_chat_menu_line(specialMenu,id,text,21);
 }
}
int SoundTrigger(unsigned,int);
void SpecialOptionSound(int id) { if(id>=31980 && id<=32080) SoundTrigger(420,0); }
struct StrPool { long id; char *buffer; long capacity; void *data; long size;
 StrPool(long,long,long); ~StrPool(); int Lock(); void Unlock(); char *GetString(long,int); char *GetSecondString(long,int);
};
struct ResMgr { char *Load(const char*,int,void*,unsigned long); };
extern ResMgr gResMgr;
extern char gDebugText[];
// optionNetwork 004aebf0; specialOptionFound 004dd0b4; specialOptionEnabled 00494290; specialOptionSelection 004c9748.
extern int optionNetwork,modeSwitch,specialOptionFound,specialOptionEnabled,specialOptionSelection;
extern int modeCheckName[];
void DebugTextClear();
inline int ModeActive() { return modeCheckName[0]; }
void ResetMultiplayerInput() {
 DebugTextClear();
 if(gResMgr.Load("CStorm.ini",0,gDebugText,65536)) {
  strupr(gDebugText);
  StrPool strings((long)"OPTIONS.bin",0,0); strings.Lock();
  if(specialMenu) { chat_menu_valid(specialMenu); specialMenu=0; }
  int flags=1;
  int network=(optionNetwork!=0); if(network) flags|=2;
  if(ModeActive()) { if(!modeSwitch) flags|=4; else flags|=8; }
  char *key;
  for(int id=31980;id<32080;id++) {
   key=strings.GetSecondString(id,1);
   if(key) {
    DecodeOption(key); strupr(key);
    if(strstr(gDebugText,key) || (signed char)*key==0) {
     key=strings.GetString(id,1); DecodeOption(key);
     int mask=(signed char)*key;
     if((flags&mask) || !mask) AddSpecialOption(id,key+1);
     if(id==31982) specialOptionFound=1;
     if(id==32070) specialOptionEnabled=0;
    }
   }
  }
  enable_chat_menu(specialMenu,specialOptionSelection+31995,1,3);
  strings.Unlock(); DebugTextClear();
 }
}
