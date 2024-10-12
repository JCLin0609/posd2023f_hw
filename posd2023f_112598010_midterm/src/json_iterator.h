#pragma once

#include<string>

using namespace std;

class Value;

class JsonIterator {
public:
    virtual void first() = 0;

    virtual std::string currentKey() const = 0;

    virtual Value * currentValue() const = 0;

    virtual void next() = 0;
    virtual bool isDone() const = 0;

    virtual int getSize() { return 0;}
};

class NullIterator : public JsonIterator {
public:
    NullIterator() {}

    void first() override { };

    std::string currentKey() const override { return ""; };

    Value * currentValue() const override { return nullptr; };

    void next() override {};
    bool isDone() const override {return true;};
};