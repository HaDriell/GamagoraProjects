#include "scene/Scene.h"
#include <sstream>


int main()
{
    Scene scene;
    scene.bias = 1;
    scene.light_sampling_count = 500;
    scene.camera.fov = 60;

    //Camera positionning
    scene.camera.position = vec3(0, 0, -200);

    //Lighting Setup
    // PointLight* lamp = scene.createLight(vec3(0, 1e5, 0), 1e10, vec3(1, 1, 1));

    //Instances Setup

    Sphere* red = scene.createSphere(vec3(-100, 0, 0), 30);
    red->material = Material::Diffuse(0.8, 0.2, 0.2);

    Sphere* blue = scene.createSphere(vec3(100, 30, -10), 50);
    blue->material = Material::Diffuse(0.2, 0.2, 0.8);

    Sphere* green = scene.createSphere(vec3(0, -30, 0), 20);
    green->material = Material::Diffuse(0.2, 0.8, 0.2);

    Sphere* floor = scene.createSphere(vec3(0, -1e4, 0), 1e4-100);
    floor->material = Material::Diffuse(0.5, 0.5, 0.5);

    Sphere* sun = scene.createSphere(vec3(0, 1e5, 0), 300);
    sun->material = Material::Emissive(1e10, 1, 1, 1);
    
    Sphere* lamp = scene.createSphere(vec3(0, -50, -50), 5);
    lamp->material = Material::Emissive(1e4, 1, 1, 1);

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