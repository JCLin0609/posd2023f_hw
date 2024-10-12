#include "../src/file.h"
#include "../src/folder.h"
#include "../src/visitor.h"
#include "../src/find_by_name_visitor.h"
#include "../src/stream_out_visitor.h"

TEST(VisitorSuite, FileFindByNameVisitor)
{
    File *file = new File("./folder/file1.txt");
    FindByNameVisitor visitor("file1.txt");
    FindByNameVisitor visitor2("filexxx.txt");

    file->accept(&visitor);
    file->accept(&visitor2);

    ASSERT_EQ(1, visitor.getPaths().size());
    ASSERT_EQ(0, visitor2.getPaths().size());
}

TEST(VisitorSuite, FolderFindByNameVisitor)
{
    Folder *folder = new Folder("./folder/folder1");
    FindByNameVisitor visitor("folder1");
    FindByNameVisitor visitor2("folderxxx");

    folder->accept(&visitor);
    folder->accept(&visitor2);

    ASSERT_EQ(1, visitor.getPaths().size());
    ASSERT_EQ(0, visitor2.getPaths().size());
}

TEST(VisitorSuite, FolderFindByNameVisitorRecursively)
{
    Folder *folder = new Folder("./folder");
    Folder *folder1 = new Folder("./folder/folder1");
    Folder *folder2 = new Folder("./folder/folder1/folder2");
    folder->add(folder1), folder1->add(folder2);

    FindByNameVisitor visitor("folder2");

    folder->accept(&visitor);

    ASSERT_EQ(1, visitor.getPaths().size());
}

TEST(VisitorSuite, FileStreamOutVisitor)
{
    File *file = new File("./folder/file1.txt");
    StreamOutVisitor visitor;

    file->accept(&visitor);

    string ans;
    ans.append("_____________________________________________\n");
    ans.append("./folder/file1.txt\n");
    ans.append("---------------------------------------------\n");
    ans.append("hello, world\n");
    ans.append("_____________________________________________\n");

    ASSERT_EQ(ans, visitor.getResult());
}

TEST(VisitorSuite, FolderStreamOutVisitor)
{
    Folder *folder = new Folder("./folder");
    Folder *folder1 = new Folder("./folder/folder1");
    File *file = new File("./folder/file1.txt");
    File *file2 = new File("./folder/folder1/file2.txt");
    folder->add(file), folder->add(folder1);
    folder1->add(file2);

    StreamOutVisitor visitor;

    folder->accept(&visitor);

    string ans;
    ans.append("_____________________________________________\n");
    ans.append("./folder/file1.txt\n");
    ans.append("---------------------------------------------\n");
    ans.append("hello, world\n");
    ans.append("_____________________________________________\n\n");
    ans.append("_____________________________________________\n");
    ans.append("./folder/folder1/file2.txt\n");
    ans.append("---------------------------------------------\n");
    ans.append("This is file 2\n");
    ans.append("This is second line\n");
    ans.append("this is end of file\n");
    ans.append("_____________________________________________\n\n");

    ASSERT_EQ(ans, visitor.getResult());
}