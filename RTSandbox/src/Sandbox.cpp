#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Mesh.h"
#include "Transform.h"

#include "Scene.h"
#include "Ray.h"
#include "render/Framebuffer.h"
#include "render/Render.h"
#include "render/Camera.h"
#include "Util.h"

#include <iostream>
const int width = 512;
const int height = 512;
const int frameCount = 60;
const int bl_sample_count = 100;

void render_love()
{
    Mesh bunny;
    bunny.load_off("bunny.off");
    Transform bunnyTransform;
    bunnyTransform.scale = {3000.f, 3000.f, 3000.f };

    Mesh love;
    love.load_obj("Love.obj");
    Transform loveTransform;
    loveTransform.scale = {4.f, 4.f, 4.f};

    Mesh cube;
    cube.load_obj("cube.obj");
    Transform cubeTransform;
    cubeTransform.translation = {0.f, -200.f, 0.f};
    cubeTransform.scale = {100.f, 100.f, 100.f};

    Scene scene;
    scene.bl_sample_count = bl_sample_count;
    
    //Objects
    //Floor
    scene.createSphere(glm::vec3{0, -1e5, 0}, 1e5 - 300, glm::vec3{0.5f, 0.5f, 0.5f});

    //Meshes
    scene.createInstance(bunny, bunnyTransform);
    // scene.createInstance(love, loveTransform);
    // scene.createInstance(cube, cubeTransform);

    //Light
    // scene.createCubeLight(glm::vec3{0, 2000, 0}, 200, 2e6, glm::vec3{1.f, 1.f, 1.f});
    scene.createPointLight(glm::vec3{0, 2000, 0}, 2e6, glm::vec3{1.f, 1.f, 1.f});

    Camera camera;
    camera.position = glm::vec3{0, 350, -500};
    camera.fov = 60.f;

    Framebuffer framebuffer = Framebuffer(width, height);
    glm::quat orbit_rotation = glm::eulerAngleY(360.f / frameCount);
    for (int i = 0; i < frameCount; i++)
    {
        std::cout << "Frame " << i << "/" << frameCount;

        //rotate around orbit
        camera.position = orbit_rotation * camera.position;
        glm::vec3 direction = glm::normalize( glm::vec3{0, 100, 0}  - camera.position );
        camera.rotation = glm::eulerAngles(glm::quatLookAt(direction, glm::vec3{0, 1, 0}));

        //Render to the framebuffer
        framebuffer.clear();
        render(scene, framebuffer, camera);
        framebuffer.save("Frame_" + to_string(i) + ".png");
        std::cout << "OK" << std::endl;
    }
}

int main()
{
    render_love();
    std::cin.get(); // prevent auto exit
    return 0;
}