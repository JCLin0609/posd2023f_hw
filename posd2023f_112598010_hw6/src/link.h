#pragma once

#include <list>
#include <string>
#include "node.h"

class Link : public Node{
private:
    Node * _target;

public:
    Link(string path, Node * target) : Node(path){
        this->setPath(path);
        _target = target;
    }

    int numberOfFiles() const override {
        return _target->numberOfFiles();
    }

    Node * find(string path) override {
        return _target->find(path);
    }

    std::list<string> findByName(string name) override {
        return _target->findByName(name);
    }

    void add(Node * node) override {
        _target->add(node);
    }

    void remove(string path) override {
        _target->remove(path);
    }

    void accept(Visitor * visitor) override {
        visitor->visitLink(this);
    }

    Node * getChildByName(const char * name) const override {
        return _target->getChildByName(name);
    }

    Node * getTarget(){
        return _target;
    }
};