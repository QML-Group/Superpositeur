#pragma once

#include "superpositeur/Common.hpp"
#include "superpositeur/config/CompileTimeConfiguration.hpp"
#include "superpositeur/utils/FloatComparison.hpp"
#include <span>
#include <algorithm>
#include <cmath>

namespace superpositeur {

template <std::uint64_t MaxNumberOfQubits = 64>
inline void applyGivensRotation(std::span<KeyValue<MaxNumberOfQubits>>& firstLine, std::uint64_t& firstHash, std::span<KeyValue<MaxNumberOfQubits>>& secondLine, std::uint64_t& secondHash) {
    assert(!firstLine.empty());
    assert(!secondLine.empty());

    assert(firstHash == secondHash);

    bool foundNonZeroEntry = false;

    std::complex<double> c = 0;
    std::complex<double> s = 0;

    auto firstIt = firstLine.begin();
    auto secondIt = secondLine.begin();
    while (firstIt != firstLine.end()) {
        if (utils::isNull(firstIt->amplitude)) [[unlikely]] {
            ++firstIt;
            if (!foundNonZeroEntry) {
                firstLine = firstLine.subspan(1);
            }
            continue;
        }

        assert(secondIt != secondLine.end());

        if (utils::isNull(secondIt->amplitude)) [[unlikely]] {
            ++secondIt;
            if (!foundNonZeroEntry) {
                secondLine = secondLine.subspan(1);
            }
            continue;
        }

        if (firstIt->ket != secondIt->ket) [[unlikely]] {
            throw std::runtime_error("Congrats, you found a hash collision");
        }

        if (!foundNonZeroEntry) [[unlikely]] {
            auto const& a = firstIt->amplitude;
            auto const& b = secondIt->amplitude;
            assert(std::hypot(std::abs(a), std::abs(b)) > config::ATOL);

            double const invr = 1 / std::hypot(std::abs(a), std::abs(b));

            c = b * invr;
            s = a * invr;

            foundNonZeroEntry = true;
        }

        auto const oldFirst = firstIt->amplitude;
        firstIt->amplitude = c * oldFirst - s * secondIt->amplitude;
        secondIt->amplitude = std::conj(s) * oldFirst + std::conj(c) * secondIt->amplitude;

        if (utils::isNull(firstIt->amplitude)) [[unlikely]] {
            firstHash -= firstIt->ket.hash();
        }

        if (utils::isNull(secondIt->amplitude)) [[unlikely]] {
            secondHash -= secondIt->ket.hash();
        }

        ++firstIt;
        ++secondIt;
    }
}

} // namespace  superpositeur