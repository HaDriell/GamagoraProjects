#include "Util.h"
#include <cmath>
#include <algorithm>


float deg2rad(float degrees)
{
    return degrees * PI / 180.0f;
}


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


std::string to_string(int value, int digits)
{
    unsigned int uvalue = value;
    if (value < 0)
        uvalue = -uvalue;

    std::string result;
    while (digits-- > 0)
    {
        result += ('0' + uvalue % 10);
        uvalue /= 10;
    }
    if (value < 0)
    {
        result += '-';
    }

    std::reverse(result.begin(), result.end());
    return result;
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');

    if (first == std::string::npos)
        return str;

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}