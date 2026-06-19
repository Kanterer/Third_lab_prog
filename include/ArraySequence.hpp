#ifndef ARRAY_SEQUENCE_HPP
#define ARRAY_SEQUENCE_HPP

#include "DynamicArray.hpp"
#include "Sequence.hpp"

template <class T>
class MutableArraySequence;

template <class T>
class ImmutableArraySequence;

template <class T, class Derived>
class ArraySequence : public Sequence<T, Derived> {
protected:
    DynamicArray<T> items;

    Derived* SelfDerived() {
        return static_cast<Derived*>(this);
    }

    const Derived* SelfDerived() const {
        return static_cast<const Derived*>(this);
    }

    void AppendInternal(T item) {
        int oldSize = items.GetSize();

        items.Resize(oldSize + 1);
        items.Set(oldSize, item);
    }

    void PrependInternal(T item) {
        int oldSize = items.GetSize();

        items.Resize(oldSize + 1);

        for (int i = oldSize; i > 0; --i) {
            items.Set(i, items.Get(i - 1));
        }

        items.Set(0, item);
    }

    void InsertAtInternal(T item, int index) {
        if (index < 0 || index > items.GetSize()) {
            throw IndexOutOfRangeException("insert index is outside ArraySequence bounds");
        }

        int oldSize = items.GetSize();

        items.Resize(oldSize + 1);

        for (int i = oldSize; i > index; --i) {
            items.Set(i, items.Get(i - 1));
        }

        items.Set(index, item);
    }

public:
    void SetExistingItem(int index, T item) {
        items.Set(index, item);
    }

protected:
    static int NormalizeSliceStart(int startIndex, int length) {
        int normalizedStart = startIndex;

        if (normalizedStart < 0) {
            normalizedStart = length + normalizedStart;
        }

        return normalizedStart;
    }

    static void ValidateSliceRange(int normalizedStart, int count, int length) {
        if (count < 0) {
            throw InvalidArgumentException("slice count cannot be negative");
        }

        if (normalizedStart < 0 || normalizedStart > length) {
            throw IndexOutOfRangeException("slice start index is outside sequence bounds");
        }

        if (normalizedStart + count > length) {
            throw IndexOutOfRangeException("slice removes more elements than sequence contains");
        }
    }

public:
    ArraySequence() : items() {}

    ArraySequence(T* inputItems, int count) : items(inputItems, count) {}

    explicit ArraySequence(const DynamicArray<T>& array) : items(array) {}

    ArraySequence(const ArraySequence<T, Derived>& other) : items(other.items) {}

    T GetFirstImpl() const {
        if (items.GetSize() == 0) {
            throw EmptySequenceException("cannot get first item from empty ArraySequence");
        }

        return items.Get(0);
    }

    T GetLastImpl() const {
        if (items.GetSize() == 0) {
            throw EmptySequenceException("cannot get last item from empty ArraySequence");
        }

        return items.Get(items.GetSize() - 1);
    }

    T GetImpl(int index) const {
        return items.Get(index);
    }

    MutableArraySequence<T>* GetSubsequenceImpl(int startIndex, int endIndex) const {
        int length = items.GetSize();

        if (startIndex < 0 || startIndex >= length || endIndex < 0 || endIndex >= length) {
            throw IndexOutOfRangeException("subsequence boundaries are outside ArraySequence bounds");
        }

        if (startIndex > endIndex) {
            throw InvalidArgumentException("startIndex cannot be greater than endIndex");
        }

        int resultLength = endIndex - startIndex + 1;
        MutableArraySequence<T>* result = new MutableArraySequence<T>(resultLength);

        for (int i = 0; i < resultLength; ++i) {
            result->SetExistingItem(i, items.Get(startIndex + i));
        }

        return result;
    }

    int GetLengthImpl() const {
        return items.GetSize();
    }

    Derived* AppendImpl(T item) {
        Derived* target = SelfDerived()->Instance();

        target->AppendInternal(item);

        return target;
    }

    Derived* PrependImpl(T item) {
        Derived* target = SelfDerived()->Instance();

        target->PrependInternal(item);

        return target;
    }

    Derived* InsertAtImpl(T item, int index) {
        Derived* target = SelfDerived()->Instance();

        target->InsertAtInternal(item, index);

        return target;
    }

    template <class OtherDerived>
    Derived* ConcatImpl(const OtherDerived& sequence) {
        Derived* target = SelfDerived()->Instance();

        int oldLength = target->items.GetSize();
        int otherLength = sequence.GetLength();

        target->items.Resize(oldLength + otherLength);

        for (int i = 0; i < otherLength; ++i) {
            target->items.Set(oldLength + i, sequence.Get(i));
        }

        return target;
    }

    T& operator[](int index) {
        return items[index];
    }

    const T& operator[](int index) const {
        return items[index];
    }

    MutableArraySequence<T>* EmptyMutableImpl() const {
        return new MutableArraySequence<T>();
    }

