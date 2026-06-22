#ifndef NUMERIC_UTILS_HPP
#define NUMERIC_UTILS_HPP

#include <cmath>
#include <complex>

template <class T>
bool IsZeroValue(const T& value) {
    return value == T{};
}

inline bool IsZeroValue(const double& value) {
    return std::fabs(value) < 1e-12;
}

inline bool IsZeroValue(const float& value) {
    return std::fabs(value) < 1e-6f;
}

inline bool IsZeroValue(const long double& value) {
    return std::fabs(value) < 1e-15L;
}

inline bool IsZeroValue(const std::complex<double>& value) {
    return std::abs(value) < 1e-12;
}

inline bool IsZeroValue(const std::complex<float>& value) {
    return std::abs(value) < 1e-6f;
}

inline bool IsZeroValue(const std::complex<long double>& value) {
    return std::abs(value) < 1e-15L;
}

template <class T>
double MagnitudeSquared(const T& value) {
    double converted = static_cast<double>(value);
    return converted * converted;
}

inline double MagnitudeSquared(const std::complex<double>& value) {
    return std::norm(value);
}

inline double MagnitudeSquared(const std::complex<float>& value) {
    return static_cast<double>(std::norm(value));
}

inline double MagnitudeSquared(const std::complex<long double>& value) {
    return static_cast<double>(std::norm(value));
}

#endif
