#include <windows.h>
#include <stdarg.h>
typedef struct FileRecord { HANDLE handle; unsigned long open, flags, error; signed char direct, counted; unsigned char reserved[2]; unsigned long position, length; } FileRecord; static char fileRootFormat[] = "\\\\.\\%c:";
extern unsigned long g_fileOpenCount;
extern unsigned long g_fileCloseCount;
extern unsigned long g_fileDirectCount;
void AssertFail(const char *, int, const char *);
int sprintf(char *, const char *, ...);
int vsprintf(char *, const char *, va_list);

FileRecord *FileRelease(FileRecord *file);
int FileOpen(FileRecord *file,const char *path,unsigned long options);
int IORelease(FileRecord *file);
unsigned long IoClose(FileRecord *file);
unsigned long StreamWrite(FileRecord *file,const void *buffer,unsigned long count);
unsigned long IoRead(FileRecord *file,long distance,long origin);
void FilePrintf(FileRecord *file,const char *format,...);
#pragma aux FileRelease "W?Release$:FileRecord$n()pn$1$";
#pragma aux FileOpen "W?Open$:FileRecord$n(pnxaul)i";
#pragma aux FilePrintf "W?Printf$:FileRecord$n(pnxae)v";
// Symbol spelling preserves the project data label during the C transition.
#pragma aux g_fileCloseCount "W?g_fileCloseCount$nul";
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
FileRecord *FileRelease(FileRecord *file){unsigned long active=file->open;if(active!=0){FileRecord *closing=file;if(active==0)AssertFail(__FILE__,__LINE__,(const char *)active);if(closing->direct==0){CloseHandle(closing->handle);++g_fileCloseCount;}if(closing->counted!=0)--g_fileDirectCount;closing->open=0;closing->handle=0;}return file;} int FileOpen(FileRecord *file,const char *path,unsigned long options){int failed=1;if(path!=0&&file->open==0){unsigned long access=0x80000000UL;unsigned long attributes=0x80;unsigned long disposition=3;HANDLE newHandle;if(options&1)access=0xc0000000UL;if(options&2)disposition=4;if(options&4)disposition=2;newHandle=CreateFileA(path,access,1,0,disposition,attributes,0);if(newHandle!=INVALID_HANDLE_VALUE){file->open=1;file->handle=newHandle;++g_fileOpenCount;failed=0;}}file->position=0;file->length=0x7fffffffUL;file->direct=0;file->counted=0;file->flags=options;g_fileDirectCount+=file->counted;return failed==0;} int IORelease(FileRecord *file){int result;unsigned long active=file->open;if(active==0)AssertFail(__FILE__,__LINE__,(const char *)active);result=0;if(file->direct==0){result=CloseHandle(file->handle)==0;++g_fileCloseCount;}if(file->counted!=0)--g_fileDirectCount;file->open=0;file->handle=0;return result;}
unsigned long IoClose(FileRecord *file)
{
    unsigned long end, current; unsigned long active=file->open;
    if (active == 0) {
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
        AssertFail(__FILE__, __LINE__, (const char *)active);
    }
    if (file->direct != 0) return file->length;
    current = IoRead(file,0, 1);
    end = IoRead(file,0, 2);
    IoRead(file,(long)current, 0);
    return end;
}
unsigned long StreamRead(FileRecord *file, void *buffer, unsigned long count)
{
    unsigned long bytesRead = 0; int failed; unsigned long active=file->open;
    if (active == 0 || buffer == 0) {
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
    failed = ReadFile(file->handle, buffer, count, &bytesRead, 0) == 0;
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
unsigned long StreamWrite(FileRecord *file,const void *buffer,unsigned long count){unsigned long bytesWritten=0xffffffffUL;int failed;unsigned long active=file->open;if(active==0||buffer==0||(file->flags&1)==0||file->direct!=0)AssertFail(__FILE__,__LINE__,0);failed=WriteFile(file->handle,buffer,count,&bytesWritten,0)==0;failed|=count!=bytesWritten;if(failed)++file->error;return bytesWritten;} unsigned long IoRead(FileRecord *file,long distance,long origin){unsigned long result=0xffffffffUL;int failed;signed char direct=file->direct;if(direct!=0){if(origin==0)distance+=file->position;if(origin==2)distance=(file->position+file->length)-distance;}if(file->open!=0&&origin>=0&&origin<=2){result=SetFilePointer(file->handle,distance,0,(unsigned long)origin);failed=result==0xffffffffUL;}else failed=1;if(failed)++file->error;return result;} int FileGetAndClearError(FileRecord *file){unsigned long error=file->error;file->error=0;return error;} void FilePrintf(FileRecord *file,const char *format,...){char buffer[200];unsigned long written;va_list args;int count,failed;if(format!=0){va_start(args,format);count=vsprintf(buffer,format,args);va_end(args);written=0xffffffffUL;{FileRecord *writer=file;void *data=buffer;if(writer->open==0||data==0||(writer->flags&1)==0||writer->direct!=0)AssertFail(__FILE__,__LINE__,0);failed=WriteFile(writer->handle,data,count,&written,0)==0;failed|=(unsigned long)count!=written;if(failed)++writer->error;}}}
void SetRange(FileRecord *file,int useDirect,long start,long size){file->position=start;file->length=size;file->direct=(start!=0||size!=0x7fffffffL);file->counted=(file->direct!=0&&useDirect!=0);g_fileDirectCount+=file->counted;}
int FileExists(const char *path)
{
    int error = 1;
    
    if (path == 0) {
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
        AssertFail(__FILE__, __LINE__, (const char *)path);
    }
    if (path != 0) error = GetFileAttributesA(path) == 0xffffffffUL;
    return error == 0;
}
int FileCopy(const char *source,const char *dest,int overwrite){int error=1;if(source!=0&&dest!=0)error=CopyFileA(source,dest,overwrite==0)==0;return error==0;}
int FileMove(const char *source,const char *dest){int error=1;if(source!=0&&dest!=0)error=MoveFileA(source,dest)==0;return error==0;}
int FileDelete(const char *path){int error=1;if(path!=0)error=DeleteFileA(path)==0;return error==0;}
int DirectoryCreate(const char *path){int error=1;if(path!=0)error=CreateDirectoryA(path,0)==0;return error==0;}
int DirectoryRemove(const char *path){int error=1;if(path!=0)error=RemoveDirectoryA(path)==0;return error==0;}
long FileFreeSpace(signed char drive){char path[16];unsigned long t,f,b,s,clusterBytes,result;result=0xffffffffUL;sprintf(path,fileRootFormat,(int)drive);if(GetDiskFreeSpaceA(drive?path:0,&s,&b,&f,&t)){clusterBytes=s*b;result=f*clusterBytes;if(clusterBytes>result)result=0x7fffffffUL;}return (long)result;}

