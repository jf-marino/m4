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

    auto jon = User::create(allocator, "Jon Snow", 29);

    exec->set("Jon", jon);

    auto retrieved = exec->get<User>("Jon");
    EXPECT_EQ(retrieved->name, "Jon Snow");
}

TEST(ExecutionTest, CanUpdateVariables) {
    auto allocator = new Allocator();
    auto exec = Execution::create(allocator, nullptr);

    auto jon = User::create(allocator, "Jon Snow", 29);

    exec->set("Jon", jon);

    auto retrieved = exec->get<User>("Jon");
    EXPECT_EQ(retrieved->name, "Jon Snow");

    auto aegon = User::create(allocator, "Aegon Targaryen", 29);

    exec->update("Jon", aegon);

    auto second_retrieved = exec->get<User>("Jon");
    EXPECT_EQ(second_retrieved->name, "Aegon Targaryen");
}

TEST(ExecutionTest, CanLookupVariablesInParentExecutions) {
    auto allocator = new Allocator();
    auto parent = Execution::create(allocator, nullptr);

    auto jon = User::create(allocator, "Jon Snow", 29);

    parent->set("Jon", jon);

    auto child = Execution::create(allocator, parent);
    auto retrieved = child->get<User>("Jon");

    EXPECT_EQ(retrieved->name, "Jon Snow");
}

TEST(ExecutionTest, CanUpdateVariablesInParentExecutions) {
    auto allocator = new Allocator();
    auto parent = Execution::create(allocator, nullptr);

    auto jon = User::create(allocator, "Jon Snow", 29);

    parent->set("Jon", jon);

    auto child = Execution::create(allocator, parent);
    auto aegon = User::create(allocator, "Aegon Targaryen", 29);

    child->update("Jon", aegon);

    auto retrieved = parent->get<User>("Jon");
    EXPECT_EQ(retrieved->name, "Aegon Targaryen");
}

TEST(ExecutionTest, CanShadowVariablesWithSet) {
    auto allocator = new Allocator();
    auto parent = Execution::create(allocator, nullptr);
    auto child = Execution::create(allocator, parent);
    auto jon = User::create(allocator, "Jon Snow", 29);

    parent->set("Jon", jon);

    auto arya = User::create(allocator, "Arya Stark", 17);
    child->set("Jon", arya);

    auto from_parent = parent->get<User>("Jon");
    auto from_child = child->get<User>("Jon");
    EXPECT_EQ(from_parent->name, "Jon Snow");
    EXPECT_EQ(from_child->name, "Arya Stark");
}
