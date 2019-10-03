#include "Sphere.h"

#include <math.h>
#include <random>


Sphere::~Sphere() {}

HitResult Sphere::intersect(const vec3& position, const vec3& direction)
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

vec3 Sphere::get_random_point_on_surface(std::default_random_engine& random, float bias)
{
    //TODO : a changer
    //http://mathworld.wolfram.com/SpherePointPicking.html
    std::uniform_real_distribution<> dist_u(-1, 1);
    std::uniform_real_distribution<> dist_theta(0, 2 * PI);

    float z = dist_u(random);
    float theta = dist_theta(random);
    float radius = std::sqrt(1 - z*z) + bias;


    float x = radius * std::cos(theta);
    float y = radius * std::sin(theta);

    //Local space Random point (shifted by bias)
    vec3 lrp = vec3(x, y, radius);
    //Transform to world space
    return transform.multiply(lrp);
}