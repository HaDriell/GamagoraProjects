#pragma once

#include "AABB.h"
#include "Sphere.h"
#include "Triangle.h"


bool intersectPlane(const vec3& position, const vec3& direction, const vec3& planeNormal, const vec3& pointOnPlane, float& distance, vec3& hitPoint);
bool intersectAABB(const vec3& position, const vec3& direction, const AABB& aabb, float& distance, vec3& hitPoint, vec3& normal);
bool intersectSphere(const vec3& position, const vec3& direction, const Sphere& sphere, float& distance, vec3& hitPoint, vec3& normal);
bool intersectTriangle(const vec3& position, const vec3& direction, const Triangle& triangle, float& distance, vec3& hitPoint, vec3& normal);