#ifndef SPARSE_MATRIX_ITEM_HPP
#define SPARSE_MATRIX_ITEM_HPP

template <class T>
class SparseMatrixItem {
private:
    int row;
    int column;
    T value;

public:
    SparseMatrixItem() : row(0), column(0), value{} {}

    SparseMatrixItem(int row, int column, const T& value)
        : row(row), column(column), value(value) {}

    int GetRow() const {
        return row;
    }

    int GetColumn() const {
        return column;
    }

    T GetValue() const {
        return value;
    }

    void SetValue(const T& newValue) {
        value = newValue;
    }

    bool operator==(const SparseMatrixItem<T>& other) const {
        return row == other.row && column == other.column && value == other.value;
    }
};

#endif
