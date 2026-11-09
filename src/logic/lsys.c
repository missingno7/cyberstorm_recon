struct LogicRect { int x, y, width, height; };
struct LogicPoint { int x, y; LogicPoint() {} };
struct LogicActorModeRecord { unsigned char reserved[0x5c]; int mode; };
struct LogicStateModeRegion { char padding[0x28]; int value; };
struct LogicStateRecord { int prefix; LogicStateModeRegion mode; };
static const char logicSourcePath[] = __FILE__;

extern "C" void *memset(void *, int, unsigned int); extern "C" void *memmove(void *, const void *, unsigned int);
extern "C" void *strcpy(char *, const char *); extern "C" unsigned int strlen(const char *);
extern "C" void AssertFail(const char *, int, const char *); extern "C" int ActorStateCall(void *);
extern "C" void *ActorName(void *); extern "C" void ActorArrayGet(void *, int);
extern "C" void ActorArraySet(void *, int, int); extern "C" void *MissionAllocateType(int);
extern "C" void *MissionAllocate(int, int); extern "C" void *ActorListFirst(void *);
extern "C" void *ActorListNext(void *, void *); extern "C" void *ActorListFind(void *, int);
extern "C" void *ActorFindAt(void *, int); extern "C" int ActorInvoke(void *, int, int, void *);
extern "C" void ActorWrite(void *, int, int, void *); extern "C" void ActorResize(void *, int, int);
extern "C" void ActorResizeMore(void *, int, int, void *); extern "C" int LogicMissionData(int, int);
extern "C" void BriefRender(int, int, int, int, const char *, int, int, int,
                              int *, int, int, int, int, int, int, int);
extern "C" void MenuSetColors(void *, int, int); extern "C" void MenuFill(void *, int, int, int, int);
extern "C" void LogicFrame(void *, int, int); extern "C" void LogicFrameInset(void *, int, int, int, int);
extern "C" void LogicDrawText(int, int, int, int, const char *, int, int, int, int,
                                int, int, int, int, int, int, int);
extern "C" int LogicDistance(int *, int *); extern "C" int ResolveName(const char *, int *);
extern "C" void f_00434a20(const char *, int, int); extern "C" void f_00434bd0(const char *, int, int, int, int, int);
extern "C" void f_00434c70(const char *, int, int, int, int); extern "C" void LogicDrawMap(int, int *);
extern "C" int LogicFindActor(int); extern "C" int LogicGetActor(int);
extern "C" void LogicSetActor(int, int, int); extern "C" void LogicSetActorData(int, int, int, void *);
extern "C" void LogicDeleteActor(int, int); extern "C" void LogicResizeActor(void *, int, int);
extern "C" void LogicResizeActorMore(void *, int, int, int, void *); extern "C" int LogicLookupAt(int, int);
extern "C" void LogicLookupSet(int, int, int); extern "C" void LogicCopyPoint(int *, int *);
extern "C" void LogicMapChange(int, int); extern "C" void LogicMapOffset(int, int);
extern "C" int LogicNamedLookup(int, int); extern "C" void LogicNamedLine(const char *, int, int, int, int, int, int);
extern "C" void LogicMapRect(void *, int, int, int, int); extern "C" void LogicMapRectEx(void *, int, int, int, int, int);
extern "C" void LogicMapConnect(void); extern "C" void LogicMapPosition(void *, int, int, int *);
extern "C" void LogicActorFlags(void *, int); extern "C" void LogicActorStatus(void *, int);
extern "C" void LogicActorStatusEx(void *, int, int, void *); extern "C" void LogicActorRemove(void *, int);
extern "C" void LogicMapInput(int, int); extern "C" void LogicMapInputEx(int, int, int, void *);
extern "C" void LogicFindActorAt(void *, int); extern "C" void BestLocSetData(void *, void *, void *);
extern "C" void LogicSetPlayerMode(int, int); extern "C" void LogicCallMode(int, int);
extern "C" void LogicClearMode(int, int); extern "C" void LogicDeleteActorFromList(void *, int);
extern "C" void LogicActorCommand(void *, int, int, void *); extern "C" int GridFindClosest(const char *, unsigned int, int);
extern "C" void *LogicActorIndexAt(void *, int);
extern "C" int LogicGetNamedPoint(const char *, int *);
extern "C" int LogicLookupRange(const char *, int, int);
extern "C" void LogicMapPoint(int *, int *, int);
extern "C" void LogicMapPointLine(int *, int *, int, int);
extern "C" void LogicMapPointUpdate(void *, int, int, int *);
extern "C" void LogicMapPointChange(void *, int, int *);
extern "C" void LogicDeleteFromMap(void *, int);
extern "C" void LogicRefreshMap(void *, int);
extern "C" void LogicMapViewUpdate(void *, int, int *);

