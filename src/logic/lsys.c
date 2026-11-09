struct LogicRect { int x, y, width, height; };
struct LogicPoint { int x, y; };

extern "C" void *memset(void *, int, unsigned int);
extern "C" void *memmove(void *, const void *, unsigned int);
extern "C" void *strcpy(char *, const char *);
extern "C" unsigned int strlen(const char *);
extern "C" void AssertFail(const char *, int, const char *);
extern "C" int ActorStateCall(void *);
extern "C" void *ActorName(void *);
extern "C" void ActorArrayGet(void *, int);
extern "C" void ActorArraySet(void *, int, int);
extern "C" void *MissionAllocateType(int);
extern "C" void *MissionAllocate(int, int);
extern "C" void *ActorListFirst(void *);
extern "C" void *ActorListNext(void *, void *);
extern "C" void *ActorListFind(void *, int);
extern "C" void *ActorFindAt(void *, int);
extern "C" int ActorInvoke(void *, int, int, void *);
extern "C" void ActorWrite(void *, int, int, void *);
extern "C" void ActorResize(void *, int, int);
extern "C" void ActorResizeMore(void *, int, int, void *);
extern "C" int LogicMissionData(int, int);
extern "C" void LogicRender(int, int, int, int, const char *, int, int, int,
                               int *, int *, int, int, int, int, int, int);
extern "C" void LogicFrame(void *, int, int);
extern "C" void LogicFrameInset(void *, int, int, int, int);
extern "C" void LogicDrawText(int, int, int, int, const char *, int, int, int, int,
                                int, int, int, int, int, int, int);
extern "C" int LogicDistance(int *, int *);
extern "C" int LogicResolveName(const char *, int *);
extern "C" void LogicDrawObject(const char *, int, int);
extern "C" void LogicDrawObjects(const char *, int, int, int, int, int, int);
extern "C" void LogicDrawMap(int, int *);
extern "C" int LogicFindActor(int);
extern "C" int LogicGetActor(int);
extern "C" void LogicSetActor(int, int, int);
extern "C" void LogicSetActorData(int, int, int, void *);
extern "C" void LogicDeleteActor(int, int);
extern "C" void LogicResizeActor(void *, int, int);
extern "C" void LogicResizeActorMore(void *, int, int, int, void *);
extern "C" int LogicLookupAt(int, int);
extern "C" void LogicLookupSet(int, int, int);
extern "C" void LogicCopyPoint(int *, int *);
extern "C" void LogicMapChange(int, int);
extern "C" void LogicMapOffset(int, int);
extern "C" int LogicNamedLookup(int, int);
extern "C" void LogicNamedDraw(const char *, int, int);
extern "C" void LogicNamedDrawEx(const char *, int, int, int, int, int, int);
extern "C" void LogicNamedLine(const char *, int, int, int, int, int, int);
extern "C" void LogicMapRect(void *, int, int, int, int);
extern "C" void LogicMapRectEx(void *, int, int, int, int, int);
extern "C" void LogicMapConnect(void);
extern "C" void LogicMapPosition(void *, int, int, int *);
extern "C" void LogicActorFlags(void *, int);
extern "C" void LogicActorStatus(void *, int);
extern "C" void LogicActorStatusEx(void *, int, int, void *);
extern "C" void LogicActorRemove(void *, int);
extern "C" void LogicMapInput(int, int);
extern "C" void LogicMapInputEx(int, int, int, void *);
extern "C" void LogicFindActorAt(void *, int);
extern "C" void LogicSetMissionPoint(int *, int *);
extern "C" void LogicSetPlayerMode(int, int);
extern "C" void LogicCallMode(int, int);
extern "C" void LogicClearMode(int, int);
extern "C" void LogicDeleteActorFromList(void *, int);
extern "C" void LogicActorCommand(void *, int, int, void *);
extern "C" int LogicActorIndex(const char *, int, int);
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
extern int logicActorList;
extern int logicMapBounds;
extern int logicFlagWord;
extern int logicMode;
extern int logicTextDirty;
extern int logicTextCursor;
extern char *logicTextWrite;
extern char *logicTextCurrent;
extern char logicTextBuffer[0x800];
extern LogicRect logicTextRect;
extern LogicPoint logicPointCache;
extern char logicFallbackName[];
extern char logicAlternateName[];
extern char logicHeaderText[];

void LogicNoop(void)
{
}

void LogicReset(void)
{
    memset(&logicCacheReady, 1, 0xe);
}

