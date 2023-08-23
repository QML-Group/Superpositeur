#pragma once

#include "superpositeur/Common.hpp"
#include "superpositeur/config/CompileTimeConfiguration.hpp"
#include "superpositeur/utils/FloatComparison.hpp"
#include <span>
#include <algorithm>
#include <cmath>

namespace superpositeur {

template <std::uint64_t MaxNumberOfQubits = 64>
inline bool applyGivensRotation(std::span<KeyValue<MaxNumberOfQubits>> firstLine, std::uint64_t& firstHash, std::span<KeyValue<MaxNumberOfQubits>> secondLine, std::uint64_t& secondHash) {
    assert(firstHash == secondHash);
    
    bool foundNonZeroEntry = false;

    std::complex<double> c = 0;
    std::complex<double> s = 0;

    auto firstIt = firstLine.begin();
    auto secondIt = secondLine.begin();
    while (firstIt != firstLine.end()) {
        assert(secondIt != secondLine.end());

        if (utils::isNull(firstIt->second)) {
            ++firstIt;
            continue;
        }

        if (utils::isNull(secondIt->second)) {
            ++secondIt;
            continue;
        }

        if (firstIt->first != secondIt->first) {
            throw std::runtime_error("Congrats, you found a hash collision");
        }

        if (!foundNonZeroEntry) {
            auto const& a = firstIt->second;
            auto const& b = secondIt->second;
            assert(std::hypot(std::abs(a), std::abs(b)) > config::ATOL);

            double const invr = 1 / std::hypot(std::abs(a), std::abs(b));

            c = b * invr;
            s = a * invr;

            foundNonZeroEntry = true;
        }

        auto const oldFirst = firstIt->second;
        firstIt->second = c * oldFirst - s * secondIt->second;
        secondIt->second = std::conj(s) * oldFirst + std::conj(c) * secondIt->second;

        if (utils::isNull(firstIt->second)) {
            firstHash -= firstIt->first.hash();
        }

        if (utils::isNull(secondIt->second)) {
            secondHash -= secondIt->first.hash();
        }

        ++firstIt;
        ++secondIt;
    }

    return foundNonZeroEntry;
}

} // namespace  superpositeur