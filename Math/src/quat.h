#pragma once

#include <iostream>

#include "vec3.h"
#include "vec4.h"
#include "mat3.h"
#include "mat4.h"

struct quat
{
    float x, y, z, w;

    quat();
    quat(float x, float y, float z, float w);
    quat(const vec3& xyz, float w);
    quat(const vec4& vec);

    float real() const;
    quat unreal() const;
    quat normalize() const;
    float norm2() const;
    float norm() const;
    float unreal_norm2() const;

    mat3 toMat3() const;
    mat4 toMat4() const;

    quat conjugate() const;
    quat inverse() const;

    static quat Euler(float yaw, float pitch, float roll);
    static quat Euler(const vec3& v);

    static quat Rotation(const vec3& from, const vec3& to);
};

quat operator+(const quat& q);
quat operator-(const quat& q);

quat& operator+=(quat& q, float real);
quat& operator-=(quat& q, float real);

quat& operator*=(quat& q, float factor);
quat& operator/=(quat& q, float factor);

quat& operator+=(quat& left, const quat& right);
quat& operator-=(quat& left, const quat& right);
quat& operator*=(quat& left, const quat& right);
quat& operator/=(quat& left, const quat& right);

bool operator==(const quat& left, const quat& right);
bool operator!=(const quat& left, const quat& right);

std::ostream& operator<<(std::ostream& stream, const quat& v);