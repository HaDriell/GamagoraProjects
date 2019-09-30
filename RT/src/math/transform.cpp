#include "transform.h"


transform::transform() : t(vec3()), r(vec3()), s(vec3(1, 1, 1)) 
{}

vec3 transform::get_translation()
{
    return t;
}

vec3 transform::get_rotation()
{
    return r;
}

vec3 transform::get_scaling()
{
    return s;
}



void transform::set_translation(float x, float y, float z)
{
    t.x = x;
    t.y = y;
    t.z = z;
    cached = false;
}

void transform::set_rotation(float x, float y, float z)
{
    r.x = x;
    r.y = y;
    r.z = z;
    cached = false;
}

void transform::set_scaling(float x, float y, float z)
{
    s.x = x;
    s.y = y;
    s.z = z;
    cached = false;
}

void transform::translate(float x, float y, float z)
{
    t.x += x;
    t.y += y;
    t.z += z;
    cached = false;
}

void transform::rotate(float xDegrees, float yDegrees, float zDegrees)
{
    r.x += xDegrees;
    r.y += yDegrees;
    r.z += zDegrees;
    cached = false;
}

void transform::scale(float xScaling, float yScaling, float zScaling)
{
    s.x = xScaling;
    s.y = yScaling;
    s.z = zScaling;
    cached = false;
}

mat4 transform::get_matrix()
{
    computeMatrices();
    return matrix;
}

mat4 transform::get_matrix_inverse()
{
    computeMatrices();
    return matrixInverse;
}

vec3 transform::apply(const vec3& point)
{
    computeMatrices();
    return matrix * point;
}

vec3 transform::appplyInverse(const vec3& point)
{
    computeMatrices();
    return matrixInverse * point;
}

void transform::computeMatrices()
{
    //skip compute when cache is enabled
    if (cached) return;

    //Reset to Identity
    matrix = mat4();
    //Scale
    matrix *= mat4::Scale(s.x, s.y, s.z);
    //Rotate
    matrix *= mat4::RotationX(r.x);
    matrix *= mat4::RotationY(r.y);
    matrix *= mat4::RotationZ(r.z);
    //Translate
    matrix *= mat4::Translation(t.x, t.y, t.z);
    //Compute inverse
    matrixInverse = matrix.inverse();

    //enable cache
    cached = true;
}
