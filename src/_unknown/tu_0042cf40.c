// Working labels: shortest-path workspace. No source path survives in this range.
struct SearchPoint { int x, y; SearchPoint &operator+=(const SearchPoint &p) { x += p.x; y += p.y; return *this; } };
struct SearchWorkspace {
    void *grid;
    int reserved[4];
    int radius, count, origin;
    SearchPoint center;
    int capacity;
    int *cost, *previous;
    int start, maxRadius;
};
extern int BestLocInitialize(void *, const char *, int);
extern int BestLocSetCandidate(void *, void *);
extern void DisplaySetRadius(int *, int);
extern int AreaFind(int *, int);
extern int AreaIndexToPoint(int *, int);

void SearchRelease(SearchWorkspace *);
extern int *AreaGetPoint(int *, int *, int);
extern "C" int ResolveName(const char *, const char *);
extern "C" void MemoryRelease(void *);
extern void *MemoryAllocationRetry(unsigned long);
extern int f_00432c00(void *, int, int);
extern SearchPoint SearchNeighbours[6];

void SearchInitialize(SearchWorkspace *self, const char *name, int radius)
{
    self = (SearchWorkspace *)BestLocInitialize(self, name, radius);
    self->cost = 0;
    self->maxRadius = radius;
    self->previous = 0;
    self->capacity = self->count + 1;
}

SearchWorkspace *SearchDestroy(SearchWorkspace *self)
{
    SearchRelease(self);
    return self;
}

int SearchAllocate(SearchWorkspace *self)
{
    SearchRelease(self);
    self->cost = new int[self->capacity];
    self->previous = new int[self->capacity];
    if (self->cost && self->previous) return 1;
    return 0;
}

void SearchRelease(SearchWorkspace *self)
{
    if (self->cost) delete[] self->cost;
    if (self->previous) delete[] self->previous;
    self->cost = 0;
    self->previous = 0;
}

void BestLocSetup(void *object, int origin, int radius)
{
    SearchWorkspace *self = (SearchWorkspace *)object;
    if (!radius) radius = self->maxRadius;
    else {
        int maximum = self->maxRadius;
        if (radius <= 1) radius = 1;
        else if (radius >= maximum) radius = maximum;
    }
    BestLocSetCandidate(self, (void *)origin);
    DisplaySetRadius((int *)self, radius);
    self->start = AreaFind((int *)self, origin);
    self->cost[0] = -10001;
    self->previous[0] = 0;
    int count = self->count;
    for (int i = 1; i <= count; ++i) {
        self->cost[i] = -10000;
        self->previous[i] = 0;
    }
    int best = 0;
    for (int current = self->start; current; current = best, best = 0) {
        SearchPoint point;
        AreaGetPoint((int *)self, (int *)&point, current);
        int location = ResolveName((const char *)self->grid, (const char *)&point);
        self->cost[current] = -self->cost[current];
        if (self->cost[current] == 10000) self->cost[current] = 0;
        SearchPoint *end = SearchNeighbours + 6;
        for (SearchPoint *delta = SearchNeighbours; delta != end; ++delta) {
            SearchPoint neighbour = point;
            neighbour += *delta;
            int nextLocation = ResolveName((const char *)self->grid, (const char *)&neighbour);
            int next = AreaFind((int *)self, nextLocation);
            if (next && self->cost[next] < 0) {
                int distance = f_00432c00(self->grid, nextLocation, location);
                if (distance > 0) {
                    distance += self->cost[current];
                    int newCost = -distance;
                    if (newCost > self->cost[next]) {
                        self->cost[next] = newCost;
                        self->previous[next] = current;
                    }
                }
                if (self->cost[best] < self->cost[next]) best = next;
            }
        }
        int *entry = self->cost;
        for (int index = 1; index <= count; ++index) {
            ++entry;
            if (*entry < 0 && self->cost[best] < *entry) best = index;
        }
    }
}

int SearchGetPath(SearchWorkspace *self, int location, int *path, int capacity)
{
    if (self->start && location) {
        int index = AreaFind((int *)self, location);
        if (index) {
            int length = 0;
            while (index != self->start) {
                index = self->previous[index]; ++length;
                if (!index) return 0;
            }
            if (length > 0 && length <= capacity) {
                int *out = path + length;
                *out = 0;
                index = AreaFind((int *)self, location);
                for (--out, --length; out != path - 1; --out, --length) {
                    *out = AreaIndexToPoint((int *)self, index);
                    index = self->previous[index];
                }
                return 1;
            }
        }
    }
    return 0;
}

int SearchGetCost(SearchWorkspace *self, int location)
{
    if (self->start && location) {
        int index = AreaFind((int *)self, location);
        if (index) {
            int cost = self->cost[index];
            int length = 0;
            while (index != self->start) {
                index = self->previous[index];
                ++length;
                if (!index) return 0;
            }
            if (length > 0) return cost;
        }
    }
    return 0;
}

