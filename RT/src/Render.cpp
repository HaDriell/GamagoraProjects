#include "Render.h"
#include "Util.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

glm::vec3 getPointLightDirectIllumination(const Scene& scene, const glm::vec3& point, const glm::vec3& normal)
{
    glm::vec3 color = {0.f, 0.f, 0.f};
    for (auto& light : scene.pointLights)
    {
        glm::vec3 htl = light->position - point;
        Ray ray;
        ray.position = light->position;
        ray.direction = glm::normalize(-htl);

        float t;
        Sphere s;
        if (intersectNearest(ray, scene.spheres, t, s))
        {
            float lightDistance2 = glm::dot(htl, htl);

            if (lightDistance2 >= (t * t) - 15)
            {
                float lightDistanceFactor = 1.f / lightDistance2;
                float hitAngleFactor = glm::dot(normal, glm::normalize(htl));
                float intensity = lightDistanceFactor * hitAngleFactor * light->intensity;
                if (intensity < 0) intensity = 0;
                color += light->color * intensity;
            }
        }
    }
    return color;
}

void render(const Scene& scene, const Framebuffer& framebuffer, const Camera& camera)
{
    glm::mat3 rotation = glm::toMat3(camera.orientation);
    glm::vec3 u = rotation * glm::vec3{1.f, 0.f, 0.f}; // camera's X unit vector
    glm::vec3 v = rotation * glm::vec3{0.f, 1.f, 0.f}; // camera's Y unit vector
    glm::vec3 w = rotation * glm::vec3{0.f, 0.f, 1.f}; // camera's Z unit vector

    std::cout << "Camera unit vectors:" << std::endl;
    std::cout << glm::to_string(u) << std::endl;
    std::cout << glm::to_string(v) << std::endl;
    std::cout << glm::to_string(w) << std::endl;

    float aspectRatio = framebuffer.width / (float) framebuffer.height;
    float focalDistance = (framebuffer.width / 2) / std::tan(deg2rad(camera.fov / 2));

    std::cout << "AspectRatio: " << aspectRatio << std::endl;
    std::cout << "FocalDistance: " << focalDistance << std::endl;

    //Camera's forward is the Z axis
    for (int j = 0; j < framebuffer.height; j++)
    {
        for (int i = 0; i < framebuffer.width; i++)
        {
            float dx = (i + 0.5f - framebuffer.width / 2) * aspectRatio;
            float dy = (j + 0.5f - framebuffer.height / 2);

            //build the target from the position
            glm::vec3 target = camera.position + (u * dx) + (v * dy) + (w * focalDistance);

            //Ray is ready to get out
            Ray ray;
            ray.position = camera.position;
            ray.direction = glm::normalize(target - camera.position);

            Sphere sphere;
            float t = 0.f;
            if (intersectNearest(ray, scene.spheres, t, sphere))
            {
                glm::vec3 hitPoint = ray.position + ray.direction * t;
                glm::vec3 normal = glm::normalize(hitPoint - sphere.position);
                glm::vec3 color = getPointLightDirectIllumination(scene, hitPoint, normal);
                framebuffer.data[i + j * framebuffer.width] = sphere.color * color;
            } else {
                framebuffer.data[i + j * framebuffer.width] = glm::vec3{0, 0, 0};
            }
        }
    }
}