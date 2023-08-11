#pragma once

#include <complex>

#include "superpositeur/config/CompileTimeConfiguration.hpp"

namespace superpositeur {
namespace utils {

/*

Comparing floating point numbers is a complex topic in general:
see for instance https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison

However, it is worth mentioning here that ALL the doubles in this program are somehow real or imaginary parts or modulus
of complex numbers involved in quantum states, and their modulus is always <= 1.

*/

inline constexpr bool isNotNull(double d) {
#if defined(_MSC_VER)
    return d > config::ATOL || -d > config::ATOL;
#else
    return std::abs(d) > config::ATOL;
#endif
}

inline constexpr bool isNotNull(std::complex<double> c) {
    return isNotNull(c.real()) || isNotNull(c.imag());
}

template <typename T> inline constexpr bool isNull(T t) {
    return !isNotNull(t);
}

} // namespace utils
} // namespace  superpositeur