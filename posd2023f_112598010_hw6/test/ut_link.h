#include "../src/link.h"

TEST(Link, LinkFile) {
    File home("structure/home/hello.txt");
    Link hello("structure/link.txt", &home);

    ASSERT_EQ("link.txt", hello.name());
    ASSERT_EQ("structure/link.txt", hello.path());
    ASSERT_EQ("structure/home/hello.txt", hello.getTarget()->path());
    ASSERT_EQ("hello.txt", hello.getTarget()->name());
}

TEST(Link, LinkFolder) {
    Folder home("structure/home");
    Link hello("structure/link", &home);

    ASSERT_EQ("link", hello.name());
    ASSERT_EQ("structure/link", hello.path());
    ASSERT_EQ("structure/home", hello.getTarget()->path());
    ASSERT_EQ("home", hello.getTarget()->name());
}