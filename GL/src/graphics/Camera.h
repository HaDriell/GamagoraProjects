#pragma once


#include "../Common.h"


class Camera
{
private:
    vec3            position;
    vec3            rotation;
    mutable mat4    viewMatrix;
    mutable bool    viewMatrixComputed;
    mat4            projectionMatrix;

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
};