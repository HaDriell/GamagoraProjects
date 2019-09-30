#include "vec3.h"
#include <math.h>

vec3::vec3() : x(0), y(0), z(0) {}
vec3::vec3(float scalar) : x(scalar), y(scalar), z(scalar) {}
vec3::vec3(float x, float y, float z) : x(x), y(y), z(z) {}

vec3 operator-(const vec3& v)
{
    return vec3(-v.x, -v.y, -v.z);
}

vec3& operator+=(vec3& left, const vec3& right)
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;
    return left;
}

vec3& operator-=(vec3& left, const vec3& right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;
    return left;
}

vec3& operator*=(vec3& left, const vec3& right)
{
    left.x *= right.x;
    left.y *= right.y;
    left.z *= right.z;
    return left;
}

vec3& operator/=(vec3& left, const vec3& right)
{
    left.x /= right.x;
    left.y /= right.y;
    left.z /= right.z;
    return left;
}


vec3& operator+=(vec3& left, float value)
{
    left.x += value;
    left.y += value;
    left.z += value;
    return left;
}

vec3& operator-=(vec3& left, float value)
{
    left.x -= value;
    left.y -= value;
    left.z -= value;
    return left;
}

vec3& operator*=(vec3& left, float value)
{
    left.x *= value;
    left.y *= value;
    left.z *= value;
    return left;
}

vec3& operator/=(vec3& left, float value)
{
    left.x /= value;
    left.y /= value;
    left.z /= value;
    return left;
}


vec3 operator+(const vec3& left, const vec3& right)
{
    return vec3
    (
        left.x + right.x,
        left.y + right.y,
        left.z + right.z
    );
}

vec3 operator-(const vec3& left, const vec3& right)
{
    return vec3
    (
        left.x - right.x,
        left.y - right.y,
        left.z - right.z
    );
}

vec3 operator*(const vec3& left, const vec3& right)
{
    return vec3
    (
        left.x * right.x,
        left.y * right.y,
        left.z * right.z
    );
}

vec3 operator/(const vec3& left, const vec3& right)
{
    return vec3
    (
        left.x / right.x,
        left.y / right.y,
        left.z / right.z
    );
}


vec3 operator+(const vec3& left, float value)
{
    return vec3
    (
        left.x + value,
        left.y + value,
        left.z + value
    );
}

vec3 operator-(const vec3& left, float value)
{
    return vec3
    (
        left.x - value,
        left.y - value,
        left.z - value
    );
}

vec3 operator*(const vec3& left, float value)
{
    return vec3
    (
        left.x * value,
        left.y * value,
        left.z * value
    );
}

vec3 operator/(const vec3& left, float value)
{
    return vec3
    (
        left.x / value,
        left.y / value,
        left.z / value
    );
}


bool operator==(const vec3& left, const vec3& right)
{
    return left.x == right.x && left.y == right.y && left.z == right.z;
}

bool operator!=(const vec3& left, const vec3& right)
{
    return !(left == right);
}

std::ostream& operator<<(std::ostream& stream, const vec3& v)
{
    stream << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
    return stream;
}

float vec3::length() const
{
    return std::sqrt(length2());
}

float vec3::length2() const
{
    return x*x + y*y + z*z;
}

vec3 vec3::normalise() const
{
    float len = length();
    if (len)
        return *this / len;
    return vec3();
}

float vec3::distance(const vec3& v) const
{
    return std::sqrt(distance2(v));
}

float vec3::distance2(const vec3& v) const
{
    float dx = x - v.x;
    float dy = y - v.y;
    float dz = z - v.z;
    return dx*dx + dy*dy + dz*dz;
}

float vec3::dot(const vec3& v) const
{
    return x*v.x + y*v.y + z*v.z;
}

vec3 vec3::cross(const vec3& v) const
{
    return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}
vec3 vec3::reflect(const vec3& normal) const
{
    return *this - 2.0f * this->dot(normal) * normal;
}

const vec3 vec3::X = vec3(1, 0, 0);
const vec3 vec3::Y = vec3(0, 1, 0);
const vec3 vec3::Z = vec3(0, 0, 1);

const vec3 vec3::UP     = vec3( 0, 1, 0);
const vec3 vec3::DOWN   = vec3( 0,-1, 0);
const vec3 vec3::LEFT   = vec3(-1, 0, 0);
const vec3 vec3::RIGHT  = vec3( 1, 0, 0);
const vec3 vec3::FRONT  = vec3( 0, 0, 1);
const vec3 vec3::BACK   = vec3( 0, 0,-1);