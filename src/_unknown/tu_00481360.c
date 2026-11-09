#include <windows.h>
extern "C" int sprintf(char *, const char *, ...);
extern "C" unsigned int strlen(const char *);
extern "C" void *memset(void *,int,unsigned int);
extern "C" char *strcpy(char *,const char *);
extern "C" char *strupr(char *);
extern "C" int memcmp(const void *,const void *,unsigned int);
#pragma function(strlen,memset,strcpy,memcmp)
struct FileRecord { HANDLE handle; unsigned long open, flags, error; signed char direct, counted; unsigned char reserved[2]; unsigned long position, length; ~FileRecord(); int IsOpen() { return open; } };
extern "C" FileRecord *FileInit(FileRecord *);
extern "C" FileRecord *FileRecordRelease(FileRecord *, int);
#pragma aux FileRecordRelease "W?Release$:FileRecord$n()pn$1$";
extern "C" int OpenFileRecord(FileRecord *,const char *,unsigned long);
#pragma aux OpenFileRecord "W?Open$:FileRecord$n(pnxaul)i";
extern "C" void IORelease(FileRecord *);
extern "C" unsigned long StreamRead(FileRecord *,void *,unsigned long);
extern "C" unsigned long IoRead(FileRecord *,long,long);
extern "C" void SetRange(FileRecord *,int,unsigned long,unsigned long);
struct ArchiveIndex { char name[12]; long offset; };
struct ResourceEntry {
    int opened, separate, count;
    ArchiveIndex *index;
    FileRecord source, independent;
    ~ResourceEntry();
};
extern "C" ResourceEntry *ResourceEntryConstruct(ResourceEntry *entry)
{
    FileRecord *file = FileInit(&entry->source);
    file = FileInit(file + 1);
    ResourceEntry *result = (ResourceEntry *)((char *)file - 0x2c);
    result->count = 0; result->index = 0; result->opened = 0;
    return result;
}
extern "C" void ResourceEntryClose(ResourceEntry *);
ResourceEntry::~ResourceEntry() { ResourceEntryClose(this); }
extern "C" int MakePath(ResourceEntry *entry, const char *name, const char **paths, int separate)
{
    char path[384];
    unsigned int magic;
    FileRecord *file = &entry->source;
    for (const char **p = paths; *p; ++p) {
        sprintf(path,"%s%s",*p,name);
        if (OpenFileRecord(file,path,0)) {
            StreamRead(file,&magic,4);
            StreamRead(file,&entry->count,4);
            entry->index = new ArchiveIndex[entry->count];
            StreamRead(file,entry->index,entry->count * sizeof(ArchiveIndex));
            int independent = separate && OpenFileRecord(&entry->independent,path,0);
            entry->opened = 1; entry->separate = independent;
            return 1;
        }
    }
    return 0;
}
extern "C" void ResourceEntryClose(ResourceEntry *entry)
{
    if (entry->source.IsOpen()) IORelease(&entry->source);
    if (entry->independent.IsOpen()) IORelease(&entry->independent);
    if (entry->index) delete [] entry->index;
    entry->opened = 0; entry->count = 0; entry->index = 0;
}
extern "C" int FileOpen(ResourceEntry *entry, const char *name, FileRecord *out, int separate)
{
    char key[13];
    unsigned int length;
    if (entry->opened && (int)strlen(name) <= 12) {
    memset(key,0,13); strcpy(key,name); strupr(key);
    int found = -1;
    int high = entry->count - 1;
    int low = 0;
    do {
        if (low > high) break;
        int middle = (low + high) >> 1;
        int result = memcmp(entry->index[middle].name,key,12);
        if (result < 0) low = middle + 1;
        else if (result > 0) high = middle - 1;
        else found = middle;
    } while (found == -1);
    if (found != -1) {
    length = -1;
    FileRecord *file = &entry->source;
    IoRead(file,entry->index[found].offset,0);
    StreamRead(file,&length,4);
    if (separate) file = &entry->independent;
    *out = *file;
    SetRange(out,separate,entry->index[found].offset+4,length);
    IoRead(out,0,0);
    return 1;
    }
    }
    return 0;
}

