#ifndef BIT_HPP
#define BIT_HPP

#include "Exceptions.hpp"

class Bit {
private:
    bool value;

public:
    Bit() : value(false) {}

    Bit(bool value) : value(value) {}

    explicit Bit(int value) {
        if (value != 0 && value != 1) {
            throw InvalidArgumentException("bit value must be 0 or 1");
        }

        this->value = value == 1;
    }

    bool ToBool() const {
        return value;
    }

    int ToInt() const {
        return value ? 1 : 0;
    }

    Bit operator~() const {
        return Bit(!value);
    }

    Bit operator&(const Bit& other) const {
        return Bit(value && other.value);
    }

    Bit operator|(const Bit& other) const {
        return Bit(value || other.value);
    }

    Bit operator^(const Bit& other) const {
        return Bit(value != other.value);
    }

    bool operator==(const Bit& other) const {
        return value == other.value;
    }

    bool operator!=(const Bit& other) const {
        return !(*this == other);
    }
};

#endif
