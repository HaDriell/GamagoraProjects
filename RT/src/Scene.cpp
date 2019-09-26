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

void Scene::createInstance(const Mesh& mesh, const Transform& transform)
{
    int faceCount = mesh.indices.size() / 3;
    for (int face = 0; face < faceCount; face++)
    {
        int i0 = mesh.indices[3 * face + 0];
        int i1 = mesh.indices[3 * face + 1];
        int i2 = mesh.indices[3 * face + 2];

        glm::vec3 v0 = transform * mesh.positions[i0];
        glm::vec3 v1 = transform * mesh.positions[i1];
        glm::vec3 v2 = transform * mesh.positions[i2];

        //TODO fix color loading

        createTriangle(v0, v1, v2, glm::vec3{1.f, 1.f, 1.f});
    }
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