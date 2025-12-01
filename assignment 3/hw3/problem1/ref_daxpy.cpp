// ref_daxpy.cpp

#include "ref_daxpy.hpp"
#include <stdexcept>

void daxpy(double a, const std::vector<double> &x, std::vector<double> &y)
{
    int n = x.size();
    if (y.size() != n)
        throw std::invalid_argument("Daxpy: Dimension Mismatch");
    for (int i = 0; i < n; i++)
        y[i] = a * x[i] + y[i];
}