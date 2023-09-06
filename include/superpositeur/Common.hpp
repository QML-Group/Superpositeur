#pragma once

#include <complex>
#include <vector>

#include "superpositeur/utils/BitSet.hpp"
#include "superpositeur/Matrix.hpp"

namespace superpositeur {

template <std::uint64_t N>
using BasisVector = utils::BitSet<N>;

template <std::uint64_t MaxNumberOfQubits>
struct KeyValue {
    static constexpr std::uint64_t MAX_NUMBER_OF_BITS = MaxNumberOfQubits;

    BasisVector<MaxNumberOfQubits> ket;
    std::complex<double> amplitude;
};

template <std::uint64_t MaxNumberOfQubits>
using SparseVector = std::vector<KeyValue<MaxNumberOfQubits>>; // Association list.

using Sizes = std::vector<std::uint64_t>;
using Hashes = std::vector<std::uint64_t>;

using KrausOperators = std::vector<Matrix>;

} // namespace  superpositeur