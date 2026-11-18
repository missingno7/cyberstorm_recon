#include <string.h>

static char commaSourceFile[] = __FILE__;
static char numberBuffer[16];

char *MenuFormatNumber(long value, char *buffer, long length)
{
    if (!buffer) {
        buffer = numberBuffer;
        length = sizeof(numberBuffer);
    }

    int digits = 0;
    long pos = length - 1;
    memset(buffer, ' ', length);
    buffer[pos--] = '\0';
    for (;;) {
        long previous = value;
        if (!value && digits)
            break;
        value /= 10;
        if (pos >= 0)
            buffer[pos--] = '0' + previous % 10;
        ++digits;
        if (digits % 3 == 0 && value && pos >= 0)
            buffer[pos--] = ',';
    }
    return buffer;
}
