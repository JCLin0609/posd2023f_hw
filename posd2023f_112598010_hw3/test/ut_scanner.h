#include "../src/node.h"
#include "../src/folder.h"
#include "../src/file.h"
#include "../src/file_system_scanner.h"

TEST(ScanerSuite, scanner)
{
    FileSystemScanner *scanner = new FileSystemScanner();
    scanner->setPath("structure");
    ASSERT_EQ("file.txt", scanner->currentNodeName());
    ASSERT_TRUE(scanner->isFile());
    scanner->nextNode();
    ASSERT_TRUE(scanner->isFolder());
    scanner->nextNode();
    ASSERT_TRUE(scanner->isFolder());
    scanner->nextNode();
    ASSERT_TRUE(scanner->isDone());

    delete scanner;
}