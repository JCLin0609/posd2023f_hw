#include "iterator.h"
#include "dfs_iterator.h"
#include "folder.h"
#include <iostream>

FolderIterator::FolderIterator(Folder *composite) : _folder(composite){};

void FolderIterator::first()
{

    _it = _folder->_ChildNodes.begin();
}

Node *FolderIterator::currentItem() const
{
    return *_it;
}

void FolderIterator::next()
{
    _it++;
}

bool FolderIterator::isDone() const
{
    return (_it == _folder->_ChildNodes.end());
}

DfsIterator::DfsIterator(Node *composite) : _folder(composite) {}

void DfsIterator::first()
{
    while (!dfsStack.empty())
        dfsStack.pop();
    dfsStack.push(_folder);
    next();
}

Node *DfsIterator::currentItem() const
{
    if (!dfsStack.empty())
        return dfsStack.top();
    return nullptr;
}

void DfsIterator::next()
{
    Folder *current = dynamic_cast<Folder *>(dfsStack.top());
    dfsStack.pop();
    if (current)
    {
        const std::vector<Node *> &childNodes = current->_ChildNodes;
        if (childNodes.size() > 0)
            for (auto it = childNodes.end() - 1; it >= childNodes.begin(); it--)
            {
                // std::cout << "Push: " << (*it)->path() << std::endl;
                dfsStack.push(*it);
            }
    }
}

bool DfsIterator::isDone() const { return dfsStack.empty(); }

BfsIterator::BfsIterator(Node *composite) : _folder(composite) {}

void BfsIterator::first()
{
    while (!bfsQueue.empty())
        bfsQueue.pop();
    bfsQueue.push(_folder);
    next();
}

Node *BfsIterator::currentItem() const
{
    if (!bfsQueue.empty())
        return bfsQueue.front();
    return nullptr;
}

void BfsIterator::next()
{
    Folder *current = dynamic_cast<Folder *>(bfsQueue.front());
    bfsQueue.pop();
    if (current)
    {
        const std::vector<Node *> &childNodes = current->_ChildNodes;
        if (childNodes.size() > 0)
            for (auto child = childNodes.begin(); child != childNodes.end(); child++)
                bfsQueue.push(*child);
    }
}

bool BfsIterator::isDone() const { return bfsQueue.empty(); }