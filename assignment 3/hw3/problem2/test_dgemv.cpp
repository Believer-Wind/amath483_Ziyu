#include "ref_dgemv.hpp"
#include <iostream>
#include <vector>
#include <cmath>

bool almost_equal(double a, double b, double tol = 1e-12) {
    return std::fabs(a - b) < tol;
}

int main() {
    // a 2×3 test
    double alpha = 2.0, beta = 0.5;
    std::vector<std::vector<double>> A = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0}
    };
    std::vector<double> x         = {7.0, 8.0, 9.0};
    std::vector<double> y_initial = {10.0, 11.0};

    // compute expected by hand:
    // row0 dot = 1*7 + 2*8 + 3*9 = 50 → y0 = 2*50 + 0.5*10  = 105
    // row1 dot = 4*7 + 5*8 + 6*9 =122 → y1 = 2*122+ 0.5*11 = 249.5
    std::vector<double> y_expected = {105.0, 249.5};

    std::vector<double> y_test = y_initial;
    dgemv(alpha, A, x, beta, y_test);

    // check
    bool pass = true;
    for (size_t i = 0; i < y_test.size(); ++i) {
        if (!almost_equal(y_test[i], y_expected[i])) {
            std::cerr << "Index " << i
                      << ": got "    << y_test[i]
                      << ", expected "<< y_expected[i] << "\n";
            pass = false;
        }
    }

    if (pass) {
        std::cout << "test_dgemv: PASS\n";
        return 0;
    } else {
        std::cout << "test_dgemv: FAIL\n";
        return 1;
    }
}
