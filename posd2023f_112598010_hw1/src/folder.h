#if !defined(FOLDER)
#define FOLDER

#include <iostream>
#include <string>
#include <stack>
#include "node.h"
#include "iterator.h"
#include "dfs_iterator.h"

using namespace std;
class Folder : public Node
{
    friend class Iterator;
    friend class FolderIterator;
    friend class DfsIterator;
    friend class BfsIterator;
    vector<Node *> _ChildNodes;

public:
    Folder(string path) { setPath(path), setIsFolder(true); };

    void add(Node *node) override
    {
        std::size_t NamePosition = node->path().find_last_of("/");
        if (NamePosition == 0)
            NamePosition = 1;
        string nodePath = node->path().substr(0, NamePosition);

        if (this->path() == nodePath)
        {
            _ChildNodes.push_back(node);
        }
        else
            throw("Path error");
    };

    Node *getChildByName(const char *name) const override
    {
        for (auto node : _ChildNodes)
        {
            if (node->name() == name)
                return node;
        }
        return nullptr;
    }

    Iterator *createIterator() override
    {
        return new FolderIterator(this);
    }

    Iterator *createDfsIterator() override
    {
        return new DfsIterator(this);
    }

    Iterator *createBfsIterator() override
    {
        return new BfsIterator(this);
    }

    Node *find(string path) override
    {
        Node *retNode = nullptr;
        if (this->path() == path)
            return this;

        stack<string> nameStack;
        string tmp = path;
        while (tmp != "")
        {
            std::size_t lastName = tmp.find_last_of("/");
            nameStack.push(tmp.substr(lastName + 1));
            tmp = tmp.substr(0, lastName);
            if (this->path() == tmp.substr(0, lastName))
                break;
        }

        while (!nameStack.empty())
        {
            string name = nameStack.top();
            Node *nextNode = this->getChildByName(name.c_str());
            if (nextNode)
            {
                if (nextNode->path() == path)
                    retNode = nextNode;
                else
                    retNode = nextNode->find(path);
            }
            nameStack.pop();
        }

        if (retNode == NULL or retNode == nullptr)
            return nullptr;
        else
            return retNode;
    }

    int numberOfFiles() const override
    {
        int total = 0;
        for (auto childnode : _ChildNodes)
        {
            if (!childnode->getIsFolder())
                total++;
            else
                total += childnode->numberOfFiles();
        }
        return total;
    }

    void remove(string path) override
    {
        int count = 0;
        for (auto node : _ChildNodes)
        {
            if (node->path() == path)
            {
                _ChildNodes.erase(_ChildNodes.begin() + count);
                return;
            }
            Folder *folder = dynamic_cast<Folder *>(node);
            if (folder)
                folder->remove(path);
            count++;
        }
    }
};

#endif // FOLDER
