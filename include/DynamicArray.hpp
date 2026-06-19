#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include "Exceptions.hpp"

template <class T>
class DynamicArray {
private:
    T* data;
    int size;
    int capacity;

    void ValidateIndex(int index) const {
        if (index < 0 || index >= size) {
            throw IndexOutOfRangeException("index is outside array bounds");
        }
    }

    void Reallocate(int newCapacity) {
        T* newData = newCapacity == 0 ? nullptr : new T[newCapacity]{};

        int itemsToCopy = size < newCapacity ? size : newCapacity;

        for (int i = 0; i < itemsToCopy; ++i) {
            newData[i] = data[i];
        }

        delete[] data;

        data = newData;
        capacity = newCapacity;

        if (size > capacity) {
            size = capacity;
        }
    }

    void EnsureCapacity(int requiredCapacity) {
        if (requiredCapacity <= capacity) {
            return;
        }

        int newCapacity = capacity == 0 ? 1 : capacity;

        while (newCapacity < requiredCapacity) {
            newCapacity *= 2;
        }

        Reallocate(newCapacity);
    }

    void ShrinkIfTooMuchExtraMemory() {
        if (size == 0) {
            delete[] data;
            data = nullptr;
            capacity = 0;
            return;
        }

        if (capacity > 4 && size * 4 <= capacity) {
            int newCapacity = capacity / 2;

            if (newCapacity < size) {
                newCapacity = size;
            }

            Reallocate(newCapacity);
        }
    }

public:
    DynamicArray() : data(nullptr), size(0), capacity(0) {}

    DynamicArray(T* items, int count) : data(nullptr), size(0), capacity(0) {
        if (count < 0) {
            throw InvalidArgumentException("array size cannot be negative");
        }

        if (count > 0 && items == nullptr) {
            throw InvalidArgumentException("source array is null while count is positive");
        }

        size = count;
        capacity = count;
        data = capacity == 0 ? nullptr : new T[capacity]{};

        for (int i = 0; i < size; ++i) {
            data[i] = items[i];
        }
    }

    explicit DynamicArray(int size) : data(nullptr), size(0), capacity(0) {
        if (size < 0) {
            throw InvalidArgumentException("array size cannot be negative");
        }

        this->size = size;
        capacity = size;
        data = capacity == 0 ? nullptr : new T[capacity]{};
    }

    DynamicArray(const DynamicArray<T>& other) : data(nullptr), size(other.size), capacity(other.size) {
        data = capacity == 0 ? nullptr : new T[capacity]{};

        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    DynamicArray(DynamicArray<T>&& other) noexcept
        : data(other.data), size(other.size), capacity(other.capacity) {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this == &other) {
            return *this;
        }

        T* newData = other.size == 0 ? nullptr : new T[other.size]{};

        for (int i = 0; i < other.size; ++i) {
            newData[i] = other.data[i];
        }

        delete[] data;

        data = newData;
        size = other.size;
        capacity = other.size;

        return *this;
    }

    DynamicArray<T>& operator=(DynamicArray<T>&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        delete[] data;

        data = other.data;
        size = other.size;
        capacity = other.capacity;

        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;

        return *this;
    }

    ~DynamicArray() {
        delete[] data;
    }

    T Get(int index) const {
        ValidateIndex(index);

        return data[index];
    }

    int GetSize() const {
        return size;
    }

    int GetCapacity() const {
        return capacity;
    }

    void Set(int index, T value) {
        ValidateIndex(index);

        data[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0) {
            throw InvalidArgumentException("new array size cannot be negative");
        }

        int oldSize = size;

        if (newSize == 0) {
            delete[] data;
            data = nullptr;
            size = 0;
            capacity = 0;
            return;
        }

        EnsureCapacity(newSize);

        if (newSize > oldSize) {
            for (int i = oldSize; i < newSize; ++i) {
                data[i] = T{};
            }
        }

        size = newSize;

        if (newSize < oldSize) {
            ShrinkIfTooMuchExtraMemory();
        }
    }

    void ShrinkToFit() {
        if (capacity != size) {
            Reallocate(size);
        }
    }

    T& operator[](int index) {
        ValidateIndex(index);

        return data[index];
    }

    const T& operator[](int index) const {
        ValidateIndex(index);

        return data[index];
    }
};

#endif
