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
    test_aabb_intersection();
}

void test_vec2()
{
    vec2 a = vec2();
    ASSERT_EQ_F(a.x, 0);
    ASSERT_EQ_F(a.y, 0);
    ASSERT_EQ_F(a.length(), 0);
    ASSERT_EQ_F(a.length2(), 0);
    ASSERT_EQ_F(vec2::dot(a, a), 0);
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
    ASSERT_EQ_F(vec2::dot(a, b), 0);
    ASSERT_EQ_F(vec2::distance2(a, b), 8);
    ASSERT_EQ_F(vec2::distance(a, b), std::sqrt(8));
    ASSERT_EQ_F(vec2::cross(a, b), 4);
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

void test_aabb_intersection()
{
    AABB aabb; //unit AABB
    aabb.min = vec3(-1, -1, -1);
    aabb.max = vec3(+1, +1, +1);

    //ZAxis test
    {
        ASSERT_TRUE(intersectAABB(vec3(0, 0, -10), vec3::FRONT, aabb));
        ASSERT_TRUE(intersectAABB(vec3(-1, -1, -10), vec3::FRONT, aabb));
        ASSERT_FALSE(intersectAABB(vec3(0, 0, -10), vec3::BACK, aabb));


        //AABB contains ray
        ASSERT_TRUE(intersectAABB(vec3(0, 0, 0), vec3::FRONT, aabb));
        ASSERT_TRUE(intersectAABB(vec3(0, 0, 0), vec3::BACK, aabb));

        //AABB is behind
        ASSERT_FALSE(intersectAABB(vec3(0, 0, 10), vec3::FRONT, aabb));
        ASSERT_TRUE(intersectAABB(vec3(0, 0, 10), vec3::BACK, aabb));
    }
}