#include "Scene.h"
#include <sstream>


int main()
{
    Scene scene;
    scene.bias = 1;
    scene.pixel_sampling = 1;
    scene.light_sampling = 10;
    scene.camera.fov = 60;

    //Camera positionning
    scene.camera.position = vec3(0, 50, -100);
    scene.camera.rotation = vec3(0, -10, 0);

    //Pure Lighting Setup 
    // Sphere* sun = scene.createSphere(vec3(0, 1e5, 0), 300);
    // sun->material = Material::Emissive(1e10, 1, 1, 1);
    scene.createLight(vec3(0, 100, -100), 1e4, vec3(1, 1, 1));
    
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
    lapin->load_obj_file("cube.obj");
    lapin->load_obj_file("Love.obj");
    lapin->transform.set_translation(0, 0, 0);
    lapin->transform.set_scaling(20, 20, 20);
    lapin->material |= Material::Diffuse(1, 1, 1, 1);

    lapin->debug();
    std::cin.get();

    scene.render();
    scene.camera.framebuffer.save("RenderClassic.png");

    for (int i = 0; i < 10; i++)
    {
        lapin->transform.rotate(0, 36, 0);

        scene.render();
        std::stringstream filename;
        filename << "Frame" << i << ".png";
        scene.camera.framebuffer.save(filename.str());
    }
}