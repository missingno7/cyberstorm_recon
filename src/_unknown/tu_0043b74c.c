#include <string.h>

struct WorldRecord {
    int field00, worldId;
    char settings[0x60];
};

struct ResMgr {
    int *Load(char *name, unsigned flags = 1, int *buffer = 0, unsigned bufferSize = 0);
};

extern ResMgr gResMgr;
extern char g4c9234[];
void ReleasePoolData(void *);

inline WorldRecord *LoadWorlds()
{
    return (WorldRecord *)gResMgr.Load("WORLDS.DAT");
}

void SimLoadPalette(int index, void *dest)
{
    WorldRecord *worlds = LoadWorlds();
    if (index == 99) {
        index = 24;
        worlds[index].worldId = 99;
    }
    memcpy(dest, &worlds[index], sizeof(WorldRecord));
    ReleasePoolData(worlds);
}

extern "C" void HercPrepareAssets()
{
    WorldRecord *worlds = LoadWorlds();
    memcpy(g4c9234, &worlds[23], sizeof(WorldRecord));
    ReleasePoolData(worlds);
}
