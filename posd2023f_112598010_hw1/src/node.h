#if !defined(NODE_H)
#define NODE_H

#include <string>
#include <vector>
#include "iterator.h"
using namespace std;

class Node
{
    string _path;
    bool _isFolder;
    string getName(string path) const
    {
        std::size_t NamePosition = path.find_last_of("/");
        string name = path.substr(NamePosition + 1);
        return name;
    };

public:
    ~Node(){};

    string name() const
    {
        return this->getName(_path);
    };

    string path() const { return _path; };

    virtual void add(Node *node){};

    virtual void remove(string path){};

    virtual Node *getChildByName(const char *name) const { return nullptr; };

    virtual Node *find(string path) { return nullptr; };

    virtual int numberOfFiles() const { return 0; };

    virtual Iterator *createIterator() = 0;

    virtual Iterator *createDfsIterator() { return nullptr; }

    virtual Iterator *createBfsIterator() { return nullptr; }

    void setPath(string path) { _path = path; };

    void setIsFolder(bool b) { _isFolder = b; };

    bool getIsFolder() { return _isFolder; };
};

#endif // NODE_H
