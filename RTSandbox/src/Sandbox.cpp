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
const int width = 300;
const int height = 300;
const int frameCount = 30;


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
    scene.createPointLight(glm::vec3{300, -600, 300}, 1e6, glm::vec3{1.0f, 1.0f, 1.0f});
    scene.createPointLight(glm::vec3{300, 700, 300}, 1e6, glm::vec3{1.0f, 1.0f, 1.0f});
    //*/

    //Setup Spheres (Colored)
    //*
    scene.createSphere(glm::vec3{280, 280, 200}, 110, glm::vec3{0.5f, 0.5f, 1.0f});
    scene.createSphere(glm::vec3{280, 280, 200}, 110, glm::vec3{0.3f, 0.3f, 0.8f});
    scene.createSphere(glm::vec3{350, 190, 100}, 50, glm::vec3{0.5f, 0.5f, 0.9f});
    scene.createSphere(glm::vec3{450, 450, 400}, 250, glm::vec3{0.4f, 0.3f, 0.7f});
    scene.createSphere(glm::vec3{410, 215, 180}, 20, glm::vec3{0.4f, 0.4f, 0.5f});
    scene.createSphere(glm::vec3{70, 470, 130}, 100, glm::vec3{0.5f, 0.f, 0.f});
    //*/

    //Some background setup
    float bg_dist = 2000;
    glm::vec3 bg[8];
    bg[0] = glm::vec3{-bg_dist, -bg_dist, -bg_dist};
    bg[1] = glm::vec3{+bg_dist, -bg_dist, -bg_dist};
    bg[2] = glm::vec3{-bg_dist, +bg_dist, -bg_dist};
    bg[3] = glm::vec3{+bg_dist, +bg_dist, -bg_dist};
    bg[4] = glm::vec3{-bg_dist, -bg_dist, +bg_dist};
    bg[5] = glm::vec3{+bg_dist, -bg_dist, +bg_dist};
    bg[6] = glm::vec3{-bg_dist, +bg_dist, +bg_dist};
    bg[7] = glm::vec3{+bg_dist, +bg_dist, +bg_dist};

    //TODO : investigate & test Ray x Triangle cases. Something is wrong

    scene.createTriangle(bg[0], bg[1], bg[2], glm::vec3{0.6f, 0.6f, 0.9f});

    render(scene, framebuffer, camera);
    framebuffer.save("SandboxRender.png");


    //Rendering GIF
    glm::vec3 pointOfInterest = scene.spheres[1]->position;
    glm::quat rotation_unit = glm::eulerAngleY(deg2rad(360.f / frameCount));

    std::cout << "Rendering " << frameCount << " Frames." << std::endl;
    for (int i = 0; i < frameCount; i++)
    {
        std::cout << "Frame " << i << "/" << frameCount << " ";
        framebuffer.clear();
        //Always look at the point of interest
        camera.position = glm::rotate(rotation_unit, camera.position);
        //Extra Weird. probably me not watching the right direction (I'm watching behind me lol)
        camera.orientation = glm::quatLookAt(glm::normalize(camera.position - pointOfInterest), glm::vec3{0, 1, 0});

        render(scene, framebuffer, camera);
        framebuffer.save("Frame_" + to_string(i) + ".png");

        std::cout << "OK" << std::endl;
    }
    std::cout << "Render done. Saving" << std::endl;
}