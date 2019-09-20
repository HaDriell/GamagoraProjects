#include <assert.h>
#include <iostream>
#include <vector>
#include <glm/gtx/string_cast.hpp>

 #include "Ray.h"
 #include "models/Sphere.h"
 #include "Intersect.h"

const float E = 0.00001f;

bool about_equal(float value, float target, float epsilon = E)
{
    return target - epsilon <= value && value <= target + epsilon;
}


// Testing Raycasting
void should_intersect_sphere()
{
    Ray r;
    r.position = glm::vec3();
    r.direction = glm::vec3{0, 0, 1};

    Sphere s;
    s.position = glm::vec3{0, 0, 10};
    s.radius = 1.f;

    float t;

    assert(intersect(r, s, t));
    assert(about_equal(t, 9));
}

// Testing Raycasting
void should_intersect_sphere_from_inside()
{
    Ray r;
    r.position = glm::vec3();
    r.direction = glm::vec3{0, 0, 1};

    Sphere s;
    s.position = glm::vec3{0, 0, 0};
    s.radius = 1.f;

    float t;

    assert(intersect(r, s, t));
    assert(about_equal(t, 1));
}

// Testing Raycasting
void should_not_intersect_sphere()
{
    Ray r;
    r.position = glm::vec3();
    r.direction = glm::vec3{0, 0, 1};

    Sphere s;
    s.position = glm::vec3{0, 0, -10};
    s.radius = 1.f;

    float t;

    assert(!intersect(r, s, t));
}

// Testing Raycasting
void should_intersect_nearest_sphere()
{
    Ray r;
    r.position = glm::vec3();
    r.direction = glm::vec3{0, 0, 1};

    std::vector<Sphere*> spheres;
    spheres.push_back(new Sphere{glm::vec3{0.f, 0.f,-20.f}, 1, glm::vec3()}); // behind
    spheres.push_back(new Sphere{glm::vec3{0.f, 0.f, 30.f}, 1, glm::vec3()});
    spheres.push_back(new Sphere{glm::vec3{0.f, 0.f, 10.f}, 1, glm::vec3()}); // nearest
    spheres.push_back(new Sphere{glm::vec3{0.f, 0.f, 20.f}, 1, glm::vec3()});

    float t;
    Sphere s;

    assert(intersectNearest(r, spheres, t, s));
    std::cout << t << std::endl;
    assert(about_equal(t, 9));
    assert(about_equal(s.position.z, 10));
}

int main()
{
    should_intersect_sphere();
    should_intersect_sphere_from_inside();
    should_not_intersect_sphere();
    should_intersect_nearest_sphere();
}