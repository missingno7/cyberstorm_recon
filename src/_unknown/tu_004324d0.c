/* Provisional ot object, 004324d0..00432930. No path evidence. */
typedef unsigned char U8;
struct Point {int x,y;
    void Set(Point *p){x=p->x;y=p->y;}
    void Add(Point *p){x+=p->x;y+=p->y;}
};
struct Cell {U8 terrain,height,feature,featureKind,overlay,overlayKind,rotation,variant;int raised;};
struct Selection {int actor;U8 selected;U8 rest[3];};
struct TerrainGrid {int prefix[10];int count,waterCount,selection;int rest[4];Cell *cells;Selection *actors;};
struct UnitModel {int indices[256];U8 values[128];int cost,count;};
struct Actor {U8 unknown[8];int owner;U8 unknown0c[0x36];int index;};
extern "C" void BestLocSetData(TerrainGrid *,Point *,int);
extern "C" int ResolveName(TerrainGrid *,Point *);
extern "C" int LogicCopyPoint(Point *,Point *);
extern "C" int TargetActorById(int);
extern "C" unsigned next_link_seed(unsigned *);
extern unsigned MissionRandomTable;
extern int *actorIndexTable;
extern int gLocalActorKind, pathVisibilityMode;
extern U8 *simActorVisibility,*simVisibleTypes;
extern Point pathDirections[6];
struct PathCounts {int values[2];};extern PathCounts emptyPathCounts;
int OccupiedMovementCost(TerrainGrid *,int,int);
inline int PathRawActor(int index) {return actorIndexTable[index*2];}
inline int ActorHidden(Actor *actor) {return !simActorVisibility[actor->index];}
inline int TerrainHidden(int index) {return !simVisibleTypes[index];}
inline int PathActor(int index,int excluded) {
    int actor=PathRawActor(index);
    if(pathVisibilityMode && actor) {
        if(actor>0) {
            Actor *a=(Actor *)TargetActorById(index);
            if(a && a->owner!=gLocalActorKind && ActorHidden(a)) actor=0;
        } else if(TerrainHidden(index)) actor=0;
        if(actor==excluded) actor=0;
    }
    return actor;
}

int BuildTerrainPath(TerrainGrid *grid,int from,int to,int *path,int limit) {
    int costs[2][6],candidates[2][6];
    Point start,next,current;Point destination;int counts[2];
    if(!from || !to) return -1;
    MissionRandomTable=0x7041776;
    BestLocSetData(grid,&start,from);
    BestLocSetData(grid,&destination,to);
    if(LogicCopyPoint(&start,&destination)>limit) return -1;
    int position=from,count=0,total=0;
    int destinationActor=PathRawActor(to);
    while(position!=to) {
        BestLocSetData(grid,&current,position);
        int distance=LogicCopyPoint(&current,&destination);
        *(PathCounts *)counts=emptyPathCounts;
        Point *direction=pathDirections;
        for(int d=0;d<6;++d,++direction) {
            next=current;next.x=current.x+direction->x;next.y=current.y+direction->y;
            int index=ResolveName(grid,&next);
            if(!index) continue;
            int actor=PathActor(index,destinationActor);
            if(actor) continue;
            int nextDistance=LogicCopyPoint(&next,&destination);
            if(nextDistance>distance) continue;
            int j; for(j=0;j<count;++j) if(index==path[j]) break;
            if(j!=count) continue;
            if(index==to) {counts[0]=counts[1]=0;}
            int group=distance-nextDistance;
            candidates[group][counts[group]]=index;
            costs[group][counts[group]]=OccupiedMovementCost(grid,index,position);
            ++counts[group];
            if(index==to) break;
        }
        int chosen=0,cost=0;
        for(int group=1;group>=0;--group) {
            if(counts[group]>0) {
                unsigned slot=next_link_seed(&MissionRandomTable) % counts[group];
                chosen=candidates[group][slot];cost=costs[group][slot];break;
            }
        }
        if(!chosen) return -1;
        if(count>=limit) return -1;
        path[count++]=chosen; total+=cost;position=chosen;
    }
    path[count]=0;return total;
}
int GridClearSelection(TerrainGrid *grid) {
    if(!grid->selection) return 0;
    int count=grid->count-1;
    U8 *p=&grid->actors[1].selected;
    while(count--) {*p=0;p+=8;}
    grid->selection=0;return 1;
}
void GridSetSelection(TerrainGrid *grid,UnitModel *unit) {
    int count=unit->count;
    int *index=unit->indices;
    U8 *value=unit->values;
    if(count) grid->selection=1;
    while(count--) {Selection *p=&grid->actors[*index++];p->selected=*value++;}
}
void ReportSelectMission(TerrainGrid *grid,int index,int selected) {
    grid->selection=1;grid->actors[index].selected=selected;
}








