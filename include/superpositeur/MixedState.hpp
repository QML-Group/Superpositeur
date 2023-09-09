#pragma once

#include <cassert>
#include <complex>
#include <limits>
#include <ranges>
#include <algorithm>
#include <variant>
#include <functional>
#include <unordered_map>
#include <execution>
#include "superpositeur/CircuitInstruction.hpp"
#include "superpositeur/Common.hpp"
#include "superpositeur/GivensRotation.hpp"
#include "superpositeur/MatrixSparseVectorMultiplication.hpp"
#include "superpositeur/utils/FloatComparison.hpp"
#include "superpositeur/StrongTypes.hpp"
#include "superpositeur/SparseVectorSort.hpp"
#include "oneapi/tbb/parallel_sort.h"

namespace superpositeur {

class MixedState {
public:
    explicit MixedState() : currentSortIndices(~BasisVector<128>()), dataVariant(SparseVector<64>{{BasisVector<64>{}, 1.}}), sizes({1}), hashes({0}) {}

    void reset() {
        dataVariant = SparseVector<64>{{BasisVector<64>{}, 1.}};
        sizes = {1};
        hashes = {0};
    }

    std::uint64_t currentSize() const {
        return std::visit([](auto const& data) { return std::remove_reference<decltype(data)>::type::value_type::MAX_NUMBER_OF_BITS; }, dataVariant);
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
            simplifyImpl(data);
        }, dataVariant);
    }

private:
    // FIXME: use std::dequeue, no needs for "lines" to be local var in simplify
    template <std::uint64_t MaxNumberOfQubits>
    void insertOrApply(std::vector<std::span<KeyValue<MaxNumberOfQubits>>>& lines, std::unordered_map<std::uint64_t, std::uint64_t>& hashToIndex, std::uint64_t index1) {
        if (lines[index1].empty()) {
            return;
        }

        auto& hash1 = hashes[index1];

        auto [it, inserted] = hashToIndex.insert(std::make_pair(hash1, index1));
        if (inserted) {
            return;
        }

        auto const index2 = it->second;
        auto& hash2 = hashes[index2];
        assert(hash2 == it->first);
        assert(hash1 == hash2);

        applyGivensRotation<MaxNumberOfQubits>(lines[index1], hash1, lines[index2], hash2);

        if (hash2 == it->first) [[likely]] {
            insertOrApply(lines, hashToIndex, index1);
            return;
        }

        hashToIndex.erase(it);
        insertOrApply(lines, hashToIndex, index1);
        insertOrApply(lines, hashToIndex, index2);
    }

