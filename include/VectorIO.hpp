#ifndef VECTOR_IO_HPP
#define VECTOR_IO_HPP

#include "Vector.hpp"

#include <ostream>

template <class T>
std::ostream& operator<<(std::ostream& out, const Vector<T>& vector) {
    out << "[";

    for (int i = 0; i < vector.GetSize(); ++i) {
        if (i != 0) {
            out << ", ";
        }

        out << vector.Get(i);
    }

    out << "]";
    return out;
}

#endif
