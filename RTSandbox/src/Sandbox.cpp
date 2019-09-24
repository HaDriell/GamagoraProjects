#include <string>

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
const int width = 500;
const int height = 500;
const int frameCount = 60;


//TODO : rework that file to fit the new version of RT


int main()
{
    Framebuffer framebuffer = Framebuffer(width, height);

    Scene scene;
    scene.vl_sample_count = 100;

    //Debug Light
    scene.createPointLight(glm::vec3{0, 900, 0}, 1e5, glm::vec3{1.f, 1.f, 1.f});
    scene.createCubeLight(glm::vec3{0, -200, -400}, 100, 8e4, glm::vec3{1.f, 1.f, 1.f});

    //Small Sphere at origin
    scene.createSphere(glm::vec3{0, 0, 0}       , 10, glm::vec3{1.f, 0.6f, 0.6f});
    scene.createSphere(glm::vec3{-100, 100, 0} , 60, glm::vec3{0.3f, 1.0f, 0.9f});
    scene.createSphere(glm::vec3{+100, 0, 100}  , 60, glm::vec3{0.8f, 0.8f, 1.0f});

    //Floor
    scene.createSphere(glm::vec3{0, -1e5, 0}, 1e5 - 300, glm::vec3{0.5f, 0.5f, 0.5f});

    Camera camera;
    camera.position = glm::vec3{0, 0, -500};
    camera.orientation = glm::quatLookAt(glm::normalize(camera.position - glm::vec3{0.f, 0.f, 0.f}), glm::vec3{0, 1, 0});
    camera.fov = 60.f;

    std::cout << glm::to_string(camera.orientation) << std::endl;

    render(scene, framebuffer, camera);
    framebuffer.save("SandboxRender.png");


    //Rendering GIF
    glm::vec3 pointOfInterest = {0.f, 0.f, 0.f};
    glm::quat rotation_unit = glm::eulerAngleY(deg2rad(360.f / frameCount));

    std::cout << "Rendering " << frameCount << " Frames." << std::endl;
    for (int i = 0; i < frameCount; i++)
    {
        std::cout << "Frame " << i << "/" << frameCount << " ";
        Framebuffer fb = Framebuffer(width, height);

        //Always look at the point of interest
        camera.position = glm::rotate(rotation_unit, camera.position);
        //Extra Weird. probably me not watching the right direction (I'm watching behind me lol)
        camera.orientation = glm::quatLookAt(glm::normalize(camera.position - pointOfInterest), glm::vec3{0, 1, 0});

        render(scene, fb, camera);
        fb.save("Frame_" + to_string(i) + ".png");

        std::cout << "OK" << std::endl;
    }
    std::cout << "Render done. Saving" << std::endl;
}