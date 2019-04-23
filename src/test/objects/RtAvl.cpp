// Created by Juan Francisco Marino on 2019-04-22.

#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "src/core/memory/RtCell.h"
#include "src/core/memory/RtAllocator.h"
#include "src/core/objects/RtAvl.h"

using testing::Le;
using testing::Ne;

/*
 * Example class used as a placeholder inside AVL node tests. This class should try to be close
 * to the size limit of CELL_SIZE (defined in RtCell.h) so as to mimic normal runtime conditions
 * as close as possible.
 */
class Placeholder : public RtCell {
public:
    std::string field_one;
    std::string field_two;
public:
    Placeholder() = default;
    std::list<RtCell*> references() override {
        return std::list<RtCell*> {};
    }
};


Placeholder* h(RtAllocator* alloc, std::string one, std::string two) {
    auto ph = alloc->allocate<Placeholder>();
    ph->field_one = one;
    ph->field_two = two;
    return ph;
}


TEST(AvlTest, ShouldNotExceedSizeLimits) {
    EXPECT_THAT(sizeof(Placeholder), Le(CELL_SIZE));
    EXPECT_THAT(sizeof(RtAvl<int, Placeholder>), Le(CELL_SIZE));
}

TEST(AvlTest, ShouldCreate) {
    auto allocator = new RtAllocator();
    auto node = allocator->allocate<RtAvl<int, Placeholder>>();
    EXPECT_THAT(node, Ne(nullptr));
}

TEST(AvlTest, CanBalanceItself) {
    auto allocator = new RtAllocator();
    auto node = RtAvl<int, Placeholder>::create(allocator, 1, h(allocator, "Jon", "Snow"));

    node = node->set(allocator, 2, h(allocator, "Danaerys", "Targaryen"));
    node = node->set(allocator, 3, h(allocator, "Arya", "Stark"));

    auto arya   = node->get(3);
    auto jon    = node->get(1);
    auto dany   = node->get(2);

    EXPECT_EQ(arya->field_one, "Arya");
    EXPECT_EQ(jon->field_one, "Jon");
    EXPECT_EQ(dany->field_one, "Danaerys");

    EXPECT_EQ(node->height(), 1);
}

TEST(AvlTest, CanRemoveElements) {
    auto allocator = new RtAllocator();
    auto node = RtAvl<int, Placeholder>::create(allocator, 1, h(allocator, "Jon", "Snow"));
    node = node->set(allocator, 2, h(allocator, "Danaerys", "Targaryen"));
    node = node->set(allocator, 3, h(allocator, "Arya", "Stark"));

    node = node->remove(allocator, 2);

    auto arya   = node->get(3);
    auto jon    = node->get(1);
    auto dany   = node->get(2);

    EXPECT_EQ(arya->field_one, "Arya");
    EXPECT_EQ(jon->field_one, "Jon");
    EXPECT_EQ(dany, nullptr);
}

TEST(AvlTest, TestGarbageCollection) {
    auto allocator = new RtAllocator();
    auto node_one = RtAvl<int, Placeholder>::create(allocator, 1, h(allocator, "Jon", "Snow"));
    auto node_two = node_one->set(allocator, 2, h(allocator, "Danaerys", "Targaryen"));
    auto node_three = node_two->set(allocator, 3, h(allocator, "Arya", "Stark"));

    EXPECT_THAT(node_two->get(1), Ne(nullptr));
    EXPECT_THAT(node_two->get(2), Ne(nullptr));

    allocator->collect(std::list<RtCell*> { node_three });

    auto arya   = node_three->get(3);
    auto jon    = node_three->get(1);
    auto dany   = node_three->get(2);

    EXPECT_EQ(arya->field_one, "Arya");
    EXPECT_EQ(jon->field_one, "Jon");
    EXPECT_EQ(dany->field_one, "Danaerys");

    EXPECT_EQ(node_two->get(1), nullptr);
    EXPECT_EQ(node_two->get(2), nullptr);
}
