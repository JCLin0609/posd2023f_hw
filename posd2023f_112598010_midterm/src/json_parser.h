#pragma once

#include "./json_scanner.h"
#include "./json_builder.h"
#include "./json_object.h"
#include <string>
#include <regex>
#include <iostream>

class JsonParser {
public:
    JsonParser(JsonScanner * scanner, JsonBuilder * builder): _scanner(scanner), _builder(builder) {}

    void parse() {
        std::string key = "";
        while (!_scanner->isDone()) {
            char token = _scanner->next();
            if(token == ' ') {
                continue;
            } else if (token == '{') {
                // std::cout << "1: " << std::endl;
                _builder->buildObject(key);              // fill in the code
            } else if (token == '"') {
                std::string value;
                token = _scanner->next();
                while(token != '"') {
                    value += token;
                    token = _scanner->next();
                }
                token = _scanner->next();

                while (token == ' ') {
                    token = _scanner->next();
                }
                
                if(token == ':') {
                    key = value;
                }
                else if(token == ',') {
                    // std::cout << "2: " << std::endl;
                    _builder->buildValue(key,value);         // fill in the code
                } else if (token == '}') {
                    // std::cout << "3: " << std::endl;
                     _builder->buildValue(key,value);        // fill in the code
                }
            }
            else if (token == '}') {
                // std::cout << "4: " << std::endl;
                _builder->endObject();                       // fill in the code
            }
        }
    }

    JsonObject * getJsonObject() {
        // std::cout << "5: " << std::endl;
        return _builder->getJsonObject();    // fill in the code
    }

    void setInput(std::string input) {
        _scanner->setInput(input);
    }

private:
    JsonScanner * _scanner;
    JsonBuilder * _builder;
};
