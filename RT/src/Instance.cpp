#include "Instance.h"
#include <iostream>


Instance::~Instance() {}


HitResult Instance::hit(const vec3& position, const vec3& direction)
{
    //Inverse Tranform the ray to Model space
    vec3 localPosition = transform.multiplyInverse(position, 1);
    vec3 localDirection = transform.multiplyInverse(direction, 0).normalize();
    //Hit the Model
    HitResult result = intersect(localPosition, localDirection);

    //Transform HitResult back to World space
    result.hitPoint = transform.multiply(result.hitPoint, 1);
    result.normal = transform.multiply(result.normal, 0).normalize();
    result.distance = vec3::distance(position, result.hitPoint); // for now I don't have anything better

    return result;
}

bool Instance::hitBoundingBox(const vec3& position, const vec3& direction)
{
    //Inverse Tranform the ray to Model space
    vec3 localPosition = transform.multiplyInverse(position, 1);
    vec3 localDirection = transform.multiplyInverse(direction, 0).normalize();
    return intersectBoundingBox(localPosition, localDirection);
}