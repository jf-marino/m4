// Created by Juan Francisco Marino on 2019-04-19.

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "src/core/memory/Cell.h"

using testing::Le;
using runtime::core::memory::CELL_SIZE;
using runtime::core::memory::Cell;

TEST(CellTest, ShouldNotExceedSizeLimits) {
    EXPECT_THAT(sizeof(Cell), Le(CELL_SIZE));
}
