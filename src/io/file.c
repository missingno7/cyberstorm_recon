#include <stdarg.h>
struct FileRecord { unsigned long handle; unsigned long open; unsigned long flags; unsigned long error; signed char direct; signed char counted; unsigned char reserved[2]; unsigned long position; unsigned long length; FileRecord *Release(); int Open(const char *, unsigned long); int Close(); unsigned long Size(); unsigned long Seek(long, long); unsigned long Write(const void *, unsigned long); void Printf(const char *, ...); }; static const char fileRootFormat[] = "\\\\.\\%c:";
extern unsigned long g_fileOpenCount;
extern unsigned long g_fileCloseCount;
extern unsigned long g_fileDirectCount;
extern "C" void AssertFail(const char *, int, const char *);
extern "C" __declspec(dllimport) unsigned long __stdcall CreateFileA(const char *, unsigned long, unsigned long, void *, unsigned long, unsigned long, unsigned long);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(unsigned long);
extern "C" __declspec(dllimport) int __stdcall ReadFile(unsigned long, void *, unsigned long, unsigned long *, void *);
extern "C" __declspec(dllimport) int __stdcall WriteFile(unsigned long, const void *, unsigned long, unsigned long *, void *);
extern "C" __declspec(dllimport) unsigned long __stdcall SetFilePointer(unsigned long, long, long *, unsigned long);
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileAttributesA(const char *);
extern "C" __declspec(dllimport) int __stdcall CopyFileA(const char *, const char *, int);
extern "C" __declspec(dllimport) int __stdcall MoveFileA(const char *, const char *);
extern "C" __declspec(dllimport) int __stdcall DeleteFileA(const char *);
extern "C" __declspec(dllimport) int __stdcall CreateDirectoryA(const char *, void *);
extern "C" __declspec(dllimport) int __stdcall RemoveDirectoryA(const char *);
extern "C" __declspec(dllimport) int __stdcall GetDiskFreeSpaceA(const char *, unsigned long *, unsigned long *, unsigned long *, unsigned long *);
extern "C" int sprintf(char *, const char *, ...);
extern "C" int vsprintf(char *, const char *, va_list);
void FileInit(FileRecord *file) { file->open = 0; file->handle = 0; file->error = 0; }
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
// source-line padding before the three line-140 assertions
FileRecord *FileRecord::Release(){if(open!=0){if(open==0)AssertFail(__FILE__,__LINE__,(const char *)open);if(direct==0){CloseHandle(handle);++g_fileCloseCount;}if(counted!=0)--g_fileDirectCount;open=0;handle=0;}return this;} int FileRecord::Open(const char *path,unsigned long options){int failed=1;if(path!=0&&open==0){unsigned long access=0x80000000UL;unsigned long attributes=0x80;unsigned long disposition=3;if(options&1)access=0xc0000000UL;if(options&2)disposition=4;if(options&4)disposition=2;unsigned long newHandle=CreateFileA(path,access,1,0,disposition,attributes,0);if(newHandle!=0xffffffffUL){open=1;handle=newHandle;++g_fileOpenCount;failed=0;}}position=0;length=0x7fffffffUL;direct=0;counted=0;flags=options;g_fileDirectCount+=counted;return failed==0;} int FileRecord::Close(){if(open==0)AssertFail(__FILE__,__LINE__,(const char *)open);int result=0;if(direct==0){result=CloseHandle(handle)==0;++g_fileCloseCount;}if(counted!=0)--g_fileDirectCount;open=0;handle=0;return result;}
unsigned long FileRecord::Size()
{
    unsigned long current, end;
    if (open == 0) {
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        // source-line padding before the line-170 ASSERT
        AssertFail(__FILE__, __LINE__, (const char *)open);
    }
    if (direct != 0) return length;
    current = Seek(0, 1);
    end = Seek(0, 2);
    Seek((long)current, 0);
    return end;
}
unsigned long FileRead(FileRecord *file, void *buffer, unsigned long count)
{
    unsigned long bytesRead = 0;
    if (file->open == 0 || buffer == 0) {
        // source-line padding before the line-193 ASSERT
        // source-line padding before the line-193 ASSERT
        // source-line padding before the line-193 ASSERT
        // source-line padding before the line-193 ASSERT
        // source-line padding before the line-193 ASSERT
        // source-line padding before the line-193 ASSERT
        // source-line padding before the line-193 ASSERT
        // source-line padding before the line-193 ASSERT
        // source-line padding before the line-193 ASSERT
        // source-line padding before the line-193 ASSERT
        // source-line padding before the line-193 ASSERT
        AssertFail(__FILE__, __LINE__, 0);
    }
    int failed = ReadFile(file->handle, buffer, count, &bytesRead, 0) == 0;
    failed |= count != bytesRead;
    if (failed) ++file->error;
    return bytesRead;
}
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
// source-line padding before the line-221 assertions
unsigned long FileRecord::Write(const void *buffer,unsigned long count){unsigned long bytesWritten=0xffffffffUL;if(open==0||buffer==0||(flags&1)==0||direct!=0)AssertFail(__FILE__,__LINE__,0);register unsigned long requested=count;register const void *data=buffer;int failed=WriteFile(handle,data,requested,&bytesWritten,0)==0;failed|=requested!=bytesWritten;if(failed)++error;return bytesWritten;} unsigned long FileRecord::Seek(long distance,long origin){unsigned long result=0xffffffffUL;int failed;if(direct!=0){if(origin==0)distance+=position;if(origin==2)distance=(position+length)-distance;}if(open!=0&&origin>=0&&origin<=2){result=SetFilePointer(handle,distance,0,(unsigned long)origin);failed=result==0xffffffffUL;}else failed=1;if(failed)++error;return result;} int FileGetAndClearError(FileRecord *file){unsigned long error=file->error;file->error=0;return error;} void FileRecord::Printf(const char *format,...){char buffer[200];va_list args;unsigned long written;int count;if(format!=0){va_start(args,format);count=vsprintf(buffer,format,args);va_end(args);written=0xffffffffUL;register FileRecord *file=this;if(file->open==0||buffer==0||(file->flags&1)==0||file->direct!=0)AssertFail(__FILE__,__LINE__,0);int failed=WriteFile(file->handle,buffer,count,&written,0)==0;failed|=(unsigned long)count!=written;if(failed)++file->error;}}
void SetRange(FileRecord *file,int useDirect,long start,long size){file->position=start;file->length=size;file->direct=(start!=0||size!=0x7fffffffL);file->counted=(file->direct!=0&&useDirect!=0);g_fileDirectCount+=file->counted;}
int FileExists(const char *path)
{
    int error = 1;
    register const char *filename = path;
    if (filename == 0) {
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        // source-line padding before the line-354 ASSERT
        AssertFail(__FILE__, __LINE__, (const char *)filename);
    }
    if (filename != 0) error = GetFileAttributesA(filename) == 0xffffffffUL;
    return error == 0;
}
int FileCopy(const char *source,const char *dest,int overwrite){int error=1;if(source!=0&&dest!=0)error=CopyFileA(source,dest,overwrite==0)==0;return error==0;}
int FileMove(const char *source,const char *dest){int error=1;if(source!=0&&dest!=0)error=MoveFileA(source,dest)==0;return error==0;}
int FileDelete(const char *path){int error=1;if(path!=0)error=DeleteFileA(path)==0;return error==0;}
int DirectoryCreate(const char *path){int error=1;if(path!=0)error=CreateDirectoryA(path,0)==0;return error==0;}
int DirectoryRemove(const char *path){int error=1;if(path!=0)error=RemoveDirectoryA(path)==0;return error==0;}
long FileFreeSpace(signed char drive){char path[16];unsigned long s,b,f,t,clusterBytes,result;result=0xffffffffUL;sprintf(path,fileRootFormat,(int)drive);if(GetDiskFreeSpaceA(drive?path:0,&s,&b,&f,&t)){clusterBytes=s*b;result=f*clusterBytes;if(clusterBytes>result)result=0x7fffffffUL;}return (long)result;}

