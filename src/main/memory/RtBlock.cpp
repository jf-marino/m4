// Created by Juan Francisco Marino on 2019-04-16.

#include "RtCell.h"
#include "RtBlock.h"

RtBlock::RtBlock(std::allocator<char> alloc) {
    this->blob = alloc.allocate(CELL_SIZE * CELL_COUNT);
}

RtBlock::~RtBlock(std::allocator<char> alloc) {
    alloc.deallocate(this->blob, CELL_SIZE * CELL_COUNT);
}
