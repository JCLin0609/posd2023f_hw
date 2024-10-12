#pragma once

#include <string>
#include <queue>
#include "folder.h"

using std::string;

class FileSystemBuilder
{
    Folder *_root;
    std::queue<Node *> _compounds;

public:
    FileSystemBuilder() {}

    Folder *getRoot() const
    {
        return _root;
    };

    void buildFile(string path)
    {
        File *file = new File(path);
        if (_compounds.empty())
            _root->add(file);
        else
            _compounds.front()->add(file);
    };

    void buildFolder(string path)
    {
        Folder *folder = new Folder(path);
        if (_compounds.empty())
            _root = folder;
        else
            _compounds.front()->add(folder);
        _compounds.push(folder);
    };

    void endFolder()
    {
        if (!_compounds.empty())
            _compounds.pop();
    };
};
