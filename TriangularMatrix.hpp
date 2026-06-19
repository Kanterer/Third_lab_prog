#ifndef TRIANGULAR_MATRIX_HPP
#define TRIANGULAR_MATRIX_HPP

#include "DynamicArray.hpp"
#include "Exceptions.hpp"
#include "NumericUtils.hpp"
#include "TriangleType.hpp"

#include <cmath>

template <class T>
class TriangularMatrix {
private:
    int size;
    TriangleType type;
    DynamicArray<T> items;

    void ValidateSize(int matrixSize) const {
        if (matrixSize <= 0) {
            throw InvalidArgumentException("triangular matrix size must be positive");
        }
    }

    void ValidatePosition(int row, int column) const {
        if (row < 0 || row >= size || column < 0 || column >= size) {
            throw IndexOutOfRangeException("triangular matrix position is outside bounds");
        }
    }

    void ValidateSameShape(const TriangularMatrix<T>& other) const {
        if (size != other.size || type != other.type) {
            throw InvalidArgumentException("triangular matrices must have the same size and type");
        }
    }

    bool IsStoredPosition(int row, int column) const {
        if (type == UpperTriangle) {
            return row <= column;
        }

        return row >= column;
    }

    int StoredPosition(int row, int column) const {
        ValidatePosition(row, column);

        if (!IsStoredPosition(row, column)) {
            throw InvalidOperationException("triangular matrix does not store this zero position");
        }

        if (type == UpperTriangle) {
            return row * size - row * (row - 1) / 2 + (column - row);
        }

        return row * (row + 1) / 2 + column;
    }

public:
    TriangularMatrix() : size(1), type(UpperTriangle), items(1) {}

    TriangularMatrix(int matrixSize, TriangleType triangleType)
        : size(matrixSize), type(triangleType), items(matrixSize * (matrixSize + 1) / 2) {
        ValidateSize(matrixSize);
    }

    TriangularMatrix(const TriangularMatrix<T>& other)
        : size(other.size), type(other.type), items(other.items) {}

    TriangularMatrix<T>& operator=(const TriangularMatrix<T>& other) {
        if (this == &other) {
            return *this;
        }

        size = other.size;
        type = other.type;
        items = other.items;

        return *this;
    }

    int GetSize() const {
        return size;
    }

    int GetRows() const {
        return size;
    }

    int GetColumns() const {
        return size;
    }

    TriangleType GetType() const {
        return type;
    }

    int GetStoredItemsCount() const {
        return items.GetSize();
    }

    T Get(int row, int column) const {
        ValidatePosition(row, column);

        if (!IsStoredPosition(row, column)) {
            return T{};
        }

        return items.Get(StoredPosition(row, column));
    }

    void Set(int row, int column, const T& value) {
        ValidatePosition(row, column);

        if (!IsStoredPosition(row, column)) {
            if (!IsZeroValue(value)) {
                throw InvalidArgumentException("cannot set non-zero value outside triangular part");
            }

            return;
        }

        items.Set(StoredPosition(row, column), value);
    }

    TriangularMatrix<T>* Add(const TriangularMatrix<T>& other) const {
        ValidateSameShape(other);

        TriangularMatrix<T>* result = new TriangularMatrix<T>(size, type);

        for (int index = 0; index < items.GetSize(); ++index) {
            result->items.Set(index, items.Get(index) + other.items.Get(index));
        }

        return result;
    }

    TriangularMatrix<T>* MultiplyByScalar(const T& scalar) const {
        TriangularMatrix<T>* result = new TriangularMatrix<T>(size, type);

        for (int index = 0; index < items.GetSize(); ++index) {
            result->items.Set(index, items.Get(index) * scalar);
        }

        return result;
    }

    double Norm() const {
        double sum = 0.0;

        for (int index = 0; index < items.GetSize(); ++index) {
            sum += MagnitudeSquared(items.Get(index));
        }

        return std::sqrt(sum);
    }

    bool operator==(const TriangularMatrix<T>& other) const {
        if (size != other.size || type != other.type) {
            return false;
        }

        for (int index = 0; index < items.GetSize(); ++index) {
            if (!(items.Get(index) == other.items.Get(index))) {
                return false;
            }
        }

        return true;
    }
};

#endif
