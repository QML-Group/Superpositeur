#pragma once

#include <cassert>
#include <complex>
#include <limits>
#include <ranges>
#include <algorithm>
#include <variant>
#include <thread>
#include "superpositeur/CircuitInstruction.hpp"
#include "superpositeur/Common.hpp"
#include "superpositeur/GivensRotation.hpp"
#include "superpositeur/MatrixSparseVectorMultiplication.hpp"
#include "superpositeur/utils/FloatComparison.hpp"
#include "superpositeur/StrongTypes.hpp"
#include "superpositeur/utils/blockingconcurrentqueue.hpp"

namespace superpositeur {

template <std::uint64_t N>
using Q = moodycamel::BlockingConcurrentQueue<KeyValue<N>>;

using Q64 = moodycamel::BlockingConcurrentQueue<KeyValue<64>;
using Q128 = moodycamel::BlockingConcurrentQueue<KeyValue<128>;

class MixedState {
public:
    explicit MixedState() : dataVariant(SparseVector<64>{{BasisVector<64>{}, 1.}}), sizes({1}), hashes({0}) {}

    void reset() {
        waitAllFutures();
        concurrentQueues.clear();
        concurrentQueues.push_back(Q<64>());
        std::get<Q<64>>(concurrentQueues.back()).enqueue({BasisVector<64>{}, 1.});
        std::get<Q<64>>(concurrentQueues.back()).enqueue({BasisVector<64>{}, { NAN, NAN }});
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
            simplifyImpl(data);
        }, dataVariant);
    }

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

        auto partial = [&] (std::uint64_t start, std::uint64_t end) {
            bool keepGoing = true;
            while (keepGoing) {
                keepGoing = false;

                for (std::uint64_t index1 = start; index1 < end; ++index1) {
                    if (lines[index1].empty()) [[unlikely]] {
                        continue;
                    }

                    for (std::uint64_t index2 = index1 + 1; index2 < end; ++index2) {
                        if (lines[index2].empty()) [[unlikely]] {
                            continue;
                        }

                        if (hashes[index1] == hashes[index2]) {
                            keepGoing = true;
                            applyGivensRotation<MaxNumberOfQubits>(lines[index1], hashes[index1], lines[index2], hashes[index2]);
                            break;
                        }
                    }
                }
            }
        };

        static constexpr std::uint64_t LINES_PER_THREAD = 1000;
        static constexpr std::uint64_t MAX_THREADS = 16;
        std::uint64_t const initial_num_threads = std::min(MAX_THREADS, lines.size() / LINES_PER_THREAD + 1);

        std::array<std::thread, MAX_THREADS> threads;

        auto doWithThreads = [&](std::uint64_t n) {
            assert(n <= MAX_THREADS);

            std::uint64_t start = 0;
            std::uint64_t const step = lines.size() / n;
            for (std::uint64_t i = 0; i < n; ++i) {
                std::uint64_t end = std::min(lines.size(), start + step);
                threads[i] = std::thread(partial, start, end);
                start = end;
            }

            for (std::uint64_t i = 0; i < n; ++i) {
                threads[i].join(); // FIXME: scheduling could be better, as well as partitioning?
            }
        };
        
        auto num_threads = initial_num_threads;
        while (num_threads >= 2) {
            doWithThreads(num_threads);
            num_threads >>= 1;
        }

        partial(0, lines.size());
    }

    void operator()(CircuitInstruction const &circuitInstruction) {
        // simplify(); // FIXME + recycle

        if (circuitInstruction.getKrausOperators().size() != 1) {
            throw std::runtime_error("Unimplemented");
        }

        assert(!concurrentQueues.empty());
        auto& inputQueueVariant = concurrentQueues.back();

        auto bitWidth = circuitInstruction.getOperandsMask().bitWidth();
        if (bitWidth > currentSize()) {
            if (bitWidth <= 64 && std::holds_alternative<Q64>(inputQueueVariant)) {
                concurrentQueues.emplace_back(Q64());
                auto outputQueue = std::get<Q64>(concurrentQueues.back());
                
                auto future = std::async(applyCircuitInstruction, instruction, std::get<Q64>(inputQueueVariant), outputQueue);
                futures.push_back(future);
            } else if (bitWidth <= 128) {
                concurrentQueues.emplace_back(Q128());
                auto outputQueue = std::get<Q128>(concurrentQueues.back());

                std::visit([&](auto& inputQueue) {
                    auto future = std::async(applyCircuitInstruction, instruction, inputQueue, outputQueue);
                    futures.push_back(future);
                }, inputQueueVariant);
            } else {
                throw std::runtime_error("Cannot handle that many qubits!");
            }
        }
    }

    template <std::uint64_t N, M>
    void applyCircuitInstruction(CircuitInstruction const &circuitInstruction, Q<N>& input, Q<M>& output) {
        if (!circuitInstruction.getControlQubitsMask().empty()) {
            throw std::runtime_error("Unimplemented: control qubits");
        }

        auto hashOfTheKeys = multiplyMatrix(circuitInstruction.getKrausOperators()[0], input, circuitInstruction.getOperandsMask(), output);
        (void) hashOfTheKeys;
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
            assert((internals.it1->first & (~internals.reductionQubits)) == (internals.it2->first & (~internals.reductionQubits)));

            Value result = Value(internals.it1->first.pext(internals.reductionQubits), internals.it2->first.pext(internals.reductionQubits), internals.it1->second * std::conj(internals.it2->second));

            auto current = internals.it1->first & (~internals.reductionQubits);
            do {
                ++internals.it2;
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
            
            Value result = Value(internals.it->first.pext(internals.reductionQubits), std::norm(internals.it->second));

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

    void waitAllFutures() {
        for (auto& f: futures) {
            f.wait();
        }
    }

public:
    ReducedDensityMatrixIterator getReducedDensityMatrixIterator(std::vector<bool> qubits) const {
        waitAllFutures()

        assert(isConsistent());
        assert(std::ranges::find_if(qubits.rbegin(), qubits.rend(), std::identity{}) - qubits.rend() < 128); // FIXME

        return std::visit([&](auto& data) {
            return ReducedDensityMatrixIterator(data, sizes, qubits);
        }, dataVariant);
    }

    ReducedDensityMatrixDiagonalIterator getReducedDensityMatrixDiagonalIterator(std::vector<bool> qubits) const {
        waitAllFutures()

        assert(isConsistent());
        assert(std::ranges::find_if(qubits.rbegin(), qubits.rend(), std::identity{}) - qubits.rend() < 128); // FIXME

        return std::visit([&](auto& data) {
            return ReducedDensityMatrixDiagonalIterator(data, qubits);
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

    Sizes sizes;
    Hashes hashes;
    std::vector<std::variant<Q<64>, Q<128>>> concurrentQueues;
    std::vector<std::future<void>> futures;
};

} // namespace  superpositeur