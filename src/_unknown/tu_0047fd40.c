#include <windows.h>
struct CriticalSection {
    CRITICAL_SECTION native;
    CriticalSection();
    ~CriticalSection();
};
CriticalSection::CriticalSection() { InitializeCriticalSection(&native); }
CriticalSection::~CriticalSection() { DeleteCriticalSection(&native); }
