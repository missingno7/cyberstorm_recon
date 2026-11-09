#include <stdarg.h>
extern "C" int vsprintf(char *, const char *, va_list);
static int formatBusy;
static char *formatBuffer;
static int formatBufferLength;
extern "C" void FormatSetBuffer(char *p, int n) { formatBuffer = p; formatBufferLength = n; }
extern "C" char *BriefFormat(const char *format, ...)
{
    if (formatBusy) return "";
    if (formatBuffer && formatBuffer != format) {
        va_list args;
        va_start(args, format);
        vsprintf(formatBuffer, format, args);
        va_end(args);
    }
    formatBusy = 0;
    return formatBuffer;
}
