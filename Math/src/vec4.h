#pragma once

#include <iostream>

struct vec4
{
    float x, y, z, w;

    vec4();
    vec4(float scalar);
    vec4(float x, float y, float z, float w);
    
    float length() const;
    float length2() const;
    vec4 normalize() const;
    float distance(const vec4& v) const;
    float distance2(const vec4& v) const;
    float dot(const vec4& v) const;
};

vec4 operator-(vec4& v);

vec4& operator+=(vec4& left, const vec4& right);
vec4& operator-=(vec4& left, const vec4& right);
vec4& operator*=(vec4& left, const vec4& right);
vec4& operator/=(vec4& left, const vec4& right);

vec4& operator+=(vec4& left, float value);
vec4& operator-=(vec4& left, float value);
vec4& operator*=(vec4& left, float value);
vec4& operator/=(vec4& left, float value);

vec4 operator+(const vec4& left, const vec4& right);
vec4 operator-(const vec4& left, const vec4& right);
vec4 operator*(const vec4& left, const vec4& right);
vec4 operator/(const vec4& left, const vec4& right);

vec4 operator+(const vec4& left, float value);
vec4 operator-(const vec4& left, float value);
vec4 operator*(const vec4& left, float value);
vec4 operator/(const vec4& left, float value);

vec4 operator+(float value, const vec4& left);
vec4 operator-(float value, const vec4& left);
vec4 operator*(float value, const vec4& left);
vec4 operator/(float value, const vec4& left);

bool operator==(const vec4& left, const vec4& right);
bool operator!=(const vec4& left, const vec4& right);

std::ostream& operator<<(std::ostream& stream, const vec4& v);