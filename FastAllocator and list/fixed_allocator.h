//
// Created by ADMIN on 21.04.2020.
//

#ifndef UNTITLED6_FIXED_ALLOCATOR_H
#define UNTITLED6_FIXED_ALLOCATOR_H
#include <iostream>

template<unsigned int chunkSize>
class FixedAllocator {
public:
    class Block_;

    static const int startSize_ = 1024 * 1024;

    Block_ *buffer_;

    size_t start_;

    char *startData_;
public:
    FixedAllocator();

    ~FixedAllocator();

    void *alloc();

    template<typename T>
    void dealloc(T *);

 };

template<unsigned int chunkSize>
struct FixedAllocator<chunkSize>::Block_ {
    size_t next;

    char data[chunkSize];
};

template<unsigned int chunkSize>
FixedAllocator<chunkSize>::FixedAllocator() :
buffer_(new Block_[startSize_]), start_(0), startData_(&buffer_[0].data[0]) {
    for (size_t i = 0; i < startSize_; ++i)
        buffer_[i].next = i + 1;
}

template<unsigned int chunkSize>
FixedAllocator<chunkSize>::~FixedAllocator() {
    delete[] buffer_;
}

template<unsigned int chunkSize>
void *FixedAllocator<chunkSize>::alloc() {
    void *ans = static_cast<void *>(&buffer_[start_].data[0]);
    start_ = buffer_[start_].next;
    return ans;
}


template<unsigned int chunkSize>
template<typename T>
void FixedAllocator<chunkSize>::dealloc(T *myBl) {
    char * bl = static_cast<char *>(static_cast<void *>(myBl));
    size_t newStart = (bl - startData_)/ sizeof(Block_);
    buffer_[newStart].next = start_;
    start_ = newStart;
}

#endif //UNTITLED6_FIXED_ALLOCATOR_H
