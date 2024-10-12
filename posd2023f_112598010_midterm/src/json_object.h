#pragma once

#include "value.h"
#include <string>
#include <map>
#include <stack>
#include <algorithm>
#include "string_value.h"
#include "json_iterator.h"
#include "visitor.h"

using namespace std;

class JsonObject : public Value {
    std::map<string , Value*> _composite;
    
public:
    JsonObject() {}

    void set(string key, Value * element) {
        _composite[key] = element;
    }

    Value* getValue(string value) {
        Value *res = _composite[value];
        if(res)
            return _composite[value];
        throw("getValue err");
    }

    string toString() override {
        string resString;

        resString += "{\n";

        int _compositeSize = _composite.size();
        int count=0;

        for(auto it : _composite) {
            resString += "\"" +  it.first + "\":" ;
            
            resString += it.second->toString();
          
            if(count==_compositeSize-1)
                resString += "\n}";
            else
                resString += ",\n";
            count++;
        }

        return resString;
    }

    JsonIterator *createIterator() override {
        return new JsonObjectIterator(this);
    }

    class JsonObjectIterator : public JsonIterator {
        JsonObject *_host;
        map<string , Value*>::iterator _current;

    public:
        JsonObjectIterator(JsonObject *host) : _host(host) {}
    
        void first() override {
            _current = _host->_composite.begin();
        };

        std::string currentKey() const override {
            return _current->first;
        };

        Value * currentValue() const override{
            return _current->second;
        };

        void next() override {
            _current++;
        };

        bool isDone() const override {
            return _current == _host->_composite.end();
        };

        int getSize() override { return _host->_composite.size();}
    };

    void accept(JsonVisitor *jsonVisitor){
        jsonVisitor->visitJsonObject(this);
    }
};