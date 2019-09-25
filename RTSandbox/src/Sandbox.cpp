#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "models/Mesh.h"

#include "Scene.h"
#include "Ray.h"
#include "Framebuffer.h"
#include "Render.h"
#include "Camera.h"
#include "Util.h"

#include <iostream>
const int width = 512;
const int height = 512;
const int frameCount = 360;
const int bl_sample_count = 200;

void load_into_scene(Scene& scene, const Mesh& mesh, glm::vec3 color = glm::vec3{1.f, 1.f, 1.f}, glm::vec3 position = glm::vec3(), glm::quat orientation = glm::quat(), glm::vec3 scale = glm::vec3{1.f, 1.f, 1.f})
{
    std::cout << "Adding Mesh to Scene " << std::endl;
    for (auto& vertex : mesh.vertices)
    {
        glm::vec3 a = mesh.positions[vertex.v0 - 1]; a *= scale; a = orientation * a; a += position;
        glm::vec3 b = mesh.positions[vertex.v1 - 1]; b *= scale; b = orientation * b; b += position;
        glm::vec3 c = mesh.positions[vertex.v2 - 1]; c *= scale; c = orientation * c; c += position;
        std::cout << "Triangle " << glm::to_string(a) << " " << glm::to_string(b) << " " << glm::to_string(c) << std::endl;
        scene.createTriangle(a, b, c, color);
    }
}

void render_love()
{
    Mesh love;
    love.load_obj("Love.obj");

    Mesh cube;
    cube.load_obj("cube.obj");

    Scene scene;
    scene.bl_sample_count = bl_sample_count;
    
    //Objects
    //Floor
    scene.createSphere(glm::vec3{0, -1e5, 0}, 1e5 - 300, glm::vec3{0.5f, 0.5f, 0.5f});
    //Love
    load_into_scene(scene, 
                    love, 
        /*color*/   glm::vec3{0.95f, 0.05f, 0.05f},
        /*position*/glm::vec3(),
        /*rotation*/glm::quat(), 
        /*scale*/   glm::vec3{4.f, 4.f, 4.f});

    //Light
    scene.createCubeLight(glm::vec3{0, 2000, 0}, 200, 2e6, glm::vec3{1.f, 1.f, 1.f});

    Camera camera;
    camera.position = glm::vec3{0, 350, -500};
    camera.fov = 60.f;

    Framebuffer framebuffer = Framebuffer(width, height);
    int frameCount = 60;
    glm::quat orbit_rotation = glm::eulerAngleY(360.f / frameCount);
    for (int i = 0; i < frameCount; i++)
    {
        std::cout << "Frame " << i << "/" << frameCount << " ";

        //rotate around orbit
        camera.position = orbit_rotation * camera.position;

        //Always look at the point of interest
        camera.orientation = glm::quatLookAt(glm::normalize(camera.position), glm::vec3{0, 1, 0});

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

    Framebuffer framebuffer = Framebuffer(width, height);

    Scene scene;
    scene.bl_sample_count = bl_sample_count;

    //Debug Light
    scene.createPointLight(glm::vec3{0, 900, 0}, 1e5, glm::vec3{1.f, 1.f, 1.f});
    scene.createCubeLight(glm::vec3{0, -200, -400}, 100, 8e4, glm::vec3{1.f, 1.f, 1.f});
//    scene.createPointLight(glm::vec3{0, -200, -400}, 8e4, glm::vec3{1.f, 1.f, 1.f});

    //Small Sphere at origin
//    scene.createSphere(glm::vec3{0, 0, 0}       , 10, glm::vec3{1.f, 0.6f, 0.6f});
    scene.createSphere(glm::vec3{-100, 100, 0} , 60, glm::vec3{0.3f, 1.0f, 0.9f});
    scene.createSphere(glm::vec3{+100, 0, 100}  , 60, glm::vec3{0.8f, 0.8f, 1.0f});

    //Floor
    scene.createSphere(glm::vec3{0, -1e5, 0}, 1e5 - 300, glm::vec3{0.5f, 0.5f, 0.5f});

    Mesh mesh;
    mesh.load_obj("cube.obj");

    //Add a new Mesh instance in the scene
    load_into_scene(scene, mesh, glm::vec3(), glm::vec3{1.f, 1.f, 1.f}, glm::quat(), glm::vec3{40.f, 40.f, 40.f});

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