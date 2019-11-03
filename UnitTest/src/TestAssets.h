#include "TestUtil.h"

#include "Assets.h"


struct Resource {};


void test_assets_api()
{
    //Assert nullptr is returned when no resource is found
    ASSERT_EQ(Assets<Resource>::Find("unknown"), nullptr);

    //Loader function simulation
    {
        std::shared_ptr<Resource> resource = std::make_shared<Resource>();
        Assets<Resource>::Add("a", resource);
        //Assert adding stores something
        ASSERT_NEQ(Assets<Resource>::Find("a"), nullptr);
    }
    //Assert that the lifetime is extended using the store
    ASSERT_NEQ(Assets<Resource>::Find("a"), nullptr);
    Assets<Resource>::Remove("a");
    //Assert that remove makes the resource (at least) unreachable
    ASSERT_EQ(Assets<Resource>::Find("a"), nullptr);
}