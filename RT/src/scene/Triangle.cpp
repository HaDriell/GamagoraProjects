#include "Triangle.h"

Triangle::~Triangle()
{

}

HitResult Triangle::intersect(const vec3& position, const vec3& direction)
{
    HitResult hit;
    hit.hit = false;

    const float EPSILON = 1e-6;
    vec3 edge1 = vertex1 - vertex0;
    vec3 edge2 = vertex2 - vertex0;

    vec3 h, s, q;
    float a, f, u, v;

    //Check is direction is parallel to the triangle
    h = direction.cross(edge2);
    a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON)
        return hit;

    f = 1.0/a;
    s = position - vertex0;
    u = f * (s.dot(h));
    if (u < 0.0 || u > 1.0)
        return hit;

    q = s.cross(edge1);
    v = f * direction.dot(q);
    if (v < 0.0 || u + v > 1.0)
        return hit;
    
    float t = f * edge2.dot(q);
    if (t > EPSILON)
    {
        hit.hit = true;
        hit.distance = t;
        hit.hitPoint = position + direction * t;
        hit.normal = edge1.cross(edge2).normalise();
        hit.instance = this;
    }

    return hit;
}
vec3 Triangle::get_random_point_on_surface(std::default_random_engine& random, float bias)
{
    //https://math.stackexchange.com/questions/18686/uniform-random-point-in-triangle
    vec3 edge1 = vertex1 - vertex0;
    vec3 edge2 = vertex2 - vertex0;
    vec3 normal = edge1.cross(edge2).normalise();
    std::uniform_real_distribution<> dist_r1(0, 1);
    std::uniform_real_distribution<> dist_r2(0, 1);
    float r1 = std::sqrt( dist_r1(random) );
    float r2 = dist_r2(random);

    vec3 point = (1 - r1) * vertex0 + r1*(1 - r2) * vertex1 + r2*r1 * vertex2;

    return point + normal * bias;
}