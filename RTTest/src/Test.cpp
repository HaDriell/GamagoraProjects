#include <assert.h>
#include <iostream>
#include <vector>
#include <glm/gtx/string_cast.hpp>

#include "Mesh.h"
#include "Ray.h"
#include "models/Sphere.h"
#include "Intersect.h"

const float E = 0.00001f;

bool about_equal(float value, float target, float epsilon = E)
{
    return target - epsilon <= value && value <= target + epsilon;
}

//Testing obj loading
void should_load_obj()
{
    Mesh mesh;
    mesh.load_obj("cube.obj");
    assert(mesh.positions.size() == 8);
    assert(mesh.indices.size() % 3 == 0);
    assert(mesh.indices.size() == 3 * 2 * 6); // (6 * 2 Triangles)

    mesh.load_obj("Love.obj");
    assert(mesh.indices.size() % 3 == 0);
}

void should_load_off()
{
    Mesh mesh;
    mesh.load_off("cube.off");
    std::cout << mesh.positions.size() << std::endl;
    assert(mesh.positions.size() == 8);
    assert(mesh.indices.size() % 3 == 0);
    assert(mesh.indices.size() == 3 * 2 * 6); // (6 * 2 Triangles)
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
    assert(about_equal(t, 9));
    assert(about_equal(s.position.z, 10));
}

void should_intersect_triangle()
{
    Ray r;
    r.position = glm::vec3();
    r.direction = glm::vec3{0, 0, 1};

    Triangle triangle;
    triangle.v0 = glm::vec3{-1, -1, 1};
    triangle.v1 = glm::vec3{+1, -1, 1};
    triangle.v2 = glm::vec3{ 0,  1, 1};

    float t;

    assert(intersect(r, triangle, t));
    assert(about_equal(t, 1));
}


void should_not_intersect_triangle_left()
{
    Ray r;
    r.position = glm::vec3(-1, 0, 0);
    r.direction = glm::vec3{0, 0, 1};

    Triangle triangle;
    triangle.v0 = glm::vec3{-1, -1, 1};
    triangle.v1 = glm::vec3{+1, -1, 1};
    triangle.v2 = glm::vec3{ 0,  1, 1};

    float t;

    assert(!intersect(r, triangle, t));
}

void should_not_intersect_triangle_right()
{
    Ray r;
    r.position = glm::vec3(+1, 0, 0);
    r.direction = glm::vec3{0, 0, 1};

    Triangle triangle;
    triangle.v0 = glm::vec3{-1, -1, 1};
    triangle.v1 = glm::vec3{+1, -1, 1};
    triangle.v2 = glm::vec3{ 0,  1, 1};

    float t;

    assert(!intersect(r, triangle, t));
}

int main()
{
    //Sphere tests
    should_intersect_sphere();
    should_intersect_sphere_from_inside();
    should_not_intersect_sphere();
    should_intersect_nearest_sphere();

    //Triangle tests
    should_intersect_triangle();
    should_not_intersect_triangle_left();
    should_not_intersect_triangle_right();

    //OBJ file test
    should_load_obj();
    should_load_off();
}