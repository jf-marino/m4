// Created by Juan Francisco Marino on 2019-04-17.

#include <cstring>
#include "Cell.h"
#include "Block.h"
#include "Allocator.h"

using runtime::core::memory::CELL_SIZE;
using runtime::core::memory::Cell;
using runtime::core::memory::Allocator;

void clear_cell(Cell* cell) {
    if (cell == nullptr) return;
    cell->~Cell(); // Call destructor
    memset((void*)cell, 0, CELL_SIZE); // Zero out the contents of the cell
}

void Allocator::add_block() {
    auto new_block = new Block();
    auto begin = new_block->blob;
    for (unsigned int i = 0; i < CELL_COUNT; i++) {
        auto current = begin + (i * CELL_SIZE);
        this->free_cells.push_back(reinterpret_cast<Cell*>(current));
    }
    this->blocks.push_back(new_block);
}

Allocator::~Allocator() {
    for (auto it = this->blocks.begin() ; it != this->blocks.end() ; ++it) {
        (*it)->~Block();
    }
}

std::list<Cell *> Allocator::pinned() {
    std::list<Cell*> pinned = {};
    for (auto it = this->used_cells.begin(); it != this->used_cells.end(); ++it) {
        if ((*it)->pinned) {
            pinned.push_back(*it);
        }
    }
    return pinned;
}

void Allocator::mark(std::list<Cell*> roots) {
    auto pinned = this->pinned();

    roots.splice(roots.end(), pinned);

    if (roots.empty()) { return; }
    for (auto it = roots.begin(); it != roots.end(); ++it) {
        (*it)->marked = true;
        auto children = (*it)->references();
        this->mark(children);
    }
}

void Allocator::sweep() {
    std::list<Cell*> remove = {};
    std::list<Cell*> keep;
    for (auto it = this->used_cells.begin(); it != this->used_cells.end(); ++it) {
        if (!(*it)->marked) {
            clear_cell(*it);
            remove.push_back(*it);
        } else {
            // Reset the marked flag
            (*it)->marked = false;
            keep.push_back(*it);
        }
    }
    this->used_cells = keep;
    this->free_cells.splice(this->free_cells.end(), remove);
}

void Allocator::collect(std::list<Cell*> roots) {
    this->mark(roots);
    this->sweep();
}

bool Allocator::can_remove() {
    return this->pinned().empty();
}