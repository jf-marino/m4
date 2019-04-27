// Created by Juan Francisco Marino on 2019-04-23.

#include <iostream>
#include <string>
#include <list>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "src/core/memory/Cell.h"
#include "src/core/memory/Allocator.h"
#include "src/core/objects/Avl.h"
#include "src/core/objects/Iterator.h"
#include "../stubs/User.h"

using testing::Le;
using testing::Ne;
using runtime::core::memory::CELL_SIZE;
using runtime::core::memory::Cell;
using runtime::core::memory::Allocator;
using runtime::core::objects::Avl;
using runtime::core::objects::Iterator;


Avl<int, User>* create(Allocator* allocator, std::list<std::string> names) {
    if (names.empty()) return nullptr;

    auto first = allocator->allocate<User>();
    first->name = names.front();

    int index = 2;
    auto node = Avl<int, User>::create(allocator, 1, first);
    for (auto it = (++names.begin()); it != names.end(); ++it, ++index) {
        auto user = allocator->allocate<User>();
        user->name = *it;
        node = node->set(allocator, index, user);
    }

    return node;
}

TEST(IteratorTest, MustNotExceedSizeLimits) {
    EXPECT_THAT(sizeof(Iterator<int, User>), Le(CELL_SIZE));
}

TEST(IteratorTest, CanIterateInOrder) {
    auto names = std::list<std::string> { "Jon", "Arya", "Dany" };
    auto allocator = new Allocator();
    auto avl = create(allocator, names);
    auto iter = Iterator<int, User>::create(allocator, avl);

    auto matches = 0;
    for (auto it = names.begin(); it != names.end(); ++it, ++matches) {
        auto node = iter->next();
        EXPECT_EQ(node->get_value()->name, *it);
    }

    EXPECT_EQ(matches, names.size());
}

TEST(IteratorTest, CompletesAfterIteratingThroughAllElements) {
    auto names = std::list<std::string> { "Jon", "Arya", "Dany" };
    auto allocator = new Allocator();
    auto avl = create(allocator, names);
    auto iter = Iterator<int, User>::create(allocator, avl);

    std::list<std::string> user_names {};
    while (!iter->done()) {
        auto user = iter->next()->get_value();
        user_names.push_back(user->name);
    }

    for (auto it_one = user_names.begin(), it_two = names.begin(); it_one != user_names.end() || it_two != names.end(); ++it_one, ++it_two) {
        EXPECT_EQ(*it_one, *it_two);
    }
}

