#include "Render.h"
#include "Util.h"

#include <limits>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

glm::vec3 getPointLightDirectIllumination(const Scene& scene, const glm::vec3& point, const glm::vec3& normal)
{
    glm::vec3 color = glm::vec3{0.f, 0.f, 0.f};
    for (auto& light : scene.pointLights)
    {
        glm::vec3 htl = glm::normalize(light->position - point);
        float lightDistance2 = glm::distance2(light->position, point);

        Ray ray;
        ray.position = light->position;
        ray.direction = -htl;

        //Sphere intersection data
        float tSphere = std::numeric_limits<float>::max();
        Sphere sphere;
        bool hitSphere = intersectNearest(ray, scene.spheres, tSphere, sphere);

        //Triangle intersection data
        float tTriangle = std::numeric_limits<float>::max();
        Triangle triangle;
        bool hitTriangle = intersectNearest(ray, scene.triangles, tTriangle, triangle);


        //Merge the results

        //Sphere is nearer
        if (hitSphere && tSphere < tTriangle)
        {
            if (lightDistance2 >= (tSphere * tSphere) - 15)
            {
                float lightDistanceFactor = 1.f / lightDistance2;
                float hitAngleFactor = glm::dot(normal, htl);
                float intensity = lightDistanceFactor * hitAngleFactor * light->intensity;
                if (intensity < 0) intensity = 0;
                color += sphere.color * (light->color * intensity);
            }
        }
        
        //Triangle is nearer
        if (hitTriangle && tTriangle < tSphere)
        {
            if (lightDistance2 >= (tTriangle * tTriangle) - 15)
            {
                float lightDistanceFactor = 1.f / lightDistance2;
                float hitAngleFactor = glm::dot(normal, htl);
                float intensity = lightDistanceFactor * hitAngleFactor * light->intensity;
                if (intensity < 0) intensity = 0;
                color += triangle.color * (light->color * intensity);
            }
        }
    }
    return color;
}

void render(const Scene& scene, Framebuffer& framebuffer, const Camera& camera)
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

            //Try to hit a Sphere or a Triangle
            float tSphere = std::numeric_limits<float>::max();
            Sphere sphere;
            bool hitSphere = intersectNearest(ray, scene.spheres, tSphere, sphere);

            float tTriangle = std::numeric_limits<float>::max();
            Triangle triangle;
            bool hitTriangle = intersectNearest(ray, scene.triangles, tTriangle, triangle);

            //Merge the results
            //Sphere is nearer
            if (hitSphere && tSphere < tTriangle)
            {
                glm::vec3 hitPoint = ray.position + ray.direction * tSphere;
                glm::vec3 normal = glm::normalize(hitPoint - sphere.position);
                glm::vec3 color = getPointLightDirectIllumination(scene, hitPoint, normal);
                framebuffer.write(i, j, color, tSphere);
            }

            //Triangle is nearer
            if (hitTriangle && tTriangle < tSphere)
            {
                glm::vec3 hitPoint = ray.position + ray.direction * tTriangle;
                glm::vec3 v0v1 = triangle.v1 - triangle.v0;
                glm::vec3 v0v2 = triangle.v2 - triangle.v0;
                glm::vec3 normal = glm::normalize(glm::cross(v0v1, v0v2));
                glm::vec3 color = getPointLightDirectIllumination(scene, hitPoint, normal);
                framebuffer.write(i, j, color, tTriangle);
            }
        }
    }
}