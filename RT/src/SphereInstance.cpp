#include "SphereInstance.h"

#include <math.h>
#include <random>


SphereInstance::~SphereInstance() {}

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

//https://math.stackexchange.com/questions/1163260/random-directions-on-hemisphere-oriented-by-an-arbitrary-vector?rq=1
vec3 SphereInstance::get_random_point_on_surface(std::default_random_engine& random, float bias)
{
    std::uniform_real_distribution<> dx(-1, 1);
    std::uniform_real_distribution<> dy(-1, 1);
    std::uniform_real_distribution<> dz(-1, 1);

    vec3 point;
    do
    {
        point.x = dx(random);
        point.y = dy(random);
        point.z = dz(random);
    } while (point.length2() > 1);
    point = point.normalize() + bias; // normalize & extrude from sphere using the bias value
    return transform.multiply(point);
}