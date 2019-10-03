#pragma once

#include <vector>
#include "Instance.h"
#include "PointLight.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Mesh.h"

#include "Camera.h"

struct Scene
{
    //Scene rendering configuration
    unsigned int    pixel_sampling = 5;
    unsigned int    light_sampling = 100;
    unsigned int    ray_max_bounce = 5;
    float           bias           = 1e-4;

    Camera          camera;

    //Scene objects
    std::vector<PointLight*>    pointLights;
    std::vector<Instance*>      instances;

    Triangle*   createTriangle(const vec3& a, const vec3& b, const vec3& c);
    Sphere*     createSphere(const vec3& position, float radius);
    Mesh*       createMesh();

    PointLight* createLight(const vec3& position, float intensity, const vec3& color);

    ~Scene();

    HitResult raycast(const vec3& position, const vec3& direction) const;
    void render();
};


HitResult raycast(const Scene& scene, const vec3& position, const vec3& direction);