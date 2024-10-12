#include <string>
#include "../src/folder.h"

using namespace std;

TEST(Folder, normal) {
    Folder home("structure/home");

    ASSERT_EQ("home", home.name());
    ASSERT_EQ("structure/home", home.path());
}

TEST(Folder, invalid_folder) {
    ASSERT_ANY_THROW(Folder("/NOT/EXIST/PATH"));
}

TEST(Folder, invalid_folder_2) {
    ASSERT_ANY_THROW(Folder("structure/home/Documents/hello.txt"));
}

TEST(Folder, add_file) {
    Folder * home = new Folder("structure/home");
    File * hello = new File("structure/home/hello.txt");
    home->add(hello);

    ASSERT_EQ("hello.txt", home->getChildByName("hello.txt")->name());

    delete home;
}

TEST(Folder, add_incorrect_path_file_to_folder) {
    Folder * home = new Folder("structure/home");
    File * hello = new File("structure/home/Documents/hello.txt");
    ASSERT_ANY_THROW(home->add(hello));
    
    delete home;
    delete hello;
}

TEST(Folder, add_folder) {
    Folder * home = new Folder("structure/home");
    Folder * document = new Folder("structure/home/Documents");

    home->add(document);

    ASSERT_EQ("Documents", home->getChildByName("Documents")->name());

    delete home;
}

TEST(Folder, rename) {
    Folder * home = new Folder("structure/home");
    File * hello = new File("structure/home/hello.txt");
    home->add(hello);
    home->rename("new_home");

    ASSERT_EQ("new_home", home->name());
    ASSERT_EQ("structure/new_home", home->path());
    ASSERT_EQ("hello.txt", home->getChildByName("hello.txt")->name());
    ASSERT_EQ("structure/new_home/hello.txt", home->getChildByName("hello.txt")->path());

    delete home;
}

TEST(Folder, renameWithRecursive) {
    Folder * home = new Folder("structure/home");
    Folder * document = new Folder("structure/home/Documents");
    File * hello = new File("structure/home/hello.txt");
    File * note = new File("structure/home/Documents/note.txt");
    home->add(hello);
    home->add(document);
    document->add(note);
    home->rename("new_home");
    

    ASSERT_EQ("new_home", home->name());
    ASSERT_EQ("structure/new_home", home->path());
    ASSERT_EQ("hello.txt", home->getChildByName("hello.txt")->name());
    ASSERT_EQ("structure/new_home/hello.txt", home->getChildByName("hello.txt")->path());
    ASSERT_EQ("Documents", home->getChildByName("Documents")->name());
    ASSERT_EQ("structure/new_home/Documents", home->getChildByName("Documents")->path());
    // ASSERT_EQ("note.txt", home->getChildByName("Documents")->getChildByName("note.txt")->name());
    // ASSERT_EQ("structure/new_home/Documents/note.txt", home->getChildByName("Documents")->getChildByName("note.txt")->path());

    delete home;
}