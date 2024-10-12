#pragma once

#include <list>
#include <sys/types.h>
#include <sys/stat.h>
#include "node.h"
#include "iterator.h"
#include "dfs_iterator.h"
#include "visitor.h"

using namespace std;

class Folder : public Node
{
private:
    list<Node *> _nodes;
    bool isAvailiable;

protected:
    void removeChild(Node *target)
    {
        _nodes.remove(target);
    }

public:
    Folder(string path) : Node(path)
    {
        isAvailiable = false;
        struct stat fileStat;
        if (stat(path.c_str(), &fileStat) == 0)
        {
            if (!S_ISDIR(fileStat.st_mode))
                throw -1;
        }
        else
            throw -1;
    }

    class FolderIterator : public Iterator
    {
    public:
        FolderIterator(Folder *composite) : _host(composite){};
        ~FolderIterator() {}
        void first()
        {
            if (_host->isAvailiable == true)
                _current = _host->_nodes.begin();
            else
                throw -1;
        };
        Node *currentItem() const
        {
            return *_current;
        };
        void next()
        {
            if (_host->isAvailiable == true)
                _current++;
            else
                throw -1;
        };
        bool isDone() const
        {
            return _current == _host->_nodes.end();
        };

    private:
        Folder *const _host;
        std::list<Node *>::iterator _current;
    };

    void add(Node *node)
    {
        if (node->path() != this->path() + "/" + node->name())
        {
            throw string("Incorrect path of node: " + node->path());
        }
        _nodes.push_back(node);
        node->parent(this);
        isAvailiable = false;
    }

    Node *getChildByName(const char *name) const
    {
        for (auto it = _nodes.begin(); it != _nodes.end(); ++it)
        {
            if ((*it)->name() == name)
            {
                return *it;
            }
        }
        return nullptr;
    }

    int numberOfFiles() const
    {
        int num = 0;
        if (_nodes.size() == 0)
        {
            return 0;
        }
        for (auto it = _nodes.begin(); it != _nodes.end(); ++it)
        {
            num += (*it)->numberOfFiles();
        }
        return num;
    }

    Iterator *createIterator()
    {
        isAvailiable = true;
        return new FolderIterator(this);
    }

    Iterator *dfsIterator()
    {
        return new DfsIterator(this);
    }

    Node *find(string path)
    {
        if (this->path() == path)
        {
            return this;
        }

        size_t index = path.find(this->path());

        if (string::npos == index)
        {
            return nullptr;
        }

        for (auto it = _nodes.begin(); it != _nodes.end(); ++it)
        {
            Node *result = (*it)->find(path);
            if (result)
            {
                return result;
            }
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

        for (auto it = _nodes.begin(); it != _nodes.end(); ++it)
        {
            std::list<string> paths = (*it)->findByName(name);
            for (auto i = paths.begin(); i != paths.end(); i++)
            {
                pathList.push_back(*i);
            }
        }

        return pathList;
    }

    void remove(string path)
    {
        Node *target = find(path);
        if (target)
        {
            target->parent()->removeChild(target);
        }
    }

    void accept(Visitor *visitor)
    {
        visitor->visitFolder(this);
    }
};