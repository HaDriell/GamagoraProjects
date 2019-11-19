#include "MeshInstance.h"
#include "Scene.h"
#include <sstream>

MeshInstance* createBlock(Scene& scene, float x, float y, float z, float gridSize)
{
    MeshInstance* block = scene.createOBJMesh("res/cube.obj");
    block->transform.setScale(gridSize, gridSize, gridSize);
    block->transform.setPosition(x * gridSize, y * gridSize, z * gridSize);
    block->material = Material::Diffuse(1, 0.15, 0.15, 0.15);
    return block;
}

int main()
{
    Scene scene;
    scene.bias = 1e-2;
    scene.pixel_sampling = 10;
    scene.light_sampling = 1;
    scene.indirect_illumination_sampling = 1;
    scene.ray_max_bounce = 5;
    scene.camera.fov = 60;
    scene.camera.framebuffer.resize(1024, 1024);

    //Camera positionning
    scene.camera.position = vec3(0, 0, -140);
    scene.camera.rotation = vec3(0, 0, 0);
    // scene.camera.rotation = vec3(0, -10, 0);

    // Comment one of them
    // PointLight* light = scene.createLight(vec3(0, 0, -300), 1e5, vec3(1, 1, 1));
    SphereInstance* light = scene.createSphere(vec3(0, 0, -300), 20);
    light->material |= Material::Emissive(2e5, 1, 1, 1);

    MeshInstance* back = createBlock(scene, 0, 0, +1, 100);
    back->material |= Material::Reflective(0.5);
    back->material |= Material::Diffuse(0.5, 1, 1, 1);
    for(int x = -1; x <= 1; x++)
    {
        for(int y = -1; y <= 1; y++)
        {
            if (x == 0 && y == 0)
                continue;
            createBlock(scene, x, y, 0, 100);
        }
    }

    SphereInstance* mirror = scene.createSphere(vec3(30, 30, 30), 10);
    mirror->material |= Material::Reflective(1);

    MeshInstance* object = scene.createMesh();
    object->transform.setPosition(0, 0, 0);
    object->transform.setScale(40, 40, 40);
    object->material |= Material::Diffuse(0.9, 1, 1, 1);
    object->material |= Material::Reflective(0.1);

    object->mesh.load_obj_file("res/bunny_1k.obj");
    scene.render();
    scene.camera.framebuffer.save("out/bunny_1k.png");

    object->mesh.load_obj_file("res/bunny_2k.obj");
    scene.render();
    scene.camera.framebuffer.save("out/bunny_2k.png");

    object->mesh.load_obj_file("res/bunny_16k.obj");
    scene.render();
    scene.camera.framebuffer.save("out/bunny_16k.png");
    
    object->mesh.load_obj_file("res/bunny_69k.obj");
    scene.render();
    scene.camera.framebuffer.save("out/bunny_69k.png");
}