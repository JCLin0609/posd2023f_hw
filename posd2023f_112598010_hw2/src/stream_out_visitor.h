#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "visitor.h"
#include "file.h"
#include "folder.h"

class StreamOutVisitor : public Visitor
{
    string output;

public:
    StreamOutVisitor() {}

    void visitFile(File *file) override
    {
        output.append("_____________________________________________\n");
        output.append(file->path());
        output.append("\n");
        output.append("---------------------------------------------\n");

        std::ifstream f(file->path());
        string tmp;
        while (getline(f, tmp))
        {
            output.append(tmp);
            output.append("\n");
        }
        output.append("_____________________________________________\n");
    }

    void visitFolder(Folder *folder) override
    {
        auto it = folder->createIterator();
        for (it->first(); !it->isDone(); it->next())
        {
            it->currentItem()->accept(this);
            if (dynamic_cast<File *>(it->currentItem()))
                output.append("\n");
        }
    }

    string getResult() const
    {
        return output;
    }
};