extern int logicCacheReady;
extern char logicStateFlags[];
extern int logicCacheCount;
extern int logicCacheTotal;
extern int logicCacheValue;
extern int logicMissionByteCount;
extern char *logicMissionBytes;
extern int logicWorldOffsetX;
extern int logicWorldOffsetY;
extern int logicLeft;
extern int logicRight;
extern int logicTop;
extern int logicBottom;
extern int logicFrameColor;
extern int logicFrameShade;
extern int logicScreenSurface;
extern int logicFrameSurface;
extern int g513888;
extern int gMenuColors;
extern int f_004bc4e4;
extern int logicActorList;
extern int logicMapBounds;
extern int logicFlagWord;
extern int logicMode;
extern unsigned char g4c9248;
extern int logicTextDirty;
extern int logicTextCursor;
extern char *logicTextWrite;
extern char *logicTextCurrent;
extern char logicTextBuffer[0x800];
extern LogicRect logicTextRect;
extern LogicPoint logicPointCache;
extern char fallbackName[];
extern char logicAlternateName[];
extern char logicHeaderText[];

inline LogicPoint &PointCache(void)
{
    static LogicPoint point;
    return point;
}

void LogicNoop(void)
{
}

void LogicReset(void)
{
    memset(logicStateFlags, 1, 0xe);
}

unsigned int __cdecl LogicStateValue(void *state, int field)
{
    switch (field) {
    case 0:
        return *(unsigned int *)((char *)state + 0x54);
    case 1:
        return *(unsigned int *)((char *)state + 0x5c);
    case 2:
        if (logicStateFlags[field] != 0) {
            logicStateFlags[field] = 0;
            int count = 0;
            for (int i = 0; i <= logicMissionByteCount; ++i) {
                if (((signed char *)logicMissionBytes)[i] != 0)
                    ++count;
            }
            if (count <= 0)
                count = 0;
            else if (count >= 0xba1)
                count = 0xba1;
            logicCacheTotal = (count * 100 + 0x5d0) / 0xba1;
        }
        return logicCacheTotal;
    case 3:
        if (logicStateFlags[field] != 0) {
            logicStateFlags[field] = 0;
            logicCacheCount = 0;
            void *actor = ActorListFirst(&logicActorList);
            while (actor != 0) {
                if (*(int *)actor != 0 && (((unsigned char *)actor)[0xc] & 1) != 0)
                    logicCacheCount += ActorStateCall(actor);
                actor = ActorListNext(&logicActorList, actor);
            }
        }
        return logicCacheCount;
    case 4:
        return g4c9248;
    case 5:
        return ((LogicStateRecord *)state)->mode.value != 0 &&
               ((LogicStateRecord *)state)->mode.value != 2;
    case 6:
        return ((LogicStateRecord *)state)->mode.value == 2 ||
               ((LogicStateRecord *)state)->mode.value == 3;
    case 7:
        return *(unsigned int *)((char *)state + 0x44);
    case 8:
        return logicWorldOffsetX;
    case 9:
        return logicWorldOffsetY;
    case 10:
        return *(unsigned int *)((char *)state + 0x38);
    case 11:
        if (((LogicStateRecord *)state)->mode.value != 0 &&
            ((LogicStateRecord *)state)->mode.value != 2)
            return *(unsigned int *)((char *)state + 0x34);
        return 0;
    case 12:
        return *(unsigned int *)((char *)state + 0x10);
    case 13:
        return logicMapBounds != logicFlagWord;
    }
    return 0;
}

void __cdecl LogicSetActorMode(LogicActorModeRecord *actor, int mode, int value)
{
    if (mode == 1) {
        if ((actor->reserved[0] & 2) == 0) {
            if (value < 1) value = 0;
            else if (value > 2) value = 3;
            actor->mode = value;
        }
    } else return;
    return;
}

void *MissionAllocateFromData(register int *data, unsigned int mode)
{
    switch (mode) {
    case 0: return MissionAllocate(data[1], data[2]);
    case 1: return MissionAllocateType(data[1]);
    default: return "$(]";
    }
}

