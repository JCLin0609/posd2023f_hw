#pragma once

#include <string>

#include "folder.h"
#include "file_system_builder.h"
#include "file_system_scanner.h"

using std::string;

class FileSystemParser
{
    FileSystemBuilder *_builder;
    string _rootPath;

public:
    FileSystemParser(FileSystemBuilder *builder) : _builder(builder){};

    Folder *getRoot() const
    {
        return _builder->getRoot();
    };

    void parse()
    {
        FileSystemScanner *_scanner = new FileSystemScanner();
        queue<string> parseFolders;

        _builder->buildFolder(_rootPath);
        parseFolders.push(_rootPath);

        while (!parseFolders.empty())
        {
            _scanner->setPath(parseFolders.front());
            while (!_scanner->isDone())
            {
                string currentNodePath = _scanner->currentNodePath();
                if (_scanner->isFile())
                    _builder->buildFile(currentNodePath);
                else
                {
                    _builder->buildFolder(currentNodePath);
                    parseFolders.push(currentNodePath);
                }
                _scanner->nextNode();
            }
            _builder->endFolder();
            parseFolders.pop();
        }
    };

    void setPath(string path)
    {
        _rootPath = path;
    };
};
