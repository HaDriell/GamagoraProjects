#include "vec4.h"
#include <math.h>

    vec4::vec4() : x(0), y(0), z(0), w(w) {}
    vec4::vec4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
    vec4::vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

vec4 operator-(vec4& v)
{
    return vec4(-v.x, -v.y, -v.z, -v.w);
}
vec4& operator+=(vec4& left, const vec4& right)
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;
    left.w += right.w;
    return left;
}

vec4& operator-=(vec4& left, const vec4& right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;
    left.w -= right.w;
    return left;
}

vec4& operator*=(vec4& left, const vec4& right)
{
    left.x *= right.x;
    left.y *= right.y;
    left.z *= right.z;
    left.w *= right.w;
    return left;
}

vec4& operator/=(vec4& left, const vec4& right)
{
    left.x /= right.x;
    left.y /= right.y;
    left.z /= right.z;
    left.w /= right.w;
    return left;
}


vec4& operator+=(vec4& left, float value)
{
    left.x += value;
    left.y += value;
    left.z += value;
    left.w += value;
    return left;
}

vec4& operator-=(vec4& left, float value)
{
    left.x -= value;
    left.y -= value;
    left.z -= value;
    left.w -= value;
    return left;
}

vec4& operator*=(vec4& left, float value)
{
    left.x *= value;
    left.y *= value;
    left.z *= value;
    left.w *= value;
    return left;
}

vec4& operator/=(vec4& left, float value)
{
    left.x /= value;
    left.y /= value;
    left.z /= value;
    left.w /= value;
    return left;
}


vec4 operator+(const vec4& left, const vec4& right)
{
    return vec4
    {
        left.x + right.x,
        left.y + right.y,
        left.z + right.z,
        left.w + right.w
    };
}

vec4 operator-(const vec4& left, const vec4& right)
{
    return vec4
    {
        left.x - right.x,
        left.y - right.y,
        left.z - right.z,
        left.w - right.w
    };
}

vec4 operator*(const vec4& left, const vec4& right)
{
    return vec4
    {
        left.x * right.x,
        left.y * right.y,
        left.z * right.z,
        left.w * right.w
    };
}

vec4 operator/(const vec4& left, const vec4& right)
{
    return vec4
    {
        left.x / right.x,
        left.y / right.y,
        left.z / right.z,
        left.w / right.w
    };
}


vec4 operator+(const vec4& left, float value)
{
    return vec4
    {
        left.x + value,
        left.y + value,
        left.z + value,
        left.w + value
    };
}

vec4 operator-(const vec4& left, float value)
{
    return vec4
    {
        left.x - value,
        left.y - value,
        left.z - value,
        left.w - value
    };
}

vec4 operator*(const vec4& left, float value)
{
    return vec4
    {
        left.x * value,
        left.y * value,
        left.z * value,
        left.w * value
    };
}

vec4 operator/(const vec4& left, float value)
{
    return vec4
    {
        left.x / value,
        left.y / value,
        left.z / value,
        left.w / value
    };
}

vec4 operator+(float value, const vec4& left)
{
    return left + value;
}

vec4 operator-(float value, const vec4& left)
{
    return left - value;
}

vec4 operator*(float value, const vec4& left)
{
    return left * value;
}

vec4 operator/(float value, const vec4& left)
{
    return left / value;
}

bool operator==(const vec4& left, const vec4& right)
{
    return left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;
}

bool operator!=(const vec4& left, const vec4& right)
{
    return !(left == right);
}

std::ostream& operator<<(std::ostream& stream, const vec4& v)
{
    stream << "vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return stream;
}

float vec4::length() const
{
    return std::sqrt(length2());
}

float vec4::length2() const
{
    return x*x + y*y + z*z + w*w;
}

vec4 vec4::normalize() const
{
    float len = length();
    if (len)
        return *this / len;
    return vec4();
}

float vec4::distance(const vec4& v) const
{
    return std::sqrt(distance(v));
}

float vec4::distance2(const vec4& v) const
{
    float dx = x - v.x;
    float dy = y - v.y;
    float dz = z - v.z;
    float dw = w - v.w;
    return dx*dx + dy*dy + dz*dz + dw*dw;
}

float vec4::dot(const vec4& v) const
{
    return x*v.x + y*v.y + z*v.z + w*v.w;
}