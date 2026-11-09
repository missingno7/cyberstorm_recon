#include <windows.h>
#include <mmsystem.h>
#include <stdarg.h>
extern "C" int MakePath(void *, const char *, const char *, int);
extern "C" void AssertFail(const char *, int, const char *); extern "C" int sprintf(char *, const char *, ...); static const char ResourceSourcePath[] = "io\\resource.c"; static const char MissingResourceFormat[] = "Missing file: %s.  Possible cause: Bad installation, or CD removed at bad time.";
extern "C" int vsprintf(char *, const char *, va_list);

struct FileRecord { HANDLE handle; unsigned long open, flags, error; signed char direct, counted; unsigned char reserved[2]; unsigned long position, length; FileRecord(); ~FileRecord(); }; struct ResourceEntry { unsigned long resource, type, size; void *buffer; FileRecord sourceFile, outputFile; ResourceEntry(); ~ResourceEntry(); };
struct ResMgr {
    ResMgr(); int count;
    int types[10];
    const char *paths[10];
    ResourceEntry entries[10];
    int loaded;
    int opened;
    char pad2[4];
    FileRecord file;
    int IsOpen();
    int Init(const char ** volatile, const char *);
    void Close();
    int FindPath(const char *, char *, int);
    int CloseFile(); int *Load(char *, unsigned int = 0, int * = 0, unsigned int = 0); ~ResMgr();
};

struct MciState; extern "C" void ResetMciState(MciState *);
struct MciState {
    MciState() {ResetMciState(this);} ~MciState();
    unsigned int device;
    unsigned int pending;
    unsigned int openType;
    unsigned int element;
    unsigned int state;
    MciState *StopClose();
    int Open(int,int); void Close(); int PlayTimeRange(int, int, unsigned int, int);
};
unsigned long IoRead(FileRecord *, long, long); unsigned long IoClose(FileRecord *); extern "C" void ResetMciState(MciState *);


extern "C" void *IORelease(void *);
extern "C" char *FindChar(const char *, int);
ResMgr::ResMgr() { opened=0; loaded=opened; count=0; }
ResMgr::~ResMgr()
{ Close(); }
/* Keep this source line aligned with the original assertion immediates. */
int ResMgr::Init(const char ** volatile paths, const char *outDir)
{ if (this == 0 || loaded != 0 || opened != 0)






















        AssertFail(ResourceSourcePath, __LINE__, 0);
    const char **source = paths;
    int i = 0;
    char *entry = (char *)entries;
    count = 0;
    while (*source) {
        count = i + 1;
        types[i] = 1;
        this->paths[i] = *source;
        if (FindChar(*source, '.')) {
            types[i] = 2;
            if (!MakePath(entry, *source, outDir, 1))
                AssertFail(ResourceSourcePath, __LINE__, "Unable to open game data file!");
        }
        ++source; ++i;
        entry += 0x48;
        if (i >= 10)
            break;
    }
    loaded = 1;
    return loaded;
}
void ResMgr::Close()
{
    if (opened) {
        ResMgr *owner = this;
        if (opened)
            IORelease((char *)owner + 0x330);
        owner->opened = 0;
    }
    if (loaded)
        loaded = 0;
}
extern "C" int FileExists(char *, char *, int);
extern "C" int FileOpen(char *, const char *, char *, int);
extern "C" char *StringCopy(char *, const char *);
extern "C" char *StringAppend(char *, const char *);
int ResMgr::FindPath(const char *suffix, char * destination, int flags)
{ char buffer[256]; int i; 
    for(i=0;i<count;++i) {
        if(types[i]==1) { StringCopy(buffer,paths[i]); StringAppend(buffer,suffix); if(FileExists(destination,buffer,0))return 1; }
        else if(types[i]==2) { if(FileOpen((char*)&entries[i],suffix,destination,flags))return 1; }
    } return 0; }
