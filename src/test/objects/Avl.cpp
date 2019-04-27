// Created by Juan Francisco Marino on 2019-04-22.

#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "src/core/memory/Cell.h"
#include "src/core/memory/Allocator.h"
#include "src/core/objects/Avl.h"
#include "../stubs/Placeholder.h"

using testing::Le;
using testing::Ne;
using runtime::core::memory::CELL_SIZE;
using runtime::core::memory::Cell;
using runtime::core::memory::Allocator;
using runtime::core::objects::Avl;



Placeholder* h(Allocator* alloc, std::string one, int age) {
    auto ph = alloc->allocate<Placeholder>();
    ph->field_one = one;
    ph->age = age;
    return ph;
}


TEST(AvlTest, MustNotExceedSizeLimits) {
    EXPECT_THAT(sizeof(Avl<int, Placeholder>), Le(CELL_SIZE));
}

TEST(AvlTest, ShouldCreate) {
    auto allocator = new Allocator();
    auto node = allocator->allocate<Avl<int, Placeholder>>();
    EXPECT_THAT(node, Ne(nullptr));
}

TEST(AvlTest, CanBalanceItself) {
    auto allocator = new Allocator();
    auto node = Avl<int, Placeholder>::create(allocator, 1, h(allocator, "Jon", 29));

    node = node->set(allocator, 2, h(allocator, "Danaerys", 30));
    node = node->set(allocator, 3, h(allocator, "Arya", 17));

    auto arya   = node->get(3);
    auto jon    = node->get(1);
    auto dany   = node->get(2);

    EXPECT_EQ(arya->field_one, "Arya");
    EXPECT_EQ(jon->field_one, "Jon");
    EXPECT_EQ(dany->field_one, "Danaerys");

    EXPECT_EQ(node->height(), 1);
}

TEST(AvlTest, CanRemoveElements) {
    auto allocator = new Allocator();
    auto node = Avl<int, Placeholder>::create(allocator, 1, h(allocator, "Jon", 29));
    node = node->set(allocator, 2, h(allocator, "Danaerys", 30));
    node = node->set(allocator, 3, h(allocator, "Arya", 17));

    node = node->remove(allocator, 2);

    auto arya   = node->get(3);
    auto jon    = node->get(1);
    auto dany   = node->get(2);

    EXPECT_EQ(arya->field_one, "Arya");
    EXPECT_EQ(jon->field_one, "Jon");
    EXPECT_EQ(dany, nullptr);
}

TEST(AvlTest, TestGarbageCollection) {
    auto allocator = new Allocator();
    auto node_one = Avl<int, Placeholder>::create(allocator, 1, h(allocator, "Jon", 29));
    auto node_two = node_one->set(allocator, 2, h(allocator, "Danaerys", 30));
    auto node_three = node_two->set(allocator, 3, h(allocator, "Arya", 17));

    EXPECT_THAT(node_two->get(1), Ne(nullptr));
    EXPECT_THAT(node_two->get(2), Ne(nullptr));

    allocator->collect(std::list<Cell*> { node_three });

    auto arya   = node_three->get(3);
    auto jon    = node_three->get(1);
    auto dany   = node_three->get(2);

    EXPECT_EQ(arya->field_one, "Arya");
    EXPECT_EQ(jon->field_one, "Jon");
    EXPECT_EQ(dany->field_one, "Danaerys");

    EXPECT_EQ(node_two->get(1), nullptr);
    EXPECT_EQ(node_two->get(2), nullptr);
}
