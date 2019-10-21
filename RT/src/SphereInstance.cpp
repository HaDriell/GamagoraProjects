#include "SphereInstance.h"

#include <math.h>
#include <random>


SphereInstance::~SphereInstance() {}

vec3 SphereInstance::get_centroid() const
{
    ::transform transform = this->transform;
    return transform.multiply(vec3());
}

AABB SphereInstance::get_bounding_box() const
{
    ::transform transform = this->transform;
    AABB box;
    box.min = vec3(-1);
    box.max = vec3(1);
    box.min = transform.multiply(box.min);
    box.max = transform.multiply(box.max);
    return box;
}


bool SphereInstance::intersectBoundingBox(const vec3& position, const vec3& direction) const
{
    AABB box;
    box.min = vec3(-1);
    box.max = vec3(1);
    return intersectAABB(position, direction, box);
}

HitResult SphereInstance::intersect(const vec3& position, const vec3& direction)
{
    HitResult result;

    //try to hit a unit sphere located at (0, 0, 0)
    float b = 2 * direction.dot(position);
    float c = position.dot(position) - 1;
    
    float det = (b * b) - (4 * c);
    if (det >= 0.f)
    {
        float sqrtDet = std::sqrt(det);

        //First solution
        float distance = (-b - sqrtDet) / 2;
        //Fallback solution
        if (distance < 0) distance = (-b + sqrtDet) / 2;

        //Hit something
        if (distance >= 0)
        {
            result.distance = distance;
            result.hit = true;
            result.hitPoint = position + direction * distance;
            result.normal = result.hitPoint.normalize();
            result.instance = this;
        }
    }
    return result;
}

vec3 SphereInstance::get_random_point_on_surface(std::default_random_engine& random, float bias)
{
    std::uniform_real_distribution<> dist(0, 1);
    float r1 = dist(random);
    float r2 = dist(random);

    //unit sphere at (0, 0, 0) with radius = 1
    vec3 point;
    point.x = 2 * std::cos(2 * PI * r1) * std::sqrt(r2 * (1 - r1));
    point.y = 2 * std::sin(2 * PI * r1) * std::sqrt(r2 * (1 - r1));
    point.z = 1 - 2 * r2;

    //extrude point from surface using the bias value
    point *= (1 + bias);

    return transform.multiply(point);
}