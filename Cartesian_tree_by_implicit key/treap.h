//
// Created by ADMIN on 30.03.2020.
//

#ifndef CARTESIAN_TREE_BY_IMPLICIT_KEY_TREAP_H
#define CARTESIAN_TREE_BY_IMPLICIT_KEY_TREAP_H

template<typename T1,typename T2>
struct Pair;


class treap {
private:
    struct node_;

    node_ *root_;

    int size_(node_ *);

    void construct_(int);

    void grow_(node_ *, int);

    void toCode_(node_ *, int);

    void remove_(node_ *);

    Pair<treap *, bool> spliter_(int);

    Pair<node_ *, node_ *> split_(node_ *, int, int);

    node_* merge_(node_ *, node_ *);

    void print_(node_ *, int &);

public:
    treap(int);

    treap(node_ *N) : root_(N) {};

    treap() : root_(nullptr) {};

    ~treap();

    treap *split(int);

    treap &merge(treap &);

    void swap(treap &);

    void toPrint(int);
};

template<typename T1,typename T2>
struct Pair {
    Pair<T1, T2>(const T1 &a,const T2 &b) : first(a), second(b) {};
    T1 first;
    T2 second;
};

struct treap::node_ {
    node_(int a) : key(a), size(1), left(nullptr), right(nullptr) {};

    node_() : key(0), size(1), left(nullptr), right(nullptr) {};

    int key;
    int size;
    node_ *left;
    node_ *right;
};

int logn(int n) {
    int h = 0;
    while(n != 0) {
        n /= 2;
        ++h;
    }
    return h;
};

int treap::size_(treap::node_ *N) {
    if (N == nullptr)
        return 0;
    else
        return N->size;
}

void treap::construct_(int n) {
    int h = logn(n);
    root_ = new node_();
    grow_(root_, h - 1);
    toCode_(root_, 0);
}

void treap::grow_(treap::node_ *N, int h) {
    if (h == 0)
        return;
    N->left = new node_();
    N->right = new node_();
    grow_(N->left, h - 1);
    grow_(N->right, h - 1);
}

void treap::toCode_(treap::node_ *N, int herit) {
    if (N->left != nullptr && N->right != nullptr) {
        toCode_(N->left, herit);
        N->key = N->left->size + herit;
        toCode_(N->right, N->key + 1);
        N->size = N->left->size + N->right->size + 1;
    } else if (N->left != nullptr) {
        toCode_(N->left, herit);
        N->key = N->left->size;
        N->size = N->left->size + 1;
    } else if (N->right != nullptr) {
        N->key = herit;
        toCode_(N->right, herit + 1);
        N->size = N->right->size + 1;
    } else {
        N->key = herit;
        N->size = 1;
    }
}

void treap::swap(treap &A) {
    node_ * t = root_;
    root_ = A.root_;
    A.root_ = t;
}

void treap::remove_(treap::node_ *N) {
    if(N == nullptr)
        return;
    remove_(N->right);
    remove_(N->left);
    delete N;
}

Pair<treap*, bool> treap::spliter_(int key) {
    Pair<node_ *, node_ *> x(split_(root_, key, 0));
    if (x.first == root_)
        return Pair<treap *, bool>(new treap(x.second), false);
    else
        return Pair<treap*, bool>(new treap(x.first), true);
}

Pair<treap::node_*, treap::node_*> treap::split_(treap::node_ *N, int key, int herit) {
    if (N == nullptr) {
        Pair<node_*, node_*> a(nullptr, nullptr);
        return a;
    }
    if (N->size - size_(N->right) + herit <= key) {
        Pair<node_*, node_*> rec = split_(N->right, key, N->size - size_(N->right) + herit);
        N->right = rec.first;
        N->size = size_(N->left) + size_(N->right) + 1;
        return Pair<node_*, node_*>(N, rec.second);
    }
    else {
        Pair<node_*, node_*> rec = split_(N->left, key, herit);
        N->left = rec.second;
        N->size = size_(N->left) + size_(N->right) + 1;
        return Pair<node_*, node_*>(rec.first, N);
    }
}

treap::node_* treap::merge_(node_ *our, node_ *other) {
    if (our == nullptr && other == nullptr)
        return nullptr;
    if(our == nullptr)
        return other;
    if(other == nullptr)
        return our;
    if (our->size >= other->size) {
        our->right = merge_(our->right, other);
        our->size = size_(our->right) + size_(our->left) + 1;
        return our;
    }
    other->left = merge_(our, other->left);
    other->size = size_(other->right) + size_(other->left) + 1;
    return other;
}

void treap::print_(node_ *N, int& t) {
    if(N == nullptr || t==0)
        return;
    print_(N->left, t);
    if (t == 0)
        return;
    std::cout<<N->key + 1<<" ";
    t--;
    print_(N->right, t);
}

treap::treap(int n) {
    construct_(n);
}

treap::~treap() {
    remove_(root_);
}

treap* treap::split(int k) {
    Pair<treap*, bool> t = spliter_(k);
    if(t.second)
        swap(*t.first);
    return t.first;
}

treap &treap::merge(treap &A) {
    node_ * other = A.root_;
    A.root_ = nullptr;
    root_ = merge_(root_, other);
    return *this;
}

void treap::toPrint(int t) {
    print_(root_, t);
}
#endif //CARTESIAN_TREE_BY_IMPLICIT_KEY_TREAP_H
