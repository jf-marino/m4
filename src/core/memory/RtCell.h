// Created by Juan Francisco Marino on 2019-04-16.

#ifndef M4_RTOBJECT_H
#define M4_RTOBJECT_H

#include <list>

namespace runtime {
    namespace core {
        namespace memory {
            const unsigned int CELL_SIZE = 64;

            class RtCell {
            public:
                bool pinned : 1;
                bool marked : 1;
            public:
                virtual ~RtCell() = default;
            public:
                virtual std::list<RtCell*> references() = 0;
            };
        }
    }
}


#endif //M4_RTOBJECT_H
