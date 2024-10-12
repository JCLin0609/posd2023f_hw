#include "../src/node.h"
#include "../src/folder.h"
#include "../src/file.h"
#include "../src/file_system_builder.h"

TEST(BuilderSuite, buildFileAndFolder)
{
    FileSystemBuilder *builder = new FileSystemBuilder();

    builder->buildFolder("structure/home");
    ASSERT_EQ("structure/home", builder->getRoot()->path());

    builder->buildFile("structure/home/my_profile");
    ASSERT_EQ(1, builder->getRoot()->numberOfFiles());
    builder->buildFolder("structure/home/Downloads");
    builder->endFolder();
    ASSERT_EQ("structure/home/Downloads", builder->getRoot()->getChildByName("Downloads")->path());

    delete builder;
}
