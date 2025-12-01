#include "ref_dgemv.hpp"
#include <stdexcept>

void dgemv(double a,
           const std::vector<std::vector<double>>& A,
           const std::vector<double>& x,
           double b,
           std::vector<double>& y)
{
    int m = A.size();
    if (m == 0) return;
    int n = A[0].size();

    if (x.size() != n || y.size() != m) 
    {
        throw std::invalid_argument("Dgemv: Dimension Mismatch");
    }

    for (int i = 0; i < m; ++i) 
    {
        double sum = 0.0;
        for (size_t j = 0; j < n; ++j) 
        {
            sum += A[i][j] * x[j];
        }
        y[i] = a * sum + b * y[i];
    }
}
