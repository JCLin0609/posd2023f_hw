#include "../src/file.h"

TEST(dfs_iteratorSuite, dfsIterator)
{
    Folder *folder1 = new Folder("/usr/path1");
    Folder *folder2 = new Folder("/usr/path1/path2");
    File *file1 = new File("/usr/file1");
    File *file2 = new File("/usr/path1/file2");
    File *file4 = new File("/usr/path1/path2/file4");
    Folder *usrFolder = new Folder("/usr");
    usrFolder->add(folder1), usrFolder->add(file1);
    folder1->add(folder2), folder1->add(file2);
    folder2->add(file4);

    Iterator *it = usrFolder->createDfsIterator();

    it->first();
    ASSERT_EQ("/usr/path1", it->currentItem()->path());
    it->next();
    ASSERT_EQ("/usr/path1/path2", it->currentItem()->path());
    it->next();
    ASSERT_EQ("/usr/path1/path2/file4", it->currentItem()->path());
    it->next();
    ASSERT_EQ("/usr/path1/file2", it->currentItem()->path());
    it->next();
    ASSERT_EQ("/usr/file1", it->currentItem()->path());
    it->next();
    ASSERT_TRUE(it->isDone());
}

TEST(dfs_iteratorSuite, bfsIterator)
{
    Folder *folder1 = new Folder("/usr/path1");
    Folder *folder2 = new Folder("/usr/path1/path2");
    File *file1 = new File("/usr/file1");
    File *file2 = new File("/usr/path1/file2");
    File *file4 = new File("/usr/path1/path2/file4");
    Folder *usrFolder = new Folder("/usr");
    usrFolder->add(folder1), usrFolder->add(file1);
    folder1->add(folder2), folder1->add(file2);
    folder2->add(file4);

    Iterator *it = usrFolder->createBfsIterator();

    it->first();
    ASSERT_EQ("/usr/path1", it->currentItem()->path());
    it->next();
    ASSERT_EQ("/usr/file1", it->currentItem()->path());
    it->next();
    ASSERT_EQ("/usr/path1/path2", it->currentItem()->path());
    it->next();
    ASSERT_EQ("/usr/path1/file2", it->currentItem()->path());
    it->next();
    ASSERT_EQ("/usr/path1/path2/file4", it->currentItem()->path());
    it->next();
    ASSERT_TRUE(it->isDone());
}