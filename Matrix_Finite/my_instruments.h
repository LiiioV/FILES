//
// Created by ADMIN on 07.03.2020.
//

#ifndef UNTITLED3_MY_INSTRUMENTS_H
#define UNTITLED3_MY_INSTRUMENTS_H

template<typename T>
void toSwap(T& a, T& b) {
    T c(a);
    a = b;
    b = c;
}

template<unsigned int N,unsigned int A, bool>
struct sqrt_helper {
    static const unsigned int sqrt = sqrt_helper<N, (N + A * A) / (2 * A), ((N + A * A) / (2 * A)) * ((N + A * A) / (2 * A)) <= N &&
                                        N < ((N + A * A) / (2 * A) + 1) * ((N + A * A) / (2 * A) + 1)>::sqrt;
};

template<unsigned int N,unsigned int A>
struct sqrt_helper<N, A, true> {
    static const unsigned int sqrt = A;
};

template<unsigned int N>
struct compile_sqrt {
    static const unsigned int sqrt = sqrt_helper<N, N + 1, false>::sqrt;
};

template<bool, typename T = void>
struct toAssert {};

template<typename T>
struct toAssert<true, T> {
    static void checker() {};
};

template<unsigned int N, unsigned int K>
struct primality_test {
    static const bool answer = N % K != 0 && primality_test<N, K - 1>::answer;
};

template<unsigned int N>
struct primality_test<N, 2> {
    static const bool answer = N % 2 != 0;
};

template<unsigned int N>
struct is_Prime {
    static const bool answer = primality_test<N, compile_sqrt<N>::sqrt>::answer;
};


template<typename A, typename B>
struct is_Same {
    static const bool answer = false;
};

template<typename A>
struct is_Same<A, A> {
    static const bool answer = true;
};

#endif //UNTITLED3_MY_INSTRUMENTS_H
