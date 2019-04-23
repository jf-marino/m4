// Created by Juan Francisco Marino on 2019-04-16.

#ifndef M4_BLOCK_H
#define M4_BLOCK_H

namespace runtime {
    namespace core {
        namespace memory {
            const unsigned int CELL_COUNT = 1024;

            class Block {
            public:
                std::allocator<char> block_allocator;
                char* blob = nullptr;
            public:
                Block();
                ~Block();
            };
        }
    }
}


#endif //M4_BLOCK_H
