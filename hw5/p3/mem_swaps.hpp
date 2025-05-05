#include <vector>
#include <cassert>
#include <algorithm>

// Column–major helper: element (r,c) lives at c*nRows + r.
inline std::size_t cmIndex(int r, int c, int nRows)
{
    return static_cast<std::size_t>(c) * nRows + r;
}

void swapRows(std::vector<double>& matrix,
              int nRows, int nCols,
              int i, int j)
{
    assert(i >= 0 && i < nRows && j >= 0 && j < nRows);
    if (i == j) return;

    for (int c = 0; c < nCols; ++c)
    {
        std::swap(matrix[cmIndex(i, c, nRows)],
                  matrix[cmIndex(j, c, nRows)]);
    }
}

// Swap columns i and j in the same storage layout.

void swapCols(std::vector<double>& matrix,
              int nRows, int nCols,
              int i, int j)
{
    assert(i >= 0 && i < nCols && j >= 0 && j < nCols);
    if (i == j) return;

    const std::size_t stride = static_cast<std::size_t>(nRows);
    std::size_t col_i_offset = static_cast<std::size_t>(i) * stride;
    std::size_t col_j_offset = static_cast<std::size_t>(j) * stride;

    for (int r = 0; r < nRows; ++r)
    {
        std::swap(matrix[col_i_offset + r],
                  matrix[col_j_offset + r]);
    }
}
