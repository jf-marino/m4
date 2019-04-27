// Created by Juan Francisco Marino on 2019-04-27.

#ifndef M4_USER_H
#define M4_USER_H

#include <list>
#include <string>
#include "src/core/memory/Cell.h"
#include "src/core/memory/Allocator.h"

using runtime::core::memory::Cell;
using runtime::core::memory::Allocator;

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
    static User* create(Allocator* allocator, std::string name, int age) {
        auto user = allocator->allocate<User>();
        user->name = name;
        user->age = age;
        return user;
    }
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
