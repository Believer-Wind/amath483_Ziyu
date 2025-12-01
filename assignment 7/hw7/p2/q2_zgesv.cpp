// q2_zgesv.cpp
#include <iostream>
#include <complex>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <chrono>
#include <limits>
#include <cblas.h>
#include <lapacke.h>

using Clock = std::chrono::high_resolution_clock;

// Compute 1-norm of complex matrix A (ma × na, column-major)
double cmatrix_infnorm(int ma, int na, std::complex<double>* a) {
    double inf_norm = 0.0;
    for (int i = 0; i < ma; ++i) {
        double sum = 0.0;
        for (int j = 0; j < na; ++j) {
            sum += std::abs(a[i + j*ma]);
        }
        inf_norm = std::max(inf_norm, sum);
    }
    return inf_norm;
}

// Compute 2-norm of complex vector v (length n)
double vector_2norm(int n, std::complex<double>* v) {
    double s = 0.0;
    for (int i = 0; i < n; ++i) {
        s += std::norm(v[i]);
    }
    return std::sqrt(s);
}

int main() {
    const double meps = std::numeric_limits<double>::epsilon();

    std::cout << "N,Residual,NormalizedError,Time(s)\n";

    for (int n = 16; n <= 8192; n *= 2) {
        int ma = n, na = n, nrhs = 1, lda = n, ldb = n;

        // allocate with malloc as per hint
        auto alloc = [&](size_t cnt){
            return (std::complex<double>*)std::malloc(cnt * sizeof(std::complex<double>));
        };
        std::complex<double>* A   = alloc(ma*na);
        std::complex<double>* A0  = alloc(ma*na);
        std::complex<double>* b   = alloc(ma);
        std::complex<double>* z   = alloc(na);
        int* ipiv = (int*)std::malloc(ma * sizeof(int));

        // initialize A with random complex entries, diagonal scaled
        std::srand(0);
        int k = 0;
        for (int j = 0; j < na; ++j) {
            for (int i = 0; i < ma; ++i) {
                double re = 0.5 - double(std::rand())/RAND_MAX;
                double im = 0.5 - double(std::rand())/RAND_MAX;
                A[k] = std::complex<double>(re, im);
                if (i == j) A[k] *= static_cast<double>(ma);
                ++k;
            }
        }
        // backup A for residual
        std::memcpy(A0, A, ma*na*sizeof(std::complex<double>));

        double a_norm = cmatrix_infnorm(ma, na, A);

        // initialize right-hand side b and initial guess z=b
        std::srand(1);
        for (int i = 0; i < ma; ++i) {
            double re = 0.5 - double(std::rand())/RAND_MAX;
            double im = 0.5 - double(std::rand())/RAND_MAX;
            b[i] = std::complex<double>(re, im);
            z[i] = b[i];
        }

        // solve A z = b
        auto t0 = Clock::now();
        LAPACKE_zgesv(LAPACK_COL_MAJOR, ma, nrhs,
                      reinterpret_cast<lapack_complex_double*>(A), lda,
                      ipiv,
                      reinterpret_cast<lapack_complex_double*>(z), ldb);
        auto t1 = Clock::now();
        double elapsed = std::chrono::duration<double>(t1 - t0).count();

        // compute residual ||A0*z - b||_2
        double res2 = 0.0;
        for (int i = 0; i < ma; ++i) {
            std::complex<double> tmp(0.0);
            for (int j = 0; j < na; ++j) {
                tmp += A0[i + j*ma] * z[j];
            }
            tmp -= b[i];
            res2 += std::norm(tmp);
        }
        double residual = std::sqrt(res2);

        double z_norm = vector_2norm(ma, z);
        double norm_err = residual / (a_norm * z_norm * meps);

        // output CSV line
        std::cout << n << ","
                  << residual << ","
                  << norm_err << ","
                  << elapsed << "\n";

        // free memory
        std::free(A);
        std::free(A0);
        std::free(b);
        std::free(z);
        std::free(ipiv);
    }

    return 0;
}
