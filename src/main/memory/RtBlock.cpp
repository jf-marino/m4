// Created by Juan Francisco Marino on 2019-04-16.

#include "RtCell.h"
#include "RtBlock.h"

RtBlock::RtBlock() {
    this->blob = this->block_allocator.allocate(CELL_SIZE * CELL_COUNT);
}

RtBlock::~RtBlock() {
    this->block_allocator.deallocate(this->blob, CELL_SIZE * CELL_COUNT);
}
