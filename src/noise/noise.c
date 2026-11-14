/* DirectSound manager. Working names; COM method slots follow DirectSound 1. */
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <string.h>
typedef unsigned long U32; static char noiseSourceFile[]=__FILE__;
struct SoundBuffer; struct SoundDevice; struct BufferDesc;
struct SoundBufferVTable {
 void *query,*addRef; U32 (__stdcall *Release)(SoundBuffer *);
 void *getCaps,*getPosition,*getFormat,*getVolume,*getPan,*getFrequency;
 long (__stdcall *GetStatus)(SoundBuffer *,U32 *);
 void *initialize;
 long (__stdcall *Lock)(SoundBuffer *,U32,U32,void **,U32 *,void **,U32 *,U32);
 long (__stdcall *Play)(SoundBuffer *,U32,U32,U32);
 void *setPosition,*setFormat;
 long (__stdcall *SetVolume)(SoundBuffer *,long);
 long (__stdcall *SetPan)(SoundBuffer *,long);
 long (__stdcall *SetFrequency)(SoundBuffer *,U32);
 long (__stdcall *Stop)(SoundBuffer *);
 long (__stdcall *Unlock)(SoundBuffer *,void *,U32,void *,U32);
 long (__stdcall *Restore)(SoundBuffer *);
};
struct SoundBuffer {SoundBufferVTable *vtable;};
struct SoundDeviceVTable {
 void *query,*addRef; U32 (__stdcall *Release)(SoundDevice *);
 long (__stdcall *CreateSoundBuffer)(SoundDevice *,BufferDesc *,SoundBuffer **,void *);
 void *getCaps;
 long (__stdcall *DuplicateSoundBuffer)(SoundDevice *,SoundBuffer *,SoundBuffer **);
 long (__stdcall *SetCooperativeLevel)(SoundDevice *,HWND,U32);
};
struct SoundDevice {SoundDeviceVTable *vtable;};
struct BufferDesc {U32 size,flags,bytes,reserved; WAVEFORMATEX *format;};
struct SoundSettings {int loop,frequency,pan,volume;};
struct SoundPriority {signed char threshold,keep,category;};
struct SoundVoice {int id; SoundBuffer *buffer;};
struct SoundQueueItem {unsigned id; int delay;};
typedef long (__stdcall *SoundCreateProc)(void *,SoundDevice **,void *);
int soundPollDeadline; SoundDevice *soundDevice;
SoundBuffer *soundCache[440]; int soundTouched[440]; int soundFrequency[440];
SoundPriority soundPriority[440]; int soundEnabled,soundCreated,soundReleased;
HMODULE soundLibrary; SoundCreateProc soundCreate;
SoundVoice soundVoices[32]; SoundQueueItem soundCurrent,soundQueue[4]; int soundQueueCount;
extern void *g_mainWindow;
extern int soundEffectsVolume,soundVoiceVolume,soundMasterVolume,soundMusicVolume;
SoundSettings soundDefaults={0,100,0,100},shieldSoundDefaults={1,100,0,100};
short soundAttenuation[101]={100,93,87,80,73,67,60,56,52,48,44,40,40,38,36,34,32,30,28,27,26,25,24,23,22,20,20,19,19,18,18,17,17,17,16,16,16,15,15,14,14,14,13,13,13,12,12,12,11,11,10,10,10,9,9,9,9,8,8,8,8,8,7,7,7,7,7,6,6,6,6,6,5,5,5,5,4,4,4,4,3,3,3,3,2,2,2,2,2,1,1,1,1,1,0,0,0,0,0,0,0};
struct ResMgr {void *Load(long,int=1,int=0,int=0);}; extern ResMgr gResMgr;
void *GlobalBlockAlloc(U32,unsigned); void *GlobalBlockFree(void *); void ReleasePoolData(void *);
U32 TimeNow();
#pragma pack(push,1)
struct WaxHeader {U32 magic,compression,bytes; WAVEFORMATEX format;};
struct LinkDecoderState {unsigned char *source,*destination; U32 count,bytes,sample; int predictor,difference; unsigned short encoded,nibble,step,stepIndex,format;};
#pragma pack(pop)
void decode_link_samples(LinkDecoderState *,unsigned);
#pragma aux decode_link_samples "W?decode_link_samples$:LinkDecoderOwner$n(uipn$LinkDecoderState$$ui)$LinkDecodeResult$$"
void ViewFlip(int);
int SoundCreateBuffer(int,SoundBuffer **,WAVEFORMATEX *);
int SoundDuplicate(int,SoundBuffer **,WAVEFORMATEX *);
int SoundCached(int); int SoundCache(int); void SoundRelease(unsigned);
int SoundAllowed(int); int SoundLoad(unsigned); int SoundFinished(unsigned);
int SoundPlay(unsigned,const SoundSettings *); int SoundPlayDefault(unsigned,int);
void SoundFlush(int); void ShellSoundShutdown();
static inline int SoundClamp(int value,int low,int high) {if(value<=low)return low;if(value>=high)return high;return value;}
static inline void SoundPan(SoundBuffer *buffer,int pan){buffer->vtable->SetPan(buffer,pan*100);}
static inline void SoundVolume(SoundBuffer *buffer,int volume){buffer->vtable->SetVolume(buffer,-(soundAttenuation[volume]*100));}
static inline void SoundRate(SoundBuffer *buffer,int percent,int rate){buffer->vtable->SetFrequency(buffer,rate*percent/100);}

