#ifndef DIAGONAL_MATRIX_HPP
#define DIAGONAL_MATRIX_HPP

#include "DynamicArray.hpp"
#include "Exceptions.hpp"
#include "NumericUtils.hpp"

#include <cmath>

template <class T>
class DiagonalMatrix {
private:
    DynamicArray<T> diagonal;

    void ValidateSize(int size) const {
        if (size <= 0) {
            throw InvalidArgumentException("diagonal matrix size must be positive");
        }
    }

    void ValidatePosition(int row, int column) const {
        int size = GetSize();

        if (row < 0 || row >= size || column < 0 || column >= size) {
            throw IndexOutOfRangeException("diagonal matrix position is outside bounds");
        }
    }

    void ValidateSameSize(const DiagonalMatrix<T>& other) const {
        if (GetSize() != other.GetSize()) {
            throw InvalidArgumentException("diagonal matrices must have the same size");
        }
    }

public:
    DiagonalMatrix() : diagonal(1) {}

    explicit DiagonalMatrix(int size) : diagonal(size) {
        ValidateSize(size);
    }

    DiagonalMatrix(T* source, int size) : diagonal(source, size) {
        ValidateSize(size);
    }

    DiagonalMatrix(const DiagonalMatrix<T>& other) : diagonal(other.diagonal) {}

    DiagonalMatrix<T>& operator=(const DiagonalMatrix<T>& other) {
        if (this == &other) {
            return *this;
        }

        diagonal = other.diagonal;
        return *this;
    }

    int GetSize() const {
        return diagonal.GetSize();
    }

    int GetRows() const {
        return GetSize();
    }

    int GetColumns() const {
        return GetSize();
    }

    int GetStoredItemsCount() const {
        return diagonal.GetSize();
    }

    T Get(int row, int column) const {
        ValidatePosition(row, column);

        if (row != column) {
            return T{};
        }

        return diagonal.Get(row);
    }

    void Set(int row, int column, const T& value) {
        ValidatePosition(row, column);

        if (row != column) {
            if (!IsZeroValue(value)) {
                throw InvalidArgumentException("cannot set non-zero value outside diagonal");
            }

            return;
        }

        diagonal.Set(row, value);
    }

    DiagonalMatrix<T>* Add(const DiagonalMatrix<T>& other) const {
        ValidateSameSize(other);

        DiagonalMatrix<T>* result = new DiagonalMatrix<T>(GetSize());

        for (int i = 0; i < GetSize(); ++i) {
            result->diagonal.Set(i, diagonal.Get(i) + other.diagonal.Get(i));
        }

        return result;
    }

    DiagonalMatrix<T>* MultiplyByScalar(const T& scalar) const {
        DiagonalMatrix<T>* result = new DiagonalMatrix<T>(GetSize());

        for (int i = 0; i < GetSize(); ++i) {
            result->diagonal.Set(i, diagonal.Get(i) * scalar);
        }

        return result;
    }

    double Norm() const {
        double sum = 0.0;

        for (int i = 0; i < GetSize(); ++i) {
            sum += MagnitudeSquared(diagonal.Get(i));
        }

        return std::sqrt(sum);
    }

    bool operator==(const DiagonalMatrix<T>& other) const {
        if (GetSize() != other.GetSize()) {
            return false;
        }

        for (int i = 0; i < GetSize(); ++i) {
            if (!(diagonal.Get(i) == other.diagonal.Get(i))) {
                return false;
            }
        }

        return true;
    }
};

#endif
