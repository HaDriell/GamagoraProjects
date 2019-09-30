#include "quat.h"
#include "util.h"

#include <math.h>

//TODO : faire des matrices !


quat::quat() : x(0), y(0), z(0), w(1) {}
quat::quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
quat::quat(const vec3& direction, float w) : x(direction.x), y(direction.y), z(direction.z), w(w) {}
quat::quat(const vec4& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}

float quat::real() const
{
    return w;
}

quat quat::unreal() const
{
    return quat(x, y, z, 0);
}

quat quat::normalize() const
{
    float len = norm();
    if (len)
        return quat(x / len, y / len, z / len, w / len);
    return quat();
}

float quat::norm2() const
{
    return x*x + y*y + z*z + w*w;
}

float quat::norm() const
{
    return std::sqrt(norm2());
}

float quat::unreal_norm2() const
{
    return x*x + y*y + z*z;
}

quat operator+(const quat& q)
{
    return q;
}

quat operator-(const quat& q)
{
    return quat(-q.x, -q.y, -q.z, -q.w);
}

quat& operator+=(quat& q, float real)
{
    q.w += real;
    return q;
}

quat& operator-=(quat& q, float real)
{
    q.w -= real;
    return q;
}

quat& operator*=(quat& q, float factor)
{
    q.x *= factor;
    q.y *= factor;
    q.z *= factor;
    q.w *= factor;
    return q;
}

quat& operator/=(quat& q, float factor)
{
    q.x /= factor;
    q.y /= factor;
    q.z /= factor;
    q.w /= factor;
    return q;
}

quat& operator+=(quat& left, const quat& right)
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;
    left.w += right.w;
    return left;
}

quat& operator-=(quat& left, const quat& right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;
    left.w -= right.w;
    return left;
}

quat& operator*=(quat& left, const quat& right)
{
    float wt = left.w * right.w - left.x * right.x - left.y * right.y - left.z * right.z;
    float xt = left.w * right.x + left.x * right.w + left.y * right.z - left.z * right.y;
    float yt = left.w * right.y - left.x * right.z + left.y * right.w + left.z * right.x;
    float zt = left.w * right.z + left.x * right.y - left.y * right.x + left.z * right.w;

    left.w = wt;
    left.x = xt;
    left.y = yt;
    left.z = zt;

    return left;
}

bool operator==(const quat& left, const quat& right)
{
    return left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;
}

bool operator!=(const quat& left, const quat& right)
{
    return !(left == right);
}

/**
 * Returns a 4x4 real matrix representation from a Quaternion x:
 * [ a  b  c  d ]
 * [-b  a -d  c ]
 * [-c  d  a -b ]
 * [-d -c  b  a ]
 */
mat4 quat::toMat4() const
{
    return mat4(
         w, x, y, z,
        -x, w,-z, y,
        -y, z, w,-x,
        -z,-y, x, w);
}

/**
 * Returns a 3D rotation matrix.
 * This is the "homogeneous" expression to convert to a rotation matrix,
 * which works if the Quaternoin is not a unit Quaternion.
 */
mat3 quat::toMat3() const
{
    float w2 = w * w; 
    float x2 = x * x; 
    float y2 = y * y;
    float z2 = z * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;
    return mat3(
        w2 + x2 - y2 - z2, 2 * (xy - wz), 2 * (xz + wy),
        2 * (xy + wz), w2 - x2 + y2 - z2, 2 * (yz - wz),
        2 * (xz - wy), 2 * (yz + wx), w2 - x2 - y2 + z2);
}

quat quat::conjugate() const
{
    return quat(-x, -y, -z, w);
}

quat quat::inverse() const
{
    float n = norm();
    quat c  = conjugate();
    return quat(
        c.x / n,
        c.y / n,
        c.z / n,
        c.w /n);
}

quat quat::Euler(float yaw, float pitch, float roll)
{
    float cy = std::cos(yaw/2);
    float sy = std::sin(yaw/2);
    float cp = std::cos(pitch/2);
    float sp = std::sin(pitch/2);
    float cr = std::cos(roll/2);
    float sr = std::sin(roll/2);

    quat q;
    
    q.w = cy * cp * cr + sy * sp * sr;
    q.x = cy * cp * sr - sy * sp * cr;
    q.y = sy * cp * sr + cy * sp * cr;
    q.z = sy * cp * cr - cy * sp * sr;

    return q;
}

quat quat::Euler(const vec3& v)
{
    return quat::Euler(v.x, v.y, v.z);
}

quat quat::Rotation(const vec3& from, const vec3& to)
{
    float chax2, rchax2;
    chax2 = std::sqrt(2.f * (1.f + from.dot(to)));
    rchax2 = 1.f / chax2;
    return quat(from.cross(to) * rchax2, chax2 * 0.5f);
}