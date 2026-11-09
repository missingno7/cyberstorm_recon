// logic\sinterp.c - mission script interpreter ("sit%03d.cs" situation scripts). Names are working labels.
#include <stdio.h>
#include <string.h>
extern "C" void AssertFail(const char *file, int line, const char *message);
extern "C" unsigned int next_link_seed(unsigned int *seed); extern unsigned int MissionRandomTable;   // 0051445c
extern "C" char *BriefFormat(const char *format, ...);     // 0047d420 (vsprintf into a static buffer)
struct ScriptPoint { long x, y; ScriptPoint() {} ScriptPoint(long ax, long ay) : x(ax), y(ay) {} };
struct ResMgr { int CloseFile(); }; extern ResMgr gResMgr;  // io\resource.c
extern "C" int ResMgrOpen(ResMgr *self, const char *name, int required);
extern "C" unsigned long ResMgrGetFileLength(ResMgr *self);
#pragma aux ResMgrGetFileLength "f_c8e0";
extern "C" unsigned long ResMgrReadStream(ResMgr *self, void *buffer, unsigned long size);
#pragma aux ResMgrReadStream "f_c900";
struct FileRecord; unsigned long StreamRead(FileRecord *file, void *buffer, unsigned long size);
unsigned long StreamWrite(FileRecord *file, const void *buffer, unsigned long size);
extern char g4dd0c0[];              // the logic system object (logic\lsys.c)
#define gLogic g4dd0c0
void LogicTextAppend(void *logic, const char *text); void LogicEmpty(void *logic, const char *text);
void LogicFail(void *logic, const char *text);  /* 004635a7 */ void LogicRedraw(void *logic, long action);
void LogicOtherRedraw(void *logic, long action); void LogicSetActorMode(void *logic, long actor, long mode);
long LogicMapUpdate(void *logic, long kind, long side, ScriptPoint *at); long *LogicMapViewPoint(void *logic, long kind, ScriptPoint *at);
void LogicActorArraySet(void *logic, long actor, int index, long value); long LogicActorArrayGet(void *logic, long actor, long index);
void LogicActorUpdate(void *logic, long actor, int mode, long value, ScriptPoint *at); void LogicActorPosition(void *logic, long actor, ScriptPoint *at);
long LogicRefreshActor(void *logic, long actor); long LogicRemoveActor(void *logic, long actor);
long LogicCopyActor(void *logic, ScriptPoint *from, ScriptPoint *to); void LogicFindObject(void *logic, ScriptPoint *at, long kind, long range);
void LogicDrawFromPoint(void *logic, ScriptPoint *at, long a, long b, long c, long d);
void LogicConnectPoints(void *logic, ScriptPoint *from, ScriptPoint *to, long a, long b);
void LogicDrawPoint(void *logic, ScriptPoint *at, long a, long b, long c); long LogicDrawAtPoint(void *logic, ScriptPoint *at, long range);
long LogicActorSearch(void *logic, int mode, long actor, long value = 0, ScriptPoint *at = 0);
long LogicStateValue(void *logic, long index); char *MissionAllocateFromData(void *logic, long index);
struct ScriptHeader { unsigned long magic, version, startExpr, limitExpr, titleText, nameText, briefText; };   // 0x1c bytes
enum {                              // the section table: offsets relative to Script::base
    SecVars, SecTimers, SecPoints, SecTargets, SecActors, SecGroups, SecStrIndex, SecStrLists, SecFlags,
    SecCode, SecExprs = SecCode + 5, SecStrings                       // SecCode + state: code of each state
};
struct ScriptTarget { unsigned long ref, value; long pad[3]; };     // 0x14 bytes
struct ScriptGroup { long actors[8]; long count; };                   // 0x24 bytes
struct ScriptList { long offset, count; };
class Script { public:
    char *buffer; ScriptHeader *header; char *base; long *table;                            // + 0x00
    long *vars, *timers; ScriptPoint *points; ScriptTarget *targets;                          // + 0x10
    long *actors; ScriptGroup *groups; long *flags, saved, time, state, start, limit;       // + 0x20 (saved from + 0x2c)
    char title[0x320], name[0x50], brief[0x320]; unsigned long size; long savedEnd;           // + 0x40 .. + 0x6d4
    Script(); ~Script();
};
static char scriptText[0x800]; Script gScript;
unsigned short *ScriptStep(Script *s, unsigned short *ip, unsigned short *stop); long ScriptEval(Script *s, int expr);
int ScriptFormat(Script *s, char *dst, int max, const char *src); char *ScriptString(Script *s, long offset); int ScriptRun(Script *s);

