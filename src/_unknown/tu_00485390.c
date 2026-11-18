#include <windows.h>
#include <string.h>
#pragma function(memcpy)

struct Palette {
    WORD version, count;
    PALETTEENTRY entries[256];

    Palette();
    ~Palette();
    void SetEntries(const PALETTEENTRY *);
    void FromRgb(const RGBQUAD *);
    void GetEntries(PALETTEENTRY *);
    void ToRgb(RGBQUAD *);
};

Palette::Palette()
{
    version = 0x300;
    count = 256;
    memset(entries, 0, count * sizeof(PALETTEENTRY));
}

Palette::~Palette()
{
}

void Palette::SetEntries(const PALETTEENTRY *source)
{
    memcpy(entries, source, count * sizeof(PALETTEENTRY));
}

void Palette::FromRgb(const RGBQUAD *source)
{
    PALETTEENTRY *entry = entries;
    for (int i = 0; i < 256; ++i, ++entry, ++source) {
        RGBQUAD color = *source;
        entry->peRed = color.rgbRed;
        entry->peGreen = color.rgbGreen;
        entry->peBlue = color.rgbBlue;
        entry->peFlags = PC_NOCOLLAPSE;
    }
}

void Palette::GetEntries(PALETTEENTRY *destination)
{
    memcpy(destination, entries, count * sizeof(PALETTEENTRY));
}

void Palette::ToRgb(RGBQUAD *destination)
{
    const PALETTEENTRY *entry = entries;
    for (int i = 0; i < 256; ++i, ++entry, ++destination) {
        destination->rgbRed = entry->peRed;
        destination->rgbGreen = entry->peGreen;
        destination->rgbBlue = entry->peBlue;
        destination->rgbReserved = 0;
    }
}
