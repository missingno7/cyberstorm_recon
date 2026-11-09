// Sound descriptor playback; labels and field meanings are provisional.
#pragma pack(1)
struct ExploPoint { int x, y; };
struct ApplyPoint { int x, y; };
struct SoundDescriptor { short id; unsigned char pitch, pitchRange, volume, volumeRange; char pad06[2]; };
#pragma pack()
struct SimSound { int pan, pitch, flags, volume; };
struct SoundSettings { int pan, pitch, flags, volume; };
extern void SimSoundSetup(SimSound *, ExploPoint *);
extern int SoundPlay(unsigned int, const SoundSettings *);
extern "C" unsigned int next_link_seed(unsigned int *);
extern unsigned int gFxSeed;
static inline int Clamp(int value, int low, int high)
{
    if (value <= low) return low;
    if (value >= high) return high;
    return value;
}
inline void RandomSpread(int &value, int radius) {
    int low = value - radius;
    unsigned int range = radius * 2 + 1;
    value = low + next_link_seed(&gFxSeed) % range;
}
void SoundAtPoint(ExploPoint *at, SoundDescriptor *s)
{
    SimSound settings;
    if (s) {
        settings.volume = s->volume;
        settings.pitch = s->pitch;
        SimSoundSetup(&settings, at);
        RandomSpread(settings.volume, s->volumeRange);
        settings.volume = Clamp(settings.volume, 0, 100);
        RandomSpread(settings.pitch, s->pitchRange);
        settings.pitch = Clamp(settings.pitch, 50, 150);
        SoundPlay(s->id, (const SoundSettings *)&settings);
    }
}
void WeaponSound(ApplyPoint *at, SoundDescriptor *s, int kind, int value, int total)
{
    SimSound settings;
    if (s) {
        SoundDescriptor *entry=&s[kind]; int volumeRange=entry->volumeRange; int pitchRange=entry->pitchRange;
        settings.volume = s->volume - volumeRange;
        settings.pitch = s->pitch + pitchRange;
        settings.flags = 0;
        if (kind == 0) settings.volume = settings.volume - volumeRange + total;
        if (total > 0) {
            settings.volume += (value / total) * volumeRange;
            settings.pitch -= volumeRange - total;
        }
        SimSoundSetup(&settings, (ExploPoint *)at);
        settings.volume = Clamp(settings.volume, 100 - volumeRange, 100);
        settings.pitch = Clamp(settings.pitch, 100 - pitchRange, 100 + pitchRange);
        SoundPlay(s[kind].id, (const SoundSettings *)&settings);
    }
}

