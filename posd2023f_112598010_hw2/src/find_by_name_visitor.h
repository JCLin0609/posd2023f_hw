#pragma once

#include <vector>
#include "visitor.h"
#include "file.h"
#include "folder.h"

class FindByNameVisitor : public Visitor
{
    string _name;
    std::vector<string> pathList;

public:
    FindByNameVisitor(string name) : _name(name) {}

    void visitFile(File *file) override
    {
        if (file->name() == _name)
            pathList.push_back(file->path());
    }

    void visitFolder(Folder *folder) override
    {
        if (folder->name() == _name)
            pathList.push_back(folder->path());

        auto it = folder->createIterator();
        for (it->first(); !it->isDone(); it->next())
        {
            it->currentItem()->accept(this);
        }
    }

    std::vector<string> getPaths() const
    {
        return pathList;
    }
};