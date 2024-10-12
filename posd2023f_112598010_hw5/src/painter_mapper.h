#pragma once

#include <string>
#include <sqlite3.h>

#include "abstract_mapper.h"

class Painter;

class PainterMapper: public AbstractMapper {
public:
    Painter* find(std::string id);
    
    void add(DomainObject * Painter);

    void update(std::string id);

    void del(std::string id);

    std::string updateStmt(DomainObject * domainObject) const;

    std::string findByIdStmt(std::string id) const;

    std::string addStmt(DomainObject * domainObject) const;

    std::string deleteByIdStmt(std::string id) const;

    static PainterMapper* instance();

protected:
    PainterMapper();

    static int callback(void* notUsed, int argc, char** argv, char** colNames){
        Painter* painter = new Painter(argv[0], argv[1]);
        PainterMapper::instance()->load(painter);
        UnitOfWork::instance()->registerClean(painter);
        return 0;
    };
    
private:
    char* _errorMessage;
    static PainterMapper* _instance;
};