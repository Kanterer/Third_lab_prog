#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "DynamicArray.hpp"
#include "Exceptions.hpp"
#include "NumericUtils.hpp"

#include <cmath>

template <class T>
class Vector {
private:
    DynamicArray<T> items;

    void ValidateSameSize(const Vector<T>& other) const {
        if (GetSize() != other.GetSize()) {
            throw InvalidArgumentException("vectors must have the same size");
        }
    }

public:
    Vector() : items() {}

    explicit Vector(int size) : items(size) {}

    Vector(T* source, int size) : items(source, size) {}

    Vector(const Vector<T>& other) : items(other.items) {}

    Vector<T>& operator=(const Vector<T>& other) {
        if (this == &other) {
            return *this;
        }

        items = other.items;
        return *this;
    }

    int GetSize() const {
        return items.GetSize();
    }

    int GetStoredItemsCount() const {
        return items.GetSize();
    }

    T Get(int index) const {
        return items.Get(index);
    }

    void Set(int index, const T& value) {
        items.Set(index, value);
    }

    void Append(const T& value) {
        int oldSize = items.GetSize();
        items.Resize(oldSize + 1);
        items.Set(oldSize, value);
    }

    Vector<T>* Add(const Vector<T>& other) const {
        ValidateSameSize(other);

        Vector<T>* result = new Vector<T>(GetSize());

        for (int i = 0; i < GetSize(); ++i) {
            result->Set(i, Get(i) + other.Get(i));
        }

        return result;
    }

    Vector<T>* MultiplyByScalar(const T& scalar) const {
        Vector<T>* result = new Vector<T>(GetSize());

        for (int i = 0; i < GetSize(); ++i) {
            result->Set(i, Get(i) * scalar);
        }

        return result;
    }

    T DotProduct(const Vector<T>& other) const {
        ValidateSameSize(other);

        T result{};

        for (int i = 0; i < GetSize(); ++i) {
            result = result + Get(i) * other.Get(i);
        }

        return result;
    }

    double Norm() const {
        double sum = 0.0;

        for (int i = 0; i < GetSize(); ++i) {
            sum += MagnitudeSquared(Get(i));
        }

        return std::sqrt(sum);
    }

    bool operator==(const Vector<T>& other) const {
        if (GetSize() != other.GetSize()) {
            return false;
        }

        for (int i = 0; i < GetSize(); ++i) {
            if (!(Get(i) == other.Get(i))) {
                return false;
            }
        }

        return true;
    }
};

#endif
