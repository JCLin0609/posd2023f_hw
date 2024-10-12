#if !defined(DFS_ITERATOR_H)
#define DFS_ITERATOR_H

#include <stack>
#include <queue>
// #include "iterator.h"
// #include "folder.h"

class Node;
class Folder;
class Iterator;
class DfsIterator : public Iterator
{
    Node *_folder;
    std::stack<Node *> dfsStack;

public:
    DfsIterator(Node *composite);
    ~DfsIterator() {}

    void first() override;

    Node *currentItem() const override;

    void next() override;

    bool isDone() const override;
};

class BfsIterator : public Iterator
{
    Node *_folder;
    std::queue<Node *> bfsQueue;

public:
    BfsIterator(Node *composite);
    ~BfsIterator() {}
    void first() override;
    Node *currentItem() const override;
    void next() override;
    bool isDone() const override;
};

#endif // DFS_ITERATOR_H
