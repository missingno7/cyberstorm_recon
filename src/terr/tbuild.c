/* terr\tbuild.c. Terrain generation; working type and function names. */ extern "C" void *memcpy(void *,const void *,unsigned); extern "C" void *memset(void *,int,unsigned);
#include <stdio.h>
typedef unsigned char U8;
struct Point { int x,y; void Add(Point *p){x+=p->x;y+=p->y;} };
struct Cell { U8 terrain,height,feature,featureKind,overlay,overlayKind,rotation,variant; U8 unknown[4]; U8 Type(){return terrain;} };
struct TerrainGrid { int fields[2]; int radius; int pitch; Point center; int layout[4]; int count,waterCount; int rest[5]; Cell *cells; };
struct Noise { char body[444]; Noise(); ~Noise(); };
struct Search { int words[10]; };
struct Visit { TerrainGrid *grid; Point point; int index; };
struct Range { int lo,hi; Range(){} Range(int l,int h){lo=l;hi=h;} }; inline int TerrainMin(int a,int b){return a<b?a:b;} inline int TerrainMax(int a,int b){return a>b?a:b;}
extern "C" void AssertFail(const char *,int,const char *);

extern "C" void NoiseConstruct(Noise *);
extern "C" void NoiseDestroy(Noise *,int);
extern "C" void NoiseClear(Noise *);
extern "C" void NoiseGenerate(Noise *,int);
extern "C" void NoiseSmooth(Noise *);
extern "C" U8 NoiseGet(Noise *,Point *);
extern "C" void NoiseSet(Noise *,Point *,U8);
extern "C" unsigned NoiseFill(Noise *,int,int);
extern "C" int LogicCopyPoint(Point *,Point *);
extern void BestLocSetData(void *,void *,void *);
extern int BestLocInitialize(void *,const char *,int);
extern int BestLocSetCandidate(void *,void *);
extern void BestLocVisit(void *,int (*)(void *));
extern "C" unsigned next_link_seed(unsigned *);
extern "C" int RandomRange(unsigned *,int,int);
extern "C" int ResolveName(TerrainGrid *,Point *);
extern void GridClearNeighbors(TerrainGrid *);
extern "C" int TerrainWater(TerrainGrid *,int);
extern "C" void TerrainRaise(TerrainGrid *);
extern int WorldActionAdd(void *,int,int);
extern "C" void f_cb80(void *);
extern char terrainFilename[];
extern int terrainNumber,seasonCounts[];
Noise terrainNoise;
extern void *terrainImage;
extern int heightSum,heightCount;
extern Point heightCenter;
extern int heightTarget,heightInner,heightOuter,heightStrength;
extern unsigned MissionRandomTable;
extern int logicMode;
extern unsigned gFxSeed;

