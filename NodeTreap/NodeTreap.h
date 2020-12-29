#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <cassert>

//
// Created by ADMIN on 23.04.2020.
//

#ifndef NODETREAP_NODETREAP_H
#define NODETREAP_NODETREAP_H

template<typename A, typename B>
struct Pair {
    Pair(A a, B b) : first(a), second(b) {};

    A first;
    B second;
};

template<typename T>
void toSwap(T &a, T &b) {
    T c = a;
    a = b;
    b = c;
}

int myAbs(int t) {
    return t >= 0 ? t : -t;
}

int min(int a, int b) {
    return (a > b ? b : a);
}

int max(int a, int b) {
    return (a > b ? a : b);
}

class NodeTreap {
    bool circle_ = false;
    bool rev_ = false;
    int num_;
    int key_ = 0;
    int size_ = 1;
    NodeTreap *left_ = nullptr;
    NodeTreap *right_ = nullptr;
    NodeTreap *parent_ = nullptr;

public:
    int getSize();

    NodeTreap(int val) : num_(val) {};

    ~NodeTreap() {};

    static void connect(NodeTreap *, NodeTreap *);

    static int getDistance(NodeTreap *, NodeTreap *);

    static void disConnect(NodeTreap *, NodeTreap *);

private:
    static NodeTreap *getLeft_(NodeTreap *);

    static NodeTreap *getRight_(NodeTreap *);

    NodeTreap *merge_(NodeTreap *N);

    static void transplant_(NodeTreap *, NodeTreap *);

    static void leftRotate_(NodeTreap *);

    static void rightRotate_(NodeTreap *);

    static void splay_(NodeTreap *);

    void updateSize_();

    void updateRev_();

    NodeTreap *getRoot_();

    int getPosition_();
};

////////////////////////
////                ////
////  MY_FUNCTIONS  ////
////                ////
////////////////////////


int getSizeNULL(NodeTreap *N) {
    return N == nullptr ? 0 : N->getSize();
}

///////////////////////////
////                   ////
////  CLASS_FUNCTIONS  ////
////                   ////
///////////////////////////


void NodeTreap::updateSize_() {
    size_ = 1 + getSizeNULL(left_) + getSizeNULL(right_);
}

void NodeTreap::updateRev_() {
    if (rev_) {
        rev_ = false;
        NodeTreap *cur = left_;
        left_ = right_;
        right_ = cur;
        if (left_ != nullptr)
            left_->rev_ ^= true;
        if (right_ != nullptr)
            right_->rev_ ^= true;
    }
}

void NodeTreap::transplant_(NodeTreap *N, NodeTreap *Child) {
    if (N->parent_ == nullptr)
        toSwap(N->circle_, Child->circle_);
    else if (N == N->parent_->left_)
        N->parent_->left_ = Child;
    else if (N == N->parent_->right_)
        N->parent_->right_ = Child;
    Child->parent_ = N->parent_;
}

void NodeTreap::leftRotate_(NodeTreap *N) {
    NodeTreap *rightChild = N->right_;
    //if (N->rev_ || N->right_->rev_)
        //assert(false);

    N->right_ = rightChild->left_;
    if (rightChild->left_ != nullptr)
        rightChild->left_->parent_ = N;

    transplant_(N, rightChild);

    rightChild->left_ = N;
    rightChild->left_->parent_ = rightChild;
    N->updateSize_();
    rightChild->updateSize_();
}

void NodeTreap::rightRotate_(NodeTreap *N) {
    NodeTreap *leftChild = N->left_;
    //if (N->rev_ || N->left_->rev_)
        //assert(false);

    N->left_ = leftChild->right_;
    if (leftChild->right_ != nullptr)
        leftChild->right_->parent_ = N;

    transplant_(N, leftChild);

    leftChild->right_ = N;
    leftChild->right_->parent_ = leftChild;
    N->updateSize_();
    leftChild->updateSize_();
}

