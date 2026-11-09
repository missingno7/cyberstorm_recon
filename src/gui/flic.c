/* gui\flic.c: inferred Flic/Surface class names; pinned 10.6, plain profile. */
#pragma aux file_init_name "W?FileInit$n(pn$FileRecord$$)v";
#pragma aux file_release_name "W?Release$:FileRecord$n()pn$1$";
#include <string.h>
typedef unsigned long U32;
struct Surface {long width,height;unsigned char *pixels;U32 flags;Surface(U32);~Surface();};
struct FileRecord {U32 handle,open,flags,error;signed char direct,counted;unsigned char reserved[2];U32 position,length;
    __pragma("file_init_name") FileRecord();__pragma("file_release_name") ~FileRecord();U32 IsOpen(){return open;}};
struct FlicSound {unsigned short id,frame;unsigned char volume,pan,pitch,flags;};
struct SoundSettings {int loop,pan,pitch,volume;};
struct Timer {U32 start,end,duration;};
struct ResMgr {void *Load(long,int=1,int=0,int=0);int FindPath(const char *,FileRecord *,int);};
extern ResMgr gResMgr;
extern void *GlobalBlockAlloc(U32,unsigned);
extern void *GlobalBlockFree(void *);
extern void ReleasePoolData(void *);
extern U32 TimeNow();
extern int IORelease(FileRecord *);
extern U32 IoRead(FileRecord *,long,long);
extern U32 StreamRead(FileRecord *,void *,U32);
extern void AssertFail(const char *,int,const char *);
extern void SurfaceFree(Surface *);
extern int SurfaceCreate(Surface *,long,long);
extern void SurfaceClear(Surface *);
extern void SurfaceCopyRegion(Surface *,Surface *,long,long);
extern void ApplyPaletteData(void *,int=0,int=256);
extern void SoundStop(unsigned);
extern void SoundFlush(int);
extern void SoundLoad(unsigned);
extern int SoundPlay(unsigned,const SoundSettings *);
extern int SoundPlayDefault(unsigned,int);
extern SoundSettings gFlicSoundDefaults;
extern long gFlicMinimumPriority;
extern U32 gFlicInputSize,gFlicDecodeSize;
extern unsigned char *gFlicInput,*gFlicDecode;
extern int gFlicFileCount,gFlicResourceCount;
void FlicFreeBuffers();
 static char flicSourceFile[] = __FILE__;



void FlicInitBuffers(U32 inputSize,U32 decodeSize){if(gFlicInput){GlobalBlockFree(gFlicInput);gFlicInput=0;}if(gFlicDecode){GlobalBlockFree(gFlicDecode);gFlicDecode=0;}gFlicInput=(unsigned char *)GlobalBlockAlloc(inputSize,0);gFlicInputSize=inputSize;gFlicDecode=(unsigned char *)GlobalBlockAlloc(decodeSize,0);gFlicDecodeSize=decodeSize;if(!gFlicInputSize||!gFlicDecodeSize||!gFlicInput||!gFlicDecode)AssertFail(flicSourceFile,__LINE__,0);}
void FlicFreeBuffers()
{
    if(gFlicInput){GlobalBlockFree(gFlicInput);gFlicInput=0;}
    if(gFlicDecode){GlobalBlockFree(gFlicDecode);gFlicDecode=0;}
}

struct FlicHeader {
    long fileSize;
    unsigned short magic,frames,width,height,depth,flags;
    long delay;
    unsigned char reserved[60]; /* remaining unknown header fields */
    U32 firstFrame,secondFrame;
    unsigned char tail[40];
};
struct FrameHeader { long size;short magic,chunks;long reserved[2]; };
struct FlicColor { unsigned char red,green,blue,reserved; };
struct FlicPalette { FlicColor colors[256];unsigned char unknown[82]; };
struct ChunkHeader { long size;short kind; };
extern int UnpackFlicPixels(unsigned char *,const void *);
extern int UnpackFlicPacked(unsigned char *,const void *);
extern void CopyFrameSettings(void *,const void *);
extern void ResetFrameSettings(void *);
extern char gFlicFrameSettings;

