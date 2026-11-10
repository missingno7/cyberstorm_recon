static char mapalertSourceFile[] = __FILE__; struct TargetTriple { int x, y, z; };
struct TargetRecord { unsigned char bytes[0xb4]; };
struct TargetBriefObject { char pad[0x10]; int active; int IsActive() const { return active != 0; } };
inline int TargetSpecialFlag(const unsigned char &x) { return (x & 16) != 0; } inline int TargetInput(int x) { return x; } struct TargetModuleState { int mode; int IsOpen() const { return mode != 0; } };
extern "C" void BriefModuleSetup(void *, int, int, int, int); extern "C" void BriefModuleOpen(void *, int, int, int); extern "C" void PopLoadResource(void *, void **, int, int);
extern "C" void BriefObjectOpen(void *, const char *); extern "C" void BriefObjectLoadResource(void *, int, void *, int, int); extern "C" int BriefGetHandle(void *, void *); extern "C" int BriefStateSet(void *);
extern "C" void BriefRenderUpdate(void *); extern "C" void SimSetCursor(int, int); extern "C" void LogicCallMode(int, void *); extern "C" int MapPickAt(void *, void *); extern "C" int MapNearestAt(void *, void *); extern "C" void SimHoverActor(void *, int, int);
extern "C" void SimSetMode(void *, int); extern "C" void SimSetView(void *, int); extern "C" void TargetObjectInit(const char *, void *, int); extern "C" void TargetInitAt(const char *, void *, int, int);
extern "C" void BestLocSetData(void *, void *, void *); extern "C" int CreateState(int); extern "C" int GridClamp(int); extern "C" int TargetActorById(int); extern "C" int TargetDistance(void *, void *); extern "C" int TargetSqrt(int);
extern "C" int TargetCompute(const char *, int, int, int, void *); extern "C" int TargetObjectData(int); extern "C" void *memset(void *, int, unsigned int); extern "C" void AssertFail(const char *, int, const char *);
extern int targetPrompt6, targetObject, gFastTransform, targetFallback, targetLimits, gInA0, targetSharedState;
extern long gInA4; extern unsigned char targetFlag;
extern TargetModuleState targetMode;
extern TargetTriple *targetDefinitionTable;
extern char targetFallbackName[], targetMenuResource[], targetBriefResource[];
void TargetEmpty(void);
typedef unsigned long U32; struct GuiControl;
// Opaque class storage sizes are evidence from the six constructor destinations.
struct GuiRect { long x,y,width,height; }; struct GuiPoint; struct GuiControlData:GuiRect { unsigned char style[4]; U32 attributes,shortcut; };
class GuiControl:public GuiControlData { public: GuiControl(U32,U32); virtual ~GuiControl(); virtual void Reset(unsigned); virtual void Draw(unsigned); virtual int Event(unsigned,U32); virtual int HitTest(const GuiPoint *); }; class SimMapGadget:public GuiControl { public: U32 state; long kind,data; SimMapGadget(U32,long,long,U32); void Reset(unsigned); void Draw(unsigned); int Event(unsigned,U32); int HitTest(const GuiPoint *); };
#pragma aux target_image_constructor "W?$ct:SimImageGadget$n(llul)_";
class PilotImageButton:public GuiControl { public: U32 state; long image,repeat; __pragma("target_image_constructor") PilotImageButton(long,long,U32); void Reset(unsigned); void Draw(unsigned); int Event(unsigned,U32); int HitTest(const GuiPoint *); };
// The alert popup: a screen over five static gadgets. Its destructor is compiler generated (last function, 0045a900).
struct InputReq; class GuiScreen : public GuiRect { public: InputReq *input; unsigned char unknown14[0x38]; GuiScreen(GuiControl **, unsigned, U32); ~GuiScreen() { input = 0; } }; class TargetPopup : public GuiScreen { public: TargetPopup(); };
SimMapGadget targetPrompt(13,1,0,0);
PilotImageButton targetPrompt1(133,500,0),targetPrompt2(136,500,0),targetPrompt3(139,500,0),targetPrompt4(142,500,0);
TargetPopup targetPrompt5; static GuiControl *targetControls[5] = { &targetPrompt, &targetPrompt1, &targetPrompt2, &targetPrompt4, &targetPrompt3 };
TargetPopup::TargetPopup() : GuiScreen(targetControls, 5, 0) { }
void TargetMapAlert(void *object, int callback)
{
    if (!((TargetBriefObject *)object)->IsActive()) { BriefObjectOpen(object, "MAPALERT.BOX"); BriefObjectLoadResource(object, callback, targetBriefResource, 0xc9, 0); }
}
void TargetClearActive(void *object) { if (((TargetBriefObject *)object)->IsActive()) *(int *)((char *)object + 0x10) = 0; }
void TargetClose(void *object) { if (((TargetBriefObject *)object)->IsActive()) { int handle = BriefGetHandle(object, &targetSharedState); if (handle >= 0) targetLimits = handle + 100; BriefStateSet(object); } }
void TargetUpdate(void *object) { if (((TargetBriefObject *)object)->IsActive()) BriefRenderUpdate(object); }
void TargetHandleEvent(void *object)
{
    int state; if (targetMode.IsOpen()) { int handle = BriefGetHandle(&targetPrompt5, &targetSharedState); if (handle >= 0) targetLimits = handle + 100; state = BriefStateSet(&targetPrompt5); } else state = 0;
    if (--state < 0) return;
    switch (state) {
    case 0:
        if (!TargetSpecialFlag(targetFlag)) { int mode = 0xb; if (TargetInput(gInA4) != 0) mode = 6; else if (TargetInput(gInA0) != 0) mode = 7; SimSetCursor(mode, 0); targetLimits = mode + 0xf; return; }
        { int target = MapPickAt((void *)&gFastTransform, &targetSharedState); if (target == 0) target = MapNearestAt((void *)&gFastTransform, &targetSharedState); SimHoverActor(object, target, 0xb); return; }
    case 1: SimSetMode(object, 1); LogicCallMode(9, (void *)0); return;
    case 2: SimSetMode(object, -1); LogicCallMode(9, (void *)0); return;
    case 3: SimSetView(object, -1); return;
    case 4: SimSetView(object, 1); return;
    default: return;
    }
}

