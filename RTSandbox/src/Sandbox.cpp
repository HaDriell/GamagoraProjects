#include "Scene.h"
#include <sstream>


int main()
{
    Scene scene;
    scene.bias = 1;
    scene.pixel_sampling = 5;
    scene.light_sampling = 100;
    scene.camera.fov = 60;

    //Camera positionning
    scene.camera.position = vec3(0, 0, -200);

    //Pure Lighting Setup 
    Sphere* sun = scene.createSphere(vec3(0, 1e5, 0), 300);
    sun->material = Material::Emissive(1e10, 1, 1, 1);
    
    //Instances Setup
    // Sphere* red = scene.createSphere(vec3(-100, 0, 0), 30);
    // red->material = Material::Diffuse(1, 0.8, 0.2, 0.2);

    // Sphere* blue = scene.createSphere(vec3(100, 30, -10), 50);
    // blue->material = Material::Diffuse(1, 0.2, 0.2, 0.8);

    // Sphere* green = scene.createSphere(vec3(0, -30, 0), 20);
    // green->material = Material::Diffuse(1, 0.2, 0.8, 0.2);

    Sphere* floor = scene.createSphere(vec3(0, -1e4, 0), 1e4-100);
    floor->material = Material::Diffuse(1, 0.3, 0.3, 0.3);

    // Sphere* mirror = scene.createSphere(vec3(-50, 0, 50), 60);
    // mirror->material = Material::Reflective(1);

    // Sphere* chrome = scene.createSphere(vec3(0, -50, -50), 25);
    // chrome->material |= Material::Reflective(0.5);
    // chrome->material |= Material::Diffuse(0.5, 0.5, 0.5, 0.5);

    Mesh* lapin = scene.createMesh();
    lapin->load_obj_file("Love.obj"); 
    lapin->transform.set_translation(0, -60, 0);

    // lapin->transform.scale(30, 30, 30);
    // lapin->normalize();

    lapin->material |= Material::Diffuse(1, 1, 1, 1);

    vec3 min = vec3(std::numeric_limits<float>::max());
    vec3 max = vec3(std::numeric_limits<float>::min());
    for (vec3 p : lapin->positions)
    {
        vec3 tp = lapin->transform.multiply(p);
        if (min.x > tp.x) min.x = tp.x;
        if (min.y > tp.y) min.y = tp.y;
        if (min.z > tp.z) min.z = tp.z;

        if (max.x < tp.x) max.x = tp.x;
        if (max.y < tp.y) max.y = tp.y;
        if (max.z < tp.z) max.z = tp.z;

        std::cout << "Local Vertex " << p <<  " Global Vertex " << tp << std::endl;
    }

    std::cout << "AABB - " << min << " <-> " << max << std::endl;

    scene.render();
    scene.camera.framebuffer.save("RenderClassic.png");

    for (int i = 0; i < 10; i++)
    {
        scene.camera.position.z += 20;

        scene.render();
        std::stringstream filename;
        filename << "Frame" << i << ".png";
        scene.camera.framebuffer.save(filename.str());
    }
}