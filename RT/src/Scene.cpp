#include "Scene.h"

void Scene::createPointLight(const glm::vec3& position, float intensity, const glm::vec3& color)
{
    pointLights.push_back(new PointLight{position, intensity, color});
}

void Scene::createCubeLight(const glm::vec3& position, float size, float intensity, const glm::vec3& color)
{
    cubeLights.push_back(new CubeLight{position, size, intensity, color});
}

void Scene::createSphere(const glm::vec3& position, float radius, const glm::vec3& color)
{
    spheres.push_back(new Sphere{position, radius, color});
}

void Scene::createTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& color)
{
    triangles.push_back(new Triangle{v0, v1, v2, color});
}

Scene::~Scene()
{
    for(auto&& instance : pointLights)
    {
        delete instance;
    }
    pointLights.clear();

    for (auto&& instance : cubeLights)
    {
        delete instance;
    }
    cubeLights.clear();

    for(auto&& instance : spheres)
    {
        delete instance;
    }
    pointLights.clear();

    for(auto&& instance : triangles)
    {
        delete instance;
    }
    pointLights.clear();    
}