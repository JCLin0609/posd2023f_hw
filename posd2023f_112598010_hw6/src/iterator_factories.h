#pragma once

#include "iterator_factory.h"
#include "iterator.h"
#include "folder.h"

class FolderIteratorFactory: public IteratorFactory {
public:
    Iterator * create(Folder * node, int operationCount) override {
        return new Folder::FolderIterator(node, operationCount);
    }

    static IteratorFactory* instance() {
        if(_instance == nullptr) {
            _instance = new FolderIteratorFactory();
        }
        return _instance;
    };

private:
    static FolderIteratorFactory* _instance;
};


class OrderByNameIteratorFactory: public IteratorFactory {
public:
    Iterator * create(Folder * node, int operationCount) override{
        return new Folder::OrderByNameIterator(node, operationCount);
    }

    static IteratorFactory* instance() {
        if(_instance == nullptr) {
            _instance = new OrderByNameIteratorFactory();
        }
        return _instance;
    };

private:
    static OrderByNameIteratorFactory* _instance;
};

class OrderByNameWithFolderFirstIteratorFactory: public IteratorFactory {
public:
    Iterator * create(Folder * node, int operationCount) override{
        return new Folder::OrderByNameWithFolderFirstIterator(node, operationCount);
    }

    static IteratorFactory* instance() {
        if(_instance == nullptr) {
            _instance = new OrderByNameWithFolderFirstIteratorFactory();
        }
        return _instance;
    };
private:
    static OrderByNameWithFolderFirstIteratorFactory* _instance;
};

class OrderByKindIteratorFactory: public IteratorFactory {
public:
    Iterator * create(Folder * node, int operationCount) override{
        return new Folder::OrderByKindIterator(node, operationCount);
    }

    static IteratorFactory* instance() {
        if(_instance == nullptr) {
            _instance = new OrderByKindIteratorFactory();
        }
        return _instance;
    };
private:
    static OrderByKindIteratorFactory* _instance;
};

OrderByNameWithFolderFirstIteratorFactory* OrderByNameWithFolderFirstIteratorFactory::_instance = nullptr;
OrderByNameIteratorFactory* OrderByNameIteratorFactory::_instance = nullptr;
OrderByKindIteratorFactory* OrderByKindIteratorFactory::_instance = nullptr;
