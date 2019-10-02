#pragma once

#include "vec2.h"
#include "vec3.h"

struct mat3
{
    //matrix data
    float m00; float m01; float m02;
    float m10; float m11; float m12;
    float m20; float m21; float m22;

    mat3();
    mat3(float scalar);
    mat3(float m00, float m01, float m02,
         float m10, float m11, float m12,
         float m20, float m21, float m22);
    
    static mat3 Translation(float x, float y);
    static mat3 Rotation(float degrees);
    static mat3 Scale(float x, float y);
    static mat3 Transform(float scaleX, float scaleY, float degrees, float tx, float ty);
};

vec2 operator*(const mat3& left, const vec2& right);
vec3 operator*(const mat3& left, const vec3& right);

mat3 operator*(const mat3& left, const mat3& right);
mat3& operator*=(mat3& left, const mat3& right);
