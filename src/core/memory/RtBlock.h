// Created by Juan Francisco Marino on 2019-04-16.

#ifndef M4_RTBLOCK_H
#define M4_RTBLOCK_H

const unsigned int CELL_COUNT = 1024;

class RtBlock {
public:
    std::allocator<char> block_allocator;
    char* blob = nullptr;
public:
    RtBlock();
    ~RtBlock();
};


#endif //M4_RTBLOCK_H
