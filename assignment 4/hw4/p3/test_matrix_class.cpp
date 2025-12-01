// The result shows as below: it satisfies my expectation

// Matrix A:
// 1 2 3 
// 4 5 6 

// Matrix B:
// 7 8 
// 9 10 
// 11 12 

// C = A * B:
// 58 64 
// 139 154 

// C^T = transpose(C):
// 58 139 
// 64 154 

// S = C + C^T:
// 116 203 
// 203 308 

// Infinity norm of A: 15

#include "matrix_class.hpp"
#include <iostream>

int main() {
    // Define A (2×3)
    Matrix<int> A(2,3);
    A(0,0)=1; A(0,1)=2; A(0,2)=3;
    A(1,0)=4; A(1,1)=5; A(1,2)=6;
    A.print("Matrix A:");

    // Define B (3×2)
    Matrix<int> B(3,2);
    B(0,0)=7;  B(0,1)=8;
    B(1,0)=9;  B(1,1)=10;
    B(2,0)=11; B(2,1)=12;
    B.print("Matrix B:");

    // Compute C = A * B
    auto C = A * B;
    C.print("C = A * B:");

    // Compute transpose of C
    auto CT = C.transpose();
    CT.print("C^T = transpose(C):");

    // Compute sum C + C^T
    auto S = C + CT;
    S.print("S = C + C^T:");

    // Infinity norm of A
    int infA = A.infinityNorm();
    std::cout << "Infinity norm of A: " << infA << "\n";

    return 0;
}