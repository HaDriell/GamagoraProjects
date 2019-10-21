#include "MeshInstance.h"

static inline vec3 __min_XYZ(const vec3& a, const vec3& b)
{
    vec3 out;
    out.x = std::min(a.x, b.x);
    out.y = std::min(a.y, b.y);
    out.z = std::min(a.z, b.z);
    return out;
}

static inline vec3 __max_XYZ(const vec3& a, const vec3& b)
{
    vec3 out;
    out.x = std::max(a.x, b.x);
    out.y = std::max(a.y, b.y);
    out.z = std::max(a.z, b.z);
    return out;
}

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
}

vec3 MeshInstance::get_centroid() const
{
    ::transform transform = this->transform;
    vec3 centroid = mesh.get_centroid();
    return transform.multiply(centroid);
}

AABB MeshInstance::get_bounding_box() const
{
    ::transform transform = this->transform;
    AABB lbox = mesh.get_bounding_box();
    
    std::vector<vec3> lbox_vertices;
    lbox_vertices.push_back( transform.multiply(vec3(lbox.min.x, lbox.min.y, lbox.min.z)) );
    lbox_vertices.push_back( transform.multiply(vec3(lbox.max.x, lbox.min.y, lbox.min.z)) );
    lbox_vertices.push_back( transform.multiply(vec3(lbox.min.x, lbox.max.y, lbox.min.z)) );
    lbox_vertices.push_back( transform.multiply(vec3(lbox.max.x, lbox.max.y, lbox.min.z)) );
    lbox_vertices.push_back( transform.multiply(vec3(lbox.min.x, lbox.min.y, lbox.max.z)) );
    lbox_vertices.push_back( transform.multiply(vec3(lbox.max.x, lbox.min.y, lbox.max.z)) );
    lbox_vertices.push_back( transform.multiply(vec3(lbox.min.x, lbox.max.y, lbox.max.z)) );
    lbox_vertices.push_back( transform.multiply(vec3(lbox.max.x, lbox.max.y, lbox.max.z)) );


    AABB gbox;
    gbox.min = vec3(std::numeric_limits<float>::max());
    gbox.max = vec3(std::numeric_limits<float>::min());
    for (const vec3& vertex : lbox_vertices)
    {
        gbox.min = __min_XYZ(gbox.min, vertex);
        gbox.max = __max_XYZ(gbox.max, vertex);
    }
    return gbox;
}

bool MeshInstance::intersectBoundingBox(const vec3& position, const vec3& direction) const
{
    return intersectAABB(position, direction, mesh.get_bounding_box());
}


vec3 MeshInstance::get_random_point_on_surface(std::default_random_engine& random, float bias)
{
    return transform.multiply(vec3()); // not supported yet
}