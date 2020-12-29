//
// Created by ADMIN on 13.04.2020.
//

#ifndef MADNESS_MADNESS_H
#define MADNESS_MADNESS_H

const long long INFINIT = 1e10;


template<typename T1, typename T2>
struct Pair {
    Pair<T1, T2>(const T1 &a, const T2 &b) : first(a), second(b) {};
    T1 first;
    T2 second;
};

template<typename T>
void toSwap(T &a, T &b) {
    T c = a;
    a = b;
    b = c;
}

long long biggestGradeOfTwo(long long n) {
    long long h = 1;
    while (h < n)
        h *= 2;
    return h / 2;
};

template<typename T>
const T& max(const T &a, const T &b) {
    return a > b ? a : b;
}

template<typename T>
const T& min(const T &a, const T &b) {
    return a > b ? b : a;
}


////MY_CLASS////

class MagicTreap {
    struct Node_;

    long long priority_;

    Node_* root_;
public:
    MagicTreap() : priority_(0), root_(nullptr) {};

    MagicTreap(Node_ *N) : priority_(0), root_(N) {};

    MagicTreap(long long size, long long* array) : priority_(0), root_(construct_(size, array)) {};

    ~MagicTreap();

    void setAdd(long long val, long long l, long long r);

    void setEqual(long long val, long long l, long long r);

    long long sum(long long l, long long r);

    void swap(MagicTreap &);

    MagicTreap *split(long long);

    MagicTreap &merge(MagicTreap &);

    void insertElement(long long val, long long pos);

    void deleteElement(long long);

    void reverse(long long l, long long r);

    void nextPermutation(long long l, long long r);

    void prevPermutation(long long l, long long r);

    void toPrint();

private:
    Node_* construct_(long long, long long *);

    static void remove_(Node_ *);

    static long long getSize_(Node_ *);

    static long long getLeft_(Node_ *, long long);

    static long long getRight_(Node_ *, long long);

    static bool getFirstOrder_(Node_ *);

    static bool getSecondOrder_(Node_ *);

    static long long getSortCount_(Node_ *, long long, long long);

    static long long getSum_(Node_ *);

    static bool getOrder_(Node_ *, long long);

    static bool compareShape_(long long, long long, long long);

    static void updateNodeValues_(Node_ *);

    static void updateNode_(Node_ *);

    static void setReversedNode_(Node_ *);

    static void setAddNode_(Node_ *, long long);

    static void setEqualNode_(Node_ *, long long);

    static void setAddSegment_(Node_ *, long long, long long, long long);

    static void setEqualSegment_(Node_ *, long long, long long, long long);

    long long getSumSegment_(Node_ *, long long, long long);

    Pair<MagicTreap *, bool> splitter_(long long);

    Pair<Node_ *, Node_ *> split_(Node_ *, long long, long long);

    Node_* merge_(Node_ *, Node_ *);

    static long long upperBound_(Node_ *, long long, long long);

    void permutateShape_(long long, long long, long long);

    void permutate_(long long);

    static long long findPositionSorting_(Node_ *, long long);

    static void print_(Node_ *);

    void nextPriority_();
};

struct MagicTreap::Node_ {
    bool reversed_;
    long long value_;
    long long size_;
    long long equal_;
    long long add_;
    long long sum_;
    long long leftValue_;
    long long rightValue_;
    long long pri_;
    long long firstSortCountStart_;
    long long firstSortCountEnd_;
    long long secondSortCountStart_;
    long long secondSortCountEnd_;
    Node_ *left_;
    Node_ *right_;

    Node_(long long val, long long pri) : reversed_(false), value_(val), size_(1),
                                          equal_(INFINIT), add_(0), sum_(val), leftValue_(val), rightValue_(val), pri_(pri),
                                          firstSortCountStart_(1), firstSortCountEnd_(1), secondSortCountStart_(1),
                                          secondSortCountEnd_(1), left_(nullptr), right_(nullptr) {};

};

////public

MagicTreap::~MagicTreap() {
    remove_(root_);
}

void MagicTreap::setAdd(long long val, long long l, long long r) {
    setAddSegment_(root_, val, l, r);
}

void MagicTreap::setEqual(long long val, long long l, long long r) {
    setEqualSegment_(root_, val, l, r);
}

long long MagicTreap::sum(long long l, long long r) {
    return getSumSegment_(root_, l, r);
}

void MagicTreap::swap(MagicTreap &A) {
    toSwap(root_, A.root_);
}

MagicTreap *MagicTreap::split(long long k) {
    Pair<MagicTreap*, bool> t = splitter_(k);
    if (t.second)
        swap(*t.first);
    return t.first;
}