Script::Script() : buffer(0) {}
Script::~Script() { if (buffer) { delete[] buffer; buffer = 0; } }
void ScriptFree(Script *s) { if (s->buffer) { delete[] s->buffer; s->buffer = 0; } }

void ScriptSetup(Script *s)
{
    s->header = (ScriptHeader *)s->buffer;
    if (!(s->header->magic == 0xabcdabcd && s->header->version == 5)) AssertFail(__FILE__, __LINE__, "old script -- remake");
    s->table = (long *)(s->base = s->buffer + sizeof(ScriptHeader));
    s->vars = (long *)(s->base + s->table[SecVars]);
    s->timers = (long *)(s->base + s->table[SecTimers]);
    s->points = (ScriptPoint *)(s->base + s->table[SecPoints]);
    s->targets = (ScriptTarget *)(s->base + s->table[SecTargets]);
    s->actors = (long *)(s->base + s->table[SecActors]);
    s->groups = (ScriptGroup *)(s->base + s->table[SecGroups]);
    s->flags = (long *)(s->base + s->table[SecFlags]);
}









void SafeCopy(char *dst, const char *src, unsigned int size)
{
    if (!(strlen(src) + 1 <= size)) AssertFail(__FILE__, __LINE__, 0);
    strcpy(dst, src);
}






void ScriptLoad(Script *s, int number, unsigned int seed)
{
    char name[40];

    ScriptFree(s);
    sprintf(name, "sit%03d.cs", number);
    if (!ResMgrOpen(&gResMgr, name, 0)) AssertFail(__FILE__, __LINE__, name);
    s->size = ResMgrGetFileLength(&gResMgr);
    s->buffer = new char[s->size];
    if (!s->buffer || ResMgrReadStream(&gResMgr, s->buffer, s->size) != s->size) AssertFail(__FILE__, __LINE__, name);
    gResMgr.CloseFile();
    ScriptSetup(s);
    MissionRandomTable = seed;
    s->time = 0;
    s->state = 0;
    ScriptRun(s);
    s->start = ScriptEval(s, s->header->startExpr);
    s->limit = ScriptEval(s, s->header->limitExpr);
    SafeCopy(s->title, ScriptString(s, s->header->titleText), sizeof(s->title));
    SafeCopy(s->name, ScriptString(s, s->header->nameText), sizeof(s->name));
    SafeCopy(s->brief, ScriptString(s, s->header->briefText), sizeof(s->brief));
}

int ScriptRestore(Script *s, FileRecord *file)
{
    unsigned long length;

    ScriptFree(s);
    length = (char *)&s->savedEnd - (char *)&s->saved;
    if (StreamRead(file, &s->saved, length) != length) return 0;
    s->buffer = new char[s->size];
    if (!s->buffer || StreamRead(file, s->buffer, s->size) != s->size) return 0;
    ScriptSetup(s);
    return 1;
}

int ScriptSave(Script *s, FileRecord *file)
{
    unsigned long length = (char *)&s->savedEnd - (char *)&s->saved;
    if (StreamWrite(file, &s->saved, length) != length) return 0;
    if (!s->buffer || StreamWrite(file, s->buffer, s->size) != s->size) return 0;
    return 1;
}

unsigned short *ScriptCode(Script *s, int offset)
{
    return (unsigned short *)(s->base + s->table[SecCode + s->state]) + offset;
}

