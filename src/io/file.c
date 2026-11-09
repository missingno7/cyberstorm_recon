struct FileRecord { unsigned long handle; unsigned long open; unsigned long flags; unsigned long error; unsigned char direct; unsigned char counted; unsigned char reserved[2]; unsigned long position; unsigned long length; };
extern "C" void AssertFail(const char *file, int line, const char *message);
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileAttributesA(const char *path);
extern "C" __declspec(dllimport) int __stdcall CopyFileA(const char *source, const char *dest, int failIfExists);
extern "C" __declspec(dllimport) int __stdcall MoveFileA(const char *source, const char *dest);
extern "C" __declspec(dllimport) int __stdcall DeleteFileA(const char *path);
extern "C" __declspec(dllimport) int __stdcall CreateDirectoryA(const char *path, void *security);
extern "C" __declspec(dllimport) int __stdcall RemoveDirectoryA(const char *path);
extern "C" __declspec(dllimport) int __stdcall GetDiskFreeSpaceA(const char *root, unsigned long *sectorsPerCluster, unsigned long *bytesPerSector, unsigned long *freeClusters, unsigned long *totalClusters);
extern "C" int sprintf(char *buffer, const char *format, ...);

void FileInit(FileRecord *file)
{
    file->open = 0;
    file->handle = 0;
    file->error = 0;
}

int FileGetAndClearError(FileRecord *file)
{
    unsigned long error = file->error;
    file->error = 0;
    return error;
}

int FileExists(const char *path)
{
    int error = 1;
    register const char *filename = path;
    if (filename == 0) {



































































































































































































































































































































        AssertFail(__FILE__, __LINE__, (const char *)filename);
    }
    if (filename != 0)
        error = GetFileAttributesA(filename) == 0xffffffffUL;
    return error == 0;
}

int FileCopy(const char *source, const char *dest, int overwrite)
{
    int error = 1;
    if (source != 0 && dest != 0)
        error = CopyFileA(source, dest, overwrite == 0) == 0;
    return error == 0;

}
int FileMove(const char *source, const char *dest)
{
    int error = 1;
    if (source != 0 && dest != 0)
        error = MoveFileA(source, dest) == 0;
    return error == 0;

}
int FileDelete(const char *path)
{
    int error = 1;
    if (path != 0)
        error = DeleteFileA(path) == 0;
    return error == 0;

}
int DirectoryCreate(const char *path)
{
    int error = 1;
    if (path != 0)
        error = CreateDirectoryA(path, 0) == 0;
    return error == 0;

}
int DirectoryRemove(const char *path)
{
    int error = 1;
    if (path != 0)
        error = RemoveDirectoryA(path) == 0;
    return error == 0;

}
long FileFreeSpace(signed char drive)
{
    char path[16];
    unsigned long sectorsPerCluster, bytesPerSector, freeClusters, totalClusters;
    unsigned long result = 0xffffffffUL;
    if (drive != 0) {
        sprintf(path, "\\\\.\\%c:", (int)drive);
    }
    if (GetDiskFreeSpaceA(drive != 0 ? path : 0, &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters)) {
        result = sectorsPerCluster * bytesPerSector;
        result = freeClusters * result;
        if (sectorsPerCluster * bytesPerSector <= result && result > 0x7fffffffUL)
            result = 0x7fffffffUL;
    }
    return (long)result;
}


