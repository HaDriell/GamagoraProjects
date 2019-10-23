#include "Scene.h"
#include <sstream>

MeshInstance* createBlock(Scene& scene, float x, float y, float z, float gridSize)
{
    MeshInstance* block = scene.createOBJMesh("cube.obj");
    block->transform.set_scaling(gridSize, gridSize, gridSize);
    block->transform.set_translation(x * gridSize, y * gridSize, z * gridSize);
    block->material = Material::Diffuse(1, 0.15, 0.15, 0.15);
    return block;
}

int main()
{
    Scene scene;
    scene.bias = 1e-2;
    scene.pixel_sampling = 100;
    scene.light_sampling = 1;
    scene.indirect_illumination_sampling = 1;
    scene.ray_max_bounce = 2;
    scene.camera.fov = 60;
    scene.camera.framebuffer.resize(256, 256);

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
    object->transform.set_translation(0, 0, 0);
    object->transform.set_scaling(40, 40, 40);
    object->material |= Material::Diffuse(0.9, 1, 1, 1);
    object->material |= Material::Reflective(0.1);

    //Small amount of triangles
    object->mesh.load_obj_file("Love.obj");
    scene.render();
    scene.camera.framebuffer.save("small.png");

    //Medium amount of triangles
    object->mesh.load_obj_file("bunny.obj");
    scene.render();
    scene.camera.framebuffer.save("medium.png");

    //Large amount of triangles
    object->mesh.load_obj_file("armadillo.obj");
    scene.render();
    scene.camera.framebuffer.save("large.png");
}