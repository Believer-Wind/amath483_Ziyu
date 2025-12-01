#ifndef FILE_SWAPS_HPP
#define FILE_SWAPS_HPP

#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>

// Swap row i and row j in an nRows×nCols matrix stored in 'file' in
// column-major order.  'file' must be open in ios::in|ios::out|ios::binary mode.
inline void swapRowsInFile(std::fstream &file,
                           int nRows, int nCols,
                           int i, int j)
{
    if (i < 0 || i >= nRows || j < 0 || j >= nRows)
        throw std::out_of_range("swapRows: row index out of range");

    constexpr std::streamoff elemSize = sizeof(double);
    double temp;

    for (int col = 0; col < nCols; ++col) {
        // compute byte offsets for (i,col) and (j,col)
        std::streamoff off_i = (std::streamoff(col) * nRows + i) * elemSize;
        std::streamoff off_j = (std::streamoff(col) * nRows + j) * elemSize;

        // read A(i,col)
        file.seekg(off_i, std::ios::beg);
        file.read(reinterpret_cast<char*>(&temp), elemSize);

        // read A(j,col) into a second buffer
        double tmp2;
        file.seekg(off_j, std::ios::beg);
        file.read(reinterpret_cast<char*>(&tmp2), elemSize);

        // write tmp2 into position i
        file.seekp(off_i, std::ios::beg);
        file.write(reinterpret_cast<const char*>(&tmp2), elemSize);

        // write temp into position j
        file.seekp(off_j, std::ios::beg);
        file.write(reinterpret_cast<const char*>(&temp), elemSize);
    }
}

// Swap column i and column j in an nRows×nCols matrix stored in 'file'.
inline void swapColsInFile(std::fstream &file,
                           int nRows, int nCols,
                           int i, int j)
{
    if (i < 0 || i >= nCols || j < 0 || j >= nCols)
        throw std::out_of_range("swapCols: column index out of range");

    constexpr std::streamoff elemSize = sizeof(double);
    // allocate buffers for full column
    std::vector<double> col_i(nRows), col_j(nRows);

    std::streamoff off_i = std::streamoff(i) * nRows * elemSize;
    std::streamoff off_j = std::streamoff(j) * nRows * elemSize;

    // read column i
    file.seekg(off_i, std::ios::beg);
    file.read(reinterpret_cast<char*>(col_i.data()), nRows*elemSize);

    // read column j
    file.seekg(off_j, std::ios::beg);
    file.read(reinterpret_cast<char*>(col_j.data()), nRows*elemSize);

    // write col_j into column i
    file.seekp(off_i, std::ios::beg);
    file.write(reinterpret_cast<const char*>(col_j.data()), nRows*elemSize);

    // write col_i into column j
    file.seekp(off_j, std::ios::beg);
    file.write(reinterpret_cast<const char*>(col_i.data()), nRows*elemSize);
}

#endif // FILE_SWAPS_HPP