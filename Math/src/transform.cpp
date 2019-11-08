#include "transform.h"


Transform::Transform() : m_Position(vec3()), m_Rotation(vec3()), m_Scale(vec3(1)), m_Matrix(mat4()), m_Inverse(mat4()), computed(true)
{
}

void Transform::setPosition(const vec3& position)
{
    m_Position = position;
    computed = false;
}

void Transform::setRotation(const vec3& rotation)
{
    m_Rotation = rotation;
    computed = false;
}

void Transform::setScale(const vec3& scale)
{
    m_Scale = scale;
    computed = false;
}

void Transform::translate(const vec3& translation)
{
    m_Position += translation;
    computed = false;
}

void Transform::rotate(const vec3& angles)
{
    m_Rotation += angles;
    computed = false;
}

void Transform::scale(const vec3& scales)
{
    m_Scale *= scales;
    computed = false;
}

vec3 Transform::getPosition() const 
{ 
    return m_Position; 
}

vec3 Transform::getRotation() const
{
    return m_Rotation;
}

vec3 Transform::getScale() const
{
    return m_Scale;
}

mat4 Transform::getMatrix() const
{
    compute();
    return m_Matrix;
}

mat4 Transform::getInverse() const
{
    compute();
    return m_Inverse;
}


vec3 Transform::multiply(const vec3& v, float w) const
{
    vec4 t = getMatrix() * vec4(v.x, v.y, v.z, w);
    return vec3(t.x, t.y, t.z);
}

vec3 Transform::multiplyInverse(const vec3& v, float w) const
{
    vec4 t = getInverse() * vec4(v.x, v.y, v.z, w);
    return vec3(t.x, t.y, t.z);
}

void Transform::compute() const
{
    if (!computed)
    {
        //Compute Matrix
        m_Matrix = mat4::Scale(m_Scale) * mat4::RotationXYZ(m_Rotation) * mat4::Translation(m_Position);
        //Compute Inverse
        m_Inverse = m_Matrix.inverse();
        computed = true;
    }
}