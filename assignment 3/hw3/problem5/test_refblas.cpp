#include "refBLAS.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

// Tolerance for floating‐point comparisons
constexpr double EPS = 1e-9;

// Compare two vectors
template<typename T>
bool compare_vec(const std::vector<T>& a,
                 const std::vector<T>& b,
                 double tol = EPS) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if constexpr (std::is_floating_point_v<T>) {
            if (std::fabs(a[i] - b[i]) > tol) return false;
        } else {
            if (a[i] != b[i]) return false;
        }
    }
    return true;
}

// Compare two matrices
template<typename T>
bool compare_mat(const std::vector<std::vector<T>>& A,
                 const std::vector<std::vector<T>>& B,
                 double tol = EPS) {
    if (A.size() != B.size()) return false;
    for (size_t i = 0; i < A.size(); ++i) {
        if (!compare_vec(A[i], B[i], tol)) return false;
    }
    return true;
}

// Helper to print PASS/FAIL
void report(const std::string& name, bool ok) {
    std::cout << name << ": " << (ok ? "PASS" : "FAIL") << "\n";
}

int main() {
    bool all_ok = true;

    // --- Test daxpy ---
    {
        std::vector<double> x{1.0, 2.0, 3.0}, y{4.0, 5.0, 6.0};
        daxpy(2.0, x, y);
        std::vector<double> expected{6.0, 9.0, 12.0};
        bool ok = compare_vec(y, expected);
        report("daxpy", ok);
        all_ok &= ok;
    }

    // --- Test dgemv ---
    {
        std::vector<std::vector<double>> A{{1,2,3},{4,5,6}};
        std::vector<double> xv{7,8,9}, yv{0,0};
        dgemv(1.0, A, xv, 0.0, yv);
        std::vector<double> expected{1*7+2*8+3*9, 4*7+5*8+6*9};
        bool ok = compare_vec(yv, expected);
        report("dgemv", ok);
        all_ok &= ok;
    }

    // --- Test dgemm ---
    {
        std::vector<std::vector<double>> A{{1,2,3},{4,5,6}};
        std::vector<std::vector<double>> B{{1,4},{2,5},{3,6}};
        std::vector<std::vector<double>> C{{0,0},{0,0}};
        dgemm(1.0, A, B, 0.0, C);
        std::vector<std::vector<double>> expected{
            {1*1 + 2*2 + 3*3, 1*4 + 2*5 + 3*6},
            {4*1 + 5*2 + 6*3, 4*4 + 5*5 + 6*6}
        };
        bool ok = compare_mat(C, expected);
        report("dgemm", ok);
        all_ok &= ok;
    }

    // --- Test templated axpy<double> ---
    {
        std::vector<double> x{1,1,1}, y{2,2,2};
        axpy<double>(3.0, x, y);
        std::vector<double> expected{5,5,5};
        bool ok = compare_vec(y, expected);
        report("axpy<double>", ok);
        all_ok &= ok;
    }

    // --- Test templated gemv<float> ---
    {
        std::vector<std::vector<float>> A{{1,2},{3,4}};
        std::vector<float> x{1,2}, y{0,0};
        gemv<float>(1.0f, A, x, 0.0f, y);
        std::vector<float> expected{1*1+2*2, 3*1+4*2};
        bool ok = compare_vec(y, expected);
        report("gemv<float>", ok);
        all_ok &= ok;
    }

    // --- Test templated gemm<int> ---
    {
        std::vector<std::vector<int>> A{{1,2},{3,4}};
        std::vector<std::vector<int>> B{{5,6},{7,8}}, C{{0,0},{0,0}};
        gemm<int>(1, A, B, 0, C);
        std::vector<std::vector<int>> expected{
            {1*5 + 2*7, 1*6 + 2*8},
            {3*5 + 4*7, 3*6 + 4*8}
        };
        bool ok = compare_mat(C, expected);
        report("gemm<int>", ok);
        all_ok &= ok;
    }

    std::cout << "\nOverall: " << (all_ok ? "ALL PASS" : "SOME FAIL") << "\n";
    return all_ok ? 0 : 1;
}
