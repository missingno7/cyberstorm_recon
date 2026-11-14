/* Polygon object starts with two switch tables at 004275bc; first function at 004275dc. */
struct GuiPoint {long x,y;};extern long g5138ac,g5138b0;
struct PlyEntry {GuiPoint points[16];long count;};
template<class T> inline T PlyMin(T a,T b){return a<b?a:b;}
template<class T> inline T PlyMax(T a,T b){return a>b?a:b;}
int PlyMatch(PlyEntry *poly,long x,long y){
 if(poly->count<3)return 0;
 long minX=g5138ac,maxX=-1,minY=g5138b0,maxY=-1;
 int i;GuiPoint *p=poly->points;
 for(i=0;i<poly->count;++i,++p){
  minX=PlyMin(minX,p->x);maxX=PlyMax(maxX,p->x);
  minY=PlyMin(minY,p->y);maxY=PlyMax(maxY,p->y);
 }
 if(x<minX||x>maxX||y<minY||y>maxY)return 0;
 int direction,found=0;
 for(direction=0;direction<4;++direction){
  p=poly->points;
  found=0;
  for(i=0;!found&&i<poly->count;++i,++p){
   switch(direction){
    case 0:found=(x==p->x&&y>=p->y);break;
    case 1:found=(x==p->x&&y<=p->y);break;
    case 2:found=(y==p->y&&x>=p->x);break;
    case 3:found=(y==p->y&&x<=p->x);break;
   }
  }
  if(!found)break;
 }
 if(found)return 1;
 p=poly->points;
 long previousX=poly->points[poly->count-1].x,previousY=poly->points[poly->count-1].y;
 int crossings=0;
 for(i=0;i<poly->count;++i,++p){
  long currentX=p->x,currentY=p->y;
  minX=PlyMin(previousX,currentX);maxX=PlyMax(previousX,currentX);
  minY=PlyMin(previousY,currentY);maxY=PlyMax(previousY,currentY);
  switch(direction){
   case 0:
    if(x>minX&&x<maxX&&y>=(currentY-previousY)*(x-previousX)/(currentX-previousX)+previousY){
     crossings+=(-(currentY-previousY)*(x-previousX)+(currentX-previousX)*(y-previousY)>0)?-1:1;
    }break;
   case 1:
    if(x>minX&&x<maxX&&y<=(currentY-previousY)*(x-previousX)/(currentX-previousX)+previousY){
     crossings+=(-(currentY-previousY)*(x-previousX)+(currentX-previousX)*(y-previousY)>0)?-1:1;
    }break;
   case 2:
    if(y>minY&&y<maxY&&x>=(currentX-previousX)*(y-previousY)/(currentY-previousY)+previousX){
     crossings+=(-(currentY-previousY)*(x-previousX)+(currentX-previousX)*(y-previousY)>0)?-1:1;
    }break;
   case 3:
    if(y>minY&&y<maxY&&x<=(currentX-previousX)*(y-previousY)/(currentY-previousY)+previousX){
     crossings+=(-(currentY-previousY)*(x-previousX)+(currentX-previousX)*(y-previousY)>0)?-1:1;
    }break;
  }
  previousX=currentX;previousY=currentY;
 }
 return crossings&1;
}

