#include "Scene.h"
#include <sstream>

MeshInstance* createBlock(Scene& scene, float x, float y, float z, float gridSize)
{
    MeshInstance* block = scene.createOBJMesh("cube.obj");
    block->transform.set_scaling(gridSize, gridSize, gridSize);
    block->transform.set_translation(x * gridSize, y * gridSize, z * gridSize);
    block->material = Material::Diffuse(1, 1, 1, 1);
    return block;
}

int main()
{
    Scene scene;
    scene.bias = 1;
    scene.pixel_sampling = 1;
    scene.light_sampling = 10;
    scene.indirect_illumination_sampling = 5;
    scene.ray_max_bounce = 2;
    scene.camera.fov = 60;

    //Camera positionning
    scene.camera.position = vec3(-40, -20, -130);
    scene.camera.rotation = vec3(10, -15, 0);
    // scene.camera.rotation = vec3(0, -10, 0);

    // PointLight* light = scene.createLight(vec3(0, 0, -100), 1e4, vec3(1, 1, 1));
    SphereInstance* light = scene.createSphere(vec3(0, 0, -300), 20);
    light->material |= Material::Emissive(1e5, 1, 1, 1);

    MeshInstance* back = createBlock(scene, 0, 0, +1, 100);
    back->material |= Material::Reflective(0);
    back->material |= Material::Diffuse(1, 1, 0, 0);
    for(int x = -1; x <= 1; x++)
    {
        for(int y = -1; y <= 1; y++)
        {
            if (x == 0 && y == 0)
                continue;
            createBlock(scene, x, y, 0, 100);
        }
    }

    MeshInstance* coeur = scene.createOBJMesh("Love.obj");
    coeur->transform.set_translation(0, 0, 0);
    coeur->transform.set_scaling(15, 15, 15);
    coeur->material |= Material::Diffuse(1, 1, 1, 1);

    scene.render();
    scene.camera.framebuffer.save("RenderClassic.png");

    int frames = 10;
    for (int i = 0; i < frames; i++)
    {
        float r = lerp(0, 1, i / (float) frames);
        back->material |= Material::Reflective(r);
        back->material |= Material::Diffuse(1 - r, 1, 0, 0);
        coeur->transform.rotate(0, 36, 0);

        scene.render();
        std::stringstream filename;
        filename << "Frame" << i << ".png";
        scene.camera.framebuffer.save(filename.str());
    }
}