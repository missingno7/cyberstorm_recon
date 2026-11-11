// Sound descriptor playback; labels and field meanings are provisional.
#pragma pack(1)
struct ExploPoint { int x, y; };
struct ApplyPoint { int x, y; };
struct SoundDescriptor { short id; unsigned char pitch, pitchRange, volume, volumeRange; char pad06[2]; };
#pragma pack()
static char weapfxSourceFile[]=__FILE__;
struct SpeedProfile { int speed, acceleration, maximum, minimum; };
SpeedProfile trajectorySpeedProfiles[]={
    {1000,0,999999,500},
    {2000,0,999999,500},
    {500,0,999999,500},
    {50,25,750,50},
    {1500,-200,999999,750},
    {20000,0,999999,500},
    {20000,0,999999,500},
    {100,0,999999,100},
    {100,10,1000,50},
    {75,15,1000,50},
    {75,20,1500,50},
    {75,25,2000,50},
};
#pragma pack(push,1)
// All effect record widths follow the kind tag and the independently referenced record starts.
// Field names beyond the common projectile parameters remain provisional.
struct BeamEffect { unsigned char kind,animation,count,delay,repeatSound,speed,at06,at07,arc,color,width,step; };
struct RailEffect { BeamEffect common; unsigned char at0c,at0d,at0e,at0f,at10,at11,at12,at13; };
struct BurstEffect { BeamEffect common; unsigned char at0c,at0d,at0e,at0f,at10,at11,at12,at13; };
struct SprayEffect { RailEffect common; unsigned char at14,at15,at16,at17; };
#pragma pack(pop)
static BeamEffect weaponEffect_004a27a0={0,0,1,0,0,1,5,0,0,2,1,8};
static BeamEffect weaponEffect_004a27ac={0,0,1,0,0,1,5,0,0,2,1,8};
static BeamEffect weaponEffect_004a27b8={0,0,1,0,0,2,5,0,0,0,1,8};
static BeamEffect weaponEffect_004a27c4={0,0,1,0,0,3,5,0,0,1,1,8};
static BeamEffect weaponEffect_004a27d0={0,0,1,0,0,2,5,0,0,14,2,8};
static BeamEffect weaponEffect_004a27dc={0,0,1,0,0,2,5,0,0,6,1,8};
static BeamEffect weaponEffect_004a27e8={0,0,1,0,0,2,5,0,0,4,1,8};
static BeamEffect weaponEffect_004a27f4={0,0,1,0,0,2,5,0,0,5,1,8};
static BeamEffect weaponEffect_004a2800={0,0,1,0,0,2,5,0,0,1,3,8};
static BeamEffect weaponEffect_004a280c={0,0,2,0,1,2,5,0,0,14,1,8};
static BeamEffect weaponEffect_004a2818={0,0,1,0,0,2,5,0,0,14,2,8};
static BeamEffect weaponEffect_004a2824={0,0,1,2,0,1,5,0,0,13,2,8};
static BeamEffect weaponEffect_004a2830={0,0,1,3,0,1,5,0,0,13,2,8};
static BeamEffect weaponEffect_004a283c={0,0,1,4,0,1,5,0,0,13,2,8};
static BeamEffect weaponEffect_004a2848={0,0,1,5,0,1,5,0,0,13,3,8};
static BeamEffect weaponEffect_004a2854={0,0,1,0,0,2,5,0,0,10,2,8};
static BeamEffect weaponEffect_004a2860={0,0,1,0,0,2,5,0,0,9,2,8};
static BeamEffect weaponEffect_004a286c={0,0,1,0,0,2,2,0,0,14,5,8};
static RailEffect weaponEffect_004a2878={{1,0,1,0,0,2,6,0,0,10,1,0},10,3,150,0,0,0,0,0};
static RailEffect weaponEffect_004a288c={{1,0,1,0,0,2,6,0,0,8,1,0},15,5,200,0,0,0,0,0};
static RailEffect weaponEffect_004a28a0={{1,0,1,0,0,2,6,0,0,9,1,0},20,7,250,0,0,0,0,0};
static RailEffect weaponEffect_004a28b4={{1,0,1,0,0,2,6,0,0,10,1,0},10,3,100,0,0,0,0,0};
static RailEffect weaponEffect_004a28c8={{1,0,1,0,0,1,6,0,0,8,1,0},13,3,100,0,0,0,0,0};
static SprayEffect weaponEffect_004a28dc={{{2,0,1,0,0,2,0,1,127,0,50,0},50,0,50,0,0,0,0,0},0,0,0,0};
static SprayEffect weaponEffect_004a28f4={{{2,0,1,0,0,2,0,1,10,0,0,60},15,51,12,2,0,0,0,0},0,0,0,0};
static SprayEffect weaponEffect_004a290c={{{2,0,1,0,0,2,0,1,10,0,0,59},20,51,15,1,0,0,0,0},0,0,0,0};
static SprayEffect weaponEffect_004a2924={{{2,1,1,0,0,2,0,1,10,0,0,57},50,51,30,1,0,0,0,0},0,0,0,0};
static SprayEffect weaponEffect_004a293c={{{2,0,1,0,0,2,0,1,10,0,0,58},30,51,15,1,0,0,0,0},0,0,0,0};
static SprayEffect weaponEffect_004a2954={{{2,2,1,0,0,2,0,1,20,63,35,57},60,51,50,1,0,0,0,0},0,0,0,0};
static SprayEffect weaponEffect_004a296c={{{2,2,1,0,0,2,0,1,10,63,20,58},60,51,50,1,0,0,0,0},0,0,0,0};
static SprayEffect weaponEffect_004a2984={{{2,1,1,0,0,2,2,1,10,31,60,58},20,51,25,1,0,0,0,0},0,0,0,0};
static SprayEffect weaponEffect_004a299c={{{2,0,1,0,0,2,0,1,10,0,0,58},20,51,25,1,0,0,0,0},0,0,0,0};
static SprayEffect weaponEffect_004a29b4={{{2,2,1,0,0,2,0,1,20,63,35,57},70,51,50,1,0,0,0,0},0,0,0,0};
static SprayEffect weaponEffect_004a29cc={{{2,0,3,4,1,2,8,1,10,35,50,0},0,0,0,0,0,0,0,0},0,0,0,0};
static SprayEffect weaponEffect_004a29e4={{{2,0,3,5,1,2,8,1,10,33,40,0},0,0,0,0,0,0,0,0},0,0,0,0};
static SprayEffect weaponEffect_004a29fc={{{2,3,3,3,1,2,3,1,10,35,200,57},100,51,100,1,0,0,0,0},0,0,0,0};
static BurstEffect weaponEffect_004a2a14={{3,0,1,0,0,2,9,1,50,0,44,140},61,30,5,255,255,0,0,0};
static BurstEffect weaponEffect_004a2a28={{3,0,1,0,0,2,11,1,50,0,44,150},61,30,5,255,255,0,0,0};
static BurstEffect weaponEffect_004a2a3c={{3,0,1,0,0,2,11,1,50,0,46,140},61,30,5,255,255,0,0,0};
static BurstEffect weaponEffect_004a2a50={{3,0,1,0,0,2,11,0,5,0,40,40},0,0,0,0,0,0,0,0};
static BurstEffect weaponEffect_004a2a64={{3,0,1,0,0,2,11,1,50,0,43,160},61,30,5,255,255,0,0,0};
static BurstEffect weaponEffect_004a2a78={{3,0,1,0,0,2,11,1,50,0,45,140},61,30,5,255,255,0,0,0};
static BurstEffect weaponEffect_004a2a8c={{3,0,3,3,1,2,11,0,15,0,46,60},61,20,5,255,255,0,0,0};
static BurstEffect weaponEffect_004a2aa0={{3,0,1,3,0,2,11,0,5,0,38,80},0,0,0,0,0,0,0,0};
static BurstEffect weaponEffect_004a2ab4={{3,0,1,3,0,2,11,0,5,0,38,120},0,0,0,0,0,0,0,0};
static BurstEffect weaponEffect_004a2ac8={{3,0,3,4,1,2,11,1,50,0,44,140},0,0,0,0,0,0,0,0};
static BurstEffect weaponEffect_004a2adc={{3,0,5,4,1,2,11,1,50,0,44,140},61,30,5,255,255,0,0,0};
static BurstEffect weaponEffect_004a2af0={{3,0,1,0,0,2,11,1,50,0,42,30},0,0,0,0,0,0,0,0};
static BurstEffect weaponEffect_004a2b04={{3,0,1,0,0,2,11,1,50,0,44,140},61,30,5,255,255,0,0,0};
static BurstEffect weaponEffect_004a2b18={{3,0,1,0,0,2,9,1,50,0,44,140},61,30,5,255,255,0,0,0};
static BurstEffect weaponEffect_004a2b2c={{3,0,1,0,0,2,11,1,50,0,44,140},61,30,5,255,255,0,0,0};
static BurstEffect weaponEffect_004a2b40={{3,0,1,0,0,2,7,1,10,0,45,180},61,30,3,255,255,0,0,0};
static BurstEffect weaponEffect_004a2b54={{3,0,3,4,1,2,11,1,50,0,44,140},61,30,5,255,255,0,0,0};
static BurstEffect weaponEffect_004a2b68={{3,0,3,4,1,2,8,1,50,0,44,140},61,30,5,255,255,0,0,0};
static BurstEffect weaponEffect_004a2b7c={{3,1,1,0,0,2,8,1,50,0,39,140},0,0,0,0,0,0,0,0};
static BurstEffect weaponEffect_004a2b90={{3,0,4,2,1,1,0,0,0,0,50,140},0,0,0,0,0,0,0,0};
static BurstEffect weaponEffect_004a2ba4={{3,0,3,4,1,2,2,1,2,0,47,100},0,0,0,0,0,0,0,0};
static BurstEffect weaponEffect_004a2bb8={{3,0,3,5,1,2,2,1,2,0,47,100},0,0,0,0,0,0,0,0};
static BurstEffect weaponEffect_004a2bcc={{3,0,3,6,1,2,2,1,3,0,47,100},0,0,0,0,0,0,0,0};
static BurstEffect weaponEffect_004a2be0={{3,0,1,0,0,2,11,0,0,0,32,50},0,30,5,255,255,0,0,0};
static BurstEffect weaponEffect_004a2bf4={{3,0,1,0,0,2,9,0,0,0,32,90},0,30,5,255,255,0,0,0};
static BurstEffect weaponEffect_004a2c08={{3,0,1,0,0,2,11,0,0,0,41,100},0,0,0,0,0,0,0,0};
static BurstEffect weaponEffect_004a2c1c={{3,0,1,0,0,2,11,0,0,0,41,120},0,0,0,0,0,0,0,0};
static BurstEffect weaponEffect_004a2c30={{3,0,1,0,0,2,11,0,0,0,41,155},0,0,0,0,0,0,0,0};
static BurstEffect weaponEffect_004a2c44={{3,0,5,2,1,2,11,0,0,0,41,35},0,0,0,0,0,0,0,0};
struct FxDef;
FxDef *fxDefs[]={
    (FxDef *)&weaponEffect_004a27a0,
    (FxDef *)&weaponEffect_004a27ac,
    (FxDef *)&weaponEffect_004a27b8,
    (FxDef *)&weaponEffect_004a27c4,
    (FxDef *)&weaponEffect_004a2a14,
    (FxDef *)&weaponEffect_004a2a28,
    (FxDef *)&weaponEffect_004a2a3c,
    (FxDef *)&weaponEffect_004a2878,
    (FxDef *)&weaponEffect_004a288c,
    (FxDef *)&weaponEffect_004a28a0,
    (FxDef *)&weaponEffect_004a2be0,
    (FxDef *)&weaponEffect_004a28f4,
    (FxDef *)&weaponEffect_004a290c,
    (FxDef *)&weaponEffect_004a2924,
    (FxDef *)&weaponEffect_004a293c,
    (FxDef *)&weaponEffect_004a2954,
    (FxDef *)&weaponEffect_004a296c,
    (FxDef *)&weaponEffect_004a2984,
    (FxDef *)&weaponEffect_004a2a78,
    (FxDef *)&weaponEffect_004a2a50,
    (FxDef *)&weaponEffect_004a27d0,
    (FxDef *)&weaponEffect_004a27dc,
    (FxDef *)&weaponEffect_004a27e8,
    (FxDef *)&weaponEffect_004a27f4,
    (FxDef *)&weaponEffect_004a2a8c,
    (FxDef *)&weaponEffect_004a2aa0,
    (FxDef *)&weaponEffect_004a2ab4,
    (FxDef *)&weaponEffect_004a2a64,
    (FxDef *)&weaponEffect_004a2800,
    (FxDef *)&weaponEffect_004a293c,
    (FxDef *)&weaponEffect_004a2b54,
    (FxDef *)&weaponEffect_004a2af0,
    (FxDef *)&weaponEffect_004a28b4,
    (FxDef *)&weaponEffect_004a29b4,
    (FxDef *)&weaponEffect_004a2824,
    (FxDef *)&weaponEffect_004a2830,
    (FxDef *)&weaponEffect_004a283c,
    (FxDef *)&weaponEffect_004a2848,
    (FxDef *)&weaponEffect_004a280c,
    (FxDef *)&weaponEffect_004a2818,
    (FxDef *)&weaponEffect_004a2ac8,
    (FxDef *)&weaponEffect_004a2b04,
    (FxDef *)&weaponEffect_004a2ba4,
    (FxDef *)&weaponEffect_004a2bb8,
    (FxDef *)&weaponEffect_004a2bcc,
    (FxDef *)&weaponEffect_004a29cc,
    (FxDef *)&weaponEffect_004a2bb8,
    (FxDef *)&weaponEffect_004a2b2c,
    (FxDef *)&weaponEffect_004a2aa0,
    (FxDef *)&weaponEffect_004a2984,
    (FxDef *)&weaponEffect_004a28c8,
    (FxDef *)&weaponEffect_004a286c,
    (FxDef *)&weaponEffect_004a2b90,
    (FxDef *)&weaponEffect_004a2c08,
    (FxDef *)&weaponEffect_004a2c1c,
    (FxDef *)&weaponEffect_004a2c30,
    (FxDef *)&weaponEffect_004a29e4,
    (FxDef *)&weaponEffect_004a2ba4,
    (FxDef *)&weaponEffect_004a2b18,
    (FxDef *)&weaponEffect_004a2bf4,
    (FxDef *)&weaponEffect_004a2adc,
    (FxDef *)&weaponEffect_004a2984,
    (FxDef *)&weaponEffect_004a2b7c,
    (FxDef *)&weaponEffect_004a2854,
    (FxDef *)&weaponEffect_004a2860,
    (FxDef *)&weaponEffect_004a2b40,
    (FxDef *)&weaponEffect_004a2b68,
    (FxDef *)&weaponEffect_004a2c44,
    (FxDef *)&weaponEffect_004a2a50,
    (FxDef *)&weaponEffect_004a29fc,
    (FxDef *)&weaponEffect_004a27a0,
    (FxDef *)&weaponEffect_004a27a0,
    (FxDef *)&weaponEffect_004a27a0,
    (FxDef *)&weaponEffect_004a27a0,
    (FxDef *)&weaponEffect_004a27a0,
    (FxDef *)&weaponEffect_004a2be0,
};
// Four sound settings per weapon bank: indexed by impact kind.
static SoundDescriptor weaponSoundBanks[43][4]={
    {{41,100,40,100,20,{0,0}},{381,100,40,100,40,{0,0}},{378,100,40,100,40,{0,0}},{44,100,40,100,50,{0,0}}},
    {{22,100,40,100,40,{0,0}},{363,100,40,100,70,{0,0}},{26,100,40,100,45,{0,0}},{376,100,40,100,50,{0,0}}},
    {{22,100,40,100,40,{0,0}},{363,100,40,100,70,{0,0}},{26,100,40,100,45,{0,0}},{376,100,40,100,50,{0,0}}},
    {{22,100,40,100,40,{0,0}},{363,100,40,60,40,{8,0}},{26,100,40,70,30,{8,0}},{266,100,40,100,50,{0,0}}},
    {{271,100,40,100,40,{0,0}},{363,100,40,100,70,{0,0}},{26,100,40,100,55,{0,0}},{376,100,40,100,50,{0,0}}},
    {{235,100,50,100,50,{0,0}},{0,100,40,100,30,{0,0}},{264,100,50,100,50,{0,0}},{266,100,40,100,50,{0,0}}},
    {{270,100,60,100,55,{0,0}},{43,100,60,100,55,{0,0}},{0,100,40,100,50,{0,0}},{266,100,40,100,50,{0,0}}},
    {{401,120,40,100,45,{0,0}},{363,120,40,100,65,{0,0}},{356,120,40,100,45,{0,0}},{266,100,40,100,50,{0,0}}},
    {{374,100,40,100,40,{2,0}},{363,100,40,100,63,{0,0}},{356,100,40,100,45,{0,0}},{266,100,40,100,50,{0,0}}},
    {{36,100,50,100,35,{0,0}},{363,100,50,100,58,{0,0}},{380,100,50,100,45,{0,0}},{266,100,40,100,50,{0,0}}},
    {{36,80,55,100,20,{0,0}},{363,100,55,100,40,{0,0}},{402,100,55,100,80,{0,0}},{266,100,55,100,50,{0,0}}},
    {{36,70,55,100,20,{0,0}},{43,100,55,100,60,{0,0}},{403,100,55,100,60,{0,0}},{266,100,40,100,50,{0,0}}},
    {{269,100,40,100,50,{0,0}},{269,100,40,100,50,{0,0}},{25,100,40,100,75,{0,0}},{266,100,40,100,50,{0,0}}},
    {{357,100,40,100,50,{0,0}},{381,100,40,100,50,{0,0}},{25,80,40,80,60,{0,0}},{44,100,40,100,50,{0,0}}},
    {{41,120,40,100,60,{0,0}},{381,120,40,100,60,{0,0}},{378,120,40,100,60,{0,0}},{44,100,40,100,50,{0,0}}},
    {{41,120,40,100,60,{0,0}},{381,120,40,100,60,{0,0}},{378,120,40,100,60,{0,0}},{44,100,40,100,50,{0,0}}},
    {{41,100,40,100,50,{0,0}},{381,100,40,100,50,{0,0}},{378,100,40,100,50,{0,0}},{44,100,40,100,50,{0,0}}},
    {{41,80,40,100,40,{0,0}},{381,80,40,100,40,{0,0}},{378,80,40,100,40,{0,0}},{44,100,40,100,50,{0,0}}},
    {{22,130,40,100,60,{0,0}},{363,130,40,100,70,{0,0}},{26,130,40,100,65,{0,0}},{376,130,40,100,50,{0,0}}},
    {{23,100,40,100,30,{0,0}},{365,100,40,80,50,{0,0}},{379,100,40,80,50,{0,0}},{266,100,40,100,50,{0,0}}},
    {{41,80,60,100,30,{0,0}},{381,100,60,100,30,{0,0}},{378,100,60,100,30,{0,0}},{44,100,40,100,50,{0,0}}},
    {{267,100,40,100,50,{0,0}},{269,100,60,100,60,{0,0}},{269,100,60,100,60,{0,0}},{266,100,40,100,50,{0,0}}},
    {{267,100,40,100,40,{0,0}},{381,80,40,100,40,{0,0}},{379,80,40,100,40,{0,0}},{44,100,40,100,50,{0,0}}},
    {{267,100,50,100,60,{0,0}},{381,80,50,100,60,{0,0}},{379,80,50,100,60,{0,0}},{44,100,40,100,50,{0,0}}},
    {{269,100,40,100,40,{0,0}},{381,100,40,100,40,{0,0}},{37,100,40,100,50,{0,0}},{266,100,40,100,50,{0,0}}},
    {{36,100,20,100,20,{0,0}},{363,100,50,100,50,{0,0}},{356,100,60,100,30,{0,0}},{266,100,40,100,50,{0,0}}},
    {{36,100,20,100,20,{0,0}},{363,100,50,100,50,{0,0}},{264,100,60,100,30,{0,0}},{266,100,40,100,50,{0,0}}},
    {{359,100,40,100,50,{0,0}},{269,100,40,100,50,{0,0}},{404,100,40,100,50,{0,0}},{266,100,40,100,50,{0,0}}},
    {{267,100,40,100,50,{0,0}},{269,100,40,0,1,{0,0}},{404,100,40,100,50,{0,0}},{266,100,40,100,50,{0,0}}},
    {{244,100,40,100,30,{0,0}},{350,100,40,100,30,{0,0}},{403,100,40,100,30,{0,0}},{266,100,40,100,50,{0,0}}},
    {{243,80,40,100,30,{0,0}},{350,80,40,100,30,{0,0}},{403,80,40,100,30,{0,0}},{266,100,40,100,50,{0,0}}},
    {{269,100,40,100,40,{0,0}},{381,100,40,100,40,{0,0}},{37,100,40,100,50,{0,0}},{266,100,40,100,50,{0,0}}},
    {{22,100,40,100,40,{0,0}},{363,100,40,60,40,{2,0}},{26,100,40,70,30,{2,0}},{266,100,40,100,50,{0,0}}},
    {{270,100,60,100,75,{0,0}},{43,100,60,100,45,{0,0}},{0,100,40,100,50,{0,0}},{266,100,40,100,50,{0,0}}},
    {{23,100,40,100,50,{0,0}},{365,100,40,100,50,{0,0}},{269,100,40,100,50,{0,0}},{266,100,40,100,50,{0,0}}},
    {{380,100,40,100,50,{0,0}},{269,100,40,100,50,{0,0}},{36,100,40,100,50,{0,0}},{266,100,40,100,50,{0,0}}},
    {{243,100,40,60,40,{0,0}},{43,100,40,60,40,{0,0}},{25,100,40,100,50,{0,0}},{266,100,40,100,50,{0,0}}},
    {{400,100,40,100,50,{0,0}},{269,100,40,100,50,{0,0}},{269,100,40,100,50,{0,0}},{266,100,40,100,50,{0,0}}},
    {{404,100,40,100,50,{0,0}},{404,100,40,100,50,{0,0}},{404,100,40,100,50,{0,0}},{404,100,40,100,50,{0,0}}},
    {{409,100,40,0,0,{0,0}},{409,100,40,100,50,{0,0}},{409,100,40,100,50,{0,0}},{409,100,40,100,50,{0,0}}},
    {{411,100,40,0,0,{0,0}},{411,100,40,100,50,{0,0}},{411,100,40,100,50,{0,0}},{411,100,40,100,50,{0,0}}},
    {{412,100,40,0,0,{0,0}},{412,100,40,100,50,{0,0}},{412,100,40,100,50,{0,0}},{412,100,40,100,50,{0,0}}},
    {{413,100,40,0,0,{0,0}},{413,100,40,100,50,{0,0}},{413,100,40,100,50,{0,0}},{413,100,40,100,50,{0,0}}},
};
SoundDescriptor *weaponSoundTable[]={
    0,
    weaponSoundBanks[0],
    weaponSoundBanks[0],
    weaponSoundBanks[0],
    weaponSoundBanks[1],
    weaponSoundBanks[2],
    weaponSoundBanks[4],
    weaponSoundBanks[5],
    weaponSoundBanks[5],
    weaponSoundBanks[5],
    weaponSoundBanks[33],
    weaponSoundBanks[7],
    weaponSoundBanks[8],
    weaponSoundBanks[9],
    weaponSoundBanks[7],
    weaponSoundBanks[10],
    weaponSoundBanks[10],
    weaponSoundBanks[11],
    weaponSoundBanks[4],
    weaponSoundBanks[12],
    weaponSoundBanks[23],
    weaponSoundBanks[0],
    weaponSoundBanks[0],
    weaponSoundBanks[0],
    weaponSoundBanks[18],
    weaponSoundBanks[19],
    weaponSoundBanks[19],
    weaponSoundBanks[1],
    weaponSoundBanks[0],
    weaponSoundBanks[7],
    weaponSoundBanks[4],
    weaponSoundBanks[35],
    weaponSoundBanks[5],
    weaponSoundBanks[10],
    weaponSoundBanks[21],
    weaponSoundBanks[21],
    weaponSoundBanks[21],
    weaponSoundBanks[21],
    weaponSoundBanks[22],
    weaponSoundBanks[23],
    weaponSoundBanks[4],
    weaponSoundBanks[2],
    weaponSoundBanks[24],
    weaponSoundBanks[24],
    weaponSoundBanks[24],
    weaponSoundBanks[25],
    weaponSoundBanks[31],
    weaponSoundBanks[1],
    weaponSoundBanks[34],
    weaponSoundBanks[11],
    weaponSoundBanks[5],
    weaponSoundBanks[27],
    weaponSoundBanks[28],
    weaponSoundBanks[34],
    weaponSoundBanks[34],
    weaponSoundBanks[34],
    weaponSoundBanks[25],
    weaponSoundBanks[31],
    weaponSoundBanks[32],
    weaponSoundBanks[33],
    weaponSoundBanks[4],
    weaponSoundBanks[36],
    weaponSoundBanks[36],
    weaponSoundBanks[0],
    weaponSoundBanks[0],
    weaponSoundBanks[4],
    weaponSoundBanks[4],
    weaponSoundBanks[34],
    weaponSoundBanks[29],
    weaponSoundBanks[30],
    weaponSoundBanks[39],
    weaponSoundBanks[40],
    weaponSoundBanks[41],
    weaponSoundBanks[42],
    weaponSoundBanks[42],
    weaponSoundBanks[33],
};
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

