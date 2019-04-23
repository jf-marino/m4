// Created by Juan Francisco Marino on 2019-04-23.

#ifndef M4_KEYVALUE_H
#define M4_KEYVALUE_H

#include "../memory/Cell.h"
#include "../memory/Allocator.h"

using runtime::core::memory::Cell;
using runtime::core::memory::Allocator;

namespace runtime {
    namespace core {
        namespace objects {

            template <typename K, class V>
            class KeyValue : public Cell {
            public:
                K key;
                V* value;

            public:
                KeyValue() {
                    // Compile-time sanity check
                    static_assert(std::is_base_of<Cell, V>::value, "Value class is not derived from Cell");
                }

                ~KeyValue() = default;

            public:
                static KeyValue<K, V>* create(Allocator* allocator, K key, V* value) {
                    auto kv = allocator->allocate<KeyValue<K, V>>();
                    kv->key = key;
                    kv->value = value;
                    return kv;
                }

            public:
                std::list<Cell*> references() override {
                    auto refs = std::list<Cell*> {};
                    if (this->value)
                        refs.push_back(this->value);
                    return refs;
                }
            };
        }
    }
}


#endif //M4_KEYVALUE_H
