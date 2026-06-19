#ifndef LIST_SEQUENCE_HPP
#define LIST_SEQUENCE_HPP

#include "LinkedList.hpp"
#include "Sequence.hpp"

template <class T>
class MutableListSequence;

template <class T>
class ImmutableListSequence;

template <class T, class Derived>
class ListSequence : public Sequence<T, Derived> {
protected:
    LinkedList<T> items;

    Derived* SelfDerived() {
        return static_cast<Derived*>(this);
    }

    const Derived* SelfDerived() const {
        return static_cast<const Derived*>(this);
    }

public:
    void AppendInternal(T item) {
        items.Append(item);
    }

    void PrependInternal(T item) {
        items.Prepend(item);
    }

    void InsertAtInternal(T item, int index) {
        items.InsertAt(item, index);
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
    ListSequence() : items() {}

    ListSequence(T* inputItems, int count) : items(inputItems, count) {}

    explicit ListSequence(const LinkedList<T>& list) : items(list) {}

    ListSequence(const ListSequence<T, Derived>& other) : items(other.items) {}

    T GetFirstImpl() const {
        return items.GetFirst();
    }

    T GetLastImpl() const {
        return items.GetLast();
    }

    T GetImpl(int index) const {
        return items.Get(index);
    }

    MutableListSequence<T>* GetSubsequenceImpl(int startIndex, int endIndex) const {
        int length = items.GetLength();

        if (startIndex < 0 || endIndex < 0 || startIndex >= length || endIndex >= length) {
            throw IndexOutOfRangeException("subsequence boundaries are outside ListSequence bounds");
        }

        if (startIndex > endIndex) {
            throw InvalidArgumentException("startIndex cannot be greater than endIndex");
        }

        MutableListSequence<T>* result = new MutableListSequence<T>();

        const typename LinkedList<T>::Node* current = items.head;
        int currentIndex = 0;

        while (currentIndex < startIndex) {
            current = current->next;
            ++currentIndex;
        }

        while (currentIndex <= endIndex) {
            result->AppendInternal(current->value);
            current = current->next;
            ++currentIndex;
        }

        return result;
    }

    int GetLengthImpl() const {
        return items.GetLength();
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
        int length = sequence.GetLength();

        for (int i = 0; i < length; ++i) {
            target->AppendInternal(sequence.Get(i));
        }

        return target;
    }

    T& operator[](int index) {
        return items[index];
    }

    const T& operator[](int index) const {
        return items[index];
    }

    MutableListSequence<T>* EmptyMutableImpl() const {
        return new MutableListSequence<T>();
    }

    MutableListSequence<T>* MapImpl(T (*mapper)(const T&)) const {
        MutableListSequence<T>* result = new MutableListSequence<T>();

        const typename LinkedList<T>::Node* current = items.head;

        while (current != nullptr) {
            result->AppendInternal(mapper(current->value));
            current = current->next;
        }

        return result;
    }

    MutableListSequence<T>* MapIndexedImpl(T (*mapper)(const T&, int)) const {
        MutableListSequence<T>* result = new MutableListSequence<T>();

        const typename LinkedList<T>::Node* current = items.head;
        int index = 0;

        while (current != nullptr) {
            result->AppendInternal(mapper(current->value, index));
            current = current->next;
            ++index;
        }

        return result;
    }

    MutableListSequence<T>* WhereImpl(bool (*predicate)(const T&)) const {
        MutableListSequence<T>* result = new MutableListSequence<T>();

        const typename LinkedList<T>::Node* current = items.head;

        while (current != nullptr) {
            if (predicate(current->value)) {
                result->AppendInternal(current->value);
            }

            current = current->next;
        }

        return result;
    }

    MutableListSequence<T>* SliceWithoutReplacementImpl(int startIndex, int count) const {
        int length = items.GetLength();
        int normalizedStart = NormalizeSliceStart(startIndex, length);

        ValidateSliceRange(normalizedStart, count, length);

        MutableListSequence<T>* result = new MutableListSequence<T>();

        const typename LinkedList<T>::Node* current = items.head;
        int index = 0;

        while (current != nullptr) {
            bool shouldRemove = index >= normalizedStart && index < normalizedStart + count;

            if (!shouldRemove) {
                result->AppendInternal(current->value);
            }

            current = current->next;
            ++index;
        }

        return result;
    }

    template <class ReplacementDerived>
    MutableListSequence<T>* SliceImpl(int startIndex, int count, const ReplacementDerived* replacement) const {
        int length = items.GetLength();
        int normalizedStart = NormalizeSliceStart(startIndex, length);

        ValidateSliceRange(normalizedStart, count, length);

        MutableListSequence<T>* result = new MutableListSequence<T>();

        const typename LinkedList<T>::Node* current = items.head;
        int index = 0;

        while (current != nullptr && index < normalizedStart) {
            result->AppendInternal(current->value);
            current = current->next;
            ++index;
        }

        for (int i = 0; replacement != nullptr && i < replacement->GetLength(); ++i) {
            result->AppendInternal(replacement->Get(i));
        }

        int skipped = 0;

        while (current != nullptr && skipped < count) {
            current = current->next;
            ++index;
            ++skipped;
        }

        while (current != nullptr) {
            result->AppendInternal(current->value);
            current = current->next;
            ++index;
        }

        return result;
    }
};

template <class T>
class MutableListSequence : public ListSequence<T, MutableListSequence<T>> {
public:
    MutableListSequence() : ListSequence<T, MutableListSequence<T>>() {}

    MutableListSequence(T* items, int count)
        : ListSequence<T, MutableListSequence<T>>(items, count) {}

    explicit MutableListSequence(const LinkedList<T>& list)
        : ListSequence<T, MutableListSequence<T>>(list) {}

    MutableListSequence(const MutableListSequence<T>& other)
        : ListSequence<T, MutableListSequence<T>>(other) {}

    MutableListSequence<T>* Instance() {
        return this;
    }

    MutableListSequence<T>* CloneImpl() const {
        return new MutableListSequence<T>(*this);
    }

    bool IsMutableImpl() const {
        return true;
    }
};

template <class T>
class ImmutableListSequence : public ListSequence<T, ImmutableListSequence<T>> {
public:
    ImmutableListSequence() : ListSequence<T, ImmutableListSequence<T>>() {}

    ImmutableListSequence(T* items, int count)
        : ListSequence<T, ImmutableListSequence<T>>(items, count) {}

    explicit ImmutableListSequence(const LinkedList<T>& list)
        : ListSequence<T, ImmutableListSequence<T>>(list) {}

    ImmutableListSequence(const ImmutableListSequence<T>& other)
        : ListSequence<T, ImmutableListSequence<T>>(other) {}

    ImmutableListSequence<T>* Instance() {
        return new ImmutableListSequence<T>(*this);
    }

    ImmutableListSequence<T>* CloneImpl() const {
        return new ImmutableListSequence<T>(*this);
    }

    bool IsMutableImpl() const {
        return false;
    }
};

#endif