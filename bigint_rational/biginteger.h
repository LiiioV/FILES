//
// Created by ADMIN on 19.10.2019.
//

#ifndef UNTITLED_BIGINTEGER_H
#define UNTITLED_BIGINTEGER_H


#include <iostream>
#include <vector>
#include <string>

class BigInteger {
public:
    BigInteger() : stringBuffer_(""), sign_(1) {};

    BigInteger(const BigInteger &A) : stringBuffer_(A.stringBuffer_), sign_(A.sign_) {};

    BigInteger(int a);

    BigInteger(const std::string &s);

    void read(const std::string &s);

    std::string toString() const;

    BigInteger abs() const;

    BigInteger &operator=(const BigInteger &A);

    BigInteger &operator=(int a);

    BigInteger &operator=(const std::string &s);

    explicit operator int();

    explicit operator bool();

    bool operator==(const BigInteger &A) const;

    bool operator!=(const BigInteger &A) const;

    bool operator<=(const BigInteger &A) const;

    bool operator>=(const BigInteger &A) const;

    bool operator>(const BigInteger &A) const;

    bool operator<(const BigInteger &A) const;

    BigInteger &operator+=(const BigInteger &A);

    BigInteger &operator-=(const BigInteger &A);

    BigInteger &operator*=(const BigInteger &A);

    BigInteger &operator/=(const BigInteger &A);

    BigInteger &operator%=(const BigInteger &A);

    BigInteger operator-() const;

    BigInteger &operator++();

    BigInteger &operator--();

    BigInteger operator++(int);

    BigInteger operator--(int);

private:
    std::string stringBuffer_;
    int sign_;

    void clean_();

    void fromInt_(int a);

    void findSign_(int &a);

    void findSign_(char c);

    void findSign_(char c, int &end);

    void fromString_(const std::string &s);

    void makeNumber_(int i);
};

std::istream &operator>>(std::istream &in, BigInteger &A);

std::ostream &operator<<(std::ostream &out, const BigInteger &A);

BigInteger operator+(const BigInteger &A, const BigInteger &B);

BigInteger operator-(const BigInteger &A, const BigInteger &B);

BigInteger operator*(const BigInteger &A, const BigInteger &B);

BigInteger operator/(const BigInteger &A, const BigInteger &B);

BigInteger operator%(const BigInteger &A, const BigInteger &B);

////realisation

std::istream &operator>>(std::istream &in, BigInteger &A) {
    std::string s;
    in >> s;
    A.read(s);
    return in;
}

std::ostream &operator<<(std::ostream &out, const BigInteger &A) {
    std::string s = A.toString();
    out << s;
    return out;
}

void BigInteger::clean_() {
    while (!stringBuffer_.empty() && stringBuffer_.back() == '0')
        stringBuffer_.pop_back();
    if (stringBuffer_.empty())
        sign_ = 1;
}

void BigInteger::findSign_(int &a) {
    sign_ = 2 * (a > 0) - 1;
    a = sign_ * a;
}

void BigInteger::fromInt_(int a) {
    findSign_(a);
    stringBuffer_ = "";
    for (; a > 0; a /= 10)
        stringBuffer_ += a % 10 + '0';
    clean_();
};

void BigInteger::findSign_(char c, int &end) {
    sign_ = 2 * (c != '-') - 1;
    end += (c == '-');
}

void BigInteger::findSign_(char c) {
    int nothing = 0;
    findSign_(c, nothing);
}

void BigInteger::fromString_(const std::string &s) {
    int begin = s.size() - 1;
    int end = 0;
    findSign_(s[0], end);
    stringBuffer_ = "";
    for (int i = begin; i >= end; --i)
        stringBuffer_ += s[i];
    clean_();
}

BigInteger::BigInteger(int a) {
    fromInt_(a);
}

BigInteger::BigInteger(const std::string &s) {
    fromString_(s);
}

BigInteger &BigInteger::operator=(const BigInteger &A) {
    sign_ = A.sign_;
    stringBuffer_ = A.stringBuffer_;
    return *this;
}

BigInteger &BigInteger::operator=(int a) {
    fromInt_(a);
    return *this;
}

BigInteger &BigInteger::operator=(const std::string &s) {
    fromString_(s);
    return *this;
}

void BigInteger::read(const std::string &s) {
    fromString_(s);
}

std::string BigInteger::toString() const {
    std::string s;
    if (sign_ == -1)
        s += '-';
    size_t size = stringBuffer_.size();
    for (size_t i = size; i > 0; --i)
        s += stringBuffer_[i - 1];
    if (stringBuffer_.empty())
        s = "0";
    return s;
}

BigInteger::operator int() {
    int ans = 0;
    int coef = 1;
    size_t size = stringBuffer_.size();
    for (size_t i = 0; i < size; ++i, coef *= 10)
        ans += coef * (stringBuffer_[i] - '0');
    if (sign_ == -1)
        ans = -ans;
    return ans;
}

BigInteger::operator bool() {
    return !stringBuffer_.empty();
}

bool BigInteger::operator==(const BigInteger &A) const {
    return A.sign_ == sign_ && A.stringBuffer_ == stringBuffer_;
}

bool BigInteger::operator!=(const BigInteger &A) const {
    return A.sign_ != sign_ || A.stringBuffer_ != stringBuffer_;
}

bool BigInteger::operator<(const BigInteger &A) const {
    if (sign_ != A.sign_)
        return sign_ == -1;
    if (sign_ == -1)
        return -A < -(*this);
    else {
        int size = stringBuffer_.size();
        int sizeA = A.stringBuffer_.size();
        if (size != sizeA)
            return size < sizeA;
        for (int i = size - 1; i >= 0; --i) {
            if (stringBuffer_[i] != A.stringBuffer_[i])
                return stringBuffer_[i] < A.stringBuffer_[i];
        }
    }
    return false;
}

