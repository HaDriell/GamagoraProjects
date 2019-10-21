#pragma once

#include <iostream>

struct vec2
{
    float x, y;

    vec2();
    vec2(float scalar);
    vec2(float x, float y);

    static const vec2 X;
    static const vec2 Y;
    
    float length() const;
    float length2() const;
    vec2 normalize() const;
    
    static float distance(const vec2& a, const vec2& b);
    static float distance2(const vec2& a, const vec2& b);
    static float dot(const vec2& a, const vec2& b);
    static float cross(const vec2& a, const vec2& b);
};

vec2 operator-(const vec2& v);

vec2& operator+=(vec2& left, const vec2& right);
vec2& operator-=(vec2& left, const vec2& right);
vec2& operator*=(vec2& left, const vec2& right);
vec2& operator/=(vec2& left, const vec2& right);

vec2& operator+=(vec2& left, float value);
vec2& operator-=(vec2& left, float value);
vec2& operator*=(vec2& left, float value);
vec2& operator/=(vec2& left, float value);

vec2 operator+(const vec2& left, const vec2& right);
vec2 operator-(const vec2& left, const vec2& right);
vec2 operator*(const vec2& left, const vec2& right);
vec2 operator/(const vec2& left, const vec2& right);

vec2 operator+(const vec2& left, float value);
vec2 operator-(const vec2& left, float value);
vec2 operator*(const vec2& left, float value);
vec2 operator/(const vec2& left, float value);

vec2 operator+(float value, const vec2& left);
vec2 operator-(float value, const vec2& left);
vec2 operator*(float value, const vec2& left);
vec2 operator/(float value, const vec2& left);

bool operator==(const vec2& left, const vec2& right);
bool operator!=(const vec2& left, const vec2& right);

std::ostream& operator<<(std::ostream& stream, const vec2& v);