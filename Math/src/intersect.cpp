#include "intersect.h"

#include <limits>
#include <math.h>

//https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
bool intersectAABB(const vec3& position, const vec3& direction, const AABB& aabb)
{
    vec3 invD = direction.inverse();

    float txmin = (aabb.min.x - position.x) * invD.x;
    float txmax = (aabb.max.x - position.x) * invD.x;
    float tymin = (aabb.min.y - position.y) * invD.y;
    float tymax = (aabb.max.y - position.y) * invD.y;
    float tzmin = (aabb.min.z - position.z) * invD.z;
    float tzmax = (aabb.max.z - position.z) * invD.z;

    float tmin = std::max(std::max(std::min(txmin, txmax), std::min(tymin, tymax)), std::min(tzmin, tzmax));
    float tmax = std::min(std::min(std::max(txmin, txmax), std::max(tymin, tymax)), std::max(tzmin, tzmax));

    //AABB is behind
    if (tmax < 0)
        return false;

    //No intersection
    if (tmin > tmax)
        return false;
    return true;
}

//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool intersectSphere(const vec3& position, const vec3& direction, const Sphere& sphere, float& distance, vec3& hitPoint, vec3& normal)
{
    vec3 oc = sphere.position - position;
    float tca = vec3::dot(oc, direction);
    float d2 = vec3::dot(oc, oc) - (tca * tca);
    float r2 = sphere.radius * sphere.radius;

    if (d2 > r2) 
        return false;
    
    float thc = std::sqrt(r2 - d2);

    float t =  tca - thc;
    if (t < 0)
        t = tca + thc;
    
    if (t < 0)
        return false;
    
    //At this point we're sure we hit something at the distance of t > 0. Update the output references
    distance = t;
    hitPoint = position + direction * t;
    normal = (hitPoint - sphere.position).normalize();
    return true;
}

//https://fr.wikipedia.org/wiki/Algorithme_d%27intersection_de_M%C3%B6ller%E2%80%93Trumbore
bool intersectTriangle(const vec3& position, const vec3& direction, const Triangle& triangle, float& distance, vec3& hitPoint, vec3& normal)
{
    const float EPSILON = 1e-6;
    vec3 edge1 = triangle.vertex1 - triangle.vertex0;
    vec3 edge2 = triangle.vertex2 - triangle.vertex0;

    vec3 h, s, q;
    float a, f, u, v;

    //Parallel test
    h = vec3::cross(direction, edge2);
    a = vec3::dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;

    //Barycentric coord tests
    f = 1.0/a;
    s = position - triangle.vertex0;
    u = f * (vec3::dot(s, h));
    if (u < 0.0 || u > 1.0)
        return false;
    q = vec3::cross(s, edge1);
    v = f * vec3::dot(direction, q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    
    //Behind test
    float t = f * vec3::dot(edge2, q);
    if (t < 0)
        return false;

    //We're sure we hit the triangle. update the output references
    distance = t;
    hitPoint = position + direction * t;
    normal   = vec3::cross(edge1, edge2).normalize();
    return true;
}