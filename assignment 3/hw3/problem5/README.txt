# README.txt

# 1. Compile object files (PIC)
g++ -c -fPIC ref_daxpy.cpp  -o ref_daxpy.o
g++ -c -fPIC ref_dgemv.cpp  -o ref_dgemv.o
g++ -c -fPIC ref_dgemm.cpp  -o ref_dgemm.o
g++ -c -fPIC ref_axpyt.cpp  -o ref_axpyt.o
g++ -c -fPIC ref_gemvt.cpp  -o ref_gemvt.o
g++ -c -fPIC ref_gemmt.cpp  -o ref_gemmt.o

# 2. Link shared library
g++ -shared -o librefBLAS.so \
    ref_daxpy.o ref_dgemv.o ref_dgemm.o \
    ref_axpyt.o ref_gemvt.o ref_gemmt.o

# 3. Compile test executable
g++ -std=c++17 -I. -L. test_refblas.cpp -o testRefBLAS -lrefBLAS

# 4. Run tests
LD_LIBRARY_PATH=. ./testRefBLAS
