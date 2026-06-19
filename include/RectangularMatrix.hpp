#ifndef RECTANGULAR_MATRIX_HPP
#define RECTANGULAR_MATRIX_HPP

#include "DynamicArray.hpp"
#include "Exceptions.hpp"
#include "NumericUtils.hpp"

#include <cmath>

template <class T>
class RectangularMatrix {
private:
    int rows;
    int columns;
    DynamicArray<T> items;

    void ValidatePositiveDimensions(int rowCount, int columnCount) const {
        if (rowCount <= 0 || columnCount <= 0) {
            throw InvalidArgumentException("matrix dimensions must be positive");
        }
    }

    void ValidatePosition(int row, int column) const {
        if (row < 0 || row >= rows || column < 0 || column >= columns) {
            throw IndexOutOfRangeException("matrix position is outside bounds");
        }
    }

    void ValidateRow(int row) const {
        if (row < 0 || row >= rows) {
            throw IndexOutOfRangeException("matrix row index is outside bounds");
        }
    }

    void ValidateColumn(int column) const {
        if (column < 0 || column >= columns) {
            throw IndexOutOfRangeException("matrix column index is outside bounds");
        }
    }

    void ValidateSameDimensions(const RectangularMatrix<T>& other) const {
        if (rows != other.rows || columns != other.columns) {
            throw InvalidArgumentException("matrices must have the same dimensions");
        }
    }

    int Position(int row, int column) const {
        ValidatePosition(row, column);
        return row * columns + column;
    }

public:
    RectangularMatrix() : rows(1), columns(1), items(1) {}

    RectangularMatrix(int rowCount, int columnCount)
        : rows(rowCount), columns(columnCount), items(rowCount * columnCount) {
        ValidatePositiveDimensions(rowCount, columnCount);
    }

    RectangularMatrix(int rowCount, int columnCount, T* source)
        : rows(rowCount), columns(columnCount), items(source, rowCount * columnCount) {
        ValidatePositiveDimensions(rowCount, columnCount);
    }

    RectangularMatrix(const RectangularMatrix<T>& other)
        : rows(other.rows), columns(other.columns), items(other.items) {}

    RectangularMatrix<T>& operator=(const RectangularMatrix<T>& other) {
        if (this == &other) {
            return *this;
        }

        rows = other.rows;
        columns = other.columns;
        items = other.items;

        return *this;
    }

    int GetRows() const {
        return rows;
    }

    int GetColumns() const {
        return columns;
    }

    int GetStoredItemsCount() const {
        return items.GetSize();
    }

    T Get(int row, int column) const {
        return items.Get(Position(row, column));
    }

    void Set(int row, int column, const T& value) {
        items.Set(Position(row, column), value);
    }

    RectangularMatrix<T>* Add(const RectangularMatrix<T>& other) const {
        ValidateSameDimensions(other);

        RectangularMatrix<T>* result = new RectangularMatrix<T>(rows, columns);

        for (int row = 0; row < rows; ++row) {
            for (int column = 0; column < columns; ++column) {
                result->Set(row, column, Get(row, column) + other.Get(row, column));
            }
        }

        return result;
    }

    RectangularMatrix<T>* MultiplyByScalar(const T& scalar) const {
        RectangularMatrix<T>* result = new RectangularMatrix<T>(rows, columns);

        for (int row = 0; row < rows; ++row) {
            for (int column = 0; column < columns; ++column) {
                result->Set(row, column, Get(row, column) * scalar);
            }
        }

        return result;
    }

    double Norm() const {
        double sum = 0.0;

        for (int i = 0; i < items.GetSize(); ++i) {
            sum += MagnitudeSquared(items.Get(i));
        }

        return std::sqrt(sum);
    }

    void SwapRows(int firstRow, int secondRow) {
        ValidateRow(firstRow);
        ValidateRow(secondRow);

        for (int column = 0; column < columns; ++column) {
            T temporary = Get(firstRow, column);
            Set(firstRow, column, Get(secondRow, column));
            Set(secondRow, column, temporary);
        }
    }

    void SwapColumns(int firstColumn, int secondColumn) {
        ValidateColumn(firstColumn);
        ValidateColumn(secondColumn);

        for (int row = 0; row < rows; ++row) {
            T temporary = Get(row, firstColumn);
            Set(row, firstColumn, Get(row, secondColumn));
            Set(row, secondColumn, temporary);
        }
    }

    void MultiplyRowByScalar(int row, const T& scalar) {
        ValidateRow(row);

        for (int column = 0; column < columns; ++column) {
            Set(row, column, Get(row, column) * scalar);
        }
    }

    void MultiplyColumnByScalar(int column, const T& scalar) {
        ValidateColumn(column);

        for (int row = 0; row < rows; ++row) {
            Set(row, column, Get(row, column) * scalar);
        }
    }

    void AddRowMultiple(int sourceRow, int targetRow, const T& scalar) {
        ValidateRow(sourceRow);
        ValidateRow(targetRow);

        for (int column = 0; column < columns; ++column) {
            Set(targetRow, column, Get(targetRow, column) + Get(sourceRow, column) * scalar);
        }
    }

    void AddColumnMultiple(int sourceColumn, int targetColumn, const T& scalar) {
        ValidateColumn(sourceColumn);
        ValidateColumn(targetColumn);

        for (int row = 0; row < rows; ++row) {
            Set(row, targetColumn, Get(row, targetColumn) + Get(row, sourceColumn) * scalar);
        }
    }

    bool operator==(const RectangularMatrix<T>& other) const {
        if (rows != other.rows || columns != other.columns) {
            return false;
        }

        for (int row = 0; row < rows; ++row) {
            for (int column = 0; column < columns; ++column) {
                if (!(Get(row, column) == other.Get(row, column))) {
                    return false;
                }
            }
        }

        return true;
    }
};

#endif
