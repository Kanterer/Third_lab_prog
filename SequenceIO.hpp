#ifndef SEQUENCE_IO_HPP
#define SEQUENCE_IO_HPP

#include "Sequence.hpp"
#include <ostream>

template <class T, class Derived>
void PrintSequence(const Sequence<T, Derived>& sequence, std::ostream& out) {
    out << "[";

    int length = sequence.GetLength();

    for (int i = 0; i < length; ++i) {
        if (i != 0) {
            out << ", ";
        }

        out << sequence.Get(i);
    }

    out << "]";
}

template <class T, class Derived>
std::ostream& operator<<(std::ostream& out, const Sequence<T, Derived>& sequence) {
    PrintSequence(sequence, out);

    return out;
}

#endif