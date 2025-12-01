#include "ref_dgemm.hpp"
#include <iostream>
#include <vector>
#include <cmath>

bool almost_equal(double a, double b, double tol = 1e-12) {
    return std::fabs(a - b) < tol;
}

int main() {
    // test matrices (2×2)
    double alpha = 2.0, beta = 0.5;
    std::vector<std::vector<double>> A = 
    {
        {1.0, 2.0},
        {3.0, 4.0}
    };
    std::vector<std::vector<double>> B = 
    {
        {5.0, 6.0},
        {7.0, 8.0}
    };
    std::vector<std::vector<double>> C = 
    {
        { 1.0,  0.0},
        { 0.0, -1.0}
    };
    // Compute expected by hand:
    // A·B = [[1*5+2*7, 1*6+2*8], [3*5+4*7, 3*6+4*8]]
    //     = [[19,22],[43,50]]
    // C_new(0,0) = 2*19 + 0.5*1  = 38  + 0.5  = 38.5
    // C_new(0,1) = 2*22 + 0.5*0  = 44  + 0    = 44
    // C_new(1,0) = 2*43 + 0.5*0  = 86  + 0    = 86
    // C_new(1,1) = 2*50 + 0.5*(-1) = 100 - 0.5 = 99.5
    std::vector<std::vector<double>> expected = {
        {38.5, 44.0},
        {86.0, 99.5}
    };

    // Run your DGEMM
    dgemm(alpha, A, B, beta, C);

    // Check
    bool pass = true;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (!almost_equal(C[i][j], expected[i][j])) {
                std::cerr << "Mismatch at ("<<i<<","<<j<<"): "
                          << C[i][j] << " vs " << expected[i][j] << "\n";
                pass = false;
            }
        }
    }

    if (pass) {
        std::cout << "test_dgemm: PASS\n";
        return 0;
    } else {
        std::cout << "test_dgemm: FAIL\n";
        return 1;
    }
}
