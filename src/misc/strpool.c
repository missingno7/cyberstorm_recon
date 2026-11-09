struct ResMgr { char pad[0x324]; int open; void *Load(long id, int a = 1, int b = 0, int c = 0); int IsOpen() { return open; } };
extern ResMgr gResMgr;
extern void AssertFail(const char *file, int line, const char *message);
#define ASSERT(e) if (!(e)) AssertFail(__FILE__, __LINE__, 0)
class StrPool { public: long id; long a; long b; void *data; StrPool(long id, long a, long b); StrPool *Reset(); int Lock(); void Unlock(); };
extern void ReleasePoolData(void *data);

StrPool::StrPool(long ident, long first, long second)
{
    data = 0;
    a = first;
    b = second;
    id = ident;
}

StrPool *StrPool::Reset()
{
    if (data != 0) {
        ReleasePoolData(data);
        data = 0;
    }
    return this;
}

int StrPool::Lock()
{
    if (data == 0 && gResMgr.IsOpen()) {































        ASSERT(id != 0);
        data = gResMgr.Load(id);
    }
    return data ? 1 : 0;
}

void StrPool::Unlock()
{
    if (data != 0) {
        ReleasePoolData(data);
        data = 0;
    }
}

void SetPointer(void **slot, void *value)
{
    *slot = value;
}