ScriptPoint *ScriptRef(Script *s, int ref);







































long ScriptGetVar(Script *s, int var)
{
    long value = 0;
    if (var & 0xf800) {
        int kind = var & 0xf000;
        if (kind == 0x8000) value = ScriptRef(s, var & 0x7ff)->x;
        else if (kind == 0x9000) value = ScriptRef(s, var & 0x7ff)->y;
        else AssertFail(__FILE__, __LINE__, "Bad Reference");
    } else
        value = s->vars[var];
    return value;
}

void ScriptSetVar(Script *s, int var, long value)
{
    s->vars[var] = value;
}












ScriptPoint *ScriptRef(Script *s, int ref)
{
    static ScriptPoint result;
    int index, kind;
    long actor;
    ScriptPoint *pair;

    if (ref == 0) {
        result.x = 0;
        result.y = 0;
    } else if (ref & 0xf800) {
        kind = ref & 0xf000;
        index = ref & 0x7ff;
        if (kind == 0x6000) {
            pair = &s->points[index];
            result.x = ScriptEval(s, pair->x);
            result.y = ScriptEval(s, pair->y);
        } else if (kind == 0xa000) {
            actor = s->actors[index];
            if (actor)
                LogicActorPosition(gLogic, actor, &result);
            else
                result = ScriptPoint(0, 0);
        } else
            AssertFail(__FILE__, __LINE__, "Bad Reference");
    } else {
        result.x = s->points[ref].x; result.y = s->points[ref].y;
    }
    return &result;
}








void ScriptCommandTarget(Script *s, int kind, unsigned short *ip)
{
    ScriptPoint at;
    long command = ScriptEval(s, ip[1]);
    int mode = ip[2];
    long value = -1;
    int index;

    at.x = 0;
    at.y = 0;
    switch (mode) {
    case 0:
        break;
    case 1:
        at = *ScriptRef(s, ip[3]);
        break;
    case 2: {
        ScriptTarget *target = &s->targets[ip[3]];
        value = ScriptEval(s, target->value);
        at = *ScriptRef(s, target->ref);
        break;
    }
    case 3:
        value = s->actors[ip[3]];
        break;
    default:
        AssertFail(__FILE__, __LINE__, "Unknown Target");
    }
    index = ip[0];
    if (kind == 4) {
        LogicActorArraySet(gLogic, s->actors[index], 7, command);
        LogicActorUpdate(gLogic, s->actors[index], mode, value, &at);
    } else {
        ScriptGroup *group = &s->groups[index];
        for (int i = 0; i < group->count; i++) {
            LogicActorArraySet(gLogic, group->actors[i], 7, command);
            LogicActorUpdate(gLogic, group->actors[i], mode, value, &at);
        }
    }
}

char *ScriptFormatArgs(Script *s, unsigned short *args, int count)
{
    long values[15];
    char *text;
    int i;

    text = ScriptString(s, args[0]);
    for (i = 1; i < count; i++)
        values[i] = ScriptEval(s, args[i]);
    return BriefFormat(text, values[1], values[2], values[3], values[4], values[5], values[6], values[7],
                      values[8], values[9], values[10], values[11], values[12], values[13], values[14]);
}

























































