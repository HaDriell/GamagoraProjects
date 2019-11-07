#include "Camera.h"


Camera::Camera(const mat4& projectionMatrix, const vec3& position, const vec3& rotation)
    : projectionMatrix(projectionMatrix), position(position), pitch(rotation.x), yaw(rotation.y), roll(rotation.z), viewMatrixComputed(false)
{
}

mat4 Camera::getProjectionMatrix() const
{
    return projectionMatrix;
}

mat4 Camera::getViewMatrix() const
{
    if (!viewMatrixComputed)
    {
        viewMatrix = mat4::Translation(-position) 
                   * mat4::RotationZ(roll)
                   * mat4::RotationY(yaw)
                   * mat4::RotationX(pitch);
        viewMatrixComputed = true;
    }
    return viewMatrix;
}

vec3 Camera::getPosition() const
{
    return position;
}

void Camera::setPosition(const vec3& position)
{
    this->position = position;
    viewMatrixComputed = false;
}

vec3 Camera::getRotation() const
{
    return rotation;
}

void Camera::setRotation(const vec3& rotationDegree)
{
    this->rotation = rotationDegree;
    viewMatrixComputed = false;
}


void Camera::translate(const vec3& position)
{
    this->position += position;
    viewMatrixComputed = false;
}

void Camera::rotate(const vec3& rotationDegree)
{
    this->rotation += rotationDegree;
    viewMatrixComputed = false;
}