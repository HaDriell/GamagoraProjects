#include "math.h"
#include "TestUtil.h"


//TODO : seriously test this shit
void test_vec2();

void test_math()
{
    test_vec2();
}

void test_vec2()
{
    vec2 a = vec2();
    ASSERT_EQ_F(a.x, 0);
    ASSERT_EQ_F(a.y, 0);
    ASSERT_EQ_F(a.length(), 0);
    ASSERT_EQ_F(a.length2(), 0);
    ASSERT_EQ_F(a.dot(a), 0);
    ASSERT_EQ(a.normalise(), a); // quirk

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