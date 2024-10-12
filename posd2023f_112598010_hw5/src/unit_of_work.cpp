#include "unit_of_work.h"
#include "drawing_mapper.h"
#include "painter_mapper.h"
#include "domain_object.h"
#include <list>
#include <iostream>


UnitOfWork * UnitOfWork::_instance = nullptr;

UnitOfWork::UnitOfWork() {
    
}

UnitOfWork * UnitOfWork::instance() {
    if (_instance == nullptr) {
        _instance = new UnitOfWork();
    }
    return _instance;
}

void UnitOfWork::registerNew(DomainObject * obj) {
    _new[obj->id()] = obj;
    _clean.erase(obj->id());
    _deleted.erase(obj->id());
    _dirty.erase(obj->id());
}

void UnitOfWork::registerDirty(DomainObject * obj) {
    _dirty[obj->id()] = obj;
    _clean.erase(obj->id());
    _new.erase(obj->id());
    _deleted.erase(obj->id());
}

void UnitOfWork::registerClean(DomainObject * obj) {
    _clean[obj->id()] = obj;
    _dirty.erase(obj->id());
    _new.erase(obj->id());
    _deleted.erase(obj->id());
}

void UnitOfWork::registerDeleted(DomainObject * obj) {
    _deleted[obj->id()] = obj;
    _clean.erase(obj->id());
    _dirty.erase(obj->id());
    _new.erase(obj->id());
}

bool UnitOfWork::inNew(std::string id) const {
    return _new.find(id) != _new.end();
}

bool UnitOfWork::inDirty(std::string id) const {
    return _dirty.find(id) != _dirty.end();
}

bool UnitOfWork::inClean(std::string id) const {
    return _clean.find(id) != _clean.end();
}

bool UnitOfWork::inDeleted(std::string id) const {
    return _deleted.find(id) != _deleted.end();
}

void UnitOfWork::commit() {
    for (auto obj : _new) {
        if(Painter * painter = dynamic_cast<Painter *>(obj.second)) {
            PainterMapper::instance()->add(painter);
        } else if(Drawing * drawing = dynamic_cast<Drawing *>(obj.second)) {
            DrawingMapper::instance()->add(drawing);
        }
        _clean[obj.first] = obj.second;
    }
    for (auto obj : _dirty) {
        if(Painter * painter = dynamic_cast<Painter *>(obj.second)) {
            PainterMapper::instance()->update(painter->id());
        } else if(Drawing * drawing = dynamic_cast<Drawing *>(obj.second)) {
            DrawingMapper::instance()->update(drawing->id());
        }
        _clean[obj.first] = obj.second;
    }
    for (auto obj : _deleted) {
        if(Painter * painter = dynamic_cast<Painter *>(obj.second)) {
            PainterMapper::instance()->del(painter->id());
        } else if(Drawing * drawing = dynamic_cast<Drawing *>(obj.second)) {
            DrawingMapper::instance()->del(drawing->id());
        }
        _clean[obj.first] = obj.second;
    }
    _new.clear();
    _dirty.clear();
    _deleted.clear();
}