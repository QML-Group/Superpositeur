#pragma once

#include <cassert>
#include <complex>
#include <limits>
#include <ranges>
#include <algorithm>

#include "superpositeur/CircuitInstruction.hpp"
#include "superpositeur/Common.hpp"
#include "superpositeur/GivensRotation.hpp"
#include "superpositeur/MatrixSparseVectorMultiplication.hpp"
#include "superpositeur/utils/FloatComparison.hpp"
#include "superpositeur/StrongTypes.hpp"

namespace superpositeur {

class MixedState {
public:
    explicit MixedState(std::uint64_t n) : numberOfQubits(n), dataVariant(initDataVariant(numberOfQubits)), sizes({1}), hashes({0}) {}

    std::uint64_t getNumberOfQubits() const { return numberOfQubits; }

    bool operator==(MixedState const &other) const {
        if (getNumberOfQubits() != other.getNumberOfQubits()) {
            return false;
        }

        throw std::runtime_error("Measure of distance between two mixed states not implemented");
    }

    void simplify() {
        // TODO: assert equality before/after simplification.

        if (sizes.size() <= 1) {
            return;
        }

        std::visit([&](auto &data) {
            simplify(data);
        }, dataVariant);
    }

    template <std::uint64_t MaxNumberOfQubits>
    void simplify(SparseVector<MaxNumberOfQubits> &data) {
        std::vector<std::uint64_t> sizesPartialSum;
        sizesPartialSum.reserve(sizes.size() + 1);
        sizesPartialSum.push_back(0);
        std::partial_sum(sizes.begin(), sizes.end(), std::back_inserter(sizesPartialSum));
        assert(sizesPartialSum.size() == sizes.size() + 1);

        std::vector<std::uint64_t> perm(hashes.size(), 0);
        std::iota(perm.begin(), perm.end(), 0);
        std::ranges::sort(perm, {}, [&](auto x) { return hashes[x]; });

        auto start = perm.begin();
        while ((start = std::ranges::adjacent_find(start, perm.end(), {}, [&](auto &&x) { return hashes[x]; })) != perm.end()) {
            auto firstIndex = *start;
            auto secondIndex = *std::next(start);

            assert(firstIndex < sizes.size());
            assert(secondIndex < sizes.size());

            if (sizes[firstIndex] != sizes[secondIndex]) {
                ++start;
                continue;
            }
            
            std::span<KeyValue<MaxNumberOfQubits>> firstLine(data.begin() + sizesPartialSum[firstIndex], data.begin() + sizesPartialSum[firstIndex + 1]);
            std::span<KeyValue<MaxNumberOfQubits>> secondLine(data.begin() + sizesPartialSum[secondIndex], data.begin() + sizesPartialSum[secondIndex + 1]);

            // FIXME: once collisions are seen to be sufficiently rare, move this to an assert.
            if (!std::ranges::equal(firstLine, secondLine, {}, &KeyValue<MaxNumberOfQubits>::first, &KeyValue<MaxNumberOfQubits>::first)) {
                throw std::runtime_error("Congrats, you found a hash collision");
            };

            applyGivensRotation(firstLine, secondLine);

            ++start;
        }
    }

    void operator()(CircuitInstruction const &circuitInstruction) {
        simplify();
        
        std::visit([&](auto&& variant) {
            applyCircuitInstruction(circuitInstruction, variant);
        }, dataVariant);

        assert(isConsistent());
    }

    template <std::uint64_t MaxNumberOfQubits>
    void applyCircuitInstruction(CircuitInstruction const &circuitInstruction, SparseVector<MaxNumberOfQubits>& data) {
        if (circuitInstruction.getControlQubitsMask() != 0) {
            throw std::runtime_error("Unimplemented: control qubits");
        }

        SparseVector<MaxNumberOfQubits> newData;
        newData.reserve(data.size());

        Sizes newSizes;
        newSizes.reserve(sizes.size());

        hashes.clear();

        auto start = data.begin();

        for (auto size: sizes) {
            auto end = std::next(start, size);

            for (auto const& krausOperator: circuitInstruction.getKrausOperators()) {
                auto aux = multiplyMatrix(krausOperator, {start, end}, circuitInstruction.getOperandsMask(), std::back_inserter(newData));
                if (aux.numberOfAddedElements > 0) {
                    newSizes.push_back(aux.numberOfAddedElements);
                    hashes.push_back(aux.hashOfTheKeys);
                }
            }

            start = end;
        }

        data.swap(newData);
        sizes.swap(newSizes);
    }

private:

private:
    struct ReducedDensityMatrixIterator {
        using Value = std::tuple<std::uint64_t, std::uint64_t, std::complex<double>>; // i, j, M[i, j]

        template <std::uint64_t N>
        ReducedDensityMatrixIterator(SparseVector<N> const& data, Sizes const& sizes, std::vector<bool> mask) :
            internalsVariant(Internals<N>{.end = data.begin() + sizes[0], .it1 = data.begin(), .it2 = data.begin(), .reductionQubits = getMask<N>(mask)}),
            sizesIterator(sizes.begin()), sizesEnd(sizes.end()) {}

