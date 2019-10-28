#include "TestUtil.h"

#include "AssetManager.h"

struct Foo {
    Foo()  { std::cout << "Foo Created" << std::endl; }
    ~Foo() { std::cout << "Foo Deleted" << std::endl; }
};

template<>
std::shared_ptr<Foo> AssetManager<Foo>::LoadAsset(const std::string& path)
{
    if (VFS::Exists(path))
    {
        return std::make_shared<Foo>();
    }

    return std::shared_ptr<Foo>(nullptr);
}

void test_asset_manager()
{
    AssetManager<Foo> manager;
    
    //Asset that is loaded successfully
    ASSERT_TRUE(manager.load("vfs/File.txt", "foo"));
    ASSERT_NEQ(manager.get("foo"), nullptr);
    manager.unload("foo");
    ASSERT_EQ(manager.get("foo"), nullptr);

    //Asset that fails to load
    ASSERT_FALSE(manager.load("/unknown/path", "bar"));
    ASSERT_EQ(manager.get("bar"), nullptr);
}