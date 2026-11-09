// Palette pulse remapping; object boundary is inferred from its private remap data.
#pragma pack(1)
struct PaletteActor { char pad00[0xc]; unsigned short type; char pad0e[0x14]; int visibility; char pad26[0x11]; unsigned char phase; char pad38[2]; signed char palette; };
#pragma pack()
struct ResMgr { void *Load(const char *,int,void *,unsigned long); };
extern ResMgr gResMgr;
extern unsigned char *gDrawRemap;
extern unsigned char worldColors[];
extern unsigned char *simActorVisibility;
extern unsigned int pulseColors[], waveColors[], flashColors[], rampColors[];
unsigned char actorRemaps[8][256];
unsigned int *actorRemap;
unsigned int actorRemapBlack;
int actorRemapFrame;
inline int ActorIsHidden(int index) { return !simActorVisibility[index]; }
void ActorPalette(PaletteActor *actor)
{
    gDrawRemap=0;
    actorRemap=(unsigned int *)actorRemaps[actor->palette & 7];
    int phase;
    switch(actor->type) {
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
        phase=(actor->phase+actorRemapFrame)%32;
        actorRemap[0]=phase<=24 ? pulseColors[0] : pulseColors[3];
        actorRemap[1]=flashColors[phase==0];
        actorRemap[2]=phase<=15 ? waveColors[0] : actorRemapBlack;
        actorRemap[3]=phase>15 ? actorRemapBlack : waveColors[0];
        gDrawRemap=(unsigned char *)actorRemap; return;
    case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17:
        phase=(actor->phase+actorRemapFrame)%32;
        actorRemap[0]=phase>=16 ? waveColors[3] : waveColors[0];
        actorRemap[1]=waveColors[phase/8];
        actorRemap[2]=phase<=15 ? waveColors[3] : waveColors[0];
        actorRemap[3]=phase ? actorRemapBlack : waveColors[0];
        gDrawRemap=(unsigned char *)actorRemap; return;
    case 18: case 19: case 20: case 21: case 22: case 23: case 24: case 25: case 32: case 33:
        phase=(actor->phase+actorRemapFrame)%20;
        actorRemap[0]=phase>=4 ? actorRemapBlack : waveColors[phase];
        actorRemap[1]=rampColors[phase];
        actorRemap[2]=phase>=4 ? actorRemapBlack : waveColors[3-phase];
        actorRemap[3]=waveColors[0];
        gDrawRemap=(unsigned char *)actorRemap; return;
    case 26: case 27: case 28: case 29: case 30:
        phase=(actor->phase+actorRemapFrame)%32;
        actorRemap[0]=phase>=16 ? pulseColors[3] : pulseColors[0];
        actorRemap[1]=phase<=15 ? waveColors[3] : waveColors[0];
        actorRemap[2]=phase==24 ? actorRemapBlack : waveColors[0];
        actorRemap[3]=flashColors[0];
        gDrawRemap=(unsigned char *)actorRemap; return;
    case 31:
        phase=(actor->phase+actorRemapFrame)%64;
        actorRemap[3]=phase>=6 ? pulseColors[3] : pulseColors[0];
        actorRemap[2]=phase>=58 ? pulseColors[0] : pulseColors[3];
        gDrawRemap=(unsigned char *)actorRemap; return;
    default:
        if(actor->type>33 && ActorIsHidden(actor->visibility)) gDrawRemap=worldColors;
        return;
    }
}
void ActorPaletteLoad() { gResMgr.Load("MpRemap.dat",1,actorRemaps,0x800); }
