#include <string>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Scene.h"
#include "Ray.h"
#include "Framebuffer.h"
#include "Render.h"
#include "Camera.h"
#include "Util.h"

#include <iostream>
const int width = 1024;
const int height = 1024;


//TODO : rework that file to fit the new version of RT


int main()
{
    Framebuffer framebuffer = Framebuffer(width, height);

    Camera camera;
    camera.position = glm::vec3{512, 512, -1024};
    camera.orientation = glm::eulerAngleXYX(0.f, 0.f, 0.f);
    camera.fov = 60.f;

    std::cout << glm::to_string(camera.orientation) << std::endl;

    Scene scene;

    //Setup Lights
    //*
    scene.createPointLight(glm::vec3{0, -500, -300}, 1e6, glm::vec3{0.1f, 0.8f, 0.1f});
    scene.createPointLight(glm::vec3{1000, -500, -300}, 1e6, glm::vec3{0.9f, 0.1f, 0.1f});
    scene.createPointLight(glm::vec3{1000, 300, 300}, 1e6, glm::vec3{0.1f, 0.1f, 0.9f});
    //*/

    //Setup Spheres (Colored)
    //*
    scene.createSphere(glm::vec3{280, 280, 200}, 110, glm::vec3{0.1f, 0.1f, 0.5f});
    scene.createSphere(glm::vec3{280, 280, 200}, 110, glm::vec3{0.1f, 0.1f, 0.5f});
    scene.createSphere(glm::vec3{350, 190, 100}, 50, glm::vec3{0.2f, 0.2f, 0.5f});
    scene.createSphere(glm::vec3{450, 450, 400}, 250, glm::vec3{0.3f, 0.3f, 0.5f});
    scene.createSphere(glm::vec3{410, 215, 180}, 20, glm::vec3{0.4f, 0.4f, 0.5f});
    scene.createSphere(glm::vec3{70, 470, 130}, 100, glm::vec3{0.5f, 0.f, 0.f});
    //*/

    render(scene, framebuffer, camera);
    framebuffer.save("SandboxRender.png");

    for (int i = 0; i < 100; i++)
    {
        glm::quat rotation = glm::eulerAngleY(deg2rad(i));
        camera.orientation = -rotation;
        camera.position = glm::rotate(rotation, glm::vec3{512, 512, -1024});
        render(scene, framebuffer, camera);

        char num[3];
        sprintf(num, "%2d", i);
        framebuffer.save("SandboxRender_" + std::string(num) + ".png");
    }
}