MagicTreap &MagicTreap::merge(MagicTreap &A) {
    Node_ * other = A.root_;
    A.root_ = nullptr;
    root_ = merge_(root_, other);
    return *this;
}

void MagicTreap::reverse(long long l, long long r) {
    MagicTreap *firstMagic = split(l);
    MagicTreap *reversedMagic = split(r - l + 1);
    setReversedNode_(reversedMagic->root_);
    firstMagic->merge(*reversedMagic);
    firstMagic->merge(*this);
    swap(*firstMagic);
    delete reversedMagic;
    delete firstMagic;
}

void MagicTreap::insertElement(long long val, long long pos) {
    nextPriority_();
    Node_ *newNode = new Node_(val, priority_);
    MagicTreap *newMagic = new MagicTreap(newNode);
    MagicTreap *firstMagic = split(pos);
    firstMagic->merge(*newMagic);
    firstMagic->merge(*this);
    swap(*firstMagic);
    delete newMagic;
    delete firstMagic;
}

void MagicTreap::deleteElement(long long pos) {
    MagicTreap *firstMagic = split(pos);
    MagicTreap *deleted = split(1);
    firstMagic->merge(*this);
    swap(*firstMagic);
    delete firstMagic;
    delete deleted;
}

void MagicTreap::nextPermutation(long long l, long long r) {
    permutateShape_(l, r, 0);
}

void MagicTreap::prevPermutation(long long l, long long r) {
    permutateShape_(l, r, 1);
}

void MagicTreap::toPrint() {
    print_(root_);
}

////private

MagicTreap::Node_ *MagicTreap::construct_(long long size, long long *array) {
    if (size <= 0)
        return nullptr;
    if (size == 1) {
        nextPriority_();
        return new Node_(array[0], priority_);
    }
    long long gradeTwo = biggestGradeOfTwo(size);
    nextPriority_();
    Node_ *newNode = new Node_(array[gradeTwo - 1], priority_);
    newNode->left_ = construct_(gradeTwo - 1, array);
    newNode->right_ = construct_(size - gradeTwo, array + gradeTwo);
    updateNodeValues_(newNode);
    return newNode;
}


void MagicTreap::remove_(Node_ *N) {
    if (N == nullptr)
        return;
    remove_(N->left_);
    remove_(N->right_);
    delete N;
}

long long MagicTreap::getSize_(Node_ *N) {
    return N == nullptr ? 0 : N->size_;
}

long long MagicTreap::getSum_(Node_ *N) {
    return N == nullptr ? 0 : N->sum_;
}

long long MagicTreap::getLeft_(Node_ *N, long long defaultT) {
    return N == nullptr ? defaultT : N->leftValue_;
}

long long MagicTreap::getRight_(Node_ *N, long long defaultT) {
    return N == nullptr ? defaultT : N->rightValue_;
}

bool MagicTreap::getFirstOrder_(Node_ *N) {
    return N == nullptr ? true : N->size_ == N->firstSortCountStart_;
}

bool MagicTreap::getSecondOrder_(Node_ *N) {
    return N == nullptr ? true : N->size_ == N->secondSortCountStart_;
}

bool MagicTreap::getOrder_(Node_ *N, long long shape) {
    if (N == nullptr)
        return true;
    return shape == 0 ? getFirstOrder_(N) : getSecondOrder_(N);
}

long long MagicTreap::getSortCount_(Node_ *N, long long shape, long long where) {
    return N == nullptr ? 0 : (shape == 0 ? (where == 0 ? N->firstSortCountStart_ : N->firstSortCountEnd_) :
                               (where == 0 ? N->secondSortCountStart_ : N->secondSortCountEnd_));
}

bool MagicTreap::compareShape_(long long l, long long r, long long shape) {
    return shape == 0 ? l >= r : l <= r;
}

