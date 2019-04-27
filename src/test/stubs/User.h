// Created by Juan Francisco Marino on 2019-04-27.

#ifndef M4_USER_H
#define M4_USER_H

#include <list>
#include <string>
#include "src/core/memory/Cell.h"

using runtime::core::memory::Cell;

/**
 * Example class used to have something to allocate
 */
class User : public Cell {
public:
    std::string name;
    unsigned int age;
    User* parent = nullptr;
public:
    User() = default;
    ~User() override = default;
public:
    std::list<Cell*> references() override {
        std::list<Cell*> refs;
        if (this->parent != nullptr) {
            refs.push_back(this->parent);
        }
        return refs;
    }
};

#endif //M4_USER_H
