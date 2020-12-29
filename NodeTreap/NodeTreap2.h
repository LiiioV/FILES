//
// Created by ADMIN on 20.05.2020.
//

#ifndef IMPLICITTREAP_IMPLICITTREAP_H
#define IMPLICITTREAP_IMPLICITTREAP_H


const long long INFINIT = 1e10;

template <typename A>
void toSwap(A &x, A &y) {
    A t = x;
    x = y;
    y = t;
}

bool greatherEqual(long long x, long long y) {
    return x >= y;
}

bool lessEqual(long long x, long long y) {
    return x <= y;
}

bool greather(long long x, long long y) {
    return x > y;
}

bool less(long long x, long long y) {
    return x < y;
}




class ImplicitTreap {
public:
    ImplicitTreap(long long value, long long priority = rand(), ImplicitTreap *left = nullptr, ImplicitTreap *right = nullptr);

    ~ImplicitTreap();

    void reverseSegment(ImplicitTreap *&other, long long l, long long r);

    void insert(ImplicitTreap *&other, long long pos, long long value, long long priority = rand());

    void erase(ImplicitTreap *&other, long long pos);

    void equalSegment(ImplicitTreap *&other, long long l, long long r, long long equal);

    void addSegment(ImplicitTreap *&other, long long l, long long r, long long add);

    long long sumSegment(ImplicitTreap *&other, long long l, long long r);

    void nextPermutationSegment(ImplicitTreap *&other, long long l, long long r);

    void previousPermutationSegment(ImplicitTreap *&other, long long l, long long r);

    void print(ImplicitTreap *);

private:
    bool reversed_ = false;
    long long size_;
    long long value_;
    long long sumValue_;
    long long add_ = 0;
    long long equal_ = INFINIT;
    long long priority_;
    long long suffixIncrease_;
    long long suffixDecrease_;
    long long prefixIncrease_;
    long long prefixDecrease_;
    ImplicitTreap *left_;
    ImplicitTreap *right_;

    enum MyAttribute {
        SI, SD, PI, PD
    };
    void update_(ImplicitTreap *);

    long long getSize_(ImplicitTreap *);

    long long sumCost_(ImplicitTreap *);

    long long getAttribute_(ImplicitTreap *, MyAttribute);

    void setAdd_(ImplicitTreap *, long long);

    void setReverse_(ImplicitTreap *);

    void setEqual_(ImplicitTreap *, long long);

    void suffixCalculation_(long long &, MyAttribute, bool (*)(long long, long long));

    void prefixCalculation_(long long &, MyAttribute, bool (*)(long long, long long));

    void updateNodeValues_(ImplicitTreap *);

    ImplicitTreap *merge_(ImplicitTreap *, ImplicitTreap *);

    void split_(long long, ImplicitTreap *&, ImplicitTreap *&);

    void devide_(ImplicitTreap *, ImplicitTreap *&, ImplicitTreap *&, ImplicitTreap *&, long long, long long);

    ImplicitTreap *getElement_(ImplicitTreap *, long long);

    void swap_(ImplicitTreap *&, long long, long long);

    long long upperBound_(long long, long long, bool (*)(long long, long long));

    ImplicitTreap *getSun_(ImplicitTreap *, bool);
};

//////////////
////public////
//////////////

ImplicitTreap::ImplicitTreap(long long value, long long priority, ImplicitTreap *left, ImplicitTreap *right)
        : value_(value), priority_(priority), left_(left), right_(right) {
    updateNodeValues_(this);
}

ImplicitTreap::~ImplicitTreap() {
    delete left_;
    delete right_;
}

void ImplicitTreap::reverseSegment(ImplicitTreap *&other, long long l, long long r) {
    ImplicitTreap *left, *right, *mid;
    devide_(other, left, mid, right, l, r);
    setReverse_(mid);
    other = merge_(left, merge_(mid, right));
}

void ImplicitTreap::insert(ImplicitTreap *&other, long long pos, long long value, long long priority) {
    if (other == nullptr)
        other = new ImplicitTreap(value, priority);
    else {
        ImplicitTreap *left, *right;
        other->split_(pos, left, right);
        other = merge_(merge_(left, new ImplicitTreap(value, priority)), right);
    }
}

void ImplicitTreap::erase(ImplicitTreap *&other, long long pos) {
    ImplicitTreap *left, *right, *mid;
    devide_(other, left, mid, right, pos, pos);
    delete mid;
    other = merge_(left, right);
}

