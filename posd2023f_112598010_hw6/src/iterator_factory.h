#pragma once

#include "iterator.h"

class Folder;

class IteratorFactory {
public:
    virtual ~IteratorFactory() {}
    virtual Iterator * create(Folder * node, int operationCount) = 0;
};