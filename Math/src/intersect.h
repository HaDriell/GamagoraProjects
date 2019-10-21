#pragma once

#include "AABB.h"
#include "Sphere.h"
#include "Triangle.h"


bool intersectAABB(const vec3& position, const vec3& direction, const AABB& aabb);
bool intersectSphere(const vec3& position, const vec3& direction, const Sphere& sphere, float& distance, vec3& hitPoint, vec3& normal);
bool intersectTriangle(const vec3& position, const vec3& direction, const Triangle& triangle, float& distance, vec3& hitPoint, vec3& normal);