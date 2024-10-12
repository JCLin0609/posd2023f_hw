#pragma once

#include "iterator_factory.h"

class FolderIteratorFactory: public IteratorFactory {
};

class OrderByNameIteratorFactory: public IteratorFactory {
};

class OrderByNameWithFolderFirstIteratorFactory: public IteratorFactory {
};

class OrderByKindIteratorFactory: public IteratorFactory {
};
