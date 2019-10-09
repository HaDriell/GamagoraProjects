#include "MeshInstance.h"

MeshInstance::~MeshInstance() {}

HitResult MeshInstance::intersect(const vec3& position, const vec3& direction)
{
    HitResult hit;
    if (mesh.intersect(position, direction, hit.distance, hit.hitPoint, hit.normal))
    {
        hit.hit = true;
        hit.instance = this;
    }
    return hit;
    // HitResult hit;
    // hit.distance = std::numeric_limits<float>::max();
    // int triangle = 0;
    // while (triangle < mesh.vertices.size())
    // {
    //     Triangle t;
    //     t.vertex0 = mesh.vertices[triangle++].position;
    //     t.vertex1 = mesh.vertices[triangle++].position;
    //     t.vertex2 = mesh.vertices[triangle++].position;
        
    //     float distance;
    //     vec3 hitPoint;
    //     vec3 normal;

    //     //Static Ray x Triangle intersection
    //     if (intersectTriangle(position, direction, t, distance, hitPoint, normal))
    //     {
    //         if (distance > 0 && distance < hit.distance)
    //         {
    //             hit.hit = true;
    //             hit.distance = distance;
    //             hit.hitPoint = hitPoint;
    //             hit.normal = normal;
    //             hit.instance = this;
    //         }
    //     }
    // }
    // return hit;
}

vec3 MeshInstance::get_random_point_on_surface(std::default_random_engine& random, float bias)
{
    return transform.multiply(vec3()); // not supported yet
}