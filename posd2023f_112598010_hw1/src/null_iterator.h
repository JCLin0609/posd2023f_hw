#if !defined(NULL_ITERATOR_H)
#define NULL_ITERATOR_H

#pragma once

class Node;
class File;

class NullIterator : public Iterator
{
    File *_file;

public:
    NullIterator(File *leaf) : _file(leaf){};
    bool isDone() const override { return true; }
};

#endif // NULL_ITERATOR_H