struct TerrainConfig { char prefix[10]; unsigned short flags; char rest[0x4c5-12]; unsigned short Flags() { return flags; } };
extern TerrainConfig g4c68b4;
#define terrainFlags g4c68b4.Flags()
extern U8 terrainWaterThreshold,terrainWaterBase;
struct FeatureChance { U8 chance,kind; }; struct TerrainType { U8 first,variants,unknown[4],flags; FeatureChance feature[16]; };
struct FeatureType { U8 base,type,unknown[6]; };
extern TerrainType terrainTypes[];
extern FeatureType featureTypes[];
extern U8 tripletDirections[][2];
extern Point pathDirections[];
struct ResMgr { void *Load(const char *,int=1,void * =0,unsigned long=0); int CloseFile(); };
extern ResMgr gResMgr;
extern void SimLoadPalette(int,void *);
extern char g4c9234;
extern U8 terrainTypeRemap[],terrainTypeOrder[],worldColors[],worldColorsBackup[],worldColorInfo[],worldColorExtras[],hexMask[];
inline U8 TerrainOrder(unsigned i){return terrainTypeOrder[i];} extern int heightStep,heightLevels[];
extern void *hexCursor;
extern "C" void PaletteRead(void *,void *,int,int);
extern "C" void PaletteCopyRange(void *,void *,int,int);
extern "C" void PaletteAttach(void *,void *);
extern "C" void PaletteBlend(void *,void *,int,int);
extern "C" int ResMgrOpen(void *,const char *,int);
extern "C" int f_c8a0(void *,int,int);
extern "C" int f_c900(void *,void *,int);
inline char *TerrainFilenameCore(void *owner,const char *extension)
{
    if (terrainNumber == 99) { sprintf(terrainFilename,"s99p99.%s",extension); return terrainFilename; }
    int season=0, number=terrainNumber;
    for (int i=0; number>=seasonCounts[i]; i++) { ++season; number-=seasonCounts[i]; }
    sprintf(terrainFilename,"s%dp%d.%s",season+1,number+1,extension);
    return terrainFilename;
}
char *TerrainFilename(void *owner,const char *extension)
{
    if (terrainNumber == 99) { sprintf(terrainFilename,"s99p99.%s",extension); return terrainFilename; }
    int season=0, number=terrainNumber;
    for (int i=0; number>=seasonCounts[i]; i++) { ++season; number-=seasonCounts[i]; }
    sprintf(terrainFilename,"s%dp%d.%s",season+1,number+1,extension);
    return terrainFilename;
}
void SimLoadImage(void *data,int number,void *palette)
{
    terrainNumber=number;
    SimLoadPalette(number,&g4c9234);
    memset(data,0,0x4c5);
    char *filename=TerrainFilenameCore(data,"dat");
    if(!gResMgr.Load(filename,1,data,0x4c5))
// Original ASSERT source-line padding.
        AssertFail(__FILE__,__LINE__,filename);
    memset(terrainTypeRemap,15,16);
    for(unsigned i=0;i<15;i++) if(terrainTypeRemap[TerrainOrder(i)]==15) terrainTypeRemap[TerrainOrder(i)]=i;
    heightStep=((U8 *)data)[18]*500/100;
    for(int j=0;j<8;j++) heightLevels[j]=j*heightStep;
    filename=TerrainFilenameCore(data,"plx");
    char colors[944];
    void *image=gResMgr.Load(filename);
    PaletteRead(image,colors,10,236);
    PaletteCopyRange(palette,colors,10,236);
    PaletteAttach(palette,(char *)data+90);
    PaletteBlend(palette,image,10,6);
    f_cb80(image);
    if(terrainImage) f_cb80(terrainImage);
    filename=TerrainFilenameCore(data,"bmx");
    terrainImage=gResMgr.Load(filename);
    ResMgrOpen(&gResMgr,"WORLDCOL.DAT",0);
    f_c8a0(&gResMgr,terrainNumber*0x4a5,0);
    f_c900(&gResMgr,worldColors,0x400);
    f_c900(&gResMgr,worldColorInfo,0x78);
    f_c900(&gResMgr,worldColorExtras,0x2d);
    gResMgr.CloseFile();
    memcpy(worldColorsBackup,worldColors,0x400);
    gResMgr.Load("hexmask.dat",1,hexMask,0xc00);
    if(!hexCursor) hexCursor=gResMgr.Load("hexcurs.bmx");
}

void TerrainReleaseImage() { if (terrainImage) f_cb80(terrainImage); terrainImage=0; }


int TerrainHeightRange(TerrainGrid *grid,Noise *noise,Range *range,int percent,unsigned mask=~0U,int overlay=0)
{
    int histogram[256];
    Point point;
    *range=Range(255,0);
    memset(histogram,0,sizeof(histogram));
    int count=0;
    Cell *cell=grid->cells+1;
    for(int i=1;i<grid->count;i++,cell++) {
        if(!(mask&(1<<cell->terrain)))continue;
        if(percent && !((!overlay&&!cell->feature)||(overlay&&!cell->overlay)))continue;
        BestLocSetData(grid,&point,(void *)i);
        if(LogicCopyPoint(&point,&grid->center)>grid->radius)continue;
        point.y>>=1;
        int value=NoiseGet(noise,&point);
        range->lo=TerrainMin(value,range->lo);
        range->hi=TerrainMax(value,range->hi);
        ++count; ++histogram[value];
    }
    if(percent) {
        int total=0;
        for(int h=range->hi;h>range->lo;h--) {
            total+=histogram[h];
            if(total>(percent*count+50)/100) {
                if(h==range->hi)*range=Range(255,0);
                else range->lo=h+1;
                break;
            }
        }
    }
    return range->lo<range->hi;
}

void TerrainSetHeights(TerrainGrid *grid,Noise *noise)
{
    Point p;
    Cell *cell=grid->cells+1;
    int i=1;
    for (;i<grid->count;i++,cell++) {
        memset(cell,0,sizeof(Cell));
        BestLocSetData(grid,&p,(void *)i);
        if (LogicCopyPoint(&p,&grid->center)>grid->radius) continue;
        p.y>>=1;
        int height=NoiseGet(noise,&p)>>5;
        cell->height=height<=0?0:(height>=7?7:height);
    }
}

