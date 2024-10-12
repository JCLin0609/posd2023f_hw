#include "../src/node.h"
#include "../src/folder.h"
#include "../src/iterator.h"
#include <string>

TEST(FolderSuite, FolderCreate)
{
    string path = ("/usr/books");
    Node *newFolder = new Folder(path);
    ASSERT_EQ("/usr/books", newFolder->path());
    ASSERT_EQ("books", newFolder->name());
    // ASSERT_EQ(0, newFolder->_ChildNodes.size());
    ASSERT_TRUE(newFolder->getIsFolder());
}

TEST(FolderSuite, FolderAddNode)
{
    string path = ("/user");
    Node *newFolder = new Folder(path);
    string path2 = ("/user/book");
    Node *newFolder2 = new Folder(path2);
    string pathDesktop = ("/Desktop");
    Node *newFolder3 = new Folder(pathDesktop);
    string path3 = ("/user/Gof.pdf");
    Node *newFile1 = new File(path3);
    string pathBook = ("/user/book/Gof.pdf");
    Node *newFile2 = new File(pathBook);

    newFolder->add(newFolder2);
    // ASSERT_EQ(1, newFolder->_ChildNodes.size());
    // newFolder->add(newFolder3);
    // ASSERT_EQ(1, newFolder->_ChildNodes.size());
    newFolder->add(newFile1);
    // ASSERT_EQ(2, newFolder->_ChildNodes.size());
    // newFolder->add(newFile2);
    // ASSERT_EQ(2, newFolder->_ChildNodes.size());
}

TEST(FolderSuite, FolderIteratorT)
{
    Folder *folder1 = new Folder("/usr/path1");
    Folder *folder2 = new Folder("/usr/path2");
    Folder *usrFolder = new Folder("/usr");
    usrFolder->add(folder1), usrFolder->add(folder2);
    Iterator *it = usrFolder->createIterator();

    it->first();
    ASSERT_FALSE(it->isDone());
    ASSERT_EQ("/usr/path1", it->currentItem()->path());
    it->next();
    ASSERT_FALSE(it->isDone());
    ASSERT_EQ("/usr/path2", it->currentItem()->path());
    it->next();
    ASSERT_TRUE(it->isDone());
}

TEST(FolderSuite, FolderGetChildByName)
{
    string path = ("/user");
    Node *newFolder = new Folder(path);
    string path2 = ("/user/book");
    Node *newFolder2 = new Folder(path2);
    newFolder->add(newFolder2);

    Node *retNode;
    retNode = newFolder->getChildByName("book");
    ASSERT_EQ("/user/book", retNode->path());
    retNode = newFolder->getChildByName("cake");
    ASSERT_EQ(nullptr, retNode);
}

TEST(FolderSuite, FolderFindFolder)
{
    string path = ("/user");
    Node *newFolder = new Folder(path);
    string path2 = ("/user/book");
    Node *newFolder2 = new Folder(path2);
    string path3 = ("/user/book/gof");
    Node *newFolder3 = new Folder(path3);
    newFolder->add(newFolder2);
    newFolder2->add(newFolder3);

    Node *retNode = newFolder->find("/user/book/gof");
    ASSERT_EQ("/user/book/gof", retNode->path());
    Node *retNode2 = newFolder->find("/Dekstop/book/gof");
    ASSERT_EQ(nullptr, retNode2);
}

TEST(FolderSuite, FolderFindFile)
{
    string path = ("/user");
    Node *newFolder = new Folder(path);
    string path2 = ("/user/book");
    Node *newFolder2 = new Folder(path2);
    string path3 = ("/user/book/gof.pdf");
    Node *newFile1 = new File(path3);
    newFolder->add(newFolder2);
    newFolder2->add(newFile1);

    Node *retNode = newFolder->find("/user/book/gof.pdf");
    ASSERT_EQ("/user/book/gof.pdf", retNode->path());
    Node *retNode2 = newFolder->find("/Desktop/book/gof.pdf");
    ASSERT_EQ(nullptr, retNode2);
}

TEST(FolderSuite, FolderNumberOfFiles)
{
    Folder *folder1 = new Folder("/usr/path1");
    Folder *folder2 = new Folder("/usr/path1/path2");
    File *file1 = new File("/usr/file1");
    File *file2 = new File("/usr/path1/file2");
    File *file3 = new File("/usr/path1/file3");
    File *file4 = new File("/usr/path1/path2/file4");
    Folder *usrFolder = new Folder("/usr");
    usrFolder->add(folder1), usrFolder->add(file1);
    folder1->add(folder2), folder1->add(file2), folder1->add(file3);
    folder2->add(file4);

    ASSERT_EQ(4, usrFolder->numberOfFiles());
}

TEST(FolderSuite, FolderRemoveFile)
{
    Folder *folder = new Folder("/");
    File *file1 = new File("/file1");
    File *file2 = new File("/file2");
    folder->add(file1);
    folder->add(file2);

    // std::cout << "file1 0: " << file1 << std::endl;
    // std::cout << "folder->_ChildNodes[0] 0: " << folder->_ChildNodes[0] << std::endl;
    folder->remove("/file1");
    // std::cout << "folder->_ChildNodes[0] 1: " << folder->_ChildNodes[0] << std::endl;
    // ASSERT_EQ("", file1->path());
    ASSERT_EQ(nullptr, folder->find("/file1"));
    ASSERT_EQ(1, folder->numberOfFiles());
    folder->remove("/file2");
    ASSERT_EQ(0, folder->numberOfFiles());
}

TEST(FolderSuite, FolderRemoveFolder)
{
    Folder *folder1 = new Folder("/usr/path1");
    Folder *folder2 = new Folder("/usr/path1/path2");
    File *file1 = new File("/usr/file1");
    File *file2 = new File("/usr/path1/file2");
    File *file3 = new File("/usr/path1/path2/file3");

    Folder *usrFolder = new Folder("/usr");
    usrFolder->add(folder1), usrFolder->add(file1);
    folder1->add(folder2), folder1->add(file2);
    folder2->add(file3);

    folder1->remove("/usr/path1/path2");
    ASSERT_EQ(2, usrFolder->numberOfFiles());
}
