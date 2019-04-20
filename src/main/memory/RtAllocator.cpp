// Created by Juan Francisco Marino on 2019-04-17.

#include <cstring>
#include "RtCell.h"
#include "RtBlock.h"
#include "RtAllocator.h"

void clear_cell(RtCell* cell) {
    if (cell == nullptr) return;
    cell->~RtCell(); // Call destructor
    memset((void*)cell, 0, CELL_SIZE); // Zero out the contents of the cell
}


RtAllocator::~RtAllocator() {
    for (auto it = this->used_cells.begin(); it != this->used_cells.end(); ++it) {
        clear_cell(*it);
    }

    for (auto it = this->blocks.begin(); it != this->blocks.end(); ++it) {
        (*it).~RtBlock();
    }
}

void RtAllocator::add_block() {
    auto new_block = RtBlock(&this->inner);
    auto begin = new_block.blob;
    for (unsigned int i = 0; i < CELL_COUNT; i++) {
        auto current = begin + (i * CELL_SIZE);
        this->free_cells.push_back(reinterpret_cast<RtCell*>(current));
    }
    this->blocks.push_back(new_block);
}

std::list<RtCell *> RtAllocator::pinned() {
    std::list<RtCell*> pinned = {};
    for (auto it = this->used_cells.begin(); it != this->used_cells.end(); ++it) {
        if ((*it)->pinned) {
            pinned.push_back(*it);
        }
    }
    return pinned;
}

void RtAllocator::mark(std::list<RtCell*> roots) {
    auto pinned = this->pinned();

    roots.splice(roots.end(), pinned);

    if (roots.empty()) { return; }
    for (auto it = roots.begin(); it != roots.end(); ++it) {
        (*it)->marked = true;
        auto children = (*it)->references();
        this->mark(children);
    }
}

void RtAllocator::sweep() {
    std::list<RtCell*> remove = {};
    std::list<RtCell*> keep;
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

void RtAllocator::collect(std::list<RtCell*> roots) {
    this->mark(roots);
    this->sweep();
}

bool RtAllocator::can_remove() {
    return this->pinned().empty();
}