void MagicTreap::updateNodeValues_(Node_ *N) {
    N->size_ = getSize_(N->left_) + getSize_(N->right_) + 1;
    N->sum_ = getSum_(N->left_) + getSum_(N->right_) + static_cast<long long>(N->value_);
    N->leftValue_ = getLeft_(N->left_, N->value_);
    N->rightValue_ = getRight_(N->right_, N->value_);
    N->firstSortCountStart_ = getSortCount_(N->left_, 0, 0);
    if (N->firstSortCountStart_ == getSize_(N->left_) && N->value_ <= getRight_(N->left_, INFINIT)) {
        ++(N->firstSortCountStart_);
        if (N->value_ >= getLeft_(N->right_, -INFINIT))
            N->firstSortCountStart_ += getSortCount_(N->right_, 0, 0);
    }
    N->secondSortCountStart_ = getSortCount_(N->left_, 1, 0);
    if (N->secondSortCountStart_ == getSize_(N->left_) && N->value_ >= getRight_(N->left_, -INFINIT)) {
        ++(N->secondSortCountStart_);
        if (N->value_ <= getLeft_(N->right_, INFINIT))
            N->secondSortCountStart_ += getSortCount_(N->right_, 1, 0);
    }
    N->firstSortCountEnd_ = getSortCount_(N->right_, 0, 1);
    if (N->firstSortCountEnd_ == getSize_(N->right_) && N->value_ >= getLeft_(N->right_, -INFINIT)) {
        ++(N->firstSortCountEnd_);
        if (N->value_ <= getRight_(N->left_, INFINIT))
            N->firstSortCountEnd_ += getSortCount_(N->left_, 0, 1);
    }
    N->secondSortCountEnd_ = getSortCount_(N->right_, 1, 1);
    if (N->secondSortCountEnd_ == getSize_(N->right_) && N->value_ <= getLeft_(N->right_, INFINIT)) {
        ++(N->secondSortCountEnd_);
        if (N->value_ >= getRight_(N->left_, -INFINIT))
            N->secondSortCountEnd_ += getSortCount_(N->left_, 1, 1);
    }
}

void MagicTreap::updateNode_(Node_ *N) {
    if (N == nullptr)
        return;
    if (N->reversed_) {
        N->reversed_ = false;
        toSwap(N->left_, N->right_);
        setReversedNode_(N->left_);
        setReversedNode_(N->right_);
    }
    if (N->equal_ != INFINIT) {
        N->value_ = N->equal_;
        setEqualNode_(N->left_, N->equal_);
        setEqualNode_(N->right_, N->equal_);
        N->equal_ = INFINIT;
    }
    if (N->add_ != 0) {
        N->value_ += N->add_;
        setAddNode_(N->left_, N->add_);
        setAddNode_(N->right_, N->add_);
        N->add_ = 0;
    }
    updateNodeValues_(N);
}


void MagicTreap::setReversedNode_(Node_ *N) {
    if (N != nullptr) {
        N->reversed_ ^= true;
        toSwap(N->rightValue_, N->leftValue_);
        toSwap(N->firstSortCountEnd_, N->secondSortCountStart_);
        toSwap(N->firstSortCountStart_, N->secondSortCountEnd_);
    }
}

void MagicTreap::setAddNode_(Node_ *N, long long val) {
    if (N != nullptr) {
        N->add_ += val;
        N->sum_ += static_cast<long long>(N->size_) * static_cast<long long>(val);
        N->rightValue_ += val;
        N->leftValue_ += val;
    }
}

void MagicTreap::setEqualNode_(Node_ *N, long long val) {
    if (N != nullptr) {
        N->add_ = 0;
        N->equal_ = val;
        N->sum_ = static_cast<long long>(N->size_) * static_cast<long long>(val);
        N->leftValue_ = val;
        N->rightValue_ = val;
        N->firstSortCountStart_ = N->firstSortCountEnd_ = N->secondSortCountStart_ = N->secondSortCountEnd_ = N->size_;
    }
}

void MagicTreap::setAddSegment_(MagicTreap::Node_ *N, long long val, long long l, long long r) {
    if (N == nullptr || l > r)
        return;
    updateNode_(N);
    if (l == 0 && r == N->size_ - 1)
        setAddNode_(N, val);
    else {
        setAddSegment_(N->left_, val, l, min(r, getSize_(N->left_) - 1));
        setAddSegment_(N->right_, val, max(l - getSize_(N->left_) - 1, static_cast<long long>(0)), r - getSize_(N->left_) - 1);
        if (getSize_(N->left_) <= r && getSize_(N->left_) >= l)
            N->value_ += val;
        updateNodeValues_(N);
    }
}

void MagicTreap::setEqualSegment_(Node_ *N, long long val, long long l, long long r) {
    if (N == nullptr || l > r)
        return;
    updateNode_(N);
    if (l == 0 && r == N->size_ - 1)
        setEqualNode_(N, val);
    else {
        setEqualSegment_(N->left_, val, l, min(r, getSize_(N->left_) - 1));
        setEqualSegment_(N->right_, val, max(l - getSize_(N->left_) - 1, static_cast<long long>(0)), r - getSize_(N->left_) - 1);
        if (getSize_(N->left_) <= r && getSize_(N->left_) >= l)
            N->value_ = val;
        updateNodeValues_(N);
    }
}