public:

    template <std::uint64_t MaxNumberOfQubits>
    void simplifyImpl(SparseVector<MaxNumberOfQubits> &data) {
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

        std::unordered_map<std::uint64_t, std::uint64_t> hashToIndex;

        for (std::uint64_t i = 0; i < hashes.size(); ++i) {
            insertOrApply(lines, hashToIndex, {i});
        }

        SparseVector<MaxNumberOfQubits> newData;
        newData.reserve(data.size());
        sizes.clear();
        hashes.clear();
        for (auto const& [hash, index]: hashToIndex) {
            auto const& line = lines[index];
            sizes.push_back(line.size());
            hashes.push_back(hash);
            newData.insert(newData.end(), line.begin(), line.end());
        }

        data.swap(newData);
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

        std::visit([&](auto&& data) {
            applyCircuitInstruction(circuitInstruction, data);
        }, dataVariant);
        

        assert(isConsistent());
    }

    template <std::uint64_t MaxNumberOfQubits>
    void applyCircuitInstruction(CircuitInstruction const &circuitInstruction, SparseVector<MaxNumberOfQubits>& data) {
        if (!circuitInstruction.getControlQubitsMask().empty()) {
            throw std::runtime_error("Unimplemented: control qubits");
        }

        if (circuitInstruction.getKrausOperators().size() > 1) {
            throw std::runtime_error("Unimplemented: Kraus");
        }

        auto const matrix = circuitInstruction.getKrausOperators()[0];

        auto ops = circuitInstruction.getOperandsMask().cast<MaxNumberOfQubits>(); // FIXME
        auto nOps = circuitInstruction.getOperandsMask().popcount();

        if (nOps > 2) {
            throw std::runtime_error("Unimplemented: 3+ ops");
        }
        
        auto negOps = ~ops;

        sortSparseVector(data, currentSortIndices.cast<MaxNumberOfQubits>(), negOps);

        // auto startTimeRegularSort = std::chrono::steady_clock::now();
        // oneapi::tbb::parallel_sort(data.begin(), data.end(), [negOps](auto left, auto right) { return (left.ket & negOps) < (right.ket & negOps); });
        // auto endTimeRegularSort = std::chrono::steady_clock::now();
        // INSTRU << "regularSort," << data.size() << "," << std::chrono::duration_cast<std::chrono::nanoseconds>(endTimeRegularSort - startTimeRegularSort).count() << "," << negOps << std::endl;

        auto startTime = std::chrono::steady_clock::now();
        auto originalDataSize = data.size();

        std::vector<std::complex<double>> inputVector;
        inputVector.resize(matrix.getNumberOfRows());

        auto it = data.begin();
        auto max = data.end();
        while (it != max) {
            auto firstKey = it->ket;
            auto rest = firstKey & negOps;

            std::fill(inputVector.begin(), inputVector.end(), 0.);

            auto end = it;
            while (end != max && (end->ket & negOps) == rest) {
                inputVector[end->ket.pext(ops)] = end->amplitude;
                ++end;
            }

            for (std::uint64_t i = 0; i < matrix.getNumberOfRows(); ++i) {
                auto v = std::inner_product(inputVector.begin(), inputVector.end(), matrix.line(i).begin(), std::complex<double>(0.));
                auto key = firstKey.pdep(i, ops);

                if (utils::isNotNull(v)) {
                    if (it == end) {
                        if (data.size() < data.capacity()) [[likely]] {
                            data.emplace_back(key, v);
                        } else {
                            auto itDist = std::distance(data.begin(), it);
                            auto endDist = std::distance(data.begin(), end);
                            auto maxDist = std::distance(data.begin(), max);
                            data.emplace_back(key, v);
                            it = std::next(data.begin(), itDist);
                            end = std::next(data.begin(), endDist);
                            max = std::next(data.begin(), maxDist);
                        }
                    } else {
                        *it = { key, v };
                        ++it;
                    }
                }
            }

            it = data.erase(it, end);
        }

        auto endTime = std::chrono::steady_clock::now();

        INSTRU << "applyGate," << originalDataSize << "," << std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() << std::endl;

        startTime = std::chrono::steady_clock::now();

        std::inplace_merge(data.begin(), max, data.end(), [negOps](auto const left, auto const right) { return (left.ket & negOps) < (right.ket & negOps); });

        endTime = std::chrono::steady_clock::now();

        INSTRU << "finalInplaceMerge," << originalDataSize << "," << data.size() << "," << std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() << std::endl;

        assert(std::is_sorted(data.begin(), data.end(), [negOps](auto const left, auto const right) {
            return (left.ket & negOps) < (right.ket & negOps);
        }));

        sizes[0] = data.size(); // FIXME: to pass the assert
        currentSortIndices = ~circuitInstruction.getOperandsMask();
    }

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
            assert((internals.it1->ket & (~internals.reductionQubits)) == (internals.it2->ket & (~internals.reductionQubits)));

            Value result = Value(internals.it1->ket.pext(internals.reductionQubits), internals.it2->ket.pext(internals.reductionQubits), internals.it1->amplitude * std::conj(internals.it2->amplitude));

            auto current = internals.it1->ket & (~internals.reductionQubits);
            do {
                ++internals.it2;
            } while (internals.it2 != internals.end && ((internals.it2->ket & (~internals.reductionQubits)) != current));

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

    struct ReducedDensityMatrixDiagonalIterator {
        using Value = std::pair<std::uint64_t, double>; // i, M[i, i]

        template <std::uint64_t N>
        ReducedDensityMatrixDiagonalIterator(SparseVector<N> const& data, std::vector<bool> mask) :
            internalsVariant(Internals<N>{.end = data.end(), .it = data.begin(), .reductionQubits = getMask<N>(mask)}) {}

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
            if (internals.it == internals.end) {
                return std::nullopt;
            }
            
            Value result = Value(internals.it->ket.pext(internals.reductionQubits), std::norm(internals.it->amplitude));

            ++internals.it;

            return result;
        }

        template <std::uint64_t N>
        struct Internals {
            typename SparseVector<N>::const_iterator end;
            typename SparseVector<N>::const_iterator it;
            BasisVector<N> reductionQubits;
        };

        std::variant<Internals<64UL>, Internals<128UL>> internalsVariant;
    };

public:
    ReducedDensityMatrixIterator getReducedDensityMatrixIterator(std::vector<bool> qubits) const {
        assert(isConsistent());
        assert(std::ranges::find_if(qubits.rbegin(), qubits.rend(), std::identity{}) - qubits.rend() < 128); // FIXME

        return std::visit([&](auto&& x) {
            return ReducedDensityMatrixIterator(x, sizes, qubits);
        }, dataVariant);
    }

    ReducedDensityMatrixDiagonalIterator getReducedDensityMatrixDiagonalIterator(std::vector<bool> qubits) const {
        assert(isConsistent());
        assert(std::ranges::find_if(qubits.rbegin(), qubits.rend(), std::identity{}) - qubits.rend() < 128); // FIXME

        return std::visit([&](auto&& x) {
            return ReducedDensityMatrixDiagonalIterator(x, qubits);
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

    std::vector<double> getReducedDensityMatrixDiagonal(std::vector<bool> const& mask) {
        auto popcount = std::ranges::count_if(mask, [](auto x) { return x; });

        assert(popcount > 0 && popcount < 64);

        auto reducedDensityMatrixDiagonalIterator = getReducedDensityMatrixDiagonalIterator(mask);

        std::vector<double> result(1UL << popcount);
        while (auto densityMatrixDiagonalEntry = reducedDensityMatrixDiagonalIterator.next()) {
            auto i = std::get<0>(*densityMatrixDiagonalEntry);
            auto v = std::get<1>(*densityMatrixDiagonalEntry);
            result[i] += v;
        }

        assert(utils::isNull(std::reduce(result.begin(), result.end()) - 1.));

        return result;
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

    BasisVector<128> currentSortIndices;
    std::variant<SparseVector<64>, SparseVector<128>> dataVariant;
    Sizes sizes;
    Hashes hashes;
};

} // namespace  superpositeur