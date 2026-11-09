static unsigned int gameClock;
struct GameTimer { unsigned int start, end, duration; };
extern "C" unsigned int TimerNow() { return gameClock; }
extern "C" void GameClockSet(unsigned int time) { gameClock = time; }
extern "C" void GameTimerSet(GameTimer *timer, unsigned int duration)
{
    timer->start = gameClock;
    timer->end = timer->start + duration; timer->duration = duration;
}
extern "C" void GameTimerNext(GameTimer *timer, unsigned int duration)
{
    timer->start = timer->end;
    timer->end = timer->start + duration; timer->duration = duration;
}
extern "C" int GameTimerExpired(GameTimer *timer) { return gameClock >= timer->end; }
extern "C" unsigned int GameTimerElapsed(GameTimer *timer) { return gameClock - timer->start; }
extern "C" unsigned int GameTimerRemaining(GameTimer *timer)
{
    unsigned int elapsed = gameClock - timer->start;
    if (elapsed < timer->duration) return timer->duration - elapsed;
    return 0;
}

