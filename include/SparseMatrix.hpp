#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include "DynamicArray.hpp"
#include "Exceptions.hpp"
#include "NumericUtils.hpp"
#include "SparseMatrixItem.hpp"

#include <cmath>

template <class T>
class SparseMatrix {
private:
    int rows;
    int columns;
    DynamicArray<SparseMatrixItem<T> > items;

    void ValidatePositiveDimensions(int rowCount, int columnCount) const {
        if (rowCount <= 0 || columnCount <= 0) {
            throw InvalidArgumentException("sparse matrix dimensions must be positive");
        }
    }

    void ValidatePosition(int row, int column) const {
        if (row < 0 || row >= rows || column < 0 || column >= columns) {
            throw IndexOutOfRangeException("sparse matrix position is outside bounds");
        }
    }

    void ValidateSameDimensions(const SparseMatrix<T>& other) const {
        if (rows != other.rows || columns != other.columns) {
            throw InvalidArgumentException("sparse matrices must have the same dimensions");
        }
    }

    void ValidateMultiplicationDimensions(const SparseMatrix<T>& other) const {
        if (columns != other.rows) {
            throw InvalidArgumentException("left matrix column count must be equal to right matrix row count");
        }
    }

    int PositionKey(int row, int column) const {
        return row * columns + column;
    }

    int ItemKey(const SparseMatrixItem<T>& item) const {
        return PositionKey(item.GetRow(), item.GetColumn());
    }

    int FindInsertPosition(int row, int column, bool& found) const {
        int targetKey = PositionKey(row, column);
        int left = 0;
        int right = items.GetSize();

        while (left < right) {
            int middle = left + (right - left) / 2;
            int middleKey = ItemKey(items.Get(middle));

            if (middleKey < targetKey) {
                left = middle + 1;
            } else {
                right = middle;
            }
        }

        found = left < items.GetSize() && ItemKey(items.Get(left)) == targetKey;
        return left;
    }

    void InsertAt(int index, const SparseMatrixItem<T>& item) {
        int oldSize = items.GetSize();
        items.Resize(oldSize + 1);

        for (int i = oldSize; i > index; --i) {
            items.Set(i, items.Get(i - 1));
        }

        items.Set(index, item);
    }

    void RemoveAt(int index) {
        int oldSize = items.GetSize();

        for (int i = index; i + 1 < oldSize; ++i) {
            items.Set(i, items.Get(i + 1));
        }

        items.Resize(oldSize - 1);
    }

    void AppendStoredItem(const SparseMatrixItem<T>& item) {
        if (IsZeroValue(item.GetValue())) {
            return;
        }

        int oldSize = items.GetSize();
        items.Resize(oldSize + 1);
        items.Set(oldSize, item);
    }

public:
    SparseMatrix() : rows(1), columns(1), items() {}

    SparseMatrix(int rowCount, int columnCount)
        : rows(rowCount), columns(columnCount), items() {
        ValidatePositiveDimensions(rowCount, columnCount);
    }

    SparseMatrix(int rowCount, int columnCount, T* source)
        : rows(rowCount), columns(columnCount), items() {
        ValidatePositiveDimensions(rowCount, columnCount);

        if (source == nullptr) {
            throw InvalidArgumentException("source matrix array cannot be null");
        }

        for (int row = 0; row < rows; ++row) {
            for (int column = 0; column < columns; ++column) {
                T value = source[row * columns + column];

                if (!IsZeroValue(value)) {
                    AppendStoredItem(SparseMatrixItem<T>(row, column, value));
                }
            }
        }
    }

    SparseMatrix(const SparseMatrix<T>& other)
        : rows(other.rows), columns(other.columns), items(other.items) {}