unsigned short *ScriptStep(Script *s, unsigned short *ip, unsigned short *stop)
{
    unsigned short op;
    int count;
    unsigned short *next, *end, *start, *targets, *values;
    int flag, run, loops, cases, otherwise, i, index, set;
    long value, kind, side, actor, best, bestDistance, distance, a, b, c;
    long *slot, *list;
    ScriptGroup *group;
    ScriptPoint *p, *at;
    ScriptPoint here, from, to;

    op = *ip;
    count = op & 0x0f;
    ip++;
    next = ip + count;
    switch (op) {
    case 0x1060:                        // next state
        if (s->state != 2) s->state++;
        return 0;
    case 0x1070:                        // end script
        if (s->state != 3) {
            s->state = 3;
            return 0;
        }
        break;
    case 0x1013:                        // once (cond, flag, end)
        flag = ip[1];
        run = ScriptEval(s, ip[0]) && s->flags[flag] == 0;
        end = ScriptCode(s, ip[2]);
        if (run) {
            while (next && next != end) next = ScriptStep(s, next, stop);
            s->flags[flag] = 1;
        } else
            next = end;
        break;
    case 0x1023:                        // if (cond, else, end)
        if (ScriptEval(s, ip[0])) {
            end = ScriptCode(s, ip[1]);
            while (next && next != end) next = ScriptStep(s, next, stop);
            if (next) next = ScriptCode(s, ip[2]);
        } else {
            next = ScriptCode(s, ip[1]);
            end = ScriptCode(s, ip[2]);
            while (next && next != end) next = ScriptStep(s, next, stop);
        }
        break;
    case 0x1042:                        // while (cond, end)
        stop = ScriptCode(s, ip[1]);
        loops = 0;
        for (;;) {
            if (++loops > 500) AssertFail(__FILE__, __LINE__, "Infinite Loop!");
            if (!ScriptEval(s, ip[0])) break;
            start = next;
            while (next) {
                next = ScriptStep(s, next, stop);
                if (next == start) break;
            }
        }
        next = stop;
        break;
    case 0x1080:                        // break
        next = stop;
        break;
    case 0x1051:                        // goto
        next = ScriptCode(s, ip[0]);
        break;
    case 0x1032:                        // switch (value, table)
        value = ScriptEval(s, ip[0]);
        stop = ScriptCode(s, ip[1]);
        cases = *stop++;
        otherwise = *stop++;
        targets = stop;
        values = stop += cases;
        stop += cases;
        for (i = 0; i < cases; i++) {
            if (ScriptEval(s, values[i]) == value) {
                next = ScriptCode(s, targets[i]);
                break;
            }
        }
        if (i == cases) next = ScriptCode(s, otherwise);
        while (next && next != stop) next = ScriptStep(s, next, stop);
        break;
    case 0x1092:
        actor = ScriptEval(s, ip[0]);
        LogicSetActorMode(gLogic, actor, ScriptEval(s, ip[1]));
        break;
    case 0x10a2:
        index = ip[0];
        s->vars[index] = ScriptEval(s, ip[1]);
        break;
    case 0x10b2:
        ((long *)(s->base + s->table[SecStrIndex]))[ip[0]] = ip[1];
        break;
    case 0x2031:
        LogicRedraw(gLogic, ScriptEval(s, ip[0]));
        break;
    case 0x2041:
        LogicOtherRedraw(gLogic, ScriptEval(s, ip[0]));
        break;
    case 0x3012:                        // start timer
        index = ip[0];
        s->timers[index] = s->time + ScriptEval(s, ip[1]);
        break;
    case 0x3021:                        // clear timer
        s->timers[ip[0]] = 0;
        break;
    case 0x4014:                        // create actor
        index = ip[0];
        kind = ScriptEval(s, ip[1]);
        side = ScriptEval(s, ip[2]);
        s->actors[index] = LogicMapUpdate(gLogic, kind, side, ScriptRef(s, ip[3]));
        break;
    case 0x5013:                        // create group
        group = &s->groups[ip[0]];
        kind = ScriptEval(s, ip[1]);
        at = ScriptRef(s, ip[2]);
        group->count = 0;
        for (list = LogicMapViewPoint(gLogic, kind, at); *list != -1; list++)
            group->actors[group->count++] = *list;
        break;
    case 0x4024:                        // command actor
        ScriptCommandTarget(s, 4, ip);
        break;
    case 0x5024:                        // command group
        ScriptCommandTarget(s, 5, ip);
        break;
    case 0x4032:
        index = ip[0];
        LogicActorArraySet(gLogic, s->actors[index], 1, ScriptEval(s, ip[1]));
        break;
    case 0x5032:
        index = ip[0];
        value = ScriptEval(s, ip[1]);
        group = &s->groups[index];
        for (i = 0; i < group->count; i++)
            LogicActorArraySet(gLogic, group->actors[i], 1, value);
        break;
    case 0x4042:
        index = ip[0];
        LogicActorArraySet(gLogic, s->actors[index], 8, ScriptEval(s, ip[1]));
        break;
    case 0x5042:
        index = ip[0];
        value = ScriptEval(s, ip[1]);
        group = &s->groups[index];
        for (i = 0; i < group->count; i++)
            LogicActorArraySet(gLogic, group->actors[i], 8, value);
        break;
    case 0x4052:                        // first actor of a set
        slot = &s->actors[ip[0]];
        set = ip[1];
        if (set == 1)
            *slot = LogicRefreshActor(gLogic, 0);
        else if (set == 2)
            *slot = LogicRemoveActor(gLogic, 0);
        else {
            group = &s->groups[set];
            *slot = group->count > 0 ? group->actors[0] : 0;
        }
        break;
    case 0x4062:                        // next actor of a set
        slot = &s->actors[ip[0]];
        if (*slot == 0) break;
        actor = *slot;
        set = ip[1];
        if (set == 1)
            actor = LogicRefreshActor(gLogic, actor);
        else if (set == 2)
            actor = LogicRemoveActor(gLogic, actor);
        else {
            group = &s->groups[set];
            for (i = 0; i < group->count; i++)
                if (actor == group->actors[i]) break;
            i++;
            actor = i < group->count ? group->actors[i] : 0;
        }
        *slot = actor;
        break;
    case 0x4073:                        // nearest actor of a set
        slot = &s->actors[ip[0]];
        set = ip[1];
        at = ScriptRef(s, ip[2]);
        best = 0;
        if (set == 1) {
            for (actor = LogicRefreshActor(gLogic, best); actor; actor = LogicRefreshActor(gLogic, actor)) {
                LogicActorPosition(gLogic, actor, &here);
                distance = LogicCopyActor(gLogic, at, &here);
                if (!best || distance < bestDistance) {
                    best = actor;
                    bestDistance = distance;
                }
            }
        } else if (set == 2) {
            for (actor = LogicRemoveActor(gLogic, best); actor; actor = LogicRemoveActor(gLogic, actor)) {
                LogicActorPosition(gLogic, actor, &here);
                distance = LogicCopyActor(gLogic, at, &here);
                if (!best || distance < bestDistance) {
                    best = actor;
                    bestDistance = distance;
                }
            }
        } else {
            group = &s->groups[set];
            for (i = 0; i < group->count; i++) {
                actor = group->actors[i];
                if (!actor) continue;
                LogicActorPosition(gLogic, actor, &here);
                distance = LogicCopyActor(gLogic, at, &here);
                if (!best || distance < bestDistance) {
                    best = actor;
                    bestDistance = distance;
                }
            }
        }
        *slot = best;
        break;
    case 0x4082:
        index = ip[0];
        s->actors[index] = ScriptEval(s, ip[1]);
        break;
    case 0x6013:                        // set point
        p = &s->points[ip[0]];
        p->x = ScriptEval(s, ip[1]);
        p->y = ScriptEval(s, ip[2]);
        break;
    case 0x6022:
        index = ip[0];
        s->points[index] = *ScriptRef(s, ip[1]);
        break;
    case 0x6032:
        index = ip[0];
        p = ScriptRef(s, ip[1]);
        s->points[index].x += p->x;
        s->points[index].y += p->y;
        break;
    case 0x6042:
        index = ip[0];
        p = ScriptRef(s, ip[1]);
        s->points[index].x -= p->x;
        s->points[index].y -= p->y;
        break;
    case 0x6053:
        index = ip[0];
        kind = ScriptEval(s, ip[1]);
        LogicFindObject(gLogic, &s->points[index], kind, ScriptEval(s, ip[2]));
        break;
    case 0x7013:                        // set target
        s->targets[ip[0]].ref = ip[1];
        s->targets[ip[0]].value = ip[2];
        break;
    case 0x8015:
        p = ScriptRef(s, ip[0]);
        a = ScriptEval(s, ip[1]);
        b = ScriptEval(s, ip[2]);
        c = ScriptEval(s, ip[3]);
        LogicDrawFromPoint(gLogic, p, a, b, c, ScriptEval(s, ip[4]));
        break;
    case 0x8024:
        from = *ScriptRef(s, ip[0]);
        to = *ScriptRef(s, ip[1]);
        a = ScriptEval(s, ip[2]);
        LogicConnectPoints(gLogic, &from, &to, a, ScriptEval(s, ip[3]));
        break;
    case 0x8034:
        here = *ScriptRef(s, ip[0]);
        a = ScriptEval(s, ip[1]);
        b = ScriptEval(s, ip[2]);
        LogicDrawPoint(gLogic, &here, a, b, ScriptEval(s, ip[3]));
        break;
    default:
        switch (op & 0xfff0) {
        case 0x2060:                    // fail mission with a message
            LogicFail(gLogic, ScriptFormatArgs(s, ip, count));
            if (s->state != 3) {
                s->state = 3;
                return 0;
            }
            break;
        case 0x2020:                    // print
            LogicTextAppend(gLogic, ScriptFormatArgs(s, ip, count));
            break;
        case 0x2050:
            LogicEmpty(gLogic, ScriptFormatArgs(s, ip, count));
            break;
        case 0x2010:
            break;
        default:

















































































































































































































































































































































































































            AssertFail(__FILE__, __LINE__, BriefFormat("Unknown opcode 0x%04x", op));
        }
    }
    return next;
}