class Flic {
public:
    Surface *target;
    Surface bitmap;
    U32 unknown14,unknown18; /* unobserved; not fabricated initialized fields */
    FlicSound *sounds;
    FileRecord file;
    FlicPalette *palette;
    U32 status;
    U32 currentFrame;
    U32 deadline;
    long loops;
    int active;
    U32 memoryPosition;
    unsigned char *resource;
    FrameHeader pending;
    U32 pendingRead;
    long fileSize,magic,frames,width,height,delay;
    U32 firstFrame,secondFrame;
    unsigned char *Resource(){return resource;}
    void PixelsCore(const void *data){unsigned char *destination=target->pixels;UnpackFlicPixels(destination,data);}
    void RegionCore(const void *data){long left=*(const long *)data,top=((const long *)data)[1];unsigned char *destination=target->pixels;UnpackFlicPixels(destination,(const unsigned char *)data+8);SurfaceCopyRegion(target,&bitmap,left,top);}
    void PackedCore(const void *data){UnpackFlicPacked(gFlicDecode,data);unsigned char *destination=target->pixels;unsigned char *source=gFlicDecode;UnpackFlicPixels(destination,source);}
    Flic();
    ~Flic();
    void Rewind();
    int Open(long source,int priority,int sourceMode);
    void Close();
    void ApplyPalette();
    void SetSounds(FlicSound *);
    void StopSounds();
    void PlaySounds();
    U32 DecodeFrame();
    U32 Update(Surface *);
    int DecodePalette(unsigned char *);
    int UnpackPixels(unsigned char *,const void *);
    int DecodePixels(const void *);
    int DecodeRegion(const void *);
    int DecodePackedPixels(const void *);
    void RewindCore() {
        if(resource)memoryPosition=firstFrame;else IoRead(&file,firstFrame,0);
        status=0;currentFrame=0;deadline=0;loops=-1;
        SurfaceClear(&bitmap);pending.size=0;active=1;
    }
    void StopSoundsCore() {
        if(sounds){while(sounds->id){SoundStop(sounds->id);++sounds;}SoundFlush(5000);sounds=0;}
    }
    void CloseCore() {
        active=0;SurfaceFree(&bitmap);
        if(file.IsOpen()){IORelease(&file);--gFlicFileCount;}
        if(palette){delete palette;palette=0;}
        if(resource){ReleasePoolData(resource);resource=0;--gFlicResourceCount;}
        StopSoundsCore();
    }
};

Flic::Flic():bitmap(0),file()
{
    pending.size=0;palette=0;sounds=0;resource=0;active=0;
}

Flic::~Flic()
{CloseCore();



}

void Flic::Rewind() {
        if(resource)memoryPosition=firstFrame;else IoRead(&file,firstFrame,0);
        status=0;currentFrame=0;deadline=0;loops=-1;
        SurfaceClear(&bitmap);pending.size=0;active=1;
    }

int Flic::Open(long source,int priority,int sourceMode)
{
    FlicHeader header;
    CloseCore();
    if(priority<gFlicMinimumPriority)return 1;
    if(sourceMode){
        resource=(sourceMode==2)?(unsigned char *)source:(unsigned char *)gResMgr.Load(source);
        if(!resource)return 0;
        header=*(FlicHeader *)resource;
        memoryPosition=128;
        ++gFlicResourceCount;
    }else{
        if(!gResMgr.FindPath((const char *)source,&file,1))return 0;
        StreamRead(&file,&header,128);
        ++gFlicFileCount;
    }
    fileSize=header.fileSize;magic=header.magic;frames=header.frames;
    width=header.width;height=header.height;
    firstFrame=header.firstFrame;secondFrame=header.secondFrame;
    delay=(header.delay*100)/100;
    palette=new FlicPalette;
    if(magic==0xaf20 || (magic==0xaf21 && SurfaceCreate(&bitmap,width,height))){
        RewindCore();
        return 1;
    }
    CloseCore();
    return 0;
}

void Flic::Close() {
        active=0;SurfaceFree(&bitmap);
        if(file.IsOpen()){IORelease(&file);--gFlicFileCount;}
        if(palette){delete palette;palette=0;}
        if(resource){ReleasePoolData(resource);resource=0;--gFlicResourceCount;}
        StopSoundsCore();
    }

void Flic::ApplyPalette() { if(active)ApplyPaletteData(palette); }

void Flic::SetSounds(FlicSound *events)
{
    if((sounds=events)!=0){
        while(events->id){
            {unsigned char flags=events->flags;if(flags&0x80)SoundLoad(events->id);}
            ++events;
        }
    }

}

void Flic::StopSounds() {
        if(sounds){while(sounds->id){SoundStop(sounds->id);++sounds;}SoundFlush(5000);sounds=0;}
    }

void Flic::PlaySounds()
{
    if(sounds){
        FlicSound *event=sounds;
        while(event->id){
            if(event->frame==currentFrame){
                SoundSettings settings=gFlicSoundDefaults;
                settings.loop=(U32)event->flags&0x40;
                settings.pan=event->pan;
                settings.pitch=event->pitch-100;
                settings.volume=event->volume;
                unsigned char flags=event->flags;if(!(flags&0x20))SoundPlay(event->id,&settings);
                else SoundPlayDefault(event->id,0);
            }
            ++event;
        }
    }
}

