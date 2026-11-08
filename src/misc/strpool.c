struct ResMgr { char pad[0x324]; int open; void *Load(long id, int a = 1, int b = 0, int c = 0); int IsOpen() { return open; } };
extern ResMgr gResMgr;

void AssertFail(const char *file, int line, const char *msg);
#define ASSERT(e) if (!(e)) AssertFail(__FILE__, __LINE__, 0)
class StrPool { public: long id; long a; long b; void *data; int Lock(); };
/* misc\strpool.c (cs11 code 0047c2a0..; path string 004a49e8). PARTIAL TU: only functions listed in
 * recovery.json are proven. Names and layouts are working labels. __LINE__ is evidence: ASSERT calls must
 * stay on their original lines (StrPool::Lock's ASSERT is line 59). */









































int StrPool::Lock()
{
    if (data == 0 && gResMgr.IsOpen()) {





        ASSERT(id != 0);
        data = gResMgr.Load(id);
    }
    return data ? 1 : 0;
}







