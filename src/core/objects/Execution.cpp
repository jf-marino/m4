// Created by Juan Francisco Marino on 2019-04-23.

#include <functional>
#include "Iterator.h"
#include "Execution.h"

using runtime::core::objects::Execution;
using runtime::core::objects::Iterator;
using runtime::core::objects::KeyValue;

std::list<Cell*> Execution::references() {
    auto refs = std::list<Cell*> {};
    auto iter = Iterator<std::size_t, KeyValue<std::string, Cell>>::create(this->allocator, this->variables);
    while (!iter->done()) {
        auto kv = iter->next()->get_value();
        auto ref = kv->value;
        refs.push_back(ref);
    }
    return refs;
}

void Execution::set(std::string name, Cell* value) {
    auto hashed = this->hasher(name);
    auto kv = KeyValue<std::string, Cell>::create(this->allocator, name, value);

    if (this->variables == nullptr)
        this->variables = Avl<std::size_t, KeyValue<std::string, Cell>>::create(this->allocator, hashed, kv);
    else
        this->variables = this->variables->set(this->allocator, hashed, kv);
}

bool Execution::update_local(std::size_t hashed, std::string name, Cell *value) {
    if (this->variables == nullptr)
        return false;

    auto node = this->variables->get(hashed);

    if (node != nullptr) {
        auto kv = KeyValue<std::string, Cell>::create(this->allocator, name, value);
        this->variables = this->variables->set(this->allocator, hashed, kv);
        return true;
    }

    return false;
}

bool Execution::update_parent(std::size_t hashed, std::string name, Cell *value) {
    if (this->parent == nullptr)
        return false;

    return this->parent->update_by_hash(hashed, name, value);
}

bool Execution::update_by_hash(std::size_t hashed, std::string name, Cell *value) {
    auto updated = this->update_local(hashed, name, value);
    return !updated
        ? this->update_parent(hashed, name, value)
        : updated;
}

void Execution::update(std::string name, Cell* value) {
    auto hashed = this->hasher(name);
    this->update_by_hash(hashed, name, value);
}
