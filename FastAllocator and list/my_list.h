//
// Created by ADMIN on 21.04.2020.
//

//#ifndef UNTITLED6_MY_LIST_H
//#define UNTITLED6_MY_LIST_H

template<typename T, typename Allocator = std::allocator<T>>
class List {
public:
    class Node;

private:
    size_t size_;

    typename Allocator::template rebind<Node>::other *alloc_;

    Node *start_;

    Node *end_;
public:
    explicit List(const Allocator &alloc = Allocator()) :
            size_(0), alloc_(new Allocator::rebind<Node>::other(alloc)), start_(nullptr), end_(nullptr) {};

    List(size_t count, const T &val = T(), const Allocator &alloc = Allocator());

    List(const List<T, Allocator> &copy);

    ~List();

    size_t size();

    List<T, Allocator> &operator=(const List<T, Allocator> &copy);

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

    void remove_(Node *);

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
List<T, Allocator>::List(size_t count, const T &val, const Allocator &alloc) :
        size_(count), alloc_(new Allocator::rebind<Node>(alloc)) {
    start_ = build_(count, val);
    findEnd_();
}

template<typename T, typename Allocator>
List<T, Allocator>::List(const List<T, Allocator> &copy) :
        size_(copy.size_), alloc_(new Allocator::rebind<Node>(copy.alloc_)) {
    start_ = copyBuild_();
    findEnd_();
}

template<typename T, typename Allocator>
List<T, Allocator>::~List() {
    remove_(start_);
}

template <typename T, typename Allocator>
size_t List<T, Allocator>::size() {
    return size_;
}

template<typename T, typename Allocator>
List<T, Allocator> &List<T, Allocator>::operator=(const List<T, Allocator> &copy) {
    remove_(start_);
    start_ = copyBuild_(copy.start_);
    end_ = findEnd_();
    return this;
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
    destroy(end_);
}

template<typename T, typename Allocator>
void List<T, Allocator>::pop_front() {
    destroy(start_)
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
    destroy_(Node *N);
}

////private

template<typename T, typename Allocator>
typename List<T, Allocator>::Node *List<T, Allocator>::build_(size_t count, const T &val) {
    Node *result = alloc_->allocate(1);
    if (count > 1) {
        result->right_ = build_(count - 1, val);
        result->right_->left_ = result;
    }
    alloc_->construct(result, val);
    return result;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::Node *List<T, Allocator>::copyBuild_(Node *N) {
    if(N == nullptr)
        return nullptr;
    Node *result = alloc_->allocate(1);
    alloc_->construct(result, N->key_);
    if (N->right_ != nullptr) {
        result->right_ = copyBuild_(N->right_);
        result->right_->left_ = result_;
    }
    return result;
}

template<typename T, typename Allocator>
void List<T, Allocator>::findEnd_() {
    end_ = start_;
    if(count != 0)
        while(end_->right_ != nullptr)
            end_ = end_->right_;
}

template<typename T, typename Allocator>
void List<T, Allocator>::destroy_(Node *N) {
    if(N->right_ != nullptr)
        N->right_->left_ = N->left_;
    else
        end_ = N->left_;
    if(N->left_ != nullptr)
        N->left_->right_ = N->right_;
    else
        start_ = N->right_;
    alloc_->destroy(N);
    alloc_->deallocate(N, 1);
    --size_;
}

template<typename T, typename Allocator>
void List<T, Allocator>::remove_(Node *N) {
    while(start_ != nullptr)
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

//#endif //UNTITLED6_MY_LIST_H
