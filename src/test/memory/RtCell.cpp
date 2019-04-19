// Created by Juan Francisco Marino on 2019-04-19.

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "src/main/memory/RtCell.h"

using testing::Le;

TEST(CellTest, ShouldNotExceedSizeLimits) {
    EXPECT_THAT(sizeof(RtCell), Le(CELL_SIZE));
}
