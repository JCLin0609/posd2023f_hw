#if !defined(ITERATOR_H)
#define ITERATOR_H

#pragma once

#include <list>
#include <vector>

class Node;
class Folder;
class Iterator
{
public:
    ~Iterator(){};
    virtual void first() {}
    virtual Node *currentItem() const { return nullptr; }
    virtual void next() {}
    virtual bool isDone() const { return false; }
};

class FolderIterator : public Iterator
{

public:
    Folder *_folder;
    std::vector<Node *>::iterator _it;
    FolderIterator(Folder *composite);
    void first() override;
    Node *currentItem() const override;
    void next() override;
    bool isDone() const override;
};

#endif // ITERATOR_H