void ImplicitTreap::equalSegment(ImplicitTreap *&other, long long l, long long r, long long equal) {
    ImplicitTreap *left, *right, *mid;
    devide_(other, left, mid, right, l, r);
    setEqual_(mid, equal);
    other = merge_(left, merge_(mid, right));
}

void ImplicitTreap::addSegment(ImplicitTreap *&other, long long l, long long r, long long add) {
    ImplicitTreap *left, *right, *mid;
    devide_(other, left, mid, right, l, r);
    setAdd_(mid, add);
    other = merge_(left, merge_(mid, right));
}

long long ImplicitTreap::sumSegment(ImplicitTreap *&other, long long l, long long r) {
    ImplicitTreap *left, *right, *mid;
    devide_(other, left, mid, right, l, r);
    long long ans = sumCost_(mid);
    other = merge_(left, merge_(mid, right));
    return ans;
}

void ImplicitTreap::nextPermutationSegment(ImplicitTreap *&other, long long l, long long r) {
    ImplicitTreap *left, *right, *mid;
    devide_(other, left, mid, right, l, r);
    if (getAttribute_(mid, SI) == getSize_(mid))
        setReverse_(mid);
    else {
        long long current = getSize_(mid) - getAttribute_(mid, SI);
        swap_(mid, current, mid->upperBound_(getAttribute_(mid, SI), getElement_(mid, current)->value_, less));
        reverseSegment(mid, current + 1, mid->size_);
    }
    other = merge_(left, merge_(mid, right));
}

void ImplicitTreap::previousPermutationSegment(ImplicitTreap *&other, long long l, long long r) {
    ImplicitTreap *left, *right, *mid;
    devide_(other, left, mid, right, l, r);
    if (getAttribute_(mid, SD) == getSize_(mid))
        setReverse_(mid);
    else {
        long long current = getSize_(mid) - getAttribute_(mid, SD);
        swap_(mid, current, mid->upperBound_(getAttribute_(mid, SD), getElement_(mid, current)->value_, greather));
        reverseSegment(mid, current + 1, mid->size_);
    }
    other = merge_(left, merge_(mid, right));
}

void ImplicitTreap::print(ImplicitTreap *other) {
    if (other == nullptr)
        return;
    update_(other);
    print(other->left_);
    std::cout << other->value_ << " ";
    print(other->right_);
}

///////////////
////private////
///////////////

void ImplicitTreap::update_(ImplicitTreap *other) {
    if (other == nullptr)
        return;
    if (other->reversed_) {
        other->reversed_ = false;
        toSwap(other->left_, other->right_);
        toSwap(other->suffixDecrease_, other->prefixDecrease_);
        toSwap(other->suffixIncrease_, other->prefixIncrease_);
        setReverse_(other->left_);
        setReverse_(other->right_);
    }
    if (other->equal_ != INFINIT) {
        other->value_ = other->equal_;
        setEqual_(other->left_, other->equal_);
        setEqual_(other->right_, other->equal_);
        other->equal_ = INFINIT;
    }
    if (other->add_ != 0) {
        other->value_ += other->add_;
        setAdd_(other->left_, other->add_);
        setAdd_(other->right_, other->add_);
        other->add_ = 0;
    }
    updateNodeValues_(other);
}

long long ImplicitTreap::getSize_(ImplicitTreap *other) {
    return other == nullptr ? 0 : other->size_;
}

long long ImplicitTreap::sumCost_(ImplicitTreap *other) {
    return other == nullptr ? 0 : other->sumValue_;
}

long long ImplicitTreap::getAttribute_(ImplicitTreap *other, MyAttribute atr) {
    if (other == nullptr)
        return 0;
    if (atr == SI)
        return other->suffixIncrease_;
    if (atr == SD)
        return other->prefixDecrease_;
    if (atr == PI)
        return other->prefixIncrease_;
    if (atr == PD)
        return other->prefixDecrease_;
}

void ImplicitTreap::setAdd_(ImplicitTreap *other, long long add) {
    if (other != nullptr)
        other->add_ += add;
}

void ImplicitTreap::setEqual_(ImplicitTreap *other, long long equal) {
    if (other != nullptr) {
        other->equal_ = equal;
        other->add_ = 0;
    }
}

void ImplicitTreap::setReverse_(ImplicitTreap *other) {
    if (other != nullptr)
        other->reversed_ = !(other->reversed_);
}