    SparseMatrix<T>& operator=(const SparseMatrix<T>& other) {
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

    SparseMatrixItem<T> GetStoredItem(int index) const {
        return items.Get(index);
    }

    T Get(int row, int column) const {
        ValidatePosition(row, column);

        bool found = false;
        int index = FindInsertPosition(row, column, found);

        if (!found) {
            return T{};
        }

        return items.Get(index).GetValue();
    }

    void Set(int row, int column, const T& value) {
        ValidatePosition(row, column);

        bool found = false;
        int index = FindInsertPosition(row, column, found);

        if (IsZeroValue(value)) {
            if (found) {
                RemoveAt(index);
            }

            return;
        }

        SparseMatrixItem<T> item(row, column, value);

        if (found) {
            items.Set(index, item);
        } else {
            InsertAt(index, item);
        }
    }

    SparseMatrix<T>* Add(const SparseMatrix<T>& other) const {
        ValidateSameDimensions(other);

        SparseMatrix<T>* result = new SparseMatrix<T>(rows, columns);

        int leftIndex = 0;
        int rightIndex = 0;

        while (leftIndex < items.GetSize() && rightIndex < other.items.GetSize()) {
            SparseMatrixItem<T> leftItem = items.Get(leftIndex);
            SparseMatrixItem<T> rightItem = other.items.Get(rightIndex);
            int leftKey = ItemKey(leftItem);
            int rightKey = other.ItemKey(rightItem);

            if (leftKey == rightKey) {
                T sum = leftItem.GetValue() + rightItem.GetValue();

                if (!IsZeroValue(sum)) {
                    result->AppendStoredItem(SparseMatrixItem<T>(leftItem.GetRow(), leftItem.GetColumn(), sum));
                }

                ++leftIndex;
                ++rightIndex;
            } else if (leftKey < rightKey) {
                result->AppendStoredItem(leftItem);
                ++leftIndex;
            } else {
                result->AppendStoredItem(rightItem);
                ++rightIndex;
            }
        }

        while (leftIndex < items.GetSize()) {
            result->AppendStoredItem(items.Get(leftIndex));
            ++leftIndex;
        }

        while (rightIndex < other.items.GetSize()) {
            result->AppendStoredItem(other.items.Get(rightIndex));
            ++rightIndex;
        }

        return result;
    }

    SparseMatrix<T>* MultiplyByScalar(const T& scalar) const {
        SparseMatrix<T>* result = new SparseMatrix<T>(rows, columns);

        if (IsZeroValue(scalar)) {
            return result;
        }

        for (int index = 0; index < items.GetSize(); ++index) {
            SparseMatrixItem<T> item = items.Get(index);
            T value = item.GetValue() * scalar;

            if (!IsZeroValue(value)) {
                result->AppendStoredItem(SparseMatrixItem<T>(item.GetRow(), item.GetColumn(), value));
            }
        }

        return result;
    }

    SparseMatrix<T>* Multiply(const SparseMatrix<T>& other) const {
        ValidateMultiplicationDimensions(other);

        SparseMatrix<T>* result = new SparseMatrix<T>(rows, other.columns);

        for (int leftIndex = 0; leftIndex < items.GetSize(); ++leftIndex) {
            SparseMatrixItem<T> leftItem = items.Get(leftIndex);

            for (int rightIndex = 0; rightIndex < other.items.GetSize(); ++rightIndex) {
                SparseMatrixItem<T> rightItem = other.items.Get(rightIndex);

                if (leftItem.GetColumn() == rightItem.GetRow()) {
                    int resultRow = leftItem.GetRow();
                    int resultColumn = rightItem.GetColumn();
                    T previousValue = result->Get(resultRow, resultColumn);
                    T nextValue = previousValue + leftItem.GetValue() * rightItem.GetValue();

                    result->Set(resultRow, resultColumn, nextValue);
                }
            }
        }

        return result;
    }

    double Norm() const {
        double sum = 0.0;

        for (int index = 0; index < items.GetSize(); ++index) {
            sum += MagnitudeSquared(items.Get(index).GetValue());
        }

        return std::sqrt(sum);
    }

    bool operator==(const SparseMatrix<T>& other) const {
        if (rows != other.rows || columns != other.columns || items.GetSize() != other.items.GetSize()) {
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
