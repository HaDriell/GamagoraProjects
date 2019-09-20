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
    //compute plane's normal
    glm::vec3 v0v1 = triangle.v1 - triangle.v0;
    glm::vec3 v0v2 = triangle.v2 - triangle.v0;
    glm::vec3 pvec = glm::cross(ray.direction, v0v2);
    float det = glm::dot(pvec, v0v1);

    if (det < 1e-8) // det ~= 0
        return false;

    float invDet = 1 / det;

    glm::vec3 tvec = ray.position - triangle.v0;
    float u = glm::dot(tvec, pvec) * invDet;
    if  (u < 0 || u > 1)
        return false;

    glm::vec3 qvec = glm::cross(tvec, v0v1);
    float v = glm::dot(ray.direction, qvec) * invDet;
    if (v < 0 || v > 1)
        return false;

    t = glm::dot(v0v2, qvec) * invDet;
    return true;
}


bool intersectAny(const Ray& ray, const std::vector<Sphere*> spheres)
{
    for (auto& spherePtr : spheres)
    {
        float dt;
        if (intersect(ray, *spherePtr, dt))
            return true;
    }
    return false;
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