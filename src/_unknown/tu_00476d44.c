// Save-file header and special-menu rebuilding; working names inferred from API use.
#include <string.h>
struct SaveHeader { unsigned magic; int version,kind; char rest[48]; };
extern int saveVersion0,saveVersion1;
struct FileRecord;
extern "C" unsigned StreamWrite(FileRecord*,const void*,unsigned);
extern "C" unsigned StreamRead(FileRecord*,void*,unsigned);
void Problem(unsigned long,int);
int SaveHeaderWrite(SaveHeader *h,FileRecord *f) {
 h->magic=0x3a425943; h->version=h->kind==0?saveVersion0:saveVersion1;
 return StreamWrite(f,h,60);
}
int SaveHeaderRead(SaveHeader *h,FileRecord *f) {
 int bad=(StreamRead(f,h,60)!=60);
 if(bad || h->magic!=0x3a425943) { Problem(305,0); return 0; }
 int version=h->kind==0?saveVersion0:saveVersion1;
 if(version!=h->version) { Problem(306,0); return 0; }
 return 1;
}
