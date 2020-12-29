//
// Created by ADMIN on 28.02.2020.
//

#ifndef UNTITLED3_FINITE_H
#define UNTITLED3_FINITE_H
#include "my_instruments.h"

template<unsigned int N>
class Finite {
    long long value_;
public:
    Finite();

    Finite(int);

    Finite(const Finite<N> &);

    Finite &operator=(int);

    Finite &operator=(const Finite<N> &);

    Finite<N> operator-() const;

    Finite<N> &operator+=(const Finite<N> &);

    Finite<N> &operator-=(const Finite<N> &);

    Finite<N> &operator*=(const Finite<N> &);

    Finite<N> &operator/=(const Finite<N> &);

    Finite<N> &operator++();

    Finite<N> &operator--();

    Finite<N> operator++(int);

    Finite<N> operator--(int);

    explicit operator int() const;

    bool operator==(const Finite<N> &) const;

    bool operator!=(const Finite<N> &) const;

    Finite<N> inverse() const;

    Finite<N> &pow(unsigned int k);

private:
    void toCorrect_();
};

template<unsigned int N>
std::istream &operator>>(std::istream &, Finite<N> &);

template<unsigned int N>
Finite<N> operator+(const Finite<N> &, const Finite<N> &);

template<unsigned int N>
Finite<N> operator-(const Finite<N> &, const Finite<N> &);

template<unsigned int N>
Finite<N> operator*(const Finite<N> &, const Finite<N> &);

template<unsigned int N>
Finite<N> operator/(const Finite<N> &, const Finite<N> &);

long long binPow(long long, unsigned int, unsigned int);

template<unsigned int N>
void Finite<N>::toCorrect_() {
    value_ %= N;
    if (value_ < 0)
        value_ += N;
}

template<unsigned int N>
Finite<N>::Finite() {
    value_ = 0;
}

template<unsigned int N>
Finite<N>::Finite(int a) {
    value_ = a;
    toCorrect_();
}

template<unsigned int N>
Finite<N>::Finite(const Finite<N> &A) {
    value_ = A.value_;
}

template<unsigned int N>
Finite<N> &Finite<N>::operator=(int a) {
    value_ = a;
    toCorrect_();
    return *this;
}

template<unsigned int N>
Finite<N> &Finite<N>::operator=(const Finite<N> &A) {
    value_ = A.value_;
    return *this;
}

template<unsigned int N>
Finite<N> Finite<N>::operator-() const {
    Finite<N> A = (*this) * Finite<N>(-1);
    A.toCorrect_();
    return A;
}

template<unsigned int N>
Finite<N> &Finite<N>::operator+=(const Finite<N> &A) {
    value_ += A.value_;
    toCorrect_();
    return *this;
}

template<unsigned int N>
Finite<N> &Finite<N>::operator-=(const Finite<N> &A) {
    value_ -= A.value_;
    toCorrect_();
    return *this;
}

template<unsigned int N>
Finite<N> &Finite<N>::operator*=(const Finite<N> &A) {
    value_ *= A.value_;
    toCorrect_();
    return *this;
}

template<unsigned int N>
Finite<N> &Finite<N>::operator/=(const Finite<N> &A) {
    value_ *= A.inverse().value_;
    toCorrect_();
    return *this;
}

template<unsigned int N>
Finite<N> &Finite<N>::operator++() {
    (*this) += 1;
    return *this;
}

template<unsigned int N>
Finite<N> &Finite<N>::operator--() {
    (*this) -= 1;
    return *this;
}

template<unsigned int N>
Finite<N> Finite<N>::operator++(int) {
    Finite<N> A = *this;
    ++(*this);
    return A;
}

template<unsigned int N>
Finite<N> Finite<N>::operator--(int) {
    Finite<N> A = *this;
    --(*this);
    return A;
}

template<unsigned int N>
Finite<N>::operator int() const {
    return value_;
}

template<unsigned int N>
bool Finite<N>::operator==(const Finite<N> &A) const {
    return (A.value_ - value_) % N == 0;
}

template<unsigned int N>
bool Finite<N>::operator!=(const Finite<N> &A) const {
    return (A.value_ - value_) % N != 0;
}

template<unsigned int N>
Finite<N> Finite<N>::inverse() const {
    toAssert<is_Prime<N>::answer>::type;
    Finite<N> Inverse = binPow(value_, N - 2, N);
    return Inverse;
}

template<unsigned int N>
Finite<N> &Finite<N>::pow(unsigned int k) {
    value_ = binPow(value_, k, N);
    return *this;
}

template<unsigned int N>
std::istream &operator>>(std::istream &in, Finite<N> &A) {
    int a;
    in >> a;
    A = a;
    return in;
}

template<unsigned int N>
std::ostream &operator<<(std::ostream &out, const Finite<N> &A) {
    int a = int(A);
    out << a;
    return out;
}

template<unsigned int N>
Finite<N> operator+(const Finite<N> &A, const Finite<N> &B) {
    Finite<N> C = A;
    C += B;
    return C;
}

template<unsigned int N>
inline Finite<N> operator-(const Finite<N> &A, const Finite<N> &B) {
    Finite<N> C = A;
    C -= B;
    return C;
}

template<unsigned int N>
Finite<N> operator*(const Finite<N> &A, const Finite<N> &B) {
    Finite<N> C = A;
    C *= B;
    return C;
}

template<unsigned int N>
Finite<N> operator/(const Finite<N> &A, const Finite<N> &B) {
    Finite<N> C = A;
    C /= B;
    return C;
}

long long binPow(long long n, unsigned int k, unsigned int mod) {
    if (k == 0)
        return 1;
    long long c = binPow(n, k / 2, mod);
    (c *= c) %= mod;
    return c * (k % 2 ? n : 1);
}


#endif //UNTITLED3_FINITE_H