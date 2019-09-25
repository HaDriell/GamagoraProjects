#pragma once

#include <vector>

#include "models/Triangle.h"
#include "models/PointLight.h"
#include "models/Sphere.h"
#include "models/CubeLight.h"

struct Scene
{
    //Light models
    std::vector<PointLight*> pointLights;
    std::vector<CubeLight*> cubeLights;
    unsigned int bl_sample_count = 4;

    //Sphere models
    std::vector<Sphere*> spheres;
    std::vector<Triangle*> triangles;

    void createPointLight(const glm::vec3& position, float intensity, const glm::vec3& color);
    void createCubeLight(const glm::vec3& position, float size, float intensity, const glm::vec3& color);

    void createSphere(const glm::vec3& position, float radius, const glm::vec3& color);
    void createTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& color);
    ~Scene();
};