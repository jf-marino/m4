// Created by Juan Francisco Marino on 2019-04-16.

#ifndef M4_RTBLOCK_H
#define M4_RTBLOCK_H

const unsigned int CELL_COUNT = 1024;

class RtBlock {
public:
    char* blob = nullptr;
public:
    RtBlock(std::allocator<char> alloc);
    ~RtBlock(std::allocator<char> alloc);
};


#endif //M4_RTBLOCK_H
