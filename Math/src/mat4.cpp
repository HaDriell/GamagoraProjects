#include "mat4.h"
#include "util.h"
#include <math.h>

mat4::mat4() : 
m00(1), m01(0), m02(0), m03(0), 
m10(0), m11(1), m12(0), m13(0), 
m20(0), m21(0), m22(1), m23(0), 
m30(0), m31(0), m32(0), m33(1) {} 

mat4::mat4(float s) : 
m00(s), m01(0), m02(0), m03(0), 
m10(0), m11(s), m12(0), m13(0), 
m20(0), m21(0), m22(s), m23(0), 
m30(0), m31(0), m32(0), m33(s) {} 

mat4::mat4(float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33) :
m00(m00), m01(m01), m02(m02), m03(m03), 
m10(m10), m11(m11), m12(m12), m13(m13), 
m20(m20), m21(m21), m22(m22), m23(m23), 
m30(m30), m31(m31), m32(m32), m33(m33) {} 

float mat4::determinant()
{
    float b00 = m00 * m11 - m01 * m10;
    float b01 = m00 * m12 - m02 * m10;
    float b02 = m00 * m13 - m03 * m10;
    float b03 = m01 * m12 - m02 * m11;
    float b04 = m01 * m13 - m03 * m11;
    float b05 = m02 * m13 - m03 * m12;
    float b06 = m20 * m31 - m21 * m30;
    float b07 = m20 * m32 - m22 * m30;
    float b08 = m20 * m33 - m23 * m30;
    float b09 = m21 * m32 - m22 * m31;
    float b10 = m21 * m33 - m23 * m31;
    float b11 = m22 * m33 - m23 * m32;
    return b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;
}

mat4 mat4::inverse()
{
    float b00 = m00 * m11 - m01 * m10;
    float b01 = m00 * m12 - m02 * m10;
    float b02 = m00 * m13 - m03 * m10;
    float b03 = m01 * m12 - m02 * m11;
    float b04 = m01 * m13 - m03 * m11;
    float b05 = m02 * m13 - m03 * m12;
    float b06 = m20 * m31 - m21 * m30;
    float b07 = m20 * m32 - m22 * m30;
    float b08 = m20 * m33 - m23 * m30;
    float b09 = m21 * m32 - m22 * m31;
    float b10 = m21 * m33 - m23 * m31;
    float b11 = m22 * m33 - m23 * m32;
    float det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

    //unsafe but shouldn't happen anyways
    det = 1.f / det;

    return mat4(
        (m11 * b11 - m12 * b10 + m13 * b09) * det,
        (m02 * b10 - m01 * b11 - m03 * b09) * det,
        (m31 * b05 - m32 * b04 + m33 * b03) * det,
        (m22 * b04 - m21 * b05 - m23 * b03) * det,
        (m12 * b08 - m10 * b11 - m13 * b07) * det,
        (m00 * b11 - m02 * b08 + m03 * b07) * det,
        (m32 * b02 - m30 * b05 - m33 * b01) * det,
        (m20 * b05 - m22 * b02 + m23 * b01) * det,
        (m10 * b10 - m11 * b08 + m13 * b06) * det,
        (m01 * b08 - m00 * b10 - m03 * b06) * det,
        (m30 * b04 - m31 * b02 + m33 * b00) * det,
        (m21 * b02 - m20 * b04 - m23 * b00) * det,
        (m11 * b07 - m10 * b09 - m12 * b06) * det,
        (m00 * b09 - m01 * b07 + m02 * b06) * det,
        (m31 * b01 - m30 * b03 - m32 * b00) * det,
        (m20 * b03 - m21 * b01 + m22 * b00) * det
    );
}

mat4 mat4::Orthographic(float left, float right, float bottom, float top, float near, float far)
{
    float x = right - left;
    float y = top - bottom;
    float z = near - far;

	float tx = (left + right) / (left - right);
	float ty = (bottom + top) / (bottom - top);
	float tz = (far + near) / (far - near);

    return mat4(
         x,  0,  0,  0,
         0,  y,  0,  0,
         0,  0,  z,  0,
        tx, ty, tz,  1);
}

mat4 mat4::Perspective(float fov, float aspectRatio, float near, float far)
{
    float q = 1.f / std::tan(deg2rad * fov * 0.5f);
    float a = q / aspectRatio;
    float b = (near + far) / (near - far);
    float c = (2.0f * near * far) / (near - far);

    return mat4(
        a, 0, 0, 0,
        0, q, 0, 0,
        0, 0, b,-1,
        0, 0, c, 0);
}