void TerrainSetTypes(TerrainGrid *grid,Noise *noise)
{
    Point p;
    Cell *cell=grid->cells+1;
    for (int i=1;i<grid->count;i++,cell++) {
        BestLocSetData(grid,&p,(void *)i);
        if (LogicCopyPoint(&p,&grid->center)>grid->radius) continue;
        p.y>>=1;
        int type=NoiseGet(noise,&p)/17;
        cell->terrain=type<=0?0:(type>=14?14:type);
        if ((unsigned short)(terrainFlags)&8) cell->terrain=14-cell->terrain;
        int variants=terrainTypes[cell->Type()].variants;
        if(variants>1) cell->variant=next_link_seed(&gFxSeed)%variants; else cell->variant=0;
    }
}


extern int terrainWaterMode;
void TerrainSetWater(TerrainGrid *grid,int seed,int percent)
{
    if(percent>0) {
        Noise noise;
        terrainWaterMode=(logicMode==2||logicMode==3);
        Range range;
        if(terrainWaterMode) {
            unsigned cutoff=NoiseFill(&noise,seed,terrainWaterMode);
            TerrainHeightRange(grid,&noise,&range,percent+1);
            if(cutoff<254)range.lo=cutoff+1;
        } else {
            NoiseFill(&noise,seed,0);
            TerrainHeightRange(grid,&noise,&range,percent+1);
        }
        Cell *cell=grid->cells+1;
        for(int i=1;i<grid->count;i++,cell++) {
            if(!cell->feature&&terrainWaterThreshold<=cell->height) {
                Point p;BestLocSetData(grid,&p,(void *)i);
                if(LogicCopyPoint(&p,&grid->center)<=grid->radius) {
                    p.y>>=1; int value=NoiseGet(&noise,&p);
                    if(value>=range.lo&&value<=range.hi) {
                        int level=range.lo<range.hi?((value-range.lo)*5+((range.hi-range.lo)>>1))/(range.hi-range.lo):0;
                        level=level<=0?0:(level>=5?5:level);
                        cell->feature=terrainWaterBase+5-level;
                        cell->featureKind=66;
                    }
                }
            }
        }
        grid->waterCount=0;
        for(int j=1;j<grid->count;j++) {
            Point p;BestLocSetData(grid,&p,(void *)j);
            if(LogicCopyPoint(&p,&grid->center)<=grid->radius)grid->waterCount+=TerrainWater(grid,j);
        }
        NoiseClear(&noise);
    }
}
void TerrainTriplet(TerrainGrid *grid,int index,int kind)
{
    Point a,b;
    BestLocSetData(grid,&a,(void *)index); b=a;
    unsigned direction=next_link_seed(&MissionRandomTable)%6;
    a.Add(&pathDirections[tripletDirections[direction][0]]);
    b.Add(&pathDirections[tripletDirections[direction][1]]);
    int i=ResolveName(grid,&a),j=ResolveName(grid,&b);
    if(i&&j) {
        Cell *left=&grid->cells[i],*right=&grid->cells[j];
        if(!left->feature&&!right->feature) {
            Cell *middle=&grid->cells[index];
            if(left->height==middle->height&&middle->height==right->height) {
                U8 base=featureTypes[kind-1].base+direction*3;
                left->feature=base;middle->feature=base+1;right->feature=base+2;
                left->featureKind=kind;middle->featureKind=kind;right->featureKind=kind;
            }
        }
    }
}
void TerrainFeatures(TerrainGrid *grid)
{
    for(int kind=0;kind<64;kind++) {
        FeatureType *feature=&featureTypes[kind+1];
        if(feature->type==3||feature->type==4) {
            int overlay=feature->type==4;
            unsigned percent=0,mask=0;
            for(unsigned terrain=0;terrain<15;terrain++) {
                for(int j=0;j<16;j++) if(terrainTypes[terrain].feature[j].kind==kind+1) {
                    if(percent<terrainTypes[terrain].feature[j].chance)percent=terrainTypes[terrain].feature[j].chance;
                    mask|=1<<terrain;
                }
            }
            if(percent) {
                Noise noise;NoiseFill(&noise,88,0);
                Range range;TerrainHeightRange(grid,&noise,&range,percent,mask,overlay);
                Cell *cell=grid->cells+1;
                for(int index=1;index<grid->count;index++,cell++) {
                    if((mask&(1<<cell->terrain))&&((!overlay&&!cell->feature)||(overlay&&!cell->overlay))) {
                        Point p;BestLocSetData(grid,&p,(void *)index);
                        if(LogicCopyPoint(&p,&grid->center)<=grid->radius) {
                            p.y>>=1;int value=NoiseGet(&noise,&p);
                            if(value>=range.lo&&value<=range.hi) {
                                int level=0;
                                if(range.lo<range.hi) {
                                    level=(((range.hi-range.lo)>>1)+(value-range.lo)*5)/(range.hi-range.lo);
                                    level=5-(level<=0?0:(level>=5?5:level));
                                }
                                if(!overlay){cell->feature=feature->base+level;cell->featureKind=kind+1;}
                                else {cell->overlay=feature->base+level;cell->overlayKind=kind+1;}
                            }
                        }
                    }
                }
                NoiseClear(&noise);
            }
        }
    }
    Cell *cell=grid->cells+1;
    for(int index=1;index<grid->count;index++,cell++) {
        int featureRoll=next_link_seed(&MissionRandomTable)%100;
        int overlayRoll=next_link_seed(&MissionRandomTable)%100;
        int actionRoll=next_link_seed(&MissionRandomTable)%100;
        if(terrainTypes[cell->terrain].flags&1)cell->rotation=next_link_seed(&MissionRandomTable)%4;
        for(int j=0;j<16;j++) {
            int kind=terrainTypes[cell->terrain].feature[j].kind;
            int chance=terrainTypes[cell->terrain].feature[j].chance;
            if(kind) {
                int type=featureTypes[kind].type;
                if(type>100) {
                    if(actionRoll<chance&&!cell->feature&&!cell->overlay) {
                        WorldActionAdd((void *)index,type-67,0x7fffffff);
                        featureRoll=overlayRoll=actionRoll=9999;
                    }
                    actionRoll-=chance;
                } else if(type==2) {
                    if(!cell->overlay&&overlayRoll<chance) {
                        cell->overlay=featureTypes[kind].base;cell->overlayKind=kind;actionRoll=9999;
                    }
                    overlayRoll-=chance;
                } else {
                    if(!cell->feature&&featureRoll<chance) {
                        if(!featureTypes[kind].type){cell->feature=featureTypes[kind].base;cell->featureKind=kind;}
                        else if(featureTypes[kind].type==1)TerrainTriplet(grid,index,kind);
                        actionRoll=9999;
                    }
                    featureRoll-=chance;
                }
            }
        }
    }
}

