#pragma once

#include "value.h"
#include <string>
#include <vector>
#include "visitor.h"

using namespace std;

class StringValue : public Value {
    string _value;

public:
    StringValue(std::string value) : _value(value) {}

    ~StringValue() {}

    string toString() override {
        return "\"" + _value + "\"";
    }

    void accept(JsonVisitor *jsonVisitor){
        jsonVisitor->visitStringValue(this);
    }
};