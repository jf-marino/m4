// Created by Juan Francisco Marino on 2019-04-22.

#ifndef M4_AVL_H
#define M4_AVL_H

#include "../memory/Allocator.h"

using runtime::core::memory::Cell;
using runtime::core::memory::Allocator;


namespace runtime {
    namespace core {
        namespace objects {

            // Forward declaration of Iterator so we can use it as a friend
            template <typename K, class V>
            class Iterator;

            template <typename K, class V>
            class Avl : public Cell {
            private:
                K key;
                V* value;
                unsigned int lh;
                Avl<K, V>* left_node;
                unsigned int rh;
                Avl<K, V>* right_node;

                friend class Iterator<K, V>;

            private:
                Avl<K, V>* clone(Allocator* allocator) {
                    auto cloned = allocator->allocate<Avl<K, V>>();
                    cloned->key = this->key;
                    cloned->value = this->value;
                    cloned->lh = this->lh;
                    cloned->left_node = this->left_node;
                    cloned->rh = this->rh;
                    cloned->right_node = this->right_node;
                    return cloned;
                }

                void set_left(Avl* new_left) {
                    this->left_node = new_left;
                    this->lh = new_left != nullptr ? new_left->height() + 1 : 0;
                }

                void set_right(Avl* new_right) {
                    this->right_node = new_right;
                    this->rh = new_right != nullptr ? new_right->height() + 1 : 0;
                }

                Avl<K, V>* balance_left_left(Allocator* allocator) {
                    auto cloned = this->clone(allocator);
                    auto left_cloned = this->left_node->clone(allocator);
                    cloned->set_left(left_cloned->right_node);
                    left_cloned->set_right(cloned);
                    return left_cloned;
                }

                Avl<K, V>* balance_left_right(Allocator* allocator) {
                    auto cloned = this->clone(allocator);
                    auto left_cloned = this->left_node->clone(allocator);
                    auto left_right_cloned = this->left_node->right_node->clone(allocator);

                    cloned->set_left(left_right_cloned->right_node);
                    left_cloned->set_right(left_right_cloned->left_node);
                    left_right_cloned->set_left(left_cloned);
                    left_right_cloned->set_right(cloned);
                    return left_right_cloned;
                }

                Avl<K, V>* balance_right_left(Allocator* allocator) {
                    auto cloned = this->clone(allocator);
                    auto right_cloned = this->right_node->clone(allocator);
                    auto right_left_cloned = this->right_node->left_node->clone(allocator);

                    cloned->set_right(right_left_cloned->left_node);
                    right_cloned->set_left(right_left_cloned->right_node);
                    right_left_cloned->set_left(cloned);
                    right_left_cloned->set_right(right_cloned);
                    return right_left_cloned;
                }

                Avl<K, V>* balance_right_right(Allocator* allocator) {
                    auto cloned = this->clone(allocator);
                    auto right_cloned = this->right_node->clone(allocator);
                    cloned->set_right(right_cloned->left_node);
                    right_cloned->set_left(cloned);
                    return right_cloned;
                }

                Avl<K, V>* balance(Allocator* allocator) {
                    auto w = this->weight();

                    if (w < -1) {
                        auto child_weight = this->left_node != nullptr ? this->left_node->weight() : 0;
                        if (child_weight < 0)
                            return this->balance_left_left(allocator);
                        else if (child_weight > 0)
                            return this->balance_left_right(allocator);
                    }

                    if (w > 1) {
                        auto child_weight = this->right_node != nullptr ? this->right_node->weight() : 0;
                        if (child_weight > 0)
                            return this->balance_right_right(allocator);
                        else if (child_weight < 0)
                            return this->balance_right_left(allocator);
                    }

                    return this;
                }

                std::tuple<Avl<K, V>*, Avl<K, V>*> pop_smallest(Allocator* allocator) {
                    auto cloned = this->clone(allocator);
                    if (this->left_node != nullptr) {
                        auto result = this->left_node->pop_smallest(allocator);
                        cloned->set_left(std::get<1>(result));
                        return std::make_tuple(std::get<0>(result), cloned);
                    }
                    return std::make_tuple(cloned, nullptr);
                }

