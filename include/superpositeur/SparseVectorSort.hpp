#include "superpositeur/Common.hpp"
#include "oneapi/tbb/parallel_invoke.h"
#include <algorithm>
#include <chrono>
#include <iostream>

namespace superpositeur {

// std::vector<It> breakPoints;
// breakPoints.push_back(data.begin());
// auto it = data.begin();
// while (it != data.end()) {
//     auto adjacent = std::adjacent_find(it, data.end(), [commonSortIndices](auto left, auto right) { return (left.ket & commonSortIndices) > (right.ket & commonSortIndices); });
//     if (adjacent == data.end()) {
//         break;
//     }
//     ++adjacent;
//     breakPoints.push_back(adjacent);
//     it = adjacent;
// }
// breakPoints.push_back(data.end());

// using ItIt = std::vector<It>::iterator;
// std::function<void(ItIt, ItIt)> process = [&process, commonSortIndices](ItIt begin, ItIt end) {
//     auto dist = std::distance(begin, end);

//     if (dist < 2) {
//         return;
//     }

//     auto mid = std::next(begin, dist / 2);

//     oneapi::tbb::parallel_invoke(
//         [&process, begin, mid]{ process(begin, mid); },
//         [&process, mid, end]{ process(mid, end); }
//     );

//     // process(begin, mid);
//     // process(mid, end);
    
//     std::inplace_merge(*begin, *mid, *end, [commonSortIndices](auto left, auto right) { return (left.ket & commonSortIndices) < (right.ket & commonSortIndices); });
// };

// process(breakPoints.begin(), std::next(breakPoints.end(), -1));


template <std::uint64_t MaxNumberOfQubits>
void removeSortIndicesImpl(typename SparseVector<MaxNumberOfQubits>::iterator begin, typename SparseVector<MaxNumberOfQubits>::iterator end, BasisVector<MaxNumberOfQubits> sortIndicesToRemove, BasisVector<MaxNumberOfQubits> targetSortIndices) {
    auto dist = std::distance(begin, end);
    
#ifndef NDEBUG
    constexpr std::int64_t MIN_PARALLEL_PROBLEM_SIZE = 2;
    constexpr std::int64_t SEQUENTIAL_LIMIT = 2;
#else
    constexpr std::int64_t MIN_PARALLEL_PROBLEM_SIZE = 25000;
    constexpr std::int64_t SEQUENTIAL_LIMIT = 200;
#endif
    if (dist < SEQUENTIAL_LIMIT) {
        std::sort(begin, end, [targetSortIndices](auto left, auto right) { return (left.ket & targetSortIndices) < (right.ket & targetSortIndices); });
        return;
    }

    auto mid = std::next(begin, dist / 2);
    auto current = (mid->ket & sortIndicesToRemove);
    auto other = std::find_if(mid, end, [current, sortIndicesToRemove](auto x) { return (x.ket & sortIndicesToRemove) > current; });
    if (other == end) {
        // Normally this would use a reverse iterator.
        other = mid;
        do {
            --other;
            if ((other->ket & sortIndicesToRemove) < current) {
                break;
            }
        } while (other != begin);

        if ((other->ket & sortIndicesToRemove) >= current) {
            return;
        }

        ++other;
    }

    if (std::distance(begin, other) > MIN_PARALLEL_PROBLEM_SIZE && std::distance(other, end) > MIN_PARALLEL_PROBLEM_SIZE) {
        oneapi::tbb::parallel_invoke(
            [begin, other, sortIndicesToRemove, targetSortIndices]{ removeSortIndicesImpl(begin, other, sortIndicesToRemove, targetSortIndices); },
            [other, end, sortIndicesToRemove, targetSortIndices]{ removeSortIndicesImpl(other, end, sortIndicesToRemove, targetSortIndices); }
        );
    } else {
        removeSortIndicesImpl(begin, other, sortIndicesToRemove, targetSortIndices);
        removeSortIndicesImpl(other, end, sortIndicesToRemove, targetSortIndices);
    }
    
    std::inplace_merge(begin, other, end, [targetSortIndices](auto left, auto right) { return (left.ket & targetSortIndices) < (right.ket & targetSortIndices); });
}


template <std::uint64_t MaxNumberOfQubits>
void removeSortIndices(SparseVector<MaxNumberOfQubits>& data, BasisVector<MaxNumberOfQubits> currentSortIndices, BasisVector<MaxNumberOfQubits> sortIndicesToRemove) {
    assert(std::is_sorted(data.begin(), data.end(), [currentSortIndices](auto const left, auto const right) {
        return (left.ket & currentSortIndices) < (right.ket & currentSortIndices);
    }));

    assert((sortIndicesToRemove & currentSortIndices) == sortIndicesToRemove);

    if (sortIndicesToRemove.empty()) {
        return;
    }

    auto startTime = std::chrono::steady_clock::now();

    auto targetSortIndices = currentSortIndices ^ sortIndicesToRemove;

    removeSortIndicesImpl(data.begin(), data.end(), sortIndicesToRemove, targetSortIndices);
    
    auto endTime = std::chrono::steady_clock::now();

    INSTRU << "removeSortIndices," << data.size() << "," << std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() << "," << currentSortIndices << "," << sortIndicesToRemove << std::endl;

    assert(std::is_sorted(data.begin(), data.end(), [targetSortIndices](auto const left, auto const right) {
        return (left.ket & targetSortIndices) < (right.ket & targetSortIndices);
    }));
}


template <std::uint64_t MaxNumberOfQubits>
void addSortIndexImpl(typename SparseVector<MaxNumberOfQubits>::iterator begin, typename SparseVector<MaxNumberOfQubits>::iterator end, std::uint64_t index, BasisVector<MaxNumberOfQubits> mask) {
    auto dist = std::distance(begin, end);

    if (dist <= 1) {
        return;
    }

// #ifndef NDEBUG
//     constexpr std::int64_t SEQUENTIAL_LIMIT = 2;
// #else
//     constexpr std::int64_t SEQUENTIAL_LIMIT = 5000;
// #endif

    // if (dist < SEQUENTIAL_LIMIT) {
    //     auto it = begin;
    //     while (it != end) {
    //         auto current = it->ket & mask;
    //         auto partitionEnd = std::find_if(it, end, [current, mask](auto x) { return (x.ket & mask) != current; });
    //         std::stable_partition(it, partitionEnd, [index](auto x) { return !x.ket.test(index); }); // Slight optimization possible: if indices are contiguous, this gives the following partitionEnd?
    //         it = partitionEnd;
    //     }
    //     return;
    // }

    auto mid = std::next(begin, dist / 2);
    auto current = mid->ket & mask;
    auto other = std::find_if(mid, end, [current, mask](auto x) { return (x.ket & mask) != current; });
    if (other == end) {
        // Normally this would use a reverse iterator.
        other = mid;
        do {
            --other;
            if ((other->ket & mask) != current) {
                break;
            }
        } while (other != begin);

        if ((other->ket & mask) == current) {
            std::stable_partition(begin, end, [index](auto x) { return !x.ket.test(index); });
            return;
        }

        ++other;
    }

#ifndef NDEBUG
    constexpr std::int64_t MIN_PARALLEL_PROBLEM_SIZE = 2;
#else
    constexpr std::int64_t MIN_PARALLEL_PROBLEM_SIZE = 25000;
#endif

    if (std::distance(begin, other) > MIN_PARALLEL_PROBLEM_SIZE && std::distance(other, end) > MIN_PARALLEL_PROBLEM_SIZE) {
        oneapi::tbb::parallel_invoke(
            [begin, other, index, mask]{ addSortIndexImpl(begin, other, index, mask); },
            [other, end, index, mask]{ addSortIndexImpl(other, end, index, mask); }
        );
    } else {
        addSortIndexImpl(begin, other, index, mask);
        addSortIndexImpl(other, end, index, mask);
    }
}

template <std::uint64_t MaxNumberOfQubits>
void addSortIndices(SparseVector<MaxNumberOfQubits>& data, BasisVector<MaxNumberOfQubits> currentSortIndices, BasisVector<MaxNumberOfQubits> sortIndicesToAdd) {
    assert(std::is_sorted(data.begin(), data.end(), [currentSortIndices](auto const left, auto const right) {
        return (left.ket & currentSortIndices) < (right.ket & currentSortIndices);
    }));

    assert((currentSortIndices & sortIndicesToAdd).empty());

    auto startTime = std::chrono::steady_clock::now();

    auto remainingSortIndicesToAdd = sortIndicesToAdd;
    while (!remainingSortIndicesToAdd.empty()) {
        auto index = MaxNumberOfQubits - 1 - remainingSortIndicesToAdd.countlZero();
        assert(remainingSortIndicesToAdd.test(index));
        remainingSortIndicesToAdd.set(index, false);

        auto mask = (index + 1 < MaxNumberOfQubits) ? ((~BasisVector<MaxNumberOfQubits>()) << (index + 1)) : BasisVector<MaxNumberOfQubits>();
        mask &= (currentSortIndices | sortIndicesToAdd);

        addSortIndexImpl(data.begin(), data.end(), index, mask);
    }

    auto endTime = std::chrono::steady_clock::now();

    INSTRU << "addSortIndices," << data.size() << "," << std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() << "," << currentSortIndices << "," << sortIndicesToAdd << std::endl;

    assert(std::is_sorted(data.begin(), data.end(), [currentSortIndices, sortIndicesToAdd](auto const left, auto const right) {
        return (left.ket & (currentSortIndices | sortIndicesToAdd)) < (right.ket & (currentSortIndices | sortIndicesToAdd));
    }));
}

template <std::uint64_t MaxNumberOfQubits>
void sortSparseVector(SparseVector<MaxNumberOfQubits>& data, BasisVector<MaxNumberOfQubits> currentSortIndices, BasisVector<MaxNumberOfQubits> desiredSortIndices) {
    assert(std::is_sorted(data.begin(), data.end(), [currentSortIndices](auto const left, auto const right) {
        return (left.ket & currentSortIndices) < (right.ket & currentSortIndices);
    }));

    // FIXME: if sortIndicesToRemove.countrZero() is too small, but non-zero, do regular sort instead?

    auto sortIndicesToRemove = currentSortIndices & (~desiredSortIndices);
    removeSortIndices(data, currentSortIndices, sortIndicesToRemove);

    auto commonSortIndices = currentSortIndices & desiredSortIndices;
    auto sortIndicesToAdd = (~currentSortIndices) & desiredSortIndices;
    addSortIndices(data, commonSortIndices, sortIndicesToAdd);

    assert(std::is_sorted(data.begin(), data.end(), [desiredSortIndices](auto const left, auto const right) {
        return (left.ket & desiredSortIndices) < (right.ket & desiredSortIndices);
    }));
}

}