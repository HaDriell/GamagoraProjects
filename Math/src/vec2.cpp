#include "vec2.h"
#include <math.h>

vec2::vec2() : x(0.f), y(0.f) {}
vec2::vec2(float scalar) : x(scalar), y(scalar) {}
vec2::vec2(float x, float y) : x(x), y(y) {}

vec2 operator-(const vec2& v)
{
    return vec2(-v.x, -v.y);
}

vec2& operator+=(vec2& left, const vec2& right)
{
    left.x += right.x;
    left.y += right.y;
    return left;
}

vec2& operator-=(vec2& left, const vec2& right)
{
    left.x -= right.x;
    left.y -= right.y;
    return left;
}

vec2& operator*=(vec2& left, const vec2& right)
{
    left.x *= right.x;
    left.y *= right.y;
    return left;

}

vec2& operator/=(vec2& left, const vec2& right)
{
    left.x /= right.x;
    left.y /= right.y;
    return left;
}

vec2& operator+=(vec2& left, float value)
{
    left.x += value;
    left.y += value;
    return left;
}

vec2& operator-=(vec2& left, float value)
{
    left.x -= value;
    left.y -= value;
    return left;
}

vec2& operator*=(vec2& left, float value)
{
    left.x *= value;
    left.y *= value;
    return left;
}

vec2& operator/=(vec2& left, float value)
{
    left.x /= value;
    left.y /= value;
    return left;
}


vec2 operator+(const vec2& left, const vec2& right)
{
    return vec2
    {
        left.x + right.x,
        left.y + right.y
    };
}

vec2 operator-(const vec2& left, const vec2& right)
{
    return vec2
    {
        left.x - right.x,
        left.y - right.y
    };
}

vec2 operator*(const vec2& left, const vec2& right)
{
    return vec2
    {
        left.x * right.x,
        left.y * right.y
    };
}

vec2 operator/(const vec2& left, const vec2& right)
{
    return vec2
    {
        left.x / right.x,
        left.y / right.y
    };
}

vec2 operator+(const vec2& left, float value)
{
    return vec2
    {
        left.x + value,
        left.y + value
    };
}

vec2 operator-(const vec2& left, float value)
{
    return vec2
    {
        left.x - value,
        left.y - value
    };
}

vec2 operator*(const vec2& left, float value)
{
    return vec2
    {
        left.x * value,
        left.y * value
    };
}

vec2 operator/(const vec2& left, float value)
{
    return vec2
    {
        left.x / value,
        left.y / value
    };
}

vec2 operator+(float value, const vec2& left)
{
    return left + value;
}

vec2 operator*(float value, const vec2& left)
{
    return left * value;
}

vec2 operator/(float value, const vec2& left)
{
    return left / value;
}

vec2 operator-(float value, const vec2& left)
{
    return left - value;
}

bool operator==(const vec2& left, const vec2& right)
{
    return left.x == right.x && left.y == right.y;
}

bool operator!=(const vec2& left, const vec2& right)
{
    return !(left == right);
}

float vec2::length() const
{
    return std::sqrt(length2());
}

float vec2::length2() const
{
    return x * x + y * y;
}

vec2 vec2::normalize() const
{
    float len = length();
    if (len)
        return *this / len;
    return vec2();
}

float vec2::distance(const vec2& v) const
{
    return std::sqrt(distance2(v));
}

float vec2::distance2(const vec2& v) const
{
    float dx = x - v.x;
    float dy = y - v.y;
    return dx * dx + dy * dy;
}

float vec2::dot(const vec2& v) const
{
    return x * v.x + y * v.y;
}

float vec2::cross(const vec2& v) const
{
    return x *v.y - y * v.x;
}

std::ostream& operator<<(std::ostream& stream, const vec2& v)
{
    stream << "vec2(" << v.x << ", " << v.y << ")";
    return stream;
}

const vec2 vec2::X = vec2(1, 0);
const vec2 vec2::Y = vec2(0, 1);