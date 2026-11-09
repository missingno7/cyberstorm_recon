extern "C" void AssertFail(const char *, int, const char *);
extern "C" unsigned int UnpackFlicPixels(void *, const void *);
extern "C" unsigned int UnpackFlicPacked(void *, const void *);
extern "C" unsigned int UnpackLzw(void *, const void *);
struct UnpackBuffer { unsigned int size; void *data; };
extern UnpackBuffer unpackScratch; static char unpackSourceFile[] = __FILE__;
extern "C" unsigned int DecodeResourceBlock(int method, const void *input, UnpackBuffer *output)
{
    unsigned int size = 0;
    if (method & 0xf0) {
        DecodeResourceBlock((method >> 4) & 15, input, &unpackScratch);
        input = unpackScratch.data;
    }
    switch ((unsigned int)method & 15) {
    case 1: size = UnpackLzw(output->data, input); break;
    case 12: size = UnpackFlicPixels(output->data, input); break;
    case 13: return 0;
    case 14: size = UnpackFlicPacked(output->data, input); break;
    }
    if (size == 0 || size > output->size) {










































        AssertFail(unpackSourceFile, __LINE__, 0);
    }
    return size;
}