int SoundInit() {
 if(soundEnabled) return 1;
 if(!soundLibrary) {soundLibrary=LoadLibraryA("DSOUND.DLL");
  if(soundLibrary) soundCreate=(SoundCreateProc)GetProcAddress(soundLibrary,"DirectSoundCreate");}
 if(soundCreate && soundCreate(0,&soundDevice,0)==0)
  soundDevice->vtable->SetCooperativeLevel(soundDevice,(HWND)g_mainWindow,1);
 gResMgr.Load((long)"DIGIPRI.XXX",0,(int)soundPriority,0x528);
 return soundEnabled=(soundDevice!=0);
}
void f_46b9bd() {
 if(soundEnabled) {
  soundEnabled=0;
  SoundVoice *voice=soundVoices;for(int i=0;i<32;i++,voice++){if(voice->id)voice->buffer->vtable->Stop(voice->buffer);}
  soundQueueCount=0;
  Sleep(100); ViewFlip(1);
  for(int j=0;j<440;j++) SoundRelease(j);
  if(soundDevice){soundDevice->vtable->Release(soundDevice);soundDevice=0;}
  if(soundLibrary){FreeLibrary(soundLibrary);soundLibrary=0;soundCreate=0;}
  memset(soundCache,0,sizeof(soundCache));memset(soundTouched,0,sizeof(soundTouched));
  memset(soundFrequency,0,sizeof(soundFrequency));memset(soundPriority,0,sizeof(soundPriority));
  memset(&soundCurrent,0,sizeof(soundCurrent));memset(soundQueue,0,sizeof(soundQueue));memset(soundVoices,0,sizeof(soundVoices));
 }
}
int SoundCreateBuffer(int id,SoundBuffer **result,WAVEFORMATEX *format) {
 unsigned char *samples; WaxHeader *resource; unsigned char *allocated; int success;
 int bytes; success=0; bytes=0; samples=0; allocated=0;
 BufferDesc desc; char name[20]; LinkDecoderState state;
 sprintf(name,"DIGI_%03d.WAX",id);
 resource=(WaxHeader *)gResMgr.Load((long)name,0);
 if(resource) {
  const WAVEFORMATEX *sourceFormat=&resource->format;
  bytes=resource->bytes; samples=(unsigned char *)(resource+1); *format=*sourceFormat;
  if(resource->compression==4) {
   int sampleCount=bytes/(format->wBitsPerSample/4);
   unsigned char *decoded=(unsigned char *)GlobalBlockAlloc(bytes+16,0); allocated=decoded;
   if(decoded) {
    U32 *sentinel=(U32 *)(decoded+bytes+12); *sentinel=0x1c43fe;
    memset(&state,0,sizeof(state));
    state.count=sampleCount;state.bytes=bytes;state.format=format->wBitsPerSample;
    state.source=samples;state.destination=decoded;state.step=7;
    decode_link_samples(&state,bytes); samples=decoded;
   }
   else samples=decoded;
   if(resource){ReleasePoolData(resource);resource=0;}
  }
 }
 if(!samples) {soundPriority[id].threshold=0; *result=0; return 0;}
 else {
  memset(&desc,0,sizeof(desc));desc.size=sizeof(desc);desc.flags=0xe0;desc.bytes=bytes;desc.format=format;
  soundFrequency[id]=format->nSamplesPerSec;
  if(soundDevice->vtable->CreateSoundBuffer(soundDevice,&desc,result,0)==0) {
   SoundBuffer **const output=result;
   ++soundCreated;
   void *first,*second; U32 firstBytes,secondBytes;
   long hr=(*output)->vtable->Lock(*output,0,bytes,&first,&firstBytes,&second,&secondBytes,0);
   if(hr==(long)0x88780096UL) {
    (*output)->vtable->Restore(*output);
    hr=(*output)->vtable->Lock(*output,0,bytes,&first,&firstBytes,&second,&secondBytes,0);
   }
   if(hr==0) {
    memcpy(first,samples,firstBytes); if(second) memcpy(second,samples+firstBytes,secondBytes);
    if((*output)->vtable->Unlock(*output,first,firstBytes,second,secondBytes)==0) success=1;
   }
  }
  if(resource)ReleasePoolData(resource); if(allocated)GlobalBlockFree(allocated);
 }
 return success;
}
int SoundDuplicate(int id,SoundBuffer **result,WAVEFORMATEX *format) {
 if(!id || id>=440 || !soundEnabled)return 0;
 if(soundCache[id]) {
  if(soundDevice->vtable->DuplicateSoundBuffer(soundDevice,soundCache[id],result)!=0)return 0;
  ++soundCreated;return 1;
 }
 return SoundCreateBuffer(id,result,format);
}
int SoundCached(int id) {if(!id || id>=440 || !soundEnabled)return 0;return soundCache[id]!=0;}
int SoundCache(int id) {
 if(SoundCached(id))return 1;
 if(SoundAllowed(id)) {
  SoundBuffer *buffer; WAVEFORMATEX format;
  if(SoundDuplicate(id,&buffer,&format)){soundCache[id]=buffer;soundTouched[id]=0;return 1;}
 }
 return 0;
}

