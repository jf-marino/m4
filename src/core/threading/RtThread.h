// Created by Juan Francisco Marino on 2019-04-19.

#ifndef M4_RTTHREAD_H
#define M4_RTTHREAD_H

#include <thread>
#include "../memory/RtAllocator.h"
#include "../memory/RtCell.h"

using runtime::core::memory::RtCell;
using runtime::core::memory::RtAllocator;

namespace runtime {
    namespace core {
        namespace threading {
            class RtThread : public RtCell {
            private:
                threading::RtThread*    parent = nullptr;
                RtAllocator*            allocator = new RtAllocator();
                std::thread             inner;

            public:
                RtThread() = default;
                RtThread(RtThread* parent);
                ~RtThread();

            public:
                void run();
                void join();

            };
        }
    }
}


#endif //M4_RTTHREAD_H
