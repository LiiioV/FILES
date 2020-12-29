//
// Created by ADMIN on 07.03.2020.
//

#ifndef UNTITLED3_MATRIX_H
#define UNTITLED3_MATRIX_H

#include "Finite.h"
#include "my_instruments.h"
#include "rational.h"

template<unsigned int N, unsigned int M, typename Field = Rational>
class Matrix {
protected:
    Field **buffer_;
public:
    Matrix();

    Matrix(Field **);

    Matrix(const Field &);

    Matrix(const Matrix<N, M, Field> &);

    Matrix(const std::initializer_list<std::initializer_list<Field>> &);

    ~Matrix();

    Field *&operator[](unsigned int);

    const Field *operator[](unsigned int) const;

    Matrix<N, M, Field> &operator=(const Matrix<N, M, Field> &);

    template<unsigned int K, unsigned int L, typename Field2>
    Matrix<N, M, Field> &operator+=(const Matrix<K, L, Field2> &);

    template<unsigned int K, unsigned int L, typename Field2>
    Matrix<N, M, Field> &operator-=(const Matrix<K, L, Field2> &);

    Matrix<N, M, Field> &operator*=(const Field &);

    template<unsigned int K, unsigned int L, typename Field2>
    Matrix<N, L, Field> operator*(const Matrix<K, L, Field2> &) const;

    template<unsigned int K, unsigned int L, typename Field2>
    bool operator==(const Matrix<K, L, Field2> &) const;

    template<unsigned int K, unsigned int L, typename Field2>
    bool operator!=(const Matrix<K, L, Field2> &) const;

    Matrix<M, N, Field> transposed() const;

    Field trace() const;

    unsigned int rank() const;

    Field det() const;

    Matrix<N, M, Field> &invert();

    Matrix<N, M, Field> inverted() const;

    std::vector<Field> getRow(unsigned int);

    std::vector<Field> getColumn(unsigned int);

    Field **getMatrix() const;

    Matrix<N, M, Field> upperTriangle() const;

    Matrix<N, M, Field> &toUpperTriangle();

private:
    void copyBuild_(Field **);

    void copyBuild_(const Field &A = Field());

    void deleteBuffer_();

    void isField_() const;

    template<unsigned int K, unsigned int L, typename Field2>
    static Matrix<K / 2 + K % 2, L / 2 + L % 2, Field2> *makeEvenAndDivide_(const Matrix<K, L, Field2> &);

    template<unsigned int K, unsigned int L, typename Field2>
    static Matrix<2 * K, 2 * L, Field2> *conquer_(Matrix<K, L, Field2> *);
};

template<unsigned int N, unsigned int M, typename Field = Rational, unsigned int K, unsigned int L, typename Field2 = Rational>
Matrix<N, M, Field> operator+(const Matrix<N, M, Field> &, const Matrix<K, L, Field2> &);

template<unsigned int N, unsigned int M, typename Field = Rational, unsigned int K, unsigned int L, typename Field2 = Rational>
Matrix<N, M, Field> operator-(const Matrix<N, M, Field> &, const Matrix<K, L, Field2> &);

template<unsigned int N, unsigned int M, typename Field = Rational>
Matrix<N, M, Field> operator*(const Field &, const Matrix<N, M, Field> &);

template<typename Field>
void deleteTwoPointer(Field **, unsigned int);

template<unsigned int N, unsigned int M, typename Field>
void Matrix<N, M, Field>::copyBuild_(Field **A) {
    buffer_ = new Field *[N];
    for (size_t i = 0; i < N; ++i) {
        buffer_[i] = new Field[M];
        for (size_t j = 0; j < M; ++j)
            buffer_[i][j] = A[i][j];
    }
}

template<unsigned int N, unsigned int M, typename Field>
void Matrix<N, M, Field>::copyBuild_(const Field &A) {
    buffer_ = new Field *[N];
    for (size_t i = 0; i < N; ++i) {
        buffer_[i] = new Field[M];
        for (size_t j = 0; j < M; ++j)
            buffer_[i][j] = A;
    }
}


template<unsigned int N, unsigned int M, typename Field>
void Matrix<N, M, Field>::deleteBuffer_() {
    deleteTwoPointer(buffer_, N);
}

template<unsigned int N, unsigned int M, typename Field>
void Matrix<N, M, Field>::isField_() const {
    Field(1) / Field(1);
}

