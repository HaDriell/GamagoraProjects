#include <assert.h>
#include <iostream>
#include <vector>

#include "math/math.h"

#include "Mesh.h"
#include "models/Sphere.h"

const float E = 0.00001f;

bool about_equal(float value, float target, float epsilon = E)
{
    return target - epsilon <= value && value <= target + epsilon;
}

bool about_equal(const vec3& a, const vec3& b, float espilon = E)
{
    return about_equal(a.x, b.x, espilon) 
        && about_equal(a.y, b.y, espilon)
        && about_equal(a.z, b.z, espilon);
}

//TODO : test other vec and even complete this test
void should_test_vec2()
{
    vec2 a = vec2();
    assert(a.x == 0);
    assert(a.y == 0);
    assert(a.length() == 0);
    assert(a.length2() == 0);
    assert(a.dot(a) == 0);
    assert(a.normalise() == a); // quirk

    vec2 b = vec2(2);
    assert(b.x == 2);
    assert(b.y == 2);
    
    vec2 c = vec2(0, 1);
    assert(c.x == 0);
    assert(c.y == 1);
    assert(b.length2() == 8);
    assert(c.length() == 1);

    a = vec2(2, 0);
    b = vec2(0, 2);
    assert(a.dot(b) == 0);
    assert(a.distance2(b) == 8);
    assert(about_equal(a.distance(b), std::sqrt(8)));
    assert(a.cross(b) == 4);
}

void should_test_vec3()
{
    vec3 a = vec3(1, 0, 0);
    vec3 b = vec3(0, 1, 0);

    assert(about_equal(a + b, vec3(1, 1, 0)));
    assert(about_equal(a - b, vec3(1, -1, 0)));
    assert(about_equal(a * b, vec3(0, 0, 0)));

    assert(about_equal(a + 10, vec3(11, 10, 10)));
    assert(about_equal(a - 10, vec3(-9, -10, -10)));
    assert(about_equal(a * 10, vec3(10, 0, 0)));
    assert(about_equal(a / 10, vec3(0.1f, 0, 0)));
}

void should_test_transform()
{
    transform t;
    vec3 a, b, c;

    //apply & inverse test
    t = transform();
    t.translate(45, -410, 50);
    t.rotate(-90, 40, 130);
    t.scale(1, 3, 0.3);
    a = vec3(-10, 0.5f, 49);
    b = t.multiply(a);
    c = t.multiplyInverse(b);
    assert(about_equal(a, c));

    //scaling test
    t = transform();
    t.scale(10, -10, 10);
    a = vec3(1, 1, 1);
    b = t.multiply(a);
    c = vec3(10, -10, 10);
    assert(about_equal(b, c));

    //rotation Z test
    t = transform();
    t.rotate(0, 0, -90);
    a = vec3(1, 0, 0);
    b = t.multiply(a);
    c = vec3(0, 1, 0);
    assert(about_equal(b, c));
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
    assert(mesh.positions.size() == 8);
    assert(mesh.indices.size() % 3 == 0);
    assert(mesh.indices.size() == 3 * 2 * 6); // (6 * 2 Triangles)
}

// Testing Raycasting
void should_intersect_sphere()
{
    // Ray r;
    // r.position = glm::vec3();
    // r.direction = glm::vec3{0, 0, 1};

    // Sphere s;
    // s.position = glm::vec3{0, 0, 10};
    // s.radius = 1.f;

    // float t;

    // assert(intersect(r, s, t));
    // assert(about_equal(t, 9));
}

// Testing Raycasting
void should_intersect_sphere_from_inside()
{
    // Ray r;
    // r.position = glm::vec3();
    // r.direction = glm::vec3{0, 0, 1};

    // Sphere s;
    // s.position = glm::vec3{0, 0, 0};
    // s.radius = 1.f;

    // float t;

    // assert(intersect(r, s, t));
    // assert(about_equal(t, 1));
}

// Testing Raycasting
void should_not_intersect_sphere()
{
    // Ray r;
    // r.position = glm::vec3();
    // r.direction = glm::vec3{0, 0, 1};

    // Sphere s;
    // s.position = glm::vec3{0, 0, -10};
    // s.radius = 1.f;

    // float t;

    // assert(!intersect(r, s, t));
}

// Testing Raycasting
void should_intersect_nearest_sphere()
{
    // Ray r;
    // r.position = glm::vec3();
    // r.direction = glm::vec3{0, 0, 1};

    // std::vector<Sphere*> spheres;
    // spheres.push_back(new Sphere{glm::vec3{0.f, 0.f,-20.f}, 1, glm::vec3()}); // behind
    // spheres.push_back(new Sphere{glm::vec3{0.f, 0.f, 30.f}, 1, glm::vec3()});
    // spheres.push_back(new Sphere{glm::vec3{0.f, 0.f, 10.f}, 1, glm::vec3()}); // nearest
    // spheres.push_back(new Sphere{glm::vec3{0.f, 0.f, 20.f}, 1, glm::vec3()});

    // float t;
    // Sphere s;

    // assert(intersectNearest(r, spheres, t, s));
    // assert(about_equal(t, 9));
    // assert(about_equal(s.position.z, 10));
}

void should_intersect_triangle()
{
    // Ray r;
    // r.position = glm::vec3();
    // r.direction = glm::vec3{0, 0, 1};

    // Triangle triangle;
    // triangle.v0 = glm::vec3{-1, -1, 1};
    // triangle.v1 = glm::vec3{+1, -1, 1};
    // triangle.v2 = glm::vec3{ 0,  1, 1};

    // float t;

    // assert(intersect(r, triangle, t));
    // assert(about_equal(t, 1));
}


void should_not_intersect_triangle_left()
{
    // Ray r;
    // r.position = glm::vec3(-1, 0, 0);
    // r.direction = glm::vec3{0, 0, 1};

    // Triangle triangle;
    // triangle.v0 = glm::vec3{-1, -1, 1};
    // triangle.v1 = glm::vec3{+1, -1, 1};
    // triangle.v2 = glm::vec3{ 0,  1, 1};

    // float t;

    // assert(!intersect(r, triangle, t));
}

void should_not_intersect_triangle_right()
{
    // Ray r;
    // r.position = glm::vec3(+1, 0, 0);
    // r.direction = glm::vec3{0, 0, 1};

    // Triangle triangle;
    // triangle.v0 = glm::vec3{-1, -1, 1};
    // triangle.v1 = glm::vec3{+1, -1, 1};
    // triangle.v2 = glm::vec3{ 0,  1, 1};

    // float t;

    // assert(!intersect(r, triangle, t));
}

int main()
{
    std::cout << "Begining tests" << std::endl;
    //Math tests
    should_test_vec2();
    should_test_vec3();
    should_test_transform();

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

    std::cout << "Tests passed !" << std::endl;
}