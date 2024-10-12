#pragma once

#include<string>
#include "visitor.h"

using namespace std;

class JsonObject;
class StringValue;
class BeautifyVisitor : public JsonVisitor {
    string resString;

public:
    BeautifyVisitor() {}

    void visitJsonObjectHelper(JsonObject * obj, int depth) {
        auto it = obj->createIterator();
        int count=0;
        
        resString += "{\n";
        for(int i=0;i<depth;i++)
            resString += "     ";
        for(it->first();!it->isDone();it->next()) {
            resString += "\"" +  it->currentKey() + "\": " ;

            if(dynamic_cast<JsonObject*>(it->currentValue()))
                this->visitJsonObjectHelper(dynamic_cast<JsonObject*>(it->currentValue()),depth+1);
            else
                resString +=  it->currentValue()->toString();
            
           
            if(count== (it->getSize()-1))
                resString += "\n";
            else
                resString += ",\n    ";
            for(int i=0;i<depth-1;i++)
                resString += "    ";
            if(count== (it->getSize()-1))
                resString += "}";

            count++;
        }
    }

    void visitJsonObject(JsonObject * obj){
        visitJsonObjectHelper(obj, 1);
    };

    void visitStringValue(StringValue * val) { 
        resString += val->toString();
    };

    string getResult() const {
        return resString;
    };
};