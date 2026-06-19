#ifndef OPTION_HPP
#define OPTION_HPP

#include "Exceptions.hpp"

template <class T>
class Option {
private:
    bool hasValue;
    T value;

public:
    Option() : hasValue(false), value{} {}

    explicit Option(const T& value) : hasValue(true), value(value) {}

    static Option<T> Some(const T& value) {
        return Option<T>(value);
    }

    static Option<T> None() {
        return Option<T>();
    }

    bool HasValue() const {
        return hasValue;
    }

    bool IsNone() const {
        return !hasValue;
    }

    T Value() const {
        if (!hasValue) {
            throw OptionHasNoValueException("attempt to read value from None");
        }

        return value;
    }
};

#endif