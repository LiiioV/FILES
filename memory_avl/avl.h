//
// Created by ADMIN on 24.03.2020.
//

#ifndef MEMORY_AVL_AVL_H
#define MEMORY_AVL_AVL_H
#include "DLL.h"
#include <queue>

class avl{
private:
    struct node_;

    node_* root_;

    void makeRoot_(std::pair<int, std::pair<int, Node *>>);

    void insert_(std::pair<int, std::pair<int, Node *>>, node_ *);

    void deleteKey_(std::pair<int, std::pair<int, Node *>>, node_ *, node_ *);

    void deleteKeyPartTwo_(node_ *, node_ *);

    std::pair<int, std::pair<int, Node *>> *findKey_(unsigned int, unsigned int, node_ *) const;

    void remove_(node_ *);

    void toHeight_(node_ *);

    int height_(node_ *);

    void balance_(node_ *);

    void smallLeftRotation_(node_ *);

    void smallRightRotation_(node_ *);

public:
    avl() : root_(nullptr) {};

    ~avl();

    void push(std::pair<int, std::pair<int, Node *>>);

    void pop(std::pair<int, std::pair<int, Node *>>);

    bool empty();

    std::pair<int, std::pair<int, Node *>> *find(unsigned int) const;

    std::pair<int, std::pair<int, Node *>> *findMax() const;
};

struct avl::node_ {
    node_(int a, int b, Node *N) : key({ a, { b, N } }), left(nullptr), right(nullptr), leftChild(0), rightChild(0), height(1) {};

    node_(std::pair<int, std::pair<int, Node *>> R) : key(R), left(nullptr), right(nullptr), leftChild(0), rightChild(0), height(1) {};

    std::pair<int, std::pair<int, Node *>> key;
    node_ *left;
    node_ *right;
    int leftChild;
    int rightChild;
    int height;
};

template<typename T>
void Swap(T& a, T& b) {
    T c = a;
    a = b;
    b = c;
}

void avl::makeRoot_(std::pair<int, std::pair<int, Node *>> R) {
    root_ = new node_(R);
}

void avl::insert_(std::pair<int, std::pair<int, Node *>> R, avl::node_* N) {
    if (R < N->key) {
        N->leftChild++;
        if (N->left == nullptr)
            N->left = new node_(R);

        else
            insert_(R, N->left);
    }
    else {
        N->rightChild++;
        if (N->right == nullptr)
            N->right = new node_(R);
        else
            insert_(R, N->right);
    }
    balance_(N);
}


void avl::deleteKeyPartTwo_(node_ *N, node_ *Fath = nullptr) {
    if (N->left == nullptr && N->right == nullptr) {
        if (Fath != nullptr) {
            if (Fath->left == N)
                Fath->left = nullptr;
            else
                Fath->right = nullptr;
            balance_(Fath);
        }
        else
            root_ = nullptr;
        delete N;
    }
    else if (N->left == nullptr) {
        N->rightChild--;
        std::queue<node_*> q;
        q.push(N);
        node_ *father = N;
        node_ *where = N->right;
        while (where->left != nullptr) {
            q.push(where);
            father = where;
            father->leftChild--;
            where = where->left;
        }
        N->key = where->key;
        deleteKeyPartTwo_(where, father);
        while (!q.empty()) {
            balance_(q.front());
            q.pop();
        }
    }
    else {
        N->leftChild--;
        std::queue<node_*> q;
        q.push(N);
        node_ *father = N;
        node_ *where = N->left;
        while (where->right != nullptr) {
            q.push(where);
            father = where;
            father->rightChild--;
            where = where->right;
        }
        N->key = where->key;
        deleteKeyPartTwo_(where, father);
        while (!q.empty()) {
            balance_(q.front());
            q.pop();
        }
    }
}

void avl::deleteKey_(std::pair<int, std::pair<int, Node *>> D, node_ *N, node_ *Fath = nullptr) {
    if (D == N->key) {
        deleteKeyPartTwo_(N, Fath);
    }
    else if (D < N->key) {
        N->leftChild--;
        deleteKey_(D, N->left, N);
        balance_(N);
    }
    else {
        N->rightChild--;
        deleteKey_(D, N->right, N);
        balance_(N);
    }
}

std::pair<int, std::pair<int, Node *>> *avl::findKey_(unsigned int i, unsigned int amount, node_ *N) const {
    if (i == amount + N->rightChild + 1)
        return &(N->key);
    else if (i < amount + N->rightChild + 1)
        return findKey_(i, amount, N->right);
    else
        return findKey_(i, amount + N->rightChild + 1, N->left);
}

void avl::remove_(node_ *A) {
    if (A == nullptr)
        return;
    if (A->left != nullptr)
        remove_(A->left);
    if (A->right != nullptr)
        remove_(A->right);
    delete A;
}

void avl::toHeight_(avl::node_ *A) {
    if (A->left == nullptr && A->right == nullptr)
        A->height = 1;
    else if (A->right == nullptr)
        A->height = A->left->height + 1;
    else if (A->left == nullptr)
        A->height = A->right->height + 1;
    else
        A->height = (A->right->height > A->left->height ? A->right->height : A->left->height) + 1;
}

int avl::height_(avl::node_ *N) {
    if (N == nullptr)
        return 0;
    return N->height;
}

void avl::balance_(avl::node_ *N) {
    if (height_(N->right) - height_(N->left) >= 2) {
        if (height_(N->right->right) >= height_(N->right->left))
            smallLeftRotation_(N);
        else {
            smallRightRotation_(N->right);
            smallLeftRotation_(N);
        }
    }
    if (height_(N->right) - height_(N->left) <= -2) {
        if (height_(N->left->left) >= height_(N->left->right))
            smallRightRotation_(N);
        else {
            smallLeftRotation_(N->left);
            smallRightRotation_(N);
        }
    }
    toHeight_(N);
}

void avl::smallLeftRotation_(avl::node_ *N) {
    Swap(N->key, N->right->key);
    Swap(N->right->left, N->left);
    Swap(N->right->right, N->left);
    Swap(N->right, N->left);
    N->rightChild = N->left->rightChild;
    N->left->rightChild = N->left->leftChild;
    N->left->leftChild = N->leftChild;
    N->leftChild = 1 + N->left->leftChild + N->left->rightChild;
    toHeight_(N->left);
}

void avl::smallRightRotation_(avl::node_ *N) {
    Swap(N->key, N->left->key);
    Swap(N->left->right, N->right);
    Swap(N->left->left, N->right);
    Swap(N->right, N->left);
    N->leftChild = N->right->leftChild;
    N->right->leftChild = N->right->rightChild;
    N->right->rightChild = N->rightChild;
    N->rightChild = 1 + N->right->rightChild + N->right->leftChild;
    toHeight_(N->right);
}

avl::~avl() {
    remove_(root_);
}

void avl::push(std::pair<int, std::pair<int, Node *>> a) {
    if (root_ == nullptr)
        makeRoot_(a);
    else
        insert_(a, root_);
}

void avl::pop(std::pair<int, std::pair<int, Node *>> a) {
    deleteKey_(a, root_);
}

bool avl::empty() {
    return root_ == nullptr;
}

std::pair<int, std::pair<int, Node *>> *avl::find(unsigned int i) const {
    return findKey_(i, 0, root_);
}

std::pair<int, std::pair<int, Node *>> *avl::findMax() const {
    node_ *r = root_;
    while (r->right != nullptr)
        r = r->right;
    return &(r->key);
}

#endif //MEMORY_AVL_AVL_H