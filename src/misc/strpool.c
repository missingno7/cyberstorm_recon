struct PoolData { long first; long last; long count; char pad[8]; long indexed; long entries[1]; };
struct ResMgr { char pad[0x324]; int open; void *Load(long id, int a = 1, int b = 0, int c = 0); int IsOpen() { return open; } };
extern ResMgr gResMgr;
extern void AssertFail(const char *file, int line, const char *message);
extern void ReleasePoolData(void *data);
extern "C" unsigned long strlen(const char *text);
extern "C" void *memcpy(void *dest, const void *source, unsigned long size);
#define ASSERT(e) if (!(e)) AssertFail(__FILE__, __LINE__, 0)
class StrPool { public: long id; char *buffer; unsigned long capacity; PoolData *data; unsigned long size; StrPool(long id, long buffer, long capacity); StrPool *Reset(); int Lock(); void Unlock(); char *GetString(long id, int loadData); char *GetSecondString(long id, int loadData); };
StrPool::StrPool(long ident, long first, long second) { data = 0; buffer = (char *)first; capacity = second; id = ident; }
StrPool *StrPool::Reset() { if (data != 0) { ReleasePoolData(data); data = 0; } return this; }
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
// source-line padding before the line-59 ASSERTs
int StrPool::Lock(){if(data==0&&gResMgr.IsOpen()){ASSERT(id!=0);data=(PoolData *)gResMgr.Load(id);}return data?1:0;} char *StrPool::GetString(long stringId,int loadData){size=0;if(stringId!=0){PoolData *pool=data;if(pool!=0&&pool->first<=stringId&&stringId<=pool->last){long *entry=pool->entries;if(pool->indexed==0){entry+=stringId-pool->first;long offset=entry[0];long end=entry[1];size=end-offset;if(size==0)size=strlen((char *)((char *)pool+offset))+1;return (char *)((char *)pool+offset);}else{for(long index=0;index<pool->count;++index,entry+=2){if(stringId==entry[1]){long offset=entry[0];long end=entry[2];size=end-offset;char *found=(char *)pool+offset;if(size==0)size=strlen(found)+1;return found;}}}}if(loadData!=0&&data==0){if(gResMgr.open!=0){if(id==0)ASSERT(id!=0);
data=(PoolData *)gResMgr.Load(id,1,0,0);}if(data!=0){char *found=GetString(stringId,0);if(found!=0){if(buffer==0||capacity<=size){
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
            // source-line padding before the line-148 ASSERT
                AssertFail(__FILE__, __LINE__, 0);
            }
            memcpy(buffer, found, size);
            if(data!=0){ReleasePoolData(data);data=0;}
            return buffer;
        }
        if(data!=0){ReleasePoolData(data);data=0;}
    }
}}return 0;}
void StrPool::Unlock(){if(data!=0){ReleasePoolData(data);data=0;}}
void SetPointer(void **slot,void *value){*slot=value;}
char *StrPool::GetSecondString(long stringId,int loadData){char *result=GetString(stringId,loadData);if(result!=0){unsigned long length=strlen(result);char *second=result+(length+1);if(*second!=0)result=second;}return result;}


