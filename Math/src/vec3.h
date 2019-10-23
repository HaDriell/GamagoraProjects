#pragma once

#include <iostream>

struct vec3
{
    float x, y, z;

    vec3();
    vec3(float scalar);
    vec3(float x, float y, float z);

    static const vec3 X;
    static const vec3 Y;
    static const vec3 Z;

    static const vec3 UP;
    static const vec3 DOWN;
    static const vec3 LEFT;
    static const vec3 RIGHT;
    static const vec3 FRONT;
    static const vec3 BACK;
    
    float length() const;
    float length2() const;
    vec3 normalize() const;
    vec3 inverse() const;

    static float distance(const vec3& a, const vec3& b);
    static float distance2(const vec3& a, const vec3& b);
    static float dot(const vec3& a, const vec3& b);
    static vec3 cross(const vec3& a, const vec3& b);
    static vec3 reflect(const vec3& direction, const vec3& normal);
};

vec3 operator-(const vec3& v);

vec3& operator+=(vec3& left, const vec3& right);
vec3& operator-=(vec3& left, const vec3& right);
vec3& operator*=(vec3& left, const vec3& right);
vec3& operator/=(vec3& left, const vec3& right);

vec3& operator+=(vec3& left, float value);
vec3& operator-=(vec3& left, float value);
vec3& operator*=(vec3& left, float value);
vec3& operator/=(vec3& left, float value);

vec3 operator+(const vec3& left, const vec3& right);
vec3 operator-(const vec3& left, const vec3& right);
vec3 operator*(const vec3& left, const vec3& right);
vec3 operator/(const vec3& left, const vec3& right);

vec3 operator+(const vec3& left, float value);
vec3 operator-(const vec3& left, float value);
vec3 operator*(const vec3& left, float value);
vec3 operator/(const vec3& left, float value);

vec3 operator+(float value, const vec3& left);
vec3 operator-(float value, const vec3& left);
vec3 operator*(float value, const vec3& left);
vec3 operator/(float value, const vec3& left);

bool operator==(const vec3& left, const vec3& right);
bool operator!=(const vec3& left, const vec3& right);

std::ostream& operator<<(std::ostream& stream, const vec3& v);