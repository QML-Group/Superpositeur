#pragma once

#include <cassert>
#include <complex>
#include <limits>
#include <ranges>
#include <algorithm>
#include <variant>
#include <atomic>

#include "superpositeur/CircuitInstruction.hpp"
#include "superpositeur/Common.hpp"
#include "superpositeur/GivensRotation.hpp"
#include "superpositeur/MatrixSparseVectorMultiplication.hpp"
#include "superpositeur/utils/FloatComparison.hpp"
#include "superpositeur/StrongTypes.hpp"

namespace superpositeur {

class MixedState {
public:
    explicit MixedState() : dataVariant(SparseVector<64>{{BasisVector<64>{}, 1.}}), sizes({1}), hashes({0}) {}

    void reset() {
        dataVariant = SparseVector<64>{{BasisVector<64>{}, 1.}};
        sizes = {1};
        hashes = {0};
    }

    std::uint64_t currentSize() const {
        return std::visit([](auto const& data) { return std::remove_reference<decltype(data)>::type::value_type::first_type::getNumberOfBits(); }, dataVariant);
    }

    template <std::uint64_t NumberOfQubits>
    void resize() {
        static_assert(NumberOfQubits % 64 == 0); // FIXME
        assert(currentSize() < NumberOfQubits);
        assert(NumberOfQubits == 128); // FIXME
        throw std::runtime_error("Unimplemented");
    }

    bool operator==(MixedState const &) const {
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
        std::vector<std::span<KeyValue<MaxNumberOfQubits>>> lines;
        lines.reserve(sizes.size());

        auto it = data.begin();
        for (auto s: sizes) {
            assert(it < data.end());
            assert(static_cast<std::uint64_t>(std::distance(it, data.end())) >= s);
            auto next = std::next(it, s);
            lines.emplace_back(it, next);
            it = next;
        }

        bool keepGoing = true;

        while (keepGoing) {
            keepGoing = false;

            std::vector<std::atomic_flag> used(hashes.size());
            for (auto& f: used) { f.clear(); }

            for (std::uint64_t index1 = 0; index1 < hashes.size(); ++index1) {
                for (std::uint64_t index2 = index1 + 1; index2 < hashes.size(); ++index2) {
                    if (!used[index1].test() && !used[index2].test() && hashes[index1] == hashes[index2]) { // FIXME: 2 indices in used need to be checked inside a lock
                        used[index1].test_and_set();
                        used[index2].test_and_set();

                        keepGoing |= applyGivensRotation(lines[index1], hashes[index1], lines[index2], hashes[index2]); // FIXME: do async

                        used[index1].clear();
                        used[index2].clear();
                    }
                }
            }
        }
    }

    void operator()(CircuitInstruction const &circuitInstruction) {
        simplify();
        
        auto bitWidth = circuitInstruction.getOperandsMask().bitWidth();
        if (bitWidth > currentSize()) {
            if (bitWidth <= 128) {
                resize<128>();
            } else {
                throw std::runtime_error("Cannot handle that many qubits!");
            }
        }

        std::visit([&](auto&& variant) {
            applyCircuitInstruction(circuitInstruction, variant);
        }, dataVariant);

        assert(isConsistent());
    }

    template <std::uint64_t MaxNumberOfQubits>
    void applyCircuitInstruction(CircuitInstruction const &circuitInstruction, SparseVector<MaxNumberOfQubits>& data) {
        if (!circuitInstruction.getControlQubitsMask().empty()) {
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
                if (next.empty()) {
                    internals.it2 = internals.end;
                    break;
                }

                assert(next > internals.it2->first);
                internals.it2 = std::ranges::lower_bound(internals.it2, internals.end, next, {}, [](auto &&x) { return x.first; });
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
        assert(std::ranges::find_if(qubits.rbegin(), qubits.rend(), std::identity{}) - qubits.rend() < 128); // FIXME

        return std::visit([&](auto&& x) {
            return ReducedDensityMatrixIterator(x, sizes, qubits);
        }, dataVariant);
    }

    Matrix getReducedDensityMatrix(std::vector<bool> const& mask) {
        auto popcount = std::ranges::count_if(mask, [](auto x) { return x; });

        assert(popcount > 0 && popcount < 64);

        auto reducedDensityMatrixIterator = getReducedDensityMatrixIterator(mask);

        Matrix m(1UL << popcount, 1UL << popcount);
        while (auto densityMatrixEntry = reducedDensityMatrixIterator.next()) {
            auto i = std::get<0>(*densityMatrixEntry);
            auto j = std::get<1>(*densityMatrixEntry);
            auto v = std::get<2>(*densityMatrixEntry);
            m.add(i, j, v);

            if (i != j) {
                m.add(j, i, std::conj(v));
            }
        }
        return m;
    }

    // Matrix getMatrixOfVectors() const {
    //     return std::visit([&](auto const& data) {
    //         Matrix m(sizes.size(), 1UL << getNumberOfQubits(???));

    //         auto it = data.cbegin();
    //         for (std::uint64_t i = 0; i < sizes.size(); ++i) {
    //             for (std::uint64_t x = 0; x < sizes[i]; ++x) {
    //                 assert(it != data.cend());
    //                 std::uint64_t j = it->first.toUInt64();
    //                 m.set(i, j, it->second);
    //                 ++it;
    //             }
    //         }
    //         return m;
    //     }, dataVariant);
    // }

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

    std::variant<SparseVector<64>, SparseVector<128>> dataVariant;
    Sizes sizes;
    Hashes hashes;
};

} // namespace  superpositeur