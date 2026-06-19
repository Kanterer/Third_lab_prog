#ifndef BIT_SEQUENCE_IO_HPP
#define BIT_SEQUENCE_IO_HPP

#include "BitSequence.hpp"
#include <ostream>

inline std::ostream& operator<<(std::ostream& out, const BitSequence& bits) {
    for (int i = 0; i < bits.GetLength(); ++i) {
        out << bits.GetBit(i).ToInt();
    }

    return out;
}

#endif