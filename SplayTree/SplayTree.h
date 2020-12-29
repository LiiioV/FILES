
//
// Created by ADMIN on 24.05.2020.
//

#ifndef SPLAY_SPLAYTREE_H
#define SPLAY_SPLAYTREE_H

const long long INF = 1e10;

template<typename T>
void toSwap(T &a, T &b) {
    T c = a;
    a = b;
    b = c;
}

int biggestGradeOfTwo(int n) {
    int h = 1;
    while (h < n)
        h *= 2;
    return h / 2;
}

template<typename T, typename G>
T min(const T &a, const G &b) {
    return a > b ? b : a;
}

template<typename T, typename G>
T max(const T &a, const G &b) {
    return a > b ? a : b;
}

bool lessEqual(long long a, long long b) {
    return a <= b;
}

bool greaterEqual(long long a, long long b) {
    return a >= b;
}


class SplayTree {
    struct Node_;

    enum Attribute {
        IncStart, IncEnd, DecStart, DecEnd
    };

    enum AddOrEqual {
        Add, Equal
    };

    Node_ *root_;
public:
    SplayTree() : root_(nullptr) {};

    explicit SplayTree(Node_ *N) : root_(N) {};

    explicit SplayTree(int size, long long *array) : root_(construct_(size, array)) {};

    ~SplayTree();

    long long getSum(int l, int r);

    void insert(long long value, int pos);

    void erase(int pos);

    void setEqual(long long value, int l, int r);

    void setAdd(long long value, int l, int r);

    void nextPermutation(int l, int r);

    void prevPermutation(int l, int r);

    void print();
private:

    static Node_ *construct_(int, long long *);

    static void destroy_(Node_ *N);

    static int getSize_(Node_ *);

    static long long getSum_(Node_ *);

    static long long getLeft_(Node_ *, long long);

    static long long getRight_(Node_ *, long long);

    static int getSortCount_(Node_ *, Attribute);

    Node_ * searchElement_(Node_ *, int);

    static void updateNode_(Node_ *);

    static void updateNodeValues_(Node_ *);

    static void updateNodeSortingCounts_(Node_ *, int &, Attribute);

    void transplant_(Node_ *, Node_ *);

    void leftRotate_(Node_ *);

    void rightRotate_(Node_ *);

    void splay_(Node_ *);

    SplayTree *split_(int);

    void merge_(SplayTree *);

    void swap_(SplayTree *);

    static void setReversedNode_(Node_ *);

    static void setAddNode_(Node_ *, long long);

    static void setEqualNode_(Node_ *, long long);

    static void setAddOrEqualSegment_(Node_ *, long long, int, int, AddOrEqual);

    static long long getSumSegment_(Node_ *, int, int);

    void permutateSegment_(Node_ *, int, int, Attribute);

    void permutate_(Attribute);

    long long upperBound_(Node_ *, long long, Attribute);

    static void toPrint_(Node_ *);
};


struct SplayTree::Node_ {
    bool reversed_ = false;
    int size_ = 1;
    int decreaseSortCountStart_ = 1;
    int decreaseSortCountEnd_ = 1;
    int increaseSortCountStart_ = 1;
    int increaseSortCountEnd_ = 1;
    long long equal_ = INF;
    long long add_ = 0;
    long long value_;
    long long sum_;
    long long leftValue_;
    long long rightValue_;
    Node_ *left_ = nullptr;
    Node_ *right_ = nullptr;
    Node_ *parent_ = nullptr;

    explicit Node_(long long val) : value_(val), sum_(val), leftValue_(val), rightValue_(val) {};
};

SplayTree::~SplayTree() {
    destroy_(root_);
}

long long SplayTree::getSum(int l, int r) {
    return getSumSegment_(root_, l, r);
}

void SplayTree::insert(long long value, int pos) {
    auto *newNode = new Node_(value);
    if (root_ == nullptr) {
        root_ = newNode;
        return;
    }
    SplayTree newSplay(newNode);
    SplayTree* firstPart = split_(pos);
    firstPart->merge_(&newSplay);
    firstPart->merge_(this);
    swap_(firstPart);
    delete firstPart;
}

