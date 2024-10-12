#pragma once

#include <string>
#include "folder.h"
#include "iterator.h"
#include <sys/types.h>
#include <dirent.h>
using std::string;

class FileSystemScanner
{
    string _path;
    DIR *_directory;
    struct dirent *_current;

public:
    FileSystemScanner() {}

    bool isFile() { return _current->d_type == DT_REG; };

    bool isFolder() { return _current->d_type == DT_DIR; };

    bool isDone() { return _current == NULL; };

    void setPath(string path)
    {
        _path = path;
        _directory = opendir(path.c_str());
        if (!_directory)
            throw "Unable to open directory.\n";
        this->nextNode();
    };

    string currentNodeName()
    {
        return _current->d_name;
    };

    string currentNodePath()
    {
        return _path + "/" + currentNodeName();
    }

    void nextNode()
    {
        _current = readdir(_directory);
        if (_current && _current->d_name[0] == '.')
            this->nextNode();
    };
};
