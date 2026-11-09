// Sierra network DLL adapter. Working names for the DLL's C export table.
#include <windows.h>
#include <string.h>
extern HWND g_mainWindow;
extern HMODULE sigsModule;
extern char sigsToken[];
extern int sigsValue0,sigsValue1,sigsGameId,sigsActive,networkEnabled,gLinkSigsMode,hostPeer,localPeer,isHostPeer;
extern HANDLE gLinkThread;
extern int (*sigsInitialize)(void*,int,int),(*sigsConnectStatus)(),(*sigsGameClose)(),(*sigsMyPlayer)(),(*sigsAnyMessage)(),(*sigsPlayerCount)(),(*sigsGetOption)(),(*sigsClose)();
extern int (*sigsPlayerName)(char*,unsigned short),(*sigsPlayerStatus)(int),(*sigsSend)(void*,unsigned short),(*sigsReceive)(void*,unsigned short,int),(*sigsPeek)(void*,int,int),(*sigsWait)(),(*sigsSendTo)(int,void*,unsigned short),(*sigsLeftCallback)(int,void (*)(int),int,int);
extern "C" void AssertExit(int);
int SigsValidate(); int SigsLoad();
int SigsStop() { if(!SigsValidate()) return 0; if(!SigsLoad()) return 0; return sigsActive=1; }
int SigsValidate() {
 HMODULE module=LoadLibrary("SNWValid.dll");
 if(!module) return 0;
 typedef int (*Validate)(const char*,void*,int*,int*,int*,const char*,HWND);
 Validate validate=(Validate)GetProcAddress(module,"SNWValidate");
 int status,result; result=0; status=-1;
 if(validate) { status=validate("CyberStorm",sigsToken,&sigsValue0,&sigsValue1,&sigsGameId,"Cstorm",g_mainWindow); if(status==0) result=1; }
 FreeLibrary(module); if(status==-2) AssertExit(0); return result;
}
int SigsLoad() {
 sigsModule=LoadLibrary("SierraNW.dll");
 if(!sigsModule) return 0;
 sigsInitialize=(int (*)(void*,int,int))GetProcAddress(sigsModule,"InitializeDLL");
 sigsConnectStatus=(int (*)())GetProcAddress(sigsModule,"CheckGameConnectStatus");
 sigsGetOption=(int (*)())GetProcAddress(sigsModule,"GetGameOption");
 sigsClose=(int (*)())GetProcAddress(sigsModule,"CloseDLL");
 sigsGameClose=(int (*)())GetProcAddress(sigsModule,"GameClose");
 sigsMyPlayer=(int (*)())GetProcAddress(sigsModule,"GetMyPlayerNumber");
 sigsPlayerCount=(int (*)())GetProcAddress(sigsModule,"GetNumberOfPlayers");
 sigsPlayerName=(int (*)(char*,unsigned short))GetProcAddress(sigsModule,"GetPlayerXName");
 sigsPlayerStatus=(int (*)(int))GetProcAddress(sigsModule,"CheckPlayerConnectStatus");
 sigsSend=(int (*)(void*,unsigned short))GetProcAddress(sigsModule,"SendTCPMessage");
 sigsReceive=(int (*)(void*,unsigned short,int))GetProcAddress(sigsModule,"RecvTCPMessage");
 sigsWait=(int (*)())GetProcAddress(sigsModule,"WaitForTCPMessage");
 sigsPeek=(int (*)(void*,int,int))GetProcAddress(sigsModule,"PeekForTCPMessage");
 sigsSendTo=(int (*)(int,void*,unsigned short))GetProcAddress(sigsModule,"SendTCPPointMessage");
 sigsAnyMessage=(int (*)())GetProcAddress(sigsModule,"CheckForAnyTCPMessage");
 sigsLeftCallback=(int (*)(int,void (*)(int),int,int))GetProcAddress(sigsModule,"TellMePlayerLeftGame");
 if(!sigsInitialize) return 0;
 return sigsInitialize(sigsToken,sigsValue0,sigsValue1);
}
void link_force_shutdown(); void SigsPlayerLeft(int);
int link_is_ready(char *name) {
 char buffer[512]; if(!sigsModule) return 0;
 int status; while(!(status=sigsConnectStatus())) Sleep(500);
 if(status!=1) { link_force_shutdown(); return 0; }
 hostPeer=1; localPeer=sigsMyPlayer()+1; isHostPeer=(localPeer!=1)+1;
 sigsPlayerName(buffer,(unsigned short)(localPeer-1));
 memset(name,0,25); strncpy(name,buffer,23); SetForegroundWindow(g_mainWindow);
 sigsLeftCallback(1,SigsPlayerLeft,0,0); return 1;
}
void link_force_shutdown() {
 if(sigsClose && sigsModule) sigsClose();
 if(sigsModule) FreeLibrary(sigsModule);
 sigsActive=0; networkEnabled=0; gLinkSigsMode=0; sigsModule=0;
}
int SigsSend(int ignored,int peer,int unused,void *data,unsigned short size) {
 if(!sigsModule) return 0;
 if(peer==0) return sigsSend(data,size); else return sigsSendTo(peer-1,data,size);
}
int link_alt_receive(void *from,void *to,int unused,void *data,int size) {
 const int noMessage=0x887700be;
 if(sigsModule && !sigsAnyMessage()) {
  int mine=localPeer-1;
  for(int i=0;i<=16;i++) {
   if(i==mine) continue;
   if(!sigsModule) return noMessage;
   int result=sigsPeek(data,10,i);
   if(result>=0) {
    if(!sigsModule) return noMessage;
    if(sigsReceive(data,(unsigned short)size,i)<0) return noMessage;
    *(int*)from=i+1; *(int*)to=localPeer; return 0;
   }
   if(result!=-2) return noMessage;
  }
 }
 return noMessage;
}
void alternate_link_disconnect() {}
void link_load_settings(void *data) {
 int *s=(int*)data; memset(s,0,28); s[0]=28; s[1]=0; s[2]=1000; s[3]=70; s[4]=16; s[5]=580; s[6]=50;
}
void SigsPlayerLeft(int peer) { PostMessage(g_mainWindow,0x111,0x7664,peer+1); }
struct InputReq;
struct Scene { void Open(InputReq*); };
extern Scene g4b3598,g4ba2bc;
extern InputReq multiplayerInput;
extern int menuInputState,menuInputResult;
void ShellMissionClose(Scene*); int StartMultiplayer(const char*,int);
int SigsRestart() {
 if(!gLinkSigsMode || !sigsModule || !sigsGameClose) return 0;
 sigsGameClose();
 if(gLinkThread) { Sleep(100); TerminateThread(gLinkThread,0); CloseHandle(gLinkThread); gLinkThread=0; }
 int result=StartMultiplayer("",1);
 if(result) { ShellMissionClose(&g4b3598); g4ba2bc.Open(&multiplayerInput); menuInputResult=37; menuInputState=37; return 1; }
 return 0;
}