void SplayTree::erase(int pos) {
    SplayTree* firstPart = split_(pos);
    SplayTree* deleting = split_(1);
    firstPart->merge_(this);
    swap_(firstPart);
    delete deleting;
    delete firstPart;
}

void SplayTree::setEqual(long long value, int l, int r) {
    setAddOrEqualSegment_(root_, value, l, r, Equal);
}

void SplayTree::setAdd(long long value, int l, int r) {
    setAddOrEqualSegment_(root_, value, l, r, Add);
}

void SplayTree::nextPermutation(int l, int r) {
    permutateSegment_(root_, l, r, DecEnd);
}

void SplayTree::prevPermutation(int l, int r){
    permutateSegment_(root_, l, r, IncEnd);
}

void SplayTree::print() {
    toPrint_(root_);
}

/////////////////////


SplayTree::Node_ *SplayTree::construct_(int size, long long *array) {
    if (size <= 0)
        return nullptr;
    if (size == 1) {
        return new Node_(array[0]);
    }
    int gradeTwo = biggestGradeOfTwo(size);
    Node_ *newNode = new Node_(array[gradeTwo - 1]);
    Node_ *leftChild = construct_(gradeTwo - 1, array);
    Node_ *rightChild = construct_(size - gradeTwo, array + gradeTwo);

    newNode->left_ = leftChild;
    newNode->right_ = rightChild;
    if (leftChild != nullptr)
        leftChild->parent_ = newNode;
    if (rightChild != nullptr)
        rightChild->parent_ = newNode;

    updateNodeValues_(newNode);
    return newNode;
}

void SplayTree::destroy_(Node_ *N) {
    if (N == nullptr)
        return;
    destroy_(N->left_);
    destroy_(N->right_);
    delete N;
}

int SplayTree::getSize_(Node_ *N) {
    return N == nullptr ? 0 : N->size_;
}

long long SplayTree::getSum_(Node_ *N) {
    return N == nullptr ? 0 : N->sum_;
}

long long SplayTree::getLeft_(Node_ *N, long long defaultVal) {
    return N == nullptr ? defaultVal : N->leftValue_;
}

long long SplayTree::getRight_(Node_ *N, long long defaultVal) {
    return N == nullptr ? defaultVal : N->rightValue_;
}

int SplayTree::getSortCount_(Node_ *N, Attribute atr) {
    if (N == nullptr)
        return 0;
    if (atr == IncStart)
        return N->increaseSortCountStart_;
    else if (atr == IncEnd)
        return N->increaseSortCountEnd_;
    else if (atr == DecStart)
        return N->decreaseSortCountStart_;
    else
        return N->decreaseSortCountEnd_;
}

SplayTree::Node_ *SplayTree::searchElement_(Node_ *N, int index) {
    if (N == nullptr || index >= getSize_(N) || index < 0)
        return nullptr;
    updateNode_(N);
    int myInd = getSize_(N->left_);
    if (index == myInd) {
        splay_(N);
        return N;
    }
    else if (index < myInd)
        return searchElement_(N->left_, index);
    else
        return searchElement_(N->right_, index - myInd - 1);
}

void SplayTree::updateNode_(Node_ *N) {
    if (N == nullptr)
        return;
    if (N->reversed_) {
        N->reversed_ = false;
        toSwap(N->left_, N->right_);
        setReversedNode_(N->left_);
        setReversedNode_(N->right_);
    }
    if (N->equal_ != INF) {
        N->value_ = N->equal_;
        setEqualNode_(N->left_, N->equal_);
        setEqualNode_(N->right_, N->equal_);
        N->equal_ = INF;
    }
    if (N->add_ != 0) {
        N->value_ += N->add_;
        setAddNode_(N->left_, N->add_);
        setAddNode_(N->right_, N->add_);
        N->add_ = 0;
    }
}

void SplayTree::updateNodeValues_(Node_ *N) {
    N->size_ = getSize_(N->left_) + getSize_(N->right_) + 1;
    N->sum_ = getSum_(N->left_) + getSum_(N->right_) + (N->value_);
    N->leftValue_ = getLeft_(N->left_, N->value_);
    N->rightValue_ = getRight_(N->right_, N->value_);
    updateNodeSortingCounts_(N, N->increaseSortCountStart_, IncStart);
    updateNodeSortingCounts_(N, N->increaseSortCountEnd_, IncEnd);
    updateNodeSortingCounts_(N, N->decreaseSortCountStart_, DecStart);
    updateNodeSortingCounts_(N, N->decreaseSortCountEnd_, DecEnd);
}

