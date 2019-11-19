#include "Scene.h"
#include "Sphere.h"
#include "PointLight.h"
#include "Timer.h"

#include <math.h>
#include <limits>
#include <omp.h>

#include <random>


static inline vec3 __min_XYZ(const vec3& a, const vec3& b)
{
    vec3 out;
    out.x = std::min(a.x, b.x);
    out.y = std::min(a.y, b.y);
    out.z = std::min(a.z, b.z);
    return out;
}

static inline vec3 __max_XYZ(const vec3& a, const vec3& b)
{
    vec3 out;
    out.x = std::max(a.x, b.x);
    out.y = std::max(a.y, b.y);
    out.z = std::max(a.z, b.z);
    return out;
}

vec3 trace(const Scene& scene, const vec3& position, const vec3& direction, unsigned int level, std::default_random_engine& entropy);
vec3 get_point_light_illumination(const Scene& scene, const PointLight& light, const HitResult& hit);
vec3 get_indirect_surface_illumination(const Scene& scene, const HitResult& hit, int level, std::default_random_engine& entropy);
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

    //Notify metrics
    metrics.raycastCount++;
    metrics.intersectionCount += hit.intersections;
    return hit;
}

vec3 get_point_light_illumination(const Scene& scene, const PointLight& light, const HitResult& hit)
{
    vec3 htl = (light.position - hit.hitPoint).normalize();
    float lightDistance2 = vec3::distance2(light.position, hit.hitPoint);

    HitResult light_hit = scene.raycast(hit.hitPoint + hit.normal * scene.bias, htl);
    if (!light_hit.hit || light_hit.distance >= lightDistance2)
    {
        float lightDistanceFactor = 1.f / lightDistance2;
        float hitAngleFactor = vec3::dot(hit.normal, htl);
        float intensity = lightDistanceFactor * hitAngleFactor * light.intensity;
        if (intensity < 0) intensity = 0;
        return light.color * intensity;
    }
    return vec3();
}

vec3 get_indirect_surface_illumination(const Scene& scene, const HitResult& hit, int level, std::default_random_engine& entropy)
{
    //Indirect illumination
    std::uniform_real_distribution<> dist(0, 1);

    vec3 total = vec3();
    for (unsigned int sample = 0; sample < scene.indirect_illumination_sampling; sample++)
    {
        float r1 = dist(entropy);
        float r2 = dist(entropy);

        //random vector in hemisphere pointing up (Y)
        float x = std::cos(2 * PI * r1) * std::sqrt(1 - r2);
        float y = std::sin(2 * PI * r1) * std::sqrt(1 - r2);
        float z = std::sqrt(r2);

        //Oriented base
        vec3 f = hit.normal;
        vec3 v = vec3(0.5f, 0.3f,-0.2f).normalize();
        vec3 s = vec3::cross(f, v).normalize();
        vec3 u = vec3::cross(s, f).normalize();

        vec3 indirection = ((s * x) + (u * y) + (f * z)).normalize();
        
        //Trace using recursion
        total += trace(scene, hit.hitPoint + hit.normal * scene.bias, indirection, level + 1, entropy) / (2.f * scene.indirect_illumination_sampling);
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
        vec3 illumination = get_direct_surface_illumination(scene, hit, entropy);
        illumination += get_indirect_surface_illumination(scene, hit, level, entropy);
        //Apply the material properties
        color += illumination * material.opaqueness * material.albedo; // should be attenuated by 2*PI but who cares it's just about lighting intensities
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
        vec3 reflection_direction = vec3::reflect(direction, hit.normal).normalize();
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
    mat4 rotation = mat4::RotationXYZ(camera.rotation);

    vec3 u = (rotation * vec3::X).normalize();
    vec3 v = (rotation * vec3::Y).normalize();
    vec3 w = (rotation * vec3::Z).normalize();


    //Mertic reset
    metrics.raycastCount        = 0;
    metrics.instanceCount       = 0;
    metrics.intersectionCount   = 0;

    std::cout << "Rendering Scene. Please wait " << std::endl;    
    int progress = 0;
    for (unsigned int y = 0; y < camera.framebuffer.height; y++)
    {
        progress = ((y + 1) * 100 / camera.framebuffer.height);
        std::cout << "Progress " << progress << "/100%"  << "\r" << std::flush;

        #pragma omp parallel for
        for (unsigned int x = 0; x < camera.framebuffer.width; x++)
        {
            std::default_random_engine entropy;
            unsigned int pixelHash = x + y * camera.framebuffer.width;
            entropy.seed(std::chrono::system_clock::now().time_since_epoch().count() + pixelHash);

            std::uniform_real_distribution<> dist(0, 1);
            vec3 color = vec3(0, 0, 0);
            for (int sample = 0; sample < pixel_sampling; sample++)
            {
                float dx = (x + dist(entropy) - camera.framebuffer.width / 2) * imageAspectRatio;
                float dy = (y + dist(entropy) - camera.framebuffer.height / 2);

                vec3 direction = (u * dx) + (v * dy) + (w * focalDistance);
                direction = direction.normalize();

                color += trace(*this, camera.position, direction, 0, entropy) / (float) pixel_sampling;
            }
            camera.framebuffer.write(x, y, color, 1);
        }
    }
    std::cout << std::endl;

    //Update Scene metrics
    metrics.instanceCount       = instances.size();

    std::cout << "Instances      : " << metrics.instanceCount << std::endl;
    std::cout << "Raycasts       : " << metrics.raycastCount << std::endl;
    std::cout << "Intersections  : " << metrics.intersectionCount << std::endl;
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