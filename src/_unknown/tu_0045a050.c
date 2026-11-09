// Sound-event descriptor selection from the current actor animation.
#pragma pack(1)
struct SoundDescriptor { short id; unsigned char pitch,pitchRange,volume,volumeRange,at06,frame; };
struct Animation { int data; unsigned char kind,state,phase,at07,frame,count; char pad0a[2]; unsigned short type; };
#pragma pack()
extern unsigned long animationKindTable[];
extern signed char animationSoundStates[];
extern int animationSoundMode;
inline signed char AnimationSoundIndex(int state) { return animationSoundStates[state]; }
SoundDescriptor *AnimSoundDescriptor(Animation *anim)
{
    SoundDescriptor *sounds=(SoundDescriptor *)animationKindTable[anim->type*15+14];
    if (!sounds) return 0;
    if (anim->state==4) {
        if (animationSoundMode>=2) return 0;
        SoundDescriptor *sound=sounds+7;
        for(int i=0;i<6;i++,sound++) {
            if (!sound->id) break;
            if (anim->frame==sound->frame*anim->count/48) return sound;
        }
        return 0;
    }
    int index=AnimationSoundIndex(anim->state);
    if(index<0) return 0;
    SoundDescriptor *sound=sounds+index;
    if(anim->frame==sound->frame)return sound;
    return 0;
}
