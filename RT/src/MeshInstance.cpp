#include "MeshInstance.h"


inline void __minXYZ(vec3& m, const vec3& b)
{
        m.x = std::min(m.x, b.x);
        m.y = std::min(m.y, b.y);
        m.z = std::min(m.z, b.z);
}

inline void __maxXYZ(vec3& m, const vec3& b)
{
        m.x = std::max(m.x, b.x);
        m.y = std::max(m.y, b.y);
        m.z = std::max(m.z, b.z);
}

void __split_half_X(MeshBVHNode& parent, )
{
    parent.left = new MeshBVHNode(half_left);
    parent.right = new MeshBVHNode(half_right);
}


MeshBVHNode::MeshBVHNode(const vector<Triangle>& triangles) : left(nullptr), right(nullptr)
{
    aabb.min = vec3(std::numeric_limits<float>::max());
    aabb.max = vec3(std::numeric_limits<float>::min());

    for (Triangle& t : triangles)
    {
        //Extend min of the AABB
        __minXYZ(aabb.min, t.vertex0);
        __minXYZ(aabb.min, t.vertex1);
        __minXYZ(aabb.min, t.vertex2);
        //Extend max of the AABB
        __maxXYZ(aabb.max, t.vertex0);
        __maxXYZ(aabb.max, t.vertex1);
        __maxXYZ(aabb.max, t.vertex2);
    }

    if (triangles.size() > 10)
    {
        //Split

    }
}

MeshBVHNode::~MeshBVHNode()
{

}

HitResult MeshBVHNode::intersect(const vec3& position, const vec3& direction)
{
    //TODO : implement
    return HitResult();
}

HitResult MeshBVHLeaf::intersect(const vec3& position, const3& direction)
{
    //TODO : implement
    return HitResult();
}


MeshInstance::~MeshInstance() {}

HitResult MeshInstance::intersect(const vec3& position, const vec3& direction)
{
    HitResult hit;
    hit.distance = std::numeric_limits<float>::max();

    int triangle = 0;
    while (triangle < mesh.vertices.size())
    {
        Triangle t;
        t.vertex0 = mesh.vertices[triangle++].position;
        t.vertex1 = mesh.vertices[triangle++].position;
        t.vertex2 = mesh.vertices[triangle++].position;
        
        float distance;
        vec3 hitPoint;
        vec3 normal;

        //Static Ray x Triangle intersection
        if (intersectTriangle(position, direction, t, distance, hitPoint, normal))
        {
            if (distance > 0 && distance < hit.distance)
            {
                hit.hit = true;
                hit.distance = distance;
                hit.hitPoint = hitPoint;
                hit.normal = normal;
                hit.instance = this;
            }
        }
    }
    return hit;
}

vec3 MeshInstance::get_random_point_on_surface(std::default_random_engine& random, float bias)
{
    return transform.multiply(vec3()); // not supported yet
}