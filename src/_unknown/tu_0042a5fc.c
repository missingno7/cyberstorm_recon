/* Range-counter object, 0042a5fc..0042a6c9; no path evidence. */
class RangeCounter {
public:
    unsigned short value,first,last,step;
    void SetRange(unsigned short,unsigned short,unsigned short);
    RangeCounter();
    ~RangeCounter();
    void Set(unsigned short);
    void Increment();
    void Decrement();
    void Next();
    void Previous();
};

void RangeCounter::SetRange(unsigned short lo,unsigned short hi,unsigned short n)
{
    first=lo;last=hi;value=n;
}
RangeCounter::RangeCounter()
{
    first=0;last=65535;value=0;step=1;
}
RangeCounter::~RangeCounter() {}
void RangeCounter::Set(unsigned short n)
{
    if(n<first||n>last)n=first;
    value=n;
}
void RangeCounter::Increment()
{
    if(value<last)value+=step;else value=first;
}
void RangeCounter::Decrement()
{
    if(value>first)value-=step;else value=last;
}
void RangeCounter::Next()
{
    if(value<last)value+=step;else value=first;
}
void RangeCounter::Previous()
{
    if(value>first)value-=step;else value=last;
}

