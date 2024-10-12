#include "../src/node.h"
#include "../src/folder.h"
#include "../src/file.h"
#include "../src/tree_visitor.h"
#include "../src/file_system_parser.h"
#include "../src/file_system_builder.h"

class ParserSuite : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        home = new Folder("structure/home");

        profile = new File("structure/home/my_profile");
        home->add(profile);

        document = new Folder("structure/home/Documents");
        home->add(document);

        favorite = new Folder("structure/home/Documents/favorites");
        document->add(favorite);
        ddd = new File("structure/home/Documents/favorites/domain-driven-design.pdf");
        favorite->add(ddd);
        ca = new File("structure/home/Documents/favorites/clean-architecture.pdf");
        favorite->add(ca);
        cqrs = new File("structure/home/Documents/favorites/cqrs.pdf");
        favorite->add(cqrs);

        note = new File("structure/home/Documents/note.txt");
        document->add(note);

        download = new Folder("structure/home/Downloads");
        home->add(download);

        funny = new File("structure/home/Downloads/funny.png");
        download->add(funny);
    }

    void TearDown()
    {
        delete egg;
        delete home;
        delete profile;
        delete download;
        delete document;
        delete note;
        delete favorite;
        delete ddd;
        delete ca;
        delete cqrs;
        delete funny;
    }

    Node *egg;
    Node *home;
    Node *profile;
    Node *download;
    Node *document;
    Node *note;
    Node *favorite;
    Node *ddd;
    Node *ca;
    Node *cqrs;
    Node *funny;
};

TEST(ParserSuite, parser)
{
    FileSystemBuilder *builder = new FileSystemBuilder();
    FileSystemParser *parser = new FileSystemParser(builder);

    parser->setPath("structure/home");
    parser->parse();

    Folder *root = parser->getRoot();
    TreeVisitor *tVisitor = new TreeVisitor(OrderBy::Name);
    string res;

    root->accept(tVisitor);
    res = tVisitor->getTree();
    // std::cout << res << endl;

    string ans = {".\n├── Documents\n│   ├── favorites\n│   │   ├── clean-architecture.pdf\n│   │   ├── cqrs.pdf\n│   │   └── domain-driven-design.pdf\n│   ├── hello.txt\n│   └── note.txt\n├── Downloads\n│   └── funny.png\n├── hello.txt\n└── my_profile\n"};

    ASSERT_EQ(res, ans);
}