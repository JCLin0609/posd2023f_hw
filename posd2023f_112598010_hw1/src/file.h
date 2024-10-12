#if !defined(FILE_H)
#define FILE_H

#include <string>
#include "node.h"
#include "null_iterator.h"

class File : public Node
{
public:
    File(string path) { setPath(path), setIsFolder(false); };

    Iterator *createIterator() override
    {
        return new NullIterator(this);
    }
};

#endif // FILE_H
