/* Provisional terrain movement object, ot. Layout names are working labels. */
#include <string.h>
typedef unsigned char U8;
#pragma pack(push,1)
struct Cell {U8 terrain,height,feature,featureKind,overlay,overlayKind,rotation,variant;int raised;};
struct Selection {int actor;U8 selected;U8 rest[3];};
struct TerrainGrid {int unknown[2];int radius;int rest0c[7];int count,waterCount,selection;int rest34[4];Cell *cells;Selection *actors;int baseCost;
    Cell *CellAt(int i){return cells+i;} int ActorAt(int i){return actors[i].actor;}
};
struct TerrainType {U8 first,variants,unknown,move,defense,cover;U8 rest[33];};
struct FeatureType {U8 base,type,unknown,defense,cover;U8 rest[3];};
struct TerrainConfig {U8 unknown[13],featureLife,unknown0e,pathCost;};
struct UnitState {U8 unknown[0xe7];int weight;};
struct FogObject {U8 unknown[0x1b];int speed;};
struct ActorItem {U8 unknown[0x28d];int ascent[8],descent[8];};
struct TargetInfo {short cost;U8 cover,defense;int actor,feature;};
struct Point {int x,y;};
struct Search {int words[10];};
struct Visit {TerrainGrid *grid;Point point;int index;};
#pragma pack(pop)
extern TerrainType terrainTypes[];
extern FeatureType featureTypes[];
extern TerrainConfig g4c68b4;
extern int heightStep;
extern int *actorIndexTable;
struct TargetTriple {U8 terrain,height,feature,featureKind,rest[8];};
extern TargetTriple *targetDefinitionTable;
extern int UnitCapability(void *,int);
extern FogObject *UnitFogObject(UnitState *,int,void *);
struct ActorUnit; extern ActorItem *PopFindItem(ActorUnit *,int,int);
extern int BestLocInitialize(void *,const char *,int);
extern int BestLocSetCandidate(void *,void *);
extern void BestLocVisit(void *,int (*)(void *));
int terrainMovementCosts[82];
int ascentCosts[8],descentCosts[8];
int selectedFeature;
inline int ActorAt(int index) {return actorIndexTable[index*2];}
inline Cell *GlobalCellAt(int index) {return (Cell *)(targetDefinitionTable+index);}
int TerrainFeatureValue(TerrainGrid *,int);
inline int MovementMax(int a,int b) {return a>b?a:b;}
void ConfigureMovement(TerrainGrid *grid,UnitState *unit,void *pilot) {
    memset(&terrainMovementCosts,0,sizeof(terrainMovementCosts));
    for(int i=0;i<15;++i) {
        int value=terrainTypes[i].move;
        int extra=(value-16)*60/20;
        if(extra>0) value+=extra;
        terrainMovementCosts[i]=value;
    }
    for(i=1;i<65;++i) terrainMovementCosts[15+i]=featureTypes[i].unknown*4+16;
    terrainMovementCosts[80]=g4c68b4.pathCost;
    terrainMovementCosts[81]=0;
    int capability=UnitCapability(pilot,6);
    int weight=unit->weight*100/(capability+50);
    char fogStorage[32];
    int speed=MovementMax(UnitFogObject(unit,2,fogStorage)->speed,10);
    ActorItem *item=PopFindItem((ActorUnit *)unit,0,-1);
    
    for(i=0;i<82;++i) terrainMovementCosts[i]=(terrainMovementCosts[i]*weight+speed-1)/speed;
    grid->baseCost=(16*weight+speed-1)/speed;
    memset(ascentCosts,0,sizeof(ascentCosts));memset(descentCosts,0,sizeof(descentCosts));
    for(i=0;i<7;++i) {
        ascentCosts[i+1]=(item->ascent[i]*weight+speed-1)/speed;
        descentCosts[i+1]=(item->descent[i]*weight+speed-1)/speed;
        ascentCosts[i+1]*=heightStep;descentCosts[i+1]*=heightStep;
        ascentCosts[i+1]/=500;descentCosts[i+1]/=500;
    }
}
int TerrainMovementCost(TerrainGrid *grid,int index,int from) {
    Cell *cell=grid->CellAt(index);
    int cost=terrainMovementCosts[cell->terrain]+terrainMovementCosts[15+cell->featureKind];
    if(from) {
        Cell *previous=grid->CellAt(from);
        
        if(cell->height>previous->height) cost+=ascentCosts[cell->height-previous->height];
        else if(cell->height<previous->height) cost+=descentCosts[previous->height-cell->height];
    }
    return MovementMax(cost,1);
}
int OccupiedMovementCost(TerrainGrid *grid,int index,int from) {
    if(grid->ActorAt(index)) return 2000;
    return TerrainMovementCost(grid,index,from);
}
extern "C" void TargetInitAt(TerrainGrid *grid,TargetInfo *out,int index,int from) {
    out->cost=TerrainMovementCost(grid,index,from);
    Cell *cell=grid->CellAt(index);
    TerrainType *type=&terrainTypes[cell->terrain];
    out->cover=type->cover;out->defense=type->defense;
    if(cell->feature) {
        int kind=cell->featureKind;
        if(kind>0 && kind<=64) {
            FeatureType *feature=&featureTypes[kind-1];
            out->cover+=feature->cover;out->defense+=feature->defense;
        }
    }
    if(cell->overlay) {
        int kind=cell->overlayKind;
        if(kind>0 && kind<=64) {
            FeatureType *feature=&featureTypes[kind-1];
            out->cover+=feature->cover;out->defense+=feature->defense;
        }
    }
    out->actor=grid->ActorAt(index);
    out->feature=TerrainFeatureValue(grid,index);
}
int TerrainFeatureValue(TerrainGrid *grid,int index) {
    Cell *cell=grid->CellAt(index);int value=0;
    if(cell->featureKind==66) value=(g4c68b4.featureLife+6-cell->feature)*10;
    return value;
}
int TakeTerrainFeature(TerrainGrid *grid,int index) {
    int value=TerrainFeatureValue(grid,index);
    if(value) {Cell *taken=grid->CellAt(index);taken->feature=0;taken->featureKind=0;grid->waterCount-=value;}
    return value;
}
int VisitTerrainFeature(void *object) {
    int index=((Visit *)object)->index;
    if(index && !ActorAt(index) && GlobalCellAt(index)->featureKind==66) {
        selectedFeature=index;return 1;
    }
    return 0;
}
int FindTerrainFeature(TerrainGrid *grid,void *point) {
    Search search;selectedFeature=0;
    if(point && grid->waterCount>0) {
        BestLocInitialize(&search,(const char *)grid,grid->radius*2+1);
        BestLocSetCandidate(&search,point);
        BestLocVisit(&search,VisitTerrainFeature);
    }
    return selectedFeature;
}
extern "C" void AnimReleaseResource(TerrainGrid *grid,int index,U8 flags) {
    Cell *cell=grid->CellAt(index);
    if(flags&1) {cell->feature=0;cell->featureKind=0;}
    if(flags&2) {cell->overlay=0;cell->overlayKind=0;}
}















