/* Shell audio settings. Names inherited from callers are provisional. */
typedef unsigned long U32;
struct AudioSettings { int enabled, volume, pan, priority; unsigned sound; };
struct Timer { U32 start,end,duration; Timer(U32); };
void TimerSet(Timer *,U32); int TimerExpired(Timer *);
inline Timer::Timer(U32 n) { TimerSet(this,n); }
extern int audioVolume, audioDisabled;
extern char gShellResources[];
extern "C" void f_d2c0(void *);
extern "C" void f_d2f0(void *,int);
extern "C" int f_d270(void *);
void SoundStop(unsigned); void SoundRelease(unsigned);
extern "C" int LogicCallMode(int,void *);
int SoundFinished(unsigned); void SoundReleaseAll();

struct AudioModule { ~AudioModule(); };
struct AudioModuleTag { AudioModuleTag() {} };
AudioModuleTag audioModuleTag;
AudioModule::~AudioModule() {}

void ShellMultiplayerInit(AudioSettings *s) {
    s->enabled=1; s->volume=100; s->priority=85; s->pan=0; s->sound=1999;
}
void f_0041cab0(AudioSettings *);
extern "C" void HercSetMissionStart(AudioSettings *s,int sound) {
    if(audioVolume>=50 && audioDisabled!=1) {
        unsigned old=s->sound;
        if(old!=1999) {
            if(old!=1999) {
                if(old<2000) f_d2c0(gShellResources);
                else { old-=2000; SoundStop(old); SoundRelease(old); }
                s->sound=1999;
            }
        }
        if(sound<2000) { f_d2f0(gShellResources,sound); s->sound=sound; }
        else if(LogicCallMode(sound-2000,s)) s->sound=sound;
        else s->sound=1999;
    }
}
void PilotManagerPoll(void *p) {
    AudioSettings *s=(AudioSettings *)p;
    static Timer timer(0);
    if(s->sound!=1999 && audioDisabled!=1) {
        if((int)s->sound<2000) {
            if(TimerExpired(&timer)) {
                TimerSet(&timer,8000);
                if(f_d270(gShellResources)) f_d2f0(gShellResources,s->sound-2000);
            }
        } else if(TimerExpired(&timer)) {
            TimerSet(&timer,8000);
            if(SoundFinished(s->sound-2000)) {
                AudioSettings *target=s;
                int next=s->sound;
                if(audioVolume>=50 && audioDisabled!=1) {
                    if(next!=1999) {
                        if(next!=1999) {
                            if((unsigned)next<2000) f_d2c0(gShellResources);
                            else { unsigned old=next-2000; SoundStop(old); SoundRelease(old); }
                            s->sound=1999;
                        }
                    }
                    if(next<2000){f_d2f0(gShellResources,next);target->sound=next;}
                    else if(LogicCallMode(next-2000,target))target->sound=next;
                    else target->sound=1999;
                }
            }
        }
    }
}
void f_0041cab0(AudioSettings *s) {
    if(s->sound!=1999) {
        unsigned sound=s->sound;
        if(sound<2000) f_d2c0(gShellResources);
        else { SoundStop(sound-2000); SoundRelease(sound-2000); }
        s->sound=1999;
    }
}
extern "C" void HercSetMission(void *p,unsigned sound) {
    AudioSettings *s=(AudioSettings *)p;
    if(sound<2000) f_d2c0(gShellResources);
    else { SoundStop(sound-2000); SoundRelease(sound-2000); }
    s->sound=1999;
}
void ShellMultiplayerShutdown(void *p) {
    SoundReleaseAll();
    AudioSettings *s=(AudioSettings *)p; s->sound=1999;
}


