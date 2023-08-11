#pragma once

#include "superpositeur/Common.hpp"
#include "superpositeur/config/CompileTimeConfiguration.hpp"
#include "superpositeur/utils/FloatComparison.hpp"
#include <span>
#include <algorithm>
#include <cmath>

namespace superpositeur {

template <std::uint64_t MaxNumberOfQubits = 64>
inline void applyGivensRotation(std::span<KeyValue<MaxNumberOfQubits>> firstLine, std::span<KeyValue<MaxNumberOfQubits>> secondLine) {
    assert(std::ranges::equal(firstLine, secondLine, {}, &KeyValue<MaxNumberOfQubits>::first, &KeyValue<MaxNumberOfQubits>::first) && "Indices not equal");

    std::complex<double> const a = firstLine.begin()->second;
    std::complex<double> const b = secondLine.begin()->second;
    assert(std::hypot(std::abs(a), std::abs(b)) > config::ATOL);

    double const invr = 1 / std::hypot(std::abs(a), std::abs(b));

    std::complex<double> const c = b * invr;
    std::complex<double> const s = a * invr;
    std::complex<double> const cConj = std::conj(c);
    std::complex<double> const sConj = std::conj(s);

    auto firstIt = firstLine.begin();
    auto secondIt = secondLine.begin();
    while (firstIt != firstLine.end()) {
        assert(secondIt != secondLine.end());

        auto const oldFirst = firstIt->second;
        firstIt->second = c * oldFirst - s * secondIt->second;
        secondIt->second = sConj * oldFirst + cConj * secondIt->second;

        std::advance(firstIt, 1);
        std::advance(secondIt, 1);
    }

    assert(utils::isNull(firstLine.begin()->second));
}

} // namespace  superpositeur