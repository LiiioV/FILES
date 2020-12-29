//
// Created by ADMIN on 08.11.2019.
//

#ifndef UNTITLED_PERMUTATION_H
#define UNTITLED_PERMUTATION_H

class Permutation {
public:
    Permutation(unsigned int n);

    Permutation(unsigned int n, const int *a);

    Permutation(const Permutation &A);

    ~Permutation() {
        delete[] buffer_;
    }

    Permutation &operator++();

    Permutation operator++(int);

    Permutation next() const;

    Permutation &operator--();

    Permutation operator--(int);

    Permutation previous() const;

    Permutation &operator=(const Permutation &A);

    Permutation &operator*=(const Permutation &A);

    Permutation inverse() const;

    void operator()(int *a) const;

    int operator[](int i) const;

    bool operator==(const Permutation &A) const;

    bool operator!=(const Permutation &A) const;

    bool operator<=(const Permutation &A) const;

    bool operator>=(const Permutation &A) const;

    bool operator>(const Permutation &A) const;

    bool operator<(const Permutation &A) const;

private:
    int *buffer_;
    unsigned int size_;

    void swap_(unsigned int i, unsigned int j);

    void reverse_(unsigned int, unsigned int);
};

Permutation::Permutation(unsigned int n) {
    buffer_ = new int[n];
    for (unsigned int i = 0; i < n; ++i)
        buffer_[i] = static_cast<int>(i);
    size_ = n;
}

Permutation::Permutation(unsigned int n, const int *a) {
    buffer_ = new int[n];
    for (unsigned int i = 0; i < n; ++i)
        buffer_[i] = *(a + i);
    size_ = n;
}

Permutation::Permutation(const Permutation &A) {
    buffer_ = new int[A.size_];
    for(unsigned int i = 0; i < A.size_; ++i)
        buffer_[i] = A.buffer_[i];
    size_ = A.size_;
}

Permutation &Permutation::operator++() {
    unsigned int begin = size_ - 1;
    while (begin > 0 && buffer_[begin] < buffer_[begin - 1])
        begin--;
    unsigned int mid;
    if (begin == 0) {
        mid = size_/2;
        for (unsigned int i = 0; i < mid; ++i)
            swap_(i, size_ - 1 - i);
        return *this;
    }
    begin--;
    for (unsigned int i = size_ - 1;; --i) {
        if (buffer_[i] > buffer_[begin]) {
            swap_(i, begin);
            break;
        }
    }
    mid = (size_ + begin) / 2;
    for (unsigned int i = begin + 1; i <= mid; ++i)
        swap_(i, size_ - i + begin);
    return *this;
}

Permutation Permutation::operator++(int) {
    Permutation A = *this;
    ++(*this);
    return A;
}

Permutation Permutation::next() const{
    Permutation A = *this;
    ++A;
    return A;
}

Permutation &Permutation::operator--() {
    unsigned int begin = size_ - 1;
    while (begin > 0 && buffer_[begin] > buffer_[begin - 1])
        begin--;

    if (begin == 0) {
        reverse_(0, size_);
        return *this;
    }

    begin--;
    for (unsigned int i = size_ - 1;; --i) {
        if (buffer_[i] < buffer_[begin]) {
            swap_(i, begin);
            break;
        }
    }

    reverse_(begin + 1, size_);
    return *this;
}

Permutation Permutation::operator--(int) {
    Permutation A = *this;
    --(*this);
    return A;
}

Permutation Permutation::previous() const {
    Permutation A = *this;
    --A;
    return A;
}

Permutation &Permutation::operator=(const Permutation &A) {
    delete[] buffer_;
    buffer_ = new int[A.size_];

    for (unsigned int i = 0; i < A.size_; ++i)
        buffer_[i] = A.buffer_[i];

    size_ = A.size_;
    return *this;
}

Permutation &Permutation::operator*=(const Permutation &A) {
    unsigned int SizeX = size_;
    if(SizeX < A.size_)
        SizeX = A.size_;
    int* a = new int[SizeX];

    for(unsigned int i = 0; i < SizeX; ++i) {
        if(static_cast<unsigned int>(A.buffer_[i]) < size_)
            a[i] = buffer_[A.buffer_[i]];
        else
            a[i] = A.buffer_[i];
    }

    delete[] buffer_;
    buffer_ = a;
    size_ = SizeX;
    return *this;
}

Permutation operator*(const Permutation &A, const Permutation &B) {
    Permutation T = A;
    T *= B;
    return T;
}

bool Permutation::operator==(const Permutation &A) const {
    if (A.size_ != size_)
        return false;

    for (unsigned int i = 0; i < size_; ++i) {
        if (buffer_[i] != A.buffer_[i])
            return false;
    }

    return true;
}

bool Permutation::operator!=(const Permutation &A) const {
    return !(*this == A);
}

bool Permutation::operator<=(const Permutation &A) const {
    unsigned int MinSIze = size_;

    if(MinSIze > A.size_)
        MinSIze = A.size_;

    for(unsigned int i = 0; i < MinSIze; ++i) {
        if (buffer_[i] != A.buffer_[i])
            return buffer_[i] < A.buffer_[i];
    }

    return size_ <= A.size_;
}

bool Permutation::operator<(const Permutation &A) const {
    return *this <= A && *this != A;
}

bool Permutation::operator>=(const Permutation &A) const {
    return !(*this < A);
}

bool Permutation::operator>(const Permutation &A) const {
    return !(*this <= A);
}

int Permutation::operator[](int i) const {
    return buffer_[i];
}

void Permutation::operator()(int *a) const {
    int* b = new int[size_];

    for(unsigned int i = 0; i < size_; ++i)
        b[buffer_[i]] = a[i];

    for(unsigned int i = 0; i < size_; ++i)
        a[i] = b[i];
    delete[] b;
}

Permutation Permutation::inverse() const {
    int* a = new int[size_];

    for(unsigned int i = 0; i < size_; ++i)
        a[buffer_[i]] = static_cast<int>(i);
    Permutation T(size_, a);

    delete[] a;
    return T;
}

void Permutation::swap_(unsigned int i, unsigned int j) {
    int swaperr = buffer_[i];
    buffer_[i] = buffer_[j];
    buffer_[j] = swaperr;
}

void Permutation::reverse_(unsigned int begin, unsigned int end) {
    unsigned int mid = (end + begin) / 2;
    for (unsigned int i = begin; i < mid; ++i)
        swap_(i, size_ - i + begin);
}

#endif //UNTITLED_PERMUTATION_H
