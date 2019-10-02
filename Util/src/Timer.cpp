#include "Timer.h"



void Timer::reset()
{
    anchor = Clock::now();
}

float Timer::elaped()
{
    Duration duration = std::chrono::duration_cast<Duration>(Clock::now() - anchor);
    return duration.count();
}