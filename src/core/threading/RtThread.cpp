// Created by Juan Francisco Marino on 2019-04-19.

#include "RtThread.h"

RtThread::RtThread(RtThread* parent) {
    this->parent = parent;
}

// TODO: Must determine what happens if the allocator can't be destroyed because some of it's memory is
//       still being used. Will the parent thread be made the owner? Will that memory be moved? etc.
RtThread::~RtThread() {

}

void RtThread::run() {

}

void RtThread::join() {

}