bool BigInteger::operator>(const BigInteger &A) const {
    return A < *this;
}

bool BigInteger::operator<=(const BigInteger &A) const {
    return !(*this > A);
}

bool BigInteger::operator>=(const BigInteger &A) const {
    return !(*this < A);
}

BigInteger &BigInteger::operator+=(const BigInteger &A) {
    if (sign_ != A.sign_) {
        *this -= (-A);
        return *this;
    }

    //sizes
    size_t size = stringBuffer_.size();
    size_t sizeA = A.stringBuffer_.size();
    size_t MaxSize = size;
    if (MaxSize < sizeA)
        MaxSize = sizeA;

    //adding
    int plusOne = 0;
    int t;
    for (size_t i = 0; i < MaxSize; ++i) {
        if (size <= i)
            stringBuffer_ += '0';
        t = stringBuffer_[i] - '0' + (sizeA > i ? A.stringBuffer_[i] - '0' : 0) + plusOne;
        if (t >= 10) {
            plusOne = 1;
            stringBuffer_[i] = t - 10 + '0';
        } else {
            plusOne = 0;
            stringBuffer_[i] = t + '0';
        }
    }
    if (plusOne == 1)
        stringBuffer_ += '1';
    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &A) {
    if (A.sign_ != sign_) {
        *this += (-A);
        return *this;
    }

    size_t size = stringBuffer_.size();
    size_t sizeA = A.stringBuffer_.size();
    size_t MaxSize = size;
    if (MaxSize < sizeA)
        MaxSize = sizeA;

    int coef = 2 * (abs() > A.abs()) - 1;
    int minusOne = 0;
    int t;
    for (size_t i = 0; i < MaxSize; ++i) {
        if (size <= i)
            stringBuffer_ += '0';
        t = coef * (stringBuffer_[i] - '0' - (sizeA > i ? A.stringBuffer_[i] - '0' : 0)) + minusOne;
        if (t < 0) {
            minusOne = -1;
            stringBuffer_[i] = t + 10 + '0';
        } else {
            minusOne = 0;
            stringBuffer_[i] = t + '0';
        }
    }
    sign_ *= coef;
    clean_();
    return *this;
}

BigInteger &BigInteger::operator*=(const BigInteger &A) {
    if (A.stringBuffer_ == "") {
        *this = 0;
        return *this;
    }

    int size = stringBuffer_.size();
    int sizeA = A.stringBuffer_.size();
    int sizeC;
    BigInteger C = 0;

    for (int i = size; i <= size + sizeA + 2; ++i)
        stringBuffer_ += '0';

    for (int i = size + sizeA - 2; i >= 0; --i) {
        C = 0;
        for (int j = 0; j <= i; ++j) {
            if (j >= size)
                break;
            if (i - j >= sizeA)
                continue;
            C += (stringBuffer_[j] - '0') * (A.stringBuffer_[i - j] - '0');
        }
        sizeC = C.stringBuffer_.size();
        stringBuffer_[i] = '0';
        for (int j = 0; j < sizeC; ++j) {
            stringBuffer_[i + j] += (C.stringBuffer_[j] - '0');
            makeNumber_(i + j);
        }
    }
    sign_ *= A.sign_;
    clean_();
    return *this;
}

BigInteger &BigInteger::operator/=(const BigInteger &A) {
    if (abs() < A.abs()) {
        *this = 0;
        return *this;
    }
    BigInteger C = 0;
    BigInteger T = A.abs();
    int j = 1;
    int size = stringBuffer_.size();

    for (int i = size - 1; i >= 0; --i) {
        C *= 10;
        C += stringBuffer_[i] - '0';
        j = 1;
        while (C >= T * j)
            j++;
        C -= (j - 1) * T;
        stringBuffer_[i] = j - 1 + '0';
    }
    sign_ *= A.sign_;
    clean_();
    return *this;
}

BigInteger &BigInteger::operator%=(const BigInteger &A) {
    *this -= ((*this) / A) * A;
    return *this;
}

BigInteger BigInteger::operator-() const {
    BigInteger T = *this;
    T.sign_ = -1 / sign_;
    return T;
}

BigInteger &BigInteger::operator++() {
    *this += 1;
    return *this;
}

BigInteger &BigInteger::operator--() {
    *this -= 1;
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger A = *this;
    ++(*this);
    return A;
}

BigInteger BigInteger::operator--(int) {
    BigInteger A = *this;
    --(*this);
    return A;
}

BigInteger BigInteger::abs() const {
    if (sign_ < 0) {
        BigInteger T = -(*this);
        return T;
    }
    return *this;
}

BigInteger operator+(const BigInteger &A, const BigInteger &B) {
    BigInteger C = A;
    C += B;
    return C;
}

BigInteger operator-(const BigInteger &A, const BigInteger &B) {
    BigInteger C = A;
    C -= B;
    return C;
}

BigInteger operator*(const BigInteger &A, const BigInteger &B) {
    BigInteger C = A;
    C *= B;
    return C;
}

BigInteger operator/(const BigInteger &A, const BigInteger &B) {
    BigInteger C = A;
    C /= B;
    return C;
}

BigInteger operator%(const BigInteger &A, const BigInteger &B) {
    BigInteger C = A;
    C %= B;
    return C;
}

void BigInteger::makeNumber_(int i) {
    if (stringBuffer_[i] <= '9')
        return;
    while (stringBuffer_[i] > '9') {
        stringBuffer_[i + 1]++;
        stringBuffer_[i] -= 10;
    }
    makeNumber_(i + 1);
}

#endif //UNTITLED_BIGINTEGER_H
