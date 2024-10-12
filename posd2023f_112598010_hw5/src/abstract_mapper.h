#pragma once

#include <unordered_map>
#include "domain_object.h"
#include "unit_of_work.h"
#include <iostream>
#include "drawing.h"

typedef int (*CallbackType)(void *, int argc, char **argv, char **col_names);

class AbstractMapper
{
public:
    virtual ~AbstractMapper()
    {
    }

    virtual std::string updateStmt(DomainObject *domainObject) const = 0;
    virtual std::string findByIdStmt(std::string id) const = 0;
    virtual std::string addStmt(DomainObject *domainObject) const = 0;
    virtual std::string deleteByIdStmt(std::string id) const = 0;

    void setDB(std::string db_file)
    {
        sqlite3_open(db_file.c_str(), &_db);
    }

    void closeDB()
    {
        sqlite3_close(_db);
    }

    bool isLoaded(const std::string &id) const
    {
        return _domainObjects.count(id);
    }

    void cleanCache()
    {
        _domainObjects.clear();
    }

protected:
    std::unordered_map<std::string, DomainObject *> _domainObjects;
    sqlite3 *_db;

    DomainObject *getDomainObject(std::string id)
    {
        if (_domainObjects.count(id))
        {
            return _domainObjects.at(id);
        }
        return nullptr;
    }

    DomainObject * abstractFind(std::string id, CallbackType callback) {
        if (isLoaded(id)) {
            return getDomainObject(id);
        }
        std::string stmt = findByIdStmt(id);
        char *errorMessage;
        sqlite3_exec(_db, stmt.c_str(), callback, 0, &errorMessage);
        if (errorMessage != nullptr) {
            std::cerr << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }
        return getDomainObject(id);
    };

    void abstractAdd(DomainObject *domainObject) {
        std::string stmt = addStmt(domainObject);
        char *errorMessage;
        sqlite3_exec(_db, stmt.c_str(), nullptr, 0, &errorMessage);
        if (errorMessage != nullptr) {
            std::cerr << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }
        else {
            _domainObjects.insert(std::make_pair(domainObject->id(), domainObject));
        }
    } ;

    void abstractUpdate(DomainObject *domainObject) {
        std::string stmt = updateStmt(domainObject);
        char *errorMessage;
        sqlite3_exec(_db, stmt.c_str(), nullptr, 0, &errorMessage);
        if (errorMessage != nullptr) {
            std::cerr << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }
        else {
            _domainObjects[domainObject->id()] = domainObject;
        }
    };

    void abstractDelete(std::string id) {
        std::string stmt = deleteByIdStmt(id);
        char *errorMessage;
        sqlite3_exec(_db, stmt.c_str(), nullptr, 0, &errorMessage);
        if (errorMessage != nullptr) {
            std::cerr << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }
        else {
            _domainObjects.erase(id);
        }
    };

    void load(DomainObject *domainObject) {
        _domainObjects.insert(std::make_pair(domainObject->id(), domainObject));
    };
private:
};