// Created by Juan Francisco Marino on 2019-04-19.

#ifndef M4_THREAD_H
#define M4_THREAD_H

#include <thread>
#include "../memory/Allocator.h"
#include "../memory/Cell.h"

using runtime::core::memory::Cell;
using runtime::core::memory::Allocator;

namespace runtime {
    namespace core {
        namespace threading {
            class Thread : public Cell {
            private:
                threading::Thread*    parent = nullptr;
                Allocator*            allocator = new Allocator();
                std::thread             inner;

            public:
                Thread() = default;
                Thread(Thread* parent);
                ~Thread();

            public:
                void run();
                void join();

            };
        }
    }
}


#endif //M4_THREAD_H
