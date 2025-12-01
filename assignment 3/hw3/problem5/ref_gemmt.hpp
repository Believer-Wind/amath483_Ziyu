#ifndef REF_GEMMT_HPP
#define REF_GEMMT_HPP

#include <vector>

// Level‑3
template <typename T>
void gemm(T a,
          const std::vector<std::vector<T>>& A,
          const std::vector<std::vector<T>>& B,
          T b,
          std::vector<std::vector<T>>& C);

#endif