template<unsigned int N, unsigned int M, typename Field>
template<unsigned int K, unsigned int L, typename Field2>
Matrix<K / 2 + K % 2, L / 2 + L % 2, Field2> *Matrix<N, M, Field>::makeEvenAndDivide_(const Matrix<K, L, Field2> &A) {
    Matrix<K / 2 + K % 2, L / 2 + L % 2, Field2> *ans = new Matrix<K / 2 + K % 2, L / 2 + L % 2, Field2>[4];
    for (size_t i = 0; i < K + K % 2; ++i)
        for (size_t j = 0; j < L + L % 2; ++j)
            ans[2 * (i >= K / 2 + K % 2) + (j >= L / 2 + L % 2)][i % (K / 2 + K % 2)][j % (L / 2 + L % 2)] = ((i >= K || j >= L) ? Field(0) : A[i][j]);
    return ans;
};

template<unsigned int N, unsigned int M, typename Field>
template<unsigned int K, unsigned int L, typename Field2>
Matrix<2 * K, 2 * L, Field2> *Matrix<N, M, Field>::conquer_(Matrix<K, L, Field2> *part) {
    Matrix<2 * K, 2 * L, Field2> *ans = new Matrix<2 * K, 2 * L, Field2>();
    for (size_t i = 0; i < 2 * K; ++i)
        for (size_t j = 0; j < 2 * L; ++j)
            (*ans)[i][j] = part[2 * (i >= K) + (j >= L)][i % K][j % L];
    return ans;
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field> Matrix<N, M, Field>::upperTriangle() const {
    Matrix<N, M, Field> A = *this;
    A.toUpperTriangle();
    return A;
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field> &Matrix<N, M, Field>::toUpperTriangle() {
    unsigned int rank = 0;
    bool sign = true;
    for (size_t j = 0; j < M; ++j) {
        size_t i = rank;
        for (; i < N; ++i) {
            if ((*this)[i][j] != 0)
                break;
        }
        if (i != N) {
            if (i != rank) {
                toSwap((*this)[i], (*this)[rank]);
                sign ^= true;
            }
            for (size_t i = rank + 1; i < N; ++i) {
                Field coef = (*this)[i][j] / (*this)[rank][j];
                for (size_t k = j; k < M; ++k)
                    (*this)[i][k] -= coef * (*this)[rank][k];
            }
            rank++;
        }
    }
    if (!sign) {
        for (size_t i = N - 1; i < M; ++i)
            (*this)[N - 1][i] *= Field(-1);
    }
    return *this;
}


template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field>::Matrix() {
    isField_();
    copyBuild_();
    if (N == M)
        for (size_t i = 0; i < N; ++i)
            buffer_[i][i] = Field(1);
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field>::Matrix(Field **A) {
    isField_();
    copyBuild_(A);
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field>::Matrix(const Field &A) {
    isField_();
    copyBuild_(A);
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field>::Matrix(const Matrix<N, M, Field> &A) {
    isField_();
    copyBuild_(A.buffer_);
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field>::Matrix(const std::initializer_list<std::initializer_list<Field>> &A) {
    isField_();
    size_t i = 0;
    buffer_ = new Field *[N];
    for (auto col : A) {
        buffer_[i] = new Field[M];
        size_t j = 0;
        for (auto element : col) {
            buffer_[i][j] = element;
            ++j;
        }
        ++i;
    }
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field>::~Matrix() {
    deleteBuffer_();
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field> &Matrix<N, M, Field>::operator=(const Matrix<N, M, Field> &A) {
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
            (*this)[i][j] = A[i][j];
    return *this;
}


template<unsigned int N, unsigned int M, typename Field>
Field *&Matrix<N, M, Field>::operator[](unsigned int i) {
    return buffer_[i];
}

template<unsigned int N, unsigned int M, typename Field>
const Field *Matrix<N, M, Field>::operator[](unsigned int i) const {
    const Field *ans = buffer_[i];
    return ans;
}


template<unsigned int N, unsigned int M, typename Field>
template<unsigned int K, unsigned int L, typename Field2>
Matrix<N, M, Field> &Matrix<N, M, Field>::operator+=(const Matrix<K, L, Field2> &A) {
    toAssert<N == K && M == L && is_Same<Field, Field2>::answer>::checker();
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
            (*this)[i][j] += A[i][j];
    return *this;
}


template<unsigned int N, unsigned int M, typename Field>
template<unsigned int K, unsigned int L, typename Field2>
Matrix<N, M, Field> &Matrix<N, M, Field>::operator-=(const Matrix<K, L, Field2> &A) {
    toAssert<N == K && M == L && is_Same<Field, Field2>::answer>::checker();
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
            (*this)[i][j] -= A[i][j];
    return *this;
}


template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field> &Matrix<N, M, Field>::operator*=(const Field &A) {
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
            (*this)[i][j] *= A;
    return *this;
}


template<unsigned int N, unsigned int M, typename Field>
template<unsigned int K, unsigned int L, typename Field2>
Matrix<N, L, Field> Matrix<N, M, Field>::operator*(const Matrix<K, L, Field2> &A) const {
    toAssert<M == K && is_Same<Field, Field2>::answer>::checker();
    if (N * M * L == 0)
        return Matrix<N, L, Field>(Field(0));
    if (N == 1 || M == 1 || L == 1) {
        Matrix<N, L, Field> ans(Field(0));
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < L; ++j)
                for (size_t k = 0; k < M; ++k)
                    ans[i][j] += (*this)[i][k] * A[k][j];
        return ans;
    }
    Matrix<N / 2 + N % 2, M / 2 + M % 2, Field> *first = makeEvenAndDivide_(*this);
    Matrix<M / 2 + M % 2, L / 2 + L % 2, Field> *second = makeEvenAndDivide_(A);

    Matrix<N / 2 + N % 2, L / 2 + L % 2, Field> *mini = new Matrix<N / 2 + N % 2, L / 2 + L % 2, Field>[7];

    mini[0] = first[0] * (second[1] - second[3]);
    mini[1] = (first[0] + first[1]) * second[3];
    mini[2] = (first[2] + first[3]) * second[0];
    mini[3] = first[3] * (second[2] - second[0]);
    mini[4] = (first[0] + first[3]) * (second[0] + second[3]);
    mini[5] = (first[1] - first[3]) * (second[2] + second[3]);
    mini[6] = (first[0] - first[2]) * (second[0] + second[1]);

    delete[] first;
    delete[] second;

    Matrix<N / 2 + N % 2, L / 2 + L % 2, Field> *part = new Matrix<N / 2 + N % 2, L / 2 + L % 2, Field>[4];
    part[0] = mini[3] - mini[1] + mini[4] + mini[5];
    part[1] = mini[0] + mini[1];
    part[2] = mini[2] + mini[3];
    part[3] = mini[0] - mini[2] + mini[4] - mini[6];

    delete[] mini;

    Matrix<2 * (N / 2 + N % 2), 2 * (L / 2 + L % 2), Field> *even = conquer_(part);

    delete[] part;

    Matrix<N, L, Field> ans;
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < L; ++j)
            ans[i][j] = (*even)[i][j];

    delete even;

    return ans;
}

template<unsigned int N, unsigned int M, typename Field>
template<unsigned int K, unsigned int L, typename Field2>
bool Matrix<N, M, Field>::operator==(const Matrix<K, L, Field2> &A) const {
    if (K != N || L != M)
        return false;
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
            if ((*this)[i][j] != A[i][j])
                return false;
    return true;
}

template<unsigned int N, unsigned int M, typename Field>
template<unsigned int K, unsigned int L, typename Field2>
bool Matrix<N, M, Field>::operator!=(const Matrix<K, L, Field2> &A) const {
    return !((*this) == A);
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<M, N, Field> Matrix<N, M, Field>::transposed() const {
    Matrix<M, N, Field> A;
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
            A[j][i] = (*this)[i][j];
    return A;
}

template<unsigned int N, unsigned int M, typename Field>
Field Matrix<N, M, Field>::trace() const {
    toAssert<N == M>::checker();
    Field trace;
    for (size_t i = 0; i < N; ++i)
        trace += (*this)[i][i];
    return trace;
}

template<unsigned int N, unsigned int M, typename Field>
unsigned int Matrix<N, M, Field>::rank() const {
    Matrix<N, M, Field> A = upperTriangle();
    unsigned int rank = 0;
    bool isEmpty;
    for (; rank < N; ++rank) {
        isEmpty = true;
        for (size_t j = 0; j < M; ++j) {
            if (A[rank][j] != Field(0)) {
                isEmpty = false;
                break;
            }
        }
        if (isEmpty)
            break;
    }
    return rank;
}

template<unsigned int N, unsigned int M, typename Field>
Field Matrix<N, M, Field>::det() const {
    toAssert<N == M>::checker();
    Matrix<N, M, Field> A = upperTriangle();
    Field det = Field(1);
    for (size_t i = 0; i < N; ++i)
        det *= A[i][i];
    return det;
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field> &Matrix<N, M, Field>::invert() {
    toAssert<N == M>::checker();
    Matrix<N, 2 * N, Field> A;
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            A[i][j] = (*this)[i][j];
    for (size_t i = 0; i < N; ++i)
        A[i][N + i] = Field(1);
    A.toUpperTriangle();

    for (size_t i = 0; i < N; ++i) {
        if (A[N - i - 1][N - i - 1] != Field(1)) {
            for (size_t j = N - i; j < 2 * N; ++j)
                A[N - i - 1][j] /= A[N - i - 1][N - i - 1];
            A[N - i - 1][N - i - 1] = Field(1);
        }

        for (size_t t = 0; t < N - i - 1; ++t) {
            for (size_t j = N - i; j < 2 * N; ++j)
                A[t][j] -= A[t][N - i - 1] * A[N - i - 1][j];
            A[t][N - i - 1] = 0;
        }
    }

    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            (*this)[i][j] = A[i][N + j];

    return *this;
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field> Matrix<N, M, Field>::inverted() const {
    Matrix<N, M, Field> A = *this;
    A.invert();
    return A;
}

template<unsigned int N, unsigned int M, typename Field>
std::vector<Field> Matrix<N, M, Field>::getColumn(unsigned int j) {
    std::vector<Field> column;
    for (size_t i = 0; i < N; ++i)
        column.push_back((*this)[i][j]);
    return column;
}

template<unsigned int N, unsigned int M, typename Field>
std::vector<Field> Matrix<N, M, Field>::getRow(unsigned int i) {
    std::vector<Field> row;
    for (size_t j = 0; j < M; ++j)
        row.push_back((*this)[i][j]);
    return row;
}

template<unsigned int N, unsigned int M, typename Field>
Field **Matrix<N, M, Field>::getMatrix() const {
    Field **ans = new Field *[N];
    for (size_t i = 0; i < N; ++i) {
        ans[i] = new Field[M];
        for (size_t j = 0; j < M; ++j)
            ans[i][j] = (*this)[i][j];
    }
    return ans;
}

template<unsigned int N, unsigned int M, typename Field, unsigned int K, unsigned int L, typename Field2>
Matrix<N, M, Field> operator+(const Matrix<N, M, Field> &A, const Matrix<K, L, Field2> &B) {
    Matrix<N, M, Field> X = A;
    X += B;
    return X;
}

template<unsigned int N, unsigned int M, typename Field, unsigned int K, unsigned int L, typename Field2>
Matrix<N, M, Field> operator-(const Matrix<N, M, Field> &A, const Matrix<K, L, Field2> &B) {
    Matrix<N, M, Field> X = A;
    X -= B;
    return X;
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field> operator*(const Field &lambda, const Matrix<N, M, Field> &A) {
    Matrix<N, M, Field> B = A;
    B *= lambda;
    return B;
}

template<typename Field>
void deleteTwoPointer(Field **buff, unsigned int column) {
    for (size_t i = 0; i < column; ++i)
        delete[] buff[i];
    delete[] buff;
}

template<unsigned int N, typename Field = Rational>
class SquareMatrix : public Matrix<N, N, Field> {
public:
    SquareMatrix() : Matrix<N, N, Field>() {};

    SquareMatrix(Field **A) : Matrix<N, N, Field>(A) {};

    SquareMatrix(const Matrix<N, N, Field> &A) : Matrix<N, N, Field>(A) {};

    SquareMatrix(const SquareMatrix<N, Field> &A) : Matrix<N, N, Field>(A) {};

    SquareMatrix(const std::initializer_list<std::initializer_list<Field>> &A) : Matrix<N, N, Field>(A) {};

    template<unsigned int K, unsigned int L, typename Field2>
    SquareMatrix<N, Field> &operator*=(const Matrix<K, L, Field2> &);
};

template<unsigned int N, typename Field>
template<unsigned int K, unsigned int L, typename Field2>
SquareMatrix<N, Field> &SquareMatrix<N, Field>::operator*=(const Matrix<K, L, Field2> &A) {
    toAssert<K == N && K == L && is_Same<Field, Field2>::answer>::checker();
    SquareMatrix<N, Field> X = *this;
    (*this) = X * A;
    return *this;
}

#endif //UNTITLED3_MATRIX_H
