/* TU at 0042a598..0042a5fc (cs11). Original file name unknown (no __FILE__ evidence). Complete object.
 * Names and layouts are working labels; the code is evidence. Profile 'plain' (-4s -s). */

unsigned long TimeNow(void);

class Rate {
public:
    unsigned long count;
    unsigned long last;
    unsigned long interval;
    Rate(unsigned long n);
    ~Rate();
    void Reset();
    unsigned short Get();
};

Rate::Rate(unsigned long n)
{
    interval = n * 10;
    if (interval < 1)
        interval = 1;
    last = TimeNow();
}

Rate::~Rate()
{
}

void Rate::Reset()
{
    last = TimeNow();
    count = 0;
}

unsigned short Rate::Get()
{
    count = (TimeNow() - last) / interval;
    return count;
}