void SplayTree::updateNodeSortingCounts_(Node_ *N, int &myAttribute, Attribute atr) {
    long long infinity = (atr == IncStart || atr == DecEnd ? -INF : INF);
    bool(*firstCompare)(long long, long long) = (atr == IncStart || atr == DecEnd ? greaterEqual : lessEqual);
    bool(*secondCompare)(long long, long long) = (atr == IncStart || atr == DecEnd ? lessEqual : greaterEqual);
    Node_ *firstChild = (atr == IncStart || atr == DecStart ? N->left_ : N->right_);
    Node_ *secondChild = (atr == IncStart || atr == DecStart ? N->right_ : N->left_);
    long long(*firstFunc)(Node_ *, long long) = (atr == IncStart || atr == DecStart ? getRight_ : getLeft_);
    long long(*secondFunc)(Node_ *, long long) = (atr == IncStart || atr == DecStart ? getLeft_ : getRight_);

    myAttribute = getSortCount_(firstChild, atr);
    if (myAttribute == getSize_(firstChild) && firstCompare(N->value_, firstFunc(firstChild, infinity))) {
        ++myAttribute;
        if (secondCompare(N->value_, secondFunc(secondChild, -infinity)))
            myAttribute += getSortCount_(secondChild, atr);
    }
}

void SplayTree::transplant_(Node_ *N, Node_ *Child) {
    if (N->parent_ == nullptr)
        root_ = Child;
    else if (N == N->parent_->left_)
        N->parent_->left_ = Child;
    else if (N == N->parent_->right_)
        N->parent_->right_ = Child;
    Child->parent_ = N->parent_;
}

void SplayTree::leftRotate_(Node_ *N) {
    Node_ *rightChild = N->right_;

    N->right_ = rightChild->left_;
    if (rightChild->left_ != nullptr)
        rightChild->left_->parent_ = N;

    transplant_(N, rightChild);

    rightChild->left_ = N;
    rightChild->left_->parent_ = rightChild;
    updateNodeValues_(N);
    updateNodeValues_(rightChild);
}

void SplayTree::rightRotate_(Node_ *N) {
    Node_ *leftChild = N->left_;

    N->left_ = leftChild->right_;
    if (leftChild->right_ != nullptr)
        leftChild->right_->parent_ = N;

    transplant_(N, leftChild);

    leftChild->right_ = N;
    leftChild->right_->parent_ = leftChild;
    updateNodeValues_(N);
    updateNodeValues_(leftChild);
}

