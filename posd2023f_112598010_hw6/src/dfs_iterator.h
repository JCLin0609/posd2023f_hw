#pragma once

#include <queue>
#include <stack>
#include "iterator.h"

class DfsIterator: public Iterator {
public:
    DfsIterator(Node* composite) : _root(composite) {}

    ~DfsIterator() {
        while(!_stack.empty()){
            delete _stack.top();
            _stack.pop();
        }
    }

    void first() {
        while(!_stack.empty()){
            _stack.pop();
        }

        _curr = _root;
        _pushCurrIter();
        next();
    }

    Node * currentItem() const {
        return _curr;
    }

    void next() {
        while(!_stack.empty() && _stack.top()->isDone()){
            delete _stack.top();
            _stack.pop();
        }
        if(_stack.empty()){
            return;
        }
        if(!_stack.top()->isDone()){
            _curr = _stack.top()->currentItem();
            _stack.top()->next();
            _pushCurrIter();
        }
    }
    
    bool isDone() const {
        return _stack.empty();
    }
private:
    Node * _root;
    Node * _curr;
    std::stack<Iterator *> _stack;

    void _pushCurrIter() {
        Iterator * it = _curr->createIterator();
        it->first();
        _stack.push(it);
    }
};

class BfsIterator: public Iterator {
public:
    BfsIterator(Node* composite) : _root(composite) {}

    ~BfsIterator() {
        
    }

    void first(){
        if(!_nextLevel.empty()){
            while(!_nextLevel.empty()){
                delete _nextLevel.front();
                _nextLevel.pop_front();
            }
            _nextLevel.clear();
        }
            
        _curr = _root;
        _nextLevel.push_back(_curr);
        next();
    }

    Node * currentItem() const {
        return _curr;
    }

    void next() {
        Iterator * it = _curr->createIterator();
        for(it->first(); !it->isDone(); it->next()){
            _nextLevel.push_back(it->currentItem());
        }
        
        _nextLevel.pop_front();
        _curr = _nextLevel.front();
        delete it;
        return;
    }
    
    bool isDone() const {
        return _nextLevel.empty();
    }
private:
    Node * _root;
    Node * _curr;
    std::list<Node *> _nextLevel;
};

