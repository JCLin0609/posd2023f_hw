#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include "node.h"
#include "visitor.h"

class File : public Node
{
public:
    File(string path) : Node(path)
    {
        struct stat fileStat;
        if (stat(path.c_str(), &fileStat) == 0)
        {
            if (!S_ISREG(fileStat.st_mode))
                throw -1;
        }
        else
            throw -1;
    }

    int numberOfFiles() const
    {
        return 1;
    }

    Node *find(string path)
    {
        if (this->path() == path)
        {
            return this;
        }
        return nullptr;
    }

    std::list<string> findByName(string name) override
    {
        std::list<string> pathList;
        if (this->name() == name)
        {
            pathList.push_back(this->path());
        }
        return pathList;
    }

    void accept(Visitor *visitor)
    {
        visitor->visitFile(this);
    }
};