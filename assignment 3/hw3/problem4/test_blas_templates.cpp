// Unit tests for templated BLAS routines: axpy, gemv, gemm

#include "ref_axpyt.hpp"
#include "ref_gemvt.hpp"
#include "ref_gemmt.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <typeinfo>

// Helper for floating-point comparison
template<typename T>
bool almost_equal(T a, T b, T tol = (T)1e-8) 
{
    return std::fabs(a - b) <= tol;
}

// Test Level-1 axpy
template<typename T>
bool test_axpy() 
{
    T alpha = (T)2;
    std::vector<T> x { (T)1, (T)2, (T)3 };
    std::vector<T> y { (T)4, (T)5, (T)6 };
    std::vector<T> y_exp { (T)6, (T)9, (T)12 };

    axpy(alpha, x, y);

    for (int i = 0; i < (int)x.size(); ++i) 
    {
        if (!almost_equal(y[i], y_exp[i])) 
        {
            std::cerr << "axpy<" << typeid(T).name() << "> failed at " << i
                      << ": got " << y[i]
                      << ", expected " << y_exp[i] << "\n";
            return false;
        }
    }
    return true;
}

// Test Level-2 gemv
template<typename T>
bool test_gemv() 
{
    T alpha = (T)2, beta = (T)0.5;
    std::vector<std::vector<T>> A = {
        { (T)1, (T)2, (T)3 },
        { (T)4, (T)5, (T)6 }
    };
    std::vector<T> x = { (T)7, (T)8, (T)9 };
    std::vector<T> y = { (T)10, (T)11 };
    std::vector<T> y_exp = { (T)105, (T)249.5 };

    gemv(alpha, A, x, beta, y);

    for (int i = 0; i < (int)y.size(); ++i) 
    {
        if (!almost_equal(y[i], y_exp[i])) 
        {
            std::cerr << "gemv<" << typeid(T).name() << "> failed at " << i
                      << ": got " << y[i]
                      << ", expected " << y_exp[i] << "\n";
            return false;
        }
    }
    return true;
}

// Test Level-3 gemm
template<typename T>
bool test_gemm() 
{
    T alpha = (T)2, beta = (T)0.5;
    std::vector<std::vector<T>> A = {
        { (T)1, (T)2 },
        { (T)3, (T)4 }
    };
    std::vector<std::vector<T>> B = {
        { (T)5, (T)6 },
        { (T)7, (T)8 }
    };
    std::vector<std::vector<T>> C = {
        { (T)1,  (T)0 },
        { (T)0, (T)-1 }
    };
    std::vector<std::vector<T>> C_exp;
    if constexpr (std::is_same_v<T,int>) {
        // integer instantiation
        C_exp = {{ 38, 44 },
                 { 86, 100 }};
    } else {
        // float/double instantiation
        C_exp = {{ (T)38.5, (T)44   },
                 { (T)86,   (T)99.5 }};
    };

    gemm(alpha, A, B, beta, C);

    for (int i = 0; i < 2; ++i) 
    {
        for (int j = 0; j < 2; ++j) 
        {
            if (!almost_equal(C[i][j], C_exp[i][j])) 
            {
                std::cerr << "gemm<" << typeid(T).name() << "> failed at ("
                          << i << "," << j << "): got " << C[i][j]
                          << ", expected " << C_exp[i][j] << "\n";
                return false;
            }
        }
    }
    return true;
}

int main() 
{
    bool ok = true;
    ok &= test_axpy<float>();
    ok &= test_axpy<double>();
    ok &= test_gemv<float>();
    ok &= test_gemv<double>();
    ok &= test_gemm<int>();
    ok &= test_gemm<float>();
    ok &= test_gemm<double>();

    if (ok) 
    {
        std::cout << "All templated BLAS tests PASSED.\n";
        return 0;
    } else 
    {
        std::cout << "Some templated BLAS tests FAILED.\n";
        return 1;
    }
}