        // Should this own shared ownership of data?

        std::optional<Value> next() {
            return std::visit([&](auto& internals) { return nextImpl(internals); }, internalsVariant);
        }

    private:
        template <std::uint64_t N>
        static BasisVector<N> getMask(std::vector<bool> v) {
            BasisVector<N> mask;
            for (std::uint64_t i = 0; i < v.size(); ++i) {
                mask.set(i, v[i]);
            }
            return mask;
        }

        template <typename Internals>
        std::optional<Value> nextImpl(Internals& internals) {
            assert(internals.it1 <= internals.it2);

            if (internals.it1 == internals.end) {
                assert(internals.it1 == internals.it2);

                if (sizesIterator == sizesEnd || ++sizesIterator == sizesEnd) {
                    return std::nullopt;
                }

                assert(*sizesIterator != 0);
                internals.end += *sizesIterator;
            }
            
            assert(sizesIterator != sizesEnd && *sizesIterator > 0);
            assert(internals.it2 != internals.end);
            assert((internals.it1->first & (~internals.reductionQubits)) == (internals.it2->first & (~internals.reductionQubits)));

            Value result = Value(internals.it1->first.pext(internals.reductionQubits), internals.it2->first.pext(internals.reductionQubits), internals.it1->second * std::conj(internals.it2->second));

            auto current = internals.it1->first & (~internals.reductionQubits);
            do {
                auto next = internals.it2->first.nextWithBits(~internals.reductionQubits, current);
                if (!next) {
                    internals.it2 = internals.end;
                    break;
                }

                assert((*next) > internals.it2->first);
                internals.it2 = std::ranges::lower_bound(internals.it2, internals.end, *next, {}, [](auto &&x) { return x.first; });
            } while (internals.it2 != internals.end && ((internals.it2->first & (~internals.reductionQubits)) != current));

            if (internals.it2 == internals.end) {
                ++internals.it1;
                internals.it2 = internals.it1;
            }

            return result;
        }

        template <std::uint64_t N>
        struct Internals {
            typename SparseVector<N>::const_iterator end;
            typename SparseVector<N>::const_iterator it1;
            typename SparseVector<N>::const_iterator it2;
            BasisVector<N> reductionQubits;
        };

        std::variant<Internals<64UL>, Internals<128UL>> internalsVariant;

        Sizes::const_iterator sizesIterator;
        Sizes::const_iterator const sizesEnd;
    };

public:
    ReducedDensityMatrixIterator getReducedDensityMatrixIterator(std::vector<bool> qubits) const {
        assert(isConsistent());
        assert(qubits.size() <= getNumberOfQubits()); // In fact: only highest set bit must be <= getNumberOfQubits().

        return std::visit([&](auto&& x) {
            return ReducedDensityMatrixIterator(x, sizes, qubits);
        }, dataVariant);
    }

    Matrix getReducedDensityMatrix(std::optional<std::vector<bool>> const& maskOptional = std::nullopt) {
        auto popcount = maskOptional ? std::ranges::count_if(*maskOptional, [](auto x) { return x; }) : getNumberOfQubits();

        assert(popcount > 0 && popcount < 64);

        auto reducedDensityMatrixIterator = getReducedDensityMatrixIterator(maskOptional ? *maskOptional : std::vector<bool>(getNumberOfQubits(), true));

        Matrix m(1UL << popcount, 1UL << popcount);
        while (auto densityMatrixEntry = reducedDensityMatrixIterator.next()) {
            auto i = std::get<0>(*densityMatrixEntry);
            auto j = std::get<1>(*densityMatrixEntry);
            auto v = std::get<2>(*densityMatrixEntry);
            m.add(i, j, v);

            if (i != j) {
                m.add(j, i, v);
            }
        }
        return m;
    }

    Matrix getMatrixOfVectors() const {
        if (getNumberOfQubits() > 8) {
            throw std::runtime_error("This is already quite large yo - function indeed for testing purposes");
        }

        return std::visit([&](auto const& data) {
            Matrix m(sizes.size(), 1UL << getNumberOfQubits());

            auto it = data.cbegin();
            for (std::uint64_t i = 0; i < sizes.size(); ++i) {
                for (std::uint64_t x = 0; x < sizes[i]; ++x) {
                    assert(it != data.cend());
                    std::uint64_t j = it->first.toUInt64();
                    m.set(i, j, it->second);
                    ++it;
                }
            }
            return m;
        }, dataVariant);
    }

private:
    static std::variant<SparseVector<64>, SparseVector<128>> initDataVariant(std::uint64_t n) {
        if (n <= 64) {
            return SparseVector<64>{{BasisVector<64>{}, 1.}};
        }
        
        if (n <= 128) {
            return SparseVector<128>{{BasisVector<128>{}, 1.}};
        }

        throw std::runtime_error("Cannot handle this many qubits in this version of QX-simulator");
    }

    bool isConsistent() const;

    std::uint64_t const numberOfQubits = 1;
    std::variant<SparseVector<64>, SparseVector<128>> dataVariant;
    Sizes sizes;
    Hashes hashes;
};

} // namespace  superpositeur