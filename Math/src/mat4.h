#pragma once

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

struct mat4
{
    //matrix data
    float m00; float m01; float m02; float m03;
    float m10; float m11; float m12; float m13;
    float m20; float m21; float m22; float m23;
    float m30; float m31; float m32; float m33;

    mat4();
    mat4(float scalar);
    mat4(float m00, float m01, float m02, float m03,
         float m10, float m11, float m12, float m13,
         float m20, float m21, float m22, float m23,
         float m30, float m31, float m32, float m33);
    
    float determinant();
    mat4 inverse();

    static mat4 Orthographic(float left, float right, float top, float bottom, float near, float far);
    static mat4 Perspective(float fov, float aspectRatio, float near, float far);
    static mat4 PerspectiveFov(float fov, float width, float height, float near, float far);
    static mat4 LookAt(const vec3& position, const vec3& target, const vec3& up);

    static mat4 Translation(float x, float y, float z);
    static mat4 Translation(const vec3& v);
    static mat4 Rotation(float degrees, const vec3& axis);
    static mat4 RotationYXZ(const vec3& degrees);
    static mat4 RotationX(float degrees);
    static mat4 RotationY(float degrees);
    static mat4 RotationZ(float degrees);
    static mat4 Scale(float x, float y, float z);
    static mat4 Scale(const vec3& scale);
    static mat4 Scale(float scalar);
};

vec2 operator*(const mat4& left, const vec2& right);
vec3 operator*(const mat4& left, const vec3& right);
vec4 operator*(const mat4& left, const vec4& right);

mat4 operator*(const mat4& left, const mat4& right);
