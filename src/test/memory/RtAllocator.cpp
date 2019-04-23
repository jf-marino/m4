// Created by Juan Francisco Marino on 2019-04-18.

#include <iostream>
#include "gtest/gtest.h"
#include "src/core/memory/RtCell.h"
#include "src/core/memory/RtAllocator.h"

using runtime::core::memory::RtCell;
using runtime::core::memory::RtAllocator;

/**
 * Example class used to have something to allocate
 */
class User : public RtCell {
public:
    std::string name;
    unsigned int age;
    User* parent = nullptr;
public:
    User() = default;
    ~User() override = default;
public:
    std::list<RtCell*> references() override {
        std::list<RtCell*> refs;
        if (this->parent != nullptr) {
            refs.push_back(this->parent);
        }
        return refs;
    }
};



TEST(AllocatorTest, AllocateAnObject) {
    auto alloc = new RtAllocator();

    auto john = alloc->allocate<User>();
    auto alice = alloc->allocate<User>();

    john->name = "John Doe";
    john->age = 20;
    alice->name = "Alice Foo";
    alice->age = 10;

    EXPECT_EQ(john->name, "John Doe");
    EXPECT_EQ(alice->name, "Alice Foo");
}

TEST(AllocatorTest, CollectUnusedMemory) {
    auto alloc = new RtAllocator();
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

    alloc->collect(std::list<RtCell*> { john });


    EXPECT_EQ(john->name, "John Doe");
    EXPECT_EQ(alice->name, "Alice Foo");
    EXPECT_EQ(helen->name, "");                 // Helen should be zeroed out
}