unsigned int LogicStateValue(void *state, int field)
{
    switch (field) {
    case 0:
        return *(unsigned int *)((char *)state + 0x54);
    case 1:
        return *(unsigned int *)((char *)state + 0x5c);
    case 2:
        if (((char *)&logicCacheReady)[field] != 0) {
            ((char *)&logicCacheReady)[field] = 0;
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
        if (((char *)&logicCacheReady)[field] != 0) {
            ((char *)&logicCacheReady)[field] = 0;
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
        return (unsigned char)logicMode;
    case 5:
        return *(int *)((char *)state + 0x2c) != 0 &&
               *(int *)((char *)state + 0x2c) != 2;
    case 6:
        return *(int *)((char *)state + 0x2c) == 2 ||
               *(int *)((char *)state + 0x2c) == 3;
    case 7:
        return *(unsigned int *)((char *)state + 0x44);
    case 8:
        return logicWorldOffsetX;
    case 9:
        return logicWorldOffsetY;
    case 10:
        return *(unsigned int *)((char *)state + 0x38);
    case 11:
        if (*(int *)((char *)state + 0x2c) != 0 &&
            *(int *)((char *)state + 0x2c) != 2)
            return *(unsigned int *)((char *)state + 0x34);
        return 0;
    case 12:
        return *(unsigned int *)((char *)state + 0x10);
    case 13:
        return logicMapBounds != logicFlagWord;
    }
    return 0;
}

void LogicSetActorMode(void *actor, int mode, int value)
{
    if (mode != 1 || (((unsigned char *)actor)[0] & 2) != 0)
        return;
    if (value <= 0)
        *(int *)((char *)actor + 0x5c) = 0;
    else {
        if (value >= 3)
            value = 3;
        *(int *)((char *)actor + 0x5c) = value;
    }
}

void *MissionAllocateFromData(void *data, int mode)
{
    if (mode <= 0)
        return MissionAllocate(*(int *)((char *)data + 4), *(int *)((char *)data + 8));
    if (mode == 1)
        return MissionAllocateType(*(int *)((char *)data + 4));
    return "$(]";
}

int LogicActorSearch(void *unused, int actorId, int mode, int flags, int *point)
{
    int result = 0;
    if (actorId == 0)
        return 0;
    int *usePoint = 0;
    if (point != 0) {
        logicPointCache.x = point[0] + logicWorldOffsetX;
        logicPointCache.y = point[1] + logicWorldOffsetY;
        if ((logicPointCache.x & 1) == 0)
            logicPointCache.y &= ~1;
        else
            logicPointCache.y |= 1;
        usePoint = &logicPointCache.x;
    }
    void *actor = ActorListFind(&logicActorList, actorId);
    if (actor != 0)
        result = (int)ActorInvoke(actor, mode, flags, usePoint);
    return result;
}

void LogicActorUpdate(void *unused, int actorId, int mode, int flags, int *point)
{
    void *actor = ActorListFind(&logicActorList, actorId);
    if (actor == 0)
        return;
    int *usePoint = 0;
    if (point != 0) {
        logicPointCache.x = point[0] + logicWorldOffsetX;
        logicPointCache.y = point[1] + logicWorldOffsetY;
        if ((logicPointCache.x & 1) == 0)
            logicPointCache.y &= ~1;
        else
            logicPointCache.y |= 1;
        usePoint = &logicPointCache.x;
    }
    ActorResizeMore(actor, mode, flags, usePoint);
}

void LogicConsoleResize(void)
{
    int width; if (*((char *)logicTextCurrent) != 0) {
        int height = logicRight - logicLeft + 1, top = logicBottom - logicTop - 0x1d; width = logicTop + 0xf; int measured;
        LogicRender(width, logicLeft, top, height, logicTextCurrent, 0, 0, 4, &measured, 0, 0, -1, 1, 0, 0, 0x7fffffff);
        logicTextRect.x = logicLeft + (height - measured) - 0xf; logicTextRect.y = measured; logicTextRect.width = width; logicTextRect.height = top; logicFlagWord |= 8;
    }
}

void LogicCheckMode(void) { if (logicMode != 2 && logicMode != 3) logicCacheReady = 0; }


void LogicTextAppend(void *unused, const char *text)
{
    if (logicCacheReady == 0)
        return;
    logicTextCursor = 0;
    if (*logicTextWrite != 0)
        logicTextWrite += strlen(logicTextWrite) + 1;
    int used = (int)logicTextWrite - (int)logicTextBuffer;
    int freeBytes = 0x800 - used;
    int needed = strlen(text) + 1;
    if (needed > freeBytes) {
        if (needed > 0x800)
            AssertFail(__FILE__, 0x10c, 0);
        int dropped = 0;
        while (freeBytes + dropped < needed) {
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
        char *p = logicTextBuffer;
        char *last = logicTextBuffer;
        while (p != logicTextCurrent) {
            unsigned int length = strlen(p);
            last = p;
            p += length + 1;
        }
        logicFlagWord |= 8;
        logicTextCurrent = last;
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
    if (*logicTextWrite != 0 || logicTextWrite != logicTextBuffer) {
        logicTextCurrent = logicTextWrite;
        *logicTextWrite = 0;
        if (logicTextCursor != 0) {
            logicTextCursor = 0;
            logicFlagWord |= 8;
        }
        if (logicTextCurrent != logicTextBuffer) {
            char *p = logicTextBuffer;
            char *last = logicTextBuffer;
            while (p != logicTextCurrent) {
                unsigned int length = strlen(p);
                last = p;
                p += length + 1;
            }
            logicFlagWord |= 8;
            logicTextCurrent = last;
        }
        LogicConsoleResize();
        logicTextWrite = logicTextCurrent;
        logicFlagWord |= 8;
    }
    LogicConsoleResize();
}

void LogicTextMark(void)
{
    if (*logicTextCurrent != 0 && logicTextCursor == 0) {
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
    return *logicTextCurrent != 0 && logicTextCursor == 0;
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
    int width = logicTextRect.width;
    int height = logicTextRect.height;
    if (*logicTextCurrent != 0 && logicTextCursor == 0) {
        LogicFrame(&logicScreenSurface, logicFrameColor, logicFrameShade);
        LogicFrameInset(&logicScreenSurface, x - 5, y - 5, width + 10, height + 10);
        LogicDrawText(x, y, width, height, logicTextCurrent,
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
    logicPointCache.x = point[0] + logicWorldOffsetX;
    logicPointCache.y = point[1] + logicWorldOffsetY;
    if ((logicPointCache.x & 1) == 0)
        logicPointCache.y &= ~1;
    else
        logicPointCache.y |= 1;
    int id = LogicResolveName(logicFallbackName, &logicPointCache.x);
    LogicNamedDraw(logicFallbackName, id, mode);
}

void LogicDrawFromPoint(void *unused, int *point, int mode, int width, int height,
                        int extra, int flags)
{
    logicPointCache.x = point[0] + logicWorldOffsetX;
    logicPointCache.y = point[1] + logicWorldOffsetY;
    if ((logicPointCache.x & 1) == 0)
        logicPointCache.y &= ~1;
    else
        logicPointCache.y |= 1;
    int id = LogicResolveName(logicFallbackName, &logicPointCache.x);
    LogicNamedDrawEx(logicFallbackName, id, mode, width, height, extra, flags);
}

void LogicConnectPoints(void *unused, int *from, int *to)
{
    logicPointCache.x = from[0] + logicWorldOffsetX;
    logicPointCache.y = from[1] + logicWorldOffsetY;
    if ((logicPointCache.x & 1) == 0)
        logicPointCache.y &= ~1;
    else
        logicPointCache.y |= 1;
    LogicResolveName(logicFallbackName, &logicPointCache.x);
    logicPointCache.x = to[0] + logicWorldOffsetX;
    logicPointCache.y = to[1] + logicWorldOffsetY;
    if ((logicPointCache.x & 1) == 0)
        logicPointCache.y &= ~1;
    else
        logicPointCache.y |= 1;
    LogicResolveName(logicFallbackName, &logicPointCache.x);
    LogicMapConnect();
}

void LogicDrawPoint(void *unused, int *point)
{
    logicPointCache.x = point[0] + logicWorldOffsetX;
    logicPointCache.y = point[1] + logicWorldOffsetY;
    if ((logicPointCache.x & 1) == 0)
        logicPointCache.y &= ~1;
    else
        logicPointCache.y |= 1;
    LogicResolveName(logicFallbackName, &logicPointCache.x);
    LogicMapConnect();
}

void LogicFindObject(void *unused, unsigned int *point, unsigned int id, int scale)
{
    int found = LogicActorIndex(logicFallbackName, id, scale * 0xb60b61);
    if (found != 0) {
        LogicSetMissionPoint((int *)point, (int *)found);
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
    logicPointCache.x = point[0] + logicWorldOffsetX;
    logicPointCache.y = point[1] + logicWorldOffsetY;
    if ((logicPointCache.x & 1) == 0)
        logicPointCache.y &= ~1;
    else
        logicPointCache.y |= 1;
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
        AssertFail(__FILE__, 0x212, 0);
    int *actorPoint = (int *)ActorName(actor);
    point[0] = actorPoint[0];
    point[1] = actorPoint[1];
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
        if (second != 0)
            LogicActorCommand((void *)first, 3, *(int *)((char *)second + 0x14), 0);
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
    logicPointCache.x = point[0] + logicWorldOffsetX;
    logicPointCache.y = point[1] + logicWorldOffsetY;
    if ((logicPointCache.x & 1) == 0)
        logicPointCache.y &= ~1;
    else
        logicPointCache.y |= 1;
    LogicMapPointUpdate(&logicActorList, actorId, mode, &logicPointCache.x);
}

void LogicMapViewPoint(void *unused, int actorId, int *point)
{
    logicPointCache.x = point[0] + logicWorldOffsetX;
    logicPointCache.y = point[1] + logicWorldOffsetY;
    if ((logicPointCache.x & 1) == 0)
        logicPointCache.y &= ~1;
    else
        logicPointCache.y |= 1;
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
