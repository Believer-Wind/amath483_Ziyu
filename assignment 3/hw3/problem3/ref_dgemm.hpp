#ifndef REF_DGEMM_HPP
#define REF_DGEMM_HPP

#include <vector>

/// A m×k, B k×n, C m×n.
void dgemm(double a,
           const std::vector<std::vector<double>>& A,
           const std::vector<std::vector<double>>& B,
           double b,
           std::vector<std::vector<double>>& C);

#endif