mat4 mat4::Translation(vec3 v)
{
    return mat4::Translation(v.x, v.y, v.z);
}

mat4 mat4::Translation(float x, float y, float z)
{
    return mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1);
}

mat4 mat4::LookAt(const vec3& position, const vec3& target, const vec3& up)
{
    vec3 f = (target - position).normalize();
    vec3 s = f.cross(up.normalize());
    vec3 u = s.cross(f);

    mat4 view = mat4(
        s.x, s.y, s.z,  0,
        u.x, u.y, u.z,  0,
        f.x, f.y, f.z,  0, //not inverted because we look into Z
          0,   0,   0,  1);
    
    return view * mat4::Translation(-position.x, -position.y, -position.z);
}

mat4 mat4::Scale(float x, float y, float z)
{
    return mat4(
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1);
}

mat4 mat4::Rotation(float degrees, const vec3& axis)
{
    float rad = degrees * deg2rad;
    float c = std::cos(rad);
    float s = std::sin(rad);
    float t = 1 - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    return mat4(
          c + x*x*t,    x*y*t - z*s,    x*z*t + y*s,    0,
        y*x*t + z*s,      c + y*y*t,    y*z*t - x*s,    0,
        z*x*t - y*s,    z*y*t + x*s,      c + z*z*t,    0,
                  0,              0,              0,    1);
}

mat4 mat4::RotationYXZ(const vec3& degrees)
{
    mat4 rot_x = mat4::RotationX(degrees.x);
    mat4 rot_y = mat4::RotationY(degrees.y);
    mat4 rot_z = mat4::RotationZ(degrees.z);
    return rot_x * rot_y * rot_z;
}

mat4 mat4::RotationX(float degrees)
{
    float rad = degrees * deg2rad;
    float c = std::cos(rad);
    float s = std::sin(rad);
    return mat4(
        1, 0, 0, 0,
        0, c,-s, 0,
        0, s, c, 0,
        0, 0, 0, 1);
}

mat4 mat4::RotationY(float degrees)
{
    float rad = degrees * deg2rad;
    float c = std::cos(rad);
    float s = std::sin(rad);
    return mat4(
        c, 0, s, 0,
        0, 1, 0, 0,
       -s, 0, c, 0,
        0, 0, 0, 1);
}

mat4 mat4::RotationZ(float degrees)
{
    float rad = degrees * deg2rad;
    float c = std::cos(rad);
    float s = std::sin(rad);
    return mat4(
        c,-s, 0, 0,
        s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
}

//assumed vec4(x, y, 0, 1)
vec2 operator*(const mat4& m, const vec2& v)
{
    return vec2(
        m.m00 * v.x + m.m10 * v.y + m.m30,
        m.m01 * v.x + m.m11 * v.y + m.m31
    );
}

//assumed vec4(x, y, z, 1)
vec3 operator*(const mat4& m, const vec3& v)
{
    return vec3(
        m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30,
        m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31,
        m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32 
    );
}

vec4 operator*(const mat4& m, const vec4& v)
{
    return vec4(
        m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30 * v.w,
        m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31 * v.w,
        m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32 * v.w,
        m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + m.m33 * v.w
    );
}

mat4 operator*(const mat4& a, const mat4& b)
{
    mat4 m = mat4();
    m.m00 = a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20 + a.m03 * b.m30;
    m.m01 = a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21 + a.m03 * b.m31;
    m.m02 = a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22 + a.m03 * b.m32;
    m.m03 = a.m00 * b.m03 + a.m01 * b.m13 + a.m02 * b.m23 + a.m03 * b.m33;

    m.m10 = a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20 + a.m13 * b.m30;
    m.m11 = a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31;
    m.m12 = a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32;
    m.m13 = a.m10 * b.m03 + a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33;

    m.m20 = a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20 + a.m23 * b.m30;
    m.m21 = a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31;
    m.m22 = a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32;
    m.m23 = a.m20 * b.m03 + a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33;

    m.m30 = a.m30 * b.m00 + a.m31 * b.m10 + a.m32 * b.m20 + a.m33 * b.m30;
    m.m31 = a.m30 * b.m01 + a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31;
    m.m32 = a.m30 * b.m02 + a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32;
    m.m33 = a.m30 * b.m03 + a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33;
    return m;
}