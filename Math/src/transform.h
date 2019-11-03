#pragma once

#include "vec3.h"
#include "mat4.h"

class Transform
{
private:
    vec3 m_Position;
    vec3 m_Rotation;
    vec3 m_Scale;

    mat4 m_Matrix;
    mat4 m_Inverse;

    void compute();
public:
    Transform();

    void setPosition(const vec3& position);
    void setRotation(const vec3& rotation);
    void setScale(const vec3& scale);
    void translate(const vec3& translation);
    void rotate(const vec3& angles);
    void scale(const vec3& scales);

    inline void setPosition(float x, float y, float z)                      { setPosition(vec3(x, y, z)); }
    inline void setRotation(float xDegrees, float yDegrees, float zDegrees) { setRotation(vec3(xDegrees, yDegrees, zDegrees)); }
    inline void setScale(float x, float y, float z)                         { setScale(vec3(x, y, z)); }
    inline void translate(float x, float y, float z)                        { translate(vec3(x, y, z)); }
    inline void rotate(float xDegrees, float yDegrees, float zDegrees)      { rotate(vec3(xDegrees, yDegrees, zDegrees)); }
    inline void scale(float x, float y, float z)                            { scale(vec3(x, y, z)); }

    vec3 multiply(const vec3& v, float w = 1) const;
    vec3 multiplyInverse(const vec3& v, float w = 1) const;

    vec3 getPosition() const;
    vec3 getRotation() const;
    vec3 getScale() const;

    mat4 getMatrix() const;
    mat4 getInverse() const;
};