void SoundCacheList(int *ids){while(*ids){SoundCache(*ids++);}}

void SoundRelease(unsigned id) {
 if(id && (int)id<440 && soundEnabled) {
  if(soundCache[id]){++soundReleased;soundCache[id]->vtable->Release(soundCache[id]);soundCache[id]=0;soundTouched[id]=0;}
 }
}
void SoundReleaseList(int *ids){while(*ids){int id=*ids++;SoundRelease(id);}}
void SoundReleaseAll(){for(int i=0;i<440;i++)SoundRelease(i);}
int SoundLoad(unsigned id) {if(SoundCached(id))return 1; if(SoundCache(id)){soundTouched[id]=TimeNow();return 1;}return 0;}

void SoundFlush(int age) {int i;int limit=TimeNow()-age;int *t=soundTouched;for(i=0;i<440;i++,t++){if(*t && limit>*t)SoundRelease(i);}}

int SoundFinished(unsigned id) {
 if(!id || (int)id>=440 || !soundEnabled)return 0;
 SoundVoice *v=soundVoices;for(int i=0;i<32;i++,v++){if(v->id==id)return 0;}return 1;
}
void ShellSoundShutdown(){SoundVoice *v=soundVoices;for(int i=0;i<32;i++,v++){if(v->id)v->buffer->vtable->Stop(v->buffer);}}
void SoundStop(unsigned id){if(id && (int)id<440 && soundEnabled){SoundVoice *v=soundVoices;for(int i=0;i<32;i++,v++){if(v->id==id)v->buffer->vtable->Stop(v->buffer);}}}
int SoundAllowed(int id) {
 if(!id || id>=440)return 0;
 signed char category=soundPriority[id].category; signed char priority=soundPriority[id].threshold;
 if(category==0)return priority>100-soundEffectsVolume; if(category==1)return priority>100-soundVoiceVolume; return priority>100-soundMusicVolume;
}

