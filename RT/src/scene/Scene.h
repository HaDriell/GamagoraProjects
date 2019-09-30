#pragma once

#include <vector>
#include "Instance.h"
#include "PointLight.h"
#include "Sphere.h"

#include "Camera.h"

struct Scene
{
    //Scene rendering configuration
    unsigned int    light_sampling_count    = 100;
    unsigned int    ray_max_bounce          = 5;
    float           bias                    = 1e-4;

    Camera          camera;

    //Scene objects
    std::vector<PointLight*>    pointLights;
    std::vector<Instance*>      instances;

    Sphere*     createSphere(const vec3& position, float radius);
    PointLight* createLight(const vec3& position, float intensity, const vec3& color);

    ~Scene();

    HitResult raycast(const vec3& position, const vec3& direction) const;
    void render();
};


HitResult raycast(const Scene& scene, const vec3& position, const vec3& direction);