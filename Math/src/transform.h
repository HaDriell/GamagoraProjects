#pragma once

#include "vec3.h"
#include "mat4.h"

class transform
{
    vec3 t = vec3();
    vec3 r = vec3(0, 0, 0);
    vec3 s = vec3(1, 1, 1);


public:
    transform();
    
    void set_translation(float x, float y, float z);
    void set_rotation(float x, float y, float z);
    void set_scaling(float x, float y, float z);

    void translate(float x, float y, float z);
    void rotate(float xDegrees, float yDegrees, float zDegrees);
    void scale(float xScaling, float yScaling, float zScaling);

    vec3 multiply(const vec3& v, bool point = true);
    vec3 multiplyInverse(const vec3& v, bool point = true);

    vec3 get_translation();
    vec3 get_rotation();
    vec3 get_scaling();

    mat4 get_matrix();
    mat4 get_matrix_inverse();

private:
    bool cached = false;
    void computeMatrices();
    mat4 matrix, matrixInverse;
};