extern "C" void *memcpy(void *, const void *, unsigned int);
struct Color { unsigned char blue, green, red, flags; Color() {} };
struct Palette { Color colors[256]; };
extern "C" void PaletteGet(Palette *p, Color *out, unsigned int index) { *out = p->colors[index]; }
extern "C" void PaletteRead(Palette *p, Color *out, unsigned int index, unsigned int count) { memcpy(out, p->colors + index, count * sizeof(Color)); }
extern "C" void PaletteSet(Palette *p, const Color *value, unsigned int index) { p->colors[index] = *value; }
extern "C" void PaletteCopyRange(Palette *p, const Color *source, unsigned int index, unsigned int count) { memcpy(p->colors + index, source, count * sizeof(Color)); }
struct PackedPalette { unsigned int offset; unsigned short first, count; };
extern "C" void DecodeLaunchPalette(Palette *p, const PackedPalette *source) {
    const char *data = (const char *)source + source->offset;
    unsigned int count = source->count;
    unsigned int first = source->first;
    memcpy(p->colors + first, data, count * sizeof(Color));
}
extern "C" unsigned int InterpolateColor(unsigned int a, unsigned int b, unsigned int step, unsigned int total)
{
    return a * (total - step) / total + b * step / total;
}
extern "C" void PaletteGradient(Palette *p, unsigned int first, unsigned int count)
{
    if (count < 2) return;
    Color a, b;
    PaletteGet(p, &a, first);
    PaletteGet(p, &b, first + count - 1);
    for (unsigned int i = 0; i < count; ++i) {
        p->colors[first + i].blue = InterpolateColor(a.blue, b.blue, i, count-1);
        p->colors[first + i].green = InterpolateColor(a.green, b.green, i, count-1);
        p->colors[first + i].red = InterpolateColor(a.red, b.red, i, count-1);
    }
}

extern "C" void PaletteFade(Palette *p, unsigned int first, unsigned int count, int amount, const Color *target)
{
    Color *entry = p->colors + first;
    while (count--) {
        entry->blue = InterpolateColor(entry->blue, target->blue, amount, 256);
        entry->green = InterpolateColor(entry->green, target->green, amount, 256);
        entry->red = InterpolateColor(entry->red, target->red, amount, 256);
        ++entry;
    }
}
extern "C" int ColorDistance(Palette *, const Color *, const Color *);
extern "C" int PaletteFindColor(Palette *p, const Color *color)
{
    int distance = -1;
    int result = 0;
    Color *entry = p->colors + 10;
    for (int i = 10; i < 246; ++i) {
        int value = ColorDistance(p, entry++, color);
        if (distance < 0 || value < distance) { distance = value; result = i; }
    }
    return result;
}
extern "C" int ColorDistance(Palette *p, const Color *a, const Color *b)
{

#define SQUARE(x) ((x)*(x))
    return SQUARE(((int)a->green-b->green)/4)*6 + SQUARE(((int)a->blue-b->blue)/4)*3 + SQUARE(((int)a->red-b->red)/4);
}





