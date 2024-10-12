#pragma once

#include<string>
#include "json_iterator.h"

class Value {
public:
    virtual ~Value() {}
    virtual std::string toString() = 0;
    virtual JsonIterator *createIterator() { return new NullIterator();}
};