void ScriptRunState(Script *s);

void ScriptAbort(Script *s)
{
    if (s->buffer && s->state != 4) {
        s->state = 3;
        ScriptRun(s);
    }
}

void ScriptTick(Script *s)
{
    s->time++;
}

void ScriptRunState(Script *s)
{
    unsigned short *ip;
    if (s->state == 3) LogicTextAppend(gLogic, "");
    ip = (unsigned short *)(s->base + s->table[SecCode + s->state]);
    while (ip) ip = ScriptStep(s, ip, 0);
}

int ScriptRun(Script *s)
{
    if (!s->buffer || s->state == 4) return 0;
    ScriptRunState(s);
    if (s->state == 3) ScriptRunState(s);
    return 1;
}


























































































long *ScriptCall(Script *s, unsigned short op, long *sp)
{
    long result = 0;

    if ((op & 0xf000) == 0xc000) {
        int more = (op & 0x0f) - 1;
        long who = *sp;
        sp = sp + 1;
        switch (op) {
        case 0xc011:
            result = s->time >= s->timers[who];
            break;
        case 0xc021:
            result = LogicActorSearch(gLogic, 3, s->actors[who]);
            break;
        case 0xc031:
            result = LogicActorSearch(gLogic, 0, s->actors[who]);
            break;
        case 0xc071:                    // every actor of the set passes the test
            if (who == 1) {
                long actor;
                result = 1;
                actor = LogicRefreshActor(gLogic, 0);
                while (actor) {
                    result = LogicActorSearch(gLogic, 0, actor);
                    if (!result) break;
                    actor = LogicRefreshActor(gLogic, actor);
                }
            } else if (who == 2) {
                long actor;
                result = 1;
                actor = LogicRemoveActor(gLogic, 0);
                while (actor) {
                    result = LogicActorSearch(gLogic, 0, actor);
                    if (!result) break;
                    actor = LogicRemoveActor(gLogic, actor);
                }
            } else {
                ScriptGroup *group;
                result = 1;
                group = &s->groups[who];
                for (int i = 0; i < group->count; i++) {
                    long actor = group->actors[i];
                    result = LogicActorSearch(gLogic, 0, actor);
                    if (!result) break;
                }
            }
            break;
        case 0xc041:
            result = LogicActorSearch(gLogic, 2, s->actors[who]);
            break;
        case 0xc081:                    // some actor of the set passes the test
            if (who == 1) {
                long actor = LogicRefreshActor(gLogic, result);
                while (actor) {
                    result = LogicActorSearch(gLogic, 2, actor);
                    if (result) break;
                    actor = LogicRefreshActor(gLogic, actor);
                }
            } else if (who == 2) {
                long actor = LogicRemoveActor(gLogic, result);
                while (actor) {
                    result = LogicActorSearch(gLogic, 2, actor);
                    if (result) break;
                    actor = LogicRemoveActor(gLogic, actor);
                }
            } else {
                ScriptGroup *group = &s->groups[who];
                for (int i = 0; i < group->count; i++) {
                    long actor = group->actors[i];
                    result = LogicActorSearch(gLogic, 2, actor);
                    if (result) break;
                }
            }
            break;
        case 0xc052: {
            ScriptTarget *target = &s->targets[*sp];
            result = LogicActorSearch(gLogic, 1, s->actors[who], ScriptEval(s, target->value), ScriptRef(s, target->ref));
            break;
        }
        case 0xc092: {
            ScriptTarget *target = &s->targets[*sp];
            long value; ScriptPoint *at = ScriptRef(s, target->ref);
            value = ScriptEval(s, target->value);
            if (who == 1) {
                long actor = LogicRefreshActor(gLogic, result);
                while (actor) {
                    result = LogicActorSearch(gLogic, 1, actor, value, at);
                    if (result) break;
                    actor = LogicRefreshActor(gLogic, actor);
                }
            } else if (who == 2) {
                long actor = LogicRemoveActor(gLogic, result);
                while (actor) {
                    result = LogicActorSearch(gLogic, 1, actor, value, at);
                    if (result) break;
                    actor = LogicRemoveActor(gLogic, actor);
                }
            } else {
                ScriptGroup *group = &s->groups[who];
                for (int i = 0; i < group->count; i++) {
                    long actor = group->actors[i];
                    result = LogicActorSearch(gLogic, 1, actor, value, at);
                    if (result) break;
                }
            }
            break;
        }
        case 0xc062:
            result = LogicActorArrayGet(gLogic, s->actors[who], *sp);
            break;
        case 0xc0a2: {
            ScriptPoint *from = ScriptRef(s, who);
            result = LogicCopyActor(gLogic, from, ScriptRef(s, *sp));
            break;
        }
        case 0xc0b1: {
            ScriptTarget *target = &s->targets[who];
            ScriptPoint *at = ScriptRef(s, target->ref);
            result = LogicDrawAtPoint(gLogic, at, ScriptEval(s, target->value));
            break;
        }
        default:
            AssertFail(__FILE__, __LINE__, "unknown function");
        }
        sp += more;
    } else if ((op & 0xfff0) == 0xb030) {
        int pairs = (op & 0x0f) - 1;
        long total = *sp;
        long pick = next_link_seed(&MissionRandomTable) % total;
        long *p = sp = sp + 1 + pairs;
        pairs >>= 1;
        while (pairs-- > 0) {
            long value = *--p;
            long weight = *--p;
            if (weight < 0) {
                long top;
                pick = value * total / -weight;
                top = total - 1;
                if (pick <= 0) pick = 0;
                else if (pick >= top) pick = top;
            } else {
                pick -= weight;
                if (pick < 0) {
                    result = value;
                    break;
                }
            }
        }
    } else {
        int n = op & 0x0f;
        switch (op) {
        case 0xb011:
            result = LogicStateValue(gLogic, *sp);
            break;
        case 0xb021: {
            long range = *sp;
            if (range > 1) result = next_link_seed(&MissionRandomTable) % range;
            break;
        }
        default:





















            AssertFail(__FILE__, __LINE__, "unknown function");
        }
        sp += n;
    }
    *--sp = result;
    return sp;
}











