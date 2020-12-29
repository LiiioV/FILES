//
// Created by ADMIN on 21.04.2020.
//


#ifndef UNTITLED6_FIXED_ALLOCATOR_H
#define UNTITLED6_FIXED_ALLOCATOR_H
#include <iostream>


template<unsigned int chunkSize>
class FixedAllocator {
    struct Block_;

    static const int startSize_ = 1024 * 512;

    Block_ *buffer_;

    size_t start_ = 0;

    char *startData_;

public:
    FixedAllocator();

    FixedAllocator(const FixedAllocator<chunkSize> &);

    ~FixedAllocator();

    FixedAllocator<chunkSize> &operator=(const FixedAllocator<chunkSize> &);

    void *alloc();

    template<typename T>
    void dealloc(T *);

private:
    void construct_();
};

template<unsigned int chunkSize>
struct FixedAllocator<chunkSize>::Block_ {
    size_t next;
    char data[chunkSize];
};

template<unsigned int chunkSize>
FixedAllocator<chunkSize>::FixedAllocator() {
    construct_();
}

template<unsigned int chunkSize>
FixedAllocator<chunkSize>::FixedAllocator(const FixedAllocator<chunkSize> &) {
    construct_();
}

template<unsigned int chunkSize>
FixedAllocator<chunkSize>::~FixedAllocator() {
    delete[] buffer_;
}

template<unsigned int chunkSize>
FixedAllocator<chunkSize> &FixedAllocator<chunkSize>::operator=(const FixedAllocator<chunkSize> &A) {
    if (&A != this) {
        delete[] buffer_;
        construct_();
    }
    return *this;
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
    char *bl = static_cast<char *>(static_cast<void *>(myBl));
    size_t newStart = (bl - startData_) / sizeof(Block_);
    buffer_[newStart].next = start_;
    start_ = newStart;
}

template<unsigned int chunkSize>
void FixedAllocator<chunkSize>::construct_() {
    buffer_ = new Block_[startSize_];
    startData_ = &buffer_[0].data[0];
    for (size_t i = 0; i < startSize_; ++i)
        buffer_[i].next = i + 1;
}


#endif //UNTITLED6_FIXED_ALLOCATOR_H


#ifndef UNTITLED6_FASTALLOCATOR_H
#define UNTITLED6_FASTALLOCATOR_H

template<typename T>
class FastAllocator {
public:
    FixedAllocator<4> allocFour_;
    FixedAllocator<12> allocTwelve_;

    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef const T *const_pointer;
    typedef const T &const_reference;

    template<typename U>
    struct rebind;

    FastAllocator() : allocFour_(FixedAllocator<4>()), allocTwelve_(FixedAllocator<12>()) {};

    FastAllocator(const FastAllocator<T> &A) : allocFour_(A.allocFour_),
                                              allocTwelve_(A.allocTwelve_) {};

    template<typename U>
    FastAllocator(const FastAllocator<U> &A) : allocFour_(A.allocFour_),
                                              allocTwelve_(A.allocTwelve_) {};

    ~FastAllocator() {};

    FastAllocator<T> &operator=(const FastAllocator<T> &);

    pointer allocate(size_t);

    void deallocate(pointer, size_t);

    void construct(pointer, const_reference);

    void destroy(pointer);
};

template<typename T>
template<typename U>
struct FastAllocator<T>::rebind {
    typedef FastAllocator<U> other;
};

template<typename T>
FastAllocator<T> &FastAllocator<T>::operator=(const FastAllocator<T> &A) {
    if (this != &A) {
        allocFour_ = A.allocFour_;
        allocTwelve_ = A.allocTwelve_;
    }
    return *this;
}

template<typename T>
typename FastAllocator<T>::pointer FastAllocator<T>::allocate(size_t n) {
    if (n * sizeof(value_type) == 4)
        return static_cast<pointer>(allocFour_.alloc());
    else if (n * sizeof(value_type) == 12)
        return static_cast<pointer>(allocTwelve_.alloc());
    return std::allocator<T>().allocate(n);
}


template<typename T>
void FastAllocator<T>::deallocate(pointer p, size_t n) {
    if (n * sizeof(value_type) == 4)
        return allocFour_.dealloc(p);
    else if (n * sizeof(value_type) == 12)
        return allocTwelve_.dealloc(p);
    return std::allocator<T>().deallocate(p, n);
}

template<typename T>
void FastAllocator<T>::construct(pointer p, const_reference val) {
    ::new(static_cast<void *>(p)) value_type(val);
}

template<typename T>
void FastAllocator<T>::destroy(pointer p) {
    p->~value_type();
}

template<typename T>
bool operator==(const FastAllocator<T> &, const FastAllocator<T> &) {
    return true;
}

template<typename T>
bool operator!=(const FastAllocator<T> &, const FastAllocator<T> &) {
    return false;
}

#endif //UNTITLED6_FASTALLOCATOR_H

#ifndef UNTITLED6_MY_LIST_H
#define UNTITLED6_MY_LIST_H

template<typename T, typename Allocator = std::allocator<T>>
class List {
public:
    class Node;

    typedef typename Allocator::template rebind<Node>::other _Node_Alloc_type;
private:
    size_t size_ = 0;

