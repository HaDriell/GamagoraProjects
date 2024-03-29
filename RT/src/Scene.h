#pragma once

#include <vector>

#include "Instance.h"
#include "SphereInstance.h"
#include "MeshInstance.h"
#include "PointLight.h"
#include "Camera.h"


struct SceneMetrics
{
    uint64_t raycastCount      = 0;
    uint64_t instanceCount     = 0;
    uint64_t intersectionCount = 0;
};

struct Scene
{
    //Scene metrics
    mutable SceneMetrics    metrics;

    //Scene rendering configuration
    unsigned int    pixel_sampling = 10;
    unsigned int    light_sampling = 10;
    unsigned int    indirect_illumination_sampling = 1;
    unsigned int    ray_max_bounce = 5;
    float           bias           = 1e-4;

    Camera          camera;

    //Scene objects
    std::vector<PointLight*>    pointLights;
    std::vector<Instance*>      instances;

    SphereInstance* createSphere(const vec3& position, float radius);
    MeshInstance*   createMesh();
    MeshInstance*   createOBJMesh(const std::string& path);
    MeshInstance*   createOFFMesh(const std::string& path);

    PointLight* createLight(const vec3& position, float intensity, const vec3& color);

    ~Scene();

    HitResult raycast(const vec3& position, const vec3& direction) const;
    void render();
};