#include "../src/node.h"
#include "../src/file.h"
#include "../src/null_iterator.h"
#include <string>

TEST(FileSuite, FileCreate)
{
    string path = "/home/newFile1";
    Node *newFile = new File(path);
    ASSERT_EQ("/home/newFile1", newFile->path());
    ASSERT_EQ("newFile1", newFile->name());
    // ASSERT_EQ(0, newFile->_ChildNodes.size());
    ASSERT_FALSE(newFile->getIsFolder());
}