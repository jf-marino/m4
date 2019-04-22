// Created by Juan Francisco Marino on 2019-04-16.

#ifndef M4_RTALLOCATOR_H
#define M4_RTALLOCATOR_H

#include <memory>
#include <list>
#include "RtCell.h"
#include "RtBlock.h"

class RtAllocator {
protected:
    std::list<RtBlock> blocks;
    std::list<RtCell*> free_cells;
    std::list<RtCell*> used_cells;
public:
    RtAllocator() = default;
    ~RtAllocator();

protected:
    void add_block();
    void mark(std::list<RtCell*> roots);
    void sweep();
    std::list<RtCell*> pinned();

public:
    void collect(std::list<RtCell*> roots);
    bool can_remove();

    template <class T>
    T* allocate() {
        if (this->free_cells.empty()) { this->add_block(); }
        RtCell* cell = this->free_cells.front();
        T* obj = new (cell) T();
        this->free_cells.pop_front();
        this->used_cells.push_back(obj);
        return obj;
    }
};


#endif //M4_RTALLOCATOR_H
