#pragma once


#include "../Common.h"


class Camera
{
private:
    mat4            projectionMatrix;
    vec3            position;
    float           pitch;
    float           yaw;
    float           roll;

    mutable mat4    viewMatrix;
    mutable bool    viewMatrixComputed;


public:
    Camera(const mat4& projectionMatrix, const vec3& position = vec3(), const vec3& rotation = vec3());

    mat4 getProjectionMatrix() const;
    mat4 getViewMatrix() const;
    vec3 getPosition() const;
    vec3 getRotation() const;

    void setPosition(const vec3& position);
    void setRotation(const vec3& rotationDegree);

    void translate(const vec3& position);
    void rotate(const vec3& rotationDegree);

    inline void setPosition(float x, float y, float z) { setPosition(vec3(x, y, z)); }
    inline void setRotation(float x, float y, float z) { setRotation(vec3(x, y, z)); }
    inline void translate(float x, float y, float z) { translate(vec3(x, y, z)); }
    inline void rotate(float xDeg, float yDeg, float zDeg) { rotate(vec3(xDeg, yDeg, zDeg)); }
};