void TerrainSetNoiseHeight(Point *point,U8 height)
{
    Point p; p.x=point->x; p.y=point->y; p.y>>=1;
    NoiseSet(&terrainNoise,&p,(U8)(height<<5));
}


struct MissionTerrain { int unknown,number; char pad[7]; U8 heightSeed,typeSeed,waterSeed; };
void TerrainBuildHeights(TerrainGrid *grid,MissionTerrain *mission,int seed,void *palette)
{
    MissionRandomTable=seed+69;
    GridClearNeighbors(grid);
    SimLoadImage(&g4c68b4,mission->number,palette);
    if(mission->heightSeed<99){NoiseGenerate(&terrainNoise,mission->heightSeed);NoiseSmooth(&terrainNoise);}
    else NoiseFill(&terrainNoise,mission->heightSeed,0);
    TerrainGrid *owner=grid;
    Point p;
    int i=1;Cell *cell=owner->cells+1;
    for(;i<owner->count;i++,cell++) {
        memset(cell,0,sizeof(Cell));BestLocSetData(owner,&p,(void *)i);
        if(LogicCopyPoint(&p,&owner->center)>owner->radius)continue;
        p.y>>=1;int height=NoiseGet(&terrainNoise,&p)>>5;
        cell->height=height<=0?0:(height>=7?7:height);
    }
}
void TerrainBuildFeatures(TerrainGrid *grid,MissionTerrain *mission,int seed,int water)
{
    MissionRandomTable=seed+95;
    if(!(terrainFlags&2)) {
        if(mission->typeSeed<99){NoiseGenerate(&terrainNoise,mission->typeSeed);NoiseSmooth(&terrainNoise);}
        else NoiseFill(&terrainNoise,mission->typeSeed,0);
    }
    MissionRandomTable=seed+0x13a66;TerrainSetTypes(grid,&terrainNoise);NoiseClear(&terrainNoise);
    MissionRandomTable=seed+0x1f96;if(terrainFlags&1)TerrainRaise(grid);
    MissionRandomTable=seed+0x1966;
    int multiplayer=(logicMode==2||logicMode==3);
    int waterSeed=multiplayer?99:mission->waterSeed;
    TerrainSetWater(grid,waterSeed,water);
    MissionRandomTable=seed+0x6502c;TerrainFeatures(grid);
    MissionRandomTable=seed+0x10118;
}

void TerrainDestroyNoise(Noise *p){NoiseDestroy(p,1);}
void TerrainConstructNoise(Noise *p){NoiseConstruct(p);}

