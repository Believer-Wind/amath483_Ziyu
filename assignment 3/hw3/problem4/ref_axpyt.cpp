#include "ref_axpyt.hpp"
#include <stdexcept>

// Template implementation
template <typename T>
void axpy(T a,
          const std::vector<T>& x,
          std::vector<T>& y)
{
    if (x.size() != y.size())
        throw std::invalid_argument("Axpy: Dimension Mismatch");
    int n = x.size();
    for (int i = 0; i < n; ++i) 
    {
        y[i] += a * x[i];
    }
}

// Explicit instantiations
template void axpy<float>(float, const std::vector<float>&, std::vector<float>&);
template void axpy<double>(double, const std::vector<double>&, std::vector<double>&);