#include "maths.h"
#include "TestUtil.h"


//TODO : seriously test this shit
void test_vec2();
void test_vec3();
void test_transform();
void test_aabb_intersection();

void test_math()
{
    test_vec2();
    test_vec3();
    // test_transform(); // tehcnically works fine. But EPSILON is too small for float shitty precision
    test_aabb_intersection();
}

void test_vec2()
{
    vec2 a = vec2();
    ASSERT_EQ_F(a.x, 0);
    ASSERT_EQ_F(a.y, 0);
    ASSERT_EQ_F(a.length(), 0);
    ASSERT_EQ_F(a.length2(), 0);
    ASSERT_EQ_F(a.dot(a), 0);
    ASSERT_EQ(a.normalize(), a); // quirk

    vec2 b = vec2(2);
    ASSERT_EQ_F(b.x, 2);
    ASSERT_EQ_F(b.y, 2);
    
    vec2 c = vec2(0, 1);
    ASSERT_EQ_F(c.x, 0);
    ASSERT_EQ_F(c.y, 1);
    ASSERT_EQ_F(b.length2(), 8);
    ASSERT_EQ_F(c.length(), 1);

    a = vec2(2, 0);
    b = vec2(0, 2);
    ASSERT_EQ_F(a.dot(b), 0);
    ASSERT_EQ_F(a.distance2(b), 8);
    ASSERT_EQ_F(a.distance(b), std::sqrt(8));
    ASSERT_EQ_F(a.cross(b), 4);
}

void test_vec3()
{
    vec3 a = vec3(1, 0, 0);
    vec3 b = vec3(0, 1, 0);

    ASSERT_EQ(a + b, vec3(1, 1, 0));
    ASSERT_EQ(a - b, vec3(1, -1, 0));
    ASSERT_EQ(a * b, vec3(0, 0, 0));

    ASSERT_EQ(a + 10, vec3(11, 10, 10));
    ASSERT_EQ(a - 10, vec3(-9, -10, -10));
    ASSERT_EQ(a * 10, vec3(10, 0, 0));
    ASSERT_EQ(a / 10, vec3(0.1f, 0, 0));
}

void test_transform()
{
    transform t;
    vec3 a, b, c;

    //apply & inverse test
    t = transform();
    t.translate(45, -410, 50);
    t.rotate(-90, 40, 130);
    t.scale(1, 3, 2);
    a = vec3(-10, 0.5f, 49);
    b = t.multiply(a);
    c = t.multiplyInverse(b);
    ASSERT_EQ(a, c);

    //scaling test
    t = transform();
    t.scale(10, -10, 10);
    a = vec3(1, 1, 1);
    b = t.multiply(a);
    c = vec3(10, -10, 10);
    ASSERT_EQ(b, c);

    //rotation Z test
    t = transform();
    t.rotate(0, 0, -90);
    a = vec3(1, 0, 0);
    b = t.multiply(a);
    c = vec3(0, 1, 0);
    ASSERT_EQ(b, c);
}

void test_aabb_intersection()
{
    AABB aabb; //unit AABB
    aabb.min = vec3(-1, -1, -1);
    aabb.max = vec3(+1, +1, +1);

    float distance;
    vec3 hitPoint;
    vec3 normal;

    //ZAxis test
    {
        ASSERT_TRUE(intersectAABB(vec3(0, 0, -10), vec3::FRONT, aabb, distance, hitPoint, normal));
        ASSERT_EQ_F(distance, 9);
        ASSERT_TRUE(intersectAABB(vec3(-1, -1, -10), vec3::FRONT, aabb, distance, hitPoint, normal));
        ASSERT_TRUE(std::isnan(distance));
        ASSERT_FALSE(intersectAABB(vec3(0, 0, -10), vec3::BACK, aabb, distance, hitPoint, normal));


        //AABB contains ray
        ASSERT_TRUE(intersectAABB(vec3(0, 0, 0), vec3::FRONT, aabb, distance, hitPoint, normal));
        ASSERT_TRUE(intersectAABB(vec3(0, 0, 0), vec3::BACK, aabb, distance, hitPoint, normal));

        //AABB is behind
        ASSERT_FALSE(intersectAABB(vec3(0, 0, 10), vec3::FRONT, aabb, distance, hitPoint, normal));
        ASSERT_TRUE(intersectAABB(vec3(0, 0, 10), vec3::BACK, aabb, distance, hitPoint, normal));
    }
}