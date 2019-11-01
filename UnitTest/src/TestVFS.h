#include "TestUtil.h"

#include <string>

#include "VFS.h"


void test_vfs()
{
    std::cout << "Testing VFS" << std::endl;

    std::string physicalPath;
    ASSERT_TRUE(VFS::Exists("vfs/File.txt"));
    ASSERT_TRUE(VFS::Exists("vfs/system/File.txt"));
    //Test no virtual resolution
    ASSERT_TRUE(VFS::Resolve("vfs/File.txt", physicalPath));
    ASSERT_EQ(physicalPath, "vfs/File.txt");
    ASSERT_FALSE(VFS::Resolve("/system/File.txt", physicalPath));
    VFS::Mount("/system", "vfs/system");
    ASSERT_TRUE(VFS::Resolve("/system/File.txt", physicalPath));
    ASSERT_EQ(physicalPath, "vfs/system/File.txt");
    VFS::Unmount("/system");
    ASSERT_FALSE(VFS::Resolve("/system/File.txt", physicalPath));
}