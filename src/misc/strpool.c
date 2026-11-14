struct PoolData { long first; long last; long count; char pad[8]; long indexed; long entries[1]; };
struct ResMgr { char pad[0x324]; int open; void *Load(long id, int a = 1, int b = 0, int c = 0); int IsOpen() { return open; } };
extern ResMgr gResMgr;
extern void AssertFail(const char *file, int line, const char *message);
extern void ReleasePoolData(void *data);
extern "C" unsigned long strlen(const char *text);
extern "C" void *memcpy(void *dest, const void *source, unsigned long size);
#define ASSERT(e) if (!(e)) AssertFail(__FILE__, __LINE__, 0)
class StrPool { public: long id; char *buffer; long capacity; PoolData *data; long size; StrPool(long id, long buffer, long capacity); StrPool *Reset(); int Lock(); int LockCore(); void Unlock(); char *GetString(long id, int loadData); char *GetSecondString(long id, int loadData); };
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
inline int StrPool::LockCore(){if(data==0&&gResMgr.IsOpen()){ASSERT(id!=0);data=(PoolData *)gResMgr.Load(id);}return data?1:0;} int StrPool::Lock(){if(data==0&&gResMgr.IsOpen()){ASSERT(id!=0);data=(PoolData *)gResMgr.Load(id);}return data?1:0;} void StrPool::Unlock(){if(data!=0){ReleasePoolData(data);data=0;}} void SetPointer(void **slot,void *value){*slot=value;} char *StrPool::GetString(long stringId,int loadData){size=0;if(stringId!=0){if(data!=0&&data->first<=stringId&&stringId<=data->last){PoolData *pool=data;long *entry=pool->entries;if(pool->indexed){long *e=entry;for(long index=0;index<data->count;++index,e+=2){if(stringId==e[1]){char *found=(char *)data+e[0];size=e[2]-e[0];if(!size)size=strlen(found)+1;return found;}}}else{stringId-=pool->first;entry+=stringId;char *found=(char *)pool+entry[0];size=entry[1]-entry[0];if(!size)size=strlen(found)+1;return found;}}if(loadData!=0&&data==0){
if(LockCore()){char *found=GetString(stringId,0);if(found!=0){if(buffer==0||size>=capacity){
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


char *StrPool::GetSecondString(long stringId,int loadData){char *result=GetString(stringId,loadData);if(result!=0){unsigned long length=strlen(result);char *second=result+(length+1);if(*second)result=second;}return result;}


