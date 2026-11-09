/* Polygon-resource TU. Native C++ with plain-oe; names are working labels.
 * 00429a04/1c/24/3c are shared constructor/destructor COMDATs.
 * unknownPlyState preserves the observed eight-byte BSS gap at 004bc418.
 * Shared GuiPoint constructors/destructor use their descriptor-backed identities. */
#include <stdio.h>
#include <string.h>
typedef unsigned long U32;
struct GuiPoint {long x,y;GuiPoint();GuiPoint(const GuiPoint &);};
inline GuiPoint::GuiPoint(){}
inline GuiPoint::GuiPoint(const GuiPoint &p):x(p.x),y(p.y){}
struct PlyEntry {GuiPoint points[16];long count;PlyEntry();};
inline PlyEntry::PlyEntry(){}

struct ResMgr {int OpenFile(long,int=0);int CloseFile();};extern ResMgr gResMgr;
extern "C" U32 f_c8e0(ResMgr *);extern "C" U32 f_c900(ResMgr *,void *,U32);
const char *currentPlyName;

struct PlyKey { long a; long b; };

int PlyMatch(PlyEntry *e, long a, long b);
void PlyGetKey(PlyKey *k);

PlyEntry gPly[8];
int gPlyCount;U32 unknownPlyState[2];PlyKey gPlyKey;GuiPoint plyScratch[16];

void PlyLoad(const char *name){
 if(name&&gResMgr.OpenFile((long)name,0)){
  U32 bytes=f_c8e0(&gResMgr);gPlyCount=bytes/sizeof(PlyEntry);
  f_c900(&gResMgr,gPly,bytes);gResMgr.CloseFile();
 }else gPlyCount=0;
}
int PlyFind(PlyKey *k)
{
    int i;
    PlyEntry *e = gPly;
    for (i = 0; i < gPlyCount; i++, e++) {
        if (PlyMatch(e, k->a, k->b))
            return i;
    }
    return -1;
}

int PlyFindCurrent()
{
    int i;
    PlyEntry *e;
    PlyGetKey(&gPlyKey);
    e = gPly;
    for (i = 0; i < gPlyCount; i++, e++) {
        if (PlyMatch(e, gPlyKey.a, gPlyKey.b))
            return i;
    }
    return -1;
}

int BriefDone(const char *name){
 char path[64];
 if(name!=currentPlyName){
  if(name){
   if(!currentPlyName||strcmp(name,currentPlyName)){
    sprintf(path,"%s.ply",name);PlyLoad(path);
   }
  }else gPlyCount=0;
  currentPlyName=name;
 }
 return PlyFindCurrent();
}
