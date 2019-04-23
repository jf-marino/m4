// Created by Juan Francisco Marino on 2019-04-16.

#include "Cell.h"
#include "Block.h"

using runtime::core::memory::Block;

Block::Block() {
    this->blob = this->block_allocator.allocate(CELL_SIZE * CELL_COUNT);
}

Block::~Block() {
    this->block_allocator.deallocate(this->blob, CELL_SIZE * CELL_COUNT);
}
