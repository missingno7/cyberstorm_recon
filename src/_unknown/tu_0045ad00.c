// Projectile path interpolation and effect updates. Names are provisional.
#include <string.h>
#pragma pack(1)
struct ExploPoint { int x,y; ExploPoint() {} ExploPoint(const ExploPoint &p) { x=p.x; y=p.y; } };
struct SpeedProfile { int speed, acceleration, maximum, minimum; };
struct ExploRecord { int depth; signed char type, anim; char positioned, repeat; unsigned char fps, frame, frames, frameStep, delay, life; signed char layer; unsigned int nextTime; int target,value; ExploPoint pos,pos2; };
struct ActorRecord { char pad00[8]; int owner; };
struct TargetRecord { char pad00[0xc]; ExploPoint source; char pad14[0x30]; ActorRecord *actor; char pad48[0x18]; ExploPoint target; };
struct ProjectileDef { char pad00[3]; signed char delay; char pad04[2]; signed char speed; char pad07; signed char arc; };
struct Weapon { char pad00[0x21]; signed char range; };
struct Trajectory {
    ExploPoint sourceScreen,targetScreen,sourceWorld,targetWorld,currentWorld,currentScreen;
    int distance, position, height, arc, done, active, crossed, at4c;
    int trailKind,trailParam,trailRate,partKind,partValue,partDirection,quality,steps,stepScale;
    SpeedProfile speed;
    ExploRecord *effect,*part;
};
#pragma pack()
extern unsigned int ISqrt(unsigned int);
extern int PointAngle(ExploPoint *,ExploPoint *);
extern void ViewToScreen(void *,ExploPoint *,ExploPoint *);
extern int ViewScale(void *,int);
extern int FixedSin(int);
extern ExploRecord *ExploAddPart(int,int);
extern void ExploSetupFrame(ExploRecord *,ExploPoint *);
extern ExploRecord *ExploAddTrail(int,int,int,ExploRecord *,ExploPoint *);
extern long GridPointToIndex(const char *,ExploPoint *);
extern void BestLocSetData(void *,void *,void *);
extern char fallbackName[],gView[];
extern int gZoom,gLocalActorKind,simLimit3,simLimit4;
extern unsigned int gFxSeed;
extern "C" unsigned int next_link_seed(unsigned int *);
extern SpeedProfile trajectorySpeedProfiles[];
inline int Clamp(int value,int low,int high) { if(value<=low)return low; if(value>=high)return high; return value; }
inline unsigned int PointDistance(ExploPoint *a,ExploPoint *b) { int x=b->x-a->x,y=b->y-a->y; return ISqrt(y*y+x*x); }
int TrajectoryArc(ProjectileDef *def,Weapon *w,TargetRecord *target)
{
    int distance=PointDistance(&target->source,&target->target);
    int arc=0;
    if(def->arc) arc=def->arc*(distance/w->range)*50/1000;
    if(arc<0)arc=0;
    if(distance<=2200)arc/=2;
    if(distance<=1100)arc/=4;
    return arc;
}
void TrajectoryInit(Trajectory *t,ProjectileDef *def,ExploRecord *effect,ExploPoint *source,ExploPoint *destination,Weapon *w,TargetRecord *target,int delay)
{
    ExploPoint start,end;
    memset(t,0,sizeof(*t));
    t->sourceScreen=*source; t->targetScreen=*destination;
    t->sourceWorld=target->source; t->targetWorld=target->target;
    t->distance=PointDistance(&t->sourceWorld,&t->targetWorld);
    t->position=-delay*def->delay;
    t->arc=TrajectoryArc(def,w,target); t->done=0; t->active=0;
    t->speed=trajectorySpeedProfiles[def->speed];
    t->effect=effect; t->part=0;
    int scatter=gZoom/4;
    if(scatter) {
        t->targetScreen.x=destination->x-scatter/2+next_link_seed(&gFxSeed)%scatter;
        t->targetScreen.y=destination->y-scatter/2+next_link_seed(&gFxSeed)%scatter;
    }
    if(t->arc>5) t->arc=t->arc*9/10+next_link_seed(&gFxSeed)%(t->arc/5);
    t->quality=gLocalActorKind==target->actor->owner?simLimit3:simLimit4;
    if(t->quality==0) {t->steps=2;t->stepScale=300;}
    else if(t->quality<5) {t->steps=2;t->stepScale=150;}
    else if(t->quality<10) {t->steps=2;t->stepScale=80;}
    else {t->steps=2;t->stepScale=40;}
    ViewToScreen(gView,&start,&target->source); ViewToScreen(gView,&end,&target->target);
    int angle=(PointAngle(&start,&end)/0xb60b61+720)%360;
    if(angle>120&&angle<240)t->effect->depth=start.y-gZoom*2;
    else t->effect->depth=20000;
}
inline int IsDefault(unsigned char value) { return value==255; }
void TrajectoryEffects(Trajectory *t,unsigned char kind,unsigned char param,unsigned char rate,unsigned char part,unsigned char value)
{
    ExploPoint start,end;
    t->trailKind=kind;t->trailParam=param;t->trailRate=rate;
    t->partKind=IsDefault(part)?t->effect->anim:part;
    t->partValue=IsDefault(value)?t->effect->value/14:value;
    if(t->quality>5 && t->partKind && t->partValue) {
        t->part=ExploAddPart(t->partKind,t->partValue);
        t->part->type=-4;t->part->depth=-10000;
        ViewToScreen(gView,&start,&t->sourceWorld); ViewToScreen(gView,&end,&t->targetWorld);
        t->partDirection=(PointAngle(&end,&start)/0xb60b61+367)/15%24;
    }
}
void TrajectoryPosition(Trajectory *t)
{
    if(!t->active || !t->distance) { t->effect->delay=100; return; }
    {
        t->effect->delay=0;t->height=0;
        if(t->arc) {
            int height=ViewScale(gView,t->arc);
            int angle=Clamp(t->position*180/t->distance,0,180);
            t->height=FixedSin(angle*0xb60b61)*height>>16;
        }
        t->currentScreen.x=t->position*(t->targetScreen.x-t->sourceScreen.x)/t->distance+t->sourceScreen.x;
        t->currentScreen.y=t->position*(t->targetScreen.y-t->sourceScreen.y)/t->distance+t->sourceScreen.y-t->height;
    }
}
void TrajectoryDraw(Trajectory *t)
{
    ExploPoint at;
    if(t->active && t->distance) {
        ExploSetupFrame(t->effect,&t->currentScreen);
        if(t->part) {
            t->currentWorld.x=t->sourceWorld.x+t->position*(t->targetWorld.x-t->sourceWorld.x)/t->distance;
            t->currentWorld.y=t->sourceWorld.y+t->position*(t->targetWorld.y-t->sourceWorld.y)/t->distance;
            ViewToScreen(gView,&at,&t->currentWorld);
            BestLocSetData(fallbackName,&t->part->pos2,(void *)GridPointToIndex(fallbackName,&t->currentWorld));
            if(at.y<=t->currentScreen.y)at.y=t->currentScreen.y+1;
            ExploSetupFrame(t->part,&at);
        }
        ExploAddTrail(t->trailKind,t->trailParam,t->trailRate,t->effect,&t->currentScreen);
    }
}
void TrajectoryAdvance(Trajectory *t)
{
    if(!t->done) {
        if(t->position<0) { t->position++; return; }
        int old=t->position;
        int step=t->speed.speed*t->stepScale/100;
        if(t->effect->anim && step>t->distance/5)step=t->distance/5;
        t->position+=step;
        t->speed.speed+=t->speed.acceleration;
        t->speed.speed=Clamp(t->speed.speed,t->speed.minimum,t->speed.maximum);
        t->crossed=old<=0 && t->position>0;
        if(t->position>0)t->active=1;
        if(t->position>=t->distance){t->done=1;t->active=0;}
    }
}
void TrajectoryDirection(Trajectory *t)
{
    int angle=PointAngle(&t->targetScreen,&t->sourceScreen);
    if(t->arc&&t->active&&!t->crossed) {
        Trajectory future=*t;
        future.sourceScreen.x*=96;future.sourceScreen.y*=96;
        future.targetScreen.x*=95;future.targetScreen.y*=95;future.arc*=96;
        TrajectoryPosition(&future);ExploPoint before=future.currentScreen;
        TrajectoryAdvance(&future);TrajectoryPosition(&future);ExploPoint after=future.currentScreen;
        if(PointDistance(&after,&before)>10)angle=PointAngle(&after,&before);
    }
    t->effect->frame=(angle/0xb60b61+367)/15%24;
    if(t->part)t->part->frame=t->partDirection;
}
int TrajectoryUpdate(Trajectory *t,int orient)
{
    for(int i=0;i<t->steps;i++){TrajectoryPosition(t);TrajectoryDraw(t);TrajectoryAdvance(t);}
    if(orient)TrajectoryDirection(t);
    return t->done;
}
int TrajectoryDone(Trajectory *t) { return t->done; }
int TrajectoryActive(Trajectory *t) { return t->active && !t->done; }
int TrajectoryCrossed(Trajectory *t) { return t->crossed; }
