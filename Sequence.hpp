#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include "Exceptions.hpp"
#include "Option.hpp"

template <class T, class Derived>
class Sequence {
private:
    Derived& Self() {
        return static_cast<Derived&>(*this);
    }

    const Derived& Self() const {
        return static_cast<const Derived&>(*this);
    }

public:
    T GetFirst() const {
        return Self().GetFirstImpl();
    }

    T GetLast() const {
        return Self().GetLastImpl();
    }

    T Get(int index) const {
        return Self().GetImpl(index);
    }

    auto GetSubsequence(int startIndex, int endIndex) const {
        return Self().GetSubsequenceImpl(startIndex, endIndex);
    }

    int GetLength() const {
        return Self().GetLengthImpl();
    }

    auto Append(T item) {
        return Self().AppendImpl(item);
    }

    auto Prepend(T item) {
        return Self().PrependImpl(item);
    }

    auto InsertAt(T item, int index) {
        return Self().InsertAtImpl(item, index);
    }

    template <class OtherDerived>
    auto Concat(const Sequence<T, OtherDerived>& sequence) {
        return Self().ConcatImpl(static_cast<const OtherDerived&>(sequence));
    }

    auto Clone() const {
        return Self().CloneImpl();
    }

    auto EmptyMutable() const {
        return Self().EmptyMutableImpl();
    }

    bool IsMutable() const {
        return Self().IsMutableImpl();
    }

    T operator[](int index) const {
        return Get(index);
    }

    auto Map(T (*mapper)(const T&)) const {
        if (mapper == nullptr) {
            throw InvalidArgumentException("mapper function is null");
        }

        return Self().MapImpl(mapper);
    }

    auto MapIndexed(T (*mapper)(const T&, int)) const {
        if (mapper == nullptr) {
            throw InvalidArgumentException("mapper function is null");
        }

        return Self().MapIndexedImpl(mapper);
    }

    auto Where(bool (*predicate)(const T&)) const {
        if (predicate == nullptr) {
            throw InvalidArgumentException("predicate function is null");
        }

        return Self().WhereImpl(predicate);
    }

    template <class R>
    R Reduce(R (*reducer)(const R&, const T&), R initial) const {
        if (reducer == nullptr) {
            throw InvalidArgumentException("reducer function is null");
        }

        R accumulator = initial;
        int length = GetLength();

        for (int i = 0; i < length; ++i) {
            accumulator = reducer(accumulator, Get(i));
        }

        return accumulator;
    }

    Option<T> TryGetFirst(bool (*predicate)(const T&) = nullptr) const {
        int length = GetLength();

        if (length == 0) {
            return Option<T>::None();
        }

        if (predicate == nullptr) {
            return Option<T>::Some(GetFirst());
        }

        for (int i = 0; i < length; ++i) {
            T item = Get(i);

            if (predicate(item)) {
                return Option<T>::Some(item);
            }
        }

        return Option<T>::None();
    }

    Option<T> TryGetLast(bool (*predicate)(const T&) = nullptr) const {
        int length = GetLength();

        if (length == 0) {
            return Option<T>::None();
        }

        if (predicate == nullptr) {
            return Option<T>::Some(GetLast());
        }

        for (int i = length - 1; i >= 0; --i) {
            T item = Get(i);

            if (predicate(item)) {
                return Option<T>::Some(item);
            }
        }

        return Option<T>::None();
    }

    auto Slice(int startIndex, int count) const {
        return Self().SliceWithoutReplacementImpl(startIndex, count);
    }

    template <class ReplacementDerived>
    auto Slice(int startIndex, int count, const Sequence<T, ReplacementDerived>* replacement) const {
        return Self().SliceImpl(startIndex, count, static_cast<const ReplacementDerived*>(replacement));
    }
};

#endif