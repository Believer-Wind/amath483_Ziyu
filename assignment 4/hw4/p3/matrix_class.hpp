#ifndef MATRIX_CLASS_HPP
#define MATRIX_CLASS_HPP

#include <vector>
#include <stdexcept>
#include <iostream>
#include <cmath>

// Simple row-major matrix class
template <typename T>
class Matrix {
private:
    size_t rows_, cols_;
    std::vector<T> data_;

public:
    // Construct an uninitialized m×n matrix
    Matrix(size_t m, size_t n)
      : rows_(m), cols_(n), data_(m * n)
    {}

    // Access element (i,j) with bounds checking
    T& operator()(size_t i, size_t j) {
        if (i >= rows_ || j >= cols_)
            throw std::out_of_range("Index out of range");
        return data_[i * cols_ + j];
    }
    const T& operator()(size_t i, size_t j) const {
        if (i >= rows_ || j >= cols_)
            throw std::out_of_range("Index out of range");
        return data_[i * cols_ + j];
    }

    // Print matrix rows, elements separated by spaces
    void print(const std::string& name = "") const {
        if (!name.empty()) std::cout << name << "\n";
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j)
                std::cout << (*this)(i,j) << " ";
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    // (+5) Transpose: returns an n×m matrix
    Matrix<T> transpose() const {
        Matrix<T> B(cols_, rows_);
        for (size_t i = 0; i < rows_; ++i)
            for (size_t j = 0; j < cols_; ++j)
                B(j,i) = (*this)(i,j);
        return B;
    }

    // (+5) Infinity norm: max row-sum of absolute values
    T infinityNorm() const {
        T maxSum = T();
        for (size_t i = 0; i < rows_; ++i) {
            T sum = T();
            for (size_t j = 0; j < cols_; ++j)
                sum += std::abs((*this)(i,j));
            if (sum > maxSum) maxSum = sum;
        }
        return maxSum;
    }

    // (+5) Matrix multiplication
    Matrix<T> operator*(const Matrix<T>& O) const {
        if (cols_ != O.rows_)
            throw std::invalid_argument("Dimension mismatch for multiplication");
        Matrix<T> C(rows_, O.cols_);
        for (size_t i = 0; i < rows_; ++i)
            for (size_t j = 0; j < O.cols_; ++j) {
                T s = T();
                for (size_t k = 0; k < cols_; ++k)
                    s += (*this)(i,k) * O(k,j);
                C(i,j) = s;
            }
        return C;
    }

    // (+5) Matrix addition
    Matrix<T> operator+(const Matrix<T>& O) const {
        if (rows_ != O.rows_ || cols_ != O.cols_)
            throw std::invalid_argument("Dimension mismatch for addition");
        Matrix<T> C(rows_, cols_);
        for (size_t i = 0; i < rows_; ++i)
            for (size_t j = 0; j < cols_; ++j)
                C(i,j) = (*this)(i,j) + O(i,j);
        return C;
    }
};

#endif // MATRIX_CLASS_HPP