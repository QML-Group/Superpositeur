#pragma once

#include "superpositeur/Common.hpp"
#include "superpositeur/config/CompileTimeConfiguration.hpp"
#include "superpositeur/utils/FloatComparison.hpp"
#include <span>
#include <algorithm>
#include <cmath>

namespace superpositeur {

template <std::uint64_t MaxNumberOfQubits>
using Line = std::pair<typename SparseVector<MaxNumberOfQubits>::iterator, typename SparseVector<MaxNumberOfQubits>::iterator>;

template <std::uint64_t MaxNumberOfQubits = 64>
inline void applyGivensRotation(Line<MaxNumberOfQubits>& firstLine, std::uint64_t& firstHash, Line<MaxNumberOfQubits>& secondLine, std::uint64_t& secondHash) {
    assert(firstLine.first != firstLine.second);
    assert(secondLine.first != secondLine.second);

    assert(firstHash == secondHash);

    auto firstIt = firstLine.first;
    auto secondIt = secondLine.first;

    auto const& a = firstIt->second;
    auto const& b = secondIt->second;
    assert(std::hypot(std::abs(a), std::abs(b)) > config::ATOL);

    double const invr = 1 / std::hypot(std::abs(a), std::abs(b));

    auto c = b * invr;
    auto s = a * invr;

    while (true) {
        auto const oldFirst = firstIt->second;
        firstIt->second = c * oldFirst - s * secondIt->second;
        secondIt->second = std::conj(s) * oldFirst + std::conj(c) * secondIt->second;

        if (utils::isNull(firstIt->second)) [[unlikely]] {
            firstHash -= firstIt->first.hash();
        }

        if (utils::isNull(secondIt->second)) [[unlikely]] {
            secondHash -= secondIt->first.hash();
        }

        do {
            ++firstIt;
        } while (firstIt != firstLine.second && utils::isNull(firstIt->second));

        do {
            ++secondIt;
        } while (secondIt != secondLine.second && utils::isNull(secondIt->second));

        if (firstIt == firstLine.second && secondIt == secondLine.second) {
            break;
        }

        if (firstIt == firstLine.second || secondIt == secondLine.second || firstIt->first != secondIt->first) [[unlikely]] {
            throw std::runtime_error("Congrats, you found a hash collision");
        }
    }

    while (firstLine.first < firstLine.second && utils::isNull(firstLine.first->second)) {
        ++firstLine.first;
    }

    while (secondLine.first < secondLine.second && utils::isNull(secondLine.first->second)) {
        ++secondLine.first;
    }
}

} // namespace  superpositeur