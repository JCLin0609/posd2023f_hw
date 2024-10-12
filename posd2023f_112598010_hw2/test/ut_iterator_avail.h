#include "../src/folder.h"
#include "../src/iterator.h"

TEST(IteratorSuite, IteratorAvailiable)
{
    Folder *folder = new Folder("./folder");
    File *file = new File("./folder/file1.txt");

    auto it = folder->createIterator();
    folder->add(file);
    ASSERT_ANY_THROW(it->first());
    ASSERT_ANY_THROW(it->next());
}