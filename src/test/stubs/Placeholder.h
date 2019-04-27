// Created by Juan Francisco Marino on 2019-04-27.

#ifndef M4_PLACEHOLDER_H
#define M4_PLACEHOLDER_H

#include <list>
#include <string>
#include "src/core/memory/Cell.h"

using runtime::core::memory::Cell;

/*
 * Example class used as a placeholder inside AVL node tests. This class should try to be close
 * to the size limit of CELL_SIZE (defined in Cell.h) so as to mimic normal runtime conditions
 * as close as possible.
 */
class Placeholder : public Cell {
public:
    std::string field_one;
    int age;
public:
    Placeholder() = default;
    std::list<Cell*> references() override {
        return std::list<Cell*> {};
    }
};

#endif //M4_PLACEHOLDER_H
