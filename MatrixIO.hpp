#ifndef MATRIX_IO_HPP
#define MATRIX_IO_HPP

#include "DiagonalMatrix.hpp"
#include "RectangularMatrix.hpp"
#include "SquareMatrix.hpp"
#include "SparseMatrix.hpp"
#include "TriangularMatrix.hpp"

#include <ostream>

template <class MatrixType>
void PrintMatrixBySize(std::ostream& out, const MatrixType& matrix, int rows, int columns) {
    for (int row = 0; row < rows; ++row) {
        out << "[";

        for (int column = 0; column < columns; ++column) {
            if (column != 0) {
                out << ", ";
            }

            out << matrix.Get(row, column);
        }

        out << "]";

        if (row + 1 < rows) {
            out << "\n";
        }
    }
}

template <class T>
std::ostream& operator<<(std::ostream& out, const RectangularMatrix<T>& matrix) {
    PrintMatrixBySize(out, matrix, matrix.GetRows(), matrix.GetColumns());
    return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const SquareMatrix<T>& matrix) {
    PrintMatrixBySize(out, matrix, matrix.GetSize(), matrix.GetSize());
    return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const TriangularMatrix<T>& matrix) {
    PrintMatrixBySize(out, matrix, matrix.GetSize(), matrix.GetSize());
    return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const DiagonalMatrix<T>& matrix) {
    PrintMatrixBySize(out, matrix, matrix.GetSize(), matrix.GetSize());
    return out;
}


template <class T>
std::ostream& operator<<(std::ostream& out, const SparseMatrix<T>& matrix) {
    PrintMatrixBySize(out, matrix, matrix.GetRows(), matrix.GetColumns());
    return out;
}

#endif
