#include "SphereInstance.h"

#include <math.h>
#include <random>


SphereInstance::~SphereInstance() {}

vec3 SphereInstance::get_centroid() const
{
    return transform.multiply(vec3());
}

AABB SphereInstance::get_bounding_box() const
{
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
    HitResult hit;

    Sphere unitSphere = Sphere();
    unitSphere.position = vec3();
    unitSphere.radius = 1.0f;

    float distance;
    vec3 hitPoint;
    vec3 normal;
    hit.intersections++;
    if (intersectSphere(position, direction, unitSphere, distance, hitPoint, normal))
    {
        hit.hit = true;
        hit.hitPoint = hitPoint;
        hit.distance = distance;
        hit.normal = normal;
        hit.instance = this;
    }
    return hit;
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