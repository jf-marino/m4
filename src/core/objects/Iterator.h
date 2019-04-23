// Created by Juan Francisco Marino on 2019-04-23.

#ifndef M4_ITERATOR_H
#define M4_ITERATOR_H

#include <stack>
#include "Avl.h"

using runtime::core::memory::Cell;
using runtime::core::memory::Allocator;
using runtime::core::objects::Avl;

namespace runtime {
    namespace core {
        namespace objects {

            template <typename K, class V>
            class Iterator : public Cell {
            private:
                bool started : 1;
                std::stack<Avl<K, V>*>* st;
                Avl<K, V>* root;

            public:
                static Iterator<K, V>* create(Allocator* allocator, Avl<K, V>* root) {
                    auto iter = allocator->allocate<Iterator<K, V>>();
                    iter->root = root;
                    return iter;
                }

            public:
                Iterator() { this->st = new std::stack<Avl<K, V>*>(); };
                ~Iterator() { delete this->st; };

            private:
                void traverse(Avl<K, V>* node) {
                    auto n = node;
                    while (n != nullptr) {
                        this->st->push(n);
                        n = n->left_node;
                    }
                }

            public:
                bool done() {
                    return  this->started && this->st->empty();
                }

                Avl<K, V>* next() {
                    if (!this->started)
                        this->traverse(this->root);

                    this->started = true;

                    if (this->st->empty())
                        return nullptr;

                    auto node = this->st->top();
                    this->st->pop();

                    if (node->right_node != nullptr) {
                        this->traverse(node->right_node);
                    }

                    return node;
                }

                std::list<Cell*> references() override {
                    auto list = std::list<Cell*> {};
                    if (this->root != nullptr)
                        list.push_back(this->root);
                    return list;
                }
            };

        }
    }
}

#endif //M4_ITERATOR_H
