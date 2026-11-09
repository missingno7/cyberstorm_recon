unsigned long TimeNow();
unsigned int MissionRandomTable;
struct RandomInitializer { RandomInitializer() { MissionRandomTable = 0x12345678; } };
static RandomInitializer randomInitializer;
extern "C" unsigned int next_link_seed(unsigned int *seed)
{
    unsigned int x = *seed;
    *seed = (((x >> 31) ^ (x >> 6) ^ (x >> 4) ^ (x >> 2) ^ (x >> 1) ^ x) & 1) << 31 | (x >> 1);
    return *seed;
}
extern "C" int RandomRange(unsigned int *seed, int low, int high)
{
    if (low < high) return low + next_link_seed(seed) % (high - low + 1);
    return low;
}
void f_0047e200(int *seed) { *seed = TimeNow(); }
extern "C" int RandomBiased(unsigned int *seed, int low, int high, int percent, int bias)
{
    if (high < low) { int t = high; high = low; low = t; }
    int range = high - low + 1;
    int target = percent * range / 100;
    int best = 0;
    int distance = 0x7fffffff;
    int tries = (!bias || next_link_seed(seed) % 15 > (unsigned int)bias) ? 1 : bias / 5 + 2;
    for (int i = 0; i < tries; ++i) {
        int value = next_link_seed(seed) % range;
        int delta = value > target ? value - target : target - value;
        if (delta < distance) { distance = delta; best = value; }
    }
    return best + low;
}
void FxStirSeed(unsigned int *seed) { for (int i = 0; i < 33; ++i) next_link_seed(seed); }

