// Created by Juan Francisco Marino on 2019-04-18.

#include <iostream>
#include "gtest/gtest.h"
#include "src/core/memory/Cell.h"
#include "src/core/memory/Allocator.h"
#include "../stubs/User.h"

using runtime::core::memory::Cell;
using runtime::core::memory::Allocator;

TEST(AllocatorTest, AllocatesAnObject) {
    auto alloc = new Allocator();

    auto john = alloc->allocate<User>();
    auto alice = alloc->allocate<User>();

    john->name = "John Doe";
    john->age = 20;
    alice->name = "Alice Foo";
    alice->age = 10;

    EXPECT_EQ(john->name, "John Doe");
    EXPECT_EQ(alice->name, "Alice Foo");
}

TEST(AllocatorTest, CollectsUnusedMemory) {
    auto alloc = new Allocator();
    auto john = alloc->allocate<User>();
    auto alice = alloc->allocate<User>();
    auto helen = alloc->allocate<User>();

    john->name = "John Doe";
    alice->name = "Alice Foo";
    helen->name = "Helen Bar";

    EXPECT_EQ(john->name, "John Doe");
    EXPECT_EQ(alice->name, "Alice Foo");
    EXPECT_EQ(helen->name, "Helen Bar");

    john->parent = alice;

    alloc->collect(std::list<Cell*> { john });


    EXPECT_EQ(john->name, "John Doe");
    EXPECT_EQ(alice->name, "Alice Foo");
    EXPECT_EQ(helen->name, "");                 // Helen should be zeroed out
}