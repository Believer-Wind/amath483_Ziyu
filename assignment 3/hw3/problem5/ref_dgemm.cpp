#include "ref_dgemm.hpp"
#include <stdexcept>

void dgemm(double a,
           const std::vector<std::vector<double>>& A,
           const std::vector<std::vector<double>>& B,
           double b,
           std::vector<std::vector<double>>& C)
{
    int m = A.size();
    if (m == 0) return;
    int p = A[0].size();
    int pb = B.size();
    int n = B[0].size();

    // dimension checks
    if (p != pb ||
        C.size()    != m ||
        C[0].size() != n)
    {
        throw std::invalid_argument("Dgemm: Dimension Mismatch");
    }

    for (int i = 0; i < m; ++i) 
    {
        for (int j = 0; j < n; ++j) 
        {
            double sum = 0.0;
            // compute dot product of A[i][*] and B[*][j]
            for (int k = 0; k < p; ++k) 
            {
                sum += A[i][k] * B[k][j];
            }
            // scale and accumulate
            C[i][j] = a * sum + b * C[i][j];
        }
    }
}