int LogicActorSearch(void *unused, int mode, int actorId, int flags, int *point)
{
    int result = 0;
    if (actorId != 0) {
        if (point != 0) {
            PointCache();
            logicPointCache = *(LogicPoint *)point;
            logicPointCache.x += logicWorldOffsetX;
            logicPointCache.y += logicWorldOffsetY;
            if ((logicPointCache.x & 1) != 0)
                logicPointCache.y |= 1;
            else
                logicPointCache.y &= ~1;
            point = &logicPointCache.x;
        }
        void *actor = ActorListFind(&logicActorList, actorId);
        if (actor != 0)
            result = (int)ActorInvoke(actor, mode, flags, point);
    }
    return result;
}

void LogicActorUpdate(void *unused, int actorId, int mode, int flags, int *point)
{
    void *actor = ActorListFind(&logicActorList, actorId);
    if (actor == 0)
        return;
    if (point != 0) {
        PointCache();
        logicPointCache = *(LogicPoint *)point;
        logicPointCache.x += logicWorldOffsetX;
        logicPointCache.y += logicWorldOffsetY;
        if ((logicPointCache.x & 1) != 0)
            logicPointCache.y |= 1;
        else
            logicPointCache.y &= ~1;
        point = &logicPointCache.x;
    }
    LogicActorCommand(actor, mode, flags, point);
}

void LogicConsoleResize(void)
{
    if (*logicTextCurrent) {
        LogicRect scratch;
        volatile int renderedHeight;
        scratch.x = logicTop + 0xf;
        scratch.y = logicLeft;
        scratch.width = logicBottom - logicTop - 0x1d;
        scratch.height = logicRight - logicLeft + 1;
        BriefRender(scratch.x, scratch.y, scratch.width, scratch.height, logicTextCurrent,
                    0, 0, 4, (int *)&renderedHeight, 0, 0, -1, 1, 0, 0, 0x7fffffff);
        logicTextRect.x = scratch.x;
        logicTextRect.y = scratch.y + (scratch.height - renderedHeight) - 0xf;
        logicTextRect.width = scratch.width;
        logicTextRect.height = renderedHeight;
        logicFlagWord |= 8;
    }
}

void LogicCheckMode(void)
{
    int state = logicMode == 2 || logicMode == 3;
    if (!state) logicCacheReady = state;
}


void LogicTextAppend(void *unused, const char *text)
{
    if (logicCacheReady == 0)
        return;
    logicTextCursor = 0;
    if (*logicTextWrite)
        logicTextWrite += strlen(logicTextWrite) + 1;
    int used = (int)logicTextWrite - (int)logicTextBuffer;
    int freeBytes = 0x800 - used;
    int needed = strlen(text) + 1;
    if (needed > freeBytes) {
        if ((unsigned int)needed > 0x800U)
            AssertFail(logicSourcePath, 0x10c, "message too long");
        int dropped = 0;
        for (;;) { if (needed <= freeBytes + dropped) break;
            int length = strlen(logicTextBuffer + dropped);
            dropped += length + 1;
        }
        memmove(logicTextBuffer, logicTextBuffer + dropped, 0x800 - dropped);
        logicTextWrite -= dropped;
    }
    strcpy(logicTextWrite, text);
    logicTextCurrent = logicTextWrite;
    LogicConsoleResize();
    logicFlagWord |= 8;
}

void LogicTextPrevious(void)
{
    if (logicTextCursor != 0) {
        logicTextCursor = 0;
        logicFlagWord |= 8;
    }
    if (logicTextCurrent != logicTextBuffer) {
        char *last = logicTextBuffer;
        char *p = logicTextBuffer;
        while (p != logicTextCurrent) {
            last = p;
            unsigned int length = strlen(p);
            p += length + 1;
        }
        logicTextCurrent = last;
        logicFlagWord |= 8;
    }
    LogicConsoleResize();
}

void LogicTextNext(void)
{
    if (logicTextCursor != 0) {
        logicTextCursor = 0;
        logicFlagWord |= 8;
    }
    if (logicTextCurrent != logicTextWrite) {
        unsigned int length = strlen(logicTextCurrent);
        logicTextCurrent += length + 1;
        logicFlagWord |= 8;
    }
    LogicConsoleResize();
}