    typename Allocator::template rebind<Node>::other alloc_;

    Node *start_ = nullptr;

    Node *end_ = nullptr;
public:
    explicit List(const Allocator &alloc = Allocator()) : alloc_(_Node_Alloc_type(alloc)) {};

    List(size_t count, const T &val = T(), const Allocator &alloc = Allocator());

    List(const List<T, Allocator> &copy);

    ~List();

    List<T, Allocator> &operator=(const List<T, Allocator> &copy);

    size_t size() const;

    void push_back(const T &val);

    void push_front(const T &val);

    void pop_back();

    void pop_front();

    void insert_after(Node *N, const T &val);

    void insert_before(Node *N, const T &val);

    void erase(Node *N);

private:
    Node *build_(size_t, const T &);

    Node *copyBuild_(Node *);

    void findEnd_();

    void destroy_(Node *);

    void remove_();

    void insertAsStart_(const T &);
};

template<typename T, typename Allocator>
class List<T, Allocator>::Node {
public:
    T key_;
    Node *left_;
    Node *right_;

    Node() : key_(T()), left_(nullptr), right_(nullptr) {};

    Node(T val) : key_(val), left_(nullptr), right_(nullptr) {};
};

////public

template<typename T, typename Allocator>
List<T, Allocator>::List(size_t count, const T &val, const Allocator &alloc) {
    size_ = count;
    alloc_ = _Node_Alloc_type(alloc);
    start_ = build_(count, val);
    findEnd_();
}

template<typename T, typename Allocator>
List<T, Allocator>::List(const List<T, Allocator> &copy) {
    size_ = copy.size_;
    alloc_ = _Node_Alloc_type(copy.alloc_);
    start_ = copyBuild_(copy.start_);
    findEnd_();
}

template<typename T, typename Allocator>
List<T, Allocator>::~List() {
    remove_();
}

template<typename T, typename Allocator>
size_t List<T, Allocator>::size() const {
    return size_;
}

template<typename T, typename Allocator>
List<T, Allocator> &List<T, Allocator>::operator=(const List<T, Allocator> &copy) {
    if (this != &copy) {
        remove_();
        size_ = copy.size_;
        start_ = copyBuild_(copy.start_);
        findEnd_();
    }
    return *this;
}

template<typename T, typename Allocator>
void List<T, Allocator>::push_back(const T &val) {
    if (size_ == 0)
        insertAsStart_(val);
    else {
        insert_after(end_, val);
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::push_front(const T &val) {
    insertAsStart_(val);
}

template<typename T, typename Allocator>
void List<T, Allocator>::pop_back() {
    destroy_(end_);
}

template<typename T, typename Allocator>
void List<T, Allocator>::pop_front() {
    destroy_(start_);
}

template<typename T, typename Allocator>
void List<T, Allocator>::insert_after(Node *N, const T &val) {
    Node *newNode = build_(1, val);
    if (N->right_ != nullptr) {
        newNode->right_ = N->right_;
        N->right_->left_ = newNode;
    } else
        end_ = newNode;
    N->right_ = newNode;
    newNode->left_ = N;
    ++size_;
}

template<typename T, typename Allocator>
void List<T, Allocator>::insert_before(Node *N, const T &val) {
    if (N == start_)
        insertAsStart_(val);
    else
        insert_after(N->left_);
}

template<typename T, typename Allocator>
void List<T, Allocator>::erase(Node *N) {
    destroy_(N);
}


////private

template<typename T, typename Allocator>
typename List<T, Allocator>::Node *List<T, Allocator>::build_(size_t count, const T &val) {
    Node *result = alloc_.allocate(1);
    alloc_.construct(result, val);
    if (count > 1) {
        result->right_ = build_(count - 1, val);
        result->right_->left_ = result;
    }
    return result;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::Node *List<T, Allocator>::copyBuild_(Node *N) {
    if (N == nullptr)
        return nullptr;
    Node *result = alloc_.allocate(1);
    alloc_.construct(result, N->key_);
    if (N->right_ != nullptr) {
        result->right_ = copyBuild_(N->right_);
        result->right_->left_ = result;
    }
    return result;
}

template<typename T, typename Allocator>
void List<T, Allocator>::findEnd_() {
    end_ = start_;
    if (size_ != 0)
        while (end_->right_ != nullptr)
            end_ = end_->right_;
}

template<typename T, typename Allocator>
void List<T, Allocator>::destroy_(Node *N) {
    if (N->right_ != nullptr)
        N->right_->left_ = N->left_;
    else
        end_ = N->left_;
    if (N->left_ != nullptr)
        N->left_->right_ = N->right_;
    else
        start_ = N->right_;
    alloc_.destroy(N);
    alloc_.deallocate(N, 1);
    --size_;
}

template<typename T, typename Allocator>
void List<T, Allocator>::remove_() {
    while (start_ != nullptr)
        destroy_(start_);
}

template<typename T, typename Allocator>
void List<T, Allocator>::insertAsStart_(const T &val) {
    Node *newStart = build_(1, val);
    newStart->right_ = start_;
    if (start_ != nullptr)
        start_->left_ = newStart;
    else
        end_ = newStart;
    start_ = newStart;
    ++size_;
}


#endif //UNTITLED6_MY_LIST_H