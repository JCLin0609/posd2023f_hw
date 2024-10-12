#pragma once

#include <string>
#include <stack>
#include "json_object.h"

class JsonObject;
class JsonBuilder {
    JsonObject *_root;
    std::stack<JsonObject*> _compounds;

public:
    void buildValue(std::string key, std::string value) {
        Value * element = new StringValue(value);
        if (_compounds.empty())
            _root->set(key, element);
        else
            _compounds.top()->set(key,element);
    };

    void buildObject(std::string key) {
        JsonObject *element = new JsonObject();
        if (_compounds.empty())
            _root = element;
        else
            _compounds.top()->set(key,element);
        _compounds.push(element);
    };

    void endObject() {
        if (!_compounds.empty())
            _compounds.pop();
    };

    JsonObject * getJsonObject() {
        return _root;
    };
};
