#include "util.h"

float clamp(float min, float max, float value)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float lerp(float start, float end, float percent)
{
    return start + percent * (end - start);
}