long *ScriptEvalOp(Script *s, unsigned short op, long *sp)
{
    long right;

    if ((op & 0xf000) == 0xb000 || (op & 0xf000) == 0xc000)
        sp = ScriptCall(s, op, sp);
    else {
        if ((op & 0x0f) == 2) {
            right = *sp;
            sp++;
        }
        switch (op) {
        case 0x1011: *sp = -*sp; break;
        case 0x1021: *sp = ~*sp; break;
        case 0x1031: *sp = !*sp; break;
        case 0x2012: *sp += right; break;
        case 0x2022: *sp -= right; break;
        case 0x2032: *sp *= right; break;
        case 0x2042:
            if (right == 0) AssertFail(__FILE__, __LINE__, "divide by zero");
            *sp /= right; break;
        case 0x2052: *sp %= right; break;
        case 0x2062: *sp &= right; break;
        case 0x2072: *sp ^= right; break;
        case 0x2082: *sp |= right; break;
        case 0x2092: *sp <<= right; break;
        case 0x20a2: *sp >>= right; break;
        case 0x3012: *sp = *sp < right; break;
        case 0x3022: *sp = *sp <= right; break;
        case 0x3032: *sp = *sp > right; break;
        case 0x3042: *sp = *sp >= right; break;
        case 0x3052: *sp = *sp == right; break;
        case 0x3062: *sp = *sp != right; break;
        case 0x4012: *sp = *sp && right; break;
        case 0x4022: *sp = *sp || right; break;
        case 0xa011: *sp = ScriptGetVar(s, *sp); break;
        default:
            AssertFail(__FILE__, __LINE__, "bad opcode");
        }
    }
    return sp;
}


