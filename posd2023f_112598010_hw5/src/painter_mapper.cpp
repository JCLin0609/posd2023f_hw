#include "painter_mapper.h"
#include <string>
#include <iostream>
#include <sqlite3.h>

#include "painter.h"
#include "abstract_mapper.h"

PainterMapper* PainterMapper::_instance = nullptr;

PainterMapper::PainterMapper() {
    
}

Painter* PainterMapper::find(std::string id) {
    return static_cast<Painter*>(abstractFind(id, PainterMapper::callback));
}

void PainterMapper::add(DomainObject * Painter) {
    abstractAdd(Painter);
}

void PainterMapper::update(std::string id) {
    abstractUpdate(getDomainObject(id));
}

void PainterMapper::del(std::string id) {
    abstractDelete(id);
}

std::string PainterMapper::updateStmt(DomainObject * domainObject) const {
    Painter* painter = static_cast<Painter*>(domainObject);
    std::string stmt = "UPDATE painter SET name = '" + painter->name() + "' WHERE ID = '" + painter->id() + "'";
    return stmt;
}

std::string PainterMapper::findByIdStmt(std::string id) const {
    std::string stmt = "SELECT * FROM painter WHERE ID = '" + id + "'";
    return stmt;
}

std::string PainterMapper::addStmt(DomainObject * domainObject) const {
    Painter* painter = static_cast<Painter*>(domainObject);
    std::string stmt = "INSERT INTO painter (ID, NAME) VALUES ('" + painter->id() + "', '" + painter->name() + "')";
    return stmt;
}

std::string PainterMapper::deleteByIdStmt(std::string id) const {
    std::string stmt = "DELETE FROM painter WHERE ID = '" + id + "'";
    return stmt;
}

PainterMapper * PainterMapper::instance() {
    if (_instance == nullptr) {
        _instance = new PainterMapper();
    }
    return _instance;
}