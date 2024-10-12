#pragma once

#include "visitor.h"
#include "order_by.h"

class File;
class Folder;
class TreeVisitor : public Visitor
{
    OrderBy _orderBy;
    string _result = ".\n";

public:
    TreeVisitor(OrderBy orderBy) : _orderBy(orderBy){};

    void visitFile(File *file)
    {
        _result += "└── " + file->name() + "\n";
    };

    void visitFolderHelperFunction(Folder *folder,long unsigned int depth, vector<bool> &hasNodeBehindRecord)
    {
        Iterator *it = folder->createIterator(_orderBy);

        int lastNodePos = folder->numberOfNode() - 1;
        int countNode = 0;

        for (it->first(); !it->isDone(); it->next())
        {
            bool isLastNode = (countNode == lastNodePos);

            if (depth >= hasNodeBehindRecord.size())
                hasNodeBehindRecord.push_back(isLastNode);
            else
                hasNodeBehindRecord[depth] = (isLastNode);

            if (depth > 0)
            {
                for (long unsigned int i = 0; i < depth; i++)
                {
                    if (!hasNodeBehindRecord[i])
                        _result += "│   ";
                    else
                        _result += "    ";
                }
            }

            if (!isLastNode)
                _result += "├── " + it->currentItem()->name() + "\n";
            else
                _result += "└── " + it->currentItem()->name() + "\n";

            Folder *innerFolder = dynamic_cast<Folder *>(it->currentItem());
            if (innerFolder)
                visitFolderHelperFunction(innerFolder, depth + 1, hasNodeBehindRecord);

            countNode++;
        }
    }

    void visitFolder(Folder *folder)
    {
        vector<bool> hasNodeBehind;
        visitFolderHelperFunction(folder, 0, hasNodeBehind);
    }

    string getTree()
    {
        return _result;
    }
};