U32 Flic::DecodeFrame()
{
    FrameHeader header,*frame;
    unsigned char *chunks;
    if(Resource()){
        U32 now=TimeNow();
        if(now<deadline)return 0;
        deadline=now+delay;
        frame=(FrameHeader *)(resource+memoryPosition);
        chunks=resource+memoryPosition+16;
        memoryPosition+=frame->size;
    }else if(gFlicFileCount==1){
        if(!file.IsOpen()||!gFlicInput)return 0;
        if(!pending.size){
            pending.size=0;pendingRead=0;
            StreamRead(&file,&pending,16);
            if((U32)(pending.size-16)>gFlicInputSize){
                IoRead(&file,pending.size-16,1);pending.size=0;return 0;
            }
        }
        U32 now=TimeNow();
        U32 readSize=now>=deadline?pending.size:delay<<6;
        U32 remaining=pending.size-16-pendingRead;
        if(readSize>remaining)readSize=remaining;
        if(readSize)StreamRead(&file,gFlicInput+pendingRead,readSize);
        pendingRead+=readSize;
        if(now<deadline)return 0;
        deadline=now+delay;
        frame=&pending;chunks=gFlicInput;
    }else{
        if(!file.IsOpen()||!gFlicInput)return 0;
        if(TimeNow()<deadline)return 0;
        deadline=TimeNow()+delay;
        StreamRead(&file,&header,16);
        long size=header.size-16;
        if(size>(long)gFlicInputSize){IoRead(&file,size,1);return 0;}
        StreamRead(&file,gFlicInput,size);
        frame=&header;chunks=gFlicInput;
    }
    U32 result=1;
    int settingsChanged=0;
    U32 offset=0;
    for(int i=0;i<frame->chunks;++i){
        U32 next=offset+*(long *)(chunks+offset);
        int kind=*(short *)(chunks+offset+4);
        unsigned char *data=chunks+offset+6;
        if(kind==100)PixelsCore(data);
        else if(kind==101)RegionCore(data);
        else if(kind==104)PackedCore(data);
        else if(kind==4){DecodePalette(data);result|=0x20;}
        else if(kind==102){
            struct SettingsRect {long values[4];} values;values.values[0]=((long *)data)[0];values.values[1]=((long *)data)[1];values.values[2]=((long *)data)[2];values.values[3]=((long *)data)[3];
            CopyFrameSettings(&gFlicFrameSettings,&values);settingsChanged=1;
        }
        offset=next;
    }
    ++currentFrame;
    if(currentFrame==frames&&loops>0)--loops;
    if(currentFrame==frames+1){
        currentFrame=1;
        if(resource)memoryPosition=secondFrame;else IoRead(&file,secondFrame,0);
        status=8;
    }
    pending.size=0;
    if(!settingsChanged)ResetFrameSettings(&gFlicFrameSettings);
    return result;












}

U32 Flic::Update(Surface *destination)
{
    if(!active)return 0;
    target=destination;
    U32 result=DecodeFrame();
    if(loops==0){status=4;result|=4;}
    if(status==8)result|=8;
    if(result&1)PlaySounds();
    if(currentFrame==1){status=0x10;result|=0x10;}
    return result;
}

int Flic::DecodePalette(unsigned char *data)
{
    short packets=*(short *)data;
    int index=0;
    data+=2;
    while(--packets >= 0){
        index+=*data++;
        int count=*data++;
        if(!count)count=256;
        for(int color=index;count;++color,--count){
            palette->colors[color].red=*data++;
            palette->colors[color].green=*data++;
            palette->colors[color].blue=*data++;
            palette->colors[color].reserved=0;
        }
    }
    memset(palette,0,4);
    return 1;
}

int Flic::UnpackPixels(unsigned char *destination,const void *data)
{
    return UnpackFlicPixels(destination,data);
}

int Flic::DecodePixels(const void *data)
{
    unsigned char *destination=target->pixels;return UnpackFlicPixels(destination,data);
}

int Flic::DecodeRegion(const void *data)
{const long *coordinates=(const long *)data;long left=coordinates[0],top=coordinates[1];unsigned char *destination=target->pixels;int result=UnpackFlicPixels(destination,coordinates+2);SurfaceCopyRegion(target,&bitmap,left,top);return result;




}

int Flic::DecodePackedPixels(const void *data)
{
    UnpackFlicPacked(gFlicDecode,data);
    unsigned char *destination=target->pixels;unsigned char *source=gFlicDecode;return UnpackFlicPixels(destination,source);
}

typedef char FlicHeaderSize[sizeof(FlicHeader)==128?1:-1];
typedef char FlicSoundSize[sizeof(FlicSound)==8?1:-1];
typedef char FlicSize[sizeof(Flic)==0x90?1:-1];
