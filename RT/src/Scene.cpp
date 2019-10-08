#include "Scene.h"
#include "Sphere.h"
#include "PointLight.h"
#include "Timer.h"

#include <math.h>
#include <limits>
#include <omp.h>

#include <random>

vec3 trace(const Scene& scene, const vec3& position, const vec3& direction, unsigned int level, std::default_random_engine& entropy);
vec3 get_point_light_illumination(const Scene& scene, const PointLight& light, const HitResult& hit);
vec3 get_indirect_surface_illumination(const Scene& scene, const HitResult& hit, int level, std::default_random_engine entropy);
vec3 get_direct_surface_illumination(const Scene& scene, const HitResult& hit, std::default_random_engine& entropy);


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
    vec3 htl = (light.position - hit.hitPoint).normalize();
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

vec3 get_indirect_surface_illumination(const Scene& scene, const HitResult& hit, int level, std::default_random_engine entropy)
{
    //Indirect illumination
    std::uniform_real_distribution<> dr1(0, 1);
    std::uniform_real_distribution<> dr2(0, 1);

    vec3 total = vec3();
    float samplingFactor = 1 / (float) scene.indirect_illumination_sampling;
    for (unsigned int sample = 0; sample < scene.indirect_illumination_sampling; sample++)
    {
        float r1 = dr1(entropy);
        float r2 = dr2(entropy);

        //random vector in hemisphere pointing up (Y)
        float x = std::cos(2 * PI * r1) * std::sqrt(1 - r2*r2);
        float y = r2;
        float z = std::sin(2 * PI * r1) * std::sqrt(1 - r2*r2);

        //Oriented base
        vec3 f = hit.normal;        // forward is "up"
        vec3 s = f.cross(vec3::UP); // side is "left" 
        vec3 u = s.cross(f);        // up is "side"
        vec3 indirection = (f * y) + (s * x) + (u * z);
        
        //Trace using recursion
        total += trace(scene, hit.hitPoint + hit.normal * scene.bias, indirection, level + 1, entropy) * samplingFactor;
    }
    return total;
}

vec3 get_direct_surface_illumination(const Scene& scene, const HitResult& hit, std::default_random_engine& entropy)
{
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
                light.position = instance->get_random_point_on_surface(entropy, scene.bias);
                light.intensity = instance->material.emission_intensity / (float) scene.light_sampling;
                light.color = instance->material.emission_color;
                total += get_point_light_illumination(scene, light, hit);                
            }
        }
    }
    return total;
}


vec3 trace(const Scene& scene, const vec3& position, const vec3& direction, unsigned int level, std::default_random_engine& entropy)
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
        //Illuminations
        vec3 d_illumination = get_direct_surface_illumination(scene, hit, entropy);
        vec3 i_illumination = get_indirect_surface_illumination(scene, hit, level, entropy);
        //Apply the material properties
        color += (d_illumination + i_illumination) * material.albedo; // should be attenuated by 2*PI but who cares it's just about lighting intensities
    }

    //The Hit surface emits light
    if (material.emissive)
    {
        //Direct Light bearing !
        color += material.emission_color * material.emission_intensity / (hit.distance * hit.distance);
    }

    if (material.reflective)
    {
        vec3 reflection_position = hit.hitPoint + hit.normal * scene.bias;
        vec3 reflection_direction = direction.reflect(hit.normal).normalize();
        color += material.reflectiveness * trace(scene, reflection_position, reflection_direction, level + 1, entropy);
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

    vec3 u = (rotation * vec3::X).normalize();
    vec3 v = (rotation * vec3::Y).normalize();
    vec3 w = (rotation * vec3::Z).normalize();

    std::cout << "Camera Settings" << std::endl;
    std::cout << "u: " << u << std::endl;
    std::cout << "v: " << v << std::endl;
    std::cout << "w: " << w << std::endl;
    std::cout << "focalDistance: " << focalDistance << std::endl;
    std::cout << "Framebuffer: " << camera.framebuffer.width << "x" << camera.framebuffer.height << std::endl;
    std::cout << "Scale:" << scale << std::endl;

    Timer timer;
    timer.reset();
    #pragma omp parallel for
    for (unsigned int y = 0; y < camera.framebuffer.height; y++)
    {
        for (unsigned int x = 0; x < camera.framebuffer.width; x++)
        {
            std::default_random_engine entropy;
            std::uniform_real_distribution<> dist_x(0, 1);
            std::uniform_real_distribution<> dist_y(0, 1);
            vec3 color = vec3(0, 0, 0);
            for (unsigned int sample = 0; sample < pixel_sampling; sample++)
            {
                float dx = (x + dist_x(entropy) - camera.framebuffer.width / 2) * imageAspectRatio;
                float dy = (y + dist_y(entropy) - camera.framebuffer.height / 2);

                vec3 direction = (u * dx) + (v * dy) + (w * focalDistance);
                direction = direction.normalize();

                color += trace(*this, camera.position, direction, 0, entropy) / (float) pixel_sampling;
            }
            camera.framebuffer.write(x, y, color, 1);
        }
    }
    std::cout << "Rendering time : " << timer.elapsed() << " seconds." << std::endl;
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

SphereInstance* Scene::createSphere(const vec3& position, float radius)
{
    SphereInstance* instance = new SphereInstance();
    instance->transform.translate(position.x, position.y, position.z);
    instance->transform.scale(radius, radius, radius);

    instances.push_back(instance);

    return instance;
}


MeshInstance* Scene::createMesh()
{
    MeshInstance* instance = new MeshInstance();

    instances.push_back(instance);

    return instance;
}

MeshInstance* Scene::createOBJMesh(const std::string& path)
{
    MeshInstance* instance = new MeshInstance();
    instance->mesh.load_obj_file(path);
    
    instances.push_back(instance);

    return instance;
}

MeshInstance* Scene::createOFFMesh(const std::string& path)
{
    MeshInstance* instance = new MeshInstance();
    instance->mesh.load_off_file(path);

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