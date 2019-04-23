// Created by Juan Francisco Marino on 2019-04-23.

#ifndef M4_EXECUTION_H
#define M4_EXECUTION_H

#include <list>
#include <string>
#include <functional>
#include "KeyValue.h"
#include "Avl.h"

using runtime::core::memory::Cell;
using runtime::core::memory::Allocator;
using runtime::core::objects::Avl;
using runtime::core::objects::Iterator;

namespace runtime {
    namespace core {
        namespace objects {
            class Execution : public Cell {
            private:
                std::hash<std::string> hasher;
                Allocator* allocator;
                Execution* parent;
                Avl<std::size_t, KeyValue<std::string, Cell>>* variables;

            public:
                static Execution* create(Allocator* allocator, Execution* parent) {
                    auto ex = allocator->allocate<Execution>();
                    ex->allocator = allocator;
                    ex->parent = parent;
                    return ex;
                }

            public:
                Execution() = default;
                ~Execution() = default;

            public:
                void set(std::string name, Cell* value);
                void update(std::string name, Cell* value);

                template <class T>
                T* get(std::string name) {
                    if (this->variables == nullptr)
                        return nullptr;

                    auto hashed = this->hasher(name);
                    auto kv = this->variables->get(hashed);

                    if (kv == nullptr)
                        return nullptr;

                    return reinterpret_cast<T*>(kv->value);
                }

            public:
                std::list<Cell*> references() override;
            };
        }
    }
}


#endif //M4_EXECUTION_H
