#include <iostream>
#include <vector>

using std::vector;

// Matrix addition
template <typename T>
vector<vector<T>> addMatrix(const vector<vector<T>>& A,
                            const vector<vector<T>>& B) {
    int n = A.size(), m = A[0].size();
    vector<vector<T>> C(n, vector<T>(m));
    for(int i=0;i<n;i++) for(int j=0;j<m;j++)
        C[i][j] = A[i][j] + B[i][j];
    return C;
}

// Matrix subtraction
template <typename T>
vector<vector<T>> subtractMatrix(const vector<vector<T>>& A,
                                 const vector<vector<T>>& B) {
    int n = A.size(), m = A[0].size();
    vector<vector<T>> C(n, vector<T>(m));
    for(int i=0;i<n;i++) for(int j=0;j<m;j++)
        C[i][j] = A[i][j] - B[i][j];
    return C;
}

// Standard multiply (base case)
template <typename T>
vector<vector<T>> standardMultiply(const vector<vector<T>>& A,
                                   const vector<vector<T>>& B) {
    int n = A.size();
    vector<vector<T>> C(n, vector<T>(n, 0));
    for(int i=0;i<n;i++)
        for(int k=0;k<n;k++)
            for(int j=0;j<n;j++)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

// Strassen recursion (pad up to next pow‑2; base case at n<=16)
template <typename T>
vector<vector<T>> strassenMultiply(const vector<vector<T>>& A,
                                   const vector<vector<T>>& B) {
    int n = A.size();
    if (n <= 16) return standardMultiply(A, B);

    // pad to next power of two
    int m = 1;
    while (m < n) m <<= 1;
    if (m != n) {
        vector<vector<T>> A_p(m, vector<T>(m, 0)), B_p(m, vector<T>(m, 0));
        for(int i=0;i<n;i++) for(int j=0;j<n;j++) {
            A_p[i][j] = A[i][j];
            B_p[i][j] = B[i][j];
        }
        auto Cbig = strassenMultiply(A_p, B_p);
        vector<vector<T>> C(n, vector<T>(n));
        for(int i=0;i<n;i++) for(int j=0;j<n;j++)
            C[i][j] = Cbig[i][j];
        return C;
    }

    int k = n/2;
    // split A and B into 4 submatrices
    vector<vector<T>> A11(k, vector<T>(k)), A12(k, vector<T>(k)),
                    A21(k, vector<T>(k)), A22(k, vector<T>(k));
    vector<vector<T>> B11(k, vector<T>(k)), B12(k, vector<T>(k)),
                    B21(k, vector<T>(k)), B22(k, vector<T>(k));
    for(int i=0; i<k; i++) for(int j=0; j<k; j++) {
        A11[i][j] = A[i][j];    A12[i][j] = A[i][j+k];
        A21[i][j] = A[i+k][j];  A22[i][j] = A[i+k][j+k];
        B11[i][j] = B[i][j];    B12[i][j] = B[i][j+k];
        B21[i][j] = B[i+k][j];  B22[i][j] = B[i+k][j+k];
    }
    // 7 products
    auto M1 = strassenMultiply(addMatrix(A11,A22), addMatrix(B11,B22));
    auto M2 = strassenMultiply(addMatrix(A21,A22), B11);
    auto M3 = strassenMultiply(A11, subtractMatrix(B12,B22));
    auto M4 = strassenMultiply(A22, subtractMatrix(B21,B11));
    auto M5 = strassenMultiply(addMatrix(A11,A12), B22);
    auto M6 = strassenMultiply(subtractMatrix(A21,A11), addMatrix(B11,B12));
    auto M7 = strassenMultiply(subtractMatrix(A12,A22), addMatrix(B21,B22));

    // recombine
    vector<vector<T>> C11 = addMatrix(subtractMatrix(addMatrix(M1,M4), M5), M7);
    vector<vector<T>> C12 = addMatrix(M3, M5);
    vector<vector<T>> C21 = addMatrix(M2, M4);
    vector<vector<T>> C22 = addMatrix(subtractMatrix(addMatrix(M1,M3), M2), M6);
    vector<vector<T>> C(n, vector<T>(n));
    for(int i=0; i<k; i++) for(int j=0; j<k; j++) {
        C[i][j]     = C11[i][j];
        C[i][j+k]   = C12[i][j];
        C[i+k][j]   = C21[i][j];
        C[i+k][j+k] = C22[i][j];
    }
    return C;
}

// (optional) print matrix for debugging
template <typename T>
void printMatrix(const vector<vector<T>>& M) {
    for(auto &row:M) {
        for(auto &x:row) std::cout<<x<<' ';
        std::cout<<"\n";
    }
}

// explicit instantiations
template vector<vector<double>> strassenMultiply<double>(
    const vector<vector<double>>&, const vector<vector<double>>&);