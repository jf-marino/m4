// Created by Juan Francisco Marino on 2019-04-16.

#ifndef M4_CELL_H
#define M4_CELL_H

#include <list>

namespace runtime {
    namespace core {
        namespace memory {
            const unsigned int CELL_SIZE = 64;

            class Cell {
            public:
                bool pinned : 1;
                bool marked : 1;
            public:
                virtual ~Cell() = default;
            public:
                virtual std::list<Cell*> references() = 0;
            };
        }
    }
}


#endif //M4_CELL_H
