// Version text and persisted 211-byte settings record; working field names.
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
extern int versionMajor,versionMinor,versionSuffix;
extern char *versionExtra;
extern "C" char assertVersion[];
extern char settingsPath[];
void ExploIdle() {}
void FormatVersion() { sprintf(assertVersion,"%d.%02d%c %s",versionMajor,versionMinor,versionSuffix,versionExtra); }
struct FileRecord; void FileInit(FileRecord*);
struct FileRecord { HANDLE handle; unsigned long open,flags,error; signed char direct,counted; char reserve[2]; unsigned long position,length;
 FileRecord() { FileInit(this); } ~FileRecord(); int Open(const char*,unsigned long);
};
void FileInit(FileRecord*); unsigned long IoClose(FileRecord*); int IORelease(FileRecord*);
unsigned long StreamRead(FileRecord*,void*,unsigned long); unsigned long StreamWrite(FileRecord*,const void*,unsigned long);
#pragma pack(push,1)
struct Settings { int enabled,music,sound,voices,ambient,video,fullEffects,field1c,field20,field24,field28,field2c,field30,quality0,quality1,quality2,mode,field44,field48,field4c,field50,field54,field58,field5c,field60,field64,field68; char rest[103]; };
#pragma pack(pop)
#pragma pack(push,2)
struct Options { char data[66]; Options(); };
#pragma pack(pop)
extern Options defaultOptions;
int FileExists(const char*);
void SettingsDefaults(Settings*);
int SettingsLoad(Settings *settings) {
 FileRecord file;
 if(!file.Open(settingsPath,0) || IoClose(&file)!=211) { SettingsDefaults(settings); return 0; }
 StreamRead(&file,settings,211); IORelease(&file); return 1;
}
int SettingsSave(Settings *settings) {
 FileRecord file;
 if(!file.Open(settingsPath,5)) return 0;
 StreamWrite(&file,settings,211); IORelease(&file); return 1;
}
void SettingsDefaults(Settings *settings) {
 memset(settings,0,211);
 unsigned cpu=0; char description[40];
 GetPrivateProfileString("Config","CPU","",description,40,"SIERRA.INI");
 if(strlen(description)) {
  if(strstr(description,"486")) cpu=1;
  else if(strstr(description,"Pro")) cpu=4;
  else { int speed=atoi(description+8); if(speed<=66) cpu=2; else if(speed<=100) cpu=3; else cpu=4; }
 }
 switch(cpu) {
 default: settings->fullEffects=0; settings->quality0=50; settings->quality1=0; settings->quality2=25; settings->field44=1; settings->field48=1; break;
 case 3: settings->fullEffects=0; settings->quality0=75; settings->quality1=25; settings->quality2=50; settings->field44=1; settings->field48=1; break;
 case 4: settings->fullEffects=1; settings->quality0=100; settings->quality1=50; settings->quality2=75; settings->field44=0; settings->field48=1; break;
 }
 settings->enabled=1; settings->music=100; settings->sound=100; settings->voices=100; settings->ambient=100; settings->video=100;
 settings->field28=1; settings->field2c=0; settings->field1c=1; settings->field20=0; settings->field24=1; settings->field30=1;
 settings->field4c=10; settings->field50=10; settings->field64=0; settings->field68=0; settings->mode=4;
 settings->field54=0; settings->field58=1; settings->field5c=1; settings->field60=1;
 if(!FileExists("CYBDATA1.RBX")) { settings->music=25; settings->sound=100; settings->voices=50; settings->ambient=0; settings->video=0; }
 Options options; defaultOptions=options;
}
