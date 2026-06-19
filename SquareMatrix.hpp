#ifndef SQUARE_MATRIX_HPP
#define SQUARE_MATRIX_HPP

#include "RectangularMatrix.hpp"

template <class T>
class SquareMatrix {
private:
    RectangularMatrix<T> matrix;

    void ValidateSameSize(const SquareMatrix<T>& other) const {
        if (GetSize() != other.GetSize()) {
            throw InvalidArgumentException("square matrices must have the same size");
        }
    }

public:
    SquareMatrix() : matrix(1, 1) {}

    explicit SquareMatrix(int size) : matrix(size, size) {}

    SquareMatrix(int size, T* source) : matrix(size, size, source) {}

    SquareMatrix(const SquareMatrix<T>& other) : matrix(other.matrix) {}

    SquareMatrix<T>& operator=(const SquareMatrix<T>& other) {
        if (this == &other) {
            return *this;
        }

        matrix = other.matrix;
        return *this;
    }

    int GetSize() const {
        return matrix.GetRows();
    }

    int GetRows() const {
        return matrix.GetRows();
    }

    int GetColumns() const {
        return matrix.GetColumns();
    }

    int GetStoredItemsCount() const {
        return matrix.GetStoredItemsCount();
    }

    T Get(int row, int column) const {
        return matrix.Get(row, column);
    }

    void Set(int row, int column, const T& value) {
        matrix.Set(row, column, value);
    }

    SquareMatrix<T>* Add(const SquareMatrix<T>& other) const {
        ValidateSameSize(other);

        SquareMatrix<T>* result = new SquareMatrix<T>(GetSize());

        for (int row = 0; row < GetSize(); ++row) {
            for (int column = 0; column < GetSize(); ++column) {
                result->Set(row, column, Get(row, column) + other.Get(row, column));
            }
        }

        return result;
    }

    SquareMatrix<T>* MultiplyByScalar(const T& scalar) const {
        SquareMatrix<T>* result = new SquareMatrix<T>(GetSize());

        for (int row = 0; row < GetSize(); ++row) {
            for (int column = 0; column < GetSize(); ++column) {
                result->Set(row, column, Get(row, column) * scalar);
            }
        }

        return result;
    }

    double Norm() const {
        return matrix.Norm();
    }

    void SwapRows(int firstRow, int secondRow) {
        matrix.SwapRows(firstRow, secondRow);
    }

    void SwapColumns(int firstColumn, int secondColumn) {
        matrix.SwapColumns(firstColumn, secondColumn);
    }

    void MultiplyRowByScalar(int row, const T& scalar) {
        matrix.MultiplyRowByScalar(row, scalar);
    }

    void MultiplyColumnByScalar(int column, const T& scalar) {
        matrix.MultiplyColumnByScalar(column, scalar);
    }

    void AddRowMultiple(int sourceRow, int targetRow, const T& scalar) {
        matrix.AddRowMultiple(sourceRow, targetRow, scalar);
    }

    void AddColumnMultiple(int sourceColumn, int targetColumn, const T& scalar) {
        matrix.AddColumnMultiple(sourceColumn, targetColumn, scalar);
    }

    bool operator==(const SquareMatrix<T>& other) const {
        return matrix == other.matrix;
    }
};

#endif
