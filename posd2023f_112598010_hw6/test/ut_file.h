#include "../src/file.h"

TEST(File, normal) {
    File hello("structure/file.txt");
    ASSERT_EQ("file.txt", hello.name());
    ASSERT_EQ("structure/file.txt", hello.path());
}


TEST(File, invalid_file) {
    ASSERT_ANY_THROW(File("/NOT/EXIST/PATH"));
}

TEST(File, invalid_file_2) {
    ASSERT_ANY_THROW(File("structure/home"));
}

TEST(File, rename) {
    File hello("structure/file.txt");
    hello.rename("new_file.txt");

    ASSERT_EQ("new_file.txt", hello.name());
    ASSERT_EQ("structure/new_file.txt", hello.path());
}