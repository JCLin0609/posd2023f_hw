#pragma once

#include <string>

class DomainObject {
public:
    DomainObject(std::string id): _id(id) {}

    virtual ~DomainObject() {
        
    }

    std::string id() const {
        return _id;
    }

    
    
protected:
    std::string _id;
};