/* Preserve the original AssertFail source line after reconstruction. */
/* Keep the missing-resource assertion at its original line as declarations evolve. */
inline void MissingResource(const char *name)
{
    char buffer[128];
    sprintf(buffer, MissingResourceFormat, name);
    /* original line padding 1 */
    /* original line padding 2 */
    /* original line padding 3 */
    /* original line padding 4 */
    /* original line padding 5 */
    /* original line padding 6 */
    /* original line padding 7 */
    /* original line padding 8 */
    /* original line padding 9 */
    /* original line padding 10 */
    /* original line padding 11 */
    /* original line padding 12 */
    /* original line padding 13 */
    /* original line padding 14 */
    /* original line padding 15 */
    /* original line padding 16 */
    /* original line padding 17 */
    /* original line padding 18 */
    /* original line padding 19 */
    /* original line padding 20 */
    /* original line padding 21 */
    /* original line padding 22 */
    /* original line padding 23 */
    /* original line padding 24 */
    /* original line padding 25 */
    /* original line padding 26 */
    /* original line padding 27 */
    /* original line padding 28 */
    /* original line padding 29 */
    /* original line padding 30 */
    /* original line padding 31 */
    /* original line padding 32 */
    /* original line padding 33 */
    /* original line padding 34 */
    /* original line padding 35 */
    /* original line padding 36 */
    /* original line padding 37 */
    /* original line padding 38 */
    /* original line padding 39 */
    /* original line padding 40 */
    /* original line padding 41 */
    /* original line padding 42 */
    /* original line padding 43 */
    /* original line padding 44 */
    /* original line padding 45 */
    /* original line padding 46 */
    /* original line padding 47 */
    /* original line padding 48 */
    /* original line padding 49 */
    /* original line padding 50 */
    /* original line padding 51 */
    /* original line padding 52 */
    /* original line padding 53 */
    /* original line padding 54 */
    /* original line padding 55 */
    /* original line padding 56 */
    /* original line padding 57 */
    /* original line padding 58 */
    /* original line padding 59 */
    /* original line padding 60 */
    /* original line padding 61 */
    /* Preserve the original assertion line after loop-form experiments. */




















    AssertFail(ResourceSourcePath, __LINE__, buffer);
}
extern "C" int ResMgrOpen(ResMgr *self, const char *name, int required)
{
    char path[128];
    if (self->FindPath(name, (char *)self + 0x330, 0)) {
        self->opened = 1;
        return 1;
    }
    if (required) {
        MissingResource(name);
    }
    return 0;
}
int ResMgr::CloseFile()
{
    if (opened)
        IORelease((char *)this + 0x330);
    opened = 0;
    return 1;
}
int ResMgr::IsOpen() { return opened; }
extern "C" int StreamRead(void *, void *, int);
extern "C" int ResMgrReadAt(void *, int, int);
#pragma aux ResMgrReadAt "f_c8a0";
extern "C" int ResMgrReadAt(void *self, int offset, int size)
{
    return IoRead((FileRecord *)((char *)self + 0x330), offset, size);
}
extern "C" int ResMgrReadFirstByte(void *);
#pragma aux ResMgrReadFirstByte "f_c8c0";
extern "C" int ResMgrReadFirstByte(void *self)
{
    return IoRead((FileRecord *)((char *)self + 0x330), 0, 1);
}
extern "C" int ResMgrGetFileLength(void *);
#pragma aux ResMgrGetFileLength "f_c8e0";
extern "C" int ResMgrGetFileLength(void *self)
{
    return IoClose((FileRecord *)((char *)self + 0x330));
}
extern "C" int ResMgrReadStream(void *, void *, int);
#pragma aux ResMgrReadStream "f_c900";
extern "C" int ResMgrReadStream(void *self, void *buffer, int size)
{
    return StreamRead((char *)self + 0x330, buffer, size);
}
extern "C" void *GlobalBlockAlloc(unsigned int, unsigned int);
extern "C" unsigned int DecodeResourceBlock(unsigned int, unsigned short *, unsigned int *);
extern "C" unsigned int gResourceFileSize;
extern "C" void GlobalBlockFree(void *);
/* Preserve the original line numbers for resource validation assertions. */
/* Keep this source spacing after relocating FileRecord helper declarations. */
int *ResMgr::Load(char *name, unsigned int flags, int *buffer, unsigned int bufferSize)
{
    struct LoadTemporaries {
        unsigned int decodedSize;
        int *decodedBuffer;
        int *originalBuffer;
        unsigned int fileSize;
    } local;
    int *header;
    int *result = 0;
    int found;
    int requiredFlag = flags & 1;
    local.originalBuffer = buffer;
    if (FindPath(name, (char *)this + 0x330, 0)) {
        opened = 1;
        found = 1;
    } else {
        if (requiredFlag)
            MissingResource(name);
        found = 0;
    }
    if (found) {
        local.fileSize = IoClose((FileRecord *)((char *)this + 0x330));
        gResourceFileSize = local.fileSize;
        if (local.fileSize != 0) {
            if (buffer == 0) {
                buffer = (int *)GlobalBlockAlloc(local.fileSize, 0);
            } else if (bufferSize < local.fileSize) {
                /* original line padding 9 */
                /* original line padding 10 */
                /* original line padding 11 */
                /* original line padding 12 */
                /* original line padding 13 */
                /* original line padding 14 */
                /* original line padding 15 */
                /* original line padding 16 */
                /* original line padding 17 */
                /* original line padding 18 */
                /* original line padding 19 */
                /* original line padding 20 */
                /* original line padding 21 */
                /* original line padding 22 */
                /* original line padding 23 */
                /* original line padding 24 */
                /* original line padding 25 */
                /* original line padding 26 */
                /* original line padding 27 */
                /* original line padding 28 */
                /* original line padding 29 */
                /* original line padding 30 */
                /* original line padding 31 */
                /* original line padding 32 */
                /* original line padding 33 */
                /* original line padding 34 */
                /* original line padding 35 */
                /* original line padding 36 */
                /* original line padding 37 */
                /* original line padding 38 */
                /* original line padding 39 */
                /* original line padding 40 */
                /* original line padding 41 */
                /* original line padding 42 */
                /* original line padding 43 */
                /* original line padding 44 */
                /* original line padding 45 */
                /* original line padding 46 */
                /* original line padding 47 */
                /* original line padding 48 */
                /* original line padding 49 */
                /* original line padding 50 */
                /* original line padding 51 */
                /* original line padding 52 */
                /* original line padding 53 */
                /* original line padding 54 */
                /* original line padding 55 */
                /* original line padding 56 */
                /* original line padding 57 */
                /* original line padding 58 */
                AssertFail(ResourceSourcePath, __LINE__, "Invalid data");
            }
            if (buffer == 0 && (flags & 1)) {
                AssertFail(ResourceSourcePath, __LINE__, "Ran out of memory or disk space.  Try clearing more disk space and running again.");
            }
            if (buffer != 0) {
                StreamRead((char *)this + 0x330, buffer, local.fileSize);
                result = buffer;
                header = (int *)buffer;
                if (header[0] == 0x3a584b50 && header[1] == 0x10011966 && header[2] == 0x9baebacf) {
                    if (header[3] <= 0 || header[3] >= 15 || local.fileSize - 0x18 != header[4] || header[5] <= 0 || header[5] >= 10000000 || local.originalBuffer != 0) {
                                                                                        /* original line padding 9 */
                        /* original line padding 10 */
                        /* original line padding 11 */
                        /* original line padding 12 */
                        /* original line padding 13 */
            /* preserve line 355 variant 1 */
            /* preserve line 355 variant 2 */
            /* preserve line 355 variant 3 */
            /* preserve line 355 variant 4 */
            /* preserve line 355 variant 5 */
            /* preserve line 355 variant 6 */
            /* preserve line 355 variant 7 */
            /* preserve line 355 variant 8 */
            AssertFail(ResourceSourcePath, __LINE__, 0);
                    }
                    result=0;
                    gResourceFileSize = header[5];
                    local.decodedSize = gResourceFileSize;
                    local.decodedBuffer = (int *)GlobalBlockAlloc(gResourceFileSize, 0);
                    if (local.decodedBuffer == 0 && (flags & 1)) {
                            /* preserve line 363 variant 1 */
                            AssertFail(ResourceSourcePath, __LINE__, "Ran out of memory or disk space.  Try clearing more disk space and running again.");
                    }
                    if (local.decodedBuffer != 0) {
                        DecodeResourceBlock(header[3], (unsigned short *)(buffer + 6), &local.decodedSize);
                        result = local.decodedBuffer;
                    }
                    if (buffer != 0)
                        GlobalBlockFree(buffer);
                }
            }
        }
        ResMgr *closeOwner=this;
        if (opened)
            IORelease((char *)this + 0x330);
        closeOwner->opened = 0;
    }
    return result;
}
extern "C" void ReleasePoolData(void *);
#pragma aux ReleasePoolData "f_cb80";
extern "C" void ReleasePoolData(void *p)
{
    if (p)
        GlobalBlockFree(p);
}
extern "C" ResMgr gResMgr;
extern "C" int LoadResourceData(int **, char *);
#pragma aux LoadResourceData "f_cba0";
extern "C" int LoadResourceData(int **out, char *name)
{
    int *resource = gResMgr.Load(name);
    *out = resource;
    if (resource != 0) {
        int field = resource[7];
        resource[7] = field + (int)resource;
        resource = *out;
        field = resource[8];
        resource[8] = field + (int)resource;
        resource = *out;
        field = resource[9];
        resource[9] = field + (int)resource;
    }
    return *out != 0;
}
extern "C" void ReleaseLoadedResource(void **);
#pragma aux ReleaseLoadedResource "f_cbf0";
extern "C" void ReleaseLoadedResource(void **p)
{
    if (*p) {
        ReleasePoolData(*p);
        *p = 0;
    }
}
struct Font { void *resource; };
extern "C" unsigned int MeasureResourceTextWidth(Font *p, const unsigned char *name)
{
    unsigned int sum;
    unsigned char ch;
    unsigned char *table;
    if (p->resource != 0 && name != 0) {
        sum = 0;
        ch = *name;
        table = *(unsigned char **)((char *)p->resource + 0x20);
        if (ch) {
            do {
                sum += table[*name++];
            } while (*name);
            return sum;
        }
        return sum;
    }
    sum = 0;
    return sum;
}
extern "C" unsigned int gCurrentGlyphColor;
extern "C" int gGlyphRenderFlags;
extern "C" int gGlyphClipLeft;
extern "C" int gGlyphClipTop;
extern "C" int gGlyphClipRight;
extern "C" int gGlyphClipBottom;
extern "C" unsigned char *gGlyphColorTable;
extern "C" unsigned int gGlyphBackgroundColor;
extern "C" unsigned int gGlyphCurrentBackground;
extern "C" unsigned char gGlyphSurface;
extern "C" void DrawGlyphPixel(void *, int, int, unsigned char);
extern "C" void DrawGlyphBackground(void *, int, int, int, int);
extern "C" void BlitGlyphRows(int, int, unsigned int, int, unsigned char *, unsigned char *);
extern "C" unsigned int RenderResourceGlyph(Font *handle, int x, int y, signed char character)
{
    register int *font = (int *)handle->resource;
    unsigned int width;
    register unsigned char *bits;
    register int height;
    unsigned char packed;
    register int index;
    register int right;
    register unsigned int row;

    if (font == 0)
        return 0;
    unsigned char code=(unsigned char)character;
    index = (int)code - font[5];
    if (index < 0 || font[6] <= index)
        return 0;
    width = *(unsigned char *)(code + (unsigned int)font[8]);
    if (width != 0 && code != 0x20) {
        register unsigned int savedColor;
        height = ((int*)handle->resource)[3];
        bits = (unsigned char *)(font[9] + *(int *)(font[7] + index * 4));
        packed = *(unsigned char *)((char *)font + 6) == 1;
        if (!packed && gGlyphRenderFlags == 1) {
            int bottom = y + height - 1;
            if (gGlyphClipLeft <= x && (int)(x + width - 1) <= gGlyphClipRight) {
                if (y < gGlyphClipTop) {
                    int clipped = gGlyphClipTop - y;
                    height -= clipped;
                    bits += clipped * width;
                    y = gGlyphClipTop;
                }
                if (gGlyphClipBottom < bottom)
                    height -= bottom - gGlyphClipBottom;
                if (height < 1)
                    return width;
                BlitGlyphRows(x, y, width, height, bits, gGlyphColorTable);
                return width;
            }
        }

        savedColor = gCurrentGlyphColor;
        if (gGlyphRenderFlags & 4)
            x += ((int*)handle->resource)[3] >> 1;
        right = x + width;
        for (row = 0; row < (unsigned int)((int*)handle->resource)[3]; ++row) {
            if ((gGlyphRenderFlags & 1) == 0) {
                gGlyphCurrentBackground = gGlyphBackgroundColor;
                DrawGlyphBackground(&gGlyphSurface, x, y, right, y);
            }
            unsigned int column = 0;
            unsigned char mask = 0x80;
            int pixelX = x;
            if (width != 0) {
                do {
                    unsigned char bit;
                if (!packed) {
                    bit = *bits;
                    if (bit != 0) {
                        unsigned int color=bit;
                        if (gGlyphColorTable != 0)
                            color = gGlyphColorTable[color];
                        gCurrentGlyphColor = color;
                    }
                    if (column < width - 1)
                        ++bits;
                } else {
                    if (mask == 0) {
                        ++bits;
                        mask = 0x80;
                    }
                    bit = *bits & mask;
                    mask >>= 1;
                }
                if (bit == 0) {
                    if ((gGlyphRenderFlags & 8) && row == (unsigned int)((int*)handle->resource)[4])
                        DrawGlyphPixel(&gGlyphSurface, pixelX, y, (unsigned char)savedColor);
                } else {
                    unsigned int color=gCurrentGlyphColor;
                    if ((gGlyphRenderFlags & 0x10) == 0) {
                        DrawGlyphPixel(&gGlyphSurface, pixelX, y, color);
                        if (gGlyphRenderFlags & 2)
                            DrawGlyphPixel(&gGlyphSurface, pixelX + 1, y, (unsigned char)gCurrentGlyphColor);
                    } else if (((pixelX + y) & 1) == 0) {
                        if (gGlyphRenderFlags & 2)
                            DrawGlyphPixel(&gGlyphSurface, pixelX + 1, y, color);
                    } else {
                        DrawGlyphPixel(&gGlyphSurface, pixelX, y, color);
                    }
                }
                ++column;
                ++pixelX;
                } while (column < width);
            }
            if ((gGlyphRenderFlags & 4) && (row & 1)) {
                --right;
                --x;
            }
            ++y;
            ++bits;
        }
        gCurrentGlyphColor = savedColor;
        if (gGlyphRenderFlags & 2)
            ++width;
    }
    return width;
}
void MenuDrawText(Font *font, long x, long y, const char *text)
{
    if (text) {
        while (*text) {
            x += RenderResourceGlyph(font, x, y, *text);
            ++text;
        }
    }
}
void MenuDrawTextAt(Font *font, long x, long y, int alignment, const char *text)
{ if(alignment<0) MenuDrawText(font,x,y,text);
    else {
        int width=(int)MeasureResourceTextWidth(font,(const unsigned char *)text);
        MenuDrawText(font,x-width/((alignment==0)+1),y,text);
    }
}
extern "C" void RenderFormattedResourceText(Font *font, int x, int y, const char *format, ...)
{
    char buffer[84];
    va_list arguments;
    va_start(arguments, format);
    vsprintf(buffer, format, arguments);
    va_end(arguments);
    register int drawY;
    register int drawX;
    register int *drawFont;
    drawFont = (int *)font;
    drawX = x;
    drawY = y;
    char *text = buffer;
    if (text) {
        while (*text) {
            drawX += RenderResourceGlyph((Font *)drawFont, drawX, drawY, *text);
            ++text;
        }
    }
}
MciState gShellResources;
extern "C" void ResetMciState(MciState *);
#pragma aux ResetMciState "f_d110";
extern "C" void ResetMciState(MciState *s)
{
    s->device = 0;
    s->pending = 0;
    s->state = 0;
}
MciState *MciState::StopClose()
{
    register MciState *deviceState = this;
    if (device) {
        register MciState *statusState = this;
        if (device)
            mciSendCommandA(device, MCI_STOP, 0, 0);
        statusState->state = 0;
        mciSendCommandA(deviceState->device, MCI_CLOSE, 0, 0);
        deviceState->device = 0;
    }
    return this;
}
int MciState::Open(int openType, int element)
{
    this->openType = openType;
    this->element = element;
    if (this->device == 0) {
        MCI_OPEN_PARMSA open;
        open.wDeviceID = this->device;
        open.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_CD_AUDIO;
        if (mciSendCommandA(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID, (unsigned long)&open) != 0)
            return 0;
        this->device = open.wDeviceID;
    }
    if (this->pending == 0) {
        MCI_STATUS_PARMS status;
        status.dwItem = 5;
        mciSendCommandA(this->device, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (unsigned long)&status);
        if (status.dwReturn == 0)
            return 0;
    }
    return 1;
}
void MciState::Close()
{
    if (device) {
        register MciState *statusState = this;
        if (device)
            mciSendCommandA(device, MCI_STOP, 0, 0);
        statusState->state = 0;
        mciSendCommandA(device, MCI_CLOSE, 0, 0);
        device = 0;
    }
}
extern "C" int MciStateNotPlaying(MciState *);
#pragma aux MciStateNotPlaying "f_d270";
extern "C" int MciStateNotPlaying(MciState *s)
{
    if (!s->device)
        return 0;
    MCI_STATUS_PARMS status;
    status.dwItem = 4;
    mciSendCommandA(s->device, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (unsigned long)&status);
    if (status.dwReturn == 0x20e)
        return 0;
    return 1;
}
extern "C" void StopMciState(MciState *);
#pragma aux StopMciState "f_d2c0";
extern "C" void StopMciState(MciState *s)
{
    if (s->device)
        mciSendCommandA(s->device, MCI_STOP, 0, 0);
    s->state = 0;
}
extern "C" void PlayNextMciTrack(MciState *, int);
#pragma aux PlayNextMciTrack "f_d2f0";
extern "C" void PlayNextMciTrack(MciState *s, int start)
{
    s->PlayTimeRange(start, 0, start + 1, 0);
}
int MciState::PlayTimeRange(int from, int fromSeconds, unsigned int to, int toSeconds)
{
    register MciState *state = this;
    struct LocalParameters {
        MCI_PLAY_PARMS play;
        MCI_SET_PARMS setTime;
    } local;
    local.setTime.dwTimeFormat = 10;
    mciSendCommandA(state->device, MCI_SET, MCI_SET_TIME_FORMAT, (unsigned long)&local.setTime);
    DWORD flags;
    if (to)
        flags = MCI_FROM | MCI_TO;
    else
        flags = MCI_FROM;
    state->state = 1;
    DWORD packedFrom = (DWORD)(unsigned char)from | ((fromSeconds / 60 & 0xffff) << 8);
    local.play.dwFrom = packedFrom | ((unsigned char)(fromSeconds % 60) << 16);
    local.play.dwTo = (to & 0xff) | ((toSeconds / 60 & 0xffff) << 8) | ((unsigned char)(toSeconds % 60) << 16);
    return mciSendCommandA(state->device, MCI_PLAY, flags, (unsigned long)&local.play) == 0;
}
