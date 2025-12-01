// test_daxpy.cpp

#include "ref_daxpy.hpp"
#include <iostream>
#include <vector>
#include <cmath>

// helper for nearly‐equal comparison
bool almost_equal(double a, double b, double tol = 1e-12) {
    return std::fabs(a - b) < tol;
}

int main(){
    // 1) define a simple test case
    double alpha = 2.0;
    std::vector<double> x{1.0, 2.0, 3.0, -1.5};
    std::vector<double> y{4.0, 5.0, -2.0,  0.5};

    // 2) compute expected result: y_i_new = alpha*x_i + y_i
    std::vector<double> y_expected(y.size());
    for (size_t i = 0; i < x.size(); ++i)
        y_expected[i] = alpha * x[i] + y[i];

    // 3) call my daxpy
    std::vector<double> y_test = y; 
    daxpy(alpha, x, y_test);

    // 4) compare elementwise
    bool pass = true;
    for (size_t i = 0; i < y.size(); ++i) {
        if (!almost_equal(y_test[i], y_expected[i])) {
            std::cerr << "Mismatch at index " << i 
                      << ": got " << y_test[i]
                      << ", expected " << y_expected[i] << "\n";
            pass = false;
        }
    }

    if (pass) {
        std::cout << "test_daxpy: PASS\n";
        return 0;
    } else {
        std::cout << "test_daxpy: FAIL\n";
        return 1;
    }
}