void NodeTreap::splay_(NodeTreap *N) {
    if (N == nullptr)
        return;

    while (N->parent_ != nullptr) {
        // Zig
        if (N->parent_->parent_ == nullptr) {
            if (N == N->parent_->left_)
                rightRotate_(N->parent_);
            else if (N == N->parent_->right_) {
                leftRotate_(N->parent_);
            }
        }
        else {
            // Zig-Zig
            if (N == N->parent_->left_ &&
                N->parent_ == N->parent_->parent_->left_) {

                rightRotate_(N->parent_->parent_);
                rightRotate_(N->parent_);

            }
            else if (N == N->parent_->right_ &&
                     N->parent_ == N->parent_->parent_->right_) {

                leftRotate_(N->parent_->parent_);
                leftRotate_(N->parent_);
            }
                // Zig-Zag
            else if (N == N->parent_->right_ &&
                     N->parent_ == N->parent_->parent_->left_) {

                leftRotate_(N->parent_);
                rightRotate_(N->parent_);

            }
            else if (N == N->parent_->left_ &&
                     N->parent_ == N->parent_->parent_->right_) {

                rightRotate_(N->parent_);
                leftRotate_(N->parent_);
            }
        }
    }
}

////public

int NodeTreap::getSize() {
    return size_;
}

NodeTreap *NodeTreap::getLeft_(NodeTreap *N) {
    if (N == nullptr)
        return nullptr;
    N->updateRev_();
    while (N->left_ != nullptr)
        N = N->left_;
    return N;
}

NodeTreap *NodeTreap::getRight_(NodeTreap *N) {
    if (N == nullptr)
        return nullptr;
    N->updateRev_();
    while (N->right_ != nullptr)
        N = N->right_;
    return N;
}

NodeTreap *NodeTreap::merge_(NodeTreap *N) {
    NodeTreap *root = getRight_(this);
    splay_(root);
    assert((root != nullptr));
    assert((root->parent_ == nullptr));
    assert((root->right_ == nullptr));
    assert((N->getRoot_() != this));
    N->getPosition_();
    root->right_ = N->getRoot_();
    if (root->right_ != nullptr)
        root->right_->parent_ = root;
    root->updateSize_();
    return root;
}


NodeTreap *NodeTreap::getRoot_() {
    if (parent_ == nullptr) {
        updateRev_();
        return this;
    }
    NodeTreap *result = parent_->getRoot_();
    updateRev_();
    return result;
}

int NodeTreap::getPosition_() {
    getRoot_();
    splay_(this);
    return getSizeNULL(left_);
}

void NodeTreap::connect(NodeTreap *N1, NodeTreap *N2) {
    int pos2 = N2->getPosition_();
    int pos1 = N1->getPosition_();
    if (N2->getRoot_() == N1) {
        N1->circle_ = true;
        return;
    }
    if (pos1 == 0 && pos2 == N2->size_ - 1) {
        N2->merge_(N1);
    } else if (pos1 == 0 && pos2 == 0) {
        N2->rev_ = true;
        N2->merge_(N1);
    } else if (pos1 == N1->size_ - 1 && pos2 == 0) {
        N1->merge_(N2);
    } else if (pos1 == N1->size_ - 1 && pos2 == N2->size_ - 1) {
        N2->rev_ = true;
        N1->merge_(N2);
    } else
        assert(false);
}

void NodeTreap::disConnect(NodeTreap *N1, NodeTreap *N2) {
    int pos2 = N1->getPosition_();
    int pos1 = N1->getPosition_();
    assert((N1 == N2->getRoot_()));
    NodeTreap *left = getLeft_(N1);
    NodeTreap *right = getRight_(N1);

    bool mod = false;
    if (N1->circle_) {
        N1->circle_ = false;
        mod = true;
    }
    if (getRight_(N1->left_) == N2) {
        N1->left_->parent_ = nullptr;
        N1->left_ = nullptr;
        N1->updateSize_();
    } else if (getLeft_(N1->right_) == N2) {
        N1->right_->parent_ = nullptr;
        N1->right_ = nullptr;
        N1->updateSize_();
    } else if (pos1 == 0 && pos2 == N1->size_ - 1 && mod)
        return;
    else if (pos2 == 0 && pos1 == N1->size_ - 1 && mod)
        return;
    else
        assert(false);
    if (mod)
        right->merge_(left);
}

int NodeTreap::getDistance(NodeTreap *N1, NodeTreap *N2) {
    int pos2 = N2->getPosition_();
    int pos1 = N1->getPosition_();
    if (N2->getRoot_() != N1)
        return -1;
    return N1->circle_ ? min(N1->size_ - myAbs(pos1 - pos2) - 1, myAbs(pos1 - pos2) - 1) : myAbs(pos1 - pos2) - 1;
}


#endif //NODETREAP_NODETREAP_H
