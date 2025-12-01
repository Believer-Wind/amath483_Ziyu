#ifndef MM_LOOP_PERMUTATIONS_HPP
#define MM_LOOP_PERMUTATIONS_HPP

#include <vector>

// Loop permutation: j-k-i order
template <typename T>
void mm_jki(T a, const std::vector<T>& A,
            const std::vector<T>& B, T b,
            std::vector<T>& C, int m, int p, int n)
{
    // C = b*C initially
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            C[i*n + j] *= b;

    // j-k-i ordering
    for (int k = 0; k < p; ++k)
        for (int j = 0; j < n; ++j)
            for (int i = 0; i < m; ++i)
                C[i*n + j] += a * A[i*p + k] * B[k*n + j];
}

// Loop permutation: k-i-j order
template <typename T>
void mm_kij(T a, const std::vector<T>& A,
            const std::vector<T>& B, T b,
            std::vector<T>& C, int m, int p, int n)
{
    // C = b*C initially
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            C[i*n + j] *= b;

    // k-i-j ordering
    for (int k = 0; k < p; ++k)
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                C[i*n + j] += a * A[i*p + k] * B[k*n + j];
}

#endif // MM_LOOP_PERMUTATIONS_HPP