void SplayTree::splay_(Node_ *N) {
    if (N == nullptr)
        return;

    while (N != root_) {
        // Zig
        if (N->parent_ == root_) {
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

SplayTree *SplayTree::split_(int index) {
    Node_ *x = searchElement_(root_, index);
    SplayTree *firstPart;
    if (x == nullptr) {
        firstPart = new SplayTree();
        swap_(firstPart);
    }
    else {
        firstPart = new SplayTree(x->left_);
        if (x->left_ != nullptr)
            x->left_->parent_ = nullptr;
        x->left_ = nullptr;
        updateNodeValues_(x);
    }
    return firstPart;
}

void SplayTree::merge_(SplayTree *secondPart) {
    Node_ *x = searchElement_(root_, getSize_(root_) - 1);
    if (x == nullptr) {
        swap_(secondPart);
        return;
    }
    x->right_ = secondPart->root_;
    if (x->right_ != nullptr)
        x->right_->parent_ = x;
    secondPart->root_ = nullptr;
    updateNodeValues_(x);
}

void SplayTree::swap_(SplayTree *otherTree) {
    toSwap(root_, otherTree->root_);
}

void SplayTree::setReversedNode_(Node_ *N) {
    if (N != nullptr) {
        N->reversed_ ^= true;
        toSwap(N->rightValue_, N->leftValue_);
        toSwap(N->decreaseSortCountEnd_, N->increaseSortCountStart_);
        toSwap(N->decreaseSortCountStart_, N->increaseSortCountEnd_);
    }
}

void SplayTree::setAddNode_(Node_ *N, long long value) {
    if (N != nullptr) {
        N->add_ += value;
        N->sum_ += N->size_ * value;
        N->rightValue_ += value;
        N->leftValue_ += value;
    }
}

void SplayTree::setEqualNode_(Node_ *N, long long value) {
    if (N != nullptr) {
        N->add_ = 0;
        N->equal_ = value;
        N->sum_ = N->size_ * value;
        N->leftValue_ = value;
        N->rightValue_ = value;
        N->decreaseSortCountStart_ = N->decreaseSortCountEnd_ = N->increaseSortCountStart_ = N->increaseSortCountEnd_ = N->size_;
    }
}

void SplayTree::setAddOrEqualSegment_(Node_ *N, long long value, int l, int r, AddOrEqual aoe) {
    if (N == nullptr || l > r)
        return;
    updateNode_(N);
    if (l == 0 && r == N->size_ - 1)
        aoe == Add ? setAddNode_(N, value) : setEqualNode_(N, value);
    else {
        setAddOrEqualSegment_(N->left_, value, l, min(r, getSize_(N->left_) - 1), aoe);
        setAddOrEqualSegment_(N->right_, value, max(l - getSize_(N->left_) - 1, 0), r - getSize_(N->left_) - 1, aoe);
        if (getSize_(N->left_) <= r && getSize_(N->left_) >= l)
            aoe == Add ? (N->value_ += value) : (N->value_ = value);
        updateNodeValues_(N);
    }
}

long long SplayTree::getSumSegment_(Node_ *N, int l, int r) {
    if (N == nullptr || l > r || r < 0)
        return 0;
    updateNode_(N);
    if (l == 0 && r == N->size_ - 1)
        return N->sum_;
    else
        return getSumSegment_(N->left_, l, min(r, getSize_(N->left_) - 1)) +
               getSumSegment_(N->right_, max(l - getSize_(N->left_) - 1, 0), r - getSize_(N->left_) - 1) +
               (r >= getSize_(N->left_) && l <= getSize_(N->left_)) * N->value_;
}

void SplayTree::permutateSegment_(Node_ *N, int l, int r, Attribute atr) {
    SplayTree *firstPart = split_(l);
    SplayTree *permutating = split_(r - l + 1);
    permutating->permutate_(atr);
    firstPart->merge_(permutating);
    firstPart->merge_(this);
    swap_(firstPart);
    delete permutating;
    delete firstPart;
}

void SplayTree::permutate_(Attribute atr) {
    int sortCount = getSortCount_(root_, atr);
    if (sortCount == root_->size_) {
        setReversedNode_(root_);
        return;
    }
    int pos = root_->size_ - sortCount - 1;
    SplayTree *firstPart = split_(pos + 1);
    long long value = firstPart->getSum(pos, pos);
    long long value2 = upperBound_(root_, value, atr);
    firstPart->setEqual(value2, pos, pos);
    setReversedNode_(root_);
    firstPart->merge_(this);
    swap_(firstPart);
    delete firstPart;
}

long long SplayTree::upperBound_(Node_ *N, long long value, Attribute atr) {
    updateNode_(N);
    bool(*compare)(long long, long long) = (atr == IncEnd ? lessEqual : greaterEqual);
    if (N == nullptr)
        return INF;
    if (compare(value, N->value_)) {
        long long returned = upperBound_(N->left_, value, atr);
        updateNodeValues_(N);
        return returned;
    }
    if (compare(value, getLeft_(N->right_, (atr == IncEnd ? 1 : -1) * INF))) {
        long long returned = N->value_;
        N->value_ = value;
        updateNodeValues_(N);
        splay_(N);
        return returned;
    }
    long long returned = upperBound_(N->right_, value, atr);
    updateNodeValues_(N);
    return returned;
}

void SplayTree::toPrint_(Node_ *N) {
    updateNode_(N);
    if (N == nullptr)
        return;
    toPrint_(N->left_);
    printf("%lld ", N->value_);
    toPrint_(N->right_);
}

#endif //SPLAY_SPLAYTREE_H