void ImplicitTreap::suffixCalculation_(long long &ans, MyAttribute atr, bool (*func)(long long, long long)) {
    if (right_ == nullptr || (getAttribute_(right_, atr) == getSize_(right_) && func(getSun_(right_, true)->value_, value_))) {
        if (left_ == nullptr || (func(value_, getSun_(left_, false)->value_)))
            ans = getSize_(right_) + getAttribute_(left_, atr) + 1;
        else
            ans = getSize_(right_) + 1;
    } else
        ans = getAttribute_(right_, atr);
}


void ImplicitTreap::prefixCalculation_(long long &ans, MyAttribute atr, bool (*func)(long long, long long)) {
    if (left_ == nullptr || (getAttribute_(left_, atr) == getSize_(left_) && func(getSun_(left_, false)->value_, value_))) {
        if (right_ == nullptr || (func(value_, getSun_(right_, true)->value_)))
            ans = getSize_(right_) + getAttribute_(left_, atr) + 1;
        else
            ans = getSize_(left_) + 1;
    } else
        ans = getAttribute_(left_, atr);
}

void ImplicitTreap::updateNodeValues_(ImplicitTreap *other) {
    if (other == nullptr)
        return;
    other->size_ = getSize_(other->left_) + getSize_(other->right_) + 1;
    other->sumValue_ = sumCost_(other->left_) + sumCost_(other->right_) + other->value_;
    other->suffixCalculation_(other->suffixIncrease_, SI, lessEqual);
    other->suffixCalculation_(other->suffixDecrease_, SD, greatherEqual);
    other->prefixCalculation_(other->prefixIncrease_, PI, lessEqual);
    other->prefixCalculation_(other->prefixDecrease_, PD, greatherEqual);
}

ImplicitTreap *ImplicitTreap::merge_(ImplicitTreap *left, ImplicitTreap *right) {
    update_(left);
    update_(right);
    if (left == nullptr)
        return right;
    if (right == nullptr)
        return left;
    if (left->priority_ > right->priority_) {
        left->right_ = merge_(left->right_, right);
        updateNodeValues_(left);
        return left;
    } else {
        right->left_ = merge_(left, right->left_);
        updateNodeValues_(right);
        return right;
    }
}

void ImplicitTreap::split_(long long key, ImplicitTreap *&left, ImplicitTreap *&right) {
    update_(this);
    long long current = getSize_(left_) + 1;
    ImplicitTreap *nptr = nullptr;
    if (current <= key) {
        if (right_ == nullptr)
            right = nullptr;
        else
            right_->split_(key - current, nptr, right);
        right_ = nptr;
        updateNodeValues_(this);
        left = this;
    } else {
        if (left_ == nullptr)
            left = nullptr;
        else
            left_->split_(key, left, nptr);
        left_ = nptr;
        updateNodeValues_(this);
        right = this;
    }
}

void ImplicitTreap::devide_(ImplicitTreap *other, ImplicitTreap *&first, ImplicitTreap *&second, ImplicitTreap *&third,
                            long long fs, long long st) {
    other->split_(fs - 1, first, third);
    third->split_(st - fs + 1, second, third);
}

ImplicitTreap *ImplicitTreap::getElement_(ImplicitTreap *other, long long key) {
    update_(other);
    while (getSize_(other->left_) + 1 != key) {
        if (getSize_(other->left_) + 1 < key) {
            key -= getSize_(other->left_) + 1;
            other = other->right_;
        } else
            other = other->left_;
        update_(other);
    }
    return other;
}

void ImplicitTreap::swap_(ImplicitTreap *&other, long long l, long long r) {
    if (l > r)
        toSwap(l, r);
    ImplicitTreap *lef1, *lef2, *mid1, *mid2, *rig1, *rig2;
    devide_(other, lef1, mid1, rig1, l, l);
    devide_(other, lef2, mid2, rig2, r - l, r - l);
    other = merge_(merge_(lef1, mid2), merge_(merge_(lef2, mid1), rig2));
}


long long ImplicitTreap::upperBound_(long long size, long long key, bool (*func)(long long, long long)) {
    long long count = size;
    long long first = size_;
    while (count > 0) {
        long long f = first;
        long long s = count / 2;
        f -= s;
        long long copy = getElement_(this, f)->value_;
        if (!func(key, copy)) {
            first = --f;
            count -= s + 1;
        } else
            count = s;
    }
    return first;
}

ImplicitTreap *ImplicitTreap::getSun_(ImplicitTreap *other, bool l) {
    update_(other);
    ImplicitTreap *anOther = (l) ? other->left_ : other->right_;
    while (anOther != nullptr) {
        other = anOther;
        anOther = (l) ? other->left_ : other->right_;
        update_(other);
    }
    return other;
}

#endif //IMPLICITTREAP_IMPLICITTREAP_H
