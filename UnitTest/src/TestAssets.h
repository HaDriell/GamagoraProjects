#include "TestUtil.h"

#include "Assets.h"


struct Resource {};


void test_assets_api()
{
    //Assert nullptr is returned when no resource is found
    ASSERT_EQ(Assets<Resource>::Get()->find("unknown"), nullptr);

    //Loader function simulation
    {
        std::shared_ptr<Resource> resource = std::make_shared<Resource>();
        Assets<Resource>::Get()->add("a", resource);
        //Assert adding stores something
        ASSERT_NEQ(Assets<Resource>::Get()->find("a"), nullptr);
    }
    //Assert that the lifetime is extended using the store
    ASSERT_NEQ(Assets<Resource>::Get()->find("a"), nullptr);
    Assets<Resource>::Get()->remove("a");
    //Assert that remove makes the resource (at least) unreachable
    ASSERT_EQ(Assets<Resource>::Get()->find("a"), nullptr);
}