#pragma once

#include <list>
#include <sys/stat.h>
#include "node.h"
#include "iterator.h"
#include <string.h>
#include <algorithm>
using namespace std;

class Folder : public Node
{
private:
    list<Node *> _nodes;
    int _operationCount = 0;
    OrderBy _order;

protected:
    void removeChild(Node *target) override
    {
        _nodes.remove(target);
        _operationCount++;
    }

public:
    Folder(string path) : Node(path)
    {
        struct stat fileInfo;
        const char *c = path.c_str();
        if (lstat(c, &fileInfo) == 0)
        {
            if (S_ISDIR(fileInfo.st_mode))
                return;
        }
        throw "No Folder exists";
    }

    void add(Node *node) override
    {
        if (node->path() != this->path() + "/" + node->name())
        {
            throw string("Incorrect path of node: " + node->path());
        }
        _nodes.push_back(node);
        node->parent(this);
        _operationCount++;
    }

    Node *getChildByName(const char *name) const override
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

    int numberOfFiles() const override
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

    int numberOfNode() const override
    {
        return _nodes.size();
    }

    Iterator *createIterator(OrderBy orderBy) override
    {
        _order = orderBy;
        switch (_order)
        {
        case OrderBy::Normal:
            // std::cout << "case OrderBy::Normal" << std::endl;
            return this->createIterator();
            break;
        case OrderBy::Name:
            // std::cout << "case OrderBy::Name" << std::endl;
            return new OrderByNameIterator(this, _operationCount);
            break;
        case OrderBy::NameWithFolderFirst:
            // std::cout << "OrderBy::NameWithFolderFirst" << std::endl;
            return new OrderByNameWithFolderFirstIterator(this, _operationCount);
            break;
        case OrderBy::Kind:
            // std::cout << "OrderBy::Kind" << std::endl;
            return new OrderByKindIterator(this, _operationCount);
            break;
        }
        return new NullIterator();
    }

    Iterator *createIterator() override
    {
        return new FolderIterator(this, _operationCount);
    }

    Node *find(string path) override
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

    void remove(string path) override
    {
        Node *target = find(path);
        if (target)
        {
            target->parent()->removeChild(target);
        }
    }

    void accept(Visitor *visitor) override
    {
        visitor->visitFolder(this);
    }

    class FolderIterator : public Iterator
    {
    public:
        FolderIterator(Folder *composite, int operationCount) : _host(composite), _operationCount(operationCount)
        {
        }

        ~FolderIterator() {}

        void first() override
        {
            checkAvailable();
            _current = _host->_nodes.begin();
        }

        Node *currentItem() const override
        {
            return *_current;
        }

        void next() override
        {

            checkAvailable();
            _current++;
        }

        bool isDone() const override
        {
            return _current == _host->_nodes.end();
        }

    private:
        Folder *const _host;
        std::list<Node *>::iterator _current;
        int _operationCount;

        void checkAvailable() const
        {
            if (_host->_operationCount != _operationCount)
            {
                throw "Iterator Not Avaliable";
            }
        }
    };

    class OrderByNameIterator : public Iterator
    {
        Folder *const _host;
        std::list<Node *>::iterator _current2;
        int _operationCount;
        void checkAvailable() const
        {
            if (_host->_operationCount != _operationCount)
            {
                throw "Iterator Not Avaliable";
            }
        }

        static bool compare(Node *a, Node *b)
        {
            return a->name() < b->name();
        }

    public:
        OrderByNameIterator(Folder *composite, int operationCount) : _host(composite), _operationCount(operationCount)
        {
        }

        ~OrderByNameIterator() {}

        void first() override
        {
            _host->_nodes.sort(compare);
            _current2 = _host->_nodes.begin();
        }

        Node *currentItem() const override
        {
            return *_current2;
        }

        void next() override
        {
            _current2++;
        }

        bool isDone() const override
        {
            return _current2 == _host->_nodes.end();
        }
    };

    class OrderByNameWithFolderFirstIterator : public Iterator
    {
        Folder *const _host;
        int _operationCount;

        std::list<Node *>::iterator _current2;

        void checkAvailable() const
        {
            if (_host->_operationCount != _operationCount)
            {
                throw "Iterator Not Avaliable";
            }
        }

        static bool compareByNameWithFolderFirst(Node *a, Node *b)
        {
            return a->name() < b->name();
        }

        void collectNodes2()
        {
            std::vector<Node *> fileToSort;   // 記錄和排序當前資料夾內的file
            std::vector<Node *> folderToSort; // 記錄和排序當前資料夾內的folder
            for (auto node : _host->_nodes)
            {
                if (dynamic_cast<File *>(node))
                    fileToSort.push_back(node);
                else
                    folderToSort.push_back(node);
            }
            std::sort(folderToSort.begin(), folderToSort.end(), compareByNameWithFolderFirst);
            std::sort(fileToSort.begin(), fileToSort.end(), compareByNameWithFolderFirst);

            _host->_nodes.clear();
            for (auto node : folderToSort)
                _host->_nodes.push_back(node);
            for (auto node : fileToSort)
                _host->_nodes.push_back(node);
        }

    public:
        OrderByNameWithFolderFirstIterator(Folder *composite, int operationCount) : _host(composite), _operationCount(operationCount)
        {
        }

        ~OrderByNameWithFolderFirstIterator() {}

        void first() override
        {
            collectNodes2();
            _current2 = _host->_nodes.begin();
        }

        Node *currentItem() const override
        {
            return *_current2;
        }

        void next() override
        {
            _current2++;
        }

        bool isDone() const override
        {
            return _current2 == _host->_nodes.end();
        }
    };

    class OrderByKindIterator : public Iterator
    {
        Folder *const _host;
        std::vector<std::pair<Node *, int>> _sortedNodes;
        std::vector<std::pair<Node *, int>>::iterator _current;
        std::vector<bool> _sortedNodesHasNodeBehind;
        std::vector<bool>::iterator _currentHasNodeBehind;

        std::list<Node *>::iterator _current2;

        int _operationCount;
        void checkAvailable() const
        {
            if (_host->_operationCount != _operationCount)
            {
                throw "Iterator Not Avaliable";
            }
        }

        static bool kindCompare(Node *a, Node *b)
        {
            std::string nameA = a->name();
            std::string nameB = b->name();

            size_t posA = nameA.find_last_of('.');
            size_t posB = nameB.find_last_of('.');

            std::string extA = (posA != std::string::npos) ? nameA.substr(posA + 1) : "";
            std::string extB = (posB != std::string::npos) ? nameB.substr(posB + 1) : "";

            if (extA != extB)
                return extA < extB;

            if (dynamic_cast<File *>(a) && dynamic_cast<Folder *>(b))
                return true;
            else if (dynamic_cast<Folder *>(a) && dynamic_cast<File *>(b))
                return false;

            return a->name() < b->name();
        }

    public:
        OrderByKindIterator(Folder *composite, int operationCount) : _host(composite), _operationCount(operationCount)
        {
        }

        ~OrderByKindIterator() {}

        void first() override
        {
            _host->_nodes.sort(kindCompare);
            _current2 = _host->_nodes.begin();
        }

        Node *currentItem() const override
        {

            return *_current2;
        }

        void next() override
        {

            _current2++;
        }

        bool isDone() const override
        {
            return _current2 == _host->_nodes.end();
        }
    };
};
