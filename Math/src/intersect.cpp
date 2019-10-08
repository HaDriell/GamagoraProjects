#include "intersect.h"

#include <limits>
#include <math.h>

bool intersectPlane(const vec3& position, const vec3& direction, const vec3& planeNormal, const vec3& pointOnPlane, float& distance, vec3& hitPoint)
{
    float d = direction.dot(planeNormal);

    //Parallel test
    if (d < 1e-6)
        return false;
    vec3 ptr = (pointOnPlane - position);
    float t = ptr.dot(planeNormal) / d;

    //Behind test
    if (t < 0)
        return false;
    
    distance = t;
    hitPoint = position + direction * t;
    return true;
}


//https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
bool intersectAABB(const vec3& position, const vec3& direction, const AABB& aabb, float& distance, vec3& hitPoint, vec3& normal)
{
    float t1 = (aabb.min.x - position.x) / direction.x;
    float t2 = (aabb.max.x - position.x) / direction.x;
    float t3 = (aabb.min.y - position.y) / direction.y;
    float t4 = (aabb.max.y - position.y) / direction.y;
    float t5 = (aabb.min.z - position.z) / direction.z;
    float t6 = (aabb.max.z - position.z) / direction.z;




    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    //Behind test
    if (tmax < 0)
        return false;

    //No intersection
    if (tmin > tmax)
        return false;
    
    distance = tmin;
    hitPoint = position + direction * distance;
    if (tmin == t1) normal = vec3::LEFT;
    if (tmin == t2) normal = vec3::RIGHT;
    if (tmin == t3) normal = vec3::DOWN;
    if (tmin == t4) normal = vec3::UP;
    if (tmin == t5) normal = vec3::BACK;
    if (tmin == t6) normal = vec3::FRONT;
    return true;
}

//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool intersectSphere(const vec3& position, const vec3& direction, const Sphere& sphere, float& distance, vec3& hitPoint, vec3& normal)
{
    vec3 oc = sphere.position - position;
    float a = direction.dot(direction);
    float b = 2 * oc.dot(direction);
    float c = oc.dot(oc) - 1; // direction is assumed to be normalized. direction.dot(direction) = 1
    
    float discriminant = (b * b) - (4 * c);

    if (discriminant < 0)
        return false;

    //Find distance solutions (they might be negative !)
    float sqrtDiscriminant = std::sqrt(discriminant);
    float t = (-b - sqrtDiscriminant) / 2;
    if (t < 0) 
        t = (-b + sqrtDiscriminant) / 2;
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
    h = direction.cross(edge2);
    a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON)
        return false;

    //Barycentric coord tests
    f = 1.0/a;
    s = position - triangle.vertex0;
    u = f * (s.dot(h));
    if (u < 0.0 || u > 1.0)
        return false;
    q = s.cross(edge1);
    v = f * direction.dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    
    //Behind test
    float t = f * edge2.dot(q);
    if (t < 0)
        return false;

    //We're sure we hit the triangle. update the output references
    distance = t;
    hitPoint = position + direction * t;
    normal   = edge1.cross(edge2).normalize();
    return true;
}