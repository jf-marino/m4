// Created by Juan Francisco Marino on 2019-04-16.

#ifndef M4_ALLOCATOR_H
#define M4_ALLOCATOR_H

#include <memory>
#include <list>
#include "Cell.h"
#include "Block.h"

namespace runtime {
    namespace core {
        namespace memory {

            class Allocator {
            protected:
                std::list<Block*> blocks;
                std::list<Cell*> free_cells;
                std::list<Cell*> used_cells;
            public:
                Allocator() = default;
                ~Allocator();

            protected:
                void add_block();
                void mark(std::list<Cell*> roots);
                void sweep();
                std::list<Cell*> pinned();

            public:
                void collect(std::list<Cell*> roots);
                bool can_remove();

                template <class T>
                T* allocate() {
                    if (this->free_cells.empty()) { this->add_block(); }
                    Cell* cell = this->free_cells.front();
                    T* obj = new (cell) T();
                    this->free_cells.pop_front();
                    this->used_cells.push_back(obj);
                    return obj;
                }
            };

        }
    }
}


#endif //M4_ALLOCATOR_H
