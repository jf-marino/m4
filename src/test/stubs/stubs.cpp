// Created by Juan Francisco Marino on 2019-04-27.

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "src/core/memory/Cell.h"
#include "User.h"
#include "Placeholder.h"

using testing::Le;
using runtime::core::memory::CELL_SIZE;

TEST(StubTests, UserMustNotExceedSizeLimits) {
    EXPECT_THAT(sizeof(User), Le(CELL_SIZE));
}

TEST(StubTests, PlaceholderMustNotExceedSizeLimits) {
    EXPECT_THAT(sizeof(Placeholder), Le(CELL_SIZE));
}
