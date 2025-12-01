#include "ref_gemmt.hpp"
#include <stdexcept>

// Template implementation
template <typename T>
void gemm(T a,
          const std::vector<std::vector<T>>& A,
          const std::vector<std::vector<T>>& B,
          T b,
          std::vector<std::vector<T>>& C)
{
    int m = A.size();
    if (m == 0) return;
    int p = A[0].size();
    int pb = B.size();
    int n = static_cast<int>(B[0].size());
    if (p != pb ||
        C.size() != m ||
        C[0].size() != n)
    {
        throw std::invalid_argument("Gemm: Dimension Mismatch");
    }

    for (int i = 0; i < m; ++i) 
    {
        for (int j = 0; j < n; ++j) 
        {
            T sum = T(0);
            for (int k = 0; k < p; ++k) 
            {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = a * sum + b * C[i][j];
        }
    }
}

// Explicit instantiations
template void gemm<int>(int,
    const std::vector<std::vector<int>>&,
    const std::vector<std::vector<int>>&,
    int,
    std::vector<std::vector<int>>&);

template void gemm<float>(float,
    const std::vector<std::vector<float>>&,
    const std::vector<std::vector<float>>&,
    float,
    std::vector<std::vector<float>>&);

template void gemm<double>(double,
    const std::vector<std::vector<double>>&,
    const std::vector<std::vector<double>>&,
    double,
    std::vector<std::vector<double>>&);