    MutableArraySequence<T>* MapImpl(T (*mapper)(const T&)) const {
        int length = items.GetSize();
        MutableArraySequence<T>* result = new MutableArraySequence<T>(length);

        for (int i = 0; i < length; ++i) {
            result->SetExistingItem(i, mapper(items.Get(i)));
        }

        return result;
    }

    MutableArraySequence<T>* MapIndexedImpl(T (*mapper)(const T&, int)) const {
        int length = items.GetSize();
        MutableArraySequence<T>* result = new MutableArraySequence<T>(length);

        for (int i = 0; i < length; ++i) {
            result->SetExistingItem(i, mapper(items.Get(i), i));
        }

        return result;
    }

    MutableArraySequence<T>* WhereImpl(bool (*predicate)(const T&)) const {
        int length = items.GetSize();
        int count = 0;

        for (int i = 0; i < length; ++i) {
            if (predicate(items.Get(i))) {
                ++count;
            }
        }

        MutableArraySequence<T>* result = new MutableArraySequence<T>(count);
        int resultIndex = 0;

        for (int i = 0; i < length; ++i) {
            T item = items.Get(i);

            if (predicate(item)) {
                result->SetExistingItem(resultIndex, item);
                ++resultIndex;
            }
        }

        return result;
    }

    MutableArraySequence<T>* SliceWithoutReplacementImpl(int startIndex, int count) const {
        int length = items.GetSize();
        int normalizedStart = NormalizeSliceStart(startIndex, length);

        ValidateSliceRange(normalizedStart, count, length);

        int resultLength = length - count;
        MutableArraySequence<T>* result = new MutableArraySequence<T>(resultLength);
        int resultIndex = 0;

        for (int i = 0; i < normalizedStart; ++i) {
            result->SetExistingItem(resultIndex, items.Get(i));
            ++resultIndex;
        }

        for (int i = normalizedStart + count; i < length; ++i) {
            result->SetExistingItem(resultIndex, items.Get(i));
            ++resultIndex;
        }

        return result;
    }

    template <class ReplacementDerived>
    MutableArraySequence<T>* SliceImpl(int startIndex, int count, const ReplacementDerived* replacement) const {
        int length = items.GetSize();
        int replacementLength = replacement == nullptr ? 0 : replacement->GetLength();
        int normalizedStart = NormalizeSliceStart(startIndex, length);

        ValidateSliceRange(normalizedStart, count, length);

        int resultLength = length - count + replacementLength;
        MutableArraySequence<T>* result = new MutableArraySequence<T>(resultLength);
        int resultIndex = 0;

        for (int i = 0; i < normalizedStart; ++i) {
            result->SetExistingItem(resultIndex, items.Get(i));
            ++resultIndex;
        }

        if (replacement != nullptr) {
            for (int i = 0; i < replacementLength; ++i) {
                result->SetExistingItem(resultIndex, replacement->Get(i));
                ++resultIndex;
            }
        }

        for (int i = normalizedStart + count; i < length; ++i) {
            result->SetExistingItem(resultIndex, items.Get(i));
            ++resultIndex;
        }

        return result;
    }
};

template <class T>
class MutableArraySequence : public ArraySequence<T, MutableArraySequence<T>> {
public:
    MutableArraySequence() : ArraySequence<T, MutableArraySequence<T>>() {}

    explicit MutableArraySequence(int size)
        : ArraySequence<T, MutableArraySequence<T>>(DynamicArray<T>(size)) {}

    MutableArraySequence(T* items, int count)
        : ArraySequence<T, MutableArraySequence<T>>(items, count) {}

    explicit MutableArraySequence(const DynamicArray<T>& array)
        : ArraySequence<T, MutableArraySequence<T>>(array) {}

    MutableArraySequence(const MutableArraySequence<T>& other)
        : ArraySequence<T, MutableArraySequence<T>>(other) {}

    MutableArraySequence<T>* Instance() {
        return this;
    }

    MutableArraySequence<T>* CloneImpl() const {
        return new MutableArraySequence<T>(*this);
    }

    bool IsMutableImpl() const {
        return true;
    }
};

template <class T>
class ImmutableArraySequence : public ArraySequence<T, ImmutableArraySequence<T>> {
public:
    ImmutableArraySequence() : ArraySequence<T, ImmutableArraySequence<T>>() {}

    ImmutableArraySequence(T* items, int count)
        : ArraySequence<T, ImmutableArraySequence<T>>(items, count) {}

    explicit ImmutableArraySequence(const DynamicArray<T>& array)
        : ArraySequence<T, ImmutableArraySequence<T>>(array) {}

    ImmutableArraySequence(const ImmutableArraySequence<T>& other)
        : ArraySequence<T, ImmutableArraySequence<T>>(other) {}

    ImmutableArraySequence<T>* Instance() {
        return new ImmutableArraySequence<T>(*this);
    }

    ImmutableArraySequence<T>* CloneImpl() const {
        return new ImmutableArraySequence<T>(*this);
    }

    bool IsMutableImpl() const {
        return false;
    }
};

#endif
