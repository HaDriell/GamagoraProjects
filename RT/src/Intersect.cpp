#include "Intersect.h"
#include <limits>

bool intersect(const Ray& ray, const Sphere& sphere, float& t)
{
    glm::vec3 oc = ray.position - sphere.position;
    float b = 2 * glm::dot(ray.direction, oc);
    float c = glm::dot(oc, oc) - (sphere.radius * sphere.radius);

    float det = (b * b) - (4 * c);
    if (det >= 0.f)
    {
        float sqDet = std::sqrt(det);

        //First solution
        float root = (- b - sqDet) / 2;
        if (root >= 0.f)
        {
            t = root;
            return true;
        }

        //Second solution
        root = (- b + sqDet) / 2;
        if (root >= 0.f)
        {
            t = root;
            return true;
        }

        //No solution
    }

    //No intersection
    return false;
}

bool intersect(const Ray& ray, const Triangle& triangle, float& t)
{
    //https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    //compute plane's normal
    const float epsilon = 1e-8;

    glm::vec3 edge1 = triangle.v1 - triangle.v0;
    glm::vec3 edge2 = triangle.v2 - triangle.v0;

    glm::vec3 h = glm::cross(ray.direction, edge2);
    float a = glm::dot(edge1, h);

    if (std::abs(a) < epsilon)
        return false;

    float f = 1.f / a;

    glm::vec3 s = ray.position - triangle.v0;
    float u = f * glm::dot(s, h);
    if  (u < 0.f || u > 1.f)
        return false;

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(ray.direction, q);
    if (v < 0.f || u + v > 1.f)
        return false;
    
    t = f * glm::dot(edge2, q);
    return t > epsilon;
}

bool intersectNearest(const Ray& ray, const std::vector<Sphere*>& spheres, float& t, Sphere& sphere)
{
    bool hit = false;
    for (auto& spherePtr : spheres)
    {
        float dt;
        if (intersect(ray, *spherePtr, dt) && (!hit || dt < t))
        {
            t = dt;
            sphere = *spherePtr;
            hit = true;
        }
    }
    return hit;
}

bool intersectNearest(const Ray& ray, const std::vector<Triangle*>& triangles, float& t, Triangle& triangle)
{
    bool hit = false;
    for (auto& trianglePtr : triangles)
    {
        float dt;
        if (intersect(ray, *trianglePtr, dt) && (!hit || dt < t))
        {
            t = dt;
            triangle = *trianglePtr;
            hit = true;
        }
    }
    return hit;
}