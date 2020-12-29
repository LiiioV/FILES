#ifndef UNTITLED_RATIONAL_H
#define UNTITLED_RATIONAL_H

#include <iostream>
#include <vector>
#include <string>
#include "biginteger.h"

class Rational {
public:
    Rational() : numerator_(0), denominator_(1) {}

    Rational(int a) : numerator_(a), denominator_(1) {}

    Rational(const BigInteger &A) : numerator_(A), denominator_(1) {}

    Rational(const Rational &A) : numerator_(A.numerator_), denominator_(A.denominator_) {}

    std::string toString() const;

    std::string asDecimal(size_t precision) const;

    explicit operator double() const;

    Rational &operator=(int a);

    Rational &operator=(const BigInteger &A);

    Rational &operator=(const Rational &A);

    Rational operator-() const;

    Rational &operator+=(const Rational &A);

    Rational &operator-=(const Rational &A);

    Rational &operator*=(const Rational &A);

    Rational &operator/=(const Rational &A);

    bool operator==(const Rational &A) const;

    bool operator!=(const Rational &A) const;

    bool operator<=(const Rational &A) const;

    bool operator>=(const Rational &A) const;

    bool operator<(const Rational &A) const;

    bool operator>(const Rational &A) const;

private:
    BigInteger numerator_;
    BigInteger denominator_;

    void cut_();
};

void swap(BigInteger&, BigInteger&);

void swap(char&, char&);

std::string reverse(std::string &);

BigInteger gcd(const BigInteger &A, const BigInteger &B);

Rational operator+(const Rational &A, const Rational &B);

Rational operator-(const Rational &A, const Rational &B);

Rational operator/(const Rational &A, const Rational &B);

Rational operator*(const Rational &A, const Rational &B);

std::string Rational::toString() const {
    std::string s = "";
    s += numerator_.toString();
    if (denominator_ == 1)
        return s;
    s += '/';
    s += denominator_.toString();
    return s;
}

std::string Rational::asDecimal(size_t precision = 0) const {
    std::string s = "";
    if((*this)<0) {
        s += "-";
        s += (-(*this)).asDecimal(precision);
        return s;
    }

    BigInteger T = numerator_ / denominator_;
    s += T.toString();
    if (precision == 0)
        return s;


    s += '.';
    T = numerator_ % denominator_;
    for (size_t i = 0; i < precision; ++i)
        T *= 10;

    (T += denominator_ / 2) /= denominator_;

    std::string t = "";
    for(size_t i = 0; i < precision; ++i) {
        t += (T % 10).toString();
        T /= 10;
    }

    reverse(t);
    s+=t;

    return s;
}

Rational &Rational::operator=(int a) {
    numerator_ = a;
    denominator_ = 1;
    return *this;
}

Rational &Rational::operator=(const BigInteger &A) {
    numerator_ = A;
    denominator_ = 1;
    return *this;
}

Rational &Rational::operator=(const Rational &A) {
    numerator_ = A.numerator_;
    denominator_ = A.denominator_;
    return *this;
}

Rational::operator double() const {
    std::string s = asDecimal(20);
    double t = 0.;
    int sign = 1;
    unsigned int begin = 0;
    if(s[0] == '-') {
        ++begin;
        sign = -1;
    }

    for(; s[begin] != '.'; ++begin)
        (t*=10.) += s[begin] - '0';
    begin++;
    double coef = 0.1;
    for(;begin < s.size(); ++begin, coef /= 10.)
        t += coef * (s[begin] - '0');

    t *= sign;
    return t;
}

Rational Rational::operator-() const {
    Rational T = *this;
    T.numerator_ *= -1;
    return T;
}

Rational &Rational::operator+=(const Rational &A) {
    (numerator_ *= A.denominator_) += A.numerator_ * denominator_;
    denominator_ *= A.denominator_;
    this->cut_();
    return *this;
}

Rational &Rational::operator-=(const Rational &A) {
    (numerator_ *= A.denominator_) -= A.numerator_ * denominator_;
    denominator_ *= A.denominator_;
    this->cut_();
    return *this;
}

Rational &Rational::operator*=(const Rational &A) {
    numerator_ *= A.numerator_;
    denominator_ *= A.denominator_;
    this->cut_();
    return *this;
}

Rational &Rational::operator/=(const Rational &A) {
    numerator_ *= A.denominator_;
    denominator_ *= A.numerator_;
    this->cut_();
    return *this;
}

Rational operator+(const Rational &A, const Rational &B) {
    Rational T = A;
    T += B;
    return T;
}

Rational operator-(const Rational &A, const Rational &B) {
    Rational T = A;
    T -= B;
    return T;
}

Rational operator*(const Rational &A, const Rational &B) {
    Rational T = A;
    T *= B;
    return T;
}

Rational operator/(const Rational &A, const Rational &B) {
    Rational T = A;
    T /= B;
    return T;
}

bool Rational::operator==(const Rational &A) const {
    return numerator_ == A.numerator_ && denominator_ == A.denominator_;
}

bool Rational::operator!=(const Rational &A) const {
    return !((*this) == A);
}

bool Rational::operator<(const Rational &A) const {
    return numerator_ * A.denominator_ < A.numerator_ * denominator_;
}

bool Rational::operator>(const Rational &A) const {
    return numerator_ * A.denominator_ > A.numerator_ * denominator_;
}

bool Rational::operator<=(const Rational &A) const {
    return !((*this) > A);
}

bool Rational::operator>=(const Rational &A) const{
    return !((*this) < A);
}

BigInteger gcd(const BigInteger &A, const BigInteger &B) {
    BigInteger T1 = A;
    BigInteger T2 = B;
    if (T1 < 0)
        T1 *= -1;
    if (T2 < 0)
        T2 *= -1;
    if (T1 < T2)
        swap(T1, T2);
    if (T2 == 0)
        return T1;
    return gcd(T2, T1 % T2);
}

void Rational::cut_() {
    BigInteger T = gcd(numerator_, denominator_);
    numerator_ /= T;
    denominator_ /= T;
    if (denominator_ < 0) {
        denominator_ *= -1;
        numerator_ *= -1;
    }
}

void swap(BigInteger &A, BigInteger &B) {
    BigInteger X = A;
    A = B;
    B = X;
}

std::string reverse(std::string &t) {
    for (size_t i = 0; i < t.size() / 2; ++i)
        swap(t[i], t[t.size() - i - 1]);
    return t;
}

void swap(char &c1, char &c2) {
    char c = c1;
    c1 = c2;
    c2 = c;
}

#endif //UNTITLED_RATIONAL_H
