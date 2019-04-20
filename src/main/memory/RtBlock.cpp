// Created by Juan Francisco Marino on 2019-04-16.

#include "RtCell.h"
#include "RtBlock.h"

RtBlock::RtBlock(std::allocator<char>* alloc) {
    this->alloc = alloc;
    this->blob = this->alloc->allocate(CELL_SIZE * CELL_COUNT);
}

RtBlock::~RtBlock() {
    this->alloc->deallocate(this->blob, CELL_SIZE * CELL_COUNT);
}