long long MagicTreap::getSumSegment_(Node_ *N, long long l, long long r) {
    if (N == nullptr || l > r || r < 0)
        return 0;
    updateNode_(N);
    if (l == 0 && r == N->size_ - 1)
        return static_cast<long long>(N->sum_);
    else
        return getSumSegment_(N->left_, l, min(r, getSize_(N->left_) - 1)) +
               getSumSegment_(N->right_, max(l - getSize_(N->left_) - 1, static_cast<long long>(0)), r - getSize_(N->left_) - 1) +
               (r >= getSize_(N->left_) && l <= getSize_(N->left_)) * N->value_;
}


Pair<MagicTreap *, bool> MagicTreap::splitter_(long long key) {
    Pair<Node_ *, Node_ *> x(split_(root_, key, 0));
    if (x.first == root_)
        return Pair<MagicTreap *, bool>(new MagicTreap(x.second), true);
    else
        return Pair<MagicTreap *, bool>(new MagicTreap(x.first), false);
}

Pair<MagicTreap::Node_ *, MagicTreap::Node_ *> MagicTreap::split_(MagicTreap::Node_ *N, long long key, long long herit) {
    if (N == nullptr) {
        return Pair<Node_*, Node_*>(nullptr, nullptr);
    }
    updateNode_(N);
    if (N->size_ - getSize_(N->right_) + herit <= key) {
        Pair<Node_*, Node_*> rec = split_(N->right_, key, N->size_ - getSize_(N->right_) + herit);
        N->right_ = rec.first;
        updateNodeValues_(N);
        return Pair<Node_ *, Node_ *>(N, rec.second);
    }
    else {
        Pair<Node_ *, Node_ *> rec = split_(N->left_, key, herit);
        N->left_ = rec.second;
        updateNodeValues_(N);
        return Pair<Node_ *, Node_ *>(rec.first, N);
    }
}

MagicTreap::Node_* MagicTreap::merge_(Node_ *our, Node_ *other) {
    if (our == nullptr && other == nullptr)
        return nullptr;
    if (our == nullptr)
        return other;
    if (other == nullptr)
        return our;
    updateNode_(our);
    updateNode_(other);
    if (our->pri_ >= other->pri_) {
        our->right_ = merge_(our->right_, other);
        updateNodeValues_(our);
        return our;
    }
    other->left_ = merge_(our, other->left_);
    updateNodeValues_(other);
    return other;
}


long long MagicTreap::upperBound_(Node_ *N, long long val, long long shape) {
    updateNode_(N);
    if (N == nullptr)
        return INFINIT;
    if (compareShape_(val, N->value_, shape)) {
        long long returned = upperBound_(N->left_, val, shape);
        updateNodeValues_(N);
        return returned;
    }
    if (compareShape_(val, getLeft_(N->right_, (2 * shape - 1) * INFINIT), shape)) {
        long long returned = N->value_;
        N->value_ = val;
        updateNodeValues_(N);
        return returned;
    }
    long long returned = upperBound_(N->right_, val, shape);
    updateNodeValues_(N);
    return returned;
}

void MagicTreap::permutateShape_(long long l, long long r, long long shape) {
    MagicTreap *firstMagic = split(l);
    MagicTreap *permutated = split(r - l + 1);
    permutated->permutate_(shape);
    firstMagic->merge(*permutated);
    firstMagic->merge(*this);
    swap(*firstMagic);
    delete firstMagic;
    delete permutated;
}

void MagicTreap::permutate_(long long shape) {
    if (getOrder_(root_, shape)) {
        setReversedNode_(root_);
        return;
    }
    long long pos = root_->size_ - getSortCount_(root_, shape, 1) - 1;
    MagicTreap *firstMagic = split(pos + 1);
    long long val = firstMagic->sum(pos, pos);
    long long val2 = upperBound_(root_, val, shape);
    firstMagic->setEqual(val2, pos, pos);
    setReversedNode_(root_);
    firstMagic->merge(*this);
    swap(*firstMagic);
    delete firstMagic;
}

long long MagicTreap::findPositionSorting_(Node_ *N, long long shape) {
    updateNode_(N);
    if (!getOrder_(N->right_, shape))
        return getSize_(N->left_) + 1 + findPositionSorting_(N->right_, shape);
    if (!compareShape_(N->value_, getLeft_(N->right_, (2 * shape - 1) * INFINIT), shape))
        return getSize_(N->left_) + 1;
    if (!compareShape_(getRight_(N->left_, (2 * shape - 1) * INFINIT), N->value_, shape))
        return getSize_(N->left_);
    return findPositionSorting_(N->left_, shape);
    return 0;
}

void MagicTreap::print_(Node_ *N) {
    if (N == nullptr)
        return;
    updateNode_(N);
    print_(N->left_);
    printf("%lld ", N->value_);
    print_(N->right_);
}

void MagicTreap::nextPriority_() {
    priority_ = rand() * rand() + rand();
}
#endif //MADNESS_MADNESS_H
