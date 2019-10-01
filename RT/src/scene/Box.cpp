#include "Box.h"

Box::~Box() {}

HitResult Box::intersect(const vec3& position, const vec3& direction)
{
    //https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
    vec3 invDir = 1 / direction; // protection against division by zero

    //Unit AABB Cube
    vec3 min = vec3(-.5, -.5, -.5);
    vec3 max = vec3(+.5, +.5, +.5);

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin  = ((direction.x < 0 ? min.x : max.x) - position.x) * invDir.x;
    tmax  = ((direction.x < 0 ? max.x : min.x) - position.x) * invDir.x;
    tymin = ((direction.y < 0 ? min.y : max.y) - position.y) * invDir.y;
    tymax = ((direction.y < 0 ? max.y : min.y) - position.y) * invDir.y;

    if (tmin > tymax || tymin > tmax)
        return HitResult();
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = ((direction.z < 0 ? min.z : max.z) - position.z) * invDir.z;
    tzmax = ((direction.z < 0 ? max.z : min.z) - position.z) * invDir.z;

    if ((tmin > tzmax) || (tzmin > tmax)) 
        return HitResult();

    if (tzmin > tmin) 
        tmin = tzmin; 
    if (tzmax < tmax) 
        tmax = tzmax; 

    //Sovle ?
    return HitResult();
}

vec3 Box::get_random_point_on_surface(std::default_random_engine& random, float bias)
{
    std::uniform_int_distribution<> dist_face(0, 5);
    vec3 origin, u, v, n;

    int face = dist_face(random);

    switch (face)
    {
    case 0: // back
        origin = vec3(-0.5, -0.5, -0.5);
        u      = vec3::X;
        v      = vec3::Y;
        n      = -vec3::Z;
    break;

    case 1: // bottom
        origin = vec3(-0.5, -0.5, -0.5);
        u      = vec3::X;
        v      = vec3::Z;
        n      = -vec3::Y;
    break;

    case 2: // left
        origin = vec3(-0.5, -0.5, -0.5);
        u      = vec3::Y;
        v      = vec3::Z;
        n      = -vec3::X;
    break;

    case 3: // front
        origin = vec3(+0.5, +0.5, +0.5);
        u      = -vec3::X;
        v      = -vec3::Y;
        n      = vec3::Z;
    break;

    case 4: // top
        origin = vec3(+0.5, +0.5, +0.5);
        u      = -vec3::X;
        v      = -vec3::Z;
        n      = vec3::Y;
    break;

    case 5: // right
        origin = vec3(+0.5, +0.5, +0.5);
        u      = -vec3::Y;
        v      = -vec3::Z;
        n      = vec3::X;
    break;
    }

    std::uniform_real_distribution<> dist_u(0, 1.f);
    std::uniform_real_distribution<> dist_v(0, 1.f);

    return origin + (u * dist_u(random)) + (v * dist_v(random)) + (n * bias);
}