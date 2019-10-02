#include "Scene.h"
#include "Sphere.h"
#include "PointLight.h"

#include <math.h>
#include <limits>
#include <omp.h>

#include <random>


HitResult Scene::raycast(const vec3& position, const vec3& direction) const
{
    HitResult hit;
    for (Instance* current : instances)
    {
        HitResult result = current->hit(position, direction);
        if (result.hit && (!hit.hit || result.distance < hit.distance))
        {
            hit = result;
        }
    }
    return hit;
}

vec3 get_point_light_illumination(const Scene& scene, const PointLight& light, const HitResult& hit)
{
    vec3 htl = (light.position - hit.hitPoint).normalise();
    float lightDistance2 = light.position.distance2(hit.hitPoint);

    HitResult light_hit = scene.raycast(hit.hitPoint + hit.normal * scene.bias, htl);
    if (!light_hit.hit || light_hit.distance >= lightDistance2)
    {
        float lightDistanceFactor = 1.f / lightDistance2;
        float hitAngleFactor = hit.normal.dot(htl);
        float intensity = lightDistanceFactor * hitAngleFactor * light.intensity;
        if (intensity < 0) intensity = 0;
        return light.color * intensity;
    }
    return vec3();
}

vec3 get_surface_illumination(const Scene& scene, const HitResult& hit)
{
    std::default_random_engine prng;
    vec3 total = vec3();
    for (PointLight* light : scene.pointLights)
    {
        total += get_point_light_illumination(scene, *light, hit);
    }

    for (Instance* instance : scene.instances)
    {
        if (instance->material.emissive)
        {
            for (unsigned int sample = 0; sample < scene.light_sampling; sample++)
            {
                PointLight light;
                light.position = instance->get_random_point_on_surface(prng, scene.bias);
                light.intensity = instance->material.emission_intensity / (float) scene.light_sampling;
                light.color = instance->material.emission_color;
                total += get_point_light_illumination(scene, light, hit);                
            }
        }
    }
    return total;
}

vec3 trace(const Scene& scene, const vec3& position, const vec3& direction, unsigned int level)
{
    //Abort tracing above a certain amount of bounces
    if (level >= scene.ray_max_bounce)
        return vec3();

    //Find the nearest hit (if any)
    HitResult hit = scene.raycast(position, direction);
    //Abort tracing if no hits
    if (!hit.hit)
        return vec3(); // background color
    
    //Now we want to compute the color of the surface using the Material of the hit object
    Material material = hit.instance->material;
    vec3 color = vec3();
    
    //The Hit surface is opaque diffuse
    if (material.diffuse)
    {
        //Direct illumination
        vec3 illumination = get_surface_illumination(scene, hit);
        color += illumination * material.albedo; // should be attenuated by 2*PI but who cares it's just about lighting intensities
        
        //Indirect illumination
        //TODO
    }

    //The Hit surface emits light
    if (material.emissive)
    {
        //Direct Light bearing !
        color += material.emission_color * material.emission_intensity / (hit.distance * hit.distance);
    }

    if (material.reflective)
    {
        //TODO
        vec3 reflection_position = hit.hitPoint + hit.normal * scene.bias;
        vec3 reflection_direction = direction.reflect(hit.normal).normalise();
        color += material.reflectiveness * trace(scene, reflection_position, reflection_direction, level + 1);
    }

    return color;
}

void Scene::render()
{
    //Clear the framebuffer before rendering
    camera.framebuffer.clear();

    float focalDistance = camera.get_focal_distance();
    float imageAspectRatio = camera.get_aspect_ratio();
    float scale = std::tan(camera.fov * deg2rad / 2);

    //Build the R matrix
    mat4 rotation = mat4::RotationYXZ(camera.rotation);

    vec3 u = (rotation * vec3::X).normalise();
    vec3 v = (rotation * vec3::Y).normalise();
    vec3 w = (rotation * vec3::Z).normalise();

    std::cout << "Camera Settings" << std::endl;
    std::cout << "u: " << u << std::endl;
    std::cout << "v: " << v << std::endl;
    std::cout << "w: " << w << std::endl;
    std::cout << "focalDistance: " << focalDistance << std::endl;
    std::cout << "Framebuffer: " << camera.framebuffer.width << "x" << camera.framebuffer.height << std::endl;
    std::cout << "Scale:" << scale << std::endl;

    #pragma omp parallel for
    for (unsigned int y = 0; y < camera.framebuffer.height; y++)
    {
        for (unsigned int x = 0; x < camera.framebuffer.width; x++)
        {
            float dx = (x + 0.5f - camera.framebuffer.width / 2) * imageAspectRatio;
            float dy = (y + 0.5f - camera.framebuffer.height / 2);

            vec3 direction = (u * dx) + (v * dy) + (w * focalDistance);
            direction = direction.normalise();

            vec3 color = trace(*this, camera.position, direction, 0);
            camera.framebuffer.write(x, y, color, 1);
        }
    }
}

Scene::~Scene()
{
    for(Instance* instance : instances)
        delete instance;
    instances.clear();
    for(PointLight* instance : pointLights)
        delete instance;    
    pointLights.clear();    
}

Sphere* Scene::createSphere(const vec3& position, float radius)
{
    Sphere* instance = new Sphere();
    instance->transform.translate(position.x, position.y, position.z);
    instance->transform.scale(radius, radius, radius);

    instances.push_back(instance);

    return instance;
}

PointLight* Scene::createLight(const vec3& position, float intensity, const vec3& color)
{
    PointLight* instance = new PointLight();
    instance->position = position;
    instance->intensity = intensity;
    instance->color = color;

    pointLights.push_back(instance);

    return instance;
}


Mesh* Scene::createMesh()
{
    Mesh* instance = new Mesh();

    instances.push_back(instance);

    return instance;
}