long ScriptEval(Script *s, int expr)
{
    long *sp;
    long stack[32];
    unsigned long mask;
    int count;
    unsigned short *code;
    unsigned long bit;
    int i;

    if (!expr) return 0;
    sp = &stack[32];
    code = (unsigned short *)(s->base + s->table[SecExprs]) + expr;
    count = *code++;
    if (count == 0) return (short)*code;
    mask = *code++;
    for (bit = 1, i = 0; i < count; bit += bit, i++) {
        unsigned short word = *code++;
        if (mask & bit)
            sp = ScriptEvalOp(s, word, sp);
        else
            *--sp = (short)word;
    }
    if (!(sp == &stack[31])) AssertFail(__FILE__, __LINE__, 0);
    return *sp;
}










































int ScriptFormat(Script *s, char *dst, int max, const char *src)
{
    char number[20];
    int len = 0;
    int part;

    while (*src) {
        if (*src == -1) {
            long index = *(long *)(src + 1);
            src += 5;
            part = ScriptFormat(s, dst, max - len, MissionAllocateFromData(gLogic, ScriptEval(s, index)));
        } else if (*src == -4) {
            long index = *(long *)(src + 1);
            src += 5;
            sprintf(number, "%d", ScriptEval(s, index));
            part = ScriptFormat(s, dst, max - len, number);
        } else if (*src == -2) {
            long *strIndex = (long *)(s->base + s->table[SecStrIndex]);
            long index = *(long *)(src + 1);
            src += 5;
            part = ScriptFormat(s, dst, max - len, s->base + s->table[SecStrings] + strIndex[index]);
        } else if (*src == -3) {
            ScriptList *lists = (ScriptList *)(s->base + s->table[SecStrLists]);
            long index = *(long *)(src + 1);
            long pick = ScriptEval(s, *(long *)(src + 5));
            ScriptList *list;
            const char *text;
            src += 9;
            list = &lists[index];
            text = s->base + s->table[SecStrings] + list->offset;
            if (pick >= 0 && pick < list->count)
                while (pick-- > 0) text += strlen(text) + 1;
            part = ScriptFormat(s, dst, max - len, text);
        } else {
            if (len < max) {
                *dst++ = *src++;
                len++;
            } else
                AssertFail(__FILE__, __LINE__, "string overflow");
            continue;
        }
        dst += part;
        len += part;
    }
    return len;
}

char *ScriptString(Script *s, long offset)
{
    int len = ScriptFormat(s, scriptText, sizeof(scriptText) - 1, s->base + s->table[SecStrings] + offset);
    scriptText[len] = 0;
    return scriptText;
}