                std::tuple<Avl<K, V>*, Avl<K, V>*> pop_largest(Allocator* allocator) {
                    auto cloned = this->clone(allocator);
                    if (this->right_node != nullptr) {
                        auto result = this->right_node->pop_largest(allocator);
                        cloned->set_right(std::get<1>(result));
                        return std::make_tuple(std::get<0>(result), cloned);
                    }
                    return std::make_tuple(cloned, nullptr);
                }

                Avl<K, V>* swap_closest(Allocator* allocator) {
                    if (this->left_node != nullptr) {
                        auto cloned = this->clone(allocator);
                        auto result = this->left_node->pop_largest(allocator);
                        auto popped = std::get<0>(result);
                        auto new_left = std::get<1>(result);
                        cloned->key = popped->key;
                        cloned->value = popped->value;
                        cloned->set_left(new_left);
                        return cloned;
                    }

                    if (this->right_node != nullptr) {
                        auto cloned = this->clone(allocator);
                        auto result = this->right_node->pop_smallest(allocator);
                        auto popped = std::get<0>(result);
                        auto new_right = std::get<1>(result);
                        cloned->key = popped->key;
                        cloned->value = popped->value;
                        cloned->set_right(new_right);
                        return cloned;
                    }

                    return nullptr;
                }

            public:
                /*
                 * Helper method to give access to key and value fields outside of the scope of this file.
                 */
                static Avl<K, V>* create(Allocator* allocator, K key, V* value) {
                    auto node = allocator->allocate<Avl<K, V>>();
                    node->key = key;
                    node->value = value;
                    return node;
                }

            public:
                Avl() {
                    // Compile-time sanity check
                    static_assert(std::is_base_of<Cell, V>::value, "Value class is not derived from Cell");
                };

            public:
                K get_key() { return this->key; }
                V* get_value() { return this->value; }
                unsigned int height() { return this->rh > this->lh ? this->rh : this->lh; }
                int weight() { return this->rh - this->lh; }

                Avl<K, V>* set(Allocator* allocator, K key, V* value) {
                    auto cloned = this->clone(allocator);
                    if (key == this->key) {
                        cloned->key = key;
                        cloned->value = value;
                    }

                    if (key < this->key) {
                        if (this->left_node != nullptr) {
                            cloned->set_left(this->left_node->set(allocator, key, value));
                        } else {
                            auto node = allocator->allocate<Avl<K, V>>();
                            node->key = key;
                            node->value = value;
                            cloned->set_left(node);
                        }
                    }

                    if (key > this->key) {
                        if (this->right_node != nullptr) {
                            cloned->set_right(this->right_node->set(allocator, key, value));
                        } else {
                            auto node = allocator->allocate<Avl<K, V>>();
                            node->key = key;
                            node->value = value;
                            cloned->set_right(node);
                        }
                    }

                    return cloned->balance(allocator);
                }

                V* get(K key) {
                    if (key == this->key) {
                        return this->value;
                    }

                    if (key < this->key) {
                        return this->left_node != nullptr
                               ? this->left_node->get(key)
                               : nullptr;
                    }

                    if (key > this->key) {
                        return this->right_node != nullptr
                               ? this->right_node->get(key)
                               : nullptr;
                    }

                    return nullptr;
                }

                Avl<K, V>* remove(Allocator* allocator, K key) {
                    if (key == this->key) {
                        auto swapped = this->swap_closest(allocator);
                        return swapped;
                    }

                    if (key < this->key) {
                        if (this->left_node != nullptr) {
                            auto cloned = this->clone(allocator);
                            auto new_left = this->left_node->remove(allocator, key);
                            cloned->set_left(new_left);
                            return cloned;
                        }
                    }

                    if (key > this->key) {
                        if (this->right_node != nullptr) {
                            auto cloned = this->clone(allocator);
                            auto new_right = this->right_node->remove(allocator, key);
                            cloned->set_right(new_right);
                            return cloned;
                        }
                    }

                    return nullptr;
                }


                std::list<Cell*> references() override {
                    std::list<Cell*> refs = { this->value };
                    if (this->left_node != nullptr)
                        refs.push_back(this->left_node);
                    if (this->right_node != nullptr)
                        refs.push_back(this->right_node);

                    return refs;
                }
            };
        }
    }
}

#endif //M4_AVL_H
