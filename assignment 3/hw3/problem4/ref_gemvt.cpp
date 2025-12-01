#include "ref_gemvt.hpp"
#include <stdexcept>

// Template implementation
template <typename T>
void gemv(T a,
          const std::vector<std::vector<T>>& A,
          const std::vector<T>& x,
          T b,
          std::vector<T>& y)
{
    int m = A.size();
    if (m == 0) return;
    int n = A[0].size();
    if (x.size() != n || y.size() != m)
        throw std::invalid_argument("Gemv: Dimension Mismatch");

    for (int i = 0; i < m; ++i) 
    {
        T sum = T(0);
        for (int j = 0; j < n; ++j)
        {
            sum += A[i][j] * x[j];
        }
        y[i] = a * sum + b * y[i];
    }
}

// Explicit instantiations
template void gemv<float>(float,
    const std::vector<std::vector<float>>&,
    const std::vector<float>&,
    float,
    std::vector<float>&);

template void gemv<double>(double,
    const std::vector<std::vector<double>>&,
    const std::vector<double>&,
    double,
    std::vector<double>&);