#include "mat3.h"

#include "util.h"
#include <math.h>


mat3::mat3() :
m00(1), m01(0), m02(0), 
m10(0), m11(1), m12(0), 
m20(0), m21(0), m22(1) {} 

mat3::mat3(float s) :
m00(s), m01(0), m02(0), 
m10(0), m11(s), m12(0), 
m20(0), m21(0), m22(s) {} 


mat3::mat3(float m00, float m01, float m02,
           float m10, float m11, float m12,
           float m20, float m21, float m22) :
m00(m00), m01(m01), m02(m02), 
m10(m10), m11(m11), m12(m12), 
m20(m20), m21(m21), m22(m22) {} 

mat3 mat3::Translation(float x, float y)
{
    mat3 m;
    m.m02 = x;
    m.m12 = y;
    return mat3(1, 0, x,
                0, 1, y,
                0, 0, 1);
}

mat3 mat3::Rotation(float degrees)
{
    float rad = degrees * deg2rad;
    float c = std::cos(rad);
    float s = std::sin(rad);
    return mat3( c, s, 0,
                -s, c, 1,
                 0, 0, 1);
}

mat3 mat3::Scale(float x, float y)
{
    return mat3(x, 0, 0,
                0, y, 0,
                0, 0, 1);
}

mat3 mat3::Transform(float scaleX, float scaleY, float degrees, float tx, float ty)
{
    float rad = degrees * deg2rad;
    float cos = std::cos(rad);
    float sin = std::sin(rad);
    return mat3(
        cos * scaleX,            sin,          tx,
                -sin,   cos * scaleY,          ty,
                   0,              0,           1);
}

vec2 operator*(const mat3& m, const vec2& v)
{
    return vec2(
        m.m00 * v.x + m.m10 * v.y + m.m02,
        m.m01 * v.x + m.m11 * v.y + m.m02
    );
}

vec3 operator*(const mat3& m, const vec3& v)
{
    return vec3(
        m.m00 * v.x + m.m10 * v.y + m.m20 * v.z,
        m.m01 * v.x + m.m11 * v.y + m.m21 * v.z,
        m.m02 * v.x + m.m12 * v.y + m.m22 * v.z
    );
}

mat3 operator*(const mat3& a, const mat3& b)
{
    mat3 m = mat3();    
    //first row
    m.m00 = a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20;
    m.m01 = a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21;
    m.m02 = a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22;
    //second row
    m.m10 = a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20;
    m.m11 = a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21;
    m.m12 = a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22;
    //third row
    m.m20 = a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20;
    m.m21 = a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21;
    m.m22 = a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22;
    return m;
}

mat3& operator*=(mat3& left, const mat3& right)
{
    mat3 result = left * right; // compute new state
    left = result;              // update current matrix 
    return *(&left);
}