int SoundPlay(unsigned id,const SoundSettings *settings) {
 if(!id || (int)id>=440)return 0;
 if(!SoundAllowed(id))return 0;
 if(settings && settings->volume<=0)return 0;
 if((soundPriority[id].threshold&128) && soundMasterVolume<10)return 0;
 if(!soundEnabled)return 0;
 SoundVoice *voice=soundVoices;int i;
 for(i=0;i<32 && voice->id;i++,voice++){}
 if(i>=32)return 0;
 int keep=soundPriority[id].keep;
 if(keep)SoundLoad(id);
 WAVEFORMATEX format; SoundBuffer *buffer;
 if(!SoundDuplicate(id,&buffer,&format))return 0;
 int volume=((unsigned char)soundPriority[id].category==1)?soundMasterVolume:100;
 int pan=0,frequency=100;
 if(settings){pan=SoundClamp(settings->pan,-100,100);
  volume=settings->volume*volume/100;frequency=settings->frequency*frequency/100;}

 if(frequency!=100)buffer->vtable->SetFrequency(buffer,frequency*soundFrequency[id]/100);

 if(volume<=0)volume=0;else if(volume>=99)volume=99;

 int attenuation=soundAttenuation[volume]*-100;buffer->vtable->SetVolume(buffer,attenuation);

 if(pan)buffer->vtable->SetPan(buffer,pan*100);
 int looping=settings && settings->loop ? 1 : 0;buffer->vtable->Play(buffer,0,0,looping);
 voice->id=id;voice->buffer=buffer;

 if(keep){int delay=(keep*1000-20000)/2;if(delay<0)delay=0;soundTouched[id]=TimeNow()+delay;}

 return 1;
}

int SoundSetSettings(int id,const SoundSettings *settings) {
 if(!id || id>=440 || !soundEnabled || !settings)return 0;
 SoundVoice *v=soundVoices;
 for(int i=0;i<32;i++,v++)if(v->id==id) {
  SoundPan(v->buffer,SoundClamp(settings->pan,-100,100));
  SoundVolume(v->buffer,SoundClamp(settings->volume,0,100));
  SoundRate(v->buffer,settings->frequency,soundFrequency[id]);
 }
 return 1;
}
int SoundPlayDefault(unsigned id,int delay) {
 int i;for(i=0;i<soundQueueCount;i++)if(soundQueue[i].id==id)return 0;
 if(!id || SoundFinished(id)) {
 if(soundQueueCount>=4){for(i=0;i<soundQueueCount-1;i++)soundQueue[i]=soundQueue[i+1];soundQueueCount--;}
 if(soundQueueCount>=4)return 0;
 soundQueue[soundQueueCount].id=id;soundQueue[soundQueueCount].delay=delay;++soundQueueCount;
 SoundLoad(id);
 ViewFlip(1);return 1;
 } return 0;
}
void SoundClearContext(){soundQueueCount=0;}
int SoundTrigger(unsigned id,int delay){return SoundPlayDefault(id,delay);}
void ViewFlip(int force) {
 if(soundEnabled) {
 int now=TimeNow(); if(force || now>=soundPollDeadline) {
 soundPollDeadline=now+250;
 SoundVoice *v=soundVoices;
 for(int i=0;i<32;i++,v++)if(v->id){U32 status;v->buffer->vtable->GetStatus(v->buffer,&status);if(!(status&1)){++soundReleased;v->buffer->vtable->Release(v->buffer);v->buffer=0;v->id=0;}}
 if(soundQueueCount){int ready=1;if(soundCurrent.id && !SoundFinished(soundCurrent.id))ready=0;if(now<soundCurrent.delay)ready=0;
  if(ready){soundCurrent=soundQueue[0];for(i=0;i<soundQueueCount-1;i++)soundQueue[i]=soundQueue[i+1];soundQueueCount--;
   soundCurrent.delay+=TimeNow();SoundSettings settings=soundDefaults;settings.volume=35;
   SoundPlay(soundCurrent.id,&settings);Sleep(13);SoundPlay(soundCurrent.id,&settings);
  }
 }
 SoundFlush(10000);
} }
}