void LogicTextClear(void)
{
    if (logicTextCursor != 0) {
        logicTextCursor = 0;
        logicFlagWord |= 8;
    }
    if (*logicTextWrite || logicTextWrite != logicTextBuffer) {
        logicTextCurrent = logicTextWrite;
        *logicTextWrite = 0;
        if (logicTextCursor != 0) {
            logicTextCursor = 0;
            logicFlagWord |= 8;
        }
        if (logicTextCurrent != logicTextBuffer) {
            char *last = logicTextBuffer;
            char *p = logicTextBuffer;
            while (p != logicTextCurrent) {
                last = p;
                unsigned int length = strlen(p);
                p += length + 1;
            }
            logicTextCurrent = last;
            logicFlagWord |= 8;
        }
        LogicConsoleResize();
        logicTextWrite = logicTextCurrent;
        logicFlagWord |= 8;
    }
    LogicConsoleResize();
}
void LogicTextMark(void)
{
    int shouldMark = *logicTextCurrent && logicTextCursor == 0;
    if (shouldMark) {
        logicTextCursor = 1;
        logicFlagWord |= 8;
    }
}
void LogicTextBegin(void)
{
    logicTextWrite = logicTextBuffer;
    logicTextCurrent = logicTextBuffer;
    logicTextBuffer[0] = 0;
    logicFlagWord |= 8;
    LogicConsoleResize();
    logicCacheReady = 1;
    logicTextCursor = 0;
}
char *LogicTextPointer(void)
{
    return logicTextWrite;
}
int LogicTextHasCurrent(void)
{
    return *logicTextCurrent && logicTextCursor == 0;
}
void LogicEmpty(void)
{
}
int LogicZero(void)
{
    return 0;
}
void LogicDrawConsole(void)
{
    int x = logicTextRect.x;
    int y = logicTextRect.y;
    volatile int width = logicTextRect.width;
    int height = logicTextRect.height;
    if (*logicTextCurrent && logicTextCursor == 0) {
        MenuSetColors(&g513888, f_004bc4e4, gMenuColors);
        MenuFill(&g513888, x - 5, y - 5, width + 10, height + 10);
        BriefRender(x, y, width, height, logicTextCurrent,
                    0, 0, 4, 0, 0, 0, 10, 1, 0, 0, 0x7fffffff);
    }
}
void LogicRedraw(int unused, int action)
{
    LogicCallMode(action, 0);
}
void LogicOtherRedraw(int unused, int action)
{
    LogicClearMode(action, 0);
}
void LogicDrawAtPoint(void *unused, int *point, int mode)
{
    PointCache();
    logicPointCache = *(LogicPoint *)point;
    logicPointCache.x += logicWorldOffsetX;
    logicPointCache.y += logicWorldOffsetY;
    if ((logicPointCache.x & 1) != 0)
        logicPointCache.y |= 1;
    else
        logicPointCache.y &= ~1;
    int id = ResolveName(fallbackName, &logicPointCache.x);
    f_00434a20(fallbackName, id, mode);
}
void LogicDrawFromPoint(void *unused, int *point, int mode, int width, int height,
                        int flags)
{
    PointCache();
    logicPointCache = *(LogicPoint *)point;
    logicPointCache.x += logicWorldOffsetX;
    logicPointCache.y += logicWorldOffsetY;
    if ((logicPointCache.x & 1) != 0)
        logicPointCache.y |= 1;
    else
        logicPointCache.y &= ~1;
    int id = ResolveName(fallbackName, &logicPointCache.x);
    f_00434bd0(fallbackName, id, mode, width, height, flags);
}
void LogicConnectPoints(void *unused, int *from, int *to, int firstMode, int secondMode)
{
    PointCache();
    logicPointCache = *(LogicPoint *)from;
    logicPointCache.x += logicWorldOffsetX;
    logicPointCache.y += logicWorldOffsetY;
    if ((logicPointCache.x & 1) != 0)
        logicPointCache.y |= 1;
    else
        logicPointCache.y &= ~1;
    int first = ResolveName(fallbackName, &logicPointCache.x);
    PointCache();
    logicPointCache = *(LogicPoint *)to;
    logicPointCache.x += logicWorldOffsetX;
    logicPointCache.y += logicWorldOffsetY;
    if ((logicPointCache.x & 1) != 0)
        logicPointCache.y |= 1;
    else
        logicPointCache.y &= ~1;
    int second = ResolveName(fallbackName, &logicPointCache.x);
    f_00434c70(fallbackName, first, second, firstMode, secondMode);
}
void LogicDrawPoint(void *unused, int *point, int mode, int width, int height)
{
    PointCache();
    logicPointCache = *(LogicPoint *)point;
    logicPointCache.x += logicWorldOffsetX;
    logicPointCache.y += logicWorldOffsetY;
    if ((logicPointCache.x & 1) != 0)
        logicPointCache.y |= 1;
    else
        logicPointCache.y &= ~1;
    int id = ResolveName(fallbackName, &logicPointCache.x);
    f_00434c70(fallbackName, id, mode, width, height);
}
void LogicFindObject(void *unused, unsigned int *point, unsigned int id, int scale)
{
    int found = GridFindClosest(fallbackName, id, scale * 0xb60b61);
    if (found != 0) {
        BestLocSetData(fallbackName, point, (void *)found);
        point[0] -= logicWorldOffsetX;
        point[1] -= logicWorldOffsetY;
    }
}
void LogicCopyActor(void *unused, int *destination, int *source)
{
    LogicCopyPoint(destination, source);
}
int *LogicTransformPoint(void *unused, int *point)
{
    PointCache();
    logicPointCache = *(LogicPoint *)point;
    logicPointCache.x += logicWorldOffsetX;
    logicPointCache.y += logicWorldOffsetY;
    if ((logicPointCache.x & 1) != 0)
        logicPointCache.y |= 1;
    else
        logicPointCache.y &= ~1;
    return &logicPointCache.x;
}
/*
 * Source-line evidence: this call carries immediate 0x212 for original line 530.
 * This comment-only block preserves that source location.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
void LogicActorPosition(void *unused, int actorId, int *point)
{
    void *actor = ActorListFind(&logicActorList, actorId);
    if (actor == 0)
        AssertFail(logicSourcePath, 0x212, 0);
    *(LogicPoint *)point = *(LogicPoint *)ActorName(actor);
    point[0] -= logicWorldOffsetX;
    point[1] -= logicWorldOffsetY;
}

void LogicActorArrayGet(void *unused, int actorId, int index, int *value)
{
    void *actor = ActorListFind(&logicActorList, actorId);
    if (actor != 0)
        ActorArrayGet(actor, index);
}

void LogicActorArraySet(void *unused, int actorId, int index, int value)
{
    void *actor = ActorListFind(&logicActorList, actorId);
    if (actor != 0)
        ActorArraySet(actor, index, value);
}

void LogicActorDelete(void *unused, int actorId)
{
    LogicDeleteActorFromList(&logicActorList, actorId);
}

void LogicSetActorLink(void *unused, int firstId, int parameter, int secondId)
{
    int first = (int)LogicActorIndexAt(&logicActorList, firstId);
    if (first != 0) {
        ActorArraySet((void *)first, 7, parameter);
        int second = (int)LogicActorIndexAt(&logicActorList, secondId);
        if (second != 0) {
            int command = *(int *)((char *)second + 0x14);
            LogicActorCommand((void *)first, 3, command, 0);
        }
    }
}

void LogicSetActorLinkAt(void *unused, int actorId, int value, int *point, int mode)
{
    int actor = (int)LogicActorIndexAt(&logicActorList, actorId);
    if (actor != 0) {
        ActorArraySet((void *)actor, 7, value);
        LogicActorCommand((void *)actor, 2, mode, point);
    }
}

void LogicMapUpdate(void *unused, int actorId, int mode, int *point)
{
    PointCache();
    logicPointCache = *(LogicPoint *)point;
    logicPointCache.x += logicWorldOffsetX;
    logicPointCache.y += logicWorldOffsetY;
    if ((logicPointCache.x & 1) != 0)
        logicPointCache.y |= 1;
    else
        logicPointCache.y &= ~1;
    LogicMapPointUpdate(&logicActorList, actorId, mode, &logicPointCache.x);
}

void LogicMapViewPoint(void *unused, int actorId, int *point)
{
    PointCache();
    logicPointCache = *(LogicPoint *)point;
    logicPointCache.x += logicWorldOffsetX;
    logicPointCache.y += logicWorldOffsetY;
    if ((logicPointCache.x & 1) != 0)
        logicPointCache.y |= 1;
    else
        logicPointCache.y &= ~1;
    LogicMapViewUpdate(&logicActorList, actorId, &logicPointCache.x);
}

void LogicRemoveActor(void *unused, int actorId)
{
    LogicDeleteFromMap(&logicActorList, actorId);
}

void LogicRefreshActor(void *unused, int actorId)
{
    LogicRefreshMap(&logicActorList, actorId);
}
