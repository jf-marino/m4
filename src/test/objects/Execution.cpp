// Created by Juan Francisco Marino on 2019-04-27.

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "src/core/memory/Cell.h"
#include "src/core/memory/Allocator.h"
#include "src/core/objects/Execution.h"
#include "../stubs/User.h"

using testing::Le;
using testing::Ne;
using runtime::core::memory::CELL_SIZE;
using runtime::core::memory::Allocator;
using runtime::core::objects::Execution;

TEST(ExecutionTest, MustNotExceedSizeLimits) {
    EXPECT_THAT(sizeof(Execution), Le(CELL_SIZE));
}

TEST(ExecutionTest, CanCreate) {
    auto allocator = new Allocator();
    auto exec = Execution::create(allocator, nullptr);
    EXPECT_THAT(exec, Ne(nullptr));
}

TEST(ExecutionTest, CanBindVariables) {
    auto allocator = new Allocator();
    auto exec = Execution::create(allocator, nullptr);

    auto jon = allocator->allocate<User>();
    jon->name = "Jon Snow";
    jon->age = 29;

    exec->set("Jon", jon);

    auto retrieved = exec->get<User>("Jon");
    EXPECT_EQ(retrieved->name, "Jon Snow");
}

TEST(ExecutionTest, CanUpdateVariables) {
    auto allocator = new Allocator();
    auto exec = Execution::create(allocator, nullptr);

    auto jon = allocator->allocate<User>();
    jon->name = "Jon Snow";
    jon->age = 29;

    exec->set("Jon", jon);

    auto retrieved = exec->get<User>("Jon");
    EXPECT_EQ(retrieved->name, "Jon Snow");

    auto aegon = allocator->allocate<User>();
    aegon->name = "Aegon Targaryen";
    aegon->age = 29;

    exec->update("Jon", aegon);

    auto second_retrieved = exec->get<User>("Jon");
    EXPECT_EQ(second_retrieved->name, "Aegon Targaryen");
}