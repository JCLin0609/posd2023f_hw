#include "../src/file.h"
#include "../src/folder.h"

TEST(NodeOsSuite, CreateFileInOs)
{
    File *file = new File("./folder/file1.txt");
    File *file2;
    ASSERT_NE(nullptr, file);
    ASSERT_ANY_THROW(file2 = new File("~/POSD_hw2/filexxx.txt"));
}

TEST(NodeOsSuite, CreateFolderInOs)
{
    Folder *folder = new Folder("./folder/folder1");
    Folder *folder2;
    ASSERT_NE(nullptr, folder);
    ASSERT_ANY_THROW(folder2 = new Folder("~/POSD_hw2/folderxxx.txt"));
}